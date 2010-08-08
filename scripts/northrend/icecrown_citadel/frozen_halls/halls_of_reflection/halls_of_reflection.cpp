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
SDName: halls_of_reflection
SD%Complete: 30%
SDComment: 
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"

enum
{
    //frostsworn general
    SPELL_THROW_SHIELD          = 69222,
    SPELL_THROW_SHIELD_H        = 73076,

    //LICH KING MINIONS
    // raging ghoul
    SPELL_LEAP                  = 70150,

    // risen which doctor
    SPELL_CURSE_OF_DOOM         = 70144,
    SPELL_CURSE_OF_DOOM_H       = 70183,
    SPELL_SHADOWBOLT            = 70080,
    SPELL_SHADOWBOLT_H          = 70182,
    SPELL_SHADOWBOLT_VOLLEY     = 70145,
    SPELL_SHADOWBOLT_VOLLEY_H   = 70184,

    // lumbering abomination
    SPELL_CLEAVE                = 40505,

    SAY_LICH_SLAY1              = -1610170,
    SAY_LICH_SLAY2              = -1610171,
    SAY_LICH_WIPE               = -1610172,

    //HALLS OF REFLECTION SOULS
    // tortured rifleman
    SPELL_CURSED_ARROW          = 72222,
    SPELL_FROST_TRAP            = 72215,
    SPELL_ICE_SHOT              = 72268,
    SPELL_ICE_SHOT_H            = 72269,
    SPELL_SHOOT                 = 72208,
    SPELL_SHOOT_H               = 72211,

    // ghostly priest
    SPELL_CIRCLE_OF_DESTRUCTION     = 72320,
    SPELL_CIRCLE_OF_DESTRUCTION_H   = 72323,
    SPELL_COWER_IN_FEAR             = 72321,
    SPELL_DARK_MENDING              = 72322,
    SPELL_DARK_MENDING_H            = 72324, // cast on allies
    SPELL_SHADOW_WORD_PAIN          = 72318,
    SPELL_SHADOW_WORD_PAIN_H        = 72319,

    // phantom mage
    SPELL_CHAINS_OF_ICE         = 72171,
    SPELL_FIREBALL              = 72163,
    SPELL_FIREBALL_H            = 72164,
    SPELL_FLAMESTRIKE           = 72169,
    SPELL_FLAMESTRIKE_H         = 72170,
    SPELL_FROSTBOLT             = 72166,
    SPELL_FROSTBOLT_H           = 72167,
    SPELL_HALLUCINATION_MAGE    = 72342,

    // phamtom hallucination
    // cast: chains of ice, fireball, flamestrike, frostbolt
    SPELL_HALLUCINATION         = 72344, // when dies
    SPELL_HALLUCINATION_H       = 72345,

    // shadowy mercenary
    SPELL_DEADLY_POISON             = 72329,
    SPELL_DEADLY_POISON_H           = 72330,
    SPELL_ENVENOMED_DAGGER_THROW    = 72333,
    SPELL_ENVENOMED_DAGGER_THROW_H  = 72334,
    SPELL_KIDNEY_SHOT               = 72335,
    SPELL_SHADOWSTEP                = 72326,
    SPELL_SHADOWSTEP_H              = 72327,

    // spectral footman
    SPELL_SHIELD_BASH               = 72194,
    SPELL_SHIELD_BASH_H             = 72196,
    SPELL_SPECTRAL_STRIKE           = 72198,
    SPELL_SPECTRAL_STRIKE_H         = 72688,
    SPELL_TORTURED_ENRAGE           = 72203,
};

struct MANGOS_DLL_DECL mob_frostsworn_generalAI : public ScriptedAI
{
    mob_frostsworn_generalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiThrowShieldTimer;

