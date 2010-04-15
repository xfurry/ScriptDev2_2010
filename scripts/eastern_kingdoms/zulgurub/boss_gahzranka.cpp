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
SDName: Boss_Gahz'ranka
SD%Complete: 85
SDComment: Massive Geyser with knockback not working. Spell buggy.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"

enum
{
    SPELL_FROSTBREATH   =   16099,
    SPELL_MASSIVEGEYSER =   22421,   //Not working. Cause its a summon...
    SPELL_SLAM          =   24326,
    SPELL_TRASH         =   3391
};
struct MANGOS_DLL_DECL boss_gahzrankaAI : public ScriptedAI
{
     boss_gahzrankaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}
 
    uint32 m_uiFrostbreath_Timer;
    uint32 m_uiMassiveGeyser_Timer;
    uint32 m_uiSlam_Timer;
    uint32 m_uiTrash_Timer;

     void Reset()
     {
        m_uiFrostbreath_Timer = 8000;
        m_uiMassiveGeyser_Timer = 25000;
        m_uiSlam_Timer = 17000;
        m_uiTrash_Timer = 10000;
     }
 
    void UpdateAI(const uint32 uiDiff)
     {
         if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
             return;
 
        if (m_uiFrostbreath_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_FROSTBREATH);
            m_uiFrostbreath_Timer = 7000 + rand()%4000;
        }
        else
            m_uiFrostbreath_Timer -= uiDiff;
 
        if (m_uiMassiveGeyser_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_MASSIVEGEYSER);
             DoResetThreat();
 
            m_uiMassiveGeyser_Timer = 22000 + rand()%10000;
        }
        else
            m_uiMassiveGeyser_Timer -= uiDiff;
 
        if (m_uiSlam_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_SLAM);
            m_uiSlam_Timer = 12000 + rand()%8000;
        }
        else
            m_uiSlam_Timer -= uiDiff;
 
        if (m_uiTrash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_TRASH);
            m_uiTrash_Timer = 10000 + rand()%10000;
        }
        else
            m_uiTrash_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_gahzranka(Creature* pCreature)
{
    return new boss_gahzrankaAI(pCreature);
}

void AddSC_boss_gahzranka()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_gahzranka";
    newscript->GetAI = &GetAI_boss_gahzranka;
    newscript->RegisterSelf();
}
