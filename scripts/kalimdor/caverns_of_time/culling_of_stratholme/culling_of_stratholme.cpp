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
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment:
SDCategory: Culling of Stratholme
EndScriptData */


#include "precompiled.h"
#include "culling_of_stratholme.h"
#include "escort_ai.h"

enum
{
    SAY_INTRO01                           = -1594071,    //Arthas
    SAY_INTRO02                           = -1594072,    //Uther
    SAY_INTRO03                           = -1594073,    //Arthas
    SAY_INTRO04                           = -1594074,    //Arthas
    SAY_INTRO05                           = -1594075,    //Uther
    SAY_INTRO06                           = -1594076,    //Arthas
    SAY_INTRO07                           = -1594077,    //Uther
    SAY_INTRO08                           = -1594078,    //Arthas
    SAY_INTRO09                           = -1594079,    //Arthas
    SAY_INTRO10                           = -1594080,    //Uther
    SAY_INTRO11                           = -1594081,    //Arthas
    SAY_INTRO12                           = -1594082,    //Uther
    SAY_INTRO13                           = -1594083,    //Jaina
    SAY_INTRO14                           = -1594084,    //Arthas
    SAY_INTRO15                           = -1594085,    //Uther
    SAY_INTRO16                           = -1594086,    //Arthas
    SAY_INTRO17                           = -1594087,    //Jaina
    SAY_INTRO18                           = -1594088,    //Arthas
    SAY_ENTER01                           = -1594089,    //Arthas
    SAY_ENTER02                           = -1594090,    //Cityman
    SAY_ENTER03                           = -1594091,    //Arthas
    SAY_ENTER04                           = -1594092,    //Crazyman
    SAY_ENTER05                           = -1594093,    //Arthas
    SAY_ENTER06                           = -1594094,    //Malganis
    SAY_ENTER07                           = -1594095,    //Malganis
    SAY_ENTER08                           = -1594096,    //Arthas
    SAY_ENTER09                           = -1594097,    //Arthas
    SAY_PHASE501                          = -1594098,    //Arthas
    SAY_PHASE502                          = -1594099,    //Arthas
    SAY_PEOPLE05                          = -1594100,    //Patricia
    SAY_PEOPLE06                          = -1594101,    //Patricia
    SAY_PEOPLE07                          = -1594103,    //Patricia
    SAY_PEOPLE08                          = -1594105,    //Patricia
    SAY_PEOPLE09                          = -1594106,    //Patricia
    SAY_EPOCH                             = -1594117,    //Arthas Dialog for Epoch
    SAY_MEATHOOK_SPAWN                    = -1594110,
    SAY_PEOPLE01                          = -1594107,    //People Run
    SAY_PEOPLE02                          = -1594108,    //People Run
    SAY_PEOPLE03                          = -1594109,    //People Run
    SAY_PEOPLE04                          = -1594104,    //People Run
    SAY_PEOPLE10                          = -1594102,    //People Run
    SAY_PEOPLE11                          = -1594126,    //People Run
    SAY_PEOPLE12                          = -1594127,    //People Run
    SAY_PEOPLE13                          = -1594128,    //People Run
    SAY_PHASE503                          = -1594152,    //Arthas Shkaf 01
    SAY_PHASE504                          = -1594153,    //Arthas Shkaf 02
    SAY_PHASE505                          = -1594142,    //Arthas Glore
    SAY_PHASE506                          = -1594143,    //Arthas That is it
    SAY_PHASE507                          = -1594144,    //Arthas
    SAY_PHASE508                          = -1594145,    //Arthas This Magic Again
    SAY_PHASE509                          = -1594147,    //Arthas We are Close in trap
    SAY_PHASE510                          = -1594146,    //Arthas Lets go
    SAY_PHASE511                          = -1594151,    //Arthas Shkaff tam
    SAY_PHASE601                          = -1594154,    //Arthas Fire
    SAY_PHASE602                          = -1594155,    //Arthas Picnic
    SAY_PHASE603                          = -1594156,    //Arthas Picnic End
    SAY_PHASE604                          = -1594157,    //Arthas Stop Escort ska on ne virybaetsa 
    SAY_PHASE605                          = -1594158,    //Arthas Pipec Malganisy

    NPC_SCARED_MAN_2                      = 31127,
    NPC_DRAKONIAN                         = 27744,
    NPC_TIME_RIFT                         = 28409,
    NPC_TIME_RIFT_2                       = 28439,
    NPC_PATRICIA                          = 31028,
    NPC_SCARED_MAN                        = 31126,
    NPC_KNIGHT_ESCORT                     = 27745,
    NPC_PRIEST_ESCORT                     = 27747,
    NPC_JAINA                             = 26497,
    NPC_UTHER                             = 26528,
    NPC_CITY_MAN                          = 28167,
    NPC_CRAZY_MAN                         = 28169,
    NPC_ZOMBIE                            = 27737,

