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
SDName:
SD%Complete: 0
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum gormok
{
    SAY_WIPE                    = -1605004,
    SAY_GARROSH_BEASTS          = -1605109,
    SAY_VARIAN_BEASTS           = -1605103,
    SAY_SUMMON_JORMUNGARS       = -1605002,

    SPELL_IMPALE_10             = 66331,
    SPELL_IMPALE_10HC           = 67477,
    SPELL_IMPALE_25             = 67478,
    SPELL_IMPALE_25HC           = 67479,
    SPELL_STOMP_10              = 66330,
    SPELL_STOMP_10HC            = 67647,
    SPELL_STOMP_25              = 67648,
    SPELL_STOMP_25HC            = 67649,
    SPELL_RISING_ANGER          = 66636,

    NPC_SNOBOLD_VASSAL          = 34800,
    SPELL_SNOBOLLED             = 66406,
    SPELL_BATTER                = 66408,
    SPELL_FIREBOMB              = 66313,
    SPELL_FIREBOMB_DOT          = 66318,
    NPC_FIREBOMB                = 34854,
    SPELL_HEADCRACK             = 66407,

    SPELL_BERSERK               = 26662,
};

enum jormungars
{
    SAY_SUMMON_ICEHOWL          = -1605003,

    // acidmaw
    // mobile
    SPELL_ACID_SPEW_10          = 66819,    // on creature
    SPELL_ACID_SPEW_10HC        = 67609,
    SPELL_ACID_SPEW_25          = 67610,
    SPELL_ACID_SPEW_25HC        = 67611,
    SPELL_ACID_SPEW_TRIG        = 66818,    
    SPELL_PARALYTIC_BITE_10     = 66824,    // on target
    SPELL_PARALYTIC_BITE_10HC   = 67612,
    SPELL_PARALYTIC_BITE_25     = 67613,
    SPELL_PARALYTIC_BITE_25HC   = 67614,
    SPELL_SLIME_POOL_10         = 66883,
    SPELL_SLIME_POOL_10HC       = 67641,
    SPELL_SLIME_POOL_25         = 67642,
    SPELL_SLIME_POOL_25HC       = 67643,
    NPC_SLIME_POOL              = 35176,
    // stationary
    SPELL_ACID_SPIT_10          = 66880,    // on target
    SPELL_ACID_SPIT_10HC        = 67606,
    SPELL_ACID_SPIT_25          = 67607,
    SPELL_ACID_SPIT_25HC        = 67608,
    SPELL_PARALYTIC_SPRAY_10    = 66901,    // on target
    SPELL_PARALYTIC_SPRAY_10HC  = 67615,
    SPELL_PARALYTIC_SPRAY_25    = 67616,
    SPELL_PARALYTIC_SPRAY_25HC  = 67617,
    SPELL_SWEEP_10              = 66794,
    SPELL_SWEEP_10HC            = 67644,
    SPELL_SWEEP_25              = 67645,
    SPELL_SWEEP_25HC            = 67646,

    SPELL_SLIME_POOL_TRIG       = 66882,
    SPELL_SLIME_POOL_VISUAL     = 63084,

    SPELL_PARALYTIC_TOXIN       = 66823,
    SPELL_ENRAGE                = 68335,

    // dreadscale
    // mobile
    SPELL_BURNING_BITE_10       = 66879,    // on target
    SPELL_BURNING_BITE_10HC     = 67624,
    SPELL_BURNING_BITE_25       = 67625,
    SPELL_BURNING_BITE_25HC     = 67626,
    SPELL_BURNING_BITE_TRIG     = 66879,
    SPELL_MOLTEN_SPEW_10        = 66820,    // on creature
    SPELL_MOLTEN_SPEW_25        = 67636,
    SPELL_MOLTEN_SPEW_10HC      = 67635,
    SPELL_MOLTEN_SPEW_25HC      = 67637,  
    SPELL_MOLTEN_SPEW_TRIG      = 66821,

    // stationary
    SPELL_FIRE_SPIT_10          = 66796,    // on target
    SPELL_FIRE_SPIT_10HC        = 67632,
    SPELL_FIRE_SPIT_25          = 67633,
    SPELL_FIRE_SPIT_25HC        = 67634,
    SPELL_BURNING_SPRAY_10      = 66902,    // on target
    SPELL_BURNING_SPRAY_10HC    = 67627,
    SPELL_BURNING_SPRAY_25      = 67628,
    SPELL_BURNING_SPRAY_25HC    = 67629,

    SPELL_BURNING_BILE          = 66869,

    SPELL_SUBMERGE              = 53421,

