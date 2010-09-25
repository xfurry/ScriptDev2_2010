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
SDName: boss_devourer_of_souls
SD%Complete: 50%
SDComment: Timer need tuning, texts, SPELL_MIRRORED_SOUL SPELL_WAILING_SOULS need research and mangos fixing
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    // TODO, how to change the face on random?, how to know which face is shown, to use the additional entries
    SAY_MALE_1_AGGRO            = -1632007,
    SAY_FEMALE_AGGRO            = -1632008,
    SAY_MALE_1_SLAY_1           = -1632009,
    SAY_FEMALE_SLAY_1           = -1632010,
    SAY_MALE_2_SLAY_1           = -1632011,
    SAY_MALE_1_SLAY_2           = -1632012,
    SAY_FEMALE_SLAY_2           = -1632013,
    SAY_MALE_2_SLAY_2           = -1632014,
    SAY_MALE_1_DEATH            = -1632015,
    SAY_FEMALE_DEATH            = -1632016,
    SAY_MALE_2_DEATH            = -1632017,
    SAY_MALE_1_SOUL_ATTACK      = -1632018,
    SAY_FEMALE_SOUL_ATTACK      = -1632019,
    SAY_MALE_2_SOUL_ATTACK      = -1632020,
    SAY_MALE_1_DARK_GLARE       = -1632021,
    SAY_FEMALE_DARK_GLARE       = -1632022,

    EMOTE_MIRRORED_SOUL         = -1632023,
    EMOTE_UNLEASH_SOULS         = -1632024,
    EMOTE_WAILING_SOULS         = -1632025,

	SPELL_PHANTOM_BLAST         = 68982,
    SPELL_PHANTOM_BLAST_H       = 70322,
    SPELL_WELL_OF_SOULS         = 68820, // spawns 36536, this one should cast 68854 (triggers normal dmg spell 68863 ) - 68855(visual) - 72630 (visual) - heroic dmg spell 70323(TODO, is needed in Mangos?)
    SPELL_WELL_OF_SOULS_TRIGGER = 68854,                    // Some Aura Stacking problems here
    SPELL_WELL_OF_SOULS_VISUAL1 = 68855,
    SPELL_WELL_OF_SOULS_VISUAL2 = 72630,
 
    // selecting target, applying aura to pass on dmg, dmg triggers 69034 with right amount
    SPELL_MIRRORED_SOUL_TARGETS = 69048,    // max_targets in mangos needed
    SPELL_MIRRORED_SOUL         = 69051,
        // 3) 69023 apply dmg-exchange aura(periodically trigger not ex. 1206)
        // 4) 69034 effect dmg, basepoint=1 - visual effect, orange bubble flying from caster to target, seems like done on every dmg to boss
        // 1) 69048 aoe, SPELL_EFFECT_SCRIPT_EFFECT -- used to get the random target
        // 2) 69051 SPELL_EFFECT_SCRIPT_EFFECT, dummy-aura (target), dummy-aura(self)
 
    // spawning of adds
    SPELL_UNLEASHED_SOULS       = 68939, // trigger (68967, select nearby target trigger 68979(summon 36595)), transform, root
 
    // 90 degree sweep of the room in approx 10s
    // atm SPELL_WAILING_TRIGGER_AURA is not used.
    // also the turning is not smooth, needs more ideas
    SPELL_WAILING_SOULS         = 68899,
    SPELL_WALIING_SOULS_TARGETS = 68912,    // max_targes in mangos needed(?)
    SPELL_WAILING_SOULS_DMG     = 68873,
    SPELL_WAILING_SOULS_DMG_H   = 70324,
    SPELL_WAILING_TRIGGER_AURA  = 68876,
        //  68871 SPELL_EFFECT_SCRIPT_EFFECT (self), apply (timed) 68876
        //  68873 dmg enemies in front (self)
        //  68875, 68876(with tool tip)  apply dummy aura <-- sweeping aura (self) // destroys visual
        // 2) 68899 transform and trigger 68871 (self)
        // 1) 68912 apply dummy aura (~ target for souls) to enemies (aoe) (after 4s casted)
        //  70324 dmg enemies in front, heroic
 
        // Spells 68875, 68876
        // If casted in ScriptEffect(SpellHitTarget of 68871) triggered, it won't work as channeld spell
        // If casted with Interrupt Previous it kill the transform-aura spell
        // anyhow casted non triggered seems to remove the transform-aura spell
        // plus the channeled spell doesn't prevent the turning towards target
    SPELL_DRUID_MORPH_1_5       = 68931,                    // delayed visual, 1.5s delay - used before wailing souls
    SPELL_DRUID_MORPH_0_5       = 68977,                    // delayed visual, .5s delay  - used before unleash souls
    SPELL_DRUID_MORPH           = 68929,                    // visual effect
    SPELL_SUBMERGE_VISUAL       = 68909,                    // visual used to 'whirl' the heads - used after special phases, after druidmorph
 
    MAX_ANGLE_STEPS_COUNT       = 100,                       // 20 = 500ms (retail periodic tic of auras), increase to smoothen
    NPC_WELL_OF_SOULS           = 36536,

    FACE_NORMAL                 = 0,
    FACE_UNLEASHING             = 1,
    FACE_WAILING                = 2,
};

