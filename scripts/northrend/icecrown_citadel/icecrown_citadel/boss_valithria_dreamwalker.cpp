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
SDName: boss_valithria
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
	SAY_START						= -1609530,
	SAY_PORTALS						= -1609531,
	SAY_75HEALTH					= -1609532,
	SAY_25HEALTH					= -1609533,
	SAY_DEATH						= -1609534,
	SAY_SLAY						= -1609535,
	SAY_BERSERK						= -1609536,
	SAY_VICTORY						= -1609537,

	SPELL_NIGHTMARE_PORTALS			= 72482,
	SPELL_EMERAL_VIGOR				= 36789,
	SPELL_DREAMWALKERS_RAGE			= 71189,
	NPC_NIGHTMARE_CLOUD				= 38421,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations SpawnLoc[]=
{
    {4203.470215f, 2484.500000f, 364.872009f},  // 0 Valithria
    {4240.688477f, 2405.794678f, 364.868591f},  // 1 Valithria Room 1
    {4165.112305f, 2405.872559f, 364.872925f},  // 2 Valithria Room 2
    {4166.216797f, 2564.197266f, 364.873047f},  // 3 Valithria Room 3
    {4239.579102f, 2566.753418f, 364.868439f},  // 4 Valithria Room 4
};

enum
{
	SPELL_CORRUPTION	            = 70602,
	SPELL_FROSTV_10                 = 70759,
	SPELL_FROSTV_25                 = 71889,
	SPELL_FROSTV_10HC               = 72015,
	SPELL_FROSTV_25HC               = 72016,
	SPELL_MANA_VOID_10              = 71179,
	SPELL_MANA_VOID_25              = 71741,
	NPC_MANA_VOID					= 38068,
	SPELL_MANA_VOID_AURA			= 71085,
};

/*######
## // Risen archmage
######*/
struct MANGOS_DLL_DECL mob_risen_archmageAI : public ScriptedAI
{
    mob_risen_archmageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;

    uint32 m_uiCorruptionTimer;
	uint32 m_uiFrostvTimer;
	uint32 m_uiManavoidTimer;
		
	void Reset()
	{
		m_uiFrostvTimer                = urand(7000, 11000);
		m_uiManavoidTimer              = 15000;

		if(Creature* pTarget = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_VALITHRIA))))
			DoCast(pTarget, SPELL_CORRUPTION);
	}

	void Aggro(Unit* pWho)
	{
		m_creature->CastStop();
	}

	void JustDied(Unit* pKiller)
	{
		// start event here
	}

	void JustSummoned(Creature* pSummon)
	{
		pSummon->CastSpell(pSummon, SPELL_MANA_VOID_AURA, false);
		pSummon->SetInCombatWithZone();
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiFrostvTimer < uiDiff)
		{
			if( Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_FROSTV_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_FROSTV_25);
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
				DoCast(m_creature, SPELL_FROSTV_10HC);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
				DoCast(m_creature, SPELL_FROSTV_25HC);
			m_uiFrostvTimer = urand(7000, 11000);
		}
		else m_uiFrostvTimer -= uiDiff;

		if (m_uiManavoidTimer < uiDiff)
        {
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_MANA_VOID_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_MANA_VOID_25);
			m_uiManavoidTimer = urand(7000, 11000);
        }
        else m_uiManavoidTimer -= uiDiff;
        
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_risen_archmage(Creature* pCreature)
{
    return new mob_risen_archmageAI(pCreature);
}

enum
{
	SPELL_FIREB_10                  = 70754,
	SPELL_FIREB_25                  = 71748,
	SPELL_FIREB_10HC                = 72023,
	SPELL_FIREB_25HC                = 72024,
	SPELL_LAY_10                    = 69325,
	SPELL_LAY_25                    = 71730,
};

/*######
## // Blazing Skeleton
######*/
struct MANGOS_DLL_DECL mob_blazing_skeletonAI : public ScriptedAI
{
    mob_blazing_skeletonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;
    
	uint32 m_uiFirebTimer;
	uint32 m_uiLayWasteTimer;
	
	void Reset()
	{
		m_creature->SetRespawnDelay(DAY);
		m_uiFirebTimer                 = urand(7000, 11000);
		m_uiLayWasteTimer              = 15000;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiFirebTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if( Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_FIREB_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_FIREB_25);
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					DoCast(pTarget, SPELL_FIREB_10HC);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					DoCast(pTarget, SPELL_FIREB_25HC);
			}
			m_uiFirebTimer = urand(7000, 11000);
        }
        else m_uiFirebTimer -= uiDiff;

		if (m_uiLayWasteTimer < uiDiff)
        {
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_LAY_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_LAY_25);
			m_uiLayWasteTimer = 15000;
        }
        else m_uiLayWasteTimer -= uiDiff;
        
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blazing_skeleton(Creature* pCreature)
{
    return new mob_blazing_skeletonAI(pCreature);
}

