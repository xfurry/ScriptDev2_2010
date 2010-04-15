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
SDName: Boss_Thekal
SD%Complete: 100
SDComment: Fakedying copied from scarlet monastery, working well
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

enum
{
    SAY_AGGRO               =   -1309009,
    SAY_DEATH               =   -1309010,

    SPELL_MORTAL_CLEAVE     =   22859,
    SPELL_SILENCE           =   22666,
    SPELL_BLOODLUST         =   24185,
    SPELL_FORCE_PUNCH       =   24189,
    SPELL_CHARGE            =   24408,
    SPELL_ENRAGE            =   8269,
    SPELL_SUMMON_TIGERS     =   24183,
    SPELL_TIGER_FORM        =   24169,
    SPELL_RESURRECT         =   24173,  //Hacked.	
    SPELL_SPEED_SLASH		=	24192,

    //Zealot Lor'Khan Spells
    SPELL_SHIELD            =   20545,    
    SPELL_DISPEL_MAGIC		=	17201,
    SPELL_GREATER_HEAL      =   24208,
    SPELL_DISARM            =   6713,	

    //Zealot Lor'Khan Spells
    SPELL_SWEEPING_STRIKES  =   18765,
    SPELL_SINISTER_STRIKE   =   15667,
    SPELL_GOUGE             =   24698,
    SPELL_KICK              =   15614,
    SPELL_BLIND             =   21060
};

struct MANGOS_DLL_DECL boss_thekalAI : public ScriptedAI
{
    boss_thekalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        fDefaultSize = m_creature->GetFloatValue(OBJECT_FIELD_SCALE_X);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiMortalCleave_Timer;
    uint32 m_uiSilence_Timer;
    uint32 m_uiFrenzy_Timer;
    uint32 m_uiForcePunch_Timer;
    uint32 m_uiCharge_Timer;
    uint32 m_uiEnrage_Timer;
    uint32 m_uiSummonTigers_Timer;    
    uint32 m_uiResurrectSelf_Timer;
    uint32 m_uiCast_Timer;
    uint32 m_uiSpeedSlash_Timer;

    bool m_bEnraged;
    bool m_bPhaseTwo;    
    bool m_bRess;

    float fDefaultSize;

