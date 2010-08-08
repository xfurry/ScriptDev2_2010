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
SDName: Instance_Icecrown_Citadel
SD%Complete: 0
SDComment: 
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

struct MANGOS_DLL_DECL instance_icecrown_citadel : public ScriptedInstance
{
    instance_icecrown_citadel(Map* pMap) : ScriptedInstance(pMap) 
    {
        Difficulty = pMap->GetDifficulty();
        Initialize();
    }

    std::string strInstData;
    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint32 Difficulty;
    bool m_bNeedSave;

    // npcs
    uint64 m_uiMarrowgarGUID;
    uint64 m_uiDeathwhisperGUID;
    uint64 m_uiSaurfangGUID;
    uint64 m_uiFestergutGUID;
    uint64 m_uiRotfaceGUID;
    uint64 m_uiPutricideGUID;
    uint64 m_uiValanarGUID;
    uint64 m_uiKelesethGUID;
    uint64 m_uiTaldaramGUID;
    uint64 m_uiLanathelGUID;
    uint64 m_uiSvalnaGUID;
    uint64 m_uiCrokGUID;
    uint64 m_uiArnathGUID;
    uint64 m_uiValithriaGUID;
    uint64 m_uiSindragosaGUID;
    uint64 m_uiLichKingGUID;

    // lower spire
    uint64 m_uiMarrowgarIce1GUID;
    uint64 m_uiMarrowgarIce2GUID;
    uint64 m_uiMarrowgarGateGUID;
    uint64 m_uiDeathwhisperGateGUID;
    uint64 m_uiDeathwhisperElevatorGUID;
    uint64 m_uiSaurfangDoorGUID;

    // plagueworks
    uint64 m_uiFestergutDoorGUID;
    uint64 m_uiRotfaceDoorGUID;
    uint64 m_uiGasValveGUID;
    uint64 m_uiOozeValveGUID;
    uint64 m_uiGreenTubeGUID;
    uint64 m_uiOrangeTubeGUID;
    uint64 m_uiScientistDoorGUID;
    uint64 m_uiScientistDoorGreenGUID;
    uint64 m_uiScientistDoorOrangeGUID;
    uint64 m_uiScientistDoorColisionGUID;
    uint64 m_uiPlagueSigilGUID;

    // crimson halls
    uint64 m_uiBloodwingDoorGUID;
    uint64 m_uiCrimsonHallDoorGUID;
    uint64 m_uiCouncilDoorLeftGUID;
    uint64 m_uiCouncilDoorRightGUID;
    uint64 m_uiBloodQueedDoorGUID;
    uint64 m_uiBloodOrbGUID;
    uint64 m_uiBloodSigilGUID;

	// frostwing
	uint64 m_uiRimefangGUID;
	uint64 m_uiSplinestalkerGUID;

	// frozen throne
	uint64 m_uiTirionFinalGUID;

    // loot
    uint64 m_uiDeathbringersCacheGUID;
    uint64 m_uiGunshipArmoryGUID;
    uint64 m_uiDreamwalkerCacheGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_bNeedSave                     = false;
        m_auiEncounter[12]              = 20;

        // npcs
        m_uiMarrowgarGUID               = 0;
        m_uiDeathwhisperGUID            = 0;
        m_uiSaurfangGUID                = 0;
        m_uiFestergutGUID               = 0;
        m_uiRotfaceGUID                 = 0;
        m_uiPutricideGUID               = 0;
		m_uiValithriaGUID				= 0;

        // lower spire
        m_uiMarrowgarIce1GUID           = 0;
        m_uiMarrowgarIce2GUID           = 0;
        m_uiDeathwhisperGateGUID        = 0;
        m_uiDeathwhisperElevatorGUID    = 0;
        m_uiSaurfangDoorGUID            = 0;

        // plagueworks
        m_uiFestergutDoorGUID           = 0;
        m_uiRotfaceDoorGUID             = 0;
        m_uiGasValveGUID                = 0;
        m_uiOozeValveGUID               = 0;
        m_uiGreenTubeGUID               = 0;
        m_uiOrangeTubeGUID              = 0;
        m_uiScientistDoorGUID           = 0;
        m_uiScientistDoorGreenGUID      = 0;
        m_uiScientistDoorOrangeGUID     = 0;
        m_uiScientistDoorColisionGUID   = 0;
        m_uiPlagueSigilGUID             = 0;

        // crimsonhalls
        m_uiBloodwingDoorGUID           = 0;
        m_uiCrimsonHallDoorGUID         = 0;
        m_uiCouncilDoorLeftGUID         = 0;
        m_uiCouncilDoorRightGUID        = 0;
        m_uiBloodQueedDoorGUID          = 0;
        m_uiBloodOrbGUID                = 0;
        m_uiBloodSigilGUID              = 0;

