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
SDName: Boss_Telestra
SD%Complete: 80%
SDComment: script depend on database spell support and eventAi for clones. transition to phase 2 also not fully implemented
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO               = -1576000,
    SAY_SPLIT_1             = -1576001,
    SAY_SPLIT_2             = -1576002,
    SAY_MERGE               = -1576003,
    SAY_KILL                = -1576004,
    SAY_DEATH               = -1576005,

    SPELL_FIREBOMB          = 47773,
    SPELL_FIREBOMB_H        = 56934,

    SPELL_ICE_NOVA          = 47772,
    SPELL_ICE_NOVA_H        = 56935,

    SPELL_GRAVITY_WELL      = 47756,

    SPELL_SUMMON_CLONES     = 47710,

    SPELL_ARCANE_VISUAL     = 47704,
    SPELL_FIRE_VISUAL       = 47705,
    SPELL_FROST_VISUAL      = 47706,

    SPELL_SUMMON_FIRE       = 47707,
    SPELL_SUMMON_ARCANE     = 47708,
    SPELL_SUMMON_FROST      = 47709,

    SPELL_FIRE_DIES         = 47711,                        // cast by clones at their death
    SPELL_ARCANE_DIES       = 47713,
    SPELL_FROST_DIES        = 47712,

    SPELL_SPAWN_BACK_IN     = 47714,

    NPC_TELEST_FIRE         = 26928,
    NPC_TELEST_ARCANE       = 26929,
    NPC_TELEST_FROST        = 26930,

    PHASE_1                 = 1,
    PHASE_2                 = 2,
    PHASE_3                 = 3,
    PHASE_4                 = 4,

    //Achievement
    ACHIEV_SPLIT_PERSONALITY  = 2150,
    ACHIEV_TIMER              = 5 * 1000
};

float CenterOfRoom[1][4] =
{
    {504.80f, 89.07f, -16.12f, 6.27f}
};

/*######
## boss_telestra
######*/

struct MANGOS_DLL_DECL boss_telestraAI : public ScriptedAI
{
    boss_telestraAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiPhase;
    uint8 m_uiCloneDeadCount;

    uint32 m_uiFirebombTimer;
    uint32 m_uiIceNovaTimer;
    uint32 m_uiGravityWellTimer;

    uint64 FireMagusGUID;
    uint64 FrostMagusGUID;
    uint64 ArcaneMagusGUID;
    bool FireMagusDead;
    bool FrostMagusDead;
    bool ArcaneMagusDead;

    uint32 AppearDelay_Timer;
    bool AppearDelay;

    bool AchievementTimerRunning;
    uint8 AchievementProgress;
    uint32 AchievementTimer;

    void Reset()
    {
        m_uiPhase = PHASE_1;
        m_uiCloneDeadCount = 0;

        m_uiFirebombTimer = urand(2000, 4000);
        m_uiIceNovaTimer = urand(8000, 12000);
        m_uiGravityWellTimer = urand(15000, 25000);

        FireMagusGUID = 0;
        FrostMagusGUID = 0;
        ArcaneMagusGUID = 0;

        AppearDelay = false;

        AchievementProgress = 0;
        AchievementTimer = 0;
        AchievementTimerRunning = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
    }

