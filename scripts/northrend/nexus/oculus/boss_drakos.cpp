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

enum
{
	// yells
	SAY_AGGRO                                    = -1578000,
	SAY_SLAY1                                    = -1578001,
	SAY_SLAY2                                    = -1578002,
	SAY_SLAY3                                    = -1578003,
	SAY_DEATH                                    = -1578004,
	SAY_VAROS_INTRO                              = -1578029,
	SAY_PULL1                                    = -1578005,
	SAY_PULL2                                    = -1578006,
	SAY_PULL3                                    = -1578007,
	SAY_PULL4                                    = -1578008,
	SAY_STOMP1                                   = -1578009,
	SAY_STOMP2                                   = -1578010,
	SAY_STOMP3                                   = -1578011,

	// spells
	SPELL_MAGIC_PULL                              = 51336,
	SPELL_MAGIC_PULL_EFFECT                       = 50770,
	SPELL_THUNDERING_STOMP                        = 50774,
	SPELL_THUNDERING_STOMP_H                      = 59370,
	SPELL_UNSTABLE_SPHERE_PASSIVE                 = 50756,
	SPELL_UNSTABLE_SPHERE_PULSE                   = 50757,
	SPELL_UNSTABLE_SPHERE_TRIGG                   = 50758,
	NPC_UNSTABLE_SPHERE                           = 28166,
};

struct boss_drakosAI : public ScriptedAI
{
	boss_drakosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 uiMagicPullTimer;
	uint32 uiStompTimer;
	uint32 uiBombSummonTimer;

	void Reset()
	{
		uiMagicPullTimer = 15000;
		uiStompTimer = 17000;
		uiBombSummonTimer = 2000;
	}

	void JustReachedHome()
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_DRAKOS, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);

		if (m_pInstance)
			m_pInstance->SetData(TYPE_DRAKOS, IN_PROGRESS);
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_DEATH, m_creature);

		if(Creature* pVaros = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VAROS)))
			DoScriptText(SAY_VAROS_INTRO, pVaros);

		if (m_pInstance)
			m_pInstance->SetData(TYPE_DRAKOS, DONE);

		// open cages
	}

	void KilledUnit(Unit* pVictim)
	{
		switch(urand(0, 3))
		{
		case 0: DoScriptText(SAY_SLAY1, m_creature); break;
		case 1: DoScriptText(SAY_SLAY2, m_creature); break;
		case 2: DoScriptText(SAY_SLAY3, m_creature); break;
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (uiBombSummonTimer <= uiDiff)
		{
			Position pPosition;
			//m_creature->GetPosition(&pPosition);

			//if (bPostPull)
			{
				for (uint8 uiI = 0; uiI >= 3; uiI++)
				{
					//m_creature->GetRandomNearPosition(pPosition, float(urand(0,10)));
					//m_creature->SummonCreature(NPC_UNSTABLE_SPHERE, pPosition);
				}
			}
			//else
			{
				//m_creature->GetRandomNearPosition(pPosition, float(urand(0,10)));
				//m_creature->SummonCreature(NPC_UNSTABLE_SPHERE, pPosition);
			}

			uiBombSummonTimer = 2000;
		} else uiBombSummonTimer -= uiDiff;

		if (uiMagicPullTimer <= uiDiff)
		{
			//DoCast(SPELL_MAGIC_PULL);

			uiMagicPullTimer = 15000;
		} else uiMagicPullTimer -= uiDiff;

		if (uiStompTimer <= uiDiff)
		{
			//DoScriptText(RAND(SAY_STOMP_1,SAY_STOMP_2,SAY_STOMP_3), me);
			//DoCast(SPELL_THUNDERING_STOMP);
			uiStompTimer = 17000;
		} else uiStompTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

struct npc_unstable_sphereAI : public ScriptedAI
{
	npc_unstable_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 uiPulseTimer;
	uint32 uiDeathTimer;

	void Reset()
	{
		DoCast(m_creature, SPELL_UNSTABLE_SPHERE_TRIGG);
		m_creature->GetMotionMaster()->MoveConfused();
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_DRAKOS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
	}
};

CreatureAI* GetAI_boss_drakos(Creature* pCreature)
{
	return new boss_drakosAI (pCreature);
}

CreatureAI* GetAI_npc_unstable_sphere(Creature* pCreature)
{
	return new npc_unstable_sphereAI (pCreature);
}

void AddSC_boss_drakos()
{
	Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_drakos";
    newscript->GetAI = &GetAI_boss_drakos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_unstable_sphere";
    newscript->GetAI = &GetAI_npc_unstable_sphere;
    newscript->RegisterSelf();
}
