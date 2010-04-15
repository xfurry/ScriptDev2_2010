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
SDName: Boss_Renataki
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SPELL_AMBUSH            =   24337,
    SPELL_THOUSANDBLADES    =   24649
};

struct MANGOS_DLL_DECL boss_renatakiAI : public ScriptedAI
{
    boss_renatakiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiDefaultDisplayId = m_creature->GetDisplayId();
        Reset();
    }

    uint32 m_uiInvisible_Timer;
    uint32 m_uiAmbush_Timer;
    uint32 m_uiVisible_Timer;
    uint32 m_uiAggro_Timer;
    uint32 m_uiThousandBlades_Timer;
    
    uint32 m_uiDefaultDisplayId;
 
    bool m_bInvisible;
    bool m_bAmbushed;

    void Reset()
    {
        m_uiInvisible_Timer = 8000 + rand()%10000;
        m_uiAmbush_Timer = 3000;
        m_uiVisible_Timer = 4000;
        m_uiAggro_Timer = 15000 + rand()%10000;
        m_uiThousandBlades_Timer = 4000 + rand()%4000;
 
        m_bInvisible = false;
        m_bAmbushed = false;
        
        m_creature->SetDisplayId(m_uiDefaultDisplayId);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiInvisible_Timer < diff)
        {
             m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
             m_creature->SetDisplayId(11686);
 
            //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
            //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO , 218171138);
            //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO  + 1, 3);
             m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetUInt32Value(UNIT_FIELD_DISPLAYID,11686);
            m_bInvisible = true;
 
            m_uiInvisible_Timer = 15000 + rand()%15000;
        }
        else
            m_uiInvisible_Timer -= diff;
 
        if (m_bInvisible)
         {
            if (m_uiAmbush_Timer < diff)
             {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                 {
                    m_creature->GetMap()->CreatureRelocation(m_creature, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f);
                    m_creature->SendMonsterMove(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), SPLINETYPE_NORMAL, SPLINEFLAG_WALKMODE, 1);
                    DoCast(pTarget,SPELL_AMBUSH);
                 }
 
                m_bAmbushed = true;
                m_uiAmbush_Timer = 3000;
            }
            else
                m_uiAmbush_Timer -= diff;
         }
 
        if (m_bAmbushed)
         {
            if (m_uiVisible_Timer < diff)
             {
                 m_creature->InterruptSpell(CURRENT_GENERIC_SPELL);
 
                m_creature->SetDisplayId(m_uiDefaultDisplayId);
 
                 m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 31818);
                //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO , 218171138);
                //m_creature->SetUInt32Value( UNIT_VIRTUAL_ITEM_INFO  + 1, 3);
                
                m_bInvisible = false;
 
                m_uiVisible_Timer = 4000;
            }
            else
                m_uiVisible_Timer -= diff;
         }
 
         //Resetting some aggro so he attacks other gamers
        if (!m_bInvisible)
         {
            if (m_uiAggro_Timer < diff)
            {
                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-50);
 
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                    AttackStart(pTarget);
 
                m_uiAggro_Timer = 7000 + rand()%13000;
            }
            else m_uiAggro_Timer -= diff;
 
            if (m_uiThousandBlades_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_THOUSANDBLADES);
                m_uiThousandBlades_Timer = 7000 + rand()%5000;
            }
            else
                m_uiThousandBlades_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_renataki(Creature* pCreature)
{
    return new boss_renatakiAI(pCreature);
}

void AddSC_boss_renataki()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_renataki";
    newscript->GetAI = &GetAI_boss_renataki;
    newscript->RegisterSelf();
}
