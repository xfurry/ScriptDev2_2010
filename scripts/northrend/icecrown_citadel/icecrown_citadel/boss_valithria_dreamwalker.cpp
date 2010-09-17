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
SDComment: ToDo: implement nightmare portals event
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

	SPELL_IMMUNITY					= 72724,
	SPELL_NIGHTMARE_PORTALS			= 72482,
	SPELL_NIGHTMARE_PORTAL_SUMMON	= 71987,
	SPELL_NIGHTMARE_VISUAL			= 71977,
	SPELL_DREAM_PREPORTAL_VISUAL	= 71304,
	SPELL_DREAM_PORTAL_VISUAL		= 70763,
	SPELL_DREAM_SPLIT				= 71196,
	SPELL_GREEN_CLOUD_VISUAL		= 70876,
	SPELL_DREAM_STATE				= 70766,	// phase = 16, fly effect
	SPELL_EMERAL_VIGOR				= 70873,
	SPELL_DREAMWALKERS_RAGE			= 71189,	// at full hp
	SPELL_CORRUPTION				= 70904,
	SPELL_COLUMN_OF_FROST			= 70702,
	SPELL_COLUMN_OF_FROST_SUMMON	= 71747,
	NPC_COLUMN_OF_FROST				= 37918,
	NPC_NIGHTMARE_PREPORTAL			= 38429,
	NPC_NIGHTMARE_CLOUD				= 38421,
	NPC_NIGHTMARE_PORTAL			= 38430,

	// Adds
	NPC_RISEN_ARCHMAGE				= 37868,
	NPC_SKELETON					= 36791,
	NPC_SUPPRESOR					= 37863,
	NPC_ZOMBIE						= 37934,
	NPC_ABOMINATION					= 37886,
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

enum phases
{
	PHASE_IDLE		= 0,
	PHASE_EVENT		= 1,
	PHASE_OUTRO		= 2,
};

