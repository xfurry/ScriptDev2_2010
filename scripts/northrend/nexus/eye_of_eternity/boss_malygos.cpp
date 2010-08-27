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
SDName: Boss_Malygos
SD%Complete: 90%
SDComment: verify timers
SDAuthor: Tassadar
SDCategory: Nexus, Eye of Eternity
EndScriptData */

#include "precompiled.h"
#include "eye_of_eternity.h"

enum
{
    // yells
	SAY_INTRO1                     = -1616000,
    SAY_INTRO2                     = -1616001,
    SAY_INTRO3                     = -1616002,
    SAY_INTRO4                     = -1616003,
    SAY_INTRO5                     = -1616004,
    SAY_INTRO_PHASE3               = -1616018,
    SAY_AGGRO1                     = -1616005,
    SAY_AGGRO2                     = -1616013,
    SAY_AGGRO3                     = -1616019,
    SAY_VORTEX                     = -1616006,
    SAY_POWER_SPARK                = -1616035,
    SAY_POWER_SPARK_BUFF           = -1616007,
    SAY_KILL1_1                    = -1616008,
    SAY_KILL1_2                    = -1616009,
    SAY_KILL1_3                    = -1616010,
    SAY_KILL2_1                    = -1616020,
    SAY_KILL2_2                    = -1616021,
    SAY_KILL2_3                    = -1616022,
    SAY_KILL3_1                    = -1616023,
    SAY_KILL3_2                    = -1616024,
    SAY_KILL3_3                    = -1616025,
    SAY_END_PHASE1                 = -1616012,
    SAY_END_PHASE2                 = -1616017,
    SAY_ARCANE_PULSE               = -1616014,
    SAY_ARCANE_PULSE_WARN          = -1616015,
    SAY_ARCANE_OVERLOAD            = -1616016,
    SAY_SURGE_OF_POWER             = -1616026,
    SAY_CAST_SPELL1                = -1616027,
    SAY_CAST_SPELL2                = -1616028,
    SAY_CAST_SPELL3                = -1616029,
    SAY_OUTRO1                     = -1616030,
    SAY_OUTRO2                     = -1616031,
    SAY_OUTRO3                     = -1616032,
    SAY_OUTRO4                     = -1616033,
    SAY_OUTRO5                     = -1616034,

	// spells
    SPELL_BERSERK                  = 26662, // Maybe wrong but enought :)
    SPELL_PORTAL_BEAM              = 56046,
	// phase 1
    SPELL_ARCANE_BREATH            = 56272,
    SPELL_ARCANE_BREATH_H          = 60072,
    SPELL_VORTEX_DUMMY             = 56105, // This is for addons, actualy does nothing
    SPELL_VORTEX                   = 56266, // Cast on trigger in middle of the room, this interrupt their cast and deal dmg
    SPELL_VORTEX_AOE_VISUAL        = 55873,    // visual effect around platform
    SPELL_POWER_SPARK              = 56152, // If spark reach malygos then buff him, if killed then to players
    SPELL_POWER_SPARK_PLAYERS      = 55852, // This one is propably for players
    SPELL_POWER_SPARK_VISUAL       = 55845,
	// phase 2
    //  SPELL_ARCANE_PULSE             = 57432, // Malygos takes a deep breath... cast on trigger
    SPELL_ARCANE_STORM             = 61693, // AOE
    SPELL_ARCANE_STORM_H           = 61694,
    SPELL_ARCANE_OVERLOAD          = 56432, // Cast this on arcane overload NPCs
    SPELL_ARCANE_BOMB              = 56431, // Cast by arcane overload
    SPELL_DEEP_BREATH              = 60071, // in Deadly boss mod this is as event spell, but not exist in DBC..
    SPELL_SURGE_OF_POWER_BREATH    = 56505, // omfg, they say deep breath, but its this!
    SPELL_DESTROY_PLATFORM_PRE     = 58842, // lights all over the platform
    SPELL_DESTROY_PLATFROM_BOOM    = 59084, // Big Blue boom
    //NPCs spells
    SPELL_ARCANE_SHOCK             = 57058,
    SPELL_ARCANE_SHOCK_H           = 60073,
    //SPELL_ARCANE_BARRAGE           = 63934, //This one has strange data in DBC
    SPELL_ARCANE_BARRAGE           = 58456, // I have to modify basepoints in this spell...
    BP_BARRAGE0                    = 14138,
    BP_BARRAGE0_H                  = 16965,
	// phase 3
    SPELL_STATIC_FIELD             = 57428, // Summon trigger and cast this on them should be enought
    SPELL_SURGE_OF_POWER           = 57407, // this is on one target
    SPELL_SURGE_OF_POWER_H         = 60936, // this is on unlimited tagets, must limit it in mangos
    
    SPELL_ARCANE_PULSE             = 57432,
    //Dragons spells
	NPC_WYRMREST_SKYTALON		= 30161, // Dragon Vehicle in Third Phase
    SPELL_FLAME_SPIKE              = 56091,
    SPELL_ENGULF_IN_FLAMES         = 56092,
    SPELL_REVIVIFY                 = 57090,
    SPELL_LIFE_BURST               = 57143,
    SPELL_FLAME_SHIELD             = 57108,
    SPELL_BLAZING_SPEED            = 57092,
    
    //////////////// PHASE 1 ////////////////
    NPC_AOE_TRIGGER                = 22517,
    NPC_VORTEX                     = 30090,
    NPC_POWER_SPARK                = 30084,
    NPC_SPARK_PORTAL               = 30118, // For power sparks
    VEHICLE_VORTEX                 = 168,

    //////////////// PHASE 2 ////////////////
    NPC_HOVER_DISC                 = 30248, // Maybe wrong, two following NPC flying on them (vehicle)
    DISPLAY_HOVER_DISC             = 26876, // DisplayID of hover disc
    NPC_NEXUS_LORD                 = 30245, // two (?) of them are spawned on beginning of phase 2
    NPC_SCION_OF_ETERNITY          = 30249, // same, but unknow count
    NPC_ARCANE_OVERLOAD            = 30282, // Bubles
    

