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
SDName: Instance_Trial_Of_the_Champion
SD%Complete: 100
SDComment: 
SDCategory: Trial Of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

struct MANGOS_DLL_DECL instance_trial_of_the_champion : public ScriptedInstance
{
    instance_trial_of_the_champion(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string m_strInstData;

    // grand champs
    // aly
    uint64 m_uiJacobGUID;
    uint64 m_uiAmbroseGUID;
    uint64 m_uiColososGUID;
    uint64 m_uiJaelyneGUID;
    uint64 m_uiLanaGUID;
    // horde
    uint64 m_uiMokraGUID;
    uint64 m_uiEresseaGUID;
    uint64 m_uiRunokGUID;
    uint64 m_uiZultoreGUID;
    uint64 m_uiVisceriGUID;
    // argent challenge
    uint64 m_uiEadricGUID;
    uint64 m_uiEadricLootGUID;
    uint64 m_uiPaletressGUID;
    uint64 m_uiPaletressLootGUID;
    uint32 m_uiArgentChallenger;
    // black knight
    uint64 m_uiBlackKnightGUID;
    // announcer
    uint64 m_uiJaerenGUID;
    uint64 m_uiArelasGUID;
    uint64 m_uiAnnouncerGUID;
    uint64 m_uiChampionsLootGUID;
    uint64 m_uiDoorGUID;
    uint32 m_auiStage;

    uint32 TeamInInstance;

    void Initialize()
    {
        // grand champs
        // aly
        m_uiJacobGUID           = 0;
        m_uiAmbroseGUID         = 0;
        m_uiColososGUID         = 0;
        m_uiJaelyneGUID         = 0;
        m_uiLanaGUID            = 0;
        // horde
        m_uiMokraGUID           = 0;
        m_uiEresseaGUID         = 0;
        m_uiRunokGUID           = 0;
        m_uiZultoreGUID         = 0;
        m_uiVisceriGUID         = 0;
        // argent challenge
        m_uiEadricGUID          = 0;
        m_uiEadricLootGUID      = 0;
        m_uiPaletressGUID       = 0;
        m_uiPaletressLootGUID   = 0;
        m_uiArgentChallenger    = 0;
        m_uiBlackKnightGUID     = 0;
        m_uiJaerenGUID          = 0;
        m_uiArelasGUID          = 0;
        m_uiAnnouncerGUID       = 0;
        m_uiChampionsLootGUID   = 0;
        m_auiStage              = 0;
        m_uiDoorGUID            = 0;

        TeamInInstance          = GetFaction();

        memset(m_auiEncounter, 0, sizeof(m_auiEncounter));
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;
        }

        return false;
    }

