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
SDName: Boss_Grobbulus
SD%Complete: 0
SDComment: Place holder
SDCategory: Naxxramas
EndScriptData */

/*Poison Cloud 26590
Slime Spray 28157
Fallout slime 28218
Mutating Injection 28169
Enrages 26527*/

#include "precompiled.h"
#include "naxxramas.h"

#define SPELL_BOMBARD_SLIME         28280

#define SPELL_POISON_CLOUD          28240
#define SPELL_MUTATING_INJECTION    28169
#define SPELL_SLIME_SPRAY           28157
#define H_SPELL_SLIME_SPRAY         54364
#define SPELL_BERSERK               26662

#define MOB_FALLOUT_SLIME       16290
#define MOB_GROBBOLUS_CLOUD     16363

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 PoisonCloud_Timer;
    uint32 MutatingInjection_Timer;
    uint32 SlimeSpary_Timer;
    uint32 Enrage_Timer;
    std::list<Creature*> lClouds;

    void Reset()
    {
        PoisonCloud_Timer = 15000;
        MutatingInjection_Timer = 20000;
        SlimeSpary_Timer = 15000+rand()%15000;
        Enrage_Timer = 720000;

        DespawnAdds();
        lClouds.clear();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, DONE);
        DespawnAdds();        
    }

    void Aggro(Unit *who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, IN_PROGRESS);
    }

    void DespawnAdds()
    {
        GetCreatureListWithEntryInGrid(lClouds, m_creature, MOB_GROBBOLUS_CLOUD, DEFAULT_VISIBILITY_INSTANCE);
        if (!lClouds.empty())
        {
            for(std::list<Creature*>::iterator iter = lClouds.begin(); iter != lClouds.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SLIME_SPRAY || spell->Id == H_SPELL_SLIME_SPRAY)
        {
            if (Creature* pTemp = m_creature->SummonCreature(MOB_FALLOUT_SLIME, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTemp->AddThreat(pTarget,0.0f);
                    pTemp->AI()->AttackStart(pTarget);
                }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (PoisonCloud_Timer < diff)
        {
            //DoCast(m_creature, SPELL_POISON_CLOUD);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(MOB_GROBBOLUS_CLOUD, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 75000);
            PoisonCloud_Timer = 15000;
        }else PoisonCloud_Timer -= diff;

        if (MutatingInjection_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_MUTATING_INJECTION);

            MutatingInjection_Timer = 20000;
        }else MutatingInjection_Timer -= diff;

        if (SlimeSpary_Timer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SLIME_SPRAY : H_SPELL_SLIME_SPRAY);
            SlimeSpary_Timer = 15000+rand()%15000;
        }else SlimeSpary_Timer -= diff;

        if (Enrage_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            Enrage_Timer = 300000;
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_grobbulus_poison_cloudAI : public Scripted_NoMovementAI
{
    npc_grobbulus_poison_cloudAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        Reset();
    }

    uint32 Cloud_Timer;

    void Reset()
    {
        Cloud_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Cloud_Timer < diff)
        {
            DoCast(m_creature, 59116);
            Cloud_Timer = 10000;
        }else Cloud_Timer -= diff;
    }
};

CreatureAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

CreatureAI* GetAI_npc_grobbulus_poison_cloud(Creature* pCreature)
{
    return new npc_grobbulus_poison_cloudAI(pCreature);
}

void AddSC_boss_grobbulus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grobbulus";
    newscript->GetAI = &GetAI_boss_grobbulus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_grobbulus_poison_cloud";
    newscript->GetAI = &GetAI_npc_grobbulus_poison_cloud;
    newscript->RegisterSelf();
}

