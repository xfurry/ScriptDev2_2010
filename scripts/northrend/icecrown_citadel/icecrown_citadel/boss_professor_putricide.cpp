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
SDName: boss_professor_putricide
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // yells
    SAY_AGGRO               = -1609481,
    SAY_AIRLOCK             = -1609482,
    SAY_PHASE               = -1609483,
    SAY_TRANSFORM1          = -1609484,
    SAY_TRANSFORM2          = -1609485,
    SAY_KILL1               = -1609486,
    SAY_KILL2               = -1609487,
    SAY_BERSERK             = -1609488,
    SAY_DEATH               = -1609489,

    // spells
    // phase 1 -> 80%
    SPELL_SLIME_PUDDLE_TRIG         = 70343,
	NPC_SLIME_PUDDLE				= 37690,
	SPELL_SLIME_PUDDLE_SUMMON		= 70342,
	SPELL_SLIME_PUDDLE_MISSLE		= 70341,
    SPELL_UNSTABLE_EXPERIMENT       = 71968,
    SPELL_TEAR_GAS                  = 71617,    // only on non hc
    SPELL_TEAR_GAS_STUN             = 71618,
    SPELL_CREATE_CONCOCTION         = 71621,	// 71704

    // phase 2 -> 35%
    SPELL_CHOKING_GAS_TRIG          = 71259,
    SPELL_CHOKING_GAS_10            = 71278,
    SPELL_CHOKING_GAS_25            = 72460,
    SPELL_CHOKING_GAS_10HC          = 72619,
    SPELL_CHOKING_GAS_25HC          = 72620,
    SPELL_CHOKING_GAS_EXPL_TRIG     = 71280,
    SPELL_CHOKING_GAS_EXPL_10       = 71279,
    SPELL_CHOKING_GAS_EXPL_25       = 72459,
    SPELL_CHOKING_GAS_EXPL_10HC     = 72621,
    SPELL_CHOKING_GAS_EXPL_25HC     = 72622,
    SPELL_MALLEABLE_GOO_10          = 72297,
    SPELL_MALLEABLE_GOO_25          = 72548,
    SPELL_MALLEABLE_GOO_10HC        = 72873,
    SPELL_MALLEABLE_GOO_25HC        = 72874,
    SPELL_MALLEABLE_GOO_SUMMON      = 72298,
	SPELL_MALLEABLE_GOO				= 72296,

    // phase 3
    SPELL_MUTATED_STRENGTH          = 71603,
    SPELL_GUZZLE_POTIONS            = 71893,
    SPELL_MUTATED_PLAGUE            = 72672,
    SPELL_BERSERK                   = 47008,

    // mutated transformation
    SPELL_MUTATED_TRANSFORMATION    = 70311,
    SPELL_EAT_OOZE                  = 72527,
    SPELL_REGURGITATED_OOZE_10      = 70539,
    SPELL_REGURGITATED_OOZE_25      = 72457,
    SPELL_REGURGITATED_OOZE_10HC    = 72875,
    SPELL_REGURGITATED_OOZE_25HC    = 72876,
    SPELL_MUTATED_SLASH             = 70542,
    SPELL_MUTATED_AURA              = 70405,

    // cloud
    SPELL_EXPUNGED_GAS              = 70701,
    SPELL_GASEOUS_BLOAT_10          = 70672,
    SPELL_GASEOUS_BLOAT_10HC        = 72455,
    SPELL_GASEOUS_BLOAT_25          = 72832,
    SPELL_GASEOUS_BLOAT_25HC        = 72833,

    // ooze
    SPELL_OOZE_ADHESIVE_10          = 70447,
    SPELL_OOZE_ADHESIVE_25          = 72836,
    SPELL_OOZE_ADHESIVE_10HC        = 72837,
    SPELL_OOZE_ADHESIVE_25HC        = 72838,
    SPELL_OOZE_ERUPTION             = 70492,

    NPC_MUTATED_ABOMINATION         = 37672,
    NPC_GAS_CLOUD                   = 37562,
    NPC_VOLATILE_OOZE               = 37697,
    NPC_GAS_BOMB                    = 38159,
    NPC_MALLEABLE_OOZE              = 38556,
};

