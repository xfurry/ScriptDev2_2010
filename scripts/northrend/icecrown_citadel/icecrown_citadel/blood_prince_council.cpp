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
	SPELL_INVOCATION_KELESETH				= 70981,
	SPELL_INVOCATION_VALANAR				= 70952,
	SPELL_INVOCATION_TALDARAM				= 70982,
	SPELL_INVOCATION_SPHERE					= 70934,	// used as a visual
	SPELL_INVOCATION_LINKS					= 71596,
	NPC_BLOOD_ORB_CONTROLLER				= 38008,
	SPELL_SHADOW_PRISON						= 72998,	// on heroic only

    //Valanar
    SPELL_KINETIC_BOMB                      = 72053,	// summon target
	NPC_KINETIC_BOMB_TARGET					= 38458,
	SPELL_KINETIC_BOMB_SUMMON				= 72080,	// summon bomb
    NPC_KINETIC_BOMB                        = 38458,	// needs advanced script
	SPELL_KINETIC_BOMB_VISUAL				= 72054,
	SPELL_KINETIC_BOMB_UNSTABLE				= 72059,
    SPELL_KINETIC_BOMB_EXPLODE              = 72052,	// used when reached the target
    SPELL_SHOCK_VORTEX                      = 72037,	// summon
    NPC_SHOCK_VORTEX                        = 38422,
    SPELL_SHOCK_VORTEX_DAMAGE               = 71945,	// shock vortex
    SPELL_EMPOWERED_SHOCK_VORTEX            = 72039,

    //Taldaram
    SPELL_GLITTERING_SPARKS                 = 71807,
    SPELL_CONJURE_FLAME                     = 71718,
    SPELL_FLAMES                            = 71393,
    SPELL_CONJURE_EMPOWERED_FLAME           = 72040,
    SPELL_EMPOWERED_FLAMES                  = 71709,	// periodic damage
	SPELL_FLAMES_VISUAL						= 55928,	// NEEDS FIX!!!
	NPC_CONJURED_FLAME						= 38332,	// needs advanced script
	NPC_EMPOWERED_CONJURED_FLAME			= 38451,	// needs advanced script

    //Keleseth
    SPELL_SHADOW_LANCE                      = 71405,
    SPELL_EMPOWERED_SHADOW_LANCE            = 71815,
    SPELL_SHADOW_RESONANCE                  = 71943,	// summon
    SPELL_SHADOW_RESONANCE_AURA	            = 71911,	//71822
    NPC_DARK_NUCLEUS                        = 38369,

	SPELL_BERSERK							= 26662,
};

enum phases
{
	PHASE_IDLE		= 0, 
	PHASE_VALANAR	= 1,
	PHASE_TALDARAM	= 2,
	PHASE_KELESETH	= 3,
};

// conjured flame
struct MANGOS_DLL_DECL mob_conjured_flameAI : public ScriptedAI
{
	mob_conjured_flameAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pCreature->SetDisplayId(11686);     // make invisible
		pCreature->setFaction(14);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance *m_pInstance;

	uint32 m_uiDieTimer;
	bool m_bMustDie;

	void Reset()
	{
	    m_bMustDie = false;
		if(m_creature->GetEntry() == NPC_EMPOWERED_CONJURED_FLAME)
			DoCast(m_creature, SPELL_EMPOWERED_FLAMES);
	}

	void SetDestination(float posX, float posY, float posZ)
	{
		m_creature->GetMotionMaster()->MovePoint(0, posX, posY, posZ);
	}

	void MovementInform(uint32 uiType, uint32 uiPointId)
	{
		if(uiType != POINT_MOTION_TYPE)
			return;

		if(uiPointId == 0)
		{
			DoCast(m_creature, SPELL_FLAMES);
			m_uiDieTimer = 1000;
			m_bMustDie = true;
		}
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(!m_creature->HasAura(SPELL_FLAMES_VISUAL, EFFECT_INDEX_0))
			DoCast(m_creature, SPELL_FLAMES_VISUAL);

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;
	}

};

