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
#include "oculus.h"

/* The Occulus encounters:
0 - Drakos the Interrogator
1 - Varos Cloudstrider
2 - Mage-Lord Urom
3 - Ley-Guardian Eregos */

struct MANGOS_DLL_DECL instance_oculus : public ScriptedInstance
{
    instance_oculus(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
	std::string strInstData;

	uint64 m_uiVarosGUID;
	uint64 m_uiEternosGUID;
	uint64 m_uiVerdisaGUID;
	uint64 m_uiBelgaristraszGUID;

	uint64 m_uiCageDoorGUID;
	uint64 m_uiEregosCacheGUID;

	bool m_bMakeCount;
	uint32 m_uiMakeCountTimer;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

		m_uiVarosGUID			= 0;
		m_uiEternosGUID			= 0;
		m_uiVerdisaGUID			= 0;
		m_uiBelgaristraszGUID	= 0;

		m_uiCageDoorGUID		= 0;
		m_uiEregosCacheGUID		= 0;

		m_bMakeCount			= false;
		m_uiMakeCountTimer		= 0;
    }

	void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
		case NPC_VAROS:
			m_uiVarosGUID = pCreature->GetGUID();
			break;
		case NPC_ETERNOS:
			m_uiEternosGUID = pCreature->GetGUID();
			if(m_auiEncounter[0] == DONE)
			{
				pCreature->GetMotionMaster()->MovePoint(0, 944.384f, 1058.418f, 359.967f);
				if(GameObject* pDoor = GetClosestGameObjectWithEntry(pCreature, GO_DRAGON_CAGE_DOOR, 5.0f))
					pDoor->SetGoState(GO_STATE_ACTIVE);
			}
			break;
		case NPC_VERDISA:
			m_uiVerdisaGUID = pCreature->GetGUID();
			if(m_auiEncounter[0] == DONE)
			{
				pCreature->GetMotionMaster()->MovePoint(0, 949.928f, 1034.753f, 359.967f);
				if(GameObject* pDoor = GetClosestGameObjectWithEntry(pCreature, GO_DRAGON_CAGE_DOOR, 5.0f))
					pDoor->SetGoState(GO_STATE_ACTIVE);
			}
			break;
		case NPC_BELGARISTRASZ:
			m_uiBelgaristraszGUID = pCreature->GetGUID();
			if(m_auiEncounter[0] == DONE)
			{
				pCreature->GetMotionMaster()->MovePoint(0, 944.868f, 1044.982f, 359.967f);
				if(GameObject* pDoor = GetClosestGameObjectWithEntry(pCreature, GO_DRAGON_CAGE_DOOR, 5.0f))
					pDoor->SetGoState(GO_STATE_ACTIVE);
			}
			break;
        }
    }

	void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
		case GO_DRAGON_CAGE_DOOR:
			m_uiCageDoorGUID = pGo->GetGUID();
			break;
		case GO_CACHE_EREGOS:
			if(instance->IsRegularDifficulty())
				m_uiEregosCacheGUID = pGo->GetGUID();
			break;
		case GO_CACHE_EREGOS_H:
			if(!instance->IsRegularDifficulty())
				m_uiEregosCacheGUID = pGo->GetGUID();
			break;
        }
    }

	void SetData(uint32 uiType, uint32 uiData)
	{
		switch(uiType)
		{
		case TYPE_DRAKOS:
			m_auiEncounter[0] = uiData;
			if(uiData == DONE)
				m_bMakeCount = true;
			break;
		case TYPE_VAROS:
			m_auiEncounter[1] = uiData;
			break;
		case TYPE_UROM:
			m_auiEncounter[2] = uiData;
			break;
		case TYPE_EREGOS:
			m_auiEncounter[3] = uiData;
			if(uiData == DONE)
			{
				DoRespawnGameObject(m_uiEregosCacheGUID);
				if(m_uiMakeCountTimer < 20*MINUTE && !instance->IsRegularDifficulty())
					DoCompleteAchievement(ACHIEV_MAKE_IT_COUNT);
			}
			break;
		}

		if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
	}

    uint32 GetData(uint32 uiType)
    {
		switch(uiType)
		{
		case TYPE_DRAKOS:
            return m_auiEncounter[0];
		case TYPE_VAROS:
			return m_auiEncounter[1];
		case TYPE_UROM:
			return m_auiEncounter[2];
		case TYPE_EREGOS:
			return m_auiEncounter[3];
		}
        return 0;
    }

	uint64 GetData64(uint32 uiType)
    {
		switch(uiType)
		{
		case NPC_VAROS:
			return m_uiVarosGUID;
		case NPC_ETERNOS:
			return m_uiEternosGUID;
		case NPC_VERDISA:
			return m_uiVerdisaGUID;
		case NPC_BELGARISTRASZ:
			return m_uiBelgaristraszGUID;
		}

        return 0;
    }

	void Update(uint32 uiDiff)
	{
		if(m_bMakeCount)
			m_uiMakeCountTimer += uiDiff;
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_oculus(Map* pMap)
{
    return new instance_oculus(pMap);
}

void AddSC_instance_oculus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_oculus";
    newscript->GetInstanceData = &GetInstanceData_instance_oculus;
    newscript->RegisterSelf();
}