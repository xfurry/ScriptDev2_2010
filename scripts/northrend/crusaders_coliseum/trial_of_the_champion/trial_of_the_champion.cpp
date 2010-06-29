/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Trial Of the Champion
SD%Complete: 80%
SDComment: event script
SDCategory: trial_of_the_champion
EndScriptData */

/* ContentData
npc_toc5_announcer
EndContentData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum
{
    // intro yells
    SAY_TIRION_INTRO            = -1614000,
    SAY_ANN_CHALLENGE           = -1614001,
    SAY_ALY_SPECTATORS          = -1614002,
    SAY_HORDE_SPECTATORS        = -1614003,
    SAY_TIRION_BEGIN            = -1614005,

    SAY_EADRIC_INTRO            = -1614010,
    SAY_PALETRESS_INTRO1        = -1614020,
    SAY_PALETRESS_INTRO2        = -1614021,

    SAY_TIRON_INTRO_BK          = -1614030,
    SAY_ANN_INTRO_BK            = -1614031,

    // trash spells
    // lightwielder
    SPELL_BLAZIN_LIGHT          = 67247,
    SPELL_BLAZIN_LIGHT_H        = 67290,
    SPELL_CLEAVE                = 15284,
    SPELL_UNBALANCING_STRIKE    = 67237,    // heroic only
    // monk
    SPELL_DIVINE_SHIELD         = 67251,    // heroic only
    SPELL_FINAL_MEDITATION      = 67255,    // heroic only
    SPELL_FLYRRY_BLOWS          = 67233,
    SPELL_PUMMEL                = 67235,
    // priestess
    SPELL_FOUNTAIN_LIGHT        = 67194,
    SPELL_HOLY_SMITE            = 36176,
    SPELL_HOLY_SMITE_H          = 67289,
    SPELL_MIND_CONTROL          = 67229,    // heroic only
    SPELL_SHADOW_WORD_PAIN      = 34941,
    SPELL_SHADOW_WORD_PAIN_H    = 34942,
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
    {"We are ready for the first challenge!",       GOSSIP_ACTION_INFO_DEF+1,   false,  TYPE_GRAND_CHAMPIONS},          //  summon champs
    {"We are ready to fight the next challenge!",   GOSSIP_ACTION_INFO_DEF+2,   false,  TYPE_ARGENT_CHALLENGE},         //  summon argent challenge
    {"We are ready to fight the next challenge!",   GOSSIP_ACTION_INFO_DEF+3,   false,  TYPE_BLACK_KNIGHT},             //  summon black knight
    {"Arena is closed",                             GOSSIP_ACTION_INFO_DEF+4,   true,   TYPE_BLACK_KNIGHT},             //  event finished
};
enum
{
    NUM_MESSAGES = 8
};

struct Locations
{
    float x, y, z, o;
    uint32 id;
};
 
static Locations SpawnLoc[]=
{
    {733.665771f, 650.031433f, 412.394623f, 4.698702f},  
    {746.864441f, 650.031433f, 412.394623f, 4.698702f},    
    {760.360779f, 650.031433f, 412.394623f, 4.698702f},      
};

const uint32 TrashEntry[3] = {NPC_ARGENT_LIGHTWIELDER, NPC_ARGENT_MONK, NPC_ARGENT_PRIESTESS};

// Argent trash
struct MANGOS_DLL_DECL mob_toc_argent_trashAI: public ScriptedAI
{
    mob_toc_argent_trashAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCreatureEntry;

    // lightwielder
    uint32 m_uiBlazinLightTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiStrikeTimer;
    // monk
    bool m_bHasShield;
    uint32 m_uiFinalMeditationTimer;
    uint32 m_uiFlurryBlowsTimer;
    uint32 m_uiPummelTimer;
    // priestess
    uint32 m_uiLightFountainTimer;
    uint32 m_uiHolySmiteTimer;
    uint32 m_uiMindControlTimer;
    uint32 m_uiShadowWordTimer;

    void Reset()
    {
        m_uiCreatureEntry = m_creature->GetEntry();
        m_creature->SetRespawnDelay(DAY);
        // lightwielder
        m_uiBlazinLightTimer    = 8000;
        m_uiCleaveTimer         = 5000;
        m_uiStrikeTimer         = 11000;
        // monk
        m_bHasShield            = false;
        m_uiFinalMeditationTimer = 9000;
        m_uiFlurryBlowsTimer    = 4000;
        m_uiPummelTimer         = 7000;
        // priestess
        m_uiLightFountainTimer  = 11000;
        m_uiHolySmiteTimer      = 5000;
        m_uiMindControlTimer    = 9000;
        m_uiShadowWordTimer     = 7000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_uiCreatureEntry)
        {
        case NPC_ARGENT_LIGHTWIELDER:
            {
                if(m_uiBlazinLightTimer < uiDiff)
                {
                    if(Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_BLAZIN_LIGHT : SPELL_BLAZIN_LIGHT_H);
                    m_uiBlazinLightTimer = urand(8000, 11000);
                }
                else m_uiBlazinLightTimer -= uiDiff;

                if(m_uiCleaveTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                    m_uiCleaveTimer = urand(5000, 8000);
                }
                else m_uiCleaveTimer -= uiDiff;

                if(m_uiStrikeTimer -= uiDiff && !m_bIsRegularMode)
                {
                    DoCast(m_creature->getVictim(), SPELL_UNBALANCING_STRIKE);
                    m_uiStrikeTimer = urand(11000, 14000);
                }
                else m_uiStrikeTimer -= uiDiff;

                break;
            }
        case NPC_ARGENT_MONK:
            {
                if(m_creature->GetHealthPercent() < 10.0f && !m_bHasShield && !m_bIsRegularMode)
                {
                    DoCast(m_creature, SPELL_DIVINE_SHIELD);
                    m_bHasShield = true;
                }

                if(m_uiFinalMeditationTimer < uiDiff && !m_bIsRegularMode)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_FINAL_MEDITATION);
                    m_uiFinalMeditationTimer = urand(9000, 13000);
                }
                else m_uiFinalMeditationTimer -= uiDiff;

                if(m_uiFlurryBlowsTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_FLYRRY_BLOWS);
                    m_uiFlurryBlowsTimer = urand(4000, 8000);
                }
                else m_uiFlurryBlowsTimer -= uiDiff;

                if(m_uiPummelTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_PUMMEL);
                    m_uiPummelTimer = urand(4000, 8000);
                }
                else m_uiPummelTimer -= uiDiff;

                break;
            }
        case NPC_ARGENT_PRIESTESS:
            {
                if(m_uiLightFountainTimer < uiDiff)
                {
                    DoCast(m_creature, SPELL_FOUNTAIN_LIGHT);
                    m_uiLightFountainTimer = urand(15000, 20000);
                }
                else m_uiLightFountainTimer -= uiDiff;

                if(m_uiHolySmiteTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_HOLY_SMITE : SPELL_HOLY_SMITE_H);
                    m_uiHolySmiteTimer = urand(4000, 8000);
                }
                else m_uiHolySmiteTimer -= uiDiff;

                if(m_uiMindControlTimer < uiDiff && !m_bIsRegularMode)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_MIND_CONTROL);
                    m_uiMindControlTimer = urand(15000, 20000);
                }
                else m_uiMindControlTimer -= uiDiff;

                if(m_uiShadowWordTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_WORD_PAIN : SPELL_SHADOW_WORD_PAIN_H);
                    m_uiShadowWordTimer = urand(6000, 9000);
                }
                else m_uiShadowWordTimer -= uiDiff;

                break;
            }
        }

        DoMeleeAttackIfReady();
    }
};
/*######
## npc_toc5_announcer
######*/
struct MANGOS_DLL_DECL npc_toc5_announcerAI: public ScriptedAI
{
    npc_toc5_announcerAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;