/*######
## // Boss Valithria
######*/
struct MANGOS_DLL_DECL boss_valithriaAI : public ScriptedAI
{
    boss_valithriaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		SetCombatMovement(false);
		m_bIsIntro = false;
        Reset();
    }

	ScriptedInstance *m_pInstance;
	uint32 Difficulty;

	uint32 m_uiBerserkTimer;
	uint32 m_uiPortalTimer;
	uint8 m_uiMaxPortals;
	uint32 m_uiSummonTimer;
	uint32 m_uiSummonCount;
	uint32 m_uiColumnOfFrostTimer;
	uint32 m_uiCloudsSummonTimer;
	uint8 m_uiCloudStage;
	uint32 m_uiDespawnTimer;
	uint8 m_uiLocId;
	uint32 m_uiAddEntry;
	bool m_bHasLessHealth;
	bool m_bHasMoreHealth;
	bool m_bIsBerserk;

    uint8 m_uiPhase;
	uint8 m_uiArchmageDied;
	bool m_bIsIntro;
	std::list<uint64> m_lAddsGUIDList;
		
	void Reset()
	{
		m_uiBerserkTimer		= 600000;	// 10 min
		m_uiPortalTimer			= 30000;
		m_uiCloudsSummonTimer	= 40000;
		m_uiCloudStage			= 0;
		m_uiColumnOfFrostTimer	= 30000;
		m_uiSummonCount			= 0;
		m_uiSummonTimer			= 10000;
		m_uiDespawnTimer		= 5000;
		m_uiLocId				= 1;
		m_uiAddEntry			= 0;
		m_bHasLessHealth		= false;
		m_bHasMoreHealth		= false;
		m_bIsBerserk			= false;
		m_lAddsGUIDList.clear();

		m_uiMaxPortals = (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL) ? 3 : 6;

		m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
		m_uiPhase	= PHASE_IDLE;
		m_uiArchmageDied	= 0;
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);		// remove feign death
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);	// unselectable, so it cannot be healed on idle
		
		ReviveMages();
	}

	void ReviveMages()				
	{
		// revive mages
		std::list<Creature*> lMobs;
		GetCreatureListWithEntryInGrid(lMobs, m_creature, NPC_RISEN_ARCHMAGE, 100.0f);

		if (!lMobs.empty())
		{
			for(std::list<Creature*>::iterator iter = lMobs.begin(); iter != lMobs.end(); ++iter)
			{
				if(*iter)
				{
					if (!(*iter)->isAlive())
						(*iter)->Respawn();
				}
			}
		}
	}

	void DespawnAdds()
	{
		if (!m_lAddsGUIDList.empty())
		{
			for(std::list<uint64>::iterator itr = m_lAddsGUIDList.begin(); itr != m_lAddsGUIDList.end(); ++itr)
			{
				if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
				{
					if (pTemp->isAlive())
						pTemp->ForcedDespawn();
				}
			}
		}
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_DEATH, m_creature);
		DespawnAdds();
		m_creature->Respawn();
	}

	void JustRespawned()
	{
		if(m_pInstance)
			m_pInstance->SetData(TYPE_DREAMWALKER, NOT_STARTED);
		Reset();
	}

	void MoveInLineOfSight(Unit* pWho)
    {
        if(!m_bIsIntro && m_creature->IsWithinDistInMap(pWho, 50) && m_creature->IsWithinLOSInMap(pWho))
		{
			// some lich king yell here
            m_bIsIntro = true;
		}
        ScriptedAI::MoveInLineOfSight(pWho);  
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void JustSummoned(Creature* pSummon)
	{
		if(pSummon->GetEntry() != NPC_NIGHTMARE_PREPORTAL && pSummon->GetEntry() != NPC_COLUMN_OF_FROST)
		{
			pSummon->AddThreat(m_creature, 10000.0f);
			pSummon->AI()->AttackStart(m_creature);
		}
	}

	void SummonAdds()
	{
		switch(urand(0, 7))
		{
		case 0:
			m_uiAddEntry = NPC_RISEN_ARCHMAGE;
			break;
		case 1:
		case 2:
			m_uiAddEntry = NPC_SKELETON;
			break;
		case 3:
		case 4:
			m_uiAddEntry = NPC_SUPPRESOR;
			break;
		case 5:
		case 6:
			m_uiAddEntry = NPC_ZOMBIE;
			break;
		case 7:
			m_uiAddEntry = NPC_ABOMINATION;
			break;
		}

		m_uiLocId = urand(1, 4);
		if(Creature* pTemp = m_creature->SummonCreature(m_uiAddEntry, SpawnLoc[m_uiLocId].x, SpawnLoc[m_uiLocId].y, SpawnLoc[m_uiLocId].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 1000))
			m_lAddsGUIDList.push_back(pTemp->GetGUID());
	}

    void UpdateAI(const uint32 uiDiff)
    {
		switch(m_uiPhase)
		{
		case PHASE_IDLE:

			if(m_pInstance->GetData(TYPE_SVALNA) == DONE && !m_creature->HasAura(SPELL_CORRUPTION))
				DoCast(m_creature, SPELL_CORRUPTION);

			if(m_uiArchmageDied == 4)
			{
				m_uiPhase = PHASE_EVENT;
				DoScriptText(SAY_START, m_creature);
				if(m_pInstance)
					m_pInstance->SetData(TYPE_DREAMWALKER, IN_PROGRESS);
				m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5f);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			}
			break;
		case PHASE_EVENT:

			if(m_uiBerserkTimer < uiDiff && !m_bIsBerserk)
			{
				DoScriptText(SAY_BERSERK, m_creature);
				m_bIsBerserk = true;
			}
			else m_uiBerserkTimer -= uiDiff;

			if(m_uiColumnOfFrostTimer < uiDiff)
			{
				for(int i = 0; i < m_uiMaxPortals; i++)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + urand(30, 40)*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + urand(30, 40)*sin(angle*(M_PI/180));
					m_creature->SummonCreature(NPC_COLUMN_OF_FROST, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
				}
				m_uiColumnOfFrostTimer = 40000;
			}
			else m_uiColumnOfFrostTimer -= uiDiff;

			if(m_uiPortalTimer < uiDiff)
			{
				DoCast(m_creature, SPELL_NIGHTMARE_VISUAL);
				// summon portals here
				for(int i = 0; i < m_uiMaxPortals; i++)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + 35*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + 35*sin(angle*(M_PI/180));
					m_creature->SummonCreature(NPC_NIGHTMARE_PREPORTAL, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
				}
				DoScriptText(SAY_PORTALS, m_creature);
				m_uiPortalTimer = 50000;
				m_uiCloudsSummonTimer = 10000;
			}
			else m_uiPortalTimer -= uiDiff;

			if(m_uiCloudsSummonTimer < uiDiff)
			{
				// summon clouds here
				for(int i = 0; i < 3 * m_uiMaxPortals; i++)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + urand(5, 35)*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + urand(5, 35)*sin(angle*(M_PI/180));
					float homeZ = m_creature->GetPositionZ() + urand(10, 15);
					if(Creature* pTemp = m_creature->SummonCreature(NPC_NIGHTMARE_CLOUD, homeX, homeY, homeZ, 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
					{
						pTemp->GetMap()->CreatureRelocation(pTemp, homeX, homeY, homeZ, 0);
						pTemp->SendMonsterMove(homeX, homeY, homeZ, SPLINETYPE_NORMAL, pTemp->GetSplineFlags(), 1);
					}
				}
				m_uiCloudStage += 1;
				if(m_uiCloudStage < 4)
					m_uiCloudsSummonTimer = 10000;
				else
				{
					m_uiCloudStage = 0;
					m_uiCloudsSummonTimer = 50000;
				}
			}
			else m_uiCloudsSummonTimer -= uiDiff;

			if(m_uiSummonTimer < uiDiff)
			{
				SummonAdds();
				m_uiSummonCount += 1;
				m_uiSummonTimer = m_bIsBerserk ? 500 : 5000 - m_uiSummonCount * 33.33;
			}
			else m_uiSummonTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 25.0f && !m_bHasLessHealth)
			{
				DoScriptText(SAY_25HEALTH, m_creature);
				m_bHasLessHealth = true;
				m_bHasMoreHealth = false;
			}

			if(m_creature->GetHealthPercent() > 75.0f && !m_bHasMoreHealth)
			{
				DoScriptText(SAY_75HEALTH, m_creature);
				m_bHasLessHealth = false;
				m_bHasMoreHealth = true;
			}

			if(m_creature->GetHealthPercent() == 100)
			{
				m_creature->RemoveAurasDueToSpell(SPELL_CORRUPTION);
				DoCast(m_creature, SPELL_DREAMWALKERS_RAGE);
				DoScriptText(SAY_VICTORY, m_creature);
				m_uiPhase = PHASE_OUTRO;
			}
			break;
		case PHASE_OUTRO:

			if(m_uiDespawnTimer < uiDiff)
			{
				m_creature->ForcedDespawn();
				if(m_pInstance)
					m_pInstance->SetData(TYPE_DREAMWALKER, DONE);
			}
			else m_uiDespawnTimer -= uiDiff;

			break;
		}
	}
};

