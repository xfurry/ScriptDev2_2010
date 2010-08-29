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

	SPELL_UNSTABLE_SPHERE_TRIGG                   = 50758,
	SPELL_UNSTABLE_SPHERE_VISUAL				  = 50756,
	NPC_UNSTABLE_SPHERE                           = 28166,
};

struct MANGOS_DLL_DECL boss_drakosAI : public ScriptedAI
{
	boss_drakosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiMagicPullTimer;
	uint32 m_uiMagicPullExpireTimer;
	uint32 m_uiStompTimer;
	uint32 m_uiBombSummonTimer;

	void Reset()
	{
		m_uiMagicPullTimer	= 15000;
		m_uiMagicPullExpireTimer = 30000;
		m_uiStompTimer		= 17000;
		m_uiBombSummonTimer = 2000;
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
		if(Creature* pEternos = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ETERNOS)))
		{
			pEternos->GetMotionMaster()->MovePoint(0, 944.384f, 1058.418f, 359.967f);
			if(GameObject* pDoor = GetClosestGameObjectWithEntry(pEternos, GO_DRAGON_CAGE_DOOR, 5.0f))
				pDoor->SetGoState(GO_STATE_ACTIVE);
		}
		if(Creature* pVerdisa = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VERDISA)))
		{
			pVerdisa->GetMotionMaster()->MovePoint(0, 949.928f, 1034.753f, 359.967f);
			if(GameObject* pDoor = GetClosestGameObjectWithEntry(pVerdisa, GO_DRAGON_CAGE_DOOR, 5.0f))
				pDoor->SetGoState(GO_STATE_ACTIVE);
		}
		if(Creature* pBelgaristrasz = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BELGARISTRASZ)))
		{
			pBelgaristrasz->GetMotionMaster()->MovePoint(0, 944.868f, 1044.982f, 359.967f);
			if(GameObject* pDoor = GetClosestGameObjectWithEntry(pBelgaristrasz, GO_DRAGON_CAGE_DOOR, 5.0f))
				pDoor->SetGoState(GO_STATE_ACTIVE);
		}
	}

	void KilledUnit(Unit* pVictim)
	{
		switch(urand(0, 2))
		{
		case 0: DoScriptText(SAY_SLAY1, m_creature); break;
		case 1: DoScriptText(SAY_SLAY2, m_creature); break;
		case 2: DoScriptText(SAY_SLAY3, m_creature); break;
		}
	}

	void TeleportPlayers()
    {
        Map* pMap = m_creature->GetMap();
        if(pMap)
        {
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            if (!lPlayers.isEmpty())
            {
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
					if (Player* pPlayer = itr->getSource())
					{
						if(m_creature->IsWithinDist2d(pPlayer->GetPositionX(), pPlayer->GetPositionY(), 20))
							pPlayer->TeleportTo(m_creature->GetMapId(), m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), pPlayer->GetOrientation());
					}
                }
            }
        }
    }

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBombSummonTimer <= uiDiff)
		{
			for (uint8 uiI = 0; uiI < 2; uiI++)
			{
				if(Creature* pBomb = m_creature->SummonCreature(NPC_UNSTABLE_SPHERE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 19000))
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + urand(20, 30)*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + urand(20, 30)*sin(angle*(M_PI/180));
					pBomb->GetMotionMaster()->MovePoint(0, homeX, homeY, m_creature->GetPositionZ());
				}
			}
			m_uiBombSummonTimer = 2000;
		} else m_uiBombSummonTimer -= uiDiff;

		if (m_uiMagicPullTimer <= uiDiff)
		{
			DoCast(m_creature, SPELL_MAGIC_PULL);
			m_uiMagicPullExpireTimer = 2000;
			m_uiMagicPullTimer = 15000;
		} else m_uiMagicPullTimer -= uiDiff;

		if(m_uiMagicPullExpireTimer < uiDiff)
		{
			TeleportPlayers();
			m_uiMagicPullExpireTimer = 30000;
		}
		else m_uiMagicPullExpireTimer -= uiDiff;

		if (m_uiStompTimer <= uiDiff)
		{
			switch(urand(0, 2))
			{
			case 0: DoScriptText(SAY_STOMP1, m_creature); break;
			case 1: DoScriptText(SAY_STOMP2, m_creature); break;
			case 2: DoScriptText(SAY_STOMP3, m_creature); break;
			}
			DoCast(m_creature, m_bIsRegularMode ? SPELL_THUNDERING_STOMP : SPELL_THUNDERING_STOMP_H);
			m_uiStompTimer = 17000;
		} else m_uiStompTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

struct MANGOS_DLL_DECL npc_unstable_sphereAI : public ScriptedAI
{
	npc_unstable_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pCreature->setFaction(14);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiPulseTimer;

	void Reset()
	{
		DoCast(m_creature, SPELL_UNSTABLE_SPHERE_VISUAL);
		m_uiPulseTimer = 16000;
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

		if(m_uiPulseTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_UNSTABLE_SPHERE_TRIGG);
			m_uiPulseTimer = 20000;
		}
		else m_uiPulseTimer -= uiDiff;
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
