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
SDName: boss_yogg_saron
SD%Complete: 90%
SDComment:  sanity, improve keepers, timers
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    //sara
    SAY_INTRO1                  = -1603307,
    SAY_INTRO2                  = -1603308,
    SAY_AGGRO                   = -1603300,
    SAY_HELP1                   = -1603301,
    SAY_HELP2                   = -1603302,
    SAY_SPECIAL1                = -1603305,
    SAY_SPECIAL2                = -1603306,
    SAY_KILL1                   = -1603303,
    SAY_KILL2                   = -1603304,
    SAY_WIPE                    = -1603309,

    //yogg
    SAY_PHASE2                  = -1603321,
    SAY_VISION                  = -1603322,
    SAY_PHASE3                  = -1603323,
    SAY_SLAY1                   = -1603324,
    SAY_SLAY2                   = -1603325,
    SAY_DEATH                   = -1603326,
    SAY_INSANITY1               = -1603327,
    SAY_INSANITY2               = -1603328,

    //visions
    //stormwind
    SAY_GARONA1                 = -1603341,
    SAY_GARONA2                 = -1603342,
    SAY_YOGG_V1_1               = -1603343,
    SAY_YOGG_V1_2               = -1603344,
    SAY_GARONA3                 = -1603345,
    SAY_GARONA4                 = -1603346,
    SAY_YOGG_V1_3               = -1603347,

    //dragons
    SAY_NELTHARION1             = -1603336,
    SAY_YSERA                   = -1603337,
    SAY_NELTHARION2             = -1603338,
    SAY_MALYGOS                 = -1603339,
    SAY_YOGG_V2                 = -1603340,

    //lich king
    SAY_LICH_KING1              = -1603330,
    SAY_CHAMPION1               = -1603331,
    SAY_CHAMPION2               = -1603332,
    SAY_LICH_KING2              = -1603333,
    SAY_YOGG_V3_1               = -1603334,
    SAY_YOGG_V3_2               = -1603335,

    //keepers yells
    SAY_HODIR_ACTIVE            = -1603086,
    SAY_FREYA_ACTIVE            = -1603009,
    SAY_THORIM_ACTIVE           = -1603238,
    SAY_MIMIRON_ACTIVE          = -1603260,

    //vision npc
    //stormwind
    NPC_GARONA                  = 33436,
    NPC_KING_LLANE              = 33437,

    MOB_SUIT_OF_ARMOR           = 33433,

    //dragons
    NPC_NELTHARION              = 33523,
    NPC_YSERA                   = 33495,
    NPC_MALYGOS                 = 33535,
    NPC_ALEXSTRASZA             = 33536,
    GO_DRAGON_SOUL              = 194462,

    MOB_RUBY_CONSORT            = 33716,
    MOB_AZURE_CONSORT           = 33717,
    MOB_BRONZE_CONSORT          = 33718,
    MOB_EMERALD_CONSORT         = 33719,
    MOB_OBSIDIAN_CONSORT        = 33720,


    //lich king
    NPC_LICH_KING               = 33441,
    NPC_IMMOLATED_CHAMPION      = 33442,

    NPC_VOICE_OF_YOGG_SARON     = 33280,
    MOB_VISION_TENTACLE         = 33943,
    SPELL_GRIM_REPRISAL         = 63305,
    // npc hp: 15k on 10 man; 40k on 25 man
    // npc no: 10

    //spells
    //phase1
    SPELL_SARAS_FERVOR          = 63138,
    SPELL_SARAS_BLESSING        = 63134,
    SPELL_SARAS_ANGER           = 63147,
    SPELL_DAMAGE_IMMUNE         = 34311,

    MOB_GUARDIAN_OF_YOGG        = 33136,
    SPELL_DARK_VOLLEY           = 63038,
    SPELL_SHADOW_NOVA           = 62714,    //when dies
    SPELL_SHADOW_NOVA_H         = 65209,
    SPELL_DOMINATE_MIND         = 63713,

    //phase2
    SPELL_EMERGE                = 20568,    // from ragnaros
    SPELL_SHADOWY_BARRIER       = 64775,
    SPELL_SHADOWY_BARRIER_YOGG  = 63894,
    SPELL_SANITY                = 63050,
    SPELL_PHYCHOSIS             = 65301,
    SPELL_PHYCHOSIS_H           = 63795,
    SPELL_MALADY_OF_THE_MIND    = 63830,
    SPELL_BRAIN_LINK            = 63802,
    SPELL_DEATH_RAY             = 63891,    //summons 4 death orbs
    MOB_DEATH_ORB               = 33882,
    SPELL_DEATH_RAY_AURA        = 63883,
    SPELL_DEATH_RAY_VISUAL      = 63886,

    // portals
    MOB_DESCEND_INTO_MADNESS    = 34072,
    SPELL_LUNATIC_GAZE          = 64167,    //affects players which take the portal to madness
    NPC_LAUGHING_SKULL          = 33990,
    SKULL_DISPLAY_ID            = 25206, 

    // brain's chamber
    MOB_BRAIN_OF_YOGG_SARON     = 33890,
    SPELL_SHATTERED_ILLUSION    = 64173,
    SPELL_INDUCE_MADNESS        = 64059,
    SPELL_INSANE                = 63120,


    // tentacules
    MOB_CRUSHER_TENTACLE        = 33966,
    SPELL_ERUPT                 = 64144,    //also used by the corruptor tentacle
    SPELL_DIMINISH_POWER        = 64145,
    SPELL_FOCUSED_ANGER         = 57689,

    MOB_CONSTRICTOR_TENTACLE    = 33983,
    SPELL_SQUEEZE               = 64125,
    SPELL_SQUEEZE_H             = 64126,

    MOB_CORRUPTOR_TENTACLE      = 33985,
    SPELL_APATHY                = 64156,
    SPELL_BLACK_PLAGUE          = 64153,
    SPELL_CURSE_OF_DOOM         = 64157,
    SPELL_DRAINING_POISON       = 64152,

    // phase 3
    SPELL_LUNATIC_GAZE_YOGG     = 64163,
    SPELL_SHADOW_BEACON         = 64465,
    SPELL_EMPOWERING_SHADOWS    = 64468,
    SPELL_EMPOWERING_SHADOWS_H  = 64486,
    SPELL_DEAFENING_ROAR        = 64189,    //only cast on 25 player with 0-3 keepes active

    MOB_IMMORTAL_GUARDIAN       = 33988,
    SPELL_EMPOWERED             = 65294,    //starts with 9 stacks and loses 1 stak at 10% hp

    SPELL_BERSERK               = 64166,    //26662, extinguish all life

    // keepers
    // freya
    SPELL_RESILIENCE_OF_NATURE  = 62670,
    MOB_SANITY_WELL             = 33991,
    SPELL_SANITY_WELL           = 64169,    // regen sanity
    SPELL_SANITY_WELL_VISUAL    = 63288,
    SPELL_SUMMON_SANITY_WELL    = 64170,

    // hodir
    SPELL_FORTITUDE_OF_FROST    = 62650,
    SPELL_HODIRS_PROTECTIVE_GAZE= 64174,    // saves players from killing blows ~25secs cd

    // thorim
    SPELL_SPEED_OF_INVENTION    = 62671,
    SPELL_DESTABILIZATION_MATRIX= 65210,    // cast in phase 2 on the tentacules

    // mimiron
    SPELL_FURY_OF_THE_STORM     = 62702,
    SPELL_TITANIC_STORM         = 64171,    // used in phase 3 to kill guardians
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {1951.097412f,-25.42042f, 326.162598f},
    {1970.677490f,-0.211162f, 325.478638f},
    {2001.049805f,-4.201718f, 325.751831f},
    {2009.346924f,-26.001806f,325.603271f},
    {1998.081665f,-46.625187f,325.551605f}, 
    {1962.782715f,-51.363148f,325.458160f},
    {1988.847778f,-71.143738f,328.647614f},
    {2024.004150f,-40.749989f,327.876617f},
    {2018.806885f,-3.451158f,327.593933f},
    {1978.651001f,18.373478f,328.420532f},
    {1940.641602f,1.761525f, 327.921661f},
    {1941.203735f,-52.999535f,327.246948f},
};

static LocationsXY SanityWellLoc[]=
{
    {1901.237915f, -46.305782f, 331.960754f}, 
    {1900.753052f,  -2.259287f, 332.061249f}, 
    {1991.020996f,  43.943943f, 331.746979f},
    {2044.219482f, -21.878244f, 329.776855f},
    {1986.226807f, -95.087761f, 330.253998f},
};

static LocationsXY KeepersLoc[]=
{
    {2036.859863f,  -74.113884f, 338.415222f, 2.488684f}, //thorim
    {1938.328247f,  -90.742043f, 338.459442f, 0.992500f}, //hodir
    {2036.107056f,  25.702380f,  338.415192f, 4.019527f}, // freya
    {1939.021240f,  43.221031f,  338.460663f, 5.214388f}, // mimiron
};

static LocationsXY TeleportLoc[]=
{
    {1941.587402f, 43.526680f,  239.666336f},    // stormwind
    {2055.460938f, -25.619570f, 239.721176f},    // dragons
    {1941.131226f, -94.654694f, 239.989639f},    // icecrown
    {1951.097f, -25.420f, 326.162f},             // yogg
};

