/* Copyright 2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
 
/* ScriptData
SDName: boss_kiljaden
SD%Complete: 90%
SDComment: 
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"
 
enum Speeches
{
    /*** Speech and sounds***/
    // These are used throughout Sunwell and Magisters(?). Players can hear this while running through the instances.
    SAY_OFFCOMBAT1							= -1580056,
    SAY_OFFCOMBAT2							= -1580057,
    SAY_OFFCOMBAT3							= -1580058,
    SAY_OFFCOMBAT4							= -1580059,
    SAY_OFFCOMBAT5							= -1580060,

    // Encounter speech and sounds
    SAY_EMERGE								= -1580061,
    SAY_SLAY1								= -1580062,
    SAY_SLAY2								= -1580063,
    SAY_REFLECTION1							= -1580064,
    SAY_REFLECTION2							= -1580065,
    SAY_DARKNESS1							= -1580066,
    SAY_DARKNESS2							= -1580067,
    SAY_DARKNESS3							= -1580068,
    SAY_PHASE_FOUR	                        = -1580070,
    SAY_PHASE_THREE                         = -1580069,
    SAY_PHASE_FIVE	                        = -1580071,

    // Kalecgos - Anveena speech
    SAY_KALECGOS_INTRO                      = -1580072,
	// phase 3
    SAY_KALECGOS_AWAKEN                     = -1580073,
    SAY_ANVEENA_IMPRISONED                  = -1580074,
	// phase 4
    SAY_KALECGOS_LETGO                      = -1580075,
    SAY_ANVEENA_LOST                        = -1580076,
	// phase 5
    SAY_KALECGOS_FOCUS                      = -1580077,
    SAY_ANVEENA_KALEC                       = -1580078,
    SAY_KALECGOS_FATE                       = -1580079,
    SAY_ANVEENA_GOODBYE                     = -1580080,
    SAY_KALECGOS_GOODBYE                    = -1580081,
    SAY_KALECGOS_ENCOURAGE                  = -1580082,

	// orbs
    SAY_KALECGOS_ORB1                       = -1580083,
    SAY_KALECGOS_ORB2                       = -1580084,
    SAY_KALECGOS_ORB3                       = -1580085,
    SAY_KALECGOS_ORB4                       = -1580086,

    /*** Outro Sounds***/
	SAY_OUTRO_01_VELEN		= -1580087,
	SAY_OUTRO_02_VELEN		= -1580088,
	SAY_OUTRO_03_VELEN		= -1580089,
	SAY_OUTRO_04_VELEN		= -1580090,
	SAY_OUTRO_05_LIADRIN	= -1580091,
	SAY_OUTRO_06_VELEN		= -1580092,
	SAY_OUTRO_07_LIADRIN	= -1580093,
	SAY_OUTRO_08_VELEN		= -1580094,
	SAY_OUTRO_09_VELEN		= -1580095,
	SAY_OUTRO_10_LIADRIN	= -1580096,
	SAY_OUTRO_11_VELEN		= -1580097,
	SAY_OUTRO_12_VELEN		= -1580098,

	// spells
	
    //Kilajden Spells
	// generic
    SPELL_REBIRTH               = 45464, // Emerge from the Sunwell Epick :D
	SPELL_DESTROY_DRAKES        = 46707,

	// phase 2
    SPELL_SOULFLY               = 45442,
	SPELL_LEGION_LIGHTING       = 45664,
	SPELL_FIREBLOOM             = 45641,

	// phase 3
	SPELL_SINISTER_REFLECTION   = 45892,
	SPELL_SHADOW_SPIKE			= 46680,
	SPELL_SHADOW_SPIKE_MISSLE	= 46589,
	SPELL_FLAMEDART				= 45740,
	SPELL_DARKNESS_EXPLOSION    = 45657, 
	SPELL_DARKNESS_OF_SOULS     = 46605,

	// phase 4
	SPELL_ARMAGEDDON_TRIGG		= 45921,
	SPELL_ARMAGEDDON_VISUAL		= 45909,
	SPELL_ARMAGEDDON_DMG		= 45915,
	SPELL_ARMAGEDDON_HELLFIRE	= 45911,
	SPELL_ARMAGEDDON_SPARK		= 45914,

	// phase 5
	// Same spells with lower timers

    //Hand of the Deceiver Spells
    SPELL_FELLFIRE_PORTAL       = 46875,
    SPELL_SHADOWBOLT_VOLLEY     = 45770,
    SPELL_SHADOW_INFUSION       = 45772,
    SPELL_SHADOW_CHANNELING     = 46757,
	SPELL_SUMMON_FELFIRE_FIEND	= 46464,

	// Felfire fiend
	SPELL_FELFIRE_FISSION		= 45779,

    // Anveena
    SPELL_ANVEENA_PRISON		= 46367,
    SPELL_ANVEENA_DRAIN			= 46410,
	SPELL_SUNWELL_IGNITION		= 46822,
	SPELL_SACRIFICE_OF_ANVEENA  = 46474,
	SPELL_ENTROPIUS_BODY		= 46819,
	SPELL_CALL_ENTROPIUS		= 46818,

    //Orb Spells
    SPELL_SHADOW_ORB_BOLT_TRIGG = 45679, 

    //Reflections Spells todo:
    SPELL_PRIEST                = 47077,
    SPELL_PALADIN               = 37369,
    SPELL_PALADIN2              = 37369,
    SPELL_WARLOCK               = 46190,
    SPELL_WARLOCK2              = 47076,
    SPELL_MAGE                  = 47074,
    SPELL_ROGUE                 = 45897,
    SPELL_WARRIOR               = 17207,
    SPELL_DRUID                 = 47072,
    SPELL_SHAMAN                = 47071,
    SPELL_HUNTER                = 48098,

	// npcs
	// mics
    NPC_ANVEENA			= 26046, // Embodiment of the Sunwell
    NPC_KALECGOS		= 25319, // Helps the raid throughout the fight
	NPC_BLUE_DRAGON		= 25653, // controlled by players
    NPC_VELEN			= 26246, // outro
    NPC_LIADRIN			= 26247, // outro
	NPC_VELEN_PORTAL	= 24925,
	NPC_CORE_ENTROPIUS  = 26262, // Used in the ending cinematic?

	// encounter
    NPC_DECIVER			= 25588,
	NPC_FELFIRE_PORTAL	= 25603,
    NPC_SHIELD_ORB		= 25502,
    NPC_SINISTER		= 25708, // Sinister Reflection
    NPC_ARMAGEDON		= 25735,
    NPC_FELFIRE_FIEND	= 25598,
    NPC_SHADOWSPIKE		= 30598,

	// objects
	NPC_BLUE_ORB_TARGET	= 25640,
	ORB_BLUE_DRAGONFLIGHT				= 188415,
	SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT  = 45839, // Possess the blue dragon from the orb to help the raid.

	// phases
	PHASE_IDLE			= 0,
	PHASE_ONE			= 1,
	PHASE_TWO			= 2,
	PHASE_THREE			= 3,
	PHASE_FOUR			= 4,
	PHASE_FIVE			= 5,
	PHASE_OUTRO			= 6,
};

