/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VIOLET_H
#define DEF_VIOLET_H

enum
{
    MAX_ENCOUNTER               = 14,

    TYPE_MAIN                   = 1,
    TYPE_SEAL                   = 2,
    TYPE_PORTAL                 = 3,

    TYPE_FIRST_BOSS             = 4,
    TYPE_SECOND_BOSS            = 5,
    TYPE_CYANIGOSA              = 6,
    
    TYPE_FIRST_BOSS_ID          = 7,
    TYPE_SECOND_BOSS_ID         = 8,

    TYPE_EREKEM                 = 9,
    TYPE_MORAGG                 = 10,
    TYPE_ICHORON                = 11,
    TYPE_XEVOZZ                 = 12,
    TYPE_LAVANTHOR              = 13,
    TYPE_ZURAMAT                = 14,

    TYPE_LASTBOSS               = 15,
    TYPE_LASTBOSS_ID            = 16,

    WORLD_STATE_ID              = 3816,
    WORLD_STATE_SEAL            = 3815,
    WORLD_STATE_PORTALS         = 3810,

    GO_INTRO_CRYSTAL            = 193615,
    GO_DEFENSE_CRYSTAL          = 193611,
    GO_PRISON_SEAL_DOOR         = 191723,

    GO_CELL_LAVANTHOR           = 191566,
    GO_CELL_MORAGG              = 191606,
    GO_CELL_ZURAMAT             = 191565,
    GO_CELL_EREKEM              = 191564,
    GO_CELL_EREKEM_GUARD_L      = 191563,
    GO_CELL_EREKEM_GUARD_R      = 191562,
    GO_CELL_XEVOZZ              = 191556,
    GO_CELL_ICHORON             = 191722,

    NPC_EVENT_CONTROLLER        = 30883,
    NPC_PORTAL_INTRO            = 31011,
    NPC_PORTAL                  = 30679,
    NPC_PORTAL_ELITE            = 32174,
    NPC_DOOR_SEAL               = 30896,

    NPC_SINCLARI                = 30658,
    NPC_SINCLARI_ALT            = 32204,                    // yeller for seal weakening and summoner for portals
    NPC_HOLD_GUARD              = 30659,

    NPC_EREKEM                  = 29315,
    NPC_EREKEM_GUARD            = 29395,
    NPC_MORAGG                  = 29316,
    NPC_ICHORON                 = 29313,
    NPC_XEVOZZ                  = 29266,
    NPC_LAVANTHOR               = 29312,
    NPC_ZURAMAT                 = 29314,
    NPC_CYANIGOSA               = 31134,

    NPC_PORTAL_GUARDIAN         = 30660,
    NPC_PORTAL_KEEPER           = 30695,

    NPC_AZURE_INVADER           = 30661,
    NPC_AZURE_SPELLBREAKER      = 30662,
    NPC_AZURE_BINDER            = 30663,
    NPC_AZURE_MAGE_SLAYER       = 30664,
    NPC_MAGE_HUNTER             = 30665,
    NPC_AZURE_CAPTAIN           = 30666,
    NPC_AZURE_SORCEROR          = 30667,
    NPC_AZURE_RAIDER            = 30668,
    NPC_AZURE_STALKER           = 32191,

    // used for intro
    NPC_AZURE_BINDER_INTRO      = 31007,
    NPC_AZURE_INVADER_INTRO     = 31008,
    NPC_AZURE_SPELLBREAKER_INTRO= 31009,
    NPC_AZURE_MAGE_SLAYER_INTRO = 31010,

    NPC_AZURE_SABOTEUR          = 31079,

    NPC_DEFENSE_SYSTEM          = 30837,
    NPC_DEFENSE_DUMMY_TARGET    = 30857,

    NPC_ARAKKOA                 = 32226,
    NPC_VOID_LORD               = 32230,
    NPC_ETHERAL                 = 32231,
    NPC_SWIRLING                = 32234,
    NPC_WATCHER                 = 32235,
    NPC_LAVA_HOUND              = 32237,

    SPELL_DEFENSE_SYSTEM_VISUAL = 57887,
    SPELL_DEFENSE_SYSTEM_SPAWN  = 57886,
    SPELL_DEFENSE_SYSTEM_DMG    = 57912,                    // spell dmg
    SPELL_DEFENSE_SYSTEM_INTRO  = 57930,                    // just visual, no dmg

    SPELL_DESTROY_DOOR_SEAL     = 58040,                    // spell periodic cast by misc
    SPELL_TELEPORTATION_PORTAL  = 57687,                    // visual aura, but possibly not used? creature_template model for portals are same

