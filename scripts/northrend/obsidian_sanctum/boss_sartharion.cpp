/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Boss Sartharion
SD%Complete: 70%
SDComment: Flame wave, achievement and portal events need to be implemented
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "obsidian_sanctum.h"

enum
{
    //Sartharion Yell
    SAY_SARTHARION_AGGRO                        = -1615018,
    SAY_SARTHARION_BERSERK                      = -1615019,
    SAY_SARTHARION_BREATH                       = -1615020,
    SAY_SARTHARION_CALL_SHADRON                 = -1615021,
    SAY_SARTHARION_CALL_TENEBRON                = -1615022,
    SAY_SARTHARION_CALL_VESPERON                = -1615023,
    SAY_SARTHARION_DEATH                        = -1615024,
    SAY_SARTHARION_SPECIAL_1                    = -1615025,
    SAY_SARTHARION_SPECIAL_2                    = -1615026,
    SAY_SARTHARION_SPECIAL_3                    = -1615027,
    SAY_SARTHARION_SPECIAL_4                    = -1615028,
    SAY_SARTHARION_SLAY_1                       = -1615029,
    SAY_SARTHARION_SLAY_2                       = -1615030,
    SAY_SARTHARION_SLAY_3                       = -1615031,

    WHISPER_LAVA_CHURN                          = -1615032,

    WHISPER_SHADRON_DICIPLE                     = -1615008,
    WHISPER_VESPERON_DICIPLE                    = -1615041,
    WHISPER_HATCH_EGGS                          = -1615017,
    WHISPER_OPEN_PORTAL                         = -1615042, // whisper, shared by two dragons

    //Sartharion Spells
    SPELL_BERSERK                               = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                                = 56909,    // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                          = 56908,    // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_H                        = 58956,    // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                             = 56910,    // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_H                           = 58957,    // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION                    = 61254,    // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                           = 57571,    // (Real spell casted should be 57578) 57571 then trigger visual missile, then summon Lava Blaze on impact(spell 57572)
    SPELL_TWILIGHT_REVENGE                      = 60639,
    SPELL_SOFT_ENRAGE                           = 28131,

    SPELL_PYROBUFFET                            = 56916,    // currently used for hard enrage after 15 minutes
    SPELL_PYROBUFFET_RANGE                      = 58907,    // possibly used when player get too far away from dummy creatures (2x creature entry 30494)

    SPELL_TWILIGHT_SHIFT                        = 57874,    // Twilight Shift Aura
    SPELL_TWILIGHT_SHIFT_ENTER                  = 57620,    // enter phase. Player get this when click GO
    SPELL_TWILIGHT_SHIFT_REMOVAL                = 61187,    // leave phase
    SPELL_TWILIGHT_SHIFT_REMOVAL_ALL            = 61190,    // leave phase (probably version to make all leave)

    //Mini bosses common spells
    SPELL_TWILIGHT_RESIDUE                      = 61885,    // makes immune to shadow damage, applied when leave phase

    //Miniboses (Vesperon, Shadron, Tenebron)
    SPELL_SHADOW_BREATH_H                       = 59126,    // Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
    SPELL_SHADOW_BREATH                         = 57570,    // Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.

    SPELL_SHADOW_FISSURE_H                      = 59127,    // Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
    SPELL_SHADOW_FISSURE                        = 57579,    // Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

    //Vesperon
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_ACOLYTE_OF_VESPERON                     = 31219,    // Acolyte of Vesperon 30858,
    NPC_DISCIPLE_OF_VESPERON                    = 30858,
    SPELL_POWER_OF_VESPERON                     = 61251,    // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_TWILIGHT_TORMENT_VESP                 = 57948,    // (Shadow only) trigger 57935 then 57988
    SPELL_TWILIGHT_TORMENT_AURA                 = 57935,    // Aura that triggers the dmg
    SPELL_TWILIGHT_TORMENT_VESP_ACO             = 58853,    // (Fire and Shadow) trigger 58835 then 57988

    //Shadron
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    NPC_ACOLYTE_OF_SHADRON                      = 31218,    // Acolyte of Shadron 30688,
    NPC_DISCIPLE_OF_SHADRON                     = 30688,    // dummy npc, used to keep the aura alive
    SPELL_POWER_OF_SHADRON                      = 58105,    // Shadron's presence increases Fire damage taken by all enemies by 100%.
    SPELL_GIFT_OF_TWILIGTH_SHA                  = 57835,    // TARGET_SCRIPT shadron
    SPELL_GIFT_OF_TWILIGTH_SAR                  = 58766,    // TARGET_SCRIPT sartharion

    //Tenebron
    //in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
    SPELL_POWER_OF_TENEBRON                     = 61248,    // Tenebron's presence increases Shadow damage taken by all enemies by 100%.
    //Tenebron, dummy spell
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // will spawn NPC_SHARTHARION_TWILIGHT_WHELP

    SPELL_HATCH_EGGS_H                          = 59189,
    SPELL_HATCH_EGGS                            = 58542,
    SPELL_HATCH_EGGS_EFFECT_H                   = 59190,
    SPELL_HATCH_EGGS_EFFECT                     = 58685,
    NPC_TWILIGHT_EGG                            = 30882,
    NPC_SARTHARION_TWILIGHT_EGG                 = 31204,

    //Whelps
    NPC_TWILIGHT_WHELP                          = 30890,
    NPC_SARTHARION_TWILIGHT_WHELP               = 31214,
    SPELL_FADE_ARMOR                            = 60708,    // Reduces the armor of an enemy by 1500 for 15s

    //flame tsunami
    SPELL_FLAME_TSUNAMI_VISUAL                  = 57494,    // the visual dummy
    SPELL_FLAME_TSUNAMI_DMG_AURA                = 57492,    // periodic damage, npc has this aura
    SPELL_FLAME_TSUNAMI_KNOCK                   = 60241,
    SPELL_FLAME_TSUNAMI_DMG                     = 57491,
    SPELL_MOLTEN_FURY                           = 60430,

    // flame tornado
    SPELL_TORNADO_VISUAL                        = 57560,    //effect tornado
    SPELL_TORNADO_SCALE                         = 57598,    //if tornado ready use lava strike

    // twilight fissure
    SPELL_VOID_BLAST                            = 57581, 
    SPELL_VOID_BLAST_H                          = 59128,

    NPC_FLAME_TSUNAMI                           = 30616,    // for the flame waves
    NPC_LAVA_BLAZE                              = 30643,    // adds spawning from flame strike
    NPC_FLAME_TORNADO                           = 30648,    //for the flame tornado

