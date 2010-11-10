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
SDName: Boss_Thaddius
SD%Complete: 0
SDComment: Placeholder. Includes Feugen & Stalagg.
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    // Stalagg
    SAY_STAL_AGGRO                  = -1533023,
    SAY_STAL_SLAY                   = -1533024,
    SAY_STAL_DEATH                  = -1533025,

    SPELL_POWERSURGE                = 28134,

    //Feugen
    SAY_FEUG_AGGRO                  = -1533026,
    SAY_FEUG_SLAY                   = -1533027,
    SAY_FEUG_DEATH                  = -1533028,

    SPELL_MANABURN                  = 28135,

    //both
    SPELL_WARSTOMP                  = 28125,

    //Thaddus
    SAY_GREET                       = -1533029,
    SAY_AGGRO1                      = -1533030,
    SAY_AGGRO2                      = -1533031,
    SAY_AGGRO3                      = -1533032,
    SAY_SLAY                        = -1533033,
    SAY_ELECT                       = -1533034,
    SAY_DEATH                       = -1533035,
    SAY_SCREAM1                     = -1533036,
    SAY_SCREAM2                     = -1533037,
    SAY_SCREAM3                     = -1533038,
    SAY_SCREAM4                     = -1533039,

    SPELL_BALL_LIGHTNING            = 28299,

    EMOTE_POLARITY_SHIFT            = -1533151,

    SPELL_CHARGE_POSITIVE_DMGBUFF   = 29659,
    SPELL_CHARGE_POSITIVE_NEARDMG   = 28059,

    SPELL_CHARGE_NEGATIVE_DMGBUFF   = 29660,
    SPELL_CHARGE_NEGATIVE_NEARDMG   = 28084,

    SPELL_CHAIN_LIGHTNING           = 28167,
    SPELL_CHAIN_LIGHTNING_H         = 54531,

    SPELL_BESERK                    = 26662,



    SPELL_STATIC_FIELD              = 28135,
    SPELL_STATIC_FIELD_H            = 54528,

    SPELL_FEUGEN_CHAIN              = 28111,
    SPELL_STALAGG_CHAIN             = 28096,
    SPELL_SHOCK_OVERLOAD            = 28159,
    SPELL_SHOCK                     = 28099,
    SPELL_BREAK_CHANNEL             = 28087,

    SPELL_POSITIVE_CHARGE           = 28059,
    SPELL_NEGATIVE_CHARGE           = 28084,

    SPELL_THADIUS_STUN              = 28160,
    SPELL_THADIUS_LIGHTNING_VISUAL  = 28136,
    SPELL_POLARITY_SHIFT            = 28089,

    //generic
    //NPC_TESLA_COIL                  = 16218,                //the coils (emotes "Tesla Coil overloads!")
    EMOTE_LOSING_LINK               = -1533149,
    EMOTE_TESLA_OVERLOAD            = -1533150,

    ACHIEV_SUBSTRACTION             = 2180,
    ACHIEV_SUBSTRACTION_H           = 2181,

    ACHIEV_SHOCKING                 = 2178,     // needs advanced script support
    ACHIEV_SHOCKING_H               = 2179,
 
    PHASE_TESLA_DO_NOTHING          = 0,
    PHASE_TESLA_SETUP_CHAINS        = 1,
    PHASE_TESLA_SHOCK_OR_REAPPLY    = 2,
    PHASE_TESLA_OVERLOAD            = 3
};
 
static const float MAX_CHAIN_RANGE = 60.0;

/************
** npc_tesla_coil
************/
struct MANGOS_DLL_DECL npc_tesla_coilAI : public ScriptedAI
{
    npc_tesla_coilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bToFeugen;

    uint32 m_uiPhaseTimer;
    uint8 m_uiPhase;
    uint64 m_uiThadAddGUID;
    uint64 m_uiNoxTeslaGUID;

    void Reset()
    {
        m_uiPhase = PHASE_TESLA_SETUP_CHAINS;
        m_uiPhaseTimer = 1*IN_MILLISECONDS;;
        m_uiThadAddGUID = 0;
        m_uiNoxTeslaGUID = 0;
    }