    DISPLAY_ACID_FIXED          = 29815,
    DISPLAY_ACID_MOBILE         = 29816,
    DISPLAY_DREAD_FIXED         = 26935,
    DISPLAY_DREAD_MOBILE        = 24564,

    ACHIEV_TWO_JORMUNGARS       = 3936,
    ACHIEV_TWO_JORMUNGARS_H     = 3937,
};

enum icehowl
{
    SAY_VICTORY                 = -1605000,
    EMOTE_TRAMPLE               = -1605131,
    EMOTE_STUN                  = -1605132,

    SPELL_FEROCIOUS_BUTT_10     = 66770,
    SPELL_FEROCIOUS_BUTT_10HC   = 67654,
    SPELL_FEROCIOUS_BUTT_25     = 67655,
    SPELL_FEROCIOUS_BUTT_25HC   = 67656,
    SPELL_ARCTIC_BREATH_10      = 66689,
    SPELL_ARCTIC_BREATH_10HC    = 67650,
    SPELL_ARCTIC_BREATH_25      = 67651,
    SPELL_ARCTIC_BREATH_25HC    = 67652,
    SPELL_WHIRL_10              = 67345,
    SPELL_WHIRL_10HC            = 67663,
    SPELL_WHIRL_25              = 67664,
    SPELL_WHIRL_25HC            = 67665,
    SPELL_MASSIVE_CRASH_10      = 66683,
    SPELL_MASSIVE_CRASH_10HC    = 67660,
    SPELL_MASSIVE_CRASH_25      = 67661,
    SPELL_MASSIVE_CRASH_25HC    = 67662,
    SPELL_SURGE_OF_ADRENALINE   = 68667,    // used only in non heroic
    SPELL_TRAMPLE               = 66734,
    SPELL_STAGGERED_DAZE        = 66758,
    SPELL_FROTHING_RAGE         = 66759,

    ACHIEV_UPPER_BACK_PAIN      = 3797,
    ACHIEV_UPPER_BACK_PAIN_H    = 3813,
};

