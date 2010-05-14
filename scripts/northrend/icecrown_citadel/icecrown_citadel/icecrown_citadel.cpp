/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: ICC teleporter & minibosses
SD%Complete:
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

/*######
## icecrown_teleporters
######*/
enum locations
{
    LIGHTS_HAMMER       = 200,
    ORATORY_OF_DAMMED   = 201,
    RAMPART_OF_SKULLS   = 202,
    DEATHBRINGERS_RISE  = 203,
    UPPER_SPIRE         = 204,
    SINDRAGOSA          = 205,
    FROZEN_THRONE       = 206,
};

// spells
enum teleport
{
    SPELL_TELEPORT_LIGHTS_HAMMER    = 70781,
    SPELL_TELEPORT_ORATORY          = 70856,
    SPELL_TELEPORT_SKULLS_RAMPART   = 70857,
    SPELL_TELEPORT_DEATHBRINGER     = 70858,
    SPELL_TELEPORT_UPPER_SPIRE      = 70859,
    SPELL_TELEPORT_SINDRAGOSAS_LAIR = 70861,
    SPELL_TELEPORT_FROZEN_THRONE    = 70860,
};

bool GoHello_icecrown_citadel_teleporter( Player *pPlayer, GameObject *pGO )
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    // Lights Hammer
    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Light's Hammer", GOSSIP_SENDER_MAIN, LIGHTS_HAMMER);

    // Oratory of the dammed
    if(pInstance->GetData(TYPE_MARROWGAR) == DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Oratory of the Dammed", GOSSIP_SENDER_MAIN, ORATORY_OF_DAMMED);

        // Rampart of Skulls
        if(pInstance->GetData(TYPE_DEATHWHISPER) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Rampart of Skulls", GOSSIP_SENDER_MAIN, RAMPART_OF_SKULLS);

            // Deathbringer's rise
            if(pInstance->GetData(TYPE_GUNSHIP_BATTLE) == DONE)
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Deathbringer's Rise", GOSSIP_SENDER_MAIN, DEATHBRINGERS_RISE);

                if(pInstance->GetData(TYPE_SAURFANG) == DONE)
                    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Upper Spire", GOSSIP_SENDER_MAIN, UPPER_SPIRE);
            }
        }
    }

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pGO->GetGUID());

    return true;
}

bool GoSelect_icecrown_citadel_teleporter( Player *pPlayer, GameObject *pGO, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!pPlayer->getAttackers().empty()) return true;

    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    switch(action)
    {
    case LIGHTS_HAMMER:
        //pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_LIGHTS_HAMMER, false);
        pPlayer->TeleportTo(631, -16.729f, 2211.418f, 30.115f, 3.123f);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case ORATORY_OF_DAMMED:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_ORATORY, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case RAMPART_OF_SKULLS:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_SKULLS_RAMPART, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case DEATHBRINGERS_RISE:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_DEATHBRINGER, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case UPPER_SPIRE:
        //pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_UPPER_SPIRE, false);
        if(pInstance->GetData(TYPE_FESTERGUT) == DONE || pInstance->GetData(TYPE_ROTFACE) == DONE)
            pPlayer->TeleportTo(631, 4199.149f, 2769.339f, 351.064f, 6.275f);
        else
            pPlayer->TeleportTo(631, 4199.149f, 2769.339f, 351.064f, 6.275f);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case SINDRAGOSA:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_SINDRAGOSAS_LAIR, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case FROZEN_THRONE:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FROZEN_THRONE, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    }
    return true;
}

/*######
## stinky & precious
######*/
enum stinky
{
    SAY_DEATH_STINKY        = -1609471,
    SAY_DEATH_PRECIOUS      = -1609460,

    SPELL_DECIMATE          = 71123,
    SPELL_MORTAL_WOUND      = 71127,
    SPELL_PLAGUE_STENCH     = 71805,
    SPELL_PRECIOUS_RIBBON   = 70404,
};

struct MANGOS_DLL_DECL miniboss_stinkyAI : public ScriptedAI
{
    miniboss_stinkyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;

    void Reset()
    {
        m_uiMortalWoundTimer    = 3000;
        m_uiDecimateTimer       = 10000;

        DoCast(m_creature, SPELL_PLAGUE_STENCH);
    }

    void JustDied(Unit *killer)
    {
        if (Creature* pFestergut = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_FESTERGUT))))
            DoScriptText(SAY_DEATH_STINKY, pFestergut);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_WOUND);
            m_uiMortalWoundTimer = urand(5000, 10000);
        }
        else
            m_uiMortalWoundTimer -= uiDiff;

        if (m_uiDecimateTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DECIMATE);
            m_uiDecimateTimer = 30000;
        }
        else
            m_uiDecimateTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_miniboss_stinky(Creature* pCreature)
{
    return new miniboss_stinkyAI(pCreature);
}

