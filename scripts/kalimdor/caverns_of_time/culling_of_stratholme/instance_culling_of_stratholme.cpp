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
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment:
SDCategory: Culling of Stratholme
EndScriptData */



#include "precompiled.h"
#include "culling_of_stratholme.h"

#define GO_SHKAF_GATE       188686
#define GO_MALGANIS_GATE1   187711    
#define GO_MALGANIS_GATE2   187723 
#define GO_MALGANIS_CHEST   190663 
#define GO_MALGANIS_CHEST_H 193597

struct MANGOS_DLL_DECL instance_culling_of_stratholme : public ScriptedInstance
{
    instance_culling_of_stratholme(Map* pMap) : ScriptedInstance(pMap) 
	{
        m_bIsRegularMode = pMap->IsRegularDifficulty();
		Initialize();
	};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;
    bool m_bIsRegularMode;

    uint64 m_uiShkafGateGUID;
    uint64 m_uiMalGate1GUID;
    uint64 m_uiMalGate2GUID;
    uint64 m_uiMalChestGUID;

    uint64 m_uilordEpochGUID;
    uint64 m_uiMeathookGUID;
	uint64 m_uiSalrammGUID;
	uint64 m_uiMalganisGUID;
	uint64 m_uiArthasGUID;

    void Initialize()
    {
        m_uiShkafGateGUID = 0;
        m_uiMalGate1GUID = 0;
        m_uiMalGate2GUID = 0;
        m_uiMalChestGUID = 0;

        m_uilordEpochGUID = 0;
		m_uiMeathookGUID = 0;
		m_uiSalrammGUID = 0;
        m_uiMalganisGUID = 0;
		m_uiArthasGUID = 0;
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

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_ARTHAS:
			{
                m_uiArthasGUID = pCreature->GetGUID();
                break;
			}
			case NPC_SALRAMM:
			{
                m_uiSalrammGUID = pCreature->GetGUID();
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
                break;
			}
            case NPC_MEATHOOK:
			{
                m_uiMeathookGUID = pCreature->GetGUID();
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
                break;
			}
			case NPC_EPOCH:
			{
                m_uilordEpochGUID = pCreature->GetGUID();
				pCreature->SetVisibility(VISIBILITY_OFF);
				pCreature->setFaction(35);
                break;
			}
			case NPC_MALGANIS:
			{
                m_uiMalganisGUID = pCreature->GetGUID();
                break;
			}
		}
    }

	void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_SHKAF_GATE:
            m_uiShkafGateGUID = pGo->GetGUID();
            break;

        case GO_MALGANIS_GATE1:
            m_uiMalGate1GUID = pGo->GetGUID();
            break;

        case GO_MALGANIS_GATE2:
            m_uiMalGate2GUID = pGo->GetGUID();
            break;

        case GO_MALGANIS_CHEST:
            if(m_bIsRegularMode)
                m_uiMalChestGUID = pGo->GetGUID();
            break;

        case GO_MALGANIS_CHEST_H:
            if(!m_bIsRegularMode)
                m_uiMalChestGUID = pGo->GetGUID();
            break;
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
			case DATA_ARTHAS: return m_uiArthasGUID;
			case DATA_SALRAMM: return m_uiSalrammGUID;
			case DATA_MEATHOOK: return m_uiMeathookGUID;
			case DATA_EPOCH: return m_uilordEpochGUID;
			case DATA_MALGANIS: return m_uiMalganisGUID;
            case DATA_GO_SHKAF_GATE: return m_uiShkafGateGUID;
            case DATA_GO_MAL_GATE1: return m_uiMalGate1GUID;
            case DATA_GO_MAL_GATE2: return m_uiMalGate2GUID;
            //case DATA_GO_MAL_CHEST: return m_uiMalChestGUID;
        }

        return 0;
    }

	void SetData(uint32 uiType, uint32 uiData)
    {
		bool needSave = false;
        switch(uiType)
        {
            case TYPE_ARTHAS:
			{
                if (uiData == IN_PROGRESS)
					needSave = true;

				m_auiEncounter[0] = uiData;
                break;
			}
			case TYPE_SALRAMM:
			{
                if (uiData == DONE)
					needSave = true;

                m_auiEncounter[1] = uiData;
                break;
			}
            case TYPE_MEATHOOK:
			{
                if (uiData == DONE)
					needSave = true;

                m_auiEncounter[2] = uiData;
                break;
			}
            case TYPE_EPOCH:
			{
                if (uiData == DONE)
					needSave = true;

                m_auiEncounter[3] = uiData;
                break;
			}
            case TYPE_MALGANIS:
			{
				if (uiData == DONE) 
				{
                    DoRespawnGameObject(m_uiMalChestGUID,30*MINUTE); //respawn time for the chest 1 day
					needSave = true;
				}
                if (uiData == IN_PROGRESS)
                    DoUseDoorOrButton(m_uiMalGate2GUID);
                m_auiEncounter[4] = uiData;
                break;
			}
            case TYPE_INFINITE_CORRUPTOR:
			{
				if (uiData == DONE) 
					needSave = true;
                m_auiEncounter[5] = uiData;
                break;
			}
		}
        if (uiData == DONE || needSave)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3] << " " << m_auiEncounter[4];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

	uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_ARTHAS:
                return m_auiEncounter[0];
			case TYPE_SALRAMM:
                return m_auiEncounter[1];
			case TYPE_MEATHOOK:
                return m_auiEncounter[2];
			case TYPE_EPOCH:
                return m_auiEncounter[3];
			case TYPE_MALGANIS:
                return m_auiEncounter[4];
            case TYPE_INFINITE_CORRUPTOR:
                return m_auiEncounter[5];
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] >> m_auiEncounter[4];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_culling_of_stratholme(Map* pMap)
{
    return new instance_culling_of_stratholme(pMap);
}

void AddSC_instance_culling_of_stratholme()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_culling_of_stratholme";
    newscript->GetInstanceData = &GetInstanceData_instance_culling_of_stratholme;
    newscript->RegisterSelf();
}