    void Reset()
    {
        m_uiThrowShieldTimer = 10000;
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_FROST_GENERAL, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_MARWYN) == DONE && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiThrowShieldTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_THROW_SHIELD : SPELL_THROW_SHIELD_H);
            m_uiThrowShieldTimer = 8000;
        }
        else
            m_uiThrowShieldTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_lichKing_minionAI : public ScriptedAI
{
    mob_lichKing_minionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 creatureEntry;

    uint32 m_uiLeapTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiCurseOfDoomTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiShadowBoltVolleyTimer;

    void Reset()
    {
        creatureEntry = m_creature->GetEntry();
        m_uiLeapTimer               = 5000;
        m_uiCleaveTimer             = 5000;
        m_uiCurseOfDoomTimer        = 5000;
        m_uiShadowBoltTimer         = 8000;
        m_uiShadowBoltVolleyTimer   = 12000;
    }

    void KilledUnit(Unit *pVictim)
    {
        if(Creature *pLichKing = GetClosestCreatureWithEntry(m_creature, NPC_LICH_KING, 100.0f))
        {
            switch (urand(0, 1))
            {
            case 0:
                DoScriptText(SAY_LICH_SLAY1, pLichKing);
                break;
            case 1:
                DoScriptText(SAY_LICH_SLAY2, pLichKing);
                break;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(creatureEntry)
        {
        case MOB_RAGING_GHOUL:
            RagingGhoul_UpdateAI(uiDiff);
            break;
        case MOB_RISEN_WHITCH_DOCTOR:
            RisenWhichDoctor_UpdateAI(uiDiff);
            break;
        case MOB_LUMBERING_ABONIMATION:
            LumberingAbomination_UpdateAI(uiDiff);
            break;
        }

        DoMeleeAttackIfReady();
    }

    //Raging ghoul
    void RagingGhoul_UpdateAI(const uint32 uiDiff)
    {
        //Leap
        if (m_uiLeapTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_LEAP);
            m_uiLeapTimer = 10000;
        }else m_uiLeapTimer -= uiDiff;
    }

    //Risen Which Doctor
    void RisenWhichDoctor_UpdateAI(const uint32 uiDiff)
    {
        //Curse of doom
        if (m_uiCurseOfDoomTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CURSE_OF_DOOM : SPELL_CURSE_OF_DOOM_H);
            m_uiCurseOfDoomTimer = 10000;
        }else m_uiCurseOfDoomTimer -= uiDiff;

        //Shadowbolt
        if (m_uiShadowBoltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOWBOLT : SPELL_SHADOWBOLT_H);
            m_uiShadowBoltTimer = 5000;
        }else m_uiShadowBoltTimer -= uiDiff;

        //Shadowbolt volley
        if (m_uiShadowBoltVolleyTimer <= uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOWBOLT_VOLLEY : SPELL_SHADOWBOLT_VOLLEY_H);
            m_uiShadowBoltVolleyTimer = 20000;
        }else m_uiShadowBoltVolleyTimer -= uiDiff;
    }

    //Lumbering Abomination
    void LumberingAbomination_UpdateAI(const uint32 uiDiff)
    {
        //Cleave
        if (m_uiCleaveTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 8000;
        }else m_uiCleaveTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_hallsOfReflectionSoulAI : public ScriptedAI
{
    mob_hallsOfReflectionSoulAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    // tortured rifleman
    uint32 m_uiCursedArrowTimer;
    uint32 m_uiFrostTrapTimer;
    uint32 m_uiIceShotTimer;
    uint32 m_uiShootTimer;

    // ghostly priest
    uint32 m_uiCircleOfDestructionTimer;
    uint32 m_uiCowerInFearTimer;
    uint32 m_uiDarkMendingTimer;
    uint32 m_uiShadowWordPainTimer;

    // phantom mage && phantom hallucination
    uint32 m_uiChainsOfIceTimer;
    uint32 m_uiFireballTimer;
    uint32 m_uiFlameStrikeTimer;
    uint32 m_uiFrostboltTimer;
    uint32 m_uiHallucinationTimer;

    // shadowy mercenary
    uint32 m_uiDeadlyPoisonTimer;
    uint32 m_uiDaggerThrowTimer;
    uint32 m_uiKidneyShotTimer;
    uint32 m_uiShadowStepTimer;

    // spectral footman
    uint32 m_uiShieldBashTimer;
    uint32 m_uiSpectralStrikeTimer;
    uint32 m_uiTorturedEnrageTimer;

    uint32 creatureEntry;
    uint32 selfKillTimer;
    bool m_bHasCasted;
	uint32 m_uiExploitCheckTimer;

    void Reset()
    {
        creatureEntry = m_creature->GetEntry();
		m_creature->SetRespawnDelay(DAY);

        // tortured rifleman
        m_uiCursedArrowTimer    = 8000;
        m_uiFrostTrapTimer      = 12000;
        m_uiIceShotTimer        = 10000;
        m_uiShootTimer          = 5000;

        // ghostly priest
        m_uiCircleOfDestructionTimer    = 10000;
        m_uiCowerInFearTimer            = 12000;
        m_uiDarkMendingTimer            = 8000;
        m_uiShadowWordPainTimer         = 5000;

        // phantom mage && phantom hallucination
        m_uiChainsOfIceTimer    = 10000;
        m_uiFireballTimer       = 8000;
        m_uiFlameStrikeTimer    = 15000;
        m_uiFrostboltTimer      = 7000;
        m_uiHallucinationTimer  = 20000;
        m_bHasCasted = false;

        // shadowy mercenary
        m_uiDeadlyPoisonTimer   = 8000;
        m_uiDaggerThrowTimer    = 10000;
        m_uiKidneyShotTimer     = 12000;
        m_uiShadowStepTimer     = 17000;

        // spectral footman
        m_uiShieldBashTimer     = 12000;
        m_uiSpectralStrikeTimer = 8000;
        m_uiTorturedEnrageTimer = 15000;

		m_uiExploitCheckTimer   = 1000;
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth() && !m_bHasCasted && creatureEntry == MOB_PHANTOM_HALLUCINATION)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth());
            DoCast(m_creature, m_bIsRegularMode ? SPELL_HALLUCINATION : SPELL_HALLUCINATION_H);
            uiDamage = 0;
            selfKillTimer = 500;
            m_bHasCasted = true;
        }
    }

	bool IsPlayerInside()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return false;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(GameObject* pAltar = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ALTAR)))
                {
                    if (i->getSource()->isAlive() && i->getSource()->GetDistance2d(pAltar) < 75.0f)
                        return true;
                }
            }
        } 

        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiExploitCheckTimer < uiDiff)
		{
			if(!IsPlayerInside())
				m_creature->ForcedDespawn();
			m_uiExploitCheckTimer = 1000;
		}
		else m_uiExploitCheckTimer -= uiDiff;

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

        switch(creatureEntry)
        {
        case MOB_SPECTRAL_FOOTMAN:
            SpectralFootman_UpdateAI(uiDiff);
            break;
        case MOB_SHADOWY_MERCENARY:
            ShadowyMercenary_UpdateAI(uiDiff);
            break;
        case MOB_PHANTOM_HALLUCINATION:
            PhantomHallucination_UpdateAI(uiDiff);
            break;
        case MOB_PHANTOM_MAGE:
            PhantomMage_UpdateAI(uiDiff);
            break;
        case MOB_GHOSTLY_PRIEST:
            GhostlyPriest_UpdateAI(uiDiff);
            break;
        case MOB_TORTURED_RIFLEMAN:
            TorturedRifleman_UpdateAI(uiDiff);
            break;
        }

        DoMeleeAttackIfReady();
    }

    //Spectral Footman
    void SpectralFootman_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiShieldBashTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHIELD_BASH : SPELL_SHIELD_BASH_H);
            m_uiShieldBashTimer = 12000;
        }else m_uiShieldBashTimer -= uiDiff;

        if (m_uiSpectralStrikeTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SPECTRAL_STRIKE : SPELL_SPECTRAL_STRIKE_H);
            m_uiSpectralStrikeTimer = 8000;
        }else m_uiSpectralStrikeTimer -= uiDiff;

        if (m_uiTorturedEnrageTimer <= uiDiff)
        {
            DoCast(m_creature, SPELL_TORTURED_ENRAGE);
            m_uiTorturedEnrageTimer = 15000;
        }else m_uiTorturedEnrageTimer -= uiDiff;
    }

    //Shadowy Mercenary
    void ShadowyMercenary_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiDeadlyPoisonTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DEADLY_POISON : SPELL_DEADLY_POISON_H);
            m_uiDeadlyPoisonTimer = 8000;
        }else m_uiDeadlyPoisonTimer -= uiDiff;

        if (m_uiDaggerThrowTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ENVENOMED_DAGGER_THROW : SPELL_ENVENOMED_DAGGER_THROW_H);
            m_uiDaggerThrowTimer = 10000;
        }else m_uiDaggerThrowTimer -= uiDiff;

        if (m_uiKidneyShotTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_KIDNEY_SHOT);
            m_uiKidneyShotTimer = 12000;
        }else m_uiKidneyShotTimer -= uiDiff;

        if (m_uiShadowStepTimer <= uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOWSTEP : SPELL_SHADOWSTEP_H);
            m_uiShadowStepTimer = 17000;
        }else m_uiShadowStepTimer -= uiDiff;
    }

    //Phantom Hallucination
    void PhantomHallucination_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiChainsOfIceTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CHAINS_OF_ICE);
            m_uiChainsOfIceTimer = 10000;
        }else m_uiChainsOfIceTimer -= uiDiff;

        if (m_uiFireballTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireballTimer = 8000;
        }else m_uiFireballTimer -= uiDiff;

        if (m_uiFlameStrikeTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FLAMESTRIKE : SPELL_FLAMESTRIKE_H);
            m_uiFlameStrikeTimer = 15000;
        }else m_uiFlameStrikeTimer -= uiDiff;

        if (m_uiFrostboltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FROSTBOLT : SPELL_FROSTBOLT_H);
            m_uiFrostboltTimer = 7000;
        }else m_uiFrostboltTimer -= uiDiff;

        if (selfKillTimer <= uiDiff && m_bHasCasted)
        {
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            selfKillTimer = 8000;
        }else selfKillTimer -= uiDiff;
    }

    //Phantom Mage
    void PhantomMage_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiChainsOfIceTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CHAINS_OF_ICE);
            m_uiChainsOfIceTimer = 10000;
        }else m_uiChainsOfIceTimer -= uiDiff;

        if (m_uiFireballTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireballTimer = 8000;
        }else m_uiFireballTimer -= uiDiff;

        if (m_uiFlameStrikeTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FLAMESTRIKE : SPELL_FLAMESTRIKE_H);
            m_uiFlameStrikeTimer = 15000;
        }else m_uiFlameStrikeTimer -= uiDiff;

        if (m_uiFrostboltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FROSTBOLT : SPELL_FROSTBOLT_H);
            m_uiFrostboltTimer = 7000;
        }else m_uiFrostboltTimer -= uiDiff;

        if (m_uiHallucinationTimer <= uiDiff)
        {
            DoCast(m_creature, SPELL_HALLUCINATION_MAGE);
            m_uiHallucinationTimer = 20000;
        }else m_uiHallucinationTimer -= uiDiff;
    }

    //Ghostly Priest
    void GhostlyPriest_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCircleOfDestructionTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CIRCLE_OF_DESTRUCTION : SPELL_CIRCLE_OF_DESTRUCTION_H);
            m_uiCircleOfDestructionTimer = 10000;
        }else m_uiCircleOfDestructionTimer -= uiDiff;

        if (m_uiCowerInFearTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_COWER_IN_FEAR);
            m_uiCowerInFearTimer = 12000;
        }else m_uiCowerInFearTimer -= uiDiff;

        if (m_uiDarkMendingTimer <= uiDiff)
        {
            if(Unit* pTarget = DoSelectLowestHpFriendly(30.0f))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DARK_MENDING : SPELL_DARK_MENDING_H);
            m_uiDarkMendingTimer = 8000;
        }else m_uiDarkMendingTimer -= uiDiff;

        if (m_uiShadowWordPainTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_WORD_PAIN : SPELL_SHADOW_WORD_PAIN_H);
            m_uiShadowWordPainTimer = 5000;
        }else m_uiShadowWordPainTimer -= uiDiff;
    }

    //Tortured Rifleman
    void TorturedRifleman_UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCursedArrowTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CURSED_ARROW);
            m_uiCursedArrowTimer = 8000;
        }else m_uiCursedArrowTimer -= uiDiff;

        if (m_uiFrostTrapTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_FROST_TRAP);
            m_uiFrostTrapTimer = 12000;
        }else m_uiFrostTrapTimer -= uiDiff;

        if (m_uiIceShotTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICE_SHOT : SPELL_ICE_SHOT_H);
            m_uiIceShotTimer = 10000;
        }else m_uiIceShotTimer -= uiDiff;

        if (m_uiShootTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHOOT : SPELL_SHOOT_H);
            m_uiShootTimer = 5000;
        }else m_uiShootTimer -= uiDiff;
    }
};

