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
SDName: Boss_Lurker_below
SD%Complete: 90
SDComment: 
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"

enum
{
	SPELL_SPOUT					= 42584,
	SPELL_GEYSER				= 37478,
	SPELL_WHIRL					= 37660,
	SPELL_WATERBOLT				= 37138,
	SPELL_SUBMERGE				= 37550,

	NPC_COILFANG_AMBUSHER		= 21865,
	NPC_COILFANG_GUARDIAN		= 21873,

	SPELL_LURKER_SPAWN_TRIGGER  = 54587,
	ACHIEVEMENT_LURKER          = 144,

	PHASE_IDLE					= 0,
	PHASE_SURFACE				= 1,
	PHASE_SUBMERGED				= 2,
};

struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

static LocationsXY AddsLoc[]=
{
	{2.855381f, -459.823914f, -19.182686f},		//MOVE_AMBUSHER_1 X, Y, Z
    {12.45842f, -466.042267f, -19.182686f},		//MOVE_AMBUSHER_2 X, Y, Z
    {51.366653f, -460.836060f, -19.182686f},	//MOVE_AMBUSHER_3 X, Y, Z
    {62.597980f, -457.433044f, -19.182686f},	//MOVE_AMBUSHER_4 X, Y, Z
    {77.607452f, -384.302765f, -19.182686f},	//MOVE_AMBUSHER_5 X, Y, Z
    {63.897900f, -378.984924f, -19.182686f},	//MOVE_AMBUSHER_6 X, Y, Z
    {34.447250f, -387.333618f, -19.182686f},	//MOVE_GUARDIAN_1 X, Y, Z
    {14.388216f, -423.468018f, -19.625271f},	//MOVE_GUARDIAN_2 X, Y, Z
	{42.471519f, -445.115295f, -19.769423f}		//MOVE_GUARDIAN_3 X, Y, Z
};

struct MANGOS_DLL_DECL boss_the_lurker_belowAI : public ScriptedAI
{
    boss_the_lurker_belowAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetSplineFlags(SPLINEFLAG_FLYING);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiPhase;
	uint32 m_uiRangeCheckTimer;
	uint32 m_uiPhaseChangeTimer;

	uint32 m_uiGeyserTimer;
	uint32 m_uiWhirlTimer;
	uint32 m_uiSpoutTimer;
	uint32 m_uiOrientationUpdated;
	uint32 m_uiSpinTimer;
	double m_dSpoutAngle;
	bool m_bIsClockwise;

    void Reset()
    {
		m_uiPhase				= PHASE_IDLE;
		m_uiRangeCheckTimer		= 5000;
		m_uiPhaseChangeTimer	= 120000;

		m_uiGeyserTimer			= urand(15000, 20000);			
		m_uiWhirlTimer			= urand(15000, 30000);
		m_uiSpoutTimer			= 30000;
		m_uiOrientationUpdated	= -1;
		m_dSpoutAngle			= 0;
		m_uiSpinTimer			= 0;
		m_bIsClockwise			= true;

        m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	}

	void JustReachedHome()
	{
		if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
    {
		m_uiPhase = PHASE_SURFACE;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, DONE);
    }