struct MANGOS_DLL_DECL boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetRespawnDelay(DAY);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    uint32 m_uiFerociousButtTimer;
    uint32 m_uiArticBreathTimer;
    uint32 m_uiWhirlTimer;
    uint32 m_uiMassiveCrashTimer;
    uint32 m_uiTrampleTimer;
    uint32 m_uiFrothingRageTimer;

    uint8 m_uiTrampleStage;

    bool m_bIsTrample;
    bool m_bTrampleCasted;
    bool m_bMovementStarted;
    bool m_bAdrenalineCasted;

    uint32 m_uiBerserkTimer;

    uint32 m_uiDoorTimer;
    bool doorClosed;

    float fPosX, fPosY, fPosZ;

    std::list<Creature*> lSnobolds;
    uint8 m_uiAchievCounter;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        m_uiFerociousButtTimer  = urand(20000,30000);
        m_uiArticBreathTimer    = urand(25000,30000);
        m_uiWhirlTimer          = urand(20000,25000);
        m_uiMassiveCrashTimer   = 45000;
        m_uiTrampleTimer        = 50000;
        m_uiFrothingRageTimer   = 30000;

        //m_uiBerserkTimer        = 300000;  // 5 min

        m_uiDoorTimer       = 5000;
        doorClosed          = false;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;

        m_uiTrampleStage    = 0;

        m_bMovementStarted  = false;
        m_bTrampleCasted    = false;
        m_bIsTrample        = false;
        m_bAdrenalineCasted = false;
        fPosX = 0;
        fPosY = 0;
        fPosZ = 0;

        lSnobolds.clear();
        m_uiAchievCounter = 0;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if ((Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != NOT_STARTED)
            {
                m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
            }
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
            m_pInstance->SetData(TYPE_STAGE,0);
        }

        if(Creature *pTirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_WIPE, pTirion);

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
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

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DONE);
            m_pInstance->SetData(TYPE_STAGE,0);
        }

        if(Creature *pTirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_VICTORY, pTirion);

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        GetCreatureListWithEntryInGrid(lSnobolds, m_creature, NPC_SNOBOLD_VASSAL, DEFAULT_VISIBILITY_INSTANCE);
        if (!lSnobolds.empty() && lSnobolds.size() >= 2)
        {
            for(std::list<Creature*>::iterator iter = lSnobolds.begin(); iter != lSnobolds.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    m_uiAchievCounter += 1;
            }
        }

        if(m_uiAchievCounter >= 2)
        {
            if(m_pInstance)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_UPPER_BACK_PAIN);
                else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_UPPER_BACK_PAIN_H);
            }
        }
    }
    void MovementInform(uint32 type, uint32 id)
    {
        if(!m_pInstance) 
            return;

        if(id != 1 && m_bMovementStarted) 
            m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
        else    
        {
            m_creature->GetMotionMaster()->MovementExpired();
            m_bMovementStarted = false;
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }
    }

    void DoCastSurgeOfAdrenaline()
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
                    i->getSource()->CastSpell(i->getSource(), SPELL_SURGE_OF_ADRENALINE, true);
            }
        } 
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        // increase damage if dazzed
        if(m_creature->HasAura(SPELL_STAGGERED_DAZE))
            uiDamage += uiDamage/2;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
            m_uiDoorTimer = 100;
        }
        else m_uiAttackStartTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDoorTimer < uiDiff && !doorClosed)
        {
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            doorClosed = true;
            m_uiDoorTimer = 30000;
        }
        else
            m_uiDoorTimer -= uiDiff;

        // spells
        if (m_bIsTrample)
        {
            if(m_uiTrampleTimer < uiDiff)
            {
                switch(m_uiTrampleStage)
                {
                    // go to center
                case 0:
                    m_creature->GetMap()->CreatureRelocation(m_creature, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, 0);
                    m_creature->SendMonsterMove(SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                    m_creature->GetMotionMaster()->MoveIdle();
                    SetCombatMovement(false);
                    m_creature->CombatStop(true);
                    m_creature->InterruptNonMeleeSpells(false);
                    ++m_uiTrampleStage;
                    m_uiTrampleTimer = 3000;
                    break;
                    // cast massive crash & stop
                case 1:
                    m_creature->GetMotionMaster()->MoveIdle();
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(m_creature, SPELL_MASSIVE_CRASH_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(m_creature, SPELL_MASSIVE_CRASH_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(m_creature, SPELL_MASSIVE_CRASH_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(m_creature, SPELL_MASSIVE_CRASH_25HC);
                    ++m_uiTrampleStage;
                    m_uiTrampleTimer = 8000;
                    break;
                    // wait 5 secs -> cast surge of addrenaline on players
                case 2:
                    if (Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    {
                        if(!m_bAdrenalineCasted)
                        {
                            m_bAdrenalineCasted = true;
                            DoCastSurgeOfAdrenaline();
                        }
                    }
                    // pick a target and run for it
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTarget->GetPosition(fPosX, fPosY, fPosZ);
                        DoScriptText(EMOTE_TRAMPLE, m_creature, pTarget);
                        m_bMovementStarted = true;
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
                        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        m_creature->GetMotionMaster()->MovePoint(1, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
                        SetCombatMovement(false);
                        if(m_creature->HasAura(SPELL_SURGE_OF_ADRENALINE, EFFECT_INDEX_0))
                            m_creature->RemoveAurasDueToSpell(SPELL_SURGE_OF_ADRENALINE);
                    }
                    ++m_uiTrampleStage;
                    m_uiTrampleTimer = 500;
                    break;
                    // run to the target; if target hit cast Trample, else go to next phase
                case 3:
                    if (m_bMovementStarted)
                    {
                        Map* pMap = m_creature->GetMap();
                        Map::PlayerList const &lPlayers = pMap->GetPlayers();
                        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                        {
                            Unit* pPlayer = itr->getSource();
                            if (!pPlayer) 
                                continue;
                            if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                            {
                                DoCast(pPlayer, SPELL_TRAMPLE);
                                m_bTrampleCasted = true;
                                m_bMovementStarted = false;
                                m_creature->GetMotionMaster()->MovementExpired();
                                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            }
                        }
                    } 
                    else 
                        ++m_uiTrampleStage;

                    if(m_bTrampleCasted)
                        ++m_uiTrampleStage;
                    break;
                    // if trample not casted, cast stun, else continue
                case 4:
                    if(!m_bTrampleCasted)
                    {
                        DoScriptText(EMOTE_STUN, m_creature);
                        DoCast(m_creature, SPELL_STAGGERED_DAZE);
                    }
                    m_bMovementStarted = false;
                    m_bAdrenalineCasted = false;
                    m_bTrampleCasted = false;
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MovementExpired();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiMassiveCrashTimer = urand(45000, 50000);
                    m_bIsTrample = false;
                    break;
                default:
                    m_uiTrampleTimer = 100000;
                }
            }else m_uiTrampleTimer -= uiDiff;
        }

        if(m_bIsTrample)
            return;

        if (m_uiFrothingRageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_FROTHING_RAGE);
            m_uiFrothingRageTimer = 40000;
        }
        else
            m_uiFrothingRageTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_uiMassiveCrashTimer < uiDiff)
        {
            m_bIsTrample = true;
            m_uiTrampleTimer = 500;
            m_uiTrampleStage = 0;
            m_uiMassiveCrashTimer = urand(45000, 50000);
        }
        else
            m_uiMassiveCrashTimer -= uiDiff;

        if (m_uiWhirlTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_WHIRL_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_WHIRL_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_WHIRL_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_WHIRL_25HC);
            m_uiWhirlTimer = urand(20000, 25000);
        }
        else
            m_uiWhirlTimer -= uiDiff;

        if (m_uiArticBreathTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_ARCTIC_BREATH_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_ARCTIC_BREATH_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_ARCTIC_BREATH_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_ARCTIC_BREATH_25HC);
            }
            m_uiArticBreathTimer = urand(25000,30000);
        }
        else
            m_uiArticBreathTimer -= uiDiff;

        if (m_uiFerociousButtTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEROCIOUS_BUTT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEROCIOUS_BUTT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEROCIOUS_BUTT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEROCIOUS_BUTT_25HC);
            }
            m_uiFerociousButtTimer = urand(20000, 30000);
        }
        else
            m_uiFerociousButtTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

