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
SDName: blood_prince_council
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    // yells
    // intro-> by lana'thel
    SAY_INTRO1              = -1609490,
    SAY_INTRO2              = -1609509,
    //keleseth
    SAY_KELESETH_BLOOD      = -1609491,
    SAY_KELESETH_SPECIAL    = -1609492,
    SAY_KELESETH_KILL1      = -1609493,
    SAY_KELESETH_KILL2      = -1609494,
    SAY_KELESETH_BERSERK    = -1609495,
    SAY_KELESETH_DEATH      = -1609496,
    //taldaram
    SAY_TALDARAM_BLOOD      = -1609497,
    SAY_TALDARAM_SPECIAL    = -1609498,
    SAY_TALDARAM_KILL1      = -1609499,
    SAY_TALDARAM_KILL2      = -1609500,
    SAY_TALDARAM_BERSERK    = -1609501,
    SAY_TALDARAM_DEATH      = -1609502,
    //valanar
    SAY_VALANAR_BLOOD       = -1609503,
    SAY_VALANAR_SPECIAL     = -1609504,
    SAY_VALANAR_KILL1       = -1609505,
    SAY_VALANAR_KILL2       = -1609506,
    SAY_VALANAR_BERSERK     = -1609507,
    SAY_VALANAR_DEATH       = -1609508,

    // spells
    //Darkfallen Orb
    SPELL_INVOCATION_OF_BLOOD               = 70952,

    //Valanar
    SPELL_KINETIC_BOMB                      = 72053,
    NPC_KINETIC_BOMB                        = 38458,
    SPELL_KINETIC_BOMB_EXPLODE              = 72052,
    SPELL_SHOCK_VORTEX                      = 72037,
    NPC_SHOCK_VORTEX                        = 38422,
    SPELL_SHOCK_VORTEX_DAMAGE               = 71944,
    SPELL_SHOCK_VORTEX_2                    = 72039,

    //Taldaram
    SPELL_GLITTERING_SPARKS                 = 71807,
    SPELL_CONJURE_FLAME                     = 71718,
    SPELL_FLAMES                            = 71393,
    SPELL_CONJURE_FLAME_2                   = 72040,
    SPELL_FLAMES_2                          = 71708,

    //Keleseth
    SPELL_SHADOW_LANCE                      = 71405,
    SPELL_SHADOW_LANCE_2                    = 71815,
    SPELL_SHADOW_RESONANCE                  = 71943,
    SPELL_SHADOW_RESONANCE_DAMAGE           = 71822,
    NPC_DARK_NUCLEUS                        = 38369,

	// custom
	//ALL
	SPELL_INVOCATION             = 70983,
    SPELL_AURA                   = 70934,
	SPELL_BERSERK                = 47008,
	
	//Valanar Spells
    //SPELL_KINETIC_BOMB           = 72053,
    SPELL_EMPOWERED_VORTEX       = 72039,
	SPELL_BOMB_VISUAL            = 72054,
	SPELL_BOMB_EXPLOTION         = 72052,
	SPELL_BOMB_UNSTABLE          = 72059,
	SPELL_VORTEX                 = 72037,
	SPELL_VORTEX_PERIODICAL      = 71945,

	//Keleseth Spells
	//SPELL_SHADOW_RESONANCE         = 71943,
	//SPELL_SHADOW_LANCE             = 71405,
	SPELL_EMPOWERED_SHADOW_LANCE   = 71815,

	//Taldaram Spells
	//SPELL_GLITTERING_SPARKS        = 71807,
	SPELL_CONJURE_FLAMES           = 71718,
	SPELL_CONJURE_EMPOWERED_FLAMES = 72040,

	MODEL_ID_INVISIBLE             = 11686,

	//Mobs

	//NPC_KINETIC_BOMB               = 38454,
	NPC_KINETIC_BOMB_TARGET        = 38458,
};

Creature* pValanar;
Creature* pTaldaram;
Creature* pKeleseth;
Creature* pBombTarget;


bool bInvocation_V = true;
bool bInvocation_T = false;
bool bInvocation_K = false;

