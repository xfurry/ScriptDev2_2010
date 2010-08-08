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

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    /***** SPELLS *****/
    // Ymirjar Wrathbringer
    SPELL_BLIGHT                        = 69603,
    SPELL_BLIGHT_H                      = 70285,

    //Ymirjar Skycaller
    SPELL_FROSTBLADE                    = 70291,
    SPELL_GLACIAL_STRIKE                = 70292,

    //Ymirjar Flamebearer
    SPELL_FIREBALL                      = 69583,
    SPELL_FIREBALL_H                    = 70282,
    SPELL_HELLFIRE                      = 69586,
    SPELL_HELLFIRE_H                    = 70283,
    SPELL_TACTICAL_BLINK                = 69584,

    //Ymirjar Deathbringer
    SPELL_EMPOWERED_SHADOW_BOLT         = 69528,
    SPELL_EMPOWERED_SHADOW_BOLT_H       = 70281,
    SPELL_SUMMON_UNDEAD                 = 69516,

    //Wrathbone Laborer
    SPELL_BLINDING_DIRT                 = 70302,
    SPELL_PUNCTURE_WOUND                = 70278,
    SPELL_PUNCTURE_WOUND_H              = 70279,
    SPELL_SHOVELLED                     = 69572,
    SPELL_SHOVELLED_H                   = 70280,

    //Wrathbone Coldwraith
    SPELL_FREEZING_CIRCLE               = 69574,
    SPELL_FREEZING_CIRCLE_H             = 70276,
    SPELL_FROSTBOLT                     = 69573,
    SPELL_FROSTBOLT_H                   = 70277,

    //Stonespine Gargoyle
    SPELL_GARGOYLE_STRIKE               = 69520,
    SPELL_GARGOYLE_STRIKE_H             = 70275,
    SPELL_STONEFORM                     = 69575,

    // Plagueborn Horror
    SPELL_BLIGHT_BOMB                   = 69582,
    SPELL_PUSTULANT_FLESH               = 69581,
    SPELL_PUSTULANT_FLESH_H             = 70273,
    SPELL_TOXIC_WASTE                   = 70274,

    //Iceborn Proto-Drake
    SPELL_FROST_BREATH                  = 69527,
    SPELL_FROST_BREATH_H                = 70272,

    //Hungering Ghoul
    SPELL_DEVOUR_FLESH                  = 70393,

    //Fallen Warrior
    SPELL_ARCING_SLICE                  = 69579,
    SPELL_DEMORALIZING_SHOUT            = 61044,
    SPELL_SHIELD_BLOCK                  = 69580,

    //Deathwhisper Torturer
    SPELL_BLACK_BRAND                   = 70392,
    SPELL_CURSE_OF_AGONY                = 70391,

    //Deathwhisper Shadowcaster
    SPELL_SHADOW_BOLT                   = 70386,
    SPELL_SHADOW_BOLT_H                 = 70387,

    //Deathwhisper Necrolyte
    SPELL_CONVERSION_BEAM               = 69578,
    SPELL_CONVERSION_BEAM_H             = 70269,
    SPELL_SHADOW_BOLT_2                 = 69577,
    SPELL_SHADOW_BOLT_2_H               = 70270,

    //Wrathbone Sorcerer
    SPELL_SHADOW_BOLT_3                 = 70386,
    SPELL_SHADOW_BOLT_3_H               = 70387,

    //Geist Ambusher
    SPELL_LEAPING_FACE_MAUL             = 69504,
    SPELL_LEAPING_FACE_MAUL_H           = 70271,

    //Disturbed glacial revenant
    SPELL_AVALANCHE                     = 55216,

    ACHIEV_DONT_LOOK_UP                 = 4525, //needs script support -> area trigger and icicles
};

struct MANGOS_DLL_DECL npc_YmirjarWrathbringerAI : public ScriptedAI
{
    npc_YmirjarWrathbringerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBlightTimer;