CreatureAI* GetAI_boss_valithria(Creature* pCreature)
{
    return new boss_valithriaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_nightmare_cloudAI : public ScriptedAI
{
    mob_nightmare_cloudAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->GetMotionMaster()->MoveRandom();
        pCreature->SetDisplayId(29352);
		SetCombatMovement(false); 
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bPortalCasted;

    void Reset()
    {
		DoCast(m_creature, SPELL_GREEN_CLOUD_VISUAL);
		m_creature->SetPhaseMask(16, true);
		m_creature->GetMotionMaster()->MoveConfused();
        m_bPortalCasted = false;
    }

	void MoveInLineOfSight(Unit* pWho) 
	{
		if (pWho->isAlive() && pWho->GetTypeId() == TYPEID_PLAYER && pWho->IsWithinDistInMap(m_creature, 2.0f) && !m_bPortalCasted)
		{
			DoCast(pWho, SPELL_EMERAL_VIGOR);
			m_bPortalCasted = true;
		}
	}

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS || m_bPortalCasted)
			m_creature->ForcedDespawn();
    }

};

CreatureAI* GetAI_mob_nightmare_cloud(Creature *pCreature)
{
    return new mob_nightmare_cloudAI(pCreature);
};

enum
{
	SPELL_CORRUPTION_CHANNEL        = 70602,
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

	std::list<uint64> m_lAddsGUIDList;
		
