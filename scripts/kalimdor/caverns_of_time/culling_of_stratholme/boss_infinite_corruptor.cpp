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
SDName: Boss_Infinite Corruptor
SD%Complete: 
SDComment: 
SDCategory: Caverns of Time, The Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"

enum
{
    SAY_AGGRO       = -1594189,
    SAY_DEPARTURE   = -1594190,
    SAY_DEATH       = -1594191,

    SPELL_COURSE      = 60588,
    SPELL_STRIKE      = 60590
};

struct MANGOS_DLL_DECL boss_infinite_corruptorAI : public ScriptedAI
{
    boss_infinite_corruptorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStrikeTimer;
    uint32 m_uiCourseTimer;

    void Reset() 
    {
        m_uiCourseTimer = 7000;
        m_uiStrikeTimer = 5000;
        m_creature->SetRespawnDelay(DAY);
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_BONUS, SPECIAL);
            m_pInstance->DoUpdateWorldState(WORLD_STATE_COS_TIME_ON, 0);
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_BONUS) == DONE)
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            else
            {
                m_pInstance->SetData(TYPE_BONUS, DONE);
                m_pInstance->DoCompleteAchievement(ACHIEV_CULLING_TIME);
            }
        }
    }

    void EnterEvadeMode()
    {
        if(!m_pInstance) return;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreaturesAddon();
        if(m_pInstance)
            m_pInstance->SetData(TYPE_BONUS, IN_PROGRESS);

        if(m_creature->isAlive())
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();

        if (m_uiCourseTimer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_COURSE);

            m_uiCourseTimer = 17000;
        }else m_uiCourseTimer -= diff;

        if (m_uiStrikeTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_STRIKE);

            m_uiStrikeTimer = 5000;
        }else m_uiStrikeTimer -= diff;
    }
};

CreatureAI* GetAI_boss_infinite_corruptor(Creature* pCreature)
{
    return new boss_infinite_corruptorAI(pCreature);
}

void AddSC_boss_infinite_corruptor()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_infinite_corruptor";
    newscript->GetAI = &GetAI_boss_infinite_corruptor;
    newscript->RegisterSelf();
}