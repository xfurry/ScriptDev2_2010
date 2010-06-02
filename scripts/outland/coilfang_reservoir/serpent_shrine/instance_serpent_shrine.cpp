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
SDName: Instance_Serpent_Shrine
SD%Complete: 90
SDComment:
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"

/* Serpentshrine cavern encounters:
0 - Hydross The Unstable event
1 - Leotheras The Blind Event
2 - The Lurker Below Event
3 - Fathom-Lord Karathress Event
4 - Morogrim Tidewalker Event
5 - Lady Vashj Event
*/

enum WaterEventState
{
    WATERSTATE_NONE     = 0,
    WATERSTATE_FRENZY   = 1,
    WATERSTATE_SCALDING = 2
};
#define SPELL_SCALDINGWATER 37284
#define MOB_COILFANG_FRENZY 21508
#define TRASHMOB_COILFANG_PRIESTESS 21220  //6*2
#define TRASHMOB_COILFANG_SHATTERER 21301  //6*3

#define MIN_KILLS 30

bool GOHello_go_main_bridge_console(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    switch(pGo->GetEntry())
    {
    case CONSOLE_LURKER: pInstance->SetData(TYPE_THELURKER_EVENT, SPECIAL); break;
    case CONSOLE_HYDROSS: pInstance->SetData(TYPE_HYDROSS_EVENT, SPECIAL); break;
    case CONSOLE_TIDEWALKER:  pInstance->SetData(TYPE_MOROGRIM_EVENT, SPECIAL);  break;
    case CONSOLE_LEOTHERAS:  pInstance->SetData(TYPE_LEOTHERAS_EVENT, SPECIAL);  break;
    case CONSOLE_FATHOM:  pInstance->SetData(TYPE_KARATHRESS_EVENT, SPECIAL);  break;
    }

    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    return false;
}

const int MAX_ENCOUNTER = 6;
const int MAX_GENERATOR = 4;

struct MANGOS_DLL_DECL instance_serpentshrine_cavern : public ScriptedInstance
{
    instance_serpentshrine_cavern(Map* pMap) : ScriptedInstance(pMap) { Initialize(); };

    uint64 m_uiSharkkis;
    uint64 m_uiTidalvess;
    uint64 m_uiCaribdis;
    uint64 m_uiLadyVashj;
    uint64 m_uiKarathress;
    uint64 m_uiKarathressEvent_Starter;
    uint64 m_uiBridgeConsole;
    uint64 m_uiLurkerConsole;
    uint64 m_uiHydrossConsole;
    uint64 m_uiLethorasConsole;
    uint64 m_uiTidewalkerConsole;
    uint64 m_uiFathomConsole;
    uint32 WaterCheckTimer;
    uint32 FrenzySpawnTimer;
    uint32 Water;
    uint32 TrashCount;
    bool DoSpawnFrenzy;