/*######
## boss_kiljaeden
######*/
struct MANGOS_DLL_DECL boss_kiljaedenAI : public Scripted_NoMovementAI
{
    boss_kiljaedenAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
 
    ScriptedInstance* m_pInstance;

	// phase idle
    uint32 m_uiOrdersTimer;

	// phase one
	uint32 m_uiDecieverDead;
	uint64 m_uiTargetGUID;
	uint32 m_uiEmergeTimer;
	bool m_bHasStarted;

	// phase two
	uint32 m_uiSoulFlyTimer;
    uint32 m_uiLegionLightingTimer;
    uint32 m_uiFireBloomTimer;
    uint32 m_uiShieldOrbTimer;
	uint32 m_uiMaxShieldOrbs;

	// phase three
	uint32 m_uiShadowSpikeTimer;
	uint32 m_uiFlameDartTimer;
	uint32 m_uiDarknessOfSoulsTimer;
	uint32 m_uiDarknessExplosionTimer;
	bool m_bIsDarknessOfSoulsCasting;
	bool m_bShadowSpikeReady;

	// phase four
	uint32 m_uiAramageddonTimer;
	uint32 m_uiMaxArmageddon;
 
    void Reset()
    {
		// phase idle
		m_uiOrdersTimer				= 10000;

		// phase one
		m_uiDecieverDead			= 0;
		m_bHasStarted				= false;
		m_uiTargetGUID				= 0;
		m_uiEmergeTimer				= 1000;

		// phase two
        m_uiSoulFlyTimer			= 5000;
        m_uiLegionLightingTimer		= 10000;
        m_uiFireBloomTimer			= 20000;
        m_uiShieldOrbTimer			= 25000;
		m_uiMaxShieldOrbs			= 1;

		// phase three
		m_bIsDarknessOfSoulsCasting  = false;
		m_uiDarknessExplosionTimer	= 120000;
		m_uiShadowSpikeTimer		= 2000;
		m_uiFlameDartTimer			= 20000;
		m_uiDarknessOfSoulsTimer	= 45000;
		m_bShadowSpikeReady			= true;

		// phase four
		m_uiAramageddonTimer		= 23000;      

        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		
		if(m_pInstance)
			m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_IDLE);
    }
    
	void Aggro(Unit *who) 
	{
		m_creature->SetInCombatWithZone();
		m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_TWO);
	}

	void JustSummoned(Creature* pSummon)
	{
		if(pSummon->GetEntry() != NPC_ARMAGEDON)
			pSummon->SetInCombatWithZone();
	}

	void AttackStart(Unit* pWho)
    {
		if(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_IDLE || m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_ONE)
			return;

        if(!pWho)
            return;

		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoCast(m_creature, SPELL_REBIRTH);
			DoScriptText(SAY_EMERGE, m_creature);
		}
    }

    void JustReachedHome()
    {
		if(m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, NOT_STARTED);

		if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
		{
			if(!pAnveena->isAlive())
				pAnveena->Respawn();
		}

		if(Creature* pController = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN_CONTROLLER)))
		{
			if(pController->isAlive())
				pController->AI()->EnterEvadeMode();
		}

		std::list<Creature*> lDecievers;
		GetCreatureListWithEntryInGrid(lDecievers, m_creature, NPC_DECIVER, 40.0f);
		if (!lDecievers.empty())
		{
			for(std::list<Creature*>::iterator iter = lDecievers.begin(); iter != lDecievers.end(); ++iter)
			{
				if ((*iter) && !(*iter)->isAlive())
					(*iter)->Respawn();
			}
		}
    }

    void KilledUnit(Unit* pVictim) 
    {
        switch (rand()%2)
        {  
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void DoSinisterReflection(uint64 uiVictimGUID)
    {
		Unit* pVictim = m_creature->GetMap()->GetUnit(uiVictimGUID);
		if(!pVictim)
			return;

		uint32 spell;
        switch(pVictim->getClass())
        {
			// ToDo:
            case CLASS_PRIEST:  spell = SPELL_PRIEST; break;
            case CLASS_PALADIN: spell = SPELL_PALADIN; break;
            case CLASS_WARLOCK: spell = SPELL_WARLOCK; break;
            case CLASS_MAGE:    spell = SPELL_MAGE; break;
            case CLASS_ROGUE:   spell = SPELL_ROGUE; break;
            case CLASS_WARRIOR: spell = SPELL_WARRIOR; break;
            case CLASS_DRUID:   spell = SPELL_DRUID; break;
            case CLASS_SHAMAN:  spell = SPELL_SHAMAN; break;
            case CLASS_HUNTER:  spell = SPELL_HUNTER; break;
        }

        for(uint8 i = 0; i < 4; ++i)
        {
            if(Creature* pSinister = m_creature->SummonCreature(NPC_SINISTER, pVictim->GetPositionX()+rand()%3, pVictim->GetPositionY()+rand()%3, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
            {
                pSinister->setFaction(14);
                pSinister->SetDisplayId(pVictim->GetDisplayId());
                pSinister->AI()->AttackStart(pSinister->getVictim());
            }
        }
    }

    void JustDied(Unit* pKiller) 
    {
        if(m_pInstance)
		{
            m_pInstance->SetData(TYPE_KILJAEDEN, DONE);
			m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_OUTRO);
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if((!m_creature->SelectHostileTarget() || !m_creature->getVictim()) &&
			m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) != PHASE_IDLE && m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) != PHASE_ONE)
			return;

		switch(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE))
		{

		case PHASE_FIVE:
			// nothing here, but the timers are reduced
			// no break;
		case PHASE_FOUR:

			if(m_uiAramageddonTimer < uiDiff && !m_bIsDarknessOfSoulsCasting)
			{
				if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					m_creature->SummonCreature(NPC_ARMAGEDON, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
				if(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_FIVE)
					m_uiAramageddonTimer = 1000;
				else
					m_uiAramageddonTimer = 3000;
			}
			else m_uiAramageddonTimer -= uiDiff;     

			if(m_creature->GetHealthPercent() < 25.0f && m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_FOUR)
			{
				m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_FIVE);
				m_uiShadowSpikeTimer = 2000;
				m_uiDarknessOfSoulsTimer = 80000;
				m_bShadowSpikeReady = true;
				// sinister reflection
				switch (rand()%2)
				{  
				case 0: DoScriptText(SAY_REFLECTION1, m_creature); break;
				case 1: DoScriptText(SAY_REFLECTION2, m_creature); break;
				}
				for(uint8 i = 0; i < 3; ++i)
				{
					if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					{
						DoCast(pTarget, SPELL_SINISTER_REFLECTION);
						DoSinisterReflection(pTarget->GetGUID());
					}
				}
			}

			// no break;
		case PHASE_THREE:

			if(m_uiDarknessExplosionTimer < uiDiff && m_bIsDarknessOfSoulsCasting)
			{
				switch (rand()%3)
				{
				case 0: DoScriptText(SAY_DARKNESS1, m_creature); break;
				case 1: DoScriptText(SAY_DARKNESS2, m_creature); break;
				case 2: DoScriptText(SAY_DARKNESS3, m_creature); break;
				}
				m_creature->CastStop();
				DoCast(m_creature, SPELL_DARKNESS_EXPLOSION);
				m_bIsDarknessOfSoulsCasting = false;
				m_uiDarknessExplosionTimer = 60000;
				if(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_FIVE)
					m_uiDarknessOfSoulsTimer = 25000;
				else
					m_uiDarknessOfSoulsTimer = 45000;
				m_uiFireBloomTimer = 10000;
				m_uiAramageddonTimer = 2000;
			}
			else m_uiDarknessExplosionTimer -= uiDiff;

			if(m_bIsDarknessOfSoulsCasting)
				return;

			if(m_uiDarknessOfSoulsTimer < uiDiff)
			{
				m_creature->CastStop();
				DoCast(m_creature, SPELL_DARKNESS_OF_SOULS);
				m_bIsDarknessOfSoulsCasting = true;
				m_uiDarknessExplosionTimer = 9000;
			}
			else m_uiDarknessOfSoulsTimer -= uiDiff;

			if(m_uiShadowSpikeTimer < uiDiff && m_bShadowSpikeReady)
			{
				m_creature->CastStop();
				DoCast(m_creature, SPELL_SHADOW_SPIKE);
				m_bShadowSpikeReady = false;
			}
			else m_uiShadowSpikeTimer -= uiDiff;

			if(m_uiFlameDartTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				DoCast(m_creature, SPELL_FLAMEDART);
				m_uiFlameDartTimer = 17000;
			}
			else m_uiFlameDartTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 55.0f && m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_THREE)
			{
				m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_FOUR);
				m_uiMaxShieldOrbs += 1;
				m_uiShadowSpikeTimer = 2000;
				m_uiDarknessOfSoulsTimer = 45000;
				m_bShadowSpikeReady = true;
				// sinister reflection
				switch (rand()%2)
				{  
				case 0: DoScriptText(SAY_REFLECTION1, m_creature); break;
				case 1: DoScriptText(SAY_REFLECTION2, m_creature); break;
				}
				for(uint8 i = 0; i < 2; ++i)
				{
					if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					{
						DoCast(pTarget, SPELL_SINISTER_REFLECTION);
						DoSinisterReflection(pTarget->GetGUID());
					}
				}
			}

			// no break;
		case PHASE_TWO:

			if(m_uiLegionLightingTimer < uiDiff)
			{
				m_creature->InterruptNonMeleeSpells(true);
				if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_LEGION_LIGHTING);
				m_uiLegionLightingTimer = urand(6000, 8000);
			}
			else m_uiLegionLightingTimer -= uiDiff;

			if(m_uiFireBloomTimer < uiDiff)
			{
				DoCast(m_creature, SPELL_FIREBLOOM);
				if(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_FIVE)
					m_uiFireBloomTimer = 15000;
				else
					m_uiFireBloomTimer = 25000;
			}
			else m_uiFireBloomTimer -= uiDiff;

			if(m_uiSoulFlyTimer < uiDiff)
			{
				if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					DoCast(pTarget, SPELL_SOULFLY);
				m_uiSoulFlyTimer = urand(5000, 7000);
			}
			else m_uiSoulFlyTimer -= uiDiff;

			if(m_uiShieldOrbTimer < uiDiff)
			{
				for(uint8 i = 0; i < m_uiMaxShieldOrbs; ++i)
				{
					float angle = (float) rand()*360/RAND_MAX + 1;
					float homeX = m_creature->GetPositionX() + 20*cos(angle*(M_PI/180));
					float homeY = m_creature->GetPositionY() + 20*sin(angle*(M_PI/180));
					m_creature->SummonCreature(NPC_SHIELD_ORB, homeX, homeY, m_creature->GetPositionZ()+15, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
				}
				if(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_FIVE)
					m_uiShieldOrbTimer = 30000;
				else
					m_uiShieldOrbTimer = 50000; 
			}
			else m_uiShieldOrbTimer -= uiDiff;

			if(m_creature->GetHealthPercent() < 85.0f && m_pInstance->GetData(TYPE_KILJAEDEN_PHASE) == PHASE_TWO)
			{
				m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_THREE);
				m_uiMaxShieldOrbs += 1;
				// sinister reflection
				switch (rand()%2)
				{  
				case 0: DoScriptText(SAY_REFLECTION1, m_creature); break;
				case 1: DoScriptText(SAY_REFLECTION2, m_creature); break;
				}
				if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				{
					DoCast(pTarget, SPELL_SINISTER_REFLECTION);
					DoSinisterReflection(pTarget->GetGUID());
				}
			}

			DoMeleeAttackIfReady();

			break;
		case PHASE_ONE:

			if(m_uiDecieverDead == 3 && !m_bHasStarted)
			{
				m_creature->SetVisibility(VISIBILITY_ON);
				m_uiEmergeTimer = 500;
				m_bHasStarted = true;
			}

			if(m_uiEmergeTimer < uiDiff && m_bHasStarted)
			{
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->SetInCombatWithZone();
				if(Unit* pTarget = m_creature->GetMap()->GetUnit(m_uiTargetGUID))
				{
					m_creature->AddThreat(pTarget, 1000.0f);
					m_creature->SetInCombatWith(pTarget);
					m_creature->AI()->AttackStart(pTarget);
				}
			}
			else m_uiEmergeTimer -= uiDiff;

			// no break
		case PHASE_IDLE:

			if(m_uiOrdersTimer < uiDiff)
            {
		        switch (rand()%5)
		        {
			        case 0: DoScriptText(SAY_OFFCOMBAT1, m_creature); break;
			        case 1: DoScriptText(SAY_OFFCOMBAT2, m_creature); break;
			        case 2: DoScriptText(SAY_OFFCOMBAT3, m_creature); break;
                    case 3: DoScriptText(SAY_OFFCOMBAT4, m_creature); break;
                    case 4: DoScriptText(SAY_OFFCOMBAT5, m_creature); break;
		        }
                m_uiOrdersTimer = 30000;
            }
			else m_uiOrdersTimer -= uiDiff;

			break;
		}
    }
};

