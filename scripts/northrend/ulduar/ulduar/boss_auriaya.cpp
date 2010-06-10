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
SDName: boss_auriaya
SD%Complete: 95%
SDComment: missing yells. need correct setstack for feral defender buff
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO       = -1603070,
    SAY_SLAY1       = -1603071,
    SAY_SLAY2       = -1603072,
    SAY_BERSERK     = -1603073,
    SAY_DEATH       = -1603074,
    EMOTE_SCREECH   = -1603358,
    EMOTE_DEFENDER  = -1603359,

    //auriaya
    SPELL_BERSERK				= 47008,
    SPELL_GUARDIAN_SWARM		= 64396,    // broken
    SPELL_SENTINEL_BLAST		= 64389,
    SPELL_SENTINEL_BLAST_H		= 64678,
    SPELL_SONIC_SCREECH			= 64422,
    SPELL_SONIC_SCREECH_H		= 64688,
    SPELL_FEAR					= 64386,

    //feral defender
    SPELL_FEIGN_DEATH			= 57685,
    SPELL_FERAL_ESSENCE			= 64455,
    SPELL_FERAL_POUNCE			= 64478,
    SPELL_FERAL_POUNCE_H		= 64669,
    SPELL_FERAL_RUSH			= 64496,
    SPELL_FERAL_RUSH_H			= 64674,

    //sanctum sentry
    SPELL_RIP_FLESH				= 64375,
    SPELL_RIP_FLESH_H			= 64667,
    SPELL_SAVAGE_POUNCE			= 64666,
    SPELL_SAVAGE_POUNCE_H		= 64374,
    SPELL_STRENGHT_OF_PACK		= 64381,
    //seeping feral essence
    AURA_VOID_ZONE				= 64458,
    AURA_VOID_ZONE_H			= 64676,
    //NPC ids
    MOB_VOID_ZONE				= 34098,
    MOB_FERAL_DEFENDER			= 34035,
    MOB_GUARDIAN_SWARN          = 34034,

    ACHIEV_CRAZY_CAT_LADY       = 3006,
    ACHIEV_CRAZY_CAT_LADY_H     = 3007,

    ACHIEV_NINE_LIVES           = 3076,
    ACHIEV_NINE_LIVES_H         = 3077,
};

bool m_bCrazyCatLady;
bool m_bNineLives;

// Seeping Feral Essence
struct MANGOS_DLL_DECL mob_seeping_feral_essenceAI : public ScriptedAI
{
    mob_seeping_feral_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 m_uiSpell_Timer;

    void Reset()
    {
        m_uiSpell_Timer = 1000;
        m_creature->SetDisplayId(11686);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature, m_bIsRegularMode ? AURA_VOID_ZONE : AURA_VOID_ZONE_H);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_AURIAYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_uiSpell_Timer < diff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                        i->getSource()->DealDamage(i->getSource(), m_bIsRegularMode ? 4500 : 6500, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
                }
            } 
            m_uiSpell_Timer = 1000;
        }else m_uiSpell_Timer -= diff; 
    }
};

CreatureAI* GetAI_mob_seeping_feral_essence(Creature* pCreature)
{
    return new mob_seeping_feral_essenceAI(pCreature);
}

class MANGOS_DLL_DECL StrengthOfPackAura : public Aura
{
public:
    StrengthOfPackAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};

// Sanctum Sentry
struct MANGOS_DLL_DECL mob_sanctum_sentryAI : public ScriptedAI
{
    mob_sanctum_sentryAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRip_Flesh_Timer;
    uint32 m_uiJump_Timer;
    uint32 m_uiCheck_Timer;
    uint8 m_uiSentryAlive;

    std::list<Creature*> lSentrys;