/******** JAINA & SYLVANAS *******/

#define GOSSIP_ITEM "Can you remove the sword?"
#define GOSSIP_ITEM_RESET "Can you remove the sword again?" // only if server crashes before the event is finished

enum
{
    SAY_SYLVANAS_INTRO1     = -1610130,
    SAY_SYLVANAS_INTRO2     = -1610131,
    SAY_SYLVANAS_INTRO3     = -1610132,
    SAY_SYLVANAS_INTRO4     = -1610133,
    SAY_SYLVANAS_INTRO5     = -1610134,
    SAY_SYLVANAS_INTRO6     = -1610135,
    SAY_SYLVANAS_INTRO7     = -1610136,
    SAY_SYLVANAS_INTRO8     = -1610137,
    SAY_SYLVANAS_INTRO9     = -1610138,

    SAY_UTHER_HORDE1        = -1610153,
    SAY_UTHER_HORDE2        = -1610154,
    SAY_UTHER_HORDE3        = -1610155,
    SAY_UTHER_HORDE4        = -1610156,
    SAY_UTHER_HORDE5        = -1610157,
    SAY_UTHER_HORDE6        = -1610158,
    SAY_UTHER_HORDE7        = -1610159,

    SAY_LICH_KING_INTRO1    = -1610139,
    SAY_LICH_KING_INTRO2    = -1610140,
    SAY_LICH_KING_INTRO3    = -1610141,
    SAY_LICH_KING_ALY4      = -1610142,
    SAY_LICH_KING_HORDE4    = -1610143,