struct MANGOS_DLL_DECL boss_acidmawAI : public ScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        // starts submerged
        SetCombatMovement(false);
        DoCast(pCreature, SPELL_SUBMERGE);
        pCreature->SetVisibility(VISIBILITY_OFF);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    // mobile
    uint32 m_uiAcidSpewTimer;
    uint32 m_uiParaliticBiteTimer;
    uint32 m_uiSlimePoolTimer;

    // stationary
    uint32 m_uiAcidSpitTimer;
    uint32 m_uiParaliticSprayTimer;
    uint32 m_uiSweepTimer;

    uint8 phase;
    uint32 phaseChangeTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiMoveTimer;

    bool hasEnraged;

    uint32 m_uiBerserkTimer;
    uint32 m_uiAchievTimer;

    uint32 m_uiNextBossTimer;
    bool m_bNextBoss;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        // mobile
        m_uiAcidSpewTimer       = 5000;
        m_uiParaliticBiteTimer  = urand(5000,10000);
        m_uiSlimePoolTimer      = urand(12000,15000);

        // stationary
        m_uiAcidSpitTimer       = 3000;
        m_uiParaliticSprayTimer = urand(7000,13000);
        m_uiSweepTimer          = urand(12000,15000);

        phase               = 0;    // not started yet
        phaseChangeTimer    = 45000;
        m_uiSubmergeTimer   = 60000;
        m_uiMoveTimer       = 60000;

        hasEnraged          = false;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;

        //m_uiBerserkTimer    = 300000;  // 5 min
        m_uiAchievTimer     = 0;

        m_uiNextBossTimer       = 150000;  
        m_bNextBoss             = true;

        m_creature->SetRespawnDelay(DAY);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if ((Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != NOT_STARTED)
            {
                m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
            }
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
            m_pInstance->SetData(TYPE_STAGE,0);
        }

        if(Creature *pTirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_WIPE, pTirion);

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
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
            //DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    bool IsThereAnyTwin()
    {
        if(GetClosestCreatureWithEntry(m_creature, NPC_DREADSCALE, 180.0f))
            return true;
 
        return false;
    }

    void SummonIcehowl()
    {
        if(Creature *Tirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_SUMMON_ICEHOWL, Tirion);

        if(Creature* pIcehowl = m_creature->SummonCreature(NPC_ICEHOWL, SpawnLoc[28].x, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
        {
            pIcehowl->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pIcehowl->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x, SpawnLoc[2].y - 15, SpawnLoc[2].z);
            ((boss_icehowlAI*)pIcehowl->AI())->m_uiBerserkTimer = m_uiBerserkTimer;
        }
        if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
            m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
    }

    void JustDied(Unit* pKiller)
    {
        if(!IsThereAnyTwin())
        {
            if(m_bNextBoss)
                SummonIcehowl();

            if(m_uiAchievTimer <= 10000)
            {
                if(m_pInstance)
                    m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWO_JORMUNGARS : ACHIEV_TWO_JORMUNGARS_H);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
            // phase stats
            phase = 1;
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            SetCombatMovement(false);
            m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            // door
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
        }
        else m_uiAttackStartTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!IsThereAnyTwin() && !m_creature->HasAura(SPELL_ENRAGE, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            hasEnraged = true;
            DoCast(m_creature, SPELL_ENRAGE);
        }

        if(hasEnraged)
            m_uiAchievTimer += uiDiff;

        // only for heroic
        if(m_uiNextBossTimer < uiDiff && m_bNextBoss)
        {
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            {
                SummonIcehowl();
                m_bNextBoss = false;
            }
        }
        else 
            m_uiNextBossTimer -= uiDiff;

        // stationary
        if(phase == 1)
        {
            if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiMoveTimer       = 2000;
                m_uiSubmergeTimer   = 4000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_ACID_MOBILE);
                phase = 2;
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;

            if (m_uiAcidSpitTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_ACID_SPIT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_ACID_SPIT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_ACID_SPIT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_ACID_SPIT_25HC);
                }
                m_uiAcidSpitTimer = 3000;
            }
            else
                m_uiAcidSpitTimer -= uiDiff;

            if (m_uiParaliticSprayTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_PARALYTIC_SPRAY_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_PARALYTIC_SPRAY_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_PARALYTIC_SPRAY_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_PARALYTIC_SPRAY_25HC);
                }
                m_uiParaliticSprayTimer = 21000;
            }
            else
                m_uiParaliticSprayTimer -= uiDiff;

            if (m_uiSweepTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature, SPELL_SWEEP_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature, SPELL_SWEEP_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature, SPELL_SWEEP_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature, SPELL_SWEEP_25HC);
                m_uiSweepTimer = urand(10000,15000);
            }
            else
                m_uiSweepTimer -= uiDiff;
        }

        // mobile
        if(phase == 2)
        {
            if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(false);
                m_uiMoveTimer       = 2000;
                m_uiSubmergeTimer   = 4000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_ACID_FIXED);
                phase = 1;
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                SetCombatMovement(false);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;

            if (m_uiParaliticBiteTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_PARALYTIC_BITE_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_PARALYTIC_BITE_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_PARALYTIC_BITE_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_PARALYTIC_BITE_25HC);
                }
                m_uiParaliticBiteTimer = urand(5000,7000);
            }
            else
                m_uiParaliticBiteTimer -= uiDiff;

            if (m_uiSlimePoolTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    //DoCast(m_creature, SPELL_SLIME_POOL_10);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    //DoCast(m_creature, SPELL_SLIME_POOL_25);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    //DoCast(m_creature, SPELL_SLIME_POOL_10HC);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    //DoCast(m_creature, SPELL_SLIME_POOL_25HC);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                m_uiSlimePoolTimer = urand(17000,23000);
            }
            else
                m_uiSlimePoolTimer -= uiDiff;

            if (m_uiAcidSpewTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_ACID_SPEW_TRIG);
                m_uiAcidSpewTimer = 21000;
            }
            else
                m_uiAcidSpewTimer -= uiDiff;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

