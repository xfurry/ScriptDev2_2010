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
SDName: boss_headless_horseman
SD%Complete: 0
SDComment: Place Holder
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"
#include "scarlet_monastery.h"

enum
{
	// yells
    SAY_ENTRANCE           = -1189022,
    SAY_REJOINED           = -1189023,
    SAY_BODY_DEFEAT        = -1189024,
    SAY_LOST_HEAD          = -1189025,
    SAY_CONFLAGRATION      = -1189026,
    SAY_SPROUTING_PUMPKINS = -1189027,
    SAY_SLAY               = -1189028,
    SAY_DEATH              = -1189029,

    EMOTE_LAUGH            = -1189030,

    SAY_PLAYER1            = -1189031,
    SAY_PLAYER2            = -1189032,
    SAY_PLAYER3            = -1189033,
    SAY_PLAYER4            = -1189034, 

	// spells
	// active
	SPELL_CLEAVE				= 42587,
	SPELL_SUMMON_PUMPKIN		= 52236,
	SPELL_CONFLAGRATE			= 42637,

	// confused
	SPELL_WHIRLWIND				= 43116,
	SPELL_IMMUNE				= 42556, 
	SPELL_BODY_REGEN			= 42403,
	SPELL_CONFUSE				= 43105,
	SPELL_BODY_STAGE_1			= 42547,
    SPELL_BODY_STAGE_2			= 42548,
    SPELL_BODY_STAGE_3			= 42549,
    SPELL_COMMAND_RETURN_HEAD	= 42405,  // TARGET_SCRIPT head 23775, no visual,  when body's hp reaches up to 100%, and phase not increases
    SPELL_COMMAND_HEAD_RETURNS  = 42410,  // TARGET_SCRIPT body 23682, no visual, all ok, next phase begins, cancel regen body checks
    SPELL_COMMAND_TOSS_HEAD     = 43101,  // TARGET_SCRIPT head 23775, 10687 visual, when body "dies"

	// visuals
	SPELL_RHYME_BIG				= 42909,
    SPELL_RHYME_SMALL			= 42910,
    SPELL_BODY_FLAME			= 42074,      // apply aura, after horseman dies
    SPELL_HEAD_FLAME			= 42971,
    SPELL_ENRAGE_VISUAL			= 42438,      // he uses this spell?
    SPELL_WISP_BLUE				= 42821,
    SPELL_WISP_FLIGHT_PORT		= 42818,
    SPELL_WISP_INVIS			= 42823,
    SPELL_SMOKE					= 42355,
    SPELL_DEATH					= 42566,       //not correct spell

	// pumpkin
	SPELL_PUMPKIN_AURA			= 42280,
	SPELL_PUMPKIN_AURA_GREEN	= 42294,
	SPELL_SQUASH_SOUL			= 42514,
    SPELL_SPROUTING				= 42281,
    SPELL_SPROUT_BODY			= 42285,

	// head
	SPELL_FLYING_HEAD			= 42399,       //visual flying head
    SPELL_HEAD					= 42413,       //visual buff, "head"
    SPELL_HEAD_SPEAKS			= 43129,
    SPELL_HEAD_LANDS			= 42400,
    SPELL_HEAD_INVISIBLE		= 44312,

	// death
	SPELL_BODY_DEATH			= 42429,
    SPELL_HEAD_IS_DEAD			= 42428,       //at killing head, Phase 3, looks like explosion
    SPELL_BODY_LEAVES_COMBAT	= 43805,

	// npc
	NPC_HORSEMAN				= 23800,	// unmounted
	NPC_HEAD					= 23775,
	NPC_PULSING_PUMPKIN			= 23694,
	NPC_PUMPKIN_FIEND			= 23545,
	NPC_FLAME_BUNNY				= 23686,
	NPC_WISP_INVIS				= 24034, 

	DISPLAY_ID_MOUNTED			= 22351,
	DISPLAY_ID_UNMOUNTED		= 22352,
};

