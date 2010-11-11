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
SDName: Boss_Tharonja
SD%Complete: 80%
SDComment: Timers
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_KING_DRED_TALON                = -1600020,
    SAY_CALL_FOR_RAPTOR                = -1600021,

    SPELL_BELLOWING_ROAR                = 22686,
    SPELL_FEARSOME_ROAR                 = 48849,
    H_SPELL_FEARSOME_ROAR               = 59422,
    SPELL_GRIEVOUS_BITE                 = 48920,
    SPELL_MANGLING_SLASH                = 48873,
    SPELL_PIERCING_SLASH                = 48878,
    SPELL_RAPTOR_CALL                   = 59416,            //not yet implemented

    SPELL_GUT_RIP                       = 49710,
    SPELL_REND                          = 13738
};

enum eArchivements
{
    ACHIEV_BETTER_OFF_DRED                        = 2039
};

uint32 m_uiRaptorCounter;

/*######
## boss_dred
######*/

struct MANGOS_DLL_DECL boss_dredAI : public ScriptedAI
{
    boss_dredAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 FearsomeRoar_Timer;
    uint32 ManglingSlash_Timer;
    uint32 PiercingSlash_Timer;
    uint32 GrievousBite_Timer;
    uint32 BellowingRoar_Timer;
    uint32 Check_Timer;
    uint32 CallForRaptor_Timer;

    void Reset()
    {
        FearsomeRoar_Timer = 15000;
        ManglingSlash_Timer = urand(5000, 10000);
        PiercingSlash_Timer = urand(10000, 15000);
        GrievousBite_Timer = urand (15000, 20000);
        BellowingRoar_Timer = 60000;
        Check_Timer = 15000;
        CallForRaptor_Timer = 25000;

        m_uiRaptorCounter = 0;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_KING_DRED, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KING_DRED, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KING_DRED, DONE);

        if(!m_bIsRegularMode && m_uiRaptorCounter >= 6)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_BETTER_OFF_DRED);
        }
    }

    Creature* SelectRandomRaptor(float fRange)
    {
        std::list<Creature* > lRapotrList;
        switch (urand(0,1))
        {
        case 0:
            GetCreatureListWithEntryInGrid(lRapotrList, m_creature, NPC_DRAKKARI_GUTRIPPER, fRange);
            break;
        case 1:
            GetCreatureListWithEntryInGrid(lRapotrList, m_creature, NPC_DRAKKARI_SCYTHECLAW, fRange);
            break;
        }

        if (lRapotrList.empty()){
            CallForRaptor_Timer = 25000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lRapotrList.begin();
        advance(iter, urand(0, lRapotrList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Fearsome Roar
        if (FearsomeRoar_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FEARSOME_ROAR : H_SPELL_FEARSOME_ROAR, true);
            FearsomeRoar_Timer = 15000;
        }else FearsomeRoar_Timer -= uiDiff;

        //Piercing Slash
        if (PiercingSlash_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_PIERCING_SLASH, true);
            PiercingSlash_Timer = urand(20000, 25000);
        }else PiercingSlash_Timer -= uiDiff;

        //Mangling Slash
        if (ManglingSlash_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_MANGLING_SLASH, true);
            ManglingSlash_Timer = urand(20000, 25000);
        }else ManglingSlash_Timer -= uiDiff;

        //Mangling Slash
        if (GrievousBite_Timer < uiDiff)
        {    
            DoCast(m_creature->getVictim(), SPELL_GRIEVOUS_BITE, true);
            GrievousBite_Timer = urand(20000, 25000);
        }else GrievousBite_Timer -= uiDiff;

        //Grievous Bite remove
        if (Check_Timer < uiDiff)
        {
            Unit* pPlayer = m_creature->getVictim();
            if (pPlayer->GetHealth() == pPlayer->GetMaxHealth())
                if (pPlayer->HasAura(SPELL_GRIEVOUS_BITE))
                    pPlayer->RemoveAura(SPELL_GRIEVOUS_BITE, EFFECT_INDEX_0);
            Check_Timer = 1000;
        }else Check_Timer -= uiDiff;

        //Bellowing Roar
        if (BellowingRoar_Timer < uiDiff)
        {    
            DoCast(m_creature, SPELL_BELLOWING_ROAR);
            BellowingRoar_Timer = 60000;
        }else BellowingRoar_Timer -= uiDiff;

        //Call For Raptor - spell
        if (CallForRaptor_Timer < uiDiff)
        {    
            DoScriptText(SAY_CALL_FOR_RAPTOR, m_creature);
            m_creature->CastSpell(m_creature, SAY_CALL_FOR_RAPTOR, true);
            CallForRaptor_Timer = 25000;
            if(Creature* pRaptor = SelectRandomRaptor(50.0f))
                pRaptor->AI()->AttackStart(m_creature->getVictim());
        }else CallForRaptor_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_dred_raptorAI : public ScriptedAI
{
    mob_dred_raptorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiRendTimer;
    uint32 GutRipTimer;
    uint32 creatureEntry;

    void Reset()
    {
        uiRendTimer = urand(10000, 15000);
        GutRipTimer = urand(10000, 15000);
        creatureEntry = m_creature->GetEntry();
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {
            if(!m_bIsRegularMode && m_pInstance->GetData(TYPE_KING_DRED) == IN_PROGRESS)
                ++m_uiRaptorCounter;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (creatureEntry)
        {
        case NPC_DRAKKARI_GUTRIPPER:
            if (GutRipTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_GUT_RIP);
                GutRipTimer = urand(10000,15000);
            }else GutRipTimer -=uiDiff;
            break;
        case NPC_DRAKKARI_SCYTHECLAW:
            if (uiRendTimer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_REND);
                uiRendTimer = urand(10000, 15000);
            }else uiRendTimer -=uiDiff;
            break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_dred(Creature* pCreature)
{
    return new boss_dredAI(pCreature);
}

CreatureAI* GetAI_mob_dred_raptor(Creature* pCreature)
{
    return new mob_dred_raptorAI(pCreature);
}

void AddSC_boss_dred()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_dred";
    newscript->GetAI = &GetAI_boss_dred;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dred_raptor";
    newscript->GetAI = &GetAI_mob_dred_raptor;
    newscript->RegisterSelf();
}