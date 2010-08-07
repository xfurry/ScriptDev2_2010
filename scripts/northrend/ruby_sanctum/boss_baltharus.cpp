/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_baltharus
SD%Complete: 
SDComment:
SDCategory: ruby_sanctum
EndScriptData */

// Not fully offlike clone work, but Blizz idea is intact.
// Need correct timers

#include "precompiled.h"
#include "ruby_sanctum.h"

enum Equipment
{
    EQUIP_MAIN           = 49888,
    EQUIP_OFFHAND        = EQUIP_NO_CHANGE,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum BossSpells
{
	//yells
	SAY_XERESTRASZA_INTRO	= -1666000,
	SAY_INTRO				= -1666305,
	SAY_AGGRO				= -1666300,
	SAY_SLAY1				= -1666301,
	SAY_SLAY2				= -1666302,
	SAY_DEATH				= -1666303,
	SAY_CLONE				= -1666304,
	SAY_OUTRO1				= -1666001,
	SAY_OUTRO2				= -1666002,
	SAY_OUTRO3				= -1666003,
	SAY_OUTRO4				= -1666004,
	SAY_OUTRO5				= -1666005,
	SAY_OUTRO6				= -1666006,
	SAY_OUTRO7				= -1666007,
	SAY_OUTRO8				= -1666008,

	// spells
    SPELL_BLADE_TEMPEST             = 75125, //every 22 secs
	SPELL_REPELLING_WAVE            = 74509, //every 10-15 secs
	SPELL_CLEAVE					= 40504,
	SPELL_SUMMON_CLONE              = 74511, //summons npc 39899 (Clone)
    SPELL_ENERVATING_BRAND          = 74502, //friendlys in 12yards = 74505
	SPELL_SIMPHONED_MIGHT			= 74507,    
    SPELL_CHANNEL_SPELL             = 76221, //Channeling dummy spell
};

static Locations SpawnLoc[]=
{
    {3155.540039f, 342.391998f, 84.596802f},   // 0 - start point
    {3152.329834f, 359.41757f, 85.301605f},    // 1 - second say
    {3152.078369f, 383.939178f, 86.337875f},   // 2 - other says and staying
    {3155.190703f, 538.717708f, 72.889038f},   // 3 - Halion spawn point
	{3152.329834f, 359.41757f, 85.301605f},    // Baltharus target point
    {3153.06f, 389.486f, 86.2596f},            // Baltharus initial point
};

/*######
## boss_xerestrasza
######*/
struct MANGOS_DLL_DECL mob_xerestraszaAI : public ScriptedAI
{
    mob_xerestraszaAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		m_bIsOutro				= false;
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 m_uiOutroTimer;
    uint32 m_uiOutroStep;
    bool m_bIsOutro;

    void Reset()
    {
        m_uiOutroTimer          = 5000;
        m_uiOutroStep           = 0;

		m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }

	void StartMovement(uint32 id)
    {
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_bIsOutro)
        {
            switch(m_uiOutroStep)
            {
            case 1:
                DoScriptText(SAY_OUTRO1, m_creature);
                StartMovement(1);
                ++m_uiOutroStep;
                m_uiOutroTimer = 10000;
                break;
            case 3:
                DoScriptText(SAY_OUTRO2, m_creature);
                StartMovement(2);
                ++m_uiOutroStep;
                m_uiOutroTimer = 10000;
                break;
            case 5:
                DoScriptText(SAY_OUTRO3, m_creature);
                ++m_uiOutroStep;
                m_uiOutroTimer = 12000;
                break;
			case 7:
				DoScriptText(SAY_OUTRO4, m_creature);
				++m_uiOutroStep;
				m_uiOutroTimer = 12000;
				break;
			case 9:
				DoScriptText(SAY_OUTRO5, m_creature);
				++m_uiOutroStep;
				m_uiOutroTimer = 10000;
				break;
			case 11:
				DoScriptText(SAY_OUTRO6, m_creature);
				++m_uiOutroStep;
				m_uiOutroTimer = 10000;
				break;
			case 13:
				DoScriptText(SAY_OUTRO7, m_creature);
				++m_uiOutroStep;
				m_uiOutroTimer = 10000;
				break;
			case 15:
				DoScriptText(SAY_OUTRO8, m_creature);
				++m_uiOutroStep;
				m_uiOutroTimer = 4000;
				break;
			case 17:
				m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                m_bIsOutro = false;
                ++m_uiOutroStep;
                m_uiOutroTimer = 10000;
                break;
            }

            if (m_uiOutroTimer <= uiDiff)
            {
                ++m_uiOutroStep;
                m_uiOutroTimer = 330000;
            } m_uiOutroTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_xerestrasza(Creature* pCreature)
{
    return new mob_xerestraszaAI (pCreature);
}

/*######
## boss_baltharus
######*/
struct MANGOS_DLL_DECL boss_baltharusAI : public ScriptedAI
{
    boss_baltharusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		m_bIsIntroDone = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
	uint32 Difficulty;

    bool m_bIsIntro;
	bool m_bIsIntroDone;
	uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
	uint32 m_uiHealthDrop;

	uint32 m_uiCleaveTimer;
	uint32 m_uiBladeTempestTimer;
	uint32 m_uiEnervatingBrandTimer;
	uint32 m_uiRepellingWaveTimer;

	void Reset()
	{
		m_bIsIntro				= false;
		m_uiIntroTimer          = 1000;
        m_uiIntroStep           = 0;

		m_uiCleaveTimer				= 5000;
		m_uiBladeTempestTimer		= 15000;
		m_uiEnervatingBrandTimer	= 10000;
		m_uiRepellingWaveTimer		= 20000;

		if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
			m_uiHealthDrop = 50;
		if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			m_uiHealthDrop = 75;

		//m_creature->GetMotionMaster()->MoveIdle();

		/*Creature* pBaltharus_Target = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BALTHARUS_TARGET)));
		if (pBaltharus_Target)
		{
			pBaltharus_Target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
			pBaltharus_Target->GetMotionMaster()->MoveIdle();
		} 
		else if (pBaltharus_Target = m_creature->SummonCreature(NPC_BALTHARUS_TARGET, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 1000))
			pBaltharus_Target->GetMotionMaster()->MoveIdle();*/

		if(Creature* pTarget = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_XERESTRASZA))))
		{
			m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
			DoCast(pTarget, SPELL_CHANNEL_SPELL);
		}
	}