static const int aTexts[6][3] =
{
    {SAY_MALE_1_AGGRO,       SAY_FEMALE_AGGRO,       0},                        // 0 - aggro
    {SAY_MALE_1_SLAY_1,      SAY_FEMALE_SLAY_1,      SAY_MALE_2_SLAY_1},        // 1 - slay1
    {SAY_MALE_1_SLAY_2,      SAY_FEMALE_SLAY_2,      SAY_MALE_2_SLAY_2},        // 2 - slay2
    {SAY_MALE_1_DEATH,       SAY_FEMALE_DEATH,       SAY_MALE_2_DEATH},         // 3 - death
    {SAY_MALE_1_SOUL_ATTACK, SAY_FEMALE_SOUL_ATTACK, SAY_MALE_2_SOUL_ATTACK},   // 4 - soul
    {SAY_MALE_1_DARK_GLARE,  SAY_FEMALE_DARK_GLARE,  0}                         // 5 - glare
};

struct MANGOS_DLL_DECL boss_devourer_of_soulsAI : public ScriptedAI
{
    boss_devourer_of_soulsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_forge_of_souls*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_forge_of_souls* m_pInstance;
    uint8 m_uiFace;
    bool m_bIsRegularMode;

	bool m_bWailingClockwise;
    bool m_bSetOrientationCalledTooSoon;
    bool m_bDoSweeping;
    uint8 m_uiWailingCount;
    float m_fOriginalAngle;
 
    uint32 m_uiPhantomBlastTimer;
    uint32 m_uiWellTimer;
    uint32 m_uiMirrorTimer;
    uint32 m_uiUnleashTimer;
    uint32 m_uiWailingTimer;
    uint32 m_uiFacingTimer;
 
    std::list<uint64> m_lWellGUIDs;