    bool m_bIsIntro;

    uint32 m_uiChamp1Entry;
    uint32 m_uiChamp2Entry;
    uint32 m_uiChamp3Entry;
    uint32 m_uiGrandChampEntry;

    uint64 m_uiChamp1GUID;
    uint64 m_uiChamp2GUID;
    uint64 m_uiChamp3GUID;
    uint64 m_uiGrandChampGUID;

    void Reset()
    {
        m_uiIntroTimer      = 1000;
        m_uiIntroStep       = 0;
        m_bIsIntro          = false;
        m_uiChamp1Entry     = 0;
        m_uiChamp2Entry     = 0;
        m_uiChamp3Entry     = 0;
        m_uiChamp1GUID      = 0;
        m_uiChamp2GUID      = 0;
        m_uiChamp3GUID      = 0;
        m_uiGrandChampGUID  = 0;
        // set the random encounters
        SetChamps();
    }

    void SetChamps()
    {
        std::vector<uint32> ChampionEntries;
        ChampionEntries.reserve(5);

        switch(m_creature->GetEntry())
        {
        case NPC_ARELAS:
            {
                ChampionEntries.push_back(NPC_MOKRA);
                ChampionEntries.push_back(NPC_ERESSEA);
                ChampionEntries.push_back(NPC_RUNOK);
                ChampionEntries.push_back(NPC_ZULTORE);
                ChampionEntries.push_back(NPC_VISCERI);
                break;
            }
        case NPC_JAEREN:
            {
                ChampionEntries.push_back(NPC_JACOB);
                ChampionEntries.push_back(NPC_AMBROSE);
                ChampionEntries.push_back(NPC_COLOSOS);
                ChampionEntries.push_back(NPC_JAELYNE);
                ChampionEntries.push_back(NPC_LANA);
                break;
            }
        }

        if (!ChampionEntries.empty())
        {
            std::random_shuffle(ChampionEntries.begin(), ChampionEntries.end());
            m_uiChamp1Entry = ChampionEntries[0];
            m_uiChamp2Entry = ChampionEntries[1];
            m_uiChamp3Entry = ChampionEntries[2];
        }
    }

