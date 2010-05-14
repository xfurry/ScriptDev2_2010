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
SDName: blood_prince_council
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    // yells
    // intro-> by lana'thel
    SAY_INTRO1              = -1609490,
    SAY_INTRO2              = -1609509,
    //keleseth
    SAY_KELESETH_BLOOD      = -1609491,
    SAY_KELESETH_SPECIAL    = -1609492,
    SAY_KELESETH_KILL1      = -1609493,
    SAY_KELESETH_KILL2      = -1609494,
    SAY_KELESETH_BERSERK    = -1609495,
    SAY_KELESETH_DEATH      = -1609496,
    //taldaram
    SAY_TALDARAM_BLOOD      = -1609497,
    SAY_TALDARAM_SPECIAL    = -1609498,
    SAY_TALDARAM_KILL1      = -1609499,
    SAY_TALDARAM_KILL2      = -1609500,
    SAY_TALDARAM_BERSERK    = -1609501,
    SAY_TALDARAM_DEATH      = -1609502,
    //valanar
    SAY_VALANAR_BLOOD       = -1609503,
    SAY_VALANAR_SPECIAL     = -1609504,
    SAY_VALANAR_KILL1       = -1609505,
    SAY_VALANAR_KILL2       = -1609506,
    SAY_VALANAR_BERSERK     = -1609507,
    SAY_VALANAR_DEATH       = -1609508,

    // spells
    //Darkfallen Orb
    SPELL_INVOCATION_OF_BLOOD               = 70952,

    //Valanar
    SPELL_KINETIC_BOMB                      = 72053,
    NPC_KINETIC_BOMB                        = 38458,
    SPELL_KINETIC_BOMB_EXPLODE              = 72052,
    SPELL_SHOCK_VORTEX                      = 72037,
    NPC_SHOCK_VORTEX                        = 38422,
    SPELL_SHOCK_VORTEX_DAMAGE               = 71944,
    SPELL_SHOCK_VORTEX_2                    = 72039,

    //Taldaram
    SPELL_GLITTERING_SPARKS                 = 71807,
    SPELL_CONJURE_FLAME                     = 71718,
    SPELL_FLAMES                            = 71393,
    SPELL_CONJURE_FLAME_2                   = 72040,
    SPELL_FLAMES_2                          = 71708,

    //Keleseth
    SPELL_SHADOW_LANCE                      = 71405,
    SPELL_SHADOW_LANCE_2                    = 71815,
    SPELL_SHADOW_RESONANCE                  = 71943,
    SPELL_SHADOW_RESONANCE_DAMAGE           = 71822,
    NPC_DARK_NUCLEUS                        = 38369,
};

struct MANGOS_DLL_DECL boss_prince_valanarAI : public ScriptedAI
{
    boss_prince_valanarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;


    void Reset() {
    if(!m_pInstance) return;
    //m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    //stage = 0;
    }
/*
    void JustReachedHome()
    {
    if (!m_pInstance) return;
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, FAIL);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void JustDied(Unit* pKiller)
    {
    if (!m_pInstance) return;
    if (pBrother1 && pBrother2 && !pBrother1->isAlive() && !pBrother2->isAlive()) 
    {
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, DONE);
    m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother1->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother2->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }
    else  m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }*/

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_VALANAR_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_VALANAR_KILL2, m_creature); break;
        }
    }

    /*void Aggro(Unit* pWho)
    {
    if (!m_pInstance) return;
    pBrother1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_TALDARAM));
    pBrother2 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_KELESETH));
    if (pBrother1 && !pBrother1->isAlive()) pBrother1->Respawn();
    if (pBrother2 && !pBrother2->isAlive()) pBrother2->Respawn();
    if (pBrother1) pBrother1->SetInCombatWithZone();
    if (pBrother2) pBrother2->SetInCombatWithZone();

    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, IN_PROGRESS);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
    if (!m_pInstance) return;
    if (!m_creature || !m_creature->isAlive())
    return;

    if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetHealth() >= uiDamage ? m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) - uiDamage : 0);

    uiDamage /=3;
    }*/

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*if (m_creature->GetHealth() > m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3 &&
        m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) != 0)
        m_creature->SetHealth(m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3);

        switch (stage)
        {
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
        default:
        break;
        }

        bsw->timedCast(SPELL_KINETIC_BOMB, uiDiff);

        bsw->timedCast(SPELL_SHOCK_VORTEX, uiDiff);*/

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_prince_valanar(Creature* pCreature)
{
    return new boss_prince_valanarAI(pCreature);
}

