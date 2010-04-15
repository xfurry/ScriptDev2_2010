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
SDName: Boss_Hakkar
SD%Complete: 75
SDComment: Charming effects not working, hakkar also should have 719k HP when all high priests are dead, enter evademode when downstairs
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SAY_AGGRO               =   -1309020,
    SAY_FLEEING             =   -1309021,
    SPELL_BLOOD_SIPHON_1    =   24324,
    SPELL_BLOOD_SIPHON_2    =   24322,
    SPELL_CORRUPTED_BLOOD   =   24328,
    SPELL_CAUSE_INSANITY    =   24327,
    SPELL_WILL_OF_HAKKAR    =   24178,
    SPELL_ENRAGE            =   24318,
 
    // The Aspects of all High Priests
    SPELL_ASPECT_OF_JEKLIK  =   24687,
    SPELL_ASPECT_OF_VENOXIS =   24688,
    SPELL_ASPECT_OF_MARLI   =   24686,
    SPELL_ASPECT_OF_THEKAL  =   24689,
    SPELL_ASPECT_OF_ARLOKK  =   24690,
    
    //Son spell and siphon debuff
    SPELL_POISONOUS_BLOOD   =   24321,
    SPELL_BLOOD_SIPHON_3    =   24323,
    SPELL_KNOCKDOWN         =   16790
};
 
class MANGOS_DLL_DECL SiphonAura : public Aura
{
    public:
        SiphonAura(SpellEntry* pSpellEntry, SpellEffectIndex uiEff, int32 *bp, Unit* pTarget, Unit* pCaster) : Aura(pSpellEntry, uiEff, bp, pTarget, pCaster, NULL)
            {}
};

struct MANGOS_DLL_DECL boss_hakkarAI : public ScriptedAI
{
    boss_hakkarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiBloodSiphon_Timer;
    uint32 m_uiCorruptedBlood_Timer;
    uint32 m_uiCauseInsanity_Timer;
    uint32 m_uiWillOfHakkar_Timer;
    uint32 m_uiEnrage_Timer;

    uint32 m_uiAspectOfJeklik_Timer;
    uint32 m_uiAspectOfVenoxis_Timer;
    uint32 m_uiAspectOfMarli_Timer;
    uint32 m_uiAspectOfThekal_Timer;
    uint32 m_uiAspectOfArlokk_Timer;

    bool m_bJeklikIsDead;
    bool m_bVenoxisIsDead;
    bool m_bMarliIsDead;
    bool m_bThekalIsDead;
    bool m_bArlokkIsDead;
 
    bool m_bAllDead;
 
    bool m_bEnraged;

