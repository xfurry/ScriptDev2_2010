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
SDName: boss_bronjahm
SD%Complete: 95%
SDComment: Timer need tuning, Spell Soulstorm need MaNGOS tuning. Mangos BUG: Soulstorm Aura (Root) gets dispelled with dmg
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    SAY_AGGRO_1                 = -1632000,                 // without sound, really correct?
    SAY_AGGRO_2                 = -1632001,
    SAY_SLAY_1                  = -1632002,
    SAY_SLAY_2                  = -1632003,
    SAY_DEATH                   = -1632004,
    SAY_SOULSTORM               = -1632005,
    SAY_CORRUPT_SOUL            = -1632006,

	SPELL_MAGICS_BANE           = 68793,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_CORRUPT_SOUL          = 68839,
    SPELL_BANISH_VISUAL         = 68862,
    SPELL_CONSUME_SOUL_TRIGGER  = 68861,
    SPELL_TELEPORT              = 68988,
    SPELL_SOULSTORM_VISUAL      = 68870,                    // castet before Soulstorm, should trigger the spells from the visual array below
    SPELL_SOULSTORM             = 68872,
    SPELL_FEAR                  = 68950,
};

// Visual spells Cast OOC every second, no trigger auras found for this
// These Spells are casted 'circularily' but OOC with 1s delay (done here in script)
// Also trigggered for some time while comabt - but then with 200ms delay
static const uint32 aSoulstormSpells[8] = {68898, 68904, 68886, 68905, 68896, 68906, 68897, 68907};
struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_forge_of_souls* m_pInstance;
    bool m_bIsRegularMode;

	uint8 m_uiPhase;
    uint8 m_uiVisualSpell;

    uint32 m_uiMagicsBaneTimer;
    uint32 m_uiCorruptSoulTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiVisualTimerOOC;
    uint32 m_uiShadowboltTimer;

    void Reset()
    {
		m_uiPhase = 0;
        m_uiVisualSpell = 0;
        m_uiMagicsBaneTimer = urand(8000, 12000);
        m_uiCorruptSoulTimer = urand(20000, 30000);
        m_uiFearTimer = 1000;
        m_uiVisualTimerOOC = 1000;
        m_uiShadowboltTimer = 5000;
        SetCombatMovement(true);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(urand(0, 1) ? SAY_AGGRO_1 : SAY_AGGRO_2, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
    }

	void SpellHitTarget(Unit* pTarget, SpellEntry const* pSpellEntry)
    {
        if (pTarget == m_creature && pSpellEntry->Id == SPELL_TELEPORT)
        {
            // Say Text and cast Soulstorm
            DoScriptText(SAY_SOULSTORM, m_creature);
            DoCastSpellIfCan(m_creature, SPELL_SOULSTORM_VISUAL, CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS);
            DoCastSpellIfCan(m_creature, SPELL_SOULSTORM, CAST_INTERRUPT_PREVIOUS);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            // Cast the visual spells, every 1s the 'next' spell
            if (m_uiVisualTimerOOC < uiDiff)
            {
                m_uiVisualTimerOOC = 1000;
                DoCastSpellIfCan(m_creature, aSoulstormSpells[m_uiVisualSpell]);
                // Set next spell
                ++m_uiVisualSpell %= 8;
            }
            else
                m_uiVisualTimerOOC -= uiDiff;

            return;
        }

        if (m_uiPhase == 0)                                 // Phase 1
        {
            // Switching Phase, Soulstorm is cast in SpellHitTarget
            if (m_creature->GetHealthPercent() < 30.0f)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_TELEPORT) == CAST_OK)
                    m_uiPhase = 1;
            }

            // Corrupt Soul
            if (m_uiCorruptSoulTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_CORRUPT_SOUL) == CAST_OK)
                    {
                        DoScriptText(SAY_CORRUPT_SOUL, m_creature);
                        m_uiCorruptSoulTimer = urand(20000, 30000);
                    }
                }
            }
            else
                m_uiCorruptSoulTimer -= uiDiff;

            // Magic's Bane
            if (m_uiMagicsBaneTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_MAGICS_BANE) == CAST_OK)
                    m_uiMagicsBaneTimer = urand(7000, 15000);
            }
            else
                m_uiMagicsBaneTimer -= uiDiff;

            // Used to prevent Shadowbolt-Casting on Aggro for a few seconds
            if (m_uiShadowboltTimer <= uiDiff)
                m_uiShadowboltTimer = 0;
            else
                m_uiShadowboltTimer -= uiDiff;

            // Use ShadowBolt as default attack if victim is not in range
            if (!m_uiShadowboltTimer && !m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            {
                if (IsCombatMovement())
                {
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->StopMoving();
                }
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOW_BOLT);
            }
            else
            {
                if (!IsCombatMovement())
                {
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiShadowboltTimer = 2000;             // Give some time to chase
                }

                DoMeleeAttackIfReady();
            }
        }
        else                                                // Soulstorm Phase
        {
            if (m_uiFearTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FEAR) == CAST_OK)
                    m_uiFearTimer = urand(10000, 15000);
            }
            else
                m_uiFearTimer -= uiDiff;

            // Default attack
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SHADOW_BOLT);
        }
    }
};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

struct MANGOS_DLL_DECL npc_corrupted_soul_fragmentAI : public ScriptedAI
{
    npc_corrupted_soul_fragmentAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        DoCastSpellIfCan(m_creature, SPELL_BANISH_VISUAL);
    }

    void Reset()
    {
        SetCombatMovement(true);
    }

    void JustDied(Unit* pKiller)
    {
        if (instance_forge_of_souls* pInstance = (instance_forge_of_souls*)m_creature->GetInstanceData())
            pInstance->SetData64(DATA_SOULFRAGMENT_REMOVE, m_creature->GetGUID());
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetEntry() == NPC_BRONJAHM)
        {
            if (m_creature->IsWithinDistInMap(pWho, INTERACTION_DISTANCE))
            {
                DoCastSpellIfCan(pWho, SPELL_CONSUME_SOUL_TRIGGER, CAST_TRIGGERED);

                // Inform the instance about a used soul fragment
                if (instance_forge_of_souls* pInstance = (instance_forge_of_souls*)m_creature->GetInstanceData())
                    pInstance->SetData64(DATA_SOULFRAGMENT_REMOVE, m_creature->GetGUID());

                m_creature->ForcedDespawn();
                return;
            }
            if (IsCombatMovement())
            {
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->MoveFollow(pWho, 0.0f, 0.0f);
            }
        }
    }
};

CreatureAI* GetAI_npc_corrupted_soul_fragment(Creature* pCreature)
{
    return new npc_corrupted_soul_fragmentAI(pCreature);
}

void AddSC_boss_bronjahm()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_bronjahm";
    pNewScript->GetAI = &GetAI_boss_bronjahm;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "npc_corrupted_soul_fragment";
    pNewScript->GetAI = &GetAI_npc_corrupted_soul_fragment;
    pNewScript->RegisterSelf();
}