struct MANGOS_DLL_DECL boss_prince_valanarAI : public ScriptedAI
{
    boss_prince_valanarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		pValanar = m_creature;
		Reset();
	}

	ScriptedInstance* m_pInstance;

	bool   m_bIsRegularMode;
    uint32 Kinetic_Bomb_Timer;
    uint32 Berserk_Timer;
	uint32 m_uiVortex_Timer;
	uint32 m_uiIvocation_Timer;
	uint32 m_uiEmpowered_Vortex_Timer;
	uint32 m_uiInvocation_Buffer;
	uint32 m_uiInvoc_Blood_Timer;
	uint32 m_uiHp_Timer;
	bool   m_bGetHealth;

	void Reset()
	{
		//bInvocation_V = true;
        //bInvocation_T = false;
        //bInvocation_K = false;
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		Berserk_Timer              = 10*MINUTE*IN_MILLISECONDS ;
		Kinetic_Bomb_Timer         = 15000;
		m_uiVortex_Timer           = 25000;
		m_uiEmpowered_Vortex_Timer = 30000;
		m_uiIvocation_Timer        = 100;
		m_uiInvoc_Blood_Timer      = 100;
		m_uiInvocation_Buffer      = 3;
		m_uiHp_Timer               = 1000;
	}

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->SetInCombatWithZone();
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_VALANAR_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_VALANAR_KILL2, m_creature); break;
        }
    }

	void JustReachedHome(){};

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		pKeleseth->SetInCombatWithZone();
		pTaldaram->SetInCombatWithZone();

		if(Berserk_Timer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_BERSERK);
			Berserk_Timer = 10*MINUTE*IN_MILLISECONDS;
		}
		else
			Berserk_Timer -= uiDiff;

		if(!bInvocation_V)
		{
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		
			if(m_uiVortex_Timer < uiDiff)
			{
				DoCast(m_creature->getVictim(),SPELL_VORTEX);
				m_uiVortex_Timer = 30000;
			}
			else 
				m_uiVortex_Timer -= uiDiff;
		}
		else if (bInvocation_V)
		{
	
			if(m_uiEmpowered_Vortex_Timer < uiDiff)
			{
				DoCast(m_creature->getVictim(),SPELL_EMPOWERED_VORTEX);
				m_uiEmpowered_Vortex_Timer = 25000;
			}
			m_uiEmpowered_Vortex_Timer -= uiDiff;
		}

		if(Kinetic_Bomb_Timer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				//DoCast(m_creature->getVictim(), SPELL_KINETIC_BOMB);
				pBombTarget = m_creature->SummonCreature(NPC_KINETIC_BOMB_TARGET,pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN, 60000);
				pBombTarget->SummonCreature(NPC_KINETIC_BOMB,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+25,0, TEMPSUMMON_TIMED_DESPAWN, 60000);
				pBombTarget->addUnitState(UNIT_STAT_ROOT);
			}
			Kinetic_Bomb_Timer = 27000;
		}
		else
			Kinetic_Bomb_Timer -= uiDiff;
		if(m_uiIvocation_Timer < uiDiff)
		{
			switch(m_uiInvocation_Buffer)
			{
			case 1:
				pTaldaram->SetHealth(m_creature->GetHealth());
				pTaldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->RemoveAllAuras();
				pTaldaram->CastSpell(pTaldaram,SPELL_INVOCATION,true,0,0,0);
				pTaldaram->CastSpell(pTaldaram,SPELL_AURA,true,0,0,0);
			    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				
				m_creature->SetHealth(1);
				bInvocation_V = false;
                bInvocation_T = true;
                bInvocation_K = false;
				m_uiInvocation_Buffer = 2;
				break;
			case 2:
				pKeleseth->SetHealth(pTaldaram->GetHealth());
				pKeleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pTaldaram->RemoveAllAuras();
				pKeleseth->CastSpell(pKeleseth,SPELL_INVOCATION,true,0,0,0);
				pKeleseth->CastSpell(pKeleseth,SPELL_AURA,true,0,0,0);
				pTaldaram->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pTaldaram->SetHealth(1);
				pTaldaram->RemoveAurasDueToSpell(SPELL_AURA);
				bInvocation_V = false;
                bInvocation_T = false;
                bInvocation_K = true;
				m_uiInvocation_Buffer = 3;
				break;
			case 3:
				m_creature->SetHealth(pKeleseth->GetHealth());
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pKeleseth->RemoveAllAuras();
				m_creature->CastSpell(m_creature,SPELL_INVOCATION,true,0,0,0);
				m_creature->CastSpell(m_creature,SPELL_AURA,true,0,0,0);
				pKeleseth->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pKeleseth->SetHealth(1);
				pTaldaram->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pTaldaram->SetHealth(1);
				bInvocation_V = true;
                bInvocation_T = false;
                bInvocation_K = false;
				m_uiInvocation_Buffer = 1;
				break;
			}
			m_uiIvocation_Timer = 47000;
		}
		else
			m_uiIvocation_Timer -= uiDiff;
        DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_valanar(Creature* pCreature)
{
    return new boss_prince_valanarAI(pCreature);
}

