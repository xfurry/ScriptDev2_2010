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
SDName: boss_the_lich_king
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // yells
	// intro
	SAY_INTRO_LK_1			= -1609550,
	SAY_INTRO_TIRION_2		= -1609551,
	SAY_INTRO_LK_3			= -1609552,
	SAY_INTRO_TIRION_4		= -1609553,
	SAY_INTRO_LK_5			= -1609554,

	// event
	SAY_AGGRO				= -1609555,
	SAY_WINTER				= -1609556,
	SAY_QUAKE				= -1609557,
	SAY_VALKYR				= -1609558,
	SAY_HARVEST_SOUL		= -1609559,
	SAY_SPECIAL_1			= -1609565,
	SAY_SPECIAL_2			= -1609566,
	SAY_EVENT_END			= -1609567,		// at 10% hp
	SAY_SLAY_1				= -1609568,
	SAY_SLAY_2				= -1609569,
	SAY_BERSERK				= -1609570,

	// frostmourne
	SAY_TERENAS_1			= -1609560,
	SAY_TERENAS_2			= -1609561,
	SAY_TERENAS_3			= -1609562,
	SAY_PLAYER_ESCAPE		= -1609563,
	SAY_PLAYER_DIES			= -1609564,

	// outro
	SAY_OUTRO_LK_1			= -1609571,
	SAY_OUTRO_LK_2			= -1609572,
	SAY_OUTRO_LK_3			= -1609573,
	SAY_OUTRO_LK_4			= -1609574,
	SAY_OUTRO_LK_5			= -1609575,
	SAY_OUTRO_TIRION_6		= -1609576,
	SAY_OUTRO_LK_7			= -1609577,
	SAY_OUTRO_TIRION_8		= -1609578,
	SAY_OUTRO_TERENAS_9		= -1609579,
	SAY_OUTRO_TERENAS_10	= -1609580,
	SAY_OUTRO_LK_11			= -1609581,
	SAY_OUTRO_LK_12			= -1609582,
	SAY_OUTRO_LK_13			= -1609583,


    // spells
	// phase 1
    SPELL_SUMMON_HORROR			= 70372,
	NPC_SHAMBLING_HORROR		= 37698,
    SPELL_SUMMON_GHOULS			= 70358,
	NPC_DRUDGE_GHOUL			= 37695,
    SPELL_INFEST				= 70541,
	SPELL_NECROTIC_PLAGUE		= 70337,
    SPELL_PLAGUE_SIPHON			= 74074,
	SPELL_SHADOW_TRAP_SUMMON	= 73539,	// on heroic only
	SPELL_SHADOW_TRAP_DUMMY		= 73525,

	// phase 2
    SPELL_SUMMON_VALKYR			= 69037,
	SPELL_SUMMON_VALKYR_TRIGG	= 74361,
	NPC_VALKYR_SHADOWGUARD		= 36609,
    SPELL_SOUL_REAPER			= 69409,
	SPELL_SOUL_REAPER_HASTE		= 69410,	// used on LK
    SPELL_DEFILE				= 72762,	//72754,
	SPELL_DEFILE_TRIGG			= 72743,
	NPC_DEFILE					= 38757,

	// phase 3
    SPELL_HARVEST_SOUL			= 68980,
	SPELL_HARVEST_SOUL_STRANG	= 68986,	// uses vehicles
	SPELL_HARVEST_SOUL_TELE		= 73655,	// tele and adds periodic dmg
	SPELL_HARVEST_SOUL_TELEPORT	= 71372,
	NPC_STRANGULATE_VEHICLE     = 36598,
	SPELL_FROSTMOURNE_TELEPORT	= 73078,
    SPELL_VILE_SPIRITS			= 70498,
    NPC_VILE_SPIRIT				= 37799,

	// frostmourne
	NPC_TERENAS_MENETHIL		= 36823,
    SPELL_LIGHTS_FAVOR			= 69382,	// pasive heal on terenas
    SPELL_RESTORE_SOUL			= 72595,	// restore the player
	NPC_SPIRIT_WARDEN			= 36824,
    SPELL_SOUL_RIP				= 69397,	// on terenas
    SPELL_DARK_HUNGER			= 69383,	// 69384
    SPELL_HARVESTED_SOUL		= 74322,	// on frostmourne

	// transition
    SPELL_REMORSELESS_WINTER	= 68981,
    SPELL_PAIN_AND_SUFFERING	= 72133,
    SPELL_SUMMON_ICE_SPHERE		= 69103,
	SPELL_ICE_SPHERE_SUMMON		= 69104,
	SPELL_ICE_SPHERE_VISUAL		= 69090,
	NPC_ICE_SPHERE				= 36633,
    SPELL_RAGING_SPIRIT			= 69200, 
	SPELL_RAGING_SPIRIT_SUMMON	= 69201,
	SPELL_RAGING_SPIRIT_VISUAL	= 69198,
	NPC_RAGING_SPIRIT			= 36701,
    SPELL_QUAKE					= 72262,

	// other
    SPELL_FURY_FROSTMOURNE		= 72350,
	SPELL_BERSERK				= 47008,
	SPELL_ICE_LOCK				= 71614,
	SPELL_RAISE_DEAD			= 71769,
	NPC_TERENAS_END				= 38579,
	SPELL_MASS_RESURRECTION		= 72429,

	// outro
	SPELL_TIRION_RELEASE		= 71797,
	SPELL_FROSTMOURNE_SOUND		= 70667,
	SPELL_BROKEN_FM_FALL		= 72399,	// not used
	SPELL_BROKEN_FM_OBJECT		= 72398,
	SPELL_BROKEN_FM_RELEASE		= 72405,
	SPELL_BROKEN_FM_SUMMON		= 72407,
	SPELL_BROKEN_FM_THROW		= 73017,	// by LK
	NPC_FROSTMOURNE_TRIGG		= 38584,
	SPELL_SUMMON_TERENAS		= 72420,
	SPELL_TERENAS_VISUAL		= 72372,
    
	// shambling horror
	SPELL_SHOCKWAVE				= 72149,
	SPELL_ENRAGE				= 72143,

	// valkyr shadowguard
	SPELL_GRAB					= 62712,	// fix id
	SPELL_WINGS_OF_DAMMED		= 74352,
	SPELL_LIFE_SIMPHON			= 73783,	// on heroic only

	// vile spirit
	SPELL_SPIRIT_BURST			= 70503,

	// ice sphere
	SPELL_ICE_PULSE				= 69099,
	SPELL_ICE_PULSE_TRIGG		= 69091,
    SPELL_ICE_BURST				= 69108,

	// raging spirit
	SPELL_SOUL_SHRIEK			= 69242,
};