    void Reset()
    {
        m_uiBlightTimer = 7000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlightTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_BLIGHT : SPELL_BLIGHT_H);
            m_uiBlightTimer = 8000;
        }
        else
            m_uiBlightTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_YmirjarSkyCallerAI: public ScriptedAI
{
    npc_YmirjarSkyCallerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFrostBladeTimer;
    uint32 m_uiGlacialStrikeTimer;

    void Reset()
    {
        m_uiFrostBladeTimer = 1000;
        m_uiGlacialStrikeTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostBladeTimer < uiDiff)
        {
            //DoCast(m_creature, SPELL_FROSTBLADE);
            m_uiFrostBladeTimer = 25000;
        }
        else
            m_uiFrostBladeTimer -= uiDiff;

        if (m_uiGlacialStrikeTimer < uiDiff)
        {
            //DoCast(m_creature->getVictim(), SPELL_GLACIAL_STRIKE);
            m_uiGlacialStrikeTimer = 8000;
        }
        else
            m_uiGlacialStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_YmirjarFlamebearerAI: public ScriptedAI
{
    npc_YmirjarFlamebearerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFireballTimer;
    uint32 m_uiHellfireTimer;
    uint32 m_uiTacticalBlinkTimer;

    void Reset()
    {
        m_uiFireballTimer       = 4000;
        m_uiHellfireTimer       = 8000;
        m_uiTacticalBlinkTimer  = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireballTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireballTimer = 5000;
        }
        else
            m_uiFireballTimer -= uiDiff;

        if (m_uiTacticalBlinkTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_TACTICAL_BLINK);
            m_uiTacticalBlinkTimer = 12000;
        }
        else
            m_uiTacticalBlinkTimer -= uiDiff;

        if (m_uiHellfireTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_HELLFIRE : SPELL_HELLFIRE_H);
            m_uiHellfireTimer = 10000;
        }
        else
            m_uiHellfireTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_YmirjarDeathbringerAI: public ScriptedAI
{
    npc_YmirjarDeathbringerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEmpoweredShadowBoltTimer;

    void Reset()
    {
        m_uiEmpoweredShadowBoltTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEmpoweredShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_EMPOWERED_SHADOW_BOLT : SPELL_EMPOWERED_SHADOW_BOLT_H);
            m_uiEmpoweredShadowBoltTimer = 8000;
        }
        else
            m_uiEmpoweredShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_WrathboneLaborerAI: public ScriptedAI
{
    npc_WrathboneLaborerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBlindingDirtTimer;
    uint32 m_uiPunctureWoundTimer;
    uint32 m_uiShovelledTimer;

    void Reset()
    {
        m_uiBlindingDirtTimer   = 8000;
        m_uiPunctureWoundTimer  = 9000;
        m_uiShovelledTimer      = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlindingDirtTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_BLINDING_DIRT);
            m_uiBlindingDirtTimer = 10000;
        }
        else
            m_uiBlindingDirtTimer -= uiDiff;

        if (m_uiPunctureWoundTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PUNCTURE_WOUND : SPELL_PUNCTURE_WOUND_H);
            m_uiPunctureWoundTimer = 9000;
        }
        else
            m_uiPunctureWoundTimer -= uiDiff;

        if (m_uiShovelledTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHOVELLED : SPELL_SHOVELLED_H);
            m_uiShovelledTimer = 7000;
        }
        else
            m_uiShovelledTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_WrathboneColdwraithAI: public ScriptedAI
{
    npc_WrathboneColdwraithAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFreezingCircleTimer;
    uint32 m_uiFrostboltTimer;

    void Reset()
    {
        m_uiFreezingCircleTimer = 9000;
        m_uiFrostboltTimer      = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFreezingCircleTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FREEZING_CIRCLE : SPELL_FREEZING_CIRCLE_H);
            m_uiFreezingCircleTimer = 9000;
        }
        else
            m_uiFreezingCircleTimer -= uiDiff;

        if (m_uiFrostboltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FROSTBOLT : SPELL_FROSTBOLT_H);
            m_uiFrostboltTimer = 5000;
        }
        else
            m_uiFrostboltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_StonespineGargoyleAI: public ScriptedAI
{
    npc_StonespineGargoyleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiGargoyleStrikeTimer;
    bool m_bHasSkin;

    void Reset()
    {
        m_uiGargoyleStrikeTimer = 5000;
        m_bHasSkin  = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiGargoyleStrikeTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_GARGOYLE_STRIKE : SPELL_GARGOYLE_STRIKE_H);
            m_uiGargoyleStrikeTimer = 6000;
        }
        else
            m_uiGargoyleStrikeTimer -= uiDiff;

        if(m_creature->GetHealthPercent() < 10 && !m_bHasSkin)
        {
            m_bHasSkin = true;
            DoCast(m_creature, SPELL_STONEFORM);
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_PlaguebornHorrorAI: public ScriptedAI
{
    npc_PlaguebornHorrorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHasBlightBomb;
    uint32 m_uiPustulantFleshTimer;
    uint32 m_uiToxicWasteTimer;

    void Reset()
    {
        m_bHasBlightBomb        = false;
        m_uiPustulantFleshTimer = 5000;
        m_uiToxicWasteTimer     = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPustulantFleshTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_PUSTULANT_FLESH : SPELL_PUSTULANT_FLESH_H);
            m_uiPustulantFleshTimer = 10000;
        }
        else
            m_uiPustulantFleshTimer -= uiDiff;

        if (m_uiToxicWasteTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_TOXIC_WASTE);
            m_uiToxicWasteTimer = 8000;
        }
        else
            m_uiToxicWasteTimer -= uiDiff;

        if(m_creature->GetHealthPercent() < 15 && !m_bHasBlightBomb)
        {
            m_bHasBlightBomb = true;
            DoCast(m_creature, SPELL_BLIGHT_BOMB);
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_IcebornProtoDrakeAI: public ScriptedAI
{
    npc_IcebornProtoDrakeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFrostBreathTimer;

    void Reset()
    {
        m_uiFrostBreathTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostBreathTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BREATH : SPELL_FROST_BREATH_H);
            m_uiFrostBreathTimer = 10000;
        }
        else
            m_uiFrostBreathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_HungeringGhoulAI: public ScriptedAI
{
    npc_HungeringGhoulAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDevourFleshTimer;

    void Reset()
    {
        m_uiDevourFleshTimer = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDevourFleshTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_DEVOUR_FLESH);
            m_uiDevourFleshTimer = 8000;
        }
        else
            m_uiDevourFleshTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_FallenWarriorAI: public ScriptedAI
{
    npc_FallenWarriorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiArcticSliceTimer;
    uint32 m_uiDemoralizingShoutTimer;
    uint32 m_uiShieldBlockTimer;

    void Reset()
    {
        m_uiArcticSliceTimer        = 8000;
        m_uiDemoralizingShoutTimer  = 20000;
        m_uiShieldBlockTimer        = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcticSliceTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCING_SLICE);
            m_uiArcticSliceTimer = 10000;
        }
        else
            m_uiArcticSliceTimer -= uiDiff;

        if (m_uiDemoralizingShoutTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DEMORALIZING_SHOUT);
            m_uiDemoralizingShoutTimer = 20000;
        }
        else
            m_uiDemoralizingShoutTimer -= uiDiff;

        if (m_uiShieldBlockTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHIELD_BLOCK);
            m_uiShieldBlockTimer = 8000;
        }
        else
            m_uiShieldBlockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathwhisperTorturerAI: public ScriptedAI
{
    npc_DeathwhisperTorturerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBlackBrandTimer;
    uint32 m_uiCurseOfAgonyTimer;

    void Reset()
    {
        m_uiBlackBrandTimer     = 10000;
        m_uiCurseOfAgonyTimer   = 6000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBlackBrandTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_BLACK_BRAND);
            m_uiBlackBrandTimer = 10000;
        }
        else
            m_uiBlackBrandTimer -= uiDiff;

        if (m_uiCurseOfAgonyTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CURSE_OF_AGONY);
            m_uiCurseOfAgonyTimer = 13000;
        }
        else
            m_uiCurseOfAgonyTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathwhisperShadowcasterAI: public ScriptedAI
{
    npc_DeathwhisperShadowcasterAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShadowBoltTimer;

    void Reset()
    {
        m_uiShadowBoltTimer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
            m_uiShadowBoltTimer = 5000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathwhisperNecrolyteAI: public ScriptedAI
{
    npc_DeathwhisperNecrolyteAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiConversionBeamTimer;
    uint32 m_uiShadowBoltTimer;

    void Reset()
    {
        m_uiConversionBeamTimer = 12000;
        m_uiShadowBoltTimer     = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiConversionBeamTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CONVERSION_BEAM : SPELL_CONVERSION_BEAM_H);
            m_uiConversionBeamTimer = 12000;
        }
        else
            m_uiConversionBeamTimer -= uiDiff;


        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT_2 : SPELL_SHADOW_BOLT_2_H);
            m_uiShadowBoltTimer = 5000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_WrathboneSorcererAI: public ScriptedAI
{
    npc_WrathboneSorcererAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShadowBoltTimer;

    void Reset()
    {
        m_uiShadowBoltTimer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT_3 : SPELL_SHADOW_BOLT_3_H);
            m_uiShadowBoltTimer = 5000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_GeistAmbusherAI: public ScriptedAI
{
    npc_GeistAmbusherAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiLeapingFaceMaulTimer;

    void Reset()
    {
        m_uiLeapingFaceMaulTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLeapingFaceMaulTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_LEAPING_FACE_MAUL : SPELL_LEAPING_FACE_MAUL_H);
            m_uiLeapingFaceMaulTimer = 5000;
        }
        else
            m_uiLeapingFaceMaulTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// TODO:
enum GaunteletEvent
{
    NPC_COLLAPSING_ICICLE       = 36847,
    SPELL_ICICLE_DUMMY          = 69428,
};

struct MANGOS_DLL_DECL npc_disturbedGlacialRevenantAI: public ScriptedAI
{
    npc_disturbedGlacialRevenantAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiAvalancheTimer;

    void Reset()
    {
        m_uiAvalancheTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAvalancheTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_AVALANCHE);
            m_uiAvalancheTimer = 5000;
        }
        else
            m_uiAvalancheTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/******** JAINA & SYLVANAS *******/
//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {445.941f, 199.304f, 528.707f},
    {452.197f, 214.461f, 528.710f},
    {444.204f, 232.011f, 528.710f},
};

static LocationsXY MoveLoc[]=
{
    {489.709f, 195.165f, 528.709f}, 
    {482.365f, 221.354f, 528.709f}, 
    {475.318f, 253.910f, 528.709f}, 
};

enum
{
    SAY_SPEECH_SYLVANAS1    = -1610047,
    SAY_SPEECH_SYLVANAS2    = -1610048,
    SAY_SPEECH_SYLVANAS3    = -1610049,
    SAY_SPEECH_SYLVANAS4    = -1610050,

    SAY_SPEECH_JAINA1       = -1610042,
    SAY_SPEECH_JAINA2       = -1610043,
    SAY_SPEECH_JAINA3       = -1610044,
    SAY_SPEECH_JAINA4       = -1610045,
    SAY_SPEECH_JAINA5       = -1610046,

    SAY_TYRANNUS1           = -1610200,
    SAY_TYRANNUS2           = -1610201,
    SAY_TYRANNUS3           = -1610202,
    SAY_TYRANNUS4           = -1610203,
    SAY_TYRANNUS5           = -1610204, 
    SPELL_NECROTIC_POWER    = 69347,
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_pos_startAI: public ScriptedAI
{
    npc_sylvanas_jaina_pos_startAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;
    uint32 creatureEntry;

    uint64 m_uiTyrannusGuid;

    void Reset()
    {
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;
        m_uiTyrannusGuid    = 0;
        creatureEntry = m_creature->GetEntry();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_INTRO, NOT_STARTED);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->isInAccessablePlaceFor(m_creature) && !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 50) && m_creature->IsWithinLOSInMap(pWho))
        {
            if(m_pInstance && m_creature->GetPositionZ() > 525.0f)
                if(m_pInstance->GetData(TYPE_INTRO) != DONE)
                {
                    m_bIsIntro = true;
                    m_uiSpeech_Timer = 5000;
                }
        }
    }

    void SummonHordeChampions()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_HORDE, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y - urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 40), MoveLoc[0].y - urand(0, 10), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_HORDE, SummonLoc[1].x + urand(0, 20), SummonLoc[1].y + urand(0, 20), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_HORDE, SummonLoc[2].x + urand(0, 20), SummonLoc[2].y + urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }
    }

    void SummonAlyChampions()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_ALLIANCE, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y - urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 40), MoveLoc[0].y - urand(0, 10), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_ALLIANCE, SummonLoc[1].x + urand(0, 20), SummonLoc[1].y + urand(0, 20), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_ALLIANCE, SummonLoc[2].x + urand(0, 20), SummonLoc[2].y + urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 40), MoveLoc[2].y + urand(0, 40), MoveLoc[2].z);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_INTRO, IN_PROGRESS);

                    if(Creature* pTyrannus = m_creature->SummonCreature(NPC_TYRANNUS_INTRO, 526.501f, 237.639f, 543.686f, 3.431f, TEMPSUMMON_TIMED_DESPAWN, 40000))
                    {
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->GetMap()->CreatureRelocation(pTyrannus, 526.501f, 237.639f, 543.686f, 3.431f);
                        pTyrannus->SendMonsterMove(526.501f, 237.639f, 543.686f, SPLINETYPE_NORMAL, pTyrannus->GetSplineFlags(), 1);
                        DoScriptText(SAY_TYRANNUS1, pTyrannus);
                        m_uiTyrannusGuid = pTyrannus->GetGUID();
                    }

                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        SummonAlyChampions();
                        break;
                    case NPC_SYLVANAS_START:
                        SummonHordeChampions();
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 1:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA1, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS1, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 2:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS2, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 13000;
                    break;
                case 3:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS3, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 4:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                    {
                        pTyrannus->CastSpell(m_creature, SPELL_NECROTIC_POWER, false);
                        DoScriptText(SAY_TYRANNUS4, pTyrannus);
                    }
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA2, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS2, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 5:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS5, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 6:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA3, m_creature);
                        m_uiSpeech_Timer = 5000;
                        break;
                    case NPC_SYLVANAS_START:
                        m_uiSpeech_Timer = 500;
                        break;
                    }
                    ++m_uiIntro_Phase;
                    break;
                case 7:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA4, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS3, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 8:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA5, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS4, m_creature);
                        break;
                    }
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_INTRO,DONE);
                    ++m_uiIntro_Phase;
                    m_bIsIntro = false;
                    m_uiSpeech_Timer = 10000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

