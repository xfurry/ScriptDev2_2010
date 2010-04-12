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
SDName: boss_falryn
SD%Complete: 40%
SDComment:
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"

enum
{
    SAY_INTRO1              = -1610101,
    SAY_INTRO2              = -1610102,
    SAY_AGGRO               = -1610103,
    SAY_IMPENDING_DESPAIR   = -1610104,
    SAY_DEFILING_HORROR     = -1610105,
    SAY_SLAY1               = -1610106,
    SAY_SLAY2               = -1610107,
    SAY_DEATH               = -1610108,

    //common
    SPELL_BERSERK                           = 47008,
    SPELL_HOPELESSNESS                      = 72395,
    SPELL_IMPENDING_DESPAIR                 = 72426,
    SPELL_DEFILING_HORROR                   = 72435,
    SPELL_DEFILING_HORROR_H                 = 72452,
    SPELL_QUIVERING_STRIKE                  = 72422,
    SPELL_QUIVERING_STRIKE_H                = 72453,

    EQUIP_ID                                = 50704,
};

struct MANGOS_DLL_DECL boss_falricAI : public ScriptedAI
{
    boss_falricAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->setFaction(35);
        pCreature->SetVisibility(VISIBILITY_OFF);
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 m_uiBerserk_Timer;
    uint32 m_uiDespair_Timer;
    uint32 m_uiHorror_Timer;
    uint32 m_uiStrike_Timer;
    uint32 m_uiSummon_Timer;

    bool m_bIsPhase1;
    bool m_bIsPhase2;
    bool m_bIsPhase3;

    uint8 SummonCount;
    bool hasIntro;

    uint64 npctype1;
    uint64 npctype2;
    uint64 npctype3;
    uint64 npctype4;

    void Reset()
    {
        m_uiBerserk_Timer = 180000;
        SummonCount = 0;

        m_bIsPhase1     = false;
        m_bIsPhase2     = false;
        m_bIsPhase3     = false;
        hasIntro        = false;
        
        m_uiDespair_Timer = m_bIsRegularMode ? 40000 : 30000;
        m_uiHorror_Timer = urand(25000,35000);
        m_uiStrike_Timer = urand(10000,15000);
        m_uiSummon_Timer = 1000;

        if (m_pInstance) 
            m_pInstance->SetData(TYPE_FALRIC, NOT_STARTED);

        ResetEvent();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
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

    bool CallGuards(TempSummonType type, uint32 _summontime )
    {
        //((mob_hallsOfReflectionSoulAI*)m_creature->AI())->ChooseForAttack();
        
        switch(urand(0,3))
        {
        case 0: {
            npctype1 = MOB_SHADOWY_MERCENARY;
            npctype2 = MOB_SPECTRAL_FOOTMAN;
            npctype3 = MOB_GHOSTLY_PRIEST;
            npctype4 = MOB_PHANTOM_HALLUCINATION;
            break;}
        case 1: {
            npctype1 = MOB_TORTURED_RIFLEMAN;
            npctype2 = MOB_SPECTRAL_FOOTMAN;
            npctype3 = MOB_PHANTOM_MAGE;
            npctype4 = MOB_GHOSTLY_PRIEST;
            break;}
        case 2: {
            npctype1 = MOB_TORTURED_RIFLEMAN;
            npctype2 = MOB_PHANTOM_HALLUCINATION;
            npctype3 = MOB_GHOSTLY_PRIEST;
            npctype4 = MOB_SHADOWY_MERCENARY;
            break;}
        case 3: {
            npctype1 = MOB_SHADOWY_MERCENARY;
            npctype2 = MOB_PHANTOM_HALLUCINATION;
            npctype3 = MOB_PHANTOM_MAGE;
            npctype4 = MOB_SPECTRAL_FOOTMAN;
            break;}
        }
        if (Creature* pSummon1 = m_creature->SummonCreature(npctype1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, type, _summontime))
            pSummon1->SetInCombatWithZone();
        if (Creature* pSummon2 = m_creature->SummonCreature(npctype2, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, 0, type, _summontime))
            pSummon2->SetInCombatWithZone();
        if (Creature* pSummon3 = m_creature->SummonCreature(npctype3, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 0, type, _summontime))
            pSummon3->SetInCombatWithZone();
        if (Creature* pSummon4 = m_creature->SummonCreature(npctype4, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 0, type, _summontime))
            pSummon4->SetInCombatWithZone();
        
        return true;
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);  
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_FALRIC) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FALRIC, DONE);
            m_pInstance->SetData(TYPE_MARWYN, SPECIAL);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_INTRO) == DONE && m_pInstance->GetData(TYPE_FALRIC) == SPECIAL)
        {
            if (m_uiSummon_Timer < uiDiff) 
            {
                if(!hasIntro)
                {
                    //if(GameObject* pEnterDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ICECROWN_DOOR)))
                            //m_pInstance->DoUseDoorOrButton(pEnterDoor->GetGUID());
                    DoScriptText(SAY_INTRO1, m_creature);
                    hasIntro = true;
                }

                ++SummonCount;
                if (SummonCount > MOB_WAVES_NUM_1)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetInCombatWithZone();
                    if(m_pInstance) 
                        m_pInstance->SetData(TYPE_FALRIC, IN_PROGRESS);
                }
                else 
                    CallGuards(TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);
                m_uiSummon_Timer = MOB_WAVES_DELAY_1;
            } 
            else 
                m_uiSummon_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance && m_pInstance->GetData(TYPE_FALRIC) == IN_PROGRESS)
                Reset();
            return;
        }

        if (m_uiDespair_Timer < uiDiff) 
        {
            DoScriptText(SAY_IMPENDING_DESPAIR, m_creature);
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_IMPENDING_DESPAIR);
            m_uiDespair_Timer= m_bIsRegularMode ? 40000 : 30000;
        } 
        else 
            m_uiDespair_Timer -= uiDiff;

        if (m_uiStrike_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_QUIVERING_STRIKE : SPELL_QUIVERING_STRIKE_H);
            m_uiStrike_Timer=urand(10000,15000);
        } 
        else 
            m_uiStrike_Timer -= uiDiff;

        if (m_uiHorror_Timer < uiDiff) 
        {
            DoScriptText(SAY_DEFILING_HORROR, m_creature);
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DEFILING_HORROR : SPELL_DEFILING_HORROR_H);
            m_uiHorror_Timer=urand(25000,35000);
        } 
        else 
            m_uiHorror_Timer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 66.0f && !m_bIsPhase1)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_HOPELESSNESS);
            m_bIsPhase1 = true;
        }

        if(m_creature->GetHealthPercent() <= 33.0f && !m_bIsPhase2)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_HOPELESSNESS);
            m_bIsPhase2 = true;
        }

        if(m_creature->GetHealthPercent() <= 10.0f && !m_bIsPhase3)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_HOPELESSNESS);
            m_bIsPhase3 = true;
        }

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

CreatureAI* GetAI_boss_falric(Creature* pCreature)
{
    return new boss_falricAI(pCreature);
}

void AddSC_boss_falric()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_falric";
    newscript->GetAI = &GetAI_boss_falric;
    newscript->RegisterSelf();
}
