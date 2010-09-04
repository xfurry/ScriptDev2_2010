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
SDName: Urom
SD%Complete: 80
SDComment: Is not working SPELL_ARCANE_SHIELD. SPELL_FROSTBOMB has some issues, the damage aura should not stack.
SDCategory: Instance Script
EndScriptData */

#include "precompiled.h"
#include "oculus.h"

enum
{
	// yells
	SAY_SUMMON1					= -1578018,
	SAY_SUMMON2					= -1578019,
	SAY_SUMMON3					= -1578020,
	SAY_AGGRO					= -1578021,
	SAY_EXPLOSION1				= -1578022,
	SAY_EXPLOSION2				= -1578023,
	SAY_SLAY1					= -1578024,
	SAY_SLAY2					= -1578025,
	SAY_SLAY3					= -1578026,
	SAY_DEATH					= -1578027,
	SAY_SPAWN_EREGOS			= -1578028,

	// spells
	SPELL_ARCANE_SHIELD                           = 53813, 
	SPELL_EMPOWERED_ARCANE_EXPLOSION              = 51110,
	SPELL_EMPOWERED_ARCANE_EXPLOSION_H            = 59377,
	SPELL_FROSTBOMB                               = 51103, 
	SPELL_SUMMON_MENAGERIE_1                      = 50476, 
	SPELL_SUMMON_MENAGERIE_2                      = 50495,
	SPELL_SUMMON_MENAGERIE_3                      = 50496,
	SPELL_TELEPORT                                = 51112, 
	SPELL_TIME_BOMB                               = 51121, 
	SPELL_TIME_BOMB_H                             = 59376,

	// npcs
	NPC_PHANTASMAL_CLOUDSCRAPER                   = 27645,
	NPC_PHANTASMAL_MAMMOTH                        = 27642,
	NPC_PHANTASMAL_WOLF                           = 27644,

	NPC_PHANTASMAL_AIR                            = 27650,
	NPC_PHANTASMAL_FIRE                           = 27651,
	NPC_PHANTASMAL_WATER                          = 27653,

	NPC_PHANTASMAL_MURLOC                         = 27649,
	NPC_PHANTASMAL_NAGAL                          = 27648,
	NPC_PHANTASMAL_OGRE                           = 27647,

	// phases
	PHASE_IDLE				= 0,
	PHASE_PLATFORM1			= 1,
	PHASE_PLATFORM2			= 2,
	PHASE_CENTER			= 3,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

static LocationsXY SummonLoc[]=
{
    {1177.469f, 937.721f, 527.405f},
    {968.659f,  1042.53f, 527.320f},
    {1164.02f,  1170.84f, 527.320f},
};

struct TrashPacks
{
    uint32 firstId, secondId, thirdId;
    uint32 id;
};

static TrashPacks PackId[]=
{
    {NPC_PHANTASMAL_CLOUDSCRAPER, NPC_PHANTASMAL_MAMMOTH, NPC_PHANTASMAL_WOLF},
    {NPC_PHANTASMAL_AIR, NPC_PHANTASMAL_FIRE, NPC_PHANTASMAL_WATER},
    {NPC_PHANTASMAL_MURLOC, NPC_PHANTASMAL_NAGAL, NPC_PHANTASMAL_OGRE},
};

struct MANGOS_DLL_DECL boss_uromAI : public ScriptedAI
{
	boss_uromAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

	uint32 m_uiPhase;
	bool m_bHasAggro;

	uint32 m_uiTeleportTimer;
	uint32 m_uiExplosionTimer;
	uint32 m_uiExplosionStage;
	uint32 m_uiFrostBombTimer;
	uint32 m_uiTimeBombTimer;
	bool m_bHasCasted;
	std::vector<uint32> TrashPacksEntries;

	void Reset()
	{
		DoCast(m_creature, SPELL_ARCANE_SHIELD);
		TrashPacksEntries.clear();
		SetPacks();

		m_uiPhase			= PHASE_IDLE;
		m_bHasCasted		= false;
		m_bHasAggro			= false;
		m_uiTeleportTimer	= urand(30000,35000);
		m_uiExplosionTimer	= 60000;
		m_uiExplosionStage	= 0;
		m_uiFrostBombTimer	= urand(5000,8000);
		m_uiTimeBombTimer	= urand(20000,25000);
	}

	void SetPacks()
	{
        TrashPacksEntries.reserve(3);
		TrashPacksEntries.push_back(0);
		TrashPacksEntries.push_back(1);
		TrashPacksEntries.push_back(2);

		if(!TrashPacksEntries.empty())
			std::random_shuffle(TrashPacksEntries.begin(), TrashPacksEntries.end());
	}