enum phases
{
	PHASE_IDLE			= 0,
	PHASE_1				= 1,
	PHASE_TRANSFORM_1	= 2,
	PHASE_2				= 3,
	PHASE_TRANSFORM_2	= 4,
	PHASE_3				= 5,
};

const float PosGreenPipe[3] = {4380.875f, 3206.018f, 389.399f};	// volatile ooze
const float PosRedPipe[3] = {4329.408f, 3213.475f, 389.399f};	// gas cloud

struct MANGOS_DLL_DECL boss_professor_putricideAI : public ScriptedAI
{
    boss_professor_putricideAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        homeX = pCreature->GetPositionX();
        homeY = pCreature->GetPositionY();
        homeZ = pCreature->GetPositionZ();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    // phase1
    uint32 m_uiUnstableExperimentTimer;
    uint32 m_uiSlimePuddleTimer;

    // phase2
    uint32 m_uiChockinGasTimer;
    uint32 m_uiMalleableGooTimer;

    // phase3
    uint32 m_uiMutatedStrengthTimer;
    uint32 m_uiMutatedPlagueTimer;

    // other
    uint8 m_uiPhase;
    uint32 m_uiDrinkPotionTimer;
    uint32 m_uiSwitchTimer;
	uint32 m_uiMovementTimer;
    bool m_bHasGasCasted;
	bool m_bIsOoze;
	uint8 m_uiMaxTargets;
    uint32 m_uiBerserkTimer;

    float homeX, homeY, homeZ;

    void Reset()
    {
        // phase1
        m_uiUnstableExperimentTimer = urand(50000, 60000);
        m_uiSlimePuddleTimer        = urand(25000, 30000);
        // phase2
        m_uiChockinGasTimer         = 15000;
        m_uiMalleableGooTimer       = 25000;
        // phase3
        m_uiMutatedStrengthTimer    = 10000;
        m_uiMutatedPlagueTimer      = 15000;

		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
			m_uiMaxTargets = 1;
		else 
			m_uiMaxTargets = 2;

        // other
		m_uiMovementTimer		= 30000;
        m_uiDrinkPotionTimer    = 30000;
        m_uiSwitchTimer         = 20000;
        m_uiPhase               = PHASE_IDLE;
        m_bHasGasCasted         = false;
		m_bIsOoze				= true;
        m_uiBerserkTimer        = 600000;  // 10 min
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_PUTRICIDE, NOT_STARTED);
            m_pInstance->SetData(TYPE_ATTEMPTS, m_pInstance->GetData(TYPE_ATTEMPTS) - 1);
            m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_uiPhase = PHASE_1;

