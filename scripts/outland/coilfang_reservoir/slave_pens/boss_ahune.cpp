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
SDName: Boss_ahune
SD%Complete:
SDComment:
SDCategory: Slave_pens
EndScriptData */

#include "precompiled.h"

enum
{
	SPELL_SUMMON_AHUNE			= 45926,

	SPELL_CRYSTAL_SPIKE			= 47944,
	SPELL_CRYSTAL_SPIKE_H		= 57067,
	SPELL_ICE_SPIKE_TRIGG		= 50097,
	SPELL_SUMMON_SPIKE			= 47955,

	/*
SPELL_ = 45930, // Ahune - Summoning Rhyme Spell, make bonfire, 3, 0, 0
SPELL_ = 45937, // Ahune - Summoning Visual 1, 27, 0, 0
SPELL_ = 45938, // Ahune - Summoning Visual 1.5, 27, 0, 0
SPELL_ = 45939, // Ahune summon Loot, 76, 0, 0
SPELL_ = 45941, // Ahune summon loot, Missile, 3, 0, 0
SPELL_ = 45945, // Ahune - Slippy Flour, 27, 0, 0
SPELL_ = 45954, // Ahune's Shield, 6, 6, 0
SPELL_ = 46103, // Midsummer - Ahune - Summon Minion, Lower, 3, 0, 0
SPELL_ = 46146, // [PH] Ahune Spanky Hands, 6, 0, 0
SPELL_ = 46314, // Ahune - Slippery Floor Ambient, 6, 0, 0
SPELL_ = 46336, // Beam Attack against Ahune, 3, 0, 0
SPELL_ = 46363, // Beam Attack against Ahune 2, 3, 0, 0
SPELL_ = 46402, // Ahune remerge, 2, 98, 0
SPELL_ = 46416, // Ahune selfstun, 6, 6, 0
SPELL_ = 46592, // Summon Leutnant of Ahune, 86, 6, 0
SPELL_ = 46622, // Summon Ahunes Lott, heroic, 76, 0, 0
SPELL_ = 46623, // Summon Ahunes Loot, heroic, 3, 0, 0
SPELL_ = 46786, // Ahunes Ghostmodel, 6, 0, 0
SPELL_ = 46809, // Ahunes Ghost implodes, 3, 0, 0
SPELL_ = 46888, // Summon Ahune bottle, 76, 0, 0
SPELL_ = 46889, // Summon Ahunes Six-Pack, 77, 0, 0
SPELL_ = 46891, // Summon Ahune loot, Bottles, 76, 0, 0
SPELL_ = 46892, // Summon Ahunes loot, heroc, Bottles, 76, 0, 0
SPELL_ = 62043, // Midsummer - Ahune - DIES, Achievement, 3, 0, 0
*/

	// adds spells
	SPELL_CHILLING_AURA			= 46542,
	SPELL_HAILSTONE_CHILL		= 46458,
	SPELL_PULVERIZE				= 2676,

	SPELL_BITTER_BLAST			= 46406,

	SPELL_LIGHTNING_SHIELD		= 12550,
	SPELL_WIND_BUFFET			= 46568,

	// npc
	NPC_AHUNE					= 25740,
	NPC_FROZEN_CORE				= 25865,
	NPC_GHOST_OF_AHUNE			= 26239,
	NPC_AHUNITE_HAILSTONE		= 25755,	// 1 at the start of phase 1
	NPC_AHUNITE_COLDWAVE		= 25756,	// pair of 2 periodicaly in phase 1
	NPC_AHUNITE_FROSTWIND		= 25757,	// periodicaly in phase 1 - not in first phase

	QUEST_SUMMON_AHUNE			= 11691,
};

struct MANGOS_DLL_DECL boss_ahuneAI : public Scripted_NoMovementAI
{
    boss_ahuneAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiPhase;
	uint32 m_uiPhaseChangeTimer;

    void Reset()
    {
		m_uiPhase = 0;
		m_uiPhaseChangeTimer = 60000;
    }

	void Aggro(Unit* pWho)
	{
		m_uiPhase = 1;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiPhase == 1)
		{

			if(m_uiPhaseChangeTimer < uiDiff)
			{
				m_uiPhase = 2;
				m_uiPhaseChangeTimer = 30000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
		else if(m_uiPhase == 2)
		{
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				m_uiPhase = 1;
				m_uiPhaseChangeTimer = 60000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;
		}
    }
};
CreatureAI* GetAI_boss_ahune(Creature* pCreature)
{
    return new boss_ahuneAI(pCreature);
}

void AddSC_boss_ahune()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ahune";
    newscript->GetAI = &GetAI_boss_ahune;
    newscript->RegisterSelf();
}