enum Phases
{
	PHASE_IDLE		= 0,
	PHASE_INTRO		= 1,
	PHASE_ONE		= 2,
	PHASE_TRANS_ONE	= 3,
	PHASE_TWO		= 4,
	PHASE_TRANS_TWO	= 5,
	PHASE_THREE		= 6,
	PHASE_FINAL		= 7,
};

enum Common
{
     FINAL_ARTHAS_MOVIE             = 16,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations SpawnLoc[]=
{
    {459.93689f, -2124.638184f, 1040.860107f},    // 0 Lich King Intro
    {503.15652f, -2124.516602f, 1040.860107f},    // 1 Lich king move end
    {491.27118f, -2124.638184f, 1040.860107f},    // 2 Tirion 1
    {481.69797f, -2124.638184f, 1040.860107f},    // 3 Tirion 2
    {498.00448f, -2201.573486f, 1046.093872f},    // 4 Valkyrs?
    {517.48291f, -2124.905762f, 1040.861328f},    // 5 Tirion?
    {529.85302f, -2124.709961f, 1040.859985f},    // 6 Lich king final, o=3.1146
    {520.311f, -2124.709961f, 1040.859985f},      // 7 Frostmourne
};

/*######
### terenas menethil
######*/

struct MANGOS_DLL_DECL npc_terenas_menethilAI : public ScriptedAI
{
    npc_terenas_menethilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint64 m_uiPlayerGUID;
	uint32 m_uiDieTimer;
	bool m_bMustDie;
	uint32 m_uiUpdateThreatTimer;

	uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
    bool m_bIsIntro;

    void Reset() 
	{
		m_creature->SetRespawnDelay(DAY);
		m_uiPlayerGUID			= 0;
		m_bMustDie				= false;
		m_uiUpdateThreatTimer	= 5000;
		m_uiIntroTimer          = 1000;
        m_uiIntroStep           = 0;
        m_bIsIntro              = false;
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_LIGHTS_FAVOR);
		m_bIsIntro = true;
	}

	void SetTarget(uint64 uiTargetGUID)
	{
		m_uiPlayerGUID = uiTargetGUID;
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (m_bIsIntro)
        {
            switch(m_uiIntroStep)
            {
            case 1:
                DoScriptText(SAY_TERENAS_1, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 3:
                DoScriptText(SAY_TERENAS_2, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 5:
                DoScriptText(SAY_TERENAS_3, m_creature);          
                m_bIsIntro = false;
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            }

            if (m_uiIntroTimer <= uiDiff)
            {
                ++m_uiIntroStep;
                m_uiIntroTimer = 330000;
            } 
			m_uiIntroTimer -= uiDiff;
        }

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiUpdateThreatTimer < uiDiff)
		{
			if(Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_SPIRIT_WARDEN, 50.0f))
				m_creature->AddThreat(pTarget, 10000.0f);
			m_uiUpdateThreatTimer = 5000;
		}
		else m_uiUpdateThreatTimer -= uiDiff;

		if(!GetClosestCreatureWithEntry(m_creature, NPC_SPIRIT_WARDEN, 50.0f))
		{
			if(Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
			{
				if(Creature* pLichKing = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LICH_KING)))
					DoScriptText(SAY_PLAYER_ESCAPE, pLichKing);
				DoCast(pPlayer, SPELL_RESTORE_SOUL);
				pPlayer->TeleportTo(631, 529.3f, -2124.7f, 1050.0f, 3.14159f);
				m_uiDieTimer = 5000;
				m_bMustDie = true;
			}
		}

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_npc_terenas_menethil(Creature* pCreature)
{
    return new npc_terenas_menethilAI(pCreature);
}

/*######
### spirit warden
######*/

struct MANGOS_DLL_DECL mob_spirit_wardenAI : public ScriptedAI
{
    mob_spirit_wardenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint64 m_uiPlayerGUID;
	uint32 m_uiDieTimer;
	bool m_bMustDie;

	uint32 m_uiSoulReapTimer;
	uint32 m_uiUpdateThreatTimer;

    void Reset() 
	{
		m_creature->SetRespawnDelay(DAY);
		m_uiSoulReapTimer = urand(5000, 7000);
		m_uiUpdateThreatTimer = 5000;
		m_uiPlayerGUID = 0;
		m_bMustDie = false;
	}

	void SetTarget(uint64 uiTargetGUID)
	{
		m_uiPlayerGUID = uiTargetGUID;
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSoulReapTimer < uiDiff)
		{
			if(Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_TERENAS_MENETHIL, 50.0f))
				DoCast(pTarget, SPELL_SOUL_RIP);
			m_uiSoulReapTimer = urand(7000, 9000);
		}
		else m_uiSoulReapTimer -= uiDiff;

