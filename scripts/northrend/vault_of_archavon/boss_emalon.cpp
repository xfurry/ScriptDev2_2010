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
SDName: Boss_Emalon_The_Storm_Watcher
SD%Complete: 0%
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

enum
{
    // Emalon spells
    SPELL_CHAIN_LIGHTNING                   = 64213,
    SPELL_CHAIN_LIGHTNING_H                 = 64215,
    SPELL_LIGHTNING_NOVA                    = 64216,
    SPELL_LIGHTNING_NOVA_H                  = 65279,
    SPELL_OVERCHARGE                        = 64379, // current spell is bugged 64218,
    SPELL_BERSERK                           = 26662,

    // Tempest Minion spells
    SPELL_SHOCK                             = 64363,
    SPELL_OVERCHARGED                       = 64217,
    SPELL_OVERCHARGED_BLAST                 = 64219,
};

/*######
## boss_emalon
######*/
struct MANGOS_DLL_DECL boss_emalonAI : public ScriptedAI
{
    boss_emalonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiChainLightningTimer;
    uint32 m_uiChainLightningCount;
    uint32 m_uiLightningNovaTimer;
    uint32 m_uiOverchargeTimer;
    uint32 m_uiRespawnTimer;
    bool m_bMustRespawn;
    uint32 m_uiCombatTimer;
    bool m_bGetInCombat;
    uint32 m_uiEnrageTimer;

    void Reset()
    {
        m_uiChainLightningTimer = 15000;
        m_uiChainLightningCount = 0;
        m_uiLightningNovaTimer  = 20000;
        m_uiOverchargeTimer     = 45000;
        m_uiRespawnTimer        = 0;
        m_bMustRespawn          = false;
        m_uiCombatTimer         = 0;
        m_bGetInCombat          = false;
        m_uiEnrageTimer         = 360000;   // 6 min       
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMALON, NOT_STARTED);

