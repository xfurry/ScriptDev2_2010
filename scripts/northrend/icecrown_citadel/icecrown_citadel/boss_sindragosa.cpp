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
SDName: boss_sindragosa
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
	// yells
	SAY_AGGRO			= -1609540,
	SAY_UNCHAINED_MAGIC	= -1609541,
	SAY_BLISTERING_COLD	= -1609542,
	SAY_RESPITE_SOUL	= -1609543,
	SAY_TAKEOFF			= -1609544,
	SAY_PHASE3			= -1609545,
	SAY_SLAY1			= -1609546,
	SAY_SLAY2			= -1609547,
	SAY_BERSERK			= -1609548,
	SAY_DEATH			= -1609549,

	// spells
	SPELL_FROSTAURA_10		= 70084,
	SPELL_FROSTAURA_25		= 71050,
	SPELL_CLEAVE_SIN		= 19983,
	SPELL_TAILSMASH			= 71077,
	SPELL_FROSTBREATH_10	= 69649,
	SPELL_FROSTBREATH_25	= 71056,
	SPELL_PERMACHILL		= 70109,
	SPELL_UNCHAINMAGIC		= 69762,
	SPELL_ICYGRIP			= 70117,
	SPELL_BLISTERINGCOLD_10	= 70123,
	SPELL_BLISTERINGCOLD_25	= 71047,

	SPELL_FROSTBEACON		= 70126,
	SPELL_ICETOMB			= 70157,
	SPELL_FROSTBOMB			= 71053,
	SPELL_MYSTICBUFFET		= 70128,

	PHASE_ONE				= 1,
    PHASE_AIR               = 2,
    PHASE_THREE             = 3,
};

const float posSindragosa[4] = {4453.670f, 2484.251f, 224.797f, 3.15f};

struct MANGOS_DLL_DECL boss_sindragosaAI : public ScriptedAI
{
	boss_sindragosaAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32	Difficulty;

	uint8	m_uiPhase;
	uint8	m_uiBombCount;
	uint8	m_uiFrostBreathCount;
	
	uint32	m_uiCleaveTimer;
	uint32	m_uiTailSmashTimer;
	uint32	m_uiFrostBreathTimer;
	uint32	m_uiBlisteringColdTimer;
	uint32	m_uiMysticBuffet;
	uint32	m_airPhaseTimer;
	uint32	m_uiFrostBeaconTimer;
	uint32	m_uiIceTombTimer;
	uint32	m_uiBombTimer;