		if(m_uiUpdateThreatTimer < uiDiff)
		{
			if(Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_TERENAS_MENETHIL, 50.0f))
				m_creature->AddThreat(pTarget, 10000.0f);
			m_uiUpdateThreatTimer = 5000;
		}
		else m_uiUpdateThreatTimer -= uiDiff;

		if(!GetClosestCreatureWithEntry(m_creature, NPC_TERENAS_MENETHIL, 50.0f))
		{
			if(Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
			{
				if(Creature* pLichKing = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LICH_KING)))
					DoScriptText(SAY_PLAYER_DIES, pLichKing);
				DoCast(pPlayer, SPELL_DARK_HUNGER);
				pPlayer->DealDamage(pPlayer, pPlayer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
				m_uiDieTimer = 5000;
				m_bMustDie = true;
			}
		}

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_spirit_warden(Creature* pCreature)
{
    return new mob_spirit_wardenAI(pCreature);
}

/*######
### The Lich King
######*/

struct MANGOS_DLL_DECL boss_the_lich_kingAI : public ScriptedAI
{
    boss_the_lich_kingAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_bIsFirstTime = true;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

	// phase 1
    uint32 m_uiSummonHorrorTimer;
    uint32 m_uiInfestTimer;
    uint32 m_uiSummonGhoulsTimer;
	uint32 m_uiNecroticPlagueTimer;
	uint32 m_uiShadowTrapTimer;
	// phase 2
    uint32 m_uiSoulReaperTimer;
	uint32 m_uiSoulReaperHasteTimer;
    uint32 m_uiDefileTimer;
    uint32 m_uiSummonValkyrTimer;
	// phase 3
    uint32 m_uiHarvestSoulTimer;
	uint32 m_uiHarvestEndTimer;
	uint64 m_uiHarvestTargetGUID;
    uint32 m_uiVileSpiritsTimer;
	// transition
	uint32 m_uiPhaseEndTimer;
    uint32 m_uiRemorselessWinterTimer;
    uint32 m_uiPainAndSufferingTimer;
    uint32 m_uiSummonIceSphereTimer;
    uint32 m_uiRagingSpiritTimer;
	// roleplay
	uint32 m_uiSpeechTimer;
    uint32 m_uiSpeechPhase;
	uint32 m_uiOldFlag;
	uint32 m_uiActivateTimer;
	bool m_bIsActive;
	uint64 m_uiBrokenFrostmourneGUID;
	uint64 m_uiTerenasGUID;
    
    uint32 m_uiBerserkTimer;
	uint32 m_uiYellTimer;
    uint8 m_uiPhase;
	bool m_bIsFirstTime;

