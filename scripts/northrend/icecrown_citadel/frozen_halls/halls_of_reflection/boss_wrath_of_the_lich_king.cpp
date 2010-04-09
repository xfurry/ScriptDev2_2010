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
SDName: boss_lich_king
SD%Complete: 0%
SDComment:
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"
#include "escort_ai.h"
#include "follower_ai.h"

enum
{
    // yells
    SAY_LICH_KING_INTRO_ALY         = -1610160,
    SAY_LICH_KING_INTRO_HORDE       = -1610161,
    SAY_LICH_KING_FIRST_WALL        = -1610162,
    SAY_LICH_KING_START             = -1610163,
    SAY_LICH_KING_SECOND_WALL       = -1610164,
    SAY_LICH_KING_THIRD_WALL        = -1610165,
    SAY_LICH_KING_FINAL_WALL        = -1610166,
    SAY_LICH_KING_OUTRO             = -1610167,
    SAY_LICH_KING_SUMMON1           = -1610168,
    SAY_LICH_KING_SUMMON2           = -1610169,
    SAY_LICH_KING_SLAY1             = -1610170,
    SAY_LICH_KING_SLAY2             = -1610171,
    SAY_LICH_KING_WIPE              = -1610172,

    // jaina
    SAY_JAINA_INTRO                 = -1610173,
    SAY_JAINA_FIRST_WALL            = -1610174, 
    SAY_JAINA_HELP                  = -1610175,
    SAY_JAINA_SECOND_WALL           = -1610176, 
    SAY_JAINA_THIRD_WALL            = -1610177,
    SAY_JAINA_FINAL_WALL            = -1610178,
    SAY_JAINA_OUTRO1                = -1610179,
    SAY_JAINA_OUTRO2_FINAL_STAND    = -1610181,
    SAY_CAPTAIN_ALY_OUTRO3          = -1610182,
    SAY_CAPTAIN_ALY_OUTRO4          = -1610183,
    SAY_JAINA_OUTRO5                = -1610184,
    SAY_JAINA_OUTRO6                = -1610185,

    // sylvanas 
    SAY_SYLVANAS_INTRO              = -1610186,
    SAY_SYLVANAS_FIRST_WALL         = -1610187,
    SAY_SYLVANAS_HELP               = -1610188,
    SAY_SYLVANAS_SECOND_WALL        = -1610189,
    SAY_SYLVANAS_THIRD_WALL         = -1610190,
    SAY_SYLVANAS_FINAL_WALL         = -1610191,
    SAY_SYLVANAS_OUTRO1_1           = -1610192,
    SAY_SYLVANAS_OUTRO1_2           = -1610193,
    SAY_SYLVANAS_OUTRO2_FINAL_STAND = -1610194,
    SAY_CAPTAIN_HORDE_OUTRO3        = -1610195,
    SAY_CAPTAIN_HORDE_OUTRO4        = -1610196,
    SAY_SYLVANAS_OUTRO5             = -1610197,


    // Jaina
    SPELL_ICE_PRISON            = 69708,

    // Sylvanas
    SPELL_DARK_BINDING          = 70194,

    // spells above aren't working. Using the following:
    SPELL_STUN				    = 3618,

    // Lich King
    SPELL_REMORSLESS_WINTER     = 69780,
    SPELL_FURY_OF_FROSTMOURNE   = 70063,
    SPELL_SOUL_REAPER           = 69409,
    SPELL_SOUL_REAPER_H         = 73797,
    SPELL_PAIN_AND_SUFFERING    = 74115,
    SPELL_HARVEST_SOUL          = 70070,

    EQUIP_ID_FROSTMOURNE        = 36942,

    ACHIEV_HOR_H                = 4521,
    ACHIEV_HOR                  = 4518,   
};      

static Locations IceWall[]=
{
    {5542.259f, 2086.836f, 731.044f, 1.071f},
    {5500.020f, 1983.946f, 736.266f, 0.882f},
    {5425.987f, 1870.023f, 753.004f, 1.028f},
    {5320.531f, 1753.154f, 771.022f, 0.823f},
};

static Locations RisenWitchDoctor[]=
{
    (5554.8,  2107.9,  731.041),
    (5510.47, 1995.38, 735.272),
    (5504.82, 1998.39, 735.069),
    (5445.18, 1892.89, 749.236),
    (5442.53, 1894.91, 748.658),
    (5374.28, 1800.98, 761.257),
    (5371.46, 1804.6,  760.557),
    (5339.96, 1770.48, 766.827),
    (5333.19, 1770.41, 767.573),
    (5338.42, 1764.32, 768.209),

};