    SPELL_FEAR                            = 39176,
    SPELL_CHAIN_N                         = 52696,
    SPELL_CHAIN_H                         = 58823,
    SPELL_EXPLODED_N                      = 52666,
    SPELL_EXPLODED_H                      = 58821,
    SPELL_FRENZY                          = 58841,
    SPELL_ARTHAS_AURA                     = 52442,
    SPELL_EXORCISM                        = 52445,
    SPELL_EXORCISM_H                      = 58822,
    SPELL_HOLY_LIGHT                      = 52444,

    ENCOUNTER_ZOMBIE_NUMBER               = 4,
    ENCOUNTER_ZOMBIE_NUMBER2              = 50,

    // infinite corruptor
    BOSS_INFINITE_CORRUPTOR                 = 32273,
    SPELL_CORRUPTING_BLIGHT                 = 60588,
    SPELL_VOID_STRIKE                       = 60590, 

    ACHIEV_THE_CULLING_OF_TIME              = 1817,
    ACHIEV_ZOMBIEFEST                       = 1872,         // needs script support
};

/*######
## npc_arthas
######*/
struct MANGOS_DLL_DECL npc_arthasAI : public npc_escortAI
{
    npc_arthasAI(Creature *pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        ArthasGUID = 0;
        SalrammGUID = 0;
        PhaseC = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 ArthasGUID;
    uint64 SalrammGUID;
    uint64 EpochGUID;

    bool m_bIsRegularMode;
    uint32 Exorcism_Timer;
    uint32 FinalFight;
    bool PhaseC;
    uint32 arthas_event;
    Unit* culling_faction;
    Creature* Patricia;
    Creature* StalkerM;
    Creature* Crazyman;
    Creature* Cityman;
    Creature* Stalker;
    Creature* TempZombie;
    Creature* Salramm;
    Creature* Meathook;
    Creature* Epoch;
    Creature* Malganis;
    Creature* TempMalganis;
    Creature* Arthas;
    Creature* Jaina;
    Creature* Uther;
    Creature* Knightescort;
    Creature* Priestescort;
    uint32 phase;
    uint32 phaseAI;
    uint32 phasetim;
    uint64 uiZombieGUID[ENCOUNTER_ZOMBIE_NUMBER];
    uint32 uiZombie_counter;
    uint32 tmpZombie;
    uint32 PatriciaEvent;
    uint32 CorruptorTimer;
    bool isCorruptor;

    void Reset() 
    {
        if(arthas_event == 2) { } 
        else 
            arthas_event = 0;
        FinalFight = 1;
        phase = 1;
        phasetim = 20000;  
        Exorcism_Timer = 7300;
        Arthas=m_creature;
        CorruptorTimer = 900000;
        isCorruptor = true;

        if(m_pInstance->GetData(TYPE_ARTHAS) == DONE) 
            Arthas->SetVisibility(VISIBILITY_OFF);

        // this is Arthas's start position in the instance he will give gossip only here and in front of the gate at Mal'Ganis
        if(PhaseC == false && m_pInstance->GetData(TYPE_ARTHAS) != DONE)
        {
            Arthas->SetVisibility(VISIBILITY_ON);
            Arthas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            Arthas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }

    }

    void Aggro(Unit* who)
    {
        DoCast(m_creature, SPELL_ARTHAS_AURA);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    AttackStart(pWho);
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho, 0.0f);
                }
            }
        }
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ARTHAS, FAIL);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                m_creature->GetMotionMaster()->MovementExpired();

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
        case 1:
            DoScriptText(SAY_PHASE501, Arthas);
            break;
        case 2:
            DoScriptText(SAY_PHASE502, Arthas);
            break;
        case 3: 
            Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            DoScriptText(SAY_PHASE505, Arthas);
            break;
        case 4:
            DoScriptText(SAY_PHASE506, Arthas);
            Arthas->SummonCreature(NPC_TIME_RIFT,2225.388f,1178.470f,136.289f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);
            break;
        case 5: 
            DoScriptText(SAY_PHASE507, Arthas);

            Arthas->SummonCreature(NPC_TIME_RIFT,2264.253f,1162.153f,137.919f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);
            Arthas->SummonCreature(NPC_TIME_RIFT,2286.946f,1182.064f,137.986f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);
            break;
        case 9: 
            DoScriptText(SAY_PHASE509, Arthas);
            Arthas->SummonCreature(NPC_TIME_RIFT,2410.561f,1187.790f,133.933f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);
            Arthas->SummonCreature(NPC_TIME_RIFT,2388.574f,1214.650f,134.239f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);     
            Arthas->SummonCreature(NPC_TIME_RIFT,2430.593f,1212.919f,134.124f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);                                   
            break;
        case 10:
            DoScriptText(SAY_PHASE510, Arthas);
            break;
        case 11:
            Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            break; 
        case 13: 
            DoScriptText(SAY_PHASE508,Arthas);
            Arthas->SummonCreature(NPC_TIME_RIFT,2393.985f,1190.519f,148.076f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);  
            Arthas->SummonCreature(NPC_TIME_RIFT,2436.202f,1200.540f,148.077f,3.15f,TEMPSUMMON_TIMED_DESPAWN,11000);  
            break;
        case 15:
            DoScriptText(SAY_PHASE511,Arthas);
            Arthas->SummonCreature(NPC_TIME_RIFT_2,2445.629f,1111.500f,148.076f,3.229f,TEMPSUMMON_TIMED_DESPAWN,9000);
            if (EpochGUID = m_pInstance->GetData64(DATA_EPOCH)) 
            {
                // check if it has ben killed already
                if(m_pInstance->GetData(TYPE_EPOCH) != DONE)
                {
                    Epoch = m_pInstance->instance->GetCreature(EpochGUID);
                    Epoch->SetVisibility(VISIBILITY_ON);
                    Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Epoch->GetGUID());
                }
                // already has ben killed as the db says so needs to die
                if(m_pInstance->GetData(TYPE_EPOCH) == DONE)
                {
                    Epoch = m_pInstance->instance->GetCreature(EpochGUID);
                    Epoch->SetDeadByDefault(true);
                }
            }
            break;
        case 18:
            DoScriptText(SAY_EPOCH, Arthas);
            break;
        case 19:
            break;
        case 20:
            Arthas->AddSplineFlag(SPLINEFLAG_WALKMODE);
            break;
        case 23:
            DoScriptText(SAY_PHASE503,Arthas);
            break;
        case 24: 
            if (m_pInstance)
            {
                GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_SHKAF_GATE));
                pGate->SetGoState(GO_STATE_ACTIVE);
            }                    
            DoScriptText(SAY_PHASE504,Arthas);
            break;
        case 32:
            DoScriptText(SAY_PHASE601,Arthas);
            Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            break;
        case 34: 
            Arthas->AddSplineFlag(SPLINEFLAG_WALKMODE);
            DoScriptText(SAY_PHASE602,Arthas);
            break;
        case 35: 
            Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            DoScriptText(SAY_PHASE603,Arthas);
            break;
        case 40:
            DoScriptText(SAY_PHASE604,Arthas);
            break;
        case 41: 
            /*if(isCorruptor && !m_bIsRegularMode)
            {
                if (m_pInstance->GetData(TYPE_INFINITE_CORRUPTOR) != DONE)
                    Arthas->SummonCreature(BOSS_INFINITE_CORRUPTOR, 2329.686f, 1270.234f, 132.829f, 1.98f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 120000);
            }*/
            FinalFight = 1;
            Arthas->setFaction(35);
            phaseAI = 94;
            break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);

        DoMeleeAttackIfReady();
        if (phaseAI == 94 && FinalFight != 2)
        {
            Arthas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            Arthas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }
        if(FinalFight == 2)
        {
            Arthas->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            Arthas->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            switch(phaseAI)
            {
            case 95:
                DoScriptText(SAY_PHASE605, Arthas);
                if (m_pInstance)
                {
                    GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MAL_GATE1));
                    pGate->SetGoState(GO_STATE_ACTIVE);
                }                    
                ++phaseAI;
                phasetim = 3000;
                break;
            case 97:
                Arthas->GetMotionMaster()->MovePoint(0, 2303.016f, 1480.070f, 128.139f);
                ++phaseAI;
                phasetim = 3000;
                break;
            case 99:
                Arthas->setFaction(culling_faction->getFaction());
                ++phaseAI;
                phasetim = 3000;
                break;                       
            }
        }

        if(FinalFight == 2)
        {
            if (phasetim <= diff)
            {
                ++phaseAI;
                phasetim = 330000;
            }	
            phasetim -= diff;
        }

        if(arthas_event == 2)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (Exorcism_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, m_bIsRegularMode ? SPELL_EXORCISM : SPELL_EXORCISM_H);

                Exorcism_Timer = 7300;
            }
            else 
                Exorcism_Timer -= diff;

            if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 40)
                DoCast(m_creature, SPELL_HOLY_LIGHT);
        }

        if(arthas_event == 1)
        {
            Arthas->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            Arthas->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            switch(phase)
            {
            case 1:
                PhaseC = true;
                Arthas = m_creature;                   
                Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                Uther = Arthas->SummonCreature(26528,1794.357f,1272.183f,140.558f,1.37f,TEMPSUMMON_TIMED_DESPAWN,180000);

                if (Creature* pJaina = GetClosestCreatureWithEntry(Arthas, NPC_JAINA, 50.0f))
                    Jaina = pJaina;

                if (m_pInstance)
                    m_pInstance->SetData(TYPE_ARTHAS, NOT_STARTED);

                Uther->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                Arthas->GetMotionMaster()->MovePoint(0, 1903.167f, 1291.573f, 143.32f);
                Uther->GetMotionMaster()->MovePoint(0, 1897.018f, 1287.487f, 143.481f);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Uther->GetGUID());
                Uther->SetUInt64Value(UNIT_FIELD_TARGET, Arthas->GetGUID());
                ++phase;
                phasetim = 17000;
                break;
            case 3:
                DoScriptText(SAY_INTRO01, Arthas);
                ++phase;
                phasetim = 2000;
                break;
            case 5:
                DoScriptText(SAY_INTRO02, Uther);
                ++phase;
                phasetim = 8000;
                break;
            case 7:
                Arthas->AddSplineFlag(SPLINEFLAG_WALKMODE);
                DoScriptText(SAY_INTRO03, Arthas);
                Arthas->GetMotionMaster()->MovePoint(0, 1911.087f, 1314.263f, 150.026f);
                ++phase;
                phasetim = 9000;
                break;
            case 9:
                //Jaina = Arthas->SummonCreature(26497, 1895.48f, 1292.66f, 143.706f, 0.023475f,TEMPSUMMON_TIMED_DESPAWN,180000);
                Jaina->SetUInt64Value(UNIT_FIELD_TARGET, Arthas->GetGUID());
                DoScriptText(SAY_INTRO04, Arthas);
                ++phase;
                phasetim = 10000;
                break;
            case 11:
                DoScriptText(SAY_INTRO05, Uther);
                ++phase;
                phasetim = 1000;
                break;
            case 13:
                DoScriptText(SAY_INTRO06, Arthas);
                ++phase;
                phasetim = 4000;
                break;
            case 15:
                DoScriptText(SAY_INTRO07, Uther);
                ++phase;
                phasetim = 6000;
                break;
            case 17:
                DoScriptText(SAY_INTRO08, Arthas);
                ++phase;
                phasetim = 4000;
                break;
            case 19:
                DoScriptText(SAY_INTRO09, Uther);
                ++phase;
                phasetim = 8000;
                break;
            case 21:
                DoScriptText(SAY_INTRO10, Arthas);
                ++phase;
                phasetim = 4000;
                break;
            case 23:
                DoScriptText(SAY_INTRO11, Uther);
                ++phase;
                phasetim = 4000;
                break;
            case 25:
                DoScriptText(SAY_INTRO12, Arthas);
                ++phase;
                phasetim = 11000;
                break;
            case 27:
                DoScriptText(SAY_INTRO13, Jaina);
                ++phase;
                phasetim = 3000;
                break;
            case 29:
                DoScriptText(SAY_INTRO14, Arthas);
                ++phase;
                phasetim = 9000;
                break;
            case 31:
                DoScriptText(SAY_INTRO15, Uther);
                ++phase;
                phasetim = 4000;
                break;
            case 33:
                Uther->AddSplineFlag(SPLINEFLAG_WALKMODE);
                Uther->GetMotionMaster()->MovePoint(0, 1794.357f,1272.183f,140.558f);
                ++phase;
                phasetim = 1000;
                break;
            case 35:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Jaina->GetGUID());
                Jaina->GetMotionMaster()->MovePoint(0, 1794.357f,1272.183f,140.558f);
                ++phase;
                phasetim = 1000;
                break;
            case 37:
                DoScriptText(SAY_INTRO16, Arthas);
                ++phase;
                phasetim = 1000;
                break;
            case 39:
                DoScriptText(SAY_INTRO17, Jaina);
                ++phase;
                phasetim = 3000;
                break;
            case 41:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                Arthas->GetMotionMaster()->MovePoint(0, 1902.959f,1295.127f,143.388f);
                ++phase;
                phasetim = 10000;
                break;
            case 43:
                Arthas->GetMotionMaster()->MovePoint(0, 1913.726f,1287.407f,141.927f);
                ++phase;
                phasetim = 6000;
                break;
            case 45:
                DoScriptText(SAY_INTRO18, Arthas);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Jaina->GetGUID());
                ++phase;
                phasetim = 10000;
                break;
            case 47:
                Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                Jaina->SetVisibility(VISIBILITY_OFF);
                Uther->SetVisibility(VISIBILITY_OFF);
                Arthas->GetMotionMaster()->MovePoint(0, 1990.833f,1293.391f,145.467f);
                ++phase;
                phasetim = 12000;
                break;
            case 49:
                Arthas->GetMotionMaster()->MovePoint(0, 1997.003f,1317.776f,142.963f);
                ++phase;
                phasetim = 5000;
                break;
            case 51:
                Arthas->GetMotionMaster()->MovePoint(0, 2019.631f,1326.084f,142.929f);
                ++phase;
                phasetim = 4000;
                break;
            case 53:
                Arthas->GetMotionMaster()->MovePoint(0, 2026.469f,1287.088f,143.596f);
                ++phase;
                phasetim = 6000;
                break;
            case 55:
                Cityman = Arthas->SummonCreature(NPC_CITY_MAN,2091.977f,1275.021f,140.757f,0.558f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                Crazyman = Arthas->SummonCreature(NPC_CRAZY_MAN,2093.514f,1275.842f,140.408f,3.801f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
                Arthas->GetMotionMaster()->MovePoint(0, 2050.660f,1287.333f,142.671f);
                ++phase;
                phasetim = 6000;
                break;
            case 57:
                Stalker = Arthas->SummonCreature(20562,2026.469f,1287.088f,143.596f,1.37f,TEMPSUMMON_TIMED_DESPAWN,14000);
                Stalker->SetVisibility(VISIBILITY_OFF);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Stalker->GetGUID());
                ++phase;
                phasetim = 1000;
                break;
            case 59:
                DoScriptText(SAY_ENTER01, Arthas);
                ++phase;
                phasetim = 12000;
                break;
            case 61:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                Arthas->AddSplineFlag(SPLINEFLAG_WALKMODE);
                Arthas->GetMotionMaster()->MovePoint(0, 2081.447f,1287.770f,141.3241f);
                ++phase;
                phasetim = 15000;
                break;
            case 63:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Cityman->GetGUID());
                Cityman->SetUInt64Value(UNIT_FIELD_TARGET, Arthas->GetGUID());
                Cityman->AddSplineFlag(SPLINEFLAG_WALKMODE);
                Cityman->GetMotionMaster()->MovePoint(0, 2088.625f,1279.191f,140.743f);
                ++phase;
                phasetim = 2000;
                break;
            case 65:
                DoScriptText(SAY_ENTER02, Cityman);
                ++phase;
                phasetim = 4000;
                break;
            case 67:
                Arthas->GetMotionMaster()->MovePoint(0, 2087.689f,1280.344f,140.73f);
                DoScriptText(SAY_ENTER03, Arthas);
                ++phase;
                phasetim = 3000;
                break;
            case 69:
                Arthas->HandleEmoteCommand(37);
                ++phase;
                phasetim = 1000;
                break;
            case 71:
                DoScriptText(SAY_ENTER04, Crazyman);
                Crazyman->SetUInt64Value(UNIT_FIELD_TARGET, Arthas->GetGUID());
                Cityman->DealDamage(Cityman, Cityman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Crazyman->GetGUID());
                Arthas->GetMotionMaster()->MovePoint(0, 2092.154f,1276.645f,140.52f);
                ++phase;
                phasetim = 3000;
                break;
            case 73:
                Arthas->HandleEmoteCommand(37);
                ++phase;
                phasetim = 1000;
                break;
            case 75:
                Crazyman->DealDamage(Crazyman, Crazyman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                ++phase;
                phasetim = 1000;
                break;
            case 77:
                Stalker = Arthas->SummonCreature(20562,2081.447f,1287.770f,141.3241f,1.37f,TEMPSUMMON_TIMED_DESPAWN,70000);
                Stalker->SetVisibility(VISIBILITY_OFF);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Stalker->GetGUID());
                DoScriptText(SAY_ENTER05, Arthas);
                ++phase;
                phasetim = 3000;
                break;
            case 79:
                StalkerM = Arthas->SummonCreature(20562,2117.349f,1288.624f,136.271f,1.37f,TEMPSUMMON_TIMED_DESPAWN,60000);
                StalkerM->SetVisibility(VISIBILITY_OFF);
                StalkerM->CastSpell(StalkerM,63793,false);
                ++phase;
                phasetim = 1000;
                break;
            case 81:
                TempMalganis = Arthas->SummonCreature(26533,2117.349f,1288.624f,136.271f,1.37f,TEMPSUMMON_TIMED_DESPAWN,29000);
                DoScriptText(SAY_ENTER06, TempMalganis);
                TempMalganis->SetVisibility(VISIBILITY_ON);
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, TempMalganis->GetGUID());
                TempMalganis->SetUInt64Value(UNIT_FIELD_TARGET, Arthas->GetGUID());
                TempMalganis->setFaction(35);
                ++phase;
                phasetim = 11000;
                break;
            case 83:
                phasetim = 500;
                if (uiZombie_counter < ENCOUNTER_ZOMBIE_NUMBER)
                {
                    if (Creature* TempZombie = GetClosestCreatureWithEntry(StalkerM, NPC_CITY_MAN, 100.0f))
                    {
                        TempZombie->UpdateEntry(NPC_ZOMBIE, 0);
                        uiZombie_counter++;
                        ++phase;
                        tmpZombie = 4;
                    }
                }
                else
                {
                    uiZombie_counter = 0;
                    ++phase;
                }
                break;
            case 85:
                phasetim = 500;
                if (uiZombie_counter < ENCOUNTER_ZOMBIE_NUMBER)
                {
                    if (Creature* TempZombie = GetClosestCreatureWithEntry(StalkerM, NPC_CRAZY_MAN, 100.0f))
                    {
                        TempZombie->UpdateEntry(NPC_ZOMBIE, 0);
                        uiZombie_counter++;
                    }
                }
                else
                {
                    uiZombie_counter = 0;
                    ++phase;
                }
                break;
            case 87: 
                DoScriptText(SAY_ENTER07, TempMalganis);
                Arthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                ++phase;
                phasetim = 17000;
                if (SalrammGUID = m_pInstance->GetData64(DATA_SALRAMM)) 
                {
                    // check if it has ben killed already
                    if(m_pInstance->GetData(TYPE_SALRAMM) != DONE)
                    {
                        Salramm = m_pInstance->instance->GetCreature(SalrammGUID);
                        // set his flags back to be atackable if there is no cd on this boss
                        Salramm->SetVisibility(VISIBILITY_ON);
                        Salramm->setFaction(14);
                        Salramm->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        Salramm->GetMotionMaster()->MovePoint(0, 2165.110f,1284.64f,133.29f);
                    }
                    // already has ben killed as the db says so needs to die or just leave him with invisible flag
                    if(m_pInstance->GetData(TYPE_SALRAMM) == DONE)
                    {
                        Salramm = m_pInstance->instance->GetCreature(SalrammGUID);
                        Salramm->SetDeadByDefault(true);
                    }
                }
                break;
            case 89:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, StalkerM->GetGUID());
                DoScriptText(SAY_ENTER08, Arthas);
                ++phase;
                phasetim = 7000;
                break;
            case 91:
                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, Stalker->GetGUID());
                DoScriptText(SAY_ENTER09, Arthas);
                ++phase;
                phasetim = 12000;
                break;
            case 93:
                phaseAI = 95;

                if (m_pInstance)
                    m_pInstance->SetData(TYPE_ARTHAS, IN_PROGRESS);

                Arthas->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                CorruptorTimer = 900000;//1500000; // 25 min-> need to be adjusted to 15 min

                if (npc_arthasAI* pEscortAI = dynamic_cast<npc_arthasAI*>(Arthas->AI())) 
                    pEscortAI->Start(false, false, culling_faction->GetGUID(), NULL, false, false);

                Arthas->setFaction(culling_faction->getFaction());
                arthas_event = 2;
                ++phase;
                phasetim = 1000;
                break;
            }
        } 
        else 
            return;

        if(arthas_event == 1)
        {
            if (phasetim <= diff)
            {
                ++phase;
                phasetim = 330000;
            } 
            phasetim -= diff;
        }

        // corruptor
        if (CorruptorTimer <= diff && isCorruptor)
        {
            isCorruptor = false;
        } 
        CorruptorTimer -= diff;
    }
};