    SAY_JAINA_INTRO1        = -1610117,
    SAY_JAINA_INTRO2        = -1610118,
    SAY_JAINA_INTRO3        = -1610119,
    SAY_JAINA_INTRO4        = -1610120,
    SAY_JAINA_INTRO5        = -1610121,
    SAY_JAINA_INTRO6        = -1610122,
    SAY_JAINA_INTRO7        = -1610123,
    SAY_JAINA_INTRO8        = -1610124,
    SAY_JAINA_INTRO9        = -1610125,
    SAY_JAINA_INTRO10       = -1610126,
    SAY_JAINA_INTRO11       = -1610127,
    SAY_JAINA_INTRO12       = -1610128,
    SAY_JAINA_INTRO13       = -1610129,

    SAY_UTHER_ALY1          = -1610144,
    SAY_UTHER_ALY2          = -1610145,
    SAY_UTHER_ALY3          = -1610146,
    SAY_UTHER_ALY4          = -1610147,
    SAY_UTHER_ALY5          = -1610148,
    SAY_UTHER_ALY6          = -1610149,
    SAY_UTHER_ALY7          = -1610150,
    SAY_UTHER_ALY8          = -1610151,
    SAY_UTHER_ALY9          = -1610152,

    SAY_INTRO_FALRIC        = -1610102,
    SAY_INTRO_MARWYN        = -1610110,