    void Reset()
    {
        m_uiMortalCleave_Timer = 4000;
        m_uiSilence_Timer = 9000;
        m_uiFrenzy_Timer = 30000;
        m_uiForcePunch_Timer = 4000;
        m_uiCharge_Timer = 25000;
        m_uiEnrage_Timer = 32000;
        m_uiSummonTigers_Timer = 52000;        
        m_uiResurrectSelf_Timer = 10000;
        m_uiSpeedSlash_Timer = 45000;

        m_bEnraged = false;
        m_bPhaseTwo = false;
        m_bRess = false;

        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, fDefaultSize);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (Creature* pZath = (Creature*)Creature::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ZATH)))
        {
            if(!pZath->isAlive())
                pZath->Respawn();
        }

        if (Creature* pLorKhan = (Creature*)Creature::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LORKHAN)))
        {
            if(!pLorKhan->isAlive())
                pLorKhan->Respawn();
        }

    }

    void Aggro(Unit *pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, IN_PROGRESS);

        if (Creature* pZath = (Creature*)Creature::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ZATH)))
        {
            if(m_creature->getVictim())
                pZath->AI()->AttackStart(m_creature->getVictim());
        }

        if (Creature* pLorKhan = (Creature*)Creature::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LORKHAN)))
        {
            if(m_creature->getVictim())
                pLorKhan->AI()->AttackStart(m_creature->getVictim());
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, NOT_STARTED);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage < m_creature->GetHealth())
            return;

        if (!m_pInstance)
            return;
        if (m_bPhaseTwo)
            return;

        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveIdle();

        m_creature->SetHealth(0);

        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);

        m_creature->ClearComboPointHolders();
        m_creature->RemoveAllAuras();
        m_creature->ClearAllReactives();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
        m_creature->AttackStop();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THEKAL, SPECIAL);

        uiDamage = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance && m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
        {
            if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL && m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
            {
                //Thekal will transform to Tiger if he died and was not resurrected after 10 seconds.
                if (!m_bPhaseTwo && m_uiResurrectSelf_Timer < uiDiff)
                {
                    DoCast(m_creature,SPELL_TIGER_FORM);
                    m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, fDefaultSize*2);
                    m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetHealth(m_creature->GetMaxHealth());

                    const CreatureInfo *cinfo = m_creature->GetCreatureInfo();
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, (cinfo->mindmg +((cinfo->mindmg/100) * 40)));
                    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, (cinfo->maxdmg +((cinfo->maxdmg/100) * 40)));
                    m_creature->UpdateDamagePhysical(BASE_ATTACK);

                    if (m_creature->getVictim())
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    DoResetThreat();
                    DoScriptText(SAY_AGGRO, m_creature);
                    m_bPhaseTwo = true;
                }
                else
                    m_uiResurrectSelf_Timer -= uiDiff;

                if (m_bPhaseTwo)
                {
                    if (m_uiCharge_Timer < uiDiff)
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                        {
                            DoCast(pTarget,SPELL_CHARGE);
                            DoResetThreat();
                            AttackStart(pTarget);
                        }
                        m_uiCharge_Timer = 15000 + rand()%15000;
                    }
                    else
                        m_uiCharge_Timer -= uiDiff;

                    if (m_uiSpeedSlash_Timer < uiDiff)
                    {
                        DoCast(m_creature->getVictim(), SPELL_SPEED_SLASH);
                        m_uiSpeedSlash_Timer = 20000 + rand()%10000;
                    }
                    else
                        m_uiSpeedSlash_Timer -= uiDiff;

                    if (m_uiFrenzy_Timer < uiDiff)
                    {
                        DoCast(m_creature,SPELL_BLOODLUST);
                        m_uiFrenzy_Timer = 50000 + rand()%20000;
                    }
                    else
                        m_uiFrenzy_Timer -= uiDiff;

                    if (m_uiForcePunch_Timer < uiDiff)
                    {
                        DoCast(m_creature->getVictim(),SPELL_FORCE_PUNCH);
                        m_uiForcePunch_Timer = 25000 + rand()%5000;
                    }
                    else
                        m_uiForcePunch_Timer -= uiDiff;

                    if (m_uiSummonTigers_Timer < uiDiff)
                    {	
                        DoCast(m_creature->getVictim(),SPELL_SUMMON_TIGERS);
                        m_uiSummonTigers_Timer = 52000;
                    }
                    else
                        m_uiSummonTigers_Timer -= uiDiff;

                    if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 11) && !m_bEnraged)
                    {
                        DoCast(m_creature, SPELL_ENRAGE);
                        m_bEnraged = true;
                    }
                }
                else
                    return;			
            }
            else
                return;
        }
        else
        {
            //Check for the death of LorKhan and Zath.
            if (!m_bRess && !m_bPhaseTwo)
            {
                if (m_pInstance)
                {
                    if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL || m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                    {
                        m_bRess = true;
                        m_uiCast_Timer = 12000;    
                    }
                }          
            }
            //Cast Resurrection Visual
            if (m_bRess && !m_bPhaseTwo && m_uiCast_Timer <= 2100)
            {
                if (m_pInstance)
                {
                    Unit* pLorKhan = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LORKHAN));
                    Unit* pZath = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ZATH));

                    if (pLorKhan && m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                        DoCast(pLorKhan,SPELL_RESURRECT);
                    if (pZath && m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
                        DoCast(pZath,SPELL_RESURRECT);
                }
            }

            if (m_bRess && !m_bPhaseTwo && m_uiCast_Timer < uiDiff)
            {	
                if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                {
                    //Resurrect LorKhan
                    if (Unit* pLorKhan = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LORKHAN)))
                    {
                        pLorKhan->setFaction(28);
                        pLorKhan->SetStandState(UNIT_STAND_STATE_STAND);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pLorKhan->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pLorKhan->SetHealth(pLorKhan->GetMaxHealth());

                        if (pLorKhan->getVictim())
                            pLorKhan->GetMotionMaster()->MoveChase(pLorKhan->getVictim());
                        pLorKhan->Attack(pLorKhan->getVictim(),true);

                        m_pInstance->SetData(TYPE_LORKHAN, DONE);
                    }
                }
                else
                {
                    //Resurrect Zath
                    if (Creature* pZath = (Creature*)Creature::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ZATH)))
                    {
                        pZath->setFaction(28);
                        pZath->SetStandState(UNIT_STAND_STATE_STAND);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pZath->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pZath->SetHealth(int(pZath->GetMaxHealth()*1.0));

                        if (pZath->getVictim())
                            pZath->GetMotionMaster()->MoveChase(pZath->getVictim());
                        pZath->Attack(pZath->getVictim(),true);

                        m_pInstance->SetData(TYPE_ZATH, DONE);
                    }
                }
                m_bRess = false;	
            }
            else
                m_uiCast_Timer -= uiDiff ;

            if (!m_bPhaseTwo && m_uiMortalCleave_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_MORTAL_CLEAVE);
                m_uiMortalCleave_Timer = 15000 + rand()%5000;
            }
            else
                m_uiMortalCleave_Timer -= uiDiff;

            if (!m_bPhaseTwo && m_uiSilence_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(),SPELL_SILENCE);
                m_uiSilence_Timer = 20000 + rand()%5000;
            }
            else
                m_uiSilence_Timer -= uiDiff;
        }
        DoMeleeAttackIfReady();
    }
};

