/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_HALLS_H
#define DEF_ICECROWN_HALLS_H

enum
{
    MAX_ENCOUNTERS              = 6,

    TYPE_INTRO                  = 0,
    TYPE_FALRIC                 = 1,
    TYPE_MARWYN                 = 2,
    TYPE_FROST_GENERAL          = 3,
    TYPE_ESCAPE                 = 4,
	TYPE_SPIRIT_EVENT			= 5,	// used to save falric in case of reset

    NPC_FALRIC                  = 38112,
    NPC_MARWYN                  = 38113,
    NPC_LICH_KING               = 37226,
    NPC_LICH_KING_INTRO         = 36954,

    NPC_JAINA_START             = 37221,
    NPC_JAINA_END               = 36955,
    NPC_SYLVANAS_START          = 37223,
    NPC_SYLVANAS_END            = 37554,

    NPC_FROST_GENERAL           = 36723,

    DATA_ICECROWN_DOOR          = 1,
    DATA_FROSTMOURNE            = 2,
    DATA_SHADOW_DOOR            = 3,
    DATA_SHADOW_DOOR_EXIT       = 4,
    DATA_IMPENETRABLE_DOOR      = 5,
    DATA_FALRIC                 = 6,
    DATA_MARWYN                 = 7,
    DATA_JAINA                  = 8,
    DATA_SYLVANAS               = 9,
    DATA_LICH_KING              = 10,
    DATA_JAINA_INTRO            = 11,
    DATA_SYLVANAS_INTRO         = 12,
    DATA_ALTAR                  = 13,

    GO_ICECROWN_DOOR             = 201976, //72802
    GO_IMPENETRABLE_DOOR         = 197341, //72801
    GO_FROSTMOURNE_ALTAR         = 202236, //3551
    GO_FROSTMOURNE               = 202302, //364

    GO_SHADOW_THRONE_DOOR       = 197342, 
    GO_SHADOW_THROME_DOOR_EXIT  = 197343,    

    GO_ICE_WALL                 = 201385,
    GO_GUNSHIP_STAIRS           = 201709,
    GO_CAVE_IN                  = 201596,
    // these 2 are vehicles
    GO_SKY_BREAKER              = 201580,// 201598
    GO_ORGRIMS_HAMMER           = 201581,// 201599

    GO_CAPTAIN_CHEST_HORDE      = 202212, //3145
    GO_CAPTAIN_CHEST_ALY        = 201710, //30357
    GO_CAPTAIN_CHEST_HORDE_H    = 202337, //3246
    GO_CAPTAIN_CHEST_ALY_H      = 202336, //3333

    MOB_WAVES_NUM               = 4,
    MOB_WAVES_DELAY             = 30000,  //in milliseconds

    MOB_SPECTRAL_FOOTMAN        = 38173,
    MOB_SHADOWY_MERCENARY       = 38177,
    MOB_PHANTOM_HALLUCINATION   = 38567,
    MOB_PHANTOM_MAGE            = 38172,
    MOB_GHOSTLY_PRIEST          = 38175,
    MOB_TORTURED_RIFLEMAN       = 38176,

    MOB_RAGING_GHOUL            = 36940,
    MOB_RISEN_WHITCH_DOCTOR     = 36941,
    MOB_LUMBERING_ABONIMATION   = 37069,

    UI_STATE_SPIRIT_WAVES       = 4884,
    UI_STATE_SPIRIT_WAVES_COUNT = 4882,
};

struct Locations
{
    float x, y, z, o;
    uint32 id;
};

