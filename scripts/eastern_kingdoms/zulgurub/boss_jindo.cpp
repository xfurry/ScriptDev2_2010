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
SDName: Boss_Jin'do the Hexxer
SD%Complete: 85
SDComment: Mind Control not working because of core bug. Shades visible for all.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    NPC_SACRIFICED_TROLL            =   14826,
    
    SAY_AGGRO                       =   -1309014,
 
    SPELL_SUMMON_BRAIN_WASH_TOTEM   =   24262,
    SPELL_POWERFULL_HEALING_WARD    =   24309,  //We will not use this spell. We will summon a totem by script cause the spell totems will not cast.
    SPELL_HEX                       =   24053,
    SPELL_DELUSIONS_OF_JINDO        =   24306,
    //SPELL_SHADEOFJINDO              24308     //We will not use this spell. We will summon a shade by script.
 
    //Healing Ward Spell
    SPELL_HEAL                      =   24311,  //Totems are not working right. Right heal spell ID is 38588 but this spell is not casting...

    //Brain Wash Totem Spell
    SPELL_BRAIN_WASH                =   24261,
 
    //Shade of Jindo Spell
    SPELL_SHADOWSHOCK               =   24458,
    SPELL_SHADE_OF_JINDO            =   24307   //passive - making them visible only when affected by Delusion Curse
};	

struct MANGOS_DLL_DECL boss_jindoAI : public ScriptedAI
{
    boss_jindoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiBrainWashTotem_Timer;
    uint32 m_uiHealingWard_Timer;
    uint32 m_uiHex_Timer;
    uint32 m_uiDelusions_Timer;
    uint32 m_uiTeleport_Timer;

    void Reset()
    {
        m_uiBrainWashTotem_Timer = 20000;
        m_uiHealingWard_Timer = 16000;
        m_uiHex_Timer = 8000;
        m_uiDelusions_Timer = 10000;
        m_uiTeleport_Timer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBrainWashTotem_Timer < uiDiff)
         {
            DoCast(m_creature, SPELL_SUMMON_BRAIN_WASH_TOTEM);
            m_creature->SummonCreature(15112, m_creature->GetPositionX()+3, m_creature->GetPositionY()-2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,30000);
            m_uiBrainWashTotem_Timer = 18000 + rand()%8000;
        }
        else
            m_uiBrainWashTotem_Timer -= uiDiff;
 
        if (m_uiHealingWard_Timer < uiDiff)
         {
            DoCast(m_creature, SPELL_POWERFULL_HEALING_WARD);
            m_creature->SummonCreature(14987, m_creature->GetPositionX()+3, m_creature->GetPositionY()-2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,30000);
            m_uiHealingWard_Timer = 14000 + rand()%6000;
        }
        else
            m_uiHealingWard_Timer -= uiDiff;
 
