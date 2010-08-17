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
SDName: boss_blood_queen_lanathel
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    // yells
    SAY_AGGRO           = -1609510,
    SAY_VAMPIRIC_BITE1  = -1609511,
    SAY_VAMPIRIC_BITE2  = -1609512,
    SAY_SHADOWS         = -1609513,
    SAY_PACT            = -1609514,
    SAY_CONTROLL        = -1609515,
    SAY_AIR_PHASE       = -1609516,
    SAY_BERSERK         = -1609517,
    SAY_DEATH           = -1609518,

    // spells
	SPELL_SHROUD_OF_SORROW                  = 72981,
	SPELL_DELIRIOUS_SLASH                   = 71623,
	SPELL_BLOOD_MIRROR_LINK					= 70445,
	SPELL_BLOOD_MIRROR_1                    = 70821,
	SPELL_BLOOD_MIRROR_2                    = 71510,
	SPELL_VAMPIRIC_BITE                     = 71726,	// triggers essence of the blood queen
	SPELL_VAMPIRIC_BITE_CHARGE				= 71728,
	SPELL_ESSENCE_OF_BLOOD_QUEEN            = 70867,	// triggers frenzied blood thirst after 75 secs
	SPELL_FRENZIED_BLOODTHIRST              = 70877,	// screen effect, must bite other players or else triggers uncontrollable frenzy in 15 secs
	SPELL_UNCONTROLLABLE_FRENZY             = 70923,
	SPELL_PACT_OF_THE_DARKFALLEN            = 71340,	// must trigger dmg
	SPELL_SWARMING_SHADOWS                  = 71264,	// 71861
	SPELL_TWILIGHT_BLOODBOLT                = 71446,
	SPELL_BLOODBOLT_WHIRL                   = 71772,
	SPELL_PRESENCE_OF_DARKFALLEN            = 71959,    // on heroic
	SPELL_INCITE_TERROR						= 73070,
	SPELL_BERSERK							= 26662,

	NPC_SHADOWS								= 38163,
	SPELL_SWARMING_SHADOWS_AURA				= 71267,
};

struct MANGOS_DLL_DECL boss_blood_queen_lanathelAI : public ScriptedAI
{
    boss_blood_queen_lanathelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiDeliriousSlashTimer;
	uint32 m_uiBloodMirrorTimer;
	uint32 m_uiVampiricBiteTimer;
	uint32 m_uiPactOfTheDarkfallenTimer;
	uint32 m_uiSwarmingShadowsTimer;
	uint32 m_uiBloodboltWhirlTimer;
	uint8 m_uiBloodBoltPhase;
	bool m_bIsBloodBolt;
	uint32 m_uiTwilightBloodboltTimer;
	uint32 m_uiBerserkTimer;