    void Reset()
    {
        m_uiRip_Flesh_Timer = 13000;
        m_uiJump_Timer = 0;
        m_uiCheck_Timer = 500;
        m_uiSentryAlive = 0;

        lSentrys.clear();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_AURIAYA))))
            {
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            }
        }

        GetCreatureListWithEntryInGrid(lSentrys, m_creature, NPC_SANCTUM_SENTRY, DEFAULT_VISIBILITY_INSTANCE);
        if (!lSentrys.empty())
        {
            for(std::list<Creature*>::iterator iter = lSentrys.begin(); iter != lSentrys.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->SetInCombatWithZone();
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        m_bCrazyCatLady = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_AURIAYA))))
            {
                if (pTemp->isAlive())
                {
                    m_creature->GetMotionMaster()->MoveFollow(pTemp,0.0f,0.0f);
                    m_creature->GetMap()->CreatureRelocation(m_creature, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ(), 0.0f);
                }
            }
        }

        if (m_uiRip_Flesh_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_RIP_FLESH : SPELL_RIP_FLESH_H);
            m_uiRip_Flesh_Timer = 13000;
        }else m_uiRip_Flesh_Timer -= diff;

        if (m_uiJump_Timer < diff)
        {
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), 8) && m_creature->IsWithinDistInMap(m_creature->getVictim(), 25))
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SAVAGE_POUNCE : SPELL_SAVAGE_POUNCE_H);
            m_uiJump_Timer = 1000;
        }else m_uiJump_Timer -= diff;

        if (m_uiCheck_Timer < diff)
        {
            lSentrys.clear();
            m_uiSentryAlive = 0;
            GetCreatureListWithEntryInGrid(lSentrys, m_creature, NPC_SANCTUM_SENTRY, 10);
            if (!lSentrys.empty())
            {
                for(std::list<Creature*>::iterator iter = lSentrys.begin(); iter != lSentrys.end(); ++iter)
                {
                    if ((*iter) && (*iter)->isAlive())
                        m_uiSentryAlive += 1;
                }
            }

            if(m_uiSentryAlive > 0)
            {
                SpellEntry* spell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_STRENGHT_OF_PACK);
                if(m_creature->AddAura(new StrengthOfPackAura(spell, EFFECT_INDEX_0, NULL, m_creature, m_creature)))
                    m_creature->GetAura(SPELL_STRENGHT_OF_PACK, EFFECT_INDEX_0)->SetStackAmount(m_uiSentryAlive);
            }
            m_uiCheck_Timer = 2100;
        }else m_uiCheck_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_sanctum_sentry(Creature* pCreature)
{
    return new mob_sanctum_sentryAI(pCreature);
}

class MANGOS_DLL_DECL FeralEssenceAura : public Aura
{
public:
    FeralEssenceAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};

// Feral Defender
struct MANGOS_DLL_DECL mob_feral_defenderAI : public ScriptedAI
{
    mob_feral_defenderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        // set feral essence to 9 stacks
        SpellEntry* spell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_FERAL_ESSENCE);
        if(m_creature->AddAura(new FeralEssenceAura(spell, EFFECT_INDEX_0, NULL, m_creature, m_creature)))
            m_creature->GetAura(SPELL_FERAL_ESSENCE, EFFECT_INDEX_0)->SetStackAmount(9);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPounce_Timer;
    uint32 m_uiRush_Start_Timer;
    uint32 m_uiRush_Finish_Timer;
    uint32 m_uiRush_Delay;
    uint32 m_uiRevive_Delay;

    bool m_bIsRush;
    bool m_bIsDead;

    void Reset()
    {
        m_uiPounce_Timer        = 5000;
        m_uiRush_Start_Timer    = 9000;
        m_bIsRush               = false;
        m_bIsDead               = false;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* pKiller)
    {
        m_bNineLives = true;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            m_creature->CastStop();
            m_creature->RemoveArenaAuras(true);
            m_creature->SummonCreature(MOB_VOID_ZONE, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            DoCast(m_creature, SPELL_FEIGN_DEATH);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (m_creature->HasAura(SPELL_FERAL_ESSENCE))
            {
                if (m_creature->GetAura(SPELL_FERAL_ESSENCE, EFFECT_INDEX_0)->GetStackAmount() == 1)
                    m_creature->RemoveAurasDueToSpell(SPELL_FERAL_ESSENCE);
                else
                    m_creature->GetAura(SPELL_FERAL_ESSENCE, EFFECT_INDEX_0)->modStackAmount(-1);

                m_uiRevive_Delay = urand(30000, 45000);
                m_bIsDead = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_AURIAYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPounce_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_FERAL_POUNCE : SPELL_FERAL_POUNCE_H);
                m_creature->AddThreat(target,0.0f);
                m_creature->AI()->AttackStart(target);
            }
            m_uiPounce_Timer = 5000;
        }else m_uiPounce_Timer -= diff;

        if (m_uiRush_Start_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_FERAL_RUSH : SPELL_FERAL_RUSH_H);
                m_creature->AddThreat(target,0.0f);
                m_creature->AI()->AttackStart(target);
            }
            m_uiRush_Start_Timer    = 35000;
            m_uiRush_Finish_Timer   = m_bIsRegularMode ? 2500 : 5000;
            m_uiRush_Delay          = 500;
            m_bIsRush               = true;
        }else m_uiRush_Start_Timer -= diff;

        if (m_uiRush_Delay < diff && m_bIsRush)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_FERAL_RUSH : SPELL_FERAL_RUSH_H);
                m_creature->AddThreat(target,0.0f);
                m_creature->AI()->AttackStart(target);
            }
            m_uiRush_Delay = 500;
        }else m_uiRush_Delay -= diff;

        if (m_uiRush_Finish_Timer < diff)
            m_bIsRush = false;
        else m_uiRush_Finish_Timer -= diff;

        if (m_uiRevive_Delay < diff && m_bIsDead)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth());
            m_creature->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_bIsDead = false;
        }else m_uiRevive_Delay -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_feral_defender(Creature* pCreature)
{
    return new mob_feral_defenderAI(pCreature);
}

