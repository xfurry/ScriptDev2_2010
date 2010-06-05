/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_moragg
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SPELL_CORROSICE_SALIVA                = 54527,
    SPELL_OPTIC_LINK                      = 54396,
    SPELL_RAY_PAIN                        = 59525,
    SPELL_RAY_SUFFERING                   = 54417,
};

struct MANGOS_DLL_DECL boss_moraggAI : public ScriptedAI
{
    boss_moraggAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_violet_hold* m_pInstance;

    bool m_bIsRegularMode;
    uint32 m_uiCorrosiveSaliva_Timer;
    uint32 m_uiOpticLink_Timer;
    uint32 m_uiRay_Timer;

    bool MovementStarted;

    void Reset()
    {
        MovementStarted = false;
        m_uiCorrosiveSaliva_Timer = urand(10000, 11000);
        m_uiOpticLink_Timer = urand(25000, 30000);
        m_uiRay_Timer = urand(2000, 7000);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MORAGG, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MORAGG, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if(Creature* pSinclari = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_SINCLARI)))
            pSinclari->DealDamage(pSinclari, pSinclari->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAIN, FAIL);
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_MORAGG) != SPECIAL && m_pInstance->GetData(TYPE_MORAGG) != IN_PROGRESS)
            return;

        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_MORAGG) == SPECIAL && !MovementStarted) 
        {
            m_creature->GetMotionMaster()->MovePoint(0, PortalLoc[8].x, PortalLoc[8].y, PortalLoc[8].z);
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MovementStarted = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCorrosiveSaliva_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CORROSICE_SALIVA);
            m_uiCorrosiveSaliva_Timer = urand(10000, 11000);
        }
        else m_uiCorrosiveSaliva_Timer -= uiDiff;

        if (m_uiOpticLink_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_OPTIC_LINK);
            m_uiOpticLink_Timer = urand(25000, 30000);
        }
        else m_uiOpticLink_Timer -= uiDiff;

        if (m_uiRay_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, urand(0, 1) ? SPELL_RAY_PAIN : SPELL_RAY_SUFFERING);
            m_uiRay_Timer = urand(2000, 7000);
        }
        else m_uiRay_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MORAGG, DONE);
            m_pInstance->SetData(TYPE_PORTAL, DONE);

            // check if boss was already killed before
            uint32 m_uiMyPortalNumber = m_pInstance->GetCurrentPortalNumber();
            if(m_uiMyPortalNumber == 6)
            {
                if(m_pInstance->GetData(TYPE_FIRST_BOSS) == DONE)
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                else
                    m_pInstance->SetData(TYPE_FIRST_BOSS, DONE);
            }
            else if(m_uiMyPortalNumber == 12)
            {
                if(m_pInstance->GetData(TYPE_SECOND_BOSS) == DONE)
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                else
                    m_pInstance->SetData(TYPE_SECOND_BOSS, DONE);
            }
        }
    }
};

CreatureAI* GetAI_boss_moragg(Creature* pCreature)
{
    return new boss_moraggAI (pCreature);
}

void AddSC_boss_moragg()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_moragg";
    newscript->GetAI = &GetAI_boss_moragg;
    newscript->RegisterSelf();
}
