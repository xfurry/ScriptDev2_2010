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
SDName: Boss_Kelidan_The_Breaker
SD%Complete: 60
SDComment: Event with channeleres vs. boss not implemented yet
SDCategory: Hellfire Citadel, Blood Furnace
EndScriptData */

/* ContentData
boss_kelidan_the_breaker
mob_shadowmoon_channeler
EndContentData */

#include "precompiled.h"
#include "blood_furnace.h"

enum
{
    SAY_WAKE                    = -1542000,
    SAY_ADD_AGGRO_1             = -1542001,
    SAY_ADD_AGGRO_2             = -1542002,
    SAY_ADD_AGGRO_3             = -1542003,
    SAY_KILL_1                  = -1542004,
    SAY_KILL_2                  = -1542005,
    SAY_NOVA                    = -1542006,
    SAY_DIE                     = -1542007,

    SPELL_CORRUPTION            = 30938,
	NPC_CHANNELER				= 17653,

    SPELL_FIRE_NOVA             = 33132,
    H_SPELL_FIRE_NOVA           = 37371,

    SPELL_SHADOW_BOLT_VOLLEY    = 28599,
    H_SPELL_SHADOW_BOLT_VOLLEY  = 40070,

	SPELL_CHANNELING			= 30935,                         //initial spell channeling boss/each other not known

    SPELL_BURNING_NOVA          = 30940,
    SPELL_VORTEX                = 37370
};

struct MANGOS_DLL_DECL boss_kelidan_the_breakerAI : public ScriptedAI
{
    boss_kelidan_the_breakerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bIsRegularMode;

    uint32 ShadowVolley_Timer;
    uint32 BurningNova_Timer;
    uint32 Firenova_Timer;
    uint32 Corruption_Timer;
    bool Firenova;

	uint32 m_uiChannelersDead;

    void Reset()
    {
        ShadowVolley_Timer = 1000;
        BurningNova_Timer = 15000;
        Corruption_Timer = 5000;
        Firenova = false;
		m_uiChannelersDead = 0;
		DoCast(m_creature, SPELL_CHANNELING);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_WAKE, m_creature);

		m_creature->CastStop();
		m_creature->GetMotionMaster()->MoveChase(who);
		if(m_pInstance)
			m_pInstance->SetData(TYPE_KELIDAN_EVENT, IN_PROGRESS);
    }

	void AttackStart(Unit* pWho)
	{
		if (m_uiChannelersDead < 5) 
			return;

		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho);
		}
	}

	void JustReachedHome()
	{
		std::list<Creature*> lChannelers;
		GetCreatureListWithEntryInGrid(lChannelers, m_creature, NPC_CHANNELER, DEFAULT_VISIBILITY_INSTANCE);
        if (!lChannelers.empty())
        {
            for(std::list<Creature*>::iterator iter = lChannelers.begin(); iter != lChannelers.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
	}

    void KilledUnit(Unit* victim)
    {
        if (urand(0, 1))
            return;

        DoScriptText(urand(0, 1) ? SAY_KILL_1 : SAY_KILL_2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DIE, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELIDAN_EVENT,DONE);
    }

    void UpdateAI(const uint32 diff)
    {
		if(m_pInstance->GetData(TYPE_KELIDAN_EVENT) == SPECIAL)
		{
			if(m_uiChannelersDead == 5)
			{
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->SetInCombatWithZone();
			}
		}

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Firenova)
        {
            if (Firenova_Timer < diff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FIRE_NOVA : H_SPELL_FIRE_NOVA);
                Firenova = false;
                ShadowVolley_Timer = 2000;
            }else Firenova_Timer -=diff;

            return;
        }

        if (ShadowVolley_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT_VOLLEY : H_SPELL_SHADOW_BOLT_VOLLEY);
            ShadowVolley_Timer = urand(5000, 13000);
        }else ShadowVolley_Timer -=diff;

        if (Corruption_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_CORRUPTION);
            Corruption_Timer = urand(30000, 50000);
        }else Corruption_Timer -=diff;

        if (BurningNova_Timer < diff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(true);

            DoScriptText(SAY_NOVA, m_creature);

            if (!m_bIsRegularMode)
                DoCastSpellIfCan(m_creature, SPELL_VORTEX);

            DoCastSpellIfCan(m_creature,SPELL_BURNING_NOVA);

            BurningNova_Timer = urand(20000, 28000);
            Firenova_Timer= 5000;
            Firenova = true;
        }else BurningNova_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kelidan_the_breaker(Creature* pCreature)
{
    return new boss_kelidan_the_breakerAI(pCreature);
}

/*######
## mob_shadowmoon_channeler
######*/

enum
{
    SPELL_SHADOW_BOLT       = 12739,
    H_SPELL_SHADOW_BOLT     = 15472,

    SPELL_MARK_OF_SHADOW    = 30937,
};                                                          //when engaged all channelers must stop, trigger yell (SAY_ADD_AGGRO_*), and engage.

struct MANGOS_DLL_DECL mob_shadowmoon_channelerAI : public ScriptedAI
{
    mob_shadowmoon_channelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 ShadowBolt_Timer;
    uint32 MarkOfShadow_Timer;

    void Reset()
    {
        ShadowBolt_Timer = urand(1000, 2000);
        MarkOfShadow_Timer = urand(5000, 7000);
    }

	void JustDied(Unit* pKiller)
	{
		if(m_pInstance->GetData(TYPE_KELIDAN_EVENT) != SPECIAL)
			m_pInstance->SetData(TYPE_KELIDAN_EVENT, SPECIAL);

		if(Creature* pKelidan = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KELIDAN_THE_MAKER)))
			((boss_kelidan_the_breakerAI*)pKelidan->AI())->m_uiChannelersDead += 1;
	}

    void Aggro(Unit* who)
    {
		m_creature->CastStop();
		m_creature->GetMotionMaster()->MoveChase(who);
		switch(urand(0, 3))
		{
		case 0: DoScriptText(SAY_ADD_AGGRO_1, m_creature); break;
		case 1: DoScriptText(SAY_ADD_AGGRO_2, m_creature); break;
		case 2: DoScriptText(SAY_ADD_AGGRO_3, m_creature); break;
		}

		std::list<Creature*> lChannelers;
		GetCreatureListWithEntryInGrid(lChannelers, m_creature, NPC_CHANNELER, DEFAULT_VISIBILITY_INSTANCE);
        if (!lChannelers.empty())
        {
            for(std::list<Creature*>::iterator iter = lChannelers.begin(); iter != lChannelers.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->AI()->AttackStart(who);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (MarkOfShadow_Timer < diff)
        {
            if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(target,SPELL_MARK_OF_SHADOW);
            MarkOfShadow_Timer = urand(15000, 20000);
        }else MarkOfShadow_Timer -=diff;

        if (ShadowBolt_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
            ShadowBolt_Timer = urand(5000, 6000);
        }else ShadowBolt_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shadowmoon_channeler(Creature* pCreature)
{
    return new mob_shadowmoon_channelerAI(pCreature);
}

void AddSC_boss_kelidan_the_breaker()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_kelidan_the_breaker";
    newscript->GetAI = &GetAI_boss_kelidan_the_breaker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadowmoon_channeler";
    newscript->GetAI = &GetAI_mob_shadowmoon_channeler;
    newscript->RegisterSelf();
}
