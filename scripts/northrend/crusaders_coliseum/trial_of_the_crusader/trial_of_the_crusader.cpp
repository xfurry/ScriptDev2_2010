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
SDName: trial_of_the_crusader
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    // aly
    SAY_VARIAN_INTRO1           = -1605100,
    SAY_VARIAN_INTRO2           = -1605101,
    SAY_VARIAN_INTRO3           = -1605102,
    SAY_JAINA_INTRO1            = -1605104,
    SAY_JAINA_INTRO2            = -1605105,

    //horde
    SAY_GARROSH_INTRO1          = -1605106,
    SAY_GARROSH_INTRO2          = -1605129,
    SAY_GARROSH_INTRO3          = -1605130,
    SAY_GARROSH_INTRO4          = -1605107,
    SAY_GARROSH_INTRO5          = -1605108,
    SAY_THRALL_INTRO1           = -1605110,
    SAY_THRALL_INTRO2           = -1605111,
    SAY_THRALL_INTRO3           = -1605112,
    SAY_THRALL_INTRO4           = -1605113,
    SAY_THRALL_INTRO5           = -1605114,
    SAY_THRALL_INTRO6           = -1605115,

    //tirion
    SAY_TIRION_WELCOME          = -1605120,
    SAY_TIRION_TRIAL            = -1605122,
    SAY_TIRION_ALY1             = -1605123,
    SAY_TIRION_ALY2             = -1605124,
    SAY_TIRION_HORDE1           = -1605125,
    SAY_TIRION_HORDE2           = -1605126,
    SAY_TIRION_HORDE3           = -1605127,
    SAY_TIRION_HORDE4           = -1605128,

    // beasts
    SAY_SUMMON_GORMOK           = -1605001,
    SAY_SUMMON_JORMUNGARS       = -1605002,
    SAY_SUMMON_ICEHOWL          = -1605003,
    SAY_BEASTS_VICTORY          = -1605000,

    // jaru
    SAY_TIRION_JARU_INTRO       = -1605005,
    SAY_WILFRED_INTRO2          = -1605006,
    SAY_WILFRED_INTRO3          = -1605007,
    SAY_WILFRED_INTRO4          = -1605008,
    SAY_WILFRED_INTRO5          = -1605009,
    SAY_JARU_INTRO6             = -1605010,
    SAY_WILFRED_INTRO7          = -1605011,
    SAY_WILFRED_INTRO8          = -1605012,
    SAY_TIRION_JARU_INTRO9      = -1605013,
    SAY_JARU_AGGRO              = -1605014,
    SAY_TIRION_JARU_OUTRO1      = -1605021,
    SAY_GORRASH_JARU_OUTRO2     = -1605022,
    SAY_VARIM_JARU_OUTRO3       = -1605023,
    SAY_TIRION_JARU_OUTRO4      = -1605024,

    SPELL_WILFRED_PORTAL        = 68424,

    //champions
    SAY_TIRION_CHAMP_INTRO1     = -1605027,
    SAY_TIRION_CHAMP_INTRO2     = -1605028,
    SAY_GARROSH_CHAMP1          = -1605030,
    SAY_GARROSH_CHAMP2          = -1605031,
    SAY_VARIAN_CHAMP1           = -1605040,
    SAY_VARIAN_CHAMP2           = -1605041,
    SAY_VICTORY_HORDE           = -1605042,
    SAY_VICTORY_ALY             = -1605032,
    SAY_OUTRO_TIRION            = -1605026,

    // twin angels
    SAY_TIRION_TWINS_INTRO1     = -1605050,
    SAY_TIRION_TWINS_INTRO2     = -1605051,
    SAY_GORRASH_TWINS_DEATH     = -1605080,
    SAY_VARIM_TWINS_DEATH       = -1605081,

    // lich king
    SAY_TIRION_ANUB_INTRO1      = -1605060, 
    SAY_LICHKING_INTRO2         = -1605061,
    SAY_LICHKING_INTRO3         = -1605062,
    SAY_TIRION_ANUB_INTRO4      = -1605063,
    SAY_LICHKING_INTRO5         = -1605064,
    SAY_LICHKING_INTRO6         = -1605065,
    SAY_LICHKING_INTRO7         = -1605066,
    SAY_LICHKING_INTRO8         = -1605075,

    SPELL_BERSERK               = 26662,

    ACHIEV_RESILIENCE_WILL_FIX      = 3798,
    ACHIEV_RESILIENCE_WILL_FIX_H    = 3814,

	GOSSIP_ITEM_START_EVENT1    = -3649000
};

struct TocMessages
{
    char const* name;
    uint32 id;
    bool state;
    uint32 encounter;
};

