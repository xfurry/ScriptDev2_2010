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
SDName: Moonglade
SD%Complete: 100
SDComment: Quest support: 30, 272, 5929, 5930, 8736, 10965. Special Flight Paths for Druid class.
SDCategory: Moonglade
EndScriptData */

/* ContentData
npc_bunthen_plainswind
npc_clintar_dw_spirit
npc_great_bear_spirit
npc_silva_filnaveth
npc_keeper_remulos
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "ObjectMgr.h"

/*######
## npc_keeper_remulos
######*/

enum
{
    SPELL_CONJURE_RIFT          =   25813,
    SPELL_HEALING_TOUCH         =   23381,
    SPELL_REGROWTH              =   20665,
    SPELL_REJUVENATION          =   20664,
    SPELL_STARFIRE              =   21668,
    SPELL_THROW_NIGHTMARE       =   25004,
    NPC_ERANIKUS_TYRANT         =   15491,
    NPC_TYRANDE_WHISPERWIND     =   15633,
    NPC_NIGHTMARE_PHANTASM      =   15629,
    NPC_ERANIKUS_REDEEMED       =   15628,
    NPC_ELUNE_PRIESTESS         =   15634,
    QUEST_NIGHTMARE_MANIFESTS   =   8736,

    SAY_REMULOS_1                =   -1493001,
    SAY_REMULOS_2                =   -1493002,
    SAY_REMULOS_3                =   -1493003,
    SAY_REMULOS_4                =   -1493004,
    SAY_REMULOS_5                =   -1493005,
    SAY_REMULOS_6                =   -1493014,
    SAY_REMULOS_7                =   -1493015,
    SAY_REMULOS_8                =   -1493017,
    SAY_REMULOS_9                =   -1493036,

    YELL_REMULOS_1               =   -1493008,
    YELL_REMULOS_2               =   -1493011,

    YELL_ERANIKUS_1             =   -1493007,
    YELL_ERANIKUS_2             =   -1493010,
    YELL_ERANIKUS_3             =   -1493012,
    YELL_ERANIKUS_4             =   -1493016,
    YELL_ERANIKUS_5             =   -1493018,
    YELL_ERANIKUS_6             =   -1493019,
    YELL_ERANIKUS_7             =   -1493020,
    YELL_ERANIKUS_8             =   -1493026,
    YELL_ERANIKUS_9             =   -1493027,
    YELL_ERANIKUS_10            =   -1493028,

    EMOTE_ERANIKUS_1            =   -1493006,
    EMOTE_ERANIKUS_2            =   -1493009,
    EMOTE_ERANIKUS_3            =   -1493013,
    EMOTE_ERANIKUS_4            =   -1493029,

    YELL_TYRANDE1               =   -1493021,
    YELL_TYRANDE2               =   -1493023,
    YELL_TYRANDE3               =   -1493024,
    YELL_TYRANDE4               =   -1493031,
    SAY_TYRANDE1                =   -1493022,
    SAY_TYRANDE2                =   -1493025,
    EMOTE_TYRANDE               =   -1493030,

    SAY_REDEEMED1               =   -1493032,
    SAY_REDEEMED2               =   -1493033,
    SAY_REDEEMED3               =   -1493034,
    SAY_REDEEMED4               =   -1493035 
};
struct SpawnLocations
{
    float x,y,z;
};

static SpawnLocations Adds[]=
{
    {7831,-2597,489},
    {7803,-2575,488},
    {7825,-2538,489},
    {7890,-2556,486},
    {7897,-2582,487}
};

struct MANGOS_DLL_DECL npc_keeper_remulosAI : public npc_escortAI
{
    npc_keeper_remulosAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    bool m_bIsInFight;

    uint32 m_uiHeal_Timer;
    uint32 m_uiStarfire_Timer;
    uint32 m_uiWait_Timer;
    uint32 m_uiEvent_Timer;
    
    uint64 m_uiEranikusGUID;
    uint64 m_uiRedeemedGUID;
    uint64 m_uiTyrandeGUID;
    
    uint8 m_uiSummonCount;
    uint8 m_uiEvent_Count;