/*######
## mob_deciever
######*/
struct MANGOS_DLL_DECL mob_deceiverAI : public ScriptedAI
{
    mob_deceiverAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
 
    ScriptedInstance* m_pInstance;

	bool m_bHasInfusion;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiPortalTimer;
	bool m_bHasPortal;
	uint64 m_uiPortalGUID;
 
    void Reset()
    {
		DoCast(m_creature, SPELL_SHADOW_CHANNELING);
		m_bHasInfusion		= false;
        m_uiPortalTimer		= urand(20000, 30000);
        m_uiShadowBoltTimer = 10000;
		m_bHasPortal		= false;
		m_uiPortalGUID		= 0;
    }
    
    void Aggro(Unit* pWho) 
    {
		if (m_pInstance)
		{
            m_pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);
			m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_ONE);
		}

        m_creature->CastStop();

		std::list<Creature*> lDecievers;
		GetCreatureListWithEntryInGrid(lDecievers, m_creature, NPC_DECIVER, 40.0f);
		if (!lDecievers.empty())
		{
			for(std::list<Creature*>::iterator iter = lDecievers.begin(); iter != lDecievers.end(); ++iter)
			{
				if ((*iter) && (*iter)->isAlive())
					(*iter)->AI()->AttackStart(pWho);
			}
		}
	}

    void JustDied(Unit* pKiller) 
    {
		if (Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
		{
			if(pKiljaeden->isAlive())
			{
				((boss_kiljaedenAI*)pKiljaeden->AI())->m_uiDecieverDead += 1;
				((boss_kiljaedenAI*)pKiljaeden->AI())->m_uiTargetGUID = pKiller->GetGUID();
			}
		}
		if(Creature* pPortal = m_creature->GetMap()->GetCreature(m_uiPortalGUID))
			pPortal->ForcedDespawn();
    }

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_KILJAEDEN) != NOT_STARTED)
			{
				m_pInstance->SetData(TYPE_KILJAEDEN, NOT_STARTED);
				m_pInstance->SetData(TYPE_KILJAEDEN_PHASE, PHASE_IDLE);
			}
		}

		std::list<Creature*> lDecievers;
		GetCreatureListWithEntryInGrid(lDecievers, m_creature, NPC_DECIVER, 40.0f);
		if (!lDecievers.empty())
		{
			for(std::list<Creature*>::iterator iter = lDecievers.begin(); iter != lDecievers.end(); ++iter)
			{
				if ((*iter) && !(*iter)->isAlive())
					(*iter)->Respawn();
			}
		}
	}

	void JustSummoned(Creature* pSummon)
	{
		if(pSummon->GetEntry() == NPC_FELFIRE_PORTAL)
			m_uiPortalGUID = pSummon->GetGUID();
	}
     
	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_creature->GetHealthPercent() < 20.0f && !m_bHasInfusion)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_SHADOW_INFUSION);
			m_bHasInfusion = true;
		}

		if(m_uiShadowBoltTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_SHADOWBOLT_VOLLEY);
			m_uiShadowBoltTimer = urand(6000, 10000);
		}
		else m_uiShadowBoltTimer -= uiDiff;

		if(m_uiPortalTimer < uiDiff && !m_bHasPortal)
		{
			//DoCast(m_creature, SPELL_FELLFIRE_PORTAL);
			m_creature->SummonCreature(NPC_FELFIRE_PORTAL, 0, 0, 0, 0, TEMPSUMMON_MANUAL_DESPAWN, 0);
			m_uiPortalTimer = 30000;
			m_bHasPortal = true;
		}
		else m_uiPortalTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

