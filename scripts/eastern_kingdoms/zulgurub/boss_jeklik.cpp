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
SDName: Boss_Jeklik
SD%Complete: 85
SDComment: Problem in finding the right flying batriders for spawning and making them fly.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SAY_AGGRO                       =   -1309002,
    SAY_RAIN_FIRE                   =   -1309003,
    SAY_DEATH                       =   -1309004,
    EMOTE_SHRIEK                    =   -1309025,
    EMOTE_GREATER_HEAL              =   -1309026,
    
    NPC_FRENZIED_BLOODSEEKER_BAT    =   14965,
 
    SPELL_CHARGE                    =   22911,
    SPELL_SONICBURST                =   23918,
    SPELL_BLOOD_LEECH               =   22644,
    SPELL_SHADOW_WORD_PAIN          =   23952, 
    SPELL_CHAIN_MIND_FLAY           =   23953, 
    SPELL_GREATERHEAL               =   23954,
    SPELL_BAT_FORM                  =   23966,
    SPELL_CURSE_OF_BLOOD            =   16098,
    SPELL_PSYCHIC_SCREAM            =   22884,
    SPELL_PIERCE_ARMOR              =   12097,
    SPELL_SWOOP                     =   23919,
    SPELL_TRANSFORMBACK             =   24085,
    SPELL_SUMMONBATS                =   23794,
 
    // Batriders Spell
    SPELL_THROWING_LIQUID_FIRE      =   23968    
};

struct MANGOS_DLL_DECL boss_jeklikAI : public ScriptedAI
{
    boss_jeklikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpawnBats_Timer;
    uint32 m_uiSpawnFlyingBats_Timer;
	uint32 m_uiRandomSpellOne_Timer;
 
    bool m_bPhaseTwo;
    bool m_bHasSaidRain;

    void Reset()
    {
        m_uiSpawnBats_Timer = 35000;
        m_uiSpawnFlyingBats_Timer = 30000;
		m_uiRandomSpellOne_Timer = 10000;
 
        m_bPhaseTwo = false;
        m_bHasSaidRain = false; 
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature,SPELL_BAT_FORM);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_JEKLIK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
     {
         if (!m_creature->getVictim() && !m_creature->SelectHostileTarget())
             return;
 
        if (!m_bPhaseTwo && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 50))
         {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_TRANSFORMBACK);
			m_bPhaseTwo = true;
			m_creature->SetDisplayId(15219);
            DoResetThreat();
		}
		// Random Spell Cast
		if (m_uiRandomSpellOne_Timer < uiDiff)
		{
            if (!m_bPhaseTwo)
             {
			    switch (rand()%5)
			    {
				    case 0:
                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
					    DoCast(target,SPELL_CHARGE);
					    break;
				    case 1:
                        DoCast(m_creature->getVictim(),SPELL_SONICBURST);
					    break;
				    case 2:
                        DoCast(m_creature->getVictim(),SPELL_PIERCE_ARMOR);
					    break;
				    case 3:
                        DoCast(m_creature->getVictim(),SPELL_BLOOD_LEECH);
					    DoScriptText(EMOTE_SHRIEK,m_creature);
					    break;
				    case 4:
                        DoCast(m_creature->getVictim(),SPELL_SWOOP);
					    break;
			    }
			}
			else
			{
			    switch (rand()%5)
				{
					case 0:
					    if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
								DoCast(target, SPELL_SHADOW_WORD_PAIN);
						break;					
					case 1:
					    //m_creature->InterruptNonMeleeSpells(false);
						DoCast(m_creature->getVictim(), SPELL_CHAIN_MIND_FLAY);
						break;
					case 2:
				    DoCast(m_creature,SPELL_PSYCHIC_SCREAM);
    					break;
					case 3:
					    DoCast(m_creature->getVictim(),SPELL_CURSE_OF_BLOOD);
						break;
					case 4:
					    //m_creature->InterruptNonMeleeSpells(false);
						DoCast(m_creature,SPELL_GREATERHEAL);
						DoScriptText(EMOTE_GREATER_HEAL,m_creature);
				}
            }
			m_uiRandomSpellOne_Timer = rand()%10000;
		}
		else
		    m_uiRandomSpellOne_Timer -= uiDiff;
 
		if (!m_bPhaseTwo)
		{
            if (m_uiSpawnBats_Timer < uiDiff)
             {
                DoCast(m_creature,SPELL_SUMMONBATS);
			    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
			    {
			        Creature* pBat = NULL;
                    for(uint8 i = 0; i < 6; ++i)
                     {
                        pBat = m_creature->SummonCreature(NPC_FRENZIED_BLOODSEEKER_BAT,-12291.6220f,-1380.2640f,144.8304f,5.483f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        if (pBat)
                            pBat->AI()->AttackStart(pTarget);
                     }
                }
                m_uiSpawnBats_Timer = 60000;
             }
             else
                m_uiSpawnBats_Timer -= uiDiff;
        }
    	else
		{
		    if (m_uiSpawnFlyingBats_Timer < uiDiff)
             {
                if (!m_bHasSaidRain)
                {
                    DoScriptText(SAY_RAIN_FIRE,m_creature);
                    m_bHasSaidRain = true;
                }
                
				if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
				{
                    Creature* pFlyingBat = m_creature->SummonCreature(11369, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ()+15, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                    if (pFlyingBat)                    
                        pFlyingBat->AI()->AttackStart(pTarget);
                }
                m_uiSpawnFlyingBats_Timer = 80000 + rand()%5000;
             }
            else
                m_uiSpawnFlyingBats_Timer -=uiDiff;
         }
         DoMeleeAttackIfReady();
     }
 };
 
 //Flying Bat
struct MANGOS_DLL_DECL mob_bloodseeker_batAI : public ScriptedAI
{
    mob_bloodseeker_batAI(Creature* pCreature) : ScriptedAI(pCreature)
     {
        m_pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
         Reset();
     }
    
    ScriptedInstance* m_pInstance;    
    
    uint32 m_uiCheck_Timer;
 
     void Reset()
    {        
        m_uiCheck_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);                    
     }
 
    void Aggro(Unit *pTarget)
    {
        DoCast(m_creature,SPELL_THROWING_LIQUID_FIRE);        
    }
    
    void UpdateAI (const uint32 uiDiff)
    {
         if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
             return;
 
        if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
         {
            //make him not move
            m_creature->Attack(pTarget,false);
            m_creature->GetMotionMaster()->MoveConfused();      
        }
        
        //m_uiCheck_Timer
        if (m_uiCheck_Timer < uiDiff)
         {
            if (m_pInstance && m_pInstance->GetData(TYPE_JEKLIK) == DONE)
             {
                m_creature->setDeathState(JUST_DIED);
                m_creature->RemoveCorpse();
                return;
             }
            m_uiCheck_Timer = 1000;
        }
        else
            m_uiCheck_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jeklik(Creature* pCreature)
{
    return new boss_jeklikAI(pCreature);
}

CreatureAI* GetAI_mob_bloodseeker_bat(Creature* pCreature)
{
    return new mob_bloodseeker_batAI(pCreature);
}

void AddSC_boss_jeklik()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_jeklik";
    newscript->GetAI = &GetAI_boss_jeklik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bloodseeker_bat";
    newscript->GetAI = &GetAI_mob_bloodseeker_bat;
    newscript->RegisterSelf();
}