        if(m_pInstance) 
		{
            m_pInstance->SetData(TYPE_PUTRICIDE, IN_PROGRESS);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_SHOW, 1);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
			if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_TOTAL, 9999);
			else
				m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_TOTAL, 25);
		}
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_PUTRICIDE, DONE);
    }

	void JustSummoned(Creature* pSummon)
	{
		if(pSummon->GetEntry() == NPC_GAS_CLOUD || pSummon->GetEntry() == NPC_VOLATILE_OOZE)
			pSummon->SetInCombatWithZone();
	}

	void DoExperiment()
	{
		if(m_bIsOoze)
		{
			DoScriptText(SAY_PHASE, m_creature);
			m_creature->SummonCreature(NPC_VOLATILE_OOZE, PosGreenPipe[0], PosGreenPipe[1], PosGreenPipe[2], 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
			m_bIsOoze = false;
		}
		else
		{
			DoScriptText(SAY_AIRLOCK, m_creature);
			m_creature->SummonCreature(NPC_GAS_CLOUD, PosRedPipe[0], PosRedPipe[1], PosRedPipe[2], 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20000);
			m_bIsOoze = true;
		}
	}

	void DoSlimePuddle()
	{
		for(uint8 i = 0; i < m_uiMaxTargets; ++i)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_SLIME_PUDDLE_MISSLE);
				m_creature->SummonCreature(NPC_SLIME_PUDDLE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
			}
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        switch (m_uiPhase)
        {
            // phase 1
        case PHASE_1:

            if (m_uiSlimePuddleTimer < uiDiff)
            {
				DoSlimePuddle();
                m_uiSlimePuddleTimer = 35000;
            }
            else m_uiSlimePuddleTimer -= uiDiff;

            if (m_uiUnstableExperimentTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_UNSTABLE_EXPERIMENT);
				DoExperiment();
                m_uiUnstableExperimentTimer = 37000;
            }
			else m_uiUnstableExperimentTimer -= uiDiff;

			if (m_creature->GetHealthPercent() < 80.0f)
			{
				m_creature->InterruptNonMeleeSpells(true);
				SetCombatMovement(false);
				if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				{
					DoCast(m_creature, SPELL_TEAR_GAS);
					m_uiMovementTimer	= 2500;
				}
				else 
					m_uiMovementTimer = 100;
				m_uiSwitchTimer		 = 17500;
				m_uiDrinkPotionTimer = 10000;
				m_uiPhase = PHASE_TRANSFORM_1;
			}

            break;
            // break -> tear gas
        case PHASE_TRANSFORM_1:

			if(m_uiMovementTimer < uiDiff)
			{
				m_creature->GetMotionMaster()->MovePoint(0, homeX, homeY, homeZ);
				m_uiMovementTimer = 30000;
			}
			else m_uiMovementTimer -= uiDiff;

            if (m_uiDrinkPotionTimer < uiDiff)
            {
				DoScriptText(SAY_TRANSFORM1, m_creature);
                DoCast(m_creature, SPELL_CREATE_CONCOCTION);
                m_uiDrinkPotionTimer = 60000;
            }
            else m_uiDrinkPotionTimer -= uiDiff;

            if(m_uiSwitchTimer < uiDiff)
            {
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase = PHASE_2;
            }
            else m_uiSwitchTimer -= uiDiff;

            break;
            // phase 2
        case PHASE_2:

            if (m_uiSlimePuddleTimer < uiDiff)
            {
				DoSlimePuddle();
                m_uiSlimePuddleTimer = 35000;
            }
            else m_uiSlimePuddleTimer -= uiDiff;

            if (m_uiUnstableExperimentTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_UNSTABLE_EXPERIMENT);
				DoExperiment();
                m_uiUnstableExperimentTimer = 37000;
            }
            else m_uiUnstableExperimentTimer -= uiDiff;

			if (m_uiChockinGasTimer < uiDiff)
			{
				for(uint8 i = 0; i < m_uiMaxTargets; ++i)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
						m_creature->SummonCreature(NPC_GAS_BOMB, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 25000);
				}
				m_uiChockinGasTimer = urand(15000, 20000);
			}
            else m_uiChockinGasTimer -= uiDiff;

            if (m_uiMalleableGooTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_MALLEABLE_GOO);
                m_uiMalleableGooTimer = urand(10000, 15000);
            }
			else m_uiMalleableGooTimer -= uiDiff;

			if (m_creature->GetHealthPercent() < 35.0f) 
			{
				m_creature->InterruptNonMeleeSpells(true);
				SetCombatMovement(false);
				if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				{
					DoCast(m_creature, SPELL_TEAR_GAS);
					m_uiMovementTimer	= 2500;
				}
				else 
					m_uiMovementTimer = 100;
				m_uiSwitchTimer		 = 17500;
				m_uiDrinkPotionTimer = 10000;
				m_uiPhase = PHASE_TRANSFORM_2;
			}

            break;
            // break -> tear gas
		case PHASE_TRANSFORM_2:

			if(m_uiMovementTimer < uiDiff)
			{
				m_creature->GetMotionMaster()->MovePoint(0, homeX, homeY, homeZ);
				m_uiMovementTimer = 30000;
			}
			else m_uiMovementTimer -= uiDiff;

            if (m_uiDrinkPotionTimer < uiDiff)
            {
				DoScriptText(SAY_TRANSFORM2, m_creature);
                DoCast(m_creature, SPELL_GUZZLE_POTIONS);
                m_uiDrinkPotionTimer = 60000;
            }
            else m_uiDrinkPotionTimer -= uiDiff;

            if(m_uiSwitchTimer < uiDiff)
            {
                if(m_pInstance)
                    m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MUTATED_TRANSFORMATION);
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase = PHASE_3;
            }
            else m_uiSwitchTimer -= uiDiff;

            break;
            // phase 3
        case PHASE_3:

            if (m_uiMutatedPlagueTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                    DoCast(pTarget, SPELL_MUTATED_PLAGUE);
                m_uiMutatedPlagueTimer = 10000;
            }
            else m_uiMutatedPlagueTimer -= uiDiff;

            break;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_professor_putricide(Creature* pCreature)
{
    return new boss_professor_putricideAI(pCreature);
}