/*######
## mob_shield_orb
######*/
struct MANGOS_DLL_DECL mob_shield_orbAI : public ScriptedAI
{
    mob_shield_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		SetCombatMovement(false);
        Reset();
    }

	ScriptedInstance* m_pInstance;

	uint32 m_uiMovementTimer;
    uint32 m_uiMovingSteps;
 
    void Reset()
    {
		DoCast(m_creature, SPELL_SHADOW_ORB_BOLT_TRIGG);
		m_creature->SetRespawnDelay(DAY);
		m_uiMovementTimer = 1000 + rand()%2000;
        m_uiMovingSteps = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    { 
		if(m_uiMovementTimer < uiDiff)
        {
            if (m_pInstance)
            {
				if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
                {
                    float angle = (M_PI / 10) * m_uiMovingSteps;
                    float X = pKiljaeden->GetPositionX() + 22 * cos(angle);
                    float Y = pKiljaeden->GetPositionY() + 22 * sin(angle);

                    m_creature->GetMotionMaster()->MovePoint(0, X, Y, 39.0f);

                    m_uiMovingSteps++;
                    if (m_uiMovingSteps > 19)
                        m_uiMovingSteps = 0;
                }
            }
            m_uiMovementTimer = 500;
        }
		else m_uiMovementTimer -= uiDiff;
	}
};

