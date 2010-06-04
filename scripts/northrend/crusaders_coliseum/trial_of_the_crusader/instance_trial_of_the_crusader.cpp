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
SDName: instance_trial_of_the_crusader
SD%Complete: 100
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

/* Trial Of The Crusader encounters:
0 - Northrend Beasts
1 - Jaraxxus
2 - Faction Champions
3 - Twin Valkyr
4 - Anubarak
*/

struct MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance
{
    instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap) 
    {
        Difficulty = pMap->GetDifficulty();
        Initialize();
    };

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 Difficulty;
    bool m_bNeedSave;
    std::string strInstData;

    uint32 m_uiTwinsCasting;

    uint32 m_uiGormokGUID;
    uint32 m_uiAcidmawGUID;
    uint32 m_uiDreadscaleGUID;
    uint32 m_uiIcehowlGUID;
    uint32 m_uiJaraxxusGUID;
    uint32 m_uiFjolaGUID;
    uint32 m_uiEydisGUID;
    uint32 m_uiAnubarakGUID;

    uint64 m_uiBarrentGUID;
    uint64 m_uiTirionGUID;
    uint64 m_uiFizzlebangGUID;
    uint64 m_uiGarroshGUID;
    uint64 m_uiVarianGUID;
    uint64 m_uiLichGUID;
    uint64 m_uiJainaGUID;
    uint64 m_uiThrallGUID;

    uint64 m_uiCrusader11Guid;
    uint64 m_uiCrusader12Guid;
    uint64 m_uiCrusader13Guid;
    uint64 m_uiCrusader14Guid;
    uint64 m_uiCrusader15Guid;
    uint64 m_uiCrusader16Guid;
    uint64 m_uiCrusader17Guid;
    uint64 m_uiCrusader18Guid;
    uint64 m_uiCrusader19Guid;
    uint64 m_uiCrusader1aGuid;

    uint64 m_uiCrusader21Guid;
    uint64 m_uiCrusader22Guid;
    uint64 m_uiCrusader23Guid;
    uint64 m_uiCrusader24Guid;
    uint64 m_uiCrusader25Guid;
    uint64 m_uiCrusader26Guid;
    uint64 m_uiCrusader27Guid;
    uint64 m_uiCrusader28Guid;
    uint64 m_uiCrusader29Guid;
    uint64 m_uiCrusader2aGuid;

    uint64 m_uiCrusader01Guid;
    uint64 m_uiCrusader02Guid;

    uint64 m_uiCrusadersCacheGUID;
    uint64 m_uiFloorGUID;

    uint64 m_uiTC10h25GUID;
    uint64 m_uiTC10h45GUID;
    uint64 m_uiTC10h50GUID;
    uint64 m_uiTC10h99GUID;

    uint64 m_uiTC25h25GUID;
    uint64 m_uiTC25h45GUID;
    uint64 m_uiTC25h50GUID;
    uint64 m_uiTC25h99GUID;

    uint64 m_uiTributeChestGUID;

    uint64 m_uiMainGateDoorGUID;
    uint64 m_uiSouthDoorGUID;
    uint64 m_uiWestDoorGUID;
    uint64 m_uiNorthDoorGUID;
    uint64 m_uiWebDoorGUID;


    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_auiEncounter[6]  = 50;
        m_auiEncounter[7]  = 0;
        m_bNeedSave        = false;

        m_uiTwinsCasting   = 0;

        m_uiGormokGUID     = 0;
        m_uiAcidmawGUID    = 0;
        m_uiDreadscaleGUID = 0;
        m_uiIcehowlGUID    = 0;
        m_uiJaraxxusGUID   = 0;
        m_uiFjolaGUID      = 0;
        m_uiEydisGUID      = 0;
        m_uiAnubarakGUID   = 0;

        m_uiBarrentGUID     = 0;
        m_uiTirionGUID      = 0;
        m_uiFizzlebangGUID  = 0;
        m_uiGarroshGUID     = 0;
        m_uiVarianGUID      = 0;
        m_uiLichGUID        = 0;
        m_uiJainaGUID       = 0;
        m_uiThrallGUID      = 0;

        m_uiCrusader11Guid  = 0;
        m_uiCrusader12Guid  = 0;
        m_uiCrusader13Guid  = 0;
        m_uiCrusader14Guid  = 0;
        m_uiCrusader15Guid  = 0;
        m_uiCrusader16Guid  = 0;
        m_uiCrusader17Guid  = 0;
        m_uiCrusader18Guid  = 0;
        m_uiCrusader19Guid  = 0;
        m_uiCrusader1aGuid  = 0;

        m_uiCrusader21Guid  = 0;
        m_uiCrusader22Guid  = 0;
        m_uiCrusader23Guid  = 0;
        m_uiCrusader24Guid  = 0;
        m_uiCrusader25Guid  = 0;
        m_uiCrusader26Guid  = 0;
        m_uiCrusader27Guid  = 0;
        m_uiCrusader28Guid  = 0;
        m_uiCrusader29Guid  = 0;
        m_uiCrusader2aGuid  = 0;

        m_uiCrusader01Guid  = 0;
        m_uiCrusader02Guid  = 0;

        m_uiCrusadersCacheGUID  = 0;
        m_uiFloorGUID       = 0;

        m_uiTC10h25GUID     = 0;
        m_uiTC10h45GUID     = 0;
        m_uiTC10h50GUID     = 0;
        m_uiTC10h99GUID     = 0;

        m_uiTC25h25GUID     = 0;
        m_uiTC25h45GUID     = 0;
        m_uiTC25h50GUID     = 0;
        m_uiTC25h99GUID     = 0;

        m_uiTributeChestGUID    = 0;

        m_uiMainGateDoorGUID    = 0;
        m_uiSouthDoorGUID       = 0;
        m_uiWestDoorGUID        = 0;
        m_uiNorthDoorGUID       = 0;
        m_uiWebDoorGUID         = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnPlayerEnter(Player *m_player)
    {
        if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
        {
            m_player->SendUpdateWorldState(UPDATE_STATE_UI_SHOW,1);
            m_player->SendUpdateWorldState(UPDATE_STATE_UI_COUNT, GetData(TYPE_COUNTER));
        }
    }

    bool IsRaidWiped()
    {
        Map::PlayerList const &players = instance->GetPlayers();

        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
        {
            if(Player* pPlayer = i->getSource())
            {
                if(pPlayer->isAlive())
                    return false;
            }
        }
        return true;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            // boss
        case NPC_GORMOK:    m_uiGormokGUID          = pCreature->GetGUID(); break;
        case NPC_ACIDMAW:   m_uiAcidmawGUID         = pCreature->GetGUID(); break;
        case NPC_DREADSCALE: m_uiDreadscaleGUID     = pCreature->GetGUID(); break;
        case NPC_ICEHOWL:   m_uiIcehowlGUID         = pCreature->GetGUID(); break;
        case NPC_JARAXXUS:  m_uiJaraxxusGUID        = pCreature->GetGUID(); break;
        case NPC_FJOLA:     m_uiFjolaGUID           = pCreature->GetGUID(); break;
        case NPC_EYDIS:     m_uiEydisGUID           = pCreature->GetGUID(); break;
        case NPC_ANUBARAK:  m_uiAnubarakGUID        = pCreature->GetGUID(); break;

            // cosmetics
        case NPC_BARRENT:     m_uiBarrentGUID       = pCreature->GetGUID(); break;
        case NPC_TIRION:      m_uiTirionGUID        = pCreature->GetGUID(); break;
        case NPC_WILFRED:     m_uiFizzlebangGUID    = pCreature->GetGUID(); break;
        case NPC_GARROSH:     m_uiGarroshGUID       = pCreature->GetGUID(); break;
        case NPC_VARIAN:      m_uiVarianGUID        = pCreature->GetGUID(); break;
        case NPC_LICH_KING:   m_uiLichGUID          = pCreature->GetGUID(); break;
        case NPC_JAINA:       m_uiJainaGUID         = pCreature->GetGUID(); break;
        case NPC_THRALL:      m_uiThrallGUID        = pCreature->GetGUID(); break;

            // crusaders
        case NPC_CRUSADER_1_1: m_uiCrusader11Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_2: m_uiCrusader12Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_3: m_uiCrusader13Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_4: m_uiCrusader14Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_5: m_uiCrusader15Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_6: m_uiCrusader16Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_7: m_uiCrusader17Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_8: m_uiCrusader18Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_9: m_uiCrusader19Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_1_10: m_uiCrusader1aGuid = pCreature->GetGUID(); break;

        case NPC_CRUSADER_2_1: m_uiCrusader21Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_2: m_uiCrusader22Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_3: m_uiCrusader23Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_4: m_uiCrusader24Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_5: m_uiCrusader25Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_6: m_uiCrusader26Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_7: m_uiCrusader27Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_8: m_uiCrusader28Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_9: m_uiCrusader29Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_2_10: m_uiCrusader2aGuid = pCreature->GetGUID(); break;

        case NPC_CRUSADER_0_1: m_uiCrusader01Guid = pCreature->GetGUID(); break;
        case NPC_CRUSADER_0_2: m_uiCrusader02Guid = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_CRUSADERS_CACHE_10:
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                m_uiCrusadersCacheGUID = pGo->GetGUID(); 
            break;
        case GO_CRUSADERS_CACHE_25:
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                m_uiCrusadersCacheGUID = pGo->GetGUID(); 
            break;
        case GO_CRUSADERS_CACHE_10_H:
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                m_uiCrusadersCacheGUID = pGo->GetGUID(); 
            break;
        case GO_CRUSADERS_CACHE_25_H:
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                m_uiCrusadersCacheGUID = pGo->GetGUID(); 
            break;
        case GO_COLISEUM_FLOOR: 
            m_uiFloorGUID = pGo->GetGUID(); 
            break;
        case GO_MAIN_GATE:
            m_uiMainGateDoorGUID = pGo->GetGUID();
            break;
        case GO_SOUTH_GATE: m_uiSouthDoorGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_WEST_GATE: m_uiWestDoorGUID = pGo->GetGUID();
            break;
        case GO_NORTH_GATE: m_uiNorthDoorGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            break;
        case GO_WEB_DOOR: m_uiWebDoorGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_TRIBUTE_CHEST_10H_25: m_uiTC10h25GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_10H_45: m_uiTC10h45GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_10H_50: m_uiTC10h50GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_10H_99: m_uiTC10h99GUID = pGo->GetGUID(); break;

        case GO_TRIBUTE_CHEST_25H_25: m_uiTC25h25GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_25H_45: m_uiTC25h45GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_25H_50: m_uiTC25h50GUID = pGo->GetGUID(); break;
        case GO_TRIBUTE_CHEST_25H_99: m_uiTC25h99GUID = pGo->GetGUID(); break;
        }
    }

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

    void SetData(uint32 uiType, uint32 uiData)
    {
        debug_log("SD2: Instance Trial Of The Crusader: SetData received for type %u with data %u",uiType,uiData);

        switch(uiType)
        {
        case TYPE_NORTHREND_BEASTS:
            m_auiEncounter[0] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiWestDoorGUID);
            else
                OpenDoor(m_uiWestDoorGUID);
            break;
        case TYPE_JARAXXUS:
            m_auiEncounter[1] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiWestDoorGUID);
            else
                OpenDoor(m_uiWestDoorGUID);
            break;
        case TYPE_FACTION_CHAMPIONS:
            m_auiEncounter[2] = uiData;
            if (uiData == DONE)
                DoRespawnGameObject(m_uiCrusadersCacheGUID, 10*MINUTE);
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiWestDoorGUID);
            else
                OpenDoor(m_uiWestDoorGUID);
            break;
        case TYPE_TWIN_VALKYR:
            m_auiEncounter[3] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiWestDoorGUID);
            else
                OpenDoor(m_uiWestDoorGUID);
            break;
        case TYPE_ANUBARAK:
            m_auiEncounter[4] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiWebDoorGUID);
            else
                OpenDoor(m_uiWebDoorGUID);
            if (uiData == DONE) 
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCompleteAchievement(ACHIEV_CALL_CRUSADER);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCompleteAchievement(ACHIEV_CALL_CRUSADER_H);

                // init crusader chest for 10 man
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    DoCompleteAchievement(ACHIEV_CALL_GRAND_CRUSADER);
                    if(m_auiEncounter[6] >= 1 && m_auiEncounter[6] < 25)
                        m_uiTributeChestGUID = m_uiTC10h25GUID;
                    else if(m_auiEncounter[6] >= 25 && m_auiEncounter[6] < 45)
                    {
                        m_uiTributeChestGUID = m_uiTC10h45GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_SKILL);
                    }
                    else if(m_auiEncounter[6] >= 45 && m_auiEncounter[6] < 50)
                    {
                        m_uiTributeChestGUID = m_uiTC10h50GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_MAD_SKILL);
                    }
                    else if(m_auiEncounter[6] == 50)
                    {
                        m_uiTributeChestGUID = m_uiTC10h99GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_INSANITY);
                    }
                }
                // init crusader chest for 25 man
                else if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    DoCompleteAchievement(ACHIEV_CALL_GRAND_CRUSADER_H);
                    if(m_auiEncounter[6] >= 1 && m_auiEncounter[6] < 25)
                        m_uiTributeChestGUID = m_uiTC25h25GUID;
                    else if(m_auiEncounter[6] >= 25 && m_auiEncounter[6] < 45)
                    {
                        m_uiTributeChestGUID = m_uiTC25h45GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_SKILL_H);
                    }
                    else if(m_auiEncounter[6] >= 45 && m_auiEncounter[6] < 50)
                    {
                        m_uiTributeChestGUID = m_uiTC25h50GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_MAD_SKILL_H);
                    }
                    else if(m_auiEncounter[6] == 50)
                    {
                        m_uiTributeChestGUID = m_uiTC25h99GUID;
                        DoCompleteAchievement(ACHIEV_TRIBUTE_INSANITY_H);
                    }
                }
                // spawn chest if exists
                if(m_uiTributeChestGUID != 0)
                    DoRespawnGameObject(m_uiTributeChestGUID, 30* MINUTE);
            };
            break;
        case TYPE_INTRO:     
            m_auiEncounter[5] = uiData; 
            break;
        case TYPE_COUNTER:   
            m_auiEncounter[6] = uiData; 
            m_bNeedSave = true;
            break;
        case TYPE_STAGE:     
            m_auiEncounter[7] = uiData; 
            break;
        case TYPE_TWINS_CASTING:
            m_uiTwinsCasting = uiData;
            break;
        default:
            error_log("SD2: Instance Trial of The Crusader: ERROR SetData = %u for type %u does not exist/not implemented.",uiType,uiData);
            break;
        }

        if (uiData == DONE || uiData == FAIL || m_bNeedSave)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
            m_bNeedSave = false;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_NORTHREND_BEASTS:  return m_auiEncounter[0];
        case TYPE_JARAXXUS:          return m_auiEncounter[1];
        case TYPE_FACTION_CHAMPIONS: return m_auiEncounter[2];
        case TYPE_TWIN_VALKYR:       return m_auiEncounter[3];
        case TYPE_ANUBARAK:          return m_auiEncounter[4];

        case TYPE_INTRO:             return m_auiEncounter[5];
        case TYPE_COUNTER:           return m_auiEncounter[6];
        case TYPE_STAGE:             return m_auiEncounter[7];

        case TYPE_TWINS_CASTING:     return m_uiTwinsCasting;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
        case DATA_GORMOK:     return m_uiGormokGUID;
        case DATA_ACIDMAW:    return m_uiAcidmawGUID;
        case DATA_DREADSCALE: return m_uiDreadscaleGUID;
        case DATA_ICEHOWL:    return m_uiIcehowlGUID;
        case DATA_JARAXXUS:   return m_uiJaraxxusGUID;
        case DATA_FJOLA:      return m_uiFjolaGUID;
        case DATA_EYDIS:      return m_uiEydisGUID;
        case DATA_ANUBARAK:   return m_uiAnubarakGUID;

         case DATA_BARRET:   return m_uiBarrentGUID;
         case DATA_TIRION:   return m_uiTirionGUID;
         case DATA_GARROSH:  return m_uiGarroshGUID;
         case DATA_VARIAN:   return m_uiVarianGUID;
         case DATA_JAINA:    return m_uiJainaGUID;
         case DATA_THRALL:   return m_uiThrallGUID;

         case NPC_CRUSADER_1_1: return m_uiCrusader11Guid;
         case NPC_CRUSADER_1_2: return m_uiCrusader12Guid;
         case NPC_CRUSADER_1_3: return m_uiCrusader13Guid;
         case NPC_CRUSADER_1_4: return m_uiCrusader14Guid;
         case NPC_CRUSADER_1_5: return m_uiCrusader15Guid;
         case NPC_CRUSADER_1_6: return m_uiCrusader16Guid;
         case NPC_CRUSADER_1_7: return m_uiCrusader17Guid;
         case NPC_CRUSADER_1_8: return m_uiCrusader18Guid;
         case NPC_CRUSADER_1_9: return m_uiCrusader19Guid;
         case NPC_CRUSADER_1_10: return m_uiCrusader1aGuid;

         case NPC_CRUSADER_2_1: return m_uiCrusader21Guid;
         case NPC_CRUSADER_2_2: return m_uiCrusader22Guid;
         case NPC_CRUSADER_2_3: return m_uiCrusader23Guid;
         case NPC_CRUSADER_2_4: return m_uiCrusader24Guid;
         case NPC_CRUSADER_2_5: return m_uiCrusader25Guid;
         case NPC_CRUSADER_2_6: return m_uiCrusader26Guid;
         case NPC_CRUSADER_2_7: return m_uiCrusader27Guid;
         case NPC_CRUSADER_2_8: return m_uiCrusader28Guid;
         case NPC_CRUSADER_2_9: return m_uiCrusader29Guid;
         case NPC_CRUSADER_2_10: return m_uiCrusader2aGuid;

         case NPC_CRUSADER_0_1: return m_uiCrusader01Guid;
         case NPC_CRUSADER_0_2: return m_uiCrusader02Guid;

         case DATA_ANNOUNCER:   return m_uiBarrentGUID;
         case DATA_FLOOR:       return m_uiFloorGUID;
         case DATA_MAIN_GATE:   return m_uiMainGateDoorGUID;
        }
        return 0;
    }

    const char* Save()
    {
        return strInstData.c_str();
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6];

        m_auiEncounter[7] = 0;

        if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_auiEncounter[6]);

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)            // Do not load an encounter as "In Progress" - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap)
{
    return new instance_trial_of_the_crusader(pMap);
}

void AddSC_instance_trial_of_the_crusader()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "instance_trial_of_the_crusader";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader;
    newscript->RegisterSelf();
}