static LocationsXY YoggPortalLoc[]=
{
    {1959.765137f, -20.697853f, 325.352966f},   
    {1973.407837f,  -6.656567f, 324.889526f},  
    {1990.271851f, -47.992981f, 324.959991f}, 
    {1978.893433f, -49.858326f, 324.777618f},
    {1967.471924f, -46.298458f, 324.840759f},
    {1959.654297f, -39.954502f, 324.997253f},
    {1956.343872f, -25.642859f, 325.188354f},
    {1985.072021f,  -2.515451f, 325.20010f},
    {1993.541626f, -10.527716f, 324.889587f}, 
    {2003.674316f, -23.050785f, 325.384064f}, 
};

static LocationsXY MadnessPortalLoc[]=
{
    {2001.015f,   4.185f, 242.747f},    
    {1999.690f, -54.931f, 242.418f},    
    {1946.898f, -25.769f, 242.169f},    
};
// vison pos
//Positional defines 
struct VisionLocXY
{
    float x, y, z, o;
    uint32 id;
};
// dragons
const float PosYsera[4] = {2114.504f, -16.118f, 242.646f, 3.91f};
const float PosMalygos[4] = {2113.388f, -34.381f, 242.646f, 2.26f};
const float PosNeltharion[4] = {2117.588f, -25.318f,  242.646f, 3.15f};
const float PosAlexstrasza[4] = {2091.679f, -25.289f, 242.646f, 6.282f};
const float PosVoiceDragon[3] = {2104.555f, -25.635f, 242.646f};
const float DisplayDragons[10] = {2718, 2718, 2717, 2717, 12869, 12869, 1687, 1687, 2719, 2719};

static VisionLocXY DragonLoc[]=
{
    {2071.951660f,   1.881840f, 239.794922f, 5.590341f},
    {2093.910156f,  19.939915f, 239.766830f, 4.962034f},
    {2114.737061f,  20.441664f, 239.757309f, 4.259104f},
    {2136.709473f,   4.874056f, 239.718658f, 3.889961f},
    {2147.894287f, -12.416141f, 239.757980f, 3.210588f},
    {2148.479004f, -37.655373f, 239.720169f, 3.033874f},
    {2136.296631f, -56.800838f, 239.754654f, 2.287745f},
    {2114.370117f, -68.110947f, 239.721100f, 1.789017f},
    {2093.946289f, -67.447899f, 239.720734f, 1.305995f},
    {2071.001709f, -54.414040f, 239.719345f, 0.528450f},
};

static VisionLocXY SkullDragonLoc[]=
{
    {2075.898193f, -5.637041f,  239.787735f},
    {2137.949219f, -26.778023f, 239.717712f},
    {2084.131836f, -52.716999f, 239.720703f},
};

// stormwind
const float PosGarona[4] = {1935.398926f, 54.017738f, 240.376465f, 2.008213f};
const float PosKing[4] = {1930.465670f, 62.674065f, 242.376373f, 5.196925f};
const float PosVoiceStormwind[3] = {1927.326f, 68.120f, 242.376f}; 

static VisionLocXY KeepLoc[]=
{
    {1930.854370f, 39.910034f, 239.666443f, 1.641476f},
    {1909.771240f, 45.685230f, 239.666443f, 0.962106f},
    {1898.966309f, 64.644989f, 239.666443f, 0.157073f},
    {1904.273926f, 85.369118f, 239.666443f, 5.662714f},
    {1923.474487f, 96.419815f, 239.666443f, 4.826267f},
    {1944.612061f, 91.062439f, 239.666443f, 4.076213f},
    {1955.231079f, 71.870926f, 239.666443f, 3.255475f},
    {1949.701416f, 51.040390f, 239.666443f, 2.481856f},
};

static VisionLocXY SkullKeepLoc[]=
{
    {1908.942261f, 58.934380f, 239.666382f},
    {1916.902954f, 86.755638f, 239.66662f},
    {1944.789307f, 78.614716f, 239.666382f},
};

// lich king
const float PosLichKing[4] = {1910.499268f,-147.709961f,239.989639f, 0.943203f};
const float PosChampion[4] = {1915.371094f,-139.934219f,239.989639f, 4.159409f};
const float PosVoiceIcecrown[3] = {1914.332f, -139.317f, 239.989f}; 

static VisionLocXY IcecrownLoc[]=
{
    {1952.521606f, -137.052094f, 239.989716f, 2.513270f},
    {1946.220337f, -130.236008f, 239.989716f, 5.387829f},
    {1942.029541f, -136.833328f, 239.989716f, 0.192416f},
    {1896.965210f, -104.922951f, 239.989716f, 5.647007f},
    {1904.356079f, -113.879349f, 239.989716f, 2.163766f},
    {1907.664795f, -106.186508f, 239.989716f, 2.976653f},
    {1919.831421f, -131.184784f, 239.989716f, 4.213656f},
    {1919.120728f, -145.960281f, 239.989716f, 1.908511f},
    {1907.462891f, -139.149307f, 239.989716f, 0.176708f},
};

static VisionLocXY SkullIcecrownLoc[]=
{
    {1962.658569f, -111.356392f, 239.98986f},
    {1940.515625f, -152.933945f, 239.989868f},
    {1889.130371f, -122.932549f, 239.98986f},
    {1908.828003f, -88.593613f, 239.98986f},
};

// location of the minds eye:
// X: 1981.422974 Y: -22.442831 Z: 236.104813

// transfer from brain
//  X: 1951.097412 Y: -25.420420 Z: 326.162598 Orientation: 0.131792
// brain room portal loc: 
// sara -> type_flags = 108; original

uint8 m_uiKeepersAlive;

struct MANGOS_DLL_DECL boss_yogg_saronAI : public ScriptedAI
{
    boss_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        //pCreature->setFaction(35);
        pCreature->SetVisibility(VISIBILITY_OFF);
        SetCombatMovement(false);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    bool isPhase3;
    bool isPhase2;
    uint32 berserkTimer;
    uint32 sanityTimer;
    uint32 m_uiKeepersActive;

    uint32 lunaticGazaTimer;
    uint32 shadowBeaconTimer;
    uint32 empoweringShadowsTimer;
    uint32 summonTimer;
    uint32 deafeningRoarTimer;
    uint8 stack;

    Creature *pGuardian;
    std::list<Creature*> lGuardianList;

    std::list<uint64> m_lGuardGUIDList;

