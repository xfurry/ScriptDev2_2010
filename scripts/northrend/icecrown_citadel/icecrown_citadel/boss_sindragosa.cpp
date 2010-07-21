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
	} 

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_FROST_AURA);
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
    NewScript->Name = "miniboss_spinestalker";
    NewScript->GetAI = &GetAI_miniboss_spinestalker;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "miniboss_rimefang";
    NewScript->GetAI = &GetAI_miniboss_rimefang;
    NewScript->RegisterSelf();
}