        if (m_uiHex_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(), SPELL_HEX);			
             if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                 m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-80);
            m_uiHex_Timer = 12000 + rand()%8000;
        }
        else
            m_uiHex_Timer -= uiDiff;
 
         //Casting the delusion curse with a shade. So shade will attack the same target with the curse.
        if (m_uiDelusions_Timer < uiDiff)
         {
             if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
             {
                DoCast(target, SPELL_DELUSIONS_OF_JINDO);

                Creature* pShade = m_creature->SummonCreature(14986, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (pShade)
                    pShade->AI()->AttackStart(target);
             }
            m_uiDelusions_Timer = 4000 + rand()%8000;
        }
        else
            m_uiDelusions_Timer -= uiDiff;
 
        if (m_uiTeleport_Timer < uiDiff)
         {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
             {
                DoTeleportPlayer(pTarget, -11583.7783f, -1249.4278f, 79.5471f, 4.745f);
 
                if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                    m_creature->getThreatManager().modifyThreatPercent(pTarget,-100);

                /*
                // Skeletons shouldn't be summoned but respawned
                Creature *Skeletons;
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-2, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()-4, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-2, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()+4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX(), target->GetPositionY()-4, target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
                Skeletons = m_creature->SummonCreature(14826, target->GetPositionX()+3, target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if (Skeletons)
                    Skeletons->AI()->AttackStart(target);
				*/
                std::list<Creature*> lSkeletons;
                GetCreatureListWithEntryInGrid(lSkeletons,pTarget,NPC_SACRIFICED_TROLL,20.0f);
                if (lSkeletons.empty())
                    debug_log("SD2 ERROR : In Zul'Gurub no skeletons with the entry %i were found",NPC_SACRIFICED_TROLL);
                else
                {
                    for(std::list<Creature*>::iterator iter = lSkeletons.begin(); iter != lSkeletons.end(); ++iter)
                        if ((*iter) && (*iter)->isDead())
                        {
                            (*iter)->setDeathState(ALIVE);
                            (*iter)->SetHealth((*iter)->GetMaxHealth());
                            (*iter)->AI()->AttackStart(pTarget);
                        }
                }
             }
 
            m_uiTeleport_Timer = 15000 + rand()%8000;
        }
        else
            m_uiTeleport_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Healing Ward
struct MANGOS_DLL_DECL mob_healing_wardAI : public ScriptedAI
{
    mob_healing_wardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiHeal_Timer;

    void Reset()
    {
        m_uiHeal_Timer = 2000;
    }

    void UpdateAI (const uint32 uiDiff)
    {
        //Heal_Timer
        if (m_uiHeal_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if (Unit *pJindo = m_pInstance->instance->GetCreature(m_pInstance->GetData64(DATA_JINDO)))
                {
                    if (pJindo->isAlive())
                        DoCastSpellIfCan(pJindo, SPELL_HEAL);
                }
            }
            m_uiHeal_Timer = 3000;
        }else m_uiHeal_Timer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

//Shade of Jindo
struct MANGOS_DLL_DECL mob_shade_of_jindoAI : public ScriptedAI
{
    mob_shade_of_jindoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiShadowShock_Timer;

    void Reset()
    {
        m_uiShadowShock_Timer = 1000;
        DoCast(m_creature, SPELL_SHADE_OF_JINDO);
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //ShadowShock_Timer
        if (m_uiShadowShock_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHADOWSHOCK);
            m_uiShadowShock_Timer = 2000;
        }else m_uiShadowShock_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

//Brain wash totem
struct MANGOS_DLL_DECL mob_brain_wash_totemAI : public ScriptedAI
{	 
	mob_brain_wash_totemAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
	}

	ScriptedInstance* m_pInstance;

	uint32 m_uiBrainWash_Timer;

	void Reset()
	{
		m_uiBrainWash_Timer = 1000;		
	}
	void UpdateAI (const uint32 uiDiff)
	{
		if (m_uiBrainWash_Timer < uiDiff)
		{
			Unit *target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
			//Unit *pJindo = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_JINDO));
			if (target)			
			DoCast(target,SPELL_BRAIN_WASH);
			//if (pJindo->getThreatManager().getThreat(pJindo->getVictim()))
              //  pJindo->getThreatManager().modifyThreatPercent(target,-100);	 // causing Crash		
		}else m_uiBrainWash_Timer -=uiDiff;
	}	
};

CreatureAI* GetAI_boss_jindo(Creature* pCreature)
{
    return new boss_jindoAI(pCreature);
}

CreatureAI* GetAI_mob_healing_ward(Creature* pCreature)
{
    return new mob_healing_wardAI(pCreature);
}

CreatureAI* GetAI_mob_shade_of_jindo(Creature* pCreature)
{
    return new mob_shade_of_jindoAI(pCreature);
}

CreatureAI* GetAI_mob_brain_wash_totem(Creature* pCreature)
{
	return new mob_brain_wash_totemAI(pCreature);
}

void AddSC_boss_jindo()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_jindo";
    newscript->GetAI = &GetAI_boss_jindo;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_healing_ward";
    newscript->GetAI = &GetAI_mob_healing_ward;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shade_of_jindo";
    newscript->GetAI = &GetAI_mob_shade_of_jindo;
    newscript->RegisterSelf();

    newscript = new Script;
	newscript->Name = "mob_brain_wash_totem";
	newscript->GetAI = &GetAI_mob_brain_wash_totem;
	newscript->RegisterSelf();
}
