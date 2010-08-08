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
SDName: boss_zarithian
SD%Complete:
SDComment:
SDCategory: ruby_sanctum
EndScriptData */

// Need correct timers

#include "precompiled.h"
#include "ruby_sanctum.h"

enum
{
	//yells
	SAY_AGGRO						= -1666200,
	SAY_SLAY1						= -1666201,
	SAY_SLAY2						= -1666202,
	SAY_DEATH						= -1666203,
	SAY_SUMMON						= -1666204,

    SPELL_CALL_FLAMECALLER          = 74398,
    SPELL_CLEAVE_ARMOR				= 74367, 
    SPELL_INTIMIDATING_ROAR			= 74384,

	// flamecaller
    SPELL_LAVA_GOUT                  = 74394,
    SPELL_BLAST_NOVA                 = 74392,

    NPC_FLAMECALLER                  = 39814,
};

static Locations SpawnLoc[]=
{
    {3008.552734f, 530.471680f, 89.195290f},     // 0 - Zarithian start point, o = 6,16
    {3014.313477f, 486.453735f, 89.255096f},    // 1 - Mob spawn 1
    {3025.324951f, 580.588501f, 88.593185f},    // 2 - Mob spawn 2
};

struct MANGOS_DLL_DECL boss_zarithrianAI : public ScriptedAI
{
    boss_zarithrianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiCleaveArmorTimer;
	uint32 m_uiIntimidatingRoarTimer;
	uint32 m_uiCallFlamecallerTimer;

    void Reset()
    {
		m_uiCleaveArmorTimer		= 5000;
		m_uiIntimidatingRoarTimer	= 12000;
		m_uiCallFlamecallerTimer	= 15000;   
    }

	void JustReachedHome()
	{
		if(m_pInstance)
			m_pInstance->SetData(TYPE_ZARITHRIAN, NOT_STARTED);
	}

	void KilledUnit(Unit* pVictim)
	{
		switch (urand(0,1)) 
		{
		case 0:
			DoScriptText(SAY_SLAY1,m_creature,pVictim);
			break;
		case 1:
			DoScriptText(SAY_SLAY2,m_creature,pVictim);
			break;
		}
	}

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->SetInCombatWithZone();
		DoScriptText(SAY_SUMMON, m_creature);
    }

    void Aggro(Unit *who) 
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_ZARITHRIAN, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_ZARITHRIAN, DONE);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiCleaveArmorTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				DoCast(pTarget, SPELL_CLEAVE_ARMOR);
			m_uiCleaveArmorTimer = 5000;
		}
		else m_uiCleaveArmorTimer -= uiDiff;

		if(m_uiIntimidatingRoarTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_INTIMIDATING_ROAR);
			m_uiIntimidatingRoarTimer = 30000;
		}
		else m_uiIntimidatingRoarTimer -= uiDiff;

		if(m_uiCallFlamecallerTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			//DoCast(m_creature, SPELL_CALL_FLAMECALLER);
			// should summon 3 mobs
			for (int i = 0; i < 3; ++i)
			{
				float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
                float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
				m_creature->SummonCreature(NPC_FLAMECALLER, homeX, homeY, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
			}
			m_uiCallFlamecallerTimer = 45000;
		}
		else m_uiCallFlamecallerTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zarithrian(Creature* pCreature)
{
    return new boss_zarithrianAI(pCreature);
};

struct MANGOS_DLL_DECL mob_flamecaller_rubyAI : public ScriptedAI
{
    mob_flamecaller_rubyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiLavaGoutTimer;
	uint32 m_uiBlastNovaTimer;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
		m_uiLavaGoutTimer	= 3000;
		m_uiBlastNovaTimer	= 7000;
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (m_pInstance && m_pInstance->GetData(TYPE_ZARITHRIAN) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiLavaGoutTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_LAVA_GOUT);
			m_uiLavaGoutTimer = 1000;
		}
		else m_uiLavaGoutTimer -= uiDiff;

		if(m_uiBlastNovaTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BLAST_NOVA);
			m_uiBlastNovaTimer = 6000;
		}
		else m_uiBlastNovaTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_flamecaller_ruby(Creature* pCreature)
{
    return new mob_flamecaller_rubyAI(pCreature);
};

void AddSC_boss_zarithrian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_zarithrian";
    newscript->GetAI = &GetAI_boss_zarithrian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flamecaller_ruby";
    newscript->GetAI = &GetAI_mob_flamecaller_ruby;
    newscript->RegisterSelf();
}
