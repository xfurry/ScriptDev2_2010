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
SDName: boss_scourgelord_tyrannus
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SPELL_FORCEFUL_SMASH        = 69155,
    SPELL_FORCEFUL_SMASH_H      = 69627,
    SPELL_OVERLORDS_BRAND       = 69172,
    SPELL_DARK_MIGHT            = 69167,
    SPELL_DARK_MIGHT_H          = 69629,
    SPELL_HOARFROST             = 69246,
    SPELL_MARK_OF_RIMEFANG      = 69275,
    SPELL_ICY_BLAST             = 69233,
    SPELL_ICY_BLAST_H           = 69646,
    SPELL_ICY_BLAST_SLOW        = 69238,
    SPELL_ICY_BLAST_SLOW_H      = 69628,

    NPC_ICY_BLAST               = 36731,
    SPELL_ICY_BLAST_AURA        = 69238,
    SPELL_ICY_BLAST_AURA_H      = 69628,

    SAY_CHASE1              = -1610081,
    SAY_CHASE2              = -1610082,
    SAY_GAUNTLET            = -1610083,

    SAY_INTRO1              = -1610084,
    SAY_INTRO2_SLAVES       = -1610085,
    SAY_INTRO3              = -1610086,

    SAY_DARK_MIGHT                = -1610089,
    SAY_DARK_MIGHT_EMOTE          = -1610088,
    SAY_MARK_OF_RIMEFANG          = -1610090,
    SAY_AGGRO                     = -1610087,
    SAY_SLAY_1                    = -1610091,
    SAY_SLAY_2                    = -1610092,
    SAY_DEATH                     = -1610093,

    SAY_OUTRO1_SLAVE              = -1610094,
    SAY_OUTRO2_SLAVE              = -1610095,
    SAY_OUTRO3_HORDE              = -1610096,
    SAY_OUTRO3_ALY                = -1610097,
    SAY_OUTRO4_HORDE              = -1610098,
    SAY_OUTRO4_ALY                = -1610099,
    SAY_OUTRO5_ALY                = -1610100,

    NPC_SINDRAGOSA                = 37755,
    SPELL_FROST_BOMB              = 70521,

    EQUIP_ID                      = 51796,
};

#define HOME_X                      1014.51f
#define HOME_Y                      170.423f

const float RimefangSummon[4] = {1013.827f, 169.71f, 628.157f, 5.31f};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {1060.955f, 107.274f, 628.424f},
    {1052.122f, 103.916f, 628.454f},
    {1068.363f, 110.432f, 629.009f},
};