struct MANGOS_DLL_DECL boss_prince_taldaramAI : public ScriptedAI
{
    boss_prince_taldaramAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		pTaldaram = m_creature;
		Reset();
	}

	ScriptedInstance* m_pInstance;

	bool m_bIsRegularMode;
	uint32 m_uiGlittering_Sparks_Timer;
	uint32 m_uiConjure_Flames_Timer;
	uint32 m_uiConjure_Empowered_Flames_Timer;

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->SetInCombatWithZone();
	}

	void Reset()
	{
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_uiGlittering_Sparks_Timer        = 22000;
		m_uiConjure_Flames_Timer           = 20000;
		m_uiConjure_Empowered_Flames_Timer = 20000;
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_TALDARAM_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_TALDARAM_KILL2, m_creature); break;
        }
    }

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		pKeleseth->SetInCombatWithZone();
		pValanar->SetInCombatWithZone();

		if(!bInvocation_T)
		{
			if(m_uiConjure_Flames_Timer < uiDiff)
			{
				DoCast(m_creature->getVictim() , SPELL_CONJURE_FLAMES);
				m_uiConjure_Flames_Timer = 20000;
			}
			else 
				m_uiConjure_Flames_Timer -= uiDiff;
		}
		else if (bInvocation_T)
		{
			if(m_uiConjure_Empowered_Flames_Timer < uiDiff)
			{
				DoCast(m_creature->getVictim(), SPELL_CONJURE_EMPOWERED_FLAMES);
				m_uiConjure_Empowered_Flames_Timer = 20000;
			}
			else
				m_uiConjure_Empowered_Flames_Timer -= uiDiff;
		}

		if (m_uiGlittering_Sparks_Timer  < uiDiff)
		{
			DoCast(m_creature->getVictim(),SPELL_GLITTERING_SPARKS);
			m_uiGlittering_Sparks_Timer  = 22000;
		}
		else 
			m_uiGlittering_Sparks_Timer  -= uiDiff;
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_taldaram(Creature* pCreature)
{
    return new boss_prince_taldaramAI(pCreature);
}

struct MANGOS_DLL_DECL boss_prince_kelesethAI : public ScriptedAI
{
    boss_prince_kelesethAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
		pKeleseth = m_creature;
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 m_uiInvocation_Timer;
	uint32 m_uiHp_Timer;
	uint32 m_uiShadow_Lance;
	uint32 m_uiShadow_Resonance;
	uint32 m_uiEmpowered_Shadow_Lance;

	bool m_bIsRegularMode;

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->SetInCombatWithZone();
	}

	void Reset()
	{
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_uiShadow_Lance               = 10000;
        m_uiShadow_Resonance           = 20000;
     	m_uiEmpowered_Shadow_Lance     = 25000;
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_KELESETH_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_KELESETH_KILL2, m_creature); break;
        }
    }

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		pValanar->SetInCombatWithZone();
		pTaldaram->SetInCombatWithZone();

		if(!bInvocation_K)
		{
			if(m_uiShadow_Lance < uiDiff)
			{
				DoCast(m_creature->getVictim(),SPELL_SHADOW_LANCE);
				m_uiShadow_Lance = 10000;
			}
			else 
				m_uiShadow_Lance -= uiDiff;
		}

		if(m_uiShadow_Resonance < uiDiff)
		{
			DoCast(m_creature->getVictim(),SPELL_SHADOW_RESONANCE);
			m_uiShadow_Resonance = 20000;
		}
		else 
			m_uiShadow_Resonance -= uiDiff;

		if (bInvocation_K)
		{
			if(m_uiEmpowered_Shadow_Lance < uiDiff)
			{
				DoCast(m_creature->getVictim(), SPELL_EMPOWERED_SHADOW_LANCE);
				m_uiEmpowered_Shadow_Lance = 25000;
			}
			else 
				m_uiEmpowered_Shadow_Lance -= uiDiff;
		}
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_keleseth(Creature* pCreature)
{
    return new boss_prince_kelesethAI(pCreature);
}