static Locations SpawnLoc[]=
{
	/*
    {5325.330078f, 1976.609985f, 707.695007f},  
    {5274.630859f, 2040.283813f, 709.319824f},  
    {5346.934570f, 2044.740234f, 707.695801f},
    {5286.936035f, 1983.856323f, 707.695129f},
    // footman
    (5313.3f,  1977.29f, 707.695f),
    (5301.35f, 1973.57f, 707.695f),
    (5338.87f, 1991.67f, 707.695f),
    (5341.95f, 1999.47f, 707.695f),
    (5312.76f, 2037.05f, 707.696f),
    (5298.75f, 2033.85f, 707.696f),
    (5281.8f,  2016.06f, 707.696f),
    (5276.48f, 1995.26f, 707.696f),//8
    //mercenary
    (5320.44f, 1980.01f, 707.695f),
    (5293.43f, 1975.91f, 707.695f),
    (5339.06f, 2004.27f, 707.695f),
    (5318.5f,  2035.36f, 707.696f),
    (5280.03f, 2000.79f, 707.696f),
    (5280.14f, 1989.21f, 707.696f),//14
    // hallucination
    (5309.83f, 1971.77f, 707.695f),
    (5338.58f, 2010.99f, 707.695f),
    (5306.13f, 2035.99f, 707.696f),
    (5293.49f, 2035.1f,  707.696f),
    (5279.73f, 2021.09f, 707.696f),
    (5280.1f,  2008.85f, 707.696f),//20
    // mage
    (5298.49f, 1978.18, 707.695f),
    (5342.1f,  2016.14f, 707.695f),
    (5309.06f, 2041.29f, 707.696f),
    (5281.38f, 1994.45f, 707.696f),//24
    // priest
    (5305.88f, 1976.52f, 707.695f),
    (5336.48f, 1996.89f, 707.695f),
    (5336.75f, 2018.03f, 707.696f),
    (5325.29f, 2036.8f,  707.696f),
    (5301.08f, 2038.36f, 707.696f),
    (5277.07f, 2013.37f, 707.696f),//30
    // rifleman
    (5325.33f, 1976.61f, 707.695f),
    (5318.86f, 1974.06f, 707.695f),
    (5343.66f, 2007.9f,  707.695f),
    (5338.7f,  2023.72f, 707.696f),
    (5316.15f, 2039.87f, 707.696f),
    (5274.96f, 2005.16f, 707.696f),//36
	*/
    //Wing 01
    {5277.409f, 1993.161f, 707.694f, 0.05f}, //27
    {5301.876f, 2041.699f, 707.694f, 4.71f}, //1
    {5339.830f, 2020.887f, 707.694f, 3.14f}, //13
    {5311.041f, 2042.935f, 707.694f, 4.71f}, //3
    {5314.750f, 2039.969f, 707.694f, 4.71f}, //4
    {5342.823f, 2003.801f, 707.694f, 3.14f}, //10
    {5311.579f, 1972.823f, 707.694f, 1.62f}, //16

    //Wing 02
    {5272.491f, 2005.673f, 707.694f, 0.05f}, //23
    {5302.669f, 1973.050f, 707.694f, 1.62f}, //18
    {5346.187f, 2008.058f, 707.694f, 3.14f}, //9
    {5319.752f, 2041.321f, 707.694f, 4.71f}, //5
    {5344.882f, 1998.714f, 707.694f, 3.14f}, //11
    {5340.552f, 1994.735f, 707.694f, 3.14f}, //12
    {5306.441f, 2040.358f, 707.694f, 4.71f}, //2

    //Wing 03
    {5273.297f, 2014.009f, 707.694f, 0.05f}, //25
    {5316.062f, 1970.777f, 707.694f, 1.62f}, //15
    {5322.498f, 2037.415f, 707.694f, 4.71f}, //6
    {5307.087f, 1970.065f, 707.694f, 1.62f}, //17
    {5342.460f, 2012.391f, 707.694f, 3.14f}, //8
    {5297.601f, 1971.420f, 707.694f, 1.62f}, //19
    {5295.668f, 1975.853f, 707.694f, 1.62f}, //20

    //Wing 04
    {5273.661f, 1996.767f, 707.694f, 0.05f}, //21
    {5275.228f, 2001.275f, 707.694f, 0.05f}, //22
    {5344.153f, 2017.753f, 707.694f, 3.14f}, //7
    {5275.310f, 2009.686f, 707.694f, 0.05f}, //24
    {5319.835f, 1975.177f, 707.694f, 1.62f}, //14
    {5277.445f, 2017.197f, 707.694f, 0.05f}, //26
    {5298.198f, 2037.762f, 707.694f, 4.71f}  //0
};

#endif
