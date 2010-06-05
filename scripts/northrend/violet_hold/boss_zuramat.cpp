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
SDName: boss_zuramat
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SAY_AGGRO                                 = -1608037,
    SAY_SLAY_1                                = -1608038,
    SAY_SLAY_2                                = -1608039,
    SAY_SLAY_3                                = -1608040,
    SAY_DEATH                                 = -1608041,
    SAY_SPAWN                                 = -1608042,
    SAY_SHIELD                                = -1608043,
    SAY_WHISPER                               = -1608044,

    SPELL_SHROUD_OF_DARKNESS                  = 54524,
    SPELL_SHROUD_OF_DARKNESS_H                = 59745,
    SPELL_SUMMON_VOID_SENTRY                  = 54369,
    SPELL_VOID_SHIFT                          = 54361,
    SPELL_VOID_SHIFT_H                        = 59743,

    NPC_VOID_SENTRY                           = 29364,
    SPELL_VOID_SENTRY_AURA                    = 54341,
    SPELL_VOID_SENTRY_AURA_H                  = 54351,
    SPELL_SHADOW_BOLT_VOLLEY                  = 54358, // 54342? 54358?
    SPELL_SHADOW_BOLT_VOLLEY_H                = 59747,

    ACHIEV_VOID_DANCE                           = 2153,
};

bool m_bIsSentryAlive;

struct MANGOS_DLL_DECL boss_zuramatAI : public ScriptedAI
{
    boss_zuramatAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    instance_violet_hold* m_pInstance;

    bool m_bIsRegularMode;
    bool MovementStarted;
    std::list<uint64> m_lSentryGUIDList;

    uint32 m_uiShroudDarkness_Timer;
    uint32 m_uiVoidShift_Timer;
    uint32 m_uiSummonVoidSentry_Timer;

    void Reset()
    {
        m_uiShroudDarkness_Timer = urand(8000, 9000);
        m_uiSummonVoidSentry_Timer = urand(5000, 10000);
        m_uiVoidShift_Timer = 10000;
        MovementStarted = false;

        m_bIsSentryAlive = true;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZURAMAT, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZURAMAT, IN_PROGRESS);
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

        if (m_pInstance->GetData(TYPE_ZURAMAT) != SPECIAL && m_pInstance->GetData(TYPE_ZURAMAT) != IN_PROGRESS)
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

    void JustSummoned(Creature* pSummoned)
    {
        m_lSentryGUIDList.push_back(pSummoned->GetGUID());
        pSummoned->AddThreat(m_creature->getVictim(), 100.0f);
        pSummoned->AI()->AttackStart(m_creature->getVictim());
    }

    void DespawnSentry()
    {
        if (m_lSentryGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lSentryGUIDList.begin(); itr != m_lSentryGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }

        m_lSentryGUIDList.clear();
    }

    void UpdateAI(const uint32 uiDiff) 
    {
        if (m_pInstance->GetData(TYPE_ZURAMAT) == SPECIAL && !MovementStarted) 
        {
            m_creature->GetMotionMaster()->MovePoint(0, PortalLoc[8].x, PortalLoc[8].y, PortalLoc[8].z);
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MovementStarted = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShroudDarkness_Timer < uiDiff)
        {
            DoScriptText(SAY_SHIELD, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHROUD_OF_DARKNESS_H : SPELL_SHROUD_OF_DARKNESS);
            m_uiShroudDarkness_Timer = urand(7000, 8000);
        }
        else m_uiShroudDarkness_Timer -= uiDiff;

        if (m_uiVoidShift_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_VOID_SHIFT_H : SPELL_VOID_SHIFT);
            m_uiVoidShift_Timer = urand(10000, 11000);
        }
        else m_uiVoidShift_Timer -= uiDiff;

        if (m_uiSummonVoidSentry_Timer < uiDiff)
        {
            m_creature->SummonCreature(NPC_VOID_SENTRY, m_creature->GetPositionX()-10+rand()%20, m_creature->GetPositionY()-10+rand()%20, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            m_uiSummonVoidSentry_Timer = urand(10000, 11000);
        }
        else m_uiSummonVoidSentry_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnSentry();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ZURAMAT, DONE);
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

        if(!m_bIsRegularMode && m_bIsSentryAlive)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_VOID_DANCE);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
        case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
        case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
        case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

struct MANGOS_DLL_DECL mob_zuramat_sentryAI : public ScriptedAI
{
    mob_zuramat_sentryAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }
    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
        //DoCast(m_creature, m_bIsRegularMode ? SPELL_VOID_SENTRY_AURA_H : SPELL_VOID_SENTRY_AURA); 
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT_VOLLEY_H : SPELL_SHADOW_BOLT_VOLLEY);

        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* pKiller)
    {
        m_bIsSentryAlive = false;
    }
};

CreatureAI* GetAI_boss_zuramat(Creature* pCreature)
{
    return new boss_zuramatAI (pCreature);
}

CreatureAI* GetAI_mob_zuramat_sentry(Creature* pCreature)
{
    return new mob_zuramat_sentryAI (pCreature);
}

void AddSC_boss_zuramat()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_zuramat";
    newscript->GetAI = &GetAI_boss_zuramat;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zuramat_sentry";
    newscript->GetAI = &GetAI_mob_zuramat_sentry;
    newscript->RegisterSelf();

}
