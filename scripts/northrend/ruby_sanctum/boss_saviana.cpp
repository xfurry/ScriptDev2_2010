/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_ragefire
SD%Complete:
SDComment:
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum BossSpells
{
	// yells
	SAY_AGGRO					= -1666400,
	SAY_SLAY1					= -1666401,
	SAY_SLAY2					= -1666402,
	SAY_DEATH					= -1666403,
	SAY_SPECIAL					= -1666404,

	// spells
    SPELL_ENRAGE                     = 78722, //soft enrage + fire nova 78723
    SPELL_FLAME_BREATH               = 74404,
    SPELL_BEACON                     = 74453, //mark for conflag, in enter to fly phase, 2 in 10, 5 in 25
    SPELL_CONFLAGATION               = 74452, // after fly up
    SPELL_CONFLAGATION_1             = 74455, // Triggered?
    SPELL_CONFLAGRATION_AURA         = 74456, // Aura

    MAX_BEACON_TARGETS               = 5,
};

static Locations SpawnLoc[]=
{
    {3151.3898f, 636.8519f, 78.7396f},		// 0 Saviana start point
    {3149.635f, 668.9644f, 90.507f},		// 1 Saviana fly phase, o=4,69
};

struct MANGOS_DLL_DECL boss_savianaAI : public ScriptedAI
{
    boss_savianaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiBreathTimer;
	uint32 m_uiEnrageTimer;
	uint32 m_uiFlyTimer;
	uint32 m_uiEndFlyTimer;
	uint32 m_uiConflagrateTimer;

	uint64 m_uiTarget1GUID;
	uint64 m_uiTarget2GUID;
	uint64 m_uiTarget3GUID;

	bool m_bIsInFly;

    void Reset()
    {
		m_uiBreathTimer		= 10000;
		m_uiEnrageTimer		= 20000;
		m_uiFlyTimer		= 30000;
		m_uiEndFlyTimer		= 40000;
		m_uiConflagrateTimer = 40000;

		m_uiTarget1GUID		= 0;
		m_uiTarget2GUID		= 0;
		m_uiTarget3GUID		= 0;
		m_bIsInFly			= false;
		SetCombatMovement(true);
    }

    void SetFly(bool command = false)
    {
        if (command)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_FLY_SIT_GROUND_UP);
            m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
			SetCombatMovement(false);
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
        }
        else
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
			SetCombatMovement(true);
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
			//m_creature->GetMotionMaster()->MovePoint(0, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1))
	{
        case 0:
               DoScriptText(SAY_SLAY1, m_creature, pVictim);
               break;
        case 1:
               DoScriptText(SAY_SLAY2, m_creature, pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAVIANA, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_SAVIANA, IN_PROGRESS);
        DoScriptText(SAY_AGGRO,m_creature);
    }

    void JustDied(Unit *killer)
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_SAVIANA, DONE);
        DoScriptText(SAY_DEATH,m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
	
		if(m_uiBreathTimer < uiDiff && !m_bIsInFly)
		{
			DoCast(m_creature->getVictim(), SPELL_FLAME_BREATH);
			m_uiBreathTimer = 5000;
		}
		else m_uiBreathTimer -= uiDiff;

		if(m_uiEnrageTimer < uiDiff && !m_bIsInFly)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_ENRAGE);
			m_uiEnrageTimer = urand(20000, 25000);
		}
		else m_uiEnrageTimer -= uiDiff;

		if(m_uiFlyTimer < uiDiff && !m_bIsInFly)
		{
			// workaround for beacon
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_BEACON);
				m_uiTarget1GUID = pTarget->GetGUID();
			}
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_BEACON);
				m_uiTarget2GUID = pTarget->GetGUID();
			}
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_BEACON);
				m_uiTarget3GUID = pTarget->GetGUID();
			}
			SetFly(true);
			m_bIsInFly = true;
			m_uiEndFlyTimer = 10000;
			m_uiConflagrateTimer = 5000;
			m_uiFlyTimer = 50000;
		}
		else m_uiFlyTimer -= uiDiff;

		if(m_uiConflagrateTimer < uiDiff && m_bIsInFly)
		{
			// workaround for conflagration
			if(Unit* pTarget = Unit::GetUnit(*m_creature, m_uiTarget1GUID))
				DoCast(pTarget, SPELL_CONFLAGRATION_AURA);
			if(Unit* pTarget = Unit::GetUnit(*m_creature, m_uiTarget2GUID))
				DoCast(pTarget, SPELL_CONFLAGRATION_AURA);
			if(Unit* pTarget = Unit::GetUnit(*m_creature, m_uiTarget3GUID))
				DoCast(pTarget, SPELL_CONFLAGRATION_AURA);
			m_uiConflagrateTimer = 60000;
		}
		else m_uiConflagrateTimer -= uiDiff;

		if(m_uiEndFlyTimer < uiDiff && m_bIsInFly)
		{
			SetFly();
			m_bIsInFly = false;
			m_uiBreathTimer		= 5000;
			m_uiEnrageTimer		= 15000;
			m_uiEndFlyTimer		= 60000;
		}
		else m_uiEndFlyTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_saviana(Creature* pCreature)
{
    return new boss_savianaAI(pCreature);
}

void AddSC_boss_saviana()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_saviana";
    newscript->GetAI = &GetAI_boss_saviana;
    newscript->RegisterSelf();
}
