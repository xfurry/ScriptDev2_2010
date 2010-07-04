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
SDName: boss_marwyn
SD%Complete: 30%
SDComment:
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"

enum
{
    SAY_INTRO1              = -1610109,
    SAY_INTRO2              = -1610110,
    SAY_AGGRO               = -1610111,
    SAY_CORRUPTED_FLESH1    = -1610112,
    SAY_CORRUPTED_FLESH2    = -1610113,
    SAY_SLAY1               = -1610114,
    SAY_SLAY2               = -1610115,
    SAY_DEATH               = -1610116,

    SPELL_BERSERK                           = 47008,
    SPELL_OBLITERATE                        = 72360,
    SPELL_OBLITERATE_H                      = 72434,
    SPELL_SHARED_SUFFERING                  = 72368,
    SPELL_SHARED_SUFFERING_H                = 72369,
    SPELL_WELL_OF_CORRUPTION                = 72362,
    SPELL_CORRUPTED_FLESH                   = 72363,
    SPELL_CORRUPTED_FLESH_H                 = 72436,

    EQUIP_ID                                = 50672,
};

struct MANGOS_DLL_DECL boss_marwynAI : public ScriptedAI
{
    boss_marwynAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetVisibility(VISIBILITY_OFF);
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBerserk_Timer;
    uint32 m_uiSharedSuffering_Timer;
    uint32 m_uiWell_Timer;
    uint32 m_uiTouch_Timer;
    uint32 m_uiFlesh_Timer;
    uint32 m_uiObliterate_Timer;
    uint32 m_uiSummon_Timer;
    uint32 m_uiExploitCheckTimer;

    uint8 m_uiSummonCount;
    bool m_bHasIntro;

    uint32 m_uiSoldierEntry[4];
    std::list<Creature*> lSoldiers[4];

    void Reset()
    {
        memset(&m_uiSoldierEntry, 0, 4);
        memset(&lSoldiers, 0, 4);
        m_uiExploitCheckTimer       = 1000;
        m_uiBerserk_Timer           = 180000;
        m_uiSharedSuffering_Timer   = 4000;
        m_uiWell_Timer              = 5000;
        m_uiTouch_Timer             = 8000;
        m_uiFlesh_Timer             = 10000;
        m_uiObliterate_Timer        = 1000;
        m_uiSummonCount             = 0;
        m_uiSummon_Timer            = 60000;
        m_bHasIntro                 = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void JustReachedHome()
    {
        if (m_pInstance) 
        {
            m_pInstance->SetData(TYPE_MARWYN, NOT_STARTED);
            m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES, 0);
        }

        ResetEvent();
    }