        std::list<Creature*> lMinions;
        GetCreatureListWithEntryInGrid(lMinions, m_creature, NPC_TEMPEST_MINION, 100.0f);
        if (!lMinions.empty())
        {
            for(std::list<Creature*>::iterator iter = lMinions.begin(); iter != lMinions.end(); ++iter)
            {
                if ((*iter))
                {
                    if((*iter)->isAlive())
                        (*iter)->AI()->EnterEvadeMode();
                    else
                        (*iter)->Respawn();
                }
            }
        }
    }

    void RespawnDeadMinions()
    {
        std::list<Creature*> lMinions;
        GetCreatureListWithEntryInGrid(lMinions, m_creature, NPC_TEMPEST_MINION, 100.0f);
        if (!lMinions.empty())
        {
            for(std::list<Creature*>::iterator iter = lMinions.begin(); iter != lMinions.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                {
                    (*iter)->Respawn();
                    (*iter)->AI()->AttackStart(m_creature->getVictim());
                }
            }
        }
    }

    void StartRespawn()
    {
        m_uiRespawnTimer = 4000;
        m_bMustRespawn = true;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMALON, IN_PROGRESS);

        GetMinionsInCombat(pWho);
    }

    void GetMinionsInCombat(Unit* pTarget)
    {
        std::list<Creature*> lMinions;
        GetCreatureListWithEntryInGrid(lMinions, m_creature, NPC_TEMPEST_MINION, 100.0f);
        if (!lMinions.empty())
        {
            for(std::list<Creature*>::iterator iter = lMinions.begin(); iter != lMinions.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->AI()->AttackStart(pTarget);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_EMALON, DONE);
    }

    Creature* SelectRandomMinion()
    {
        std::list<Creature*> lMinions;
        GetCreatureListWithEntryInGrid(lMinions, m_creature, NPC_TEMPEST_MINION, 100.0f);

        //This should not appear!
        if (lMinions.empty()){
            m_uiOverchargeTimer = 5000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lMinions.begin();
        advance(iter, urand(0, lMinions.size()-1));

        if((*iter)->isAlive() && (*iter)->GetHealthPercent() > 1.0f)
            return *iter;
        else
        {
            m_uiOverchargeTimer = 500;
            return NULL;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER)
            EnterEvadeMode();

        if (m_uiOverchargeTimer < uiDiff)
        {
            if(Creature* pMinion = SelectRandomMinion())
            {
                m_creature->MonsterTextEmote("%s overcharges Tempest Minion!", 0, true);
                pMinion->SetHealth(pMinion->GetMaxHealth());
                pMinion->CastSpell(pMinion, SPELL_OVERCHARGE, false);
            }
            m_uiOverchargeTimer = 45000;
        }
        else
            m_uiOverchargeTimer -= uiDiff;

        if (m_uiRespawnTimer < uiDiff && m_bMustRespawn)
        {
            RespawnDeadMinions();
            m_uiCombatTimer = 500;
            m_bGetInCombat = true;
            m_bMustRespawn = false;
        }
        else 
            m_uiRespawnTimer -= uiDiff;

        if (m_uiCombatTimer < uiDiff && m_bGetInCombat)
        {
            GetMinionsInCombat(m_creature->getVictim());
            m_bGetInCombat = false;
        }
        else
            m_uiCombatTimer -= uiDiff;

        if (m_uiChainLightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
            m_uiChainLightningTimer = 10000 + rand()%15000;
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        if (m_uiLightningNovaTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_NOVA : SPELL_LIGHTNING_NOVA_H);
            m_uiLightningNovaTimer = 45000;
        }
        else
            m_uiLightningNovaTimer -= uiDiff;

        if (m_uiEnrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiEnrageTimer = 30000;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        if (m_creature->GetDistance2d(-219.119f, -289.037f) > 80.0f)
            EnterEvadeMode();

        DoMeleeAttackIfReady();
    }
};

/*######
## npc_tempest_minion
######*/
struct MANGOS_DLL_DECL npc_tempest_minionAI : public ScriptedAI
{
    npc_tempest_minionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShockTimer;
    uint32 m_uiRespawnTimer;
    uint32 m_uiOverchargedStacksCheckTimer;
    uint32 m_uiDieTimer;
    bool m_bMustDie;

    void Reset()
    {
        m_uiShockTimer  = 8000+rand()%4000;
        m_uiDieTimer    = 0;
        m_bMustDie      = false;
        m_uiOverchargedStacksCheckTimer = 2000;
    }

    void Aggro(Unit* pWho)
    {
        if(Creature* pEmalon = GetClosestCreatureWithEntry(m_creature, NPC_EMALON, 100.0f))
            pEmalon->AI()->AttackStart(pWho);

        std::list<Creature*> lMinions;
        GetCreatureListWithEntryInGrid(lMinions, m_creature, NPC_TEMPEST_MINION, 100.0f);
        if (!lMinions.empty())
        {
            for(std::list<Creature*>::iterator iter = lMinions.begin(); iter != lMinions.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->AI()->AttackStart(pWho);
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if(Creature* pEmalon = GetClosestCreatureWithEntry(m_creature, NPC_EMALON, 100.0f))
            ((boss_emalonAI*)pEmalon->AI())->StartRespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_EMALON) == DONE)
        {
            m_creature->ForcedDespawn();
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiDieTimer < uiDiff && m_bMustDie)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDieTimer -= uiDiff;

        if (m_uiOverchargedStacksCheckTimer < uiDiff)
        {
            m_uiOverchargedStacksCheckTimer = 1000;
            Aura* pAuraOvercharged = m_creature->GetAura(SPELL_OVERCHARGED, EFFECT_INDEX_0);
            if(pAuraOvercharged && pAuraOvercharged->GetStackAmount() >= 10)
            {
                DoCast(m_creature, SPELL_OVERCHARGED_BLAST);
                m_bMustDie = true;
                m_uiDieTimer = 500;
                return;
            }
        }
        else
            m_uiOverchargedStacksCheckTimer -= uiDiff;

        if (m_uiShockTimer < uiDiff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SHOCK);
            m_uiShockTimer = urand(3000, 6000);
        }
        else
            m_uiShockTimer -= uiDiff;

        if (m_creature->GetDistance2d(-219.119f, -289.037f) > 80.0f)
            EnterEvadeMode();

        DoMeleeAttackIfReady();
    }
};

/*######
## npc_tempest_warder
######*/
struct MANGOS_DLL_DECL npc_tempest_warderAI : public ScriptedAI
{
    npc_tempest_warderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShockTimer;

    void Reset()
    {
        m_uiShockTimer = urand(4000, 8000);
    }

    void UpdateAI(const uint32 uiDiff)
    {  
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShockTimer < uiDiff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SHOCK);
            m_uiShockTimer = urand(3000, 6000);
        }
        else
            m_uiShockTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_emalonAI(Creature* pCreature)
{
    return new boss_emalonAI(pCreature);
}

CreatureAI* GetAI_npc_tempest_minionAI(Creature* pCreature)
{
    return new npc_tempest_minionAI(pCreature);
}

CreatureAI* GetAI_npc_tempest_warderAI(Creature* pCreature)
{
    return new npc_tempest_warderAI(pCreature);
}

void AddSC_boss_emalon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_emalon";
    newscript->GetAI = &GetAI_boss_emalonAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tempest_minion";
    newscript->GetAI = &GetAI_npc_tempest_minionAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tempest_warder";
    newscript->GetAI = &GetAI_npc_tempest_warderAI;
    newscript->RegisterSelf();
}
