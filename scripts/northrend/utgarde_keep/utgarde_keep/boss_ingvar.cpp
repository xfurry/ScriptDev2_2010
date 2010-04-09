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
SDName: Boss_Ingvar
SD%Complete: 35%
SDComment: TODO: correct timers. Create ressurection sequenze and phase 2.
SDCategory: Utgarde Keep
EndScriptData */

#include "precompiled.h"
#include "utgarde_keep.h"

enum
{
    SAY_AGGRO_FIRST             = -1574005,
    SAY_AGGRO_SECOND            = -1574006,
    SAY_DEATH_FIRST             = -1574007,
    SAY_DEATH_SECOND            = -1574008,
    SAY_KILL_FIRST              = -1574009,
    SAY_KILL_SECOND             = -1574010,
    EMOTE_ROAR                  = -1574022,

    NPC_ANNHYLDE                = 24068,
    NPC_THROW_TARGET            = 23996,                    //the target, casting spell and target of moving dummy
    NPC_THROW_DUMMY             = 23997,                    //the axe, moving to target

    //phase 1
    SPELL_CLEAVE                = 42724,

    SPELL_SMASH                 = 42669,
    SPELL_SMASH_H               = 59706,

    SPELL_ENRAGE                = 42705,
    SPELL_ENRAGE_H              = 59707,

    SPELL_STAGGERING_ROAR       = 42708,
    SPELL_STAGGERING_ROAR_H     = 59708,

    //phase 2
    SPELL_DARK_SMASH_H          = 42723,

    SPELL_DREADFUL_ROAR         = 42729,
    SPELL_DREADFUL_ROAR_H       = 59734,

    SPELL_WOE_STRIKE            = 42730,
    SPELL_WOE_STRIKE_H          = 59735,

    SPELL_SHADOW_AXE            = 42748,
    SPELL_SHADOW_AXE_PROC       = 42751,
    SPELL_SHADOW_AXE_PROC_H     = 59720,

    //ressurection sequenze
    SPELL_FEIGN_DEATH           = 42795,
    SPELL_TRANSFORM             = 42796,
    SPELL_SCOURGE_RES_SUMMON    = 42863,                    //summones a dummy target
    SPELL_SCOURGE_RES_HEAL      = 42704,                    //heals max HP
    SPELL_SCOURGE_RES_BUBBLE    = 42862,                    //black bubble
    SPELL_SCOURGE_RES_CHANNEL   = 42857,                    //the whirl from annhylde

    SPELL_SUMMON_BANSHEE                        = 42912,
    SPELL_SCOURG_RESURRECTION_EFFEKTSPAWN       = 42863, //Spawn resurrecteffekt around Ingvar

    ACHIEV_UK                      = 477,
    ACHIEV_UK_H                    = 489,
};

#define MODEL_INGVAR_UNDEAD                         26351
#define MODEL_INGVAR_HUMAN                          21953

/*######
## boss_ingvar
######*/

struct MANGOS_DLL_DECL boss_ingvarAI : public ScriptedAI
{
    boss_ingvarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsResurrected;
    bool event_inProgress;

    uint32 m_uiCleaveTimer;
    uint32 m_uiSmashTimer;
    uint32 m_uiStaggeringRoarTimer;
    uint32 m_uiEnrageTimer;
    uint32 SpawnResTimer;
    uint32 wait_Timer;

    void Reset()
    {
        m_bIsResurrected = false;
        event_inProgress = false;
        if(m_bIsResurrected) // Visual Hack
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_INGVAR_HUMAN);

