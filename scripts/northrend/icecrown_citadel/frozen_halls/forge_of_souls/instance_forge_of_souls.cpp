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
SDName: instance_forge_of_souls
SD%Complete: 100%
SDComment:
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

#define MAX_ENCOUNTER     2

struct MANGOS_DLL_DECL instance_forge_of_souls : public ScriptedInstance
{
    instance_forge_of_souls(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

	std::string str_data;
    uint64 m_uiBronjahn;
	uint64 m_uiDevourer;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

   void Initialize()
   {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiBronjahn = 0;
		m_uiDevourer = 0;
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
            case NPC_BRONJAHM:    
                m_uiBronjahn = pCreature->GetGUID();        
                break;
			case NPC_DEVOURER_OF_SOULS:    
                m_uiDevourer = pCreature->GetGUID();        
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
                   pCreature->UpdateEntry(NPC_CHAMPION_3_ALLIANCE, ALLIANCE);
				break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case TYPE_BRONJAHN:			    return m_uiBronjahn;
			case TYPE_DEVOURER_OF_SOULS:    return m_uiDevourer;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
        case TYPE_BRONJAHN:
            m_auiEncounter[0] = data; break;
        case TYPE_DEVOURER_OF_SOULS:
            m_auiEncounter[1] = data; break;
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
            case TYPE_BRONJAHN:			    return m_auiEncounter[0];
			case TYPE_DEVOURER_OF_SOULS:    return m_auiEncounter[1];
        }

        return 0;
    }

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << "F S " << m_auiEncounter[0] << " " << m_auiEncounter[1];

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
        uint16 data0, data1, data2, data3;

        std::istringstream loadStream(in);
        loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3;

        if (dataHead1 == 'F' && dataHead2 == 'S')
        {
            m_auiEncounter[0] = data0;
            m_auiEncounter[1] = data1;

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;

        } else OUT_LOAD_INST_DATA_FAIL;

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_forge_of_souls(Map* pMap)
{
   return new instance_forge_of_souls(pMap);
}

void AddSC_instance_forge_of_souls()
{
   Script *newscript;
   newscript = new Script;
   newscript->Name = "instance_forge_of_souls";
   newscript->GetInstanceData = &GetInstanceData_instance_forge_of_souls;
   newscript->RegisterSelf();
}