    void Reset()
    {
        m_uiBerserkTimer			= 900000;
		m_uiYellTimer				= 30000;
		// phase 1
        m_uiSummonHorrorTimer		= 22000;
        m_uiInfestTimer				= 26000;
        m_uiSummonGhoulsTimer		= 15000;
		m_uiNecroticPlagueTimer		= 11000;
		m_uiShadowTrapTimer			= 12000;
		// phase 2
        m_uiSoulReaperTimer			= 15000;
		m_uiSoulReaperHasteTimer	= 60000;
        m_uiDefileTimer				= 30000;
        m_uiSummonValkyrTimer		= 10000;
		// phase 3
        m_uiHarvestSoulTimer		= 30000;
		m_uiHarvestEndTimer			= 60000;
		m_uiHarvestTargetGUID		= 0;
        m_uiVileSpiritsTimer		= 10000;
		// transition
		m_uiPhaseEndTimer			= 60000;
        m_uiRemorselessWinterTimer	= 60000;
        m_uiPainAndSufferingTimer	= 5000;
        m_uiSummonIceSphereTimer	= 10000;
        m_uiRagingSpiritTimer		= 17000;
		// roleplay
		m_uiSpeechTimer				= 1000;
        m_uiSpeechPhase				= 0;
		m_uiActivateTimer			= 3000;
		m_bIsActive					= false;

		m_uiBrokenFrostmourneGUID	= 0;
		m_uiTerenasGUID				= 0;
        
        m_uiPhase					= PHASE_IDLE;

		if(!m_bIsFirstTime)
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
		if(!m_bIsFirstTime)
			DoScriptText(SAY_AGGRO, m_creature);

		m_uiPhase = PHASE_ONE;
		m_bIsFirstTime = false;

        if (m_pInstance)
		{
            m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_SHOW, 1);
			m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
			if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_TOTAL, 9999);
			else
				m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_TOTAL, 25);
		}
    }

	void AttackStart(Unit* pWho)
    {
		if (m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        }
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(m_uiPhase == PHASE_TRANS_ONE || m_uiPhase == PHASE_TRANS_TWO)
			uiDamage = 0;
	}

    void JustReachedHome()
    {
        if (m_pInstance)
		{
            m_pInstance->SetData(TYPE_LICH_KING, FAIL);
			m_pInstance->SetData(TYPE_ATTEMPTS, m_pInstance->GetData(TYPE_ATTEMPTS) - 1);
            m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_ATTEMPTS));
		}

		if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_1)))
			pStanding->SetGoState(GO_STATE_READY);
		if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_2)))
			pStanding->SetGoState(GO_STATE_READY);
		if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_3)))
			pStanding->SetGoState(GO_STATE_READY);
		if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_4)))
			pStanding->SetGoState(GO_STATE_READY);
		if(GameObject* pEdge = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_SNOW_EDGE)))
			pEdge->SetGoState(GO_STATE_READY);
		if(GameObject* pWind = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_THRONE_FROSTY_WIND)))
			pWind->SetGoState(GO_STATE_READY);
		if (GameObject* pGoFloor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ARTHAS_PLATFORM)))
		{
			pGoFloor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
			pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1, m_uiOldFlag);
		}

		/*if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
		{
		if(pTirion->isAlive())
			{
				pTirion->AI()->EnterEvadeMode();
				pTirion->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			}
		}*/
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
        }
    }

	void DoSendCinematic()
    {
        Map::PlayerList const &pList = m_creature->GetMap()->GetPlayers();
        if (pList.isEmpty()) 
			return;

        for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
		{
             if (Player* pPlayer = i->getSource())
			 {
                 if (pPlayer && pPlayer->isAlive() && pPlayer->IsInMap(m_creature))
                     pPlayer->SendMovieStart(FINAL_ARTHAS_MOVIE);
			 }
		}
    }

    void DoRevivePlayers()
    {
        Map::PlayerList const &pList = m_creature->GetMap()->GetPlayers();
        if (pList.isEmpty()) 
			return;

		for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
		{
			if (Player* pPlayer = i->getSource())
			{
				if (pPlayer && !pPlayer->isAlive() && pPlayer->IsInMap(m_creature))
					pPlayer->ResurrectPlayer(100, false);
			}
		};
	}

    void JustDied(Unit* pKiller)
    {
		DoSendCinematic();
		DoScriptText(SAY_OUTRO_LK_13, m_creature);

		if(Creature* pTerenas = m_creature->GetMap()->GetCreature(m_uiTerenasGUID))
			pTerenas->ForcedDespawn();
		if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
			pTirion->AI()->EnterEvadeMode();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, DONE);
    }

	void JustSummoned(Creature* pSummoned)
	{
		if (pSummoned->GetEntry() == NPC_TERENAS_END)
		{
			m_uiTerenasGUID = pSummoned->GetGUID();
			pSummoned->CastSpell(pSummoned, SPELL_TERENAS_VISUAL, false);
		}
		else
			pSummoned->SetInCombatWithZone();
	}

    void UpdateAI(const uint32 uiDiff)
	{
		if(m_uiPhase != PHASE_INTRO && m_uiPhase != PHASE_FINAL)
		{
			if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
				return;

			if(m_uiBerserkTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoScriptText(SAY_BERSERK, m_creature);
				DoCast(m_creature, SPELL_BERSERK);
				m_uiBerserkTimer = 30000;
			}
			else m_uiBerserkTimer -= uiDiff;

			if(m_uiYellTimer < uiDiff)
			{
				switch(urand(0, 1))
				{
				case 0: DoScriptText(SAY_SPECIAL_1, m_creature); break;
				case 1: DoScriptText(SAY_SPECIAL_2, m_creature); break;
				}
				m_uiYellTimer = urand(30000, 60000);
			}
			else m_uiYellTimer -= uiDiff;

			if(m_uiActivateTimer < uiDiff && m_bIsActive)
			{
				if(GameObject* pEdge = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_SNOW_EDGE)))
					pEdge->SetGoState(GO_STATE_READY);
				if(m_uiPhase == PHASE_TWO || m_uiPhase == PHASE_THREE)
				{
					if (GameObject* pGoFloor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ARTHAS_PLATFORM)))
					{
						pGoFloor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
						m_uiOldFlag = pGoFloor->GetUInt32Value(GAMEOBJECT_BYTES_1);
						pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1,8449);
					}
				}
				else if(m_uiPhase == PHASE_TRANS_TWO)
				{
					// restore
					if (GameObject* pGoFloor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ARTHAS_PLATFORM)))
					{
						pGoFloor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
						pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1, m_uiOldFlag);
					}
				}
				m_bIsActive = false;
			}
			else m_uiActivateTimer -= uiDiff;
		}

        switch (m_uiPhase)
        {
		case PHASE_INTRO:

			if(m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
					{
						pTirion->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
						pTirion->GetMotionMaster()->MovePoint(0, 490.667f, -2124.972f, 1040.860f);
						pTirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
					}
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 3000;
                    break;
                case 1:
					DoScriptText(SAY_INTRO_LK_1, m_creature);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 1000;
                    break;
                case 2:
					m_creature->SetStandState(UNIT_STAND_STATE_STAND);
					m_creature->GetMotionMaster()->MovePoint(0, 461.728f, -2124.406f, 1040.86f);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 13000;
                    break;
				case 3:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoScriptText(SAY_INTRO_TIRION_2, pTirion);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 8000;
					break;
				case 4:
					DoScriptText(SAY_INTRO_LK_3, m_creature);
					m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_LAUGH);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 3000;
					break;
				case 5:
					m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_ONESHOT_POINT_NOSHEATHE);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 6:
					m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_ONESHOT_NONE);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 17000;
					break;
				case 7:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoScriptText(SAY_INTRO_TIRION_4, pTirion);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 8:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NOSHEATHE);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 9:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->GetMotionMaster()->MovePoint(0, 478.948f, -2124.756f, 1040.859f);
					// cast some summon ghoul spell
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 10:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoCast(pTirion, SPELL_ICE_LOCK);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 11:
					DoScriptText(SAY_INTRO_LK_5, m_creature);
					m_creature->setFaction(14);
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
					m_creature->SetInCombatWithZone();
					m_uiPhase = PHASE_ONE;
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
					break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			break;
        case PHASE_ONE:

            if (m_uiSummonHorrorTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_SUMMON_HORROR);
                m_uiSummonHorrorTimer = 60000;
            } 
			else m_uiSummonHorrorTimer -= uiDiff;

            if (m_uiSummonGhoulsTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_SUMMON_GHOULS);
                m_uiSummonGhoulsTimer = 20000;
            } 
			else m_uiSummonGhoulsTimer -= uiDiff;

            if (m_uiInfestTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_INFEST);
                m_uiInfestTimer = 25000;
            } 
			else m_uiInfestTimer -= uiDiff;

            if (m_uiNecroticPlagueTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
					DoCast(pTarget, SPELL_NECROTIC_PLAGUE);
                m_uiNecroticPlagueTimer = 30000;
            } 
			else m_uiNecroticPlagueTimer -= uiDiff;

			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
			{
				if(m_uiShadowTrapTimer < uiDiff)
				{
					DoCast(m_creature, SPELL_SHADOW_TRAP_SUMMON);
					m_uiShadowTrapTimer = urand(12000, 15000);
				}
				else m_uiShadowTrapTimer -= uiDiff;
			}

			if(m_creature->GetHealthPercent() < 70.0f)
			{
				m_creature->GetMap()->CreatureRelocation(m_creature, 502.935f, -2125.925f, 1040.860f, 0);
				m_creature->SendMonsterMove(502.935f, -2125.925f, 1040.860f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
				m_creature->CastStop();
				m_creature->GetMotionMaster()->MoveIdle();
				SetCombatMovement(false);
				DoCast(m_creature, SPELL_REMORSELESS_WINTER);
				DoScriptText(SAY_WINTER, m_creature);
				m_uiPhase = PHASE_TRANS_ONE;
				m_uiPhaseEndTimer = 60000;
				if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_1)))
					pStanding->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_2)))
					pStanding->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_3)))
					pStanding->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pStanding = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_ICESHARD_STANDINGS_4)))
					pStanding->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pWind = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_THRONE_FROSTY_WIND)))
					pWind->SetGoState(GO_STATE_ACTIVE);
				m_uiPainAndSufferingTimer	= 5000;
				m_uiSummonIceSphereTimer	= 6000;
				m_uiRagingSpiritTimer		= 7000;
			}
            
			DoMeleeAttackIfReady();
            
			break; 

        case PHASE_TWO:

            if (m_uiSummonValkyrTimer < uiDiff)
            {
				DoScriptText(SAY_VALKYR, m_creature);
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_SUMMON_VALKYR);
                m_uiSummonValkyrTimer = 45000;
            } 
			else m_uiSummonValkyrTimer -= uiDiff;

            if (m_uiInfestTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_INFEST);
                m_uiInfestTimer = 25000;
            } 
			else m_uiInfestTimer -= uiDiff;

            if (m_uiSoulReaperTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_SOUL_REAPER);
				m_uiSoulReaperHasteTimer = 5000;
                m_uiSoulReaperTimer = 30000;
            } 
			else m_uiSoulReaperTimer -= uiDiff;

			if(m_uiSoulReaperHasteTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoCast(m_creature, SPELL_SOUL_REAPER_HASTE);
				m_uiSoulReaperHasteTimer = 60000;
			}
			else m_uiSoulReaperHasteTimer -= uiDiff;

            if (m_uiDefileTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_DEFILE);
                m_uiDefileTimer = 35000;
            } 
			else m_uiDefileTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 40.0f)
			{
				m_creature->GetMap()->CreatureRelocation(m_creature, 502.935f, -2125.925f, 1040.860f, 0);
				m_creature->SendMonsterMove(502.935f, -2125.925f, 1040.860f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
				m_creature->GetMotionMaster()->MoveIdle();
				SetCombatMovement(false);
				m_creature->CastStop();
				DoCast(m_creature, SPELL_REMORSELESS_WINTER);
				DoScriptText(SAY_WINTER, m_creature);
				m_uiPhase = PHASE_TRANS_TWO;
				if(GameObject* pEdge = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_SNOW_EDGE)))
					pEdge->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pWind = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_THRONE_FROSTY_WIND)))
					pWind->SetGoState(GO_STATE_ACTIVE);
				m_bIsActive					= true;
				m_uiActivateTimer			= 1000;
				m_uiPhaseEndTimer			= 60000;
				m_uiPainAndSufferingTimer	= 5000;
				m_uiSummonIceSphereTimer	= 6000;
				m_uiRagingSpiritTimer		= 7000;
			}

            DoMeleeAttackIfReady();

            break;

        case PHASE_THREE:

            if (m_uiDefileTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_DEFILE);
                m_uiDefileTimer = 35000;
            } 
			else m_uiDefileTimer -= uiDiff;

            if (m_uiSoulReaperTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_SOUL_REAPER);
				m_uiSoulReaperHasteTimer = 5000;
                m_uiSoulReaperTimer = 30000;
            } 
			else m_uiSoulReaperTimer -= uiDiff;

			if(m_uiSoulReaperHasteTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoCast(m_creature, SPELL_SOUL_REAPER_HASTE);
				m_uiSoulReaperHasteTimer = 60000;
			}
			else m_uiSoulReaperHasteTimer -= uiDiff;

            if (m_uiHarvestSoulTimer < uiDiff) 
            {
				DoScriptText(SAY_HARVEST_SOUL, m_creature);
				m_creature->InterruptNonMeleeSpells(true);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				{
					DoCast(pTarget, SPELL_HARVEST_SOUL);
					m_uiHarvestTargetGUID = pTarget->GetGUID();
				}
				m_uiHarvestEndTimer	= 5000;
                m_uiHarvestSoulTimer = 75000;
            } 
			else m_uiHarvestSoulTimer -= uiDiff;

			if(m_uiHarvestEndTimer < uiDiff)
			{
				// teleport to frostmourne here
				if(Player* pTarget = m_creature->GetMap()->GetPlayer(m_uiHarvestTargetGUID))
				{
					if(pTarget->isAlive())
					{
						DoCast(pTarget, SPELL_HARVEST_SOUL_TELEPORT);
						pTarget->TeleportTo(631, 513.557f, -2548.935f, 1047.595f, 2.32f);
						if(Creature* pTerenas = m_creature->SummonCreature(NPC_TERENAS_MENETHIL, 496.617f, -2520.889f, 1046.531f, 3.086f, TEMPSUMMON_CORPSE_DESPAWN, 1000))
						{
							if(Creature* pSpiritWarden = m_creature->SummonCreature(NPC_SPIRIT_WARDEN, 477.981f, -2520.915f, 1046.531f, 6.169f, TEMPSUMMON_CORPSE_DESPAWN, 1000))
							{
								pTerenas->AddThreat(pSpiritWarden, 10000.0f);
								pSpiritWarden->AddThreat(pTerenas, 10000.0f);
								pTerenas->AI()->AttackStart(pSpiritWarden);
								pTerenas->SetInCombatWith(pSpiritWarden);
								pSpiritWarden->AI()->AttackStart(pTerenas);
								pSpiritWarden->SetInCombatWith(pTerenas);
								((npc_terenas_menethilAI*)pTerenas->AI())->SetTarget(pTarget->GetGUID());
								((mob_spirit_wardenAI*)pSpiritWarden->AI())->SetTarget(pTarget->GetGUID());
							}
						}
					}
				}
				m_uiHarvestEndTimer = 600000;
			}
			else m_uiHarvestEndTimer -= uiDiff;

            if (m_uiVileSpiritsTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
				// not working, needs override
				for(int i = 0; i < 10; i++)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = 505.212f + urand(0, 25)*cos(angle*(M_PI/180));
					float homeY = -2124.350f + urand(0, 25)*sin(angle*(M_PI/180));
					float homeZ = 1040.939f + 15;
					if(Creature* pTemp = m_creature->SummonCreature(NPC_VILE_SPIRIT, homeX, homeY, homeZ, 0, TEMPSUMMON_TIMED_DESPAWN, 35000))
					{
						pTemp->GetMap()->CreatureRelocation(pTemp, homeX, homeY, homeZ, 0);
						pTemp->SendMonsterMove(homeX, homeY, homeZ, SPLINETYPE_NORMAL, pTemp->GetSplineFlags(), 1);
					}
				}
                //DoCast(m_creature, SPELL_VILE_SPIRITS);
                m_uiVileSpiritsTimer = 30000;
            } 
			else m_uiVileSpiritsTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 10.0f)
			{
				m_creature->CastStop();
				SetCombatMovement(false);
				DoScriptText(SAY_EVENT_END, m_creature);
				DoCast(m_creature, SPELL_FURY_FROSTMOURNE);
				m_uiPhase = PHASE_FINAL;
				m_uiSpeechTimer = 20000;
				m_uiSpeechPhase = 0;
			}

            DoMeleeAttackIfReady();

            break;

        case PHASE_TRANS_ONE:

			if (m_uiPhaseEndTimer <= uiDiff)
			{
				DoScriptText(SAY_QUAKE, m_creature);
				DoCast(m_creature, SPELL_QUAKE);
				if(GameObject* pEdge = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_SNOW_EDGE)))
					pEdge->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pWind = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_THRONE_FROSTY_WIND)))
					pWind->SetGoState(GO_STATE_READY);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				SetCombatMovement(true);
				m_uiPhase			= PHASE_TWO;
				m_bIsActive			= true;
				m_uiActivateTimer	= 6000;
				m_uiPhaseEndTimer	= 60000;
				break;
			} 
			else m_uiPhaseEndTimer -= uiDiff;

            if (m_uiPainAndSufferingTimer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                m_uiPainAndSufferingTimer = 3000;
            } 
			else m_uiPainAndSufferingTimer -= uiDiff;

            if (m_uiSummonIceSphereTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_ICE_SPHERE_SUMMON);
                m_uiSummonIceSphereTimer = 4000;
            } 
			else m_uiSummonIceSphereTimer -= uiDiff;

            if (m_uiRagingSpiritTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_RAGING_SPIRIT_SUMMON);
                m_uiRagingSpiritTimer = 20000;
            } 
			else m_uiRagingSpiritTimer -= uiDiff;

            break;

        case PHASE_TRANS_TWO:

			if (m_uiPhaseEndTimer <= uiDiff)
			{
				DoScriptText(SAY_QUAKE, m_creature);
				DoCast(m_creature, SPELL_QUAKE);
				if(GameObject* pEdge = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_SNOW_EDGE)))
					pEdge->SetGoState(GO_STATE_ACTIVE);
				if(GameObject* pWind = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_THRONE_FROSTY_WIND)))
					pWind->SetGoState(GO_STATE_READY);
				m_creature->GetMotionMaster()->Clear();
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				SetCombatMovement(true);
				m_uiPhase			= PHASE_THREE;
				m_bIsActive			= true;
				m_uiActivateTimer	= 6000;
				m_uiPhaseEndTimer	= 60000;
				break;
			} 
			else m_uiPhaseEndTimer -= uiDiff;

            if (m_uiPainAndSufferingTimer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                m_uiPainAndSufferingTimer = 3000;
            } 
			else m_uiPainAndSufferingTimer -= uiDiff;

            if (m_uiSummonIceSphereTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_ICE_SPHERE_SUMMON);
                m_uiSummonIceSphereTimer = 4000;
            } 
			else m_uiSummonIceSphereTimer -= uiDiff;

            if (m_uiRagingSpiritTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_RAGING_SPIRIT_SUMMON);
                m_uiRagingSpiritTimer = 20000;
            } 
			else m_uiRagingSpiritTimer -= uiDiff;
 
            break;
		case PHASE_FINAL:

			if(m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					DoScriptText(SAY_OUTRO_LK_1, m_creature);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 30000;
                    break;
                case 1:
					DoScriptText(SAY_OUTRO_LK_2, m_creature);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 8000;
                    break;
				case 2:
					DoScriptText(SAY_OUTRO_LK_3, m_creature);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 25000;
					break;
				case 3:
					m_creature->GetMotionMaster()->MovePoint(0, 505.212f, -2124.350f, m_creature->GetPositionZ());
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 4:
					DoScriptText(SAY_OUTRO_LK_4, m_creature);
					m_creature->GetMotionMaster()->MoveIdle();
					SetCombatMovement(false);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 3000;
					break;
				case 5:
					DoCast(m_creature, SPELL_RAISE_DEAD);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 24000;
					break;
				case 6:
					DoScriptText(SAY_OUTRO_LK_5, m_creature);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 6000;
					break;
					// tirion breaks free
				case 7:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoScriptText(SAY_OUTRO_TIRION_6, pTirion);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 9000;
					break;
				case 8:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->CastSpell(pTirion, SPELL_TIRION_RELEASE, false);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 6000;
					break;
				case 9:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->RemoveAurasDueToSpell(SPELL_ICE_LOCK);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 3000;
					break;
				case 10:
					// here frostmourne breakes
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->GetMotionMaster()->MovePoint(0, 513.503f, -2124.332f, 1040.860f);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 11:
					m_creature->CastStop();
					SetEquipmentSlots(false, -1, -1, -1);
					DoCast(m_creature, SPELL_BROKEN_FM_THROW);
					DoScriptText(SAY_OUTRO_LK_7, m_creature);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 500;
					break;
				case 12:
					DoCast(m_creature, SPELL_BROKEN_FM_SUMMON);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1500;
					break;
				case 13:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
					{
						pTirion->GetMotionMaster()->MovePoint(0, 529.584f, -2124.953f, 1040.86f);
						DoScriptText(SAY_OUTRO_TIRION_8, pTirion);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 14:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
					{
						pTirion->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
						m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTirion->GetGUID());
					}
					DoCast(m_creature, SPELL_SUMMON_TERENAS);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 15:
					if(Creature* pTerenas = m_creature->GetMap()->GetCreature(m_uiTerenasGUID))
						DoScriptText(SAY_OUTRO_TERENAS_9, pTerenas);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 9000;
					break;
				case 16:
					if(Creature* pTerenas = m_creature->GetMap()->GetCreature(m_uiTerenasGUID))
					{
						DoScriptText(SAY_OUTRO_TERENAS_10, pTerenas);
						pTerenas->CastSpell(pTerenas, SPELL_MASS_RESURRECTION, false);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
					break;
				case 17:
					DoRevivePlayers();
					DoScriptText(SAY_OUTRO_LK_11, m_creature);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 9000;
					break;
				case 18:
					DoScriptText(SAY_OUTRO_LK_12, m_creature);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			break;
        }
    }
};