    void Reset()
    {
        m_uiBloodSiphon_Timer = 90000;
        m_uiCorruptedBlood_Timer = 25000;
        m_uiCauseInsanity_Timer = 17000;
        m_uiWillOfHakkar_Timer = 17000;
        m_uiEnrage_Timer = 600000;
        m_uiAspectOfJeklik_Timer = 4000;
        m_uiAspectOfVenoxis_Timer = 7000;
        m_uiAspectOfMarli_Timer = 12000;
        m_uiAspectOfThekal_Timer = 8000;
        m_uiAspectOfArlokk_Timer = 18000;

        m_bJeklikIsDead = false;
        m_bVenoxisIsDead = false;
        m_bMarliIsDead = false;
        m_bThekalIsDead = false;
        m_bArlokkIsDead = false;
        
        m_bAllDead = false;
        m_bEnraged = false;
    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
     {
         if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
             return;
 
        // check the highpriests whether they are dead or not
        // check only, when not all bosses are dead -> reduce checks!
        if (!m_bAllDead && m_pInstance)
         {
            if (!m_bJeklikIsDead && m_pInstance->GetData(TYPE_JEKLIK) == DONE)
                m_bJeklikIsDead = true;
 
            if (!m_bVenoxisIsDead && m_pInstance->GetData(TYPE_VENOXIS) == DONE)
                m_bVenoxisIsDead = true;

            if (!m_bMarliIsDead && m_pInstance->GetData(TYPE_MARLI) == DONE)
                m_bMarliIsDead = true;

            if (!m_bThekalIsDead && m_pInstance->GetData(TYPE_THEKAL) == DONE)
                m_bThekalIsDead = true;

            if (!m_bArlokkIsDead && m_pInstance->GetData(TYPE_ARLOKK) == DONE)
                m_bArlokkIsDead = true;
            
            if (m_bJeklikIsDead && m_bVenoxisIsDead && m_bMarliIsDead && m_bThekalIsDead && m_bArlokkIsDead)
                m_bAllDead = true;
        }

        if (m_uiBloodSiphon_Timer < uiDiff)
         {
            DoCast(m_creature->getVictim(),SPELL_BLOOD_SIPHON_2);			
            ThreatList const& m_threatlist = m_creature->getThreatManager().getThreatList();
            if(!m_threatlist.empty())
            {
                ThreatList::const_iterator i = m_threatlist.begin();
                for (i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
                {
                    if (Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                    {
                        SpellEntry* pSpellEntry = (SpellEntry *)GetSpellStore()->LookupEntry(SPELL_BLOOD_SIPHON_1);
                        if(pUnit->HasAura(SPELL_POISONOUS_BLOOD,EFFECT_INDEX_0))
                        {
                            //DoT on hakkar
                            for (uint8 i = 0; i < 3; ++i)
                            {
                                if (!pSpellEntry->Effect[i])
                                    continue;
                                m_creature->AddAura(new SiphonAura(pSpellEntry, EFFECT_INDEX_0, NULL, m_creature, m_creature));
                            }
                        }
                        else
                        {
                            //DoT on the player
                            for (uint8 i = 0; i < 3; ++i)
                            {
                                if (!pSpellEntry->Effect[i])
                                    continue;
                                pUnit->AddAura(new SiphonAura(pSpellEntry, EFFECT_INDEX_0, NULL, pUnit, pUnit));
                            }
                            //and hakkar heals himself (1000 per sec on 8 sec = 8000hp)
                            m_creature->SetHealth(m_creature->GetHealth()+8000);
                        }	
                    }
                }
            }
            m_uiBloodSiphon_Timer = 90000;
        }
        else
            m_uiBloodSiphon_Timer -= uiDiff;
 
        if (m_uiCorruptedBlood_Timer < uiDiff)
         {
            DoCast(m_creature->getVictim(),SPELL_CORRUPTED_BLOOD);
            m_uiCorruptedBlood_Timer = 30000 + rand()%15000;
        }
        else
            m_uiCorruptedBlood_Timer -= uiDiff;
 
        if (m_uiCauseInsanity_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_CAUSE_INSANITY);
 
            m_uiCauseInsanity_Timer = 35000 + rand()%8000;
        }
        else
            m_uiCauseInsanity_Timer -= uiDiff;

        if (m_uiWillOfHakkar_Timer < uiDiff)
         {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_WILL_OF_HAKKAR);
 
            m_uiWillOfHakkar_Timer = 25000 + rand()%10000;
        }
        else
            m_uiWillOfHakkar_Timer -= uiDiff;
 
        if (!m_bEnraged && m_uiEnrage_Timer < uiDiff)
         {
             DoCast(m_creature, SPELL_ENRAGE);
            m_bEnraged = true;
        }
        else
            m_uiEnrage_Timer -= uiDiff;
        
        // check only, when not all bosses are dead -> reduce checks!
        if (!m_bAllDead)
         {
            // when Jeklik is alive, Hakkar casts his aspect
            if (!m_bJeklikIsDead && m_uiAspectOfJeklik_Timer < uiDiff)
             {
                DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_JEKLIK);

                m_uiAspectOfJeklik_Timer = 10000 + rand()%4000;
             }
            else if (!m_bJeklikIsDead && m_uiAspectOfJeklik_Timer >= uiDiff)
                m_uiAspectOfJeklik_Timer -= uiDiff;
            
            // when Venoxis is alive, hakkar casts his aspect
            if (!m_bVenoxisIsDead && m_uiAspectOfVenoxis_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_VENOXIS);
 
                m_uiAspectOfVenoxis_Timer = 8000;
             }
            else if (!m_bVenoxisIsDead && m_uiAspectOfVenoxis_Timer >= uiDiff)
                m_uiAspectOfVenoxis_Timer -= uiDiff;    
 
            // when Marli is alive, hakkar casts his aspect
            if (!m_bMarliIsDead && m_uiAspectOfMarli_Timer < uiDiff)
             {
                DoCast(m_creature->getVictim(),SPELL_ASPECT_OF_MARLI);
 
                m_uiAspectOfMarli_Timer = 10000;
             }
            else if (!m_bMarliIsDead && m_uiAspectOfMarli_Timer >= uiDiff)
                m_uiAspectOfMarli_Timer -= uiDiff;
 
            // when Thekal is alive, hakkar casts his aspect
            if (!m_bThekalIsDead && m_uiAspectOfThekal_Timer < uiDiff)
             {
                DoCast(m_creature,SPELL_ASPECT_OF_THEKAL);
                m_uiAspectOfThekal_Timer = 15000;
             }
            else if (!m_bThekalIsDead && m_uiAspectOfThekal_Timer >= uiDiff)
                m_uiAspectOfThekal_Timer -= uiDiff;
            
            // when Arlokk is alive, hakkar casts his aspect
            if (!m_bArlokkIsDead && m_uiAspectOfArlokk_Timer < uiDiff)
             {
                DoCast(m_creature,SPELL_ASPECT_OF_ARLOKK);
                DoResetThreat();
                m_uiAspectOfArlokk_Timer = 10000 + rand()%5000;
             }
            else if (!m_bArlokkIsDead && m_uiAspectOfArlokk_Timer >= uiDiff)
                m_uiAspectOfArlokk_Timer -= uiDiff;
        }
        
        DoMeleeAttackIfReady();
    }
};
 
struct MANGOS_DLL_DECL mob_sonofhakkarAI : public ScriptedAI
{
    mob_sonofhakkarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Reset();
	}
	ScriptedInstance* m_pInstance;

	uint32 m_uiKnockdown_Timer;

    void Reset()
    {
        m_uiKnockdown_Timer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
		
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiKnockdown_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_KNOCKDOWN);
            m_uiKnockdown_Timer = 8000 + rand()%6000;
        }
		else 
			m_uiKnockdown_Timer -= diff;
			
		if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 10)
        {
            ThreatList const& m_threatlist = m_creature->getThreatManager().getThreatList();
            if(!m_threatlist.empty())
            {
                ThreatList::const_iterator i = m_threatlist.begin();
                for (i = m_threatlist.begin(); i != m_threatlist.end(); ++i)
                {
                    if (Unit* pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid()))
                        pUnit->CastSpell(pUnit,SPELL_POISONOUS_BLOOD,1);
                }
            }
		}
        DoMeleeAttackIfReady();
   }
};

CreatureAI* GetAI_boss_hakkar(Creature* pCreature)
{
    return new boss_hakkarAI(pCreature);
}

CreatureAI* GetAI_mob_sonofhakkar(Creature* pCreature)
{
    return new mob_sonofhakkarAI(pCreature);
}

void AddSC_boss_hakkar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hakkar";
    newscript->GetAI = &GetAI_boss_hakkar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sonofhakkar";
    newscript->GetAI = &GetAI_mob_sonofhakkar;
    newscript->RegisterSelf();
}
