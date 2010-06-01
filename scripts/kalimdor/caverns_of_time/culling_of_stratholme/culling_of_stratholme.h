/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_CULLING_OF_STRATHOLME_H
#define DEF_CULLING_OF_STRATHOLME_H  

enum Data
{
    TYPE_QUEST           = 1,
    TYPE_INTRO           = 2,
    TYPE_CRATES_COUNT    = 3,
    TYPE_PHASE           = 4,
    TYPE_ENCOUNTER       = 5,
    TYPE_WAVE_COUNT      = 6,
    TYPE_WING            = 7,
    TYPE_BONUS           = 8,
    TYPE_MALGANIS        = 9,

    // boss saves
    TYPE_MEATHOOK        = 11,
    TYPE_SALRAMM         = 12,
    TYPE_EPOCH           = 13,

    TYPE_ZOMBIEFEST      = 14,
    TYPE_ZOMBIE_COUNT    = 15,

    DATA_TEMPSUMMON      = 10,

    NPC_CHROMI01            = 26527,
    NPC_CHROMI02            = 27915,
    NPC_ARTHAS              = 26499,
    NPC_JAINA               = 26497,
    NPC_UTHER               = 26528,
    NPC_KNIGHT              = 28612,
    NPC_MIKE                = 30571,
    NPC_MAL_CORICS          = 31017,
    NPC_GRIAN_STONE         = 30561,
    NPC_JAMES               = 30553,
    NPC_FRAS_FRASIABI       = 30552,
    NPC_FORRESTER           = 30551,
    NPC_ROGER               = 27903,
    NPC_CRATE               = 30996,
    NPC_MORIGAN             = 27877,
    NPC_PERELLI             = 27876,
    NPC_JENA                = 27885,
    NPC_MARTHA              = 27884,
    NPC_MALCOLM             = 27891,
    NPC_DOG                 = 27892,
    NPC_BARTLEBY            = 27907,
    NPC_MARINE              = 27745,
    NPC_PRIEST              = 27747,

    NPC_INFINITE_ADVERSARY  = 27742,
    NPC_INFINITE_HUNTER     = 27743,
    NPC_INFINITE_AGENT      = 27744,
    NPC_TIME_RIFT           = 28409,
    NPC_TIME_RIFT_2         = 28439,
    NPC_ZOMBIE              = 27737,
    NPC_GHOUL               = 28249,
    NPC_NECROMANCER         = 28200,
    NPC_STALKER             = 28199,
    NPC_FIEND               = 27734,
    NPC_GOLEM               = 28201,
    NPC_EGHOUL              = 27729,
    NPC_CONSTRUCT           = 27736,
    NPC_ACOLYTE             = 27731,
    NPC_MEATHOOK            = 26529,
    NPC_SALRAMM             = 26530,
    NPC_EPOCH               = 26532,
    NPC_MALGANIS            = 26533,
    NPC_CITY                = 28167,
    NPC_INFINITE_CORRUPTOR  = 32273,

    GO_CRATE_LIGHT         = 190117,
    GO_SHKAF_GATE          = 188686,
    GO_MALGANIS_GATE1      = 187711,    
    GO_MALGANIS_GATE2      = 187723, 
    GO_MALGANIS_CHEST      = 190663,
    GO_MALGANIS_CHEST_H    = 193597,
    GO_EXIT                = 191788,

    SAY_INFINITE_DEPARTURE        = -1594190,


    WORLD_STATE_COS_TIME_ON       = 3932,
    WORLD_STATE_COS_TIME_COUNT    = 3931,
    WORLD_STATE_COS_WAVE_COUNT    = 3504,
    WORLD_STATE_COS_CRATE_ON      = 3479,
    WORLD_STATE_COS_CRATE_COUNT   = 3480,

    ACHIEV_COS          = 479,
    ACHIEV_COS_H        = 500,
    ACHIEV_CULLING_TIME = 1817,
    ACHIEV_ZOMBIEFEST   = 1872,

    RIGHT               = 1,
    LEFT                = 2
};

#endif