    //using these custom points for dragons start and end
    POINT_ID_INIT                               = 100,
    POINT_ID_LAND                               = 200,

    //Achievements
    ACHIEV_TWILIGHT_ASSIST                      = 2049,
    ACHIEV_TWILIGHT_ASSIST_H                    = 2052,
    ACHIEV_TWILIGHT_DUO                         = 2050,
    ACHIEV_TWILIGHT_DUO_H                       = 2053,
    ACHIEV_TWILIGHT_ZONE                        = 2051,
    ACHIEV_TWILIGHT_ZONE_H                      = 2054
};

struct Waypoint
{
    float m_fX, m_fY, m_fZ;
};

struct LocationXYZ
{
    float x,y,z;
};

Waypoint m_aTsu[]=
{
     {3283.85f, 499.56f, 57.15f},                                    //start
     {3209.73f, 509.61f, 56.42f}                                      //End
};

//each dragons special points. First where fly to before connect to connon, second where land point is.
Waypoint m_aTene[]=
{
    {3212.854f, 575.597f, 109.856f},                        //init
    {3246.425f, 565.367f, 61.249f}                          //end
};

Waypoint m_aShad[]=
{
    {3293.238f, 472.223f, 106.968f},
    {3271.669f, 526.907f, 61.931f}
};

Waypoint m_aVesp[]=
{
    {3193.310f, 472.861f, 102.697f},
    {3227.268f, 533.238f, 59.995f}
};

//points around raid "isle", counter clockwise. should probably be adjusted to be more alike
Waypoint m_aDragonCommon[]=
{
    {3214.012f, 468.932f, 98.652f},
    {3244.950f, 468.427f, 98.652f},
    {3283.520f, 496.869f, 98.652f},
    {3287.316f, 555.875f, 98.652f},
    {3250.479f, 585.827f, 98.652f},
    {3209.969f, 566.523f, 98.652f}
};

float ThsunamiLocations[5][4]=
{
    //left side
    {3198.790f, 486.745f, 55.686f, 6.230f},
    {3204.037f, 533.541f, 58.216f, 6.275f},
    {3204.363f, 579.140f, 56.915f, 6.254f},
    // right side
    {3287.655f, 552.528f, 58.357f, 3.185f},
    {3287.928f, 511.096f, 58.439f, 3.094f},
};

static LocationXYZ AcolyteofShadron =     { 3363.92f, 534.703f, 97.2683f };
static LocationXYZ AcolyteofShadron2 =    { 3246.57f, 551.263f, 58.6164f };
static LocationXYZ AcolyteofVesperon =    { 3145.68f, 520.71f, 89.7f };
static LocationXYZ AcolyteofVesperon2 =   { 3246.57f, 551.263f, 58.6164f };

LocationXYZ TwilightEggs[] =
{
    {3219.28f, 669.121f , 88.5549f},
    {3221.55f, 682.852f , 90.5361f},
    {3239.77f, 685.94f , 90.3168f},
    {3250.33f, 669.749f , 88.7637f},
    {3246.6f, 642.365f , 84.8752f},
    {3233.68f, 653.117f , 85.7051f}
};
LocationXYZ TwilightEggsSarth[] =
{
    {3261.75f, 539.14f , 58.6082f},
    {3257.41f, 512.939f , 58.5432f},
    {3231.04f, 498.281f , 58.6439f}
};

enum TeneText
{
    SAY_TENEBRON_AGGRO                      = -1615009,
    SAY_TENEBRON_SLAY_1                     = -1615010,
    SAY_TENEBRON_SLAY_2                     = -1615011,
    SAY_TENEBRON_DEATH                      = -1615012,
    SAY_TENEBRON_BREATH                     = -1615013,
    SAY_TENEBRON_RESPOND                    = -1615014,
    SAY_TENEBRON_SPECIAL_1                  = -1615015,
    SAY_TENEBRON_SPECIAL_2                  = -1615016
};

enum ShadText
{
    SAY_SHADRON_AGGRO                       = -1615000,
    SAY_SHADRON_SLAY_1                      = -1615001,
    SAY_SHADRON_SLAY_2                      = -1615002,
    SAY_SHADRON_DEATH                       = -1615003,
    SAY_SHADRON_BREATH                      = -1615004,
    SAY_SHADRON_RESPOND                     = -1615005,
    SAY_SHADRON_SPECIAL_1                   = -1615006,
    SAY_SHADRON_SPECIAL_2                   = -1615007
};

enum VespText
{
    SAY_VESPERON_AGGRO                      = -1615033,
    SAY_VESPERON_SLAY_1                     = -1615034,
    SAY_VESPERON_SLAY_2                     = -1615035,
    SAY_VESPERON_DEATH                      = -1615036,
    SAY_VESPERON_BREATH                     = -1615037,
    SAY_VESPERON_RESPOND                    = -1615038,
    SAY_VESPERON_SPECIAL_1                  = -1615039,
    SAY_VESPERON_SPECIAL_2                  = -1615040
};