//Zealot Lor'Khan
struct MANGOS_DLL_DECL mob_zealot_lorkhanAI : public ScriptedAI
{
    mob_zealot_lorkhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    uint32 m_uiShield_Timer;
    uint32 m_uiDispelMagic_Timer;
    uint32 m_uiGreaterHeal_Timer;
    uint32 m_uiDisarm_Timer;
	uint32 m_uiCast_Timer;
    
	bool m_bRess;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        m_uiShield_Timer = 1000;        
        m_uiGreaterHeal_Timer = 32000;
        m_uiDisarm_Timer = 6000;
        m_uiDispelMagic_Timer = 10000;
 
        m_bRess = false;
 
         if (m_pInstance)
            m_pInstance->SetData(TYPE_LORKHAN, NOT_STARTED);		
 
         m_creature->SetStandState(UNIT_STAND_STATE_STAND);
         m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		 m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage < m_creature->GetHealth())
            return;

        if (!m_pInstance)
            return;

        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveIdle();

        m_creature->SetHealth(0);

        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);

        m_creature->ClearComboPointHolders();
        m_creature->RemoveAllAuras();
        m_creature->ClearAllReactives();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LORKHAN, SPECIAL);

        uiDamage = 0;
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance->GetData(TYPE_THEKAL) == DONE)
            m_creature->setDeathState(CORPSE);			//die when finished

        if (m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
            return;

        if (m_uiShield_Timer < uiDiff)
         {
             DoCast(m_creature,SPELL_SHIELD);
            m_uiShield_Timer = 61000;
        }
        else
            m_uiShield_Timer -= uiDiff;
 
        if (m_uiDispelMagic_Timer < uiDiff)
         {
            DoCast(m_creature,SPELL_DISPEL_MAGIC);
            m_uiDispelMagic_Timer = 10000;
        }
        else
            m_uiDispelMagic_Timer -= uiDiff;
 
         //Casting Greaterheal to Thekal or Zath if they are in meele range.
        if (m_uiGreaterHeal_Timer < uiDiff)
         {
            if (Unit* pHealTarget = DoSelectLowestHpFriendly(ATTACK_DISTANCE))
			    DoCast(pHealTarget,SPELL_GREATER_HEAL);
 
            m_uiGreaterHeal_Timer = 15000 + rand()%5000;
        }
        else
            m_uiGreaterHeal_Timer -= uiDiff;
 
        if (m_uiDisarm_Timer < uiDiff)
         {
             DoCast(m_creature->getVictim(),SPELL_DISARM);
            m_uiDisarm_Timer = 15000+rand()%10000;
        }
        else
            m_uiDisarm_Timer -= uiDiff;
 
        if (!m_bRess)
         {
             if (m_pInstance)
             {
                 if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                 {
                    m_bRess = true;
                    m_uiCast_Timer = 12000;    
                 }
            }          
        }
		//Cast resurrect visual
        if (m_bRess && m_uiCast_Timer <= 2100)
        {
            if (Unit* pThekal = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_THEKAL)))
                DoCast(pThekal,SPELL_RESURRECT);			
        }
 
        if (m_bRess && m_uiCast_Timer < uiDiff)
        {	
            if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
                //Resurrect Thekal
                if (Unit *pThekal = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_THEKAL)))
                {
                    pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                    pThekal->setFaction(28);
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pThekal->Attack(pThekal->getVictim(),true);
 
                    if (pThekal->getVictim())
                        pThekal->GetMotionMaster()->MoveChase(pThekal->getVictim());
 
                    m_pInstance->SetData(TYPE_THEKAL,IN_PROGRESS);
                }
 
                m_bRess = false;	
        }else m_uiCast_Timer -= uiDiff ;

        DoMeleeAttackIfReady();
    }
};

//Zealot Zath
struct MANGOS_DLL_DECL mob_zealot_zathAI : public ScriptedAI
{
    mob_zealot_zathAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    uint32 m_uiSweepingStrikes_Timer;
    uint32 m_uiSinisterStrike_Timer;
    uint32 m_uiGouge_Timer;
    uint32 m_uiKick_Timer;
    uint32 m_uiBlind_Timer;
    uint32 m_uiCast_Timer;
    