CreatureAI* GetAI_npc_arthas(Creature* pCreature)
{
    return new npc_arthasAI(pCreature);
}

bool GossipHello_npc_arthas(Player *player, Creature *mCreature)
{
    if (mCreature->isQuestGiver())
        player->PrepareQuestMenu(mCreature->GetGUID());

    if(((npc_arthasAI*)mCreature->AI())->arthas_event == 0)
    {
        /*
        if (player->FindQuestSlot(13151)>=25)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "You don't have the Quest for this instance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(mCreature), mCreature->GetGUID());
            return true;
        }
        if (player->FindQuestSlot(13151)<=25)
        {
        */
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I\'m ready start culling of stratholme", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(mCreature), mCreature->GetGUID());
            return true;
        //}
    }
    if(((npc_arthasAI*)mCreature->AI())->FinalFight == 1)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We are ready to Final Combat!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(mCreature), mCreature->GetGUID());
        player->hasQuest(13151);
        return true;
    }
    return true;
}

bool GossipSelect_npc_arthas(Player *player, Creature *mCreature, uint32 sender, uint32 uiAction )
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        player->CLOSE_GOSSIP_MENU();
        ((npc_arthasAI*)mCreature->AI())->arthas_event = 1;
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        player->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF+3:
        player->CLOSE_GOSSIP_MENU();
        ((npc_arthasAI*)mCreature->AI())->FinalFight = 2;
    }
    ((npc_arthasAI*)mCreature->AI())->culling_faction = player;

    return true;
}