struct MANGOS_DLL_DECL mob_icc_gas_cloudAI : public ScriptedAI
{
    mob_icc_gas_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
		DoCast(m_creature, SPELL_GASEOUS_BLOAT_10);
		m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_icc_gas_cloud(Creature* pCreature)
{
    return new mob_icc_gas_cloudAI(pCreature);
}

struct MANGOS_DLL_DECL mob_icc_volatile_oozeAI : public ScriptedAI
{
    mob_icc_volatile_oozeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance* m_pInstance;

	bool m_bHasTarget;

    void Reset()
    {
		m_bHasTarget = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(!m_bHasTarget)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_OOZE_ADHESIVE_10);
			m_bHasTarget = true;
		}

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_icc_volatile_ooze(Creature* pCreature)
{
    return new mob_icc_volatile_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_icc_gas_bombAI : public ScriptedAI
{
    mob_icc_gas_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiExplodeTimer;
	uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiExplodeTimer	= 20000;
		m_uiDieTimer		= 21000;
		m_creature->SetRespawnTime(DAY);
		DoCast(m_creature, SPELL_CHOKING_GAS_TRIG);
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiExplodeTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_CHOKING_GAS_EXPL_TRIG);
			m_uiExplodeTimer = 10000;
		}
		else m_uiExplodeTimer -= uiDiff;

		if(m_uiDieTimer < uiDiff)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_icc_gas_bomb(Creature* pCreature)
{
    return new mob_icc_gas_bombAI(pCreature);
}

struct MANGOS_DLL_DECL mob_slime_puddleAI : public ScriptedAI
{
    mob_slime_puddleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

	float m_fSize;
    uint32 m_uiSizeTimer;

    void Reset()
    {
		m_creature->SetRespawnTime(DAY);
        DoCast(m_creature, SPELL_SLIME_PUDDLE_TRIG);

		m_fSize = 0.33f;
		m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
        m_uiSizeTimer = 1000;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if(m_uiSizeTimer < uiDiff && m_fSize <= 1.0f)
        {
			m_fSize += 0.032f;
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
            m_uiSizeTimer = 1000;
        }
        else
            m_uiSizeTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_slime_puddle(Creature* pCreature)
{
    return new mob_slime_puddleAI(pCreature);
}

void AddSC_boss_professor_putricide()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_professor_putricide";
    newscript->GetAI = &GetAI_boss_professor_putricide;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_volatile_ooze";
    newscript->GetAI = &GetAI_mob_icc_volatile_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_gas_cloud";
    newscript->GetAI = &GetAI_mob_icc_gas_cloud;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_gas_bomb";
    newscript->GetAI = &GetAI_mob_icc_gas_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_slime_puddle";
    newscript->GetAI = &GetAI_mob_slime_puddle;
    newscript->RegisterSelf();
}