static LocationsXY MoveLoc[]=
{
    {1019.006f, 129.684f, 628.156f}, 
    {1003.889f, 159.652f, 628.159f},
    {1015.389f, 183.650f, 628.156f},
    {1065.827f, 210.836f, 628.156f},
    {1072.659f, 204.432f, 628.156f},
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_pos_endAI: public ScriptedAI
{
    npc_sylvanas_jaina_pos_endAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiOutro_Phase;
    bool m_bIsOutro;
    uint32 creatureEntry;

    uint64 m_uiMartinGuid;
    uint64 m_uiGorkunGuid;
    uint64 m_uiTyrannusGuid;
    uint64 m_uiSindragosaGuid;

    std::list<Creature*> lSlavesList;

    void Reset()
    {
        lSlavesList.clear();
        m_uiOutro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsOutro          = true;
        creatureEntry = m_creature->GetEntry();

        m_uiMartinGuid      = 0;
        m_uiGorkunGuid      = 0;
        m_uiTyrannusGuid    = 0;
        m_uiSindragosaGuid  = 0;
    }

    void TeleportPlayers()
    {
        Map* pMap = m_creature->GetMap();
        if(pMap)
        {
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            if (!lPlayers.isEmpty())
            {
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                        pPlayer->TeleportTo(m_creature->GetMapId(), 1065.983f, 94.954f, 630.997f, 2.247f);
                }
            }
        }
    }

    void SummonHordeSlaves()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
        }
    }

    void SummonAlySlaves()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
        }
    }

    void SummonAlyAssist()
    {
        Creature *pElandra = m_creature->SummonCreature(NPC_ELANDRA, SummonLoc[0].x, SummonLoc[0].y, SummonLoc[0].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pElandra)
            pElandra->GetMotionMaster()->MovePoint(0, MoveLoc[3].x, MoveLoc[3].y, MoveLoc[3].z);
        Creature *pKoreln = m_creature->SummonCreature(NPC_KORELN, SummonLoc[1].x, SummonLoc[1].y, SummonLoc[1].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pKoreln)
            pKoreln->GetMotionMaster()->MovePoint(0, MoveLoc[4].x, MoveLoc[4].y, MoveLoc[4].z);
    }

    void SummonHordeAssist()
    {
        Creature *pLoralen = m_creature->SummonCreature(NPC_LORALEN, SummonLoc[0].x, SummonLoc[0].y, SummonLoc[0].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pLoralen)
            pLoralen->GetMotionMaster()->MovePoint(0, MoveLoc[3].x, MoveLoc[3].y, MoveLoc[3].z);
        Creature *pKelira = m_creature->SummonCreature(NPC_KALIRA, SummonLoc[1].x, SummonLoc[1].y, SummonLoc[1].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pKelira)
            pKelira->GetMotionMaster()->MovePoint(0, MoveLoc[4].x, MoveLoc[4].y, MoveLoc[4].z);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsOutro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiOutro_Phase)
                {
                case 0:
                    if(Creature* pTyrannus = GetClosestCreatureWithEntry(m_creature, NPC_RIMEFANG, 100.0f))
                        m_uiTyrannusGuid = pTyrannus->GetGUID();
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        if(Creature* pMartin = GetClosestCreatureWithEntry(m_creature, NPC_MARTIN_VICTUS_END, 100.0f))
                        {
                            DoScriptText(SAY_OUTRO1_SLAVE, pMartin);
                            //SummonAlySlaves();
                            pMartin->GetMotionMaster()->MovePoint(0, 1014.670f, 158.714f, 628.156f);
                            m_uiMartinGuid = pMartin->GetGUID();
                        }
                        break;
                    case NPC_SYLVANAS_END:
                        if(Creature* pGorkun = GetClosestCreatureWithEntry(m_creature, NPC_GORKUN_IRONSKULL_END, 100.0f))
                        {
                            DoScriptText(SAY_OUTRO1_SLAVE, pGorkun);
                            //SummonHordeSlaves();
                            pGorkun->GetMotionMaster()->MovePoint(0, 1014.670f, 158.714f, 628.156f);
                            m_uiGorkunGuid = pGorkun->GetGUID();
                        }
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;
                case 1:
                    if(Creature* pSindragosa = m_creature->SummonCreature(NPC_SINDRAGOSA, 977.224f, 164.056f, 653.216f,  0.064f, TEMPSUMMON_TIMED_DESPAWN, 15000))
                    {
                        pSindragosa->GetMotionMaster()->MoveIdle();
                        pSindragosa->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                        pSindragosa->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                        pSindragosa->GetMotionMaster()->MoveIdle();
                        pSindragosa->GetMap()->CreatureRelocation(pSindragosa, 977.224f, 164.056f, 653.216f,  0.064f);
                        pSindragosa->SendMonsterMove(977.224f, 164.056f, 653.216f, SPLINETYPE_NORMAL, pSindragosa->GetSplineFlags(), 1);
                        m_uiSindragosaGuid = pSindragosa->GetGUID();
                    }
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        if(Creature* pMartin = m_pInstance->instance->GetCreature(m_uiMartinGuid))
                            DoScriptText(SAY_OUTRO2_SLAVE, pMartin);
                        break;
                    case NPC_SYLVANAS_END:
                        if(Creature* pGorkun = m_pInstance->instance->GetCreature(m_uiGorkunGuid))
                            DoScriptText(SAY_OUTRO2_SLAVE, pGorkun);
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 2:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        if(Creature* pMartin = m_pInstance->instance->GetCreature(m_uiMartinGuid))
                        {
                            pMartin->SetHealth(0);
                            pMartin->SetStandFlags(UNIT_STAND_STATE_DEAD);
                            if(Creature* pSindragosa = m_pInstance->instance->GetCreature(m_uiSindragosaGuid))
                                pSindragosa->CastSpell(pMartin, SPELL_FROST_BOMB, false);
                        }
                        DoScriptText(SAY_OUTRO3_ALY, m_creature);
                        break;
                    case NPC_SYLVANAS_END:
                        if(Creature* pGorkun = m_pInstance->instance->GetCreature(m_uiGorkunGuid))
                        {
                            pGorkun->SetHealth(0);
                            pGorkun->SetStandFlags(UNIT_STAND_STATE_DEAD);
                            if(Creature* pSindragosa = m_pInstance->instance->GetCreature(m_uiSindragosaGuid))
                                pSindragosa->CastSpell(pGorkun, SPELL_FROST_BOMB, false);
                        }
                        DoScriptText(SAY_OUTRO3_HORDE, m_creature);
                        break;
                    }
                    TeleportPlayers();
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 3:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_OUTRO4_ALY, m_creature);
                        m_creature->GetMotionMaster()->MovePoint(0, 1068.709f, 208.378f, 628.156f);
                        SummonAlyAssist();
                        m_uiSpeech_Timer = 7000;
                        break;
                    case NPC_SYLVANAS_END:
                        DoScriptText(SAY_OUTRO4_HORDE, m_creature);
                        m_creature->GetMotionMaster()->MovePoint(0, 1068.709f, 208.378f, 628.156f);
                        SummonHordeAssist();
                        m_bIsOutro = false;
                        break;
                    }
                    ++m_uiOutro_Phase;
                    break;
                case 4:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_OUTRO5_ALY, m_creature);
                        break;
                    case NPC_SYLVANAS_END:
                        break;
                    }
                    m_bIsOutro = false;
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL boss_RimefangAI : public ScriptedAI
{
    boss_RimefangAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiHoarfrostTimer;
    uint32 m_uiIcyBlastTimer;
    uint32 m_uiIcyBlastSlowTimer;
    uint64 m_uiMainTargetGUID;

    void Reset()
    {
        m_uiHoarfrostTimer  = 25000;
        m_uiIcyBlastTimer   = 35000;
        m_uiIcyBlastSlowTimer   = 30000;
        m_uiMainTargetGUID  = 0;
        m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF); 

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
    }

    void SetMainTarget(uint64 m_uiTargetGUID)
    {
        m_uiMainTargetGUID = m_uiTargetGUID;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHoarfrostTimer < uiDiff)
        {
            if (Unit* pTarget = Unit::GetUnit(*m_creature, m_uiMainTargetGUID))
                DoCast(pTarget, SPELL_HOARFROST);
            else if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_HOARFROST);
            m_uiHoarfrostTimer = 20000;
        }
        else
            m_uiHoarfrostTimer -= uiDiff;

        if (m_uiIcyBlastTimer < uiDiff)
        {
            if (Unit* pTarget = Unit::GetUnit(*m_creature, m_uiMainTargetGUID))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICY_BLAST : SPELL_ICY_BLAST_H);
            else if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICY_BLAST : SPELL_ICY_BLAST_H);
            m_uiIcyBlastTimer = 35000;
        }
        else
            m_uiIcyBlastTimer -= uiDiff;

        if (m_uiIcyBlastSlowTimer < uiDiff)
        {
            if (Unit* pTarget = Unit::GetUnit(*m_creature, m_uiMainTargetGUID))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICY_BLAST_SLOW : SPELL_ICY_BLAST_SLOW_H);
            else if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICY_BLAST_SLOW : SPELL_ICY_BLAST_SLOW_H);
            m_uiIcyBlastSlowTimer = 40000;
        }
        else
            m_uiIcyBlastSlowTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_TyrannusAI : public ScriptedAI
{
    boss_TyrannusAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiRimefangGuid;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;
    bool m_bIsTunnelIntro;
    bool m_bHasTaunted;
    bool m_bHasPassedTunnel;

    uint64 m_uiMartinGuid;
    uint64 m_uiGorkunGuid;

    uint32 m_uiForcefulSmashTimer;
    uint32 m_uiOverlordsBrandTimer;
    uint32 m_uiDarkMightTimer;
    uint32 m_uiMarkOfRimefangTimer;
    uint32 TeamInInstance;

    void Reset()
    {
        m_uiForcefulSmashTimer  = 10000;
        m_uiOverlordsBrandTimer = 35000;
        m_uiDarkMightTimer      = 40000;
        m_uiMarkOfRimefangTimer = 30000;

        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false; 

        m_bHasTaunted = false;

        m_uiMartinGuid      = 0;
        m_uiGorkunGuid      = 0;
        m_uiRimefangGuid    = 0;
        
        TeamInInstance = GetFaction();
        
        if(Creature* pRimefang = GetClosestCreatureWithEntry(m_creature, NPC_RIMEFANG, 100.0f))
        {
            pRimefang->GetMotionMaster()->MoveIdle();
            pRimefang->GetMap()->CreatureRelocation(pRimefang, RimefangSummon[0], RimefangSummon[1], RimefangSummon[2]  + 10, RimefangSummon[3]);
            pRimefang->SendMonsterMove(RimefangSummon[0], RimefangSummon[1], RimefangSummon[2] + 10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            m_uiRimefangGuid = pRimefang->GetGUID();
        }
        else
        {
            pRimefang = m_creature->SummonCreature(NPC_RIMEFANG, RimefangSummon[0], RimefangSummon[1], RimefangSummon[2], RimefangSummon[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
            m_uiRimefangGuid = pRimefang->GetGUID();

            if(pRimefang)
            {
                pRimefang->GetMotionMaster()->MoveIdle();
                pRimefang->GetMap()->CreatureRelocation(pRimefang, RimefangSummon[0], RimefangSummon[1], RimefangSummon[2]  + 10, RimefangSummon[3]);
                pRimefang->SendMonsterMove(RimefangSummon[0], RimefangSummon[1], RimefangSummon[2] + 10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, NOT_STARTED);
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
        if (!m_bHasTaunted && pWho->isInAccessablePlaceFor(m_creature) && !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80) && m_creature->IsWithinLOSInMap(pWho))
        {
            m_bIsIntro = true;
            m_bHasTaunted = true;

            m_uiIntro_Phase     = 0;
            m_uiSpeech_Timer    = 1000;

            if(Creature* pRimefang = m_pInstance->instance->GetCreature(m_uiRimefangGuid))
            {
                pRimefang->GetMotionMaster()->MoveIdle();
                pRimefang->GetMap()->CreatureRelocation(pRimefang, RimefangSummon[0], RimefangSummon[1], RimefangSummon[2]  + 10, RimefangSummon[3]);
                pRimefang->SendMonsterMove(RimefangSummon[0], RimefangSummon[1], RimefangSummon[2] + 10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            }
        }  
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        m_bIsIntro = false;

        if(Creature* pRimefang = m_pInstance->instance->GetCreature(m_uiRimefangGuid))
        {
            pRimefang->SetInCombatWithZone();
            pRimefang->GetMotionMaster()->MoveIdle();
            pRimefang->GetMap()->CreatureRelocation(pRimefang, RimefangSummon[0], RimefangSummon[1], RimefangSummon[2]  + 20, RimefangSummon[3]);
            pRimefang->SendMonsterMove(RimefangSummon[0], RimefangSummon[1], RimefangSummon[2] + 20, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        }
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    DoScriptText(SAY_INTRO1, m_creature);
                    if(TeamInInstance == ALLIANCE)
                        if(Creature* pMartin = m_creature->SummonCreature(NPC_MARTIN_VICTUS_END, 1060.955f, 107.274f, 629.424f, 2.084f, TEMPSUMMON_DEAD_DESPAWN, 0))
                            m_uiMartinGuid = pMartin->GetGUID();
                    if(TeamInInstance == HORDE)
                        if(Creature* pGorkun = m_creature->SummonCreature(NPC_GORKUN_IRONSKULL_END, 1060.955f, 107.274f, 629.424f, 2.084f, TEMPSUMMON_DEAD_DESPAWN, 0))
                            m_uiGorkunGuid = pGorkun->GetGUID();
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;
                case 1:
                    if(TeamInInstance == ALLIANCE)
                    {
                        if(Creature* pMartin = m_pInstance->instance->GetCreature(m_uiMartinGuid))
                            DoScriptText(SAY_INTRO2_SLAVES, pMartin);
                    }
                    if(TeamInInstance == HORDE)
                    {
                        if(Creature* pGorkun = m_pInstance->instance->GetCreature(m_uiGorkunGuid))
                            DoScriptText(SAY_INTRO2_SLAVES, pGorkun);
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 2:
                    DoScriptText(SAY_INTRO3, m_creature);
                    m_bIsIntro = false;
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 15000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiForcefulSmashTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FORCEFUL_SMASH : SPELL_FORCEFUL_SMASH_H);
            m_uiForcefulSmashTimer = 10000;
        }
        else
            m_uiForcefulSmashTimer -= uiDiff;

        if (m_uiOverlordsBrandTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_OVERLORDS_BRAND);
            m_uiOverlordsBrandTimer = 45000;
        }
        else
            m_uiOverlordsBrandTimer -= uiDiff;

        if (m_uiDarkMightTimer < uiDiff)
        {
            DoScriptText(SAY_DARK_MIGHT, m_creature);
            DoScriptText(SAY_DARK_MIGHT_EMOTE, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_DARK_MIGHT : SPELL_DARK_MIGHT_H);
            m_uiDarkMightTimer = 60000;
        }
        else
            m_uiDarkMightTimer -= uiDiff;

        if (m_uiMarkOfRimefangTimer < uiDiff)
        {
            DoScriptText(SAY_MARK_OF_RIMEFANG, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_MARK_OF_RIMEFANG);
                if(Creature* pRimefang = m_pInstance->instance->GetCreature(m_uiRimefangGuid))
                    ((boss_RimefangAI*)pRimefang->AI())->SetMainTarget(pTarget->GetGUID());
            }
            m_uiMarkOfRimefangTimer = urand(30000, 40000);
        }
        else
            m_uiMarkOfRimefangTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 80)
            EnterEvadeMode();
    }
    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (Creature* pRimefang = m_pInstance->instance->GetCreature(m_uiRimefangGuid))
        {
            pRimefang->GetMotionMaster()->Clear();
            pRimefang->GetMotionMaster()->MovePoint(0, 844.752f, 358.993f, 645.330f);
            pRimefang->setFaction(35);
            pRimefang->DeleteThreatList();
            pRimefang->RemoveAllAuras();
            pRimefang->SetVisibility(VISIBILITY_OFF);
            //pRimefang->DealDamage(pRimefang, pRimefang->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TYRANNUS, DONE);

        if(TeamInInstance == ALLIANCE)
            Creature *pJaina = m_creature->SummonCreature(NPC_JAINA_END, 1065.983f, 94.954f, 630.997f, 2.247f, TEMPSUMMON_DEAD_DESPAWN, 0);

        if(TeamInInstance == HORDE)
            Creature *pSylvanas = m_creature->SummonCreature(NPC_SYLVANAS_END, 1065.983f, 94.954f, 630.997f, 2.247f, TEMPSUMMON_DEAD_DESPAWN, 0);
    }
};

struct MANGOS_DLL_DECL mob_icy_blastAI : public ScriptedAI
{
    mob_icy_blastAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(11686);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    { 
        DoCast(m_creature, m_bIsRegularMode ? SPELL_ICY_BLAST_AURA : SPELL_ICY_BLAST_AURA_H);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_TYRANNUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_boss_Tyrannus(Creature* pCreature)
{
    return new boss_TyrannusAI (pCreature);
}

CreatureAI* GetAI_boss_Rimefang(Creature* pCreature)
{
    return new boss_RimefangAI (pCreature);
}

CreatureAI* GetAI_mob_icy_blast(Creature* pCreature)
{
    return new mob_icy_blastAI (pCreature);
}

CreatureAI* GetAI_npc_sylvanas_jaina_pos_end(Creature* pCreature)
{
    return new npc_sylvanas_jaina_pos_endAI (pCreature);
}

void AddSC_boss_Tyrannus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_Tyrannus";
    newscript->GetAI = &GetAI_boss_Tyrannus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_Rimefang";
    newscript->GetAI = &GetAI_boss_Rimefang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_icy_blast";
    newscript->GetAI = &GetAI_mob_icy_blast;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_pos_end;
    newscript->Name = "npc_slyvanas_jaina_pos_end";
    newscript->RegisterSelf();
}
