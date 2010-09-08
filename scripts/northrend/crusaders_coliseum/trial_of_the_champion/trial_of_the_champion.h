/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_TOC_H
#define DEF_TOC_H

enum
{
    MAX_ENCOUNTER               = 3,

    TYPE_GRAND_CHAMPIONS        = 0,
    TYPE_ARGENT_CHALLENGE       = 1,
    TYPE_BLACK_KNIGHT           = 2,
    TYPE_STAGE                  = 3,

    DATA_TOC_ANNOUNCER          = 4,
    DATA_ARGENT_CHALLERGER      = 5,

	DATA_CHAMPION_1				= 6,
	DATA_CHAMPION_2				= 7,
	DATA_CHAMPION_3				= 8,

    // aly champs
    NPC_JACOB                   = 34705,
    NPC_AMBROSE                 = 34702,
    NPC_COLOSOS                 = 34701,
    NPC_JAELYNE                 = 34657,
    NPC_LANA                    = 34703,
    // horde champs
    NPC_MOKRA                   = 35572,
    NPC_ERESSEA                 = 35569,
    NPC_RUNOK                   = 35571,
    NPC_ZULTORE                 = 35570,
    NPC_VISCERI                 = 35617,
    // argent challegers
    // trash
    NPC_ARGENT_LIGHTWIELDER     = 35309,
    NPC_ARGENT_MONK             = 35305,
    NPC_ARGENT_PRIESTESS        = 35307,
    // boss
    NPC_EADRIC                  = 35119,
    NPC_PALETRESS               = 34928,
    // black knight
    NPC_BLACK_KNIGHT            = 35451,
    NPC_RISEN_JAEREN            = 35545,
    NPC_RISEN_ARELAS            = 35564,
    NPC_RISEN_CHAMPION          = 35590,
    // announcer
    NPC_JAEREN                  = 35004,
    NPC_ARELAS                  = 35005,
    NPC_TIRION                  = 34996,

    // memories
    MEMORY_ALGALON              = 35052,
    MEMORY_ARCHIMONDE           = 35041,
    MEMORY_CHROMAGGUS           = 35033,
    MEMORY_CYANIGOSA            = 35046,
    MEMORY_DELRISSA             = 35043,
    MEMORY_ECK                  = 35047,
    MEMORY_ENTROPIUS            = 35044,
    MEMORY_GRUUL                = 35039,
    MEMORY_HAKKAR               = 35034,
    MEMORY_HEIGAN               = 35049,
    MEMORY_HEROD                = 35030,
    MEMORY_HOGGER               = 34942,
    MEMORY_IGNIS                = 35050,
    MEMORY_ILLIDAN              = 35042,
    MEMORY_INGVAR               = 35045,
    MEMORY_KALITHRESH           = 35037,
    MEMORY_LUCIFRON             = 35031,
    MEMORY_MALCHEZAAR           = 35038,
    MEMORY_MUTANUS              = 35029,
    MEMORY_ONYXIA               = 35048,
    MEMORY_THUNDERAAN           = 35032,
    MEMORY_VANCLEEF             = 35028,
    MEMORY_VASHJ                = 35040,
    MEMORY_VEKNILASH            = 35036,
    MEMORY_VEZAX                = 35051,

    // doors
    GO_MAIN_GATE                = 195647,
    GO_WEST_GATE                = 195589,
    GO_SOUTH_GATE               = 195590,
    GO_NORTH_GATE               = 195650,

    // loot
    GO_CHAMPIONS_LOOT           = 195709,
    GO_CHAMPIONS_LOOT_H         = 195710,
    GO_EADRIC_LOOT              = 195374,
    GO_EADRIC_LOOT_H            = 195375,
    GO_PALETRESS_LOOT           = 195323,
    GO_PALETRESS_LOOT_H         = 195324,

    // achievs
    ACHIEV_TOC_ALY_H            = 4298,
    ACHIEV_TOC_HORDE_H          = 4297,
    ACHIEV_TOC_ALY              = 4296,
    ACHIEV_TOC_HORDE            = 3778,
};

enum eVehicles
{
    //Grand Champions Alliance Vehicles
    VEHICLE_MARSHAL_JACOB_ALERIUS_MOUNT             = 35637,
    VEHICLE_AMBROSE_BOLTSPARK_MOUNT                 = 35633,
    VEHICLE_COLOSOS_MOUNT                           = 35768,
    VEHICLE_EVENSONG_MOUNT                          = 34658,
    VEHICLE_LANA_STOUTHAMMER_MOUNT                  = 35636,
    //Faction Champions (ALLIANCE)
    VEHICLE_DARNASSIA_NIGHTSABER                    = 33298,
    VEHICLE_EXODAR_ELEKK                            = 33416,
    VEHICLE_STORMWIND_STEED                         = 33297,
    VEHICLE_GNOMEREGAN_MECHANOSTRIDER               = 33301,
    VEHICLE_IRONFORGE_RAM                           = 33408,
    //Grand Champions Horde Vehicles
    VEHICLE_MOKRA_SKILLCRUSHER_MOUNT                = 35638,
    VEHICLE_ERESSEA_DAWNSINGER_MOUNT                = 35635,
    VEHICLE_RUNOK_WILDMANE_MOUNT                    = 35640,
    VEHICLE_ZUL_TORE_MOUNT                          = 35641,
    VEHICLE_DEATHSTALKER_VESCERI_MOUNT              = 35634,
    //Faction Champions (HORDE)
    VEHICLE_FORSAKE_WARHORSE                        = 33414,
    VEHICLE_THUNDER_BLUFF_KODO                      = 33300,
    VEHICLE_ORGRIMMAR_WOLF                          = 33409,
    VEHICLE_SILVERMOON_HAWKSTRIDER                  = 33418,
    VEHICLE_DARKSPEAR_RAPTOR                        = 33299,

    VEHICLE_ARGENT_WARHORSE                         = 35644,
    VEHICLE_ARGENT_BATTLEWORG                       = 36558,

    VEHICLE_BLACK_KNIGHT                            = 35491
};


#endif
