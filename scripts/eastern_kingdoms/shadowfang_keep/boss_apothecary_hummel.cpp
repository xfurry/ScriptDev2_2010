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
SDName: boss_apothecary_hummel
SD%Complete: 0
SDComment: 
SDCategory: Apothecary Hummel
EndScriptData */

#include "precompiled.h"
#include "shadowfang_keep.h"

enum
{
    SPELL_CHAIN_REACTION		= 68821,	// by hummel and baxter
	SPELL_COLOGNE_SPRAY			= 68948,	// by baxter
	SPELL_PERFUME_SPRAY			= 68607,	// by hummel
	SPELL_IRRESISTIBLE_COLOGNE	= 68946,	// by baxter - aura
	SPELL_ALLURING_PERFUME		= 68589,	// by hummel - aura

	SPELL_PERFUME_SPILL			= 68798,	// by frye
	SPELL_PERFUME_SPILL_DMG		= 68927,	// by frye
	SPELL_COLOGNE_SPILL			= 68614,	// by frye
	SPELL_COLOGNE_SPILL_DMG		= 68934,	// by frye

	SPELL_UNSTABLE_REACTION		= 68957,	// by adds

	// npc
	NCP_CRAZED_APOTHECARY		= 36568,	// this is the right one
	NPC_CROWN_APOTHECARY		= 36885,

	QUEST_BEEN_SERVED			= 14488,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

static LocationsXY PositionLoc[]=
{
    {-238.335f, 2164.789f, 89.714f},
    {-167.565f, 2219.466f, 80.964f},
    {-214.742f, 2239.666f, 79.765f},
};

struct MANGOS_DLL_DECL boss_apothecary_baxterAI : public ScriptedAI
{
    boss_apothecary_baxterAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiSprayTimer;
	uint32 m_uiReactionTimer;
	uint32 m_uiSummonTimer;
	bool m_bSummon;

    void Reset()
    {
		m_uiSprayTimer		= 7000;
		m_uiReactionTimer	= 15000;
		m_uiSummonTimer		= 5000;
		m_bSummon			= false;
		DoCast(m_creature, SPELL_IRRESISTIBLE_COLOGNE);
		m_creature->setFaction(35);
    }

