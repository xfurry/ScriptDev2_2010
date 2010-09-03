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
	SAY_SPAWN									= -1578028,
	SAY_AGGRO									= -1578029,
	SAY_ARCANE_SHIELD							= -1578030,
	SAY_FIRE_SHIELD								= -1578031,
	SAY_NATURE_SHIELD							= -1578032,
	SAY_FRENZY									= -1578033,
	SAY_SLAY1									= -1578034,
	SAY_SLAY2									= -1578035,
	SAY_SLAY3									= -1578036,
	SAY_DEATH									= -1578037,

	// spells
    SPELL_ARCANE_BARRAGE                          = 50804,
    SPELL_ARCANE_BARRAGE_H                        = 59381,
    SPELL_ARCANE_VOLLEY                           = 51153,
    SPELL_ARCANE_VOLLEY_H                         = 59382,
    SPELL_ENRAGED_ASSAULT                         = 51170,
    SPELL_PLANAR_ANOMALIES                        = 57959,
	SPELL_PLANAR_ANOMALIES_SUMMON				  = 57963,
    SPELL_PLANAR_SHIFT                            = 51162,

	NPC_PLANAR_ANOMALY							  = 30879,
	SPELL_PLANAR_BLAST							  = 57976,

	NPC_GREATER_LEY_WHELP						  = 28276,
};


struct MANGOS_DLL_DECL boss_eregosAI : public ScriptedAI
{
	boss_eregosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiArcaneBarrageTimer;
	uint32 m_uiArcaneVolleyTimer;
	uint32 m_uiEnrageTimer;
	uint32 m_uiAnomaliesTimer;
	uint32 m_uiShiftEndTimer;

	void Reset()
	{
		m_uiArcaneBarrageTimer	= 7000;
		m_uiArcaneVolleyTimer	= 15000;
		m_uiEnrageTimer			= 25000;
		m_uiAnomaliesTimer		= 40000;
		m_uiShiftEndTimer		= 60000;

		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
	}

	void JustReachedHome()
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_EREGOS, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);
		if (m_pInstance)
			m_pInstance->SetData(TYPE_EREGOS, IN_PROGRESS);
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_DEATH, m_creature);
		if (m_pInstance)
			m_pInstance->SetData(TYPE_EREGOS, DONE);
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

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiArcaneBarrageTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE : SPELL_ARCANE_BARRAGE_H);
			m_uiArcaneBarrageTimer = urand(7000, 9000);
		}
		else m_uiArcaneBarrageTimer -= uiDiff;

		if(m_uiArcaneVolleyTimer < uiDiff)
		{
			DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_VOLLEY : SPELL_ARCANE_VOLLEY_H);
			m_uiArcaneVolleyTimer = urand(17000, 20000);
		}
		else m_uiArcaneVolleyTimer -= uiDiff;

		if(m_uiEnrageTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_FRENZY, m_creature);
			DoCast(m_creature, SPELL_ENRAGED_ASSAULT);
			m_uiEnrageTimer = urand(20000, 30000);
		}
		else m_uiEnrageTimer -= uiDiff;

		if(!m_bIsRegularMode)
		{
			/* todo: 
			at 60 and 20% he enters planar shift and summons anomalies
			also summons whelps
			if(m_uiAnomaliesTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoCast(m_creature, SPELL_PLANAR_SHIFT);
				m_uiShiftEndTimer = 18000;
				m_uiAnomaliesTimer = 40000;
			}
			else m_uiAnomaliesTimer -= uiDiff;

			if(m_uiShiftEndTimer < uiDiff)
			{
				DoCast(m_creature, SPELL_PLANAR_ANOMALIES);
				m_uiShiftEndTimer = 40000;
			}
			else m_uiShiftEndTimer -= uiDiff;
			*/
		}

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_eregos(Creature* pCreature)
{
	return new boss_eregosAI (pCreature);
}

void AddSC_boss_eregos()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "boss_eregos";
	newscript->GetAI = &GetAI_boss_eregos;
	newscript->RegisterSelf();
}
