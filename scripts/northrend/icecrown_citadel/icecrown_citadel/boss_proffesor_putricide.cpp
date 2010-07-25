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
SDName: boss_proffesor_putricide
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // yells
    SAY_AGGRO               = -1609481,
    SAY_AIRLOCK             = -1609482,
    SAY_PHASE               = -1609483,
    SAY_TRANSFORM1          = -1609484,
    SAY_TRANSFORM2          = -1609485,
    SAY_KILL1               = -1609486,
    SAY_KILL2               = -1609487,
    SAY_BERSERK             = -1609488,
    SAY_DEATH               = -1609489,

    // spells
    // phase 1 -> 80%
    SPELL_SLIME_PUDDLE              = 70341,
    SPELL_SLIME_PUDDLE_10           = 70346,
    SPELL_SLIME_PUDDLE_25           = 72868,
    SPELL_SLIME_PUDDLE_25HC         = 72869,
    SPELL_SLIME_PUDDLE_TRIG         = 70343,
    SPELL_UNSTABLE_EXPERIMENT       = 71968,
    SPELL_TEAR_GAS                  = 71617,    // only on non hc
    SPELL_TEAR_GAS_STUN             = 71618,
    SPELL_CREATE_CONCOCTION         = 71621,

    // phase 2 -> 35%
    SPELL_CHOKING_GAS_TRIG          = 71259,
    SPELL_CHOKING_GAS_10            = 71278,
    SPELL_CHOKING_GAS_25            = 72460,
    SPELL_CHOKING_GAS_10HC          = 72619,
    SPELL_CHOKING_GAS_25HC          = 72620,
    SPELL_CHOKING_GAS_EXPL_TRIG     = 71280,
    SPELL_CHOKING_GAS_EXPL_10       = 71279,
    SPELL_CHOKING_GAS_EXPL_25       = 72459,
    SPELL_CHOKING_GAS_EXPL_10HC     = 72621,
    SPELL_CHOKING_GAS_EXPL_25HC     = 72622,
    SPELL_MALLEABLE_GOO_10          = 72297,
    SPELL_MALLEABLE_GOO_25          = 72548,
    SPELL_MALLEABLE_GOO_10HC        = 72873,
    SPELL_MALLEABLE_GOO_25HC        = 72874,
    SPELL_MALLEABLE_GOO_SUMMON      = 72298,
    SPELL_MALLEABLE_GOO_VISUAL      = 75845,

    // phase 3
    SPELL_MUTATED_STRENGTH          = 71603,
    SPELL_GUZZLE_POTIONS            = 71893,
    SPELL_MUTATED_PLAGUE            = 72672,
    SPELL_BERSERK                   = 47008,

    // mutated transformation
    SPELL_MUTATED_TRANSFORMATION    = 70311,
    SPELL_EAT_OOZE                  = 72527,
    SPELL_REGURGITATED_OOZE_10      = 70539,
    SPELL_REGURGITATED_OOZE_25      = 72457,
    SPELL_REGURGITATED_OOZE_10HC    = 72875,
    SPELL_REGURGITATED_OOZE_25HC    = 72876,
    SPELL_MUTATED_SLASH             = 70542,
    SPELL_MUTATED_AURA              = 70405,

    // cloud
    SPELL_EXPUNGED_GAS              = 70701,
    SPELL_GASEOUS_BLOAT_10          = 70672,
    SPELL_GASEOUS_BLOAT_10HC        = 72455,
    SPELL_GASEOUS_BLOAT_25          = 72832,
    SPELL_GASEOUS_BLOAT_25HC        = 72833,

    // ooze
    SPELL_OOZE_ADHESIVE_10          = 70447,
    SPELL_OOZE_ADHESIVE_25          = 72836,
    SPELL_OOZE_ADHESIVE_10HC        = 72837,
    SPELL_OOZE_ADHESIVE_25HC        = 72838,
    SPELL_OOZE_ERUPTION             = 70492,

    NPC_MUTATED_ABOMINATION         = 37672,
    NPC_GAS_CLOUD                   = 37562,
    NPC_VOLATILE_OOZE               = 37697,
    NPC_GAS_BOMB                    = 38159,
    NPC_MALLEABLE_OOZE              = 38556,
    NPC_SLIME_PUDDLE                = 38234, 
};

struct MANGOS_DLL_DECL boss_professor_putricideAI : public ScriptedAI
{
    boss_professor_putricideAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        homeX = pCreature->GetPositionX();
        homeY = pCreature->GetPositionY();
        homeZ = pCreature->GetPositionZ();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    // phase1
    uint32 m_uiUnstableExperimentTimer;
    uint32 m_uiSlimePuddleTimer;