    //////////////// PHASE 3 ////////////////
    NPC_SURGE_OF_POWER             = 30334, // Because its on three targets, malygos cant cast it by hymself
    NPC_STATIC_FIELD               = 30592, // Trigger for that spell. Hope its fly

    //////////////// PHASE 4 ////////////////
    NPC_ALEXSTRASZA                = 32295, // The Life-Binder

    

    SHELL_MIN_X                    = 722,
    SHELL_MAX_X                    = 768,
    SHELL_MIN_Y                    = 1290,
    SHELL_MAX_Y                    = 1339,

    NEXUS_LORD_COUNT               = 2,
    NEXUS_LORD_COUNT_H             = 4,
    SCION_OF_ETERNITY_COUNT        = 4,
    SCION_OF_ETERNITY_COUNT_H      = 8,

    PHASE_NOSTART                  = 0,
        SUBPHASE_FLY_DOWN1         = 04,
        SUBPHASE_FLY_DOWN2         = 05,
    PHASE_FLOOR                    = 1,
        SUBPHASE_VORTEX            = 11,
    PHASE_ADDS                     = 2,
        SUBPHASE_TALK              = 21,
    PHASE_DRAGONS                  = 3,
        SUBPHASE_DESTROY_PLATFORM1 = 31,
        SUBPHASE_DESTROY_PLATFORM2 = 32,
        SUBPHASE_DESTROY_PLATFORM3 = 33,
    PHASE_OUTRO                    = 4,
        SUBPHASE_STOP_COMBAT       = 41,
        SUBPHASE_DIE               = 42,
};
struct Locations
{
    float x, y, z, o;
    uint32 id;
};
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static Locations GOPositions[]=
{
    {754.346f, 1300.87f, 256.249f, 3.14159f},   // Raid Platform position
    {754.731f, 1300.12f, 266.171f, 5.01343f},   // Focusing iris and Alexstrazas gift
    {724.684f, 1332.92f, 267.234f, -0.802851f}, // Exit Portal
};
static LocationsXY SparkLoc[]=
{
    {652.417f, 1200.52f},
    {847.67f, 1408.05f},
    {647.675f, 1403.8f},
    {843.182f, 1215.42f},
};

//Also spawn locations for scions of eternity
static LocationsXY VortexLoc[]=
{
    {754, 1311},
    {734, 1334},
    {756, 1339},
    {781, 1329},
    {791, 1311},
    {790, 1283},
    {768, 1264},
    {739, 1261},
    {720, 1280},
    {714, 1299},
    {716, 1318},
    {734, 1334},
    {756, 1339},
    {781, 1329},
    {791, 1311},
    {790, 1283},
    {768, 1264},
    {739, 1261},
    {720, 1280},
    {714, 1299},
    {716, 1318},
};
static Locations OtherLoc[]=
{
    {808.0f, 1301.0f, 268.0f, 0},          // Phase 3 position 
    {749.0f, 1244.0f, 332.0f, 1.544f},      // Vortex FarSight loc
    {754.29f, 1301.18f, 266.17f, 0}, // Center of the platform, ground.
    {823.0f, 1241.0f, 299.0f, 0},          // Alexstrasza's  position
};
#define MAX_VORTEX              21
#define VORTEX_Z                268