// start tyrannus gauntlet event
bool AreaTrigger_at_tyrannus(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
		if (pInstance->GetData(TYPE_TYRANNUS) == NOT_STARTED && pInstance->GetData(TYPE_KRICK_AND_ICK) == DONE && pInstance->GetData(TYPE_GARFROST) == DONE)
		{
			// summon controller
			pPlayer->SummonCreature(NPC_TYRANNUS,  994.127f, 165.074f, 628.156f, 5.81f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY);
			pInstance->SetData(TYPE_TYRANNUS, SPECIAL);
		}
    }

    return false;
}

CreatureAI* GetAI_npc_YmirjarWrathbringerAI(Creature* pCreature)
{
    return new npc_YmirjarWrathbringerAI (pCreature);
}

CreatureAI* GetAI_npc_YmirjarSkyCallerAI(Creature* pCreature)
{
    return new npc_YmirjarSkyCallerAI (pCreature);
}

CreatureAI* GetAI_npc_YmirjarFlamebearerAI(Creature* pCreature)
{
    return new npc_YmirjarFlamebearerAI (pCreature);
}

CreatureAI* GetAI_npc_YmirjarDeathbringerAI(Creature* pCreature)
{
    return new npc_YmirjarDeathbringerAI (pCreature);
}

CreatureAI* GetAI_npc_WrathboneLaborerAI(Creature* pCreature)
{
    return new npc_WrathboneLaborerAI (pCreature);
}

