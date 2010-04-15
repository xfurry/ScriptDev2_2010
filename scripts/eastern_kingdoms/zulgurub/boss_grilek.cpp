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
SDName: Boss_Grilek
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SPELL_AVARTAR       =   24646,   //The Enrage Spell
    SPELL_GROUNDTREMOR  =   6524
};
 
struct MANGOS_DLL_DECL boss_grilekAI : public ScriptedAI
{
     boss_grilekAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
 
    uint32 m_uiAvartar_Timer;
    uint32 m_uiGroundTremor_Timer;
 
     void Reset()
     {
        m_uiAvartar_Timer = 15000 + rand()%10000;
        m_uiGroundTremor_Timer = 8000 + rand()%8000;
     }
 
    void UpdateAI(const uint32 uiDiff)
     {
         if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
             return;
 
        if (m_uiAvartar_Timer < uiDiff)
        {
             DoCast(m_creature, SPELL_AVARTAR);
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
            {
                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-50);
 
                AttackStart(pTarget);
            }
            m_uiAvartar_Timer = 25000 + rand()%10000;
        }
        else
            m_uiAvartar_Timer -= uiDiff;
 
        if (m_uiGroundTremor_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(), SPELL_GROUNDTREMOR);
            m_uiGroundTremor_Timer = 12000 + rand()%4000;
        }
        else
            m_uiGroundTremor_Timer -= uiDiff;
        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_grilek(Creature* pCreature)
{
    return new boss_grilekAI(pCreature);
}

void AddSC_boss_grilek()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_grilek";
    newscript->GetAI = &GetAI_boss_grilek;
    newscript->RegisterSelf();
}