enum
{	
	SPELL_SUPPRESSION   = 70588,
};

/*######
## // Suppresser
######*/
struct MANGOS_DLL_DECL mob_suppresserAI : public ScriptedAI
{
    mob_suppresserAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;
    uint32 m_uiSuppressionTimer;
	bool m_bHasSuppresed;
	
	void Reset()
	{
		m_creature->SetRespawnDelay(DAY);
		m_uiSuppressionTimer     = 5000;
		m_bHasSuppresed			 = false;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSuppressionTimer < uiDiff && !m_bHasSuppresed)
        {
			if(Creature* pTarget = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_VALITHRIA))))
			    DoCast(pTarget, SPELL_SUPPRESSION);
			m_bHasSuppresed = true;
        }
        else m_uiSuppressionTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_suppresser(Creature* pCreature)
{
    return new mob_suppresserAI(pCreature);
}

enum
{
	SPELL_CORROSION					= 70749,
	SPELL_ACIDB_10					= 70744,
	SPELL_ACIDB_25					= 71733,
	SPELL_ACIDB_10HC				= 72017,
	SPELL_ACIDB_25HC				= 72018,
};

/*######
## // Blistering Zombies
######*/
struct MANGOS_DLL_DECL mob_blistering_zombieAI : public ScriptedAI
{
    mob_blistering_zombieAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;

	uint32 m_uiDieTimer;

	void Reset()
	{
		m_creature->SetRespawnDelay(DAY);
		DoCast(m_creature, SPELL_CORROSION);
		m_uiDieTimer = 600000;
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(uiDamage > m_creature->GetHealth())
		{
			uiDamage = 0;
			m_creature->SetHealth(m_creature->GetMaxHealth());

			if( Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_FROSTV_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_FROSTV_25);
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
				DoCast(m_creature, SPELL_FROSTV_10HC);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
				DoCast(m_creature, SPELL_FROSTV_25HC);

			m_uiDieTimer = 1000;
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiDieTimer < uiDiff)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= uiDiff;
		    
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blistering_zombie(Creature* pCreature)
{
    return new mob_blistering_zombieAI(pCreature);
}

enum
{

	
	SPELL_GUT_SPRAY_10      = 70633,
	SPELL_GUT_SPRAY_25      = 71283,
	SPELL_GUT_SPRAY_10HC    = 72025,
	SPELL_GUT_SPRAY_25HC    = 72026,
	
	NPC_ROT_WORM			= 37907,
	SPELL_ROT_WORM			= 72962,
	
};

/*######
## // Gluttonous Abomination
######*/
struct MANGOS_DLL_DECL mob_gluttonous_abominationAI : public ScriptedAI
{
    mob_gluttonous_abominationAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;

	uint32 m_uiGutSprayTimer;
	
	void Reset()
	{
		m_uiGutSprayTimer                 = 14000;
		m_creature->SetRespawnDelay(DAY);
	}

	void JustDied(Unit* pKiller)
	{
		if(Creature* pTemp = m_creature->SummonCreature(NPC_ROT_WORM, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000))
			pTemp->CastSpell(pTemp, SPELL_ROT_WORM, false);
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiGutSprayTimer < uiDiff)
        {
			if( Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_GUT_SPRAY_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_GUT_SPRAY_25);
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
				DoCast(m_creature, SPELL_GUT_SPRAY_10HC);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
				DoCast(m_creature, SPELL_GUT_SPRAY_25HC);
			m_uiGutSprayTimer = urand(7000, 11000);
        }
        else m_uiGutSprayTimer -= uiDiff;
		    
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_gluttonous_abomination(Creature* pCreature)
{
    return new mob_gluttonous_abominationAI(pCreature);
}

void AddSC_boss_valithria()
{
	Script *newscript;

	newscript = new Script;
    newscript->Name = "mob_risen_archmage";
    newscript->GetAI = &GetAI_mob_risen_archmage;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_blazing_skeleton";
    newscript->GetAI = &GetAI_mob_blazing_skeleton;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_suppresser";
    newscript->GetAI = &GetAI_mob_suppresser;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_blistering_zombie";
    newscript->GetAI = &GetAI_mob_blistering_zombie;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gluttonous_abomination";
    newscript->GetAI = &GetAI_mob_gluttonous_abomination;
    newscript->RegisterSelf();
}