CreatureAI* GetAI_npc_WrathboneColdwraithAI(Creature* pCreature)
{
    return new npc_WrathboneColdwraithAI (pCreature);
}

CreatureAI* GetAI_npc_StonespineGargoyleAI(Creature* pCreature)
{
    return new npc_StonespineGargoyleAI (pCreature);
}

CreatureAI* GetAI_npc_PlaguebornHorrorAI(Creature* pCreature)
{
    return new npc_PlaguebornHorrorAI (pCreature);
}

CreatureAI* GetAI_npc_IcebornProtoDrakeAI(Creature* pCreature)
{
    return new npc_IcebornProtoDrakeAI (pCreature);
}

CreatureAI* GetAI_npc_HungeringGhoulAI(Creature* pCreature)
{
    return new npc_HungeringGhoulAI (pCreature);
}

CreatureAI* GetAI_npc_FallenWarriorAI(Creature* pCreature)
{
    return new npc_FallenWarriorAI (pCreature);
}

CreatureAI* GetAI_npc_DeathwhisperTorturerAI(Creature* pCreature)
{
    return new npc_DeathwhisperTorturerAI (pCreature);
}

CreatureAI* GetAI_npc_DeathwhisperShadowcasterAI(Creature* pCreature)
{
    return new npc_DeathwhisperShadowcasterAI (pCreature);
}