#define FLOOR_Z                 268.17f
#define AIR_Z                   297.24f 
/*######
## boss_malygos
######*/

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI
{
    boss_malygosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
        pAlexstrasza = NULL;
        pTrigger = NULL;
        pFloor = NULL;
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    Creature *pAlexstrasza;
    Creature *pTrigger;
    GameObject *pFloor;
    
    uint8 m_uiPhase; //Fight Phase
    uint8 m_uiSubPhase; //Subphase if needed
    uint8 m_uiSpeechCount;
    uint8 m_uiVortexPhase;
    std::list<uint64> m_lSparkGUIDList;
    std::list<uint64> m_lDiscGUIDList;

    uint32 m_uiEnrageTimer;
    uint32 m_uiSpeechTimer[5];
    uint32 m_uiTimer;
    uint32 m_uiVortexTimer;
    uint32 m_uiArcaneBreathTimer;
    uint32 m_uiPowerSparkTimer;
    uint32 m_uiDeepBreathTimer;
    uint32 m_uiShellTimer;
    uint32 m_uiArcaneStormTimer;
    uint32 m_uiStaticFieldTimer;
    uint32 m_uiArcanePulseTimer;
    uint32 m_uiSurgeOfPowerTimer;
    
    void Reset()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, NOT_STARTED);
        else
            m_creature->ForcedDespawn();
        m_uiPhase = PHASE_NOSTART;
        m_uiSubPhase = 0;
        m_uiSpeechCount = 0;
        m_uiVortexPhase = 0;
        m_lSparkGUIDList.clear();

        m_uiEnrageTimer = 600000;
        m_uiSpeechTimer[0] = 15000;
        m_uiSpeechTimer[1] = 22000;
        m_uiSpeechTimer[2] = 24000;
        m_uiSpeechTimer[3] = 26000;
        m_uiSpeechTimer[4] = 23000;
        m_uiSpeechTimer[5] = 22000;
        m_uiTimer = 7000;
        m_uiVortexTimer = 60000;
        m_uiArcaneBreathTimer = 15000;
        m_uiPowerSparkTimer = 30000;
        m_uiDeepBreathTimer = 70000;
        m_uiShellTimer = 0;
        m_uiArcaneStormTimer = 15000;
        m_uiStaticFieldTimer = 15000;
        m_uiArcanePulseTimer = 1000;
        m_uiSurgeOfPowerTimer = 30000;
        
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        m_creature->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
        m_creature->SetSpeedRate(MOVE_RUN, 3.5f, true);
        m_creature->SetSpeedRate(MOVE_WALK, 3.5f, true);
    }

    void JustReachedHome()
    {
        Reset();
        //Summon Platform
        //if(!GetClosestGameObjectWithEntry(m_creature, GO_PLATFORM, 120.0f))
        //    pFloor = m_creature->SummonGameobject(GO_PLATFORM, GOPositions[0].x, GOPositions[0].y, GOPositions[0].z, GOPositions[0].o, 0);

        //Summon focusing iris
        //if(!GetClosestGameObjectWithEntry(m_creature, m_bIsRegularMode ? GO_FOCUSING_IRIS : GO_FOCUSING_IRIS_H, 120.0f))
        //    m_creature->SummonGameobject(m_bIsRegularMode ? GO_FOCUSING_IRIS : GO_FOCUSING_IRIS_H, GOPositions[1].x, GOPositions[1].y, GOPositions[1].z, GOPositions[1].o, 0);

        //Summon exit portal
        //if(!GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
        //    m_creature->SummonGameobject(GO_EXIT_PORTAL, GOPositions[2].x, GOPositions[2].y, GOPositions[2].z, GOPositions[2].o, 0);

        //Despawn all summoned creatures	 
        DespawnCreatures(NPC_POWER_SPARK, 150.0f);
        DespawnCreatures(NPC_ARCANE_OVERLOAD, 150.0f);
        DespawnCreatures(NPC_NEXUS_LORD, 150.0f);
        DespawnCreatures(NPC_SCION_OF_ETERNITY, 150.0f);
        DespawnCreatures(NPC_HOVER_DISC, 70.0f, true);
        DespawnCreatures(NPC_STATIC_FIELD, 150.0f);      
    }

    void AttackStart(Unit* pWho)
    {
        if(m_uiPhase != PHASE_FLOOR && m_uiPhase != PHASE_DRAGONS && !m_creature->HasAura(SPELL_BERSERK))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            if(m_uiPhase == PHASE_FLOOR || m_creature->HasAura(SPELL_BERSERK))
            if(!(m_uiPhase == PHASE_FLOOR && !m_creature->HasAura(SPELL_BERSERK)))
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetSpeedRate(MOVE_FLIGHT, 1.0f, true);
        m_creature->SetSpeedRate(MOVE_RUN, 1.0f, true);
        m_creature->SetSpeedRate(MOVE_WALK, 1.0f, true);
        m_pInstance->SetData(TYPE_MALYGOS, IN_PROGRESS);
        DoScriptText(SAY_AGGRO1, m_creature);
        //Despawn exit portal
        if(GameObject *pPortal = GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
            pPortal->Delete();

        pFloor = GetClosestGameObjectWithEntry(m_creature, GO_PLATFORM, 180.0f);
        pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 180.0f);

        //if(m_pInstance->GetData(TYPE_OUTRO_CHECK) == 1) //Should be enought to trigger outro immediatly
        {
            //Destroy Platform
            if(pTrigger)
                pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFROM_BOOM, false);
            if(pFloor)
                pFloor->Delete();                
                   
            //Mount Players
            MountPlayers();
            
            m_uiPhase = PHASE_OUTRO;
            m_uiSubPhase = SUBPHASE_STOP_COMBAT;
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_uiPhase == PHASE_OUTRO && m_uiSubPhase != SUBPHASE_DIE)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage >= m_creature->GetHealth() && m_uiSubPhase != SUBPHASE_DIE)
        {
            m_uiPhase = PHASE_OUTRO;
            m_uiSubPhase = SUBPHASE_STOP_COMBAT;
            uiDamage = 0;
        }
    }
    void JustDied(Unit* pKiller)
    {
        m_pInstance->SetData(TYPE_MALYGOS, DONE);
        //m_pInstance->SetData(TYPE_OUTRO_CHECK, 0);
    }
    void KilledUnit(Unit* pVictim)
    {
        uint8 text = 0;
        switch(m_uiPhase)
        {
            case PHASE_FLOOR:
                text = urand(0, 2);
                break;
            case PHASE_ADDS:
                text = urand(3, 5);
                break;
            case PHASE_DRAGONS:
                text = urand(6, 8);
                break;
            default:
                return;
        }
        switch(text)
        {
            case 0: DoScriptText(SAY_KILL1_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL1_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL1_3, m_creature); break;

            case 3: DoScriptText(SAY_KILL2_1, m_creature); break;
            case 4: DoScriptText(SAY_KILL2_2, m_creature); break;
            case 5: DoScriptText(SAY_KILL2_3, m_creature); break;

            case 6: DoScriptText(SAY_KILL3_1, m_creature); break;
            case 7: DoScriptText(SAY_KILL3_2, m_creature); break;
            case 8: DoScriptText(SAY_KILL3_3, m_creature); break;
        }
            
    }
    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if(pSpell->Id == SPELL_POWER_SPARK && m_uiPhase == PHASE_FLOOR)
            DoScriptText(SAY_POWER_SPARK_BUFF, m_creature);
        else if(pSpell->Id == SPELL_POWER_SPARK && m_uiPhase != PHASE_FLOOR)
            m_creature->RemoveAurasDueToSpell(SPELL_POWER_SPARK);
    }
    void SummonedCreatureDespawn(Creature* pDespawned)
    {
        if((pDespawned->GetDisplayId() != 24316 && pDespawned->GetDisplayId() != 24317 && 
            pDespawned->GetDisplayId() != 24318 && pDespawned->GetDisplayId() != 24319)
            || m_uiPhase == PHASE_NOSTART)
            return;

        float x,y,z;
        pDespawned->GetPosition(x,y,z);
        z = FLOOR_Z;
        /*if(Vehicle *pDisc = m_creature->SummonVehicle(NPC_HOVER_DISC, x, y, z, 0))
        {
            ((Creature*)pDisc)->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
            ((Creature*)pDisc)->SetSpeedRate(MOVE_RUN, 3.5f, true);
            ((Creature*)pDisc)->SetSpeedRate(MOVE_WALK, 3.5f, true);
            m_lDiscGUIDList.push_back(((Creature*)pDisc)->GetGUID());
        }*/
    }
    void DoMovement(float x, float y, float z, uint32 time, bool tofly = false, bool movepoint = true)
    {        
        if(tofly){
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        }else{
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        }
        WorldPacket heart;
        m_creature->BuildHeartBeatMsg(&heart);
        m_creature->SendMessageToSet(&heart, false);
        if(movepoint)
            m_creature->GetMotionMaster()->MovePoint(0, x,y,z);
        else
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, x, y, z, m_creature->GetOrientation());
            m_creature->SendMonsterMove(x, y, z, SPLINETYPE_NORMAL , m_creature->GetSplineFlags(), time);
        }
    }
    void DoVortex(uint8 phase)
    {
        if(phase == 0)
        {
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(false);
            DoMovement(OtherLoc[2].x, OtherLoc[2].y, OtherLoc[2].z+20, 0, true, false);
        }
        else if(phase == 1)
        {
            if(pTrigger)
                pTrigger->CastSpell(pTrigger, SPELL_VORTEX_AOE_VISUAL, false);

            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;

            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if(!itr->getSource()->isAlive())
                    continue;
                itr->getSource()->NearTeleportTo(VortexLoc[0].x, VortexLoc[0].y, VORTEX_Z, 0); 
                itr->getSource()->CastSpell(itr->getSource(), SPELL_VORTEX, true, NULL, NULL, m_creature->GetGUID());

                //Far sight, should be vehicle but this is enought
                // Crash the server in group update far members, dunno why
                // I will try to use this again, maybe I have fix...
                if(Creature *pVortex = m_creature->SummonCreature(NPC_VORTEX, OtherLoc[1].x, OtherLoc[1].y, OtherLoc[1].z, OtherLoc[1].o, TEMPSUMMON_TIMED_DESPAWN, 18000))          
                    itr->getSource()->GetCamera().SetView(pVortex);
            }        
        }
        else if(phase > 1 && phase < 26){
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;
            if(Creature *pVortex = m_creature->SummonCreature(NPC_VORTEX, VortexLoc[phase-1].x, VortexLoc[phase-1].y, VORTEX_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
            {
                pVortex->SetVisibility(VISIBILITY_OFF);
                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if(!itr->getSource()->isAlive())
                        continue;

                    float z = itr->getSource()->GetPositionZ() - VORTEX_Z;
                    itr->getSource()->KnockBackFrom(pVortex,-float(pVortex->GetDistance2d(itr->getSource())),7);
                    itr->getSource()->SetFacingToObject(m_creature);
                }
            }
        }else if (phase == 30 || phase == 31)
        {    
            if(phase == 31)
            {
                if(m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                
                m_uiSubPhase = 0;
                return;
            }
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                itr->getSource()->GetCamera().SetView(itr->getSource());
                //itr->getSource()->NearTeleportTo(VortexLoc[0].x, VortexLoc[0].y, VORTEX_Z+10, 0);
            }
            
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(true);
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            z = FLOOR_Z;
            DoMovement(x, y, z, 0, false, false);
        }
    }
    void PowerSpark(uint8 action)
    {
        if(action == 1) //Summon
        {
            uint8 random = urand(0, 3);
            if(Creature *pSpark = m_creature->SummonCreature(NPC_POWER_SPARK, SparkLoc[random].x, SparkLoc[random].y, FLOOR_Z+10, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
            {
                pSpark->CastSpell(pSpark, SPELL_POWER_SPARK_VISUAL, false);
                pSpark->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                m_lSparkGUIDList.push_back(pSpark->GetGUID());
            }
        }
        else if(action == 2 || action == 3) // Start/stop movement
        {
            if(action == 3)
                m_creature->RemoveAurasDueToSpell(SPELL_POWER_SPARK);

            if (m_lSparkGUIDList.empty())
                return;

            for(std::list<uint64>::iterator itr = m_lSparkGUIDList.begin(); itr != m_lSparkGUIDList.end(); ++itr)
            {
				if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pTemp->isAlive())
                    {
                        if(action == 2)
                            pTemp->GetMotionMaster()->MoveChase(m_creature);
                        else
                            pTemp->StopMoving();
                    }
                }
            }
        }
    }
    void DoSpawnAdds()
    {
        //Nexus lords
        int max_lords = m_bIsRegularMode ? NEXUS_LORD_COUNT : NEXUS_LORD_COUNT_H;
        for(int i=0; i < max_lords;++i)
        {
            if(Creature *pLord = m_creature->SummonCreature(NPC_NEXUS_LORD, m_creature->getVictim()->GetPositionX()-5+rand()%10, m_creature->getVictim()->GetPositionY()-5+rand()%10, m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                pLord->AI()->AttackStart(m_creature->getVictim());
        }
        //Scions of eternity
        int max_scions = m_bIsRegularMode ? SCION_OF_ETERNITY_COUNT : SCION_OF_ETERNITY_COUNT_H;
        for(int i=0; i < max_scions;++i)
        {
            uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
            uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
            if(Creature *pScion = m_creature->SummonCreature(NPC_SCION_OF_ETERNITY, x, y, FLOOR_Z+10, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pScion->AI()->AttackStart(pTarget);
        }       
    }
    bool IsThereAnyAdd()
    {
        //Search for Nexus lords
        if(GetClosestCreatureWithEntry(m_creature, NPC_NEXUS_LORD, 180.0f))
            return true;

        //Search for Scions of eternity
        if(GetClosestCreatureWithEntry(m_creature, NPC_SCION_OF_ETERNITY, 180.0f))
            return true;

        return false;
    }
    void DoSpawnShell()
    {
        uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
        uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
        if(Creature *pShell = m_creature->SummonCreature(NPC_ARCANE_OVERLOAD, x, y, FLOOR_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 45000))
        {
            pShell->CastSpell(pShell, SPELL_ARCANE_OVERLOAD, true);
            pShell->CastSpell(pShell, SPELL_ARCANE_BOMB, false);
        }
    }
    void MountPlayers()
    {
        Map *pMap = m_creature->GetMap();

        if(!pMap)
            return;

        Map::PlayerList const &lPlayers = pMap->GetPlayers();
        if (lPlayers.isEmpty())
            return;
        
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                //if(pPlayer->GetVehicleGUID())
                    //pPlayer->ExitVehicle();

                /*if(Vehicle *pTemp = m_creature->SummonVehicle(NPC_WYRMREST_SKYTALON, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0))
                {
                    ((Creature*)pTemp)->SetCreatorGUID(pPlayer->GetGUID());
                    uint32 health = ((Creature*)pTemp)->GetHealth() + (pPlayer->GetMaxHealth()*2); // may be wrong
                    //((Creature*)pTemp)->SetArmor(pPlayer->GetArmor());  // ???
                    ((Creature*)pTemp)->SetMaxHealth(health);
                    ((Creature*)pTemp)->SetHealth(health);
                    m_creature->AddThreat((Creature*)pTemp, 1.0f);  // To not leave combat
                    pPlayer->EnterVehicle(pTemp, 0, false);
                    if(((Creature*)pTemp)->GetHealth() != health)
                    {
                        ((Creature*)pTemp)->SetMaxHealth(health);
                        ((Creature*)pTemp)->SetHealth(health);
                    }
                    ((Creature*)pTemp)->SetFacingToObject(m_creature);
                }*/
            }
        }
    }
    void DespawnCreatures(uint32 entry, float distance, bool discs = false)
    {
        //Because vehicles cant be found by GetCreatureListWithEntryInGrid()
        if(discs)
        {
            if(m_lDiscGUIDList.empty())
                return;
            Map *pMap = m_creature->GetMap();

            if(!pMap)
                return;

            //for(std::list<uint64>::iterator itr = m_lDiscGUIDList.begin(); itr != m_lDiscGUIDList.end(); ++itr)
            //    if(Vehicle *pVehicle = pMap->GetVehicle(*itr))
            //        pVehicle->Dismiss();

            m_lDiscGUIDList.clear();
            return;
        }

        std::list<Creature*> m_pCreatures;
        GetCreatureListWithEntryInGrid(m_pCreatures, m_creature, entry, distance);

        if (m_pCreatures.empty())
            return;

        for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
            (*iter)->ForcedDespawn();

    }
    //Spell not in DBC, but on retail client recieve its opcode, so..
    void SendDeepBreathCast()
    {
        /*WorldPacket data(SMSG_SPELL_GO, 50);
        data << m_creature->GetPackGUID();

        data << m_creature->GetPackGUID();
        data << uint8(1);
        data << uint32(SPELL_DEEP_BREATH);
        data << uint32(256);
        data << uint32(getMSTime());

        data << uint8(0);
        data << uint8(0);
        data << uint8(0);

        m_creature->SendMessageToSet(&data, false);*/
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiPhase == PHASE_NOSTART)
        {
            if(m_uiSubPhase == SUBPHASE_FLY_DOWN1){
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->RemoveAurasDueToSpell(SPELL_PORTAL_BEAM);
                m_creature->GetMotionMaster()->Clear(false);
                DoMovement(OtherLoc[2].x, OtherLoc[2].y, FLOOR_Z, 0);
                m_uiSubPhase = SUBPHASE_FLY_DOWN2;
                m_uiTimer = 3500;
            }
            else if(m_uiSubPhase == SUBPHASE_FLY_DOWN2){
                if(m_uiTimer <= uiDiff)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_uiSubPhase = 0;
                    m_uiPhase = PHASE_FLOOR;
                }else m_uiTimer -= uiDiff;
            }  
            else
            {
                //Speech
                if(m_uiSpeechTimer[m_uiSpeechCount] <= uiDiff)
                {
                    DoScriptText(SAY_INTRO1-m_uiSpeechCount, m_creature);
                    m_uiSpeechCount++;
                    if(m_uiSpeechCount == 5){
                        m_uiSpeechCount = 0;
                        m_uiSpeechTimer[0] = 28000;
                        m_uiSpeechTimer[1] = 22000;
                        m_uiSpeechTimer[2] = 24000;
                        m_uiSpeechTimer[3] = 26000;
                        m_uiSpeechTimer[4] = 23000;
                        m_uiSpeechTimer[5] = 22000;
                    }
                }else m_uiSpeechTimer[m_uiSpeechCount] -= uiDiff;
                //Random movement over platform
                if(m_uiTimer <= uiDiff)
                {
                    uint8 tmp = urand(0,3);
                    DoMovement(SparkLoc[tmp].x, SparkLoc[tmp].y, m_creature->GetPositionZ(), 0, true);
                    m_uiTimer = 25000;
                }else m_uiTimer -= uiDiff;
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Enrage timer.....
        if(m_uiEnrageTimer <= uiDiff && m_uiPhase != PHASE_OUTRO)
        {
            //m_creature->StopMoving();
            SetCombatMovement(true);
            DoCast(m_creature, SPELL_BERSERK, true);
            m_uiEnrageTimer = 600000;
            m_creature->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
            m_creature->SetSpeedRate(MOVE_RUN, 3.5f, true);
            m_creature->SetSpeedRate(MOVE_WALK, 3.5f, true);
            m_creature->GetMotionMaster()->Clear(false, true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            //m_creature->AI()->AttackStart(m_creature->getVictim());
        }else m_uiEnrageTimer -= uiDiff;

        if(m_uiPhase == PHASE_FLOOR)
        {
            if(m_uiSubPhase == SUBPHASE_VORTEX)
            {
                if(m_uiTimer <= uiDiff)
                {
                    DoVortex(m_uiVortexPhase);
                    
                    if(m_uiVortexPhase == 1 || m_uiVortexPhase == 11){
                        if(m_uiVortexPhase == 1)
                            DoCast(m_creature, SPELL_VORTEX_DUMMY);
                        m_uiTimer = 300;
                    }else
                        m_uiTimer = 500;

                    if(m_uiVortexPhase >= MAX_VORTEX && m_uiVortexPhase < 29)
                    {
                        m_uiVortexPhase = 30;
                        DoVortex(m_uiVortexPhase);
                        PowerSpark(2);
                        m_uiTimer = 1000;
                        m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
                        return;
                    }
                    ++m_uiVortexPhase;
                }else m_uiTimer -= uiDiff;
                return;
            }

            //Vortex
            if(m_uiVortexTimer <= uiDiff)
            {
                PowerSpark(3);
                DoVortex(0);
                m_uiVortexPhase = 1;
                m_uiSubPhase = SUBPHASE_VORTEX;
                m_uiVortexTimer = 56000;
                m_uiTimer = 6000;
                DoScriptText(SAY_VORTEX, m_creature);
                m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
                return;
            }else m_uiVortexTimer -= uiDiff;

            //Arcane Breath
            if(m_uiArcaneBreathTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_BREATH : SPELL_ARCANE_BREATH_H);
                m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
            }else m_uiArcaneBreathTimer -= uiDiff;

            //PowerSpark
            if(m_uiPowerSparkTimer<= uiDiff)
            {
                PowerSpark(1);
                DoScriptText(SAY_POWER_SPARK, m_creature);
                m_uiPowerSparkTimer = 30000;
            }else m_uiPowerSparkTimer -= uiDiff;

            //Health check
            if(m_uiTimer<= uiDiff)
            {
                uint8 health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();                    
                if(health <= 50)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear(false);
                    DoScriptText(SAY_END_PHASE1, m_creature);
                    DoMovement(OtherLoc[2].x, OtherLoc[2].y, OtherLoc[2].z+40, 0, true, false);
                    //Despawn power sparks
                    DespawnCreatures(NPC_POWER_SPARK, 180.0f);
                    m_uiPhase = PHASE_ADDS;
                    m_uiSubPhase = SUBPHASE_TALK;
                    m_uiTimer = 23000;
                    m_creature->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
                    m_creature->SetSpeedRate(MOVE_RUN, 3.5f, true);
                    m_creature->SetSpeedRate(MOVE_WALK, 3.5f, true);
                    return;
                }
                m_uiTimer = 1500;
            }else m_uiTimer -= uiDiff;  

            DoMeleeAttackIfReady();
        }else if(m_uiPhase == PHASE_ADDS)
        {
            if(m_uiSubPhase == SUBPHASE_TALK)
            {
                if(m_uiTimer <= uiDiff)
                {
                    DoScriptText(SAY_AGGRO2, m_creature);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    DoSpawnAdds();
                    DoSpawnShell();
                    m_uiShellTimer = 30000;
                    m_uiSubPhase = 0;
                    m_uiTimer = 15000;
                }else m_uiTimer -= uiDiff;
                return;
            }
            
            //Arcane overload (bubble)
            if(m_uiShellTimer <= uiDiff)
            {
                DoSpawnShell();
                DoScriptText(SAY_ARCANE_OVERLOAD, m_creature);
                m_uiShellTimer = 20000;
            }else m_uiShellTimer -= uiDiff;

            // Deep breath
            if(m_uiDeepBreathTimer <= uiDiff)
            {
                m_creature->StopMoving();
                DoScriptText(SAY_ARCANE_PULSE, m_creature);
                DoScriptText(SAY_ARCANE_PULSE_WARN, m_creature);
                SendDeepBreathCast();
                if(pTrigger)
                    DoCast(pTrigger, SPELL_SURGE_OF_POWER_BREATH);

                m_uiDeepBreathTimer = 60000;
            }else m_uiDeepBreathTimer -= uiDiff;

            // Arcane Storm
            if(m_uiArcaneStormTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_STORM : SPELL_ARCANE_STORM_H);
                m_uiArcaneStormTimer = 15000;
            }else m_uiArcaneStormTimer -= uiDiff;

            if(m_uiTimer <= uiDiff)
            {
                if(!IsThereAnyAdd())
                {
                    m_creature->StopMoving();
                    m_uiPhase = PHASE_DRAGONS;
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM1;
                    DoScriptText(SAY_END_PHASE2, m_creature);
                    if(pTrigger)
                        pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFORM_PRE, false);
                    m_uiTimer = 6500;
                    return;
                }
                m_uiTimer = 5000;
            }else m_uiTimer -= uiDiff;  

            if(m_creature->HasAura(SPELL_BERSERK))
                DoMeleeAttackIfReady(); // this is there just for case of enrage
        }
        else if(m_uiPhase == PHASE_DRAGONS)
        {
            if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM1)
            {
                if(m_uiTimer<= uiDiff)
                {
                    //Destroy Platform
                    if(pTrigger)
                        pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFROM_BOOM, false);
                    if(pFloor)
                        pFloor->Delete();                
                    
                    //Mount Players
                    MountPlayers();

                    //Despawn bubbles and discs
                    DespawnCreatures(NPC_ARCANE_OVERLOAD, 70.0f);
                    DespawnCreatures(NPC_HOVER_DISC, 70.0f, true);

                    m_uiTimer = 2000;
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM2;
                }else m_uiTimer -= uiDiff;
                return;
            }
            else if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM2)
            {
                if(m_uiTimer<= uiDiff){
                    DoMovement(OtherLoc[0].x, OtherLoc[0].y, OtherLoc[0].z, 0, true);
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM3;
                    DoScriptText(SAY_INTRO_PHASE3, m_creature);
                    m_uiTimer = 14900;
                }else m_uiTimer -= uiDiff;
                return;
            }
            else if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM3)
            {
                if(m_uiTimer<= uiDiff){
                    m_uiSubPhase = 0;
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear(false, true);        // No moving!
                    m_creature->GetMotionMaster()->MoveIdle();
                    /*if(Unit *pVehicle = m_creature->GetMap()->GetCreature(m_creature->getVictim()->GetVehicleGUID()))
                    {
                        float victim_threat = m_creature->getThreatManager().getThreat(m_creature->getVictim());
                        DoResetThreat();
                        m_creature->AI()->AttackStart(pVehicle);
                        m_creature->AddThreat(pVehicle, victim_threat);
                    }*/
                    DoScriptText(SAY_AGGRO3, m_creature);
                }else m_uiTimer -= uiDiff;
                return;
            }
            //Arcane Pulse
            if(m_uiArcanePulseTimer <= uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_PULSE);
                m_uiArcanePulseTimer = 1000;
            }else m_uiArcanePulseTimer -= uiDiff;

            //Static field
            if(m_uiStaticFieldTimer <= uiDiff)
            {
                if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if(Creature *pField = m_creature->SummonCreature(NPC_STATIC_FIELD, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
                        pField->CastSpell(pField, SPELL_STATIC_FIELD, true);
                DoScriptText(SAY_CAST_SPELL1-urand(0,2), m_creature);
                m_uiStaticFieldTimer = 20000+rand()%15000;
            }else m_uiStaticFieldTimer -= uiDiff;

            //Surge of power
            if(m_uiSurgeOfPowerTimer <= uiDiff)
            {
                Unit* pTarget;
                //Malygos has some triggers in aggro list in that phase :o
                uint8 count = 0; // limit it to prevent lag
                for(pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0); pTarget->GetEntry() != NPC_WYRMREST_SKYTALON; )
                {
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                    ++count;
                    if(count >= 50)
                        break;
                }
                float victim_threat = m_creature->getThreatManager().getThreat(m_creature->getVictim());
                DoResetThreat();
                m_creature->AddThreat(pTarget, victim_threat);
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SURGE_OF_POWER : SPELL_SURGE_OF_POWER_H);
                DoScriptText(SAY_SURGE_OF_POWER, m_creature);
                m_uiSurgeOfPowerTimer = 16000+rand()%15000;
            }else m_uiSurgeOfPowerTimer -= uiDiff;	

            if(m_creature->HasAura(SPELL_BERSERK))
                DoMeleeAttackIfReady();
        }
        //Outro!
        else if(m_uiPhase == PHASE_OUTRO)
        {
            if(m_uiSubPhase == SUBPHASE_STOP_COMBAT)
            {
                //m_pInstance->SetData(TYPE_OUTRO_CHECK, 1);

                if (m_creature->IsNonMeleeSpellCasted(false))
                    m_creature->InterruptNonMeleeSpells(false);

                m_creature->RemoveAllAuras();
                SetCombatMovement(false);

                if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                   m_creature->GetMotionMaster()->MovementExpired();

                DespawnCreatures(NPC_STATIC_FIELD, 120.0f);
            
                m_creature->SetHealth(1);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->Clear(false);        // No moving!
                m_creature->GetMotionMaster()->MoveIdle();
                m_uiSpeechCount = 0;
                m_uiSpeechTimer[0] = 2000;
                m_uiSpeechTimer[1] = 8500;
                m_uiSpeechTimer[2] = 5000;
                m_uiSpeechTimer[3] = 3000;
                m_uiSpeechTimer[4] = 22000;

                if(Creature *pTemp = m_creature->SummonCreature(NPC_ALEXSTRASZA, OtherLoc[3].x, OtherLoc[3].y, OtherLoc[3].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    pTemp->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    pTemp->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    m_creature->SetFacingToObject(pTemp);
                    pTemp->SetFacingToObject(m_creature);
                    pTemp->SetVisibility(VISIBILITY_OFF);
                    pAlexstrasza = pTemp;
                }

                m_uiSubPhase = 0;
                return;
            }
            if(m_uiSpeechCount >= 5 || m_uiSubPhase == SUBPHASE_DIE)
                return;

            if(m_uiSpeechTimer[m_uiSpeechCount] <= uiDiff)
            {
                Creature *pSpeaker = NULL;
                if(m_uiSpeechCount == 0)
                    pSpeaker = m_creature;
                else
                    pSpeaker = pAlexstrasza;

                if(pSpeaker && pSpeaker->isAlive())
                    DoScriptText(SAY_OUTRO1-m_uiSpeechCount, pSpeaker);

                switch(m_uiSpeechCount)
                {
                    case 1:
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        pAlexstrasza->SetVisibility(VISIBILITY_ON);
                        pAlexstrasza->SetFacingToObject(m_creature->getVictim());
                        break;
                    case 4:

                        m_uiSubPhase = SUBPHASE_DIE;
                        //Summon exit portal, platform and loot
                        //m_creature->SummonGameobject(GO_EXIT_PORTAL, GOPositions[2].x, GOPositions[2].y, GOPositions[2].z, GOPositions[2].o, 0);
                       // m_creature->SummonGameobject(GO_PLATFORM, GOPositions[0].x, GOPositions[0].y, GOPositions[0].z, GOPositions[0].o, 0);
                        //if(GameObject *pGift = m_creature->SummonGameobject(m_bIsRegularMode ? GO_ALEXSTRASZAS_GIFT : GO_ALEXSTRASZAS_GIFT_H, GOPositions[1].x, GOPositions[1].y, GOPositions[1].z, GOPositions[1].o,0))
                            //pAlexstrasza->SetFacingToObject(pGift);
                        m_creature->getVictim()->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        break;
                }
                ++m_uiSpeechCount;
            }else m_uiSpeechTimer[m_uiSpeechCount] -= uiDiff;
            
        }
    }
};
/*######
## mob_power_spark
######*/
struct MANGOS_DLL_DECL mob_power_sparkAI : public ScriptedAI
{
    mob_power_sparkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool isDead;
    uint32 m_uiCheckTimer;
    Creature *pMalygos;