CreatureAI* GetAI_mob_conjured_flame(Creature* pCreature)
{
    return new mob_conjured_flameAI(pCreature);
}

// kinetic bomb
struct MANGOS_DLL_DECL mob_kinetic_bombAI : public ScriptedAI
{
	mob_kinetic_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetDisplayId(11686);     // make invisible
		pCreature->setFaction(14);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiDieTimer;
	bool m_bMustDie;
	bool m_bIsFalling;
	float m_fSpeedRate;
	float m_fTargetX;
	float m_fTargetY;
	float m_fTargetZ;

	void Reset()
	{
		DoCast(m_creature,SPELL_KINETIC_BOMB_VISUAL);
		m_bMustDie = false;
		m_bIsFalling = false;
		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
			m_fSpeedRate = 1.0f;
		else
			m_fSpeedRate = 0.5f;
		m_creature->SetSpeedRate(MOVE_WALK, m_fSpeedRate);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > 0)
        {
			uiDamage = 0;
			m_bIsFalling = false;
			m_creature->NearTeleportTo(m_creature->GetPositionX(), m_creature->GetPositionY(), 391.0f, 0, false);
			m_creature->GetMap()->CreatureRelocation(m_creature, m_fTargetX, m_fTargetY, m_fTargetZ + 31, 0);
			m_creature->SendMonsterMove(m_fTargetX, m_fTargetY, m_fTargetZ + 31, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
		}
    }

	void SetDestination(float posX, float posY, float posZ)
	{
		m_creature->GetMotionMaster()->MovePoint(0, posX, posY, posZ);
		m_fTargetX = posX;
		m_fTargetY = posY;
		m_fTargetZ = posZ;
		m_bIsFalling = true;
	}

	void MovementInform(uint32 uiType, uint32 uiPointId)
	{
		if(uiType != POINT_MOTION_TYPE)
			return;

		if(uiPointId == 0)
		{
			DoCast(m_creature, SPELL_KINETIC_BOMB_EXPLODE);
			m_uiDieTimer = 1000;
			m_bMustDie = true;
		}
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_creature->GetPositionZ() > 390 && !m_bIsFalling)
		{
			m_creature->GetMotionMaster()->MovePoint(0, m_fTargetX, m_fTargetY, m_fTargetZ);
			m_creature->SetSpeedRate(MOVE_WALK, m_fSpeedRate);
			m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
			m_bIsFalling = true;
		}

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;
	}
};

CreatureAI* GetAI_mob_kinetic_bomb(Creature* pCreature)
{
    return new mob_kinetic_bombAI(pCreature);
}

// right side
struct MANGOS_DLL_DECL boss_prince_taldaramAI : public ScriptedAI
{
    boss_prince_taldaramAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetStandState(UNIT_STAND_STATE_DEAD);
		pCreature->SetHealth(1);
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiBerserkTimer;
	uint32 m_uiGlitteringSparksTimer;
	uint32 m_uiConjureFlamesTimer;

