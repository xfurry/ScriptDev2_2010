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

	std::string str_data;
    uint64 m_uiKrick;
    uint64 m_uiIck;
    uint64 m_uiRimefang;
	uint64 m_uiGarfrost;
	uint64 m_uiTyrannus;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

   void Initialize()
   {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiGarfrost = 0;
		m_uiKrick = 0;
		m_uiTyrannus = 0;
        m_uiIck = 0;
        m_uiRimefang = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS) return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature, bool add)
    {
        Map::PlayerList const &players = instance->GetPlayers();
        uint32 TeamInInstance = 0;

        if (!players.isEmpty())
        {
            if (Player* pPlayer = players.begin()->getSource())
                TeamInInstance = pPlayer->GetTeam();
        }

        switch(pCreature->GetEntry())
        {
           case NPC_KRICK:    
               m_uiKrick = pCreature->GetGUID();     
               break;
           case NPC_ICK: 
               m_uiIck = pCreature->GetGUID();
               break;
		   case NPC_GARFROST:    
               m_uiGarfrost = pCreature->GetGUID();        
               break;
		   case NPC_TYRANNUS:    
               m_uiTyrannus = pCreature->GetGUID();        
               break;
           case NPC_RIMEFANG:
               m_uiRimefang = pCreature->GetGUID();
               break;
           case NPC_SLYVANAS_PART1:
                if (TeamInInstance == ALLIANCE)
                   pCreature->UpdateEntry(NPC_JAINA_PART1, ALLIANCE);
				break;
           case NPC_KALIRA:
                if (TeamInInstance == ALLIANCE)
                   pCreature->UpdateEntry(NPC_ELANDRA, ALLIANCE);
				break;
           case NPC_LORALEN:
                if (TeamInInstance == ALLIANCE)
                   pCreature->UpdateEntry(NPC_KORELN, ALLIANCE);
				break;
            case NPC_CHAMPION_1_HORDE:
                if (TeamInInstance == ALLIANCE)
                   pCreature->UpdateEntry(NPC_CHAMPION_1_ALLIANCE, ALLIANCE);
				break;
            case NPC_CHAMPION_2_HORDE:
                if (TeamInInstance == ALLIANCE)
                   pCreature->UpdateEntry(NPC_CHAMPION_2_ALLIANCE, ALLIANCE);
				break;
            case NPC_CHAMPION_3_HORDE: // No 3rd set for Alliance?
                if (TeamInInstance == ALLIANCE) 
                   pCreature->UpdateEntry(NPC_CHAMPION_2_ALLIANCE, ALLIANCE);
				break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case TYPE_GARFROST:			    return m_uiGarfrost;
			case TYPE_TYRANNUS:             return m_uiTyrannus;
			case TYPE_KRICK_AND_ICK:        return m_uiKrick;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
        case TYPE_GARFROST:
            m_auiEncounter[0] = data; break;
		case TYPE_TYRANNUS:
            m_auiEncounter[1] = data; break;
        case TYPE_KRICK_AND_ICK:
            m_auiEncounter[2] = data; break;
        case TYPE_INTRO:
            m_auiEncounter[3] = data; break;
        }

        if (data == DONE)
        {
            SaveToDB();
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

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << "P S " << m_auiEncounter[0] << " " << m_auiEncounter[1]  << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        str_data = saveStream.str();

        OUT_SAVE_INST_DATA_COMPLETE;
        return str_data;
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        char dataHead1, dataHead2;
        uint16 data0, data1, data2, data3, data4;

        std::istringstream loadStream(in);
        loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3 >> data4;

        if (dataHead1 == 'F' && dataHead2 == 'S')
        {
            m_auiEncounter[0] = data0;
            m_auiEncounter[1] = data1;
			m_auiEncounter[2] = data2;
            m_auiEncounter[3] = data3;

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;

        } else OUT_LOAD_INST_DATA_FAIL;

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
