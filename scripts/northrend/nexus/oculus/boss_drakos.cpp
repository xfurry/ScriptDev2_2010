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

#include "precompiled.h"
#include "oculus.h"

enum Spells
{
	SPELL_MAGIC_PULL                              = 51336,
	SPELL_MAGIC_PULL_EFFECT                       = 50770,
	SPELL_THUNDERING_STOMP                        = 50774,
	SPELL_THUNDERING_STOMP_H                      = 59370,
	SPELL_UNSTABLE_SPHERE_PASSIVE                 = 50756,
	SPELL_UNSTABLE_SPHERE_PULSE                   = 50757,
	SPELL_UNSTABLE_SPHERE_TIMER                   = 50758,
	NPC_UNSTABLE_SPHERE                           = 28166,
};

//not in db
enum Yells
{
	SAY_AGGRO                                     = -1578000,
	SAY_KILL_1                                    = -1578001,
	SAY_KILL_2                                    = -1578002,
	SAY_KILL_3                                    = -1578003,
	SAY_DEATH                                     = -1578004,
	SAY_PULL_1                                    = -1578005,
	SAY_PULL_2                                    = -1578006,
	SAY_PULL_3                                    = -1578007,
	SAY_PULL_4                                    = -1578008,
	SAY_STOMP_1                                   = -1578009,
	SAY_STOMP_2                                   = -1578010,
	SAY_STOMP_3                                   = -1578011
};

enum
{
	ACHIEV_TIMED_START_EVENT                      = 18153,
};
/*
struct boss_drakosAI : public ScriptedAI
{
	boss_drakosAI(Creature* pCreature) : ScriptedAI(pCreature), lSummons(me)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	uint32 uiMagicPullTimer;
	uint32 uiStompTimer;
	uint32 uiBombSummonTimer;

	bool bPostPull;

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	void Reset()
	{
		lSummons.clear();
		uiMagicPullTimer = 15000;
		uiStompTimer = 17000;
		uiBombSummonTimer = 2000;

		bPostPull = false;

		if (m_pInstance)
			m_pInstance->SetData(DATA_DRAKOS_EVENT, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, me);

		if (m_pInstance)
			m_pInstance->SetData(DATA_DRAKOS_EVENT, IN_PROGRESS);
	}

	void JustSummoned(Creature* pSummon)
	{
		lSummons.Summon(pSummon);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		//Return since we have no target
		if (!UpdateVictim())
			return;

		if (uiBombSummonTimer <= uiDiff)
		{
			Position pPosition;
			m_creature->GetPosition(&pPosition);

			if (bPostPull)
			{
				for (uint8 uiI = 0; uiI >= 3; uiI++)
				{
					//m_creature->GetRandomNearPosition(pPosition, float(urand(0,10)));
					//m_creature->SummonCreature(NPC_UNSTABLE_SPHERE, pPosition);
				}
			}
			else
			{
				//m_creature->GetRandomNearPosition(pPosition, float(urand(0,10)));
				//m_creature->SummonCreature(NPC_UNSTABLE_SPHERE, pPosition);
			}

			uiBombSummonTimer = 2000;
		} else uiBombSummonTimer -= uiDiff;

		if (uiMagicPullTimer <= uiDiff)
		{
			DoCast(SPELL_MAGIC_PULL);

			bPostPull = true;

			uiMagicPullTimer = 15000;
		} else uiMagicPullTimer -= uiDiff;

		if (uiStompTimer <= uiDiff)
		{
			DoScriptText(RAND(SAY_STOMP_1,SAY_STOMP_2,SAY_STOMP_3), me);
			DoCast(SPELL_THUNDERING_STOMP);
			uiStompTimer = 17000;
		} else uiStompTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_DEATH, me);

		if (m_pInstance)
		{
			m_pInstance->SetData(DATA_DRAKOS_EVENT, DONE);
			// start achievement timer (kill Eregos within 20 min)
			//m_pInstance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
		}

		lSummons.DespawnAll();
	}
	void KilledUnit(Unit* pVictim)
	{
		DoScriptText(RAND(SAY_KILL_1,SAY_KILL_2,SAY_KILL_3), me);
	}
};



struct npc_unstable_sphereAI : public ScriptedAI
{
	npc_unstable_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) {}

	uint32 uiPulseTimer;
	uint32 uiDeathTimer;

	void Reset()
	{
		//m_creature->SetReactState(REACT_PASSIVE);
		m_creature->GetMotionMaster()->MoveRandom(40.0f);

		//m_creature->AddAura(SPELL_UNSTABLE_SPHERE_PASSIVE, me);
		//m_creature->AddAura(SPELL_UNSTABLE_SPHERE_TIMER, me);

		uiPulseTimer = 3000;
		uiDeathTimer = 19000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (uiPulseTimer <= uiDiff)
		{
			DoCast(SPELL_UNSTABLE_SPHERE_PULSE);
			uiPulseTimer = 3*IN_MILLISECONDS;
		} else uiPulseTimer -= uiDiff;

		if (uiDeathTimer <= uiDiff)
		{}//m_creature->DisappearAndDie();
		else uiDeathTimer -= uiDiff;
	}
};

CreatureAI* GetAI(Creature* pCreature) const
{
	return new boss_drakosAI (pCreature);
}

CreatureAI* GetAI(Creature* pCreature) const
{
	return new npc_unstable_sphereAI (pCreature);
}

void AddSC_boss_drakos()
{
	new boss_drakos();
	new npc_unstable_sphere();
}
*/