	void SummonAdd(uint32 uiEntry, float fX, float fY, float fZ)
	{
		if(Creature* pTemp = m_creature->SummonCreature(uiEntry, fX, fY, fZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
			pTemp->SetInCombatWithZone();
	}
	
	void UpdateAI(const uint32 uiDiff)
    {
		//Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiPhase == PHASE_SURFACE)
		{
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				m_uiPhase = PHASE_SUBMERGED;
				DoCast(m_creature,SPELL_SUBMERGE);
				m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				// summon adds
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[0].x, AddsLoc[0].y, AddsLoc[0].z);
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[1].x, AddsLoc[1].y, AddsLoc[1].z);
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[2].x, AddsLoc[2].y, AddsLoc[2].z);
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[3].x, AddsLoc[3].y, AddsLoc[3].z);
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[4].x, AddsLoc[4].y, AddsLoc[4].z);
				SummonAdd(NPC_COILFANG_AMBUSHER, AddsLoc[5].x, AddsLoc[5].y, AddsLoc[5].z);
				SummonAdd(NPC_COILFANG_GUARDIAN, AddsLoc[6].x, AddsLoc[6].y, AddsLoc[6].z);
				SummonAdd(NPC_COILFANG_GUARDIAN, AddsLoc[7].x, AddsLoc[7].y, AddsLoc[7].z);
				SummonAdd(NPC_COILFANG_GUARDIAN, AddsLoc[8].x, AddsLoc[8].y, AddsLoc[8].z);
				m_uiPhaseChangeTimer = 60000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;

			if(m_uiSpoutTimer < uiDiff)
			{
				// ToDo: Update this!
				if(m_uiOrientationUpdated == -1)
				{
					m_uiOrientationUpdated = 0;
					if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					{
						m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
						m_dSpoutAngle = pTarget->GetAngle(m_creature) - M_PI;
						m_uiSpoutTimer = 3000;

						switch(rand()%2)
						{
						case 0:
							m_bIsClockwise = true; 
							break;
						case 1: 
							m_bIsClockwise = false; 
							break;
						}
						return;
					}
				}
				else
				{
					if(m_bIsClockwise)
						m_dSpoutAngle += M_PI/30;
					else
						m_dSpoutAngle -= M_PI/30;

					if(m_dSpoutAngle > 2 * M_PI)
						m_dSpoutAngle -= 2 * M_PI;
					if(m_dSpoutAngle < 0)
						m_dSpoutAngle += 2 * M_PI;

					m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
					m_creature->SetOrientation(m_dSpoutAngle);
					m_creature->StopMoving();

					//Spout part
					if(m_uiSpinTimer < uiDiff)
					{
						DoCast(m_creature, SPELL_SPOUT);
						m_uiSpinTimer = 1250;
					}
					m_uiSpinTimer -= uiDiff;
					m_uiOrientationUpdated++;
					m_uiSpoutTimer = 250;

					if (m_uiOrientationUpdated == 60)
					{
						m_uiSpoutTimer = 45000;
						m_uiOrientationUpdated = -1;
						m_uiWhirlTimer = 500;
					}
				}
			}
			else m_uiSpoutTimer -= uiDiff;

			if(m_uiOrientationUpdated != -1)
				return;

			if(m_uiGeyserTimer < uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_GEYSER);
				m_uiGeyserTimer = urand(15000, 20000);
			}
			else m_uiGeyserTimer -= uiDiff;

			if(m_uiWhirlTimer < uiDiff)
			{
				DoCast(m_creature,SPELL_WHIRL);
				m_uiWhirlTimer = urand(15000, 30000);
			}
			else m_uiWhirlTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
		else if(m_uiPhase == PHASE_SUBMERGED)
		{
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				m_uiPhase = PHASE_SURFACE;
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
				m_uiPhaseChangeTimer = 120000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;
		}

		if(m_uiRangeCheckTimer < uiDiff)
		{
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_WATERBOLT);
			}
			m_uiRangeCheckTimer = 1000;
		}
		else m_uiRangeCheckTimer -= uiDiff;
	}
};

CreatureAI* GetAI_boss_the_lurker_below(Creature *pCreature)
{
    return new boss_the_lurker_belowAI (pCreature);
}

// Cast the spell that should summon the Lurker-Below
bool GOHello_go_strange_pool(Player* pPlayer, GameObject* pGo)
{
    // There is some chance to fish The Lurker Below, sources are from 20s to 10minutes, average 5min => 20 tries, hence 5%
    if (urand(0,99) < 5)
    {
        if (ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData())
		{
            if (pInstance->GetData(TYPE_THELURKER_EVENT) == NOT_STARTED)
            {
                pPlayer->CastSpell(pPlayer, SPELL_LURKER_SPAWN_TRIGGER, true);
				if(AchievementEntry const *AchievLurker = GetAchievementStore()->LookupEntry(ACHIEVEMENT_LURKER))
					pPlayer->CompletedAchievement(AchievLurker);
				if(Creature* pLurker = pGo->GetMap()->GetCreature(pInstance->GetData64(NPC_LURKER_BELOW)))
				{
					pLurker->GetMap()->CreatureRelocation(pLurker, pLurker->GetPositionX(), pLurker->GetPositionY(), -21.59f, pLurker->GetOrientation());
					pLurker->SendMonsterMove(pLurker->GetPositionX(), pLurker->GetPositionY(), -21.59f, SPLINETYPE_NORMAL, pLurker->GetSplineFlags(), 1);
					pLurker->AddThreat(pPlayer, 1000.0f);
					pLurker->SetInCombatWith(pPlayer);
					pLurker->AI()->AttackStart(pPlayer);
					pPlayer->SetInCombatWith(pLurker);
				}
                return true;
            }
		}
    }
    return false;
}

void AddSC_boss_the_lurker_below()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name="boss_the_lurker_below";
    pNewScript->GetAI = GetAI_boss_the_lurker_below;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "go_strange_pool";
    pNewScript->pGOHello = &GOHello_go_strange_pool;
    pNewScript->RegisterSelf();
}