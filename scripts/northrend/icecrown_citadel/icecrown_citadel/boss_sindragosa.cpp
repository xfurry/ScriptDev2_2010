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
	SPELL_ICETOMB_AURA		= 70157,
	SPELL_ICETOMB_DUMMY		= 69675,
	SPELL_ICETOMB_DMG		= 71330,	// not used
	NPC_ICETOMB				= 36980,
	SPELL_AXPHYXIATION		= 71665,
	SPELL_FROSTBOMB			= 71053,
	SPELL_FROSTBOMB_SUMMON	= 69846,
	SPELL_MYSTICBUFFET		= 70128,
	NPC_FROST_BOMB			= 37186,

	SPELL_BERSERK			= 26662,

	PHASE_IDLE				= 0,
	PHASE_ONE				= 1,
    PHASE_AIR               = 2,
    PHASE_THREE             = 3,
};

const float posSindragosa[4] = {4453.670f, 2484.251f, 240.797f, 3.15f};

struct MANGOS_DLL_DECL boss_sindragosaAI : public ScriptedAI
{
	boss_sindragosaAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_bStartAttack = false;
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32	Difficulty;

	uint8 m_uiPhase;
	uint8 m_uiBombCount;
	uint8 m_uiFrostBreathCount;
	
	// phase 1 & 3
	uint32 m_uiCleaveTimer;
	uint32 m_uiTailSmashTimer;
	uint32 m_uiFrostBreathTimer;
	uint32 m_uiIcyGripTimer;
	uint32 m_uiMagicTimer;
	uint32 m_uiBlisteringColdTimer;
	bool m_bHasBuffet;
	// phase 2
	uint32 m_uiAirPhaseTimer;
	uint32 m_uiFrostBeaconTimer;
	uint32 m_uiIceTombTimer;
	uint32 m_uiBombTimer;

	// general
	uint32 m_uiBerserkTimer;
	bool m_bStartAttack;
	uint32 m_uiMaxTargets;
    uint32 m_uiAttackStartTimer;
	std::list<uint64> m_lTargetsGUIDList;
	uint64 m_uiFrostTargetGUID;

	void Reset()
	{
		m_uiPhase				= PHASE_IDLE;
		m_uiBerserkTimer		= 600000;	// 10 min
		m_uiAirPhaseTimer		= 100;
		m_uiBombCount			= 0;
		m_uiIcyGripTimer		= 30000;
		m_uiBlisteringColdTimer	= 30500;
		m_uiFrostBeaconTimer	= 3000;
		m_uiBombTimer			= 60000;

		m_bHasBuffet			= false;
		m_uiMagicTimer			= 30000;
		m_uiCleaveTimer			= urand(4000, 7000);
		m_uiTailSmashTimer		= urand(8000, 13000);
		m_uiFrostBreathTimer	= urand(13000, 16000);
        m_uiAttackStartTimer    = 5000;
		m_lTargetsGUIDList.clear();
		m_uiFrostTargetGUID		= 0;

		m_uiMaxTargets = (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL) ? 2 : 5;

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
		m_creature->SetRespawnDelay(DAY);
	} 

	void Aggro(Unit* pWho)
	{
		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
			DoCast(m_creature, SPELL_FROSTAURA_10);
		if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			DoCast(m_creature, SPELL_FROSTAURA_25);

		DoScriptText(SAY_AGGRO, m_creature);
		if(m_pInstance) 
			m_pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);

		m_uiPhase = PHASE_ONE;

		// make land
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

	void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
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
		if(m_pInstance) 
			m_pInstance->SetData(TYPE_SINDRAGOSA, NOT_STARTED);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            //m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else m_uiAttackStartTimer -= uiDiff;

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(!m_creature->HasAura(SPELL_PERMACHILL))
			DoCast(m_creature, SPELL_PERMACHILL);
		