    bool m_bRess;

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        m_uiSweepingStrikes_Timer = 13000;
        m_uiSinisterStrike_Timer = 8000;
        m_uiGouge_Timer = 25000;
        m_uiKick_Timer = 18000;
        m_uiBlind_Timer = 5000;        
        
        m_bRess = false;
 
         if (m_pInstance)
             m_pInstance->SetData(TYPE_ZATH, NOT_STARTED);
 
         m_creature->SetStandState(UNIT_STAND_STATE_STAND);
         m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage < m_creature->GetHealth())
             return;
 
        if (!m_pInstance)
            return;
 
        m_creature->GetMotionMaster()->MovementExpired();
        m_creature->GetMotionMaster()->MoveIdle();
 
        m_creature->SetHealth(0);
 
        if (m_creature->IsNonMeleeSpellCasted(false))
            m_creature->InterruptNonMeleeSpells(false);
 
        m_creature->ClearComboPointHolders();
        m_creature->RemoveAllAuras();
        m_creature->ClearAllReactives();
 
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
 
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZATH, SPECIAL);
 
        uiDamage = 0;
    }

    void UpdateAI (const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance->GetData(TYPE_THEKAL) == DONE)
            m_creature->setDeathState(CORPSE);				//die when finished

        if (m_pInstance->GetData(TYPE_ZATH) == SPECIAL)
            return;

        if (!m_bRess)
         {
             if (m_pInstance)
             {
                if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL && m_pInstance->GetData(TYPE_LORKHAN) == SPECIAL)
                 {
                    m_bRess = true;
                    m_uiCast_Timer = 12000;    
                 }
            }          
        }
        //resurrect visual
        if (m_bRess && m_uiCast_Timer <= 2100)
        {
            Unit *pThekal = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_THEKAL));
            DoCast(pThekal,SPELL_RESURRECT);			
        }
 
        if (m_bRess && m_uiCast_Timer < uiDiff)
        {	
            if (m_pInstance->GetData(TYPE_THEKAL) == SPECIAL)
            {
                //Resurrect Thekal if Zath and Lor'khan are dead
                if (Unit *pThekal = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_THEKAL)))
                {   
                    pThekal->setFaction(28);
                    pThekal->SetStandState(UNIT_STAND_STATE_STAND);
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pThekal->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pThekal->SetHealth(int(pThekal->GetMaxHealth()*1.0));

                    if (pThekal->getVictim())
                     {
                        pThekal->GetMotionMaster()->MoveChase(pThekal->getVictim());
                        pThekal->Attack(pThekal->getVictim(),true);
                    }	

                    m_pInstance->SetData(TYPE_THEKAL,IN_PROGRESS);
                 }
             }
            m_bRess = false;	
        }
        else
            m_uiCast_Timer -= uiDiff ;
 
        if (m_uiSweepingStrikes_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_SWEEPING_STRIKES);
            m_uiSweepingStrikes_Timer = 22000+rand()%4000;
        }
        else
            m_uiSweepingStrikes_Timer -= uiDiff;
 
        if (m_uiSinisterStrike_Timer < uiDiff)
         {
            DoCast(m_creature->getVictim(),SPELL_SINISTER_STRIKE);
            m_uiSinisterStrike_Timer = 8000+rand()%8000;
        }
        else
            m_uiSinisterStrike_Timer -= uiDiff;
 
        if (m_uiGouge_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_GOUGE);
 
            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-100);

            m_uiGouge_Timer = 17000+rand()%10000;
         }
        else
            m_uiGouge_Timer -= uiDiff;
 
        if (m_uiKick_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_KICK);
            m_uiKick_Timer = 15000+rand()%10000;
        }
        else
            m_uiKick_Timer -= uiDiff;

        if (m_uiBlind_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_BLIND);
            m_uiBlind_Timer = 10000+rand()%10000;
        }
        else
            m_uiBlind_Timer -= uiDiff;        

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thekal(Creature* pCreature)
{
    return new boss_thekalAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_lorkhan(Creature* pCreature)
{
    return new mob_zealot_lorkhanAI(pCreature);
}

CreatureAI* GetAI_mob_zealot_zath(Creature* pCreature)
{
    return new mob_zealot_zathAI(pCreature);
}

void AddSC_boss_thekal()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_thekal";
    newscript->GetAI = &GetAI_boss_thekal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zealot_lorkhan";
    newscript->GetAI = &GetAI_mob_zealot_lorkhan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_zealot_zath";
    newscript->GetAI = &GetAI_mob_zealot_zath;
    newscript->RegisterSelf();
}