    void Reset()
    {
        if (!HasEscortState(STATE_ESCORT_ESCORTING))
        {
            m_bIsInFight = false;
            m_uiSummonCount = 0;
            m_uiEvent_Count = 0;
            m_uiEvent_Timer = 15000;
            m_uiHeal_Timer = 10000;
            m_uiStarfire_Timer = 25000;
            m_uiWait_Timer = 45000;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
        case NPC_ERANIKUS_TYRANT: m_uiEranikusGUID = pSummoned->GetGUID();break;
        case NPC_ERANIKUS_REDEEMED: m_uiRedeemedGUID = pSummoned->GetGUID();break;
        case NPC_TYRANDE_WHISPERWIND: m_uiTyrandeGUID = pSummoned->GetGUID();break;
        case NPC_NIGHTMARE_PHANTASM:
            if (Player* pPlayer = GetPlayerForEscort())
            {
                pSummoned->setFaction(14);
                pSummoned->AI()->AttackStart(pPlayer);
                pSummoned->AddThreat(m_creature, 0.0f);
                ++m_uiSummonCount;
            }
            break;
        }
    }

    void SummonedCreatureDespawn(Creature *pDespawned)
    {
        if (pDespawned->GetEntry() != NPC_NIGHTMARE_PHANTASM)
            return;

        --m_uiSummonCount;
        if (m_uiEvent_Count == 2 && m_uiSummonCount < 1)
            if (Creature *pEranikus = (Creature*)Unit::GetUnit(*m_creature,m_uiEranikusGUID))
                if (Player* pPlayer = GetPlayerForEscort())
                {
                    pEranikus->AI()->AttackStart(pPlayer);
                    pEranikus->SetInCombatWithZone();
                    DoScriptText(YELL_ERANIKUS_6,pEranikus);
                }
    }

    void JustDied(Unit *pKiller)
    {
        if (Player* pPlayer = GetPlayerForEscort())
            pPlayer->FailQuest(QUEST_NIGHTMARE_MANIFESTS);
    }

