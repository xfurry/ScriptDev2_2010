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
SDName: Lethon
SD%Complete: 60%
SDComment: Shades and mark not working
SDCategory: Bosses
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO               = -1000420,
    SAY_SUMMONSHADE         = -1000421,

    //Nightmare Dragons' shared spells
    SPELL_SLEEP             = 24778,
    SPELL_NOXIOUSBREATH     = 24818,
    SPELL_TAILSWEEP         = 15847,
    SPELL_SUMMON_PLAYER     = 24776,
    //SPELL_MARKOFNATURE    = 25040,                        // Not working
    
    //Specific Lethon's spells
    SPELL_DRAW_SPIRIT       = 24811,
    SPELL_SHADOWVOLLEY      = 24834, 

    NPC_SPIRIT_SHADE        = 15261,

    //Spells of Shades of Lethon
    SPELL_DARK_OFFERING     = 24804,
    SPELL_SHADE_VISUAL      = 24809
};

struct MANGOS_DLL_DECL boss_lethonAI : public ScriptedAI
{
    boss_lethonAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSleep_Timer;
    uint32 m_uiNoxiousBreath_Timer;
    uint32 m_uiTailSweep_Timer;
    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiDrawSpirit_Timer;
    uint32 m_uiShadowBoltVolley_Timer;

    uint8 m_uiWave_Count;

    void Reset()
    {
        m_uiSleep_Timer = 15000 + rand()%5000;
        m_uiNoxiousBreath_Timer = 8000;
        m_uiTailSweep_Timer = 4000;
        //m_uiMarkOfNature_Timer = 45000;
        m_uiDrawSpirit_Timer = 12000;
        m_uiShadowBoltVolley_Timer = 30000;

        m_uiWave_Count = 1;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void DamageTaken(Unit *pDoneBy,uint32 &uiDamage)
    {
        if (m_creature->GetHealth()*100/m_creature->GetMaxHealth() <= (100+urand(-5,5)-(25*m_uiWave_Count)))
        {
            if (m_uiWave_Count >= 4)
                return;

            ++m_uiWave_Count;
            DoCast(m_creature->getVictim(),SPELL_DRAW_SPIRIT);

            std::list<HostileReference*> t_list = m_creature->getThreatManager().getThreatList();
            
            if (t_list.empty())
                return;

            DoScriptText(SAY_SUMMONSHADE, m_creature);
            std::vector<Unit*> target_list;
            for (std::list<HostileReference*>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
				if (Unit *pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                    m_creature->SummonCreature(NPC_SPIRIT_SHADE,pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(),0,TEMPSUMMON_CORPSE_DESPAWN,0);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Sleep_Timer
        if (m_uiSleep_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SLEEP);

            m_uiSleep_Timer = 8000 + rand()%7000;
        }
        else
            m_uiSleep_Timer -= uiDiff;

        //NoxiousBreath_Timer
        if (m_uiNoxiousBreath_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_NOXIOUSBREATH);
            m_uiNoxiousBreath_Timer = 14000 + rand()%6000;
        }
        else
            m_uiNoxiousBreath_Timer -= uiDiff;

        //Tailsweep every 2 seconds
        if (m_uiTailSweep_Timer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
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
        //    DoCast(m_creature->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
            //m_uiMarkOfNature_Timer -= uiDiff;

        //m_uiShadowBoltVolley_Timer
        if (m_uiShadowBoltVolley_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOWVOLLEY);
            m_uiShadowBoltVolley_Timer = 20000 + rand()%10000;
        }
        else            m_uiShadowBoltVolley_Timer -= uiDiff;
        DoMeleeAttackIfReady();
   }
};

// Shades of Taerar Script
struct MANGOS_DLL_DECL mob_spirit_shadeAI : public ScriptedAI
{
   mob_spirit_shadeAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    bool m_bBuffed;

    void Reset()
    {
        m_bBuffed = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bBuffed)
            DoCast(m_creature,SPELL_SHADE_VISUAL);

        if (m_creature->IsWithinDistInMap(m_creature,ATTACK_DISTANCE))
        {
            DoCast(m_creature,SPELL_DARK_OFFERING);
            m_creature->ForcedDespawn();
        }
    }
};

CreatureAI* GetAI_boss_lethon(Creature* pCreature)
{
    return new boss_lethonAI(pCreature);
}

CreatureAI* GetAI_mob_spirit_shade(Creature* pCreature)
{
    return new mob_spirit_shadeAI(pCreature);
}

void AddSC_boss_lethon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_lethon";
    newscript->GetAI = &GetAI_boss_lethon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spirit_shade";
    newscript->GetAI = &GetAI_mob_spirit_shade;
    newscript->RegisterSelf();
}