		switch (m_uiPhase)
        {
			case PHASE_THREE:
			{
				//mystic buffet
				if(!m_bHasBuffet)
				{
					m_creature->InterruptNonMeleeSpells(true);
					DoCast(m_creature, SPELL_MYSTICBUFFET);
					m_bHasBuffet = true;
				}
				
				//frost beacon
				if(m_uiFrostBeaconTimer < uiDiff)
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
					{
						DoCast(pTarget, SPELL_FROSTBEACON);
						m_uiFrostTargetGUID = pTarget->GetGUID();
					}
					m_uiIceTombTimer		= 5000;
					m_uiFrostBeaconTimer	= 10000;
				}
				else m_uiFrostBeaconTimer -=uiDiff;

				//ice tomb
				if(m_uiIceTombTimer < uiDiff)
				{
					if(Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiFrostTargetGUID))
					{
						DoCast(pTarget, SPELL_ICETOMB_DUMMY);
						if(Creature* pTomb = m_creature->SummonCreature(NPC_ICETOMB, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
							pTomb->AddThreat(pTarget, 1000.0f);
					}
					m_uiIceTombTimer = 30000;
				}
				else m_uiIceTombTimer -=uiDiff;

				// no break, phase 3 will use same abilities as in 1
			}
			case PHASE_ONE:
			{
				// start phase 3
				if(m_creature->GetHealthPercent() < 35.0f && m_uiPhase == PHASE_ONE)
				{
					DoScriptText(SAY_PHASE3, m_creature);
					m_uiPhase = PHASE_THREE;
				}

				// fly timer
				if(m_creature->GetHealthPercent() < 85.0f && m_uiPhase == PHASE_ONE)
				{
					if(m_uiAirPhaseTimer < uiDiff)
					{
						DoScriptText(SAY_TAKEOFF, m_creature);
						SetCombatMovement(false);
						m_uiPhase = PHASE_AIR;
						m_creature->GetMotionMaster()->MovePoint(0, posSindragosa[0], posSindragosa[1], posSindragosa[2]);
						m_creature->GetMap()->CreatureRelocation(m_creature, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3]);
						m_creature->SendMonsterMove(posSindragosa[0], posSindragosa[1], posSindragosa[2], SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
						m_creature->GetMotionMaster()->MoveIdle();
						// make fly
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
						m_uiFrostBeaconTimer = 1000;
						m_uiIceTombTimer = 60000;
						m_uiAirPhaseTimer = 600000;
					}
					else m_uiAirPhaseTimer -= uiDiff;
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

				if(m_uiMagicTimer < uiDiff)
				{
					m_creature->InterruptNonMeleeSpells(true);
					DoScriptText(SAY_UNCHAINED_MAGIC, m_creature);
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
						DoCast(pTarget, SPELL_UNCHAINMAGIC);
					m_uiMagicTimer = 30000;
				}
				else m_uiMagicTimer -= uiDiff;

				//frost breath
				if (m_uiFrostBreathTimer < uiDiff)
				{
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
						DoCast(m_creature, SPELL_FROSTBREATH_10);
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
						DoCast(m_creature, SPELL_FROSTBREATH_25);
					m_uiFrostBreathTimer = urand(13000, 16000);
				}
				else m_uiFrostBreathTimer -= uiDiff;

				// icy grip
				if(m_uiIcyGripTimer < uiDiff)
				{
					m_creature->InterruptNonMeleeSpells(true);
					DoCast(m_creature, SPELL_ICYGRIP);
					m_uiIcyGripTimer = 67000;
					m_uiBlisteringColdTimer = 500;
				}
				else m_uiIcyGripTimer -= uiDiff;

				// blistering cold just after the icy grip
				if (m_uiBlisteringColdTimer < uiDiff)
				{
					m_creature->InterruptNonMeleeSpells(true);
					DoScriptText(SAY_BLISTERING_COLD, m_creature);
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
						DoCast(m_creature, SPELL_BLISTERINGCOLD_10);
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
						DoCast(m_creature, SPELL_BLISTERINGCOLD_25);
					m_uiBlisteringColdTimer = 67000;
				}
				else m_uiBlisteringColdTimer -= uiDiff;

				DoMeleeAttackIfReady();

				break;
			}
			case PHASE_AIR:
			{
				//frost beacon
				if(m_uiFrostBeaconTimer < uiDiff)
				{
					for(uint8 i = 0; i < m_uiMaxTargets; i++)
					{
						if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
						{
							DoCast(pTarget, SPELL_FROSTBEACON);
							m_lTargetsGUIDList.push_back(pTarget->GetGUID());
						}
					}
					m_uiFrostBeaconTimer = 60000; // no more beacon this phase
					m_uiIceTombTimer = 5000;
				}
				else m_uiFrostBeaconTimer -=uiDiff;

				//ice tomb
				if(m_uiIceTombTimer < uiDiff)
				{
					if (!m_lTargetsGUIDList.empty())
					{
						for(std::list<uint64>::iterator itr = m_lTargetsGUIDList.begin(); itr != m_lTargetsGUIDList.end(); ++itr)
						{
							if (Unit* pTarget = m_creature->GetMap()->GetCreature(*itr))
							{
								if (pTarget->isAlive())
								{
									DoCast(pTarget, SPELL_ICETOMB_DUMMY);
									if(Creature* pTomb = m_creature->SummonCreature(NPC_ICETOMB, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
										pTomb->AddThreat(pTarget, 1000.0f);
								}
							}
						}
					}
					m_uiBombTimer		= 10000;
					m_uiIceTombTimer	= 60000; //no more ice tomb this phase
				}
				else m_uiIceTombTimer -=uiDiff;

				//frost bomb x4
				if(m_uiBombTimer < uiDiff)
				{
					if(m_uiBombCount > 4)
					{
						m_uiBombCount = 0;
						m_uiPhase = PHASE_ONE;
						SetCombatMovement(true);
						m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
						// reset timers
						m_uiAirPhaseTimer		= 60000; //60 seconds to next air phase
						m_uiCleaveTimer			= urand(4000, 7000);
						m_uiTailSmashTimer		= urand(8000, 13000);
						m_uiFrostBreathTimer	= urand(13000, 16000);
						m_uiIcyGripTimer		= 35000;
						m_uiMagicTimer			= 10000;
						m_uiBlisteringColdTimer	= 35500;

						// make land
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
						m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
						m_lTargetsGUIDList.clear();
					}
					else
					{
						//drop a bomb
						if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
							DoCast(pTarget, SPELL_FROSTBOMB_SUMMON);
						m_uiBombCount += 1;
						m_uiBombTimer = 10000; //drop another bomb after 10 seconds
					}
				}
				else m_uiBombTimer -=uiDiff;
				break;
			}
		}
		
		if(m_uiBerserkTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_BERSERK, m_creature);
			DoCast(m_creature, SPELL_BERSERK);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;
	}
};

CreatureAI* GetAI_boss_sindragosa(Creature* pCreature)
{
    return new boss_sindragosaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ice_tombAI : public ScriptedAI
{
    mob_ice_tombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint64 m_uiVictimGUID;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        DoCast(pWho, SPELL_ICETOMB_AURA);
		pWho->CastSpell(pWho, SPELL_AXPHYXIATION, false);
		pWho->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_uiVictimGUID = pWho->GetGUID();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimGUID))
				{
                    pVictim->RemoveAurasDueToSpell(SPELL_ICETOMB_AURA);
					pVictim->RemoveAurasDueToSpell(SPELL_AXPHYXIATION);
					pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				}
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
		{
            pVictim->RemoveAurasDueToSpell(SPELL_ICETOMB_AURA);
			pVictim->RemoveAurasDueToSpell(SPELL_AXPHYXIATION);
			pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}
    }

