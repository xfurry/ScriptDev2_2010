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
SDName: Instance_Violet_Hold
SD%Complete: 50
SDComment: "experimental" use of header/source object
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

instance_violet_hold::instance_violet_hold(Map* pMap) : ScriptedInstance(pMap),

    m_uiSinclariGUID(0),
    m_uiSinclariAltGUID(0),
    m_uiErekemGUID(0),
    m_uiMoraggGUID(0),
    m_uiIchoronGUID(0),
    m_uiXevozzGUID(0),
    m_uiLavanthorGUID(0),
    m_uiZuramatGUID(0),

    m_uiCellErekemGuard_LGUID(0),
    m_uiCellErekemGuard_RGUID(0),
    m_uiIntroCrystalGUID(0),
    m_uiDefenseCrystalGUID(0),

    m_uiWorldState(0),
    m_uiWorldStateSealCount(100),
    m_uiWorldStatePortalCount(0),

    m_uiLastBossID(0),
    m_uiLastBossIDConst(0),
    m_bDefenseUsed(false),

    m_uiPortalId(0),
    m_uiPortalTimer(0),
    m_uiMaxCountPortalLoc(0)
{
    Initialize();
}

void instance_violet_hold::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    m_uiMaxCountPortalLoc = (sizeof(afPortalLocation)/sizeof(sPortalData)) - 1;
}

void instance_violet_hold::ResetVariables()
{
    m_uiWorldStateSealCount = 100;
    m_uiWorldStatePortalCount = 0;
}

