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
SDName: Boss_Wushoolay
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SPELL_LIGHTNING_CLOUD   =   25033,
    SPELL_LIGHTNING_WAVE    =   24819
};

struct MANGOS_DLL_DECL boss_wushoolayAI : public ScriptedAI
{
    boss_wushoolayAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiLightningCloud_Timer;
    uint32 m_uiLightningWave_Timer;

    void Reset()
    {
        m_uiLightningCloud_Timer = 5000 + rand()%5000;
        m_uiLightningWave_Timer = 8000 + rand()%8000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLightningCloud_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_LIGHTNING_CLOUD);
            m_uiLightningCloud_Timer = 15000 + rand()%5000;
        }
        else
            m_uiLightningCloud_Timer -= diff;
 
        if (m_uiLightningWave_Timer < diff)
         {
            if (Unit* pTarget =SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_LIGHTNING_WAVE);
 
            m_uiLightningWave_Timer = 12000 + rand()%4000;
        }
        else
            m_uiLightningWave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_wushoolay(Creature* pCreature)
{
    return new boss_wushoolayAI(pCreature);
}

void AddSC_boss_wushoolay()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_wushoolay";
    newscript->GetAI = &GetAI_boss_wushoolay;
    newscript->RegisterSelf();
}
