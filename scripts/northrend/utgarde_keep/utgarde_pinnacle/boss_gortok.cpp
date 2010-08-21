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
SDName: Boss_Gortok
SD%Complete: 60%
SDComment: Beast event not implemented!
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO               = -1575015,
    SAY_SLAY_1              = -1575016,
    SAY_SLAY_2              = -1575017,
    SAY_DEATH               = -1575018,

    SPELL_FREEZE_ANIM       = 16245,

    SPELL_IMPALE            = 48261,
    SPELL_IMPALE_H          = 59268,

    SPELL_WITHERING_ROAR    = 48256,
    SPELL_WITHERING_ROAR_H  = 59267,

    SPELL_ARCING_SMASH      = 48260,

    // Massive Jormungar
    SPELL_ACID_SPIT                          = 48132,
    SPELL_ACID_SPLATTER_N                    = 48136,
    SPELL_ACID_SPLATTER_H                    = 59272,
    SPELL_POISON_BREATH_N                    = 48133,
    SPELL_POISON_BREATH_H                    = 59271,
    NPC_JORMUNGAR_WORM                       = 27228,

    // Ferocious Rhino
    SPELL_GORE_N                             = 48130,
    SPELL_GORE_H                             = 59264,
    SPELL_GRIEVOUS_WOUND_N                   = 48105,
    SPELL_GRIEVOUS_WOUND_H                   = 59263,
    SPELL_STOMP                              = 48131,

    // Ravenous Furbolg
    SPELL_CHAIN_LIGHTING_N                   = 48140,
    SPELL_CHAIN_LIGHTING_H                   = 59273,
    SPELL_CRAZED                             = 48139,
    SPELL_TERRIFYING_ROAD                    = 48144,

    // Frenzied Worgen
    SPELL_MORTAL_WOUND_N                     = 48137,
    SPELL_MORTAL_WOUND_H                     = 59265,
    SPELL_ENRAGE_1                           = 48138,
    SPELL_ENRAGE_2                           = 48142
};

/*######
## boss_gortok
######*/

struct MANGOS_DLL_DECL boss_gortokAI : public ScriptedAI
{
    boss_gortokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRoarTimer;
    uint32 m_uiImpaleTimer;
    uint32 m_uiArcingSmashTimer;
    uint32 m_uiAnimalCheck_Timer;
    uint32 m_uiAnimalCounter;
    Unit* pPlayer;

    void Reset()
    {
        m_uiRoarTimer        = urand(22000,28000);
        m_uiImpaleTimer      = urand(15000,25000);
        m_uiArcingSmashTimer = urand(10000,30000);
        m_uiAnimalCheck_Timer = 1000;
        m_uiAnimalCounter = 0;
        pPlayer = NULL;
        DoCast(m_creature, SPELL_FREEZE_ANIM);

        if (m_pInstance)
        {
            if(m_creature->isAlive())
            {
                m_pInstance->SetData(TYPE_GORTOK, NOT_STARTED);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            if(GameObject* pGenerator = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_STATIS_GENERATOR)))
                pGenerator->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

            if (Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_WORGEN)))
            {
                if (pTemp->isDead())
                    ((Creature*)pTemp)->Respawn();
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            if (Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FURBOLG)))
            {
                if (pTemp->isDead())
                    ((Creature*)pTemp)->Respawn();
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            if (Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_JORMUNGAR)))
            {
                if (pTemp->isDead())
                    ((Creature*)pTemp)->Respawn();
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            if (Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RHINO)))
            {
                if (pTemp->isDead())
                    ((Creature*)pTemp)->Respawn();
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_uiAnimalCounter < 5)
            return;

        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    /*void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(pWho) &&
            !m_uiAnimalCounter && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40))
        {
            if(m_pInstance)
                m_pInstance->SetData(TYPE_GORTOK, IN_PROGRESS);

            pPlayer = pWho;
            ++m_uiAnimalCounter;
        }
    }*/

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GORTOK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // start the encounter
        if(m_pInstance && m_pInstance->GetData(TYPE_GORTOK) == SPECIAL)
        {
            ++m_uiAnimalCounter;
            m_pInstance->SetData(TYPE_GORTOK, IN_PROGRESS);
            if(GameObject* pGenerator = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_STATIS_GENERATOR)))
                pGenerator->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        }

        if (m_uiAnimalCounter)
        {
            if(m_uiAnimalCheck_Timer < uiDiff)
            {
                Creature* pTemp = NULL;

                if (m_uiAnimalCounter == 1)
                    pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_WORGEN));
                if (m_uiAnimalCounter == 2)
                    pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FURBOLG));
                if (m_uiAnimalCounter == 3)
                    pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_JORMUNGAR));
                if (m_uiAnimalCounter == 4)
                    pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RHINO));

                if (pTemp)
                {
                    if (!pTemp->isAlive())
                        ++m_uiAnimalCounter;
                    if (pTemp->isAlive() && !pTemp->getVictim())
                    {
                        if (pTemp->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
                            pTemp->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        ((Unit*)pTemp)->SetStandState(UNIT_STAND_STATE_STAND);
                        pTemp->AI()->AttackStart(pPlayer);
                    }
                }

                if (m_uiAnimalCounter == 5)
                {
                    if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
                        m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    ((Unit*)m_creature)->SetStandState(UNIT_STAND_STATE_STAND);
                    m_uiAnimalCheck_Timer = 10000000;
                    AttackStart(pPlayer);
                }
                else
                {
                    m_uiAnimalCheck_Timer = 1000;
                    return;
                }
            }else m_uiAnimalCheck_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiRoarTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WITHERING_ROAR : SPELL_WITHERING_ROAR_H, false);
            m_uiRoarTimer = urand(22000,28000);
        }else m_uiRoarTimer -= uiDiff;

        if(m_uiImpaleTimer < uiDiff)
        {
            if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H, false);
            m_uiImpaleTimer = urand(15000,25000);
        }else m_uiImpaleTimer -= uiDiff;

        if(m_uiArcingSmashTimer < uiDiff)   
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_ARCING_SMASH, false);
            m_uiArcingSmashTimer = urand(10000,30000);
        }else m_uiArcingSmashTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Massive Jormungar