        m_uiCleaveTimer = urand(5000, 7000);
        m_uiSmashTimer = urand(8000, 15000);
        m_uiStaggeringRoarTimer = urand(10000, 25000);
        m_uiEnrageTimer = 30000;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_INGVAR, NOT_STARTED);

        SpawnResTimer = 3000;
        wait_Timer = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(m_bIsResurrected ? SAY_AGGRO_SECOND : SAY_AGGRO_FIRST, m_creature);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if( damage >= m_creature->GetHealth() && !m_bIsResurrected)
        {
            //DoCast(m_creature,SPELL_FEIGN_DEATH,true);  // Dont work ???
            // visuel hack
            m_creature->SetHealth(0);
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->RemoveAllAuras();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovementExpired(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
            // visuel hack end
            
            event_inProgress = true;
            m_bIsResurrected = true;

            DoScriptText(SAY_DEATH_FIRST,m_creature);
        }

        if(event_inProgress)
        {
            damage = 0;
        }
    }

    void StartZombiePhase()
    {
        m_bIsResurrected = true;
        event_inProgress = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());

        DoScriptText(SAY_AGGRO_SECOND,m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH_SECOND, m_creature);

        if (m_pInstance && m_pInstance->GetData(TYPE_KELESETH) == DONE && m_pInstance->GetData(TYPE_DALRONN) == DONE)
        {
            m_pInstance->SetData(TYPE_INGVAR, DONE);

            // Complete achiev for Ingvar
            Map* pMap = m_creature->GetMap();
            AchievementEntry const *Achiev = GetAchievementStore()->LookupEntry(m_bIsRegularMode ? ACHIEV_UK : ACHIEV_UK_H);
            if(Achiev && pMap)
            {
                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                if (!lPlayers.isEmpty())
                {
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        if (Player* pPlayer = itr->getSource())
                            pPlayer->CompletedAchievement(Achiev);
                    }
                }
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(m_bIsResurrected ? SAY_KILL_SECOND : SAY_KILL_FIRST, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(event_inProgress)
        {
            if(SpawnResTimer)
                if(SpawnResTimer< uiDiff)
                {
                    DoCast(m_creature,SPELL_SUMMON_BANSHEE); // Summons direktly on caster position
                    //DoCast(m_creature,SPELL_SCOURG_RESURRECTION_EFFEKTSPAWN); // Dont needet ?
                    SpawnResTimer = 0;
                }else SpawnResTimer -= uiDiff;

            return;
        }

        // This is used for a spell queue ... the spells should not castet if one spell is already casting
        if(wait_Timer)
            if(wait_Timer < uiDiff)
            {
                wait_Timer = 0;
            }else wait_Timer -= uiDiff;

        if(m_uiCleaveTimer < uiDiff)
        {
            if(!wait_Timer)
            {
                if(m_bIsResurrected)
                    DoCast(m_creature->getVictim(), m_bIsResurrected ? SPELL_WOE_STRIKE : SPELL_WOE_STRIKE_H);
                else
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = urand(2500, 7000);
                wait_Timer = 1000;
            }
        }else m_uiCleaveTimer -= uiDiff;

        if(m_uiSmashTimer < uiDiff)
        {
            if(!wait_Timer)
            {
                if(m_bIsResurrected)
                    DoCast(m_creature->getVictim(), SPELL_DARK_SMASH_H);
                else
                    DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SMASH : SPELL_SMASH_H);
                m_uiSmashTimer = urand(8000, 15000);
                wait_Timer = 5000;
            }
        }else m_uiSmashTimer -= uiDiff;

        if(!m_bIsResurrected)
        {
            if(m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
                m_uiEnrageTimer = urand(10000, 20000);
            }
            else m_uiEnrageTimer -= uiDiff;
        }
        // In Undead form used to summon weapon
        else 
        {
            if(m_uiEnrageTimer < uiDiff)
            {
                if(!wait_Timer)
                {
                    // Spawn target for Axe
                    Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);
                    if(target)
                    {
                        Creature* temp = m_creature->SummonCreature(NPC_THROW_TARGET,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,2000);
                        DoCast(m_creature,SPELL_SHADOW_AXE);
                    }
                    m_uiEnrageTimer = 30000;
                }
            }else m_uiEnrageTimer -= uiDiff;
        }


        if(m_uiStaggeringRoarTimer < uiDiff)
        {
            if(!wait_Timer)
            {
                if(m_bIsResurrected)
                    DoCast(m_creature,m_bIsRegularMode ? SPELL_DREADFUL_ROAR : SPELL_DREADFUL_ROAR_H);
                else
                {
                    DoScriptText(EMOTE_ROAR, m_creature);
                    DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STAGGERING_ROAR : SPELL_STAGGERING_ROAR_H);
                }
                m_uiStaggeringRoarTimer = urand(15000, 30000);
                wait_Timer = 5000;
            }
        }else m_uiStaggeringRoarTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ingvar(Creature* pCreature)
{
    return new boss_ingvarAI(pCreature);
}

/*######
## npc_annhylde
######*/
#define SAY_RESSURECT                       -1999524

//Spells for Annhylde
#define SPELL_SCOURG_RESURRECTION_HEAL              42704 //Heal Max + DummyAura
#define SPELL_SCOURG_RESURRECTION_BEAM              42857 //Channeling Beam of Annhylde
#define SPELL_SCOURG_RESURRECTION_DUMMY             42862 //Some Emote Dummy?
#define SPELL_INGVAR_TRANSFORM                      42796