	void JustReachedHome()
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_UROM, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_UROM, IN_PROGRESS);
	}

	void AttackStart(Unit* pWho)
	{
		if(m_pInstance->GetData(TYPE_VAROS) != DONE)
			return;

		if(m_uiPhase != PHASE_CENTER && !m_bHasAggro)
			return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
			DoScriptText(SAY_AGGRO, m_creature);
			m_bHasAggro = true;
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

	void JustDied(Unit* killer)
	{
		DoScriptText(SAY_DEATH, m_creature);
		if(Creature* pEregos = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_EREGOS)))
		{
			pEregos->SetVisibility(VISIBILITY_ON);
			DoScriptText(SAY_SPAWN_EREGOS, pEregos);
		}

		if (m_pInstance)
			m_pInstance->SetData(TYPE_UROM, DONE);
	}

	void JustSummoned(Creature* pSummon)
	{
		pSummon->SetInCombatWithZone();
	}

	void SummonTrashPack(uint32 uiPackId, uint32 uiLocId)
	{
		float angle = (float) rand()*360/RAND_MAX + 1;
		float homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
		float homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
		m_creature->SummonCreature(PackId[uiPackId].firstId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

		angle = (float) rand()*360/RAND_MAX + 1;
		homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
		homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
		m_creature->SummonCreature(PackId[uiPackId].secondId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

		angle = (float) rand()*360/RAND_MAX + 1;
		homeX = SummonLoc[uiLocId].x + 10*cos(angle*(M_PI/180));
		homeY = SummonLoc[uiLocId].y + 10*sin(angle*(M_PI/180));
		m_creature->SummonCreature(PackId[uiPackId].thirdId, homeX, homeY, SummonLoc[uiLocId].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		switch(m_uiPhase)
		{
		case PHASE_IDLE:
			if(!m_bHasCasted)
			{
				m_bHasCasted = true;
				m_creature->CastStop();
				DoScriptText(SAY_SUMMON1, m_creature);
				DoCast(m_creature, SPELL_SUMMON_MENAGERIE_1);
			}
			break;
		case PHASE_PLATFORM1:
			if(!m_bHasCasted)
			{
				m_bHasCasted = true;
				m_creature->CastStop();
				DoScriptText(SAY_SUMMON2, m_creature);
				DoCast(m_creature, SPELL_SUMMON_MENAGERIE_2);
			}
			break;
		case PHASE_PLATFORM2:
			if(!m_bHasCasted)
			{
				m_bHasCasted = true;
				m_creature->CastStop();
				DoScriptText(SAY_SUMMON3, m_creature);
				DoCast(m_creature, SPELL_SUMMON_MENAGERIE_3);
			}
			break;
		}

		if(m_uiPhase != PHASE_CENTER)
			uiDamage = 0;
		else if(!m_bHasAggro)
			m_bHasAggro = true;
	}

	void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
	{
		switch(pSpell->Id)
		{
		case SPELL_SUMMON_MENAGERIE_1:
			m_bHasCasted = false;
			SummonTrashPack(TrashPacksEntries[0], 0);
			DoCast(m_creature, SPELL_ARCANE_SHIELD);
			m_uiPhase = PHASE_PLATFORM1;
			break;
		case SPELL_SUMMON_MENAGERIE_2:
			m_bHasCasted = false;
			SummonTrashPack(TrashPacksEntries[1], 1);
			DoCast(m_creature, SPELL_ARCANE_SHIELD);
			m_uiPhase = PHASE_PLATFORM2;
			break;
		case SPELL_SUMMON_MENAGERIE_3:
			m_bHasCasted = false;
			SummonTrashPack(TrashPacksEntries[2], 2);
			DoCast(m_creature, SPELL_ARCANE_SHIELD);
			m_uiPhase = PHASE_CENTER;
			break;
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(m_bHasAggro && m_uiPhase == PHASE_CENTER)
		{
			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

			if (m_uiTeleportTimer <= uiDiff)
			{
				switch(urand(0, 1))
				{
				case 0: DoScriptText(SAY_EXPLOSION1, m_creature); break;
				case 1: DoScriptText(SAY_EXPLOSION2, m_creature); break;
				}
				m_creature->InterruptNonMeleeSpells(false);
				DoCast(m_creature, SPELL_TELEPORT);
				m_creature->GetMotionMaster()->MoveIdle();
				m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
				m_uiTeleportTimer = urand(30000,35000);
				m_uiExplosionTimer = 1500;
			} 
			else m_uiTeleportTimer -= uiDiff;

			if(m_uiExplosionTimer < uiDiff)
			{
				DoCast(m_creature, m_bIsRegularMode ? SPELL_EMPOWERED_ARCANE_EXPLOSION : SPELL_EMPOWERED_ARCANE_EXPLOSION_H);
				m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				m_uiExplosionTimer = 60000;
			}
			else m_uiExplosionTimer -= uiDiff;

			if (m_uiFrostBombTimer < uiDiff)
			{
				DoCast(m_creature->getVictim(), SPELL_FROSTBOMB);
				m_uiFrostBombTimer = urand(5000,8000);
			} 
			else m_uiFrostBombTimer -= uiDiff;

			if (m_uiTimeBombTimer <= uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, m_bIsRegularMode ? SPELL_TIME_BOMB : SPELL_TIME_BOMB_H);
				m_uiTimeBombTimer = urand(20000,25000);
			} 
			else m_uiTimeBombTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
	}
};

CreatureAI* GetAI_boss_urom(Creature* pCreature)
{
	return new boss_uromAI (pCreature);
}

void AddSC_boss_urom()
{
	Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_urom";
    newscript->GetAI = &GetAI_boss_urom;
    newscript->RegisterSelf();
}
