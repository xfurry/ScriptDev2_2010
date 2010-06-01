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
SDName: Instance Culling of Stratholme
SD%Complete: 
SDComment: 
SDCategory: Caverns of Time, The Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"

struct MANGOS_DLL_DECL instance_culling_of_stratholme : public ScriptedInstance
{
    instance_culling_of_stratholme(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    std::string strInstData;

    uint8 m_uiCratesCount;
    uint32 m_auiEncounter[11];
    uint32 m_uiHeroicTimer;
    uint32 m_uiLastTimer;
    uint32 m_uiZombieCount;
    uint32 m_uiZombieTimer;

    uint64 m_uiChromi01GUID;
    uint64 m_uiChromi02GUID;
    uint64 m_uiMikeGUID;
    uint64 m_uiMalCoricsGUID;
    uint64 m_uiGrianStoneGUID;
    uint64 m_uiJamesGUID;
    uint64 m_uiFrasCiabiGUID;
    uint64 m_uiForrestenGUID;
    uint64 m_uiRogerGUID;
    uint64 m_uiMoriganGUID;
    uint64 m_uiPerelliGUID;
    uint64 m_uiJenaGUID;
    uint64 m_uiMarthaGUID;
    uint64 m_uiMalcolmGUID;
    uint64 m_uiDogGUID;
    uint64 m_uiBartlebyGUID;
    uint64 m_uiArthasGUID;
    uint64 m_uiUtherGUID;
    uint64 m_uiJainaGUID;
    uint64 m_uiSalrammGUID;
    uint64 m_uiMalganisGUID;
    uint64 m_uiCorruptorGUID;

    uint64 m_uiShkafGateGUID;
    uint64 m_uiMalGate1GUID;
    uint64 m_uiMalGate2GUID;
    uint64 m_uiMalChestGUID;
    uint64 m_uiExitGUID;

    void Initialize()
    {
        m_uiHeroicTimer = 1500000;
        m_uiLastTimer = 1500000;
        m_uiZombieTimer = 1500000;
        m_auiEncounter[0] = NOT_STARTED;    // crates
        m_auiEncounter[1] = NOT_STARTED;
        m_auiEncounter[2] = 0;
        m_auiEncounter[3] = NOT_STARTED;
        m_auiEncounter[4] = 0;
        m_auiEncounter[5] = NOT_STARTED;    // corruptor
        m_auiEncounter[6] = NOT_STARTED;    // malganis
        m_auiEncounter[7] = NOT_STARTED;    // meathook
        m_auiEncounter[8] = NOT_STARTED;    // salram
        m_auiEncounter[9] = NOT_STARTED;    // epoch
        m_auiEncounter[10] = NOT_STARTED;   // zombiefest

        DoUpdateWorldState(WORLD_STATE_COS_CRATE_COUNT, 0);
        DoUpdateWorldState(WORLD_STATE_COS_CRATE_ON, 0);
        DoUpdateWorldState(WORLD_STATE_COS_WAVE_COUNT, 0);
        DoUpdateWorldState(WORLD_STATE_COS_TIME_COUNT, 0);
        DoUpdateWorldState(WORLD_STATE_COS_TIME_ON, 0);

        m_uiCratesCount = 0;
        m_uiZombieCount = 0;
        m_uiMikeGUID = 0;
        m_uiChromi01GUID = 0;
        m_uiChromi02GUID = 0;
        m_uiMalCoricsGUID = 0;
        m_uiGrianStoneGUID = 0;
        m_uiJamesGUID = 0;
        m_uiFrasCiabiGUID = 0;
        m_uiForrestenGUID = 0;
        m_uiRogerGUID = 0;
        m_uiMoriganGUID = 0;
        m_uiPerelliGUID = 0;
        m_uiJenaGUID = 0;
        m_uiMarthaGUID = 0;
        m_uiMalcolmGUID = 0;
        m_uiDogGUID = 0;
        m_uiBartlebyGUID = 0;
        m_uiArthasGUID = 0;
        m_uiUtherGUID = 0;
        m_uiJainaGUID = 0;
        m_uiShkafGateGUID = 0;
        m_uiSalrammGUID = 0;
        m_uiCorruptorGUID = 0;
        m_uiMalganisGUID = 0;
        m_uiMalGate1GUID = 0;
        m_uiMalGate2GUID = 0;
        m_uiMalChestGUID = 0;
        m_uiExitGUID = 0;
    }

    void OnPlayerEnter(Player *m_player)
    {
        /*if(m_auiEncounter[0] != DONE)
        {
            DoUpdateWorldState(WORLD_STATE_COS_CRATE_ON, 1);
            DoUpdateWorldState(WORLD_STATE_COS_CRATE_COUNT, m_uiCratesCount);
        }*/     
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
        case NPC_CHROMI01: 
            pCreature->SetActiveObjectState(true);
            m_uiChromi01GUID = pCreature->GetGUID();
            break;
        case NPC_MIKE: 
            m_uiMikeGUID = pCreature->GetGUID();
            break;
        case NPC_MAL_CORICS: 
            pCreature->SetActiveObjectState(true);
            m_uiMalCoricsGUID = pCreature->GetGUID();
            break;
        case NPC_GRIAN_STONE: 
            pCreature->SetActiveObjectState(true);
            pCreature->SetStandState(UNIT_STAND_STATE_SIT_MEDIUM_CHAIR);
            m_uiGrianStoneGUID = pCreature->GetGUID();
            break;
        case NPC_JAMES: 
            pCreature->SetActiveObjectState(true);
            pCreature->SetStandState(UNIT_STAND_STATE_SIT_MEDIUM_CHAIR);
            m_uiJamesGUID = pCreature->GetGUID();
            break;
        case NPC_FRAS_FRASIABI:
            pCreature->SetActiveObjectState(true);
            pCreature->SetStandState(UNIT_STAND_STATE_SIT_MEDIUM_CHAIR); 
            m_uiFrasCiabiGUID = pCreature->GetGUID();
            break;
        case NPC_FORRESTER: 
            pCreature->SetActiveObjectState(true);
            pCreature->SetStandState(UNIT_STAND_STATE_SIT_MEDIUM_CHAIR);
            m_uiForrestenGUID = pCreature->GetGUID();
            break;
        case NPC_ROGER:
            m_uiRogerGUID = pCreature->GetGUID();
            break;
        case NPC_MORIGAN:
            m_uiMoriganGUID = pCreature->GetGUID();
            break;
        case NPC_PERELLI:
            pCreature->SetActiveObjectState(true);
            m_uiPerelliGUID = pCreature->GetGUID();
            break;
        case NPC_JENA:
            m_uiJenaGUID = pCreature->GetGUID();
            break;
        case NPC_MARTHA:
            pCreature->CastSpell(pCreature, 58925, false);
            pCreature->SetActiveObjectState(true);
            m_uiMarthaGUID = pCreature->GetGUID();
            break;
        case NPC_MALCOLM:
            m_uiMalcolmGUID = pCreature->GetGUID();
            break;
        case NPC_DOG:
            pCreature->SetActiveObjectState(true);
            m_uiDogGUID = pCreature->GetGUID();
            break;
        case NPC_BARTLEBY:
            m_uiBartlebyGUID = pCreature->GetGUID();
            break;
        case NPC_UTHER:
            m_uiUtherGUID = pCreature->GetGUID();
            break;
        case NPC_ARTHAS:
            m_uiArthasGUID = pCreature->GetGUID();
            break;
        case NPC_JAINA:
            pCreature->SetActiveObjectState(true);
            m_uiJainaGUID = pCreature->GetGUID();
            break;
        case NPC_INFINITE_CORRUPTOR: 
            pCreature->SetPhaseMask(0, true);
            m_uiCorruptorGUID = pCreature->GetGUID();
            break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        if (pGo->GetEntry() == GO_SHKAF_GATE)
            m_uiShkafGateGUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_GATE1)
            m_uiMalGate1GUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_GATE2)
            m_uiMalGate2GUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_CHEST)
        {
            if(instance->IsRegularDifficulty())
                m_uiMalChestGUID = pGo->GetGUID();
            //pGo->SetUInt32Value(GAMEOBJECT_FACTION, 1375);
        }

        if (pGo->GetEntry() == GO_MALGANIS_CHEST_H)
        {
            if(!instance->IsRegularDifficulty())
                m_uiMalChestGUID = pGo->GetGUID();
        }

        if (pGo->GetEntry() == GO_EXIT)
            m_uiExitGUID = pGo->GetGUID();
    }

    void ChromiWhispers()
    {

        Map::PlayerList const &PlayerList = instance->GetPlayers();

        if (PlayerList.isEmpty())
            return;

        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
        {
            if(Creature* pChromi = instance->GetCreature(m_uiChromi01GUID))
                pChromi->MonsterWhisper("Good work with crates! Come to me in front of Stratholme for your next assighment!", i->getSource()->GetGUID(), false);
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case TYPE_QUEST:
            m_auiEncounter[0] = uiData;
            break; 
        case TYPE_CRATES_COUNT:
            m_uiCratesCount = m_uiCratesCount + uiData;
            if(m_uiCratesCount == 5)
            {
                m_auiEncounter[0] = DONE;
                ChromiWhispers();
            }
            DoUpdateWorldState(WORLD_STATE_COS_CRATE_COUNT, m_uiCratesCount);
            break;
        case TYPE_INTRO:
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_PHASE:
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_ENCOUNTER:
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_WING:
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_BONUS:
            m_auiEncounter[5] = uiData;
            if(uiData == IN_PROGRESS)
            {
                if(Creature* Corruptor = instance->GetCreature(m_uiCorruptorGUID))
                    Corruptor->SetPhaseMask(1, true);
                DoUpdateWorldState(WORLD_STATE_COS_TIME_ON, 1);
                DoUpdateWorldState(WORLD_STATE_COS_TIME_COUNT, 25);  
            } 
            break;
        case TYPE_MALGANIS:
            m_auiEncounter[6] = uiData;
            break;
        case TYPE_MEATHOOK:
            m_auiEncounter[7] = uiData;
            break;
        case TYPE_SALRAMM:
            m_auiEncounter[8] = uiData;
            break;
        case TYPE_EPOCH:
            m_auiEncounter[9] = uiData;
            break;
        case TYPE_ZOMBIEFEST:
            m_auiEncounter[10] = uiData;
            if(uiData == IN_PROGRESS)
                m_uiZombieTimer = 0;
            break;
        case TYPE_ZOMBIE_COUNT:
            m_uiZombieCount = m_uiZombieCount + uiData;
            break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[5] << " " << m_auiEncounter[6] << " "
                << m_auiEncounter[7] << " " << m_auiEncounter[8] << " " << m_auiEncounter[9];

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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
        >> m_auiEncounter[8] >> m_auiEncounter[9];

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    void SetData64(uint32 uiData, uint64 uiGuid)
    {
        switch(uiData)
        {
        case NPC_SALRAMM:
            m_uiSalrammGUID = uiGuid;
            break; 
        case NPC_MALGANIS:
            m_uiMalganisGUID = uiGuid;
            break; 
        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_QUEST:
            return m_auiEncounter[0];
        case TYPE_INTRO:
            return m_auiEncounter[1];
        case TYPE_PHASE:
            return m_auiEncounter[2];
        case TYPE_ENCOUNTER:
            return m_auiEncounter[3];
        case TYPE_WING:
            return m_auiEncounter[4];
        case TYPE_BONUS:
            return m_auiEncounter[5];
        case TYPE_MALGANIS:
            return m_auiEncounter[6];
        case TYPE_MEATHOOK:
            return m_auiEncounter[7];
        case TYPE_SALRAMM:
            return m_auiEncounter[8];
        case TYPE_EPOCH:
            return m_auiEncounter[9];
        case TYPE_ZOMBIEFEST:
            return m_auiEncounter[10];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
        case NPC_FORRESTER: return m_uiForrestenGUID;
        case NPC_JAMES: return m_uiJamesGUID;
        case NPC_FRAS_FRASIABI: return m_uiFrasCiabiGUID;
        case NPC_MAL_CORICS: return m_uiMalCoricsGUID;
        case NPC_GRIAN_STONE: return m_uiGrianStoneGUID;
        case NPC_ROGER: return m_uiRogerGUID;
        case NPC_MORIGAN: return m_uiMoriganGUID;
        case NPC_PERELLI: return m_uiPerelliGUID;
        case NPC_JENA: return m_uiJenaGUID;
        case NPC_MARTHA: return m_uiMarthaGUID;
        case NPC_MALCOLM: return m_uiMalcolmGUID;
        case NPC_DOG: return m_uiDogGUID;
        case NPC_BARTLEBY: return m_uiBartlebyGUID;
        case NPC_UTHER: return m_uiUtherGUID;
        case NPC_ARTHAS: return m_uiArthasGUID;
        case NPC_JAINA: return m_uiJainaGUID;
        case NPC_SALRAMM: return m_uiSalrammGUID;
        case NPC_MALGANIS: return m_uiMalganisGUID;
        case GO_SHKAF_GATE: return m_uiShkafGateGUID;
        case GO_MALGANIS_GATE1: return m_uiMalGate1GUID;
        case GO_MALGANIS_GATE2: return m_uiMalGate2GUID;
        case GO_MALGANIS_CHEST: return m_uiMalChestGUID;
        case GO_MALGANIS_CHEST_H: return m_uiMalChestGUID;
        case GO_EXIT: return m_uiExitGUID;
        }

        return 0;
    }

    void Update(uint32 uiDiff)
    {
        if(m_auiEncounter[5] == IN_PROGRESS)
        {
            if(m_uiHeroicTimer < uiDiff)
            {
                m_auiEncounter[5] = FAIL;
                DoUpdateWorldState(WORLD_STATE_COS_TIME_ON, 0);
                if(Creature* Corruptor = instance->GetCreature(m_uiCorruptorGUID))
                {
                    Corruptor->SetPhaseMask(0, true);
                    DoScriptText(SAY_INFINITE_DEPARTURE, Corruptor);
                }

            }else m_uiHeroicTimer -= uiDiff;

            if(m_uiHeroicTimer < m_uiLastTimer - 60000)
            {
                m_uiLastTimer = m_uiHeroicTimer;
                uint32 tMinutes = m_uiHeroicTimer / 60000;
                DoUpdateWorldState(WORLD_STATE_COS_TIME_COUNT, tMinutes);
            }
        }

        if(m_auiEncounter[10] == IN_PROGRESS)
        {
            m_uiZombieTimer += uiDiff;

            if (m_uiZombieTimer > 60000)
            {
                m_uiZombieCount = 0;
                m_uiZombieTimer = 1500000;
                m_auiEncounter[10] = FAIL;
            }
            else if(m_uiZombieCount >= 100)
            {
                DoCompleteAchievement(ACHIEV_ZOMBIEFEST);
                m_uiZombieCount = 0;
                m_auiEncounter[10] = DONE;
            }
        }

        return;
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
