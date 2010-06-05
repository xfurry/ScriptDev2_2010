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
SDName: boss_xevozz
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

enum
{
    SAY_AGGRO                                 = -1608027,
    SAY_SLAY_1                                = -1608028,
    SAY_SLAY_2                                = -1608029,
    SAY_SLAY_3                                = -1608030,
    SAY_DEATH                                 = -1608031,
    SAY_SPAWN                                 = -1608032,
    SAY_CHARGED                               = -1608033,
    SAY_REPEAT_SUMMON_1                       = -1608034,
    SAY_REPEAT_SUMMON_2                       = -1608035,
    SAY_SUMMON_ENERGY                         = -1608036,

    SPELL_ARCANE_BARRAGE_VOLLEY               = 54202,
    SPELL_ARCANE_BARRAGE_VOLLEY_H             = 59483,
    SPELL_ARCANE_BUFFET                       = 54226,
    SPELL_ARCANE_BUFFET_H                     = 59485,
    SPELL_SUMMON_ETHEREAL_SPHERE_1            = 54102,
    SPELL_SUMMON_ETHEREAL_SPHERE_2            = 54137,
    SPELL_SUMMON_ETHEREAL_SPHERE_3            = 54138,

    NPC_ETHEREAL_SPHERE                       = 29271,
    //NPC_ETHEREAL_SPHERE2                      = 32582, // heroic only?
    SPELL_ARCANE_POWER                        = 54160,
    SPELL_ARCANE_POWER_H                      = 59474,
    SPELL_SUMMON_PLAYERS                      = 54164,
};

struct MANGOS_DLL_DECL boss_xevozzAI : public ScriptedAI
{
    boss_xevozzAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    instance_violet_hold* m_pInstance;

    bool m_bIsRegularMode;
    bool MovementStarted;

    uint32 m_uiSummonEtherealSphere_Timer;
    uint32 m_uiArcaneBarrageVolley_Timer;
    uint32 m_uiArcaneBuffet_Timer;

    void Reset()
    {
        m_uiSummonEtherealSphere_Timer = urand(10000, 12000);
        m_uiArcaneBarrageVolley_Timer = urand(20000, 22000);
        m_uiArcaneBuffet_Timer = m_uiSummonEtherealSphere_Timer + urand(5000, 6000);
        DespawnSphere();
        MovementStarted = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_XEVOZZ, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_XEVOZZ, IN_PROGRESS);
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

        if (m_pInstance->GetData(TYPE_XEVOZZ) != SPECIAL && m_pInstance->GetData(TYPE_XEVOZZ) != IN_PROGRESS)
            return;

        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void DespawnSphere()
    {
        std::list<Creature*> assistList;
        GetCreatureListWithEntryInGrid(assistList,m_creature, NPC_ETHEREAL_SPHERE ,150.0f);

        if (assistList.empty())
            return;

        for(std::list<Creature*>::iterator iter = assistList.begin(); iter != assistList.end(); ++iter)
            (*iter)->DealDamage((*iter), (*iter)->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->SetSpeedRate(MOVE_RUN, 0.5f);
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
        {
            pSummoned->AddThreat(pTarget);
            pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_XEVOZZ) == SPECIAL && !MovementStarted) 
        {
            m_creature->GetMotionMaster()->MovePoint(0, PortalLoc[8].x, PortalLoc[8].y, PortalLoc[8].z);
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            MovementStarted = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiArcaneBarrageVolley_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE_VOLLEY_H : SPELL_ARCANE_BARRAGE_VOLLEY);
            m_uiArcaneBarrageVolley_Timer = urand(20000, 22000);
        }
        else m_uiArcaneBarrageVolley_Timer -= uiDiff;

        if (m_uiArcaneBuffet_Timer)
            if (m_uiArcaneBuffet_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ARCANE_BUFFET_H : SPELL_ARCANE_BUFFET);
                m_uiArcaneBuffet_Timer = 0;
            }
            else m_uiArcaneBuffet_Timer -= uiDiff;

            if (m_uiSummonEtherealSphere_Timer < uiDiff)
            {
                DoScriptText(SAY_SUMMON_ENERGY, m_creature);
                DoCast(m_creature, SPELL_SUMMON_ETHEREAL_SPHERE_1);
                if (m_bIsRegularMode) // extra one for heroic
                    m_creature->SummonCreature(NPC_ETHEREAL_SPHERE, m_creature->GetPositionX()-5+rand()%10, m_creature->GetPositionY()-5+rand()%10, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 40000);

                m_uiSummonEtherealSphere_Timer = urand(45000, 47000);
                m_uiArcaneBuffet_Timer = urand(5000, 6000);
            }
            else m_uiSummonEtherealSphere_Timer -= uiDiff;

            DoMeleeAttackIfReady();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnSphere();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XEVOZZ, DONE);
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

struct MANGOS_DLL_DECL mob_ethereal_sphereAI : public ScriptedAI
{
    mob_ethereal_sphereAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSummonPlayers_Timer;
    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiSummonPlayers_Timer = urand(33000, 35000);
        m_uiRangeCheck_Timer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if (Creature* pXevozz = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_XEVOZZ))))
                {
                    float fDistance = m_creature->GetDistance2d(pXevozz);
                    if (fDistance <= 3)
                        DoCast(pXevozz, m_bIsRegularMode ? SPELL_ARCANE_POWER_H : SPELL_ARCANE_POWER);
                    else
                        DoCast(m_creature, 35845);
                }
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;

        if (m_uiSummonPlayers_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_SUMMON_PLAYERS); // not working right

            Map* pMap = m_creature->GetMap();
            if (pMap && pMap->IsDungeon())
            {
                Map::PlayerList const &PlayerList = pMap->GetPlayers();

                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (i->getSource()->isAlive())
                            DoTeleportPlayer(i->getSource(), m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), i->getSource()->GetOrientation());
            }

            m_uiSummonPlayers_Timer = urand(33000, 35000);
        }
        else m_uiSummonPlayers_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_xevozz(Creature* pCreature)
{
    return new boss_xevozzAI (pCreature);
}

CreatureAI* GetAI_mob_ethereal_sphere(Creature* pCreature)
{
    return new mob_ethereal_sphereAI (pCreature);
}

void AddSC_boss_xevozz()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_xevozz";
    newscript->GetAI = &GetAI_boss_xevozz;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ethereal_sphere";
    newscript->GetAI = &GetAI_mob_ethereal_sphere;
    newscript->RegisterSelf();
}
