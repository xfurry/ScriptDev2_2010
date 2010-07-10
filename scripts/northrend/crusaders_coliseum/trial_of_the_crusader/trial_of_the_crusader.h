/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TRIAL_OF_THE_CRUSADER_H
#define DEF_TRIAL_OF_THE_CRUSADER_H

enum
{
    MAX_ENCOUNTER          = 8,

    TYPE_NORTHREND_BEASTS  = 1,
    TYPE_JARAXXUS          = 2,
    TYPE_FACTION_CHAMPIONS = 3,
    TYPE_TWIN_VALKYR       = 4,
    TYPE_ANUBARAK          = 5,

    TYPE_COUNTER           = 7,
    TYPE_INTRO             = 8,
    TYPE_STAGE             = 0,

    // implement this in order to avoid the valkyrs
    // to cast their special spell in the same time
    TYPE_TWINS_CASTING     = 9,

    DATA_GORMOK            = 6,
    DATA_ACIDMAW           = 7,
    DATA_DREADSCALE        = 8,
    DATA_ICEHOWL           = 9,
    DATA_JARAXXUS          = 10,
    DATA_FACTION_CHAMPIONS = 11,
    DATA_FJOLA             = 12,
    DATA_EYDIS             = 13,
    DATA_ANUBARAK          = 14,

    DATA_TIRION             = 15, 
    DATA_VARIAN             = 16,
    DATA_GARROSH            = 17,
    DATA_BARRET             = 18,
    DATA_JAINA              = 19,
    DATA_THRALL             = 20,

    MAX_EVENT              = 4,
    
    NPC_BARRENT                 = 34816,
    NPC_TIRION                  = 34996,
    NPC_TIRION_END              = 36095,
    NPC_WILFRED                 = 35458,
    NPC_GARROSH                 = 34995,
    NPC_VARIAN                  = 34990,
    NPC_LICH_KING               = 35877,

    NPC_THRALL                  = 34994,
    NPC_JAINA                   = 34992,
    NPC_PORTAL                  = 19224,
    NPC_TRIGGER                 = 22517,

    NPC_ICEHOWL                 = 34797,
    NPC_GORMOK                  = 34796,
    NPC_DREADSCALE              = 34799,
    NPC_ACIDMAW                 = 35144,

    NPC_JARAXXUS                = 34780,

    // aly
    NPC_CRUSADER_1_1            = 34460,
    NPC_CRUSADER_1_2            = 34463,
    NPC_CRUSADER_1_3            = 34461,
    NPC_CRUSADER_1_4            = 34471,
    NPC_CRUSADER_1_5            = 34475,
    NPC_CRUSADER_1_6            = 34472,
    NPC_CRUSADER_1_7            = 34467,
    NPC_CRUSADER_1_8            = 34468,
    NPC_CRUSADER_1_9            = 34473,
    NPC_CRUSADER_1_10           = 34474,

    // horde
    NPC_CRUSADER_2_1            = 34453,
    NPC_CRUSADER_2_2            = 34455,
    NPC_CRUSADER_2_3            = 34458,
    NPC_CRUSADER_2_4            = 34454,
    NPC_CRUSADER_2_5            = 34451,
    NPC_CRUSADER_2_6            = 34456,
    NPC_CRUSADER_2_7            = 34441,
    NPC_CRUSADER_2_8            = 34449,
    NPC_CRUSADER_2_9            = 34448,
    NPC_CRUSADER_2_10           = 34450,

    // crusaders pets
    NPC_CRUSADER_0_1            = 35465,
    NPC_CRUSADER_0_2            = 35610,

    NPC_FJOLA                   = 34497,
    NPC_EYDIS                   = 34496,

    NPC_DARK_ESSENCE            = 34567,
    NPC_LIGHT_ESSENCE           = 34568,

    NPC_ANUBARAK                = 34564,
 
    GO_CRUSADERS_CACHE_10       = 195631,
    GO_CRUSADERS_CACHE_25       = 195632,
    GO_CRUSADERS_CACHE_10_H     = 195633,
    GO_CRUSADERS_CACHE_25_H     = 195635,

    GO_TRIBUTE_CHEST_10H_25     = 195665,   // 1-25
    GO_TRIBUTE_CHEST_10H_45     = 195666,   // 25-44
    GO_TRIBUTE_CHEST_10H_50     = 195667,   // 45-49
    GO_TRIBUTE_CHEST_10H_99     = 195668,   // no wipe

