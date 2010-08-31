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
SDName: boss_alar
SD%Complete: 95
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

enum
{
	// spells
	// phase 1
	SPELL_FLAME_BUFFET            = 34121,
	SPELL_FLAME_QUILLS            = 34229,
	SPELL_EMBER_BLAST_ALAR		  = 34341,
	SPELL_REBIRTH                 = 34342,
	SPELL_REBIRTH_2               = 35369,
	SPELL_CHARGE                  = 35412,

	// phase 2
	SPELL_MELT_ARMOR              = 35410,
	SPELL_DIVE_BOMB_VISUAL        = 35367,
	SPELL_DIVE_BOMB               = 35181,
	NPC_FIRE_BOMB_TARGET		  = 18225,
	SPELL_ENRAGE                  = 27680,

	NPC_FLAME_PATCH				  = 20602,
	SPELL_FLAME_PATCH             = 35380,

	NPC_EMBER_OF_ALAR			  = 19551,
	SPELL_EMBER_BLAST             = 34133,

	PHASE_IDLE					  = 0,
	PHASE_ONE					  = 1,
	PHASE_TWO					  = 2,
};

struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

static LocationsXY PlatformLoc[]=
{
	{340.15f, 58.65f, 17.71f},		// platforms
    {388.09f, 31.54f, 20.18f},
    {388.18f, -32.85f, 20.18f},
    {340.29f, -60.19f, 17.72f},
    {331.0f, 0.01f, 39.0f},			// center loc
    {331.0, 0.01f, -2.39f}			// rebirth loc
};

struct MANGOS_DLL_DECL boss_alarAI : public ScriptedAI
{
    boss_alarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    
    ScriptedInstance* m_pInstance;

	uint32 m_uiPhase;
	uint32 m_uiRangeCheckTimer;

	// phase 1
	uint32 m_uiReviveTimer;
	bool m_bIsRebirth;
	uint32 m_uiFlameQuillsTimer;
	uint32 m_uiFlameQuillsStage;
	uint32 m_uiMoveTimer;
	uint32 m_uiMoveStage;
	uint8 m_uiLastPlatformId;

	// phase 2
	uint32 m_uiFlamePatchTimer;
	uint32 m_uiDiveBombTimer;
	uint32 m_uiDiveBombStage;
	uint32 m_uiMeltArmorTimer;
	uint32 m_uiBerserkTimer;

    void Reset()
    {
		m_uiPhase = PHASE_IDLE;
		m_uiRangeCheckTimer	= 15000;

		// phase 1
		m_uiFlameQuillsTimer	= 150000;
		m_uiFlameQuillsStage	= 0;
		m_uiMoveTimer			= urand(30000, 45000);
		m_uiMoveStage			= 1;
		m_uiLastPlatformId		= 0;
		m_bIsRebirth			= false;

		// phase 2
		m_uiFlamePatchTimer		= 20000;
		m_uiDiveBombTimer		= 30000;
		m_uiDiveBombStage		= 0;
		m_uiMeltArmorTimer		= 60000;
		m_uiBerserkTimer		= 600000;

		SetCombatMovement(false);
    }
    