    void Reset()
    {
        isDead = false;
        pMalygos = GetClosestCreatureWithEntry(m_creature, NPC_MALYGOS, 150.0f);
        m_uiCheckTimer = 2500;
        WorldPacket heart;
        m_creature->BuildHeartBeatMsg(&heart);
        m_creature->SendMessageToSet(&heart, false);
    }
    void AttackStart(Unit *pWho)
    {
        return;
    }
    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {

        if (isDead || pDoneBy->GetTypeId() != TYPEID_PLAYER)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage > m_creature->GetHealth())
        {
            isDead = true;

            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            m_creature->RemoveAllAuras();
            m_creature->AttackStop();
            SetCombatMovement(false);

            m_creature->GetMotionMaster()->MovementExpired(false);

            m_creature->GetMotionMaster()->Clear(false);
            m_creature->StopMoving();
            uiDamage = 0;
            m_creature->SetHealth(1);
            m_creature->CastSpell(m_creature, SPELL_POWER_SPARK_PLAYERS, false);
            m_creature->ForcedDespawn(60000);
            if(pMalygos && pMalygos->isAlive())
            {
                bool delete_itr = false;
                if(!((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.empty())	
                {
                    std::list<uint64>::iterator itr; 
                    for(itr = ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.begin(); itr != ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.end(); ++itr)
                    {
                        if((*itr) == m_creature->GetGUID())
                        {
                            delete_itr = true;
                            break;
                        }
                    }
                    if(delete_itr)
                        ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.erase(itr);
                }
            }
        }
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCheckTimer <= uiDiff)
        {
            if(pMalygos && pMalygos->isAlive() && m_creature->GetVisibility() == VISIBILITY_ON && !isDead)
            {
                if(m_creature->IsWithinDist(pMalygos, 3.0f, false))
                {
                    ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.clear();
                    m_creature->CastSpell(pMalygos, SPELL_POWER_SPARK, true);
                    m_creature->SetVisibility(VISIBILITY_OFF);
                }
            }
            m_uiCheckTimer = 2500;
        }else m_uiCheckTimer -= uiDiff;
    }
};
/*######
## mob_scion_of_eternity
######*/