    void Reset()
    {
		m_uiDeliriousSlashTimer				= 3000; 
		m_uiBloodMirrorTimer				= 15000; //
		m_uiVampiricBiteTimer				= 15000;
		m_uiPactOfTheDarkfallenTimer		= 15000; //
		m_uiSwarmingShadowsTimer			= 30500; //
		m_uiTwilightBloodboltTimer			= 10000;
		m_uiBloodboltWhirlTimer				= 120000; // 2 min
		m_uiBloodBoltPhase					= 0;
		m_bIsBloodBolt						= false;
		m_uiBerserkTimer					= 330000;  // 5:30 min
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_BLOOD_QUEEN, NOT_STARTED);
            m_pInstance->SetData(TYPE_ATTEMPTS, m_pInstance->GetData(TYPE_ATTEMPTS) - 1);
            m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance) 
		{
            m_pInstance->SetData(TYPE_BLOOD_QUEEN, IN_PROGRESS);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_SHOW, 1);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
		}
        DoCast(m_creature,SPELL_SHROUD_OF_SORROW);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_BLOOD_QUEEN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		// heroic only
		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
		{
			if(!m_creature->HasAura(SPELL_PRESENCE_OF_DARKFALLEN, EFFECT_INDEX_0))
				DoCast(m_creature, SPELL_PRESENCE_OF_DARKFALLEN);
		}

		if(m_uiBloodboltWhirlTimer < uiDiff)
		{
			switch(m_uiBloodBoltPhase)
			{
				// move center
			case 0:
				DoScriptText(SAY_AIR_PHASE, m_creature);
				DoCast(m_creature, SPELL_INCITE_TERROR);
				SetCombatMovement(false);
				m_creature->GetMotionMaster()->MovePoint(0, 4595.87f, 2769.25f, 400.137f);
				m_bIsBloodBolt = true;
				++m_uiBloodBoltPhase;
				m_uiSwarmingShadowsTimer	= 30000;
				m_uiPactOfTheDarkfallenTimer = 25000;
				m_uiBloodboltWhirlTimer		= 3000;
				break;
				// fly
			case 1:
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
				m_creature->GetMotionMaster()->MovePoint(0, 4595.89f, 2769.25f, 420.02f);
				m_creature->GetMap()->CreatureRelocation(m_creature, 4595.89f, 2769.25f, 420.02f, 3.20f);
                m_creature->SendMonsterMove(4595.89f, 2769.25f, 420.02f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
				++m_uiBloodBoltPhase;
				m_uiBloodboltWhirlTimer = 3000;
				break;
				// cast spell
			case 2:
				DoCast(m_creature, SPELL_BLOODBOLT_WHIRL);
				++m_uiBloodBoltPhase;
				m_uiBloodboltWhirlTimer = 6100;
				break;
				// move down
			case 3:
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
				SetCombatMovement(true);
				m_creature->GetMotionMaster()->MovePoint(0, 4595.87f, 2769.25f, 400.137f);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				m_bIsBloodBolt = false;
				m_uiBloodBoltPhase = 0;
				m_uiBloodboltWhirlTimer = 110000; // 120 - 12
				break;
			}
		}
		else m_uiBloodboltWhirlTimer -= uiDiff;

		// return if is casting blood bolt
		if(m_bIsBloodBolt)
			return;

		if (m_uiVampiricBiteTimer < uiDiff)
		{
			switch(urand(0, 1))
			{
			case 0: DoScriptText(SAY_VAMPIRIC_BITE1, m_creature); break;
			case 1: DoScriptText(SAY_VAMPIRIC_BITE2, m_creature); break;
			}
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(pTarget, SPELL_VAMPIRIC_BITE);
			m_uiVampiricBiteTimer = 600000;	// cast only once
		}
		else m_uiVampiricBiteTimer -= uiDiff;

		if(m_uiTwilightBloodboltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(pTarget, SPELL_TWILIGHT_BLOODBOLT);
			m_uiTwilightBloodboltTimer = urand(5000, 10000);
		}
		else m_uiTwilightBloodboltTimer -= uiDiff;

		if (m_uiPactOfTheDarkfallenTimer < uiDiff)
		{
			DoScriptText(SAY_PACT, m_creature);
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(pTarget, SPELL_PACT_OF_THE_DARKFALLEN);
			m_uiPactOfTheDarkfallenTimer = 30000;
		}
		else m_uiPactOfTheDarkfallenTimer -= uiDiff;

		if (m_uiSwarmingShadowsTimer < uiDiff)
		{
			DoScriptText(SAY_SHADOWS, m_creature);
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				DoCast(pTarget,SPELL_SWARMING_SHADOWS);
			m_uiSwarmingShadowsTimer = 30000;
		}
		else m_uiSwarmingShadowsTimer -= uiDiff;

		if (m_uiBloodMirrorTimer < uiDiff)
		{
			//if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
				//DoCast(pTarget, SPELL_BLOOD_MIRROR_1);
			m_uiBloodMirrorTimer = 15000;
		}
		else m_uiBloodMirrorTimer -= uiDiff;

		if (m_uiDeliriousSlashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_DELIRIOUS_SLASH);
			m_uiDeliriousSlashTimer = urand(3000, 6000);
		}
		else m_uiDeliriousSlashTimer -= uiDiff;

		if (m_uiBerserkTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_BERSERK, m_creature);
			DoCast(m_creature, SPELL_BERSERK);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}

struct MANGOS_DLL_DECL mob_swarming_shadowAI : public ScriptedAI
{
    mob_swarming_shadowAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    void Reset()
    {
		DoCast(m_creature, SPELL_SWARMING_SHADOWS_AURA);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_BLOOD_QUEEN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_swarming_shadow(Creature* pCreature)
{
    return new mob_swarming_shadowAI (pCreature);
}

void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_swarming_shadow";
    newscript->GetAI = &GetAI_mob_swarming_shadow;
    newscript->RegisterSelf();
}