    void JustDied(Unit* pKiller)
    {
        if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimGUID))
		{
            pVictim->RemoveAurasDueToSpell(SPELL_ICETOMB_AURA);
			pVictim->RemoveAurasDueToSpell(SPELL_AXPHYXIATION);
			pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}

        if (pKiller)
		{
            pKiller->RemoveAurasDueToSpell(SPELL_ICETOMB_AURA);
			pKiller->RemoveAurasDueToSpell(SPELL_AXPHYXIATION);
			pKiller->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_SINDRAGOSA) != IN_PROGRESS)
        {
            if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimGUID))
			{
                pVictim->RemoveAurasDueToSpell(SPELL_ICETOMB_AURA);
				pVictim->RemoveAurasDueToSpell(SPELL_AXPHYXIATION);
				pVictim->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			}
            m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_ice_tomb(Creature* pCreature)
{
    return new mob_ice_tombAI(pCreature);
}

// frost bomb
struct MANGOS_DLL_DECL mob_icc_frost_bombAI : public ScriptedAI
{
	mob_icc_frost_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
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

	uint32 m_uiDieTimer;
	bool m_bMustDie;
	bool m_bSetPath;

	void Reset()
	{
	    m_bMustDie = false;
		m_bSetPath = false;
	}

	void MovementInform(uint32 uiType, uint32 uiPointId)
	{
		if(uiType != POINT_MOTION_TYPE)
			return;

		if(uiPointId == 1)
		{
			DoCast(m_creature, SPELL_FROSTBOMB);
			m_uiDieTimer = 500;
			m_bMustDie = true;
		}
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_SINDRAGOSA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(!m_bSetPath)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = 4408.099f + urand(25, 30)*cos(angle*(M_PI/180));
			float homeY = 2484.570f + urand(25, 30)*sin(angle*(M_PI/180));
			float homeZ = 203.374f;
			m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
			m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
			m_creature->GetMotionMaster()->MovePoint(1, homeX, homeY, homeZ);
			m_bSetPath = true;
		}

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;
	}

};

