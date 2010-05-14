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
SDName: Instance_Blackwing_Lair
SD%Complete: 0
SDComment:
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"


struct MANGOS_DLL_DECL instance_blackwing_lair : public ScriptedInstance
{
    instance_blackwing_lair(Map* pMap) : ScriptedInstance(pMap) 
    {
        Initialize();
    }

    std::string strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];

    // npcs
    uint64 m_uiRazorgoreGUID;
    uint64 m_uiVaelastraszGUID;
    uint64 m_uiLashlayerGUID;
    uint64 m_uiFiremawGUID;
    uint64 m_uiEbonrocGUID;
    uint64 m_uiFlamegorGUID;
    uint64 m_uiChromaggusGUID;
    uint64 m_uiNefarianGUID;
    uint64 m_uiLordVictorGUID;

    // doors
    uint64 m_uiRazorgoreEnterDoorGUID;
    uint64 m_uiRazorgoreExitDoorGUID;
    uint64 m_uiVaelastraszDoorGUID;
    uint64 m_uiLashlayerDoorGUID;
    uint64 m_uiFlamegorDoorGUID;
    uint64 m_uiChromaggusExitDoorGUID;
    uint64 m_uiChromaggusSideDoorGUID;
    uint64 m_uiNefarianDoorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        // npcs
        m_uiRazorgoreGUID       = 0;
        m_uiVaelastraszGUID     = 0;
        m_uiLashlayerGUID       = 0;
        m_uiFiremawGUID         = 0;
        m_uiEbonrocGUID         = 0;
        m_uiFlamegorGUID        = 0;
        m_uiChromaggusGUID      = 0;
        m_uiNefarianGUID        = 0;
        m_uiLordVictorGUID      = 0;

        // doors
        m_uiRazorgoreEnterDoorGUID  = 0;
        m_uiRazorgoreExitDoorGUID   = 0;
        m_uiVaelastraszDoorGUID     = 0;
        m_uiLashlayerDoorGUID       = 0;
        m_uiFlamegorDoorGUID        = 0;
        m_uiChromaggusExitDoorGUID  = 0;
        m_uiChromaggusSideDoorGUID  = 0;
        m_uiNefarianDoorGUID        = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
        case NPC_RAZORGORE:     m_uiRazorgoreGUID = pCreature->GetGUID();   break;
        case NPC_VAELASTRASZ:   m_uiVaelastraszGUID = pCreature->GetGUID(); break;
        case NPC_LASHLAYER:     m_uiLashlayerGUID = pCreature->GetGUID();   break;
        case NPC_FIREMAW:       m_uiFiremawGUID = pCreature->GetGUID();     break;
        case NPC_EBONROC:       m_uiEbonrocGUID = pCreature->GetGUID();     break;
        case NPC_FLAMEGOR:      m_uiFlamegorGUID = pCreature->GetGUID();    break;
        case NPC_CHROMAGGUS:    m_uiChromaggusGUID = pCreature->GetGUID();  break;
        case NPC_NEFARIAN:      m_uiNefarianGUID = pCreature->GetGUID();    break;
        case NPC_LORD_NEFARIAN: m_uiLordVictorGUID = pCreature->GetGUID();  break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_DOOR_RAZORGORE_ENTER:
            m_uiRazorgoreEnterDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_RAZORGORE_EXIT:
            m_uiRazorgoreExitDoorGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_NEFARIAN:
            m_uiNefarianDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_FLAMEGOR:
            m_uiFlamegorDoorGUID = pGo->GetGUID();
            pGo->SetGoState(GO_STATE_READY);
            if (m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_CHROMAGGUS_SIDE:
            m_uiChromaggusSideDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_CHROMAGGUS_EXIT:
            m_uiChromaggusExitDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[6] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_VAELASTRASZ:
            m_uiVaelastraszDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_DOOR_LASHLAYER:
            m_uiLashlayerDoorGUID = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
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

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            
        case TYPE_RAZORGORE:
            m_auiEncounter[0] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiRazorgoreEnterDoorGUID);
            else
                OpenDoor(m_uiRazorgoreEnterDoorGUID);
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiRazorgoreExitDoorGUID);
            if(uiData != DONE)
                CloseDoor(m_uiRazorgoreExitDoorGUID);
            break;

        case TYPE_VAELASTRASZ:
            m_auiEncounter[1] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiRazorgoreExitDoorGUID);
            else
                OpenDoor(m_uiRazorgoreExitDoorGUID);
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiVaelastraszDoorGUID);
            break;

        case TYPE_LASHLAYER:
            m_auiEncounter[2] = uiData;
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiLashlayerDoorGUID);
            break;

        case TYPE_FIREMAW:
            m_auiEncounter[3] = uiData;
            if(uiData == DONE && m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE)
                DoUseDoorOrButton(m_uiFlamegorDoorGUID);
            break;

        case TYPE_EBONROC:
            m_auiEncounter[4] = uiData;
            if(uiData == DONE && m_auiEncounter[3] == DONE && m_auiEncounter[5] == DONE)
                DoUseDoorOrButton(m_uiFlamegorDoorGUID);
            break;

        case TYPE_FLAMEGOR:
            m_auiEncounter[5] = uiData;
            if(uiData == DONE && m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE)
                DoUseDoorOrButton(m_uiFlamegorDoorGUID);
            break;

        case TYPE_CHROMAGGUS:
            m_auiEncounter[6] = uiData;
            if(uiData == IN_PROGRESS)
            {
                CloseDoor(m_uiFlamegorDoorGUID);
                DoUseDoorOrButton(m_uiChromaggusSideDoorGUID);
            }
            else
                OpenDoor(m_uiFlamegorDoorGUID);
            if (uiData == DONE)
                DoUseDoorOrButton(m_uiChromaggusExitDoorGUID);
            break;

        case TYPE_NEFARIAN:
            m_auiEncounter[7] = uiData;
            if(uiData == IN_PROGRESS)
                CloseDoor(m_uiNefarianDoorGUID);
            else
                OpenDoor(m_uiNefarianDoorGUID);
            break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6] << " " << m_auiEncounter[7];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
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
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_RAZORGORE:
            return m_auiEncounter[0];
        case TYPE_VAELASTRASZ:
            return m_auiEncounter[1];
        case TYPE_LASHLAYER:
            return m_auiEncounter[2];
        case TYPE_FIREMAW:
            return m_auiEncounter[3];
        case TYPE_EBONROC:
            return m_auiEncounter[4];
        case TYPE_FLAMEGOR:
            return m_auiEncounter[5];
        case TYPE_CHROMAGGUS:
            return m_auiEncounter[6];
        case TYPE_NEFARIAN:
            return m_auiEncounter[7];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
        case NPC_RAZORGORE:
            return m_uiRazorgoreGUID;
        case NPC_VAELASTRASZ:
            return m_uiVaelastraszGUID;
        case NPC_LASHLAYER:
            return m_uiLashlayerGUID;
        case NPC_FIREMAW:
            return m_uiFiremawGUID;
        case NPC_EBONROC:
            return m_uiEbonrocGUID;
        case NPC_FLAMEGOR:
            return m_uiFlamegorGUID;
        case NPC_CHROMAGGUS:
            return m_uiChromaggusGUID;
        case NPC_NEFARIAN:
            return m_uiNefarianGUID;
        case NPC_LORD_NEFARIAN:
            return m_uiLordVictorGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_blackwing_lair(Map* pMap)
{
    return new instance_blackwing_lair(pMap);
}

void AddSC_instance_blackwing_lair()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_blackwing_lair";
    pNewScript->GetInstanceData = &GetInstanceData_instance_blackwing_lair;
    pNewScript->RegisterSelf();
}