######*/
struct MANGOS_DLL_DECL mob_massive_jormungarAI : public ScriptedAI
{
    mob_massive_jormungarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 AcidSpit_timer;
    uint32 AcidSplatter_timer;
    uint32 PoisonBreath_timer;

    void Reset() {
        AcidSpit_timer = 3000;
        AcidSplatter_timer = 12000;
        PoisonBreath_timer = 10000;
        if (m_pInstance)
            if (Creature* pPalehoof = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GORTOK_PALEHOOF)))
                ((boss_gortokAI*)pPalehoof->AI())->EnterEvadeMode();

        DoCast(m_creature, SPELL_FREEZE_ANIM);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Acid Spit
        if(AcidSpit_timer < uiDiff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_ACID_SPIT);
            AcidSpit_timer = 2000 + rand()%2000;
        }else AcidSpit_timer -= uiDiff;

        // Acid Splatter
        if(AcidSplatter_timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_ACID_SPLATTER_N : SPELL_ACID_SPLATTER_H);

            // spell doesn't work, summon 6 Jormungar Worm instead
            float x, y, z;
            m_creature->GetPosition(x,y,z);
            for(uint8 i = 0; i<6; ++i)
            {
                Creature* pJormungarWorm = m_creature->SummonCreature(NPC_JORMUNGAR_WORM, x+rand()%10, y+rand()%10, z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 360000);

                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (pTarget && pJormungarWorm) {
                    pJormungarWorm->AddThreat(pTarget, 0.0f);
                    pJormungarWorm->AI()->AttackStart(pTarget);
                }
            }

            AcidSplatter_timer = 10000 + rand()%4000;
        }else AcidSplatter_timer -= uiDiff;

        // Poison Breath
        if(PoisonBreath_timer < uiDiff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_POISON_BREATH_N : SPELL_POISON_BREATH_H);
            PoisonBreath_timer = 8000 + rand()%4000;
        }else PoisonBreath_timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Ferocious Rhino
######*/
struct MANGOS_DLL_DECL mob_ferocious_rhinoAI : public ScriptedAI
{
    mob_ferocious_rhinoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 Stomp_timer;
    uint32 Gore_timer;
    uint32 GrievousWound_timer;

    void Reset() {
        Stomp_timer = 10000;
        Gore_timer = 15000;
        GrievousWound_timer = 20000;

        if (m_pInstance)
            if(Creature* pPalehoof = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GORTOK_PALEHOOF)))
                ((boss_gortokAI*)pPalehoof->AI())->EnterEvadeMode();

        DoCast(m_creature, SPELL_FREEZE_ANIM);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Stomp
        if(Stomp_timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_STOMP);
            Stomp_timer = 8000 + rand()%4000;
        }else Stomp_timer -= uiDiff;

        // Gore
        if(Gore_timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_STOMP);
            Gore_timer = 13000 + rand()%4000;
        }else Gore_timer -= uiDiff;

        // Grievous Wound
        if(GrievousWound_timer < uiDiff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_GRIEVOUS_WOUND_N : SPELL_GRIEVOUS_WOUND_H);
            GrievousWound_timer = 18000 + rand()%4000;
        }else GrievousWound_timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Ravenous Furbolg
