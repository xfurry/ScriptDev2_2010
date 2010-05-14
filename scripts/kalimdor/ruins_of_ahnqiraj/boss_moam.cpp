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
SDName: Boss_Moam
SD%Complete: 100
SDComment:
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"

enum
{
    EMOTE_AGGRO              = -1509000,
    EMOTE_MANA_FULL          = -1509001,
    EMOTE_ENERGIZING         = -1509028,

     SPELL_TRAMPLE           =  15550,
     SPELL_DRAIN_MANA         = 25671,
     SPELL_ARCANEERUPTION    =  25672,
     SPELL_SUMMON_MANA_FIEND =  25681,                      //25682,25683 
     SPELL_ENERGIZE          =  25685,
 
    //mana fiend
     NPC_MANA_FIEND          =  15527,
};

struct MANGOS_DLL_DECL boss_moamAI : public ScriptedAI
{
    boss_moamAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiTrample_Timer;
    uint32 m_uiSummonManaFiend_Timer;
    uint32 m_uiManaGain_Timer;
    uint32 m_uiWait_Timer;
    uint8 m_uiFiendCount;
    uint32 m_uiDrainMana_Timer;

    void Reset()
    {
        m_uiTrample_Timer = 10000;
        m_uiSummonManaFiend_Timer = 90000;
        m_uiManaGain_Timer = 5000;
        m_uiWait_Timer = 20000;
        m_uiDrainMana_Timer = 3000;

        m_uiFiendCount = 0;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(EMOTE_AGGRO, m_creature);
        m_creature->SetPower(POWER_MANA,0);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            pSummoned->AI()->AttackStart(pTarget);

        if (pSummoned->GetEntry() == NPC_MANA_FIEND)
             ++m_uiFiendCount;
    }

    void SummonedCreatureDespawn(Creature* pCreature) 
    {
         if (pCreature->GetEntry() == NPC_MANA_FIEND)
             --m_uiFiendCount;

         if (!m_uiFiendCount)
             m_creature->RemoveAurasDueToSpell(SPELL_ENERGIZE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiManaGain_Timer < uiDiff)
        {
            m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)+1000);
            m_uiManaGain_Timer = 5000;
        }
        else
            m_uiManaGain_Timer -= uiDiff;

         //If we are 100%MANA cast Arcane Erruption
        if (m_creature->GetPower(POWER_MANA) == m_creature->GetMaxPower(POWER_MANA) && !m_creature->IsNonMeleeSpellCasted(false))
         {
            m_creature->RemoveAurasDueToSpell(SPELL_ENERGIZE);
             DoCast(m_creature->getVictim(),SPELL_ARCANEERUPTION);
             DoScriptText(EMOTE_MANA_FULL, m_creature);
         }
 
        //m_uiSummonManaFiend_Timer
        if (m_uiSummonManaFiend_Timer < uiDiff)
         {
            //DoCast(m_creature,SUMMON_MANA_FIEND); //summons only one
            for (uint8 i = 0; i < 3; ++i)
                m_creature->SummonCreature(NPC_MANA_FIEND,m_creature->GetPositionX()+2,m_creature->GetPositionY(),m_creature->GetPositionZ(),0,TEMPSUMMON_CORPSE_DESPAWN,0);            
            m_uiSummonManaFiend_Timer = 90000;
            m_creature->AttackStop();
            DoCast(m_creature,SPELL_ENERGIZE);
            DoScriptText(EMOTE_ENERGIZING,m_creature);
            m_uiWait_Timer = 10000;
         }
        else
            m_uiSummonManaFiend_Timer -= uiDiff;
 
        //m_uiTrample_Timer
        if (m_uiTrample_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_TRAMPLE);
            m_uiTrample_Timer = 10000;
        }
        else
            m_uiTrample_Timer -= uiDiff;
 
        //m_uiDrainMana_Timer
        if (m_uiDrainMana_Timer < uiDiff)
        {
             DoCast(m_creature->getVictim(),SPELL_DRAIN_MANA);
            m_uiDrainMana_Timer = 30000;
        }
        else
            m_uiDrainMana_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_moam(Creature* pCreature)
{
    return new boss_moamAI(pCreature);
}

void AddSC_boss_moam()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_moam";
    newscript->GetAI = &GetAI_boss_moam;
    newscript->RegisterSelf();
}
