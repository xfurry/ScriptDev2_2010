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
SDName: Boss_Buru
SD%Complete: 0
SDComment: Place Holder
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

enum
{
    EMOTE_TARGET                =   -1509002,

    SPELL_CREEPING_PLAGUE       =   20512,
    SPELL_DISMEMBER             =   96,
    SPELL_FULL_SPEED            =   1557,
    SPELL_THORNS                =   25640,
    SPELL_GATHERING_SPEED       =   1834,
    SPELL_BURU_TRANSFORM        =   24721,

    NPC_BURU_EGG                =   15514,
    SPELL_SUMMON_HATCHLING      =   1881,
    SPELL_EXPLODE               =   19593,
    NPC_BURU_EGG_TRIGGER        =   15964,

    NPC_HIVEZARA_HATCHLING      =   15521
};

struct MANGOS_DLL_DECL boss_buruAI : public ScriptedAI
{
    boss_buruAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bIsEnraged;

    uint32 m_uiDismember_Timer;
    uint32 m_uiSpeed_Timer;
    uint32 m_uiCreepingPlague_Timer;

    void Reset() 
    {
        m_creature->RemoveAllAuras();        
        m_creature->SetSpeedRate(MOVE_RUN,0.5f);
        m_bIsEnraged = false;
        m_uiDismember_Timer = 5000;
        m_uiSpeed_Timer = 5000;
        m_uiCreepingPlague_Timer = 6000;
    }

    void SpellHit(Unit *pCaster, const SpellEntry *pSpell)        //not working
    {
        if (pSpell->Id == SPELL_EXPLODE)
        {
            m_creature->SetSpeedRate(MOVE_RUN,0.5f);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_bIsEnraged && (m_creature->GetHealth()+uiDamage)*100 / m_creature->GetMaxHealth() < 20)
        {            
            DoCast(m_creature,SPELL_BURU_TRANSFORM);
            m_creature->SetArmor(0);
            m_bIsEnraged = true;
        }
    }

    void Aggro(Unit *pWho)
    {
        DoCast(m_creature,SPELL_THORNS);
        m_creature->SetArmor(20000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDismember_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_DISMEMBER);
            m_uiDismember_Timer = 20000;
        }
        else
            m_uiDismember_Timer -= uiDiff;

        if (m_bIsEnraged)
            if (m_uiCreepingPlague_Timer < uiDiff)
            {
                DoCast(m_creature,SPELL_CREEPING_PLAGUE);
                m_uiCreepingPlague_Timer = 6000;
            }
            else
                m_uiCreepingPlague_Timer -= uiDiff;

        if (m_uiSpeed_Timer < uiDiff)
        {
            m_creature->SetSpeedRate(MOVE_RUN,m_creature->GetSpeedRate(MOVE_RUN) + 0.1f);
            m_uiSpeed_Timer = 5000;
        }
        else
            m_uiSpeed_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_buru_eggAI : public ScriptedAI
{
    mob_buru_eggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }
    ScriptedInstance* m_pInstance;
    Creature *pBuru;

    void Reset()
    {
        SetCombatMovement(false);
    }

    void EnterCombat(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        if (Creature* pBuru = (Creature*)Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_BURU)))
            if (pBuru->isAlive())
            {
                pBuru->GetMotionMaster()->MoveChase(pWho);
                pBuru->AI()->AttackStart(pWho);
            }
    }
    void JustSummoned(Creature *pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_BURU_EGG_TRIGGER)
        {
            pSummoned->CastSpell(pBuru,SPELL_EXPLODE,true);            
            pSummoned->CastSpell(pBuru,SPELL_SUMMON_HATCHLING,true);
        }
    }

    void JustDied(Unit *pKiller)
    {
        if (!m_pInstance)
            return;

        if (Unit* pBuru = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_BURU)))
            if (pBuru->isAlive())
            {
                pBuru->GetMotionMaster()->MoveChase(pKiller);
                pBuru->Attack(pKiller,true);
                DoScriptText(EMOTE_TARGET,pBuru,pKiller);
                pBuru->CastSpell(pBuru,SPELL_EXPLODE,true);            
                pBuru->CastSpell(pBuru,SPELL_SUMMON_HATCHLING,true);
                //pBuru->RemoveAurasDueToSpell(SPELL_FULL_SPEED);
                //m_creature->SummonCreature(NPC_BURU_EGG_TRIGGER,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,1000);                
                pBuru->SetHealth(pBuru->GetHealth() - 45000);
                pBuru->SetSpeedRate(MOVE_RUN,0.5f);
            }
    }
};

CreatureAI* GetAI_boss_buru(Creature* pCreature)
{
    return new boss_buruAI(pCreature);
}

CreatureAI* GetAI_mob_buru_egg(Creature* pCreature)
{
    return new mob_buru_eggAI(pCreature);
}

void AddSC_boss_buru()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_buru";
    newscript->GetAI = &GetAI_boss_buru;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_buru_egg";
    newscript->GetAI = &GetAI_mob_buru_egg;
    newscript->RegisterSelf();
}