CreatureAI* GetAI_boss_the_lich_king(Creature* pCreature)
{
    return new boss_the_lich_kingAI(pCreature);
}

#define GOSSIP_START     "Let's finish this!"

/*######
### Tirion
######*/

bool GossipHello_npc_tirion_final(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(pInstance->GetData(TYPE_LICH_KING) != DONE)
	    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_tirion_final(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
		pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		ScriptedInstance* m_pInstance;
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

		if (Creature* pLichKing = pCreature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LICH_KING)))
			((boss_the_lich_kingAI*)pLichKing->AI())->m_uiPhase = PHASE_INTRO;
	}

    return true;
}

 
/*######
### Shambling Horror
######*/

struct MANGOS_DLL_DECL mob_shambling_horrorAI : public ScriptedAI
{
    mob_shambling_horrorAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiShockwaveTimer;
	uint32 m_uiEnrageTimer;

    void Reset() 
	{
		m_uiShockwaveTimer	= 10000;
		m_uiEnrageTimer		= 30000;
		m_creature->SetRespawnDelay(DAY);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiShockwaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHOCKWAVE);
			m_uiShockwaveTimer = 10000;
		}
		else m_uiShockwaveTimer -= uiDiff;

		if(m_uiEnrageTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ENRAGE);
			m_uiEnrageTimer = 30000;
		}
		else m_uiEnrageTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_shambling_horror(Creature* pCreature)
{
    return new mob_shambling_horrorAI(pCreature);
}

 
/*######
### valkyr shadowguard
######*/

