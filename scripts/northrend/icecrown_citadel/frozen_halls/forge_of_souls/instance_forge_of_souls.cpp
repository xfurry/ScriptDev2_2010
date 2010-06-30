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

    uint64 m_uiBronjahnGUID;
	uint64 m_uiDevourerGUID;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

   void Initialize()
   {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiBronjahnGUID = 0;
		m_uiDevourerGUID = 0;
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
            case NPC_BRONJAHM:    
                m_uiBronjahnGUID = pCreature->GetGUID();        
                break;
			case NPC_DEVOURER_OF_SOULS:    
                m_uiDevourerGUID = pCreature->GetGUID();        
                break;
        }
    }

    uint64 GetData64(uint32 identifier)
    {
        switch(identifier)
        {
            case NPC_BRONJAHM:			    return m_uiBronjahnGUID;
			case NPC_DEVOURER_OF_SOULS:     return m_uiDevourerGUID;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
        case TYPE_BRONJAHM:
            m_auiEncounter[0] = data; break;
        case TYPE_DEVOURER_OF_SOULS:
            m_auiEncounter[1] = data; break;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_BRONJAHM:			    return m_auiEncounter[0];
			case TYPE_DEVOURER_OF_SOULS:    return m_auiEncounter[1];
        }

        return 0;
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