    void Reset()
    {
        m_uiFace = FACE_NORMAL;
		m_bSetOrientationCalledTooSoon = false;
        m_bDoSweeping = false;
        m_uiPhantomBlastTimer = urand(5000, 10000); // TODO, all timers
        m_uiWellTimer = urand(10000, 15000);
        m_uiMirrorTimer = m_uiUnleashTimer = urand(10000, 15000);
        m_uiWailingTimer = 10000;// urand(50000, 70000);
        m_uiFacingTimer = 0;//100;
        SetCombatMovement(true);
        // Remove remaining adds
        for (std::list<uint64>::const_iterator itr = m_lWellGUIDs.begin(); itr != m_lWellGUIDs.end(); itr++)
            if (Creature* pWell = m_creature->GetMap()->GetCreature(*itr))
                pWell->ForcedDespawn();
        m_lWellGUIDs.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(aTexts[0][m_uiFace], m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DECOURER_OF_SOULS, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(aTexts[urand(1, 2)][m_uiFace], m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(aTexts[3][m_uiFace], m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_DECOURER_OF_SOULS, DONE);
        }
		// Remove remaining adds
        for (std::list<uint64>::const_iterator itr = m_lWellGUIDs.begin(); itr != m_lWellGUIDs.end(); itr++)
            if (Creature* pWell = m_creature->GetMap()->GetCreature(*itr))
                pWell->ForcedDespawn();
        m_lWellGUIDs.clear();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(NPC_DEVOURER_OF_SOULS, NOT_STARTED);
            // If we previously failed, set such that possible to try again
            m_pInstance->SetData(TYPE_ACHIEV_PHANTOM_BLAST, IN_PROGRESS);
        }
		// Remove remaining adds
        for (std::list<uint64>::const_iterator itr = m_lWellGUIDs.begin(); itr != m_lWellGUIDs.end(); itr++)
            if (Creature* pWell = m_creature->GetMap()->GetCreature(*itr))
                pWell->ForcedDespawn();
        m_lWellGUIDs.clear();
    }

