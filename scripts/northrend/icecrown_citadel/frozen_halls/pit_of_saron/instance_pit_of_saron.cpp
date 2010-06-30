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
#include "pit_of_saron.h"

#define MAX_ENCOUNTER     4

struct MANGOS_DLL_DECL instance_pit_of_saron : public ScriptedInstance
{
    instance_pit_of_saron(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

	std::string m_strInstData;
    uint64 m_uiKrickGUID;
    uint64 m_uiIckGUID;
    uint64 m_uiRimefangGUID;
	uint64 m_uiGarfrostGUID;
	uint64 m_uiTyrannusGUID;

    uint64 m_uiHorPortcullisGUID;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

   void Initialize()
   {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiGarfrostGUID    = 0;
		m_uiKrickGUID       = 0;
		m_uiTyrannusGUID    = 0;
        m_uiIckGUID         = 0;
        m_uiRimefangGUID    = 0;
        m_uiHorPortcullisGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature, bool add)
    {
        switch(pCreature->GetEntry())
        {
           case NPC_KRICK:    
               m_uiKrickGUID = pCreature->GetGUID();     
               break;
           case NPC_ICK: 
               m_uiIckGUID = pCreature->GetGUID();
               break;
		   case NPC_GARFROST:    
               m_uiGarfrostGUID = pCreature->GetGUID();        
               break;
		   case NPC_TYRANNUS:    
               m_uiTyrannusGUID = pCreature->GetGUID();        
               break;
           case NPC_RIMEFANG:
               m_uiRimefangGUID = pCreature->GetGUID();
               break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_HOR_PORTCULLIS:
            m_uiHorPortcullisGUID = pGo->GetGUID();
            if(m_auiEncounter[1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case NPC_GARFROST:			    return m_uiGarfrostGUID;
			case NPC_TYRANNUS:              return m_uiTyrannusGUID;
			case NPC_KRICK:                 return m_uiKrickGUID;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 uiData)
    {
        switch(type)
        {
        case TYPE_GARFROST:
            m_auiEncounter[0] = uiData; break;
		case TYPE_TYRANNUS:
            m_auiEncounter[1] = uiData; 
            if(uiData == DONE)
                DoUseDoorOrButton(m_uiHorPortcullisGUID);
            break;
        case TYPE_KRICK_AND_ICK:
            m_auiEncounter[2] = uiData; break;
        case TYPE_INTRO:
            m_auiEncounter[3] = uiData; break;
        }

        if (uiData == DONE)
        {
            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];
            m_strInstData = saveStream.str();

            OUT_SAVE_INST_DATA;
            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_GARFROST:			    return m_auiEncounter[0];
			case TYPE_TYRANNUS:			    return m_auiEncounter[1];
			case TYPE_KRICK_AND_ICK:		return m_auiEncounter[2];
            case TYPE_INTRO:                return m_auiEncounter[3];
        }

        return 0;
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_pit_of_saron(Map* pMap)
{
   return new instance_pit_of_saron(pMap);
}

void AddSC_instance_pit_of_saron()
{
   Script *newscript;
   newscript = new Script;
   newscript->Name = "instance_pit_of_saron";
   newscript->GetInstanceData = &GetInstanceData_instance_pit_of_saron;
   newscript->RegisterSelf();
}