/*######
## mob_felfire_portal
######*/
struct MANGOS_DLL_DECL mob_felfire_portalAI : public Scripted_NoMovementAI
{
    mob_felfire_portalAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->setFaction(14);
        Reset();
    }

	ScriptedInstance* m_pInstance;
 
    void Reset()
    {
		DoCast(m_creature, SPELL_SUMMON_FELFIRE_FIEND);
		m_creature->SetRespawnDelay(DAY);
    }

	void JustSummoned(Creature* pSummon)
	{
		pSummon->SetInCombatWithZone();
	}

    void UpdateAI(const uint32 uiDiff)
    { 
		if(m_pInstance->GetData(TYPE_KILJAEDEN) != IN_PROGRESS)
			m_creature->ForcedDespawn();
	}
};

/*######
## mob_armageddon_target
######*/
struct MANGOS_DLL_DECL mob_armagedon_targetAI : public Scripted_NoMovementAI
{
    mob_armagedon_targetAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetDisplayId(11686);     // make invisible
		SetCombatMovement(false);
		pCreature->setFaction(14);
        Reset();
    }
 
	uint32 m_uiVisualTimer;
	uint32 m_uiDamageTimer;
	uint32 m_uiHellfireTimer;
 
    void Reset()
    {
        m_uiVisualTimer	= 8000;
		m_uiDamageTimer	= 9000;
		m_uiHellfireTimer = 1000;
		DoCast(m_creature, SPELL_ARMAGEDDON_SPARK);
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiHellfireTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ARMAGEDDON_HELLFIRE);
			m_uiHellfireTimer = 60000;
		}
		else m_uiHellfireTimer -= uiDiff;

        if(m_uiVisualTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ARMAGEDDON_VISUAL);
			m_uiVisualTimer = 10000;
		}
		else m_uiVisualTimer -= uiDiff;

		if(m_uiDamageTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ARMAGEDDON_DMG);
			m_uiDamageTimer = 10000;
		}
		else m_uiDamageTimer -= uiDiff;
    }
};

/*######
## mob_felfire_fiend
######*/
struct MANGOS_DLL_DECL mob_felfire_fiendAI : public ScriptedAI
{
    mob_felfire_fiendAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
	    Reset(); 
    }

    bool m_bMustDie;
	uint32 m_uiDieTimer;

    void Reset() 
    {
        m_bMustDie = false;
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(uiDamage > m_creature->GetHealth())
		{
			uiDamage = 0;
			if(!m_bMustDie)
			{
				DoCast(m_creature, SPELL_FELFIRE_FISSION);
				m_uiDieTimer = 500;
				m_bMustDie = true;
			}
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;

		if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE) && !m_bMustDie)
		{
			DoCast(m_creature, SPELL_FELFIRE_FISSION);
			m_uiDieTimer = 500;
			m_bMustDie = true;
		}

		DoMeleeAttackIfReady();
    }
};          