	void Reset()
	{
		m_uiPhase = PHASE_ONE;
		m_airPhaseTimer = 0;
		m_uiBombCount = 0;
		m_uiFrostBreathCount = 0;
		m_uiFrostBeaconTimer = 3000;
		m_uiBombTimer = 10000;

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
		m_creature->SetRespawnDelay(DAY);
	} 

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_PERMACHILL);
		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
			DoCast(m_creature, SPELL_FROSTAURA_10);
		if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			DoCast(m_creature, SPELL_FROSTAURA_25);

		DoScriptText(SAY_AGGRO, m_creature);

		// make land
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);

		m_uiCleaveTimer = urand(4000, 7000);
		m_uiTailSmashTimer = urand(8000, 13000);
		m_uiFrostBreathTimer = urand(13000, 16000);
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
		if(m_pInstance) 
            m_pInstance->SetData(TYPE_SINDRAGOSA, DONE);
    }

	void JustReachedHome()
	{
		// respawn minibosses
		if (Creature* pSplinestalker = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_SPLINESTALKER))))
		{
			if(!pSplinestalker->isAlive())
				pSplinestalker->Respawn();
		}
		if (Creature* pRimefang = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RIMEFANG))))
		{
			if(!pRimefang->isAlive())
				pRimefang->Respawn();
		}
		m_creature->ForcedDespawn();
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
		
		switch (m_uiPhase)
        {
			case PHASE_THREE:
			{
				//mystic buffet
				if(m_uiMysticBuffet < uiDiff)
				{
					DoCast(m_creature, SPELL_MYSTICBUFFET);
					m_uiMysticBuffet = 5000;
				}
				else m_uiMysticBuffet -= uiDiff;
				
				//frost beacon
				if(m_uiFrostBeaconTimer < uiDiff)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_FROSTBEACON);
					m_uiFrostBeaconTimer = 10000; // 10s before next beacon
				}
				else m_uiFrostBeaconTimer -=uiDiff;

				//ice tomb
				if(m_uiIceTombTimer < uiDiff)
				{
					//TODO: select the beaconed target to cast ice tomb on him
					m_uiIceTombTimer = m_uiFrostBeaconTimer + 7000; // ~10s before next ice tomb (3s remaining till next beacon + 7s for ice tomb)
				}
				else m_uiIceTombTimer -=uiDiff;

				// no break, phase 3 will use same abilities as in 1
			}
			case PHASE_ONE :
			{
				if (m_uiPhase == PHASE_ONE)
                {
					float hp = m_creature->GetHealthPercent();
					if(hp < 35.0f) 
					{//less than 35%, enter last phase
						DoScriptText(SAY_PHASE3, m_creature);
						m_uiPhase = PHASE_THREE;
						m_uiMysticBuffet = 5000;
						m_uiFrostBeaconTimer = 3000;
						m_uiIceTombTimer = 10000;
						return;
					}
					if(hp < 85.0f) 
					{//less than 85%, enter first air phase
						if(m_airPhaseTimer < uiDiff)
						{
							m_uiPhase = PHASE_AIR;
							m_uiFrostBeaconTimer = 3000;
							m_uiIceTombTimer = 10000;
							m_uiBombTimer = 15000;
							
							DoScriptText(SAY_TAKEOFF, m_creature);
							// make fly
							m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
							m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
							return;
						}
						else m_airPhaseTimer -= uiDiff;
					}
					break;
				}

				//cleave
				if(m_uiCleaveTimer < uiDiff)
				{
					DoCast(m_creature->getVictim(), SPELL_CLEAVE_SIN);
					m_uiCleaveTimer = urand(4000, 7000);
				}
				else m_uiCleaveTimer -= uiDiff;

				//tail smash
				if (m_uiTailSmashTimer < uiDiff)
				{
					DoCast(m_creature, SPELL_TAILSMASH);
					m_uiTailSmashTimer = urand(8000, 13000);
				}
				else m_uiTailSmashTimer -= uiDiff;

				//frost breath
				if (m_uiFrostBreathTimer < uiDiff)
				{
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
						DoCast(m_creature, SPELL_FROSTBREATH_10);
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
						DoCast(m_creature, SPELL_FROSTBREATH_25);
					
					m_uiFrostBreathTimer = urand(13000, 16000);
					m_uiFrostBreathCount ++; //trigger a grip & cold 2-4 sec after the second frost breath
					m_uiBlisteringColdTimer = urand(2000, 4000);
				}
				else m_uiFrostBreathTimer -= uiDiff;

				//icy grip & blistering cold
				if (m_uiFrostBreathCount == 2 && m_uiBlisteringColdTimer < uiDiff)
				{
					//reset frostbreath counter
					m_uiFrostBreathCount=0;
					//cast grip
					DoCast(m_creature, SPELL_ICYGRIP);

					DoScriptText(SAY_BLISTERING_COLD, m_creature);
					//cast blistering cold
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
						DoCast(m_creature, SPELL_BLISTERINGCOLD_10);
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
						DoCast(m_creature, SPELL_BLISTERINGCOLD_25);
				}
				else m_uiBlisteringColdTimer -= uiDiff;

				DoMeleeAttackIfReady();
				break;
			}
			case PHASE_AIR :
			{
				//frost beacon
				if(m_uiFrostBeaconTimer < uiDiff)
				{
					uint8 count = (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL) ? 2 : 5;

					for(uint8 i=0; i<count; i++)
					{
						if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
						DoCast(pTarget, SPELL_FROSTBEACON);
					}
					m_uiFrostBeaconTimer = 60000; // no more beacon this phase
				}
				else m_uiFrostBeaconTimer -=uiDiff;

				//ice tomb
				if(m_uiIceTombTimer < uiDiff)
				{
					//TODO: select the beaconed targets to cast ice tomb on them
					m_uiIceTombTimer = 60000; //no more ice tomb this phase
				}
				else m_uiIceTombTimer -=uiDiff;

				//frost bomb x4
				if(m_uiBombTimer < uiDiff)
				{
					if(m_uiBombCount>3)
					{
						//reset the bomb counter
						m_uiBombCount = 0;
						//change to phase one
						m_uiPhase = PHASE_ONE;
						m_airPhaseTimer = 90000; //90 seconds to next air phase

						// make land
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
						return;
					}

					//drop a bomb
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_FROSTBOMB);
					m_uiBombCount ++;
					m_uiBombTimer = 10000; //drop another bomb after 10 seconds
				}
				else m_uiBombTimer -=uiDiff;
				break;
			}
		}
		
	}

	void SpellHit(Unit* pWho, const SpellEntry* spell) 
	{
		if(pWho->GetTypeId() == TYPEID_PLAYER)
		{
			DoCast(pWho, SPELL_UNCHAINMAGIC);
		}
	}
};