struct MANGOS_DLL_DECL mob_valkyr_shadowguardAI : public ScriptedAI
{
    mob_valkyr_shadowguardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	uint32 m_uiGrabTimer;
	uint64 m_uiPlayerGUID;
	bool m_bSetPath;
	float m_fHomeX;
	float m_fHomeY;
	float m_fDist[20];
	float m_fCoordX[20];
	float m_fCoordY[20];
	float m_fDistMin;

    void Reset() 
	{
		memset(&m_fDist, 0, sizeof(m_fDist));
		memset(&m_fCoordX, 0, sizeof(m_fCoordX));
		memset(&m_fCoordY, 0, sizeof(m_fCoordY));

		m_uiGrabTimer		= 5000;
		m_uiPlayerGUID		= 0;
		m_bSetPath			= false;
		m_fHomeX			= 0;
		m_fHomeY			= 0;
		m_fDistMin			= 0;
		m_creature->SetRespawnDelay(DAY);
		DoCast(m_creature, SPELL_WINGS_OF_DAMMED);

		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
		m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
		m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(),  m_creature->GetPositionZ() + 10, 0);
		m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(),  m_creature->GetPositionZ() + 10, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void SetDestination()
	{
		// get some random points
		float fDist = 0;
		for(int i = 0; i < 20; i++)
		{
			float angle = (float) rand()*360/RAND_MAX + 1;
			m_fHomeX = 505.212f + 35*cos(angle*(M_PI/180));
			m_fHomeY = -2124.350f + 35*sin(angle*(M_PI/180));
			fDist = sqrt(pow((m_fHomeX - 505.212f), 2) - pow((m_fHomeY + 2124.350f), 2));
			m_fCoordY[i] = m_fHomeY;
			m_fCoordX[i] = m_fHomeX;
			m_fDist[i] = fDist;
		}

		// set min
		m_fDistMin = m_fDist[0];
		uint8 vectLoc = 0;
		for(int i = 0; i < 20; i++)
		{
			if(m_fDist[i] < m_fDistMin)
			{
				m_fDistMin = m_fDist[i];
				vectLoc = 0;
			}
		}
		// set dest
		m_creature->GetMotionMaster()->MovePoint(0, m_fCoordX[vectLoc], m_fCoordY[vectLoc], m_creature->GetPositionZ());
	}