static Locations RagingGhoul[]=
{
    (5554.54, 2110.54, 731.016), 
    (5556.99, 2108.53, 731.045),
    (5555.44, 2105.85, 731.099),
    (5552.51, 2107.56, 731.103),
    (5509.17, 2002.13, 734.589),
    (5510.64, 2001.35, 734.656),
    (5513.12, 1999.87, 734.786),
    (5506.69, 2003.8,  734.494),
    (5446.65, 1898.22, 748.076),
    (5449.03, 1898.23, 748.158),
    (5449.35, 1896.17, 748.632),
    (5445.42, 1900.73, 747.55),
    (5443.28, 1900.52, 747.795),
    (5375.13, 1805.89, 760.429),
    (5375.47, 1803.07, 760.944),
    (5372.76, 1806.39, 760.398),
    (5373.04, 1808.72, 760.215),
    (5377.59, 1802.5,  761.426),
};

static Locations LumberingAbomination[]=
{
    (5509.37, 1996.32, 735.185),
    (5443.51, 1897.61, 748.146),
    (5446.97, 1895.47, 748.713),
    (5372.51, 1799.01, 761.455),
    (5369.58, 1802.71, 760.726),
    (5336.56, 1768.01, 767.407),
    (5340.65, 1765.74, 767.966),
    (5335.91, 1772.12, 767.011),
};


#define GOSSIP_ITEM "Let's get out of here!"

struct MANGOS_DLL_DECL boss_lich_king_hor_endAI: public FollowerAI
{
    boss_lich_king_hor_endAI(Creature *pCreature) : FollowerAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetEquipmentSlots(false, EQUIP_ID_FROSTMOURNE, -1, -1);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;

    bool m_bHasTaunted;
    bool m_bHasWiped;
    bool m_bIsEventInProgress;

    uint32 Check_Timer;
    uint32 TeamInInstance;

    Creature *pJaina;
    Creature *pSylvanas;

    void Reset()
    {
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;

        m_bHasTaunted       = false;
        m_bHasWiped         = false;
        m_bIsEventInProgress = false;

        Check_Timer         = 10000;
        pJaina              = NULL;
        pSylvanas           = NULL;
        TeamInInstance = GetFaction();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_ESCAPE, NOT_STARTED);