// Auriaya
struct MANGOS_DLL_DECL boss_auriayaAI : public ScriptedAI
{
    boss_auriayaAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEnrage_Timer;
    uint32 m_uiSwarm_Timer;
    uint32 m_uiSonic_Screech_Timer;
    uint32 m_uiSentinel_Blast_Timer;
    uint32 m_uiFear_Timer;
    uint32 m_uiSummon_Timer;
    uint8 m_uiSwarmcount;

    std::list<Creature*> lSentrys;

    bool m_bHasBerserk;
    bool m_bIsDefender;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_uiEnrage_Timer        = 600000;
        m_uiSwarm_Timer         = urand(50000, 60000);
        m_uiSonic_Screech_Timer = urand(90000, 100000);
        m_uiSentinel_Blast_Timer = 62000;
        m_uiFear_Timer          = 60000;
        m_uiSummon_Timer        = urand(60000, 70000);
        m_uiSwarmcount          = 10;
        m_bHasBerserk           = false;
        m_bIsDefender           = false;

        lSentrys.clear();

        // achievs
        m_bCrazyCatLady = true;
        m_bNineLives    = false;
    }

    void JustDied(Unit* pKiller)
    {
        //death yell
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, DONE);

        if (m_bCrazyCatLady)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_CRAZY_CAT_LADY : ACHIEV_CRAZY_CAT_LADY_H);
        }

        if (m_bNineLives)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_NINE_LIVES : ACHIEV_NINE_LIVES_H);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, IN_PROGRESS);

        GetCreatureListWithEntryInGrid(lSentrys, m_creature, NPC_SANCTUM_SENTRY, DEFAULT_VISIBILITY_INSTANCE);
        if (!lSentrys.empty())
        {
            for(std::list<Creature*>::iterator iter = lSentrys.begin(); iter != lSentrys.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->SetInCombatWithZone();
            }
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, FAIL);

        GetCreatureListWithEntryInGrid(lSentrys, m_creature, NPC_SANCTUM_SENTRY, DEFAULT_VISIBILITY_INSTANCE);
        if (!lSentrys.empty())
        {
            for(std::list<Creature*>::iterator iter = lSentrys.begin(); iter != lSentrys.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFear_Timer < uiDiff)
        {
            DoScriptText(EMOTE_SCREECH, m_creature);
            DoCast(m_creature, SPELL_FEAR);
            m_uiFear_Timer = urand(35000, 45000);
            m_uiSentinel_Blast_Timer = 2500;
        }else m_uiFear_Timer -= uiDiff;

        if (m_uiSentinel_Blast_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SENTINEL_BLAST : SPELL_SENTINEL_BLAST_H);
            m_uiSentinel_Blast_Timer = urand(30000, 40000);
        }else m_uiSentinel_Blast_Timer -= uiDiff;

        if (m_uiSummon_Timer < uiDiff && !m_bIsDefender)
        {
            if (Creature* pTemp = m_creature->SummonCreature(MOB_FERAL_DEFENDER, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTemp->AddThreat(pTarget,0.0f);
                    pTemp->AI()->AttackStart(pTarget);
                }
                DoScriptText(EMOTE_DEFENDER, m_creature);
                m_bIsDefender = true;
            }
        }else m_uiSummon_Timer -= uiDiff;

        if (m_uiSonic_Screech_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SONIC_SCREECH : SPELL_SONIC_SCREECH_H);
            m_uiSonic_Screech_Timer = urand(40000, 50000);
        }else m_uiSonic_Screech_Timer -= uiDiff;

        if (m_uiSwarm_Timer < uiDiff)
        {
            for(int i = 0; i < 10; i++)
            {
                float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
                float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
                if (Creature* pTemp = m_creature->SummonCreature(MOB_GUARDIAN_SWARN, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoCast(pTarget, SPELL_GUARDIAN_SWARM);
                        pTemp->AddThreat(pTarget, 100.0f);
                        pTemp->SetInCombatWithZone();
                    }
                }
            }
            m_uiSwarm_Timer = urand(40000, 50000);
        }else m_uiSwarm_Timer -= uiDiff;

        if (m_uiEnrage_Timer < uiDiff && !m_bHasBerserk)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasBerserk = true;
        }else m_uiEnrage_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_auriaya(Creature* pCreature)
{
    return new boss_auriayaAI(pCreature);
}

void AddSC_boss_auriaya()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_auriaya";
    NewScript->GetAI = GetAI_boss_auriaya;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_seeping_feral_essence";
    NewScript->GetAI = &GetAI_mob_seeping_feral_essence;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_sanctum_sentry";
    NewScript->GetAI = &GetAI_mob_sanctum_sentry;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_feral_defender";
    NewScript->GetAI = &GetAI_mob_feral_defender;
    NewScript->RegisterSelf();
}