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
SDName: boss_coren_direbrew
SD%Complete:
SDComment:
SDCategory: Blackrock Depths
EndScriptData */

#include "precompiled.h"

enum
{
	// yells

	// spells
	SPELL_DIREBREW_DISARM			= 47310,
	SPELL_SUMMON_DIREBREW_MINION	= 47375,
	SPELL_DIREBREW_CHARGE			= 47718,

	SPELL_BARRELED					= 47442,	// 8 sec cd
	SPELL_BARRELED_TRIGG			= 50278,
	SPELL_CREATE_BREW				= 47345,
	SPELL_HAS_BREW					= 47331,
	SPELL_SEND_FIRST_BREW			= 47333,	// missle
	SPELL_SEND_SECOND_BREW			= 47339,	// missle
	SPELL_REQUEST_MUG				= 47344,
	SPELL_BREWMAIDEN_STUN			= 47340,	
	SPELL_BREWMAIDEN_DESPAWN		= 48186,

	NPC_COREN_DIREBREW				= 23872,
	NPC_DIREBREW_MINION				= 26776,
	NPC_ILSA_DIREBREW				= 26764,	// at 66%
	NPC_URSULA_DIREBREW				= 26822,	// at 33%

	QUEST_INSULT_COREN				= 12062,
};

struct MANGOS_DLL_DECL boss_coren_direbrewAI : public ScriptedAI
{
    boss_coren_direbrewAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiDisarmTimer;
	uint32 m_uiChargeTimer;
	uint32 m_uiSummonTimer;
	uint32 m_uiHpPhase;

	uint64 m_uiIlsaGUID;
	uint64 m_uiUrsulaGUID;

    void Reset()
    {
		m_uiDisarmTimer		= 10000;
		m_uiChargeTimer		= 5000;
		m_uiSummonTimer		= 15000;
		m_uiHpPhase			= 0;

		m_uiIlsaGUID		= 0;
		m_uiUrsulaGUID		= 0;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

	void Aggro(Unit* pWho)
	{
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SummonCreature(NPC_DIREBREW_MINION, 892.757f, -133.445f, -49.748f, 2.21f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
		m_creature->SummonCreature(NPC_DIREBREW_MINION, 889.757f, -133.445f, -49.748f, 2.21f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
		m_creature->SummonCreature(NPC_DIREBREW_MINION, 895.757f, -133.445f, -49.748f, 2.21f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_creature->GetHealthPercent() < 66.0f && m_uiHpPhase == 0)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = m_creature->GetPositionX() + urand(0, 10)*cos(angle*(M_PI/180));
			float homeY = m_creature->GetPositionY() + urand(0, 10)*sin(angle*(M_PI/180));
			if(Creature* pTemp = m_creature->SummonCreature(NPC_ILSA_DIREBREW, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
			{
				m_uiIlsaGUID = pTemp->GetGUID();
				pTemp->AI()->AttackStart(m_creature->getVictim());
			}
			m_uiHpPhase = 1;
		}

		if(m_creature->GetHealthPercent() < 33.0f && m_uiHpPhase == 1)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = m_creature->GetPositionX() + urand(0, 10)*cos(angle*(M_PI/180));
			float homeY = m_creature->GetPositionY() + urand(0, 10)*sin(angle*(M_PI/180));
			if(Creature* pTemp = m_creature->SummonCreature(NPC_URSULA_DIREBREW, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
			{
				m_uiUrsulaGUID = pTemp->GetGUID();
				pTemp->AI()->AttackStart(m_creature->getVictim());
			}
			m_uiHpPhase = 2;
		}
	}

	void JustSummoned(Creature* pSummon)
	{
		pSummon->setFaction(14);
		pSummon->AI()->AttackStart(m_creature->getVictim());
	}

	void JustDied(Unit* pKiller)
	{
		if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_uiIlsaGUID))
			pTemp->ForcedDespawn();
		if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_uiUrsulaGUID))
			pTemp->ForcedDespawn();
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiDisarmTimer < uiDiff)
		{
			DoCastSpellIfCan(m_creature, SPELL_DIREBREW_DISARM);
			m_uiDisarmTimer = 15000;
		}
		else m_uiDisarmTimer -= uiDiff;

		if(m_uiChargeTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				DoCastSpellIfCan(pTarget, SPELL_DIREBREW_CHARGE);
			m_uiChargeTimer = 5000;
		}
		else m_uiChargeTimer -= uiDiff;

		if(m_uiSummonTimer < uiDiff)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = m_creature->GetPositionX() + urand(0, 10)*cos(angle*(M_PI/180));
			float homeY = m_creature->GetPositionY() + urand(0, 10)*sin(angle*(M_PI/180));
			for(uint8 i = 0; i < 3; ++i)
				m_creature->SummonCreature(NPC_DIREBREW_MINION, homeX + urand(0, 2), homeY + urand(0, 2), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
			m_uiSummonTimer = 15000;
		}
		else m_uiSummonTimer -= uiDiff;        

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_coren_direbrew(Creature* pCreature)
{
    return new boss_coren_direbrewAI(pCreature);
}

struct MANGOS_DLL_DECL mob_direbrew_maidenAI : public ScriptedAI
{
    mob_direbrew_maidenAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiMugTimer;
	uint32 m_uiBarreledTimer;

    void Reset()
    {
		m_uiMugTimer		= 2000;
		m_uiBarreledTimer	= 5000;
		// respawn if Coren is Alive
		if(Creature* pCoren = GetClosestCreatureWithEntry(m_creature, NPC_COREN_DIREBREW, 50.0f))
		{
			if(pCoren->isAlive())
				m_creature->SetRespawnDelay(30000);
		}
    }

	void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
	{
		if(pSpell->Id == SPELL_SEND_FIRST_BREW)
			pTarget->CastSpell(pTarget, SPELL_CREATE_BREW, false);
		// stun the target at the second brew
		//else if(pSpell->Id == SPELL_SEND_SECOND_BREW)
		//	pTarget->CastSpell(pTarget, SPELL_BREWMAIDEN_STUN, false);
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiMugTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_SEND_FIRST_BREW);
			m_uiMugTimer = 10000;
		}
		else m_uiMugTimer -= uiDiff;

		if(m_uiBarreledTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_BARRELED);
			m_uiBarreledTimer = 15000;
		}
		else m_uiBarreledTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_direbrew_maiden(Creature* pCreature)
{
    return new mob_direbrew_maidenAI(pCreature);
}

bool ChooseReward_coren_direbrew(Player* pPlayer, Creature* pCreature, const Quest* pQuest, uint32 item)
{
	if(pQuest->GetQuestId() == QUEST_INSULT_COREN)
	{
		pCreature->setFaction(14);
		pCreature->AddThreat(pPlayer, 1000.0f);
		pCreature->AI()->AttackStart(pPlayer);
	}

	return true;
}

void AddSC_boss_coren_direbrew()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_coren_direbrew";
    newscript->GetAI = &GetAI_boss_coren_direbrew;
	newscript->pChooseReward = &ChooseReward_coren_direbrew;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_direbrew_maiden";
    newscript->GetAI = &GetAI_mob_direbrew_maiden;
    newscript->RegisterSelf();
}
