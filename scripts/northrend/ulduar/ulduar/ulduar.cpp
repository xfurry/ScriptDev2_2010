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
SDName: Ulduar teleport
SD%Complete:
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
- Spark of Imagination
- Prison of Yogg-Saron
*/

#define BASE_CAMP    200
#define GROUNDS      201
#define FORGE        202
#define SCRAPYARD    203
#define ANTECHAMBER  204
#define WALKWAY      205
#define CONSERVATORY 206
#define SPARK        207
#define PRISON       208

// spells
#define SPELL_TELEPORT_BASE_CAMP    64014
#define SPELL_TELEPORT_GROUNDS      64032
#define SPELL_TELEPORT_FORGE        64028
#define SPELL_TELEPORT_SCRAPYARD    64031
#define SPELL_TELEPORT_ANTECHAMBER  64030
#define SPELL_TELEPORT_WALKWAY      64029
#define SPELL_TELEPORT_CONSERVATORY 64024
#define SPELL_TELEPORT_SPARK        65061
#define SPELL_TELEPORT_PRISON       65042


bool GoHello_ulduar_teleporter( Player *pPlayer, GameObject *pGO )
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    // base camp
    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);

    // formation grounds & colossal forge
    if(pInstance->GetData(TYPE_IGNIS) == DONE && pInstance->GetData(TYPE_RAZORSCALE) == DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);


        // scrapyard & antechamber
        if(pInstance->GetData(TYPE_XT002) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
        }
    }

    // shattered walkway
    if(pInstance->GetData(TYPE_KOLOGARN) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);

    // conservatory of life
    if(pInstance->GetData(TYPE_AURIAYA) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);

    // spark of imagination
    if(pInstance->GetData(TYPE_MIMIRON) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Spark of Imagination", GOSSIP_SENDER_MAIN, SPARK);

    // prison of yogg saron
    if(pInstance->GetData(TYPE_VEZAX) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Prison of Yogg-Saron", GOSSIP_SENDER_MAIN, PRISON);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pGO->GetGUID());

    return true;
}

bool GoSelect_ulduar_teleporter( Player *pPlayer, GameObject *pGO, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!pPlayer->getAttackers().empty()) return true;

    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    switch(action)
    {
    case BASE_CAMP:
        //pGO->CastSpell(pPlayer, SPELL_TELEPORT_BASE_CAMP, true);
        pPlayer->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case GROUNDS:
        pPlayer->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case FORGE:
        pPlayer->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case SCRAPYARD:
        pPlayer->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case ANTECHAMBER:
        pPlayer->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case WALKWAY:
        pPlayer->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0); 
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case CONSERVATORY:
        pPlayer->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case SPARK:
        pPlayer->TeleportTo(603, 2536.87f, 2569.15f, 412.304f, 0);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case PRISON:
        pPlayer->TeleportTo(603, 1854.39f, -6.47f, 334.814f, 4.71f);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    }

    return true;
}

void AddSC_ulduar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "ulduar_teleporter";
    newscript->pGOHello = &GoHello_ulduar_teleporter;
    newscript->pGOGossipSelect = &GoSelect_ulduar_teleporter;
    newscript->RegisterSelf();
}