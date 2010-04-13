/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_HALLS_OF_STONE_H
#define DEF_HALLS_OF_STONE_H

enum
{
    MAX_ENCOUNTER           = 4,

    TYPE_TRIBUNAL           = 0,
    TYPE_MAIDEN             = 1,
    TYPE_KRYSTALLUS         = 2,
    TYPE_SJONNIR            = 3,

    DATA_KRYSTALLUS         = 5,
    DATA_MAIDEN             = 6,
    DATA_TRIBUNAL           = 7,
    DATA_SJONNIR            = 8,

    DATA_KADDRAK            = 9,
    DATA_ABEDNEUM           = 10,
    DATA_MARNAK             = 11,

    DATA_GO_TRIBUNAL_CONSOLE = 12,
    DATA_GO_SKY_FLOOR       = 13,
    DATA_GO_KADDRAK         = 14,
    DATA_GO_ABEDNEUM        = 15,
    DATA_GO_MARNAK          = 16,

    NPC_BRANN               = 28070,
    NPC_KRYSTALLUS          = 27977,
    NPC_MAIDEN              = 27975,
    NPC_SJONNIR             = 27978,

    NPC_KADDRAK             = 30898,
    NPC_ABEDNEUM            = 30899,
    NPC_MARNAK              = 30897,

    GO_DOOR_SJONNIR         = 191296,
    GO_DOOR_TRIBUNAL        = 191295,                       
    GO_DOOR_MAIDEN          = 191292,
    GO_DOOR_ENTER           = 191459,
    GO_DOOR_BRAN            = 191293,
    GO_DOOR_CHAMBER         = 191294,

    GO_TRIBUNAL_CHEST       = 190586,
    GO_TRIBUNAL_CHEST_H     = 193996,

    GO_TRIBUNAL_HEAD_RIGHT  = 191670,                       // marnak
    GO_TRIBUNAL_HEAD_CENTER = 191669,                       // abedneum
    GO_TRIBUNAL_HEAD_LEFT   = 191671,                       // kaddrak

    GO_TRIBUNAL_CONSOLE     = 193907,
    GO_TRIBUNAL_FLOOR       = 191527,

    GO_SJONNIR_CONSOLE      = 193906
};

#endif
