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
	SPELL_BLOOD_MIRROR_1                    = 70821,
	SPELL_BLOOD_MIRROR_2                    = 71510,
	SPELL_VAMPIRIC_BITE                     = 71726,
	SPELL_BITE								= 71728,
	SPELL_ESSENCE_OF_BLOOD_QUEEN            = 70867,
	SPELL_FRENZIED_BLOODTHIRST              = 70877,
	SPELL_UNCONTROLLABLE_FRENZY             = 70923,
	SPELL_PACT_OF_THE_DARKFALLEN            = 71340,
	SPELL_SWARMING_SHADOWS                  = 71265,
	SPELL_TWILIGHT_BLOODBOLT                = 71446,
	SPELL_BLOODBOLT_WHIRL                   = 71772,
	SPELL_MASS_FEAR							= 64386,	//this is auriaya's fear
	SPELL_PRESENCE_OF_DARKFALLEN            = 71959,    // on heroic
	SPELL_ENRAGE							= 26662,
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

	uint32 m_uiDeliriousSlash_Timer;
	uint32 m_uiBloodMirror1_Timer;
	uint32 m_uiVampiricBite_Timer;
	uint32 m_uiPactOfTheDarkfallen_Timer;
	uint32 m_uiSwarmingShadows_Timer;
	uint32 m_uiBloodboltWhirl_Timer;
	uint32 m_uiBerserk_Timer;
	uint32 m_uiFly_Timer;
	uint32 phase;

	uint32 m_uiPhaseChangeTimer;

    void Reset()
    {
		phase								= 1;
		m_uiDeliriousSlash_Timer			= 10000; // not accurate
		m_uiBloodMirror1_Timer				= 15000; //
		m_uiVampiricBite_Timer				= 15000; //
		m_uiPactOfTheDarkfallen_Timer		= 15000; //
		m_uiSwarmingShadows_Timer			= 30500; //
		//m_uiBloodboltWhirl_Timer			= 120000; // 2 min
		m_uiFly_Timer						= 6500;
		m_uiPhaseChangeTimer                = 120000;
		m_uiBerserk_Timer					= 330000;  // 5:30 min
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_BLOOD_QUEEN, NOT_STARTED);
            m_pInstance->SetData(TYPE_ATTEMPTS, m_pInstance->GetData(TYPE_ATTEMPTS) - 1);
            //m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_BLOOD_QUEEN, IN_PROGRESS);
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

		if (phase == 1)
		{


			Unit* pomocni;
			if (m_uiVampiricBite_Timer < uiDiff)
			{
				DoScriptText(SAY_VAMPIRIC_BITE1, m_creature);
				if (pomocni = m_creature->SelectRandomUnfriendlyTarget())
				{
					DoCast(pomocni, SPELL_VAMPIRIC_BITE);
					DoCast(pomocni, SPELL_ESSENCE_OF_BLOOD_QUEEN);
				}
				m_uiVampiricBite_Timer = 1500000; 
				//so he doesnt cast this again xD
			}
			else m_uiVampiricBite_Timer -= uiDiff;

			if (m_uiPactOfTheDarkfallen_Timer < uiDiff)
			{
				if (Unit* target = m_creature->SelectRandomUnfriendlyTarget())
				{
					DoScriptText(SAY_PACT, m_creature);
					DoCast(target, SPELL_PACT_OF_THE_DARKFALLEN);
					//DoCast(target->SelectRandomFriendlyTarget(), SPELL_PACT_OF_THE_DARFALLEN);
					//if this doesn't work on two players, then enable second docast line
					//and update it to make it cast on nearby friendly in less than 5 yards	
				}
				m_uiPactOfTheDarkfallen_Timer = 30000;
			}
			else m_uiPactOfTheDarkfallen_Timer -= uiDiff;

			if (m_uiSwarmingShadows_Timer < uiDiff)
			{
				DoCast(m_creature->SelectRandomUnfriendlyTarget(),SPELL_SWARMING_SHADOWS);
				m_uiSwarmingShadows_Timer = 30500;
			}
			else m_uiSwarmingShadows_Timer -= uiDiff;

			if (m_uiBloodMirror1_Timer < uiDiff)
			{
				DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,0),SPELL_BLOOD_MIRROR_1);


				DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),SPELL_BLOOD_MIRROR_2);
				//this should hit nearest to tank, atm is set to random
				//needs to update
				m_uiBloodMirror1_Timer = 15000;

			}
			else m_uiBloodMirror1_Timer -= uiDiff;

			if (m_uiDeliriousSlash_Timer < uiDiff)
			{
				DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO,0),SPELL_DELIRIOUS_SLASH);
				m_uiDeliriousSlash_Timer = 10000;
			}

			//applies only 1 instance- bugged



			else m_uiDeliriousSlash_Timer -= uiDiff;

			if (m_uiBerserk_Timer < uiDiff)
			{
				DoScriptText(SAY_BERSERK, m_creature);
				DoCast(m_creature, SPELL_ENRAGE);
				m_uiBerserk_Timer = 60000;
			}
			else m_uiBerserk_Timer -= uiDiff;

			DoMeleeAttackIfReady();

			// make fly
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				DoCast(m_creature,SPELL_MASS_FEAR);
				m_creature->GetMap()->CreatureRelocation(m_creature, 4595.89f, 2769.15f, 418.02f, 0.0f);
				m_creature->SendMonsterMove(4595.89f, 2769.15f, 418.02f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveIdle();
				m_uiFly_Timer = 1000;
				phase = 2;
				m_uiPhaseChangeTimer = 8000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;
		}

		if (phase == 2)
		{

			//bugged.
			if (m_uiFly_Timer<uiDiff)
			{
				DoCast(m_creature, SPELL_BLOODBOLT_WHIRL);
				m_uiFly_Timer = 8000;
			}
			else m_uiFly_Timer -= uiDiff;

			// make land
			if(m_uiPhaseChangeTimer < uiDiff)
			{
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
				m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				phase = 1;
				m_uiPhaseChangeTimer = 120000;
			}
			else m_uiPhaseChangeTimer -= uiDiff;
		}
    }
};


CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}

void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();
}