struct MANGOS_DLL_DECL boss_headless_horsemanAI : public ScriptedAI
{
    boss_headless_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		Reset();
	}

	uint8 m_uiPhase;
	uint8 m_uiCombatPhase;
	uint64 m_uiHeadGUID;
	uint32 m_uiCleaveTimer;
	uint32 m_uiConflagrateTimer;
	uint32 m_uiPumpkinTimer;
	uint32 m_uiLaughTimer;
	uint32 m_uiWhirlTimer;

    void Reset()
    {
		m_uiLaughTimer = urand(5000, 10000);
		m_uiPumpkinTimer = 1000;
		m_uiCleaveTimer = 3000;
		m_uiConflagrateTimer = 10000;
		m_uiPhase = 0;
		m_uiCombatPhase = 0;
    }

    void Aggro(Unit* pWho)
    {
		DoScriptText(SAY_ENTRANCE, m_creature);
		m_uiCombatPhase = 1;
        m_creature->SetInCombatWithZone();
    }

	void JustReachedHome()
	{
		m_creature->ForcedDespawn();
		if(Creature* pHead = m_creature->GetMap()->GetCreature(m_uiHeadGUID))
			pHead->ForcedDespawn();
	}

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_uiCombatPhase == 2)
		{
			uiDamage = 0;
			return;
		}

		if(m_uiPhase == 0 && m_creature->GetHealth() < uiDamage)
		{
			m_uiPhase = 1;
			uiDamage = 0;
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = 1797.632f + urand(10, 40)*cos(angle*(M_PI/180));
			float homeY = 1348.985f + urand(10, 40)*sin(angle*(M_PI/180));
			if(Creature* pHead = m_creature->SummonCreature(NPC_HEAD, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 1000))
				m_uiHeadGUID = pHead->GetGUID();
			m_creature->SetDisplayId(DISPLAY_ID_UNMOUNTED);
			DoCast(m_creature, SPELL_IMMUNE);
			m_uiCombatPhase = 2;
			DoScriptText(SAY_LOST_HEAD, m_creature);
		}
		else if((m_uiPhase == 1 || m_uiPhase == 2) && m_creature->GetHealth() < uiDamage)
		{
			if(Creature* pHead = m_creature->GetMap()->GetCreature(m_uiHeadGUID))
			{
				pHead->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				pHead->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				pHead->SetVisibility(VISIBILITY_ON);
			}
			uiDamage = 0;
			m_uiPhase = 2;
			m_uiCombatPhase = 2;
			m_uiWhirlTimer = 2000;
			m_uiPumpkinTimer = 10000;
			m_creature->SetDisplayId(DISPLAY_ID_UNMOUNTED);
			DoCast(m_creature, SPELL_IMMUNE);
			DoScriptText(SAY_LOST_HEAD, m_creature);
		}
	}

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
		if(Creature* pTemp = m_creature->SummonCreature(NPC_FLAME_BUNNY, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
			pTemp->CastSpell(pTemp, SPELL_BODY_FLAME, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiCombatPhase == 1)
		{
			if(m_uiLaughTimer < uiDiff)
			{
				DoScriptText(EMOTE_LAUGH, m_creature);
				m_uiLaughTimer = urand(15000, 30000);
			}
			else m_uiLaughTimer -= uiDiff;

			if(m_uiConflagrateTimer < uiDiff && m_uiPhase >= 1)
			{
				DoScriptText(SAY_CONFLAGRATION, m_creature);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_SUMMON_PUMPKIN);
				m_uiConflagrateTimer = urand(5000, 9000);
			}
			else m_uiConflagrateTimer -= uiDiff;

			if(m_uiCleaveTimer < uiDiff)
			{
				DoCast(m_creature->getVictim(), SPELL_CLEAVE);
				m_uiCleaveTimer = urand(3000, 6000);
			}
			else m_uiCleaveTimer -= uiDiff;

			if(m_uiPumpkinTimer < uiDiff && m_uiPhase == 2)
			{
				DoScriptText(SAY_SPROUTING_PUMPKINS, m_creature);
				DoCast(m_creature, SPELL_SUMMON_PUMPKIN);
				m_uiPumpkinTimer = 60000;
			}
			else m_uiPumpkinTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
		else if(m_uiCombatPhase == 2)
		{
			if(!m_creature->HasAura(SPELL_BODY_REGEN, EFFECT_INDEX_0))
				DoCast(m_creature, SPELL_BODY_REGEN);

			if(!m_creature->HasAura(SPELL_CONFUSE, EFFECT_INDEX_0))
				DoCast(m_creature, SPELL_CONFUSE);

			if(m_uiPhase >= 1)
			{
				if(m_uiWhirlTimer < uiDiff)
				{
					DoCast(m_creature, SPELL_WHIRLWIND);
					m_uiWhirlTimer = 10000;
				}
				else m_uiWhirlTimer -= uiDiff;
			}
		}
    }
};

CreatureAI* GetAI_boss_headless_horseman(Creature* pCreature)
{
    return new boss_headless_horsemanAI(pCreature);
}

struct MANGOS_DLL_DECL mob_horsemans_headAI : public ScriptedAI
{
    mob_horsemans_headAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->setFaction(14);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance *m_pInstance;

	uint8 m_uiPhase;
	uint32 m_uiMovementTimer;

	uint32 m_uiDieTimer;
	bool m_bMustDie;