struct MANGOS_DLL_DECL boss_dreadscaleAI : public ScriptedAI
{
    boss_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    // mobile
    uint32 m_uiBurningBiteTimer;
    uint32 m_uiMoltenSpewTimer;
    uint32 m_uiSlimePoolTimer;

    // stationary
    uint32 m_uiFireSpitTimer;
    uint32 m_uiBurningSprayTimer;
    uint32 m_uiSweepTimer;

    uint8 phase;
    uint32 phaseChangeTimer;
    uint32 m_uiSubmergeTimer;
    uint32 m_uiMoveTimer;

    bool hasEnraged;

    uint32 m_uiBerserkTimer;
    uint32 m_uiAchievTimer;

    uint32 m_uiNextBossTimer;
    bool m_bNextBoss;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        // mobile
        m_uiBurningBiteTimer    = urand(3000,5000);
        m_uiMoltenSpewTimer     = urand(7000,13000);
        m_uiSlimePoolTimer      = urand(13000,15000);

        // stationary
        m_uiFireSpitTimer       = 3000;
        m_uiBurningSprayTimer   = urand(5000,7000);
        m_uiSweepTimer          = urand(13000,15000);

        phase               = 0;    // not started yet
        phaseChangeTimer    = 45000;
        m_uiSubmergeTimer   = 60000;
        m_uiMoveTimer       = 60000;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;

        hasEnraged          = false;

        //m_uiBerserkTimer    = 300000;  // 5 min
        m_uiAchievTimer     = 0;

        m_uiNextBossTimer       = 150000;  
        m_bNextBoss             = true;

        m_creature->SetRespawnDelay(DAY);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if(!IsThereAnyTwin())
            {
                if ((Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != NOT_STARTED)
                {
                    m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                    m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
                }
            }
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
            m_pInstance->SetData(TYPE_STAGE,0);
        }

