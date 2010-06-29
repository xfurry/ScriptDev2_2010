/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_argent_challenge
SD%Complete: 92%
SDComment: missing yells. radiance is "wrong"
SDCategory: Trial Of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum Spells
{
    // yells
    SAY_EADRIC_AGGRO            = -1614011,
    SAY_EADRIC_HAMMER           = -1614012,
    SAY_EADRIC_KILL1            = -1614013,
    SAY_EADRIC_KILL2            = -1614014,
    SAY_EADRIC_DEFEAT           = -1614015,

    SAY_PALETRESS_AGGRO         = -1614022,
    SAY_SUMMON_MEMORY           = -1614023,
    SAY_MEMORY_DIES             = -1614024,
    SAY_PALETRESS_KILL1         = -1614025,
    SAY_PALETRESS_KILL2         = -1614026,
    SAY_PALETRESS_DEFEAT        = -1614027,

    //eadric
    SPELL_VENGEANCE             = 66889,
    SPELL_RADIANCE_TRIG         = 66935,
    SPELL_RADIANCE              = 66862,
    SPELL_RADIANCE_H            = 67681,
    SPELL_HAMMER_OF_JUSTICE     = 66863,
    SPELL_STUN_AURA             = 66940,
    SPELL_HAMMER                = 66867,
    SPELL_HAMMER_DMG            = 66903,
    SPELL_HAMMER_DMG_H          = 67680,
    //paletress
    SPELL_SMITE                 = 66536,
    SPELL_SMITE_H               = 67674,
    SPELL_HOLY_FIRE             = 66538,
    SPELL_HOLY_FIRE_H           = 67676,
    SPELL_RENEW                 = 66537,
    SPELL_RENEW_H               = 67675,
    SPELL_HOLY_NOVA             = 66546,
    SPELL_SHIELD                = 66515,
    SPELL_CONFESS               = 66680,
    //memory
    SPELL_FEAR                  = 66552,
    SPELL_FEAR_H                = 67677,
    SPELL_SHADOWS               = 66619,
    SPELL_SHADOWS_H             = 67678,
    SPELL_OLD_WOUNDS            = 66620,
    SPELL_OLD_WOUNDS_H          = 67679,

    ACHIEV_FACEROLLER           = 3803,
    ACHIEV_ARGENT_CONFESSOR     = 3802,
};

// Eadric The Pure
struct MANGOS_DLL_DECL boss_eadricAI: public ScriptedAI
{
    boss_eadricAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bHasFlag = true;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHasFlag;
    uint32 m_uiCheckDelay;

    uint32 m_uiVengeance_Timer;
    uint32 m_uiRadiance_Timer;
    uint32 m_uiHammerJustice_Timer;
    uint32 m_uiHammer_Dmg_Timer;
    uint32 m_uiHammerVisualTimer;
    uint64 m_uiHammerTargetGUID;

    void Reset()
    {
        m_uiCheckDelay          = 20000;
        m_uiVengeance_Timer     = 1000;
        m_uiRadiance_Timer      = 15000;
        m_uiHammerJustice_Timer = 40000;
        m_uiHammerVisualTimer   = 42500;
        m_uiHammer_Dmg_Timer    = 45000;
        m_uiHammerTargetGUID    = 0;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_EADRIC_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_EADRIC_KILL2, m_creature); break;
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_ARGENT_CHALLENGE) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_EADRIC_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, IN_PROGRESS);
    }

    void JustDied(Unit *pKiller)
    {
        DoScriptText(SAY_EADRIC_DEFEAT, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, DONE);
            m_pInstance->SetData(TYPE_STAGE, 0);

            if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_JAEREN, 180.0f))
            {
                pAnnoucer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pAnnoucer->SetVisibility(VISIBILITY_ON);
            }

            if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_ARELAS, 180.0f))
            {
                pAnnoucer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pAnnoucer->SetVisibility(VISIBILITY_ON);
            }
        }
    }

    bool IsThereAnyTrash()
    {
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_LIGHTWIELDER, 180.0f))
            return true;
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_MONK, 180.0f))
            return true;
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_PRIESTESS, 180.0f))
            return true;
 
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCheckDelay < uiDiff)
        {
            if (!IsThereAnyTrash() && m_bHasFlag)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bHasFlag = false;
            }
        }
        else m_uiCheckDelay -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVengeance_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_VENGEANCE);
            m_uiVengeance_Timer = 12000;
        }
        else
            m_uiVengeance_Timer -= uiDiff;  

        if (m_uiRadiance_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_RADIANCE_TRIG);
            //DoCast(m_creature, m_bIsRegularMode ? SPELL_RADIANCE : SPELL_RADIANCE_H);
            m_uiRadiance_Timer = urand(15000, 20000);
        }
        else
            m_uiRadiance_Timer -= uiDiff;

        if (m_uiHammerJustice_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_HAMMER_OF_JUSTICE);
                m_uiHammerTargetGUID = pTarget->GetGUID();
            }
            m_uiHammerVisualTimer = 2000;
            m_uiHammerJustice_Timer = 40000;
        }
        else
            m_uiHammerJustice_Timer -= uiDiff;

        if (m_uiHammerVisualTimer < uiDiff)
        {
            DoScriptText(SAY_EADRIC_HAMMER, m_creature);
            if (Unit* pHammerTarget = Unit::GetUnit(*m_creature, m_uiHammerTargetGUID))
                DoCast(pHammerTarget, SPELL_HAMMER);
            m_uiHammer_Dmg_Timer = 2500;
            m_uiHammerVisualTimer = 40000;
        }
        else
            m_uiHammerVisualTimer -= uiDiff;

        if (m_uiHammer_Dmg_Timer < uiDiff)
        {
            if (Unit* pHammerTarget = Unit::GetUnit(*m_creature, m_uiHammerTargetGUID))
            {
                if(pHammerTarget->HasAura(SPELL_STUN_AURA, EFFECT_INDEX_0))
                    DoCast(pHammerTarget, m_bIsRegularMode ? SPELL_HAMMER_DMG : SPELL_HAMMER_DMG_H);
            }
            m_uiHammer_Dmg_Timer = 40000;
        }
        else m_uiHammer_Dmg_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Argent Confessor Paletress
