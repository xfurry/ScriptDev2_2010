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
SDName: Boss_Hazzarah
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    NPC_NIGHTMARE_ILLUSION  =   15163,
/*
    // find out the model ids of the possible outward appearences of the illusions
    
    MODEL_FELGUARD          =   
    MODEL_ABOMINATION       =   
    MODEL_LASHER            =   
    MODEL_DEVILSAUR         =   
*/
    SPELL_MANABURN          =   26046,
    SPELL_SLEEP             =   24664
};

struct MANGOS_DLL_DECL boss_hazzarahAI : public ScriptedAI
{
    boss_hazzarahAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiManaBurn_Timer;
    uint32 m_uiSleep_Timer;
    uint32 m_uiIllusions_Timer;

    void Reset()
    {
        m_uiManaBurn_Timer = 4000 + rand()%6000;
        m_uiSleep_Timer = 10000 + rand()%8000;
        m_uiIllusions_Timer = 10000 + rand()%8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiManaBurn_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_MANABURN);
            m_uiManaBurn_Timer = 8000 + rand()%8000;
        }
        else
            m_uiManaBurn_Timer -= uiDiff;
 
        if (m_uiSleep_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_SLEEP);
            m_uiSleep_Timer = 12000 + rand()%8000;
        }
        else
            m_uiSleep_Timer -= uiDiff;
 
        if (m_uiIllusions_Timer < uiDiff)
         {
            //We will summon 3 illusions that will spawn on a random target and attack this target
             //We will just use one model for the beginning
            Creature* pIllusion = NULL;
            Unit* pTarget = NULL;
            for(uint8 i = 0; i < 3; ++i)
             {
                pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
                pIllusion = m_creature->SummonCreature(NPC_NIGHTMARE_ILLUSION, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                if (pIllusion)
                {
                    /*
                    // uncomment this, when we have found out the model ids
                    switch(rand()%4)
                    {
                        case 0: pIllusion->SetDisplayId(MODEL_FELGUARD); break;
                        case 1: pIllusion->SetDisplayId(MODEL_ABOMINATION); break;
                        case 2: pIllusion->SetDisplayId(MODEL_LASHER); break;
                        case 3: pIllusion->SetDisplayId(MODEL_DEVILSAUR);
                    }
                    */
                    if (pTarget)
                        pIllusion->AI()->AttackStart(pTarget);
                }
            }
            m_uiIllusions_Timer = 15000 + rand()%10000;
        }
        else
            m_uiIllusions_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_hazzarah(Creature* pCreature)
{
    return new boss_hazzarahAI(pCreature);
}

void AddSC_boss_hazzarah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hazzarah";
    newscript->GetAI = &GetAI_boss_hazzarah;
    newscript->RegisterSelf();
}
