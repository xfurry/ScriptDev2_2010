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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    SPELL_EMERGE                = 54850,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MERGE                 = 54878,
    SPELL_SURGE                 = 54801,
    SPELL_FREEZE_ANIM           = 16245,
    SPELL_MOJO_PUDDLE           = 55627,
    SPELL_MOJO_PUDDLE_H         = 58994,
    SPELL_MOJO_WAVE             = 55626,
    SPELL_MOJO_WAVE_H           = 58993,

    NPC_LIVING_MOJO             = 29830,
    NPC_DRAKKARI_ELEMENTAL      = 29573,
};

/*######
## boss_colossus
######*/
struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bStartAttack;
    uint32 m_uiMightyBlowTimer;
    uint32 m_uiFreezeTimer;

    uint64 m_uiElementalGUID;

    bool m_bMustDie;
    bool m_bHasEmerged;
    bool m_bHasSummoned;
    bool m_bIsPhase;

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, NOT_STARTED);
        
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        DoCast(m_creature,SPELL_FREEZE_ANIM);
        m_uiMightyBlowTimer = 10000;
        m_uiFreezeTimer = 3000;
        m_bStartAttack  = true;
        m_bMustDie      = false;
        m_bHasEmerged   = false;
        m_bHasSummoned  = false;
        m_bIsPhase      = false;
        m_uiElementalGUID   = 0;

        if(Creature* pElemental = GetClosestCreatureWithEntry(m_creature, NPC_DRAKKARI_ELEMENTAL, 50.0f))
            pElemental->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);

        if(m_bIsPhase)
            m_creature->SetHealth(m_creature->GetMaxHealth()/2);

        if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);
    }

    void JustDied(Unit* pKiller)
    {
       if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, SPECIAL);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            DoCast(m_creature,SPELL_EMERGE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveAllAuras();
            m_creature->CombatStop(true);
            m_creature->InterruptNonMeleeSpells(false);
            m_bHasEmerged   = true;
            m_bMustDie      = true;
            m_uiFreezeTimer = 3000;
            m_creature->SetHealth(m_creature->GetMaxHealth());
        }
    }

    void StartPhase()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_bIsPhase      = true;
        m_bStartAttack  = true;
        m_creature->SetInCombatWithZone();
        m_creature->SetHealth(m_creature->GetMaxHealth()/2);
        if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);

        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_creature->GetHealthPercent() < 50.0f && !m_bHasSummoned)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveAllAuras();
            m_creature->CombatStop(true);
            m_bStartAttack  = false;
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature,SPELL_EMERGE);
            m_bHasEmerged   = true;
            m_bHasSummoned  = true;
            m_uiFreezeTimer = 3000;
            m_bMustDie      = false;
        }

        if (m_uiFreezeTimer <= uiDiff && m_bHasEmerged)
        {
            if(!m_bMustDie)
            {
                DoCast(m_creature,SPELL_FREEZE_ANIM);
                m_bHasEmerged = false;
                if(Creature* pElemental = m_creature->SummonCreature(NPC_DRAKKARI_ELEMENTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    pElemental->SetInCombatWithZone();
                    pElemental->AI()->AttackStart(m_creature->getVictim());
                    m_uiElementalGUID = pElemental->GetGUID();
                }
            }
            else
                m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        } else m_uiFreezeTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMightyBlowTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 10000;
        } else m_uiMightyBlowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_drakkari_elementalAI : public ScriptedAI
{
    boss_drakkari_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSurgeTimer;
    uint32 m_uiMojoPuddleTimer;

    bool m_bGoToColossus;
    bool m_bStartAttack;

    void Reset()
    {
        m_bStartAttack      = true;
        m_uiSurgeTimer      = 7000;
        m_uiMojoPuddleTimer = 2000;
        m_bGoToColossus     = false;
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
        if(Creature* pColossus = GetClosestCreatureWithEntry(m_creature, NPC_COLOSSUS, 100.0f))
        {
            if(!pColossus->isAlive())
                pColossus->Respawn();
            else
                pColossus->AI()->EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_COLOSSUS) == SPECIAL)
        {
            m_bStartAttack = true;
            m_creature->SetInCombatWithZone();
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->SetHealth(m_creature->GetMaxHealth()/2);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (m_creature->HasAura(SPELL_FREEZE_ANIM, EFFECT_INDEX_0))
                m_creature->RemoveAurasDueToSpell(SPELL_FREEZE_ANIM);

            if (m_pInstance)
                m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
        }

        if(m_creature->GetHealthPercent() < 50.0f && !m_bGoToColossus)
        {
            m_bStartAttack  = false;
            m_bGoToColossus = true;
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            //DoCast(m_creature,SPELL_FREEZE_ANIM);
            m_creature->RemoveAllAuras();
            m_creature->CombatStop(true);
            m_creature->InterruptNonMeleeSpells(false);
            m_creature->SetVisibility(VISIBILITY_OFF);
            //DoCast(m_creature, SPELL_MERGE);
            if(Creature* pColossus = GetClosestCreatureWithEntry(m_creature, NPC_COLOSSUS, 100.0f))
            {
                if(pColossus->isAlive())
                    ((boss_colossusAI*)pColossus->AI())->StartPhase();
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSurgeTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SURGE);
            m_uiSurgeTimer = 7000;
        } else m_uiSurgeTimer -= uiDiff;

        if (m_uiMojoPuddleTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);
            m_uiMojoPuddleTimer = 2000;
        } else m_uiMojoPuddleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
    }
};

struct MANGOS_DLL_DECL npc_living_mojoAI : public ScriptedAI
{
    npc_living_mojoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiMojoWaveTimer;
    uint32 uiMojoPuddleTimer;

    void Reset()
    {
        uiMojoWaveTimer = 2000;
        uiMojoPuddleTimer = 7000;
    }

    void Aggro(Unit* pWho)
    {
        //Check if the npc is near of Drakkari Colossus.
        if(Creature* pColossus = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_COLOSSUS))))
        {
            if(pColossus->isAlive() && m_creature->IsWithinDist(pColossus, 15.0f))
                m_creature->GetMotionMaster()->MovePoint(0, pColossus->GetPositionX(), pColossus->GetPositionY(), pColossus->GetPositionZ());
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE)
                return;

        switch(uiPointId)
        {
        case 0:
            m_creature->ForcedDespawn();
            break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiMojoWaveTimer <= diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_WAVE : SPELL_MOJO_WAVE_H);
            uiMojoWaveTimer = 5000;
        } else uiMojoWaveTimer -= diff;

        if (uiMojoPuddleTimer <= diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);
            uiMojoPuddleTimer = 3000;
        } else uiMojoPuddleTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

CreatureAI* GetAI_boss_drakkari_elemental(Creature* pCreature)
{
    return new boss_drakkari_elementalAI (pCreature);
}

CreatureAI* GetAI_npc_living_mojo(Creature* pCreature)
{
    return new npc_living_mojoAI (pCreature);
}

void AddSC_boss_colossus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_colossus";
    newscript->GetAI = &GetAI_boss_colossus;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_drakkari_elemental";
    newscript->GetAI = &GetAI_boss_drakkari_elemental;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_living_mojo";
    newscript->GetAI = &GetAI_npc_living_mojo;
    newscript->RegisterSelf();
}
