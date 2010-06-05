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
SDName: boss_ichoron
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"


enum
{
    SAY_AGGRO                                 = -1608018,
    SAY_SLAY_1                                = -1608019,
    SAY_SLAY_2                                = -1608020,
    SAY_SLAY_3                                = -1608021,
    SAY_DEATH                                 = -1608022,
    SAY_SPAWN                                 = -1608023,
    SAY_ENRAGE                                = -1608024,
    SAY_SHATTER                               = -1608025,
    SAY_BUBBLE                                = -1608026,

    SPELL_DRAINED                             = 59820,
    SPELL_FRENZY                              = 54312,
    SPELL_FRENZY_H                            = 59522,
    SPELL_PROTECTIVE_BUBBLE                   = 54306,
    SPELL_WATER_BLAST                         = 54237,
    SPELL_WATER_BLAST_H                       = 59520,
    SPELL_WATER_BOLT_VOLLEY                   = 54241,
    SPELL_WATER_BOLT_VOLLEY_H                 = 59521,

    NPC_ICHOR_GLOBULE                         = 29321,
    SPELL_SPLASH                              = 59516,

    ACHIEV_DEHYDRATATION                        = 2041,     // needs advanced script support
};

struct MANGOS_DLL_DECL boss_ichoronAI : public ScriptedAI
{
    boss_ichoronAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    instance_violet_hold* m_pInstance;
    std::list<uint64> m_lWaterElementsGUIDList;

    bool m_bIsRegularMode;
    bool m_bIsExploded;
    bool m_bIsFrenzy;
    bool MovementStarted;
    bool m_bDehydratation;

    uint32 m_uiBuubleChecker_Timer;
    uint32 m_uiWaterBoltVolley_Timer;
    uint32 m_uiShowup_Counter;