static TocMessages TocGossipMessage[]=
{ 
    {"We are ready for the first challenge!",       GOSSIP_ACTION_INFO_DEF+1,   false,  TYPE_NORTHREND_BEASTS},     //  summon northernd beasts
    {"We are ready for the next challenge!",        GOSSIP_ACTION_INFO_DEF+2,   false,  TYPE_JARAXXUS},             //  summon jarraxus
    {"We are ready to fight the next challenge!",   GOSSIP_ACTION_INFO_DEF+3,   false,  TYPE_FACTION_CHAMPIONS},    //  summon aly champs
    {"We are ready to fight the next challenge!",   GOSSIP_ACTION_INFO_DEF+4,   false,  TYPE_FACTION_CHAMPIONS},    //  summon horde champs
    {"We are ready to face the next challenge!",    GOSSIP_ACTION_INFO_DEF+5,   false,  TYPE_TWIN_VALKYR},          //  summon twin valkyrs
    {"We are ready to face the next challenge!",    GOSSIP_ACTION_INFO_DEF+6,   false,  TYPE_ANUBARAK},             //  summon lich king
    {"Arena is closed",                             GOSSIP_ACTION_INFO_DEF+7,   true,   TYPE_ANUBARAK},             //  event finished
};
enum
{
    NUM_MESSAGES = 8
};

struct MANGOS_DLL_DECL npc_tirionAI : public ScriptedAI
{
    npc_tirionAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 IntroTimer;
    uint32 IntroStep;

    uint32 m_uiDoorTimer;
    bool doorClosed;

    uint64 m_uiWilfredGUID;
    uint64 m_uiJaraxxusGUID;
    uint64 m_uiJaruPortalGUID;

    bool isJaruIntro;

    bool isChampIntro;
    uint8 faction;

	uint32 m_uiCrusaderEntry1;
	uint32 m_uiCrusaderEntry2;
	uint32 m_uiCrusaderEntry3;
	uint32 m_uiCrusaderEntry4;
	uint32 m_uiCrusaderEntry5;
	uint32 m_uiCrusaderEntry6;
	uint32 m_uiCrusaderEntry7;
	uint32 m_uiCrusaderEntry8;
	uint32 m_uiCrusaderEntry9;
	uint32 m_uiCrusaderEntry10;

	std::vector<uint32> CrusaderHealers;
	std::vector<uint32> CrusaderRanged;
	std::vector<uint32> CrusaderMelee;

	void Reset()
    {
        IntroTimer          = 10000;
        IntroStep           = 1;
        m_uiDoorTimer       = 5000;
        doorClosed          = false;
        m_uiWilfredGUID     = 0;
        m_uiJaraxxusGUID    = 0;
        m_uiJaruPortalGUID  = 0;
        isJaruIntro         = false;

        isChampIntro        = false;
        faction = 0;

		m_uiCrusaderEntry1	= 0;
		m_uiCrusaderEntry2	= 0;
		m_uiCrusaderEntry3	= 0;
		m_uiCrusaderEntry4	= 0;
		m_uiCrusaderEntry5	= 0;
		m_uiCrusaderEntry6	= 0;
		m_uiCrusaderEntry7	= 0;
		m_uiCrusaderEntry8	= 0;
		m_uiCrusaderEntry9	= 0;
		m_uiCrusaderEntry10	= 0;

		CrusaderHealers.reserve(4);
		CrusaderRanged.reserve(5);
		CrusaderMelee.reserve(5);
    }

    void SetJaruIntro()
    {
        IntroTimer          = 10000;
        IntroStep           = 1;
        isJaruIntro = true;
    }

    void SetChampIntro(uint8 fact)
    {
        IntroTimer          = 10000;
        IntroStep           = 1;
        isChampIntro = true;
        faction = fact;
    }