	void JustReachedHome()
	{
		if(Creature* pFrye = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FRYE)))
		{
			if(!pFrye->isAlive())
				pFrye->Respawn();
		}
		if(Creature* pHummel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HUMMEL)))
		{
			if(!pHummel->isAlive())
				pHummel->Respawn();
		}
	}

    void JustDied(Unit* pKiller)
    {
		bool bAddsDead = true;
		if(Creature* pFrye = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FRYE)))
		{
			if(pFrye->isAlive())
				bAddsDead = false;
		}

		if(!bAddsDead)
			return;

		if(Creature* pHummel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HUMMEL)))
		{
			if(!pHummel->isAlive())
				pHummel->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
		}
    }

	void JustSummoned(Creature* pSummon)
	{
		pSummon->setFaction(14);
		pSummon->AI()->AttackStart(m_creature->getVictim());
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSprayTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_COLOGNE_SPRAY);
			m_uiSprayTimer = urand(5000, 7000);
		}
		else m_uiSprayTimer -= uiDiff;

		if(m_uiReactionTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_CHAIN_REACTION);
			m_uiReactionTimer = urand(10000, 13000);
		}
		else m_uiReactionTimer -= uiDiff;

		if(m_uiSummonTimer < uiDiff && m_bSummon)
		{
			for(uint8 i = 0; i < 3; i++)
				m_creature->SummonCreature(NCP_CRAZED_APOTHECARY, PositionLoc[i].x, PositionLoc[i].y, PositionLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
			m_uiSummonTimer = 10000;
		}
		else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_apothecary_baxter(Creature* pCreature)
{
    return new boss_apothecary_baxterAI(pCreature);
}

struct MANGOS_DLL_DECL boss_apothecary_fryeAI : public ScriptedAI
{
    boss_apothecary_fryeAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiSpillTimer;
	uint32 m_uiSummonTimer;
	bool m_bSummon;

    void Reset()
    {
		m_creature->setFaction(35);
		m_uiSpillTimer = 5000;
		m_uiSummonTimer	= 5000;
		m_bSummon		= false;
    }

	void JustReachedHome()
	{
		if(Creature* pBaxter = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BAXTER)))
		{
			if(!pBaxter->isAlive())
				pBaxter->Respawn();
		}
		if(Creature* pHummel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HUMMEL)))
		{
			if(!pHummel->isAlive())
				pHummel->Respawn();
		}
	}

	void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

	void JustSummoned(Creature* pSummon)
	{
		pSummon->setFaction(14);
		pSummon->AI()->AttackStart(m_creature->getVictim());
	}

    void JustDied(Unit* pKiller)
    {
		bool bAddsDead = true;
		if(Creature* pBaxter = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BAXTER)))
		{
			if(pBaxter->isAlive())
			{
				((boss_apothecary_baxterAI*)pBaxter->AI())->m_bSummon = true;
				bAddsDead = false;
			}
		}

		if(!bAddsDead)
			return;

		if(Creature* pHummel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HUMMEL)))
		{
			if(!pHummel->isAlive())
				pHummel->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSpillTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				switch(urand(0, 1))
				{
				case 0:
					DoCast(pTarget, SPELL_PERFUME_SPILL);
					break;
				case 1:
					DoCast(pTarget, SPELL_COLOGNE_SPILL);
					break;
				}
			}
			m_uiSpillTimer = 10000;
		}
		else m_uiSpillTimer -= uiDiff;

		if(m_uiSummonTimer < uiDiff && m_bSummon)
		{
			for(uint8 i = 0; i < 3; i++)
				m_creature->SummonCreature(NCP_CRAZED_APOTHECARY, PositionLoc[i].x, PositionLoc[i].y, PositionLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
			m_uiSummonTimer = 10000;
		}
		else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_apothecary_frye(Creature* pCreature)
{
    return new boss_apothecary_fryeAI(pCreature);
}

struct MANGOS_DLL_DECL boss_apothecary_hummelAI : public ScriptedAI
{
    boss_apothecary_hummelAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiBaxterTimer;
	uint32 m_uiFryeTimer;
	bool m_bBaxterAggro;
	bool m_bFryeAggro;

	uint32 m_uiSprayTimer;
	uint32 m_uiReactionTimer;

	uint32 m_uiSummonTimer;

    void Reset()
    {
		m_uiBaxterTimer		= 10000;
		m_uiFryeTimer		= 20000;
		m_bBaxterAggro		= false;
		m_bFryeAggro		= false;

		m_uiSprayTimer		= 7000;
		m_uiReactionTimer	= 15000;

		m_uiSummonTimer		= 5000;
		DoCast(m_creature, SPELL_ALLURING_PERFUME);
		m_creature->setFaction(35);
    }

	void JustReachedHome()
	{
		if(Creature* pBaxter = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BAXTER)))
		{
			if(!pBaxter->isAlive())
				pBaxter->Respawn();
		}
		if(Creature* pFrye = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FRYE)))
		{
			if(!pFrye->isAlive())
				pFrye->Respawn();
		}
	}

    void JustDied(Unit* pKiller)
    {
		bool bAddsDead = true;
		if(Creature* pBaxter = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BAXTER)))
		{
			if(pBaxter->isAlive())
				bAddsDead = false;
		}
		if(Creature* pFrye = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FRYE)))
		{
			if(pFrye->isAlive())
			{
				((boss_apothecary_fryeAI*)pFrye->AI())->m_bSummon = true;
				bAddsDead = false;
			}
		}
		if(!bAddsDead)
			m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }

	void JustSummoned(Creature* pSummon)
	{
		pSummon->setFaction(14);
		pSummon->AI()->AttackStart(m_creature->getVictim());
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiBaxterTimer < uiDiff && !m_bBaxterAggro)
		{
			if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BAXTER, 50.0f))
			{
				pTemp->setFaction(14);
				pTemp->AI()->AttackStart(m_creature->getVictim());
			}
			m_bBaxterAggro = true;
		}
		else m_uiBaxterTimer -= uiDiff;

		if(m_uiFryeTimer < uiDiff && !m_bFryeAggro)
		{
			if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_FRYE, 50.0f))
			{
				pTemp->setFaction(14);
				pTemp->AI()->AttackStart(m_creature->getVictim());
			}
			m_bFryeAggro = true;
		}
		else m_uiFryeTimer -= uiDiff;

		if(m_uiSprayTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_PERFUME_SPRAY);
			m_uiSprayTimer = urand(5000, 7000);
		}
		else m_uiSprayTimer -= uiDiff;

		if(m_uiReactionTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_CHAIN_REACTION);
			m_uiReactionTimer = urand(10000, 13000);
		}
		else m_uiReactionTimer -= uiDiff;

		if(m_uiSummonTimer < uiDiff)
		{
			for(uint8 i = 0; i < 3; i++)
				m_creature->SummonCreature(NCP_CRAZED_APOTHECARY, PositionLoc[i].x, PositionLoc[i].y, PositionLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
			m_uiSummonTimer = 10000;
		}
		else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_apothecary_hummel(Creature* pCreature)
{
    return new boss_apothecary_hummelAI(pCreature);
}

struct MANGOS_DLL_DECL mob_crazed_apothecaryAI : public ScriptedAI
{
    mob_crazed_apothecaryAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
			DoCast(m_creature, SPELL_UNSTABLE_REACTION);
    }
};

CreatureAI* GetAI_mob_crazed_apothecary(Creature* pCreature)
{
    return new mob_crazed_apothecaryAI(pCreature);
}

bool ChooseReward_apothecary_hummel(Player* pPlayer, Creature* pCreature, const Quest* pQuest, uint32 item)
{
	if(pQuest->GetQuestId() == QUEST_BEEN_SERVED)
	{
		pCreature->setFaction(14);
		pCreature->AddThreat(pPlayer, 1000.0f);
		pCreature->AI()->AttackStart(pPlayer);
	}

	return true;
}

void AddSC_boss_apothecarey_hummel()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_apothecary_hummel";
    NewScript->GetAI = GetAI_boss_apothecary_hummel;
	NewScript->pChooseReward = &ChooseReward_apothecary_hummel;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "boss_apothecary_baxter";
    NewScript->GetAI = GetAI_boss_apothecary_baxter;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "boss_apothecary_frye";
    NewScript->GetAI = GetAI_boss_apothecary_frye;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_crazed_apothecary";
    NewScript->GetAI = GetAI_mob_crazed_apothecary;
    NewScript->RegisterSelf();
}