    SPELL_SHIELD_DISRUPTION     = 58291,                    // dummy when opening a cell

    SPELL_PORTAL_PERIODIC       = 58008,                    // most likely the tick for each summon (tick each 15 seconds)
    SPELL_PORTAL_CHANNEL        = 58012,                    // the blue "stream" between portal and guardian/keeper
    SPELL_PORTAL_BEAM           = 56046,                    // large beam, unsure if really used here (or possible for something different)

    SPELL_PORTAL_VISUAL_1       = 57872,                    // no idea, but is possibly related based on it's visual appearence
    SPELL_PORTAL_VISUAL_2       = 57630,

    SAY_SEAL_75                 = -1608002,
    SAY_SEAL_50                 = -1608003,
    SAY_SEAL_5                  = -1608004,

    EMOTE_GUARDIAN_PORTAL       = -1608005,
    EMOTE_DRAGONFLIGHT_PORTAL   = -1608006,
    EMOTE_KEEPER_PORTAL         = -1608007,

    ACHIEV_DEFENSELESS          = 1816,

    MAX_NORMAL_PORTAL           = 8
};

static float fDefenseSystemLoc[4] = {1888.146f, 803.382f, 58.604f, 3.072f};

enum ePortalType
{
    PORTAL_TYPE_NORM = 0,
    PORTAL_TYPE_SQUAD,
    PORTAL_TYPE_BOSS,
};

struct sPortalData
{
    ePortalType pPortalType;
    float fX, fY, fZ, fOrient;
};

static sPortalData afPortalLocation[]=
{
    {PORTAL_TYPE_NORM, 1936.07f, 803.198f, 53.3749f, 3.1241f},  //balcony
    {PORTAL_TYPE_NORM, 1877.51f, 850.104f, 44.6599f, 4.7822f},  //erekem
    {PORTAL_TYPE_NORM, 1890.64f, 753.471f, 48.7224f, 1.7104f},  //moragg
    {PORTAL_TYPE_SQUAD, 1911.06f, 802.103f, 38.6465f, 2.8908f}, //below balcony
    {PORTAL_TYPE_SQUAD, 1928.06f, 763.256f, 51.3167f, 2.3905f}, //bridge
    {PORTAL_TYPE_SQUAD, 1924.26f, 847.661f, 47.1591f, 4.0202f}, //zuramat
    {PORTAL_TYPE_NORM, 1914.16f, 832.527f, 38.6441f, 3.5160f},  //xevozz
    {PORTAL_TYPE_NORM, 1857.30f, 764.145f, 38.6543f, 0.8339f},  //lavanthor
    {PORTAL_TYPE_BOSS, 1890.73f, 803.309f, 38.4001f, 2.4139f},  //center
};