    void Aggro (Unit* pWho)
    {
		m_creature->GetMotionMaster()->MovePoint(1, PlatformLoc[0].x, PlatformLoc[0].y, PlatformLoc[0].z);
		m_uiPhase = PHASE_ONE;
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ALAR, IN_PROGRESS);
    }

	void JustReachedHome()
	{
		if(m_pInstance)
			m_pInstance->SetData(TYPE_ALAR, NOT_STARTED);
	}
 
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ALAR, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
		if(uiDamage > m_creature->GetHealth() || m_creature->GetHealthPercent() < 1.0f && m_uiPhase == PHASE_ONE && !m_bIsRebirth)
		{
			uiDamage = 0;
			m_creature->GetMotionMaster()->Clear(); 
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->InterruptNonMeleeSpells(false);
			m_creature->SetHealth(0);
			m_creature->StopMoving();
			m_creature->RemoveAllAuras();
			m_creature->GetMap()->CreatureRelocation(m_creature, PlatformLoc[5].x, PlatformLoc[5].y, PlatformLoc[5].z, 0.0f);
			m_creature->SendMonsterMove(PlatformLoc[5].x, PlatformLoc[5].y, PlatformLoc[5].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
			m_creature->SetVisibility(VISIBILITY_OFF);
			m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
			DoCast(m_creature, SPELL_REBIRTH);
			m_uiReviveTimer = 2000;
			m_bIsRebirth = true;
		}
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
		if(uiType != POINT_MOTION_TYPE)
			return;

		if(uiPointId == 1)
		{
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MoveIdle();
			if(Creature* pTemp = m_creature->SummonCreature(NPC_EMBER_OF_ALAR, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
				pTemp->SetInCombatWithZone();
			m_uiMoveTimer = urand(30000, 45000);
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiPhase == PHASE_ONE)
		{
			if(m_uiReviveTimer < uiDiff && m_bIsRebirth)
			{
				m_uiPhase = PHASE_TWO;
				SetCombatMovement(true);
				m_creature->SetStandState(UNIT_STAND_STATE_STAND);
				m_creature->SetHealth(m_creature->GetMaxHealth());
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
				m_creature->SetVisibility(VISIBILITY_ON);
				m_bIsRebirth = false;
			}
			else m_uiReviveTimer -= uiDiff;

			if(m_bIsRebirth)
				return;

			if(m_uiFlameQuillsTimer < uiDiff)
			{
				switch(m_uiFlameQuillsStage)
				{
				case 0:
					m_creature->GetMotionMaster()->Clear();
					m_creature->GetMotionMaster()->MovePoint(0, PlatformLoc[4].x, PlatformLoc[4].y, PlatformLoc[4].z);
					m_uiFlameQuillsTimer = 3000;
					++m_uiFlameQuillsStage;
					break;
				case 1:
					DoCast(m_creature, SPELL_FLAME_QUILLS);
					m_creature->GetMotionMaster()->MoveIdle();
					m_creature->GetMap()->CreatureRelocation(m_creature, PlatformLoc[4].x, PlatformLoc[4].y, PlatformLoc[4].z, 0.0f);
					m_creature->SendMonsterMove(PlatformLoc[4].x, PlatformLoc[4].y, PlatformLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
					m_uiFlameQuillsTimer = 10000;
					++m_uiFlameQuillsStage;
					break;
				case 2:
					m_creature->GetMotionMaster()->Clear();
					m_creature->GetMotionMaster()->MovePoint(0, PlatformLoc[m_uiLastPlatformId].x, PlatformLoc[m_uiLastPlatformId].y, PlatformLoc[m_uiLastPlatformId].z);
					m_uiFlameQuillsTimer = 150000;
					m_uiFlameQuillsStage = 0;
					break;
				}
			}
			else m_uiFlameQuillsTimer -= uiDiff;

			if(m_uiFlameQuillsStage > 0)
				return;

			if(m_uiMoveTimer < uiDiff)
			{
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MovePoint(1, PlatformLoc[m_uiMoveStage].x, PlatformLoc[m_uiMoveStage].y, PlatformLoc[m_uiMoveStage].z);
				++m_uiMoveStage;
				if(m_uiMoveStage == 4)
					m_uiMoveStage = 0;
				m_uiRangeCheckTimer = 5000;
				m_uiMoveTimer = urand(30000, 45000);
			}
			else m_uiMoveTimer -= uiDiff;
		}
		else if(m_uiPhase == PHASE_TWO)
		{
			if(m_uiBerserkTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoCast(m_creature, SPELL_ENRAGE);
				m_uiBerserkTimer = 30000;
			}
			else m_uiBerserkTimer -= uiDiff;

			if(m_uiDiveBombTimer < uiDiff)
			{
				switch(m_uiDiveBombStage)
				{
				case 0:
					DoCast(m_creature, SPELL_DIVE_BOMB_VISUAL);
					SetCombatMovement(false);
					m_uiDiveBombTimer = 2000;
					++m_uiDiveBombStage;
					break;
				case 1:
					m_creature->GetMap()->CreatureRelocation(m_creature, PlatformLoc[4].x, PlatformLoc[4].y, PlatformLoc[4].z, 0.0f);
					m_creature->SendMonsterMove(PlatformLoc[4].x, PlatformLoc[4].y, PlatformLoc[4].z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
					m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_uiDiveBombTimer = 2000;
					++m_uiDiveBombStage;
					break;
				case 2:
					if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
					{
						m_creature->GetMap()->CreatureRelocation(m_creature, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f);
						m_creature->SendMonsterMove(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
						m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						m_creature->RemoveAurasDueToSpell(SPELL_DIVE_BOMB_VISUAL);
						DoCast(pTarget, SPELL_CHARGE);
					}
					SetCombatMovement(true);
					for(uint8 i = 0; i < 2; ++i)
					{
						if(Creature* pTemp = m_creature->SummonCreature(NPC_EMBER_OF_ALAR, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
							pTemp->SetInCombatWithZone();
					}
					m_uiDiveBombTimer = 30000;
					m_uiDiveBombStage = 0;
					break;
				}
			}
			else m_uiDiveBombTimer -= uiDiff;

			if(m_uiDiveBombStage > 0)
				return;

			if(m_uiFlamePatchTimer < uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
					m_creature->SummonCreature(NPC_FLAME_PATCH, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
			}
			else m_uiFlamePatchTimer -= uiDiff;

			if(m_uiMeltArmorTimer < uiDiff)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_MELT_ARMOR);
				m_uiMeltArmorTimer = 60000;
			}
			else m_uiMeltArmorTimer -= uiDiff;
		}

		if(m_uiRangeCheckTimer < uiDiff)
		{
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
				DoCast(m_creature, SPELL_FLAME_BUFFET);
			m_uiRangeCheckTimer = 2000;
		}
		else m_uiRangeCheckTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_alar(Creature *pCreature)
{
    return new boss_alarAI (pCreature);
}

struct MANGOS_DLL_DECL mob_ember_of_alarAI : public ScriptedAI
{
    mob_ember_of_alarAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

	ScriptedInstance* m_pInstance;

    uint32 m_uiDieTimer;
	bool m_bMustDie;

    void Reset()
    {
        m_bMustDie = false;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
		if(uiDamage > m_creature->GetHealth())
		{
			if(!m_bMustDie)
				DoCast(m_creature, SPELL_EMBER_BLAST);
			uiDamage = 0;
			m_uiDieTimer = 1000;
			m_bMustDie = true;
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiDieTimer < uiDiff && m_bMustDie)
		{
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			if(Creature* pAlar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ALAR)))
				pAlar->DealDamage(pAlar, pAlar->GetMaxHealth() * 0.03, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		}
		else m_uiDieTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_ember_of_alar(Creature* pCreature)
{
    return new mob_ember_of_alarAI (pCreature);
}

struct MANGOS_DLL_DECL mob_flame_patch_alarAI : public ScriptedAI
{
    mob_flame_patch_alarAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		SetCombatMovement(false);
	}

	ScriptedInstance* m_pInstance;

    void Reset() 
	{
		DoCast(m_creature, SPELL_EMBER_BLAST);
	}

    void AttackStart(Unit* pWho) 
	{
		return;
	}

    void UpdateAI(const uint32 diff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_ALAR) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
	}

};

CreatureAI* GetAI_mob_flame_patch_alar(Creature* pCreature)
{
    return new mob_flame_patch_alarAI (pCreature);
}

void AddSC_boss_alar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_alar";
    newscript->GetAI = &GetAI_boss_alar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ember_of_alar";
    newscript->GetAI = &GetAI_mob_ember_of_alar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_patch_alar";
    newscript->GetAI = &GetAI_mob_flame_patch_alar;
    newscript->RegisterSelf();
}