    bool SetupChain()
    {
        if (!m_pInstance)
            return false;
        if (m_pInstance->GetData(TYPE_THADDIUS) == DONE)
            return true; // Do nothing more, if encounter finished

        GameObject* pNoxTeslaFeugen  = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CONS_NOX_TESLA_FEUGEN));
        GameObject* pNoxTeslaStalagg = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CONS_NOX_TESLA_STALAGG));

        if (!pNoxTeslaFeugen || !pNoxTeslaStalagg)
            return false;
        if (m_creature->GetDistanceOrder(pNoxTeslaFeugen, pNoxTeslaStalagg))
        {
            m_uiNoxTeslaGUID = pNoxTeslaFeugen->GetGUID();
            m_uiThadAddGUID = m_pInstance->GetData64(NPC_FEUGEN);
            m_bToFeugen = true;
        }
        else
        {
            m_uiNoxTeslaGUID = pNoxTeslaStalagg->GetGUID();
            m_uiThadAddGUID = m_pInstance->GetData64(NPC_STALAGG);
            m_bToFeugen = false;
        }

        if (Creature* pTarget = m_pInstance->instance->GetCreature(m_uiThadAddGUID))
        {
            m_pInstance->DoUseDoorOrButton(m_uiNoxTeslaGUID);
            if (DoCastSpellIfCan(pTarget, m_bToFeugen ? SPELL_FEUGEN_CHAIN : SPELL_STALAGG_CHAIN) == CAST_OK)
                return true;
        }
        return false;
    }

    void ReApplyChain()
    {
        Creature* pTarget = m_pInstance->instance->GetCreature(m_uiThadAddGUID);
        GameObject* pGo = m_pInstance->instance->GetGameObject(m_uiNoxTeslaGUID);

        if (pGo && pGo->GetGoType() == GAMEOBJECT_TYPE_BUTTON && pGo->getLootState() == GO_READY)
            pGo->UseDoorOrButton(0, false);
        if (pTarget)
            (DoCastSpellIfCan(pTarget, m_bToFeugen ? SPELL_FEUGEN_CHAIN : SPELL_STALAGG_CHAIN, CAST_INTERRUPT_PREVIOUS));
    }

    void LinkLost()
    {
        DoScriptText(EMOTE_LOSING_LINK, m_creature);
        m_uiPhaseTimer = 2*IN_MILLISECONDS;
        m_uiPhase = PHASE_TESLA_SHOCK_OR_REAPPLY;
    }

    void SetOverloading()
    {
        m_uiPhaseTimer = 14*IN_MILLISECONDS;
        m_uiPhase = PHASE_TESLA_OVERLOAD;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;
        
        switch (m_uiPhase)
        {
            case PHASE_TESLA_SETUP_CHAINS:
                if (m_uiPhaseTimer < uiDiff)
                    if (SetupChain())
                        m_uiPhase = PHASE_TESLA_DO_NOTHING;
                    else
                        m_uiPhaseTimer = 5*IN_MILLISECONDS;
                else
                    m_uiPhaseTimer -= uiDiff;
                break;
            case PHASE_TESLA_SHOCK_OR_REAPPLY:
                if (m_uiPhaseTimer < uiDiff)
                {
                    if (Creature* pTarget = m_pInstance->instance->GetCreature(m_uiThadAddGUID))
                        if (m_creature->GetDistance(pTarget)  > MAX_CHAIN_RANGE)
                        {
                            m_uiPhaseTimer = 1*IN_MILLISECONDS;
                            if (pTarget && pTarget->getVictim())
                                DoCastSpellIfCan(pTarget->getVictim(), SPELL_SHOCK);
                        }
                        else
                        {
                            ReApplyChain();
                            m_uiPhase = PHASE_TESLA_DO_NOTHING;
                        }
                }
                else
                    m_uiPhaseTimer -= uiDiff;
                break;
            case PHASE_TESLA_OVERLOAD:
                if (m_uiPhaseTimer <  uiDiff)
                {
                    m_uiPhase = PHASE_TESLA_DO_NOTHING;
                    if (Creature* pThaddius = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_THADDIUS)))
                    {
                        DoCastSpellIfCan(pThaddius,  SPELL_SHOCK_OVERLOAD, CAST_INTERRUPT_PREVIOUS);
                        DoScriptText(EMOTE_TESLA_OVERLOAD, m_creature);
                    }
                    m_pInstance->DoUseDoorOrButton(m_uiNoxTeslaGUID);
                }
                else
                    m_uiPhaseTimer -= uiDiff;
                break;
        }
    }
};

CreatureAI* GetAI_npc_tesla_coil(Creature* pCreature)
{
    return new npc_tesla_coilAI(pCreature);
}

