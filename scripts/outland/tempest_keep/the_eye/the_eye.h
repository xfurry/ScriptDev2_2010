/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_THE_EYE_H
#define DEF_THE_EYE_H

enum
{
    MAX_ENCOUNTER               = 4,

    TYPE_ALAR                   = 1,
    TYPE_ASTROMANCER            = 2,
    TYPE_SOLARIAN               = 3,
    TYPE_VOIDREAVER             = 4,
    TYPE_KAELTHAS_PHASE         = 5,                        //not regular encounter, contains phase instead

    DATA_ASTROMANCER            = 8,
    DATA_KAELTHAS               = 9,

    DATA_CAPERNIAN              = 10,
    DATA_SANGUINAR              = 11,
    DATA_TELONICUS              = 12,
    DATA_THALADRED              = 13,
    DATA_ALAR                   = 14,

    GO_DOOR_KAEL_INNER_LEFT     = 184324,
    GO_DOOR_KAEL_INNER_RIGHT    = 184325,
    GO_DOOR_KAEL_OUTER_RIGHT    = 184326,
    GO_DOOR_ENTER_RIGHT         = 184327,
    GO_DOOR_KAEL_OUTER_LEFT     = 184328,
    GO_DOOR_ENTER_LEFT          = 184329
};

#endif
