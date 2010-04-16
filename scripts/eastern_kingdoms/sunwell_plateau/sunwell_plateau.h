/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_SUNWELLPLATEAU_H
#define DEF_SUNWELLPLATEAU_H

enum InstanceSWP
{
    MAX_ENCOUNTER               = 6,

    TYPE_KALECGOS               = 0,
    TYPE_BRUTALLUS              = 1,
    TYPE_FELMYST                = 2,
    TYPE_EREDAR_TWINS           = 3,
    TYPE_MURU                   = 4,
    TYPE_KILJAEDEN              = 5,

    TYPE_DECIVER                = 28,
    TYPE_SACROLASH              = 40,
    TYPE_ALYSTHESS              = 41,
    TYPE_SATHROVARR             = 42,

    DATA_KALECGOS_DRAGON        = 6,
    DATA_KALECGOS_HUMAN         = 7,
    DATA_SATHROVARR             = 8,
    DATA_BRUTALLUS              = 9,
    DATA_FELMYST                = 10,
    DATA_ALYTHESS               = 11,
    DATA_SACROLASH              = 12,
    DATA_MURU                   = 13,
    DATA_KILJAEDEN              = 14,
    DATA_KILJAEDEN_CONTROLLER   = 15,
    DATA_ANVEENA                = 16,
    DATA_KALECGOS               = 17,

    DATA_GO_FORCEFIELD          = 18,
    DATA_GO_FIRE_BARRIER        = 19,
    DATA_GO_FIRST_GATE          = 20,
    DATA_GO_SECOND_GATE         = 21,
    DATA_GO_RAID_GATE_07        = 22,
    DATA_GO_RAID_GATE_08        = 23,
    DATA_GO_THIRD_GATE          = 24,

    DATA_PLAYER_SPECTRAL_REALM  = 25,
    DATA_SET_SPECTRAL_CHECK     = 26,
    DATA_RANDOM_SPECTRAL_PLAYER = 26,
    DATA_INST_EJECT_PLAYERS     = 27,

    /*** Doors ***/
    GO_FIRE_BARRIER                = 188075,    // brutalus + felmyst door
    G0_FORCE_FIELD                 = 188421,    // kalegnos door
    GO_MURU_GATE                   = 188118,    // muru door exit
    GO_GATE_7                      = 187990,    // muru door enter
    GO_FIRST_GATE                  = 187766,    // kalegnos door exit -> not in db
    GO_SECOND_GATE                 = 187764,    // eredar twins exit
    GO_THIRD_GATE                  = 187765,    // unk

    NPC_KALECGNOS_DRAGON            = 24850,
    NPC_KALECGNOS_HUMAN             = 24891,
    NPC_SATHROVARR                  = 24892,
    NPC_BRUTALLUS                   = 24882,
    NPC_FELMYST                     = 25038,
    NPC_ALYTHESS                    = 25166,
    NPC_SACROLASH                   = 25165,
    NPC_MURU                        = 25741,
    NPC_KILJAEDEN                   = 25315,
    NPC_KILJAEDEN_CONTROLLER        = 25608,
    NPC_ANVEENA                     = 26046,
    NPC_KALECGNOS                   = 25319,

    SPELL_SPECTRAL_REALM                = 46021,
    SPELL_TELEPORT_NORMAL_REALM         = 46020,    // invalid location -> FIXME
    SPELL_TELEPORT_TO_SPECTRAL_REALM    = 46019,    // invalid location -> FIXME
    SPELL_SPECTRAL_EXHAUSTION           = 44867,
    SPELL_SPECTRAL_REALM_FORCE_FACTION  = 44852
};
#endif