/************
** npc_thaddiusAddsAI - Super'class' for Feugen&Stalagg
************/

struct MANGOS_DLL_DECL npc_thaddiusAddsAI : public ScriptedAI
{
    npc_thaddiusAddsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        m_uiThaddiusGUID = 0;
        m_uiStalaggGUID = 0;
        m_uiFeugenGUID = 0;

        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bFakeDeath;
    bool m_bBothDead;

    uint32 m_uiHoldTimer;
    uint32 m_uiWarStompTimer;
    uint32 m_uiReviveTimer;

    uint64 m_uiThaddiusGUID;
    uint64 m_uiStalaggGUID;
    uint64 m_uiFeugenGUID;

    void Reset()
    {
        m_bFakeDeath = false;
        m_bBothDead = false;
        m_uiReviveTimer = 5*IN_MILLISECONDS;
        m_uiHoldTimer = 2*IN_MILLISECONDS;
        m_uiWarStompTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
    }

    Creature* GetOtherAdd()
    {
        switch (m_creature->GetEntry())
        {
            case NPC_FEUGEN:  return m_pInstance->instance->GetCreature(m_uiStalaggGUID);
            case NPC_STALAGG: return m_pInstance->instance->GetCreature(m_uiFeugenGUID);
        }
        return NULL;
    }

    void JustRespawned()
    {
        Reset();

        std::list<uint64> lTeslaGUIDList;
        if (!m_pInstance)
            return;

        m_pInstance->GetThadTeslaCreatures(lTeslaGUIDList);
        if (lTeslaGUIDList.empty())
            return;
        
        for (std::list<uint64>::const_iterator itr = lTeslaGUIDList.begin(); itr != lTeslaGUIDList.end(); itr++)
            if (Creature* pTesla = m_pInstance->instance->GetCreature(*itr))
                if (npc_tesla_coilAI* pTeslaAI = dynamic_cast<npc_tesla_coilAI*> (pTesla->AI()))
                    pTeslaAI->ReApplyChain();
    }

    void JustReachedHome()
    {
        if (!m_pInstance)
            return;
        m_pInstance->SetData(TYPE_THADDIUS, FAIL);
        if (Creature* pOther = GetOtherAdd())
            if (npc_thaddiusAddsAI* pOtherAI = dynamic_cast<npc_thaddiusAddsAI*> (pOther->AI()))
                if (pOtherAI->isCountingDead())
                {
                    pOther->ForcedDespawn();
                    pOther->Respawn();
                }
    }

    void KilledUnit(Unit *pVictim)
    {
        switch (m_creature->GetEntry())
        {
            case NPC_STALAGG:   DoScriptText(SAY_STAL_SLAY, m_creature); break;
            case NPC_FEUGEN:    DoScriptText(SAY_FEUG_SLAY, m_creature); break;
        }
    }

    void Revive()
    {
        DoResetThreat();
        PauseCombatMovement();
        Reset();
    }

    bool isCountingDead()
    {
        return m_bFakeDeath || m_creature->isDead();
    }

    void PauseCombatMovement()
    {
        SetCombatMovement(false);
        m_uiHoldTimer = 2*IN_MILLISECONDS;
    }