        if(Creature *pTirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_WIPE, pTirion);

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
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

    bool IsThereAnyTwin()
    {
        if(GetClosestCreatureWithEntry(m_creature, NPC_ACIDMAW, 180.0f))
            return true;
 
        return false;
    }

    void SummonIcehowl()
    {
        if(Creature *Tirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_SUMMON_ICEHOWL, Tirion);

        if(Creature* pIcehowl = m_creature->SummonCreature(NPC_ICEHOWL, SpawnLoc[28].x, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME))
        {
            pIcehowl->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pIcehowl->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x, SpawnLoc[2].y - 15, SpawnLoc[2].z);
            ((boss_icehowlAI*)pIcehowl->AI())->m_uiBerserkTimer = m_uiBerserkTimer;
        }
        if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
            m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
    }

    void JustDied(Unit* pKiller)
    {
        if(!IsThereAnyTwin())
        {
            if(m_bNextBoss)
                SummonIcehowl();

            if(m_uiAchievTimer <= 10000)
            {
                if(m_pInstance)
                    m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_TWO_JORMUNGARS : ACHIEV_TWO_JORMUNGARS_H);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
            // set the phase stats
            phase = 2;
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            SetCombatMovement(true);
        }
        else m_uiAttackStartTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!IsThereAnyTwin() && !m_creature->HasAura(SPELL_ENRAGE, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            hasEnraged = true;
            DoCast(m_creature, SPELL_ENRAGE);
        }

        if(hasEnraged)
            m_uiAchievTimer += uiDiff;

        // only for heroic
        if(m_uiNextBossTimer < uiDiff && m_bNextBoss && !IsThereAnyTwin())
        {
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            {
                SummonIcehowl();
                m_bNextBoss = false;
            }
        }
        else 
            m_uiNextBossTimer -= uiDiff;

        // stationary
        if(phase == 1)
        {
            if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiSubmergeTimer   = 4000;
                m_uiMoveTimer       = 2000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_DREAD_MOBILE);
                phase = 2;
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                SetCombatMovement(true);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;

            if (m_uiFireSpitTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_FIRE_SPIT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_FIRE_SPIT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_FIRE_SPIT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_FIRE_SPIT_25HC);
                }
                m_uiFireSpitTimer = urand(3000, 5000);
            }
            else
                m_uiFireSpitTimer -= uiDiff;

            if (m_uiBurningSprayTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_BURNING_SPRAY_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_BURNING_SPRAY_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_BURNING_SPRAY_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_BURNING_SPRAY_25HC);
                }
                m_uiBurningSprayTimer = 21000;
            }
            else
                m_uiBurningSprayTimer -= uiDiff;

            if (m_uiSweepTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature, SPELL_SWEEP_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature, SPELL_SWEEP_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature, SPELL_SWEEP_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature, SPELL_SWEEP_25HC);
                m_uiSweepTimer = urand(10000,15000);
            }
            else
                m_uiSweepTimer -= uiDiff;
        }

        // mobile
        if(phase == 2)
        {
            if (phaseChangeTimer < uiDiff)
            {
                m_creature->CastStop();
                DoCast(m_creature, SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(false);
                m_uiSubmergeTimer   = 4000;
                m_uiMoveTimer       = 2000;
                phaseChangeTimer    = 45000;
            }
            else
                phaseChangeTimer -= uiDiff;

            if (m_uiMoveTimer < uiDiff)
            {
                float posZ = m_creature->GetPositionZ();
                // init random location
                float angle = (float) rand()*360/RAND_MAX + 1;
                float posX = SpawnLoc[1].x + urand(0, 40)*cos(angle*(M_PI/180));
                float posY = SpawnLoc[1].y + urand(0, 40)*sin(angle*(M_PI/180));
                m_creature->GetMap()->CreatureRelocation(m_creature, posX, posY, posZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(posX, posY, posZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                m_uiMoveTimer = 60000;
            }
            else m_uiMoveTimer -= uiDiff;

            if (m_uiSubmergeTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetDisplayId(DISPLAY_DREAD_FIXED);
                phase = 1;
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                SetCombatMovement(false);
                m_uiSubmergeTimer = 60000;
            }
            else
                m_uiSubmergeTimer -= uiDiff;

            if (m_creature->HasAura(SPELL_SUBMERGE, EFFECT_INDEX_0))
                return;

            if (m_uiBurningBiteTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoCast(pTarget, SPELL_BURNING_BITE_TRIG);
                }
                m_uiBurningBiteTimer = urand(5000,7000);
            }
            else
                m_uiBurningBiteTimer -= uiDiff;

            if (m_uiSlimePoolTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    //DoCast(m_creature, SPELL_SLIME_POOL_10);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    //DoCast(m_creature, SPELL_SLIME_POOL_25);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    //DoCast(m_creature, SPELL_SLIME_POOL_10HC);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 45000);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    //DoCast(m_creature, SPELL_SLIME_POOL_25HC);
                    m_creature->SummonCreature(NPC_SLIME_POOL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                m_uiSlimePoolTimer = urand(17000,23000);
            }
            else
                m_uiSlimePoolTimer -= uiDiff;

            if (m_uiMoltenSpewTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_MOLTEN_SPEW_TRIG);
                m_uiMoltenSpewTimer = 21000;
            }
            else
                m_uiMoltenSpewTimer -= uiDiff;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

struct MANGOS_DLL_DECL boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            pCreature->ApplySpellImmune(0, IMMUNITY_EFFECT, IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK, true); // check this!!!
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint8 m_uiMaxSnobold;
    uint8 m_uiSnoboldNo;
    uint8 m_uiHealthPoint;

    uint32 m_uiStompTimer;
    uint32 m_uiImpaleTimer;

    uint32 m_uiDoorTimer;
    bool doorClosed;

    uint32 TeamInInstance;

    uint32 m_uiBerserkTimer;

    uint32 m_uiNextBossTimer;
    bool m_bNextBoss;
    uint32 m_uiIceholwTimer;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        m_uiStompTimer      = urand(20000, 25000);
        m_uiImpaleTimer     = 10000;
        m_uiSnoboldNo           = 1;

        m_uiDoorTimer       = 8000;
        doorClosed          = false;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;

        TeamInInstance = GetFaction();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            m_uiBerserkTimer    = 900000;   // 15 min
        else
            m_uiBerserkTimer    = 522000;   // 8.7 min

        m_uiNextBossTimer       = 150000;  
        m_uiIceholwTimer        = 340000;
        m_bNextBoss             = true;

        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
        {
            m_uiMaxSnobold  = 5;
            m_uiHealthPoint = 20;
        }
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
        {
            m_uiMaxSnobold  = 6;
            m_uiHealthPoint = 17;
        }

        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if ((Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != NOT_STARTED)
            {
                m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
            }
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
            m_pInstance->SetData(TYPE_STAGE,0);
        }

        if(Creature *pTirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_WIPE, pTirion);

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        m_creature->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
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

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void SummonJormungars()
    {

        if(Creature *Tirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
            DoScriptText(SAY_SUMMON_JORMUNGARS, Tirion);

        if (Creature* pDreadscale = m_creature->SummonCreature(NPC_DREADSCALE, SpawnLoc[28].x + 10, SpawnLoc[28].y, SpawnLoc[28].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            pDreadscale->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pDreadscale->GetMotionMaster()->MovePoint(0, SpawnLoc[2].x + 10, SpawnLoc[2].y - 15, SpawnLoc[2].z);
            ((boss_dreadscaleAI*)pDreadscale->AI())->m_uiBerserkTimer = m_uiBerserkTimer;
            ((boss_dreadscaleAI*)pDreadscale->AI())->m_uiNextBossTimer = m_uiIceholwTimer;
        }
        if (Creature* pAcidmaw = m_creature->SummonCreature(NPC_ACIDMAW,  552.773f, 171.971f, 394.671f, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
        {
            pAcidmaw->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            //pAcidmaw->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x - 10, SpawnLoc[1].y, SpawnLoc[1].z);
            ((boss_acidmawAI*)pAcidmaw->AI())->m_uiBerserkTimer = m_uiBerserkTimer;
            ((boss_acidmawAI*)pAcidmaw->AI())->m_uiNextBossTimer = m_uiIceholwTimer;
        }
        
        // open gate
        if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
            m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
    }

    void JustDied(Unit* pKiller)
    {
        if(m_bNextBoss)
            SummonJormungars();
    }

    void DamageTaken(Unit *pDoneBy,uint32 &uiDamage)
    {
        if (m_uiSnoboldNo > m_uiMaxSnobold)
            return;

        if (m_creature->GetHealthPercent() <= (100-(m_uiHealthPoint*m_uiSnoboldNo)))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(pTarget, SPELL_SNOBOLLED, true);
				DoCast(m_creature, SPELL_RISING_ANGER);
                if(Creature *pSnobold = m_creature->SummonCreature(NPC_SNOBOLD_VASSAL, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    pSnobold->AddThreat(pTarget,0.0f);
            }
            m_uiSnoboldNo += 1;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
            m_uiDoorTimer = 100;
        }
        else m_uiAttackStartTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDoorTimer < uiDiff && !doorClosed)
        {
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            if(TeamInInstance == ALLIANCE)
            {
                if(Creature *Varian = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_VARIAN)))
                    DoScriptText(SAY_VARIAN_BEASTS, Varian);
            }
            else if(TeamInInstance == HORDE)
            {
                if(Creature *Garrosh = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_GARROSH)))
                    DoScriptText(SAY_GARROSH_BEASTS, Garrosh);
            }
            doorClosed = true;
            m_uiDoorTimer = 30000;
        }
        else
            m_uiDoorTimer -= uiDiff;

        // only for heroic
        if(m_uiNextBossTimer < uiDiff && m_bNextBoss)
        {
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            {
                SummonJormungars();
                m_bNextBoss = false;
            }
        }
        else 
            m_uiNextBossTimer -= uiDiff;

        // iceholwTimer for heroic
        m_uiIceholwTimer -= uiDiff;

        // spells
        if (m_uiStompTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_STOMP_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_STOMP_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_STOMP_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_STOMP_25HC);
            m_uiStompTimer = 20000;
        }
        else
            m_uiStompTimer -= uiDiff;

        if (m_uiImpaleTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_IMPALE_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_IMPALE_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_IMPALE_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_IMPALE_25HC);
            }
            m_uiImpaleTimer = 10000;
        }
        else
            m_uiImpaleTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