/*######
## Boss Sartharion
######*/
struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsBerserk;
    bool m_bIsSoftEnraged;

    uint32 m_uiEnrageTimer;
    bool m_bIsHardEnraged;

    uint32 m_uiTenebronTimer;
    uint32 m_uiShadronTimer;
    uint32 m_uiVesperonTimer;

    uint32 m_uiFlameTsunamiTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiLavaStrikeTimer;

    bool m_bHasCalledTenebron;
    bool m_bHasCalledShadron;
    bool m_bHasCalledVesperon;
    bool bCanUseWill;
    bool m_bHasUsedWill;
    bool m_bSwitchSide;

    uint32 m_uiAchievProgress;

    void Reset()
    {
        m_bIsBerserk = false;
        m_bIsSoftEnraged = false;

        m_uiEnrageTimer = MINUTE*15*IN_MILLISECONDS;
        m_bIsHardEnraged = false;
        m_bSwitchSide   = true;

        m_uiTenebronTimer = 30000;
        m_uiShadronTimer = 75000;
        m_uiVesperonTimer = 120000;

        m_uiFlameTsunamiTimer = 30000;
        m_uiFlameBreathTimer = 20000;
        m_uiTailSweepTimer = 20000;
        m_uiCleaveTimer = 7000;
        m_uiLavaStrikeTimer = 5000;

        m_bHasCalledTenebron = false;
        m_bHasCalledShadron = false;
        m_bHasCalledVesperon = false;
        bCanUseWill         = false;
        m_bHasUsedWill      = false;

        if (m_creature->HasAura(SPELL_TWILIGHT_REVENGE))
            m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_REVENGE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DRAKES, 0);

        m_uiAchievProgress = 0;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, NOT_STARTED);

            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT_ENTER);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SARTHARION_AGGRO,m_creature);

        m_creature->SetInCombatWithZone();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, IN_PROGRESS);

            m_creature->SummonCreature(NPC_FLAME_TORNADO,3209.872f, 533.078f, 57.155f,3,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            m_creature->SummonCreature(NPC_FLAME_TORNADO,3209.596f, 556.051f, 57.155f,3,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            m_creature->SummonCreature(NPC_FLAME_TORNADO,3285.880f, 533.474f, 57.155f,3,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            m_creature->SummonCreature(NPC_FLAME_TORNADO,3283.321f, 550.309f, 57.155f,3,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);

            FetchDragons();
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_SARTHARION_DEATH,m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_DRAKES, 0);

        if (m_uiAchievProgress == 1)
        {
            if(m_pInstance)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWILIGHT_ASSIST : ACHIEV_TWILIGHT_ASSIST_H);
                m_pInstance->SetData(TYPE_DRAKES, 1);
            }
        }
        else if (m_uiAchievProgress == 2)
        {
            if(m_pInstance)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWILIGHT_DUO : ACHIEV_TWILIGHT_DUO_H);
                m_pInstance->SetData(TYPE_DRAKES, 2);
            }
        }
        else if (m_uiAchievProgress == 3)
        {
            if(m_pInstance)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWILIGHT_ZONE : ACHIEV_TWILIGHT_ZONE_H);
                m_pInstance->SetData(TYPE_DRAKES, 3);
            }
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
        case 0: DoScriptText(SAY_SARTHARION_SLAY_1, m_creature); break;
        case 1: DoScriptText(SAY_SARTHARION_SLAY_2, m_creature); break;
        case 2: DoScriptText(SAY_SARTHARION_SLAY_3, m_creature); break;
        }
    }

    void FetchDragons()
    {
        Unit* pTene = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_TENEBRON));
        Unit* pShad = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_SHADRON));
        Unit* pVesp = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_VESPERON));

        //if at least one of the dragons are alive and are being called
        if (pTene && pTene->isAlive() && !pTene->getVictim())
        {
            bCanUseWill = true;
            m_uiAchievProgress += 1;
            pTene->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aTene[0].m_fX, m_aTene[0].m_fY, m_aTene[0].m_fZ);

            if (!pTene->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pTene->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pShad && pShad->isAlive() && !pShad->getVictim())
        {
            bCanUseWill = true;
            m_uiAchievProgress += 1;
            pShad->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aShad[0].m_fX, m_aShad[0].m_fY, m_aShad[0].m_fZ);

            if (!pShad->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pShad->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pVesp && pVesp->isAlive() && !pVesp->getVictim())
        {
            bCanUseWill = true;
            m_uiAchievProgress += 1;
            pVesp->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aVesp[0].m_fX, m_aVesp[0].m_fY, m_aVesp[0].m_fZ);

            if (!pVesp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pVesp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (bCanUseWill)
        {
            DoCast(m_creature, SPELL_WILL_OF_SARTHARION);
            m_bHasUsedWill = true;
        }
    }

    void CallDragon(uint32 uiDataId)
    {
        if (m_pInstance)
        {
            Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(uiDataId));

            if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
            {
                if (pTemp->HasSplineFlag(SPLINEFLAG_WALKMODE))
                    pTemp->RemoveSplineFlag(SPLINEFLAG_WALKMODE);

                if (pTemp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                int32 iTextId = 0;

                switch(pTemp->GetEntry())
                {
                case NPC_TENEBRON:
                    iTextId = SAY_SARTHARION_CALL_TENEBRON;
                    DoScriptText(SAY_TENEBRON_RESPOND, pTemp);
                    pTemp->SetMaxHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->SetHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aTene[1].m_fX, m_aTene[1].m_fY, m_aTene[1].m_fZ);
                    break;
                case NPC_SHADRON:
                    iTextId = SAY_SARTHARION_CALL_SHADRON;
                    DoScriptText(SAY_SHADRON_RESPOND, pTemp);
                    pTemp->SetMaxHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->SetHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aShad[1].m_fX, m_aShad[1].m_fY, m_aShad[1].m_fZ);
                    break;
                case NPC_VESPERON:
                    iTextId = SAY_SARTHARION_CALL_VESPERON;
                    DoScriptText(SAY_VESPERON_RESPOND, pTemp);
                    pTemp->SetMaxHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->SetHealth(pTemp->GetMaxHealth() + (pTemp->GetMaxHealth() * 0.25));
                    pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aVesp[1].m_fX, m_aVesp[1].m_fY, m_aVesp[1].m_fZ);
                    break;
                }

                DoScriptText(iTextId, m_creature);
            }
        }
    }

    void SendFlameTsunami()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_LAVA_CHURN, m_creature,i->getSource());
                }
            }
        }
        
        if(m_bSwitchSide)
        {
            // right side
            m_creature->SummonCreature(30616,ThsunamiLocations[3][0],ThsunamiLocations[3][1],ThsunamiLocations[3][2],ThsunamiLocations[3][3],TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_creature->SummonCreature(30616,ThsunamiLocations[4][0],ThsunamiLocations[4][1],ThsunamiLocations[4][2],ThsunamiLocations[4][3],TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_bSwitchSide = false; 
        }
        else
        {
            // left side
            m_creature->SummonCreature(30616,ThsunamiLocations[0][0],ThsunamiLocations[0][1],ThsunamiLocations[0][2],ThsunamiLocations[0][3],TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_creature->SummonCreature(30616,ThsunamiLocations[1][0],ThsunamiLocations[1][1],ThsunamiLocations[1][2],ThsunamiLocations[1][3],TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_creature->SummonCreature(30616,ThsunamiLocations[2][0],ThsunamiLocations[2][1],ThsunamiLocations[2][2],ThsunamiLocations[2][3],TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_bSwitchSide = true;
        }
        
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //spell will target dragons, if they are still alive at 35%
        if (!m_bIsBerserk && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 35)
        {
            Creature* pVesperon = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_VESPERON));
            Creature* pShadron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SHADRON));
            Creature* pTenebron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_TENEBRON));

            if(pVesperon->isAlive() || pShadron->isAlive() || pTenebron->isAlive())
            {
                DoScriptText(SAY_SARTHARION_BERSERK, m_creature);
                DoCast(m_creature, SPELL_BERSERK);
                m_bIsBerserk = true;
            }
        }

        //soft enrage
        if (!m_bIsSoftEnraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 10)
        {
            DoCast(m_creature, SPELL_SOFT_ENRAGE);
            m_bIsSoftEnraged = true;
        }

        // hard enrage
        if (!m_bIsHardEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_PYROBUFFET, true);
                m_bIsHardEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        if (m_bHasUsedWill)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth() + (m_creature->GetMaxHealth() * 0.25));
            m_bHasUsedWill = false;
        }

        // flame tsunami
        if (m_uiFlameTsunamiTimer < uiDiff)
        {
            SendFlameTsunami();
            m_uiFlameTsunamiTimer = 30000;
        }
        else
            m_uiFlameTsunamiTimer -= uiDiff;

        // flame breath
        if (m_uiFlameBreathTimer < uiDiff)
        {
            DoScriptText(SAY_SARTHARION_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H);
            m_uiFlameBreathTimer = urand(25000, 35000);
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Tail Sweep
        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
            m_uiTailSweepTimer = urand(15000, 20000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(7000, 10000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Lavas Strike
        if (m_uiLavaStrikeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_LAVA_STRIKE);

                switch(urand(0, 15))
                {
                case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
                case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
                case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
                }
            }
            m_uiLavaStrikeTimer = urand(5000, 20000);
        }
        else
            m_uiLavaStrikeTimer -= uiDiff;

        // call tenebron
        if (!m_bHasCalledTenebron && m_uiTenebronTimer < uiDiff)
        {
            CallDragon(DATA_TENEBRON);
            m_bHasCalledTenebron = true;
        }
        else
            m_uiTenebronTimer -= uiDiff;

        // call shadron
        if (!m_bHasCalledShadron && m_uiShadronTimer < uiDiff)
        {
            CallDragon(DATA_SHADRON);
            m_bHasCalledShadron = true;
        }
        else
            m_uiShadronTimer -= uiDiff;

        // call vesperon
        if (!m_bHasCalledVesperon && m_uiVesperonTimer < uiDiff)
        {
            CallDragon(DATA_VESPERON);
            m_bHasCalledVesperon = true;
        }
        else
            m_uiVesperonTimer -= uiDiff;

        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(uiDiff);
    }
};

