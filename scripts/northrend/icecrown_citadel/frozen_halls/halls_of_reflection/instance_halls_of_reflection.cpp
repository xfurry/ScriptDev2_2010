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
#include "halls_of_reflection.h"

struct MANGOS_DLL_DECL instance_halls_of_reflection : public ScriptedInstance
{
    instance_halls_of_reflection(Map* pMap) : ScriptedInstance(pMap) 
    {
        Regular = pMap->IsRegularDifficulty();
        Initialize();
    }

    bool Regular;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiFalrynGUID;
    uint64 m_uiMarwynGUID;
    uint64 m_uiLichKingGUID;
    uint64 m_uiJainaGUID;
    uint64 m_uiSylvanasGUID;
    uint64 m_uiJainaIntroGUID;
    uint64 m_uiSylvanasIntroGUID;

    uint64 m_uiCaptainsChestGUID;
    uint64 m_uiIcecrownDoorGUID;
    uint64 m_uiImpenetrableDoorGUID;
    uint64 m_uiFrostmourneGUID;
    uint64 m_uiFrostmourneAltarGUID;
    uint64 m_uiShadowThroneDoorGUID;
    uint64 m_uiShadowThroneExitDoorGUID;

    void OpenDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_ACTIVE);
    }

    void CloseDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_READY);
    }

    void Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_uiFalrynGUID              = 0;
        m_uiMarwynGUID              = 0;
        m_uiLichKingGUID            = 0;
        m_uiJainaGUID               = 0;
        m_uiSylvanasGUID            = 0;
        m_uiCaptainsChestGUID       = 0;
        m_uiIcecrownDoorGUID        = 0;
        m_uiImpenetrableDoorGUID    = 0;
        m_uiFrostmourneGUID         = 0;
        m_uiFrostmourneAltarGUID    = 0;
        m_uiShadowThroneDoorGUID    = 0;
        m_uiShadowThroneExitDoorGUID= 0;
        m_uiJainaIntroGUID          = 0;
        m_uiSylvanasIntroGUID       = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
        case NPC_FALRYN: 
            m_uiFalrynGUID = pCreature->GetGUID();
            break;
        case NPC_MARWYN: 
            m_uiMarwynGUID = pCreature->GetGUID();
            break;
        case NPC_LICH_KING: 
            m_uiLichKingGUID = pCreature->GetGUID();
            break;
        case NPC_JAINA_END:
            m_uiJainaGUID = pCreature->GetGUID();
            break;
        case NPC_SYLVANAS_END:
            m_uiSylvanasGUID = pCreature->GetGUID();
            break;
        case NPC_JAINA_START:
            m_uiJainaIntroGUID = pCreature->GetGUID();
            break;
        case NPC_SYLVANAS_START:
            m_uiSylvanasIntroGUID = pCreature->GetGUID();
            break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        Map::PlayerList const &players = instance->GetPlayers();
        uint32 TeamInInstance = 0;

        if (!players.isEmpty())
        {
            if (Player* pPlayer = players.begin()->getSource())
                TeamInInstance = pPlayer->GetTeam();
        }

        switch(pGo->GetEntry())
        {
        case GO_CAPTAIN_CHEST_HORDE:
            if(Regular && TeamInInstance == HORDE) 
                m_uiCaptainsChestGUID = pGo->GetGUID();
            break;
        case GO_CAPTAIN_CHEST_ALY:
            if(Regular && TeamInInstance == ALLIANCE) 
                m_uiCaptainsChestGUID = pGo->GetGUID();
            break;
        case GO_CAPTAIN_CHEST_HORDE_H:
            if(!Regular && TeamInInstance == HORDE) 
                m_uiCaptainsChestGUID = pGo->GetGUID();
            break;
        case GO_CAPTAIN_CHEST_ALY_H:
            if(!Regular && TeamInInstance == ALLIANCE) 
                m_uiCaptainsChestGUID = pGo->GetGUID();
            break;
        case GO_ICECROWN_DOOR:     
            m_uiIcecrownDoorGUID = pGo->GetGUID(); 
            break;
        case GO_IMPENETRABLE_DOOR: 
            m_uiImpenetrableDoorGUID = pGo->GetGUID();
            if(m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            else
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_FROSTMOURNE:      
            m_uiFrostmourneGUID = pGo->GetGUID(); 
            break;
        case GO_FROSTMOURNE_ALTAR: 
            m_uiFrostmourneAltarGUID = pGo->GetGUID();
            break;
        case GO_SHADOW_THRONE_DOOR:
            m_uiShadowThroneDoorGUID = pGo->GetGUID();
            if(m_auiEncounter[3] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            else
                pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_SHADOW_THROME_DOOR_EXIT:
            m_uiShadowThroneExitDoorGUID = pGo->GetGUID();
            if(m_auiEncounter[4] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            else
                pGo->SetGoState(GO_STATE_READY);
            break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case TYPE_INTRO:   
            m_auiEncounter[0] = uiData;
            break;
        case TYPE_FALRIC:        
            m_auiEncounter[1] = uiData; 
            if(uiData == NOT_STARTED)
                OpenDoor(m_uiIcecrownDoorGUID);
            if(uiData == IN_PROGRESS || uiData == SPECIAL)
                CloseDoor(m_uiIcecrownDoorGUID);
            break;
        case TYPE_MARWYN:        
            m_auiEncounter[2] = uiData;
            if(uiData == NOT_STARTED)
                OpenDoor(m_uiIcecrownDoorGUID);
            if(uiData == IN_PROGRESS || uiData == SPECIAL)
                CloseDoor(m_uiIcecrownDoorGUID);
            if(uiData == DONE)
            {
                OpenDoor(m_uiImpenetrableDoorGUID);
                OpenDoor(m_uiIcecrownDoorGUID);
            }
            break;
        case TYPE_FROST_GENERAL:
            m_auiEncounter[3] = uiData;
            if(uiData == DONE)
                OpenDoor(m_uiShadowThroneDoorGUID);
            break;
        case TYPE_ESCAPE:        
            m_auiEncounter[4] = uiData; 
            if(uiData == NOT_STARTED)
                CloseDoor(m_uiShadowThroneExitDoorGUID);
            if(uiData == DONE)
                DoRespawnGameObject(m_uiCaptainsChestGUID);
            break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];
            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_INTRO:   
            return m_auiEncounter[0];
        case TYPE_FALRIC:        
            return m_auiEncounter[1];
        case TYPE_MARWYN:        
            return m_auiEncounter[2];
        case TYPE_FROST_GENERAL:
            return m_auiEncounter[3];
        case TYPE_ESCAPE:        
            return m_auiEncounter[4];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
        case NPC_FALRYN:            return  m_uiFalrynGUID;
        case NPC_MARWYN:            return  m_uiMarwynGUID;
        case NPC_LICH_KING:         return m_uiLichKingGUID;
        case DATA_ICECROWN_DOOR:    return m_uiIcecrownDoorGUID;
        case DATA_SHADOW_DOOR:      return m_uiShadowThroneDoorGUID;
        case DATA_SHADOW_DOOR_EXIT: return m_uiShadowThroneExitDoorGUID;
        case DATA_FROSTMOURNE:      return m_uiFrostmourneGUID;
        case DATA_IMPENETRABLE_DOOR: return m_uiImpenetrableDoorGUID;
        case DATA_FALRIC:           return m_uiFalrynGUID;
        case DATA_MARWYN:           return m_uiMarwynGUID;
        case DATA_JAINA:            return m_uiJainaGUID;
        case DATA_SYLVANAS:         return m_uiSylvanasGUID;
        case DATA_LICH_KING:        return m_uiLichKingGUID;
        case DATA_JAINA_INTRO:      return m_uiJainaIntroGUID;
        case DATA_SYLVANAS_INTRO:   return m_uiSylvanasIntroGUID;
        case DATA_ALTAR:            return m_uiFrostmourneAltarGUID;
        }
        return 0;
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];
        
        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            if (m_auiEncounter[i] != DONE)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
        OpenDoor(m_uiIcecrownDoorGUID);
    }
};

InstanceData* GetInstanceData_instance_halls_of_reflection(Map* pMap)
{
    return new instance_halls_of_reflection(pMap);
}

void AddSC_instance_halls_of_reflection()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_halls_of_reflection";
    newscript->GetInstanceData = &GetInstanceData_instance_halls_of_reflection;
    newscript->RegisterSelf();
}
