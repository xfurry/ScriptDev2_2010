/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_OBSIDIAN_SANCTUM_H
#define DEF_OBSIDIAN_SANCTUM_H

enum
{
    MAX_ENCOUNTER               = 1,

    TYPE_SARTHARION_EVENT       = 1,
    TYPE_DRAKES                 = 2,    // used for loot modes

    DATA_SARTHARION             = 10,
    DATA_TENEBRON               = 11,
    DATA_SHADRON                = 12,
    DATA_VESPERON               = 13,

    // bonus loot - workaround
    GO_ONE_DRAKE_CHEST          = 500101,
    GO_TWO_DRAKE_CHEST          = 500102,
    GO_THREE_DRAKE_CHEST        = 500103,
    GO_ONE_DRAKE_CHEST_H        = 500104,
    GO_TWO_DRAKE_CHEST_H        = 500105,
    GO_THREE_DRAKE_CHEST_H      = 500106,

    NPC_SARTHARION              = 28860,
    NPC_TENEBRON                = 30452,
    NPC_SHADRON                 = 30451,
    NPC_VESPERON                = 30449,
    GO_TWILIGHT_PORTAL          = 193988
};

#endif