struct MANGOS_DLL_DECL boss_paletressAI: public ScriptedAI
{
    boss_paletressAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bHasFlag = true;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHasFlag;
    uint32 m_uiCheckDelay;

    uint32 m_uiSmite_Timer;
    uint32 m_uiHoly_Fire_Timer;
    uint32 m_uiRenew_Timer;
    uint32 m_uiShield_Delay;
    uint32 m_uiMemoryEntry;
    uint64 m_uiMemoryGUID;

    bool m_bHasSummoned;
    bool m_bHasShielded;

    void Reset()
    {
        m_uiCheckDelay          = 20000;
        m_uiSmite_Timer         = 5000;
        m_uiHoly_Fire_Timer     = 10000;
        m_uiRenew_Timer         = 7000;
        m_uiShield_Delay        = 0;
        m_uiMemoryEntry         = SelectRandomMemory();
        m_uiMemoryGUID          = 0;
        m_bHasSummoned          = false;
        m_bHasShielded          = false;
        m_creature->RemoveAurasDueToSpell(SPELL_SHIELD);
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_PALETRESS_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_PALETRESS_KILL2, m_creature); break;
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_ARGENT_CHALLENGE) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_PALETRESS_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_PALETRESS_DEFEAT, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ARGENT_CHALLENGE, DONE);
            m_pInstance->SetData(TYPE_STAGE, 0);

            if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_JAEREN, 180.0f))
            {
                pAnnoucer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pAnnoucer->SetVisibility(VISIBILITY_ON);
            }

            if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_ARELAS, 180.0f))
            {
                pAnnoucer->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pAnnoucer->SetVisibility(VISIBILITY_ON);
            }
        }
    }

    bool IsThereAnyTrash()
    {
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_LIGHTWIELDER, 180.0f))
            return true;
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_MONK, 180.0f))
            return true;
        if(GetClosestCreatureWithEntry(m_creature, NPC_ARGENT_PRIESTESS, 180.0f))
            return true;
 
        return false;
    }

    uint32 SelectRandomMemory()
    {
        uint32 m_uiEntry = 0;
        switch(urand(0, 24))
        {
        case 0:
            m_uiEntry = MEMORY_ALGALON;
            break;
        case 1:
            m_uiEntry = MEMORY_ARCHIMONDE;
            break;
        case 2:
            m_uiEntry = MEMORY_CHROMAGGUS;
            break;
        case 3:
            m_uiEntry = MEMORY_CYANIGOSA;
            break;
        case 4:
            m_uiEntry = MEMORY_DELRISSA;
            break;
        case 5:
            m_uiEntry = MEMORY_ECK;
            break;
        case 6:
            m_uiEntry = MEMORY_ENTROPIUS;
            break;
        case 7:
            m_uiEntry = MEMORY_GRUUL;
            break;
        case 8:
            m_uiEntry = MEMORY_HAKKAR;
            break;
        case 9:
            m_uiEntry = MEMORY_HEIGAN;
            break;
        case 10:
            m_uiEntry = MEMORY_HEROD;
            break;
        case 11:
            m_uiEntry = MEMORY_HOGGER;
            break;
        case 12:
            m_uiEntry = MEMORY_IGNIS;
            break;
        case 13:
            m_uiEntry = MEMORY_ILLIDAN;
            break;
        case 14:
            m_uiEntry = MEMORY_INGVAR;
            break;
        case 15:
            m_uiEntry = MEMORY_KALITHRESH;
            break;
        case 16:
            m_uiEntry = MEMORY_LUCIFRON;
            break;
        case 17:
            m_uiEntry = MEMORY_MALCHEZAAR;
            break;
        case 18:
            m_uiEntry = MEMORY_MUTANUS;
            break;
        case 19:
            m_uiEntry = MEMORY_ONYXIA;
            break;
        case 20:
            m_uiEntry = MEMORY_THUNDERAAN;
            break;
        case 21:
            m_uiEntry = MEMORY_VANCLEEF;
            break;
        case 22:
            m_uiEntry = MEMORY_VASHJ;
            break;
        case 23:
            m_uiEntry = MEMORY_VEKNILASH;
            break;
        case 24:
            m_uiEntry = MEMORY_VEZAX;
            break;
        }
        return m_uiEntry;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCheckDelay < uiDiff)
        {
            if (!IsThereAnyTrash() && m_bHasFlag)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bHasFlag = false;
            }
        }
        else m_uiCheckDelay -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSmite_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SMITE : SPELL_SMITE_H);
            m_uiSmite_Timer = 2000;
        }
        else
            m_uiSmite_Timer -= uiDiff;  

        if (m_uiHoly_Fire_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_HOLY_FIRE : SPELL_HOLY_FIRE_H);
            m_uiHoly_Fire_Timer = 10000;
        }
        else
            m_uiHoly_Fire_Timer -= uiDiff;

        if (m_uiRenew_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            switch(urand(0, 1))
            {
            case 0:
                if (Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMemoryGUID))
                {
                    if (pTemp->isAlive())
                        DoCast(pTemp, m_bIsRegularMode ? SPELL_RENEW : SPELL_RENEW_H);
                    else
                        DoCast(m_creature, m_bIsRegularMode ? SPELL_RENEW : SPELL_RENEW_H);
                }
                else
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_RENEW : SPELL_RENEW_H);
                break;
            case 1:
                DoCast(m_creature, m_bIsRegularMode ? SPELL_RENEW : SPELL_RENEW_H);
                break;
            }
            m_uiRenew_Timer = urand(20000, 25000);
        }
        else
            m_uiRenew_Timer -= uiDiff;

        if (!m_bHasSummoned && m_creature->GetHealthPercent() < 25.0f)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_HOLY_NOVA);
            m_uiMemoryEntry = SelectRandomMemory();
            if(m_uiMemoryEntry != 0)
            {
                DoScriptText(SAY_SUMMON_MEMORY, m_creature);
                if (Creature* pSummon = m_creature->SummonCreature(m_uiMemoryEntry, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                {
                    pSummon->AddThreat(m_creature->getVictim());
                    m_uiMemoryGUID = pSummon->GetGUID();
                    if (pSummon->AI())
                        pSummon->AI()->AttackStart(m_creature->getVictim());

                }
                m_bHasSummoned = true;
                m_uiShield_Delay = 1000;
            }
        }

        if (m_uiShield_Delay < uiDiff && !m_bHasShielded && m_bHasSummoned)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_SHIELD);
            m_bHasShielded = true;
        }
        else
            m_uiShield_Delay -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Summoned Memory