	void Reset()
	{
		m_uiBerserkTimer				= 600000;
		m_uiGlitteringSparksTimer		= 22000;
		m_uiConjureFlamesTimer          = 20000;

		if(m_pInstance)
			m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_IDLE);
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_TALDARAM_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_TALDARAM_KILL2, m_creature); break;
        }
    }

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != NOT_STARTED)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, NOT_STARTED);
		}
		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
		{
			if(pKeleseth->isAlive())
				pKeleseth->AI()->EnterEvadeMode();
			else
				pKeleseth->Respawn();
		}
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
		{
			if(pValanar->isAlive())
				pValanar->AI()->EnterEvadeMode();
			else
				pValanar->Respawn();
		}
	}

	void JustSummoned(Creature* pSummoned)
	{
		if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
		{
			pSummoned->SetInCombatWithZone();
			((mob_conjured_flameAI*)pSummoned->AI())->SetDestination(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ());
		}
	}

	void Aggro(Unit* pWho)
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != IN_PROGRESS)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, IN_PROGRESS);
		}
		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
			pKeleseth->AI()->AttackStart(pWho);
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
			pValanar->AI()->AttackStart(pWho);
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_TALDARAM_DEATH, m_creature);
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != DONE)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, DONE);
		}
		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
			pKeleseth->DealDamage(pKeleseth, pKeleseth->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
			pValanar->DealDamage(pValanar, pValanar->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
	}

	void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
		if(m_pInstance->GetData(TYPE_PRINCES_PHASE) != PHASE_TALDARAM)
			uiDamage = 0;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
		{
			if(m_creature->getVictim()->GetGUID() == pValanar->GetGUID()) 
                EnterEvadeMode();
		}

		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
		{
			if(m_creature->getVictim()->GetGUID() == pKeleseth->GetGUID()) 
                EnterEvadeMode();
		}

		if(m_uiBerserkTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_TALDARAM_BERSERK, m_creature);
			DoCast(m_creature, SPELL_BERSERK);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;

		if(m_uiGlitteringSparksTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				DoCast(pTarget, SPELL_GLITTERING_SPARKS);
			m_uiGlitteringSparksTimer = 22000;
		}
		else m_uiGlitteringSparksTimer -= uiDiff;

		if(m_uiConjureFlamesTimer < uiDiff)
		{
			if(m_pInstance->GetData(TYPE_PRINCES_PHASE) == PHASE_TALDARAM)
			{
				DoScriptText(SAY_TALDARAM_SPECIAL, m_creature);
				DoCast(m_creature, SPELL_CONJURE_EMPOWERED_FLAME);
				m_creature->SummonCreature(NPC_EMPOWERED_CONJURED_FLAME, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 1000);
			}
			else
			{
				m_creature->SummonCreature(NPC_CONJURED_FLAME, 0, 0, 0, 0, TEMPSUMMON_CORPSE_DESPAWN, 1000);
				DoCast(m_creature, SPELL_CONJURE_FLAME);
			}
			m_uiConjureFlamesTimer = 20000;
		}
		else m_uiConjureFlamesTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_taldaram(Creature* pCreature)
{
    return new boss_prince_taldaramAI(pCreature);
}

// left side
struct MANGOS_DLL_DECL boss_prince_kelesethAI : public ScriptedAI
{
    boss_prince_kelesethAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetStandState(UNIT_STAND_STATE_DEAD);
		pCreature->SetHealth(1);
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiBerserkTimer;
	uint32 m_uiShadowLance;
	uint32 m_uiShadowResonance;

	void Reset()
	{
		m_uiBerserkTimer				= 600000;
		m_uiShadowLance					= 10000;
        m_uiShadowResonance				= 20000;

		if(m_pInstance)
			m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_IDLE);
	}

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->SetInCombatWithZone();
		pSummoned->CastSpell(pSummoned, SPELL_SHADOW_RESONANCE_AURA, false);
	}

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != NOT_STARTED)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, NOT_STARTED);
		}
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
		{
			if(pTaldaram->isAlive())
				pTaldaram->AI()->EnterEvadeMode();
			else
				pTaldaram->Respawn();
		}
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
		{
			if(pValanar->isAlive())
				pValanar->AI()->EnterEvadeMode();
			else
				pValanar->Respawn();
		}
	}

	void Aggro(Unit* pWho)
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != IN_PROGRESS)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, IN_PROGRESS);
		}
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
			pTaldaram->AI()->AttackStart(pWho);
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
			pValanar->AI()->AttackStart(pWho);
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_TALDARAM_DEATH, m_creature);
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != DONE)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, DONE);
		}
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
			pTaldaram->DealDamage(pTaldaram, pTaldaram->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
			pValanar->DealDamage(pValanar, pValanar->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_KELESETH_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_KELESETH_KILL2, m_creature); break;
        }
    }

	void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
		if(m_pInstance->GetData(TYPE_PRINCES_PHASE) != PHASE_KELESETH)
			uiDamage = 0;
	}

	void AttackStart(Unit* pWho)
    {
        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20);
        }
    }

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Creature* pValanar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_VALANAR)))
		{
			if(m_creature->getVictim()->GetGUID() == pValanar->GetGUID()) 
                EnterEvadeMode();
		}

		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
		{
			if(m_creature->getVictim()->GetGUID() == pTaldaram->GetGUID()) 
                EnterEvadeMode();
		}

		if(m_uiBerserkTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_KELESETH_BERSERK, m_creature);
			DoCast(m_creature, SPELL_BERSERK);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;

		if(m_uiShadowResonance < uiDiff)
		{
			DoCast(m_creature,SPELL_SHADOW_RESONANCE);
			m_uiShadowResonance = 20000;
		}
		else m_uiShadowResonance -= uiDiff;

		if(m_uiShadowLance < uiDiff)
		{
			if(m_pInstance->GetData(TYPE_PRINCES_PHASE) == PHASE_KELESETH)
			{
				DoScriptText(SAY_KELESETH_SPECIAL, m_creature);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_EMPOWERED_SHADOW_LANCE);
				m_uiShadowLance = 15000;
			}
			else
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_SHADOW_LANCE);
				m_uiShadowLance = urand(5000, 7000);
			}
		}
		else m_uiShadowLance -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_keleseth(Creature* pCreature)
{
    return new boss_prince_kelesethAI(pCreature);
}