    SPELL_TAKE_FROSTMOURNE             = 72729,
    SPELL_FROSTMOURNE_DESPAWN          = 72726,
    SPELL_FROSTMOURNE_SOUNDS           = 70667,
    SPELL_CAST_VISUAL                  = 65633,  //Jaina And Sylavana cast this when summon uther.
    SPELL_BOSS_SPAWN_AURA              = 72712,  //Falric and Marwyn
    SPELL_UTHER_DESPAWN                = 70693,
    SPELL_FROSTMOURNE_VISUAL           = 73220,

    NPC_UTHER               = 37225,
    EQUIP_ID_FROSTMOURNE    = 36942,
};

struct MANGOS_DLL_DECL npc_lich_king_hor_startAI: public ScriptedAI
{
    npc_lich_king_hor_startAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    { }

    void UpdateAI(const uint32 uiDiff)
    { }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void EquipFrostmourne()
    {
        //SetEquipmentSlots(false, EQUIP_ID_FROSTMOURNE, -1, -1);
    }
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_hor_startAI: public ScriptedAI
{
    npc_sylvanas_jaina_hor_startAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;
    uint32 creatureEntry;
	uint64 m_uiLichKingGuid;
	uint64 m_uiUtherGuid;
	uint64 m_uiFalricGUID;
	uint64 m_uiMarwynGUID;

    void Reset()
    {
		m_uiLichKingGuid    = 0;
		m_uiUtherGuid       = 0;
		m_uiFalricGUID		= 0;
		m_uiMarwynGUID		= 0;
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;
        creatureEntry = m_creature->GetEntry();
    }

    void StartIntro()
    {
        m_bIsIntro = true;
    }

    void StartEncounter()
    {
        // Falric & Marwyn
		if(Creature* pFalric = m_creature->SummonCreature(NPC_FALRIC, 5270.289f, 2043.269f, 709.3204f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
		{
			m_uiFalricGUID = pFalric->GetGUID();
			pFalric->CastSpell(pFalric, SPELL_BOSS_SPAWN_AURA, false);
			pFalric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
		}
		if(Creature* pMarwyn = m_creature->SummonCreature(NPC_MARWYN, 5348.593f, 1969.527f, 709.3192f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DAY))
		{
			m_uiMarwynGUID = pMarwyn->GetGUID();
			pMarwyn->CastSpell(pMarwyn, SPELL_BOSS_SPAWN_AURA, false);
			pMarwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
		}

        // delete frostmourne
        if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FROSTMOURNE)))
            pFrostmourne->Delete();

        if(m_pInstance && m_pInstance->GetData(TYPE_INTRO) == DONE /*&& m_pInstance->GetData(TYPE_FALRIC) != DONE*/)
            m_pInstance->SetData(TYPE_FALRIC, SPECIAL);

        //if(m_pInstance && m_pInstance->GetData(TYPE_FALRIC) == DONE)
            //m_pInstance->SetData(TYPE_MARWYN, SPECIAL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch (creatureEntry)
                {
                    // JAINA INTRO
                case NPC_JAINA_START:
                    switch(m_uiIntro_Phase)
                    {
                    case 0:
                        DoScriptText(SAY_JAINA_INTRO1, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 1:
                        DoScriptText(SAY_JAINA_INTRO2, m_creature);
                        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(0, 5305.374f, 1997.526f, 709.341f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 10000;
                        break;
                    case 2:
                        DoScriptText(SAY_JAINA_INTRO3, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 3:
                        DoScriptText(SAY_JAINA_INTRO4, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 15000;
                        break;
                    case 4:
                        DoCast(m_creature, SPELL_CAST_VISUAL);
                        DoCast(m_creature, SPELL_FROSTMOURNE_SOUNDS);
                        if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FROSTMOURNE)))
                            m_pInstance->DoUseDoorOrButton(pFrostmourne->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 5:
                        if(Creature* pUther = m_creature->SummonCreature(NPC_UTHER, 5307.018f, 2004.103f, 709.342f, 4.22f, TEMPSUMMON_TIMED_DESPAWN, 150000))
						{
                            pUther->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pUther->GetGUID());
							DoScriptText(SAY_UTHER_ALY1, pUther);
							m_uiUtherGuid = pUther->GetGUID();
						}
                        ++m_uiIntro_Phase;
                        //m_uiIntro_Phase = 20; // for debug
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 6:
                        DoScriptText(SAY_JAINA_INTRO5, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 7:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY2, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 8:
                        DoScriptText(SAY_JAINA_INTRO6, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 9:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY3, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 9000;
                        break;
                    case 10:
                        DoScriptText(SAY_JAINA_INTRO7, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 11:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY4, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 15000;
                        break;
                    case 12:
                        DoScriptText(SAY_JAINA_INTRO8, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 13:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY5, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 13000;
                        break;
                    case 14:
                        DoScriptText(SAY_JAINA_INTRO9, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 13000;
                        break;
                    case 15:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY6, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 25000;
                        break;
                    case 16:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY7, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 17:
                        DoScriptText(SAY_JAINA_INTRO10, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 18:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY8, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 13000;
                        break;
                    case 19:
                        DoScriptText(SAY_JAINA_INTRO11, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 4000;
                        break;
                    case 20:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_ALY9, pUther);
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        if(Creature* pLichKing = m_creature->SummonCreature(NPC_LICH_KING_INTRO, 5364.653f, 2064.413f, 707.695f, 3.85f, TEMPSUMMON_TIMED_DESPAWN, 55000))
						{
							pLichKing->AddSplineFlag(SPLINEFLAG_WALKMODE);
							pLichKing->SetSpeedRate(MOVE_WALK, 1.5f);
							pLichKing->GetMotionMaster()->MovePoint(0, 5334.068f, 2031.866f, 707.694f);
							pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							m_uiLichKingGuid = pLichKing->GetGUID();
						}
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 10000;
                        break;
                    case 21:
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 22:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
						{
							DoScriptText(SAY_LICH_KING_INTRO1, pLichKing);
							pLichKing->GetMotionMaster()->MovePoint(0, 5316.053f, 2013.209f, 709.341f);
						}
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
                        {
                            pUther->CastSpell(pUther, SPELL_UTHER_DESPAWN, false);
							pUther->ForcedDespawn();
                        }
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 23:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
							DoScriptText(SAY_LICH_KING_INTRO2, pLichKing);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 24:
                        if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FROSTMOURNE)))
                            pFrostmourne->Delete();
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
                        {
							((npc_lich_king_hor_startAI*)pLichKing->AI())->EquipFrostmourne();
                            pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, false);
                        }
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 25:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
                        {
							DoScriptText(SAY_LICH_KING_INTRO3, pLichKing);
                            pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, false);
                        }
                        m_creature->RemoveAurasDueToSpell(SPELL_FROSTMOURNE_SOUNDS);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 26:
                        StartEncounter();
						if(Creature* pFalric = m_pInstance->instance->GetCreature(m_uiFalricGUID))
                            DoScriptText(SAY_INTRO_FALRIC, pFalric);
						if(Creature* pMarwyn = m_pInstance->instance->GetCreature(m_uiMarwynGUID))
                            DoScriptText(SAY_INTRO_MARWYN, pMarwyn);
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
							pLichKing->GetMotionMaster()->MovePoint(0, 5364.653f, 2064.413f, 707.695f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 27:
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 28:
                        DoScriptText(SAY_JAINA_INTRO12, m_creature);
                        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(0, 5364.653f, 2064.413f, 707.695f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 12000;
                        break;
                    case 29:
                        if(GameObject* pEnterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ICECROWN_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pEnterDoor->GetGUID());
                        if(GameObject* pExitDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pExitDoor->GetGUID());
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_INTRO, DONE);
                            m_pInstance->SetData(TYPE_FALRIC, SPECIAL);
                        }
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        m_bIsIntro = false;
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;

                    default:
                        m_uiSpeech_Timer = 100000;
                    }
                    break;
                    // SYLVANAS INTRO
                case NPC_SYLVANAS_START:
                    switch(m_uiIntro_Phase)
                    {
                    case 0:
                        DoScriptText(SAY_SYLVANAS_INTRO1, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 1:
                        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(0, 5305.374f, 1997.526f, 709.341f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 10000;
                        break;
                    case 2:
                        DoScriptText(SAY_SYLVANAS_INTRO2, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 3:
                        DoScriptText(SAY_SYLVANAS_INTRO3, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 15000;
                        break;
                    case 4:
                        DoCast(m_creature, SPELL_CAST_VISUAL);
                        DoCast(m_creature, SPELL_FROSTMOURNE_SOUNDS);
                        if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FROSTMOURNE)))
                            m_pInstance->DoUseDoorOrButton(pFrostmourne->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 5:
                        if(Creature* pUther = m_creature->SummonCreature(NPC_UTHER, 5307.018f, 2004.103f, 709.342f, 4.22f, TEMPSUMMON_TIMED_DESPAWN, 135000))
						{
                            pUther->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pUther->GetGUID());
							DoScriptText(SAY_UTHER_HORDE1, pUther);
							m_uiUtherGuid = pUther->GetGUID();
						}
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 11000;
                        break;
                    case 6:
                        DoScriptText(SAY_SYLVANAS_INTRO4, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 7:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE2, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 9000;
                        break;
                    case 8:
                        DoScriptText(SAY_SYLVANAS_INTRO5, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 9:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE3, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 20000;
                        break;
                    case 10:
                        DoScriptText(SAY_SYLVANAS_INTRO6, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 11:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE4, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 23000;
                        break;
                    case 12:
                        DoScriptText(SAY_SYLVANAS_INTRO7, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 13:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE5, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 15000;
                        break;
                    case 14:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE6, pUther);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 10000;
                        break;
                    case 15:
                        DoScriptText(SAY_SYLVANAS_INTRO8, m_creature);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 4000;
                        break;
                    case 16:
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							DoScriptText(SAY_UTHER_HORDE7, pUther);
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        if(Creature* pLichKing = m_creature->SummonCreature(NPC_LICH_KING_INTRO, 5364.653f, 2064.413f, 707.695f, 3.85f, TEMPSUMMON_TIMED_DESPAWN, 55000))
						{
							pLichKing->AddSplineFlag(SPLINEFLAG_WALKMODE);
                            pLichKing->SetSpeedRate(MOVE_WALK, 1.5f);
							pLichKing->GetMotionMaster()->MovePoint(0, 5334.068f, 2031.866f, 707.694f);
							pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
							m_uiLichKingGuid = pLichKing->GetGUID();
						}
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 10000;
                        break;
                    case 17:
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 18:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
						{
							DoScriptText(SAY_LICH_KING_INTRO1, pLichKing);
							pLichKing->GetMotionMaster()->MovePoint(0, 5316.053f, 2013.209f, 709.341f);
						}
						if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGuid))
							pUther->ForcedDespawn();
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 19:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
							DoScriptText(SAY_LICH_KING_INTRO2, pLichKing);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 7000;
                        break;
                    case 20:
                        if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_FROSTMOURNE)))
                            pFrostmourne->Delete();
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
                        {
							((npc_lich_king_hor_startAI*)pLichKing->AI())->EquipFrostmourne();
                            pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, false);
                        }
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 3000;
                        break;
                    case 21:
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
							DoScriptText(SAY_LICH_KING_INTRO3, pLichKing);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 8000;
                        break;
                    case 22:
                        StartEncounter();
						if(Creature* pFalric = m_pInstance->instance->GetCreature(m_uiFalricGUID))
                            DoScriptText(SAY_INTRO_FALRIC, pFalric);
						if(Creature* pMarwyn = m_pInstance->instance->GetCreature(m_uiMarwynGUID))
                            DoScriptText(SAY_INTRO_MARWYN, pMarwyn);
						if(Creature* pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGuid))
							pLichKing->GetMotionMaster()->MovePoint(0, 5364.653f, 2064.413f, 707.695f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 23:
                        if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 5000;
                        break;
                    case 24:
                        DoScriptText(SAY_SYLVANAS_INTRO9, m_creature);
                        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(0, 5364.653f, 2064.413f, 707.695f);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 12000;
                        break;
                    case 25:
                        if(GameObject* pEnterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ICECROWN_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pEnterDoor->GetGUID());
                        if(GameObject* pExitDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_IMPENETRABLE_DOOR)))
                            m_pInstance->DoUseDoorOrButton(pExitDoor->GetGUID());
                        m_bIsIntro = false;
                        if(m_pInstance)
                        {
                            m_pInstance->SetData(TYPE_INTRO, DONE);
                            m_pInstance->SetData(TYPE_FALRIC, SPECIAL);
                        }
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        ++m_uiIntro_Phase;
                        m_uiSpeech_Timer = 12000;
                        break;

                    default:
                        m_uiSpeech_Timer = 100000;
                    }
                    break;
                }
            }
            else 
                m_uiSpeech_Timer -= uiDiff;
        }
    }
};