/*######
## npc_patricia
######*/
struct MANGOS_DLL_DECL npc_patriciaAI : public ScriptedAI
{
    npc_patriciaAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Unit* Target;
    Creature* Arthas;
    Creature* Meathook;
    Creature* Patricia;

    uint32 Step;
    uint32 Steptim;
    uint64 MeathookGUID;
    bool Event;
    bool Event2;
    bool Event2Com;

    void Reset() 
    {
        Event = false;
        Event2 = true;
        Event2Com = false;
        Step = 1;
        Steptim = 20000;
        if(Event == true){}
        else
            Event = false;
    }

    void MoveInLineOfSight(Unit *who)
    {

        if (Event2 == false)
        {
            Target = who;
            Event2Com = true;
        }

        if (Event == false && m_creature->IsWithinDistInMap(who, 20.0f))
        {
            if (m_pInstance->GetData(TYPE_ARTHAS) == IN_PROGRESS)
            {
                Event = true;
            }
        }
        ScriptedAI::MoveInLineOfSight(who);
    }

    void UpdateAI(const uint32 diff)
    {
        DoMeleeAttackIfReady();

        if(Event == true)
        {
            switch(Step)
            {
            case 1:
                Patricia = m_creature;
                DoScriptText(SAY_PEOPLE05, Patricia);
                ++Step;
                Steptim = 5000;
                break;
            case 3:
                if (MeathookGUID = m_pInstance->GetData64(DATA_MEATHOOK)) 
                {
                    // check if it has ben killed already
                    if (m_pInstance->GetData(TYPE_MEATHOOK) != DONE)
                    {
                        Meathook = m_pInstance->instance->GetCreature(MeathookGUID);
                        Meathook->SetVisibility(VISIBILITY_ON);
                        Meathook->setFaction(14);
                        Meathook->GetMotionMaster()->MovePoint(0, 2354.626f,1192.099f,130.535f);
                        DoScriptText(SAY_MEATHOOK_SPAWN, Meathook);
                    }
                    // If is on cooldown kill it
                    if(m_pInstance->GetData(TYPE_MEATHOOK) == DONE)
                    {
                        Meathook = m_pInstance->instance->GetCreature(MeathookGUID);
                        Meathook->SetDeadByDefault(true);
                    }
                }
                Patricia->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                DoScriptText(SAY_PEOPLE06, Patricia);
                Patricia->GetMotionMaster()->MovePoint(0, 2395.487f,1203.199f,134.125f); 
                ++Step;
                Steptim = 13000;
                break;
            case 5:
                Patricia->GetMotionMaster()->MovePoint(0, 2431.674f,1211.797f,134.124f);
                ++Step;
                Steptim = 7000;
                break;
            case 7:
                Patricia->GetMotionMaster()->MovePoint(0, 2438.028f,1206.680f,133.935f);
                ++Step;
                Steptim = 3000;
                break;
            case 9:
                Patricia->GetMotionMaster()->MovePoint(0, 2437.524f,1206.971f,133.935f);
                Event2 = false;
                ++Step;
                Steptim = 3000;
                break; 
            case 11:
                if(Event2Com == false) 
                    return;
                Patricia->SetUInt64Value(UNIT_FIELD_TARGET, Target->GetGUID());
                ++Step;
                Steptim = 1000;
                break; 
            case 13:
                DoScriptText(SAY_PEOPLE07, m_creature);
                ++Step;
                Steptim = 4000;
                break; 
            case 15:
                DoScriptText(SAY_PEOPLE08, m_creature);
                ++Step;
                Steptim = 4000;
                break; 
            case 17:
                DoScriptText(SAY_PEOPLE09, m_creature);
                ++Step;
                Steptim = 9000;
                break; 
            case 19:
                m_creature->UpdateEntry(NPC_ZOMBIE, 0);
                Event = 0;
                ++Step;
                Steptim = 7000;
                break; 
            }
        }
        else 
            return;

        if (Steptim <= diff)
        {
            ++Step;
            Steptim = 330000;
        } 
        Steptim -= diff;

    }
};

