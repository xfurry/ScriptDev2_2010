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
SDName: Boss_Skadi
SD%Complete: 20%
SDComment: starts at trigger 4991
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO                       = -1575019,
    SAY_DRAKEBREATH_1               = -1575020,
    SAY_DRAKEBREATH_2               = -1575021,
    SAY_DRAKEBREATH_3               = -1575022,
    SAY_DRAKE_HARPOON_1             = -1575023,
    SAY_DRAKE_HARPOON_2             = -1575024,
    SAY_KILL_1                      = -1575025,
    SAY_KILL_2                      = -1575026,
    SAY_KILL_3                      = -1575027,
    SAY_DEATH                       = -1575028,
    SAY_DRAKE_DEATH                 = -1575029,
    EMOTE_HARPOON_RANGE             = -1575030,

    SPELL_CRUSH                     = 50234,
    SPELL_CRUSH_H                   = 59330,

    SPELL_WHIRLWIND                 = 50228,
    SPELL_WHIRLWIND_H               = 59322,

    SPELL_POISONED_SPEAR            = 50255,
    SPELL_POISONED_SPEAR_H          = 59331,
    SPELL_POISONED                  = 50258,
    SPELL_POISONED_H                = 59334,

    // casted with base of creature 22515 (World Trigger), so we must make sure
    // to use the close one by the door leading further in to instance.
    SPELL_SUMMON_GAUNTLET_MOBS      = 48630,                // tick every 30 sec
    SPELL_SUMMON_GAUNTLET_MOBS_H    = 59275,                // tick every 25 sec

    SPELL_GAUNTLET_PERIODIC         = 47546,                // what is this? Unknown use/effect, but probably related

    SPELL_LAUNCH_HARPOON            = 48642,                // this spell hit drake to reduce HP (force triggered from 48641)
    ITEM_HARPOON                    = 37372,

    NPC_YMIRJAR_WARRIOR         = 26690,
    NPC_YMIRJAR_WITCH_DOCTOR    = 26691,
    NPC_YMIRJAR_HARPOONER       = 26692,

    ACHIEV_LODI_DODI                = 1873,
    ACHIEV_MY_GIRL_LIKES_TO_SKADI   = 2156,         // needs script support
};