CreatureAI* GetAI_boss_sartharion(Creature* pCreature)
{
    return new boss_sartharionAI(pCreature);
}


//to control each dragons common abilities
struct MANGOS_DLL_DECL dummy_dragonAI : public ScriptedAI
{
    dummy_dragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWaypointId;
    uint32 m_uiMoveNextTimer;
    int32 m_iPortalRespawnTime;
    bool m_bCanMoveFree;

    void Reset()
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_uiWaypointId = 0;
        m_uiMoveNextTimer = 500;
        m_iPortalRespawnTime = 30000;
        m_bCanMoveFree = false;
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (!m_pInstance || uiType != POINT_MOTION_TYPE)
            return;

        debug_log("dummy_dragonAI: %s reached point %u", m_creature->GetName(), uiPointId);

        //if healers messed up the raid and we was already initialized
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            EnterEvadeMode();
            return;
        }

        //this is the end (!)
        if (uiPointId == POINT_ID_LAND)
        {
            m_creature->GetMotionMaster()->Clear();
            m_bCanMoveFree = false;
            m_creature->SetInCombatWithZone();
            return;
        }

        //get amount of common points
        uint32 uiCommonWPCount = sizeof(m_aDragonCommon)/sizeof(Waypoint);

        //increase
        m_uiWaypointId = uiPointId+1;

        //if we have reached a point bigger or equal to count, it mean we must reset to point 0
        if (m_uiWaypointId >= uiCommonWPCount)
        {
            if (!m_bCanMoveFree)
                m_bCanMoveFree = true;

            m_uiWaypointId = 0;
        }

        m_uiMoveNextTimer = 500;
    }

    //used when open portal and spawn mobs in phase
    void DoRaidWhisper(int32 iTextId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    DoScriptText(iTextId, m_creature, i->getSource());
            }
        }
    }

    //"opens" the portal and does the "opening" whisper
    void OpenPortal()
    {
        int32 iTextId = 0;

        //there are 4 portal spawn locations, each are expected to be spawned with negative spawntimesecs in database

        //using a grid search here seem to be more efficient than caching all four guids
        //in instance script and calculate range to each.
        GameObject* pPortal = GetClosestGameObjectWithEntry(m_creature,GO_TWILIGHT_PORTAL,50.0f);

        switch(m_creature->GetEntry())
        {
        case NPC_TENEBRON:
            iTextId = WHISPER_HATCH_EGGS;
            break;
        case NPC_SHADRON:
            iTextId = WHISPER_OPEN_PORTAL;
            break;
        case NPC_VESPERON:
            iTextId = WHISPER_OPEN_PORTAL;
            break;
        }

        DoRaidWhisper(iTextId);

        //By using SetRespawnTime() we will actually "spawn" the object with our defined time.
        //Once time is up, portal will disappear again.
        if (pPortal && !pPortal->isSpawned())
            pPortal->SetRespawnTime(m_iPortalRespawnTime);

        //Unclear what are expected to happen if one drake has a portal open already
        //Refresh respawnTime so time again are set to 30secs?
    }

    //Removes each drakes unique debuff from players
    void RemoveDebuff(uint32 uiSpellId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(uiSpellId))
                    i->getSource()->RemoveAurasDueToSpell(uiSpellId);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        int32 iTextId = 0;
        uint32 uiSpellId = 0;

        switch(m_creature->GetEntry())
        {
        case NPC_TENEBRON:
            iTextId = SAY_TENEBRON_DEATH;
            uiSpellId = SPELL_POWER_OF_TENEBRON;
            break;
        case NPC_SHADRON:
            iTextId = SAY_SHADRON_DEATH;
            uiSpellId = SPELL_POWER_OF_SHADRON;
            break;
        case NPC_VESPERON:
            iTextId = SAY_VESPERON_DEATH;
            uiSpellId = SPELL_POWER_OF_VESPERON;
            break;
        }

        DoScriptText(iTextId, m_creature);

        RemoveDebuff(uiSpellId);

        if (m_pInstance)
        {
            // not if solo mini-boss fight
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
                return;

            // Twilight Revenge to main boss
            if (Unit* pSartharion = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SARTHARION)))
            {
                if (pSartharion->isAlive())
                    pSartharion->CastSpell(pSartharion,SPELL_TWILIGHT_REVENGE,true);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanMoveFree && m_uiMoveNextTimer)
        {
            if (m_uiMoveNextTimer <= uiDiff)
            {
                m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId,
                    m_aDragonCommon[m_uiWaypointId].m_fX, m_aDragonCommon[m_uiWaypointId].m_fY, m_aDragonCommon[m_uiWaypointId].m_fZ);

                debug_log("dummy_dragonAI: %s moving to point %u", m_creature->GetName(), m_uiWaypointId);
                m_uiMoveNextTimer = 0;
            }
            else
                m_uiMoveNextTimer -= uiDiff;
        }
    }
};