struct MANGOS_DLL_DECL mob_toc_memoryAI: public ScriptedAI
{
    mob_toc_memoryAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiOld_Wounds_Timer;
    uint32 m_uiShadows_Timer;
    uint32 m_uiFear_Timer;

    void Reset()
    {
        m_uiOld_Wounds_Timer    = 5000;
        m_uiShadows_Timer       = 8000;
        m_uiFear_Timer          = 13000;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* pKiller)
    {
        if (Creature* pPaletress = GetClosestCreatureWithEntry(m_creature, NPC_PALETRESS, 200.0f))
        {
            if (pPaletress->isAlive())
            {
                pPaletress->RemoveAurasDueToSpell(SPELL_SHIELD);
                DoScriptText(SAY_MEMORY_DIES, pPaletress);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ARGENT_CHALLENGE) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiOld_Wounds_Timer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_OLD_WOUNDS : SPELL_OLD_WOUNDS_H);
            m_uiOld_Wounds_Timer = 10000;
        }
        else
            m_uiOld_Wounds_Timer -= diff;  

        if (m_uiFear_Timer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FEAR : SPELL_FEAR_H);
            m_uiFear_Timer = 40000;
        }
        else
            m_uiFear_Timer -= diff; 

        if (m_uiShadows_Timer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOWS : SPELL_SHADOWS_H);
            m_uiShadows_Timer = 10000;
        }
        else
            m_uiShadows_Timer -= diff; 

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eadric(Creature* pCreature)
{
    return new boss_eadricAI(pCreature);
}

CreatureAI* GetAI_boss_paletress(Creature* pCreature)
{
    return new boss_paletressAI(pCreature);
}

CreatureAI* GetAI_mob_toc_memory(Creature* pCreature)
{
    return new mob_toc_memoryAI(pCreature);
}

void AddSC_boss_argent_challenge()
{
    Script *NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_eadric";
    NewScript->GetAI = &GetAI_boss_eadric;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_paletress";
    NewScript->GetAI = &GetAI_boss_paletress;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc_memory";
    NewScript->GetAI = &GetAI_mob_toc_memory;
    NewScript->RegisterSelf();
}
