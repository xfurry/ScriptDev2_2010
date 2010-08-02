/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Brutallus
SD%Complete: 
SDComment: 
SDCategory: Sunwell Plateau
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum Brutallus
{
    YELL_INTRO                      = -1580017,
    YELL_INTRO_BREAK_ICE            = -1580018,
    YELL_INTRO_CHARGE               = -1580019,
    YELL_INTRO_KILL_MADRIGOSA       = -1580020,
    YELL_INTRO_TAUNT                = -1580021,

    YELL_MADR_ICE_BARRIER           = -1580031,
    YELL_MADR_INTRO                 = -1580032,
    YELL_MADR_ICE_BLOCK             = -1580033,
    YELL_MADR_TRAP                  = -1580034,
    YELL_MADR_DEATH                 = -1580035,

    YELL_AGGRO                      = -1580022,
    YELL_KILL1                      = -1580023,
    YELL_KILL2                      = -1580024,
    YELL_KILL3                      = -1580025,
    YELL_LOVE1                      = -1580026,
    YELL_LOVE2                      = -1580027,
    YELL_LOVE3                      = -1580028,
    YELL_BERSERK                    = -1580029,
    YELL_DEATH                      = -1580030,

    SPELL_METEOR_SLASH              = 45150,
    SPELL_BURN                      = 45141,
    SPELL_BURN_AURA_EFFECT          = 46394,
    SPELL_STOMP                     = 45185,
    SPELL_BERSERK                   = 26662,

    //Madrigosa
    SPELL_ICE_BARRIER               = 45203,
    SPELL_FROZEN_PRISON             = 47854,

    CREATURE_MADRIGOSA              = 25160,
};

struct MANGOS_DLL_DECL boss_brutallusAI : public ScriptedAI
{
    boss_brutallusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bHasTaunted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSlashTimer;
    uint32 m_uiBurnTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiLoveTimer;
    uint32 m_uiBurnCheckTimer;
    uint32 m_uiIntroCount;
    uint32 m_uiIntroTimer;
    bool m_bIsIntroNow;
    bool m_bHasTaunted;

    uint64 m_uiMadrigosaGuid;