struct Locations
{
    float x, y, z;
    uint32 id;
};
struct WayPoints
{
    WayPoints(uint32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    uint32 id;
    float x, y, z;
};

static Locations PortalLoc[]=
{
    {1936.101f, 802.950f, 52.417f}, // 0 balcony
    {1878.198f, 850.005f, 43.333f}, // 1 Portal in front of Erekem
    {1892.737f, 744.589f, 47.666f}, // 2 Moragg
    {1909.381f, 806.796f, 38.645f}, // 3 Portal outside of Ichoron
    {1928.060f, 763.256f, 51.316f}, // 4 bridge
    {1925.480f, 849.981f, 47.174f}, // 5 Zuramat
    {1914.160f, 832.527f, 38.644f}, // 6 xevozz
    {1857.125f, 763.295f, 38.654f}, // 7 Lavanthor
    {1888.271f, 810.781f, 38.441f}, // 8 center
};

static Locations BossLoc[]=
{
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {1876.100f, 857.079f, 43.333f}, // Erekem
    {1892.737f, 744.589f, 47.666f}, // Moragg
    {1908.863f, 785.647f, 37.435f}, // Ichoron
    {1905.364f, 840.607f, 38.670f}, // Xevozz
    {1857.125f, 763.295f, 38.654f}, // Lavanthor
    {1925.480f, 849.981f, 47.174f}, // Zuramat
};

static Locations DragonsWP[]=
{
    //center, ichoron
    {1869.393f, 803.902f, 38.768f}, // 0 
    {1859.843f, 804.222f, 44.008f}, // 1 
    {1827.960f, 804.208f, 44.364f}, // 2 

    //From left side (lavanthor)
    {1861.016f, 789.717f, 38.908f}, // 3 
    {1856.217f, 796.705f, 44.008f}, // 4 
    {1827.960f, 804.208f, 44.364f}, // 5 

    //From Zuramat
    {1931.446f, 826.734f, 47.556f}, // 6 
    {1913.049f, 823.930f, 38.792f}, // 7 
    {1827.960f, 804.208f, 44.364f}, // 8 
    {1869.393f, 803.902f, 38.768f}, // 9 
    {1859.843f, 804.222f, 44.008f}, // 10 
    {1827.960f, 804.208f, 44.364f}, // 11 

    //From Morag & Bridge
    {1892.737f, 744.589f, 47.666f}, // 12
    {1887.500f, 763.096f, 47.666f}, // 13 
    {1880.837f, 775.769f, 38.796f}, // 14 
    {1861.016f, 789.717f, 38.908f}, // 15 
    {1856.217f, 796.705f, 44.008f}, // 16 
    {1827.960f, 804.208f, 44.364f}, // 17 

    //From erekem
    {1878.280f, 843.267f, 43.333f}, // 18 
    {1872.311f, 835.531f, 38.780f}, // 19 
    {1861.997f, 818.766f, 38.650f}, // 20 
    {1857.348f, 811.230f, 44.008f}, // 21
    {1827.960f, 804.208f, 44.364f}, // 22 

    //From Highest platform
    {1937.298f, 824.557f, 52.332f}, // 23
    {1913.049f, 823.930f, 38.792f}, // 24
    {1869.393f, 803.902f, 38.768f}, // 25
    {1859.843f, 804.222f, 44.008f}, // 26
    {1827.960f, 804.208f, 44.364f}, // 27
};

class MANGOS_DLL_DECL instance_violet_hold : public ScriptedInstance
{
    public:
        instance_violet_hold(Map* pMap);
        ~instance_violet_hold() {}

        void Initialize();
        void ResetAll();
        void ResetVariables();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void UpdateCellForBoss(uint32 uiBossEntry);
        void UpdateWorldState(bool bEnable = true);

        void SetIntroPortals(bool bDeactivate);
        void SpawnPortal();

        void SetPortalId();

        void CallGuards(bool bRespawn);

        uint32 GetRandomPortalEliteEntry();
        uint32 GetRandomMobForNormalPortal();

        uint32 GetCurrentPortalNumber() { return m_uiWorldStatePortalCount; }
        uint8 GetCurrentPortalId() { return m_uiPortalId; }
        uint32 GetCurrentSealCount() { return m_uiWorldStateSealCount; }

        sPortalData const* GetPortalData() { return &afPortalLocation[m_uiPortalId]; }

        bool IsCurrentPortalForTrash()
        {
            if (m_uiWorldStatePortalCount % 6)
                return true;

            return false;
        }

        bool IsNextPortalForTrash()
        {
            if ((m_uiWorldStatePortalCount+1) % 6)
                return true;

            return false;
        }

        void ProcessActivationCrystal(Unit* pUser, bool bIsIntro = false);

        void SetRandomBosses();

        void OnPlayerEnter(Player* pPlayer);

        void SetData(uint32 uiType, uint32 uiData);
        uint64 GetData64(uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save();
        void Load(const char* chrIn);

        void Update(uint32 uiDiff);

        typedef std::multimap<uint32, uint64> BossToCellMap;

    protected:

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiSinclariGUID;
        uint64 m_uiSinclariAltGUID;
        uint64 m_uiErekemGUID;
        uint64 m_uiMoraggGUID;
        uint64 m_uiIchoronGUID;
        uint64 m_uiXevozzGUID;
        uint64 m_uiLavanthorGUID;
        uint64 m_uiZuramatGUID;

        uint64 m_uiCellErekemGuard_LGUID;
        uint64 m_uiCellErekemGuard_RGUID;
        uint64 m_uiIntroCrystalGUID;
        uint64 m_uiDefenseCrystalGUID;
        uint64 m_uiDoorSealGUID;

        uint32 m_uiWorldState;
        uint32 m_uiWorldStateSealCount;
        uint32 m_uiWorldStatePortalCount;

        uint8 m_uiPortalId;
        uint32 m_uiPortalTimer;
        uint32 m_uiMaxCountPortalLoc;

        uint8 m_uiLastBossID;
        uint8 m_uiLastBossIDConst;
        bool m_bDefenseUsed;

        BossToCellMap m_mBossToCellMap;

        std::list<uint64> m_lIntroPortalList;
        std::list<uint64> m_lGuardsList;
        std::list<uint32> m_lRandomBossList;
};

#endif