/*######
## Mob Tenebron
######*/
struct MANGOS_DLL_DECL mob_tenebronAI : public dummy_dragonAI
{
    mob_tenebronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiSummonEggTimer;
    uint32 m_uiHatchEggsTimer;

    std::list<uint64> m_lEggsGUIDList;
    std::list<uint64> m_lWhelpsGUIDList;

    void Reset()
    {
        m_uiShadowBreathTimer = 20000;
        m_uiShadowFissureTimer = 5000;
        m_uiSummonEggTimer = 20000;

        m_lEggsGUIDList.clear();
        m_lWhelpsGUIDList.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_TENEBRON_AGGRO, m_creature);

        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCast(m_creature, SPELL_POWER_OF_TENEBRON);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_TENEBRON_SLAY_1 : SAY_TENEBRON_SLAY_2, m_creature);
    }

    void HatchEggs()
    {
        uint32 m_uiWhelpId = 0;
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            m_uiWhelpId = NPC_SARTHARION_TWILIGHT_WHELP;
        else
            m_uiWhelpId = NPC_TWILIGHT_WHELP;

        if (!m_lEggsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lEggsGUIDList.begin(); itr != m_lEggsGUIDList.end(); ++itr)
            {
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                {
                    if (pTemp->isAlive())
                    {
                        if(Creature* pWhelp = m_creature->SummonCreature(m_uiWhelpId, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ(), pTemp->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            pWhelp->SetInCombatWithZone();
                    }
                }
            }
        }
        if (!m_lWhelpsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lWhelpsGUIDList.begin(); itr != m_lWhelpsGUIDList.end(); ++itr)
            {
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                {
                    if (pTemp->isAlive())
                    {
                        if(Creature* pWhelp = m_creature->SummonCreature(m_uiWhelpId, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ(), pTemp->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            pWhelp->SetInCombatWithZone();
                    }
                }
            }
        }
        m_lEggsGUIDList.clear();
        m_lWhelpsGUIDList.clear();
        m_uiSummonEggTimer = 20000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);
            m_uiShadowFissureTimer = urand(15000, 20000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_TENEBRON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(20000, 25000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        // twilight eggs
        if (m_uiSummonEggTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_TENEBRON_SPECIAL_1 : SAY_TENEBRON_SPECIAL_2, m_creature);
            OpenPortal();

            uint32 m_uiEggId = 0;
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                m_uiEggId = NPC_SARTHARION_TWILIGHT_EGG;
            else
                m_uiEggId = NPC_TWILIGHT_EGG;

            uint32 m_uiWhelpId = 0;
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                m_uiWhelpId = NPC_SARTHARION_TWILIGHT_WHELP;
            else
                m_uiWhelpId = NPC_TWILIGHT_WHELP;

            SpellEntry* spell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_TWILIGHT_SHIFT_ENTER);

            for (uint8 i = 0; i < 6; ++i)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Creature* pEgg = m_creature->SummonCreature(m_uiEggId, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 1000))
                        m_lEggsGUIDList.push_back(pEgg->GetGUID());
                    if(Creature* pWhelp = m_creature->SummonCreature(m_uiWhelpId, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
						pWhelp->CastSpell(pWhelp, SPELL_TWILIGHT_SHIFT_ENTER, false);
                }
            }
            m_uiSummonEggTimer = 60000;
            m_uiHatchEggsTimer = 15000;
        }
        else
            m_uiSummonEggTimer -= uiDiff;

        if (m_uiHatchEggsTimer < uiDiff)
        {
            HatchEggs();
            m_uiHatchEggsTimer = 60000;
        }
        else m_uiHatchEggsTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_tenebron(Creature* pCreature)
{
    return new mob_tenebronAI(pCreature);
}

/*######
## Mob Shadron
######*/
struct MANGOS_DLL_DECL mob_shadronAI : public dummy_dragonAI
{
    mob_shadronAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteShadronTimer;
    uint32 m_uiPortalTimer;
    uint64 m_uiDiscipleGUID;

    void Reset()
    {
        m_uiShadowBreathTimer = 20000;
        m_uiShadowFissureTimer = 5000;
        m_uiAcolyteShadronTimer = 25000;
        m_uiPortalTimer = 20000;
        m_uiDiscipleGUID = 0;

        if (m_creature->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
            m_creature->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SHADRON_AGGRO,m_creature);

        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCast(m_creature, SPELL_POWER_OF_SHADRON);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SHADRON_SLAY_1 : SAY_SHADRON_SLAY_2, m_creature);
    }

    void SummonAcolyte()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_SHADRON_DICIPLE, m_creature,i->getSource());
                }
            }
        }
    }

    void KillDisciple()
    {
        if(Unit* pDisciple = Unit::GetUnit(*m_creature, m_uiDiscipleGUID))
            pDisciple->DealDamage(pDisciple, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        m_creature->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
        m_uiPortalTimer = 20000;
        m_uiAcolyteShadronTimer = 30000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);
            m_uiShadowFissureTimer = urand(15000, 20000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_SHADRON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(20000, 25000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        // portal
        if (m_uiPortalTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_SHADRON_SPECIAL_1 : SAY_SHADRON_SPECIAL_2, m_creature);
            OpenPortal();
            m_uiAcolyteShadronTimer = urand(1000, 3000);
            m_uiPortalTimer = 60000;
        }
        else
            m_uiPortalTimer -= uiDiff;

        // acolyte of shadron
        if (m_uiAcolyteShadronTimer < uiDiff)
        {
            SummonAcolyte();

            // twilight acolyte
            Creature *Acolyte = m_creature->SummonCreature(NPC_ACOLYTE_OF_SHADRON, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
            Acolyte->SetInCombatWithZone();

            // dummy acolyte
            if(Creature* pDisciple = m_creature->SummonCreature(NPC_DISCIPLE_OF_SHADRON, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
            {
                m_uiDiscipleGUID = pDisciple->GetGUID();
                pDisciple->CombatStop();
                pDisciple->GetMotionMaster()->MoveIdle();
                pDisciple->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pDisciple->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pDisciple->SetDisplayId(11686);     // make invisible

                if (m_pInstance)
                {
                    //if not solo figth, buff main boss, else place debuff on mini-boss. both spells TARGET_SCRIPT
                    if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                        pDisciple->CastSpell(pDisciple, SPELL_GIFT_OF_TWILIGTH_SAR, false);
                    else
                        pDisciple->CastSpell(pDisciple, SPELL_GIFT_OF_TWILIGTH_SHA, false);
                }
            }

            m_uiAcolyteShadronTimer = 60000;
        }
        else
            m_uiAcolyteShadronTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shadron(Creature* pCreature)
{
    return new mob_shadronAI(pCreature);
}

/*######
## Mob Vesperon
######*/
struct MANGOS_DLL_DECL mob_vesperonAI : public dummy_dragonAI
{
    mob_vesperonAI(Creature* pCreature) : dummy_dragonAI(pCreature) { Reset(); }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteVesperonTimer;
    uint32 m_uiPortalTimer;
    uint64 m_uiDiscipleGUID;

    void Reset()
    {
        m_uiShadowBreathTimer = 20000;
        m_uiShadowFissureTimer = 5000;
        m_uiAcolyteVesperonTimer = 25000;
        m_uiPortalTimer = 20000;
        m_uiDiscipleGUID = 0;

        if (m_creature->HasAura(SPELL_TWILIGHT_TORMENT_VESP))
            m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_VESPERON_AGGRO,m_creature);

        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            DoCast(m_creature, SPELL_POWER_OF_VESPERON);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_VESPERON_SLAY_1 : SAY_VESPERON_SLAY_2, m_creature);
    }

    void KillDisciple()
    {
        if(Unit* pDisciple = Unit::GetUnit(*m_creature, m_uiDiscipleGUID))
            pDisciple->DealDamage(pDisciple, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);
        m_uiPortalTimer = 20000;
        m_uiAcolyteVesperonTimer = 30000;
    }

    void SummonAcolyte()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_VESPERON_DICIPLE, m_creature,i->getSource());
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H);
            m_uiShadowFissureTimer = urand(15000, 20000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            DoScriptText(SAY_VESPERON_BREATH, m_creature);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H);
            m_uiShadowBreathTimer = urand(20000, 25000);
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        // portal
        if (m_uiPortalTimer < uiDiff)
        {
            DoScriptText(urand(0, 1) ? SAY_VESPERON_SPECIAL_1 : SAY_VESPERON_SPECIAL_2, m_creature);
            OpenPortal();
            m_uiAcolyteVesperonTimer = urand(1000, 3000);
            m_uiPortalTimer = 60000;
        }
        else
            m_uiPortalTimer -= uiDiff;

        // acolyte of vesperon
        if (m_uiAcolyteVesperonTimer < uiDiff)
        {
            SummonAcolyte();

            Creature *Acolyte = m_creature->SummonCreature(NPC_ACOLYTE_OF_VESPERON, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000);
            Acolyte->SetInCombatWithZone();

            // dummy acolyte
            if(Creature* pDisciple = m_creature->SummonCreature(NPC_DISCIPLE_OF_VESPERON, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
            {
                m_uiDiscipleGUID = pDisciple->GetGUID();
                pDisciple->GetMotionMaster()->MoveIdle();
                pDisciple->CombatStop();
                pDisciple->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pDisciple->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                pDisciple->SetDisplayId(11686);     // make invisible
                pDisciple->CastSpell(pDisciple, SPELL_TWILIGHT_TORMENT_VESP, false);
            }
            m_uiAcolyteVesperonTimer = 60000;
        }
        else
            m_uiAcolyteVesperonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vesperon(Creature* pCreature)
{
    return new mob_vesperonAI(pCreature);
}

/*######
## Mob Acolyte of Shadron
######*/
struct MANGOS_DLL_DECL mob_acolyte_of_shadronAI : public ScriptedAI
{
    mob_acolyte_of_shadronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
		DoCast(m_creature, SPELL_TWILIGHT_SHIFT_ENTER);
    }

    void DoCastResidue()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_TWILIGHT_SHIFT_ENTER, EFFECT_INDEX_0))
                    i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_RESIDUE, true);
            }
        } 
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {
            Creature* pDebuffTarget = NULL;

            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            {
                //not solo fight, so main boss has deduff
                pDebuffTarget = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SARTHARION));

                if (pDebuffTarget && pDebuffTarget->isAlive() && pDebuffTarget->HasAura(SPELL_GIFT_OF_TWILIGTH_SAR))
                    pDebuffTarget->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SAR);
            }
            else
            {
                //event not in progress, then solo fight and must remove debuff mini-boss
                pDebuffTarget = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SHADRON));

                if (pDebuffTarget && pDebuffTarget->isAlive() && pDebuffTarget->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
                    pDebuffTarget->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
            }

            DoCastResidue();
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT_ENTER);

            if(Creature* pShadron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_SHADRON)))
                ((mob_shadronAI*)pShadron->AI())->KillDisciple();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_shadron(Creature* pCreature)
{
    return new mob_acolyte_of_shadronAI(pCreature);
}