    GO_TRIBUTE_CHEST_25H_25     = 195669,
    GO_TRIBUTE_CHEST_25H_45     = 195670,
    GO_TRIBUTE_CHEST_25H_50     = 195671,
    GO_TRIBUTE_CHEST_25H_99     = 195672,

    GO_MAIN_GATE                = 195647,
    GO_WEST_GATE                = 195589,
    GO_SOUTH_GATE               = 195590,
    GO_NORTH_GATE               = 195591,
    GO_COLISEUM_FLOOR           = 195527,
    GO_WEB_DOOR                 = 195485,

    DATA_HEALTH_EYDIS           = 201,
    DATA_HEALTH_FJOLA           = 202,
    DATA_CASTING_EYDIS          = 203,
    DATA_CASTING_FJOLA          = 204,
 
    DESPAWN_TIME                = 900000, 

    DATA_ANNOUNCER              = 0,
    DATA_FLOOR                  = 1,
    DATA_MAIN_GATE              = 2,

    ACHIEV_TRIBUTE_DEDICATED_INSANITY   = 4080,
    ACHIEV_TRIBUTE_IMMORTALITY_ALY      = 4156,
    ACHIEV_TRIBUTE_IMMORTALITY_HORDE    = 4079,
    ACHIEV_TRIBUTE_INSANITY             = 3810,
    ACHIEV_TRIBUTE_INSANITY_H           = 3819,
    ACHIEV_TRIBUTE_MAD_SKILL            = 3809,
    ACHIEV_TRIBUTE_MAD_SKILL_H          = 3818,
    ACHIEV_TRIBUTE_SKILL                = 3808,
    ACHIEV_TRIBUTE_SKILL_H              = 3817,
    ACHIEV_CALL_CRUSADER                = 3917,
    ACHIEV_CALL_CRUSADER_H              = 3916,
    ACHIEV_CALL_GRAND_CRUSADER          = 3918,
    ACHIEV_CALL_GRAND_CRUSADER_H        = 3812,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};
 
static Locations SpawnLoc[]=
{
    {559.257996f, 90.266197f, 395.122986f},  // 0 Barrent
    {563.672974f, 139.571f, 393.837006f},    // 1 Center
    {563.833008f, 187.244995f, 394.5f},      // 2 Backdoor
    {577.347839f, 195.338888f, 395.14f},     // 3 - Right
    {550.955933f, 195.338888f, 395.14f},     // 4 - Left
    {575.042358f, 195.260727f, 395.137146f}, // 5
    {552.248901f, 195.331955f, 395.132658f}, // 6
    {573.342285f, 195.515823f, 395.135956f}, // 7
    {554.239929f, 195.825577f, 395.137909f}, // 8
    {571.042358f, 195.260727f, 395.137146f}, // 9
    {556.720581f, 195.015472f, 395.132658f}, // 10
    {569.534119f, 195.214478f, 395.139526f}, // 11
    {569.231201f, 195.941071f, 395.139526f}, // 12
    {558.811610f, 195.985779f, 394.671661f}, // 13
    {567.641724f, 195.351501f, 394.659943f}, // 14
    {560.633972f, 195.391708f, 395.137543f}, // 15
    {565.816956f, 195.477921f, 395.136810f}, // 16
    {563.549f, 152.474f, 394.393f},          // 17 - Lich king start
    {563.547f, 141.613f, 393.908f},          // 18 - Lich king end
    {787.932556f, 133.28978f, 142.612152f},  // 19 - Anub'arak start location
    {618.157898f, 132.640869f, 139.559769f}, // 20 - Anub'arak move point location
    {508.104767f, 138.247345f, 395.128052f}, // 21 - Fizzlebang start location
    {586.060242f, 117.514809f, 394.314026f}, // 22 - Dark essence 1
    {541.602112f, 161.879837f, 394.587952f}, // 23 - Dark essence 2
    {541.021118f, 117.262932f, 395.314819f}, // 24 - Light essence 1
    {586.200562f, 162.145523f, 394.626129f}, // 25 - Light essence 2
    {563.833008f, 195.244995f, 394.585561f}, // 26 - outdoor
    {548.610596f, 139.807800f, 394.321838f}, // 27 - fizzlebang end
    {564.161133f, 230.246307f, 394.661560f}, // 28 outside the door   
};

enum uiWorldStates
{
    UPDATE_STATE_UI_SHOW            = 4390,
    UPDATE_STATE_UI_COUNT           = 4389,
};

#endif
