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
SDName: instance_ruby_sanctum
SD%Complete: 50%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

struct MANGOS_DLL_DECL instance_ruby_sanctum : public ScriptedInstance
{
	instance_ruby_sanctum(Map* pMap) : ScriptedInstance(pMap) 
	{
		Initialize();
	}

	std::string m_strInstData;

	uint32 m_auiEncounter[MAX_ENCOUNTERS];
	uint32 m_uiHalionPhase;

	uint64 m_uiHalionRealGUID;
	uint64 m_uiHalionTwilightGUID;
	uint64 m_uiSavianaGUID;
	uint64 m_uiZarithrianGUID;
	uint64 m_uiBaltharusGUID;
	uint64 m_uiXerestraszaGUID;

	uint64 m_uiFireFieldGUID;
	uint64 m_uiFlameWallsGUID;
	uint64 m_uiFlameRingGUID;

	void Initialize()
	{
		memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
		m_uiHalionPhase		= 0;

		m_uiHalionRealGUID	= 0;
		m_uiHalionTwilightGUID = 0;
		m_uiSavianaGUID		= 0;
		m_uiZarithrianGUID	= 0;
		m_uiBaltharusGUID	= 0;
		m_uiXerestraszaGUID = 0;

		m_uiFireFieldGUID	= 0;
		m_uiFlameWallsGUID	= 0;
		m_uiFlameRingGUID	= 0;
	}

	bool IsEncounterInProgress() const
	{
		for(uint8 i = 1; i < MAX_ENCOUNTERS ; ++i)
			if (m_auiEncounter[i] == IN_PROGRESS)
				return true;
		return false;
	}

	void OnCreatureCreate(Creature* pCreature)
	{
		switch(pCreature->GetEntry())
		{
		case NPC_SAVIANA:
			m_uiSavianaGUID = pCreature->GetGUID();
			break;
		case NPC_ZARITHRIAN:
			m_uiZarithrianGUID = pCreature->GetGUID();
			pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			if(m_auiEncounter[0] == DONE && m_auiEncounter[1] == DONE)
				pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			break;
		case NPC_BALTHARUS:
			m_uiBaltharusGUID = pCreature->GetGUID();
			break;
		case NPC_XERESTRASZA:
			m_uiXerestraszaGUID = pCreature->GetGUID();
			if(m_auiEncounter[2] == DONE)
			{
				if(Creature* pTemp = pCreature->SummonCreature(NPC_HALION_REAL, 3155.190703f, 538.717708f, 72.889038f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
					m_uiHalionRealGUID = pTemp->GetGUID();
				if(Creature* pTemp = pCreature->SummonCreature(NPC_HALION_TWILIGHT, 3155.190703f, 538.717708f, 72.889038f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
					m_uiHalionTwilightGUID = pTemp->GetGUID();
			}
			break;
		}
	}

	void OnObjectCreate(GameObject* pGo)
	{
		switch(pGo->GetEntry())
		{
		case GO_FLAME_WALLS:     
			m_uiFlameWallsGUID = pGo->GetGUID();
			if(m_auiEncounter[0] == DONE && m_auiEncounter[1] == DONE)
				pGo->SetGoState(GO_STATE_ACTIVE);
			break;
		case GO_FLAME_RING:       
			m_uiFlameRingGUID = pGo->GetGUID();      
			break;
		case GO_FIRE_FIELD:       
			m_uiFireFieldGUID = pGo->GetGUID(); 
			if(m_auiEncounter[1] == DONE)
				pGo->SetGoState(GO_STATE_ACTIVE);
			break;
		}
	}

	void CheckForGeneralDoor()
	{
		if(m_auiEncounter[0] == DONE && m_auiEncounter[1] == DONE)
		{
			DoUseDoorOrButton(m_uiFlameWallsGUID);
			if(Creature* pZarithrian = instance->GetCreature(m_uiZarithrianGUID))
				pZarithrian->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		}
	}

	void SetData(uint32 uiType, uint32 uiData)
	{
		switch(uiType)
		{
		case TYPE_SAVIANA:     
			m_auiEncounter[0] = uiData; 
			if(uiData == DONE)
				CheckForGeneralDoor();
			break;
		case TYPE_BALTHARUS:    
			m_auiEncounter[1] = uiData;
			if(uiData == DONE)
			{
				CheckForGeneralDoor();
				DoUseDoorOrButton(m_uiFireFieldGUID);
			}
			break;
		case TYPE_ZARITHRIAN:    
			m_auiEncounter[2] = uiData;
			DoUseDoorOrButton(m_uiFlameWallsGUID);
			if (uiData == DONE)
			{
				if(Creature* pXerestrasza = instance->GetCreature(m_uiXerestraszaGUID))
				{
					if(Creature* pTemp = pXerestrasza->SummonCreature(NPC_HALION_REAL, 3155.190703f, 538.717708f, 72.889038f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
						m_uiHalionRealGUID = pTemp->GetGUID();
					if(Creature* pTemp = pXerestrasza->SummonCreature(NPC_HALION_TWILIGHT, 3155.190703f, 538.717708f, 72.889038f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
						m_uiHalionTwilightGUID = pTemp->GetGUID();
				}
			}
			break;
		case TYPE_HALION:
			m_auiEncounter[3] = uiData;
			DoUseDoorOrButton(m_uiFlameRingGUID);
			break;
		case TYPE_HALION_PHASE:
			m_uiHalionPhase = uiData;
			break;
		}

		if (uiData == DONE)
		{
			OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3];

            m_strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
		}
	}

	const char* Save()
	{
		return m_strInstData.c_str();
	}

	uint32 GetData(uint32 uiType)
	{
		switch(uiType)
		{
		case TYPE_SAVIANA:      
			return m_auiEncounter[0];
		case TYPE_BALTHARUS:     
			return m_auiEncounter[1];
		case TYPE_ZARITHRIAN:     
			return m_auiEncounter[2];
		case TYPE_HALION:        
			return m_auiEncounter[3];
		case TYPE_HALION_PHASE:
			return m_uiHalionPhase;
		}
		return 0;
	}

	uint64 GetData64(uint32 uiData)
	{
		switch(uiData)
		{
		case NPC_BALTHARUS:  
			return m_uiBaltharusGUID;
		case NPC_ZARITHRIAN:  
			return m_uiZarithrianGUID;
		case NPC_SAVIANA:   
			return m_uiSavianaGUID;
		case NPC_HALION_REAL:               
			return m_uiHalionRealGUID;
		case NPC_HALION_TWILIGHT:           
			return m_uiHalionTwilightGUID;
		case NPC_XERESTRASZA:
			return m_uiXerestraszaGUID;
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
	}
};

InstanceData* GetInstanceData_instance_ruby_sanctum(Map* pMap)
{
	return new instance_ruby_sanctum(pMap);
}

void AddSC_instance_ruby_sanctum()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "instance_ruby_sanctum";
	pNewScript->GetInstanceData = &GetInstanceData_instance_ruby_sanctum;
	pNewScript->RegisterSelf();
}