CreatureAI* GetAI_npc_patricia(Creature* pCreature)
{
    return new npc_patriciaAI(pCreature);
}

/*######
## dark_conversion_AI
######*/
struct MANGOS_DLL_DECL dark_conversionAI : public ScriptedAI
{
    dark_conversionAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Unit* Target;
    Creature* Arthas;
    bool Conversion;
    uint32 Step;
    uint32 Steptim;

    void Reset() 
    {
        m_creature->setFaction(35);
        Conversion = false;
        Step = 1;
        Steptim = 500;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (Conversion == false && m_creature->IsWithinDistInMap(who, 30.0f))
        {
            if (m_pInstance->GetData(TYPE_ARTHAS) == IN_PROGRESS)
            {
                Target = who;
                Conversion = true;
            }
        }
        ScriptedAI::MoveInLineOfSight(who);
    }

    void UpdateAI(const uint32 diff)
    {

        DoMeleeAttackIfReady();

        if(Conversion == true)
        {
            switch(Step)
            {
            case 1:
                m_creature->setFaction(14);  //2078
                m_creature->CastSpell(m_creature,SPELL_FEAR,false);
                switch(rand()%12)
                {
                case 0: DoScriptText(SAY_PEOPLE01, m_creature); break;
                case 1: DoScriptText(SAY_PEOPLE02, m_creature); break;
                case 2: DoScriptText(SAY_PEOPLE03, m_creature); break;
                case 3: DoScriptText(SAY_PEOPLE04, m_creature); break; 
                case 4: DoScriptText(SAY_PEOPLE10, m_creature); break;
                case 5: DoScriptText(SAY_PEOPLE11, m_creature); break;
                case 6: DoScriptText(SAY_PEOPLE12, m_creature); break;
                case 7: DoScriptText(SAY_PEOPLE13, m_creature); break;
                }
                ++Step;
                Steptim = 5000 + rand()%5000;
                break;
            case 3:
                m_creature->UpdateEntry(NPC_ZOMBIE, 0);
                m_creature->GetMotionMaster()->MovePoint(0, Target->GetPositionX(), Target->GetPositionY(), Target->GetPositionZ());
                ++Step;
                Steptim = 1000;
                break;     
            }
        }
        else 
            return;

        if (Steptim <= diff)
        {
            ++Step;
            Steptim = 330000;
        } 
        Steptim -= diff;

    }
};