CreatureAI* GetAI_npc_DeathwhisperNecrolyteAI(Creature* pCreature)
{
    return new npc_DeathwhisperNecrolyteAI (pCreature);
}

CreatureAI* GetAI_npc_WrathboneSorcererAI(Creature* pCreature)
{
    return new npc_WrathboneSorcererAI (pCreature);
}

CreatureAI* GetAI_npc_GeistAmbusherAI(Creature* pCreature)
{
    return new npc_GeistAmbusherAI (pCreature);
}

CreatureAI* GetAI_npc_disturbedGlacialRevenant(Creature* pCreature)
{
    return new npc_disturbedGlacialRevenantAI (pCreature);
}


CreatureAI* GetAI_npc_sylvanas_jaina_pos_start(Creature* pCreature)
{
    return new npc_sylvanas_jaina_pos_startAI (pCreature);
}

void AddSC_Pit_Of_Saron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_YmirjarWrathbringer";
    newscript->GetAI = &GetAI_npc_YmirjarWrathbringerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_YmirjarSkycaller";
    newscript->GetAI = &GetAI_npc_YmirjarSkyCallerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_YmirjarFlamebearer";
    newscript->GetAI = &GetAI_npc_YmirjarFlamebearerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_YmirjarDeathbringer";
    newscript->GetAI = &GetAI_npc_YmirjarDeathbringerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_WrathboneLaborer";
    newscript->GetAI = &GetAI_npc_WrathboneLaborerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_WrathboneColdwraith";
    newscript->GetAI = &GetAI_npc_WrathboneColdwraithAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_StonespineGargoyle";
    newscript->GetAI = &GetAI_npc_StonespineGargoyleAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_PlaguebornHorror";
    newscript->GetAI = &GetAI_npc_PlaguebornHorrorAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_IcebornProtoDrake";
    newscript->GetAI = &GetAI_npc_IcebornProtoDrakeAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_HungeringGhoul";
    newscript->GetAI = &GetAI_npc_HungeringGhoulAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_FallenWarrior";
    newscript->GetAI = &GetAI_npc_FallenWarriorAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathwhisperTorturer";
    newscript->GetAI = &GetAI_npc_DeathwhisperTorturerAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathwhisperShadowcaster";
    newscript->GetAI = &GetAI_npc_DeathwhisperShadowcasterAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathwhisperNecrolyte";
    newscript->GetAI = &GetAI_npc_DeathwhisperNecrolyteAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_WrathboneSorcerer";
    newscript->GetAI = &GetAI_npc_WrathboneSorcererAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_GeistAmbusher";
    newscript->GetAI = &GetAI_npc_GeistAmbusherAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_disturbedGlacialRevenant";
    newscript->GetAI = &GetAI_npc_disturbedGlacialRevenant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_pos_start;
    newscript->Name = "npc_slyvanas_jaina_pos_start";
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "at_tyrannus";
    newscript->pAreaTrigger = &AreaTrigger_at_tyrannus;
    newscript->RegisterSelf();
}