CreatureAI* GetAI_mob_icc_frost_bomb(Creature* pCreature)
{
    return new mob_icc_frost_bombAI(pCreature);
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
		m_creature->SetRespawnDelay(DAY);
		m_uiFrost_BreathTimer = 13000;
		m_uiIcy_BlastTimer = 10000;

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
	} 

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_FROST_AURA);
		if (Creature* pSplinestalker = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_SPLINESTALKER)))
			pSplinestalker->AI()->AttackStart(pWho);

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
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_SINDRAGOSA) == DONE)
				return;
		}

		// check if splinestalker is alive && summon Sindragosa
		if (Creature* pSplinestalker = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_SPLINESTALKER)))
		{
			if(!pSplinestalker->isAlive())
			{
				if(Creature* pSindragosa = m_creature->SummonCreature(NPC_SINDRAGOSA, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000))
				{
					pSindragosa->GetMap()->CreatureRelocation(pSindragosa, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3]);
					pSindragosa->SendMonsterMove(posSindragosa[0], posSindragosa[1], posSindragosa[2], SPLINETYPE_NORMAL, pSindragosa->GetSplineFlags(), 1);
				}
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
		m_creature->SetRespawnDelay(DAY);
		m_uiBellowing_RoarTimer = urand(10000, 20000);
		m_uiCleaveTimer			= urand(4000, 7000);
		m_uiTail_SweepTimer		= urand(8000, 13000);

		// make fly
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
	}

	void Aggro(Unit* pWho)
	{
		if (Creature* pRimefang = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIMEFANG)))
			pRimefang->AI()->AttackStart(pWho);
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
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_SINDRAGOSA) == DONE)
				return;
		}

		// check if rimefang is alive && summon Sindragosa
		if (Creature* pRimefang = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIMEFANG)))
		{
			if(!pRimefang->isAlive())
			{
				if(Creature* pSindragosa = m_creature->SummonCreature(NPC_SINDRAGOSA, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000))
				{
					pSindragosa->GetMap()->CreatureRelocation(pSindragosa, posSindragosa[0], posSindragosa[1], posSindragosa[2], posSindragosa[3]);
					pSindragosa->SendMonsterMove(posSindragosa[0], posSindragosa[1], posSindragosa[2], SPLINETYPE_NORMAL, pSindragosa->GetSplineFlags(), 1);
				}
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

	NewScript = new Script;
    NewScript->Name = "mob_icc_ice_tomb";
    NewScript->GetAI = &GetAI_mob_ice_tomb;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_icc_frost_bomb";
    NewScript->GetAI = &GetAI_mob_icc_frost_bomb;
    NewScript->RegisterSelf();
}