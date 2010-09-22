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
	NPC_HUMMEL					= 36296,
	NPC_FRYE					= 36272,
	NPC_BAXTER					= 36565,
	NCP_CRAZED_APOTHECARY		= 36568,	// this is the right one
	NPC_CROWN_APOTHECARY		= 36885,

	QUEST_BEEN_SERVED			= 14488,
};

struct MANGOS_DLL_DECL boss_apothecary_hummelAI : public ScriptedAI
{
    boss_apothecary_hummelAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint32 m_uiBaxterTimer;
	uint32 m_uiFryeTimer;

    void Reset()
    {
		m_uiBaxterTimer		= 10000;
		m_uiFryeTimer		= 20000;
		DoCast(m_creature, SPELL_ALLURING_PERFUME);
		m_creature->setFaction(35);
    }

    void JustDied(Unit* pKiller)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_apothecary_hummel(Creature* pCreature)
{
    return new boss_apothecary_hummelAI(pCreature);
}

struct MANGOS_DLL_DECL boss_apothecary_baxterAI : public ScriptedAI
{
    boss_apothecary_baxterAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

    void Reset()
    {
		DoCast(m_creature, SPELL_IRRESISTIBLE_COLOGNE);
		m_creature->setFaction(35);
    }

    void JustDied(Unit* pKiller)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

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
		Reset();
	}

    void Reset()
    {
		m_creature->setFaction(35);
    }

    void JustDied(Unit* pKiller)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_apothecary_frye(Creature* pCreature)
{
    return new boss_apothecary_fryeAI(pCreature);
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

void AddSC_boss_apothecarey_hummel()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_apothecary_hummel";
    NewScript->GetAI = GetAI_boss_apothecary_hummel;
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