struct MANGOS_DLL_DECL mob_scion_of_eternityAI : public ScriptedAI
{
    mob_scion_of_eternityAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiArcaneBarrageTimer;
    //Movement - they are moving in "circles"....
    uint32 m_uiMoveTimer;
    bool m_bClockWise;
    float m_fDistance;
    float m_fAngle;

    void Reset()
    {
        m_creature->SetSpeedRate(MOVE_WALK, 0.7f, true);
        m_creature->SetSpeedRate(MOVE_RUN, 0.7f, true);
        m_creature->SetSpeedRate(MOVE_FLIGHT, 0.7f, true);
        m_uiArcaneBarrageTimer = 5000 + rand()%15000;
        InitMovement();
    }
    void InitMovement()
    {
        m_bClockWise = urand(0,1) ? true : false;
        m_uiMoveTimer = 1000;
        m_fDistance = m_creature->GetDistance2d(OtherLoc[2].x, OtherLoc[2].y); // From center of platform
        
        //Calculate angle - lol, i hope its right.
        float m_fLenght = 2*M_PI_F*m_fDistance; // perimeter of circle
        m_fAngle = (M_PI_F - ((2*M_PI_F) / m_fLenght)) / 2; // (Triangle(PI) - angle at center of circle) / 2  =  one of two another angles, I wish this can be explain in ASCII image :/

        if(m_bClockWise)
            m_fAngle += ((2*M_PI_F) / m_fLenght) + m_creature->GetAngle(OtherLoc[2].x, OtherLoc[2].y); // angle from 0 to center + alpha angle + beta angle
        else
            m_fAngle = m_creature->GetAngle(OtherLoc[2].x, OtherLoc[2].y) - m_fAngle - ((2*M_PI_F) / m_fLenght);

        //because it cant be lower than 0 or bigger than 2*PI
        m_fAngle = (m_fAngle >= 0) ? m_fAngle : 2 * M_PI_F + m_fAngle;
        m_fAngle = (m_fAngle <= 2*M_PI_F) ? m_fAngle : m_fAngle - 2 * M_PI_F;
    }
    