    // phase2
    uint32 m_uiChockinGasTimer;
    uint32 m_uiMalleableGooTimer;

    // phase3
    uint32 m_uiMutatedStrengthTimer;
    uint32 m_uiMutatedPlagueTimer;

    // other
    uint8 m_uiPhase;
    uint32 m_uiDrinkPotionTimer;
    uint32 m_uiSwitchTimer;
    bool m_bHasGasCasted;
    uint32 m_uiBerserkTimer;

    float homeX, homeY, homeZ;

    void Reset()
    {
        // phase1
        m_uiUnstableExperimentTimer = urand(50000, 60000);
        m_uiSlimePuddleTimer        = urand(25000, 30000);
        // phase2
        m_uiChockinGasTimer         = 15000;
        m_uiMalleableGooTimer       = 25000;
        // phase3
        m_uiMutatedStrengthTimer    = 10000;
        m_uiMutatedPlagueTimer      = 15000;

        // other
        m_uiDrinkPotionTimer    = 30000;
        m_uiSwitchTimer         = 20000;
        m_uiPhase               = 0;
        m_bHasGasCasted         = false;
        m_uiBerserkTimer        = 600000;  // 10 min
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_PUTRICIDE, NOT_STARTED);
            m_pInstance->SetData(TYPE_ATTEMPTS, m_pInstance->GetData(TYPE_ATTEMPTS) - 1);
            //m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_uiPhase = 1;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_PUTRICIDE, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_PUTRICIDE, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        switch (m_uiPhase)
        {
            // phase 1
        case 1:

            if (m_uiSlimePuddleTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SLIME_PUDDLE);
                m_uiSlimePuddleTimer = urand(20000, 30000);
            }
            else m_uiSlimePuddleTimer -= uiDiff;

