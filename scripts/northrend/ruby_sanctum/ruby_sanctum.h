/* Copyright (C) 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RUBY_SANCTUM_H
#define DEF_RUBY_SANCTUM_H

enum
{
    MAX_ENCOUNTERS                 = 4,

    TYPE_SAVIANA                   = 1,
    TYPE_BALTHARUS                 = 2,
    TYPE_ZARITHRIAN                = 3,
    TYPE_HALION                    = 4,
	TYPE_HALION_PHASE			   = 5,

    NPC_HALION_REAL                = 39863, // Halion Physical Realm NPC
    NPC_HALION_TWILIGHT            = 40142, // Halion Twilight Realm NPC
	NPC_HALION_CONTROL             = 40146,

    NPC_BALTHARUS                  = 39751,
    NPC_ZARITHRIAN                 = 39746,
    NPC_SAVIANA	                   = 39747,

    NPC_XERESTRASZA                = 40429,

	// what for?
    NPC_BALTHARUS_TARGET           = 39900,
	NPC_ZARITHIAN_SPAWN_STALKER    = 39794,

	// Orb rotation
    NPC_SHADOW_PULSAR_N            = 40083, //spinning orb N spawn
    NPC_SHADOW_PULSAR_S            = 40100, //spinning orb S spawn
    NPC_ORB_CARRIER                = 40081,
    NPC_ORB_ROTATION_FOCUS         = 40091,

    GO_HALION_PORTAL_1             = 202794, //1327 ENTRY
    GO_HALION_PORTAL_2             = 202795, //1327 ENTRY
    GO_HALION_PORTAL_3             = 202796, //1327 EXIT

    GO_FIRE_FIELD                  = 203005,	// xerestrasza door
    GO_FLAME_WALLS                 = 203006,	// zarithrian ring
    GO_FLAME_RING                  = 203007,	// halion ring
};

// used for corporality
enum uiWorldStates
{
    UPDATE_STATE_UI_SHOW            = 5049,
    UPDATE_STATE_UI_COUNT           = 5050,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

#endif
