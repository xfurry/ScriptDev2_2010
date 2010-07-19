/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_CITADEL_H
#define DEF_ICECROWN_CITADEL_H

enum
{
    MAX_ENCOUNTER               = 13,

    TYPE_MARROWGAR              = 1,
    TYPE_DEATHWHISPER           = 2,
    TYPE_GUNSHIP_BATTLE         = 3,
    TYPE_SAURFANG               = 4,
    TYPE_FESTERGUT              = 5,
    TYPE_ROTFACE                = 6,
    TYPE_PUTRICIDE              = 7,
    TYPE_PRINCE_COUNCIL         = 8,
    TYPE_BLOOD_QUEEN            = 9,
    TYPE_DREAMWALKER            = 10,
    TYPE_SINDRAGOSA             = 11,
    TYPE_LICH_KING              = 12,

    TYPE_ATTEMPTS               = 13,

    NPC_MARROWGAR               = 36612,
    NPC_DEATHWHISPER            = 36855,
    NPC_SAURFANG                = 37813,
    NPC_FESTERGUT               = 36626,
    NPC_ROTFACE                 = 36627,
    NPC_PUTRICIDE               = 36678,
    NPC_VALANAR                 = 37970,
    NPC_KELESETH                = 37972,
    NPC_TALDARAM                = 37973,
    NPC_LANATHEL                = 37955,

    // Outer citadel
    GO_RAID_EXIT                = 201583,
    GO_MARROWGAR_ICE_1          = 201910,
    GO_MARROWGAR_ICE_2          = 201911,
    GO_MARROWGAR_GATE           = 201857,
    GO_DEATHWHISPER_GATE        = 201563,
    GO_DEATHWHISPER_ELEVATOR    = 202220,
    GO_GUNSHIP_STAIRS           = 201211,
    GO_SAURFANG_DOOR            = 201825, 
    
    // Plaguewing
    GO_GREEN_MONSTER_ENTRANCE   = 201370,
    GO_ORANGE_MONSTER_ENTRANCE  = 201371,
    GO_GAS_VALVE                = 201616,
    GO_OOSE_VALVE               = 201615,
    GO_GREEN_TUBE               = 201618,
    GO_ORANGE_TUBE              = 201617,
    GO_SCIENTIST_DOOR_GREEN     = 201614,
    GO_SCIENTIST_DOOR_ORANGE    = 201613,
    GO_SCIENTIST_DOOR_COLISION  = 201612,
    GO_SCIENTIST_ENTRANCE       = 201372,

    // Bloodwing
    GO_BLOOD_COUNCIL_DOOR_LEFT  = 201378,
    GO_BLOOD_COUNCIL_DOOR_RIGHT = 201379,
    GO_BLOODWING_DOOR           = 201920,
    GO_CRIMSON_HALL_DOOR        = 201376,
    GO_BLOODQUEEN_DOOR          = 201746,
    GO_EMPOWERING_BLOOD_ORB     = 201741,

    // Frostwing
    GO_SINDRAGOSA_ENTRANCE      = 201373,
    GO_SINDRAGOSA_EXIT          = 201379,
    GO_SINDRAGOSA_SHORT_ENT     = 201369,
    GO_FROSTWING_DOOR           = 201919,
    GO_GREEN_DRAGON_ENTRANCE    = 201375,
    GO_GREEN_DRAGON_EXIT        = 201374,
    GO_ROOST_PORTCULLIS1        = 201380,
    GO_ROOST_PORTCULLIS2        = 201381,
    GO_ROOST_PORTCULLIS3        = 201382,
    GO_ROOST_PORTCULLIS4        = 201383,

    // Sigils
    GO_BLOODWING_SIGIL          = 202183,
    GO_FROSTWING_SIGIL          = 202181,
    GO_PLAGUEWING_SIGIL         = 201182,

    // Frozen throne
    GO_ARTHAS_PLATFORM          = 202161,
    GO_ICESHARD_STANDINGS_1     = 202141,
    GO_ICESHARD_STANDINGS_2     = 202142,
    GO_ICESHARD_STANDINGS_3     = 202143,
    GO_ICESHARD_STANDINGS_4     = 202144,

    // Loot
    GO_DEATHBRINGERS_CACHE_10       = 202239,
    GO_DEATHBRINGERS_CACHE_10HC     = 202238,
    GO_DEATHBRINGERS_CACHE_25       = 202240,
    GO_DEATHBRINGERS_CACHE_25HC     = 202241,

    // these are unk for now -> need to rename later
    GO_GUNSHIP_ARMORY_ALY_10        = 201872,
    GO_GUNSHIP_ARMORY_ALY_10HC      = 201873,
    GO_GUNSHIP_ARMORY_ALY_25        = 201874,
    GO_GUNSHIP_ARMORY_ALY_25HC      = 201875,

    GO_GUNSHIP_ARMORY_HORDE_10      = 202177,
    GO_GUNSHIP_ARMORY_HORDE_10HC    = 202178,
    GO_GUNSHIP_ARMORY_HORDE_25      = 202179,
    GO_GUNSHIP_ARMORY_HORDE_25HC    = 202180,

    GO_CACHE_OF_DREAMWALKER_10      = 201959,
    GO_CACHE_OF_DREAMWALKER_10HC    = 202338,
    GO_CACHE_OF_DREAMWALKER_25      = 202339,
    GO_CACHE_OF_DREAMWALKER_25HC    = 202340,

	SPELL_SOUL_FEAST				= 71203,
};

#endif