    void JustReachedHome()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            m_creature->GetMotionMaster()->MoveChase(pWho, 15.0f);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TELESTRA, DONE);

        if (!m_bIsRegularMode && AchievementProgress == 2)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_SPLIT_PERSONALITY);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void SpellHit(Unit* pCaster, const SpellEntry *pSpell)
    {
        switch(pSpell->Id)
        {
            // eventAi must make sure clones cast spells when each of them die
            case SPELL_FIRE_DIES:
            case SPELL_ARCANE_DIES:
            case SPELL_FROST_DIES:
            {
                ++m_uiCloneDeadCount;

                if (m_uiCloneDeadCount == 3 || m_uiCloneDeadCount == 6)
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_SUMMON_CLONES);
                    m_creature->CastSpell(m_creature, SPELL_SPAWN_BACK_IN, false);

                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                    DoScriptText(SAY_MERGE, m_creature);

                    m_uiPhase = m_uiCloneDeadCount == 3 ? PHASE_3 : PHASE_4;
                }
                break;
            }
            case SPELL_SUMMON_CLONES:
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_TELEST_FIRE: pSummoned->CastSpell(pSummoned, SPELL_FIRE_VISUAL, true); break;
            case NPC_TELEST_ARCANE: pSummoned->CastSpell(pSummoned, SPELL_ARCANE_VISUAL, true); break;
            case NPC_TELEST_FROST: pSummoned->CastSpell(pSummoned, SPELL_FROST_VISUAL, true); break;
        }
    }

        uint64 SplitPersonality(uint32 entry)
    {
        Creature* Summoned = m_creature->SummonCreature(entry, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000);
        if (Summoned)
        {
            switch (entry)
            {
            case NPC_TELEST_FIRE:
                {
                    Summoned->CastSpell(Summoned, SPELL_FIRE_VISUAL, false);
                    break;
                }
            case NPC_TELEST_FROST:
                {
                    Summoned->CastSpell(Summoned, SPELL_FROST_VISUAL, false);
                    break;
                }
            case NPC_TELEST_ARCANE:
                {
                    Summoned->CastSpell(Summoned, SPELL_ARCANE_VISUAL, false);
                    break;
                }
            }
            if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                Summoned->AI()->AttackStart(pTarget);
            return Summoned->GetGUID();
        }
        return 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(m_uiPhase)
        {
            case PHASE_1:
            case PHASE_3:
            case PHASE_4:
            {
                if (!m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                {
                    if (m_uiFirebombTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBOMB : SPELL_FIREBOMB_H) == CAST_OK)
                            m_uiFirebombTimer = urand(4000, 6000);
                    }
                    else
                        m_uiFirebombTimer -= uiDiff;

                    if (m_uiIceNovaTimer < uiDiff)
                    {
                        if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ICE_NOVA : SPELL_ICE_NOVA_H) == CAST_OK)
                            m_uiIceNovaTimer = urand(10000, 15000);
                    }
                    else
                        m_uiIceNovaTimer -= uiDiff;

                    if (m_uiPhase == PHASE_1 && m_creature->GetHealthPercent() < 50.0f)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_CLONES, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                        {
                            m_creature->CastStop();
                            m_creature->RemoveAllAuras();
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            FireMagusGUID = SplitPersonality(NPC_TELEST_FIRE);
                            FrostMagusGUID = SplitPersonality(NPC_TELEST_FROST);
                            ArcaneMagusGUID = SplitPersonality(NPC_TELEST_ARCANE);
                            FireMagusDead = false;
                            FrostMagusDead = false;
                            ArcaneMagusDead = false;
                            DoScriptText(urand(0, 1) ? SAY_SPLIT_1 : SAY_SPLIT_2, m_creature);
                            m_uiPhase = PHASE_2;
                        }
                    }

                    if (m_uiPhase == PHASE_3 && !m_bIsRegularMode && m_creature->GetHealthPercent() < 15.0f)
                    {
                        if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_CLONES, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                        {
                            m_creature->CastStop();
                            m_creature->RemoveAllAuras();
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            FireMagusGUID = SplitPersonality(NPC_TELEST_FIRE);
                            FrostMagusGUID = SplitPersonality(NPC_TELEST_FROST);
                            ArcaneMagusGUID = SplitPersonality(NPC_TELEST_ARCANE);
                            FireMagusDead = false;
                            FrostMagusDead = false;
                            ArcaneMagusDead = false;
                            DoScriptText(urand(0, 1) ? SAY_SPLIT_1 : SAY_SPLIT_2, m_creature);
                            m_uiPhase = PHASE_2;
                        }
                    }

                    DoMeleeAttackIfReady();
                }

                if (m_uiGravityWellTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_GRAVITY_WELL) == CAST_OK)
                        m_uiGravityWellTimer = urand(15000, 30000);
                }
                else
                    m_uiGravityWellTimer -= uiDiff;

                if (AppearDelay)
                {
                    m_creature->StopMoving();
                    m_creature->AttackStop();
                    if (AppearDelay_Timer <= uiDiff)
                    {
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        AppearDelay = false;
                    } else AppearDelay_Timer -= uiDiff;
                    return;
                }

                break;
            }
            case PHASE_2:
            {
                Unit* FireMagus;
                Unit* FrostMagus;
                Unit* ArcaneMagus;
                if (FireMagusGUID)
                    FireMagus = Unit::GetUnit((*m_creature), FireMagusGUID);
                if (FrostMagusGUID)
                    FrostMagus = Unit::GetUnit((*m_creature), FrostMagusGUID);
                if (ArcaneMagusGUID)
                    ArcaneMagus = Unit::GetUnit((*m_creature), ArcaneMagusGUID);
                if (FireMagus && FireMagus->isDead())
                {
                    FireMagusDead = true;
                    if (!AchievementTimerRunning)
                        AchievementTimerRunning = true;
                }
                if (FrostMagus && FrostMagus->isDead())
                {
                    FrostMagusDead = true;
                    if (!AchievementTimerRunning)
                        AchievementTimerRunning = true;
                }
                if (ArcaneMagus && ArcaneMagus->isDead())
                {
                    ArcaneMagusDead = true;
                    if (!AchievementTimerRunning)
                        AchievementTimerRunning = true;
                }
                if (AchievementTimerRunning)
                    AchievementTimer += uiDiff;

                if (FireMagusDead && FrostMagusDead && ArcaneMagusDead)
                {
                    if (AchievementTimer <= ACHIEV_TIMER)
                    AchievementProgress +=1;

                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMap()->CreatureRelocation(m_creature, CenterOfRoom[0][0], CenterOfRoom[0][1], CenterOfRoom[0][2], CenterOfRoom[0][3]);
                    DoCast(m_creature, SPELL_SPAWN_BACK_IN);
                    m_creature->SetVisibility(VISIBILITY_ON);
                    if (m_creature->GetHealth()*100 < m_creature->GetMaxHealth()*50)
                        m_uiPhase = PHASE_3;
                    if (!m_bIsRegularMode && m_creature->GetHealth()*100 < m_creature->GetMaxHealth()*15)
                        m_uiPhase = PHASE_4;
                    FireMagusGUID = 0;
                    FrostMagusGUID = 0;
                    ArcaneMagusGUID = 0;
                    AppearDelay = true;
                    AppearDelay_Timer = 4000;
                    DoScriptText(SAY_MERGE, m_creature);
                    AchievementTimerRunning = false;
                }else
                    return;

                break;
            }
        }
    }
};

CreatureAI* GetAI_boss_telestra(Creature* pCreature)
{
    return new boss_telestraAI(pCreature);
}

void AddSC_boss_telestra()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_telestra";
    newscript->GetAI = &GetAI_boss_telestra;
    newscript->RegisterSelf();
}