/*
**********************************************
***************    VORTEX    *****************
**********************************************
*/

struct MANGOS_DLL_DECL VortexAI : public ScriptedAI
{
	VortexAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiVortex_Periodical;
	uint32 m_uiDespawn_Timer;

	void Reset()
	{
		m_creature->setFaction(14);
		m_uiVortex_Periodical = 100;
	    m_uiDespawn_Timer = 30000;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if(m_uiVortex_Periodical < uiDiff)
		{
			DoCast(m_creature,SPELL_VORTEX_PERIODICAL);
			m_uiVortex_Periodical = 31000;
		}
		else
			m_uiVortex_Periodical -= uiDiff;

		if(m_uiDespawn_Timer < uiDiff)
		{
			m_creature->ForcedDespawn();
			m_uiDespawn_Timer = 30000;
		}
		else
			m_uiDespawn_Timer -= uiDiff;
	}

};


CreatureAI* GetAI_Vortex(Creature* pCreature)
{
    return new VortexAI(pCreature);
}


/*
**********************************************
*************** KINETIC BOMB *****************
**********************************************
*/

struct MANGOS_DLL_DECL Kinetic_BombAI : public ScriptedAI
{
	Kinetic_BombAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}

	uint32 m_uiCheck_Timer;
	uint32 m_uiMove_Timer;
	uint32 m_uiBomb_Unstable;
	uint32 m_uiDespawn_Timer;

	void Reset()
	{
	    m_creature->HasSplineFlag(SPLINEFLAG_FLYING);
		m_creature->SetDisplayId(MODEL_ID_INVISIBLE);
		m_creature->SetHealth(100000);
		DoCast(m_creature,SPELL_BOMB_VISUAL);
		m_uiCheck_Timer = 200;
		m_uiBomb_Unstable = 500;
		m_uiMove_Timer  = 100;
		m_uiDespawn_Timer = 2000;
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > 1000 )
        {
            m_creature->SetHealth(100000);
            m_creature->NearTeleportTo(m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()+5,0,false);
        }
    }

	void UpdateAI(const uint32 uiDiff)
	{
		if(m_uiBomb_Unstable <uiDiff)
		{
			DoCast(m_creature,SPELL_BOMB_UNSTABLE);
			m_uiBomb_Unstable = 61000;
		}
		else
			m_uiBomb_Unstable -= uiDiff;
		if(m_uiMove_Timer < uiDiff)
		{
			m_creature->NearTeleportTo(m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ()-0.125f,false);
			m_uiMove_Timer = 100;
		}
		else 
			m_uiMove_Timer -= uiDiff;
		
		if (m_uiCheck_Timer < uiDiff)
        {
            if (m_creature->IsWithinDist3d(pBombTarget->GetPositionX(), pBombTarget->GetPositionY(), pBombTarget->GetPositionZ(),5.0f))
            {
				DoCast(m_creature->getVictim(),SPELL_BOMB_EXPLOTION);
				if(m_uiDespawn_Timer < uiDiff)
				{
					m_creature->ForcedDespawn();
					m_uiDespawn_Timer = 100;
				}
            }
           m_uiCheck_Timer = 1000;
        }
        else
			m_uiCheck_Timer -= uiDiff;
	}

};

CreatureAI* GetAI_Kinetic_Bomb(Creature* pCreature)
{
    return new Kinetic_BombAI(pCreature);
}

void AddSC_blood_prince_council()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_prince_taldaram";
    newscript->GetAI = &GetAI_boss_prince_taldaram;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_prince_keleseth";
    newscript->GetAI = &GetAI_boss_prince_keleseth;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_prince_valanar";
    newscript->GetAI = &GetAI_boss_prince_valanar;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_Shock_Vortex";
    newscript->GetAI = &GetAI_Vortex;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_kinetic_bomb";
    newscript->GetAI = &GetAI_Kinetic_Bomb;
    newscript->RegisterSelf();
}