struct MANGOS_DLL_DECL miniboss_preciousAI : public ScriptedAI
{
    miniboss_preciousAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;

    void Reset()
    {
        m_uiMortalWoundTimer    = 3000;
        m_uiDecimateTimer       = 10000;

        DoCast(m_creature, SPELL_PRECIOUS_RIBBON);
    }

    void JustDied(Unit *killer)
    {
        if (Creature* pRotface = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_ROTFACE))))
            DoScriptText(SAY_DEATH_PRECIOUS, pRotface);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_WOUND);
            m_uiMortalWoundTimer = urand(5000, 10000);
        }
        else
            m_uiMortalWoundTimer -= uiDiff;

        if (m_uiDecimateTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DECIMATE);
            m_uiDecimateTimer = 30000;
        }
        else
            m_uiDecimateTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_miniboss_precious(Creature* pCreature)
{
    return new miniboss_preciousAI(pCreature);
}

/*######
## Val'kyr Herald: TODO;
######*/

/*######
## Decaying Colossus: TODO;
######*/

/*######
## sister svalna: TODO;
######*/

/*######
## trash mobs
######*/
enum
{
    // spire frostwyrm
    SPELL_BERSERK                           = 47008,
    SPELL_FROST_BREATH                      = 70116,
    SPELL_FROST_BREATH_H                    = 72641,
    SPELL_BLIZZARD                          = 70362,
    SPELL_BLIZZARD_H                        = 71118,
    SPELL_SOUL_FEAST                        = 71203,
    SPELL_CLEAVE                            = 70361,

    // rotting frost giant
    SPELL_STOMP                             = 64652,
    SPELL_DEATH_PLAGUE                      = 72865,
};

struct MANGOS_DLL_DECL mob_spire_frostwyrmAI : public ScriptedAI
{
    mob_spire_frostwyrmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCleaveTimer;
    uint32 m_uiBreathTimer;
    uint32 m_uiBlizzardTimer;
    bool m_bHasEnraged;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_uiCleaveTimer     = 5000;
        m_uiBreathTimer     = 12000;
        m_uiBlizzardTimer   = 15000;
        m_bHasEnraged       = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(20000, 25000);
        }
        else m_uiCleaveTimer -= uiDiff;

        if(m_uiBreathTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FROST_BREATH : SPELL_FROST_BREATH_H);
            m_uiBreathTimer = urand(20000, 25000);
        }
        else m_uiBreathTimer -= uiDiff;

        if(m_uiBlizzardTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, m_bIsRegularMode ? SPELL_BLIZZARD : SPELL_BLIZZARD_H);
            m_uiBlizzardTimer = urand(20000, 25000);
        }
        else m_uiBlizzardTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 2.0f && !m_bHasEnraged)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasEnraged = true;
        }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_spire_frostwyrm(Creature* pCreature)
{
    return new mob_spire_frostwyrmAI(pCreature);
}

struct MANGOS_DLL_DECL mob_rotting_frost_giantAI : public ScriptedAI
{
    mob_rotting_frost_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStompTimer;
    uint32 m_uiDeathPlagueTimer;
    bool m_bHasEnraged;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_uiStompTimer          = 10000;
        m_uiDeathPlagueTimer    = 20000;
        m_bHasEnraged           = false;
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiStompTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_STOMP);
            m_uiStompTimer = urand(10000, 15000);
        }
        else m_uiStompTimer -= uiDiff;

        if(m_uiDeathPlagueTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_DEATH_PLAGUE);
            m_uiDeathPlagueTimer = urand(20000, 25000);
        }
        else m_uiDeathPlagueTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 2.0f && !m_bHasEnraged)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasEnraged = true;
        }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_rotting_frost_giant(Creature* pCreature)
{
    return new mob_rotting_frost_giantAI(pCreature);
}

void AddSC_icecrown_citadel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "icecrown_citadel_teleporter";
    newscript->pGOHello = &GoHello_icecrown_citadel_teleporter;
    newscript->pGOGossipSelect = &GoSelect_icecrown_citadel_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "miniboss_stinky";
    newscript->GetAI = &GetAI_miniboss_stinky;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "miniboss_precious";
    newscript->GetAI = &GetAI_miniboss_precious;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spire_frostwyrm";
    newscript->GetAI = &GetAI_mob_spire_frostwyrm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_rotting_frost_giant";
    newscript->GetAI = &GetAI_mob_rotting_frost_giant;
    newscript->RegisterSelf();
}