	void Reset()
	{
		m_uiFrostvTimer                = urand(7000, 11000);
		m_uiManavoidTimer              = 15000;
		m_lAddsGUIDList.clear();

		if(Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
			DoCast(pTarget, SPELL_CORRUPTION_CHANNEL);
	}

	void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
        }
    }

	void Aggro(Unit* pWho)
	{
		m_creature->CastStop();
		std::list<Creature*> lMobs;
		GetCreatureListWithEntryInGrid(lMobs, m_creature, NPC_RISEN_ARCHMAGE, 100.0f);

		if (!lMobs.empty())
		{
			for(std::list<Creature*>::iterator iter = lMobs.begin(); iter != lMobs.end(); ++iter)
			{
				if(*iter)
				{
					if ((*iter)->isAlive())
						(*iter)->AI()->AttackStart(pWho);
				}
			}
		}
	}

	void JustReachedHome()
	{
		if(Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
			DoCast(pTarget, SPELL_CORRUPTION_CHANNEL);

		std::list<Creature*> lMobs;
		GetCreatureListWithEntryInGrid(lMobs, m_creature, NPC_RISEN_ARCHMAGE, 100.0f);

		if (!lMobs.empty())
		{
			for(std::list<Creature*>::iterator iter = lMobs.begin(); iter != lMobs.end(); ++iter)
			{
				if(*iter)
				{
					if ((*iter)->isAlive())
						(*iter)->AI()->EnterEvadeMode();
				}
			}
		}
	}

	void JustDied(Unit* pKiller)
	{
		if(m_pInstance->GetData(TYPE_SVALNA) == DONE)
		{
			if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
				((boss_valithriaAI*)pValithria->AI())->m_uiArchmageDied += 1;
		}
		DespawnAdds();
	}

	void DespawnAdds()
	{
		if (!m_lAddsGUIDList.empty())
		{
			for(std::list<uint64>::iterator itr = m_lAddsGUIDList.begin(); itr != m_lAddsGUIDList.end(); ++itr)
			{
				if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
				{
					if (pTemp->isAlive())
						pTemp->ForcedDespawn();
				}
			}
		}
	}

	void KilledUnit(Unit* pVictim)
    {
       if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
            DoScriptText(SAY_SLAY, pValithria);
    }

	void JustSummoned(Creature* pSummon)
	{
		pSummon->CastSpell(pSummon, SPELL_MANA_VOID_AURA, false);
		pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pSummon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pSummon->SetDisplayId(11686);     // make invisible
        pSummon->setFaction(14);
		pSummon->AttackStop();
		pSummon->GetMotionMaster()->MoveConfused();
		m_lAddsGUIDList.push_back(pSummon->GetGUID());
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
			m_uiManavoidTimer = 30000;
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

	void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
        }
    }

	void KilledUnit(Unit* pVictim)
    {
       if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
            DoScriptText(SAY_SLAY, pValithria);
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

	void KilledUnit(Unit* pVictim)
    {
       if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
            DoScriptText(SAY_SLAY, pValithria);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS)
			m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSuppressionTimer < uiDiff && !m_bHasSuppresed)
        {
			if(Creature* pTarget = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
			    DoCast(pTarget, SPELL_SUPPRESSION);
			m_bHasSuppresed		 = true;
			//m_uiSuppressionTimer = 5000;
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

	void KilledUnit(Unit* pVictim)
    {
       if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
            DoScriptText(SAY_SLAY, pValithria);
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

	void KilledUnit(Unit* pVictim)
    {
       if (Creature* pValithria = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALITHRIA)))
            DoScriptText(SAY_SLAY, pValithria);
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

struct MANGOS_DLL_DECL mob_nightmare_preportalAI : public ScriptedAI
{
    mob_nightmare_preportalAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

	uint32 m_uiSummonTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_DREAM_PREPORTAL_VISUAL);
        m_creature->SetRespawnDelay(DAY);
		m_uiSummonTimer = 8000;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_uiSummonTimer < uiDiff)
		{
			m_creature->SummonCreature(NPC_NIGHTMARE_PORTAL, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
			//DoCast(m_creature, SPELL_NIGHTMARE_PORTAL_SUMMON);
			m_uiSummonTimer = 10000;
		}
		else m_uiSummonTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_nightmare_preportal(Creature* pCreature)
{
    return new mob_nightmare_preportalAI (pCreature);
}

struct MANGOS_DLL_DECL mob_nightmare_portalAI : public ScriptedAI
{
    mob_nightmare_portalAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

	bool m_bHasCasted;

    void Reset()
    {
		DoCast(m_creature, SPELL_DREAM_PORTAL_VISUAL);
        m_creature->SetRespawnDelay(DAY);
		m_bHasCasted = false;
    }

	void MoveInLineOfSight(Unit* pWho)
    {
		if(m_creature->IsWithinDistInMap(pWho, 2) && m_creature->IsWithinLOSInMap(pWho) && pWho->GetTypeId() == TYPEID_PLAYER && !m_bHasCasted)
		{
			pWho->CastSpell(pWho, SPELL_DREAM_STATE, false);
            m_bHasCasted = true;
		} 
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_nightmare_portal(Creature* pCreature)
{
    return new mob_nightmare_portalAI (pCreature);
}

struct MANGOS_DLL_DECL mob_column_of_frostAI : public ScriptedAI
{
    mob_column_of_frostAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

	uint32 m_uiCastTimer;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
		m_uiCastTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_DREAMWALKER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_uiCastTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_COLUMN_OF_FROST);
			m_uiCastTimer = 10000;
		}
		else m_uiCastTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_column_of_frost(Creature* pCreature)
{
    return new mob_column_of_frostAI (pCreature);
}

void AddSC_boss_valithria()
{
	Script *newscript;

	newscript = new Script;
    newscript->Name = "boss_valithria";
    newscript->GetAI = &GetAI_boss_valithria;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_nightmare_cloud";
    newscript->GetAI = &GetAI_mob_nightmare_cloud;
    newscript->RegisterSelf();

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

	newscript = new Script;
    newscript->Name = "mob_nightmare_preportal";
    newscript->GetAI = &GetAI_mob_nightmare_preportal;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_nightmare_portal";
    newscript->GetAI = &GetAI_mob_nightmare_portal;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_column_of_frost";
    newscript->GetAI = &GetAI_mob_column_of_frost;
    newscript->RegisterSelf();
}