    void DoNextMovement()
    {
        WorldPacket heart;
        m_creature->BuildHeartBeatMsg(&heart);
        m_creature->SendMessageToSet(&heart, false);
        //Just rand point in range, not very smooth
        /*++m_uiMovePoint;
        uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
        uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
        m_creature->GetMotionMaster()->MovePoint(m_uiMovePoint, x, y, FLOOR_Z+10); */

        // Moving in "circles", <3 numberz :P
        bool canIncerase = true;
        bool canDecerase = true;
        if(m_fDistance > 22)
            canIncerase = false;
        if(m_fDistance < 4)
            canDecerase = false;

        uint8 tmp = urand(0,2); // 0 nothing, 1 incerase, 2 decerase
        if(tmp == 1 && canIncerase)
            m_fDistance += 0.5f;
        else if(tmp == 2 && canDecerase)
            m_fDistance -= 0.5f; 

        float m_fLenght = 2*M_PI_F*m_fDistance;
        float m_fRotateAngle = (2*M_PI_F) / m_fLenght; // Moving by 1y every 700ms
        //float m_fDestAngle = m_creature->GetOrientation();  <-- cant use this, creature is in combat
        if(m_bClockWise)
            m_fAngle -= m_fRotateAngle;
        else
            m_fAngle += m_fRotateAngle;

        //because it cant be lower than 0 or bigger than 2*PI
        m_fAngle = (m_fAngle >= 0) ? m_fAngle : 2 * M_PI_F + m_fAngle;
        m_fAngle = (m_fAngle <= 2*M_PI_F) ? m_fAngle : m_fAngle - 2 * M_PI_F;

        float destX = m_creature->GetPositionX();
        float destY = m_creature->GetPositionY();

        destX += cos(m_fAngle);
        destY += sin(m_fAngle);

        MaNGOS::NormalizeMapCoord(destX);
        MaNGOS::NormalizeMapCoord(destY);
        m_creature->AddSplineFlag(SPLINEFLAG_UNKNOWN7);
        m_creature->GetMap()->CreatureRelocation(m_creature, destX, destY, m_creature->GetPositionZ(), m_fAngle);
        m_creature->SendMonsterMove(destX, destY, m_creature->GetPositionZ(), SPLINETYPE_NORMAL , m_creature->GetSplineFlags(), 900);
    }
    void AttackStart(Unit *pWho)
    {
        if(pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            //m_creature->GetMotionMaster()->MoveChase(pWho, 15.0f);
        }
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if(m_uiArcaneBarrageTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                int32 bpoints0 = m_bIsRegularMode ? int32(BP_BARRAGE0) : int32(BP_BARRAGE0_H);
                m_creature->CastCustomSpell(pTarget, SPELL_ARCANE_BARRAGE, &bpoints0, 0, 0, false);  
                //DoCast(pTarget, SPELL_ARCANE_BARRAGE);  // this spell is not right :/
            }
            m_uiArcaneBarrageTimer = 3000 + rand()%19000;
        }else m_uiArcaneBarrageTimer -= uiDiff;

        if(m_uiMoveTimer <= uiDiff)
        {
            m_uiMoveTimer = 800;
            DoNextMovement();
        }else m_uiMoveTimer -= uiDiff;
    }
}; 

CreatureAI* GetAI_boss_malygos(Creature* pCreature)
{
    return new boss_malygosAI(pCreature);
}

CreatureAI* GetAI_mob_power_spark(Creature* pCreature)
{
    return new mob_power_sparkAI(pCreature);
}

CreatureAI* GetAI_mob_scion_of_eternity(Creature* pCreature)
{
    return new mob_scion_of_eternityAI(pCreature);
} 

void AddSC_boss_malygos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_malygos";
    newscript->GetAI = &GetAI_boss_malygos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_power_spark";
    newscript->GetAI = &GetAI_mob_power_spark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_scion_of_eternity";
    newscript->GetAI = &GetAI_mob_scion_of_eternity;
    newscript->RegisterSelf(); 
}