/*######
## Mob Acolyte of Vesperon
######*/
struct MANGOS_DLL_DECL mob_acolyte_of_vesperonAI : public ScriptedAI
{
    mob_acolyte_of_vesperonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {    
        DoCast(m_creature, SPELL_TWILIGHT_SHIFT_ENTER);
    }

    void DoCastResidue()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_TWILIGHT_SHIFT_ENTER, EFFECT_INDEX_0))
                    i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_RESIDUE, true);
            }
        } 
    }

    void JustDied(Unit* pKiller)
    {
        // remove twilight torment on Vesperon
        if (m_pInstance)
        {
            Creature* pVesperon = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_VESPERON));

            if (pVesperon && pVesperon->isAlive() && pVesperon->HasAura(SPELL_TWILIGHT_TORMENT_VESP))
                pVesperon->RemoveAurasDueToSpell(SPELL_TWILIGHT_TORMENT_VESP);

            DoCastResidue();
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_TORMENT_AURA);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT_ENTER);

            if(Creature* pVesperon = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_VESPERON)))
                ((mob_vesperonAI*)pVesperon->AI())->KillDisciple();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_vesperon(Creature* pCreature)
{
    return new mob_acolyte_of_vesperonAI(pCreature);
}

/*######
## Mob Twilight Whelps
######*/
struct MANGOS_DLL_DECL mob_twilight_whelpAI : public ScriptedAI
{
    mob_twilight_whelpAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        Reset(); 
    }

    uint32 m_uiFadeArmorTimer;

    void Reset()
    {
        m_uiFadeArmorTimer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFadeArmorTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FADE_ARMOR);
            m_uiFadeArmorTimer = urand(5000, 10000);
        }
        else
            m_uiFadeArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_twilight_whelp(Creature* pCreature)
{
    return new mob_twilight_whelpAI(pCreature);
}