######*/
struct MANGOS_DLL_DECL mob_ravenous_furbolgAI : public ScriptedAI
{
    mob_ravenous_furbolgAI(Creature* pCreature) : ScriptedAI(pCreature) {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 ChainLightning_timer;
    uint32 Crazed_timer;
    uint32 TerrifyingRoar_timer;

    void Reset() {
        ChainLightning_timer = 5000;
        Crazed_timer = 10000;
        TerrifyingRoar_timer = 15000;

        if (m_pInstance)
            if (Creature* pPalehoof = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GORTOK_PALEHOOF)))
                ((boss_gortokAI*)pPalehoof->AI())->EnterEvadeMode();

        DoCast(m_creature, SPELL_FREEZE_ANIM);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Chain Lightning
        if(ChainLightning_timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHAIN_LIGHTING_N : SPELL_CHAIN_LIGHTING_H);
            ChainLightning_timer = 5000 + rand()%5000;
        }else ChainLightning_timer -= uiDiff;

        // Crazed
        if(Crazed_timer < uiDiff)
        {
            DoCast(m_creature, SPELL_CRAZED);
            Crazed_timer = 8000 + rand()%4000;
        }else Crazed_timer -= uiDiff;

        // Terrifying Roar
        if(TerrifyingRoar_timer < uiDiff)
        {
            DoCast(m_creature, SPELL_TERRIFYING_ROAD);
            TerrifyingRoar_timer = 10000 + rand()%10000;
        }else TerrifyingRoar_timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Frenzied Worgen
######*/
struct MANGOS_DLL_DECL mob_frenzied_worgenAI : public ScriptedAI
{
    mob_frenzied_worgenAI(Creature* pCreature) : ScriptedAI(pCreature) {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 MortalWound_timer;
    uint32 Enrage1_timer;
    uint32 Enrage2_timer;

    void Reset() {
        MortalWound_timer = 5000;
        Enrage1_timer = 15000;
        Enrage2_timer = 10000;

        if (m_pInstance)
            if (Creature* pPalehoof = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_GORTOK_PALEHOOF)))
                ((boss_gortokAI*)pPalehoof->AI())->EnterEvadeMode();

        DoCast(m_creature, SPELL_FREEZE_ANIM);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {            
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }    
    }
    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Mortal Wound
        if(MortalWound_timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MORTAL_WOUND_H : SPELL_MORTAL_WOUND_H);
            MortalWound_timer = 3000 + rand()%4000;
        }else MortalWound_timer -= uiDiff;

        // Enrage1
        if(Enrage1_timer < uiDiff)
        {
            DoCast(m_creature, SPELL_ENRAGE_1);
            Enrage1_timer = 15000;
        }else Enrage1_timer -= uiDiff;

        // Enrage2
        if(Enrage2_timer < uiDiff)
        {
            DoCast(m_creature, SPELL_ENRAGE_2);
            Enrage2_timer = 10000;
        }else Enrage2_timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool GOHello_go_statis_generator(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if(pGo->GetEntry() == GO_STASIS_GENERATOR)
        pInstance->SetData(TYPE_GORTOK, SPECIAL);

    return false;
}

CreatureAI* GetAI_boss_gortok(Creature* pCreature)
{
    return new boss_gortokAI(pCreature);
}

CreatureAI* GetAI_mob_massive_jormungar(Creature* pCreature)
{
    return new mob_massive_jormungarAI (pCreature);
}

CreatureAI* GetAI_mob_ferocious_rhino(Creature* pCreature)
{
    return new mob_ferocious_rhinoAI (pCreature);
}

CreatureAI* GetAI_mob_ravenous_furbolg(Creature* pCreature)
{
    return new mob_ravenous_furbolgAI (pCreature);
}

CreatureAI* GetAI_mob_frenzied_worgen(Creature* pCreature)
{
    return new mob_frenzied_worgenAI (pCreature);
}

void AddSC_boss_gortok()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_gortok";
    newscript->GetAI = &GetAI_boss_gortok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_massive_jormungar";
    newscript->GetAI = &GetAI_mob_massive_jormungar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ferocious_rhino";
    newscript->GetAI = &GetAI_mob_ferocious_rhino;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ravenous_furbolg";
    newscript->GetAI = &GetAI_mob_ravenous_furbolg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frenzied_worgen";
    newscript->GetAI = &GetAI_mob_frenzied_worgen;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_statis_generator";
    newscript->pGOHello = &GOHello_go_statis_generator;
    newscript->RegisterSelf();
}