    void Reset()
    {
        isPhase3 = false;
        isPhase2 = false;
        sanityTimer = 10000;
        m_uiKeepersActive = 0;
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->SetHealth(m_creature->GetMaxHealth());

        lGuardianList.clear();
        m_lGuardGUIDList.clear();

        if(m_pInstance) m_pInstance->SetData(TYPE_YOGGSARON, NOT_STARTED);

        if(m_creature->HasAura(SPELL_SHADOWY_BARRIER_YOGG))
            m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER_YOGG);
    }

    void Aggro(Unit *who) 
    {
        m_creature->SetInCombatWithZone();
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_YOGGSARON, IN_PROGRESS);
            // summon thorim
            if(m_pInstance->GetData(TYPE_KEEPER_THORIM) == DONE)
            {
                m_creature->SummonCreature(KEEPER_THORIM, KeepersLoc[0].x, KeepersLoc[0].y, KeepersLoc[0].z, KeepersLoc[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon hodir
            if(m_pInstance->GetData(TYPE_KEEPER_HODIR) == DONE)
            {
                m_creature->SummonCreature(KEEPER_HODIR, KeepersLoc[1].x, KeepersLoc[1].y, KeepersLoc[1].z, KeepersLoc[1].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon freya
            if(m_pInstance->GetData(TYPE_KEEPER_FREYA) == DONE)
            {
                m_creature->SummonCreature(KEEPER_FREYA, KeepersLoc[2].x, KeepersLoc[2].y, KeepersLoc[2].z, KeepersLoc[2].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
            // summon mimiron
            if(m_pInstance->GetData(TYPE_KEEPER_MIMIRON) == DONE)
            {
                m_creature->SummonCreature(KEEPER_MIMIRON, KeepersLoc[3].x, KeepersLoc[3].y, KeepersLoc[3].z, KeepersLoc[3].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                m_uiKeepersActive += 1;
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_YOGGSARON, NOT_STARTED);

            if(Creature* pSara = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_SARA)))
            {
                if(!pSara->isAlive())
                    pSara->Respawn();
            }

            if (Creature* pYoggBrain = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGG_BRAIN))))
            {
                if(!pYoggBrain->isAlive())
                    pYoggBrain->Respawn();
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        m_lGuardGUIDList.clear();
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_YOGGSARON, DONE);

            if(m_uiKeepersAlive == 0)
                m_pInstance->SetData(TYPE_YOGGSARON_HARD, DONE);
        }

        GetCreatureListWithEntryInGrid(lGuardianList, m_creature, MOB_IMMORTAL_GUARDIAN, DEFAULT_VISIBILITY_INSTANCE);

        if (!lGuardianList.empty())
        {
            for(std::list<Creature*>::iterator iter = lGuardianList.begin(); iter != lGuardianList.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
        if(Creature* pSara = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_SARA)))
        {
            if(pSara->isAlive())
                pSara->DealDamage(pSara, pSara->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        if (Creature* pYoggBrain = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGG_BRAIN))))
        {
            if(pYoggBrain->isAlive())
                pYoggBrain->DealDamage(pYoggBrain, pYoggBrain->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

    }

    void StartSecondPhase()
    {
        m_creature->SetVisibility(VISIBILITY_ON);
        //DoCast(m_creature, SPELL_EMERGE); 
        m_creature->setFaction(14);
        StartSanity();
        isPhase2 = true;
        DoCast(m_creature, SPELL_SHADOWY_BARRIER_YOGG);
    }

    void StartThirdPhase(uint32 bsrkTimer)
    {
        DoScriptText(SAY_PHASE3, m_creature);
        m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER_YOGG);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.3);
        berserkTimer = bsrkTimer;
        lunaticGazaTimer        = 20000;
        shadowBeaconTimer       = 15000;
        empoweringShadowsTimer  = 60000;
        summonTimer             = 40000;
        deafeningRoarTimer      = 30000;
        isPhase3 = true;

        if(Creature* pSara = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_SARA)))
        {
            pSara->SetVisibility(VISIBILITY_OFF);
            m_creature->DealDamage(pSara, pSara->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void CastShatterIllusion()
    {
        //DoCast(m_creature, SPELL_SHATTERED_ILLUSION); // spell is broken -> is cast on players and should be cast on Yogg & tentacles
    }

    void StartSanity()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                    DoCast(i->getSource(), SPELL_SANITY);
                    if(i->getSource()->HasAura(SPELL_SANITY, EFFECT_INDEX_0))
                        i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(100);
                }
            }
        }
    }

    void DoCastSanity()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                    // reduce sanity
                    if(i->getSource()->HasAura(SPELL_SANITY, EFFECT_INDEX_0))
                    {
                        if(Aura *aura = i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0))
                        {
                            stack = aura->GetStackAmount();
                            if(stack == 100)
                                DoCast(i->getSource(), SPELL_INSANE);
                            /*if(stack > 1)
                                i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(stack - 1);
                            else
                            {
                                i->getSource()->RemoveAurasDueToSpell(SPELL_SANITY);
                                DoCast(i->getSource(), SPELL_INSANE);
                            }*/
                        }
                    }
                }
            }
        }
    }

    Creature* SelectRandomGuardian(float fRange)
    {
        std::list<Creature* > lGuardiansList;
        GetCreatureListWithEntryInGrid(lGuardiansList, m_creature, MOB_IMMORTAL_GUARDIAN, fRange);

        //This should not appear!
        if (lGuardiansList.empty()){
            empoweringShadowsTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lGuardiansList.begin();
        advance(iter, urand(0, lGuardiansList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(isPhase3 || isPhase2)
        {
            if (sanityTimer < uiDiff)
            {
                DoCastSanity();
                DoCast(m_creature, SPELL_SANITY);
                sanityTimer = 20000;
            }else sanityTimer -= uiDiff;
        }

        if(isPhase3)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_YOGGSARON, FAIL);
                Reset();
                return;
            }

            if (summonTimer < uiDiff)
            {
                uint8 i = urand(0, 11);
                if(Creature *pTemp = m_creature->SummonCreature(MOB_IMMORTAL_GUARDIAN, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                        pTemp->SetInCombatWithZone();
                        m_lGuardGUIDList.push_back(pTemp->GetGUID());
                    }
                }
                summonTimer = 40000;
            }else summonTimer -= uiDiff;

            if (lunaticGazaTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_LUNATIC_GAZE_YOGG);
                lunaticGazaTimer = 30000;
            }else lunaticGazaTimer -= uiDiff;

            if (empoweringShadowsTimer < uiDiff)
            {
                if (pGuardian = SelectRandomGuardian(80.0f))
                    DoCast(pGuardian, m_bIsRegularMode ? SPELL_EMPOWERING_SHADOWS : SPELL_EMPOWERING_SHADOWS_H);
                empoweringShadowsTimer = 60000;
            }else empoweringShadowsTimer -= uiDiff;

            if (deafeningRoarTimer < uiDiff)
            {
                if(!m_bIsRegularMode && m_uiKeepersAlive < 4)
                    DoCast(m_creature, SPELL_DEAFENING_ROAR);
                deafeningRoarTimer = 30000;
            }else deafeningRoarTimer -= uiDiff;

            if (berserkTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_BERSERK);
                berserkTimer = 300000;
            }else berserkTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

struct MANGOS_DLL_DECL mob_madness_portalAI : public ScriptedAI
{
    mob_madness_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint8 m_uiDestination;
    uint32 m_uiCheckTimer;

    bool m_bHasTeleported;