// snobold
struct MANGOS_DLL_DECL mob_snoboldAI : public ScriptedAI
{
    mob_snoboldAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 FireBombTimer;
    uint32 m_uiBatterTimer;
    uint32 m_uiHeadcrackTimer;

    void Reset()
    {
        m_uiBatterTimer     = 3000;
        m_uiHeadcrackTimer  = 5000 + rand()%2000;
        FireBombTimer = 10000 + rand()%5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiHeadcrackTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_HEADCRACK);
            m_uiHeadcrackTimer = urand(7000, 9000);
        }else m_uiHeadcrackTimer -= uiDiff;

        if (m_uiBatterTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_BATTER);
            m_uiBatterTimer = urand(3000, 6000);
        }else m_uiBatterTimer -= uiDiff;

        if(FireBombTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_FIREBOMB);
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->SummonCreature(NPC_FIREBOMB, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
            FireBombTimer = 10000;
        }
        else FireBombTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_snobold(Creature* pCreature)
{
    return new mob_snoboldAI (pCreature);
}

// slime pool
struct MANGOS_DLL_DECL mob_slime_poolAI : public ScriptedAI
{
    mob_slime_poolAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    uint32 Difficulty;
    float m_fSize;
    uint32 m_uiSizeTimer;
    uint32 m_uiSpellTimer;
    bool m_bHasSpell;