    void StartEvent()
    {
        m_bIsIntro          = true;
        m_uiIntroTimer      = 1000;
        m_uiIntroStep       = 0;
    }

    void SummmonTrash()
    {
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = SpawnLoc[i].x + 3*cos(angle*(M_PI/180));
                float homeY = SpawnLoc[i].y + 3*sin(angle*(M_PI/180));
                m_creature->SummonCreature(TrashEntry[j], homeX, homeY, SpawnLoc[i].z, SpawnLoc[i].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        switch(m_pInstance->GetData(TYPE_STAGE))
        {
        case 0:
            // blank
            break;
        case 1:
            {
                // champs
                if (m_bIsIntro)
                {
                    if(m_uiIntroTimer < uiDiff)
                    {
                        switch(m_uiIntroStep)
                        {
                        case 0:
                            // some intro here
                            // vehicles event here
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 1000;
                            break;
                        case 1:
                            if(Creature* pTemp = m_creature->SummonCreature(m_uiChamp1Entry, 738.665771f, 661.031433f, 412.394623f, 4.698702f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                                m_uiChamp1GUID = pTemp->GetGUID();
                            if(Creature* pTemp = m_creature->SummonCreature(m_uiChamp2Entry, 746.864441f, 660.918762f, 411.695465f, 4.698700f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                                m_uiChamp2GUID = pTemp->GetGUID();
                            if(Creature* pTemp = m_creature->SummonCreature(m_uiChamp3Entry, 754.360779f, 660.816162f, 412.395996f, 4.698700f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                                m_uiChamp3GUID = pTemp->GetGUID();
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 2000;
                            break;
                        case 2:
                            m_bIsIntro = false;
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 3000;
                            break;
                        }
                    }else m_uiIntroTimer -= uiDiff;
                }

                // check if champs are dead
                if(!m_bIsIntro)
                {
                    Creature* pChamp1 = m_pInstance->instance->GetCreature(m_uiChamp1GUID);
                    Creature* pChamp2 = m_pInstance->instance->GetCreature(m_uiChamp2GUID);
                    Creature* pChamp3 = m_pInstance->instance->GetCreature(m_uiChamp3GUID);

                    if(pChamp1 && pChamp2 && pChamp3)
                    {
                        if(!pChamp1->isAlive() && !pChamp2->isAlive() && !pChamp3->isAlive())
                        {
                            m_pInstance->SetData(TYPE_STAGE, 0);
                            m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, DONE);
                            m_creature->SetVisibility(VISIBILITY_ON);
                            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);        
                        }
                    }
                }
                break;
            }
        case 2:
            {
                // argent challenge
                if (m_bIsIntro)
                {
                    if(m_uiIntroTimer < uiDiff)
                    {
                        switch(m_uiIntroStep)
                        {
                        case 0:
                            // tirion
                            if(Creature* pTirion = GetClosestCreatureWithEntry(m_creature, NPC_TIRION, 180.0f))
                                DoScriptText(SAY_TIRION_INTRO, pTirion);

                            switch(urand(0, 1))
                            {
                            case 0: m_uiGrandChampEntry = NPC_EADRIC; break;
                            case 1: m_uiGrandChampEntry = NPC_PALETRESS; break;
                            }

                            if(m_pInstance)
                                m_pInstance->SetData(DATA_ARGENT_CHALLERGER, m_uiGrandChampEntry);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 7000;
                            break;
                        case 1:
                            // announcer
                            if(m_uiGrandChampEntry == NPC_PALETRESS)
                                DoScriptText(SAY_ANN_CHALLENGE, m_creature);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 3000;
                            break;
                        case 2:
                            // spectators
                            if(m_uiGrandChampEntry == NPC_PALETRESS)
                            {
                                switch(m_creature->GetEntry())
                                {
                                case NPC_ARELAS:
                                    DoScriptText(SAY_HORDE_SPECTATORS, m_creature);
                                    break;
                                case NPC_JAEREN:
                                    DoScriptText(SAY_ALY_SPECTATORS, m_creature);
                                    break;
                                }
                            }
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            if(Creature* pTemp = m_creature->SummonCreature(m_uiGrandChampEntry, 746.864441f, 660.918762f, 411.695465f, 4.698700f, TEMPSUMMON_CORPSE_DESPAWN, 10000))
                            {
                                m_uiGrandChampGUID = pTemp->GetGUID();
                                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            }
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 1000;
                            break;
                        case 3:
                            // intro 1
                            if(m_uiGrandChampEntry == NPC_PALETRESS)
                            {
                                if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiGrandChampGUID))
                                    DoScriptText(SAY_PALETRESS_INTRO1, pTemp);
                            }
                            else if (m_uiGrandChampEntry == NPC_EADRIC)
                            {
                                if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiGrandChampGUID))
                                    DoScriptText(SAY_EADRIC_INTRO, pTemp);
                            }
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 5000;
                            break;
                        case 4:
                            // intro 2
                            if(m_uiGrandChampEntry == NPC_PALETRESS)
                            {
                                if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiGrandChampGUID))
                                    DoScriptText(SAY_PALETRESS_INTRO2, pTemp);
                                m_uiIntroTimer = 6000;
                            }
                            else if (m_uiGrandChampEntry == NPC_EADRIC)
                                m_uiIntroTimer = 1000;
                            ++m_uiIntroStep;
                            break;
                        case 5:
                            // begin
                            if(Creature* pTirion = GetClosestCreatureWithEntry(m_creature, NPC_TIRION, 180.0f))
                                DoScriptText(SAY_TIRION_BEGIN, pTirion);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 2000;
                            break;
                        case 6:
                            // summon trash
                            SummmonTrash();
                            m_bIsIntro = false;
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 3000;
                            break;
                        }
                    }else m_uiIntroTimer -= uiDiff;
                }

                break;
            }
        case 3:
            {
                // black knight
                if (m_bIsIntro)
                {
                    if(m_uiIntroTimer < uiDiff)
                    {
                        switch(m_uiIntroStep)
                        {
                        case 0:
                            // tirion
                            if(Creature* pTirion = GetClosestCreatureWithEntry(m_creature, NPC_TIRION, 180.0f))
                                DoScriptText(SAY_TIRON_INTRO_BK, pTirion);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 4000;
                            break;
                        case 1:
                            // announcer
                            DoScriptText(SAY_ANN_INTRO_BK, m_creature);
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 2000;
                            break;
                        case 2:
                            // summmon 
                            if(Creature* pTemp = m_creature->SummonCreature(NPC_BLACK_KNIGHT, 746.864441f, 660.918762f, 411.695465f, 4.698700f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                            {
                                pTemp->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY() + 10, m_creature->GetPositionZ());
                                pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                            }
                            m_bIsIntro = false;
                            ++m_uiIntroStep;
                            m_uiIntroTimer = 3000;
                            break;
                        }
                    }else m_uiIntroTimer -= uiDiff;
                }
                break;
            }
        }
    }
};

