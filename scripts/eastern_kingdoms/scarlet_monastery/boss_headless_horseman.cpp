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
SDName: boss_headless_horseman
SD%Complete: 0
SDComment: Place Holder
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"

enum
{
	// yells
    SAY_ENTRANCE           = -1189022,
    SAY_REJOINED           = -1189023,
    SAY_BODY_DEFEAT        = -1189024,
    SAY_LOST_HEAD          = -1189025,
    SAY_CONFLAGRATION      = -1189026,
    SAY_SPROUTING_PUMPKINS = -1189027,
    SAY_SLAY               = -1189028,
    SAY_DEATH              = -1189029,

    EMOTE_LAUGH            = -1189030,

    SAY_PLAYER1            = -1189031,
    SAY_PLAYER2            = -1189032,
    SAY_PLAYER3            = -1189033,
    SAY_PLAYER4            = -1189034, 

	// spells
	// active
	SPELL_CLEAVE				= 42587,
	SPELL_SUMMON_PUMPKIN		= 52236,

	// confused
	SPELL_WHIRLWIND				= 43116,
	SPELL_IMMUNE				= 42556, 
	SPELL_BODY_REGEN			= 42403,
	SPELL_CONFUSE				= 43105,
	SPELL_BODY_STAGE_1			= 42547,
    SPELL_BODY_STAGE_2			= 42548,
    SPELL_BODY_STAGE_3			= 42549,
    SPELL_COMMAND_RETURN_HEAD	= 42405,  // TARGET_SCRIPT head 23775, no visual,  when body's hp reaches up to 100%, and phase not increases
    SPELL_COMMAND_HEAD_RETURNS  = 42410,  // TARGET_SCRIPT body 23682, no visual, all ok, next phase begins, cancel regen body checks
    SPELL_COMMAND_TOSS_HEAD     = 43101,  // TARGET_SCRIPT head 23775, 10687 visual, when body "dies"

	// visuals
	SPELL_RHYME_BIG				= 42909,
    SPELL_RHYME_SMALL			= 42910,
    SPELL_BODY_FLAME			= 42074,      // apply aura, after horseman dies
    SPELL_HEAD_FLAME			= 42971,
    SPELL_ENRAGE_VISUAL			= 42438,      // he uses this spell?
    SPELL_WISP_BLUE				= 42821,
    SPELL_WISP_FLIGHT_PORT		= 42818,
    SPELL_WISP_INVIS			= 42823,
    SPELL_SMOKE					= 42355,
    SPELL_DEATH					= 42566,       //not correct spell

	// pumpkin
	SPELL_PUMPKIN_AURA			= 42280,
	SPELL_PUMPKIN_AURA_GREEN	= 42294,
	SPELL_SQUASH_SOUL			= 42514,
    SPELL_SPROUTING				= 42281,
    SPELL_SPROUT_BODY			= 42285,

	// head
	SPELL_FLYING_HEAD			= 42399,       //visual flying head
    SPELL_HEAD					= 42413,       //visual buff, "head"
    SPELL_HEAD_SPEAKS			= 43129,
    SPELL_HEAD_LANDS			= 42400,
    SPELL_HEAD_INVISIBLE		= 44312,

	// death
	SPELL_BODY_DEATH			= 42429,
    SPELL_HEAD_IS_DEAD			= 42428,       //at killing head, Phase 3, looks like explosion
    SPELL_BODY_LEAVES_COMBAT	= 43805,

	// npc
	NPC_HEADLESS_HORSEMAN		= 23682,
	NPC_HORSEMAN				= 23800,	// unmounted
	NPC_HEAD					= 23775,
	NPC_PULSING_PUMPKIN			= 23694,
	NPC_PUMPKIN_FIEND			= 23545,
	NPC_FLAME_BUNNY				= 23686,
	NPC_WISP_INVIS				= 24034
};

#define NODES_COUNT             21
#define FLIGHT_TRAVEL_TIME      16000

struct LocationsXY
{
    float x, y, z;
};

static const LocationsXY FlightPoint[NODES_COUNT]=
{
    {1754.00f,1346.00f,17.50f},
    {1765.00f,1347.00f,19.00f},
    {1784.00f,1346.80f,25.40f},
    {1803.30f,1347.60f,33.00f},
    {1824.00f,1350.00f,42.60f},
    {1838.80f,1353.20f,49.80f},
    {1852.00f,1357.60f,55.70f},
    {1861.30f,1364.00f,59.40f},
    {1866.30f,1374.80f,61.70f},
    {1864.00f,1387.30f,63.20f},
    {1854.80f,1399.40f,64.10f},
    {1844.00f,1406.90f,64.10f},
    {1824.30f,1411.40f,63.30f},
    {1801.00f,1412.30f,60.40f},
    {1782.00f,1410.10f,55.50f},
    {1770.50f,1405.20f,50.30f},
    {1765.20f,1400.70f,46.60f},
    {1761.40f,1393.40f,41.70f},
    {1759.10f,1386.70f,36.60f},
    {1757.80f,1378.20f,29.00f},
    {1758.00f,1367.00f,19.51f}
};