    void WaypointReached(uint32 uiPointId)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch(uiPointId)
        {
        case 1:DoScriptText(SAY_REMULOS_2,m_creature,pPlayer); break;
        case 10:DoScriptText(SAY_REMULOS_3,m_creature,pPlayer); break;
        case 11:DoScriptText(SAY_REMULOS_4,m_creature,pPlayer); break;
        case 12:DoScriptText(SAY_REMULOS_5,m_creature,pPlayer); break;
        case 13:DoCast(m_creature,SPELL_CONJURE_RIFT);break;
        case 14:
            if (Creature *pEranikus = m_creature->SummonCreature(NPC_ERANIKUS_TYRANT,7873,-2661,460,0.75,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,600000))
            {
                DoScriptText(EMOTE_ERANIKUS_1,pEranikus);
                DoScriptText(YELL_ERANIKUS_1,pEranikus);
            }
            break;
        case 15:DoScriptText(YELL_REMULOS_1,m_creature,pPlayer); break;
        case 16:
            if (Unit *pEranikus = Unit::GetUnit(*m_creature,m_uiEranikusGUID))
            {
                DoScriptText(EMOTE_ERANIKUS_2,pEranikus);
                DoScriptText(YELL_ERANIKUS_2,pEranikus);
            }
            break;
        case 17:DoScriptText(YELL_REMULOS_2,m_creature,pPlayer); break;
        case 18:
            if (Unit *pEranikus = Unit::GetUnit(*m_creature,m_uiEranikusGUID))
            {
                DoScriptText(YELL_ERANIKUS_3,pEranikus);
                DoScriptText(EMOTE_ERANIKUS_3,pEranikus);
                pEranikus->GetMotionMaster()->MovePoint(0,7833,-2614,515);
            }
            break;
        case 19:DoScriptText(SAY_REMULOS_6,m_creature,pPlayer); break;
        case 20:DoScriptText(SAY_REMULOS_7,m_creature,pPlayer); break;
        case 23:
            DoScriptText(SAY_REMULOS_8,m_creature,pPlayer);
            if(Unit *pEranikus = Unit::GetUnit(*m_creature,m_uiEranikusGUID))
                DoScriptText(YELL_ERANIKUS_4,pEranikus);
            for (uint8 i = 0; i < 5; ++i)
                m_creature->SummonCreature(NPC_NIGHTMARE_PHANTASM,Adds[i].x,Adds[i].y,Adds[i].z,0,TEMPSUMMON_CORPSE_DESPAWN,0);
            m_bIsInFight = true;
            SetEscortPaused(true);
            break;
        case 24:
            m_creature->Respawn();
            break;
        }
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        Unit *pEranikus = Unit::GetUnit(*m_creature,m_uiEranikusGUID);
        Unit *pRedeemed = Unit::GetUnit(*m_creature,m_uiRedeemedGUID);
        if (pEranikus||pRedeemed)
        {
            if (m_bIsInFight)
            {
                if (m_uiEvent_Timer < uiDiff)
                {
                    if (pEranikus && pEranikus->GetHealth()*100 / pEranikus->GetMaxHealth() >= 20)
                    {
                        if (m_uiEvent_Count == 0)
                        {
                            for (uint8 i = 0; i < 5; ++i)
                                m_creature->SummonCreature(NPC_NIGHTMARE_PHANTASM,Adds[i].x,Adds[i].y,Adds[i].z,0,TEMPSUMMON_CORPSE_DESPAWN,0);
                            m_uiEvent_Timer = 15000;
                            ++m_uiEvent_Count;
                        }
                        else if (m_uiEvent_Count == 1)
                        {
                            DoScriptText(YELL_ERANIKUS_5,pEranikus);
                            for (uint8 i = 0; i < 5; ++i)
                                m_creature->SummonCreature(NPC_NIGHTMARE_PHANTASM,Adds[i].x,Adds[i].y,Adds[i].z,0,TEMPSUMMON_CORPSE_DESPAWN,0);
                            m_uiEvent_Timer = 5000;
                            ++m_uiEvent_Count;
                        }
                    }
                    else
                    {
                        if (Unit *pTyrande = Unit::GetUnit(*m_creature,m_uiTyrandeGUID))
                        {
                            switch(m_uiEvent_Count)
                            {
                            case 2:DoScriptText(YELL_TYRANDE1,pTyrande);m_uiEvent_Timer = 25000;break;
                            case 3:DoScriptText(SAY_TYRANDE1,pTyrande);m_uiEvent_Timer = 5000;break;
                            case 4:DoScriptText(YELL_TYRANDE2,pTyrande);m_uiEvent_Timer = 5000;break;
                            case 5:DoScriptText(YELL_TYRANDE3,pTyrande);m_uiEvent_Timer = 5000;break;
                            case 6:DoScriptText(SAY_TYRANDE2,pTyrande);m_uiEvent_Timer = 5000;break;
                            case 7:DoScriptText(YELL_ERANIKUS_8,pEranikus);m_uiEvent_Timer = 5000;break;
                            case 8:DoScriptText(YELL_ERANIKUS_9,pEranikus);m_uiEvent_Timer = 5000;break;
                            case 9:DoScriptText(YELL_ERANIKUS_10,pEranikus);m_uiEvent_Timer = 5000;break;
                            case 10:
                                DoScriptText(EMOTE_ERANIKUS_4,pEranikus);
                                DoScriptText(EMOTE_TYRANDE,pTyrande);
                                pTyrande->SetStandState(UNIT_STAND_STATE_KNEEL);
                                m_uiEvent_Timer = 5000;
                                break;
                            case 11:
                                DoScriptText(YELL_TYRANDE4,pTyrande);
                                m_creature->SummonCreature(NPC_ERANIKUS_REDEEMED,pEranikus->GetPositionX(),pEranikus->GetPositionY(),pEranikus->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
                                ((Creature*)pEranikus)->ForcedDespawn();
                                m_uiEvent_Timer = 5000;
                                break;
                            case 12:
                                if(Unit *pRedeemed = Unit::GetUnit(*m_creature,m_uiRedeemedGUID))
                                    DoScriptText(SAY_REDEEMED1,pRedeemed);
                                m_uiEvent_Timer = 15000;
                                break;
                            case 13:
                                if(Unit *pRedeemed = Unit::GetUnit(*m_creature,m_uiRedeemedGUID))
                                    DoScriptText(SAY_REDEEMED2,pRedeemed);
                                m_uiEvent_Timer = 15000;
                                break;
                            case 14:
                                if(Unit *pRedeemed = Unit::GetUnit(*m_creature,m_uiRedeemedGUID))
                                    DoScriptText(SAY_REDEEMED3,pRedeemed);
                                break;
                                m_uiEvent_Timer = 5000;
                            case 15:
                                if(Unit *pRedeemed = Unit::GetUnit(*m_creature,m_uiRedeemedGUID))
                                    DoScriptText(SAY_REDEEMED4,pRedeemed);
                                break;
                                m_uiEvent_Timer = 5000;
                            case 16:
                                DoScriptText(SAY_REMULOS_9,m_creature);
                                SetEscortPaused(false);
                                if (Player* pPlayer = GetPlayerForEscort())
                                    pPlayer->CompleteQuest(QUEST_NIGHTMARE_MANIFESTS);
                                break;
                            }
                        }
                        else
                        {
                            m_creature->SummonCreature(NPC_TYRANDE_WHISPERWIND,7926.0f,-2571.0f,489.64f,3.14f,TEMPSUMMON_TIMED_DESPAWN,90000);
                            for (uint8 j = 0; j < 6;++j)
                                m_creature->SummonCreature(NPC_ELUNE_PRIESTESS,7926.0f + urand(-5,5),-2571.0f + urand(-5,5),489.64f,3.14f,TEMPSUMMON_TIMED_DESPAWN,48000);
                            DoScriptText(YELL_ERANIKUS_7,pEranikus);
                        }
                        ++m_uiEvent_Count;
                    } 
                }
                else
                    m_uiEvent_Timer -= uiDiff;
            }
        }

         if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHeal_Timer < uiDiff)
        {
            if (Unit *pTarget = DoSelectLowestHpFriendly(DEFAULT_VISIBILITY_DISTANCE))
            {
                switch(rand()%3)
                {
                case 0:DoCast(pTarget,SPELL_HEALING_TOUCH);break;
                case 1:DoCast(pTarget,SPELL_REJUVENATION);break;
                case 2:DoCast(pTarget,SPELL_REGROWTH);break;
                }
            }
            m_uiHeal_Timer = 10000;
        }
        else
            m_uiHeal_Timer -= uiDiff;

