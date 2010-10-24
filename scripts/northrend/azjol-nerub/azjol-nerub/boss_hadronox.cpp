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
SDName: Boss_Hadronox
SD%Complete: 20%
SDComment: Implement gauntlet event; fix web grab
SDCategory: Azjol'Nerub
EndScriptData */

//Phase 0 => Gauntlet Event described below
//Phase 1 => Boss Encounter

/*When your group enters the main room (the one after the bridge), you will notice a group of 3 Nerubians.
When you engage them, 2 more groups like this one spawn behind the first one - it is important to pull the first group back,
so you don't aggro all 3. Hadronox will be under you, fighting Nerubians.

This is the timed gauntlet - waves of non-elite spiders
 will spawn from the 3 doors located a little above the main room, and will then head down to fight Hadronox. After clearing the
 main room, it is recommended to just stay in it, kill the occasional non-elites that will attack you instead of the boss, and wait for
 Hadronox to make his way to you. When Hadronox enters the main room, she will web the doors, and no more non-elites will spawn.*/

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_ACID_CLOUD        = 53400, // Victim
    SPELL_LEECH_POISON      = 53030, // Victim
    SPELL_PIERCE_ARMOR      = 53418, // Victim
    SPELL_WEB_GRAB          = 57731, // Victim
    SPELL_WEB_FRONT_DOORS   = 53177, // Self
    SPELL_WEB_SIDE_DOORS    = 53185, // Self

    SPELL_ACID_CLOUD_H      = 59419,
    SPELL_LEECH_POISON_H    = 59417,
    SPELL_WEB_GRAB_H        = 59421, 

    ACHIEV_HADRONOX_DENIED  = 1297,     //needs script support
};

/*######
## boss_hadronox
######*/

struct MANGOS_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;

    uint32 acidtimer;
    uint32 leechtimer;
    uint32 piercetimer;
    uint32 grabtimer;
    uint32 doorstimer;

    void Reset()
    {
        acidtimer   = urand(10000,14000);
        leechtimer  = urand(3000,9000);
        piercetimer = urand(1000,3000);
        grabtimer   = urand(15000,19000);
        doorstimer  = urand(20000,30000);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HADRONOX, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HADRONOX, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        m_creature->SetHealth(m_creature->GetHealth() + (m_creature->GetMaxHealth() * 0.1));
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HADRONOX, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Without he comes up through the air to players on the bridge after krikthir if players crossing this bridge!
        if (m_creature->HasAura(SPELL_WEB_FRONT_DOORS) || m_creature->HasAura(SPELL_WEB_SIDE_DOORS))
        {
            if (IsCombatMovement())
                SetCombatMovement(false);
        }
        else if (!IsCombatMovement())
            SetCombatMovement(true);


        if (piercetimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_PIERCE_ARMOR);
            piercetimer = 8000;
        } else piercetimer -= uiDiff;


        if (acidtimer <= uiDiff)
        {
             if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_ACID_CLOUD : SPELL_ACID_CLOUD_H) == CAST_OK)
                    acidtimer = urand(20000,30000);
            }
        } else acidtimer -= uiDiff;


        if (leechtimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_LEECH_POISON : SPELL_LEECH_POISON_H) == CAST_OK)
                    leechtimer = urand(11000,14000);
            }   
        } else leechtimer -= uiDiff;


        if (grabtimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WEB_GRAB : SPELL_WEB_GRAB_H) == CAST_OK)
                grabtimer = urand(15000,30000);
        } else grabtimer -= uiDiff;


        if (doorstimer <= uiDiff)
        {/* Debuff bleibt auf den Spielern bis zum Tod...
            DoCast(m_creature, RAND(SPELL_WEB_FRONT_DOORS, SPELL_WEB_SIDE_DOORS));
            */
            doorstimer = urand(30000,60000);
        } else doorstimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI(pCreature);
}

void AddSC_boss_hadronox()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hadronox";
    pNewScript->GetAI = &GetAI_boss_hadronox;
    pNewScript->RegisterSelf();
}