bool GossipHello_npc_slyvanas_jaina_hor_start(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!m_pInstance) return true;

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if(m_pInstance->GetData(TYPE_INTRO) != DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    else
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_RESET, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_slyvanas_jaina_hor_start(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        ((npc_sylvanas_jaina_hor_startAI*)pCreature->AI())->StartIntro();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pPlayer->CLOSE_GOSSIP_MENU();
        break;
    case GOSSIP_ACTION_INFO_DEF+2:
        ((npc_sylvanas_jaina_hor_startAI*)pCreature->AI())->StartEncounter();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pPlayer->CLOSE_GOSSIP_MENU();
        break;
    }

    return true;
}

CreatureAI* GetAI_mob_frostsworn_general(Creature* pCreature)
{
    return new mob_frostsworn_generalAI(pCreature);
}

CreatureAI* GetAI_mob_lichKing_minion(Creature* pCreature)
{
    return new mob_lichKing_minionAI(pCreature);
}

CreatureAI* GetAI_mob_hallsOfReflectionSoul(Creature* pCreature)
{
    return new mob_hallsOfReflectionSoulAI(pCreature);
}

CreatureAI* GetAI_npc_sylvanas_jaina_hor_start(Creature* pCreature)
{
    return new npc_sylvanas_jaina_hor_startAI (pCreature);
}

CreatureAI* GetAI_npc_lich_king_hor_start(Creature* pCreature)
{
    return new npc_lich_king_hor_startAI (pCreature);
}

void AddSC_halls_of_reflection()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_frostsworn_general";
    newscript->GetAI = &GetAI_mob_frostsworn_general;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_lichKing_minion";
    newscript->GetAI = &GetAI_mob_lichKing_minion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_hallsOfReflectionSoul";
    newscript->GetAI = &GetAI_mob_hallsOfReflectionSoul;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lich_king_hor_start";
    newscript->GetAI = &GetAI_npc_lich_king_hor_start;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_hor_start;
    newscript->Name = "npc_slyvanas_jaina_hor_start";
    newscript->pGossipHello = &GossipHello_npc_slyvanas_jaina_hor_start;
    newscript->pGossipSelect = &GossipSelect_npc_slyvanas_jaina_hor_start;
    newscript->RegisterSelf();
}