	void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_WELL_OF_SOULS)
        {
            m_lWellGUIDs.push_back(pSummoned->GetGUID());
            // Here is some aura Stacking problem I think!
            pSummoned->CastSpell(pSummoned, SPELL_WELL_OF_SOULS_TRIGGER, true);
            pSummoned->CastSpell(pSummoned, SPELL_WELL_OF_SOULS_VISUAL1, true);
            pSummoned->CastSpell(pSummoned, SPELL_WELL_OF_SOULS_VISUAL2, true);
            //pSummoned->AI()->SetCombatMovement(false);
            // Prevent Combat Movement
        }
    }
 
    /* This is some kind of workaround, because some of these spells work with SPELL_EFFECT_SCRIPT_EFFECT */
    void SpellHitTarget(Unit* pTarget, SpellEntry const* pSpellEntry)
    {
        switch (pSpellEntry->Id)
        {
            // if we hit a target with phantom blast, the event is to be failed
            case SPELL_PHANTOM_BLAST:
            case SPELL_PHANTOM_BLAST_H:
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_ACHIEV_PHANTOM_BLAST, FAIL);
                break;
            // collect possible targets for mirror soul -WORKAROUND, could be done in DB spell-scripts
            case SPELL_MIRRORED_SOUL_TARGETS:
                DoCastSpellIfCan(pTarget, SPELL_MIRRORED_SOUL, CAST_INTERRUPT_PREVIOUS);
                break;
            case SPELL_UNLEASHED_SOULS:
                m_uiFace = FACE_UNLEASHING;
                DoScriptText(aTexts[4][m_uiFace], m_creature);
                break;
            // collect possible targets for begin of sweeping
            case SPELL_WALIING_SOULS_TARGETS:
                m_fOriginalAngle = m_creature->GetAngle(pTarget);
                break;
            // set sweeping start position, say text and enable it
            case SPELL_WAILING_SOULS:
                DoScriptText(aTexts[5][m_uiFace], m_creature);
                m_uiFace = FACE_WAILING;
                m_creature->SetFacingTo(m_fOriginalAngle);
                m_creature->SetStandState(0);
                // remove Target from boss
                m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
                m_bDoSweeping = true;
                break;
            case 68871: // simulate ScriptEffect casting the periodic spells, but doesn't help yet
                DoCastSpellIfCan(m_creature, urand(0, 1) ? 68876 : 68875, CAST_TRIGGERED);
                break;
        }
    }
 
    void DoNextAngle()
    {
        if (m_uiWailingCount < MAX_ANGLE_STEPS_COUNT)
        {
            m_uiWailingCount++;
 
            // Seting angle
            float fAngle = m_fOriginalAngle + (m_bWailingClockwise ? -1 : 1) * (m_uiWailingCount-1) * 90*2*3.14f/360/MAX_ANGLE_STEPS_COUNT;
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), SPLINETYPE_FACINGANGLE, SPLINEFLAG_WALKMODE, 0, NULL, (double)fAngle);
            m_creature->Relocate(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), fAngle);
 
            // Do not cast every tick, do not cast till end to improve some visual effect
            if (m_uiWailingCount < MAX_ANGLE_STEPS_COUNT *0.90f && (m_uiWailingCount % (MAX_ANGLE_STEPS_COUNT/20)) == 0)
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WAILING_SOULS_DMG : SPELL_WAILING_SOULS_DMG_H);
        }
        else
        {
            DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_VISUAL, CAST_TRIGGERED);
            // Remove Auras and reset targets and stuff
            m_creature->RemoveAurasDueToSpell(SPELL_WAILING_SOULS);
            m_uiWailingCount = 0;
            m_uiFace = FACE_NORMAL;
            m_bDoSweeping = false;
 
            // In case the spell killed all enemies
            if (m_creature->SelectHostileTarget() && m_creature->getVictim())
            {
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->SetFacingToObject(m_creature->getVictim());
                m_creature->SetUInt32Value(UNIT_FIELD_TARGET, m_creature->getVictim()->GetGUIDLow());
            }
            SetCombatMovement(true);
        }
        // Improves an visual effect, if we used the aura
        //if (m_uiWailingCount >= MAX_ANGLE_STEPS_COUNT * 0.90f)
        //    m_creature->RemoveAurasDueToSpell(SPELL_WAILING_TRIGGER_AURA);
    }
 
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->isInCombat())
            return;
 
        if (m_uiWailingTimer < uiDiff)
        {
            m_uiWailingTimer = 10000;
            if (m_creature->HasAura(68875) || m_creature->HasAura(68876))
            {
                // Remove auras
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SelectHostileTarget();
                if (m_creature->getVictim())
                    AttackStart(m_creature->getVictim());
                m_uiFacingTimer = 0;
                return;
            }
            else
            {
               m_creature->SetStandState(0);
                // remove Target from boss
                m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
                m_creature->SetUInt64Value(19, 0);
                uint32 uiSpell = 0;
                if (urand(0,1))
                {
                    uiSpell = 68875;
                    m_creature->MonsterSay("dbg: cast turning - 5",0,0);
                }
                else
                    {
                    uiSpell = 68876;
                    m_creature->MonsterSay("dbg: cast turning - 6",0,0);
                }
                DoCastSpellIfCan(m_creature, uiSpell);
                m_uiFacingTimer = 100;
                m_fOriginalAngle = m_creature->GetOrientation();
            }
        }
        else
            m_uiWailingTimer -= uiDiff;
 
        if (m_uiFacingTimer)
        {
            if (m_uiFacingTimer <= uiDiff)
            {
                m_uiFacingTimer = 100;
                float fAngle = m_fOriginalAngle + (m_uiWailingCount-1) * 90*2*3.14f/360/80;
                m_uiWailingCount++;
                m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), SPLINETYPE_FACINGANGLE, SPLINEFLAG_WALKMODE, 0, NULL, (double)fAngle);
            }
            else
                m_uiFacingTimer -= uiDiff;
        }
 
        if (!m_creature->isInCombat())
            return;
 
        switch (m_uiFace)
        {
            case FACE_WAILING:
                // Code before SelectHostileTarget, to avoid change of direction
                if (m_bDoSweeping)                                  // Do nothing else while sweeping
                {
                    if (m_uiFacingTimer < uiDiff)
                    {
                        DoNextAngle();
                        m_uiFacingTimer = 10000/MAX_ANGLE_STEPS_COUNT;
                    }
                    else
                        m_uiFacingTimer -= uiDiff;
                }
                if (!m_creature->HasAura(SPELL_WAILING_SOULS))
                {
                    // Finished sweeping
                    //m_uiFace = FACE_NORMAL;
                    DoCastSpellIfCan(m_creature, SPELL_DRUID_MORPH, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_VISUAL, CAST_TRIGGERED);
                }
 
                return;                                         // TODO: research, the sounds indicate that he might start unleashing souls in this phase
 
            case FACE_UNLEASHING:
                if (!m_creature->HasAura(SPELL_UNLEASHED_SOULS))
                {
                    // Finished unleashing
                    m_uiFace = FACE_NORMAL;
                    DoCastSpellIfCan(m_creature, SPELL_DRUID_MORPH, CAST_TRIGGERED);
                    DoCastSpellIfCan(m_creature, SPELL_SUBMERGE_VISUAL, CAST_TRIGGERED);
                }
                // no break here, as has same abilities as normal phase
 
            case FACE_NORMAL:
                // Update Target and Do Combat Spells
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;
 
                if (m_uiPhantomBlastTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_PHANTOM_BLAST : SPELL_PHANTOM_BLAST_H) == CAST_OK)
                            m_uiPhantomBlastTimer = urand(5000, 10000); // TODO
                }
                else
                    m_uiPhantomBlastTimer -= uiDiff;
 
                if (m_uiWellTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        if (DoCastSpellIfCan(pTarget, SPELL_WELL_OF_SOULS) == CAST_OK)
                            m_uiWellTimer = urand(15000, 25000); // TODO
                }
                else
                    m_uiWellTimer -= uiDiff;
 
                if (m_uiMirrorTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_MIRRORED_SOUL_TARGETS) == CAST_OK)
                    {
                        m_uiMirrorTimer = urand(25000, 35000); // TODO
                        DoScriptText(EMOTE_MIRRORED_SOUL, m_creature);
                    }
                }
                else
                    m_uiMirrorTimer -= uiDiff;
 
                if (m_uiUnleashTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_DRUID_MORPH_0_5, CAST_TRIGGERED) == CAST_OK)
                        if (DoCastSpellIfCan(m_creature, SPELL_UNLEASHED_SOULS) == CAST_OK)
                        {
                            m_uiUnleashTimer = urand(30000, 60000); // TODO
                            DoScriptText(EMOTE_UNLEASH_SOULS, m_creature);
                        }
                }
                else
                    m_uiUnleashTimer -= uiDiff;
 
                if (m_uiWailingTimer < uiDiff)
                {
                    // cast-time of target-aura = 4s, cast-time of wailing souls spell = 3s, time to first periodic dmg aura tick = .5s, hence return such that the timeers fit
                    if (DoCastSpellIfCan(m_creature, SPELL_WALIING_SOULS_TARGETS, CAST_TRIGGERED) == CAST_OK)
                    {
                        DoCastSpellIfCan(m_creature, SPELL_DRUID_MORPH_1_5, CAST_TRIGGERED);
                        if (DoCastSpellIfCan(m_creature, SPELL_WAILING_SOULS, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                        {
                            // Unaura Wailing Aura
 
                            m_uiWailingTimer = urand(25000, 35000); // TODO
                            DoScriptText(EMOTE_WAILING_SOULS, m_creature);
                            m_bWailingClockwise = urand(0,1);           // Clockwise or Counter-Clockwise
                            m_uiWailingCount = 0;                       // Reset counter for angle
                            // Used here, because otherwise there is a tick between aura applying and setting combat movement
                            SetCombatMovement(false);
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveIdle();
                        }
                    }
                }
                else
                    m_uiWailingTimer -= uiDiff;
 
                DoMeleeAttackIfReady();
                break;
        }
    }
};

CreatureAI* GetAI_boss_devourer_of_souls(Creature* pCreature)
{
    return new boss_devourer_of_soulsAI(pCreature);
}

void AddSC_boss_devourer_of_souls()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_devourer_of_souls";
    pNewScript->GetAI = &GetAI_boss_devourer_of_souls;
    pNewScript->RegisterSelf();
}