/*######
## mob_kiljaeden_controller
######*/
struct MANGOS_DLL_DECL mob_kiljaeden_controllerAI : public Scripted_NoMovementAI
{
    mob_kiljaeden_controllerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) 
    {
	    m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
	    Reset(); 
    }
    ScriptedInstance* m_pInstance; 

	uint32 m_uiSpeechTimer;
    uint32 m_uiSpeechPhase;
	bool m_bIsSpeech;
	bool m_bIsPhaseThree;
	bool m_bIsPhaseFour;
	bool m_bIsPhaseFive;
	bool m_bIsOutro;

	uint32 m_uiEmpowerOrbTimer;
	bool m_bHasEmpowered;

    uint64 m_uiVelenGUID;
    uint64 m_uiLiadrinGUID;
	uint64 m_uiKalecGUID;
	uint64 m_uiMuruGUID;

    void Reset() 
    { 
		DoCast(m_creature, SPELL_ANVEENA_DRAIN);

		if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
			pKalec->ForcedDespawn();

		m_uiSpeechTimer		= 30000;
        m_uiSpeechPhase		= 0;
		m_bIsSpeech			= true;
		m_uiEmpowerOrbTimer = 20000;
		m_bHasEmpowered		= true;
		m_bIsPhaseThree		= true;
		m_bIsPhaseFour		= true;
		m_bIsPhaseFive		= true;
		m_bIsOutro			= true;

        m_uiVelenGUID       = 0;
        m_uiLiadrinGUID		= 0;
		m_uiKalecGUID		= 0;
		m_uiMuruGUID		= 0;

        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_KILJAEDEN) != DONE)
            {
                if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
                    pAnveena->CastSpell(pAnveena, SPELL_ANVEENA_PRISON, false);
            }
        }
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

	GameObject* SelectRandomOrb()
    {
        std::list<GameObject* > lOrbList;
		if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
			GetGameObjectListWithEntryInGrid(lOrbList, pKiljaeden, ORB_BLUE_DRAGONFLIGHT, 60.0f);

        if (lOrbList.empty()){
            return NULL;
        }

        std::list<GameObject* >::iterator iter = lOrbList.begin();
        advance(iter, urand(0, lOrbList.size()-1));

        return *iter;
    }

	void EmpoweAllOrbs()
	{
		std::list<GameObject* > lOrbList;
		if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
			GetGameObjectListWithEntryInGrid(lOrbList, pKiljaeden, ORB_BLUE_DRAGONFLIGHT, 60.0f);

		if (!lOrbList.empty())
		{
			for(std::list<GameObject*>::iterator iter = lOrbList.begin(); iter != lOrbList.end(); ++iter)
			{
				if ((*iter))
					(*iter)->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
			}
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
		switch(m_pInstance->GetData(TYPE_KILJAEDEN_PHASE))
		{
		case PHASE_TWO:
			
			if(m_creature->HasAura(SPELL_ANVEENA_DRAIN, EFFECT_INDEX_0))
				m_creature->RemoveAurasDueToSpell(SPELL_ANVEENA_DRAIN);

			if(m_uiSpeechTimer < uiDiff && m_bIsSpeech)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					if(Creature* pKalec = m_creature->SummonCreature(NPC_KALECGOS, 1698.44f, 628.03f, 120.0f, 1.05f, TEMPSUMMON_MANUAL_DESPAWN, 0))
					{
						m_uiKalecGUID = pKalec->GetGUID();
						pKalec->SetSplineFlags(SPLINEFLAG_FLYING);
						pKalec->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
						pKalec->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
						pKalec->GetMotionMaster()->MoveConfused();
					}
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 3000;
                    break;
                case 1:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_INTRO, pKalec);
                    ++m_uiSpeechPhase;
					m_bIsSpeech = false;
                    m_uiSpeechTimer = 1000;
                    break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			break;
		case PHASE_THREE:

			if(!m_bIsSpeech && m_uiSpeechPhase > 0 && m_bIsPhaseThree)
			{
				m_uiSpeechPhase = 0;
				m_uiSpeechTimer = 4000;
				m_bIsSpeech = true;
				m_bIsPhaseThree = false;
			}

			if(m_uiSpeechTimer < uiDiff && m_bIsSpeech)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_AWAKEN, pKalec);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 6000;
                    break;
                case 1:
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
						DoScriptText(SAY_ANVEENA_IMPRISONED, pAnveena);
                    ++m_uiSpeechPhase;
					m_uiSpeechTimer = 4000;
                    break;
				case 2:
					if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
						DoScriptText(SAY_PHASE_THREE, pKiljaeden);
					++m_uiSpeechPhase;
					m_bIsSpeech = false;
                    m_uiSpeechTimer = 1000;
					m_uiEmpowerOrbTimer = 20000;
					m_bHasEmpowered = false;
					break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			if(m_uiEmpowerOrbTimer < uiDiff && !m_bHasEmpowered)
			{
				if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
					DoScriptText(SAY_KALECGOS_ORB1, pKalec);
				if(GameObject* pOrb = SelectRandomOrb())
					pOrb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
				m_bHasEmpowered = true;
			}
			else m_uiEmpowerOrbTimer -= uiDiff;

			break;
		case PHASE_FOUR:

			if(!m_bIsSpeech && m_uiSpeechPhase > 0 && m_bIsPhaseFour)
			{
				m_uiSpeechPhase = 0;
				m_uiSpeechTimer = 4000;
				m_bIsSpeech = true;
				m_bIsPhaseFour = false;
			}

			if(m_uiSpeechTimer < uiDiff && m_bIsSpeech)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_LETGO, pKalec);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 9000;
                    break;
                case 1:
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
						DoScriptText(SAY_ANVEENA_LOST, pAnveena);
                    ++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
                    break;
				case 2:
					if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
						DoScriptText(SAY_PHASE_FOUR, pKiljaeden);
					++m_uiSpeechPhase;
					m_bIsSpeech = false;
                    m_uiSpeechTimer = 1000;
					m_uiEmpowerOrbTimer = 20000;
					m_bHasEmpowered = false;
					break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			if(m_uiEmpowerOrbTimer < uiDiff && !m_bHasEmpowered)
			{
				if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
				{
					switch (rand()%2)
					{  
					case 0: DoScriptText(SAY_KALECGOS_ORB2, pKalec); break;
					case 1: DoScriptText(SAY_KALECGOS_ORB3, pKalec); break;
					}
				}
				if(GameObject* pOrb = SelectRandomOrb())
					pOrb->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
				m_bHasEmpowered = true;
			}
			else m_uiEmpowerOrbTimer -= uiDiff;

			break;
		case PHASE_FIVE:

			if(!m_bIsSpeech && m_uiSpeechPhase > 0 && m_bIsPhaseFive)
			{
				m_uiSpeechPhase = 0;
				m_uiSpeechTimer = 4000;
				m_bIsSpeech = true;
				m_bIsPhaseFive = false;
			}

			if(m_uiSpeechTimer < uiDiff && m_bIsSpeech)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_FOCUS, pKalec);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 12000;
                    break;
                case 1:
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
						DoScriptText(SAY_ANVEENA_KALEC, pAnveena);
                    ++m_uiSpeechPhase;
					m_uiSpeechTimer = 4000;
					break;
				case 2:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_FATE, pKalec);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 6000;
					break;
				case 3:
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
					{
						DoScriptText(SAY_ANVEENA_GOODBYE, pAnveena);
						pAnveena->RemoveAurasDueToSpell(SPELL_ANVEENA_PRISON);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 8000;
					break;
				case 4:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_GOODBYE, pKalec);
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
					{
						if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
						{
							pAnveena->CastSpell(pKiljaeden, SPELL_SACRIFICE_OF_ANVEENA, false);
							pAnveena->GetMotionMaster()->MovePoint(0, pKiljaeden->GetPositionX(), pKiljaeden->GetPositionY(), pKiljaeden->GetPositionZ());
						}
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 13000;
					break;
				case 5:
					if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
						pAnveena->ForcedDespawn();
					if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
						DoScriptText(SAY_PHASE_FIVE, pKiljaeden);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 6:
					if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
						DoScriptText(SAY_KALECGOS_ENCOURAGE, pKalec);
					++m_uiSpeechPhase;
					m_bIsSpeech = false;
                    m_uiSpeechTimer = 1000;
					m_uiEmpowerOrbTimer = 10000;
					m_bHasEmpowered = false;
					break;
				default:
					m_uiSpeechTimer = 100000;
				}
            }
			else m_uiSpeechTimer -= uiDiff;

			if(m_uiEmpowerOrbTimer < uiDiff && !m_bHasEmpowered)
			{
				if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
					DoScriptText(SAY_KALECGOS_ORB4, pKalec);
				EmpoweAllOrbs();
				m_bHasEmpowered = true;
			}
			else m_uiEmpowerOrbTimer -= uiDiff;

			break;
		case PHASE_OUTRO:

			if(!m_bIsSpeech && m_uiSpeechPhase > 0 && m_bIsOutro)
			{
				m_uiSpeechPhase = 0;
				m_uiSpeechTimer = 5000;
				m_bIsSpeech = true;
				m_bIsOutro = false;
				if(Creature* pKalec = m_creature->GetMap()->GetCreature(m_uiKalecGUID))
					pKalec->ForcedDespawn();
			}

			if(m_uiSpeechTimer < uiDiff && m_bIsSpeech)
            {
                switch(m_uiSpeechPhase)
                {
                case 0:
					m_creature->SummonCreature(NPC_VELEN_PORTAL, 1726.395f, 599.631f, 28.154f, 2.39f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    ++m_uiSpeechPhase;
                    m_uiSpeechTimer = 2000;
                    break;
                case 1:
					if(Creature* pVelen = m_creature->SummonCreature(NPC_VELEN, 1726.395f, 599.631f, 28.154f, 2.39f, TEMPSUMMON_MANUAL_DESPAWN, 0))
					{
						m_uiVelenGUID = pVelen->GetGUID();
						pVelen->GetMotionMaster()->MovePoint(0, 1719.708f, 612.418f, 28.05f);
					}
                    ++m_uiSpeechPhase;
					m_uiSpeechTimer = 1000;
					break;
				case 2:
					if(Creature* pLiadrin = m_creature->SummonCreature(NPC_LIADRIN, 1726.395f, 599.631f, 28.154f, 2.39f, TEMPSUMMON_MANUAL_DESPAWN, 0))
					{
						m_uiLiadrinGUID = pLiadrin->GetGUID();
						pLiadrin->GetMotionMaster()->MovePoint(0, 1713.057f, 606.208f, 28.05f);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 3:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						pVelen->GetMotionMaster()->MovePoint(0, 1714.13f, 615.782f, 28.05f);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 4:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_01_VELEN, pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 25000;
					break;
				case 5:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_02_VELEN, pVelen);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 15000;
					break;
				case 6:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_03_VELEN, pVelen); 
					if(Creature* pEntropius = m_creature->SummonCreature(NPC_CORE_ENTROPIUS, 1698.449f, 628.03f, 80.0f, 0, TEMPSUMMON_MANUAL_DESPAWN, 0))
					{
						m_uiMuruGUID = pEntropius->GetGUID();
						pEntropius->CastSpell(pEntropius, SPELL_ENTROPIUS_BODY, false);
						pEntropius->SetSplineFlags(SPLINEFLAG_FLYING);
						pEntropius->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
						pEntropius->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 7:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
					{
						if(Creature* pMuru = m_creature->GetMap()->GetCreature(m_uiMuruGUID))
						{
							pVelen->CastSpell(pMuru, SPELL_CALL_ENTROPIUS, false);
							pMuru->GetMotionMaster()->MovePoint(0, 1698.44f, 628.03f, 35.0f);
							pMuru->SendMonsterMove(1698.44f, 628.03f, 35.0f, SPLINETYPE_NORMAL, SPLINEFLAG_FLYING, 7000);
						}
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 8:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
					{
						pVelen->CastStop();
						DoScriptText(SAY_OUTRO_04_VELEN, pVelen);
					}
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 26000;
					break;
				case 9:
					if(Creature* pLiadrin = m_creature->GetMap()->GetCreature(m_uiLiadrinGUID))
						pLiadrin->GetMotionMaster()->MovePoint(0, 1709.89f, 611.06f, 28.17f);
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
					break;
				case 10:
					if(Creature* pLiadrin = m_creature->GetMap()->GetCreature(m_uiLiadrinGUID))
						DoScriptText(SAY_OUTRO_05_LIADRIN, pLiadrin); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 20000;
					break;
				case 11:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_06_VELEN, pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 16000;
					break;
				case 12:
					if(Creature* pLiadrin = m_creature->GetMap()->GetCreature(m_uiLiadrinGUID))
						DoScriptText(SAY_OUTRO_07_LIADRIN,pLiadrin); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 3000;
					break;
				case 13:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_08_VELEN,pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 7000;
					break;
				case 14:
					// should be some spell here
					if(Creature* pMuru = m_creature->GetMap()->GetCreature(m_uiMuruGUID))
						pMuru->ForcedDespawn();
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 10000;
					break;
				case 15:
					DoCast(m_creature, SPELL_SUNWELL_IGNITION);
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_09_VELEN, pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 16000;
					break;
				case 16:
					if(Creature* pLiadrin = m_creature->GetMap()->GetCreature(m_uiLiadrinGUID))
						DoScriptText(SAY_OUTRO_10_LIADRIN, pLiadrin); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 18000;
					break;
				case 17:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_11_VELEN, pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 7000;
					break;
				case 18:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_12_VELEN, pVelen); 
					++m_uiSpeechPhase;
					m_uiSpeechTimer = 5000;
					break;
				case 19:
					if(Creature* pVelen = m_creature->GetMap()->GetCreature(m_uiVelenGUID))
						pVelen->ForcedDespawn();
					if(Creature* pLiadrin = m_creature->GetMap()->GetCreature(m_uiLiadrinGUID))
						pLiadrin->ForcedDespawn();
					++m_uiSpeechPhase;
					m_bIsSpeech = false;
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

/*######
## go_orb_of_the_blue_fight
######*/
bool GOHello_go_orb_of_the_blue_flight(Player* pPlayer, GameObject* pGo)
{
    if (ScriptedInstance* pInstance = (ScriptedInstance*)pPlayer->GetInstanceData())
    {
		if(pInstance->GetData(TYPE_KILJAEDEN) == IN_PROGRESS)
		{
			if(Creature* pDragon = pPlayer->SummonCreature(NPC_BLUE_DRAGON, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 120000))
				pPlayer->CastSpell(pDragon, SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT, false);
			pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
		}
    }

    return true;
}

CreatureAI* GetAI_mob_felfire_fiend(Creature *pCreature)
{
    return new mob_felfire_fiendAI(pCreature);
}

CreatureAI* GetAI_mob_deceiver(Creature *pCreature)
{
    return new mob_deceiverAI(pCreature);
}

CreatureAI* GetAI_mob_armagedon_target(Creature *pCreature)
{
    return new mob_armagedon_targetAI(pCreature);
}

CreatureAI* GetAI_boss_kiljaeden(Creature *pCreature)
{
    return new boss_kiljaedenAI(pCreature);
}

CreatureAI* GetAI_mob_shield_orb(Creature *pCreature)
{
    return new mob_shield_orbAI(pCreature);
}

CreatureAI* GetAI_mob_felfire_portal(Creature *pCreature)
{
    return new mob_felfire_portalAI(pCreature);
}

CreatureAI* GetAI_mob_kiljaeden_controller(Creature *pCreature)
{
    return new mob_kiljaeden_controllerAI(pCreature);
}

void AddSC_boss_kiljaden()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_kiljaeden_controller";
    newscript->GetAI = &GetAI_mob_kiljaeden_controller;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="boss_kiljaeden";
    newscript->GetAI = &GetAI_boss_kiljaeden;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_felfire_fiend";
    newscript->GetAI = &GetAI_mob_felfire_fiend;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_shield_orb";
    newscript->GetAI = &GetAI_mob_shield_orb;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="mob_felfire_portal";
    newscript->GetAI = &GetAI_mob_felfire_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_armagedon_target";
    newscript->GetAI = &GetAI_mob_armagedon_target;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_deceiver";
    newscript->GetAI = &GetAI_mob_deceiver;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_orb_of_the_blue_flight";
	newscript->pGOHello = &GOHello_go_orb_of_the_blue_flight;
    newscript->RegisterSelf();
}