	void SummonCrusaders()
	{
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry1, SpawnLoc[3].x, SpawnLoc[3].y - 13, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_1, pTemp->GetGUID());
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry2, SpawnLoc[4].x, SpawnLoc[4].y - 13, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_2, pTemp->GetGUID());
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry3, SpawnLoc[5].x, SpawnLoc[5].y - 13, SpawnLoc[5].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_3, pTemp->GetGUID());
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry4, SpawnLoc[6].x, SpawnLoc[6].y - 13, SpawnLoc[6].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_4, pTemp->GetGUID());
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry5, SpawnLoc[7].x, SpawnLoc[7].y - 13, SpawnLoc[7].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_5, pTemp->GetGUID());
		if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry6, SpawnLoc[8].x, SpawnLoc[8].y - 13, SpawnLoc[8].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
			m_pInstance->SetData64(DATA_CRUSADER_6, pTemp->GetGUID());

		if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
        {
			if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry7, SpawnLoc[9].x, SpawnLoc[9].y - 13, SpawnLoc[9].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
				m_pInstance->SetData64(DATA_CRUSADER_7, pTemp->GetGUID());
			if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry8, SpawnLoc[10].x, SpawnLoc[10].y - 13, SpawnLoc[10].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
				m_pInstance->SetData64(DATA_CRUSADER_8, pTemp->GetGUID());
			if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry9, SpawnLoc[11].x, SpawnLoc[11].y - 13, SpawnLoc[11].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
				m_pInstance->SetData64(DATA_CRUSADER_9, pTemp->GetGUID());
			if(Creature* pTemp = m_creature->SummonCreature(m_uiCrusaderEntry10, SpawnLoc[12].x, SpawnLoc[12].y - 13, SpawnLoc[12].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
				m_pInstance->SetData64(DATA_CRUSADER_10, pTemp->GetGUID());
		}
	}

	void SetPattern()
	{
		// on 10 man:
		// 1-2 heals
		// 2-3 ranged
		// 2-3 melee
		// total = 6

		// on 25 man
		// 2-3 heals
		// 3-4 ranged
		// 3-4 melee
		// total = 10

		switch(urand(0, 3))
		{
		case 0: // 1-2-3 || 2-4-4
			// heal
			m_uiCrusaderEntry1 = CrusaderHealers[1];
			m_uiCrusaderEntry9 = CrusaderHealers[2];	// 25

			// ranged
			m_uiCrusaderEntry2 = CrusaderRanged[1];
			m_uiCrusaderEntry4 = CrusaderRanged[2];
			m_uiCrusaderEntry7 = CrusaderRanged[3];		// 25
			m_uiCrusaderEntry10 = CrusaderRanged[0];

			// melee
			m_uiCrusaderEntry3 = CrusaderMelee[0];
			m_uiCrusaderEntry5 = CrusaderMelee[1];
			m_uiCrusaderEntry6 = CrusaderMelee[2];
			m_uiCrusaderEntry8 = CrusaderMelee[4];		// 25
			break;
		case 1:	// 2-2-2 || 3-3-4
			// heal
			m_uiCrusaderEntry1 = CrusaderHealers[1];
			m_uiCrusaderEntry5 = CrusaderHealers[2];	// 25
			m_uiCrusaderEntry10 = CrusaderHealers[0];

			// ranged
			m_uiCrusaderEntry2 = CrusaderRanged[1];
			m_uiCrusaderEntry4 = CrusaderRanged[2];
			m_uiCrusaderEntry7 = CrusaderRanged[3];		// 25

			// melee
			m_uiCrusaderEntry3 = CrusaderMelee[0];
			m_uiCrusaderEntry6 = CrusaderMelee[2];
			m_uiCrusaderEntry9 = CrusaderMelee[1];		// 25
			m_uiCrusaderEntry8 = CrusaderMelee[4];		
			break;
		case 2:	// 1-3-2 || 2-3-5
			// heal
			m_uiCrusaderEntry1 = CrusaderHealers[1];
			m_uiCrusaderEntry9 = CrusaderHealers[2];	// 25

			// ranged
			m_uiCrusaderEntry2 = CrusaderRanged[1];
			m_uiCrusaderEntry4 = CrusaderRanged[2];
			m_uiCrusaderEntry6 = CrusaderRanged[3];
			
			// melee
			m_uiCrusaderEntry3 = CrusaderMelee[0];
			m_uiCrusaderEntry5 = CrusaderMelee[1];
			m_uiCrusaderEntry7 = CrusaderMelee[2];		// 25
			m_uiCrusaderEntry8 = CrusaderMelee[4];		
			m_uiCrusaderEntry10 = CrusaderMelee[3];
			break;
		case 3:	// 2-3-1 || 3-4-3
			// heal
			m_uiCrusaderEntry1 = CrusaderHealers[1];
			m_uiCrusaderEntry5 = CrusaderHealers[2];	// 25
			m_uiCrusaderEntry10 = CrusaderHealers[0];

			// ranged
			m_uiCrusaderEntry2 = CrusaderRanged[1];
			m_uiCrusaderEntry4 = CrusaderRanged[2];
			m_uiCrusaderEntry6 = CrusaderRanged[3];		
			m_uiCrusaderEntry8 = CrusaderRanged[4];		// 25

			// melee
			m_uiCrusaderEntry3 = CrusaderMelee[0];		// 25
			m_uiCrusaderEntry7 = CrusaderMelee[2];
			m_uiCrusaderEntry9 = CrusaderMelee[1];		
			break;
		}
	}

    void SummonHorde()
    {
        CrusaderHealers.clear();
		CrusaderRanged.clear();
		CrusaderMelee.clear();

		CrusaderHealers.push_back(NPC_MISTHOOF);
		CrusaderHealers.push_back(NPC_SUNCALLER);
		CrusaderHealers.push_back(NPC_STERN);
		CrusaderHealers.push_back(NPC_THRAKGAR);

		if (!CrusaderHealers.empty())
			std::random_shuffle(CrusaderHealers.begin(), CrusaderHealers.end());

		CrusaderRanged.push_back(NPC_STORMHOOF);
		CrusaderRanged.push_back(NPC_RUJKAH);
		CrusaderRanged.push_back(NPC_BLIGHTSLINGER);
		CrusaderRanged.push_back(NPC_BLACKWHISPER);
		CrusaderRanged.push_back(NPC_HARKZOG);

		if (!CrusaderRanged.empty())
			std::random_shuffle(CrusaderRanged.begin(), CrusaderRanged.end());

		CrusaderMelee.push_back(NPC_SHADOWCLEAVE);
		CrusaderMelee.push_back(NPC_BRIGHTBLADE);
		CrusaderMelee.push_back(NPC_MAZDINAH);
		CrusaderMelee.push_back(NPC_STOUTHORM);
		CrusaderMelee.push_back(NPC_STEELBREAKER);

		if (!CrusaderMelee.empty())
			std::random_shuffle(CrusaderMelee.begin(), CrusaderMelee.end());
		
		SetPattern();
		SummonCrusaders();
    }

    void SummonAly()
    {
		CrusaderHealers.clear();
		CrusaderRanged.clear();
		CrusaderMelee.clear();

		CrusaderHealers.push_back(NPC_VALESTRIDER);
		CrusaderHealers.push_back(NPC_VALENAA);
		CrusaderHealers.push_back(NPC_FORGEMENDER);
		CrusaderHealers.push_back(NPC_SAAMUL);

		if (!CrusaderHealers.empty())
			std::random_shuffle(CrusaderHealers.begin(), CrusaderHealers.end());

		CrusaderRanged.push_back(NPC_GROVESONG);
		CrusaderRanged.push_back(NPC_MOONSTALKER);
		CrusaderRanged.push_back(NPC_WHIZZLESTICK);
		CrusaderRanged.push_back(NPC_NIGHTFELL);
		CrusaderRanged.push_back(NPC_GRIMDABBLER);

		if (!CrusaderRanged.empty())
			std::random_shuffle(CrusaderRanged.begin(), CrusaderRanged.end());

		CrusaderMelee.push_back(NPC_DUSKBLADE);
		CrusaderMelee.push_back(NPC_LIGHTBEARER);
		CrusaderMelee.push_back(NPC_SHADOWSTEP);
		CrusaderMelee.push_back(NPC_SHAABAD);
		CrusaderMelee.push_back(NPC_SHOCUUL);

		if (!CrusaderMelee.empty())
			std::random_shuffle(CrusaderMelee.begin(), CrusaderMelee.end());

        SetPattern();
		SummonCrusaders();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(isJaruIntro)
        {
            if (m_uiDoorTimer < uiDiff && !doorClosed)
            {
                doorClosed = true;
                m_uiDoorTimer = 30000;
            }
            else
                m_uiDoorTimer -= uiDiff;

            switch(IntroStep)
            {
            case 1:
                DoScriptText(SAY_TIRION_JARU_INTRO, m_creature);
                if(Creature* Wilfred = m_creature->SummonCreature(NPC_WILFRED, SpawnLoc[28].x, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    m_uiWilfredGUID = Wilfred->GetGUID();
                    Wilfred->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y + 10, SpawnLoc[1].z);
                }
                ++IntroStep;
                IntroTimer = 33000;
                break;
            case 3:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                    DoScriptText(SAY_WILFRED_INTRO2, Wilfred);
                if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                    m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
                ++IntroStep;
                IntroTimer = 10000;
                break;
            case 5:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                {
                    if(Creature* pPortal = m_creature->SummonCreature(NPC_PORTAL, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 20000))
                    {
                        pPortal->SetDisplayId(11686);
                        pPortal->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.0f);
                        m_uiJaruPortalGUID = pPortal->GetGUID();
                        pPortal->CastSpell(pPortal, SPELL_WILFRED_PORTAL, true);
                        DoScriptText(SAY_WILFRED_INTRO3, Wilfred);
                    }
                }
                ++IntroStep;
                IntroTimer = 5000;
                break;
            case 7:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                {
                    DoScriptText(SAY_WILFRED_INTRO4, Wilfred);
                    if(Creature* Jaru = m_creature->SummonCreature(NPC_JARAXXUS, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
                    {
                        Jaru->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        Jaru->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y - 10, SpawnLoc[1].z);
                        Jaru->SetUInt64Value(UNIT_FIELD_TARGET, Wilfred->GetGUID());
                        m_uiJaraxxusGUID = Jaru->GetGUID();
                    }
                }
                ++IntroStep;
                IntroTimer = 3000;
                break;
            case 9:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                    DoScriptText(SAY_WILFRED_INTRO5, Wilfred);
                ++IntroStep;
                IntroTimer = 12000;
                break;
            case 11:
                if(Creature* Jaru = m_pInstance->instance->GetCreature(m_uiJaraxxusGUID))
                {    
                    DoScriptText(SAY_JARU_INTRO6, Jaru);
                    if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                        Jaru->AI()->AttackStart(Wilfred);
                }
                ++IntroStep;
                IntroTimer = 4000;
                break;
            case 13:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                    DoScriptText(SAY_WILFRED_INTRO7, Wilfred);
                if(Creature* pPortal = m_pInstance->instance->GetCreature(m_uiJaruPortalGUID))
                    pPortal->ForcedDespawn();
                ++IntroStep;
                IntroTimer = 2000;
                break;
            case 15:
                if(Creature* Wilfred = m_pInstance->instance->GetCreature(m_uiWilfredGUID))
                {
                    DoScriptText(SAY_WILFRED_INTRO8, Wilfred);
                    if(Creature* Jaru = m_pInstance->instance->GetCreature(m_uiJaraxxusGUID))
                    {
                        Jaru->CastSpell(Wilfred, 67888, true);
                        Jaru->DeleteThreatList();
                        Jaru->CombatStop();
                    }
                }
                ++IntroStep;
                IntroTimer = 3000;
                break;
            case 17:
                DoScriptText(SAY_TIRION_JARU_INTRO9, m_creature);
                ++IntroStep;
                IntroTimer = 8000;
                break;
            case 19:
                if(Creature* Jaru = m_pInstance->instance->GetCreature(m_uiJaraxxusGUID))
                {
                    Jaru->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    Jaru->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                    Jaru->SetInCombatWithZone();
                    m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);
                }
                ++IntroStep;
                isJaruIntro = false;
                IntroTimer = 9000;
                break;
            }

            if (IntroTimer <= uiDiff)
            {
                ++IntroStep;
                IntroTimer = 330000;
            } IntroTimer -= uiDiff;
        }

        if(isChampIntro)
        {
            switch(IntroStep)
            {
            case 1:
                DoScriptText(SAY_TIRION_CHAMP_INTRO1, m_creature);
                ++IntroStep;
                IntroTimer = 8000;
                break;
            case 3:
                if(faction == 1)
                {
                    if(Creature* Varian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
                        DoScriptText(SAY_VARIAN_CHAMP1, Varian);
                }
                if(faction == 2)
                {
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_CHAMP1, Garrosh);
                }
                ++IntroStep;
                IntroTimer = 15000;
                break;
            case 5:
                DoScriptText(SAY_TIRION_CHAMP_INTRO2, m_creature);
                ++IntroStep;
                IntroTimer = 5000;
                break;
            case 7:
                if(faction == 2)
                {
                    if(Creature* Varian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
                    {
                        DoScriptText(SAY_VARIAN_CHAMP2, Varian);
                        SummonHorde();
                        m_pInstance->SetData(TYPE_STAGE,4);
                    }
                }
                if(faction == 1)
                {
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                    {
                        DoScriptText(SAY_GARROSH_CHAMP2, Garrosh);
                        SummonAly();
                        m_pInstance->SetData(TYPE_STAGE,3);
                    }
                }
                isChampIntro = false;
                ++IntroStep;
                IntroTimer = 3000;
                break;
            }

            if (IntroTimer <= uiDiff)
            {
                ++IntroStep;
                IntroTimer = 330000;
            } IntroTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_tirion(Creature* pCreature)
{
    return new npc_tirionAI (pCreature);
}

/*######
## npc_crusader_anouncer
######*/

struct MANGOS_DLL_DECL npc_crusader_anouncerAI : public ScriptedAI
{
    npc_crusader_anouncerAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    bool flag25;

    bool m_bIsIntro;
    bool isIntroDone;
    uint8 m_uiIntro_Phase;
    uint32 m_uiSpeech_Timer;

    bool m_bIsAchiev;
    uint32 m_uiResilienceAchievTimer;
    uint32 m_uiBeastsBerserkTimer;
    bool m_bHasBerserk;

    void Reset() 
    {
        m_pInstance->SetData(TYPE_STAGE,0);
        flag25 = true;

        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;
        isIntroDone         = false;
        m_bIsAchiev         = false;
        m_bHasBerserk       = false;
        m_uiResilienceAchievTimer   = 0;
        m_uiBeastsBerserkTimer  = 6000000;
    }

    /*void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && m_pInstance->GetData(TYPE_INTRO) != DONE &&
            !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 110) && !isIntroDone)
            m_bIsIntro = true;
    }*/

    void SetBeastsBerserk()
    {
        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiBeastsBerserkTimer = 900000;   // 15 min
        else if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            m_uiBeastsBerserkTimer = 522000;   // 8.7 min
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // achiev timer
        if(m_bIsAchiev)
            m_uiResilienceAchievTimer += uiDiff;

        if (m_bIsIntro && !isIntroDone)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_WELCOME, Tirion);

                    /* temp */
                    m_bIsIntro = false;
                    isIntroDone = true;
                    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if(m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_COUNTER,50);
                        m_pInstance->SetData(TYPE_INTRO,DONE);
                    }
                    /* temp */

                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 23000;
                    break;
                    // aly intro
                case 1:
                    if(Creature* Varian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
                        DoScriptText(SAY_VARIAN_INTRO1, Varian);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 3000;
                    break;
                case 2:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_ALY1, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 3:
                    if(Creature* Jaina = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_JAINA)))
                        DoScriptText(SAY_JAINA_INTRO1, Jaina);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 4:
                    if(Creature* Varian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
                        DoScriptText(SAY_VARIAN_INTRO2, Varian);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 5:
                    if(Creature* Jaina = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_JAINA)))
                        DoScriptText(SAY_JAINA_INTRO2, Jaina);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 6:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_ALY2, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 7:
                    if(Creature* Varian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
                        DoScriptText(SAY_VARIAN_INTRO3, Varian);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                    //horde intro
                case 8:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO1, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 9:
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_INTRO1, Garrosh);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 10:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO2, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 11:
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_INTRO2, Garrosh);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 12:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_HORDE1, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 13:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO3, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 14:
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_INTRO3, Garrosh);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 15:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_HORDE2, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 16:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO4, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 17:
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_INTRO4, Garrosh);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 18:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO5, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 3000;
                    break;
                case 19:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_HORDE3, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 20:
                    if(Creature* Garrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
                        DoScriptText(SAY_GARROSH_INTRO5, Garrosh);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 3000;
                    break;
                case 21:
                    if(Creature* Thrall = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_THRALL)))
                        DoScriptText(SAY_THRALL_INTRO6, Thrall);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 22:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_HORDE4, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                    // start trial
                case 23:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_TRIAL, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;
                case 24:
                    if(Creature* Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                        DoScriptText(SAY_TIRION_TWINS_INTRO2, Tirion);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 3000;
                    break;
                case 25:
                    m_bIsIntro = false;
                    isIntroDone = true;
                    m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if(m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_INTRO,DONE);
                    }
                    m_uiSpeech_Timer = 1000;
                    break;
                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }

        if (!m_pInstance) 
            return;

        // Stages
        switch (m_pInstance->GetData(TYPE_STAGE)) 
        {
            // idle
        case 0:
            break; 

            // beasts -> enrage timer
        case 1: 
            if(m_uiBeastsBerserkTimer < uiDiff && !m_bHasBerserk)
            {
                if(Creature* pGormok = GetClosestCreatureWithEntry(m_creature, NPC_GORMOK, 200.0f))
                {
                    pGormok->CastStop();
                    pGormok->CastSpell(pGormok, SPELL_BERSERK, false);
                }
                if(Creature* pAcidmaw = GetClosestCreatureWithEntry(m_creature, NPC_ACIDMAW, 200.0f))
                {
                    pAcidmaw->CastStop();
                    pAcidmaw->CastSpell(pAcidmaw, SPELL_BERSERK, false);
                }
                if(Creature* pDreadscale = GetClosestCreatureWithEntry(m_creature, NPC_DREADSCALE, 200.0f))
                {
                    pDreadscale->CastStop();
                    pDreadscale->CastSpell(pDreadscale, SPELL_BERSERK, false);
                }
                if(Creature* pIcehowl = GetClosestCreatureWithEntry(m_creature, NPC_ICEHOWL, 200.0f))
                {
                    pIcehowl->CastStop();
                    pIcehowl->CastSpell(pIcehowl, SPELL_BERSERK, false);
                }
                m_bHasBerserk = true;
            }
            else
                m_uiBeastsBerserkTimer -= uiDiff;
            break;

            // Jaraxxus -> code handeled in boss script
        case 2: 
            break;

            // Crusaders -> alliance
        case 3: 
		case 4:
            {
				Creature* pTemp1 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_1));
                Creature* pTemp2 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_2));
                Creature* pTemp3 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_3));
                Creature* pTemp4 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_4));
                Creature* pTemp5 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_5));
                Creature* pTemp6 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_6));

                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    Creature* pTemp7 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_7));
                    Creature* pTemp8 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_8));
                    Creature* pTemp9 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_9));
                    Creature* pTemp10 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_CRUSADER_10));
                    if (pTemp7 && pTemp8 && pTemp9 && pTemp10) 
                    {
                        if (!pTemp7->isAlive() && !pTemp8->isAlive() && !pTemp9->isAlive() && !pTemp10->isAlive())
                            flag25 = true;
                        else 
                            flag25 = false;

                        if (!pTemp7->isAlive() || !pTemp8->isAlive() || !pTemp9->isAlive() || !pTemp10->isAlive())
                            m_bIsAchiev = true;
                        // if neither of them is dead -> reset counter (in case of wipe)
                        else
                        {
                            m_bIsAchiev = false;
                            m_uiResilienceAchievTimer = 0;
                        }
                    }
                } 
                else flag25 = true;

                if (pTemp1 && pTemp2 && pTemp3 && pTemp4 && pTemp5 && pTemp6) 
                {
                    if (!pTemp1->isAlive() && !pTemp2->isAlive() && !pTemp3->isAlive() && !pTemp4->isAlive() && !pTemp5->isAlive() && !pTemp6->isAlive() && flag25) 
                    {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_FACTION_CHAMPIONS,DONE);
                        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        m_creature->SetVisibility(VISIBILITY_ON);

						if(m_pInstance->GetData(TYPE_STAGE) == 3)
						{
							if(Creature *pGarrosh = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GARROSH)))
								DoScriptText(SAY_VICTORY_HORDE, pGarrosh);
						}
						else if(m_pInstance->GetData(TYPE_STAGE) == 4)
						{
							if(Creature *pVarian = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_VARIAN)))
								DoScriptText(SAY_VICTORY_ALY, pVarian);
						}

						if(Creature *pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
							DoScriptText(SAY_OUTRO_TIRION, pTirion);

                        if(m_uiResilienceAchievTimer <= 60000)
                        {
                            if(m_pInstance)
                            {
                                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                    m_pInstance->DoCompleteAchievement(ACHIEV_RESILIENCE_WILL_FIX);
                                else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                    m_pInstance->DoCompleteAchievement(ACHIEV_RESILIENCE_WILL_FIX_H);
                            }
                        }
                        m_bIsAchiev = false;
                    }

                    if (!pTemp1->isAlive() || !pTemp2->isAlive() || !pTemp3->isAlive() || !pTemp4->isAlive() || !pTemp5->isAlive() || !pTemp6->isAlive())
                        m_bIsAchiev = true;
                    // if neither of them is dead -> reset counter (in case of wipe)
                    else
                    {
                        m_bIsAchiev = false;
                        m_uiResilienceAchievTimer = 0;
                    }
                };
                break;
            };

            // twin valkyrs -> code handeled in boss script
        case 5: 
            break;

            // anub'arak -> code handeled in boss script
        case 6: 
            break;
        }
    }
};

