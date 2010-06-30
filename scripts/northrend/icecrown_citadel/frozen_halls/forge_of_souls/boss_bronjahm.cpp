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
SDName: boss_bronjahm
SD%Complete: 70%
SDComment: Soulstorm, Corrupt soul
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum Spells
{
    SPELL_CORRUPT_SOUL          = 68839,
    SPELL_MAGICSBANE            = 68793,
    SPELL_MAGICSBANE_H          = 69050,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_SHADOW_BOLT_H         = 70043,
    SPELL_FEAR                  = 68950,
    SPELL_SOULSTORM             = 68872,
    SPELL_TELEPORT              = 68988,
    SPELL_DRAW_SOUL             = 68846,
    SPELL_CONSUME_SOUL          = 68858,
    SPELL_CONSUME_SOUL_H        = 69047,

    NPC_CORRUPT_SOUL_FRAGMENT   = 36535,
    EQUIP_ID                    = 35514,

    ACHIEV_SOUL_POWER           = 4522,
};

enum Yells
{
    SAY_AGGRO                               = -1610017,
    SAY_SOULSTORM                           = -1610018,
    SAY_SLAY1                               = -1610019,
    SAY_SLAY2                               = -1610020,
    SAY_CORRUPT_SOUL                        = -1610021,
    SAY_DEATH                               = -1610022,
};

#define HOME_X                      5297.33f
#define HOME_Y                      2506.64f

struct MANGOS_DLL_DECL boss_BronjahnAI : public ScriptedAI
{
    boss_BronjahnAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCorruptSoulTimer;
    uint32 m_uiCorruptSoulSummon;
    uint64 m_uiCorruptTargetGUID;
    uint32 m_uiMagicsBaneTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiSoulStormTimer;
    uint32 m_uiSoulStormDmgTimer;
    uint8 m_uiAchievCounter;

    bool m_bIsSoulStorm;
    bool m_bHasTeleported;

    void Reset()
    {        
        SetCombatMovement(true);
        m_creature->GetMotionMaster()->Clear();
        m_uiCorruptSoulTimer    = urand(15000, 25000);
        m_uiCorruptSoulSummon   = 60000;
        m_uiCorruptTargetGUID   = 0;
        m_uiMagicsBaneTimer     = urand(8000, 15000);
        m_uiShadowBoltTimer     = 2000;
        m_uiFearTimer           = 1000;
        m_uiAchievCounter       = 0;

        m_bIsSoulStorm          = false;
        m_bHasTeleported        = false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit *pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)  
    {
        DoScriptText(SAY_DEATH, m_creature);

        std::list<Creature*> lSouls;
        GetCreatureListWithEntryInGrid(lSouls, m_creature, NPC_CORRUPT_SOUL_FRAGMENT, 100.0f);
        if (!lSouls.empty() && lSouls.size() >= 4)
        {
            for(std::list<Creature*>::iterator iter = lSouls.begin(); iter != lSouls.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    m_uiAchievCounter += 1;
            }
        }

        if(m_uiAchievCounter >= 4)
        {
            if(m_pInstance && !m_bIsRegularMode)
                m_pInstance->DoCompleteAchievement(ACHIEV_SOUL_POWER);
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, DONE);
    }

    void DoSoulstorm()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()) > 10.0f)
                    i->getSource()->DealDamage(i->getSource(), m_bIsRegularMode ? 1000 : 2500, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCorruptSoulTimer < uiDiff && !m_bIsSoulStorm)
        {
            DoScriptText(SAY_CORRUPT_SOUL, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_CORRUPT_SOUL);
                m_uiCorruptTargetGUID = pTarget->GetGUID();
            }
            m_uiCorruptSoulSummon = 4000;
            m_uiCorruptSoulTimer = urand(15000, 25000);
        }
        else
            m_uiCorruptSoulTimer -= uiDiff;

        if(m_uiCorruptSoulSummon < uiDiff && !m_bIsSoulStorm)
        {
            if(Unit* pTarget = Unit::GetUnit(*m_creature, m_uiCorruptTargetGUID))
                pTarget->CastSpell(pTarget, SPELL_DRAW_SOUL, false);
            m_uiCorruptSoulSummon = 60000;
        }
        else m_uiCorruptSoulSummon -= uiDiff;

        if (m_uiMagicsBaneTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGICSBANE : SPELL_MAGICSBANE_H);
            m_uiMagicsBaneTimer = urand(8000, 15000);
        }
        else
            m_uiMagicsBaneTimer -= uiDiff;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), 3))
            {
                if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
            }
            m_uiShadowBoltTimer = 1500;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        if (m_uiFearTimer < uiDiff && m_bIsSoulStorm)
        {
            m_creature->InterruptNonMeleeSpells(true);
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(m_creature, SPELL_FEAR);
            m_uiFearTimer = urand(8000, 12000);
        }
        else
            m_uiFearTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 30.0f && !m_bHasTeleported)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_TELEPORT);
            m_uiSoulStormTimer = 1000;
            m_bHasTeleported = true;
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();  
        }

        if (m_uiSoulStormTimer < uiDiff && !m_bIsSoulStorm && m_bHasTeleported)
        {
            m_creature->InterruptNonMeleeSpells(true);
            m_uiFearTimer = 7000;
            m_uiSoulStormDmgTimer = 5000;
            DoCast(m_creature, SPELL_SOULSTORM);
            DoScriptText(SAY_SOULSTORM, m_creature);            
            m_bIsSoulStorm = true;;
        }
        else
            m_uiSoulStormTimer -= uiDiff;

        // workaround because the soulstorm aura can be removed
        if (m_uiSoulStormDmgTimer < uiDiff && m_bIsSoulStorm)
        {
            DoSoulstorm();
            m_uiSoulStormDmgTimer = 1000;
        }
        else
            m_uiSoulStormDmgTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 30)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_Bronjahn(Creature* pCreature)
{
    return new boss_BronjahnAI (pCreature);
}

struct MANGOS_DLL_DECL mob_corrupted_soul_fragmentAI : public ScriptedAI
{
    mob_corrupted_soul_fragmentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetSpeedRate(MOVE_RUN, 0.2f);
        if(Creature* pBronjahm = GetClosestCreatureWithEntry(m_creature, NPC_BRONJAHM, 100.0f))
            m_creature->GetMotionMaster()->MoveFollow(pBronjahm, 0, 0);
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if(Creature* pBronjahm = GetClosestCreatureWithEntry(m_creature, NPC_BRONJAHM, 100.0f))
                {
                    if (m_creature->GetDistance2d(pBronjahm) <= 2)
                    {
                        pBronjahm->InterruptNonMeleeSpells(true);
                        pBronjahm->CastSpell(pBronjahm, m_bIsRegularMode ? SPELL_CONSUME_SOUL : SPELL_CONSUME_SOUL_H, false);
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
            m_uiRangeCheck_Timer = 500;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_corrupted_soul_fragment(Creature* pCreature)
{
    return new mob_corrupted_soul_fragmentAI (pCreature);
}

void AddSC_boss_Bronjahn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_Bronjahn";
    newscript->GetAI = &GetAI_boss_Bronjahn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_corrupted_soul_fragment";
    newscript->GetAI = &GetAI_mob_corrupted_soul_fragment;
    newscript->RegisterSelf();
}