	void JustReachedHome()
	{
		if(m_pInstance)
			m_pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
	}

    void MoveInLineOfSight(Unit* pWho) 
    {
		if (!m_bIsIntro && !m_bIsIntroDone && m_creature->IsWithinDistInMap(pWho, 60.0f))
		{
			m_bIsIntro = true;
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		}
    }

	void JustDied(Unit* pKiller)
	{
		//if (Creature* pBaltharus_Target = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BALTHARUS_TARGET)))) 
			//pBaltharus_Target->ForcedDespawn();

		if(Creature* pXerestrasza = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_XERESTRASZA))))
            ((mob_xerestraszaAI*)pXerestrasza->AI())->m_bIsOutro = true;

		DoScriptText(SAY_DEATH, m_creature);

		if(m_pInstance)
			m_pInstance->SetData(TYPE_BALTHARUS, DONE);
	}

	void KilledUnit(Unit* pVictim)
	{
		switch (urand(0,1)) 
		{
		case 0:
			DoScriptText(SAY_SLAY1, m_creature, pVictim);
			break;
		case 1:
			DoScriptText(SAY_SLAY2, m_creature, pVictim);
			break;
		}
	}

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() != NPC_BALTHARUS_TARGET) 
             pSummoned->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        //SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
		m_creature->CastStop();
		m_creature->SetInCombatWithZone();
		m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

		if(m_pInstance)
			m_pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
        DoScriptText(SAY_AGGRO,m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
		if(m_creature->GetHealthPercent() < m_uiHealthDrop)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_SUMMON_CLONE);

			if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				m_uiHealthDrop -= 50;
			if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				m_uiHealthDrop -= 25;
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
		// some intro
		if (m_bIsIntro && !m_bIsIntroDone)
        {
            switch(m_uiIntroStep)
            {
            case 1:
				if(Creature* pXerestrasza = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_XERESTRASZA))))
					DoScriptText(SAY_XERESTRASZA_INTRO,  pXerestrasza);
                ++m_uiIntroStep;
                m_uiIntroTimer = 7000;
                break;
            case 3:
                DoScriptText(SAY_INTRO, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 5:
				// end intro
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
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

        if(m_uiCleaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
			m_uiCleaveTimer = 5000;
		}
		else m_uiCleaveTimer -= uiDiff;

		if(m_uiBladeTempestTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BLADE_TEMPEST);
			m_uiBladeTempestTimer = 15000;
		}
		else m_uiBladeTempestTimer -= uiDiff;

		if(m_uiEnervatingBrandTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ENERVATING_BRAND);
			m_uiEnervatingBrandTimer = 10000;
		}
		else m_uiEnervatingBrandTimer -= uiDiff;

		if(m_uiRepellingWaveTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_REPELLING_WAVE);
			m_uiRepellingWaveTimer = 20000;
		}
		else m_uiRepellingWaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

/*######
## mob_baltharus_clone
######*/
struct MANGOS_DLL_DECL mob_baltharus_cloneAI : public ScriptedAI
{
    mob_baltharus_cloneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		//SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiCleaveTimer;
	uint32 m_uiBladeTempestTimer;
	uint32 m_uiEnervatingBrandTimer;

    void Reset()
    {
		m_uiCleaveTimer				= 5000;
		m_uiBladeTempestTimer		= 15000;
		m_uiEnervatingBrandTimer	= 10000;
        m_creature->SetRespawnDelay(DAY);
    }

	void KilledUnit(Unit* pVictim)
	{
		if(Creature* pBaltharus = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BALTHARUS))))
		{
			switch (urand(0,1)) 
			{
			case 0:
				DoScriptText(SAY_SLAY1,pBaltharus,pVictim);
				break;
			case 1:
				DoScriptText(SAY_SLAY2,pBaltharus,pVictim);
				break;
			}
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance->GetData(TYPE_BALTHARUS) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiCleaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_CLEAVE);
			m_uiCleaveTimer = 5000;
		}
		else m_uiCleaveTimer -= uiDiff;

		if(m_uiBladeTempestTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BLADE_TEMPEST);
			m_uiBladeTempestTimer = 15000;
		}
		else m_uiBladeTempestTimer -= uiDiff;

		if(m_uiEnervatingBrandTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ENERVATING_BRAND);
			m_uiEnervatingBrandTimer = 10000;
		}
		else m_uiEnervatingBrandTimer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_baltharus_clone(Creature* pCreature)
{
    return new mob_baltharus_cloneAI(pCreature);
}

void AddSC_boss_baltharus()
{
    Script *newscript;

	newscript = new Script;
    newscript->Name = "mob_xerestrasza";
    newscript->GetAI = &GetAI_mob_xerestrasza;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_baltharus";
    newscript->GetAI = &GetAI_boss_baltharus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_baltharus_clone";
    newscript->GetAI = &GetAI_mob_baltharus_clone;
    newscript->RegisterSelf();
}