static const LocationsXY Spawn[]=
{
    {1776.27f,1348.74f,19.20f},        //spawn point for pumpkin shrine mob
    {1765.28f,1347.46f,17.55f}         //spawn point for smoke
};

/*

+
+struct MANGOS_DLL_DECL mob_pulsing_pumpkinAI : public ScriptedAI   // also known as pumpkin fiend 
+{
+    mob_pulsing_pumpkinAI(Creature *c) : ScriptedAI(c) {InitRootedState();}
+
+    void Reset() {}
+
+    void InitRootedState()
     {
+        SetCombatMovement(false);
+
+        //DoCast is not very good function here 
+        //little problem: first two auras here are not stackable
+        //so i commmented less important aura
+        //SELF_CAST(SPELL_PUMPKIN_AURA_GREEN,true);
+        //SELF_CAST(SPELL_PUMPKIN_AURA,true);
+        ApplyAuras(m_creature, SPELL_PUMPKIN_AURA_GREEN);
+        ApplyAuras(m_creature, SPELL_PUMPKIN_AURA);
+        SELF_CAST(SPELL_SPROUTING,false);
     }
 
-    void Aggro(Unit* pWho)
+    void SpellHit(Unit *caster, const SpellEntry *spell)
     {
-        m_creature->SetInCombatWithZone();
+        switch(spell->Id)
+        {
+        case SPELL_SPROUTING:
+            //m_creature->RemoveAllAuras(); not sure that need remove all auras
+            //SPELL_AURA_TRANSFORM works incorrect - 
+            //it must really change entry, stats of creatue, not displayid only
+            SELF_CAST(SPELL_SPROUT_BODY,true);
+            SetCombatMovement(true);
+            break;
+        case 42428:
+            m_creature->setDeathState(JUST_DIED);
+            break;
+        }
     }
 
-    void KilledUnit(Unit* pVictim)
+    void MoveInLineOfSight(Unit *who)
     {
-        DoScriptText(SAY_SLAY, m_creature);
+        if(IsCombatMovement())
+            ScriptedAI::MoveInLineOfSight(who);
+        else   
+        {
+            if (m_creature->IsWithinDist(who,0.1f) && m_creature->IsHostileTo(who))
+                m_creature->CastSpell(who,SPELL_SQUASH_SOUL,true);
+        }
     }
 
-    void JustDied(Unit* pKiller)
+    void UpdateAI(const uint32 diff)
     {
-        DoScriptText(SAY_DEATH, m_creature);
+        if(IsCombatMovement())
+            ScriptedAI::UpdateAI(diff);
     }
 
-    void UpdateAI(const uint32 uiDiff)
+    void AttackStart(Unit* who)
     {
-        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
+        if(IsCombatMovement())
+            ScriptedAI::AttackStart(who);
+    }
+
+    void EnterEvadeMode()
+    {
+        if (IsCombatMovement()){
+            ScriptedAI::EnterEvadeMode();
+            //restore some auras - that's hack, need remove only negative auras on enter evade call
+            SELF_CAST(SPELL_SPROUT_BODY,true);  
+        }
+    }
+};
+
+CreatureAI* GetAI_mob_pulsing_pumpkin(Creature* pCreature)
+{
+    return new mob_pulsing_pumpkinAI(pCreature);
+}
+
+
+
+
+struct MANGOS_DLL_DECL mob_horseman_headAI : public ScriptedAI, public EventsPool
+{
+    mob_horseman_headAI(Creature *c) : ScriptedAI(c), EventsPool(EventInfo) { Reset();}
+
+    void SaySound(int32 textEntry, Unit *target = 0)
+    {
+        DoScriptText(textEntry, m_creature, target);
+        ApplyAuras(m_creature,SPELL_HEAD_SPEAKS);
+    }
+
+    void Reset()
+    {
+        ClearTimers();
+        SetState(0xFFFFFFFF, false);
+        SetPhase(PH_ONE-1);
+    }
+
+    void MoveInLineOfSight(Unit*) {}
+    void AttackStart(Unit*){}
+    void EnterEvadeMode(){}
+
+    void DamageTaken(Unit* done_by,uint32 &damage)
+    {
+        if(!HasState(STATE_COMBAT))
+        { // set incoming dmg to null while invisible
+            damage = 0;
             return;
+        }
 
-        DoMeleeAttackIfReady();
+        uint32 health_perc = (m_creature->GetHealth() - damage)*100/m_creature->GetMaxHealth();
+
+        switch(GetPhaseMask())
+        {
+        case MSK(PH_ONE):
+            if (health_perc < 67)
+            {
+                SELF_CAST(SPELL_COMMAND_HEAD_RETURNS, true);
+                SetState(STATE_COMBAT, false);
+            }break;
+        case MSK(PH_TWO):
+            if (health_perc < 34)
+            {
+                SELF_CAST(SPELL_COMMAND_HEAD_RETURNS, true);
+                SetState(STATE_COMBAT, false);
+            }break;
+        case MSK(PH_THREE):
+            if (damage >= m_creature->GetHealth())
+            {
+                damage = m_creature->GetHealth() - m_creature->GetMaxHealth() * 0.01;
+                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
+                SELF_CAST(SPELL_HEAD_IS_DEAD,true);
+            }break;
+        }
     }
+
+    void SpellHit(Unit *caster, const SpellEntry* spell)
+    {
+        switch(spell->Id)
+        {
+        case SPELL_FLYING_HEAD:
+            error_log("HeadAI::SpellHit by SPELL_FLYING_HEAD");
+            MovePhase(true);
+            SetState(STATE_COMBAT, true);
+            SaySound(SAY_LOST_HEAD);
+            break;
+        case SPELL_COMMAND_RETURN_HEAD:
+            MovePhase(false);
+            m_creature->CastSpell(caster,SPELL_FLYING_HEAD, true);
+            SetState(STATE_COMBAT, false);
+            break;
+        }
+    }
+
+    void SpellHitTarget(Unit* target, const SpellEntry* spell)
+    {
+        if(spell->Id == SPELL_COMMAND_HEAD_RETURNS)
+            m_creature->CastSpell(target,SPELL_FLYING_HEAD, true);
+    }
+
+    void OnStateChanged(uint32 state)
+    {
+        switch(state)
+        {
+        case STATE_COMBAT:
+            if (HasState(STATE_COMBAT))
+            {
+                error_log("HeadAI applying STATE_COMBAT");
+                m_creature->RemoveAurasDueToSpell(SPELL_HEAD_INVISIBLE);
+                SELF_CAST(SPELL_HEAD_LANDS,true);
+                ApplyAuras(m_creature,SPELL_HEAD);
+
+                SheduleProcess(EV_LAUGHT,11000,5000,12000);
+                SheduleProcess(EV_HEAD_FLEE,1000);
+            }
+            else
+            {
+                error_log("HeadAI removing STATE_COMBAT");
+                m_creature->RemoveAllAuras();
+                ApplyAuras(m_creature,SPELL_HEAD_INVISIBLE);
+                m_creature->SetHealth(m_creature->GetMaxHealth());
+                //m_creature->CombatStop();
+            }
+            break;
+        }
+    }
+
+    void ProcessEvent(uint32 id)
+    {
+        switch(id)
+        {
+        case EV_LAUGHT:
+            SaySound(EMOTE_LAUGH);
+            break;
+        case EV_HEAD_FLEE:
+            if (Unit* u = m_creature->getAttackerForHelper())
+            {
+                m_creature->GetMotionMaster()->Clear(false);
+                m_creature->GetMotionMaster()->MoveFleeing(u);
+            }break;
+        }
+    }
+
+    void UpdateAI(const uint32 diff)
+    {
+        EventsPool::Update(diff);
+    }
 };
 
+CreatureAI* GetAI_mob_horseman_head(Creature* pCreature)
+{
+    return new mob_horseman_headAI(pCreature);
+}
+
+struct MANGOS_DLL_DECL boss_headless_horsemanAI : public ScriptedAI, public EventsPool
+{
+    boss_headless_horsemanAI(Creature *c) : ScriptedAI(c), EventsPool(EventInfo)
+    {
+        /*if (SpellEntry *confl = GET_SPELL(SPELL_CONFLAGRATION))
+        {
+            confl->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE_PERCENT;
+            confl->EffectBasePoints[0] = 10;
+            confl->EffectBaseDice[0] = 10;
+            confl->DmgMultiplier[0] = 1;
+        }
+        Reset();
+    }
+
+    uint64 headguid;
+
+    void Reset()
+    {
+        headguid = 0;
+        ClearTimers();
+        //for testing
+        SetState(0xFFFFFFFF, false);//clean states
+
+        SetState(STATE_MOUNTED, true);
+        SetPhase(PH_INTRO);
+    }
+
+    void MoveInLineOfSight(Unit *who)
+    {
+        if (HasState(STATE_COMBAT))
+            ScriptedAI::MoveInLineOfSight(who);
+    }
+
+    void AttackStart(Unit* who)
+    {
+        if (HasState(STATE_COMBAT))
+            ScriptedAI::AttackStart(who);
+    }
+
+    void KilledUnit(Unit*) { SaySound(SAY_PLAYER1 - rand()%4);}
+    void JustDied(Unit*) { SaySound(SAY_DEATH);}
+
+    void SaySound(int32 textEntry, Unit *target = 0)
+    {
+        DoScriptText(textEntry, m_creature, target);
+        ApplyAuras(m_creature,SPELL_HEAD_SPEAKS);
+        if(EventTimer *t = GetEventTimer(EV_LAUGHT))
+            t->ModifyTime(4000);
+    }
+
+    void SpellHit(Unit *caster, const SpellEntry* spell)
+    {
+        switch(spell->Id)
+        {
+            case SPELL_COMMAND_HEAD_RETURNS:
+                CancelEvent(EV_SPELL_42403_TICK);
+                break;
+            case SPELL_FLYING_HEAD:
+                caster->GetMotionMaster()->Clear(false);
+                caster->GetMotionMaster()->MoveFollow(m_creature,10.0f,float(rand()%6));
+
+                MovePhase(true);
+
+                SetState(STATE_HEADLESS,false);
+                SetState(STATE_MOUNTED|STATE_COMBAT,true);
+
+                SaySound(SAY_REJOINED);
+                break;
+            case SPELL_HEAD_IS_DEAD:
+                SELF_CAST(SPELL_HEAD_IS_DEAD, true);
+                SELF_CAST(SPELL_BODY_DEATH, true);
+                break;
+        }
+    }
+
+    void SpellHitTarget(Unit* target, const SpellEntry* spell)
+    {
+        if(spell->Id == SPELL_COMMAND_TOSS_HEAD)
+            m_creature->CastSpell(target,SPELL_FLYING_HEAD,true);
+    }
+
+    void DamageTaken(Unit *done_by, uint32 &damage)
+    {
+        if (damage >= m_creature->GetHealth())
+        {
+            damage = 0;//m_creature->GetHealth() - m_creature->GetMaxHealth()/100;
+            m_creature->SetHealth(1);
+
+            SetState(HEAD_SPAWNED, true);
+            SELF_CAST(SPELL_COMMAND_TOSS_HEAD, true);
+
+            SetState(STATE_MOUNTED|STATE_COMBAT, false);
+            SetState(STATE_HEADLESS,true);
+        }
+    }
+
+    void OnStateChanged(uint32 state)
+    {
+        bool apply = HasState(state);
+        switch(state)
+        {
+        case STATE_MOUNTED:
+            if(apply)
+            {
+                ApplyAuras(m_creature, SPELL_HEAD);
+                SheduleProcess(EV_LAUGHT,11000,5000,12000);
+            }
+            else
+                m_creature->RemoveAurasDueToSpell(SPELL_HEAD);
+            break;
+        case STATE_HEADLESS:
+            if(apply)
+            {
+                //must be part of confused mov generator
+                m_creature->SetTargetGUID(0);
+                SaySound(SAY_BODY_DEFEAT);
+
+                //it's core problem - damage can remove auras below 
+                ApplyAuras(m_creature, SPELL_IMMUNE);
+                ApplyAuras(m_creature, SPELL_CONFUSE);
+                ApplyAuras(m_creature, SPELL_BODY_REGEN);
+
+                SheduleProcess(EV_SPELL_42403_TICK,1000);
+                SheduleProcess(EV_WHIRLWIND,3000,3000,4000);
+            }
+            else
+            {
+                m_creature->RemoveAurasDueToSpell(SPELL_IMMUNE);
+                m_creature->RemoveAurasDueToSpell(SPELL_BODY_REGEN);
+                m_creature->RemoveAurasDueToSpell(SPELL_CONFUSE);
+                m_creature->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
+            }
+            break;
+        case HEAD_SPAWNED:
+            if (apply)
+            {
+                float x, y, z;
+                m_creature->GetClosePoint(x,y,z,m_creature->GetObjectSize(),float(rand()%6),float(rand()%6));
+                headguid = m_creature->SummonCreature(HEAD,x,y,z,0.0f, TEMPSUMMON_DEAD_DESPAWN, 0)->GetGUID();
+            }
+            else if(Unit* u = Unit::GetUnit((*m_creature), headguid))
+            {
+                u->AddObjectToRemoveList();
+                headguid = 0;
+            }
+            break;
+        case MSK(PH_INTRO):
+            if(apply)
+            {
+                m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
+                SendMoveByPath(m_creature, FLIGHT_TRAVEL_TIME);
+                SheduleProcess(EV_INTRO_TIMER, FLIGHT_TRAVEL_TIME,FLIGHT_TRAVEL_TIME);
+            }
+            else
+            {
+                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
+                const Locations& loc = FlightPoint[NODES_COUNT-1];
+                m_creature->Relocate(loc.x,loc.y,loc.z);
+                SaySound(SAY_ENTRANCE);
+            }break;
+        case MSK(PH_ONE):
+            if(apply)
+            {
+                ApplyAuras(m_creature,SPELL_BODY_STAGE_1);
+                SheduleProcess(EV_CLEAVE,2000,0,4000);
+            }else
+                m_creature->RemoveAurasDueToSpell(SPELL_BODY_STAGE_1);
+            break;
+        case MSK(PH_TWO):
+            if(apply)
+            {
+                ApplyAuras(m_creature,SPELL_BODY_STAGE_2);
+                SheduleProcess(EV_CONFLAGRATE,10000,15000,7000);
+            }else
+                m_creature->RemoveAurasDueToSpell(SPELL_BODY_STAGE_2);
+            break;
+        case MSK(PH_THREE):
+            if(apply)
+            {
+                ApplyAuras(m_creature,SPELL_BODY_STAGE_3);
+                SheduleProcess(EV_SPROUT_PUMPKINS,25000,15000,11000);
+            }
+            else
+                m_creature->RemoveAurasDueToSpell(SPELL_BODY_STAGE_3);
+            break;
+        }
+    }
+
+    void ProcessEvent(uint32 ev_id)
+    {
+        switch (ev_id)
+        {
+        case EV_CLEAVE:
+            if(m_creature->getVictim())
+                m_creature->CastSpell(m_creature->getVictim(),SPELL_CLEAVE, false);
+            break;
+        case EV_LAUGHT:
+            m_creature->MonsterTextEmote(EMOTE_LAUGH,NULL);
+            DoPlaySoundToSet(m_creature, RandomLaugh[rand()%3]);
+            break;
+        case EV_CONFLAGRATE:
+            if (Unit *u = SelectUnit(SELECT_TARGET_RANDOM,0))
+            {
+                SaySound(SAY_CONFLAGRATION, u);
+                m_creature->CastSpell(u,SPELL_CONFLAGRATION,false);
+                //u->CastSpell(u,SPELL_CONFL_SPEED,true);  SPELL_CONFL_SPEED must be casterd after SPELL_CONFLAGRATION(core)
+            }break;
+        case EV_SPROUT_PUMPKINS:
+            SELF_CAST(SPELL_SUMMON_PUMPKIN,false);
+            SaySound(SAY_SPROUTING_PUMPKINS);
+            break;
+        case EV_WHIRLWIND:
+            // other auras removes: (confused, immune and regenerate aura) :(.. fucking mangos
+            if (rand()%2)
+                ApplyAuras(m_creature,SPELL_WHIRLWIND);
+            else
+                m_creature->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
+            break;
+        case EV_SPELL_42403_TICK:
+            if (m_creature->GetHealth() == m_creature->GetMaxHealth())
+            {
+                MovePhase(false);
+
+                SELF_CAST(SPELL_COMMAND_RETURN_HEAD, true);
+                CancelEvent(EV_SPELL_42403_TICK);
+            }break;
+        case EV_INTRO_TIMER:
+            SetPhase(PH_ONE);
+            SetState(STATE_COMBAT, true);
+            CancelEvent(EV_INTRO_TIMER);
+            break;
+        }
+
+    }
+
+    void UpdateAI(const uint32 diff)
+    {
+        if (HasState(STATE_COMBAT))
+            ScriptedAI::UpdateAI(diff);
+
+        EventsPool::Update(diff);
+    }
+};
*/

struct MANGOS_DLL_DECL boss_headless_horsemanAI : public ScriptedAI
{
    boss_headless_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

    void Reset()
    {
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_headless_horseman(Creature* pCreature)
{
    return new boss_headless_horsemanAI(pCreature);
}

void AddSC_boss_headless_horseman()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_headless_horseman";
    NewScript->GetAI = GetAI_boss_headless_horseman;
    NewScript->RegisterSelf();
}
