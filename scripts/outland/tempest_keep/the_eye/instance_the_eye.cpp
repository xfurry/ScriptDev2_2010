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
SDName: Instance_The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

/* The Eye encounters:
0 - Kael'thas event
1 - Al' ar event
2 - Solarian Event
3 - Void Reaver event
*/

struct MANGOS_DLL_DECL instance_the_eye : public ScriptedInstance
{
    instance_the_eye(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiThaladredGUID;
    uint64 m_uiSanguinarGUID;
    uint64 m_uiCapernianGUID;
    uint64 m_uiTelonicusGUID;
    uint64 m_uiKaelthasGUID;
    uint64 m_uiAstromancerGUID;
    uint64 m_uiAlarGUID;

    uint64 m_uiAlarDoorLeftGUID;
    uint64 m_uiAlarDoorRightGUID;
    uint64 m_uiAstroDoorLeftGUID;
    uint64 m_uiAstroDoorRightGUID;
    uint64 m_uiVoidDoorLeftGUID;
    uint64 m_uiVoidDoorRightGUID;

    uint32 m_uiKaelthasEventPhase;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiThaladredGUID = 0;
        m_uiSanguinarGUID = 0;
        m_uiCapernianGUID = 0;
        m_uiTelonicusGUID = 0;
        m_uiKaelthasGUID = 0;
        m_uiAstromancerGUID = 0;

        m_uiKaelthasEventPhase = 0;

        m_uiAlarDoorLeftGUID    = 0;
        m_uiAlarDoorRightGUID   = 0;
        m_uiAstroDoorLeftGUID   = 0;
        m_uiAstroDoorRightGUID  = 0;
        m_uiVoidDoorLeftGUID    = 0;
        m_uiVoidDoorRightGUID   = 0;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 20064: m_uiThaladredGUID = pCreature->GetGUID(); break;
            case 20063: m_uiTelonicusGUID = pCreature->GetGUID(); break;
            case 20062: m_uiCapernianGUID = pCreature->GetGUID(); break;
            case 20060: m_uiSanguinarGUID = pCreature->GetGUID(); break;
            case 19622: m_uiKaelthasGUID = pCreature->GetGUID(); break;
            case 18805: m_uiAstromancerGUID = pCreature->GetGUID(); break;
            case 19514: m_uiAlarGUID = pCreature->GetGUID(); break;
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

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_KAEL_INNER_LEFT:
                m_uiAstroDoorLeftGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_KAEL_INNER_RIGHT:
                m_uiAstroDoorRightGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_KAEL_OUTER_RIGHT:
                m_uiVoidDoorRightGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_KAEL_OUTER_LEFT:
                m_uiVoidDoorLeftGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[2] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_ENTER_RIGHT:
                m_uiAlarDoorRightGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_DOOR_ENTER_LEFT:
                m_uiAlarDoorLeftGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_ALAR:
                m_auiEncounter[0] = uiData;
                // Open outer doors
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiAlarDoorRightGUID);
                    DoUseDoorOrButton(m_uiAlarDoorLeftGUID);
                }
                break;
            case TYPE_SOLARIAN:
                m_auiEncounter[1] = uiData;
                break;
            case TYPE_VOIDREAVER:
                m_auiEncounter[2] = uiData;
                // Open Kael outer doors
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiVoidDoorRightGUID);
                    DoUseDoorOrButton(m_uiVoidDoorLeftGUID);
                }
                break;
            case TYPE_ASTROMANCER:
                m_auiEncounter[3] = uiData;
                // Open Kael inner doors
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiAstroDoorLeftGUID);
                    DoUseDoorOrButton(m_uiAstroDoorRightGUID);
                }
                break;
            case TYPE_KAELTHAS_PHASE:
                m_uiKaelthasEventPhase = uiData;
                break;
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_ALAR:
                return m_auiEncounter[0];
            case TYPE_SOLARIAN:
                return m_auiEncounter[1];
            case TYPE_VOIDREAVER:
                return m_auiEncounter[2];
            case TYPE_ASTROMANCER:
                return m_auiEncounter[3];

            case TYPE_KAELTHAS_PHASE:
                return m_uiKaelthasEventPhase;
        }

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_THALADRED:
                return m_uiThaladredGUID;
            case DATA_SANGUINAR:
                return m_uiSanguinarGUID;
            case DATA_CAPERNIAN:
                return m_uiCapernianGUID;
            case DATA_TELONICUS:
                return m_uiTelonicusGUID;
            case DATA_KAELTHAS:
                return m_uiKaelthasGUID;
            case DATA_ASTROMANCER:
                return m_uiAstromancerGUID;
            case DATA_ALAR:
                return m_uiAlarGUID;
        }

        return 0;
    }
};

InstanceData* GetInstanceData_instance_the_eye(Map* pMap)
{
    return new instance_the_eye(pMap);
}

void AddSC_instance_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_the_eye";
    newscript->GetInstanceData = &GetInstanceData_instance_the_eye;
    newscript->RegisterSelf();
}