	void MovementInform(uint32 uiType, uint32 uiPointId)
	{
		if(uiType != POINT_MOTION_TYPE)
			return;

		if(uiPointId == 1)
		{
			if(Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
				pPlayer->TeleportTo(m_creature->GetMapId(), m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0);
			m_uiGrabTimer = 3000;
			m_bSetPath = false;
		}
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(!m_bSetPath)
		{
			if (Creature* pLichKing = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LICH_KING)))
			{
				if (Unit* pTarget = pLichKing->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				{
					m_creature->GetMotionMaster()->MovePoint(0, pTarget->GetPositionX(), pTarget->GetPositionY(), m_creature->GetPositionZ());
					m_uiPlayerGUID = pTarget->GetGUID();
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
						DoCast(pTarget, SPELL_LIFE_SIMPHON);
				}
			}
			m_uiGrabTimer = 3000;
			m_bSetPath = true;
		}

		if(m_uiGrabTimer < uiDiff && m_bSetPath)
		{
			SetDestination();
			m_uiGrabTimer = 10000;
		}
		else m_uiGrabTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_valkyr_shadowguard(Creature* pCreature)
{
    return new mob_valkyr_shadowguardAI(pCreature);
}

/*######
### vile spirit
######*/

struct MANGOS_DLL_DECL mob_icc_vile_spiritAI : public ScriptedAI
{
    mob_icc_vile_spiritAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiSpiritBurstTimer;
	uint32 m_uiPreBurstTimer;

    void Reset() 
	{
		m_uiSpiritBurstTimer = 30000;
		m_uiPreBurstTimer	= 27000;
		//m_creature->GetMotionMaster()->MoveConfused();
		m_creature->SetRespawnDelay(DAY);
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_uiPreBurstTimer < uiDiff)
		{
			m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()-13, 0);
			m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()-13, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
			m_uiPreBurstTimer = 10000;
		}
		else m_uiPreBurstTimer -= uiDiff;

		if(m_uiSpiritBurstTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SPIRIT_BURST);
			m_uiSpiritBurstTimer = 10000;
		}
		else m_uiSpiritBurstTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_icc_vile_spirit(Creature* pCreature)
{
    return new mob_icc_vile_spiritAI(pCreature);
}

/*######
### raging spirit
######*/

struct MANGOS_DLL_DECL mob_icc_raging_spiritAI : public ScriptedAI
{
    mob_icc_raging_spiritAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiSoulShriekTimer;