    void Reset()
    {
        m_uiSlashTimer = 11000;
        m_uiStompTimer = 30000;
        m_uiBurnTimer = 60000;
        m_uiBerserkTimer = 360000;
        m_uiLoveTimer = urand(10000, 17000);

        m_uiIntroTimer = 5000;
        m_bIsIntroNow = false;
        m_uiIntroCount = 0;

        m_uiMadrigosaGuid = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRUTALLUS, NOT_STARTED);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(YELL_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRUTALLUS, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if(Creature* Madrigosa = m_creature->SummonCreature(CREATURE_MADRIGOSA, 1465.831f, 647.065f, m_creature->GetPositionZ(), 4.729f, TEMPSUMMON_TIMED_DESPAWN, 42000))
                m_uiMadrigosaGuid = Madrigosa->GetGUID();
            m_bHasTaunted = true;
            m_bIsIntroNow = true;
            m_uiIntroTimer = 5000;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(YELL_KILL1, m_creature); break;
            case 1: DoScriptText(YELL_KILL2, m_creature); break;
            case 2: DoScriptText(YELL_KILL3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(YELL_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRUTALLUS, DONE);
    }

    void SpellHitTarget(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_BURN)
            pCaster->CastSpell(pCaster, SPELL_BURN_AURA_EFFECT, true, NULL, NULL, m_creature->GetGUID());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiIntroTimer < uiDiff)
        {
            if(m_bIsIntroNow)
            {
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();

                switch(m_uiIntroCount)
                {
                    case 0:
                        if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                        {
                            DoScriptText(YELL_MADR_ICE_BARRIER, pMadrigosa);
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pMadrigosa->GetGUID());
                        }
                        m_uiIntroTimer = 6000; break;
                    case 1:
                        if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                            DoScriptText(YELL_MADR_INTRO, pMadrigosa);
                        m_uiIntroTimer = 5000; break;
                    case 2:
                        DoScriptText(YELL_INTRO, m_creature);
                        m_uiIntroTimer = 6000; break;
                    case 3:
                        if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                        {
                            DoScriptText(YELL_MADR_ICE_BLOCK, pMadrigosa);
                            //pMadrigosa->CastSpell(m_creature, SPELL_ICE_BARRIER, false);
                        }
                        m_uiIntroTimer = 4000; break;
                    case 4:
                        DoScriptText(YELL_INTRO_BREAK_ICE, m_creature);
                        //m_creature->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);
                        m_uiIntroTimer = 5000; break;
                    case 5:
                        if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                            DoScriptText(YELL_MADR_TRAP, pMadrigosa);
                        m_uiIntroTimer = 5000; break;
                    case 6:
                        DoScriptText(YELL_INTRO_CHARGE, m_creature);
                        //if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                            //DoCast(pMadrigosa, SPELL_FROZEN_PRISON);
                        m_uiIntroTimer = 5000; break;
                    case 7:
                        if(Creature* pMadrigosa = m_pInstance->instance->GetCreature(m_uiMadrigosaGuid))
                            DoScriptText(YELL_MADR_DEATH, pMadrigosa);
                        m_uiIntroTimer = 5000; break;
                    case 8:
                        DoScriptText(YELL_INTRO_KILL_MADRIGOSA, m_creature);
                        m_uiIntroTimer = 6000; break;
                    case 9:
                        DoScriptText(YELL_INTRO_TAUNT, m_creature);
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_uiBerserkTimer = 360000;
                        m_bIsIntroNow = false; break;
                }
                ++m_uiIntroCount;
            }
        }else m_uiIntroTimer -= uiDiff;

        if(m_bIsIntroNow)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLoveTimer < uiDiff)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(YELL_LOVE1, m_creature); break;
                case 1: DoScriptText(YELL_LOVE2, m_creature); break;
                case 2: DoScriptText(YELL_LOVE3, m_creature); break;
            }
            m_uiLoveTimer = urand(15000, 23000);
        }
        else
            m_uiLoveTimer -= uiDiff;

        if (m_uiBurnCheckTimer < uiDiff)
        {
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *BurnedPlayer = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if (BurnedPlayer && BurnedPlayer->GetTypeId() == TYPEID_PLAYER && BurnedPlayer->HasAura(SPELL_BURN_AURA_EFFECT))
                {
                    std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                    {
                        Unit *TargetedPlayer = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());  
                        if (TargetedPlayer && TargetedPlayer->GetTypeId() == TYPEID_PLAYER && TargetedPlayer->IsWithinDistInMap(BurnedPlayer, 6) && !TargetedPlayer->HasAura(SPELL_BURN_AURA_EFFECT))
                            TargetedPlayer->CastSpell(TargetedPlayer,SPELL_BURN_AURA_EFFECT,true);
                    }
                }
            }          
            m_uiBurnCheckTimer = 1000;        
        }else m_uiBurnCheckTimer -= uiDiff;

        if (m_uiSlashTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_METEOR_SLASH);
            m_uiSlashTimer = 11000;
        }
        else
            m_uiSlashTimer -= uiDiff;

        if (m_uiStompTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->getVictim())
            {
                DoCastSpellIfCan(pTarget,SPELL_STOMP);

                if (pTarget->HasAura(SPELL_BURN_AURA_EFFECT, EFFECT_INDEX_0))
                    pTarget->RemoveAurasDueToSpell(SPELL_BURN_AURA_EFFECT);
            }

            m_uiStompTimer = 30000;
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiBurnTimer < uiDiff)
        {
            //returns any unit
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                //so we get owner, in case unit was pet/totem/etc
                if (Player* pPlayer = pTarget->GetCharmerOrOwnerPlayerOrPlayerItself())
                    DoCastSpellIfCan(pPlayer, SPELL_BURN);
            }

            m_uiBurnTimer = 60000;
        }
        else
            m_uiBurnTimer -= uiDiff;

        if (m_uiBerserkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature,SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(YELL_BERSERK, m_creature);
                m_uiBerserkTimer = 20000;
            }
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_brutallus(Creature* pCreature)
{
    return new boss_brutallusAI(pCreature);
}

bool AreaTrigger_at_madrigosa(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        //this simply set encounter state, and trigger ice barrier become active
        //bosses can start pre-event based on this new state
        if (pInstance->GetData(TYPE_BRUTALLUS) == NOT_STARTED)
            pInstance->SetData(TYPE_BRUTALLUS, SPECIAL);
    }

    return false;
}

void AddSC_boss_brutallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_brutallus";
    newscript->GetAI = &GetAI_boss_brutallus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_madrigosa";
    newscript->pAreaTrigger = &AreaTrigger_at_madrigosa;
    newscript->RegisterSelf();
}