		// frostwing
		m_uiRimefangGUID				= 0;
		m_uiSplinestalkerGUID			= 0;

		// frozen throne
		m_uiLichKingGUID				= 0;
		m_uiTirionFinalGUID				= 0;

        // loot
        m_uiDeathbringersCacheGUID      = 0;
        m_uiGunshipArmoryGUID           = 0;
        m_uiDreamwalkerCacheGUID        = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_MARROWGAR:     m_uiMarrowgarGUID       = pCreature->GetGUID(); break;
            case NPC_DEATHWHISPER:  m_uiDeathwhisperGUID    = pCreature->GetGUID(); break;
            case NPC_SAURFANG:      m_uiSaurfangGUID        = pCreature->GetGUID(); break;
            case NPC_FESTERGUT:     m_uiFestergutGUID       = pCreature->GetGUID(); break;
            case NPC_ROTFACE:       m_uiRotfaceGUID         = pCreature->GetGUID(); break;
            case NPC_PUTRICIDE:     m_uiPutricideGUID       = pCreature->GetGUID(); break;
            case NPC_VALANAR:       m_uiValanarGUID         = pCreature->GetGUID(); break;
            case NPC_KELESETH:      m_uiKelesethGUID        = pCreature->GetGUID(); break;
            case NPC_TALDARAM:      m_uiTaldaramGUID        = pCreature->GetGUID(); break;
            case NPC_LANATHEL:      m_uiLanathelGUID        = pCreature->GetGUID(); break;
			case NPC_VALITHRIA:		m_uiValithriaGUID		= pCreature->GetGUID(); break;
			case NPC_SPLINESTALKER:	m_uiSplinestalkerGUID	= pCreature->GetGUID(); break;
			case NPC_RIMEFANG:		m_uiRimefangGUID		= pCreature->GetGUID(); break;
			case NPC_LICH_KING:		m_uiLichKingGUID		= pCreature->GetGUID(); break;
			case NPC_TIRION_FINAL:  m_uiTirionFinalGUID		= pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        Map::PlayerList const &players = instance->GetPlayers();
        uint32 TeamInInstance = 0;

        if (!players.isEmpty())
        {
            if (Player* pPlayer = players.begin()->getSource())
                TeamInInstance = pPlayer->GetTeam();
        }