        // Jaina & Sylvanas
        if(pJaina = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_JAINA))))
        {
            if(!pJaina->isAlive())
                pJaina->Respawn();
        }
        if(pSylvanas = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SYLVANAS))))
        {
            if(!pSylvanas->isAlive())
                pSylvanas->Respawn();
        }
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (!m_bHasTaunted && pWho->isInAccessablePlaceFor(m_creature) && !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40) && m_creature->IsWithinLOSInMap(pWho) && m_pInstance->GetData(TYPE_MARWYN) == DONE)
        {
            m_bIsIntro = true;
            m_bHasTaunted = true;

            m_uiIntro_Phase     = 0;
            m_uiSpeech_Timer    = 7000;

            if(TeamInInstance == ALLIANCE)
            {
                pJaina = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_JAINA)));
                pJaina->setFaction(1);
                pJaina->Attack(m_creature, true);
                m_creature->Attack(pJaina, true);
                m_creature->AddThreat(pJaina, 100.0f);
            }

            if(TeamInInstance == HORDE)
            {
                pSylvanas = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SYLVANAS)));
                pSylvanas->setFaction(2);
                pSylvanas->Attack(m_creature, true);
                m_creature->Attack(pSylvanas, true);
                m_creature->AddThreat(pSylvanas, 100.0f);
            }
        }  
    }

    void StartChase()
    {
        m_bIsEventInProgress = true;
        m_creature->RemoveAurasDueToSpell(SPELL_STUN);
        if(TeamInInstance == ALLIANCE)
            m_creature->GetMotionMaster()->MoveChase(pJaina);
        if(TeamInInstance == HORDE)
            m_creature->GetMotionMaster()->MoveChase(pSylvanas);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_ESCAPE, IN_PROGRESS);
    }

    void CompleteQuest()
    {
        Map *map = m_creature->GetMap();
        Map::PlayerList const& players = map->GetPlayers();
        if (!players.isEmpty() && map->IsDungeon())
        {
            for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if (Player* pPlayer = itr->getSource())
                {
                    if(TeamInInstance == ALLIANCE)
                        pPlayer->KilledMonsterCredit(NPC_JAINA_END, m_creature->GetGUID());
                    if(TeamInInstance == HORDE)
                        pPlayer->KilledMonsterCredit(NPC_SYLVANAS_END, m_creature->GetGUID());
                }
            }
        }
    }

    void AttackStart(Unit* pWho)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_ESCAPE) == IN_PROGRESS)
            return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        FollowerAI::UpdateAI(uiDiff);

        if (m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    if(TeamInInstance == ALLIANCE)
                        DoScriptText(SAY_LICH_KING_INTRO_ALY, m_creature);
                    if(TeamInInstance == HORDE)
                        DoScriptText(SAY_LICH_KING_INTRO_HORDE, m_creature);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;
                case 1:
                    if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_SHADOW_DOOR_EXIT)))
                        m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                    if(TeamInInstance == ALLIANCE)
                    {
                        DoScriptText(SAY_JAINA_INTRO, pJaina);
                        pJaina->CastSpell(m_creature, SPELL_ICE_PRISON, false);
                        pJaina->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pJaina->GetMotionMaster()->MovePoint(0, 5577.909f, 2235.264f, 733.012f);
                        pJaina->AttackStop();
                        pJaina->DeleteThreatList();
                    }
                    if(TeamInInstance == HORDE)
                    {
                        DoScriptText(SAY_SYLVANAS_INTRO, pSylvanas);
                        pSylvanas->CastSpell(m_creature, SPELL_DARK_BINDING, false);
                        pSylvanas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pSylvanas->GetMotionMaster()->MovePoint(0, 5577.909f, 2235.264f, 733.012f);
                        pSylvanas->AttackStop();
                        pSylvanas->DeleteThreatList();
                    }
                    DoCast(m_creature, SPELL_STUN);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 2:
                    /*if(TeamInInstance == ALLIANCE)
                        pJaina->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    if(TeamInInstance == HORDE)
                        pSylvanas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);*/ // Remove comment after event is implemented!
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); // Remove this after event is implemented!
                    m_creature->AttackStop();
                    m_creature->DeleteThreatList();
                    CompleteQuest();
                    m_bIsIntro = false;
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }

        if(!m_bIsIntro && m_bIsEventInProgress)
        {
            //Return since we have no target
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // wipe check
            if (Check_Timer < uiDiff && !m_bHasWiped)
            {
                if(TeamInInstance == ALLIANCE)
                {
                    if (m_creature->IsWithinDistInMap(pJaina, 5))
                    {
                        m_creature->DealDamage(pJaina, pJaina->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        DoCast(m_creature, SPELL_FURY_OF_FROSTMOURNE);
                        Reset();
                        m_bHasWiped = true;
                    }
                }
                if(TeamInInstance == HORDE)
                {
                    if (m_creature->IsWithinDistInMap(pSylvanas, 10))
                    {
                        DoCast(m_creature, SPELL_FURY_OF_FROSTMOURNE);
                        m_creature->DealDamage(pSylvanas, pSylvanas->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        Reset();
                        m_bHasWiped = true;
                    }
                }
                Check_Timer = 500;
            }else Check_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_hor_endAI: public npc_escortAI
{
    npc_sylvanas_jaina_hor_endAI(Creature *pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiOutro_Phase;
    bool m_bIsOutro;
    uint32 creatureEntry;

    Creature *pLichKing;
    Creature *pCaptain;
    uint32 TeamInInstance;
    uint32 m_uiMinionTimer;
    uint32 m_bMinionCheck;

    void Reset()
    {
        pLichKing = NULL;
        pCaptain = NULL;
        m_uiSpeech_Timer    = 1000;
        m_uiOutro_Phase     = 0;
        m_bIsOutro          = false;
        creatureEntry = m_creature->GetEntry();
        TeamInInstance = GetFaction();

        m_bMinionCheck = false;
        m_uiMinionTimer = 1000;
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void WaypointReached(uint32 uiWP)
    {
        switch (uiWP)
        {
        case 1:
            m_creature->SummonGameobject(GO_ICE_WALL, IceWall[0].x, IceWall[0].y, IceWall[0].z, IceWall[0].o, 0);
            if(pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LICH_KING))))
            {
                DoScriptText(SAY_LICH_KING_FIRST_WALL, pLichKing);
                pLichKing->GetMotionMaster()->MoveChase(m_creature);
            }
            break;
        case 2:
            SetEscortPaused(true);
            m_bMinionCheck = true;
            m_uiMinionTimer = 1000;
            switch (creatureEntry)
            {
            case NPC_JAINA_END:
                DoScriptText(SAY_JAINA_FIRST_WALL, m_creature);
                break;
            case NPC_SYLVANAS_END:
                DoScriptText(SAY_SYLVANAS_FIRST_WALL, m_creature);
                break;
            }
            break;
        case 3:
            m_creature->SummonGameobject(GO_ICE_WALL, IceWall[1].x, IceWall[1].y, IceWall[1].z, IceWall[1].o, 0);
            if(pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LICH_KING))))
            {
                DoScriptText(SAY_LICH_KING_SECOND_WALL, pLichKing);
                pLichKing->GetMotionMaster()->MoveChase(m_creature);
            }
            break;
        case 4:
            SetEscortPaused(true);
            m_bMinionCheck = true;
            m_uiMinionTimer = 1000;
            switch (creatureEntry)
            {
            case NPC_JAINA_END:
                DoScriptText(SAY_JAINA_SECOND_WALL, m_creature);
                break;
            case NPC_SYLVANAS_END:
                DoScriptText(SAY_SYLVANAS_SECOND_WALL, m_creature);
                break;
            }
            break;
        case 5:
            m_creature->SummonGameobject(GO_ICE_WALL, IceWall[2].x, IceWall[2].y, IceWall[2].z, IceWall[2].o, 0);
            if(pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LICH_KING))))
            {
                DoScriptText(SAY_LICH_KING_THIRD_WALL, pLichKing);
                pLichKing->GetMotionMaster()->MoveChase(m_creature);
            }
            break;
        case 6:
            SetEscortPaused(true);
            m_bMinionCheck = true;
            m_uiMinionTimer = 1000;
            switch (creatureEntry)
            {
            case NPC_JAINA_END:
                DoScriptText(SAY_JAINA_THIRD_WALL, m_creature);
                break;
            case NPC_SYLVANAS_END:
                DoScriptText(SAY_SYLVANAS_THIRD_WALL, m_creature);
                break;
            }
            break;
        case 7:
            m_creature->SummonGameobject(GO_ICE_WALL, IceWall[3].x, IceWall[3].y, IceWall[3].z, IceWall[3].o, 0);
            if(pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LICH_KING))))
            {
                DoScriptText(SAY_LICH_KING_FINAL_WALL, pLichKing);
                pLichKing->GetMotionMaster()->MoveChase(m_creature);
            }
            break;
        case 8:
            SetEscortPaused(true);
            m_bMinionCheck = true;
            m_uiMinionTimer = 1000;
            switch (creatureEntry)
            {
            case NPC_JAINA_END:
                DoScriptText(SAY_JAINA_FINAL_WALL, m_creature);
                break;
            case NPC_SYLVANAS_END:
                DoScriptText(SAY_SYLVANAS_FINAL_WALL, m_creature);
                break;
            }
            break;
        case 9:
            switch (creatureEntry)
            {
            case NPC_JAINA_END:
                DoScriptText(SAY_JAINA_OUTRO2_FINAL_STAND, m_creature);
                break;
            case NPC_SYLVANAS_END:
                DoScriptText(SAY_SYLVANAS_OUTRO2_FINAL_STAND, m_creature);
                break;
            }
            m_bIsOutro = true;
            break;
        }
    }

    void CompleteAchiev()
    {
        // Complete achiev for LichKing
        Map* pMap = m_creature->GetMap();
        AchievementEntry const *Achiev = GetAchievementStore()->LookupEntry(m_bIsRegularMode ? ACHIEV_HOR : ACHIEV_HOR_H);
        if(Achiev && pMap)
        {
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            if (!lPlayers.isEmpty())
            {
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                        pPlayer->CompletedAchievement(Achiev);
                }
            }
        }
    }

    bool AreMinionsDead()
    {
        if(GetClosestCreatureWithEntry(m_creature, MOB_RAGING_GHOUL, 80.0f))
            return false;

        if(GetClosestCreatureWithEntry(m_creature, MOB_RISEN_WHITCH_DOCTOR, 80.0f))
            return false;

        if(GetClosestCreatureWithEntry(m_creature, MOB_LUMBERING_ABONIMATION, 80.0f))
            return false;

        return true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        npc_escortAI::UpdateAI(uiDiff);

        if (m_uiMinionTimer < uiDiff && m_bMinionCheck)
        {
            if(AreMinionsDead())
            {
                m_bMinionCheck = false;
                SetEscortPaused(false);
                if(GameObject *pWall = GetClosestGameObjectWithEntry(m_creature, GO_ICE_WALL, 80.0f))
                {
                    if(m_pInstance)
                        m_pInstance->DoUseDoorOrButton(pWall->GetGUID());
                }
            }
            m_uiMinionTimer = 1000;
        }else m_uiMinionTimer -= uiDiff;
            

        if (m_bIsOutro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiOutro_Phase)
                {
                case 0:
                    if(pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LICH_KING))))
                    {
                        pLichKing->GetMotionMaster()->Clear();
                        pLichKing->GetMotionMaster()->MovePoint(0, 5278.043f, 1697.554f, 785.836f);
                    }
                    m_creature->GetMotionMaster()->MovePoint(0, 5262.469f, 1679.191f, 784.302f);
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 1:
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
                    pLichKing->CastSpell(m_creature, SPELL_HARVEST_SOUL, false);
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 2:
                    // summon gunship
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        pCaptain = m_creature->SummonCreature(30350, 5220.143f, 1670.436f, 784.301f, 0.166f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        DoScriptText(SAY_CAPTAIN_ALY_OUTRO3, pCaptain);
                        break;
                    case NPC_SYLVANAS_END:
                        pCaptain = m_creature->SummonCreature(30752, 5220.143f, 1670.436f, 784.301f, 0.166f, TEMPSUMMON_TIMED_DESPAWN, 60000);
                        DoScriptText(SAY_CAPTAIN_HORDE_OUTRO3, pCaptain);
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 3:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_CAPTAIN_ALY_OUTRO4, pCaptain);
                        break;
                    case NPC_SYLVANAS_END:
                        DoScriptText(SAY_CAPTAIN_HORDE_OUTRO4, pCaptain);
                        break;
                    }
                    m_creature->SummonGameobject(GO_GUNSHIP_STAIRS, 5235.58f, 1649.965f, 784.3f,  0.863f, 0);
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 8000;
                    break;
                case 4:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_JAINA_OUTRO5, m_creature);
                        break;
                    case NPC_SYLVANAS_END:
                        DoScriptText(SAY_SYLVANAS_OUTRO5, m_creature);
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 12000;
                    break;
                case 5:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_JAINA_OUTRO6, m_creature);
                        break;
                    case NPC_SYLVANAS_END:
                        if(m_pInstance)
                            m_pInstance->SetData(TYPE_ESCAPE, DONE);
                        CompleteAchiev();
                        m_bIsOutro = false;
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 6:
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_ESCAPE, DONE);
                    CompleteAchiev();
                    m_bIsOutro = false;
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_npc_slyvanas_jaina_hor_end(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(m_pInstance && m_pInstance->GetData(TYPE_ESCAPE) != DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_slyvanas_jaina_hor_end(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        if (npc_sylvanas_jaina_hor_endAI* pEscortAI = dynamic_cast<npc_sylvanas_jaina_hor_endAI*>(pCreature->AI()))
            pEscortAI->Start(false, true, pPlayer->GetGUID());
        if(Creature *pLichKing = ((Creature*)Unit::GetUnit((*pCreature), m_pInstance->GetData64(DATA_LICH_KING))))
            ((boss_lich_king_hor_endAI*)pLichKing->AI())->StartChase();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

CreatureAI* GetAI_npc_sylvanas_jaina_hor_end(Creature* pCreature)
{
    return new npc_sylvanas_jaina_hor_endAI (pCreature);
}

CreatureAI* GetAI_boss_lich_king_hor_end(Creature* pCreature)
{
    return new boss_lich_king_hor_endAI (pCreature);
}

void AddSC_boss_wrath_of_the_lich_king()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lich_king_hor_end";
    newscript->GetAI = &GetAI_boss_lich_king_hor_end;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_hor_end;
    newscript->Name = "npc_slyvanas_jaina_hor_end";
    newscript->pGossipHello = &GossipHello_npc_slyvanas_jaina_hor_end;
    newscript->pGossipSelect = &GossipSelect_npc_slyvanas_jaina_hor_end;
    newscript->RegisterSelf();
}