/*######
## Mob Twilight Eggs
######*/
struct MANGOS_DLL_DECL mob_twilight_eggsAI : public ScriptedAI
{
    mob_twilight_eggsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        SetCombatMovement(false);
        Reset(); 
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSummonTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiSummonTimer = 14000;
        m_uiDieTimer    = 15000;

        DoCast(m_creature, SPELL_TWILIGHT_SHIFT_ENTER);
    }

    void JustDied(Unit* killer)
    {
        if(!GetOtherEggs())
        {
            EjectPlayers();
            if(Creature* pTenebron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_TENEBRON)))
                ((mob_tenebronAI*)pTenebron->AI())->HatchEggs();
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->SetInCombatWithZone();
    }

    bool GetOtherEggs()
    {
        if(GetClosestCreatureWithEntry(m_creature, NPC_SARTHARION_TWILIGHT_EGG, 100.0f))
            return true;
        if(GetClosestCreatureWithEntry(m_creature, NPC_TWILIGHT_EGG, 100.0f))
            return true;
 
        return false;
    }

    void AttackStart(Unit* pWho) 
    {
        return;
    }

    void DoCastResidue()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_TWILIGHT_SHIFT_ENTER, EFFECT_INDEX_0))
                    i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_RESIDUE, true);
            }
        } 
    }

    void EjectPlayers()
    {
        DoCastResidue();
        if(m_pInstance)
        {
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_SHIFT_ENTER);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiSummonTimer < uiDiff)
        {
            if(m_pInstance->GetData(TYPE_SARTHARION_EVENT == IN_PROGRESS))
                DoCast(m_creature, SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP);
            else
                DoCast(m_creature, SPELL_SUMMON_TWILIGHT_WHELP);
            m_uiSummonTimer = 5000;
        }
        else
            m_uiSummonTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff)
        {
            EjectPlayers();
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_twilight_eggs(Creature* pCreature)
{
    return new mob_twilight_eggsAI(pCreature);
}


/*######
## Mob Twilight Fissure
######*/
struct MANGOS_DLL_DECL mob_twilight_fissureAI : public ScriptedAI
{
    mob_twilight_fissureAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);
        pCreature->setFaction(14);
        Reset();         
    }

    bool m_bIsRegularMode;
    uint32 m_uiVoidBlast;
    uint32 m_uiDieTimer;
    
    void Reset()
    {
        m_uiVoidBlast = 5000;
        m_uiDieTimer  = 7000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiVoidBlast < uiDiff)
        {
            DoCast(m_creature,m_bIsRegularMode ? SPELL_VOID_BLAST : SPELL_VOID_BLAST_H);
            m_uiVoidBlast = 5000;
        }
        else m_uiVoidBlast -= uiDiff;

        if (m_uiDieTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_twilight_fissure(Creature* pCreature)
{
    return new mob_twilight_fissureAI(pCreature);
}

/*######
## Mob Flame Tsunami
######*/
struct MANGOS_DLL_DECL mob_flame_tsunamiAI : public ScriptedAI
{
    mob_flame_tsunamiAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->setFaction(14);
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 m_uiDamageTimer;
    uint32 m_uiMoveTimer;
    bool m_bHasAura;

    // workaround
    uint32 m_uiLeapTimer;

    void Reset()
    {
        m_uiDamageTimer = 1000;
        m_bHasAura      = false;
        m_uiMoveTimer   = 5000;
        m_uiLeapTimer   = 5000;
        DoCast(m_creature, SPELL_FLAME_TSUNAMI_VISUAL);    
    }

    void AttackStart(Unit *who) 
    { 
        return; 
    }

    void DoLeap()
    {
        // dmg
        DoCast(m_creature, SPELL_FLAME_TSUNAMI_DMG, true);
        // knockgack
        Map* pMap = m_creature->GetMap();
        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();
            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                    {
                        if (m_creature->GetDistance2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) <= 7)
                            i->getSource()->CastSpell(i->getSource(), SPELL_FLAME_TSUNAMI_KNOCK, true);
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDamageTimer < uiDiff && !m_bHasAura)
        {
            //DoCast(m_creature, SPELL_FLAME_TSUNAMI_DMG_AURA);  // broken
            m_bHasAura = true;
        }
        else m_uiDamageTimer -= uiDiff;

        if (m_uiLeapTimer < uiDiff)
        {
            DoLeap();
            m_uiLeapTimer = 1000;
        }
        else m_uiLeapTimer -= uiDiff;

        if (m_uiMoveTimer <= uiDiff)
        {
            if (m_creature->GetPositionX() < 3205)
            {
                m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_creature->GetPositionX()+84.0f, m_creature->GetPositionY(), m_creature->GetPositionZ());
            }
            else
            {
                m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_creature->GetPositionX()-84.0f, m_creature->GetPositionY(), m_creature->GetPositionZ());
            }
            m_uiMoveTimer = 60000;
        }
        else m_uiMoveTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_flame_tsunami(Creature* pCreature)
{
    return new mob_flame_tsunamiAI(pCreature);
}
 
/*######
## Mob Onyx Flight Captain
######*/
struct MANGOS_DLL_DECL mob_onyx_flight_captainAI : public ScriptedAI
{
    mob_onyx_flight_captainAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
    uint32 m_uiPummpelTimer, m_uiHammmerTimer;
    bool m_uiAggro;
    ScriptedInstance* m_pInstance;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiPummpelTimer = 10000;
        m_uiHammmerTimer = 7000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            {
                if (Unit* pSart = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_SARTHARION)))
                {
                    if (pSart->getVictim())
                    {
                        m_creature->AI()->AttackStart(pSart->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiPummpelTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(),58953,true);
            m_uiPummpelTimer = 10000;
        }else m_uiPummpelTimer -= uiDiff;
 
        if (m_uiHammmerTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(),57759,true);
            m_uiHammmerTimer = 7000;
        }else m_uiHammmerTimer -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};
 