    void Aggro(Unit* pWho)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);
    }

    virtual void UpdateAddAI(const uint32 uiDiff){}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bBothDead)
            return;
        
        if (m_bFakeDeath)
        {
            if (m_uiReviveTimer < uiDiff)
            {
                if (Creature* pOther = GetOtherAdd())
                    if (npc_thaddiusAddsAI* pOtherAI = (npc_thaddiusAddsAI*) pOther->AI())
                    {
                        if (!pOtherAI->isCountingDead())
                            Revive();
                        else
                        {
                            m_bBothDead = true;
                            pOtherAI->m_bBothDead = true;
                            // Set Teslas
                            std::list<uint64> lTeslaGUIDList;
                            m_pInstance->GetThadTeslaCreatures(lTeslaGUIDList);
                            for (std::list<uint64>::const_iterator itr = lTeslaGUIDList.begin(); itr != lTeslaGUIDList.end(); itr++)
                                if (Creature* pTesla = m_pInstance->instance->GetCreature(*itr))
                                    if (npc_tesla_coilAI* pTeslaAI = dynamic_cast<npc_tesla_coilAI*> (pTesla->AI()))
                                        pTeslaAI->SetOverloading();
                        }
                    }
            }
            else
                m_uiReviveTimer -= uiDiff;
            return;
        }
        
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHoldTimer)
        {
            if (m_uiHoldTimer <= uiDiff)
            {
                SetCombatMovement(true);
                AttackStart(m_creature->getVictim());
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiHoldTimer = 0;
            }
            else
                m_uiHoldTimer -= uiDiff;
        }

        if (m_uiWarStompTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WARSTOMP) == CAST_OK)
            m_uiWarStompTimer = urand(8*IN_MILLISECONDS, 10*IN_MILLISECONDS);
        }
        else
            m_uiWarStompTimer -= uiDiff;

        UpdateAddAI(uiDiff);                    // For Add Specific Abilities

        DoMeleeAttackIfReady();
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        if (damage < m_creature->GetHealth())
            return;
        
        if (!m_pInstance)
            return;

        //Prevent glitch if in fake death
        if (m_bFakeDeath)
        {
            damage = 0;
            return;
        }

        //prevent death
        damage = 0;
        m_bFakeDeath = true;

        m_creature->InterruptNonMeleeSpells(false);
        m_creature->SetHealth(0);
        m_creature->StopMoving();
        m_creature->ClearComboPointHolders();
        m_creature->RemoveAllAurasOnDeath();
        m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
        m_creature->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->ClearAllReactives();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

        JustDied(pKiller);
    }
};

/************
** boss_thaddius
************/

struct MANGOS_DLL_DECL boss_thaddiusAI : public Scripted_NoMovementAI
{
    boss_thaddiusAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        m_uiStalaggGUID = 0;
        m_uiFeugenGUID = 0;

        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiResetFlagsTimer;
    uint32 m_uiPolarityShiftTimer;
    uint32 m_uiChainLightningTimer;
    uint32 m_uiBallLightningTimer;
    uint32 m_uiBerserkTimer;

    uint64 m_uiStalaggGUID;
    uint64 m_uiFeugenGUID;

    uint32 m_uiPlayerCheckTimer;
    uint8 m_uiMaxRaidPlayers;
    bool m_bSubstraction;

    void Reset()
    {
        m_uiResetFlagsTimer = 5*IN_MILLISECONDS;
        m_uiPolarityShiftTimer = 15*IN_MILLISECONDS;
        m_uiChainLightningTimer = 8*IN_MILLISECONDS;
        m_uiBallLightningTimer = 1*IN_MILLISECONDS;
        m_uiBerserkTimer = 6*MINUTE*IN_MILLISECONDS;

        m_bSubstraction = true;
        m_uiPlayerCheckTimer = 1000;
        m_uiMaxRaidPlayers = m_bIsRegularMode ? 8 : 20;
    }

