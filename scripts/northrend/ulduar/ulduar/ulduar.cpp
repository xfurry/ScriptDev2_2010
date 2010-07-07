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
enum
{
    BASE_CAMP    = 200,
    GROUNDS      = 201,
    FORGE        = 202,
    SCRAPYARD    = 203,
    ANTECHAMBER  = 204,
    WALKWAY      = 205,
    CONSERVATORY = 206,
    SPARK        = 207,
    PRISON       = 208,

    // spells
    SPELL_TELEPORT_BASE_CAMP    = 64014,
    SPELL_TELEPORT_GROUNDS      = 64032,
    SPELL_TELEPORT_FORGE        = 64028,
    SPELL_TELEPORT_SCRAPYARD    = 64031,
    SPELL_TELEPORT_ANTECHAMBER  = 64030,
    SPELL_TELEPORT_WALKWAY      = 64029,
    SPELL_TELEPORT_CONSERVATORY = 64024,
    SPELL_TELEPORT_SPARK        = 65061,
    SPELL_TELEPORT_PRISON       = 65042,
};

#define REQUEST_HELP    "Help me fight Yogg-Saron!"
#define DENY_HELP       "I don't need your help."

bool GoHello_ulduar_teleporter( Player *pPlayer, GameObject *pGO )
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    // base camp
    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);

    // formation grounds
    if(pInstance->GetData(TYPE_LEVIATHAN_TP) == DONE)
    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);

    // colossal forge -> disabled check because of the missing leviathan
    //if(pInstance->GetData(TYPE_LEVIATHAN) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);

        // remove this after leviathan implemented; now ignis & razor = mandatory
    if(pInstance->GetData(TYPE_IGNIS) == DONE && pInstance->GetData(TYPE_RAZORSCALE) == DONE)
    {
        // scrapyard
        if(pInstance->GetData(TYPE_XT002_TP) == DONE)
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);

        // antechamber
        if(pInstance->GetData(TYPE_XT002) == DONE)
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
    }

    // shattered walkway
    if(pInstance->GetData(TYPE_KOLOGARN) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);

    // conservatory of life
    if(pInstance->GetData(TYPE_AURIAYA) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);

    // spark of imagination
    if(pInstance->GetData(TYPE_MIMIRON_TP) == DONE)
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

/*#######
*### Keepers images
#######*/
// HODIR
bool GossipHello_hodir_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_HODIR) != DONE && m_pInstance->GetData(TYPE_KEEPER_HODIR) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_hodir_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_HODIR, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_HODIR, FAIL);
    }
    return true;
}

// FREYA
bool GossipHello_freya_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_FREYA) != DONE && m_pInstance->GetData(TYPE_KEEPER_FREYA) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_freya_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_FREYA, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_FREYA, FAIL);
    }
    return true;
}
// MIMIRON
bool GossipHello_mimiron_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_MIMIRON) != DONE && m_pInstance->GetData(TYPE_KEEPER_MIMIRON) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_mimiron_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_MIMIRON, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_MIMIRON, FAIL);
    }
    return true;
}

// THORIM
bool GossipHello_thorim_image(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_KEEPER_THORIM) != DONE && m_pInstance->GetData(TYPE_KEEPER_THORIM) != FAIL)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, REQUEST_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DENY_HELP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_thorim_image(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    pPlayer->CLOSE_GOSSIP_MENU();

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_THORIM, DONE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KEEPER_THORIM, FAIL);
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

    newscript = new Script;
    newscript->Name = "hodir_image";
    newscript->pGossipHello = &GossipHello_hodir_image;
    newscript->pGossipSelect = &GossipSelect_hodir_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "freya_image";
    newscript->pGossipHello = &GossipHello_freya_image;
    newscript->pGossipSelect = &GossipSelect_freya_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "thorim_image";
    newscript->pGossipHello = &GossipHello_thorim_image;
    newscript->pGossipSelect = &GossipSelect_thorim_image;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mimiron_image";
    newscript->pGossipHello = &GossipHello_mimiron_image;
    newscript->pGossipSelect = &GossipSelect_mimiron_image;
    newscript->RegisterSelf();

}