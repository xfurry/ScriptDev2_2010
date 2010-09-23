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
SDName: Boss_ahune
SD%Complete:
SDComment:
SDCategory: Slave_pens
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_ICE_SPIKE_TRIGG		= 50097,	// spike spell
	SPELL_AHUNE_SHIELD			= 45954,	// in phase 1
	SPELL_AHUNE_LOOT			= 45939,
	SPELL_ICE_AURA_VISUAL		= 45945,
	SPELL_REMERGE				= 46402,
	SPELL_GHOST					= 46786,

	// unused
	SPELL_SUMMON_AHUNE			= 45926,
	SPELL_AHUNE_ACHIEV			= 62043,
	SPELL_CRYSTAL_SPIKE			= 47944,
	SPELL_CRYSTAL_SPIKE_H		= 57067,
	SPELL_SUMMON_SPIKE			= 47955,

	// adds spells
	// hailstone
	SPELL_CHILLING_AURA			= 46542,
	SPELL_HAILSTONE_CHILL		= 46458,
	SPELL_PULVERIZE				= 2676,

	// coldwave
	SPELL_BITTER_BLAST			= 46406,

	// frostwind
	SPELL_LIGHTNING_SHIELD		= 12550,
	SPELL_WIND_BUFFET			= 46568,

	// skarthis
	SPELL_CRASHING_WAVE			= 55909,
	SPELL_FROST_NOVA			= 11831,
	SPELL_FROSTBOLT				= 15043,

	// npc
	NPC_AHUNE					= 25740,
	NPC_FROZEN_CORE				= 25865,
	NPC_GHOST_OF_AHUNE			= 26239,
	NPC_SKARTHIS_SUMMONER		= 40446,
	NPC_AHUNITE_HAILSTONE		= 25755,	// 1 at the start of phase 1
	NPC_AHUNITE_COLDWAVE		= 25756,	// pair of 2 periodicaly in phase 1
	NPC_AHUNITE_FROSTWIND		= 25757,	// periodicaly in phase 1 - not in first phase
};

struct MANGOS_DLL_DECL boss_ahuneAI : public Scripted_NoMovementAI
{
    boss_ahuneAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiPhase;
	uint32 m_uiPhaseChangeTimer;

	uint32 m_uiSpikeTimer;
	uint32 m_uiAddsTimer;
	bool m_bIsFirstPhase;
	uint8 m_uiWaveCount;

	uint64 m_uiCoreGUID;
	uint32 m_uiCoreHp;

	uint32 m_uiDieTimer;
	bool m_bMustDie;

    void Reset()
    {
		m_uiPhase = 0;
		m_uiPhaseChangeTimer = 90000;
		m_uiSpikeTimer		= 7000;
		m_uiAddsTimer		= 10000;
		m_bIsFirstPhase		= true;
		m_uiWaveCount		= 1;
		m_uiCoreGUID		= 0;
		m_uiCoreHp			= 0;
		m_uiDieTimer		= 1000;
		m_bMustDie			= false;
		DoCast(m_creature, SPELL_AHUNE_SHIELD);
    }

	void Aggro(Unit* pWho)
	{
		m_creature->SetInCombatWithZone();
		DoCast(m_creature, SPELL_ICE_AURA_VISUAL);
		m_uiPhase = 1;
	}

