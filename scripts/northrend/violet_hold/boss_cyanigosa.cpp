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
SDName: boss_cyanigosa
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SAY_AGGRO                                 = -1608050,
    SAY_SLAY_1                                = -1608051,
    SAY_SLAY_2                                = -1608052,
    SAY_SLAY_3                                = -1608053,
    SAY_DEATH                                 = -1608054,
    SAY_SPAWN                                 = -1608055,
    SAY_DISRUPTION                            = -1608056,
    SAY_BREATH_ATTACK                         = -1608057,
    SAY_SPECIAL_ATTACK_1                      = -1608058,
    SAY_SPECIAL_ATTACK_2                      = -1608059,

    SPELL_ARCANE_VACUM                        = 58694,
    SPELL_BLIZZARD                            = 58693,
    SPELL_BLIZZARD_H                          = 59369,
    SPELL_MANA_DESTRUCTION                    = 59374,
    SPELL_TAIL_SWEEP                          = 58690,
    SPELL_TAIL_SWEEP_H                        = 59283,
    SPELL_UNCONTROLLABLE_ENERGY               = 58688,
    SPELL_UNCONTROLLABLE_ENERGY_H             = 59281,
    SPELL_CYANIGOSA_TRANSFORM                 = 58668,
};

struct MANGOS_DLL_DECL boss_cyanigosaAI : public ScriptedAI
{
    boss_cyanigosaAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        MovementStarted = false;
        Reset();
    }
    instance_violet_hold* m_pInstance;

    bool m_bIsRegularMode;

    uint32 m_uiTailSweep_Timer;
    uint32 m_uiManaDestruction_Timer;
    uint32 m_uiBlizzard_Timer;
    uint32 m_uiUncontrollableEnergy_Timer;
    uint32 m_uiArcaneVacuum_Timer;
    uint32 m_uiSpeechTimer;
    uint32 m_uiMovementTimer;
    bool MovementStarted;

    void Reset()
    {
        m_uiUncontrollableEnergy_Timer = urand(15000, 16000);
        m_uiManaDestruction_Timer = urand(5000, 6000);
        m_uiBlizzard_Timer = urand(20000, 25000);
        m_uiTailSweep_Timer = urand(10000, 11000);
        m_uiArcaneVacuum_Timer = urand(28000, 33000);
        m_uiSpeechTimer = 40000;
        m_uiMovementTimer = 8000;

        m_creature->SetRespawnDelay(DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if(Creature* pSinclari = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_SINCLARI)))
            pSinclari->DealDamage(pSinclari, pSinclari->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MAIN, FAIL);
            m_pInstance->SetData(TYPE_CYANIGOSA, FAIL);
        }

        m_creature->ForcedDespawn();
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (!MovementStarted) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCast(m_creature, SPELL_CYANIGOSA_TRANSFORM);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_CYANIGOSA, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiMovementTimer < uiDiff && !MovementStarted)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MovementStarted = true;
        }
        else
            m_uiMovementTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiUncontrollableEnergy_Timer < uiDiff)
        {
            DoScriptText(SAY_BREATH_ATTACK, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_UNCONTROLLABLE_ENERGY_H : SPELL_UNCONTROLLABLE_ENERGY);
            m_uiUncontrollableEnergy_Timer = urand(15000, 16000);
        }
        else
            m_uiUncontrollableEnergy_Timer -= uiDiff;

        if (m_uiManaDestruction_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_MANA_DESTRUCTION);
            m_uiManaDestruction_Timer = urand(8000, 13000);
        }
        else
            m_uiManaDestruction_Timer -= uiDiff;

        if (m_uiBlizzard_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_BLIZZARD_H : SPELL_BLIZZARD);
            m_uiBlizzard_Timer = urand(20000, 25000);
        }
        else
            m_uiBlizzard_Timer -= uiDiff;

        if (m_uiArcaneVacuum_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_ARCANE_VACUM);
            DoResetThreat();
            m_uiArcaneVacuum_Timer = urand(28000, 33000);
        }
        else
            m_uiArcaneVacuum_Timer -= uiDiff;

        if (m_uiTailSweep_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_SWEEP_H : SPELL_TAIL_SWEEP);
            m_uiTailSweep_Timer = urand(10000, 11000);
        }
        else
            m_uiTailSweep_Timer -= uiDiff;

        if (m_uiSpeechTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0: DoScriptText(SAY_SPECIAL_ATTACK_1, m_creature);break;
            case 1: DoScriptText(SAY_SPECIAL_ATTACK_2, m_creature);break;
            }
            m_uiSpeechTimer = urand(30000, 60000);
        }
        else
            m_uiSpeechTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_MAIN, DONE);
            m_pInstance->SetData(TYPE_SEAL, DONE);
            m_pInstance->SetData(TYPE_CYANIGOSA, DONE);
            m_pInstance->UpdateWorldState(false);
        }

        if(GameObject* pDoor = GetClosestGameObjectWithEntry(m_creature, GO_PRISON_SEAL_DOOR, 150.0f))
            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
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

CreatureAI* GetAI_boss_cyanigosa(Creature* pCreature)
{
    return new boss_cyanigosaAI (pCreature);
}

void AddSC_boss_cyanigosa()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_cyanigosa";
    newscript->GetAI = &GetAI_boss_cyanigosa;
    newscript->RegisterSelf();
}