CreatureAI* GetAI_mob_onyx_flight_captain(Creature* pCreature)
{
    return new mob_onyx_flight_captainAI(pCreature);
}

/*######
## Mob Onyx Sanctum Guardian
######*/
struct MANGOS_DLL_DECL mob_onyx_sanctum_guardianAI : public ScriptedAI
{
    mob_onyx_sanctum_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();         
    }
 
    uint32 m_uiCurseTimer, m_uiShockwaveTimer;
    bool m_bIsRegularMode;
    bool m_uiAggro;
    bool m_bHasEnraged;
    ScriptedInstance* m_pInstance;
 
    void Aggro(Unit* who)
    {
        m_creature->CallAssistance();
    }
 
    void Reset()
    {
        m_uiAggro = false;
        m_uiCurseTimer = 10000;
        m_uiShockwaveTimer = 7000;
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && !m_uiAggro)
        {
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
            {
                if (Unit* pSart = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_SARTHARION)))
                {
                    if (pSart->getVictim())
                    {
                        m_creature->AI()->AttackStart(pSart->getVictim());
                        m_uiAggro = true;
                    }
                }
            }
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
 
        if (m_uiCurseTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget,39647);
            m_uiCurseTimer = 10000;
        }else m_uiCurseTimer -= uiDiff;
 
        if (m_uiShockwaveTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(),m_bIsRegularMode ? 57728 : 58947);
            m_uiShockwaveTimer = 7000;
        }else m_uiShockwaveTimer -= uiDiff;

        if(m_creature->GetHealthPercent() < 30.0f && !m_bHasEnraged)
        {
            DoCast(m_creature, 53801);
            m_bHasEnraged = true;
        }
 
        DoMeleeAttackIfReady();
    }
};
 
CreatureAI* GetAI_mob_onyx_sanctum_guardian(Creature* pCreature)
{
    return new mob_onyx_sanctum_guardianAI(pCreature);
}

/*######
## Mob Flame Tornado
######*/
struct MANGOS_DLL_DECL npc_flame_tornadoAI : public ScriptedAI
{
    npc_flame_tornadoAI(Creature* pCreature) : ScriptedAI(pCreature)
    { 
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        Reset(); 
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 CX;
    uint32 CY;
    uint32 m_uiMovementTimer;
    uint32 m_uiMovingSteps;

    void Reset()
    {
        m_uiStep = 1;
        m_uiStepTimer = urand(5000, 20000);

        CX = m_creature->GetPositionX();
        CY = m_creature->GetPositionY();

        m_uiMovementTimer = 1000 + rand()%2000;
        m_uiMovingSteps = 0;

        DoCast(m_creature, SPELL_TORNADO_VISUAL);
    }

    void AttackStart(Unit *who) 
    { 
        return; 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
        {
            switch(m_uiStep)
            {
            case 1:
                m_creature->SetInCombatWithZone();
                //DoCast(m_creature,SPELL_TORNADO_SCALE);
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 2);
                ++m_uiStep;
                m_uiStepTimer = urand(2000, 6000);
                break;
            case 3:
                if (Creature* pSartharion = (Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SARTHARION)))
                {
                    // TODO: Fix lava blaze!!!
                    /*if (Unit* target = pSartharion->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(target,SPELL_LAVA_STRIKE);

                    switch(urand(0, 15))
                    {
                    case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, pSartharion); break;
                    case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, pSartharion); break;
                    case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, pSartharion); break;
                    }*/
                }
                ++m_uiStep;
                m_uiStepTimer = 1000;
                break;
            case 5:
                //m_creature->RemoveAurasDueToSpell(SPELL_TORNADO_SCALE);
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1);
                ++m_uiStep;
                m_uiStepTimer = 1000;
                break;
            case 7:
                m_uiStepTimer = urand(5000, 20000);
                m_uiStep = 1;
                break;
            }

            if (m_uiStepTimer <= uiDiff)
            {
                ++m_uiStep;
            } 
            m_uiStepTimer -= uiDiff;

            // movement
            if(m_uiMovementTimer < uiDiff)
            {
                if(Creature* pSartharion = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SARTHARION))))
                {
                    float angle = (M_PI / 10) * m_uiMovingSteps;
                    float X = CX + 1 * cos(angle);
                    float Y = CY + 1 * sin(angle);

                    m_creature->GetMotionMaster()->MovementExpired(false);
                    m_creature->GetMotionMaster()->MovePoint(0, X, Y, m_creature->GetPositionZ());

                    m_uiMovingSteps++;
                    if (m_uiMovingSteps > 19)
                        m_uiMovingSteps = 0;
                }
                m_uiMovementTimer = 1000;
            }
            else m_uiMovementTimer -= uiDiff;
        } 
        else 
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }
};

CreatureAI* GetAI_npc_flame_tornado(Creature* pCreature)
{
    return new npc_flame_tornadoAI(pCreature);
}

void AddSC_boss_sartharion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sartharion";
    newscript->GetAI = &GetAI_boss_sartharion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vesperon";
    newscript->GetAI = &GetAI_mob_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadron";
    newscript->GetAI = &GetAI_mob_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_tenebron";
    newscript->GetAI = &GetAI_mob_tenebron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_shadron";
    newscript->GetAI = &GetAI_mob_acolyte_of_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_vesperon";
    newscript->GetAI = &GetAI_mob_acolyte_of_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_eggs";
    newscript->GetAI = &GetAI_mob_twilight_eggs;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_whelp";
    newscript->GetAI = &GetAI_mob_twilight_whelp;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_tsunami";
    newscript->GetAI = &GetAI_mob_flame_tsunami;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_flame_tornado";
    newscript->GetAI = &GetAI_npc_flame_tornado;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name = "mob_twilight_fissure";
    newscript->GetAI = &GetAI_mob_twilight_fissure;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name = "mob_onyx_flight_captain";
    newscript->GetAI = &GetAI_mob_onyx_flight_captain;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_onyx_sanctum_guardian";
    newscript->GetAI = &GetAI_mob_onyx_sanctum_guardian;
    newscript->RegisterSelf();
}
