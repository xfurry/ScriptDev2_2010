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
	SPELL_PLANAR_DISTORSION						  = 59379,
	SPELL_PLANAR_ANOMALY_AGGRO					  = 57971,

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
	uint32 m_uiSummonWhelpsTimer;
	uint32 m_uiHpPercent;

	void Reset()
	{
		m_uiArcaneBarrageTimer	= 7000;
		m_uiArcaneVolleyTimer	= 15000;
		m_uiEnrageTimer			= 25000;
		m_uiSummonWhelpsTimer	= 35000;
		m_uiHpPercent			= 60;

		m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
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

	void AttackStart(Unit* pWho)
    {
		if(m_pInstance->GetData(TYPE_UROM) != DONE)
			return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
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

		if(m_creature->GetHealthPercent() < m_uiHpPercent)
		{
			switch(urand(0, 2))
			{
			case 0: DoScriptText(SAY_ARCANE_SHIELD, m_creature); break;
			case 1: DoScriptText(SAY_FIRE_SHIELD, m_creature); break;
			case 2: DoScriptText(SAY_NATURE_SHIELD, m_creature); break;
			}
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_PLANAR_SHIFT);
			for(uint8 i = 0; i < 5; i++)
			{
				if(Creature* pTemp = m_creature->SummonCreature(NPC_PLANAR_ANOMALY, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
				{
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
						pTemp->GetMotionMaster()->MoveChase(pTarget);
				}
			}
			// set next phase to 20%
			m_uiHpPercent -= 40;
		}

		if(m_creature->HasAura(SPELL_PLANAR_SHIFT, EFFECT_INDEX_0))
			return;

		if(m_uiArcaneBarrageTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE : SPELL_ARCANE_BARRAGE_H);
			m_uiArcaneBarrageTimer = urand(3000, 5000);
		}
		else m_uiArcaneBarrageTimer -= uiDiff;

		if(m_uiSummonWhelpsTimer < uiDiff)
		{
			for(uint8 i = 0; i < 5; i++)
			{
				if(Creature* pTemp = m_creature->SummonCreature(NPC_GREATER_LEY_WHELP, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
					pTemp->SetInCombatWithZone();
			}
			m_uiSummonWhelpsTimer = 30000;
		}
		else m_uiSummonWhelpsTimer -= uiDiff;

		if(m_uiArcaneVolleyTimer < uiDiff)
		{
			DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_VOLLEY : SPELL_ARCANE_VOLLEY_H);
			m_uiArcaneVolleyTimer = urand(12000, 15000);
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

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_eregos(Creature* pCreature)
{
	return new boss_eregosAI (pCreature);
}

struct MANGOS_DLL_DECL npc_planar_anomalyAI : public ScriptedAI
{
	npc_planar_anomalyAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		m_creature->SetDisplayId(11686);
		Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiExplodeTimer;

	void Reset()
	{
		DoCast(m_creature, SPELL_PLANAR_DISTORSION);
		m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
		m_uiExplodeTimer = 18000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(!m_creature->HasAura(SPELL_PLANAR_ANOMALY_AGGRO, EFFECT_INDEX_0))
			DoCast(m_creature, SPELL_PLANAR_ANOMALY_AGGRO);
		
		if(m_uiExplodeTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_PLANAR_BLAST);
			m_uiExplodeTimer = 20000;
		}
		else m_uiExplodeTimer -= uiDiff;
	}
};

CreatureAI* GetAI_npc_planar_anomaly(Creature* pCreature)
{
	return new npc_planar_anomalyAI (pCreature);
}

void AddSC_boss_eregos()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "boss_eregos";
	newscript->GetAI = &GetAI_boss_eregos;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_planar_anomaly";
	newscript->GetAI = &GetAI_npc_planar_anomaly;
	newscript->RegisterSelf();
}