    void Reset() 
	{
		m_uiSoulShriekTimer = 2000;
		DoCast(m_creature, SPELL_RAGING_SPIRIT_VISUAL);
		m_creature->SetRespawnDelay(DAY);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiSoulShriekTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SOUL_SHRIEK);
			m_uiSoulShriekTimer = 4000;
		}
		else m_uiSoulShriekTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_icc_raging_spirit(Creature* pCreature)
{
    return new mob_icc_raging_spiritAI(pCreature);
}

/*######
### ice sphere
######*/

struct MANGOS_DLL_DECL mob_icc_ice_sphereAI : public ScriptedAI
{
    mob_icc_ice_sphereAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiIcePulseTimer;
	uint32 m_uiIceBurstTimer;

    void Reset() 
	{
		m_uiIcePulseTimer	= 3000;
		m_uiIceBurstTimer	= 5000;
		DoCast(m_creature, SPELL_ICE_SPHERE_VISUAL);
		m_creature->SetRespawnDelay(DAY);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiIcePulseTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_ICE_PULSE_TRIGG);
			m_uiIcePulseTimer = 3000;
		}
		else m_uiIcePulseTimer -= uiDiff;

		if(m_uiIceBurstTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ICE_BURST);
			m_uiIceBurstTimer = 7000;
		}
		else m_uiIceBurstTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_icc_ice_sphere(Creature* pCreature)
{
    return new mob_icc_ice_sphereAI(pCreature);
}

struct MANGOS_DLL_DECL mob_defile_targetAI : public ScriptedAI
{
    mob_defile_targetAI(Creature *pCreature) : ScriptedAI(pCreature)
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

	uint32 m_uiDespawnTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_DEFILE_TRIGG);
        m_creature->SetRespawnDelay(DAY);
		m_uiDespawnTimer = 60000;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_uiDespawnTimer < uiDiff)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDespawnTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_defile_target(Creature* pCreature)
{
    return new mob_defile_targetAI (pCreature);
}

struct MANGOS_DLL_DECL mob_frostmourneDummyAI : public ScriptedAI
{
    mob_frostmourneDummyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

	uint32 m_uiSpellTimer;

    void Reset()
    {
		DoCast(m_creature, SPELL_BROKEN_FM_OBJECT);
        m_creature->SetRespawnDelay(DAY);
		m_uiSpellTimer = 500;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if(m_uiSpellTimer < uiDiff)
		{	
			DoCast(m_creature, SPELL_BROKEN_FM_RELEASE);
			m_uiSpellTimer = 600000;
		}
		else m_uiSpellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_frostmourneDummy(Creature* pCreature)
{
    return new mob_frostmourneDummyAI (pCreature);
}

void AddSC_boss_the_lich_king()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_the_lich_king";
    NewScript->GetAI = &GetAI_boss_the_lich_king;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "npc_tirion_final";
    NewScript->pGossipHello = &GossipHello_npc_tirion_final;
    NewScript->pGossipSelect = &GossipSelect_npc_tirion_final;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_shambling_horror";
    NewScript->GetAI = &GetAI_mob_shambling_horror;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_valkyr_shadowguard";
    NewScript->GetAI = &GetAI_mob_valkyr_shadowguard;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_icc_vile_spirit";
    NewScript->GetAI = &GetAI_mob_icc_vile_spirit;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_icc_raging_spirit";
    NewScript->GetAI = &GetAI_mob_icc_raging_spirit;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_icc_ice_sphere";
    NewScript->GetAI = &GetAI_mob_icc_ice_sphere;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "npc_terenas_menethil";
    NewScript->GetAI = &GetAI_npc_terenas_menethil;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_spirit_warden";
    NewScript->GetAI = &GetAI_mob_spirit_warden;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_defile_target";
    NewScript->GetAI = &GetAI_mob_defile_target;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "mob_frostmourneDummy";
    NewScript->GetAI = &GetAI_mob_frostmourneDummy;
    NewScript->RegisterSelf();
}

