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

	// phase 2
    SPELL_SUMMON_VALKYR			= 69037,
	SPELL_SUMMON_VALKYR_TRIGG	= 74361,
	NPC_VALKYR_SHADOWGUARD		= 36609,
    SPELL_SOUL_REAPER			= 69409,
	SPELL_SOUL_REAPER_HASTE		= 69410,	// used on LK
    SPELL_DEFILE				= 72754,
	SPELL_DEFILE_TRIGG			= 72743,
	NPC_DEFILE					= 38757,

	// phase 3
    SPELL_HARVEST_SOUL			= 68980,
	SPELL_HARVEST_SOUL_STRANG	= 68986,	// uses vehicles
	SPELL_HARVEST_SOUL_TELE		= 73655,	// tele and adds periodic dmg
    SPELL_VILE_SPIRITS			= 70498,
    NPC_VILE_SPIRIT				= 37799,

	// frostmourne
	NPC_TERENAS_MENETHIL		= 36823,
    SPELL_LIGHTS_FAVOR			= 69382,	// pasive heal on terenas
    SPELL_RESTORE_SOUL			= 72595,	// restore the player
	NPC_SPIRIT_WARDEN			= 36824,
    SPELL_SOUL_RIP				= 69397,	// on terenas
    SPELL_DARK_HUNGER			= 69383,
    SPELL_HARVESTED_SOUL		= 74322,	// on frostmourne

	// transition
    SPELL_REMORSELESS_WINTER	= 68981,
    SPELL_PAIN_AND_SUFFERING	= 72133,
    SPELL_SUMMON_ICE_SPHERE		= 69103,
	SPELL_ICE_SPHERE_VISUAL		= 69104,
	NPC_ICE_SPHERE				= 36633,
    SPELL_RAGING_SPIRIT			= 69200, 
	NPC_RAGING_SPIRIT			= 36701,
    SPELL_QUAKE					= 72262,

	// other
    SPELL_FURY_FROSTMOURNE		= 72350,
	SPELL_BERSERK				= 47008,
	SPELL_ICE_LOCK				= 71614,
	SPELL_RAISE_DEAD			= 71769,
	NPC_TERENAS_END				= 38579,
	SPELL_MASS_RESURRECTION		= 72429,
    
	// shambling horror
	SPELL_SHOCKWAVE				= 72149,
	SPELL_ENRAGE				= 72143,

	// valkyr shadowguard
	SPELL_GRAB					= 62712,	// fix id
	SPELL_WINGS_OF_DAMMED		= 74352,

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
    
    uint32 m_uiBerserkTimer;
    uint8 m_uiPhase;
	bool m_bIsFirstTime;

    void Reset()
    {
        m_uiBerserkTimer			= 600000;
		// phase 1
        m_uiSummonHorrorTimer		= 10000;
        m_uiInfestTimer				= 20000;
        m_uiSummonGhoulsTimer		= 15000;
		m_uiNecroticPlagueTimer		= 5000;
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
            m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
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

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, FAIL);

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
        if (pList.isEmpty()) return;
        for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
             if (Player* pPlayer = i->getSource())
                 if (pPlayer && pPlayer->isAlive() && pPlayer->IsInMap(m_creature))
                     pPlayer->SendMovieStart(FINAL_ARTHAS_MOVIE);
    }

    void DoRevivePlayers()
    {
        Map::PlayerList const &pList = m_creature->GetMap()->GetPlayers();
        if (pList.isEmpty()) return;
        for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
            {
               if (Player* pPlayer = i->getSource())
               {
                   if (pPlayer && !pPlayer->isAlive() /*&& pPlayer->IsInMap(pMenethil)*/)
                   {
                       //pMenethil->CastSpell(pPlayer, SPELL_REVALL, true);
                       pPlayer->ResurrectPlayer(100, false);
                   }
                }
             };
    }

    void JustDied(Unit* pKiller)
    {
		DoSendCinematic();
		DoScriptText(SAY_OUTRO_LK_13, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, DONE);
    }

	void JustSummoned(Creature* pSummoned)
	{
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
					m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH_NOSHEATHE);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 22000;
					break;
				case 5:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoScriptText(SAY_INTRO_TIRION_4, pTirion);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 2000;
					break;
				case 6:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->HandleEmoteCommand(EMOTE_ONESHOT_POINT_NOSHEATHE);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 7:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						pTirion->GetMotionMaster()->MovePoint(0, 478.948f, -2124.756f, 1040.859f);
					// cast some summon ghoul spell
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 8:
					if (Creature* pTirion = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TIRION_FINAL)))
						DoCast(pTirion, SPELL_ICE_LOCK);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 9:
					DoScriptText(SAY_INTRO_LK_5, m_creature);
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

			if(m_creature->GetHealthPercent() < 70.0f)
			{
				m_uiPhase = PHASE_TRANS_ONE;
				m_uiPhaseEndTimer = 60000;
			}
            
			DoMeleeAttackIfReady();
            
			break; 

        case PHASE_TWO:

            if (m_uiSummonValkyrTimer < uiDiff)
            {
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
                DoCast(m_creature, SPELL_DEFILE_TRIGG);
                m_uiDefileTimer = 35000;
            } 
			else m_uiDefileTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 40.0f)
			{
				m_uiPhase = PHASE_TRANS_TWO;
				m_uiPhaseEndTimer = 60000;
			}

            DoMeleeAttackIfReady();

            break;

        case PHASE_THREE:

            if (m_uiDefileTimer < uiDiff)
            {
				m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_DEFILE_TRIGG);
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
				// ToDo:
				//if(Player* pTarget = m_creature->GetMap()->GetPlayer(m_uiHarvestTargetGUID))
				//{
				//	if(pTarget->isAlive())
				//		pTarget->TeleportTo(631, 529.3f, -2124.7f, 1050.0f, 3.14159f);
				//}
				m_uiHarvestEndTimer = 600000;
			}
			else m_uiHarvestEndTimer -= uiDiff;

            if (m_uiVileSpiritsTimer < uiDiff) 
            {
				m_creature->InterruptNonMeleeSpells(true);
				// not working, needs override
                DoCast(m_creature, SPELL_VILE_SPIRITS);
                m_uiVileSpiritsTimer = 30000;
            } 
			else m_uiVileSpiritsTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 10.0f)
			{
				m_uiPhase = PHASE_FINAL;
				m_uiSpeechTimer = 1000;
				m_uiSpeechPhase = 0;
			}

            DoMeleeAttackIfReady();

            break;

        case PHASE_TRANS_ONE:

			if (m_uiPhaseEndTimer <= uiDiff)
			{
				DoCast(m_creature, SPELL_QUAKE);
				m_uiPhase = PHASE_TWO;
				m_uiPhaseEndTimer = 60000;
				break;
			} 
			else m_uiPhaseEndTimer -= uiDiff;

			if (m_uiRemorselessWinterTimer < uiDiff) 
			{
                DoCast(m_creature, SPELL_REMORSELESS_WINTER);
                m_uiRemorselessWinterTimer = 60000;
            } 
			else m_uiRemorselessWinterTimer -= uiDiff; 

            if (m_uiPainAndSufferingTimer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                m_uiPainAndSufferingTimer = 3000;
            } 
			else m_uiPainAndSufferingTimer -= uiDiff;

            if (m_uiSummonIceSphereTimer < uiDiff) 
            {
                DoCast(m_creature, SPELL_ICE_SPHERE_VISUAL);
                m_uiSummonIceSphereTimer = 5000;
            } 
			else m_uiSummonIceSphereTimer -= uiDiff;

            if (m_uiRagingSpiritTimer < uiDiff) 
            {
                DoCast(m_creature, SPELL_RAGING_SPIRIT);
                m_uiRagingSpiritTimer = 20000;
            } 
			else m_uiRagingSpiritTimer -= uiDiff;

            break;

        case PHASE_TRANS_TWO:

			if (m_uiPhaseEndTimer <= uiDiff)
			{
				DoCast(m_creature, SPELL_QUAKE);
				m_uiPhase = PHASE_THREE;
				m_uiPhaseEndTimer = 60000;
				break;
			} 
			else m_uiPhaseEndTimer -= uiDiff;

			if (m_uiRemorselessWinterTimer < uiDiff) 
			{
                DoCast(m_creature, SPELL_REMORSELESS_WINTER);
                m_uiRemorselessWinterTimer = 60000;
            } 
			else m_uiRemorselessWinterTimer -= uiDiff; 

            if (m_uiPainAndSufferingTimer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                m_uiPainAndSufferingTimer = 3000;
            } 
			else m_uiPainAndSufferingTimer -= uiDiff;

            if (m_uiSummonIceSphereTimer < uiDiff) 
            {
                DoCast(m_creature, SPELL_ICE_SPHERE_VISUAL);
                m_uiSummonIceSphereTimer = 5000;
            } 
			else m_uiSummonIceSphereTimer -= uiDiff;

            if (m_uiRagingSpiritTimer < uiDiff) 
            {
                DoCast(m_creature, SPELL_RAGING_SPIRIT);
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
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 3000;
                    break;
                case 1:
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 1000;
                    break;
                case 2:
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 10000;
                    break;
				case 3:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 4:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 13000;
					break;
				case 5:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 3000;
					break;
				case 6:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 7:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 8:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
					break;
				case 9:
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
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
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiGrabTimer;

    void Reset() 
	{
		m_uiGrabTimer		= 5000;
		m_creature->SetRespawnDelay(DAY);
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_WINGS_OF_DAMMED);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiGrabTimer < uiDiff)
		{
			// grab event here
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

    void Reset() 
	{
		m_uiSpiritBurstTimer = 30000;
		m_creature->SetRespawnDelay(DAY);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

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
		m_uiSoulShriekTimer = 10000;
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
			m_uiSoulShriekTimer = 10000;
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
		m_uiIcePulseTimer	= 10000;
		m_uiIceBurstTimer	= 20000;
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
			m_uiIcePulseTimer = 10000;
		}
		else m_uiIcePulseTimer -= uiDiff;

		if(m_uiIceBurstTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ICE_BURST);
			m_uiIceBurstTimer = 20000;
		}
		else m_uiIceBurstTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_icc_ice_sphere(Creature* pCreature)
{
    return new mob_icc_ice_sphereAI(pCreature);
}

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

    void Reset() 
	{
		m_creature->SetRespawnDelay(DAY);
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_LIGHTS_FAVOR);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// restore soul event here

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

    void Reset() 
	{
		m_creature->SetRespawnDelay(DAY);
	}

    void UpdateAI(const uint32 uiDiff) 
	{
		if (m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// fight with terenas here

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_spirit_warden(Creature* pCreature)
{
    return new mob_spirit_wardenAI(pCreature);
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
}

