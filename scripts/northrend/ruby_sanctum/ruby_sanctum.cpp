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
SDName: ruby_sanctum mobs
SD%Complete:
SDComment:
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

/*######
### Trash mobs
###### */

// charscale invoker 40417
enum
{
	SPELL_FLAME_WAVE			= 75413,
	SPELL_SCORCH				= 75412,
};

struct MANGOS_DLL_DECL mob_charscale_invokerAI : public ScriptedAI
{
    mob_charscale_invokerAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
	ScriptedInstance* m_pInstance;

    uint32 m_uiFlameWaveTimer;
	uint32 m_uiScorchTimer;
    bool m_uiAggro;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiFlameWaveTimer = 10000;
        m_uiScorchTimer = 3000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
			if (m_pInstance->GetData(TYPE_ZARITHRIAN) == IN_PROGRESS)
            {
				if (Unit* pZarithrian = Unit::GetUnit(*m_creature,m_pInstance->GetData64(NPC_ZARITHRIAN)))
                {
                    if (pZarithrian->getVictim())
                    {
                        m_creature->AI()->AttackStart(pZarithrian->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiFlameWaveTimer <= uiDiff)
        {
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_FLAME_WAVE);
            m_uiFlameWaveTimer = 10000;
        }else m_uiFlameWaveTimer -= uiDiff;
 
        if (m_uiScorchTimer <= uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SCORCH);
            m_uiScorchTimer = 3000;
        }else m_uiScorchTimer -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};
 
CreatureAI* GetAI_mob_charscale_invoker(Creature* pCreature)
{
    return new mob_charscale_invokerAI(pCreature);
}

// charscale assaulter	40419
enum
{
	SPELL_CLEAVE				= 15284,
	SPELL_SHOCKWAVE				= 75417,
};

struct MANGOS_DLL_DECL mob_charscale_assaulterAI : public ScriptedAI
{
    mob_charscale_assaulterAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
	ScriptedInstance* m_pInstance;

    uint32 m_uiCleaveTimer;
	uint32 m_uiSchockwaveTimer;
    bool m_uiAggro;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiSchockwaveTimer = 10000;
        m_uiCleaveTimer = 3000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
			if (m_pInstance->GetData(TYPE_ZARITHRIAN) == IN_PROGRESS)
            {
				if (Unit* pZarithrian = Unit::GetUnit(*m_creature,m_pInstance->GetData64(NPC_ZARITHRIAN)))
                {
                    if (pZarithrian->getVictim())
                    {
                        m_creature->AI()->AttackStart(pZarithrian->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiSchockwaveTimer <= uiDiff)
        {
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_SHOCKWAVE);
            m_uiSchockwaveTimer = 10000;
        }else m_uiSchockwaveTimer -= uiDiff;
 
        if (m_uiCleaveTimer <= uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 3000;
        }else m_uiCleaveTimer -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_charscale_assaulter(Creature* pCreature)
{
    return new mob_charscale_assaulterAI(pCreature);
}

// charscale commander	40423
enum
{
	SPELL_MORTAL_STRIKE			= 13737,
	SPELL_RALLYING_SHOUT		= 75414,
};

struct MANGOS_DLL_DECL mob_charscale_commanderAI : public ScriptedAI
{
    mob_charscale_commanderAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
	ScriptedInstance* m_pInstance;

    uint32 m_uiMortalStrikeTimer;
    bool m_uiAggro;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
		DoCast(m_creature, SPELL_RALLYING_SHOUT);
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiMortalStrikeTimer = 3000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
			if (m_pInstance->GetData(TYPE_ZARITHRIAN) == IN_PROGRESS)
            {
				if (Unit* pZarithrian = Unit::GetUnit(*m_creature,m_pInstance->GetData64(NPC_ZARITHRIAN)))
                {
                    if (pZarithrian->getVictim())
                    {
                        m_creature->AI()->AttackStart(pZarithrian->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiMortalStrikeTimer <= uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
            m_uiMortalStrikeTimer = 3000;
        }else m_uiMortalStrikeTimer -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_charscale_commander(Creature* pCreature)
{
    return new mob_charscale_commanderAI(pCreature);
}

// charscale elite	40421
enum
{
	SPELL_SKULL_CRACK			= 15621,
};

struct MANGOS_DLL_DECL mob_charscale_eliteAI : public ScriptedAI
{
    mob_charscale_eliteAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
	ScriptedInstance* m_pInstance;

    uint32 m_uiSkullCrackTimer;
    bool m_uiAggro;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiSkullCrackTimer = 3000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
			if (m_pInstance->GetData(TYPE_ZARITHRIAN) == IN_PROGRESS)
            {
				if (Unit* pZarithrian = Unit::GetUnit(*m_creature,m_pInstance->GetData64(NPC_ZARITHRIAN)))
                {
                    if (pZarithrian->getVictim())
                    {
                        m_creature->AI()->AttackStart(pZarithrian->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiSkullCrackTimer <= uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_SKULL_CRACK);
            m_uiSkullCrackTimer = 10000;
        }else m_uiSkullCrackTimer -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_charscale_elite(Creature* pCreature)
{
    return new mob_charscale_eliteAI(pCreature);
}

void AddSC_ruby_sanctum()
{
    Script *newscript;

    

	newscript = new Script;
    newscript->Name = "mob_charscale_invoker";
    newscript->GetAI = &GetAI_mob_charscale_invoker;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_charscale_assaulter";
    newscript->GetAI = &GetAI_mob_charscale_assaulter;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_charscale_commander";
    newscript->GetAI = &GetAI_mob_charscale_commander;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_charscale_elite";
    newscript->GetAI = &GetAI_mob_charscale_elite;
    newscript->RegisterSelf();
}
