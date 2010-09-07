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
	// intro
	SAY_INTRO1                     = -1616000,
    SAY_INTRO2                     = -1616001,
    SAY_INTRO3                     = -1616002,
    SAY_INTRO4                     = -1616003,
    SAY_INTRO5                     = -1616004,

	// phase 1
	SAY_AGGRO1                     = -1616005,
	SAY_VORTEX                     = -1616006,
    SAY_POWER_SPARK                = -1616035,
    SAY_POWER_SPARK_BUFF           = -1616007,
    SAY_KILL1_1                    = -1616008,
    SAY_KILL1_2                    = -1616009,
    SAY_KILL1_3                    = -1616010,
	SAY_END_PHASE1                 = -1616012,

	// phase 2
	SAY_AGGRO2                     = -1616013,
	SAY_KILL2_1                    = -1616020,
    SAY_KILL2_2                    = -1616021,
    SAY_KILL2_3                    = -1616022,
	SAY_ARCANE_PULSE               = -1616014,
    SAY_ARCANE_PULSE_WARN          = -1616015,
	SAY_ARCANE_OVERLOAD            = -1616016,
	SAY_END_PHASE2                 = -1616017,

	// phase 3
    SAY_INTRO_PHASE3               = -1616018,
	SAY_AGGRO3                     = -1616019,
	SAY_KILL3_1                    = -1616023,
    SAY_KILL3_2                    = -1616024,
    SAY_KILL3_3                    = -1616025,
	SAY_CAST_SPELL1                = -1616027,
    SAY_CAST_SPELL2                = -1616028,
    SAY_CAST_SPELL3                = -1616029,
	SAY_SURGE_OF_POWER             = -1616026,
	SAY_DEATH                      = -1616030,
    
	// outro
    SAY_OUTRO1                     = -1616031,
    SAY_OUTRO2                     = -1616032,
    SAY_OUTRO3                     = -1616033,
    SAY_OUTRO4                     = -1616034,

	// spells
	// generic
    SPELL_BERSERK                  = 26662,

	// phase 1
    SPELL_ARCANE_BREATH            = 56272,
    SPELL_ARCANE_BREATH_H          = 60072,
    SPELL_VORTEX_DUMMY             = 56105, // This is for addons, actualy does nothing
    SPELL_VORTEX                   = 56266, // Cast on trigger in middle of the room, this interrupt their cast and deal dmg
    SPELL_VORTEX_AOE_VISUAL        = 55873, // visual effect around platform
    SPELL_POWER_SPARK              = 56152, // If spark reach malygos then buff him, if killed then to players
    SPELL_POWER_SPARK_PLAYERS      = 55852, // This one is propably for players
    SPELL_POWER_SPARK_VISUAL       = 55845,

	// phase 2
    SPELL_ARCANE_PULSE             = 57432, // Malygos takes a deep breath... cast on trigger
    SPELL_ARCANE_STORM             = 61693, // AOE
    SPELL_ARCANE_STORM_H           = 61694,
    SPELL_ARCANE_OVERLOAD          = 56432, // Cast this on arcane overload NPCs
    SPELL_DESTROY_PLATFORM_PRE     = 58842, // lights all over the platform
    SPELL_DESTROY_PLATFROM_BOOM    = 59084, // Big Blue boom
    
	// phase 3
    SPELL_STATIC_FIELD             = 57428, // Summon trigger and cast this on them should be enought
    SPELL_SURGE_OF_POWER           = 57407, // this is on one target
    SPELL_SURGE_OF_POWER_H         = 60936, // this is on unlimited tagets, must limit it in mangos
	SPELL_SURGE_OF_POWER_TRIGG	   = 56505,

	// outro
	SPELL_ALEXSTRASZA_BEAM			= 61028,
	SPELL_ALEXSTRASZA_VISUAL		= 61023,

	// nexus lord
    SPELL_ARCANE_SHOCK             = 57058,
    SPELL_ARCANE_SHOCK_H           = 60073,
	SPELL_HASTE					   = 57060,
	// scion of eternity
    SPELL_ARCANE_BARRAGE           = 58456,

    //Dragons spells
	NPC_WYRMREST_SKYTALON		   = 30161, // Dragon Vehicle in Third Phase
    SPELL_FLAME_SPIKE              = 56091,
    SPELL_ENGULF_IN_FLAMES         = 56092,
    SPELL_REVIVIFY                 = 57090,
    SPELL_LIFE_BURST               = 57143,
    SPELL_FLAME_SHIELD             = 57108,
    SPELL_BLAZING_SPEED            = 57092,
    
    // items
	ITEM_KEY_TO_FOCUSING_IRIS      = 44582,
    ITEM_KEY_TO_FOCUSING_IRIS_H    = 44581,
    
	// phase 1
    NPC_VORTEX                     = 30090,
    NPC_POWER_SPARK                = 30084,
    NPC_SPARK_PORTAL               = 30118, // For power sparks
    VEHICLE_VORTEX                 = 168,

    // phase 2
    NPC_HOVER_DISC                 = 30248, // Maybe wrong, two following NPC flying on them (vehicle)
    DISPLAY_HOVER_DISC             = 26876, // DisplayID of hover disc
    NPC_NEXUS_LORD                 = 30245, // two (?) of them are spawned on beginning of phase 2
    NPC_SCION_OF_ETERNITY          = 30249, // same, but unknow count
    NPC_ARCANE_OVERLOAD            = 30282, // Bubles    

    // phase 3
    NPC_SURGE_OF_POWER             = 30334, // Because its on three targets, malygos cant cast it by hymself
    NPC_STATIC_FIELD               = 30592, // Trigger for that spell. Hope its fly

    // outro
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
    PHASE_FLOOR                    = 1,
    PHASE_ADDS                     = 2,
    PHASE_DRAGONS                  = 3,
    PHASE_OUTRO                    = 4,

	ACHIEV_ETERNITY					= 1874,
	ACHIEV_ETERNITY_H				= 1875,
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
	{843.182f, 1215.42f},
    {847.67f, 1408.05f},
    {647.675f, 1403.8f},
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
    {808.0f, 1301.0f, 268.0f, 0},			// Phase 3 position 
    {749.0f, 1244.0f, 332.0f, 1.544f},      // Vortex FarSight loc
    {754.29f, 1301.18f, 266.17f, 0},		// Center of the platform, ground.
    {823.0f, 1241.0f, 299.0f, 0},			// Alexstrasza's  position
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
		m_bIsIntro = true;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint8 m_uiPhase;
	uint32 m_uiSpeechTimer;
    uint32 m_uiSpeechPhase;
	bool m_bIsIntro;
	uint32 m_uiPhaseChangeTimer;
	uint32 m_uiPhaseChangeStage;
	bool m_bIsNextPhase;
	uint32 m_uiMovementTimer;
	uint8 m_uiMovementStage;
	uint32 m_uiEncounterTimer;

    std::list<uint64> m_lSparkGUIDList;
    std::list<uint64> m_lDiscGUIDList;

	// phase 1
    uint32 m_uiEnrageTimer;
    uint32 m_uiVortexTimer;
    uint32 m_uiArcaneBreathTimer;
    uint32 m_uiPowerSparkTimer;
	uint8 m_uiVortexPhase;
	uint32 m_uiVortexMovementTimer;
	bool m_bIsVortex;
	// phase 2
    uint32 m_uiDeepBreathTimer;
    uint32 m_uiShellTimer;
    uint32 m_uiArcaneStormTimer;
	uint32 m_uiAddsDeadCount;
	uint32 m_uiMaxAdds;
	// phase 3
    uint32 m_uiStaticFieldTimer;
    uint32 m_uiArcanePulseTimer;
    uint32 m_uiSurgeOfPowerTimer;
    
    void Reset()
    {
        m_lSparkGUIDList.clear();

		m_uiPhase				= PHASE_NOSTART;
		m_uiMovementTimer		= 2000;
		m_uiMovementStage		= 0;
		m_uiSpeechTimer			= 10000;
        m_uiSpeechPhase			= 0;
		m_uiPhaseChangeTimer	= 7000;
		m_uiPhaseChangeStage	= 0;
		m_bIsNextPhase			= false;
		m_uiEncounterTimer		= 0;

		// phase 1
        m_uiEnrageTimer			= 600000;	// 10 min
        m_uiVortexTimer			= 60000;
        m_uiArcaneBreathTimer	= 15000;
        m_uiPowerSparkTimer		= 30000;
		m_uiVortexMovementTimer = 10000;
		m_uiVortexPhase			= 0;
		m_bIsVortex				= false;
		// phase 2
        m_uiDeepBreathTimer		= 70000;
        m_uiShellTimer			= 1000;
        m_uiArcaneStormTimer	= 15000;
		m_uiAddsDeadCount		= 0;
		m_uiMaxAdds				= m_bIsRegularMode ? 6 : 12;
		// phase 3
        m_uiStaticFieldTimer	= 15000;
        m_uiArcanePulseTimer	= 1000;
        m_uiSurgeOfPowerTimer	= 30000;
        
		m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
    }

    void JustReachedHome()
    {
		if(GameObject* pIris = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(m_bIsRegularMode ? GO_FOCUSING_IRIS : GO_FOCUSING_IRIS_H)))
			pIris->Respawn();
		if(GameObject* pPortal = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_EXIT_PORTAL)))
			pPortal->Respawn();
		if(GameObject* pPlatform = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_PLATFORM)))
			pPlatform->Respawn();

		if(m_pInstance)
			m_pInstance->SetData(TYPE_MALYGOS, NOT_STARTED);

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
		if(m_uiPhase == PHASE_NOSTART)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_MALYGOS, IN_PROGRESS);
        DoScriptText(SAY_AGGRO1, m_creature);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
		m_uiPhase = PHASE_FLOOR;
        
		if(GameObject* pPortal = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_EXIT_PORTAL)))
			pPortal->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    }

    void JustDied(Unit* pKiller)
    {
		if(m_pInstance)
			m_pInstance->SetData(TYPE_MALYGOS, DONE);
		DoScriptText(SAY_DEATH, m_creature);
		DespawnCreatures(NPC_STATIC_FIELD, 120.0f);

		if(m_uiEncounterTimer < 360000)
		{
			if(m_pInstance)
				m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_ETERNITY : ACHIEV_ETERNITY_H);
		}

		if(Creature* pTemp = m_creature->SummonCreature(NPC_ALEXSTRASZA, OtherLoc[3].x, OtherLoc[3].y, OtherLoc[3].z, 0, TEMPSUMMON_TIMED_DESPAWN, 90000))
		{
			pTemp->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
			pTemp->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
			pTemp->AddSplineFlag(SPLINEFLAG_FLYING);
			if(Creature* pTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_AOE_TRIGGER)))
				pTemp->CastSpell(pTrigger, SPELL_ALEXSTRASZA_BEAM, false);
		}
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
    }

    void DoMovement(float fPosX, float fPosY, float fPosZ, uint32 uiTime, bool bFly = false, bool bPoint = true)
    {        
        if(bFly)
		{
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        }
		else
		{
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        }

        if(bPoint)
            m_creature->GetMotionMaster()->MovePoint(0, fPosX, fPosY, fPosZ);
        else
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, fPosX, fPosY, fPosZ, m_creature->GetOrientation());
            m_creature->SendMonsterMove(fPosX, fPosY, fPosZ, SPLINETYPE_NORMAL , m_creature->GetSplineFlags(), uiTime);
        }
    }

    void DoVortex(uint8 uiPhase)
    {
        if(uiPhase == 0)
        {
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(false);
            DoMovement(OtherLoc[2].x, OtherLoc[2].y, OtherLoc[2].z+20, 0, true, false);
        }
        else if(uiPhase == 1)
        {
            if(Creature* pTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_AOE_TRIGGER)))
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

                if(Creature *pVortex = m_creature->SummonCreature(NPC_VORTEX, OtherLoc[1].x, OtherLoc[1].y, OtherLoc[1].z, OtherLoc[1].o, TEMPSUMMON_TIMED_DESPAWN, 18000))  
				{
					pVortex->SetSplineFlags(SPLINEFLAG_FLYING);
                    itr->getSource()->GetCamera().SetView(pVortex);
				}
            }        
        }
        else if(uiPhase > 1 && uiPhase < 26)
		{
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;

            if(Creature *pVortex = m_creature->SummonCreature(NPC_VORTEX, VortexLoc[uiPhase-1].x, VortexLoc[uiPhase-1].y, VORTEX_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
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
        }
		else if (uiPhase == 30 || uiPhase == 31)
        {    
            if(uiPhase == 31)
            {
                if(m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                
				m_uiVortexPhase = 0;
                m_bIsVortex = false;
                return;
            }
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                itr->getSource()->GetCamera().SetView(itr->getSource());
            
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(true);
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            z = FLOOR_Z;
            DoMovement(x, y, z, 0, false, false);
        }
    }

    void PowerSpark(uint8 uiAction)
    {
        if(uiAction == 1) //Summon
        {
            uint8 uiRandom = urand(0, 3);
            if(Creature *pSpark = m_creature->SummonCreature(NPC_POWER_SPARK, SparkLoc[uiRandom].x, SparkLoc[uiRandom].y, FLOOR_Z+10, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
            {
                pSpark->CastSpell(pSpark, SPELL_POWER_SPARK_VISUAL, false);
                pSpark->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                m_lSparkGUIDList.push_back(pSpark->GetGUID());
            }
        }
        else if(uiAction == 2 || uiAction == 3) // Start/stop movement
        {
            if(uiAction == 3)
                m_creature->RemoveAurasDueToSpell(SPELL_POWER_SPARK);

            if (m_lSparkGUIDList.empty())
                return;

            for(std::list<uint64>::iterator itr = m_lSparkGUIDList.begin(); itr != m_lSparkGUIDList.end(); ++itr)
            {
				if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pTemp->isAlive())
                    {
                        if(uiAction == 2)
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

	void SummonedCreatureJustDied(Creature* pSummon)
	{
		if(pSummon->GetEntry() == NPC_NEXUS_LORD || pSummon->GetEntry() == NPC_SCION_OF_ETERNITY)
			m_uiAddsDeadCount += 1;

		if(pSummon->GetEntry() == NPC_NEXUS_LORD)
		{
			float x,y,z;
			pSummon->GetPosition(x,y,z);
			z = FLOOR_Z;
			/*if(Vehicle *pDisc = m_creature->SummonVehicle(NPC_HOVER_DISC, x, y, z, 0))
			{
			((Creature*)pDisc)->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
			((Creature*)pDisc)->SetSpeedRate(MOVE_RUN, 3.5f, true);
			((Creature*)pDisc)->SetSpeedRate(MOVE_WALK, 3.5f, true);
			m_lDiscGUIDList.push_back(((Creature*)pDisc)->GetGUID());
			}*/
		}
	}

    void DoSpawnShell()
    {
        uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
        uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
        if(Creature *pShell = m_creature->SummonCreature(NPC_ARCANE_OVERLOAD, x, y, FLOOR_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 45000))
		{
			pShell->setFaction(14);
            pShell->CastSpell(pShell, SPELL_ARCANE_OVERLOAD, true);
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
        
        /*for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                if(pPlayer->GetVehicleGUID())
                    pPlayer->ExitVehicle();

                if(Vehicle *pTemp = m_creature->SummonVehicle(NPC_WYRMREST_SKYTALON, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0))
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
                }
            }
        }*/
    }

    void DespawnCreatures(uint32 uiEntry, float fDistance, bool bIsDisc = false)
    {
        if(bIsDisc)
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
        GetCreatureListWithEntryInGrid(m_pCreatures, m_creature, uiEntry, fDistance);

        if (m_pCreatures.empty())
            return;

        for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
            (*iter)->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiPhase == PHASE_NOSTART)
		{
			if(m_uiMovementTimer < uiDiff)
			{
				DoMovement(SparkLoc[m_uiMovementStage].x, SparkLoc[m_uiMovementStage].y, m_creature->GetPositionZ(), 0, true);
				++m_uiMovementStage;
				if(m_uiMovementStage == 4)
					m_uiMovementStage = 0;
				m_uiMovementTimer = 20000;
			}
			else m_uiMovementTimer -= uiDiff;

			if(m_pInstance->GetData(TYPE_MALYGOS) == SPECIAL)
			{
				if(m_uiSpeechTimer < uiDiff && m_bIsIntro)
				{
					switch(m_uiSpeechPhase)
					{
					case 0:
						DoScriptText(SAY_INTRO1, m_creature);
						++m_uiSpeechPhase;
						m_uiSpeechTimer = 13000;
						break;
					case 1:
						DoScriptText(SAY_INTRO2, m_creature);
						++m_uiSpeechPhase;
						m_uiSpeechTimer = 15000;
						break;
					case 2:
						DoScriptText(SAY_INTRO3, m_creature);
						++m_uiSpeechPhase;
						m_uiSpeechTimer = 15000;
						break;
					case 3:
						DoScriptText(SAY_INTRO4, m_creature);
						++m_uiSpeechPhase;
						m_uiSpeechTimer = 15000;
						break;
					case 4:
						DoScriptText(SAY_INTRO5, m_creature);
						m_bIsIntro = false;
						++m_uiSpeechPhase;
						m_uiSpeechTimer = 1000;
						break;
					default:
						m_uiSpeechTimer = 100000;
					}
				}
				else m_uiSpeechTimer -= uiDiff;
			}
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// for achiev
		m_uiEncounterTimer += uiDiff;

        if(m_uiEnrageTimer < uiDiff)
        {
            SetCombatMovement(true);
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
			m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_BERSERK);
            m_uiEnrageTimer = 60000;
        }
		else m_uiEnrageTimer -= uiDiff;

		switch(m_uiPhase)
		{
		case PHASE_FLOOR:

			if(m_uiVortexMovementTimer < uiDiff && m_bIsVortex)
			{
				DoVortex(m_uiVortexPhase);

				if(m_uiVortexPhase == 1 || m_uiVortexPhase == 11)
				{
					if(m_uiVortexPhase == 1)
						DoCast(m_creature, SPELL_VORTEX_DUMMY);
					m_uiVortexMovementTimer = 300;
				}
				else
					m_uiVortexMovementTimer = 500;

				if(m_uiVortexPhase >= MAX_VORTEX && m_uiVortexPhase < 29)
				{
					m_uiVortexPhase = 30;
					DoVortex(m_uiVortexPhase);
					PowerSpark(2);
					m_uiVortexMovementTimer = 1000;
					m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
					return;
				}
				++m_uiVortexPhase;
			}
			else m_uiVortexMovementTimer -= uiDiff;

			if(m_bIsVortex)
				return;

            if(m_uiVortexTimer < uiDiff)
            {
				// repair this hack!!
				// makes crash!
                //PowerSpark(3);
                //DoVortex(0);
                //m_uiVortexPhase = 1;
                m_uiVortexTimer = 56000;
                //m_uiVortexMovementTimer = 6000;
				//m_bIsVortex = true;
                DoScriptText(SAY_VORTEX, m_creature);
                m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
                return;
            }
			else m_uiVortexTimer -= uiDiff;

            if(m_uiArcaneBreathTimer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_BREATH : SPELL_ARCANE_BREATH_H);
                m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
            }
			else m_uiArcaneBreathTimer -= uiDiff;

            if(m_uiPowerSparkTimer < uiDiff)
            {
                PowerSpark(1);
                DoScriptText(SAY_POWER_SPARK, m_creature);
                m_uiPowerSparkTimer = 30000;
            }
			else m_uiPowerSparkTimer -= uiDiff;
                  
			if(m_creature->GetHealthPercent() < 50.0f)
			{
				m_creature->InterruptNonMeleeSpells(true);
				SetCombatMovement(false);
				m_creature->GetMotionMaster()->Clear(false);
				DoScriptText(SAY_END_PHASE1, m_creature);
				DoMovement(OtherLoc[2].x, OtherLoc[2].y, OtherLoc[2].z+40, 0, true, false);
				//Despawn power sparks
				DespawnCreatures(NPC_POWER_SPARK, 180.0f);
				m_uiPhase = PHASE_ADDS;
				m_uiPhaseChangeTimer = 25000;
				m_bIsNextPhase = true;
			}

			DoMeleeAttackIfReady();

			break;
		case PHASE_ADDS:

			if(m_uiPhaseChangeTimer < uiDiff && m_bIsNextPhase)
			{
				DoScriptText(SAY_AGGRO2, m_creature);
				m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				DoSpawnAdds();
				DoSpawnShell();
				m_uiShellTimer = 30000;
				m_uiPhaseChangeTimer = 10000;
				m_bIsNextPhase = false;
			}
			else m_uiPhaseChangeTimer -= uiDiff;

			if(m_bIsNextPhase)
				return;

            if(m_uiShellTimer < uiDiff)
            {
                DoSpawnShell();
                DoScriptText(SAY_ARCANE_OVERLOAD, m_creature);
                m_uiShellTimer = 20000;
            }
			else m_uiShellTimer -= uiDiff;

            if(m_uiDeepBreathTimer < uiDiff)
            {
                m_creature->StopMoving();
                DoScriptText(SAY_ARCANE_PULSE, m_creature);
                DoScriptText(SAY_ARCANE_PULSE_WARN, m_creature);
                if(Creature* pTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_AOE_TRIGGER)))
                    DoCast(pTrigger, SPELL_ARCANE_PULSE);
                m_uiDeepBreathTimer = 60000;
            }
			else m_uiDeepBreathTimer -= uiDiff;

            if(m_uiArcaneStormTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ARCANE_STORM : SPELL_ARCANE_STORM_H);
                m_uiArcaneStormTimer = 15000;
            }
			else m_uiArcaneStormTimer -= uiDiff;

			if(m_uiAddsDeadCount == m_uiMaxAdds)
			{
				m_creature->StopMoving();
				m_uiPhase = PHASE_DRAGONS;
				DoScriptText(SAY_END_PHASE2, m_creature);
				if(Creature* pTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_AOE_TRIGGER)))
					pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFORM_PRE, false);
				m_uiPhaseChangeTimer = 6500;
				m_bIsNextPhase = true;
			}

			if(m_creature->HasAura(SPELL_BERSERK))
                DoMeleeAttackIfReady();

			break;
		case PHASE_DRAGONS:

			if(m_uiPhaseChangeTimer < uiDiff && m_bIsNextPhase)
			{
				switch(m_uiPhaseChangeStage)
				{
				case 0:
					if(Creature* pTrigger = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_AOE_TRIGGER)))
                        pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFROM_BOOM, false);
					DespawnCreatures(NPC_ARCANE_OVERLOAD, 70.0f);
					DespawnCreatures(NPC_HOVER_DISC, 70.0f, true);
					++m_uiPhaseChangeStage;
					m_uiPhaseChangeTimer = 3000;
					break;
				case 1:
					DoMovement(OtherLoc[0].x, OtherLoc[0].y, OtherLoc[0].z, 0, true);
                    DoScriptText(SAY_INTRO_PHASE3, m_creature);
					if(GameObject* pPlatform = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_PLATFORM)))
					{
						pPlatform->SetUInt32Value(GAMEOBJECT_DISPLAYID,9060);
						pPlatform->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
						pPlatform->SetUInt32Value(GAMEOBJECT_BYTES_1,8449);
					}
					++m_uiPhaseChangeStage;
					m_uiPhaseChangeTimer = 2000;
					break;
				case 2:
					MountPlayers();
					++m_uiPhaseChangeStage;
					m_uiPhaseChangeTimer = 13000;
					break;
				case 3:
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					SetCombatMovement(false);
					m_creature->GetMotionMaster()->Clear(false, true);
					m_creature->GetMotionMaster()->MoveIdle();
					DoScriptText(SAY_AGGRO3, m_creature);
					m_bIsNextPhase = false;
					break;
				}				
			}
			else m_uiPhaseChangeTimer -= uiDiff;

			if(m_bIsNextPhase)
				return;

            if(m_uiArcanePulseTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ARCANE_PULSE);
                m_uiArcanePulseTimer = 1000;
            }
			else m_uiArcanePulseTimer -= uiDiff;

            if(m_uiStaticFieldTimer < uiDiff)
            {
                if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{
                    if(Creature *pField = m_creature->SummonCreature(NPC_STATIC_FIELD, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 25000))
					{
						pField->setFaction(14);
                        pField->CastSpell(pField, SPELL_STATIC_FIELD, true);
					}
				}
                DoScriptText(SAY_CAST_SPELL1-urand(0,2), m_creature);
                m_uiStaticFieldTimer = 20000+rand()%15000;
            }
			else m_uiStaticFieldTimer -= uiDiff;

            if(m_uiSurgeOfPowerTimer < uiDiff)
            {
				if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, m_bIsRegularMode ? SPELL_SURGE_OF_POWER : SPELL_SURGE_OF_POWER_H);
                DoScriptText(SAY_SURGE_OF_POWER, m_creature);
                m_uiSurgeOfPowerTimer = 16000+rand()%15000;
            }
			else m_uiSurgeOfPowerTimer -= uiDiff;	

			if(m_creature->HasAura(SPELL_BERSERK))
                DoMeleeAttackIfReady();

			break;
		}
    }
};
/*######
## npc_alexstrasza_eoe
######*/
struct MANGOS_DLL_DECL npc_alexstrasza_eoeAI : public ScriptedAI
{
    npc_alexstrasza_eoeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