uint64 goHarpoons[3] =
{
    GO_HARPOON1,
    GO_HARPOON2,
    GO_HARPOON3
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations SpawnLoc[]=
{
    {340.556f, -511.493f, 104.352f},
    {367.741f, -512.865f, 104.828f},
    {399.546f, -512.755f, 104.834f},
    {430.551f, -514.320f, 105.055f},
    {468.931f, -513.555f, 104.723f}
};

/*######
## boss_skadi
######*/

struct MANGOS_DLL_DECL boss_skadiAI : public ScriptedAI
{
    boss_skadiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    //Land Phase
    uint32 m_uiCrushTimer;
    uint32 m_uiPoisonedSpearTimer;
    uint32 m_uiWirlwhindTimer;
    bool m_bIsLandPhase;

    //Event Phase
    uint8  m_uiNextWaveCount;
    uint32 m_uiIsInHarpoonRangeTimer;
    uint32 m_uiNextWaveTimer;
    uint32 m_uiGraufBrathTimer;

    uint32 m_uiEventTimer;
    uint8 m_uiphase;
    uint8 m_uiSpawn_counter;
    uint32 m_uiWaypointId;
    uint32 m_uiMoveNext_Timer;
    uint32 m_uiPlayerCheck_Timer;

    void Reset()
    {
        //Land Phase
        m_uiCrushTimer              = urand(5000,10000);
        m_uiPoisonedSpearTimer      = urand(5000,10000);
        m_uiWirlwhindTimer          = urand(5000,10000);
        m_bIsLandPhase              = false;

        //Event Phase
        m_uiGraufBrathTimer         = 30000;
        m_uiNextWaveCount           = 0;
        m_uiIsInHarpoonRangeTimer   = urand(5000,10000);
        m_uiNextWaveTimer           = urand(5000,10000);
        m_uiphase                   = 0;
        m_uiSpawn_counter           = 0;
        m_uiWaypointId              = 0;
        m_uiMoveNext_Timer          = 25000;
        m_uiPlayerCheck_Timer       = 1000;

        m_uiEventTimer              = 0;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->Unmount();

        if(Creature* pGrauf = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_GRAUF))))
        {
            pGrauf->setFaction(14);
            pGrauf->SetVisibility(VISIBILITY_ON);
        }

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SKADI, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_uiphase < 2)
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

    void MoveInLineOfSight(Unit* pWho)
    {
    	if (!pWho)
    	    return;

        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(pWho) &&
        	!m_uiphase && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 20))
        {
            if(m_pInstance)
                m_pInstance->SetData(TYPE_SKADI, IN_PROGRESS);

            if(Creature* pGrauf = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_GRAUF))))
            {
                pGrauf->setFaction(35);
                pGrauf->SetVisibility(VISIBILITY_OFF);
            }

            DoScriptText(SAY_AGGRO, m_creature);

            m_creature->SetInCombatWithZone();
            m_creature->Mount(27043);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            if (m_creature->HasSplineFlag(SPLINEFLAG_WALKMODE))
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId, 340.259f, -510.541f, 120.869f);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        	m_uiphase = 1;
        }
	}

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SKADI, DONE);

        if(!m_bIsRegularMode && m_uiEventTimer <= 180000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_LODI_DODI);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE)
                return;

        if ((m_uiSpawn_counter >= 4 && m_bIsRegularMode) || (m_uiSpawn_counter >= 5 && !m_bIsRegularMode))
        {
            m_uiWaypointId = 200;
            m_uiMoveNext_Timer = 3000;
        }
        else
        {
            switch(uiPointId)
            {
                case 0:
                    SpawnMobs(m_uiSpawn_counter);
                    m_uiWaypointId = 1;
                    ++m_uiSpawn_counter;
                    m_uiMoveNext_Timer = 3000;
                    break;
                case 1:
                    SpawnMobs(m_uiSpawn_counter);
                    m_uiWaypointId = 0;
                    ++m_uiSpawn_counter;
                    m_uiMoveNext_Timer = 3000;
                    break;
            }
        }
    }

    void SpawnMobs(uint32 uiSpot)
    {
        uint8 maxSpawn = (m_bIsRegularMode ? 4 : 6);
        for(uint8 i = 0; i < maxSpawn; ++i)
        {
            Creature* pTemp;
            switch (rand()%3)
            {
                case 0: pTemp = m_creature->SummonCreature(NPC_YMIRJAR_WARRIOR, SpawnLoc[uiSpot].x+rand()%5, SpawnLoc[uiSpot].y+rand()%5, SpawnLoc[uiSpot].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000); break;
                case 1: pTemp = m_creature->SummonCreature(NPC_YMIRJAR_WITCH_DOCTOR, SpawnLoc[uiSpot].x+rand()%5, SpawnLoc[uiSpot].y+rand()%5, SpawnLoc[uiSpot].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000); break;
                case 2: pTemp = m_creature->SummonCreature(NPC_YMIRJAR_HARPOONER, SpawnLoc[uiSpot].x+rand()%5, SpawnLoc[uiSpot].y+rand()%5, SpawnLoc[uiSpot].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000); break;
            }
            if (pTemp)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTemp->AddThreat(pTarget, 0.0f);
                    pTemp->AI()->AttackStart(pTarget);
                }
            }
        }
        ++m_uiNextWaveCount;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Achievement counter
        if(m_uiphase > 0)
            m_uiEventTimer += uiDiff;

        if (m_uiphase == 0)
             return;

        // Flying & adds
        else if (m_uiphase == 1)
        {
            if (m_uiPlayerCheck_Timer < uiDiff)
            {
                Map *map = m_creature->GetMap();
                if (map->IsDungeon() && m_pInstance->GetData(TYPE_SKADI) == IN_PROGRESS)
                {
                    Map::PlayerList const &PlayerList = map->GetPlayers();
 
                    if (PlayerList.isEmpty())
                        return;

                    bool bIsAlive = false;
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (i->getSource()->isAlive() && i->getSource()->isTargetableForAttack())
                        	 bIsAlive = true;

                    if (!bIsAlive)
                    {
                        m_creature->GetMotionMaster()->Clear(false);
                        m_creature->StopMoving();
                        EnterEvadeMode();
                        return;
                    }
                }
                m_uiPlayerCheck_Timer = 1000;
            } else m_uiPlayerCheck_Timer -= uiDiff;

            if(m_uiGraufBrathTimer < uiDiff)
            {
                switch(urand(0, 2))
                {
                case 0: DoScriptText(SAY_DRAKEBREATH_1, m_creature); break;
                case 1: DoScriptText(SAY_DRAKEBREATH_2, m_creature); break;
                case 2: DoScriptText(SAY_DRAKEBREATH_3, m_creature); break;
                }
                //breath ID missing
                /*if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED_SPEAR : SPELL_POISONED_SPEAR_H, false);
                    pPlayer->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED : SPELL_POISONED_H, true);
                }*/
                //Spell brath id ?
                m_uiGraufBrathTimer = urand(10000,20000);
            }else m_uiGraufBrathTimer -= uiDiff;

            if (m_uiMoveNext_Timer < uiDiff)
            {
                switch(m_uiWaypointId)
                {
                    case 0: m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId, 340.259f, -510.541f, 120.869f); break;
                    case 1: m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId, 472.977f, -513.636f, 120.869f); break;
                    case 200:
                        m_creature->GetMotionMaster()->Clear();
                        m_creature->Unmount();
                        m_uiphase = 2;
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        DoScriptText(SAY_DRAKE_DEATH, m_creature);
                        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
                        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            AttackStart(pTarget);
                    break;
                }
                m_uiMoveNext_Timer = 25000; // prevent stuck
            } else m_uiMoveNext_Timer -= uiDiff;
        }
        // Land & attack
        else if (m_uiphase == 2)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        /*if(!m_bIsLandPhase)
        {
            if(m_uiNextWaveCount > 4)
            {
                m_bIsLandPhase = true;
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->Unmount();
                DoScriptText(SAY_DRAKE_DEATH, m_creature);
                if(Creature* pGrauf = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_GRAUF))))
                {
                    pGrauf->setFaction(35);
                    pGrauf->SetVisibility(VISIBILITY_OFF);
                }
                /* needs fixing!
                if(m_pInstance && m_pInstance->GetData(TYPE_HARPOONLUNCHER) != (m_bIsRegularMode ? DONE : SPECIAL))
                {
                    if(m_uiIsInHarpoonRangeTimer < uiDiff)
                    {
                        DoScriptText(EMOTE_HARPOON_RANGE, m_creature);

                        //only 1 from 3 harpoons  is  aloowe to use at one time
                        if (GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(goHarpoons[urand(0,2)])))
                            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

                        m_uiIsInHarpoonRangeTimer = urand(20000,30000);
                    }else m_uiIsInHarpoonRangeTimer -= uiDiff;
                }
                else
                {
                    DoScriptText(SAY_DRAKE_DEATH, m_creature);
                    m_bIsLandPhase = true;
                    m_creature->SetVisibility(VISIBILITY_ON);
                    if(m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_SKADI, IN_PROGRESS);
                        m_pInstance->SetData(TYPE_HARPOONLUNCHER, 0);
                    }
                }
            }

            if(m_uiNextWaveTimer < uiDiff)
            {
                SpawnMobs(m_uiNextWaveCount);
                m_uiNextWaveTimer = urand(20000,30000);
            }else m_uiNextWaveTimer -= uiDiff;
        }
        else
        {*/

            if(m_uiPoisonedSpearTimer < uiDiff)
            {
                if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED_SPEAR : SPELL_POISONED_SPEAR_H, false);
                    pPlayer->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_POISONED : SPELL_POISONED_H, true);
                }
                m_uiPoisonedSpearTimer = urand(5000,10000);
            }else m_uiPoisonedSpearTimer -= uiDiff;

            if(m_uiCrushTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CRUSH : SPELL_CRUSH_H, false);
                m_uiCrushTimer = urand(10000,15000);
            }else m_uiCrushTimer -= uiDiff;

            if(m_uiWirlwhindTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WHIRLWIND : SPELL_WHIRLWIND_H, false);
                m_uiWirlwhindTimer = urand(10000,20000);
            }else m_uiWirlwhindTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_skadi(Creature* pCreature)
{
    return new boss_skadiAI(pCreature);
}

bool AreaTrigger_at_skadi(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
        if (pInstance->GetData(TYPE_SKADI) == NOT_STARTED)
            pInstance->SetData(TYPE_SKADI, SPECIAL);
    }

    return false;
}

bool GOHello_go_skaldi_harpoonluncher(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if(pPlayer->HasItemCount(ITEM_HARPOON,1))
    {
        pInstance->SetData(TYPE_HARPOONLUNCHER, IN_PROGRESS);
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    }
    return false;
}

void AddSC_boss_skadi()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_skadi";
    newscript->GetAI = &GetAI_boss_skadi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "at_skadi";
    newscript->pAreaTrigger = &AreaTrigger_at_skadi;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_skaldi_harpoonluncher";
    newscript->pGOHello = &GOHello_go_skaldi_harpoonluncher;
    newscript->RegisterSelf();
}