            if (m_uiUnstableExperimentTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_UNSTABLE_EXPERIMENT);
                m_uiUnstableExperimentTimer = urand(50000, 60000);
            }
            else m_uiUnstableExperimentTimer -= uiDiff;

            break;
            // break -> tear gas
        case 2:

            if (m_uiDrinkPotionTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_CREATE_CONCOCTION);
                m_uiSwitchTimer = 15000;
                m_uiDrinkPotionTimer = 60000;
            }
            else m_uiDrinkPotionTimer -= uiDiff;

            if(m_uiSwitchTimer < uiDiff)
            {
                SetCombatMovement(true);
                m_creature->RemoveAurasDueToSpell(SPELL_TEAR_GAS);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase = 3;
            }
            else m_uiSwitchTimer -= uiDiff;

            break;
            // phase 2
        case 3:

            if (m_uiSlimePuddleTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SLIME_PUDDLE);
                m_uiSlimePuddleTimer = urand(20000, 30000);
            }
            else m_uiSlimePuddleTimer -= uiDiff;

            if (m_uiUnstableExperimentTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_UNSTABLE_EXPERIMENT);
                m_uiUnstableExperimentTimer = urand(50000, 60000);
            }
            else m_uiUnstableExperimentTimer -= uiDiff;

            if (m_uiChockinGasTimer < uiDiff)
            {
                //DoCast(m_creature, SPELL_SLIME_PUDDLE);
                m_uiChockinGasTimer = urand(15000, 20000);
            }
            else m_uiChockinGasTimer -= uiDiff;

            if (m_uiMalleableGooTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_MALLEABLE_GOO_SUMMON);
                DoCast(m_creature, SPELL_MALLEABLE_GOO_VISUAL);
                m_uiMalleableGooTimer = urand(10000, 15000);
            }
            else m_uiMalleableGooTimer -= uiDiff;

            break;
            // break -> tear gas
        case 4:

            if (m_uiDrinkPotionTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_GUZZLE_POTIONS);
                m_uiSwitchTimer = 15000;
                m_uiDrinkPotionTimer = 60000;
            }
            else m_uiDrinkPotionTimer -= uiDiff;

            if(m_uiSwitchTimer < uiDiff)
            {
                if(m_pInstance)
                    m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MUTATED_TRANSFORMATION);
                SetCombatMovement(true);
                m_creature->RemoveAurasDueToSpell(SPELL_TEAR_GAS);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase = 5;
            }
            else m_uiSwitchTimer -= uiDiff;

            break;
            // phase 3
        case 5:

            if (m_uiMutatedStrengthTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_MUTATED_STRENGTH);
                m_uiMutatedStrengthTimer = urand(13000, 15000);
            }
            else m_uiMutatedStrengthTimer -= uiDiff;

            if (m_uiMutatedPlagueTimer < uiDiff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                    DoCast(target, SPELL_MUTATED_PLAGUE);
                m_uiMutatedPlagueTimer = urand(20000, 25000);
            }
            else m_uiMutatedPlagueTimer -= uiDiff;

            break;
        }

        // phase changes
        if ( m_uiPhase == 1 && m_creature->GetHealthPercent() < 80.0f )
        {
            DoScriptText(SAY_TRANSFORM1, m_creature);
            DoCast(m_creature, SPELL_TEAR_GAS);
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->MovePoint(0, homeX, homeY, homeZ);
            m_uiDrinkPotionTimer = 5000;
            m_uiPhase = 2;
        }
        if ( m_uiPhase == 3 && m_creature->GetHealthPercent() < 35.0f ) 
        {
            DoScriptText(SAY_TRANSFORM2, m_creature);
            DoCast(m_creature, SPELL_TEAR_GAS);
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->MovePoint(0, homeX, homeY, homeZ);
            m_uiDrinkPotionTimer = 5000;
            m_uiPhase = 4;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_professor_putricide(Creature* pCreature)
{
    return new boss_professor_putricideAI(pCreature);
}

struct MANGOS_DLL_DECL mob_icc_gas_cloudAI : public ScriptedAI
{
    mob_icc_gas_cloudAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_icc_gas_cloud(Creature* pCreature)
{
    return new mob_icc_gas_cloudAI(pCreature);
}

struct MANGOS_DLL_DECL mob_icc_volatile_oozeAI : public ScriptedAI
{
    mob_icc_volatile_oozeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_icc_volatile_ooze(Creature* pCreature)
{
    return new mob_icc_volatile_oozeAI(pCreature);
}


struct MANGOS_DLL_DECL mob_icc_gas_bombAI : public ScriptedAI
{
    mob_icc_gas_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetRespawnTime(DAY);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiExplodeTimer;

    void Reset()
    {
        m_uiExplodeTimer = 20000;

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature, SPELL_CHOKING_GAS_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature, SPELL_CHOKING_GAS_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature, SPELL_CHOKING_GAS_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature, SPELL_CHOKING_GAS_25HC);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if(m_uiExplodeTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_CHOKING_GAS_EXPL_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_CHOKING_GAS_EXPL_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_CHOKING_GAS_EXPL_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_CHOKING_GAS_EXPL_25HC);

            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else m_uiExplodeTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_icc_gas_bomb(Creature* pCreature)
{
    return new mob_icc_gas_bombAI(pCreature);
}

struct MANGOS_DLL_DECL mob_malleable_oozeAI : public ScriptedAI
{
    mob_malleable_oozeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetRespawnTime(DAY);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    void Reset()
    {
        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature->getVictim(), SPELL_MALLEABLE_GOO_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature->getVictim(), SPELL_MALLEABLE_GOO_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature->getVictim(), SPELL_MALLEABLE_GOO_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature->getVictim(), SPELL_MALLEABLE_GOO_25HC);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_malleable_ooze(Creature* pCreature)
{
    return new mob_malleable_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_slime_puddleAI : public ScriptedAI
{
    mob_slime_puddleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetRespawnTime(DAY);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    void Reset()
    {
        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature->getVictim(), SPELL_SLIME_PUDDLE_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature->getVictim(), SPELL_SLIME_PUDDLE_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature->getVictim(), SPELL_SLIME_PUDDLE_25);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature->getVictim(), SPELL_SLIME_PUDDLE_25HC);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_PUTRICIDE) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_slime_puddle(Creature* pCreature)
{
    return new mob_slime_puddleAI(pCreature);
}

void AddSC_boss_professor_putricide()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_professor_putricide";
    newscript->GetAI = &GetAI_boss_professor_putricide;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_volatile_ooze";
    newscript->GetAI = &GetAI_mob_icc_volatile_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_gas_cloud";
    newscript->GetAI = &GetAI_mob_icc_gas_cloud;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icc_gas_bomb";
    newscript->GetAI = &GetAI_mob_icc_gas_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_malleable_ooze";
    newscript->GetAI = &GetAI_mob_malleable_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_slime_puddle";
    newscript->GetAI = &GetAI_mob_slime_puddle;
    newscript->RegisterSelf();
}