CreatureAI* GetAI_boss_sindragosa(Creature* pCreature)
{
    return new boss_sindragosaAI(pCreature);
}

enum
{
	SPELL_FROST_AURA					= 71387,
	SPELL_FROST_BREATH					= 71386,
	SPELL_ICY_BLAST						= 71376,
};

/*######
## Rimefang
######*/
struct MANGOS_DLL_DECL miniboss_rimefangAI : public ScriptedAI
{
	miniboss_rimefangAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiFrost_BreathTimer;
	uint32 m_uiIcy_BlastTimer;

	void Reset()
	{
		m_uiFrost_BreathTimer = 13000;
		m_uiIcy_BlastTimer = 10000;

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
	} 

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_FROST_AURA);

		// make land
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && pWho->GetTypeId() == TYPEID_PLAYER && 
			m_creature->IsWithinDist3d(pWho->GetPositionX(), pWho->GetPositionY(), pWho->GetPositionZ(), 60))
			m_creature->SetInCombatWithZone();
	}

	void JustDied(Unit* pKiller)
	{
		// check if splinestalker is alive && summon Sindragosa
		if (Creature* pSplinestalker = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_SPLINESTALKER))))
		{
			if(!pSplinestalker->isAlive())
			{
				if(Creature* pSindragosa = m_creature->SummonCreature(NPC_SINDRAGOSA, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000))
					pSindragosa->SetInCombatWithZone();
			}
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiFrost_BreathTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_FROST_BREATH);
			m_uiFrost_BreathTimer = urand(13000, 16000);
		}
		else m_uiFrost_BreathTimer -= uiDiff;

		if(m_uiIcy_BlastTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ICY_BLAST);
			m_uiIcy_BlastTimer = urand(10000, 15000);
		}
		else m_uiIcy_BlastTimer -= uiDiff;

		DoMeleeAttackIfReady();																					
	}
};

CreatureAI* GetAI_miniboss_rimefang(Creature* pCreature)
{
    return new miniboss_rimefangAI(pCreature);
}

enum
{
    SPELL_BELLOWING_ROAR				= 36922, 
	SPELL_CLEAVE						= 40505,
	SPELL_TAIL_SWEEP_10					= 71369,
	SPELL_TAIL_SWEEP_25					= 71370,
};

/*######
## Spinestalker
######*/
struct MANGOS_DLL_DECL miniboss_spinestalkerAI : public ScriptedAI
{
	miniboss_spinestalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiBellowing_RoarTimer;
	uint32 m_uiCleaveTimer;
	uint32 m_uiTail_SweepTimer;

	void Reset()
	{
		m_uiBellowing_RoarTimer = urand(10000, 20000);
		m_uiCleaveTimer			= urand(4000, 7000);
		m_uiTail_SweepTimer		= urand(8000, 13000);

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
	}

	void Aggro(Unit* pWho)
	{
		// make land
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && pWho->GetTypeId() == TYPEID_PLAYER && 
			m_creature->IsWithinDist3d(pWho->GetPositionX(), pWho->GetPositionY(), pWho->GetPositionZ(), 60))
			m_creature->SetInCombatWithZone();
	}

	void JustDied(Unit* pKiller)
	{
		// check if rimefang is alive && summon Sindragosa
		if (Creature* pRimefang = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RIMEFANG))))
		{
			if(!pRimefang->isAlive())
			{
				if(Creature* pSindragosa = m_creature->SummonCreature(NPC_SINDRAGOSA, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000))
					pSindragosa->SetInCombatWithZone();
			}
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiCleaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
			m_uiCleaveTimer = urand(4000, 7000);
		}
		else m_uiCleaveTimer -= uiDiff;

		if(m_uiBellowing_RoarTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BELLOWING_ROAR);
			m_uiBellowing_RoarTimer = urand(10000, 20000);
		}
		else m_uiBellowing_RoarTimer -= uiDiff;

		if (m_uiTail_SweepTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_TAIL_SWEEP_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_TAIL_SWEEP_25);
			m_uiTail_SweepTimer = urand(8000, 13000);
		}
		else m_uiTail_SweepTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_miniboss_spinestalker(Creature* pCreature)
{
    return new miniboss_spinestalkerAI(pCreature);
}

void AddSC_boss_sindragosa()
{
    Script* NewScript;
	NewScript = new Script;
    NewScript->Name = "boss_sindragosa";
    NewScript->GetAI = &GetAI_boss_sindragosa;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "miniboss_spinestalker";
    NewScript->GetAI = &GetAI_miniboss_spinestalker;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "miniboss_rimefang";
    NewScript->GetAI = &GetAI_miniboss_rimefang;
    NewScript->RegisterSelf();
}