    uint32 m_auiShieldGenerator[MAX_GENERATOR];
    uint32 m_auiEncounter[MAX_ENCOUNTER];

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
        memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));

        m_uiSharkkis = 0;
        m_uiTidalvess = 0;
        m_uiCaribdis = 0;
        m_uiLadyVashj = 0;
        m_uiKarathress = 0;
        m_uiKarathressEvent_Starter = 0;

        m_uiBridgeConsole = 0;
        m_uiLurkerConsole = 0;
        m_uiHydrossConsole  = 0;
        m_uiLethorasConsole = 0;
        m_uiTidewalkerConsole = 0;
        m_uiFathomConsole   = 0;

        WaterCheckTimer = 500;
        FrenzySpawnTimer = 2000;
        Water = WATERSTATE_FRENZY;
        TrashCount = 0;
        DoSpawnFrenzy = false;
    }

    bool IsEncounterInProgress() const
    {
        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void Update (uint32 diff)
    {
        //Water checks
        if (WaterCheckTimer <= diff)
        {
            if(TrashCount >= MIN_KILLS)
                Water = WATERSTATE_SCALDING;
            else
                Water = WATERSTATE_FRENZY;

            Map::PlayerList const &PlayerList = instance->GetPlayers();
            if (PlayerList.isEmpty())
                return;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (Player* pPlayer = i->getSource())
                {
                    if (pPlayer->isAlive() && /*i->getSource()->GetPositionZ() <= -21.434931f*/pPlayer->IsInWater())
                    {
                        if(Water == WATERSTATE_SCALDING)
                        {

                            if(!pPlayer->HasAura(SPELL_SCALDINGWATER))
                            {
                                pPlayer->CastSpell(pPlayer, SPELL_SCALDINGWATER,true);
                            }
                        } else if(Water == WATERSTATE_FRENZY)
                        {
                            //spawn frenzy
                            if(DoSpawnFrenzy)
                            {
                                if(Creature* frenzy = pPlayer->SummonCreature(MOB_COILFANG_FRENZY,pPlayer->GetPositionX(),pPlayer->GetPositionY(),pPlayer->GetPositionZ(),pPlayer->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,2000))
                                {
                                    frenzy->Attack(pPlayer,false);
                                    frenzy->SetSplineFlags(SPLINEFLAG_UNKNOWN12);
                                }
                                DoSpawnFrenzy = false;
                            }
                        }
                    }
                    if(!pPlayer->IsInWater())
                        pPlayer->RemoveAurasDueToSpell(SPELL_SCALDINGWATER);
                }

            }
            WaterCheckTimer = 500;//remove stress from core
        } else WaterCheckTimer -= diff;
        if (FrenzySpawnTimer <= diff)
        {
            DoSpawnFrenzy = true;
            FrenzySpawnTimer = 2000;
        } else FrenzySpawnTimer -= diff;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
        case 21212: m_uiLadyVashj  = pCreature->GetGUID(); break;
        case 21214: m_uiKarathress = pCreature->GetGUID(); break;
        case 21966: m_uiSharkkis   = pCreature->GetGUID(); break;
        case 21965: m_uiTidalvess  = pCreature->GetGUID(); break;
        case 21964: m_uiCaribdis   = pCreature->GetGUID(); break;
        }
    }

    void OnGameObjectCreate(GameObject* pGo, bool add)
    {
        switch(pGo->GetEntry())
        {
        case BRIDGE_CONSOLE:   m_uiBridgeConsole = pGo->GetGUID(); break;
        case CONSOLE_LURKER:   m_uiLurkerConsole = pGo->GetGUID();
            if (m_auiEncounter[2] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case CONSOLE_HYDROSS:   m_uiHydrossConsole = pGo->GetGUID(); 
            if (m_auiEncounter[0] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case CONSOLE_TIDEWALKER:   m_uiTidewalkerConsole = pGo->GetGUID();
            if (m_auiEncounter[4] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case CONSOLE_LEOTHERAS:   m_uiLethorasConsole = pGo->GetGUID();
            if (m_auiEncounter[1] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case CONSOLE_FATHOM:   m_uiFathomConsole = pGo->GetGUID(); 
            if (m_auiEncounter[3] == DONE)
                pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        }
    }

    void SetData64(uint32 uiType, uint64 uiData)
    {
        if (uiType == DATA_KARATHRESS_STARTER)
            m_uiKarathressEvent_Starter = uiData;
    }

    uint64 GetData64(uint32 uiIdentifier)
    {
        switch(uiIdentifier)
        {
        case DATA_SHARKKIS:
            return m_uiSharkkis;
        case DATA_TIDALVESS:
            return m_uiTidalvess;
        case DATA_CARIBDIS:
            return m_uiCaribdis;
        case DATA_LADYVASHJ:
            return m_uiLadyVashj;
        case DATA_KARATHRESS:
            return m_uiKarathress;
        case DATA_KARATHRESS_STARTER:
            return m_uiKarathressEvent_Starter;
        }
        return 0;
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case DATA_TRASH:
            {
                if(uiData == 1 && TrashCount < MIN_KILLS)
                    ++TrashCount;//+1 died
                SaveToDB();
            }
            break;
        case TYPE_HYDROSS_EVENT:
            if (uiData == SPECIAL)
                if (uiData == DONE)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiHydrossConsole))
                        pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                }
                m_auiEncounter[0] = uiData;
                break;
        case TYPE_LEOTHERAS_EVENT:
            if (uiData == DONE)
            {
                if (GameObject* pGo = instance->GetGameObject(m_uiLethorasConsole))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            }
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_THELURKER_EVENT:
            if (uiData == DONE)
            {
                if (GameObject* pGo = instance->GetGameObject(m_uiLurkerConsole))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            }
            m_auiEncounter[2] = uiData;
            break;
        case TYPE_KARATHRESS_EVENT:
            if (uiData == DONE)
            {
                if (GameObject* pGo = instance->GetGameObject(m_uiFathomConsole))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            }
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_MOROGRIM_EVENT:
            if (uiData == DONE)
            {
                if (GameObject* pGo = instance->GetGameObject(m_uiTidewalkerConsole))
                    pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            }
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_LADYVASHJ_EVENT:
            if (uiData == NOT_STARTED)
                memset(&m_auiShieldGenerator, 0, sizeof(m_auiShieldGenerator));
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_SHIELDGENERATOR1:
            m_auiShieldGenerator[0] = uiData;
            break;
        case TYPE_SHIELDGENERATOR2:
            m_auiShieldGenerator[1] = uiData;
            break;
        case TYPE_SHIELDGENERATOR3:
            m_auiShieldGenerator[2] = uiData;
            break;
        case TYPE_SHIELDGENERATOR4:
            m_auiShieldGenerator[3] = uiData;
            break;
        }

        if (TYPE_HYDROSS_EVENT == SPECIAL && TYPE_LEOTHERAS_EVENT == SPECIAL && TYPE_THELURKER_EVENT == SPECIAL && 
            TYPE_KARATHRESS_EVENT == SPECIAL && TYPE_MOROGRIM_EVENT == SPECIAL)
        {
            DoUseDoorOrButton(m_uiBridgeConsole);
        }
        if (uiData == DONE || uiData == SPECIAL)
            SaveToDB();
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
        case TYPE_HYDROSS_EVENT:
            return m_auiEncounter[0];

        case TYPE_LEOTHERAS_EVENT:
            return m_auiEncounter[1];

        case TYPE_THELURKER_EVENT:
            return m_auiEncounter[2];

        case TYPE_KARATHRESS_EVENT:
            return m_auiEncounter[3];

        case TYPE_MOROGRIM_EVENT:
            return m_auiEncounter[4];

        case TYPE_LADYVASHJ_EVENT:
            return m_auiEncounter[5];

        case TYPE_SHIELDGENERATOR1:
            return m_auiShieldGenerator[0];

        case TYPE_SHIELDGENERATOR2:
            return m_auiShieldGenerator[1];

        case TYPE_SHIELDGENERATOR3:
            return m_auiShieldGenerator[2];

        case TYPE_SHIELDGENERATOR4:
            return m_auiShieldGenerator[3];

        case TYPE_VASHJ_PHASE3_CHECK:
            for(uint8 i = 0; i < MAX_GENERATOR; ++i)
            {
                if (m_auiShieldGenerator[i] != DONE)
                    return NOT_STARTED;
            }
            return DONE;
        }

        return 0;
    }

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;
        std::ostringstream stream;
        stream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " " << TrashCount;
        char* out = new char[stream.str().length() + 1];
        strcpy(out, stream.str().c_str());
        if (out)
        {
            OUT_SAVE_INST_DATA_COMPLETE;
            return out;
        }
        return NULL;
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }
        OUT_LOAD_INST_DATA(in);
        std::istringstream stream(in);
        stream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> TrashCount;
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)                // Do not load an encounter as "In Progress" - reset it instead.
                m_auiEncounter[i] = NOT_STARTED;
        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_serpentshrine_cavern(Map* pMap)
{
    return new instance_serpentshrine_cavern(pMap);
}

void AddSC_instance_serpentshrine_cavern()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_serpent_shrine";
    newscript->GetInstanceData = &GetInstanceData_instance_serpentshrine_cavern;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_main_bridge_console";
    newscript->pGOHello = &GOHello_go_main_bridge_console;
    newscript->RegisterSelf();
}