struct MANGOS_DLL_DECL npc_annhyldeAI : public ScriptedAI
{
    npc_annhyldeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    float x,y,z;
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Resurect_Timer;
    uint32 Resurect_Phase;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
        m_creature->SetSpeedRate(MOVE_FLIGHT , 1.0f);

        m_creature->GetPosition(x,y,z);
        DoTeleportPlayer(m_creature, x+1, y, z+30, m_creature->GetOrientation());
        Unit* ingvar = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_INGVAR));
        if(ingvar)
        {
            m_creature->GetMotionMaster()->MovePoint(1,x,y,z+15);

            DoScriptText(SAY_RESSURECT,m_creature);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type != POINT_MOTION_TYPE)
            return;
        Unit* ingvar = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_INGVAR));
        if(ingvar)
        {
            switch (id)
            {
            case 1:
                ingvar->RemoveAura(SPELL_SUMMON_BANSHEE,EFFECT_INDEX_1);
                ingvar->CastSpell(ingvar,SPELL_SCOURG_RESURRECTION_DUMMY,true);
                DoCast(ingvar,SPELL_SCOURG_RESURRECTION_BEAM);
                Resurect_Timer = 8000;
                Resurect_Phase = 1;
                break;
            case 2:
                m_creature->DealDamage(m_creature,m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                m_creature->RemoveCorpse();
                break;
            }
        }
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void Aggro(Unit *who) {}
    void UpdateAI(const uint32 diff)
    {
        //if(Resurect_Timer)
            if(Resurect_Timer < diff)
            {
                if(Resurect_Phase == 1)
                {
                    Unit* ingvar = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_INGVAR));
                    if(ingvar)
                    {
                        ingvar->SetStandState(UNIT_STAND_STATE_STAND);
                        ingvar->CastSpell(ingvar,SPELL_SCOURG_RESURRECTION_HEAL,false);
                        ingvar->SetHealth(ingvar->GetMaxHealth());
                    }
                    Resurect_Timer = 3000;
                    Resurect_Phase = 2;
                }else if (Resurect_Phase == 2)
                {
                    Unit* ingvar = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_INGVAR));
                    if(ingvar)
                    {
                        ingvar->RemoveAurasDueToSpell(SPELL_SCOURG_RESURRECTION_DUMMY);
                        ingvar->CastSpell(ingvar,SPELL_INGVAR_TRANSFORM,false);
                        ingvar->SetUInt32Value(UNIT_FIELD_DISPLAYID, MODEL_INGVAR_UNDEAD); // Visual Hack - when he dies he becomes human model -> wrong
                        Creature* c_ingvar = (Creature*)ingvar;

                        ((boss_ingvarAI*)(c_ingvar->AI()))->StartZombiePhase();

                        m_creature->GetMotionMaster()->MovePoint(2,x+1,y,z+30);
                        Resurect_Phase++;
                    }
                }
                
            }else Resurect_Timer -= diff;
    }
};

CreatureAI* GetAI_npc_annhylde(Creature* pCreature)
{
    return new npc_annhyldeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ingvar_throw_dummyAI : public ScriptedAI
{
    mob_ingvar_throw_dummyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;

    void Reset()
    {
        Unit *pTarget = GetClosestCreatureWithEntry(m_creature, NPC_THROW_TARGET,50);
        if (pTarget)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_AXE_PROC : SPELL_SHADOW_AXE_PROC_H);
            float x,y,z;
            pTarget->GetPosition(x,y,z);
            m_creature->GetMotionMaster()->MovePoint(0,x,y,z);
        }
        Despawn_Timer = 7000;
    }
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void EnterCombat(Unit *who) {}
    void UpdateAI(const uint32 diff)
    {
        if (Despawn_Timer <= diff)
        {
            m_creature->DealDamage(m_creature,m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_creature->RemoveCorpse();
            Despawn_Timer = 0;
        } else Despawn_Timer -= diff;
    }
};

CreatureAI* GetAI_mob_ingvar_throw_dummy(Creature* pCreature)
{
    return new mob_ingvar_throw_dummyAI (pCreature);
}

void AddSC_boss_ingvar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ingvar";
    newscript->GetAI = &GetAI_boss_ingvar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_annhylde";
    newscript->GetAI = &GetAI_npc_annhylde;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ingvar_throw_dummy";
    newscript->GetAI = &GetAI_mob_ingvar_throw_dummy;
    newscript->RegisterSelf();
}