bool GossipHello_npc_crusader_anouncer(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!m_pInstance) return false;

    if(!pPlayer->getAttackers().empty()) return true;

    for(uint8 i = 0; i < NUM_MESSAGES; i++) 
    {
        if (!TocGossipMessage[i].state && (m_pInstance->GetData(TocGossipMessage[i].encounter) == NOT_STARTED || m_pInstance->GetData(TocGossipMessage[i].encounter) == IN_PROGRESS || m_pInstance->GetData(TocGossipMessage[i].encounter) == FAIL)) 
        {
			if (TocGossipMessage[i].encounter == TYPE_NORTHREND_BEASTS)
			{
				pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START_EVENT1, GOSSIP_SENDER_MAIN,TocGossipMessage[i].id);
				break;
			}

            if (TocGossipMessage[i].encounter == TYPE_FACTION_CHAMPIONS)
            {
                // check if aly
                if(pPlayer->GetTeam() == ALLIANCE)
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[3].name, GOSSIP_SENDER_MAIN,TocGossipMessage[3].id);
                    break;
                }
                // check if horde
                if(pPlayer->GetTeam() == HORDE)
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[2].name, GOSSIP_SENDER_MAIN,TocGossipMessage[2].id);
                    break;
                }
            }

            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[i].name, GOSSIP_SENDER_MAIN,TocGossipMessage[i].id);
            break;

        }
        if (TocGossipMessage[i].state && m_pInstance->GetData(TocGossipMessage[i].encounter) == DONE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[i].name, GOSSIP_SENDER_MAIN,TocGossipMessage[i].id);
    };

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_crusader_anouncer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    pPlayer->CLOSE_GOSSIP_MENU();

    switch(uiAction) 
    {
    case GOSSIP_ACTION_INFO_DEF+1: 
        {
            if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == NOT_STARTED || m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == FAIL) 
            {
                ((npc_crusader_anouncerAI*)pCreature->AI())->SetBeastsBerserk();
                m_pInstance->SetData(TYPE_STAGE,1);
                m_pInstance->SetData(TYPE_NORTHREND_BEASTS,IN_PROGRESS);
            }
            pCreature->SetVisibility(VISIBILITY_OFF);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if (Creature* pGromok = pCreature->SummonCreature(NPC_GORMOK, SpawnLoc[28].x, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
            {
                pGromok->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pGromok->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x, SpawnLoc[2].y - 15, SpawnLoc[2].z);
            }

            if(Creature *Tirion = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                DoScriptText(SAY_SUMMON_GORMOK, Tirion);
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());

            break;
        };

    case GOSSIP_ACTION_INFO_DEF+2: 
        {
            m_pInstance->SetData(TYPE_STAGE,2);
            pCreature->SetVisibility(VISIBILITY_OFF);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            if(Creature *pTirion = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                ((npc_tirionAI*)pTirion->AI())->SetJaruIntro();

            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+3: 
        {
            m_pInstance->SetData(TYPE_FACTION_CHAMPIONS,IN_PROGRESS);
            pCreature->SetVisibility(VISIBILITY_OFF);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if(Creature *pTirion = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                ((npc_tirionAI*)pTirion->AI())->SetChampIntro(1);
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+4: 
        {
            m_pInstance->SetData(TYPE_FACTION_CHAMPIONS,IN_PROGRESS);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->SetVisibility(VISIBILITY_OFF);

            if(Creature *pTirion = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                ((npc_tirionAI*)pTirion->AI())->SetChampIntro(2);
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+5: 
        {
            m_pInstance->SetData(TYPE_STAGE,5);
            m_pInstance->SetData(TYPE_TWIN_VALKYR,IN_PROGRESS);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(Creature *Tirion = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                DoScriptText(SAY_TIRION_TWINS_INTRO1, Tirion);
            if (Creature* pFjola = pCreature->SummonCreature(NPC_FJOLA, SpawnLoc[28].x + 5, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
            {
                m_pInstance->SetData64(DATA_FJOLA, pFjola->GetGUID());
                pFjola->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pFjola->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x + 10, SpawnLoc[2].y -15, SpawnLoc[2].z);
            }
            if (Creature* pEydis = pCreature->SummonCreature(NPC_EYDIS, SpawnLoc[28].x - 5, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME)) 
            {
                m_pInstance->SetData64(DATA_EYDIS, pEydis->GetGUID());
                pEydis->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pEydis->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x - 10, SpawnLoc[2].y -15, SpawnLoc[2].z);
            }
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            // summon essences
            if(!GetClosestCreatureWithEntry(pCreature, NPC_DARK_ESSENCE, 180.0f) || !GetClosestCreatureWithEntry(pCreature, NPC_LIGHT_ESSENCE, 180.0f))
            {
                for(uint8 i = 22; i < 24; i++)
                    pCreature->SummonCreature(NPC_DARK_ESSENCE, SpawnLoc[i].x, SpawnLoc[i].y, SpawnLoc[i].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 0);
                for(uint8 i = 24; i < 26; i++)
                    pCreature->SummonCreature(NPC_LIGHT_ESSENCE, SpawnLoc[i].x, SpawnLoc[i].y, SpawnLoc[i].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 0);
            }

            break;
        };

    case GOSSIP_ACTION_INFO_DEF+6: 
        {
            m_pInstance->SetData(TYPE_STAGE,6);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pCreature->SetVisibility(VISIBILITY_OFF);
            if(Creature *pTemp = pCreature->SummonCreature(NPC_LICH_KING, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
                pTemp->SetVisibility(VISIBILITY_OFF);
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+8:
        {
            m_pInstance->SetData(TYPE_STAGE,0);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            break;
        };
    };

    return true;
}

CreatureAI* GetAI_npc_crusader_anouncer(Creature* pCreature)
{
    return new npc_crusader_anouncerAI(pCreature);
}

struct MANGOS_DLL_DECL npc_lich_kingAI : public ScriptedAI
{
    npc_lich_kingAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 IntroTimer;
    uint32 IntroStep;

    void Reset()
    {
        IntroTimer          = 10000;
        IntroStep           = 1;
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        switch(IntroStep)
        {
        case 1:
            if(Creature *Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                DoScriptText(SAY_TIRION_ANUB_INTRO1, Tirion);
            ++IntroStep;
            IntroTimer = 18000;
            break;
        case 3:
            m_creature->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->SetVisibility(VISIBILITY_ON);
            DoScriptText(SAY_LICHKING_INTRO2, m_creature);
            DoScriptText(SAY_LICHKING_INTRO3, m_creature);
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
            ++IntroStep;
            IntroTimer = 5000;
            break;
        case 5:
            if(Creature *Tirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_TIRION)))
                DoScriptText(SAY_TIRION_ANUB_INTRO4, Tirion);
            ++IntroStep;
            IntroTimer = 8000;
            break;
        case 7:
            DoScriptText(SAY_LICHKING_INTRO5, m_creature);
            DoScriptText(SAY_LICHKING_INTRO6, m_creature);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
            ++IntroStep;
            IntroTimer = 15000;
            break;
        case 9:
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
            ++IntroStep;
            IntroTimer = 5000;
            break;
        case 11:
            DoScriptText(SAY_LICHKING_INTRO8, m_creature);
            if(GameObject* pFloor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FLOOR)))  
            {
                //pFloor->Delete();
                pFloor->SetUInt32Value(GAMEOBJECT_DISPLAYID,9060);
                pFloor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
                pFloor->SetUInt32Value(GAMEOBJECT_BYTES_1,8449);
            }
            m_pInstance->SetData(TYPE_STAGE,6);
            ++IntroStep;
            IntroTimer = 1000;
            break;
        case 13:
            DoScriptText(SAY_LICHKING_INTRO7, m_creature);
            ++IntroStep;
            IntroTimer = 9000;
            break;
        case 15:
            m_creature->SetVisibility(VISIBILITY_OFF);
            ++IntroStep;
            IntroTimer = 9000;
            break;
        }

        if (IntroTimer <= uiDiff)
        {
            ++IntroStep;
            IntroTimer = 330000;
        } IntroTimer -= uiDiff;
    }

};

CreatureAI* GetAI_npc_lich_king(Creature* pCreature)
{
    return new npc_lich_kingAI (pCreature);
}

void AddSC_trial_of_the_crusader()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_crusader_anouncer";
    newscript->GetAI = &GetAI_npc_crusader_anouncer;
    newscript->pGossipHello = &GossipHello_npc_crusader_anouncer;
    newscript->pGossipSelect = &GossipSelect_npc_crusader_anouncer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lich_king";
    newscript->GetAI = &GetAI_npc_lich_king;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tirion";
    newscript->GetAI = &GetAI_npc_tirion;
    newscript->RegisterSelf();
}