    uint32 GetFaction()
    {
        Map::PlayerList const &players = instance->GetPlayers();
        uint32 m_uiTeam = 0;

        if (!players.isEmpty())
        {
            if (Player* pPlayer = players.begin()->getSource())
                m_uiTeam = pPlayer->GetTeam();
        }
        return m_uiTeam;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            // Champions of the Alliance
            case NPC_JACOB:
                m_uiJacobGUID = pCreature->GetGUID();
                break;
            case NPC_AMBROSE:
                m_uiAmbroseGUID = pCreature->GetGUID();
                break;
            case NPC_COLOSOS:
                m_uiColososGUID = pCreature->GetGUID();
                break;
            case NPC_JAELYNE:
                m_uiJaelyneGUID = pCreature->GetGUID();
                break;
            case NPC_LANA:
                m_uiLanaGUID = pCreature->GetGUID();
                break;
            // Champions of the Horde
            case NPC_MOKRA:
                m_uiMokraGUID = pCreature->GetGUID();
                break;
            case NPC_ERESSEA:
                m_uiEresseaGUID = pCreature->GetGUID();
                break;
            case NPC_RUNOK:
                m_uiRunokGUID = pCreature->GetGUID();
                break;
            case NPC_ZULTORE:
                m_uiZultoreGUID = pCreature->GetGUID();
                break;
            case NPC_VISCERI:
                m_uiVisceriGUID = pCreature->GetGUID();
                break;

            // Argent Challenge
            case NPC_EADRIC:
                m_uiEadricGUID = pCreature->GetGUID();
                break;
            case NPC_PALETRESS:
                m_uiPaletressGUID = pCreature->GetGUID();
                break;

            // Black Knight
            case NPC_BLACK_KNIGHT:
                m_uiBlackKnightGUID = pCreature->GetGUID();
                break;

            // Coliseum Announcers
            case NPC_JAEREN:
                m_uiJaerenGUID = pCreature->GetGUID();
                if (TeamInInstance == HORDE)
                    m_uiAnnouncerGUID = pCreature->GetGUID();
                break;
            case NPC_ARELAS:
                m_uiArelasGUID = pCreature->GetGUID();
                if (TeamInInstance == ALLIANCE)
                    m_uiAnnouncerGUID = pCreature->GetGUID();
                break;
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_CHAMPIONS_LOOT:
                if(instance->IsRegularDifficulty())
                    m_uiChampionsLootGUID = pGo->GetGUID();
            case GO_CHAMPIONS_LOOT_H:
                if(!instance->IsRegularDifficulty())
                    m_uiChampionsLootGUID = pGo->GetGUID();
                break;
            case GO_EADRIC_LOOT:
                if(instance->IsRegularDifficulty())
                    m_uiEadricLootGUID = pGo->GetGUID();
            case GO_EADRIC_LOOT_H:
                if(!instance->IsRegularDifficulty())
                    m_uiEadricLootGUID = pGo->GetGUID();
                break;
            case GO_PALETRESS_LOOT:
                if(instance->IsRegularDifficulty())
                    m_uiPaletressLootGUID = pGo->GetGUID();
            case GO_PALETRESS_LOOT_H:
                if(!instance->IsRegularDifficulty())
                    m_uiPaletressLootGUID = pGo->GetGUID();
                break;   
            case GO_NORTH_GATE:
                m_uiDoorGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_GRAND_CHAMPIONS:
                m_auiEncounter[0] = uiData;
                DoUseDoorOrButton(m_uiDoorGUID);
                if (uiData == DONE)
                    DoRespawnGameObject(m_uiChampionsLootGUID, DAY);
                break;
            case TYPE_ARGENT_CHALLENGE:
                m_auiEncounter[1] = uiData;
                DoUseDoorOrButton(m_uiDoorGUID);
                if (uiData == DONE)
                {
                    if (m_uiArgentChallenger == NPC_EADRIC)
                        DoRespawnGameObject(m_uiEadricLootGUID, DAY);
                    if (m_uiArgentChallenger == NPC_PALETRESS)
                        DoRespawnGameObject(m_uiPaletressLootGUID, DAY);
                }
                break;
            case TYPE_BLACK_KNIGHT:
                m_auiEncounter[2] = uiData;
                DoUseDoorOrButton(m_uiDoorGUID);
                if(uiData == DONE)
                {
                    if(TeamInInstance == ALLIANCE)
                        DoCompleteAchievement(instance->IsRegularDifficulty() ? ACHIEV_TOC_ALY : ACHIEV_TOC_ALY_H);
                    if(TeamInInstance == HORDE)
                        DoCompleteAchievement(instance->IsRegularDifficulty() ? ACHIEV_TOC_HORDE : ACHIEV_TOC_HORDE_H);
                }
                break;
            case TYPE_STAGE:
                m_auiStage = uiData;
                break;
            case DATA_ARGENT_CHALLERGER:
                m_uiArgentChallenger = uiData;
                break;
        }

        if (uiData == DONE)
        {
            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2];
            m_strInstData = saveStream.str();

            OUT_SAVE_INST_DATA;
            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_GRAND_CHAMPIONS:
                return m_auiEncounter[0];
            case TYPE_ARGENT_CHALLENGE:
                return m_auiEncounter[1];
            case TYPE_BLACK_KNIGHT:
                return m_auiEncounter[2];
            case TYPE_STAGE:
                return m_auiStage;
            case DATA_ARGENT_CHALLERGER:
                return m_uiArgentChallenger;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiType)
    {
        switch(uiType)
        {
            // aly
        case NPC_JACOB:     return m_uiJacobGUID;   break;
        case NPC_AMBROSE:   return m_uiAmbroseGUID; break;
        case NPC_COLOSOS:   return m_uiColososGUID; break;
        case NPC_JAELYNE:   return m_uiJaelyneGUID; break;
        case NPC_LANA:      return m_uiLanaGUID;    break;
            // horde
        case NPC_MOKRA:     return m_uiMokraGUID;   break;
        case NPC_ERESSEA:   return m_uiEresseaGUID; break;
        case NPC_RUNOK:     return m_uiRunokGUID;   break;
        case NPC_ZULTORE:   return m_uiZultoreGUID; break;
        case NPC_VISCERI:   return m_uiVisceriGUID; break;

        case DATA_TOC_ANNOUNCER:
            return m_uiAnnouncerGUID;
        }

        return 0;
    }

    const char* Save()
    {
        return m_strInstData.c_str();
    }

    void Load(const char* strIn)
    {
        if (!strIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(strIn);

        std::istringstream loadStream(strIn);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_trial_of_the_champion(Map* pMap)
{
    return new instance_trial_of_the_champion(pMap);
}

void AddSC_instance_trial_of_the_champion()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_trial_of_the_champion";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_champion;
    newscript->RegisterSelf();
}