    void ResetEvent()
    {
        uint32 TeamInInstance = 0;

        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    TeamInInstance = pPlayer->GetTeam();
            }
        }
        if(TeamInInstance == ALLIANCE)
        {
            if(Creature *pJaina = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_JAINA_INTRO))))
            {
                pJaina->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pJaina->SetVisibility(VISIBILITY_ON);
                pJaina->GetMap()->CreatureRelocation(pJaina, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pJaina->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pJaina->GetSplineFlags(), 1);
            }
        }
        else if(TeamInInstance == HORDE) 
        {
            if(Creature *pSylvanas = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SYLVANAS_INTRO))))
            {
                pSylvanas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pSylvanas->SetVisibility(VISIBILITY_ON);
                pSylvanas->GetMap()->CreatureRelocation(pSylvanas, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pSylvanas->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pSylvanas->GetSplineFlags(), 1);
            }
        }
    }

    bool CallGuards(TempSummonType type, uint32 ui_summontime )
    {
        switch(urand(0,3))
        {
        case 0:
            m_uiSoldierEntry[0] = MOB_SHADOWY_MERCENARY;
            m_uiSoldierEntry[1] = MOB_SPECTRAL_FOOTMAN;
            m_uiSoldierEntry[2] = MOB_GHOSTLY_PRIEST;
            m_uiSoldierEntry[3] = MOB_PHANTOM_HALLUCINATION;
            break;
        case 1:
            m_uiSoldierEntry[0] = MOB_TORTURED_RIFLEMAN;
            m_uiSoldierEntry[1] = MOB_SPECTRAL_FOOTMAN;
            m_uiSoldierEntry[2] = MOB_PHANTOM_MAGE;
            m_uiSoldierEntry[3] = MOB_GHOSTLY_PRIEST;
            break;
        case 2:
            m_uiSoldierEntry[0] = MOB_TORTURED_RIFLEMAN;
            m_uiSoldierEntry[1] = MOB_PHANTOM_HALLUCINATION;
            m_uiSoldierEntry[2] = MOB_GHOSTLY_PRIEST;
            m_uiSoldierEntry[3] = MOB_SHADOWY_MERCENARY;
            break;
        case 3:
            m_uiSoldierEntry[0] = MOB_SHADOWY_MERCENARY;
            m_uiSoldierEntry[1] = MOB_PHANTOM_HALLUCINATION;
            m_uiSoldierEntry[2] = MOB_PHANTOM_MAGE;
            m_uiSoldierEntry[3] = MOB_SPECTRAL_FOOTMAN;
            break;
        }

        // get soldiers
        for (uint8 i = 0; i < 4; ++i)
        {
            /*GetCreatureListWithEntryInGrid(lSoldiers[i], m_creature, m_uiSoldierEntry[i], DEFAULT_VISIBILITY_INSTANCE);
            if (!lSoldiers[i].empty())
            {
                for(std::list<Creature*>::iterator iter = lSoldiers[i].begin(); iter != lSoldiers[i].end(); ++iter)
                {
                    //if ((*iter) && (*iter)->isAlive())
                        //(*iter)->Respawn();
                }
            }*/

            // temp
            if (Creature* pSummon = m_creature->SummonCreature(m_uiSoldierEntry[i], SpawnLoc[i].x, SpawnLoc[i].y, SpawnLoc[i].z, 0, type, ui_summontime))
                pSummon->SetInCombatWithZone();
        }
        
        return true;
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_MARWYN) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_MARWYN, DONE);
            m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES, 0);
        }
    }

    bool IsPlayerInside()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return false;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(GameObject* pAltar = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ALTAR)))
                {
                    if (i->getSource()->isAlive() && i->getSource()->GetDistance2d(pAltar) < 75.0f)
                        return true;
                }
            }
        } 

        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_MARWYN) == SPECIAL ) 
        {
            if(m_uiExploitCheckTimer < uiDiff)
            {
                if(!IsPlayerInside())
                    EnterEvadeMode();
                m_uiExploitCheckTimer = 1000;
            }
            else m_uiExploitCheckTimer -= uiDiff;

            if (m_uiSummon_Timer < uiDiff) 
            {
                if(!m_bHasIntro)
                {
                    if(m_pInstance)
                        m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES, 1);
                    DoScriptText(SAY_INTRO1, m_creature);
                    m_bHasIntro = true;
                }

                ++m_uiSummonCount;
                if (m_uiSummonCount > MOB_WAVES_NUM) 
                {
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetInCombatWithZone();
                }
                else 
                {
                    if(m_pInstance)
                        m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES_COUNT, m_uiSummonCount + 5);
                    CallGuards(TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                }
                m_uiSummon_Timer = MOB_WAVES_DELAY;
            } 
            else 
                m_uiSummon_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance && m_pInstance->GetData(TYPE_MARWYN) == IN_PROGRESS)
                EnterEvadeMode();
            return;
        }

        if (m_uiSharedSuffering_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHARED_SUFFERING : SPELL_SHARED_SUFFERING_H);
            m_uiSharedSuffering_Timer= 20000;
        } 
        else 
            m_uiSharedSuffering_Timer -= uiDiff;

        if (m_uiWell_Timer < uiDiff) 
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WELL_OF_CORRUPTION);
            m_uiWell_Timer= 30000;
        } 
        else 
            m_uiWell_Timer -= uiDiff;

        if (m_uiFlesh_Timer < uiDiff) 
        {
            switch(urand(0, 1))
            {
            case 0: DoScriptText(SAY_CORRUPTED_FLESH1, m_creature); break;
            case 1: DoScriptText(SAY_CORRUPTED_FLESH2, m_creature); break;
            }
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CORRUPTED_FLESH : SPELL_CORRUPTED_FLESH_H);
            m_uiFlesh_Timer= 10000;
        } 
        else 
            m_uiFlesh_Timer -= uiDiff;

        if (m_uiObliterate_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OBLITERATE : SPELL_OBLITERATE_H);
            m_uiObliterate_Timer=urand(8000,12000);
        } 
        else 
            m_uiObliterate_Timer -= uiDiff;

        if (m_uiBerserk_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 180000;
        } 
        else  
            m_uiBerserk_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_marwyn(Creature* pCreature)
{
    return new boss_marwynAI(pCreature);
}

void AddSC_boss_marwyn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_marwyn";
    newscript->GetAI = &GetAI_boss_marwyn;
    newscript->RegisterSelf();
}