        if (m_uiStarfire_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_STARFIRE);
            m_uiStarfire_Timer = 20000;
        }
        else
            m_uiStarfire_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_keeper_remulos(Creature* pCreature)
{
    return new npc_keeper_remulosAI(pCreature);
}

bool QuestAccept_npc_keeper_remulos(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_NIGHTMARE_MANIFESTS)
    {
        pCreature->SetHealth(1523420);
        if (npc_keeper_remulosAI* pEscortAI = dynamic_cast<npc_keeper_remulosAI*>(pCreature->AI()))
        {
            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest);
            pEscortAI->SetRun();
            DoScriptText(SAY_REMULOS_1,pCreature,pPlayer);
        }
    }
    return true;
}

/*######
## npc_bunthen_plainswind
######*/

enum
{
    QUEST_SEA_LION_HORDE        = 30,
    QUEST_SEA_LION_ALLY         = 272,
    TAXI_PATH_ID_ALLY           = 315,
    TAXI_PATH_ID_HORDE          = 316
};

#define GOSSIP_ITEM_THUNDER     "I'd like to fly to Thunder Bluff."
#define GOSSIP_ITEM_AQ_END      "Do you know where I can find Half Pendant of Aquatic Endurance?"

bool GossipHello_npc_bunthen_plainswind(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->getClass() != CLASS_DRUID)
        pPlayer->SEND_GOSSIP_MENU(4916, pCreature->GetGUID());
    else if (pPlayer->GetTeam() != HORDE)
    {
        if (pPlayer->GetQuestStatus(QUEST_SEA_LION_ALLY) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AQ_END, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        pPlayer->SEND_GOSSIP_MENU(4917, pCreature->GetGUID());
    }
    else if (pPlayer->getClass() == CLASS_DRUID && pPlayer->GetTeam() == HORDE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_THUNDER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (pPlayer->GetQuestStatus(QUEST_SEA_LION_HORDE) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AQ_END, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        pPlayer->SEND_GOSSIP_MENU(4918, pCreature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_bunthen_plainswind(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->CLOSE_GOSSIP_MENU();

            if (pPlayer->getClass() == CLASS_DRUID && pPlayer->GetTeam() == HORDE)
                pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_HORDE);

            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->SEND_GOSSIP_MENU(5373, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->SEND_GOSSIP_MENU(5376, pCreature->GetGUID());
            break;
    }
    return true;
}

/*####
# npc_clintar_dw_spirit
####*/

enum
{
    SAY_START               = -1000280,
    SAY_AGGRO_1             = -1000281,
    SAY_AGGRO_2             = -1000282,
    SAY_RELIC1              = -1000283,
    SAY_RELIC2              = -1000284,
    SAY_RELIC3              = -1000285,
    SAY_END                 = -1000286,

    QUEST_MERE_DREAM        = 10965,
    SPELL_EMERALD_DREAM     = 39601,
    NPC_CLINTAR_DW_SPIRIT   = 22916,
    NPC_CLINTAR_SPIRIT      = 22901,
    NPC_ASPECT_OF_RAVEN     = 22915,
};

struct MANGOS_DLL_DECL npc_clintar_dw_spiritAI : public npc_escortAI
{
    npc_clintar_dw_spiritAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        //visual details here probably need refinement
        switch(i)
        {
            case 0:
                DoScriptText(SAY_START, m_creature, pPlayer);
                break;
            case 13:
                m_creature->HandleEmote(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 14:
                DoScriptText(SAY_RELIC1, m_creature, pPlayer);
                break;
            case 26:
                m_creature->HandleEmote(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 27:
                DoScriptText(SAY_RELIC2, m_creature, pPlayer);
                break;
            case 31:
                m_creature->SummonCreature(NPC_ASPECT_OF_RAVEN, 7465.321f, -3088.515f, 429.006f, 5.550f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                break;
            case 35:
                m_creature->HandleEmote(EMOTE_STATE_USESTANDING_NOSHEATHE);
                break;
            case 36:
                DoScriptText(SAY_RELIC3, m_creature, pPlayer);
                break;
            case 49:
                DoScriptText(SAY_END, m_creature, pPlayer);
                pPlayer->TalkedToCreature(m_creature->GetEntry(), m_creature->GetGUID());
                break;
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(urand(0, 1) ? SAY_AGGRO_1 : SAY_AGGRO_2, m_creature);
    }

    void Reset()
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        //m_creature are expected to always be spawned, but not visible for player
        //spell casted from quest_template.SrcSpell require this to be this way
        //we handle the triggered spell to get a "hook" to our guy so he can be escorted on quest accept

        if (CreatureInfo const* pTemp = GetCreatureTemplateStore(m_creature->GetEntry()))
            m_creature->SetDisplayId(Creature::ChooseDisplayId(0, pTemp));

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    //called only from EffectDummy
    void DoStart(uint64 uiPlayerGuid)
    {
        //not the best way, maybe check in DummyEffect if this creature are "free" and not in escort.
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            return;

        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        Start(false, uiPlayerGuid);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }
};

CreatureAI* GetAI_npc_clintar_dw_spirit(Creature* pCreature)
{
    return new npc_clintar_dw_spiritAI(pCreature);
}

//we expect this spell to be triggered from spell casted at questAccept
bool EffectDummyCreature_npc_clintar_dw_spirit(Unit *pCaster, uint32 spellId, SpellEffectIndex effIndex, Creature *pCreatureTarget)
{
    //always check spellid and effectindex
    if (spellId == SPELL_EMERALD_DREAM && effIndex == EFFECT_INDEX_0)
    {
        if (pCaster->GetTypeId() != TYPEID_PLAYER || pCaster->HasAura(SPELL_EMERALD_DREAM))
            return true;

        if (pCreatureTarget->GetEntry() != NPC_CLINTAR_DW_SPIRIT)
            return true;

        if (CreatureInfo const* pTemp = GetCreatureTemplateStore(NPC_CLINTAR_SPIRIT))
            pCreatureTarget->SetDisplayId(Creature::ChooseDisplayId(0, pTemp));
        else
            return true;

        //done here, escort can start
        ((npc_clintar_dw_spiritAI*)pCreatureTarget->AI())->DoStart(pCaster->GetGUID());

        //always return true when we are handling this spell and effect
        return true;
    }
    return true;
}

/*######
## npc_great_bear_spirit
######*/

#define GOSSIP_BEAR1 "What do you represent, spirit?"
#define GOSSIP_BEAR2 "I seek to understand the importance of strength of the body."
#define GOSSIP_BEAR3 "I seek to understand the importance of strength of the heart."
#define GOSSIP_BEAR4 "I have heard your words, Great Bear Spirit, and I understand. I now seek your blessings to fully learn the way of the Claw."

bool GossipHello_npc_great_bear_spirit(Player* pPlayer, Creature* pCreature)
{
    //ally or horde quest
    if (pPlayer->GetQuestStatus(5929) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(5930) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BEAR1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(4719, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(4718, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_great_bear_spirit(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BEAR2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(4721, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BEAR3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(4733, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BEAR4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(4734, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->SEND_GOSSIP_MENU(4735, pCreature->GetGUID());
            if (pPlayer->GetQuestStatus(5929)==QUEST_STATUS_INCOMPLETE)
                pPlayer->AreaExploredOrEventHappens(5929);
            if (pPlayer->GetQuestStatus(5930)==QUEST_STATUS_INCOMPLETE)
                pPlayer->AreaExploredOrEventHappens(5930);
            break;
    }
    return true;
}

/*######
## npc_silva_filnaveth
######*/

#define GOSSIP_ITEM_RUTHERAN    "I'd like to fly to Rut'theran Village."
#define GOSSIP_ITEM_AQ_AGI      "Do you know where I can find Half Pendant of Aquatic Agility?"

bool GossipHello_npc_silva_filnaveth(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->getClass() != CLASS_DRUID)
        pPlayer->SEND_GOSSIP_MENU(4913, pCreature->GetGUID());
    else if (pPlayer->GetTeam() != ALLIANCE)
    {
        if (pPlayer->GetQuestStatus(QUEST_SEA_LION_HORDE) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AQ_AGI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        pPlayer->SEND_GOSSIP_MENU(4915, pCreature->GetGUID());
    }
    else if (pPlayer->getClass() == CLASS_DRUID && pPlayer->GetTeam() == ALLIANCE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RUTHERAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (pPlayer->GetQuestStatus(QUEST_SEA_LION_ALLY) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_AQ_AGI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

        pPlayer->SEND_GOSSIP_MENU(4914, pCreature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_silva_filnaveth(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            pPlayer->CLOSE_GOSSIP_MENU();

            if (pPlayer->getClass() == CLASS_DRUID && pPlayer->GetTeam() == ALLIANCE)
                pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_ALLY);

            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->SEND_GOSSIP_MENU(5374, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->SEND_GOSSIP_MENU(5375, pCreature->GetGUID());
            break;
    }
    return true;
}

/*######
##
######*/

void AddSC_moonglade()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_bunthen_plainswind";
    newscript->pGossipHello =  &GossipHello_npc_bunthen_plainswind;
    newscript->pGossipSelect = &GossipSelect_npc_bunthen_plainswind;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_clintar_dw_spirit";
    newscript->GetAI = &GetAI_npc_clintar_dw_spirit;
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_clintar_dw_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_great_bear_spirit";
    newscript->pGossipHello =  &GossipHello_npc_great_bear_spirit;
    newscript->pGossipSelect = &GossipSelect_npc_great_bear_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_silva_filnaveth";
    newscript->pGossipHello =  &GossipHello_npc_silva_filnaveth;
    newscript->pGossipSelect = &GossipSelect_npc_silva_filnaveth;
    newscript->RegisterSelf();

     newscript = new Script;
    newscript->Name = "npc_keeper_remulos";
    newscript->GetAI = &GetAI_npc_keeper_remulos;
    newscript->pQuestAccept = &QuestAccept_npc_keeper_remulos;
    newscript->RegisterSelf();
}