struct MANGOS_DLL_DECL boss_prince_taldaramAI : public ScriptedAI
{
    boss_prince_taldaramAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {
    if(!m_pInstance) return;
    //m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    //stage = 0;
    }
/*
    void JustReachedHome()
    {
    if (!m_pInstance) return;
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, FAIL);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void JustDied(Unit* pKiller)
    {
    if (!m_pInstance) return;
    if (pBrother1 && pBrother2 && !pBrother1->isAlive() && !pBrother2->isAlive()) 
    {
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, DONE);
    m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother1->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother2->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }
    else  m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }*/

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_TALDARAM_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_TALDARAM_KILL2, m_creature); break;
        }
    }

    /*void Aggro(Unit* pWho)
    {
    if (!m_pInstance) return;
    pBrother1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_VALANAR));
    pBrother2 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_KELESETH));
    if (pBrother1 && !pBrother1->isAlive()) pBrother1->Respawn();
    if (pBrother2 && !pBrother2->isAlive()) pBrother2->Respawn();
    if (pBrother1) pBrother1->SetInCombatWithZone();
    if (pBrother2) pBrother2->SetInCombatWithZone();

    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, IN_PROGRESS);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
    if (!m_pInstance) return;
    if (!m_creature || !m_creature->isAlive())
    return;

    if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetHealth() >= uiDamage ? m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) - uiDamage : 0);

    uiDamage /=3;
    }*/

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*if (m_creature->GetHealth() > m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3 &&
        m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) != 0)
        m_creature->SetHealth(m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3);

        switch (stage)
        {
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
        default:
        break;
        }

        bsw->timedCast(SPELL_GLITTERING_SPARKS, uiDiff);

        bsw->timedCast(SPELL_CONJURE_FLAME, uiDiff);

        bsw->timedCast(SPELL_FLAMES, uiDiff);*/

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_prince_taldaram(Creature* pCreature)
{
    return new boss_prince_taldaramAI(pCreature);
}

struct MANGOS_DLL_DECL boss_prince_kelesethAI : public ScriptedAI
{
    boss_prince_kelesethAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    void Reset() {
    if(!m_pInstance) return;
    //m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    //stage = 0;
    }
/*
    void JustReachedHome()
    {
    if (!m_pInstance) return;
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, FAIL);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void JustDied(Unit* pKiller)
    {
    if (!m_pInstance) return;
    if (pBrother1 && pBrother2 && !pBrother1->isAlive() && !pBrother2->isAlive()) 
    {
    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, DONE);
    m_creature->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother1->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    pBrother2->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }
    else  m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }*/

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_KELESETH_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_KELESETH_KILL2, m_creature); break;
        }
    }

    /*void Aggro(Unit* pWho)
    {
    if (!m_pInstance) return;
    pBrother1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_TALDARAM));
    pBrother2 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_VALANAR));
    if (pBrother1 && !pBrother1->isAlive()) pBrother1->Respawn();
    if (pBrother2 && !pBrother2->isAlive()) pBrother2->Respawn();
    if (pBrother1) pBrother1->SetInCombatWithZone();
    if (pBrother2) pBrother2->SetInCombatWithZone();

    m_pInstance->SetData(TYPE_BLOOD_COUNCIL, IN_PROGRESS);
    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetMaxHealth()*3);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
    if (!m_pInstance) return;
    if (!m_creature || !m_creature->isAlive())
    return;

    if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

    m_pInstance->SetData(DATA_BLOOD_COUNCIL_HEALTH, m_creature->GetHealth() >= uiDamage ? m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) - uiDamage : 0);

    uiDamage /=3;
    }*/

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*if (m_creature->GetHealth() > m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3 &&
        m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH) != 0)
        m_creature->SetHealth(m_pInstance->GetData(DATA_BLOOD_COUNCIL_HEALTH)/3);

        switch (stage)
        {
        case 0:
        break;
        case 1:
        break;
        case 2:
        break;
        default:
        break;
        }

        bsw->timedCast(SPELL_SHADOW_LANCE, uiDiff);

        bsw->timedCast(SPELL_SHADOW_RESONANCE, uiDiff);*/

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_prince_keleseth(Creature* pCreature)
{
    return new boss_prince_kelesethAI(pCreature);
}

void AddSC_blood_prince_council()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_prince_taldaram";
    newscript->GetAI = &GetAI_boss_prince_taldaram;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_prince_keleseth";
    newscript->GetAI = &GetAI_boss_prince_keleseth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_prince_valanar";
    newscript->GetAI = &GetAI_boss_prince_valanar;
    newscript->RegisterSelf();

}