    void Reset()
    {
        DoCast(m_creature, SPELL_SLIME_POOL_VISUAL);
        m_fSize = 1.0f;
        m_uiSizeTimer   = 5000;
        m_uiSpellTimer  = 1000;
        m_bHasSpell     = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiSizeTimer < uiDiff)
        {
            m_fSize += 0.2f;
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
            m_uiSizeTimer = 5000;
        }
        else
            m_uiSizeTimer -= uiDiff;

        if(m_uiSpellTimer < uiDiff && !m_bHasSpell)
        {
            DoCast(m_creature, SPELL_SLIME_POOL_TRIG);
            m_bHasSpell = true;
        }
        else
            m_uiSpellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_slime_pool(Creature* pCreature)
{
    return new mob_slime_poolAI (pCreature);
}

// fire bomb
struct MANGOS_DLL_DECL mob_toc_fire_bombAI : public ScriptedAI
{
    mob_toc_fire_bombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    {
        DoCast(m_creature, SPELL_FIREBOMB_DOT);
    }

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_mob_toc_fire_bomb(Creature* pCreature)
{
    return new mob_toc_fire_bombAI (pCreature);
}

void AddSC_northrend_beasts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_gormok";
    newscript->GetAI = &GetAI_boss_gormok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_snobold_vassal";
    newscript->GetAI = &GetAI_mob_snobold;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_fire_bomb";
    newscript->GetAI = &GetAI_mob_toc_fire_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_acidmaw";
    newscript->GetAI = &GetAI_boss_acidmaw;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_dreadscale";
    newscript->GetAI = &GetAI_boss_dreadscale;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_slime_pool";
    newscript->GetAI = &GetAI_mob_slime_pool;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_icehowl";
    newscript->GetAI = &GetAI_boss_icehowl;
    newscript->RegisterSelf();

}
