/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BLACK_TEMPLE_H
#define DEF_BLACK_TEMPLE_H

enum
{
    MAX_ENCOUNTER                  = 9,

    TYPE_NAJENTUS                  = 1,
    TYPE_SUPREMUS                  = 2,
    TYPE_SHADE                     = 3,
    TYPE_GOREFIEND                 = 4,
    TYPE_BLOODBOIL                 = 5,
    TYPE_RELIQUIARY                = 6,
    TYPE_SHAHRAZ                   = 7,
    TYPE_COUNCIL                   = 8,
    TYPE_ILLIDAN                   = 9,

    DATA_HIGHWARLORDNAJENTUS       = 10,
    DATA_SUPREMUS                  = 11,
    DATA_SHADEOFAKAMA              = 12,
    DATA_AKAMA_SHADE               = 13,
    DATA_ILLIDARICOUNCIL           = 14,
    DATA_BLOOD_ELF_COUNCIL_VOICE   = 15,
    DATA_LADYMALANDE               = 16,
    DATA_HIGHNETHERMANCERZEREVOR   = 17,
    DATA_GATHIOSTHESHATTERER       = 18,
    DATA_VERASDARKSHADOW           = 19,
    DATA_AKAMA                     = 20,
    DATA_ILLIDANSTORMRAGE          = 21,

    DATA_GAMEOBJECT_NAJENTUS_GATE  = 22,
    DATA_GAMEOBJECT_SUPREMUS_DOORS = 23,
    DATA_GO_PRE_SHAHRAZ_DOOR       = 24,
    DATA_GO_POST_SHAHRAZ_DOOR      = 25,
    DATA_GO_COUNCIL_DOOR           = 26,
    DATA_GAMEOBJECT_ILLIDAN_GATE   = 27,
    DATA_GAMEOBJECT_ILLIDAN_DOOR_R = 28,
    DATA_GAMEOBJECT_ILLIDAN_DOOR_L = 29,

    GO_DOOR_MAIN            = 185882,
    GO_DOOR_COUNCIL_ENTER   = 185481,
    GO_DOOR_SHADE_AKAMA     = 185478,
    GO_DOOR_GURTOGG         = 185892,
    GO_DOOR_GOREFIEND       = 185480,
    GO_DOOR_MOTHER_ENTER    = 185479,
    GO_DOOR_MOTHER_EXIT     = 185482,
    GO_GATE_ILIDAN          = 185905,
    GO_GATE_WARLORD         = 185483,
    GO_DOOR_GOREFIEND_COMBAT= 186153,  
    GO_DOOR_COUNCIL_COMBAT  = 186152,
    GO_DOOR_ILI_RIGHT       = 186261,
    GO_DOOR_ILI_LEFT        = 186262
};

#endif