void instance_violet_hold::ResetAll()
{
    ResetVariables();
    UpdateWorldState(false);
    CallGuards(true);
    SetIntroPortals(false);
    m_bDefenseUsed = false;

    // reset defense crystals
    std::list<GameObject*> lCrystals;

    if(Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
        GetGameObjectListWithEntryInGrid(lCrystals, pSinclari, GO_DEFENSE_CRYSTAL, 180.0f);

    if (!lCrystals.empty())
    {
        for(std::list<GameObject*>::iterator iter = lCrystals.begin(); iter != lCrystals.end(); ++iter)
        {
            if ((*iter))
                (*iter)->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        }
    }

    // reset bosses
    if(Creature* pTemp = instance->GetCreature(m_uiLavanthorGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }
    if(Creature* pTemp = instance->GetCreature(m_uiErekemGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }
    if(Creature* pTemp = instance->GetCreature(m_uiMoraggGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }
    if(Creature* pTemp = instance->GetCreature(m_uiIchoronGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }
    if(Creature* pTemp = instance->GetCreature(m_uiXevozzGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }
    if(Creature* pTemp = instance->GetCreature(m_uiZuramatGUID))
    {
        if(!pTemp->isAlive())
            pTemp->Respawn();
    }

    // respawn adds 
    std::list<Creature*> lAdds;

    if(Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
        GetCreatureListWithEntryInGrid(lAdds, pSinclari, NPC_EREKEM_GUARD, 180.0f);

    if (!lAdds.empty())
    {
        for(std::list<Creature*>::iterator iter = lAdds.begin(); iter != lAdds.end(); ++iter)
        {
            if ((*iter) && !(*iter)->isAlive())
                (*iter)->Respawn();
        }
    }
}

void instance_violet_hold::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SINCLARI: m_uiSinclariGUID = pCreature->GetGUID(); break;
        case NPC_SINCLARI_ALT: m_uiSinclariAltGUID = pCreature->GetGUID(); break;
        case NPC_DOOR_SEAL: m_uiDoorSealGUID = pCreature->GetGUID(); break;

        case NPC_EREKEM:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiErekemGUID = pCreature->GetGUID();
            break;
        case NPC_MORAGG:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiMoraggGUID = pCreature->GetGUID();
            break;
        case NPC_ICHORON:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiIchoronGUID = pCreature->GetGUID();
            break;
        case NPC_XEVOZZ:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiXevozzGUID = pCreature->GetGUID();
            break;
        case NPC_LAVANTHOR:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiLavanthorGUID = pCreature->GetGUID();
            break;
        case NPC_ZURAMAT:
            m_lRandomBossList.push_back(pCreature->GetEntry());
            m_uiZuramatGUID = pCreature->GetGUID();
            break;

        case NPC_PORTAL_INTRO:
            m_lIntroPortalList.push_back(pCreature->GetGUID());
            break;
        case NPC_HOLD_GUARD:
            m_lGuardsList.push_back(pCreature->GetGUID());
            break;
    }
}

void instance_violet_hold::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_CELL_LAVANTHOR:
            m_mBossToCellMap.insert(BossToCellMap::value_type(12, pGo->GetGUID()));
            break;
        case GO_CELL_MORAGG:
            m_mBossToCellMap.insert(BossToCellMap::value_type(9, pGo->GetGUID()));
            break;
        case GO_CELL_ZURAMAT:
            m_mBossToCellMap.insert(BossToCellMap::value_type(13, pGo->GetGUID()));
            break;
        case GO_CELL_XEVOZZ:
            m_mBossToCellMap.insert(BossToCellMap::value_type(11, pGo->GetGUID()));
            break;
        case GO_CELL_ICHORON:
            m_mBossToCellMap.insert(BossToCellMap::value_type(10, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM:
            m_mBossToCellMap.insert(BossToCellMap::value_type(8, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_L:
            m_mBossToCellMap.insert(BossToCellMap::value_type(8, pGo->GetGUID()));
            break;
        case GO_CELL_EREKEM_GUARD_R:
            m_mBossToCellMap.insert(BossToCellMap::value_type(8, pGo->GetGUID()));
            break;

        case GO_INTRO_CRYSTAL:
            m_uiIntroCrystalGUID = pGo->GetGUID();
            break;
        case GO_DEFENSE_CRYSTAL:
            m_uiDefenseCrystalGUID = pGo->GetGUID();
            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
            break;
        case GO_PRISON_SEAL_DOOR:
            m_uiDoorSealGUID = pGo->GetGUID();
            break;
    }
}

void instance_violet_hold::UpdateCellForBoss(uint32 uiBossEntry)
{
    BossToCellMap::const_iterator itrCellLower = m_mBossToCellMap.lower_bound(uiBossEntry);
    BossToCellMap::const_iterator itrCellUpper = m_mBossToCellMap.upper_bound(uiBossEntry);

    if (itrCellLower == itrCellUpper)
        return;

    for(BossToCellMap::const_iterator itr = itrCellLower; itr != itrCellUpper; ++itr)
        DoUseDoorOrButton(itr->second);
}

void instance_violet_hold::UpdateWorldState(bool bEnable)
{
    if (bEnable)
        m_uiWorldState = 1;
    else
        m_uiWorldState = 0;

    DoUpdateWorldState(WORLD_STATE_ID, m_uiWorldState);
    DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);
    DoUpdateWorldState(WORLD_STATE_PORTALS, m_uiWorldStatePortalCount);
}

void instance_violet_hold::OnPlayerEnter(Player* pPlayer)
{
    UpdateWorldState(m_auiEncounter[0] == IN_PROGRESS ? true : false);
}

void instance_violet_hold::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: instance_violet_hold: SetData got type % u, data %u.", uiType, uiData);

    switch(uiType)
    {
        case TYPE_MAIN:
        {
            if (uiData == m_auiEncounter[0])
                return;

            switch(uiData)
            {
                case NOT_STARTED:
                    ResetAll();
                    break;
                case IN_PROGRESS:
                    SetRandomBosses();
                    UpdateWorldState();
                    m_uiPortalId = urand(0, 2);
                    m_uiPortalTimer = 15000;
                    break;
                case FAIL:
                    if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                    {
                        pSinclari->Respawn();
                        if(GameObject* pDoor = GetClosestGameObjectWithEntry(pSinclari, GO_PRISON_SEAL_DOOR, 150.0f))
                            DoUseDoorOrButton(pDoor->GetGUID());
                    }
                    if (m_auiEncounter[3] == DONE)
                        UpdateCellForBoss(m_auiEncounter[6]);
                    if (m_auiEncounter[4] == DONE)
                        UpdateCellForBoss(m_auiEncounter[7]);
                    m_auiEncounter[0] = NOT_STARTED;
                    break;
                case DONE:
                    if(!m_bDefenseUsed && m_uiWorldStateSealCount == 100)
                    {
                        if(!instance->IsRegularDifficulty())
                            DoCompleteAchievement(ACHIEV_DEFENSELESS);
                    }
                    break;
                case SPECIAL:
                    break;
            }
            m_auiEncounter[0] = uiData;
            break;
        }
        case TYPE_SEAL:
            if(uiData == IN_PROGRESS)
            {
                --m_uiWorldStateSealCount;
                DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);

                if (!m_uiWorldStateSealCount)
                {
                    if (Creature* pSeal = instance->GetCreature(m_uiDoorSealGUID))
                    {
                        if (pSeal->isAlive())
                        {
                            pSeal->DealDamage(pSeal, pSeal->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            m_auiEncounter[0] = FAIL;
                            m_auiEncounter[1] = NOT_STARTED;
                        }
                    }
                }
            }
            m_auiEncounter[1] = uiData;
            break;
        case TYPE_PORTAL:
        {
            switch(uiData)
            {
                case SPECIAL:                               // timer to next
                    m_uiPortalTimer = 90000;
                    break;
                case DONE:                                  // portal done, set timer to 5 secs
                    if(m_uiWorldStatePortalCount == 6 || m_uiWorldStatePortalCount == 12)
                        m_uiPortalTimer = 20000;            // set 20 sec portal delay after bosses
                    else
                        m_uiPortalTimer = 5000;             
                    break;
            }
            m_auiEncounter[2] = uiData;
            break;
        }
        case TYPE_FIRST_BOSS:
            m_auiEncounter[3] = uiData;
            break;
        case TYPE_SECOND_BOSS:
            m_auiEncounter[4] = uiData;
            break;
        case TYPE_CYANIGOSA:
            m_auiEncounter[5] = uiData;
            break;
        case TYPE_FIRST_BOSS_ID:
            m_auiEncounter[6] = uiData;
            break;
        case TYPE_SECOND_BOSS_ID:
            m_auiEncounter[7] = uiData;
            break;
        case TYPE_EREKEM:
            m_auiEncounter[8] = uiData;
            break;
        case TYPE_MORAGG:
            m_auiEncounter[9] = uiData;
            break;
        case TYPE_ICHORON:
            m_auiEncounter[10] = uiData;
            break;
        case TYPE_XEVOZZ:
            m_auiEncounter[11] = uiData;
            break;
        case TYPE_LAVANTHOR:
            m_auiEncounter[12] = uiData;
            break;
        case TYPE_ZURAMAT:
            m_auiEncounter[13] = uiData;
            break;
        case TYPE_LASTBOSS_ID:
            m_uiLastBossIDConst = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
            << m_auiEncounter[12] << " " << m_auiEncounter[13];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

const char* instance_violet_hold::Save()
{
    return strInstData.c_str();
}

void instance_violet_hold::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
    >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
    >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
    >> m_auiEncounter[12] >> m_auiEncounter[13];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

void instance_violet_hold::SetIntroPortals(bool bDeactivate)
{
    for(std::list<uint64>::iterator i = m_lIntroPortalList.begin(); i != m_lIntroPortalList.end(); ++i)
    {
        if (Creature* pPortal = instance->GetCreature(*i))
        {
            if (bDeactivate)
                pPortal->ForcedDespawn();
            else
                pPortal->Respawn();
        }
    }
}

void instance_violet_hold::SpawnPortal()
{
    if (const sPortalData* pData = GetPortalData())
    {
        if (Creature* pController = instance->GetCreature(m_uiSinclariAltGUID))
        {
            uint32 uiPortalEntry;

            switch(pData->pPortalType)
            {
                case PORTAL_TYPE_NORM: uiPortalEntry = NPC_PORTAL; break;
                case PORTAL_TYPE_SQUAD:
                case PORTAL_TYPE_BOSS: uiPortalEntry = NPC_PORTAL_ELITE; break;
            }

            pController->SummonCreature(uiPortalEntry, pData->fX, pData->fY, pData->fZ, pData->fOrient, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 1800*IN_MILLISECONDS);
        }
    }
}

void instance_violet_hold::SetPortalId()
{
    if (IsCurrentPortalForTrash())
    {
        int iTemp = rand()%(m_uiMaxCountPortalLoc - 1);

        if (iTemp >= m_uiPortalId)
            ++iTemp;

        debug_log("SD2: instance_violet_hold: SetPortalId %i, old was id %u.", iTemp, m_uiPortalId);

        m_uiPortalId = iTemp;
    }
    else
    {
        debug_log("SD2: instance_violet_hold: SetPortalId %u (is boss), old was id %u.", m_uiMaxCountPortalLoc, m_uiPortalId);
        m_uiPortalId = m_uiMaxCountPortalLoc;
    }
}

void instance_violet_hold::SetRandomBosses()
{
    while (m_lRandomBossList.size() > 2)
    {
        uint32 uiPosition = urand(0, m_lRandomBossList.size()-1);

        for(std::list<uint32>::iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr, --uiPosition)
        {
            if (!*itr)
                continue;

            if (!uiPosition)
            {
                m_lRandomBossList.erase(itr);
                break;
            }
        }
    }

    for(std::list<uint32>::iterator itr = m_lRandomBossList.begin(); itr != m_lRandomBossList.end(); ++itr)
        debug_log("SD2: instance_violet_hold random boss is entry %u", *itr);
}

void instance_violet_hold::CallGuards(bool bRespawn)
{
    for(std::list<uint64>::iterator i = m_lGuardsList.begin(); i != m_lGuardsList.end(); ++i)
    {
        if (Creature* pGuard = instance->GetCreature(*i))
        {
            if (bRespawn)
            {
                pGuard->Respawn();
            }
            else if (pGuard->isAlive())
            {
                pGuard->AI()->EnterEvadeMode();

                if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                    pGuard->GetMotionMaster()->MoveFollow(pSinclari, 0.0f, 0.0f);

                pGuard->ForcedDespawn(20000);
            }
        }
    }
}

void instance_violet_hold::ProcessActivationCrystal(Unit* pUser, bool bIsIntro)
{
    if (Creature* pSummon = pUser->SummonCreature(NPC_DEFENSE_SYSTEM, fDefenseSystemLoc[0], fDefenseSystemLoc[1], fDefenseSystemLoc[2], fDefenseSystemLoc[3], TEMPSUMMON_TIMED_DESPAWN, 10000))
    {
        pSummon->GetMap()->CreatureRelocation(pSummon, fDefenseSystemLoc[0], fDefenseSystemLoc[1], fDefenseSystemLoc[2], fDefenseSystemLoc[3]);
        pSummon->SendMonsterMove(fDefenseSystemLoc[0], fDefenseSystemLoc[1], fDefenseSystemLoc[2], SPLINETYPE_NORMAL, pSummon->GetSplineFlags(), 1);
        pSummon->CastSpell(pSummon, SPELL_DEFENSE_SYSTEM_VISUAL, true);

        // TODO: figure out how the rest work
        // NPC's NPC_DEFENSE_DUMMY_TARGET are probably channeling some spell to the defense system

        if (bIsIntro)
        {
            DoUseDoorOrButton(m_uiIntroCrystalGUID);
            pSummon->CastSpell(pSummon, SPELL_DEFENSE_SYSTEM_INTRO, true);
        }
        else
        {
            pSummon->CastSpell(pSummon, SPELL_DEFENSE_SYSTEM_DMG, true);
            m_bDefenseUsed = true;

            std::list<GameObject*> lCrystals;
            GetGameObjectListWithEntryInGrid(lCrystals, pSummon, GO_DEFENSE_CRYSTAL, 180.0f);

            if (!lCrystals.empty())
            {
                for(std::list<GameObject*>::iterator iter = lCrystals.begin(); iter != lCrystals.end(); ++iter)
                {
                    if ((*iter))
                        (*iter)->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                }
            }
        }
    }

    // else, kill (and despawn?) certain trash mobs. Also boss affected, but not killed.
}

uint32 instance_violet_hold::GetRandomPortalEliteEntry()
{
    return (urand(0, 1) ? NPC_PORTAL_GUARDIAN : NPC_PORTAL_KEEPER);
}

uint32 instance_violet_hold::GetRandomMobForNormalPortal()
{
    switch(urand(1, 4))
    {
        case 1: return NPC_AZURE_INVADER;
        case 2: return NPC_MAGE_HUNTER;
        case 3: return NPC_AZURE_SPELLBREAKER;
        case 4: return NPC_AZURE_BINDER;
    }

    return 0;
}

uint64 instance_violet_hold::GetData64(uint32 uiData)
{
    switch(uiData)
    {
    case NPC_EREKEM:
        return m_uiErekemGUID;
    case NPC_MORAGG:
        return m_uiMoraggGUID;
    case NPC_ICHORON:
        return m_uiIchoronGUID;
    case NPC_XEVOZZ:
        return m_uiXevozzGUID;
    case NPC_LAVANTHOR:
        return m_uiLavanthorGUID;
    case NPC_ZURAMAT:
        return m_uiZuramatGUID;
    case NPC_SINCLARI_ALT:
        return m_uiSinclariAltGUID;
    case NPC_SINCLARI:
        return m_uiSinclariGUID;
    }
    return 0;
}

uint32 instance_violet_hold::GetData(uint32 uiType)
{
    switch(uiType)
    {
    case TYPE_MAIN:
        return m_auiEncounter[0];
    case TYPE_SEAL:
        return m_auiEncounter[1];
    case TYPE_PORTAL:
        return m_auiEncounter[2];
    case TYPE_FIRST_BOSS:
        return m_auiEncounter[3];
    case TYPE_SECOND_BOSS:
        return m_auiEncounter[4];
    case TYPE_CYANIGOSA:
        return m_auiEncounter[5];
    case TYPE_FIRST_BOSS_ID:
        return m_auiEncounter[6];
    case TYPE_SECOND_BOSS_ID:
        return m_auiEncounter[7];
    case TYPE_EREKEM:
        return m_auiEncounter[8];
    case TYPE_MORAGG:
        return m_auiEncounter[9];
    case TYPE_ICHORON:
        return m_auiEncounter[10];
    case TYPE_XEVOZZ:
        return m_auiEncounter[11];
    case TYPE_LAVANTHOR:
        return m_auiEncounter[12];
    case TYPE_ZURAMAT:
        return m_auiEncounter[13];
    case TYPE_LASTBOSS_ID:
        return m_uiLastBossIDConst;
    case TYPE_LASTBOSS:
        {
            if (m_uiLastBossID == 0)
                m_uiLastBossID = urand(8, 13);
            else
            {
                m_uiLastBossID = urand(8, 13);
                if (m_auiEncounter[3] == DONE && m_auiEncounter[4] == DONE) 
                    return 0;

                while (m_auiEncounter[m_uiLastBossID] == DONE || m_auiEncounter[m_uiLastBossID] == IN_PROGRESS || m_auiEncounter[m_uiLastBossID] == SPECIAL ) 
                    m_uiLastBossID = urand(8, 13);
            }
            return m_uiLastBossID;
        }
    }
    return 0;
}

void instance_violet_hold::Update(uint32 uiDiff)
{
    if (m_auiEncounter[0] != IN_PROGRESS)
        return;

    if (m_uiPortalTimer)
    {
        if (m_uiPortalTimer <= uiDiff)
        {
            DoUpdateWorldState(WORLD_STATE_PORTALS, ++m_uiWorldStatePortalCount);

            SetPortalId();
            SpawnPortal();

            m_uiPortalTimer = 0;
        }
        else
            m_uiPortalTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_violet_hold";
    newscript->GetInstanceData = GetInstanceData_instance_violet_hold;
    newscript->RegisterSelf();
}