CreatureAI* GetAI_dark_conversion(Creature* pCreature)
{
    return new dark_conversionAI(pCreature);
}

/*######
## npc_time_riftCSAI
######*/
struct MANGOS_DLL_DECL npc_time_riftCSAI : public ScriptedAI
{
    npc_time_riftCSAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
    }

    Creature* Drakonian01;
    Creature* Drakonian02;
    Creature* Drakonian03;
    Creature* Arthas;
    bool Conversion;
    uint32 Step;
    uint32 Steptim;

    void Reset()
    {
        Conversion = false;
        Step = 1;
        Steptim = 500;
    }
    void UpdateAI(const uint32 diff)
    {
        switch(Step)
        {
        case 1:
            if (Creature* pArthas = GetClosestCreatureWithEntry(m_creature, NPC_ARTHAS, 180.0f))
                Arthas = pArthas;
            Drakonian01 = m_creature->SummonCreature(NPC_DRAKONIAN,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+1,3.229f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            Drakonian01->GetMotionMaster()->MovePoint(0, Arthas->GetPositionX(), Arthas->GetPositionY(), Arthas->GetPositionZ());
            ++Step;
            Steptim = 3000;
            break;
        case 3:
            Drakonian02 = m_creature->SummonCreature(NPC_DRAKONIAN,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+1,3.229f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            Drakonian02->GetMotionMaster()->MovePoint(0, Arthas->GetPositionX(), Arthas->GetPositionY(), Arthas->GetPositionZ());
            ++Step;
            Steptim = 3000;
            break;
        case 5:
            Drakonian03 = m_creature->SummonCreature(NPC_DRAKONIAN,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+1,3.229f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            Drakonian03->GetMotionMaster()->MovePoint(0, Arthas->GetPositionX(), Arthas->GetPositionY(), Arthas->GetPositionZ());
            ++Step;
            Steptim = 3000;
            break;
        }

        if (Steptim <= diff)
        {
            ++Step;
            Steptim = 330000;
        }
        Steptim -= diff;
    }
};

CreatureAI* GetAI_npc_time_riftCS(Creature* pCreature)
{
    return new npc_time_riftCSAI(pCreature);
}

/*######
## boss_infinite_corruptor
######*/
struct MANGOS_DLL_DECL boss_infinite_corruptorAI : public ScriptedAI
{
    boss_infinite_corruptorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 corruptingBlightTimer;
    uint32 voidStrikeTimer;

    void Reset()
    {
        corruptingBlightTimer = 30000;
        voidStrikeTimer = 7000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_INFINITE_CORRUPTOR, NOT_STARTED);
    }

    void Aggro(Unit* who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_INFINITE_CORRUPTOR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_INFINITE_CORRUPTOR, DONE);
            m_pInstance->DoCompleteAchievement(ACHIEV_THE_CULLING_OF_TIME);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();

        if (corruptingBlightTimer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_CORRUPTING_BLIGHT);
            corruptingBlightTimer = 30000;
        }
        else 
            corruptingBlightTimer -= diff;

        if (voidStrikeTimer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_VOID_STRIKE);
            voidStrikeTimer = 10000 + urand(1000, 4000);
        }
        else 
            voidStrikeTimer -= diff;
    }
};

CreatureAI* GetAI_boss_infinite_corruptor(Creature* pCreature)
{
    return new boss_infinite_corruptorAI(pCreature);
}

void AddSC_culling_of_stratholme()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_arthas";
    newscript->GetAI = &GetAI_npc_arthas;
    newscript->pGossipHello = &GossipHello_npc_arthas;
    newscript->pGossipSelect = &GossipSelect_npc_arthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "dark_conversion";
    newscript->GetAI = &GetAI_dark_conversion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_patricia";
    newscript->GetAI = &GetAI_npc_patricia;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_time_riftCS";
    newscript->GetAI = &GetAI_npc_time_riftCS;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_infinite_corruptor";
    newscript->GetAI = &GetAI_boss_infinite_corruptor;
    newscript->RegisterSelf();
}