    void Aggro(Unit* pWho)
    {
        switch (urand(0,2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void StartEvent()
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_THADDIUS) != IN_PROGRESS)
        {
            m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);
            // Make Attackable
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            m_creature->SetInCombatWithZone();
            m_uiStalaggGUID = m_pInstance->GetData64(NPC_STALAGG);
            m_uiFeugenGUID = m_pInstance->GetData64(NPC_FEUGEN);
        }
    }

    void JustReachedHome()
    {
        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_THADDIUS, FAIL);
        RemoveAllAuras();
        // Respawn Adds:
        Creature* pFeugen  = m_pInstance->instance->GetCreature(m_uiFeugenGUID);
        Creature* pStalagg = m_pInstance->instance->GetCreature(m_uiStalaggGUID);
        if (pFeugen)
        {
            pFeugen->ForcedDespawn();
            pFeugen->Respawn();
        }
        if (pStalagg)
        {
            pStalagg->ForcedDespawn();
            pStalagg->Respawn();
        }
        Reset();
    }

    void KilledUnit(Unit *pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        RemoveAllAuras();
        
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_THADDIUS, DONE);

            if(m_bSubstraction)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_SUBSTRACTION : ACHIEV_SUBSTRACTION_H);

            // Force Despawn of Adds
            Creature* pFeugen  = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN));
            Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG));

            if (pFeugen)
                pFeugen->DealDamage(pFeugen, pFeugen->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            if (pStalagg)
                pStalagg->DealDamage(pStalagg, pStalagg->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void DoCastPolarityShift()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                uint32 SpellId = 0;
                switch (urand(0, 1))
                {
                case 0:
                    SpellId = SPELL_POSITIVE_CHARGE;
                    break;
                case 1:
                    SpellId = SPELL_NEGATIVE_CHARGE;
                    break;
                }
                if (i->getSource()->isAlive())
                {
                    if(SpellId == SPELL_POSITIVE_CHARGE)
                    {
                        if(i->getSource()->HasAura(SPELL_NEGATIVE_CHARGE, EFFECT_INDEX_0))
                            i->getSource()->RemoveAurasDueToSpell(SPELL_NEGATIVE_CHARGE);
                        i->getSource()->CastSpell(i->getSource(), SpellId, false);
                    }
                    else if(SpellId == SPELL_NEGATIVE_CHARGE)
                    {
                        if(i->getSource()->HasAura(SPELL_POSITIVE_CHARGE, EFFECT_INDEX_0))
                            i->getSource()->RemoveAurasDueToSpell(SPELL_POSITIVE_CHARGE);
                        i->getSource()->CastSpell(i->getSource(), SpellId, false);
                    }
                }
            }
        } 
    }

    void RemoveAllAuras()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                    if(i->getSource()->HasAura(SPELL_POSITIVE_CHARGE, EFFECT_INDEX_0))
                        i->getSource()->RemoveAurasDueToSpell(SPELL_POSITIVE_CHARGE);
                    if(i->getSource()->HasAura(SPELL_NEGATIVE_CHARGE, EFFECT_INDEX_0))
                        i->getSource()->RemoveAurasDueToSpell(SPELL_NEGATIVE_CHARGE);
                }
            }
        } 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_uiResetFlagsTimer)
        {
            if (m_uiResetFlagsTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_THADIUS_STUN);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                m_uiResetFlagsTimer = 0;
            }
            else
                m_uiResetFlagsTimer = (m_uiResetFlagsTimer == uiDiff) ? 1 : m_uiResetFlagsTimer - uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_pInstance)
            if(m_pInstance->GetData(TYPE_FEUGEN) != DONE && m_pInstance->GetData(TYPE_STALAGG) != DONE)
                return;

        // Players check
        if (m_uiPlayerCheckTimer < uiDiff && m_bSubstraction)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                if(PlayerList.getSize() > m_uiMaxRaidPlayers)
                    m_bSubstraction = false;
            } 
            m_uiPlayerCheckTimer = 1000;
        }
        else
            m_uiPlayerCheckTimer -= uiDiff;

        // Berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BESERK) == CAST_OK)                  // allow combat movement?
                m_uiBerserkTimer = 10*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        // Ball Lightning if target not in melee range
        if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            if (m_uiBallLightningTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BALL_LIGHTNING) == CAST_OK)
                    m_uiBallLightningTimer = 1*IN_MILLISECONDS;
            }
            else
                m_uiBallLightningTimer -= uiDiff;
        }

        // Polarity Shift
        if (m_uiPolarityShiftTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_POLARITY_SHIFT, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
            {
                DoCastPolarityShift();
                DoScriptText(SAY_ELECT, m_creature);
                DoScriptText(EMOTE_POLARITY_SHIFT, m_creature);
                m_uiPolarityShiftTimer = 30*IN_MILLISECONDS;
            }
        }
        else
            m_uiPolarityShiftTimer -= uiDiff;

        // ChainLightning
        if (m_uiChainLightningTimer < uiDiff)
        {
            Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (pTarget && DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H) == CAST_OK)
                m_uiChainLightningTimer = 15*IN_MILLISECONDS;
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thaddius(Creature* pCreature)
{
    return new boss_thaddiusAI(pCreature);
}

bool EffectDummyCreature_spell_thaddius_encounter(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    if (uiEffIndex != EFFECT_INDEX_0)
        return true;

    switch (uiSpellId)
    {
        case SPELL_BREAK_CHANNEL:
            if (pCreatureTarget->GetEntry() != NPC_TESLA_COIL)
                return false;
            if (npc_tesla_coilAI* pTeslaAI = dynamic_cast<npc_tesla_coilAI*>(pCreatureTarget->AI()))
                pTeslaAI->LinkLost();
            break;
        case SPELL_SHOCK_OVERLOAD:
            if (pCreatureTarget->GetEntry() != NPC_THADDIUS)
                return false;
            // remove Stun and then Cast
            pCreatureTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);    // needed? (in UDB set).
            pCreatureTarget->RemoveAurasDueToSpell(SPELL_THADIUS_STUN);
            pCreatureTarget->CastSpell(pCreatureTarget, SPELL_THADIUS_LIGHTNING_VISUAL, false);
            break;
        case SPELL_THADIUS_LIGHTNING_VISUAL:
            if (pCreatureTarget->GetEntry() != NPC_THADDIUS)
                return false;
            if (boss_thaddiusAI* pThaddiusAI = dynamic_cast<boss_thaddiusAI*> (pCreatureTarget->AI()))
                pThaddiusAI->StartEvent();
            break;
    }
    return true;
}