	void JustReachedHome()
	{
		m_creature->ForcedDespawn();
		if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_uiCoreGUID))
			pTemp->ForcedDespawn();
	}

	void JustDied(Unit* pKiller)
	{
		DoCast(m_creature, SPELL_AHUNE_ACHIEV);
	}

	void JustSummoned(Creature* pSummon)
	{
		pSummon->SetInCombatWithZone();
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_creature->GetHealth() < uiDamage)
		{
			uiDamage = 0;
			DoSummonLoot();
		}
	}

	void DoSummonLoot()
	{
		DoCast(m_creature, SPELL_AHUNE_LOOT);
		m_uiDieTimer = 100;
		m_bMustDie = true;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSpikeTimer < uiDiff)
		{ 
			// ToDo: needs fixing!
			//if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			//	DoCast(pTarget, SPELL_ICE_SPIKE_TRIGG);
			m_uiSpikeTimer = urand(10000, 15000);
		}
		else m_uiSpikeTimer -= uiDiff;

		if(m_uiDieTimer < uiDiff && m_bMustDie)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= uiDiff;

		if(m_uiPhase == 1)
		{
			if(!m_creature->HasAura(SPELL_AHUNE_SHIELD, EFFECT_INDEX_0))
				DoCast(m_creature, SPELL_AHUNE_SHIELD);

			if(m_uiAddsTimer < uiDiff)
			{
				for(uint8 i = 0; i < m_uiWaveCount; i++)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
					for(uint8 j = 0; j < 2; j++)
						m_creature->SummonCreature(NPC_AHUNITE_COLDWAVE, homeX + urand(0, 2), homeY + urand(0, 2), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
				}
				if(!m_bIsFirstPhase)
				{
					for(uint8 i = 0; i < m_uiWaveCount - 1; i++)
						m_creature->SummonCreature(NPC_AHUNITE_FROSTWIND, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
				}
				m_uiAddsTimer = urand(5000, 7000);
			}
			else m_uiAddsTimer -= uiDiff;

			if(m_uiPhaseChangeTimer < uiDiff)
			{
				if(m_bIsFirstPhase)
				{
					if(Creature* pTemp = m_creature->SummonCreature(NPC_FROZEN_CORE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
					{
						m_uiCoreGUID = pTemp->GetGUID();
						pTemp->setFaction(14);
					}
				}
				else
				{
					if(Creature* pCore = m_creature->GetMap()->GetCreature(m_uiCoreGUID))
					{
						pCore->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						pCore->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						pCore->SetVisibility(VISIBILITY_ON);
					}
				}
				m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->RemoveAurasDueToSpell(SPELL_AHUNE_SHIELD);
				DoCast(m_creature, SPELL_GHOST);
				m_uiPhase = 2;
				m_uiWaveCount += 1;
				m_uiPhaseChangeTimer = 40000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
		else if(m_uiPhase == 2)
		{
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				if(Creature* pCore = m_creature->GetMap()->GetCreature(m_uiCoreGUID))
				{
					pCore->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					pCore->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					pCore->SetVisibility(VISIBILITY_OFF);
				}
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->SummonCreature(NPC_AHUNITE_HAILSTONE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
				m_creature->RemoveAurasDueToSpell(SPELL_GHOST);
				DoCast(m_creature, SPELL_REMERGE);
				m_bIsFirstPhase = false;
				m_uiPhase = 1;
				m_uiPhaseChangeTimer = 90000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;
		}
    }
};
CreatureAI* GetAI_boss_ahune(Creature* pCreature)
{
    return new boss_ahuneAI(pCreature);
}

struct MANGOS_DLL_DECL mob_frozen_coreAI : public Scripted_NoMovementAI
{
    mob_frozen_coreAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) 
	{
		Reset();
	}

    void Reset()
    {
    }

	void JustDied(Unit* pKiller)
	{
		if(Creature* pAhune = GetClosestCreatureWithEntry(m_creature, NPC_AHUNE, 5.0f))
			((boss_ahuneAI*)pAhune->AI())->DoSummonLoot();
	}

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_mob_frozen_core(Creature* pCreature)
{
    return new mob_frozen_coreAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ahunite_hailstoneAI : public ScriptedAI
{
    mob_ahunite_hailstoneAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiChillTimer;
	uint32 m_uiPulverizeTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_CHILLING_AURA);
		m_uiPulverizeTimer = 7000;
		m_uiChillTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiChillTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(pTarget, SPELL_HAILSTONE_CHILL);
			m_uiChillTimer = 5000;
		}
		else m_uiChillTimer -= uiDiff;

		if(m_uiPulverizeTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_PULVERIZE);
			m_uiPulverizeTimer = 10000;
		}
		else m_uiPulverizeTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ahunite_hailstone(Creature* pCreature)
{
    return new mob_ahunite_hailstoneAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ahunite_coldwaveAI : public ScriptedAI
{
    mob_ahunite_coldwaveAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiBlastTimer;

    void Reset()
    {
		m_uiBlastTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiBlastTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_BITTER_BLAST);
			m_uiBlastTimer = urand(4000, 7000);
		}
		else m_uiBlastTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ahunite_coldwave(Creature* pCreature)
{
    return new mob_ahunite_coldwaveAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ahunite_frostwindAI : public ScriptedAI
{
    mob_ahunite_frostwindAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiBuffetTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_LIGHTNING_SHIELD);
		m_uiBuffetTimer = 6000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiBuffetTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_WIND_BUFFET);
			m_uiBuffetTimer = 6000;
		}
		else m_uiBuffetTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ahunite_frostwind(Creature* pCreature)
{
    return new mob_ahunite_frostwindAI(pCreature);
}

void AddSC_boss_ahune()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ahune";
    newscript->GetAI = &GetAI_boss_ahune;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_frozen_core";
    newscript->GetAI = &GetAI_mob_frozen_core;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ahunite_hailstone";
    newscript->GetAI = &GetAI_mob_ahunite_hailstone;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ahunite_coldwave";
    newscript->GetAI = &GetAI_mob_ahunite_coldwave;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ahunite_frostwind";
    newscript->GetAI = &GetAI_mob_ahunite_frostwind;
    newscript->RegisterSelf();
}
