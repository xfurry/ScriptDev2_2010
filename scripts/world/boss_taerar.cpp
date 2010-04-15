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
SDName: Taerar
SD%Complete: 70
SDComment: Mark of Nature & Teleport NYI.
SDCategory: Bosses
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO               = -1000399,
    SAY_SUMMONSHADE         = -1000400,

    //Spells of Taerar
    SPELL_SLEEP             = 24778,
    SPELL_NOXIOUSBREATH     = 24818,
    SPELL_TAILSWEEP         = 15847,
    SPELL_SUMMON_PLAYER     = 24776,
    // SPELL_MARKOFNATURE   = 25040,                        // Not working

    //Taerar's specific spells
    SPELL_ARCANEBLAST       = 24857,
    SPELL_BELLOWINGROAR     = 22686,

    SPELL_SUMMONSHADE_1     = 24841,
    SPELL_SUMMONSHADE_2     = 24842,
    SPELL_SUMMONSHADE_3     = 24843,

    NPC_SHADE               = 15302,

    //Spells of Shades of Taerar
    SPELL_POSIONCLOUD       = 24840,
    SPELL_POSIONBREATH      = 20667
};

uint32 m_auiSpellSummonShade[]=
{
    SPELL_SUMMONSHADE_1, SPELL_SUMMONSHADE_2, SPELL_SUMMONSHADE_3
};

struct MANGOS_DLL_DECL boss_taerarAI : public ScriptedAI
{
    boss_taerarAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSleep_Timer;
    uint32 m_uiNoxiousBreath_Timer;
    uint32 m_uiTailSweep_Timer;
    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiArcaneBlast_Timer;
    uint32 m_uiBellowingRoar_Timer;
    uint8 m_uiAdds_Summoned;
    uint8 m_uiWave_Count;

    bool m_bShades;

    void Reset()
    {
        m_uiSleep_Timer = urand(15000, 20000);
        m_uiNoxiousBreath_Timer = 8000;
        m_uiTailSweep_Timer = 4000;
        //m_uiMarkOfNature_Timer = 45000;
        m_uiArcaneBlast_Timer = 12000;
        m_uiBellowingRoar_Timer = 30000;
        m_uiAdds_Summoned = 0;
        m_uiWave_Count = 1;

        m_bShades = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void DamageTaken(Unit *pDoneBy,uint32 &uiDamage)
    {
        if (m_uiWave_Count >= 4)
            return;

        if (m_creature->GetHealth()*100/m_creature->GetMaxHealth() <= (100+urand(-5,5)-(25*m_uiWave_Count)))
        {
            ++m_uiWave_Count;

            //Inturrupt any spell casting
            m_creature->InterruptNonMeleeSpells(false);

            //horrible workaround, need to fix
            m_creature->setFaction(35);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            DoScriptText(SAY_SUMMONSHADE, m_creature);
            for(uint8 i = 0; i < 3; ++i)
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(NPC_SHADE,pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(),0,TEMPSUMMON_CORPSE_DESPAWN,0);                                      
            m_bShades = true;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);

        ++m_uiAdds_Summoned;
    }

    void SummonedCreatureDespawn(Creature *pDespawned)
    {
        --m_uiAdds_Summoned;
        if (m_uiAdds_Summoned == 0)
         {
             m_creature->setFaction(14);
             m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
             m_bShades = false;
         }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (m_bShades || !m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Sleep_Timer
        if (m_uiSleep_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_SLEEP);

            m_uiSleep_Timer = urand(8000, 15000);
        }
        else
            m_uiSleep_Timer -= uiDiff;

        //NoxiousBreath_Timer
        if (m_uiNoxiousBreath_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_NOXIOUSBREATH);
            m_uiNoxiousBreath_Timer = urand(14000, 20000);
        }
        else
            m_uiNoxiousBreath_Timer -= uiDiff;

        //Tailsweep every 2 seconds
        if (m_uiTailSweep_Timer < uiDiff)
        {
            if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                if (m_creature->HasInArc(3.1415f, pTarget))
                {
                    DoCast(pTarget, SPELL_TAILSWEEP);
                    m_uiTailSweep_Timer = 2000;
                }
        }
        else
            m_uiTailSweep_Timer -= uiDiff;

        if (Unit *pEscaper = GetPlayerAtMinimumRange(DEFAULT_VISIBILITY_DISTANCE - 10.0f))
            DoCast(pEscaper,SPELL_SUMMON_PLAYER);

        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer < uiDiff)
        //{
        //    DoCastSpellIfCan(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //ArcaneBlast_Timer
        if (m_uiArcaneBlast_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARCANEBLAST);
            m_uiArcaneBlast_Timer = urand(7000, 12000);
        }
        else
            m_uiArcaneBlast_Timer -= uiDiff;

        //BellowingRoar_Timer
        if (m_uiBellowingRoar_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_BELLOWINGROAR);
            m_uiBellowingRoar_Timer = urand(20000, 30000);
        }
        else
            m_uiBellowingRoar_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Shades of Taerar Script
struct MANGOS_DLL_DECL boss_shadeoftaerarAI : public ScriptedAI
{
    boss_shadeoftaerarAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiPoisonCloud_Timer;
    uint32 m_uiPosionBreath_Timer;

    void Reset()
    {
        m_uiPoisonCloud_Timer = 8000;
        m_uiPosionBreath_Timer = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //PoisonCloud_Timer
        if (m_uiPoisonCloud_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_POSIONCLOUD);
            m_uiPoisonCloud_Timer = 30000;
        }
        else
            m_uiPoisonCloud_Timer -= uiDiff;

        //PosionBreath_Timer
        if (m_uiPosionBreath_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_POSIONBREATH);
            m_uiPosionBreath_Timer = 12000;
        }
        else
            m_uiPosionBreath_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taerar(Creature* pCreature)
{
    return new boss_taerarAI(pCreature);
}

CreatureAI* GetAI_boss_shadeoftaerar(Creature* pCreature)
{
    return new boss_shadeoftaerarAI(pCreature);
}

void AddSC_boss_taerar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_taerar";
    newscript->GetAI = &GetAI_boss_taerar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_shade_of_taerar";
    newscript->GetAI = &GetAI_boss_shadeoftaerar;
    newscript->RegisterSelf();
}
