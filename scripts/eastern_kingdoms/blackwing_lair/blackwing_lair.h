/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLACKWING_LAIR
#define DEF_BLACKWING_LAIR

enum
{
    MAX_ENCOUNTER           = 8,

    TYPE_RAZORGORE          = 1,
    TYPE_VAELASTRASZ        = 2,
    TYPE_LASHLAYER          = 3,
    TYPE_FIREMAW            = 4,
    TYPE_EBONROC            = 5,
    TYPE_FLAMEGOR           = 6,
    TYPE_CHROMAGGUS         = 7,
    TYPE_NEFARIAN           = 8, 

    NPC_RAZORGORE           = 12435, 
    NPC_VAELASTRASZ         = 13020, 
    NPC_LASHLAYER           = 12017,
    NPC_FIREMAW             = 11983,
    NPC_EBONROC             = 14601,
    NPC_FLAMEGOR            = 11981,
    NPC_CHROMAGGUS          = 14020,
    NPC_NEFARIAN            = 11583,
    NPC_LORD_NEFARIAN       = 10162,

    GO_DOOR_RAZORGORE_ENTER = 176964,
    GO_DOOR_RAZORGORE_EXIT  = 176965,
    GO_DOOR_NEFARIAN        = 176966,
    GO_DOOR_FLAMEGOR        = 179115,
    GO_DOOR_CHROMAGGUS_SIDE = 179116,
    GO_DOOR_CHROMAGGUS_EXIT = 179117,
    GO_DOOR_VAELASTRASZ     = 179364,
    GO_DOOR_LASHLAYER       = 179365
};

#endif