CreatureAI* GetAI_npc_toc5_announcer(Creature* pCreature)
{
    return new npc_toc5_announcerAI(pCreature);
}

bool GossipHello_npc_toc5_announcer(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!m_pInstance) return false;

    if(!pPlayer->getAttackers().empty()) return true;

    for(uint8 i = 0; i < 4; i++) 
    {
        if (!TocGossipMessage[i].state && (m_pInstance->GetData(TocGossipMessage[i].encounter) == NOT_STARTED || m_pInstance->GetData(TocGossipMessage[i].encounter) == IN_PROGRESS || m_pInstance->GetData(TocGossipMessage[i].encounter) == FAIL)) 
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[i].name, GOSSIP_SENDER_MAIN,TocGossipMessage[i].id);
            break;
        }
        if (TocGossipMessage[i].state && m_pInstance->GetData(TocGossipMessage[i].encounter) == DONE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, TocGossipMessage[i].name, GOSSIP_SENDER_MAIN,TocGossipMessage[i].id);
    };

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_toc5_announcer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    pPlayer->CLOSE_GOSSIP_MENU();

    switch(uiAction) 
    {
    case GOSSIP_ACTION_INFO_DEF+1: 
        {
            m_pInstance->SetData(TYPE_STAGE, 1);
            ((npc_toc5_announcerAI*)pCreature->AI())->StartEvent();
            pCreature->SetVisibility(VISIBILITY_OFF);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            //if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                //m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());

            break;
        };

    case GOSSIP_ACTION_INFO_DEF+2: 
        {
            m_pInstance->SetData(TYPE_STAGE,2);
            ((npc_toc5_announcerAI*)pCreature->AI())->StartEvent();
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            //if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                //m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+3: 
        {
            m_pInstance->SetData(TYPE_STAGE,3);
            ((npc_toc5_announcerAI*)pCreature->AI())->StartEvent();
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            break;
        };

    case GOSSIP_ACTION_INFO_DEF+4:
        {
            m_pInstance->SetData(TYPE_STAGE,0);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            break;
        };
    };

    return true;
}

CreatureAI* GetAI_mob_toc_argent_trash(Creature* pCreature)
{
    return new mob_toc_argent_trashAI(pCreature);
}

void AddSC_trial_of_the_champion()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_toc5_announcer";
    NewScript->GetAI = &GetAI_npc_toc5_announcer;
    NewScript->pGossipHello = &GossipHello_npc_toc5_announcer;
    NewScript->pGossipSelect = &GossipSelect_npc_toc5_announcer;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc_argent_trash";
    NewScript->GetAI = &GetAI_mob_toc_argent_trash;
    NewScript->RegisterSelf();
}