/************
** npc_stalagg
************/

struct MANGOS_DLL_DECL npc_stalaggAI : public npc_thaddiusAddsAI
{
    npc_stalaggAI(Creature* pCreature) : npc_thaddiusAddsAI(pCreature)
    {
        Reset();
    }
    uint32 m_uiPowerSurgeTimer;

    void Reset()
    {
        npc_thaddiusAddsAI::Reset();
        m_uiPowerSurgeTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_STALAGG, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_STAL_AGGRO, m_creature);

        if (!m_pInstance)
            return;

        m_uiFeugenGUID   = m_pInstance->GetData64(NPC_FEUGEN);
        m_uiStalaggGUID  = m_creature->GetGUID();

        if (Creature* pFeugen = GetOtherAdd())
            if (!pFeugen->isInCombat())
                pFeugen->SetInCombatWithZone();

        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_STAL_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_STALAGG, DONE);
    }

    void UpdateAddAI(const uint32 uiDiff)
    {
        if (m_uiPowerSurgeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_POWERSURGE) == CAST_OK)
                m_uiPowerSurgeTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);
        }
        else
            m_uiPowerSurgeTimer -= uiDiff;
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        npc_thaddiusAddsAI::DamageTaken(pKiller, damage);
    }
};

CreatureAI* GetAI_npc_stalagg(Creature* pCreature)
{
    return new npc_stalaggAI(pCreature);
}

/************
** npc_feugen
************/

struct MANGOS_DLL_DECL npc_feugenAI : public npc_thaddiusAddsAI
{
    npc_feugenAI(Creature* pCreature) : npc_thaddiusAddsAI(pCreature)
    {
        Reset();
    }
    uint32 m_uiStaticFieldTimer;
    uint32 m_uiMagneticPullTimer;                                       // TODO, missing

    void Reset()
    {
        npc_thaddiusAddsAI::Reset();
        m_uiStaticFieldTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);
        m_uiMagneticPullTimer = 20*IN_MILLISECONDS;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_FEUGEN, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_FEUG_AGGRO, m_creature);

        if (!m_pInstance)
            return;

        m_uiStalaggGUID = m_pInstance->GetData64(NPC_STALAGG);
        m_uiFeugenGUID  = m_creature->GetGUID();

        if (Creature* pStalagg = GetOtherAdd())
            if (pStalagg && !pStalagg->isInCombat())
                pStalagg->SetInCombatWithZone();

        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_FEUG_DEATH, m_creature);
        if(m_pInstance)
            m_pInstance->SetData(TYPE_FEUGEN, DONE);
    }

    void UpdateAddAI(const uint32 uiDiff)
    {
        if (m_uiStaticFieldTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STATIC_FIELD : SPELL_STATIC_FIELD_H) == CAST_OK)
                m_uiStaticFieldTimer = urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS);
        }
        else
            m_uiStaticFieldTimer -= uiDiff;
    }

    void DamageTaken(Unit* pKiller, uint32 &damage)
    {
        npc_thaddiusAddsAI::DamageTaken(pKiller, damage);
    }
};

CreatureAI* GetAI_npc_feugen(Creature* pCreature)
{
    return new npc_feugenAI(pCreature);
}

void AddSC_boss_thaddius()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_thaddius";
    NewScript->GetAI = &GetAI_boss_thaddius;
    NewScript->pEffectDummyCreature = &EffectDummyCreature_spell_thaddius_encounter;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_stalagg";
    NewScript->GetAI = &GetAI_npc_stalagg;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_feugen";
    NewScript->GetAI = &GetAI_npc_feugen;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_tesla_coil";
    NewScript->GetAI = &GetAI_npc_tesla_coil;
    NewScript->pEffectDummyCreature = &EffectDummyCreature_spell_thaddius_encounter;
    NewScript->RegisterSelf();
}