    void Reset()
    {
        m_uiDestination = 10;
        m_uiCheckTimer = 500;
        m_bHasTeleported = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_VISION1) == IN_PROGRESS)
                m_uiDestination = 0;

            if(m_pInstance->GetData(TYPE_VISION2) == IN_PROGRESS)
                m_uiDestination = 1;

            if(m_pInstance->GetData(TYPE_VISION3) == IN_PROGRESS)
                m_uiDestination = 2;
        }

        if(m_creature->GetPositionZ() < 245.0f)
            m_uiDestination = 3;

        if(m_uiCheckTimer < uiDiff && !m_bHasTeleported)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                    {
                        i->getSource()->TeleportTo(i->getSource()->GetMapId(), TeleportLoc[m_uiDestination].x, TeleportLoc[m_uiDestination].y, TeleportLoc[m_uiDestination].z, i->getSource()->GetOrientation());
                        if(m_uiDestination < 3)
                        {
                            //m_creature->ForcedDespawn();
                            //m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            m_bHasTeleported = true;
                            m_creature->SetVisibility(VISIBILITY_OFF);
                        }
                    }
                }
            } 
            m_uiCheckTimer = 500;
        }else m_uiCheckTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_brain_of_yogg_saronAI : public ScriptedAI
{
    boss_brain_of_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 berserkTimer;
    uint8 vision;
    uint8 m_uiVisionPhase;
    uint32 m_uiSpeechTimer;
    uint32 madnessTimer;
    uint32 tentacleCheckTimer;
    bool isStormWindVision;
    bool isDragonVision;
    bool isLichKingVision;

    bool isOver;
    bool isVisionFinished;

    uint64 m_uiLichKingGUID;
    uint64 m_uiChampionGUID;
    uint64 m_uiGaronaGUID;
    uint64 m_uiKingLlaneGUID;
    uint64 m_uiNeltharionGUID;
    uint64 m_uiMalygosGUID;
    uint64 m_uiYseraGUID;
    uint64 m_uiVoiceOfYoggGUID;

    std::list<Creature*> lPortals;
    std::list<uint64> PlayerList;

    void Reset()
    {
        isStormWindVision   = false;
        isDragonVision      = false;
        isLichKingVision    = false;
        isOver              = false;

        m_uiLichKingGUID    = 0;
        m_uiChampionGUID    = 0;
        m_uiGaronaGUID      = 0;
        m_uiKingLlaneGUID   = 0;
        m_uiNeltharionGUID  = 0;
        m_uiMalygosGUID     = 0;
        m_uiYseraGUID       = 0;
        m_uiVoiceOfYoggGUID = 0;

        m_uiVisionPhase     = 0;
        m_uiSpeechTimer     = 1000;
        m_creature->SetHealth(m_creature->GetMaxHealth());

        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_VISION1, NOT_STARTED);
            m_pInstance->SetData(TYPE_VISION2, NOT_STARTED);
            m_pInstance->SetData(TYPE_VISION3, NOT_STARTED);
            m_pInstance->SetData(TYPE_YOGG_BRAIN, NOT_STARTED);
        }

        lPortals.clear();
        PlayerList.clear();
    }

    void Aggro(Unit *who) 
    {
        // fix how the creature looks! it should be positioned upper, but the melee can still reach him. Strange...
        //StartVisions(2, 600000);    //for debug only!
        //m_creature->GetMotionMaster()->MoveIdle();
        //m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 255.011f, 0.0f);
        //m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 255.011f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // player list
        if (m_creature->IsHostileTo(pWho) && !ListContains(PlayerList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80, true) && m_creature->IsWithinLOSInMap(pWho))
            PlayerList.push_back(pWho->GetGUID());
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void KillPlayers()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && m_creature->IsWithinLOSInMap(i->getSource()) && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 200.0f)
                    //i->getSource()->TeleportTo(pTemp->GetMapId(), TeleportLoc[3].x, TeleportLoc[3].y, TeleportLoc[3].z, pTemp->GetOrientation());
                    i->getSource()->DealDamage(i->getSource(), i->getSource()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        } 
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30)
            uiDamage = 0;

        if(uiDamage > 0 && m_creature->IsWithinDistInMap(done_by, 35))
        {
            if (Creature* pYogg = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGGSARON))))
            {
                if(pYogg->isAlive())
                    ((boss_yogg_saronAI*)pYogg->AI())->CastShatterIllusion();
            }
        }
    }

    void SummonPortals()
    {
        for(uint8 i = 0; i < 3; i++)
            m_creature->SummonCreature(MOB_DESCEND_INTO_MADNESS, MadnessPortalLoc[i].x, MadnessPortalLoc[i].y, MadnessPortalLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
    }

    void StartVisions(uint8 v, uint32 bsrkTimer)
    {
        berserkTimer = bsrkTimer;
        vision = v;
        PlayerList.clear();
        DoCast(m_creature, SPELL_INDUCE_MADNESS);
        m_uiVisionPhase     = 0;
        m_uiSpeechTimer     = 1000;
        tentacleCheckTimer  = 1000;
        madnessTimer        = 60000;
        isVisionFinished    = false;
        switch(vision)
        {
        case 1:
            isStormWindVision = true;
            break;
        case 2:
            isDragonVision = true;
            break;
        case 3:
            isLichKingVision = true;
            break;
        }
    }

    bool IsThereAnyAdd(WorldObject *source)
    {
        if(GetClosestCreatureWithEntry(source, MOB_VISION_TENTACLE, 80.0f))
            return true;
        
        if(m_pInstance)
        {
            switch(vision)
            {
                    // sw keep vision
            case 1:
                if(GameObject* pVisionDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_BRAIN_DOOR3)))
                    m_pInstance->DoUseDoorOrButton(pVisionDoor->GetGUID());
                    break;
                    // dragons vision
            case 2:
                if(GameObject* pVisionDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_BRAIN_DOOR1)))
                    m_pInstance->DoUseDoorOrButton(pVisionDoor->GetGUID());
                break;
                    // lich king vision
            case 3:
                if(GameObject* pVisionDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_BRAIN_DOOR2)))
                    m_pInstance->DoUseDoorOrButton(pVisionDoor->GetGUID());
                break;
            }
        }
        return false;
    }

    void SelfDestruct()
    {
        m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Stormwind keep vision
        if(isStormWindVision)
        {
            if(m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiVisionPhase)
                {
                case 0:
                    if(Creature* Garona = m_creature->SummonCreature(NPC_GARONA, PosGarona[0], PosGarona[1], PosGarona[2], PosGarona[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiGaronaGUID = Garona->GetGUID();
                    if(Creature* KingLlane = m_creature->SummonCreature(NPC_KING_LLANE, PosKing[0], PosKing[1], PosKing[2], PosKing[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiKingLlaneGUID = KingLlane->GetGUID();
                    if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceStormwind[0], PosVoiceStormwind[1], PosVoiceStormwind[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                    {
                        m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                        VoiceOfYogg->SetVisibility(VISIBILITY_OFF);
                    }
                    for(uint8 i = 0; i < 8; i++)
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, KeepLoc[i].x, KeepLoc[i].y, KeepLoc[i].z, KeepLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                        {
                            pTemp->SetDisplayId(28621);
                            pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                            pTemp->setFaction(7); 
                            pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                        }
                    }
                    for(uint8 i = 0; i < 3; i++)
                    {
                        m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullKeepLoc[i].x, SkullKeepLoc[i].y, SkullKeepLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 1000;
                    break;
                case 1:
                    if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                        DoScriptText(SAY_GARONA1, Garona);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 12000;
                    break;
                case 2:
                    if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                        DoScriptText(SAY_GARONA2, Garona);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 12000;
                    break;
                case 3:
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V1_1, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 4000;
                    break;
                case 4:
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V1_2, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 4000;
                    break;
                case 5:
                    if(Creature* KingLlane = m_pInstance->instance->GetCreature(m_uiKingLlaneGUID))
                        DoScriptText(SAY_GARONA3, KingLlane);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 6000;
                    break;
                case 6:
                    if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                        Garona->GetMotionMaster()->MovePoint(0, 1931.348f, 61.033f, 241.709f);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 6000;
                    break;
                case 7:
                    if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                    {
                        DoScriptText(SAY_GARONA4, Garona);
                        if(Creature* KingLlane = m_pInstance->instance->GetCreature(m_uiKingLlaneGUID))
                        {
                            KingLlane->SetStandState(UNIT_STAND_STATE_DEAD);
                            KingLlane->SetHealth(0);
                            Garona->Attack(KingLlane, true);
                        }
                    }
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 6000;
                    break;
                case 8:
                    if(Creature* Garona = m_pInstance->instance->GetCreature(m_uiGaronaGUID))
                        Garona->AttackStop();
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V1_3, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 8000;
                    break;
                default:
                    m_uiSpeechTimer = 100000;
                }
            }else m_uiSpeechTimer -= uiDiff;

            if (tentacleCheckTimer < uiDiff && !isVisionFinished)
            {
                if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                {
                    if(!IsThereAnyAdd(VoiceOfYogg))
                    {
                        if(m_pInstance) 
                            m_pInstance->SetData(TYPE_VISION1, DONE);

                        SummonPortals();
                        tentacleCheckTimer = 300000;
                    }
                    else
                        tentacleCheckTimer = 500;
                }
                else
                    tentacleCheckTimer = 500;
            }else tentacleCheckTimer -= uiDiff;

            if (madnessTimer < uiDiff)
            {
                isVisionFinished = true;
                isStormWindVision = false;
                KillPlayers();
                madnessTimer = 300000;
            }else madnessTimer -= uiDiff;
        }
        // Dragon Soul Vision
        if(isDragonVision)
        {
            if(m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiVisionPhase)
                {
                case 0:
                    m_creature->SummonCreature(NPC_ALEXSTRASZA, PosAlexstrasza[0], PosAlexstrasza[1], PosAlexstrasza[2], PosAlexstrasza[3], TEMPSUMMON_TIMED_DESPAWN, 60000);
                    if(Creature* Neltharion = m_creature->SummonCreature(NPC_NELTHARION, PosNeltharion[0], PosNeltharion[1], PosNeltharion[2], PosNeltharion[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiNeltharionGUID = Neltharion->GetGUID();
                    if(Creature* Malygos = m_creature->SummonCreature(NPC_MALYGOS, PosMalygos[0], PosMalygos[1], PosMalygos[2], PosMalygos[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiMalygosGUID = Malygos->GetGUID();
                    if(Creature* Ysera = m_creature->SummonCreature(NPC_YSERA, PosYsera[0], PosYsera[1], PosYsera[2], PosYsera[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiYseraGUID = Ysera->GetGUID();
                    //m_creature->SummonGameobject(GO_DRAGON_SOUL, PosVoiceDragon[0], PosVoiceDragon[1], PosVoiceDragon[2], 0, 60000);
                    if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceDragon[0], PosVoiceDragon[1], PosVoiceDragon[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                    {
                        m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                        VoiceOfYogg->SetVisibility(VISIBILITY_OFF);
                    }
                    for(uint8 i = 0; i < 10; i++)
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, DragonLoc[i].x, DragonLoc[i].y, DragonLoc[i].z, DragonLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                        {
                            pTemp->SetDisplayId(DisplayDragons[i]);
                            pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                            pTemp->setFaction(7);
                            pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                        }
                    }
                    for(uint8 i = 0; i < 3; i++)
                    {
                        m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullDragonLoc[i].x, SkullDragonLoc[i].y, SkullDragonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 1000;
                    break;
                case 1:
                    if(Creature* Neltharion = m_pInstance->instance->GetCreature(m_uiNeltharionGUID))
                        DoScriptText(SAY_NELTHARION1, Neltharion);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 10000;
                    break;
                case 2:
                    if(Creature* Ysera = m_pInstance->instance->GetCreature(m_uiYseraGUID))
                        DoScriptText(SAY_YSERA, Ysera);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 7000;
                    break;
                case 3:
                    if(Creature* Neltharion = m_pInstance->instance->GetCreature(m_uiNeltharionGUID))
                        DoScriptText(SAY_NELTHARION2, Neltharion);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 6000;
                    break;
                case 4:
                    if(Creature* Malygos = m_pInstance->instance->GetCreature(m_uiMalygosGUID))
                        DoScriptText(SAY_MALYGOS, Malygos);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 9000;
                    break;
                case 5:
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V2, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 10000;
                    break;
                default:
                    m_uiSpeechTimer = 100000;
                }
            }else m_uiSpeechTimer -= uiDiff;

            if (tentacleCheckTimer < uiDiff && !isVisionFinished)
            {
                if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                {
                    if(!IsThereAnyAdd(VoiceOfYogg))
                    {
                        if(m_pInstance) 
                            m_pInstance->SetData(TYPE_VISION2, DONE);

                        SummonPortals();
                        tentacleCheckTimer = 300000;
                    }
                    else
                        tentacleCheckTimer = 500;
                }
                else
                    tentacleCheckTimer = 500;
            }else tentacleCheckTimer -= uiDiff;

            if (madnessTimer < uiDiff)
            {
                isVisionFinished = true;
                isDragonVision = false;
                KillPlayers();
                madnessTimer = 300000;
            }else madnessTimer -= uiDiff;
        }
        // Lich king vision
        if(isLichKingVision)
        {
            if(m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiVisionPhase)
                {
                case 0:
                    if(Creature* LichKing = m_creature->SummonCreature(NPC_LICH_KING, PosLichKing[0], PosLichKing[1], PosLichKing[2], PosLichKing[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiLichKingGUID = LichKing->GetGUID();
                    if(Creature* Champion = m_creature->SummonCreature(NPC_IMMOLATED_CHAMPION, PosChampion[0], PosChampion[1], PosChampion[2], PosChampion[3], TEMPSUMMON_TIMED_DESPAWN, 60000))
                        m_uiChampionGUID = Champion->GetGUID();
                    if(Creature* VoiceOfYogg = m_creature->SummonCreature(NPC_VOICE_OF_YOGG_SARON, PosVoiceIcecrown[0], PosVoiceIcecrown[1], PosVoiceIcecrown[2], 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                    {
                        m_uiVoiceOfYoggGUID = VoiceOfYogg->GetGUID();
                        VoiceOfYogg->SetVisibility(VISIBILITY_OFF);
                    }
                    for(uint8 i = 0; i < 9; i++)
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_VISION_TENTACLE, IcecrownLoc[i].x, IcecrownLoc[i].y, IcecrownLoc[i].z, IcecrownLoc[i].o, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 80000))
                        {
                            pTemp->SetMaxHealth(m_bIsRegularMode ? 15000 : 40000);
                            pTemp->SetDisplayId(25627);
                            pTemp->setFaction(7);
                            pTemp->CastSpell(pTemp, SPELL_GRIM_REPRISAL, false);
                        }
                    }
                    for(uint8 i = 0; i < 4; i++)
                    {
                        m_creature->SummonCreature(NPC_LAUGHING_SKULL, SkullIcecrownLoc[i].x, SkullIcecrownLoc[i].y, SkullIcecrownLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 1000;
                    break;
                case 1:
                    if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                    {
                        Champion->SetStandFlags(UNIT_STAND_STATE_KNEEL);
                        if(Creature* LichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID))
                        {
                            LichKing->CastSpell(Champion, 54142, false);
                            DoScriptText(SAY_LICH_KING1, LichKing);
                        }
                    }
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 5000;
                    break;
                case 2:
                    if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                        DoScriptText(SAY_CHAMPION1, Champion);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 8000;
                    break;
                case 3:
                    if(Creature* Champion = m_pInstance->instance->GetCreature(m_uiChampionGUID))
                        DoScriptText(SAY_CHAMPION2, Champion);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 8000;
                    break;
                case 4:
                    if(Creature* LichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID))
                        DoScriptText(SAY_LICH_KING2, LichKing);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 7000;
                    break;
                case 5:
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V3_1, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 5000;
                    break;
                case 6:
                    if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                        DoScriptText(SAY_YOGG_V3_2, VoiceOfYogg);
                    ++m_uiVisionPhase;
                    m_uiSpeechTimer = 10000;
                    break;
                default:
                    m_uiSpeechTimer = 100000;
                }
            }else m_uiSpeechTimer -= uiDiff;

            if (tentacleCheckTimer < uiDiff && !isVisionFinished)
            {
                if(Creature* VoiceOfYogg = m_pInstance->instance->GetCreature(m_uiVoiceOfYoggGUID))
                {
                    if(!IsThereAnyAdd(VoiceOfYogg))
                    {
                        if(m_pInstance) 
                            m_pInstance->SetData(TYPE_VISION3, DONE);

                        SummonPortals();
                        tentacleCheckTimer = 300000;
                    }
                    else
                        tentacleCheckTimer = 500;
                }
                else
                    tentacleCheckTimer = 500;
            }else tentacleCheckTimer -= uiDiff;

            if (madnessTimer < uiDiff)
            {
                isVisionFinished = true;
                isLichKingVision = false;
                KillPlayers();
                madnessTimer = 300000;
            }else madnessTimer -= uiDiff;
        }

        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30 && !isOver)
        {
            if (Creature* pYogg = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGGSARON))))
            {
                pYogg->setFaction(14);
                pYogg->SetVisibility(VISIBILITY_ON);
                ((boss_yogg_saronAI*)pYogg->AI())->StartThirdPhase(berserkTimer - uiDiff);
                isOver = true;
            }
            if(m_pInstance) 
                m_pInstance->SetData(TYPE_YOGG_BRAIN, DONE);
        }

        if (berserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            berserkTimer = 300000;
        }else berserkTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL boss_saraAI : public ScriptedAI
{
    boss_saraAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->ApplySpellImmune(SPELL_DARK_VOLLEY, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, true);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint8 phase;
    uint32 berserkTimer;

    // intro
    bool m_bIsIntro;
    uint8 m_uiIntro_Phase;
    uint32 m_uiSpeech_Timer;
    bool isIntroDone;
    bool isFactionSet;

    // phase 1
    uint32 phaseYellTimer;
    uint32 summonTimer;
    uint32 sarasFervorTimer;
    uint32 sarasBlessingTimer;
    uint32 sarasAngerTimer;
    uint32 guardCheckTimer;
    Creature *pGuardian;
    std::list<Creature*> lGuardianList;
    std::list<uint64> m_lGuardGUIDList;
    std::list<uint64> m_lTentacleGUIDList;
    std::list<Creature*> lPortals;

    // transition
    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;

    // phase 2
    uint32 sanityTimer;
    uint32 psychosisTimer;
    uint32 maladyTimer;
    uint32 brainLinkTimer;
    uint32 deathRayTimer;
    uint32 descentTimer;
    uint32 waitTimer;
    uint8 vision;
    uint8 maxPlayers;

    std::list<uint64> FriendlyList;
    std::list<uint64> PlayerList;

    void Reset()
    {
        // intro
        phase               = 0;
        berserkTimer        = 900000;   // 15 min
        m_bIsIntro          = false;
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        isIntroDone         = false;
        isFactionSet        = false;

        // phase 1
        phaseYellTimer      = 30000;
        summonTimer         = 10000 + urand (1000, 5000);
        sarasFervorTimer    = urand(10000, 15000);
        sarasBlessingTimer  = urand(15000, 20000);
        sarasAngerTimer     = urand(20000, 25000);
        guardCheckTimer     = 1000;

        // transition
        isOutro             = false;
        OutroTimer          = 10000;
        Step                = 1;

        // phase 2
        sanityTimer         = 10000;
        psychosisTimer      = 15000;     
        maladyTimer         = 20000;
        brainLinkTimer      = 25000;
        deathRayTimer       = 30000;
        descentTimer        = 90000;
        vision              = 0;
        lPortals.clear();
        maxPlayers          = m_bIsRegularMode ? 4 : 10;

        FriendlyList.clear();
        PlayerList.clear();
        m_lGuardGUIDList.clear();
        m_lTentacleGUIDList.clear();

        if(m_creature->HasAura(SPELL_SHADOWY_BARRIER))
            m_creature->RemoveAurasDueToSpell(SPELL_SHADOWY_BARRIER);

        m_creature->SetDisplayId(29117);
        m_creature->setFaction(35);
        m_creature->SetMaxHealth(199999);
        m_creature->SetHealth(m_creature->GetMaxHealth());


        std::list<Creature*> lAddsList;
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, MOB_IMMORTAL_GUARDIAN, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, MOB_GUARDIAN_OF_YOGG, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, MOB_CONSTRICTOR_TENTACLE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, MOB_CRUSHER_TENTACLE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, MOB_CORRUPTOR_TENTACLE, DEFAULT_VISIBILITY_INSTANCE);
        if (!lAddsList.empty())
        {
            for(std::list<Creature*>::iterator iter = lAddsList.begin(); iter != lAddsList.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_YOGGSARON, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    { }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) m_pInstance->SetData(TYPE_YOGGSARON, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(phase == 2)
        {
            if(irand(0,1))
                DoScriptText(SAY_KILL1, m_creature);
            else
                DoScriptText(SAY_KILL2, m_creature);
        }
    }

    void SummonPortals()
    {
        uint8 maxPortals = m_bIsRegularMode ? 4 : 10;
        for(uint8 i = 0; i < maxPortals; i++)
            m_creature->SummonCreature(MOB_DESCEND_INTO_MADNESS, YoggPortalLoc[i].x, YoggPortalLoc[i].y, YoggPortalLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
    }

    void SetDestination(uint8 location)
    {
        switch (location)
        {
            case 0:
                if(m_pInstance)
                    m_pInstance->SetData(TYPE_VISION1, IN_PROGRESS);
                break;
            case 1:
                if(m_pInstance)
                    m_pInstance->SetData(TYPE_VISION2, IN_PROGRESS);
                break;
            case 2:
                if(m_pInstance)
                    m_pInstance->SetData(TYPE_VISION3, IN_PROGRESS);
                break;
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            isOutro = true;
            m_creature->SetHealth(m_creature->GetMaxHealth());
            uiDamage = 0;
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        uint32 spellId = m_bIsRegularMode ? SPELL_SHADOW_NOVA : SPELL_SHADOW_NOVA_H;
        uint32 maxHealth = m_bIsRegularMode ? 21994 : 27500;
        if(spell->Id == spellId && caster->GetEntry() == MOB_GUARDIAN_OF_YOGG)
        {
            if(m_creature->GetHealth() > maxHealth)
                m_creature->DealDamage(m_creature, m_bIsRegularMode ? urand(20000, 21994) : urand(25000, 27500), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
            else
            {
                isOutro = true;
                m_creature->SetHealth(m_creature->GetMaxHealth());
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && phase == 0 &&
            !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 110) && m_creature->IsWithinLOSInMap(pWho))
            m_bIsIntro = true;

        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && phase == 0 &&
            isIntroDone && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40) && m_creature->IsWithinLOSInMap(pWho))
        {
            if(m_pInstance) 
                m_pInstance->SetData(TYPE_YOGGSARON, IN_PROGRESS);

            DoScriptText(SAY_AGGRO, m_creature);
            phase = 1;
        }

        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80, true) && phase == 1)
            FriendlyList.push_back(pWho->GetGUID());

        // player list
        if (m_creature->IsHostileTo(pWho) && !ListContains(PlayerList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80, true) && m_creature->IsWithinLOSInMap(pWho) && phase == 2)
            PlayerList.push_back(pWho->GetGUID());

        if(phase == 1)
        {
            if(pWho->getFaction() == 1 || pWho->getFaction() == 3 || pWho->getFaction() == 4 || pWho->getFaction() == 115 || pWho->getFaction() == 1629
                && phase == 1 && !isOutro && !isFactionSet)
            {
                isFactionSet = true;
                m_creature->setFaction(1);
            }

            if(pWho->getFaction() == 2 || pWho->getFaction() == 5 || pWho->getFaction() == 6 || pWho->getFaction() == 116 || pWho->getFaction() == 1610
                && phase == 1 && !isOutro && !isFactionSet)
            {
                isFactionSet = true;
                m_creature->setFaction(2);
            }
        }
        else if(phase == 2 || phase == 3)
            m_creature->setFaction(14);
    }

    void EventReset()
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_YOGGSARON, FAIL);

        if (Creature* pYogg = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGGSARON))))
        {
            if(pYogg->isAlive())
                ((boss_yogg_saronAI*)pYogg->AI())->Reset();
        }
        if (Creature* pYoggBrain = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGG_BRAIN))))
        {
            if(pYoggBrain->isAlive())
                ((boss_brain_of_yogg_saronAI*)pYoggBrain->AI())->Reset();
        }
        Reset();
    }

    Creature* SelectRandomGuardian(float fRange)
    {
        std::list<Creature* > lGuardiansList;
        GetCreatureListWithEntryInGrid(lGuardiansList, m_creature, MOB_GUARDIAN_OF_YOGG, fRange);

        //This should not appear!
        if (lGuardiansList.empty()){
            sarasAngerTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lGuardiansList.begin();
        advance(iter, urand(0, lGuardiansList.size()-1));

        return *iter;
    }

    uint64 SelectRandomPlayer(std::list<uint64> UnitList)
    {
        //This should not appear!
        if (UnitList.empty())
        {
            sarasFervorTimer = 30000;
            sarasBlessingTimer = 30000;
            return NULL;
        }

        std::list<uint64>::iterator iter = UnitList.begin();
        advance(iter, urand(0, UnitList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void SummonFirstTentacles()
    {
        uint8 i = urand(0, 11);
        if(Creature *pTemp = m_creature->SummonCreature(MOB_CONSTRICTOR_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                pTemp->AddThreat(pTarget,1000.0f);
                pTemp->AI()->AttackStart(pTarget);
                m_lTentacleGUIDList.push_back(pTemp->GetGUID());
            }
        }
        i = urand(0, 11);
        if(Creature *pTemp = m_creature->SummonCreature(MOB_CRUSHER_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                pTemp->AddThreat(pTarget,1000.0f);
                pTemp->AI()->AttackStart(pTarget);
                m_lTentacleGUIDList.push_back(pTemp->GetGUID());
            }
        }
        i = urand(0, 11);
        if(Creature *pTemp = m_creature->SummonCreature(MOB_CORRUPTOR_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                pTemp->AddThreat(pTarget,1000.0f);
                pTemp->AI()->AttackStart(pTarget);
                m_lTentacleGUIDList.push_back(pTemp->GetGUID());
            }
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
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    m_creature->GetMotionMaster()->MoveIdle();
                    SetCombatMovement(false);
                    m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, 5.9f);
                    m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 329.397f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 8000;
                    break;
                case 1:
                    // m_creature->setFaction(14);
                    DoScriptText(SAY_INTRO2, m_creature);
                    DoCast(m_creature, SPELL_DAMAGE_IMMUNE);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 8000;
                    break;
                case 2:
                    m_bIsIntro = false;
                    isIntroDone = true;
                    m_uiSpeech_Timer = 12000;
                    break;
                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }

        if(phase == 1)
        {
            if(!isOutro)
            {
                if (Creature* pYogg = GetClosestCreatureWithEntry(m_creature, NPC_YOGGSARON, 150.0f))
                {
                    if(!pYogg->getVictim() || !pYogg->SelectHostileTarget())
                    {
                        EventReset();
                        DoScriptText(SAY_WIPE, m_creature);
                    }
                }

                if (phaseYellTimer < uiDiff)
                {
                    switch(urand(0, 1))
                    {
                    case 0: DoScriptText(SAY_HELP1, m_creature); break;
                    case 1: DoScriptText(SAY_HELP2, m_creature); break;
                    }
                    phaseYellTimer = 30000;
                }else phaseYellTimer -= uiDiff;

                if (summonTimer < uiDiff)
                {
                    uint8 i = urand(0, 11);
                    if(Creature *pTemp = m_creature->SummonCreature(MOB_GUARDIAN_OF_YOGG, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000)) 
                    {
                        Unit *pPlayer = Unit::GetUnit((*m_creature),(SelectRandomPlayer(FriendlyList)));
                        if (pPlayer && pPlayer->isAlive() && m_creature->GetDistance(pPlayer) < 100)
                        {
                            pTemp->AddThreat(pPlayer,100.0f);
                            pTemp->AI()->AttackStart(pPlayer);
                            pTemp->SetInCombatWithZone();
                            m_lGuardGUIDList.push_back(pTemp->GetGUID());
                        }
                    }
                    summonTimer = urand(15000, 20000);
                }else summonTimer -= uiDiff;

                if (sarasFervorTimer < uiDiff)
                {
                    Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer(FriendlyList)));
                    if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 100)
                        DoCast(pTemp, SPELL_SARAS_FERVOR);
                    sarasFervorTimer = urand(20000, 30000);
                }else sarasFervorTimer -= uiDiff;

                if (sarasBlessingTimer < uiDiff)
                {
                    Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer(FriendlyList)));
                    if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 100)
                        DoCast(pTemp, SPELL_SARAS_BLESSING); //remove for debug!
                        sarasBlessingTimer = urand(20000, 30000);
                }else sarasBlessingTimer -= uiDiff;

                if (sarasAngerTimer < uiDiff)
                {
                    if (pGuardian = SelectRandomGuardian(80.0f))
                        DoCast(pGuardian, SPELL_SARAS_ANGER);
                    sarasAngerTimer = urand(20000, 30000);
                }else sarasAngerTimer -= uiDiff;
            }
            if(isOutro)
            {
                switch(Step)
                {
                case 1:
                    m_creature->SetHealth(m_creature->GetMaxHealth());
                    //m_creature->RemoveAllAuras();
                    //m_creature->DeleteThreatList();
                    m_creature->CombatStop(true);
                    m_creature->InterruptNonMeleeSpells(false);
                    ++Step;
                    OutroTimer = 5000;
                    break;
                case 3:
                    DoScriptText(SAY_PHASE2, m_creature);
                    ++Step;
                    OutroTimer = 15000;
                    break;
                case 5:
                    Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer(FriendlyList)));
                    if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 100)
                    {
                        m_creature->AddThreat(pTemp, 100.0f);
                        m_creature->AI()->AttackStart(pTemp);
                    }
                    m_creature->setFaction(14);
                    m_creature->SetDisplayId(24991);
                    DoCast(m_creature, SPELL_SHADOWY_BARRIER);
                    m_creature->RemoveAurasDueToSpell(SPELL_DAMAGE_IMMUNE);
                    m_creature->SetHealth(m_creature->GetMaxHealth());
                    m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  10, 5.9f);
                    m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                    if (Creature* pYogg = GetClosestCreatureWithEntry(m_creature, NPC_YOGGSARON, 150.0f))
                    {
                        pYogg->CastSpell(pYogg, SPELL_SHADOWY_BARRIER_YOGG, false);
                        ((boss_yogg_saronAI*)pYogg->AI())->StartSecondPhase();
                    }
                    SummonFirstTentacles();
                    phaseYellTimer = 30000 + urand(5000, 10000);
                    summonTimer = 10000;
                    waitTimer = 20000;
                    phase = 2;
                    ++Step;
                    OutroTimer = 3000;
                    break;
                }
            }
            else return;

            if (OutroTimer <= uiDiff)
            {
                ++Step;
                OutroTimer = 330000;
            } OutroTimer -= uiDiff;
        }
        if(phase == 2)
        {    
            if (Creature* pYogg = GetClosestCreatureWithEntry(m_creature, NPC_YOGGSARON, 150.0f))
            {
                if(!pYogg->getVictim() || !pYogg->SelectHostileTarget())
                    EventReset();
            }

            if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == FAIL))
                EventReset();

            if (phaseYellTimer < uiDiff)
            {
                switch(urand(0, 1))
                {
                case 0: DoScriptText(SAY_SPECIAL1, m_creature); break;
                case 1: DoScriptText(SAY_SPECIAL2, m_creature); break;
                }
                phaseYellTimer = 30000 + urand(5000, 10000);
            }else phaseYellTimer -= uiDiff;

            if (psychosisTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_PHYCHOSIS : SPELL_PHYCHOSIS_H);
                psychosisTimer = urand(15000, 20000);
            }else psychosisTimer -= uiDiff;

            if (maladyTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_MALADY_OF_THE_MIND);
                maladyTimer = urand(20000, 25000);
            }else maladyTimer -= uiDiff;

            if (brainLinkTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_BRAIN_LINK);
                brainLinkTimer = urand(25000, 30000);
            }else brainLinkTimer -= uiDiff;

            if (deathRayTimer < uiDiff)
            {
                uint8 i = urand(0, 7);
                for(uint8 j = 0; j < i+4; j++)
                    m_creature->SummonCreature(MOB_DEATH_ORB, SummonLoc[j].x, SummonLoc[j].y, SummonLoc[j].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                DoCast(m_creature, SPELL_DEATH_RAY);
                deathRayTimer = 30000;
            }else deathRayTimer -= uiDiff;

            if (descentTimer < uiDiff)
            {
                if(vision < 3)
                {
                    if (Creature* pYogg = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGGSARON))))
                        DoScriptText(SAY_VISION, pYogg);
					/*
                    for (uint8 i = 0; i < maxPlayers; i++)
                    {
                        Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer(PlayerList)));
                        if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 100)
                            ((Player*)(pTemp))->TeleportTo(pTemp->GetMapId(), TeleportLoc[vision].x, TeleportLoc[vision].y, TeleportLoc[vision].z, pTemp->GetOrientation());
                    }
                    */
                    SummonPortals();
                    SetDestination(vision);
                    if (Creature* pYoggBrain = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGG_BRAIN))))
                    {
                        if(pYoggBrain->isAlive())
                            ((boss_brain_of_yogg_saronAI*)pYoggBrain->AI())->StartVisions(vision + 1, berserkTimer - uiDiff);
                        vision += 1;
                    }

                }
                else
                    berserkTimer = 1000;
                descentTimer = 90000;
            }else descentTimer -= uiDiff;

            if (summonTimer < uiDiff)
            {
                uint8 i = urand(0, 11);
                switch(urand(0, 4))
                {
                case 0:
                case 1:
                    if(Creature *pTemp = m_creature->SummonCreature(MOB_CONSTRICTOR_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTemp->AddThreat(pTarget,1000.0f);
                            pTemp->AI()->AttackStart(pTarget);
                            m_lTentacleGUIDList.push_back(pTemp->GetGUID());
                        }
                    }
                    break;
                case 2:
                    if(!GetClosestCreatureWithEntry(m_creature, MOB_CRUSHER_TENTACLE, 100.0f))
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_CRUSHER_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pTemp->AddThreat(pTarget,1000.0f);
                                pTemp->AI()->AttackStart(pTarget);
                                m_lTentacleGUIDList.push_back(pTemp->GetGUID());
                            }
                        }
                        break;
                    }
                case 3:
                case 4:
                    if(Creature *pTemp = m_creature->SummonCreature(MOB_CORRUPTOR_TENTACLE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTemp->AddThreat(pTarget,1000.0f);
                            pTemp->AI()->AttackStart(pTarget);
                            m_lTentacleGUIDList.push_back(pTemp->GetGUID());
                        }
                    }
                    break;
                }
                summonTimer = urand(10000, 15000);
            }else summonTimer -= uiDiff;

            if (berserkTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_BERSERK);
                berserkTimer = 300000;
            }else berserkTimer -= uiDiff;

            if(m_pInstance && m_pInstance->GetData(TYPE_YOGG_BRAIN == DONE))
            {
                m_creature->SetVisibility(VISIBILITY_OFF);
                waitTimer = 10000;
                phase = 3;
            }
        }
        if(phase == 3)
        {    
            if (Creature* pYogg = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGGSARON))))
            {
                if(!pYogg->getVictim() || !pYogg->SelectHostileTarget())
                    EventReset();
            }

            if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == DONE))
            {
                if (Creature* pYoggBrain = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_YOGG_BRAIN))))
                {
                    if(pYoggBrain->isAlive())
                        ((boss_brain_of_yogg_saronAI*)pYoggBrain->AI())->SelfDestruct();
                }
                m_creature->ForcedDespawn();
            }
        }
    }
};

