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

enum Spells
{
	// yells
	SAY_AGGRO                                     = -1578013,
    SAY_SLAY1                                     = -1578038,
    SAY_SLAY2                                     = -1578039,
    SAY_DEATH                                     = -1578017,
    SAY_STRIKE1                                   = -1578014,
    SAY_STRIKE2                                   = -1578015,
    SAY_STRIKE3                                   = -1578016,
	
	// spells
    SPELL_ENERGIZE_CORES                        = 50785,
    SPELL_ENERGIZE_CORES_H                      = 59372,
    SPELL_CALL_AZURE_RING_CAPTAIN               = 51002,
    SPELL_AMPLIFY_MAGIC							= 51054,
    SPELL_AMPLIFY_MAGIC_H						= 59371,

	SPELL_CENTRIFUGE_SHIELD						= 50053,
	SPELL_CENTRIFUGE_CORE_PASSIVE				= 50798,
	NPC_CENTRIFUGE_CORE							= 28183,
	NPC_CENTRIFUGE_CONSTRUCT					= 27641,


	// channeling
	// 54219, 
};

struct MANGOS_DLL_DECL boss_varosAI : public ScriptedAI
{
	boss_varosAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		Reset();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 m_uiAmplifyMagicTimer;
	uint32 m_uiCallCaptainTimer;
	uint32 m_uiEnergizeCoreTimer;

	void Reset()
	{
		m_uiAmplifyMagicTimer	= 10000;
		m_uiCallCaptainTimer	= 15000;
		m_uiEnergizeCoreTimer	= 20000;
		//if(m_pInstance->GetData(TYPE_DRAKOS) != DONE)
		//	DoCast(m_creature, SPELL_CENTRIFUGE_SHIELD);
	}

	void JustReachedHome()
	{
		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAROS, NOT_STARTED);
	}

	void Aggro(Unit* pWho)
	{
		DoScriptText(SAY_AGGRO, m_creature);

		if (m_pInstance)
			m_pInstance->SetData(TYPE_VAROS, IN_PROGRESS);
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_creature->HasAura(SPELL_CENTRIFUGE_SHIELD, EFFECT_INDEX_0))
			uiDamage = 0;
	}

	void AttackStart(Unit* pWho)
    {
		if(m_creature->HasAura(SPELL_CENTRIFUGE_SHIELD, EFFECT_INDEX_0))
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
			m_pInstance->SetData(TYPE_VAROS, DONE);
	}

	void KilledUnit(Unit* pVictim)
	{
		switch(urand(0, 1))
		{
		case 0: DoScriptText(SAY_SLAY1, m_creature); break;
		case 1: DoScriptText(SAY_SLAY2, m_creature); break;
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiAmplifyMagicTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, m_bIsRegularMode ? SPELL_AMPLIFY_MAGIC : SPELL_AMPLIFY_MAGIC_H);
			m_uiAmplifyMagicTimer = 20000;
		}
		else m_uiAmplifyMagicTimer -= uiDiff;

		if(m_uiCallCaptainTimer < uiDiff)
		{
			switch(urand(0, 2))
			{
			case 0: DoScriptText(SAY_STRIKE1, m_creature); break;
			case 1: DoScriptText(SAY_STRIKE2, m_creature); break;
			case 2: DoScriptText(SAY_STRIKE3, m_creature); break;
			}
			// todo: needs advanced script
			DoCast(m_creature, SPELL_CALL_AZURE_RING_CAPTAIN);
			m_uiCallCaptainTimer = 13000;
		}
		else m_uiCallCaptainTimer -= uiDiff;

		if(m_uiEnergizeCoreTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, m_bIsRegularMode ? SPELL_ENERGIZE_CORES : SPELL_ENERGIZE_CORES_H);
			m_uiEnergizeCoreTimer = 20000;
		}
		else m_uiEnergizeCoreTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

struct MANGOS_DLL_DECL npc_centrifuge_sphereAI : public ScriptedAI
{
	npc_centrifuge_sphereAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		Reset();
	}

	ScriptedInstance* m_pInstance;

	void Reset()
	{
		DoCast(m_creature, SPELL_CENTRIFUGE_CORE_PASSIVE);
	}
};

CreatureAI* GetAI_boss_varos(Creature* pCreature)
{
	return new boss_varosAI (pCreature);
}

CreatureAI* GetAI_npc_centrifuge_sphere(Creature* pCreature)
{
	return new npc_centrifuge_sphereAI (pCreature);
}

void AddSC_boss_varos()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "boss_varos";
	newscript->GetAI = &GetAI_boss_varos;
	newscript->RegisterSelf();

	newscript = new Script;
	newscript->Name = "npc_centrifuge_sphere";
	newscript->GetAI = &GetAI_npc_centrifuge_sphere;
	newscript->RegisterSelf();
}