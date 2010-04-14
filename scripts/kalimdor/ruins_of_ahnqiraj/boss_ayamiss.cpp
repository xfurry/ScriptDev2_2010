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
SDName: Boss_Ayamiss
SD%Complete: 50
SDComment: VERIFY SCRIPT
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

enum
{
     SPELL_STINGERSPRAY          =  25749,
     SPELL_POISONSTINGER         =  25748,                          //only used in phase1
     SPELL_PARALYZE              =  25725,
     SPELL_TRASH                 =  3391,
     SPELL_FRENZY                =  8269,
     SPELL_LASH                  =  25852,
 
     EMOTE_FRENZY                =  -1000002,
 
     NPC_HIVEZARA_LARVA          =  15555,

     SPELL_FEED                  =  25721,

     NPC_HIVEZARA_HORNET         =  15934,
     NPC_HIVEZARA_SWARMER        =  15546
};

struct SpawnLocations
{
    float x, y, z;
};

static SpawnLocations Larva[]=
{
    {-9695,1585,25},
    {-9627,1538,21.44f}
};

static SpawnLocations Swarmers[]=
{
    {-9650,1577,47}
};

struct MANGOS_DLL_DECL boss_ayamissAI : public ScriptedAI
{
    boss_ayamissAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    Unit *pTarget;
    Creature *pLarva;
 
    uint32 m_uiStingerSpray_Timer;
    uint32 m_uiPoisonStinger_Timer;
    uint32 m_uiSummonSwarmer_Timer;
    uint32 m_uiSummonPlayer_Timer;
    uint32 m_uiTrash_Timer;
    uint32 m_uiRelocate_Timer;
    uint32 m_uiLash_Timer;

    uint64 m_uiLarvaTargetGUID;
   
    bool m_bIsInPhaseTwo;
    bool m_bIsEnraged;
    bool m_bRelocated;

    void Reset()
    {
        pTarget = NULL;
        pLarva = NULL;

        m_uiStingerSpray_Timer = 10000;
        m_uiPoisonStinger_Timer = 5000;
        m_uiSummonSwarmer_Timer = 60000;
        m_uiSummonPlayer_Timer = 10000;
        m_uiTrash_Timer = 10000;
        m_uiRelocate_Timer = 5000;
        m_uiLash_Timer = 15000;

        m_uiLarvaTargetGUID = NULL;

        m_bIsInPhaseTwo = false;
        m_bIsEnraged = false;
        m_bRelocated = false;

        SetCombatMovement(false);
    }

    void Aggro(Unit *who)
    {
        m_creature->SendMonsterMove(m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+20.0f,SPLINETYPE_NORMAL,m_creature->GetSplineFlags(),3000); 
    }

    void JustSummoned(Creature* pSummoned)
    {
        //disabled for testing
        if (pSummoned->GetEntry() == NPC_HIVEZARA_LARVA)
            if (Unit* pLarvaTarget = Unit::GetUnit(*m_creature,m_uiLarvaTargetGUID))
                pSummoned->AI()->AttackStart(pLarvaTarget);
        
        if (pSummoned->GetEntry() == NPC_HIVEZARA_SWARMER)
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bRelocated && m_uiRelocate_Timer < uiDiff)
         {
            m_creature->Relocate(m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+20.0f,0.0f);
            m_bRelocated = true;
         }
        else
            m_uiRelocate_Timer -= uiDiff;
 
        if (!m_bIsInPhaseTwo && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 70)
         {
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            m_creature->AttackerStateUpdate(m_creature->getVictim(),BASE_ATTACK,true);
            m_bIsInPhaseTwo = true;
        }
 
        if (!m_bIsEnraged && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20)
         {
            DoCast(m_creature,SPELL_FRENZY);
            DoScriptText(EMOTE_FRENZY,m_creature);
            m_bIsEnraged = true;
        }
 
        if (m_uiSummonSwarmer_Timer < uiDiff)
         {
            for (uint8 i = 0; i < 20; ++i)
                m_creature->SummonCreature(NPC_HIVEZARA_SWARMER,Swarmers[0].x + rand()%10, Swarmers[0].y + rand()%10,Swarmers[0].z + rand()%10,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,15000);
            m_uiSummonSwarmer_Timer = 60000;
        }
        else 
            m_uiSummonSwarmer_Timer -= uiDiff;
 
        if (m_uiSummonPlayer_Timer < uiDiff)
        {
            if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                m_creature->CastSpell(pTarget,SPELL_PARALYZE,false);
                uint32 random = rand()%2;
                m_creature->SummonCreature(NPC_HIVEZARA_LARVA,Larva[random].x,Larva[random].y,Larva[random].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                m_uiLarvaTargetGUID = pTarget->GetGUID();
            }
            m_uiSummonPlayer_Timer = 15000;
        }
        else
            m_uiSummonPlayer_Timer -= uiDiff;
            
        if (!m_bIsInPhaseTwo)
        {
            if (m_uiPoisonStinger_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_POISONSTINGER);
                m_uiPoisonStinger_Timer = 3000;
            }
            else
                m_uiPoisonStinger_Timer -= uiDiff;
        }
        else
        {
            if (m_uiStingerSpray_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_STINGERSPRAY);
                m_uiStingerSpray_Timer = 10000;
            }
            else 
                m_uiStingerSpray_Timer -= uiDiff;

            if (m_uiLash_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_LASH);
                m_uiLash_Timer = 10000 + rand()%10000;
            }
            else
                m_uiLash_Timer -= uiDiff;

            if (m_uiTrash_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_TRASH);
                m_uiTrash_Timer = 10000 + rand()%10000;
            }
            else
                m_uiTrash_Timer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_zara_larvaAI : public ScriptedAI
{
    mob_zara_larvaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 m_uiTargetGUID;

    void Aggro(Unit *pWho)
    {
        m_uiTargetGUID = pWho->GetGUID();
    }

    void Reset() 
    {
        m_uiTargetGUID = NULL;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if (Unit* pTarget = Unit::GetUnit(*m_creature,m_uiTargetGUID))
            if (m_creature->IsWithinDistInMap(pTarget,ATTACK_DISTANCE))
                if(m_creature->getVictim()->HasAura(SPELL_PARALYZE))
                    DoCast(pTarget, SPELL_FEED);

         DoMeleeAttackIfReady();
     }
 };

CreatureAI* GetAI_boss_ayamiss(Creature* pCreature)
{
    return new boss_ayamissAI(pCreature);
}

CreatureAI* GetAI_mob_zara_larva(Creature* pCreature)
{
    return new mob_zara_larvaAI(pCreature);
}

void AddSC_boss_ayamiss()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_ayamiss";
    newscript->GetAI = &GetAI_boss_ayamiss;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zara_larva";
    newscript->GetAI = &GetAI_mob_zara_larva;
    newscript->RegisterSelf();
}