    void Reset()
    {
        m_bIsExploded = false;
        m_bIsFrenzy = false;
        MovementStarted = false;
        m_uiBuubleChecker_Timer = 1000;
        m_uiWaterBoltVolley_Timer = urand(10000, 15000);
        m_uiShowup_Counter = 0;
        m_bDehydratation = true;

        m_creature->SetVisibility(VISIBILITY_ON);
        DespawnWaterElements();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ICHORON, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ICHORON, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if(Creature* pSinclari = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_SINCLARI)))
            pSinclari->DealDamage(pSinclari, pSinclari->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAIN, FAIL);
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_ICHORON) != SPECIAL && m_pInstance->GetData(TYPE_ICHORON) != IN_PROGRESS)
            return;

        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->Attack(pWho, true))
        {
            DoCast(m_creature, SPELL_PROTECTIVE_BUBBLE);
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void WaterElementHit()
    {
        m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.01);
        if (m_bIsExploded)
        {
            DoCast(m_creature, SPELL_PROTECTIVE_BUBBLE);
            m_bIsExploded = false;
            m_bDehydratation = false;
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->SetSpeedRate(MOVE_RUN, 0.2f);
        pSummoned->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
        m_lWaterElementsGUIDList.push_back(pSummoned->GetGUID());
    }

    void DespawnWaterElements()
    {
        if (m_lWaterElementsGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lWaterElementsGUIDList.begin(); itr != m_lWaterElementsGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }

        m_lWaterElementsGUIDList.clear();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_ICHORON) == SPECIAL && !MovementStarted) 
        {
            m_creature->GetMotionMaster()->MovePoint(0, PortalLoc[8].x, PortalLoc[8].y, PortalLoc[8].z);
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MovementStarted = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsFrenzy)
        {
            if (m_uiBuubleChecker_Timer < uiDiff)
            {
                if (!m_bIsExploded)
                {
                    if (!m_creature->HasAura(SPELL_PROTECTIVE_BUBBLE, EFFECT_INDEX_0))
                    {
                        DoCast(m_creature, m_bIsRegularMode ? SPELL_WATER_BLAST_H : SPELL_WATER_BLAST);
                        DoCast(m_creature, SPELL_DRAINED);
                        m_bIsExploded = true;
                        m_uiShowup_Counter = 0;
                        DoCast(m_creature, SPELL_PROTECTIVE_BUBBLE);
                        m_creature->AttackStop();
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        for(uint8 i = 0; i < 10; i++)
                        {
                            int tmp = urand(1, 8);
                            m_creature->SummonCreature(NPC_ICHOR_GLOBULE, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                            //m_creature->SummonCreature(NPC_ICHOR_GLOBULE, m_creature->GetPositionX()-10+rand()%20, m_creature->GetPositionY()-10+rand()%20, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        }
                    }
                    m_uiBuubleChecker_Timer = 3000;
                }
                else
                {
                    bool bIsWaterElementsAlive = false;
                    ++m_uiShowup_Counter;
                    if (!m_lWaterElementsGUIDList.empty())
                    {
                        for(std::list<uint64>::iterator itr = m_lWaterElementsGUIDList.begin(); itr != m_lWaterElementsGUIDList.end(); ++itr)
                            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                                if (pTemp->isAlive())
                                    bIsWaterElementsAlive = true;
                    }
                    if (!bIsWaterElementsAlive || m_uiShowup_Counter > 20)
                    {
                        m_bIsExploded = false;
                        m_uiShowup_Counter = 0;
                        m_creature->SetVisibility(VISIBILITY_ON);
                        m_creature->RemoveAurasDueToSpell(SPELL_DRAINED);
                        m_creature->SetInCombatWithZone();
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    }
                    m_uiBuubleChecker_Timer = 1000;
                }
            }
            else m_uiBuubleChecker_Timer -= uiDiff;
        }

        if (!m_bIsExploded)
        {
            if (m_uiWaterBoltVolley_Timer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_WATER_BOLT_VOLLEY_H : SPELL_WATER_BOLT_VOLLEY);
                m_uiWaterBoltVolley_Timer = urand(10000, 15000);
            }
            else m_uiWaterBoltVolley_Timer -= uiDiff;

            if (!m_bIsFrenzy && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25)
            {
                DoScriptText(SAY_ENRAGE, m_creature);
                DoCast(m_creature, m_bIsRegularMode ? SPELL_FRENZY_H : SPELL_FRENZY);
                m_bIsFrenzy = true;
            }
        }

        if(!m_creature->HasAura(SPELL_DRAINED, EFFECT_INDEX_0))
            DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnWaterElements();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ICHORON, DONE);
            m_pInstance->SetData(TYPE_PORTAL, DONE);

            // check if boss was already killed before
            uint32 m_uiMyPortalNumber = m_pInstance->GetCurrentPortalNumber();
            if(m_uiMyPortalNumber == 6)
            {
                if(m_pInstance->GetData(TYPE_FIRST_BOSS) == DONE)
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                else
                    m_pInstance->SetData(TYPE_FIRST_BOSS, DONE);
            }
            else if(m_uiMyPortalNumber == 12)
            {
                if(m_pInstance->GetData(TYPE_SECOND_BOSS) == DONE)
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                else
                    m_pInstance->SetData(TYPE_SECOND_BOSS, DONE);
            }
        }

        if(!m_bIsRegularMode && m_bDehydratation)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_DEHYDRATATION);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
        case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
        case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
        case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};


struct MANGOS_DLL_DECL mob_ichor_globuleAI : public ScriptedAI
{
    mob_ichor_globuleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if (Creature* pIchoron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_ICHORON))))
                {
                    float fDistance = m_creature->GetDistance2d(pIchoron);
                    if (fDistance <= 2)
                    {
                        ((boss_ichoronAI*)pIchoron->AI())->WaterElementHit();
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

    void JustDied(Unit* pKiller)
    {
        DoCast(m_creature, SPELL_SPLASH);
    }
};

CreatureAI* GetAI_boss_ichoron(Creature* pCreature)
{
    return new boss_ichoronAI (pCreature);
}

CreatureAI* GetAI_mob_ichor_globule(Creature* pCreature)
{
    return new mob_ichor_globuleAI (pCreature);
}

void AddSC_boss_ichoron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ichoron";
    newscript->GetAI = &GetAI_boss_ichoron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ichor_globule";
    newscript->GetAI = &GetAI_mob_ichor_globule;
    newscript->RegisterSelf();
}