	ScriptedInstance* m_pInstance;

	uint32 m_uiSpeechTimer;
	uint32 m_uiSpeechPhase;

	void Reset()
	{
		m_uiSpeechTimer				= 10000;
		m_uiSpeechPhase				= 0;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(m_uiSpeechTimer < uiDiff)
		{
			switch(m_uiSpeechPhase)
			{
			case 0:
				m_creature->CastStop();
				DoScriptText(SAY_OUTRO1, m_creature);
				++m_uiSpeechPhase;
				m_uiSpeechTimer = 8000;
				break;
			case 1:
				DoScriptText(SAY_OUTRO2, m_creature);
				++m_uiSpeechPhase;
				m_uiSpeechTimer = 5000;
				break;
			case 2:
				DoScriptText(SAY_OUTRO3, m_creature);
				++m_uiSpeechPhase;
				m_uiSpeechTimer = 25000;
				break;
			case 3:
				DoScriptText(SAY_OUTRO4, m_creature);
				++m_uiSpeechPhase;
				m_uiSpeechTimer = 20000;
				break;
			case 4:
				if(GameObject* pPortal = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_EXIT_PORTAL)))
					pPortal->Respawn();
				if(GameObject* pPlatform = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_PLATFORM)))
					pPlatform->Respawn();
				++m_uiSpeechPhase;
				m_uiSpeechTimer = 20000;
				break;
			default:
				m_uiSpeechTimer = 100000;
			}
		}
		else m_uiSpeechTimer -= uiDiff;
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
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsDead;
    uint32 m_uiCheckTimer;

    void Reset()
    {
        m_bIsDead = false;
        m_uiCheckTimer = 2500;
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_bIsDead || pDoneBy->GetTypeId() != TYPEID_PLAYER)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage > m_creature->GetHealth())
        {
            m_bIsDead = true;

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
			if(Creature* pMalygos = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_MALYGOS)))
			{
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
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCheckTimer < uiDiff)
        {
			if(Creature* pMalygos = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_MALYGOS)))
			{
				if(pMalygos->isAlive() && m_creature->GetVisibility() == VISIBILITY_ON && !m_bIsDead)
				{
					if(m_creature->IsWithinDist(pMalygos, 3.0f, false))
					{
						((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.clear();
						m_creature->CastSpell(pMalygos, SPELL_POWER_SPARK, true);
						m_creature->SetVisibility(VISIBILITY_OFF);
					}
				}
			}
			m_uiCheckTimer = 2500;
        }
		else m_uiCheckTimer -= uiDiff;
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
		pCreature->SetSplineFlags(SPLINEFLAG_FLYING);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiArcaneBarrageTimer;

    void Reset()
    {
        m_uiArcaneBarrageTimer = urand(5000, 10000);
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
            m_creature->GetMotionMaster()->MoveChase(pWho, 20.0f, 60.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if(m_uiArcaneBarrageTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_ARCANE_BARRAGE);
            m_uiArcaneBarrageTimer = urand(5000, 10000);
        }
		else m_uiArcaneBarrageTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

/*######
## mob_nexus_lord
######*/
struct MANGOS_DLL_DECL mob_nexus_lordAI : public ScriptedAI
{
    mob_nexus_lordAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		pCreature->SetSplineFlags(SPLINEFLAG_FLYING);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
	uint32 m_uiArcaneShockTimer;
	uint32 m_uiHasteTimer;

    void Reset()
    {
        m_uiArcaneShockTimer	= urand(5000, 10000);
		m_uiHasteTimer			= 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if(m_uiArcaneShockTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, m_bIsRegularMode ? SPELL_ARCANE_SHOCK : SPELL_ARCANE_SHOCK_H);
            m_uiArcaneShockTimer = 3000 + rand()%10000;
        }
		else m_uiArcaneShockTimer -= uiDiff;

        if(m_uiHasteTimer < uiDiff)
        {
			DoCast(m_creature, SPELL_HASTE);
            m_uiHasteTimer = 15000;
        }
		else m_uiHasteTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

/*######
## go_focusing_iris
######*/
bool GOHello_go_focusing_iris(Player* pPlayer, GameObject* pGo)
{
	if(pGo->GetInstanceData()->GetData(TYPE_MALYGOS) == DONE)
		return true;

    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

	bool hasItem = false;
	if (m_bIsRegularMode)
	{
		if(pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS, 1) || pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS_H, 1)) 
			hasItem = true;
	}
	else if(pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS_H, 1))
		hasItem = true;        

	if(!hasItem)        
		return false;

	if(Creature *pMalygos = pGo->GetMap()->GetCreature(pGo->GetInstanceData()->GetData64(NPC_MALYGOS)))
	{
		if(pMalygos->isAlive())
		{
			pMalygos->AI()->AttackStart(pPlayer);
			pMalygos->AddThreat(pPlayer, 1000.0f);
			pMalygos->SetInCombatWithZone();
			((boss_malygosAI*)pMalygos->AI())->m_uiPhase = PHASE_FLOOR;
		}
		pGo->SetPhaseMask(0, true);
    }
    return false;
}

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

CreatureAI* GetAI_mob_nexus_lord(Creature* pCreature)
{
    return new mob_nexus_lordAI(pCreature);
} 

CreatureAI* GetAI_npc_alexstrasza_eoe(Creature* pCreature)
{
    return new npc_alexstrasza_eoeAI(pCreature);
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

	newscript = new Script;
    newscript->Name = "mob_nexus_lord";
    newscript->GetAI = &GetAI_mob_nexus_lord;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_alexstrasza_eoe";
    newscript->GetAI = &GetAI_npc_alexstrasza_eoe;
    newscript->RegisterSelf(); 

	newscript = new Script;
    newscript->Name = "go_focusing_iris";
    newscript->pGOHello = &GOHello_go_focusing_iris;
    newscript->RegisterSelf();
}