/*
*   Keepers
*/
struct MANGOS_DLL_DECL keeper_hodirAI : public ScriptedAI
{
    keeper_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 hodirProtectivGazeTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        hodirProtectivGazeTimer = 30000;
        FriendlyList.clear();
        DoCast(m_creature, SPELL_FORTITUDE_OF_FROST);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            EnterEvadeMode();
            return NULL;
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == IN_PROGRESS))
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (hodirProtectivGazeTimer < uiDiff)
            {
                Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 100)
                {
                    DoCast(pTemp, SPELL_HODIRS_PROTECTIVE_GAZE);
                }
                hodirProtectivGazeTimer = 30000 + urand(10000,30000);
            }else hodirProtectivGazeTimer -= uiDiff;
        }

        if(m_pInstance && (m_pInstance->GetData(TYPE_YOGGSARON) == DONE || m_pInstance->GetData(TYPE_YOGGSARON) == FAIL))
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL keeper_freyaAI : public ScriptedAI
{
    keeper_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 sanityWellTimer;

    void Reset()
    {
        sanityWellTimer = 30000;
        DoCast(m_creature, SPELL_RESILIENCE_OF_NATURE);

        for(uint8 i = 0; i < 5; i++)
        {
            if(Creature* pSanityWell = m_creature->SummonCreature(MOB_SANITY_WELL, SanityWellLoc[i].x, SanityWellLoc[i].y, SanityWellLoc[i].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 30000))
                pSanityWell->CastSpell(pSanityWell, SPELL_SANITY_WELL_VISUAL, true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == IN_PROGRESS))
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        }

        if(m_pInstance && (m_pInstance->GetData(TYPE_YOGGSARON) == DONE || m_pInstance->GetData(TYPE_YOGGSARON) == FAIL))
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL keeper_thorimAI : public ScriptedAI
{
    keeper_thorimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 titanicStormTimer;
    Creature *pGuardian;
    std::list<Creature*> lGuardianList;


    void Reset()
    {
        titanicStormTimer = 30000;
        DoCast(m_creature, SPELL_FURY_OF_THE_STORM);
    }

    Creature* SelectRandomGuardian(float fRange)
    {
        std::list<Creature* > lGuardiansList;
        GetCreatureListWithEntryInGrid(lGuardiansList, m_creature, MOB_IMMORTAL_GUARDIAN, fRange);

        //This should not appear!
        if (lGuardiansList.empty()){
            titanicStormTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lGuardiansList.begin();
        advance(iter, urand(0, lGuardiansList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == IN_PROGRESS))
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (titanicStormTimer < uiDiff)
            {
                if (pGuardian = SelectRandomGuardian(80.0f))
                    DoCast(pGuardian, SPELL_TITANIC_STORM);
                titanicStormTimer = 30000 + urand(10000,30000);
            }else titanicStormTimer -= uiDiff;
        }

        if(m_pInstance && (m_pInstance->GetData(TYPE_YOGGSARON) == DONE || m_pInstance->GetData(TYPE_YOGGSARON) == FAIL))
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL keeper_mimironAI : public ScriptedAI
{
    keeper_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 destabilizationMatrixTimer;
    Creature *pTentacle;
    std::list<Creature*> lCrusherTentacleList;
    std::list<Creature*> lCorruptorTentacleList;

    void Reset()
    {
        destabilizationMatrixTimer = 30000;
        DoCast(m_creature, SPELL_SPEED_OF_INVENTION);
    }

    Creature* SelectRandomCrusherTentacle(float fRange)
    {
        std::list<Creature* > lCrusherTentacleList;
        GetCreatureListWithEntryInGrid(lCrusherTentacleList, m_creature, MOB_CRUSHER_TENTACLE, fRange);

        //This should not appear!
        if (lCrusherTentacleList.empty()){
            destabilizationMatrixTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lCrusherTentacleList.begin();
        advance(iter, urand(0, lCrusherTentacleList.size()-1));

        return *iter;
    }

    Creature* SelectRandomCorruptorTentacle(float fRange)
    {
        std::list<Creature* > lCorruptorTentacleList;
        GetCreatureListWithEntryInGrid(lCorruptorTentacleList, m_creature, MOB_CORRUPTOR_TENTACLE, fRange);

        //This should not appear!
        if (lCorruptorTentacleList.empty()){
            destabilizationMatrixTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lCorruptorTentacleList.begin();
        advance(iter, urand(0, lCorruptorTentacleList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON == IN_PROGRESS))
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (destabilizationMatrixTimer < uiDiff)
            {
                switch(urand(0, 1))
                {
                case 0: 
                    if(pTentacle = SelectRandomCorruptorTentacle(80.0f))
                        DoCast(pTentacle, SPELL_DESTABILIZATION_MATRIX);
                    break;
                case 1: 
                    if(pTentacle = SelectRandomCrusherTentacle(80.0f))
                        DoCast(pTentacle, SPELL_DESTABILIZATION_MATRIX);
                    break;
                }
                destabilizationMatrixTimer = 30000 + urand(10000,30000);
            }else destabilizationMatrixTimer -= uiDiff;
        }

        if(m_pInstance && (m_pInstance->GetData(TYPE_YOGGSARON) == DONE || m_pInstance->GetData(TYPE_YOGGSARON) == FAIL))
            m_creature->ForcedDespawn();
    }
};

/*
*   Guardians
*/
class MANGOS_DLL_DECL EmpoweredAura : public Aura
{
public:
    EmpoweredAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};
struct MANGOS_DLL_DECL mob_immortal_guardianAI : public ScriptedAI
{
    mob_immortal_guardianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        if(pCreature->isAlive())
        {
            sp = (SpellEntry *)GetSpellStore()->LookupEntry(SPELL_EMPOWERED);
            bp = 8;
            if(!pCreature->HasAura(SPELL_EMPOWERED, EFFECT_INDEX_0))
                pCreature->AddAura(new EmpoweredAura(sp, EFFECT_INDEX_0, &bp, pCreature, pCreature));
            pCreature->GetAura(SPELL_EMPOWERED, EFFECT_INDEX_0)->SetStackAmount(9);
        }
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    SpellEntry const *sp;
    int32 bp;

    uint32 healthCheckTimer;
    uint8 stack;
    uint8 hp;

    void Reset()
    {
        healthCheckTimer = 1000;
        stack = 0;
        hp = 90;
        m_creature->SetRespawnDelay(DAY);

        if(m_creature->isAlive())
        {
            if(!m_creature->HasAura(SPELL_EMPOWERED, EFFECT_INDEX_0))
                m_creature->AddAura(new EmpoweredAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
            m_creature->GetAura(SPELL_EMPOWERED, EFFECT_INDEX_0)->SetStackAmount(9);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 1)
            uiDamage = 0;
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 1)
        {
            if(spell->Id == SPELL_TITANIC_STORM && caster->GetEntry() == KEEPER_THORIM)
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit *who) 
    {
        if(!m_creature->HasAura(SPELL_EMPOWERED, EFFECT_INDEX_0))
            m_creature->AddAura(new EmpoweredAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
        m_creature->GetAura(SPELL_EMPOWERED, EFFECT_INDEX_0)->SetStackAmount(9);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(hp >= 10)
        {
            if (healthCheckTimer < uiDiff)
            {
                if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= hp)
                {
                    if(Aura *aura = m_creature->GetAura(SPELL_EMPOWERED, EFFECT_INDEX_0))
                    {
                        stack = aura->GetStackAmount();
                        m_creature->GetAura(SPELL_EMPOWERED, EFFECT_INDEX_0)->SetStackAmount(stack - 1);
                        hp -= 10;
                    }
                }
                healthCheckTimer = 1000;
            }else healthCheckTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_guardian_of_yogg_saronAI : public ScriptedAI
{
    mob_guardian_of_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 darkVolleyTimer;
    uint32 selfKillTimer;
    uint32 dominateMindTimer;
    bool mustDie;
    bool hasCasted;

    void Reset()
    {
        darkVolleyTimer = 10000;
        dominateMindTimer = 30000;
        mustDie = false;
        hasCasted = false;
        m_creature->SetRespawnDelay(DAY);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SARAS_ANGER)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    m_creature->AddThreat(pTarget,100.0f);
                    m_creature->AI()->AttackStart(pTarget);
                }
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            selfKillTimer = 500;
            m_creature->SetHealth(m_creature->GetMaxHealth());
            if(!hasCasted)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_NOVA : SPELL_SHADOW_NOVA_H);
                mustDie = true;
                hasCasted = true;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (darkVolleyTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DARK_VOLLEY);
            darkVolleyTimer = 15000;
        }else darkVolleyTimer -= uiDiff;

        if (dominateMindTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_DOMINATE_MIND);
            dominateMindTimer = 30000;
        }else dominateMindTimer -= uiDiff;

        if (selfKillTimer < uiDiff && mustDie)
        {
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            selfKillTimer = 15000;
        }else selfKillTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*
*   Tentacules
*/
struct MANGOS_DLL_DECL mob_corruptor_tentacleAI : public ScriptedAI
{
    mob_corruptor_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 eruptTimer;
    bool hasErupted;
    uint32 spellTimer;

    void Reset()
    {
        eruptTimer = 100;
        hasErupted = false;
        spellTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (eruptTimer < uiDiff && !hasErupted)
        {
            DoCast(m_creature, SPELL_ERUPT);
            hasErupted = true;
        }else eruptTimer -= uiDiff;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
            case 0:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_BLACK_PLAGUE);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_APATHY);
                break;
            case 2:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_CURSE_OF_DOOM);
                break;
            case 3:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_DRAINING_POISON);
                break;
            }
            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_constrictor_tentacleAI : public ScriptedAI
{
    mob_constrictor_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 squeezeTimer;

    void Reset()
    {
        squeezeTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (squeezeTimer < uiDiff)
        {
            // broken spell
            //if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            //DoCast(pTarget, m_bIsRegularMode ? SPELL_SQUEEZE : SPELL_SQUEEZE_H);
            squeezeTimer = 15000;
        }else squeezeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_crusher_tentacleAI : public ScriptedAI
{
    mob_crusher_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 eruptTimer;
    bool hasErupted;
    uint32 diminishPowerTimer;

    void Reset()
    {
        eruptTimer = 100;
        hasErupted = false;
        diminishPowerTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > 0)
            DoCast(m_creature, SPELL_FOCUSED_ANGER);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_YOGGSARON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (eruptTimer < uiDiff && !hasErupted)
        {
            DoCast(m_creature, SPELL_ERUPT);
            hasErupted = true;
        }else eruptTimer -= uiDiff;

        if (diminishPowerTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DIMINISH_POWER);
            diminishPowerTimer = 10000 + urand(1000, 5000);
        }else diminishPowerTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_vision_tentacleAI : public ScriptedAI
{
    mob_vision_tentacleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    void Reset()
    {  }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > 0 && m_creature->GetDisplayId() != 28813)
            m_creature->SetDisplayId(28813);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

/*
*   Other
*/
struct MANGOS_DLL_DECL mob_death_orbAI : public ScriptedAI
{
    mob_death_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 checkTimer;

    void Reset()
    {
        checkTimer = 1000;
        DoCast(m_creature, SPELL_DEATH_RAY_AURA);
        DoCast(m_creature, SPELL_DEATH_RAY_VISUAL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (checkTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                        i->getSource()->DealDamage(i->getSource(), 10000, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            } 
            checkTimer = 1000;
        }
        else checkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_sanity_wellAI : public ScriptedAI
{
    mob_sanity_wellAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 checkTimer;
    uint8 stack;

    void Reset()
    {
        checkTimer = 2000;
        DoCast(m_creature, SPELL_SANITY_WELL);
        DoCast(m_creature, SPELL_SANITY_WELL_VISUAL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (checkTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                    {
                        // increase sanity
                        if(i->getSource()->HasAura(SPELL_SANITY, EFFECT_INDEX_0))
                        {
                            if(Aura *aura = i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0))
                            {
                                stack = aura->GetStackAmount();
                                i->getSource()->GetAura(SPELL_SANITY, EFFECT_INDEX_0)->SetStackAmount(stack - 1);
                            }
                        }
                    }
                }
            } 
            checkTimer = 2000;
        }
        else checkTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_laughing_skullAI : public ScriptedAI
{
    mob_laughing_skullAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(SKULL_DISPLAY_ID);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        pCreature->GetMotionMaster()->MoveConfused();
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 checkTimer;

    void Reset()
    {
        checkTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (checkTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 4)
                        DoCast(i->getSource(), SPELL_LUNATIC_GAZE);
                }
            } 
            checkTimer = 2000;
        }
        else checkTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_yogg_saron(Creature* pCreature)
{
    return new boss_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_boss_sara(Creature* pCreature)
{
    return new boss_saraAI(pCreature);
}

CreatureAI* GetAI_mob_madness_portal(Creature* pCreature)
{
    return new mob_madness_portalAI(pCreature);
}

CreatureAI* GetAI_boss_brain_of_yogg_saron(Creature* pCreature)
{
    return new boss_brain_of_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_mob_corruptor_tentacle(Creature* pCreature)
{
    return new mob_corruptor_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_constrictor_tentacle(Creature* pCreature)
{
    return new mob_constrictor_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_crusher_tentacle(Creature* pCreature)
{
    return new mob_crusher_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_vision_tentacle(Creature* pCreature)
{
    return new mob_vision_tentacleAI(pCreature);
}

CreatureAI* GetAI_mob_guardian_of_yogg_saron(Creature* pCreature)
{
    return new mob_guardian_of_yogg_saronAI(pCreature);
}

CreatureAI* GetAI_mob_immortal_guardian(Creature* pCreature)
{
    return new mob_immortal_guardianAI(pCreature);
}

CreatureAI* GetAI_keeper_hodir(Creature* pCreature)
{
    return new keeper_hodirAI(pCreature);
}

CreatureAI* GetAI_keeper_freya(Creature* pCreature)
{
    return new keeper_freyaAI(pCreature);
}

CreatureAI* GetAI_keeper_thorim(Creature* pCreature)
{
    return new keeper_thorimAI(pCreature);
}

CreatureAI* GetAI_keeper_mimiron(Creature* pCreature)
{
    return new keeper_mimironAI(pCreature);
}

CreatureAI* GetAI_mob_death_orb(Creature* pCreature)
{
    return new mob_death_orbAI(pCreature);
}

CreatureAI* GetAI_mob_sanity_well(Creature* pCreature)
{
    return new mob_sanity_wellAI(pCreature);
}

CreatureAI* GetAI_mob_laughing_skull(Creature* pCreature)
{
    return new mob_laughing_skullAI(pCreature);
}

void AddSC_boss_yogg_saron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_yogg_saron";
    newscript->GetAI = &GetAI_boss_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_sara";
    newscript->GetAI = &GetAI_boss_sara;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_madness_portal";
    newscript->GetAI = &GetAI_mob_madness_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_brain_of_yogg_saron";
    newscript->GetAI = &GetAI_boss_brain_of_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_corruptor_tentacle";
    newscript->GetAI = &GetAI_mob_corruptor_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_constrictor_tentacle";
    newscript->GetAI = &GetAI_mob_constrictor_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_crusher_tentacle";
    newscript->GetAI = &GetAI_mob_crusher_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vision_tentacle";
    newscript->GetAI = &GetAI_mob_vision_tentacle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_guardian_of_yogg_saron";
    newscript->GetAI = &GetAI_mob_guardian_of_yogg_saron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_immortal_guardian";
    newscript->GetAI = &GetAI_mob_immortal_guardian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_hodir";
    newscript->GetAI = &GetAI_keeper_hodir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_freya";
    newscript->GetAI = &GetAI_keeper_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_thorim";
    newscript->GetAI = &GetAI_keeper_thorim;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "keeper_mimiron";
    newscript->GetAI = &GetAI_keeper_mimiron;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_death_orb";
    newscript->GetAI = &GetAI_mob_death_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sanity_well";
    newscript->GetAI = &GetAI_mob_sanity_well;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_laughing_skull";
    newscript->GetAI = &GetAI_mob_laughing_skull;
    newscript->RegisterSelf();
}