        switch(pGo->GetEntry())
        {
            // doors & other
            // lower spire
            case GO_MARROWGAR_ICE_1:
                m_uiMarrowgarIce1GUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_MARROWGAR_ICE_2:
                m_uiMarrowgarIce2GUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if (m_auiEncounter[0] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_MARROWGAR_GATE:
                m_uiMarrowgarGateGUID = pGo->GetGUID();
                break;
            case GO_DEATHWHISPER_GATE:
                m_uiDeathwhisperGateGUID = pGo->GetGUID();
                break;
            case GO_DEATHWHISPER_ELEVATOR:
                m_uiDeathwhisperElevatorGUID = pGo->GetGUID();
                break;
            case GO_SAURFANG_DOOR:
                m_uiSaurfangDoorGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if (m_auiEncounter[3] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
                // plagueworks
            case GO_GREEN_MONSTER_ENTRANCE:
                m_uiRotfaceDoorGUID = pGo->GetGUID();
                break;
            case GO_ORANGE_MONSTER_ENTRANCE:
                m_uiFestergutDoorGUID = pGo->GetGUID();
                break;
            case GO_SCIENTIST_ENTRANCE:
                m_uiScientistDoorGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_GAS_VALVE:
                m_uiGasValveGUID = pGo->GetGUID();
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
            case GO_OOSE_VALVE:
                m_uiOozeValveGUID = pGo->GetGUID();
                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                break;
            case GO_GREEN_TUBE:
                m_uiGreenTubeGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_ACTIVE);
                if(m_auiEncounter[5] == DONE)
                    pGo->SetGoState(GO_STATE_READY);
                break;
            case GO_ORANGE_TUBE:
                m_uiOrangeTubeGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_ACTIVE);
                if(m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_READY);
                break;
            case GO_SCIENTIST_DOOR_GREEN:
                m_uiScientistDoorGreenGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[5] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SCIENTIST_DOOR_ORANGE:
                m_uiScientistDoorOrangeGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_READY);
                if(m_auiEncounter[4] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_SCIENTIST_DOOR_COLISION:
                m_uiScientistDoorColisionGUID = pGo->GetGUID();
                if(m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_PLAGUEWING_SIGIL:
                m_uiPlagueSigilGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_ACTIVE);
                if(m_auiEncounter[6] == DONE)
                    pGo->SetGoState(GO_STATE_READY);
                break;
                // crimson halls
            case GO_BLOODWING_DOOR:
                m_uiBloodwingDoorGUID = pGo->GetGUID();
                if(m_auiEncounter[6] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_CRIMSON_HALL_DOOR:
                m_uiCrimsonHallDoorGUID = pGo->GetGUID();
                pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BLOOD_COUNCIL_DOOR_LEFT:
                m_uiCouncilDoorLeftGUID = pGo->GetGUID();
                if(m_auiEncounter[7] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BLOOD_COUNCIL_DOOR_RIGHT:
                m_uiCouncilDoorRightGUID = pGo->GetGUID();
                if(m_auiEncounter[7] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_BLOODQUEEN_DOOR:
                m_uiBloodQueedDoorGUID = pGo->GetGUID();
                break;
            case GO_EMPOWERING_BLOOD_ORB:
                m_uiBloodOrbGUID = pGo->GetGUID();
                break;
            case GO_BLOODWING_SIGIL:
                m_uiBloodSigilGUID = pGo->GetGUID();
                break;
                // frostwyrm halls
                // frozen throne

                // loot
            case GO_DEATHBRINGERS_CACHE_10:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    m_uiDeathbringersCacheGUID = pGo->GetGUID();
                break;
            case GO_DEATHBRINGERS_CACHE_10HC:
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    m_uiDeathbringersCacheGUID = pGo->GetGUID();
                break;
            case GO_DEATHBRINGERS_CACHE_25:
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_uiDeathbringersCacheGUID = pGo->GetGUID();
                break;
            case GO_DEATHBRINGERS_CACHE_25HC:
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    m_uiDeathbringersCacheGUID = pGo->GetGUID();
                break;
            case GO_GUNSHIP_ARMORY_ALY_10:
                if(TeamInInstance == ALLIANCE)
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_ALY_10HC:
                if(TeamInInstance == ALLIANCE)
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_ALY_25:
                if(TeamInInstance == ALLIANCE)
                {
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_ALY_25HC:
                if(TeamInInstance == ALLIANCE)
                {
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_HORDE_10:
                if(TeamInInstance == HORDE)
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_HORDE_10HC:
                if(TeamInInstance == HORDE)
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_HORDE_25:
                if(TeamInInstance == HORDE)
                {
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_GUNSHIP_ARMORY_HORDE_25HC:
                if(TeamInInstance == HORDE)
                {
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        m_uiGunshipArmoryGUID = pGo->GetGUID();
                }
                break;
            case GO_CACHE_OF_DREAMWALKER_10:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    m_uiDreamwalkerCacheGUID = pGo->GetGUID();
                break;
            case GO_CACHE_OF_DREAMWALKER_10HC:
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    m_uiDreamwalkerCacheGUID = pGo->GetGUID();
                break;
            case GO_CACHE_OF_DREAMWALKER_25:
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_uiDreamwalkerCacheGUID = pGo->GetGUID();
                break;
            case GO_CACHE_OF_DREAMWALKER_25HC:
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    m_uiDreamwalkerCacheGUID = pGo->GetGUID();
                break;
        }
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OpenPutricideDoor()
    {
        if(m_auiEncounter[4] == DONE && m_auiEncounter[5] == DONE)
            DoUseDoorOrButton(m_uiScientistDoorColisionGUID);
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_MARROWGAR:
                m_auiEncounter[0] = uiData;
                DoUseDoorOrButton(m_uiMarrowgarGateGUID);
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiMarrowgarIce1GUID);
                    DoUseDoorOrButton(m_uiMarrowgarIce2GUID);
                }
                break;
            case TYPE_DEATHWHISPER:
                m_auiEncounter[1] = uiData;
                DoUseDoorOrButton(m_uiDeathwhisperGateGUID);
				if(uiData == DONE)
				{
					if(GameObject* pLift = instance->GetGameObject(m_uiDeathwhisperElevatorGUID))
						pLift->SetGoState(GO_STATE_ACTIVE);
				}
                break;
            case TYPE_GUNSHIP_BATTLE:
                m_auiEncounter[2] = uiData;
                //if (uiData == DONE)
                //    DoRespawnGameObject(m_uiGunshipArmoryGUID, 30*MINUTE);
                break;
            case TYPE_SAURFANG:
                m_auiEncounter[3] = uiData;
                if (uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiSaurfangDoorGUID);
                    DoRespawnGameObject(m_uiDeathbringersCacheGUID, 30*MINUTE);
                }
                break;
            case TYPE_FESTERGUT:
                m_auiEncounter[4] = uiData;
                DoUseDoorOrButton(m_uiFestergutDoorGUID);
                if(uiData == DONE)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiGasValveGUID))
                        pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                    DoUseDoorOrButton(m_uiOrangeTubeGUID);
                    DoUseDoorOrButton(m_uiScientistDoorOrangeGUID);
                    OpenPutricideDoor();
                }
                break;
            case TYPE_ROTFACE:
                m_auiEncounter[5] = uiData;
                DoUseDoorOrButton(m_uiRotfaceDoorGUID);
                if(uiData == DONE)
                {
                    if (GameObject* pGo = instance->GetGameObject(m_uiOozeValveGUID))
                        pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                    DoUseDoorOrButton(m_uiGreenTubeGUID);
                    DoUseDoorOrButton(m_uiScientistDoorGreenGUID);
                    OpenPutricideDoor();
                }
                break;
            case TYPE_PUTRICIDE:
                m_auiEncounter[6] = uiData;
                DoUseDoorOrButton(m_uiScientistDoorGUID);
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiPlagueSigilGUID);
                    DoUseDoorOrButton(m_uiBloodwingDoorGUID);
                }
                break;
            case TYPE_PRINCE_COUNCIL:
                m_auiEncounter[7] = uiData;
                DoUseDoorOrButton(m_uiCrimsonHallDoorGUID);
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiCouncilDoorLeftGUID);
                    DoUseDoorOrButton(m_uiCouncilDoorRightGUID);
                }
                break;
            case TYPE_BLOOD_QUEEN:
                m_auiEncounter[8] = uiData;
                DoUseDoorOrButton(m_uiBloodQueedDoorGUID);
                if(uiData == DONE)
                {
                    DoUseDoorOrButton(m_uiBloodSigilGUID);
                    //DoUseDoorOrButton(m_uiBloodwingDoorGUID);
                }
                break;
            case TYPE_DREAMWALKER:
                m_auiEncounter[9] = uiData;
                if(uiData == DONE)
                    DoRespawnGameObject(m_uiDreamwalkerCacheGUID, 30* MINUTE);
                break;
            case TYPE_SINDRAGOSA:
                m_auiEncounter[10] = uiData;
                break;
            case TYPE_LICH_KING:
                m_auiEncounter[11] = uiData;
                break;
            case TYPE_ATTEMPTS:
                m_auiEncounter[12] = uiData;
                m_bNeedSave = true;
                break;
        }

        if (uiData == DONE || m_bNeedSave)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
                << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
                << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
                << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " " << m_auiEncounter[12];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
            m_bNeedSave = false;
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
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3] 
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7] 
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11] >> m_auiEncounter[12];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_MARROWGAR:
                return m_auiEncounter[0];
            case TYPE_DEATHWHISPER:
                return m_auiEncounter[1];
            case TYPE_GUNSHIP_BATTLE:
                return m_auiEncounter[2];
            case TYPE_SAURFANG:
                return m_auiEncounter[3];
            case TYPE_FESTERGUT:
                return m_auiEncounter[4];
            case TYPE_ROTFACE:
                return m_auiEncounter[5];
            case TYPE_PUTRICIDE:
                return m_auiEncounter[6];
            case TYPE_PRINCE_COUNCIL:
                return m_auiEncounter[7];
            case TYPE_BLOOD_QUEEN:
                return m_auiEncounter[8];
            case TYPE_DREAMWALKER:
                return m_auiEncounter[9];
            case TYPE_SINDRAGOSA:
                return m_auiEncounter[10];
            case TYPE_LICH_KING:
                return m_auiEncounter[11];
            case TYPE_ATTEMPTS:
                return m_auiEncounter[12];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_MARROWGAR:
                return m_uiMarrowgarGUID;
            case NPC_DEATHWHISPER:
                return m_uiDeathwhisperGUID;
            case NPC_SAURFANG:
                return m_uiSaurfangGUID;
            case NPC_FESTERGUT:
                return m_uiFestergutGUID;
            case NPC_ROTFACE:
                return m_uiRotfaceGUID;
            case NPC_PUTRICIDE:
                return m_uiPutricideGUID;
			case NPC_VALITHRIA:
				return m_uiValithriaGUID;
			case NPC_RIMEFANG:
				return m_uiRimefangGUID;
			case NPC_SPLINESTALKER:
				return m_uiSplinestalkerGUID;
			case NPC_LICH_KING:
				return m_uiLichKingGUID;
			case NPC_TIRION_FINAL:
				return m_uiTirionFinalGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_icecrown_citadel(Map* pMap)
{
    return new instance_icecrown_citadel(pMap);
}

void AddSC_instance_icecrown_citadel()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_citadel";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_citadel;
    pNewScript->RegisterSelf();
}