    void Reset()
    {
		DoCast(m_creature, SPELL_HEAD_FLAME);
		m_uiPhase = 0;
		m_uiDieTimer		= 1000;
		m_bMustDie			= false;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void DoExplode()
	{
		DoCast(m_creature, SPELL_HEAD_IS_DEAD);
		m_uiDieTimer = 100;
		m_bMustDie = true;
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_uiPhase == 0 && m_creature->GetHealthPercent() < 50.0f)
		{
			m_uiPhase = 1;
			m_creature->SetVisibility(VISIBILITY_OFF);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			DoScriptText(SAY_REJOINED, m_creature);
			if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HEADLESS_HORSEMAN)))
			{
				pTemp->SetDisplayId(DISPLAY_ID_MOUNTED);
				pTemp->SetHealth(pTemp->GetMaxHealth());
				pTemp->RemoveAllAuras();
				pTemp->GetMotionMaster()->MoveChase(pTemp->getVictim());
				((boss_headless_horsemanAI*)pTemp->AI())->m_uiCombatPhase = 1;
			}
		}
		else if(m_uiPhase == 1 && m_creature->GetHealthPercent() < 10.0f)
		{
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			m_creature->SetVisibility(VISIBILITY_OFF);
			m_uiPhase = 2;
			DoScriptText(SAY_REJOINED, m_creature);
			if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HEADLESS_HORSEMAN)))
			{
				pTemp->SetDisplayId(DISPLAY_ID_MOUNTED);
				pTemp->SetHealth(pTemp->GetMaxHealth());
				pTemp->RemoveAllAuras();
				pTemp->GetMotionMaster()->MoveChase(pTemp->getVictim());
				((boss_headless_horsemanAI*)pTemp->AI())->m_uiCombatPhase = 1;
			}
		}
		else if(m_uiPhase == 2 && m_creature->GetHealth() < uiDamage)
		{
			uiDamage = 0;
			DoExplode();
		}
	}

	void JustDied(Unit* pKiller)
	{
		if(Creature* pTemp = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HEADLESS_HORSEMAN)))
			pTemp->CastSpell(pTemp, SPELL_BODY_DEATH, false);
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiDieTimer < uiDiff && m_bMustDie)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= uiDiff;

		if(m_uiMovementTimer < uiDiff)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			float homeX = 1797.632f + urand(10, 40)*cos(angle*(M_PI/180));
			float homeY = 1348.985f + urand(10, 40)*sin(angle*(M_PI/180));
			m_creature->GetMotionMaster()->MovePoint(0, homeX, homeY, m_creature->GetPositionZ());
			m_uiMovementTimer = urand(3000, 6000);
		}
		else m_uiMovementTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_horsemans_head(Creature* pCreature)
{
    return new mob_horsemans_headAI(pCreature);
}

struct MANGOS_DLL_DECL mob_pumpkinAI : public ScriptedAI
{
    mob_pumpkinAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{
		pCreature->setFaction(14);
		Reset();
	}

	bool m_bHasGreenAura;
	uint32 m_uiTransformTimer;
	uint32 m_uiSproutTimer;
	bool m_bStartAttack;

	uint32 m_uiSquashTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_PUMPKIN_AURA);
		m_bHasGreenAura = false;
		m_bStartAttack = false;
		m_uiSproutTimer = 1000;
		m_uiTransformTimer = 10000;
		m_uiSquashTimer = 5000;
    }

	void AttackStart(Unit* pWho)
    {
		if(!m_bStartAttack)
			return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if(!m_bHasGreenAura && m_creature->HasAura(SPELL_PUMPKIN_AURA_GREEN, EFFECT_INDEX_0))
		{
			DoCast(m_creature, SPELL_PUMPKIN_AURA_GREEN);
			m_bHasGreenAura = true;
		}
		
		if(m_uiSproutTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SPROUTING);
			m_uiSproutTimer = 6000000;
		}
		else m_uiSproutTimer -= uiDiff;

		if(m_uiTransformTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SPROUT_BODY);
			m_bStartAttack = true;
			m_creature->SetInCombatWithZone();
			m_uiTransformTimer = 6000000;
		}
		else m_uiTransformTimer -= uiDiff;

		if(!m_bStartAttack)
			return;

		if(m_uiSquashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SQUASH_SOUL);
			m_uiSquashTimer = urand(4000, 7000);
		}
		else m_uiSquashTimer -= uiDiff;

		DoMeleeAttackIfReady();;
    }
};

CreatureAI* GetAI_mob_pumpkin(Creature* pCreature)
{
    return new mob_pumpkinAI(pCreature);
}

void AddSC_boss_headless_horseman()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_headless_horseman";
    NewScript->GetAI = GetAI_boss_headless_horseman;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_horsemans_head";
    NewScript->GetAI = GetAI_mob_horsemans_head;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_pumpkin";
    NewScript->GetAI = GetAI_mob_pumpkin;
    NewScript->RegisterSelf();
}