// main controller
// center
struct MANGOS_DLL_DECL boss_prince_valanarAI : public ScriptedAI
{
    boss_prince_valanarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetStandState(UNIT_STAND_STATE_DEAD);
		pCreature->SetHealth(1);
		m_bIsIntroDone = false;
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

    uint32 m_uiBerserkTimer;
	uint32 m_uiKineticBombTimer;
	uint32 m_uiVortexTimer;
	uint64 m_uiBloodOrbGUID;

	uint32 m_uiPhaseChangeTimer;
	uint8 m_uiPrincesPhase;

	uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
    bool m_bIsIntroDone;
    bool m_bIsIntro;

	void Reset()
	{
		m_uiBerserkTimer            = 600000;
		m_uiKineticBombTimer		= 15000;
		m_uiVortexTimer				= 25000;
		m_uiBloodOrbGUID			= 0;

		m_uiPhaseChangeTimer		= 47000;
		m_uiPrincesPhase			= 0;

		m_uiIntroTimer				= 1000;
        m_uiIntroStep				= 0;
        m_bIsIntro					= false;

		if(m_pInstance)
			m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_IDLE);
	}

	void JustSummoned(Creature* pSummoned)
	{
		// set vortex
		if(pSummoned->GetEntry() == NPC_SHOCK_VORTEX)
		{
			pSummoned->CastSpell(pSummoned, SPELL_SHOCK_VORTEX_DAMAGE, false);
			pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			pSummoned->setFaction(14);
			pSummoned->CombatStop();
			pSummoned->GetMotionMaster()->MoveIdle();
		}
		else if(pSummoned->GetEntry() == NPC_KINETIC_BOMB_TARGET)
		{
			pSummoned->SetDisplayId(11686);     // make invisible
			if(Creature* pBomb = m_creature->SummonCreature(NPC_KINETIC_BOMB, pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ() + 10, 0, TEMPSUMMON_CORPSE_DESPAWN, 1000))
			{
				((mob_kinetic_bombAI*)pBomb->AI())->SetDestination(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ());
				pBomb->GetMap()->CreatureRelocation(pBomb, pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ() + 30, 0);
                pBomb->SendMonsterMove(pSummoned->GetPositionX(), pSummoned->GetPositionY(), pSummoned->GetPositionZ() + 30, SPLINETYPE_NORMAL, pBomb->GetSplineFlags(), 1);
				pBomb->AddSplineFlag(SPLINEFLAG_FLYING);
				pBomb->SetInCombatWithZone();
			}
		}
	}

	void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_VALANAR_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_VALANAR_KILL2, m_creature); break;
        }
    }

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != NOT_STARTED)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, NOT_STARTED);
		}

		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
		{
			if(pKeleseth->isAlive())
				pKeleseth->AI()->EnterEvadeMode();
			else
				pKeleseth->Respawn();
		}
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
		{
			if(pTaldaram->isAlive())
				pTaldaram->AI()->EnterEvadeMode();
			else
				pTaldaram->Respawn();
		}
	}

	void MoveInLineOfSight(Unit* pWho)
    {
        if(!m_bIsIntro && !m_bIsIntroDone && m_creature->IsWithinDistInMap(pWho, 50) && m_creature->IsWithinLOSInMap(pWho))
            m_bIsIntro = true;
        ScriptedAI::MoveInLineOfSight(pWho);  
    }

	void Aggro(Unit* pWho)
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != IN_PROGRESS)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, IN_PROGRESS);
			m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_VALANAR);
		}
		m_uiPrincesPhase = 1;
		DoCast(m_creature, SPELL_INVOCATION_VALANAR);
		DoScriptText(SAY_VALANAR_BLOOD, m_creature);
		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
		{
			pKeleseth->AI()->AttackStart(pWho);
			pKeleseth->SetHealth(1);
		}
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
		{
			pTaldaram->AI()->AttackStart(pWho);
			pTaldaram->SetHealth(1);
		}

		// summon controller orb -> blizzliek way
		// here we use this prince as a controller
		//if(Creature* pOrb = m_creature->SummonCreature(NPC_BLOOD_ORB_CONTROLLER, 0, 0, 0, 0, TEMPSUMMON_MANUAL_DESPAWN, 0))
			//m_uiBloodOrbGUID = pOrb->GetGUID();
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_VALANAR_DEATH, m_creature);
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_PRINCE_COUNCIL) != DONE)
				m_pInstance->SetData(TYPE_PRINCE_COUNCIL, DONE);
		}
		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
			pKeleseth->DealDamage(pKeleseth, pKeleseth->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
			pTaldaram->DealDamage(pTaldaram, pTaldaram->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
	}

	void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
		if(m_pInstance->GetData(TYPE_PRINCES_PHASE) != PHASE_VALANAR)
			uiDamage = 0;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (m_bIsIntro && !m_bIsIntroDone)
        {
            switch(m_uiIntroStep)
			{
			case 1:
				if (Creature* pLanthel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LANATHEL)))
					DoScriptText(SAY_INTRO1, pLanthel);
				++m_uiIntroStep;
				m_uiIntroTimer = 16000;
				break;
			case 3:
				if (Creature* pLanthel = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LANATHEL)))
					DoScriptText(SAY_INTRO2, pLanthel);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->SetStandState(UNIT_STAND_STATE_STAND);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);		// remove feign death
				m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);	// remove dead flag
				m_creature->SetHealth(m_creature->GetMaxHealth());
				if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
				{
					pKeleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					pKeleseth->SetStandState(UNIT_STAND_STATE_STAND);
					pKeleseth->SetHealth(pKeleseth->GetMaxHealth());
					pKeleseth->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);		// remove feign death
					pKeleseth->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);	// remove dead flag
				}
				if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
				{
					pTaldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					pTaldaram->SetStandState(UNIT_STAND_STATE_STAND);
					pTaldaram->SetHealth(pTaldaram->GetMaxHealth());
					pTaldaram->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_29);		// remove feign death
					pTaldaram->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);	// remove dead flag
				}
				if(m_pInstance)
					m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_VALANAR);
				m_bIsIntro = false;
				m_bIsIntroDone = true;
				++m_uiIntroStep;
				m_uiIntroTimer = 10000;
                break;
            }

            if (m_uiIntroTimer <= uiDiff)
            {
                ++m_uiIntroStep;
                m_uiIntroTimer = 330000;
            } m_uiIntroTimer -= uiDiff;
        }

        if (!m_bIsIntroDone) 
            return;

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
		{
			if(m_creature->getVictim()->GetGUID() == pTaldaram->GetGUID()) 
                EnterEvadeMode();
		}

		if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
		{
			if(m_creature->getVictim()->GetGUID() == pKeleseth->GetGUID()) 
                EnterEvadeMode();
		}

		if(m_uiBerserkTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_VALANAR_BERSERK, m_creature);
			DoCast(m_creature, SPELL_BERSERK);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;

		if(m_uiKineticBombTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_KINETIC_BOMB);	// summons the target
			m_uiKineticBombTimer = 27000;
		}
		else m_uiKineticBombTimer -= uiDiff;

		if(m_uiVortexTimer < uiDiff)
		{
			if(m_pInstance->GetData(TYPE_PRINCES_PHASE) == PHASE_VALANAR)
			{
				DoScriptText(SAY_VALANAR_SPECIAL, m_creature);
				DoCast(m_creature, SPELL_EMPOWERED_SHOCK_VORTEX);
			}
			else
				DoCast(m_creature, SPELL_SHOCK_VORTEX);
			m_uiVortexTimer = 15000;
		}
		else m_uiVortexTimer -= uiDiff;

		if(m_uiPhaseChangeTimer < uiDiff)
		{
			switch(m_uiPrincesPhase)
			{
				// switch to Taldaram
			case 1:
				m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_TALDARAM);
				if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
				{
					DoScriptText(SAY_TALDARAM_BLOOD, pTaldaram);
					pTaldaram->CastSpell(pTaldaram, SPELL_INVOCATION_TALDARAM, false);
					pTaldaram->SetHealth(m_creature->GetHealth());
					((boss_prince_taldaramAI*)pTaldaram->AI())->m_uiConjureFlamesTimer = 20000;
					m_creature->RemoveAurasDueToSpell(SPELL_INVOCATION_VALANAR);
					m_creature->SetHealth(1);
				}
				++m_uiPrincesPhase;
				break;
				// switch to Keleseth
			case 2:
				m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_KELESETH);
				if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
				{
					DoScriptText(SAY_KELESETH_BLOOD, pKeleseth);
					pKeleseth->CastSpell(pKeleseth, SPELL_INVOCATION_KELESETH, false);
					((boss_prince_kelesethAI*)pKeleseth->AI())->m_uiShadowLance = 15000;
					if (Creature* pTaldaram = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TALDARAM)))
					{
						pKeleseth->SetHealth(pTaldaram->GetHealth());
						pTaldaram->RemoveAurasDueToSpell(SPELL_INVOCATION_TALDARAM);
						pTaldaram->SetHealth(1);
					}
				}
				++m_uiPrincesPhase;
				break;
				// switch back to Valanar
			case 3:
				m_pInstance->SetData(TYPE_PRINCES_PHASE, PHASE_VALANAR);
				DoCast(m_creature, SPELL_INVOCATION_VALANAR);
				DoScriptText(SAY_VALANAR_BLOOD, m_creature);
				if (Creature* pKeleseth = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_KELESETH)))
				{
					pKeleseth->RemoveAurasDueToSpell(SPELL_INVOCATION_KELESETH);
					m_creature->SetHealth(pKeleseth->GetHealth());
					pKeleseth->SetHealth(1);
				}
				m_uiVortexTimer = 15000;
				m_uiPrincesPhase = 1;
				break;
			}
			m_uiPhaseChangeTimer = 47000;
		}
		else m_uiPhaseChangeTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_prince_valanar(Creature* pCreature)
{
    return new boss_prince_valanarAI(pCreature);
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
    newscript->Name = "mob_kinetic_bomb";
    newscript->GetAI = &GetAI_mob_kinetic_bomb;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_conjured_flame";
    newscript->GetAI = &GetAI_mob_conjured_flame;
    newscript->RegisterSelf();
}
