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
SDName: instance_culling_of_stratholme
SD%Complete: ?%
SDComment:
SDCategory: Culling of Stratholme
EndScriptData */



#include "precompiled.h"
#include "culling_of_stratholme.h"

enum
{
    SAY_SALRAMM_SPAWN            = -1594129,
    SAY_SALRAMM_AGGRO            = -1594130,   
    SAY_SALRAMM_DEATH            = -1594131,  
    SAY_SALRAMM_SLAY01           = -1594132, 
    SAY_SALRAMM_SLAY02           = -1594133, 
    SAY_SALRAMM_SLAY03           = -1594134,
    SAY_SALRAMM_STEAL01          = -1594135,
    SAY_SALRAMM_STEAL02          = -1594136,
    SAY_SALRAMM_STEAL03          = -1594137,

    SPELL_SB                     = 57725,
    SPELL_SB_H                   = 58827,
    SPELL_FLESH                  = 58845,
    SPELL_STEAL                  = 52708
};

/*######
## boss_salramm
######*/
struct MANGOS_DLL_DECL boss_salrammAI : public ScriptedAI
{
	boss_salrammAI(Creature *pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

	ScriptedInstance* m_pInstance;

	bool m_bIsRegularMode;
	uint32 Step;
	uint32 Steptim;
	uint32 Motion;
	uint32 ShadowBolt_Timer;
	uint32 Flesh_Timer;
	uint32 Steal_Timer;
	
	void Reset()
	{
		Step = 1;
		Steptim = 7000;
		ShadowBolt_Timer = 5300;
		Flesh_Timer = 7300;
		Steal_Timer = 17300;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SALRAMM, NOT_STARTED);
	}
	
	void Aggro(Unit* who)
	{
		Motion = 0;
		DoScriptText(SAY_SALRAMM_AGGRO, m_creature);
		m_creature->GetMotionMaster()->Clear(false);

		if (m_pInstance)
            m_pInstance->SetData(TYPE_SALRAMM, IN_PROGRESS);
	}
	
	void JustDied(Unit *killer)
	{
		DoScriptText(SAY_SALRAMM_DEATH, m_creature);

		if (m_pInstance)
            m_pInstance->SetData(TYPE_SALRAMM, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SALRAMM_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_SALRAMM_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_SALRAMM_SLAY03, m_creature); break;
        }
    }

	void UpdateAI(const uint32 diff)
	{
		DoMeleeAttackIfReady();
		
		if(Motion == 0)
		{
			switch(Step)
			{
				case 1:
					DoScriptText(SAY_SALRAMM_SPAWN, m_creature); 
                    ++Step;
                    Steptim = 7000;
                    break;
                case 3:
                    m_creature->GetMotionMaster()->MovePoint(0, 2165.073f,1279.338f,133.40f);
                    ++Step;
                    Steptim = 7000;
                    break;
			}
		}
		else
			return;
		
		if (Steptim <= diff)
		{
			++Step;
			Steptim = 330000;
		}
		Steptim -= diff;
		
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;
		
		if (ShadowBolt_Timer < diff)
		{
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
				DoCast(target, m_bIsRegularMode ? SPELL_SB : SPELL_SB_H);

            ShadowBolt_Timer = 5300;
        }
		else 
			ShadowBolt_Timer -= diff;

		if (Flesh_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_FLESH);

            Flesh_Timer = 7300;
        }
		else 
			Flesh_Timer -= diff;

		if (Steal_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_STEAL);
			
			switch(rand()%3)
			{
				case 0: DoScriptText(SAY_SALRAMM_STEAL01, m_creature); break;
				case 1: DoScriptText(SAY_SALRAMM_STEAL02, m_creature); break;
				case 2: DoScriptText(SAY_SALRAMM_STEAL03, m_creature); break;
			}

            Steal_Timer = 17300;
        }
		else 
			Steal_Timer -= diff;

	}
};

CreatureAI* GetAI_boss_salramm(Creature *_Creature)
{
    boss_salrammAI* salrammAI = new boss_salrammAI(_Creature);
    return (CreatureAI*)salrammAI;
};

void AddSC_boss_salramm()
{
    Script *newscript;
	
    newscript = new Script;
    newscript->Name = "boss_salramm";
    newscript->GetAI = &GetAI_boss_salramm;
    newscript->RegisterSelf();
}
