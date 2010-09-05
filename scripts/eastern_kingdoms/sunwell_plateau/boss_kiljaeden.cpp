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
    SAY_KJ_OFFCOMBAT1                       = -1580056,
    SAY_KJ_OFFCOMBAT2                       = -1580057,
    SAY_KJ_OFFCOMBAT3                       = -1580058,
    SAY_KJ_OFFCOMBAT4                       = -1580059,
    SAY_KJ_OFFCOMBAT5                       = -1580060,

    // Encounter speech and sounds
    SAY_KJ_EMERGE                           = -1580061,
    SAY_KJ_SLAY1                            = -1580062,
    SAY_KJ_SLAY2                            = -1580063,
    SAY_KJ_REFLECTION1                      = -1580064,
    SAY_KJ_REFLECTION2                      = -1580065,
    SAY_KJ_DARKNESS1                        = -1580066,
    SAY_KJ_DARKNESS2                        = -1580067,
    SAY_KJ_DARKNESS3                        = -1580068,
    SAY_KJ_CANNOT_WIN                       = -1580070,
    SAY_KJ_DENINE                           = -1580069,
    SAY_KJ_LOST_POWER                       = -1580071,

    /*** Kalecgos - Anveena speech at the beginning of Phase 5; Anveena's sacrifice ***/
    SAY_KALECGOS_INTRO                      = -1580072,
    SAY_KALECGOS_AWAKEN                     = -1580073,
    SAY_ANVEENA_IMPRISONED                  = -1580074,
    SAY_KALECGOS_LETGO                      = -1580075,
    SAY_ANVEENA_LOST                        = -1580076,
    SAY_KALECGOS_FOCUS                      = -1580077,
    SAY_ANVEENA_KALEC                       = -1580078,
    SAY_KALECGOS_FATE                       = -1580079,
    SAY_ANVEENA_GOODBYE                     = -1580080,
    SAY_KALECGOS_GOODBYE                    = -1580081,
    SAY_KALECGOS_ENCOURAGE                  = -1580082,
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
    SPELL_REBIRTH               = 44200, // Emerge from the Sunwell Epick :D
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
	SPELL_SACRIFICE_OF_ANVEENA  = 46474,
	SPELL_ENTROPIUS_BODY		= 46819,

    //Orb Spells
    SPELL_SHADOW_ORB_BOLT_TRIGG = 45679, 

    //Reflections Spells
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
	NPC_CORE_ENTROPIUS  = 26262, // Used in the ending cinematic?

	// encounter
    NPC_DECIVER			= 25588,
	NPC_FELFIRE_PORTAL	= 25603,
    NPC_SHIELD_ORB		= 25502,
    NPC_SINISTER		= 25708, // Sinister Reflection
    NPC_ARMAGEDON		= 25735,
    NPC_FELFIRE_FIEND	= 25598,
    NPC_DRAGON			= 25653,
    NPC_SHADOWSPIKE		= 30598,

	// objects
	ORB_BLUE_DRAGONFLIGHT				= 188415,
	SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT  = 45839, // Possess the blue dragon from the orb to help the raid.

	// phases
	PHASE_IDLE			= 0,
	PHASE_ONE			= 1,
	PHASE_TWO			= 2,
	PHASE_THREE			= 3,
	PHASE_FOUR			= 4,
	PHASE_FIFE			= 5,
	PHASE_OUTRO			= 6,
};
 
//Positional defines 
struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

float OrbSpawn [4][2] =
{
    {1853.300f,588.653f},
    {1698.900f,627.870f},
    {1781.502f,659.254f},
    {1853.300f,588.653f},
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
    
    uint64 m_uiKalecgosGUID;
    uint64 m_uiAnveenaGUID;

    uint64 m_uiDragonGUID[4];

    uint32 m_uiOrbTimer;
    bool m_bBoolOrb;

    uint32 m_uiKalecgosTimer;
    bool m_bIsKalecgosSpawned;

    //SpeachTimers
    uint32 m_uiKJOrdersTimer;

    //Anvena and Kalecgos Timer
    uint32 m_uiKalecgosAnvenaCount;
    bool m_bIsAnvena;
    uint32 m_uiKalecgosAnvenaTimer;

    //FireBloom Targets Table
    uint64 m_uiFireBloomTarget[5];

    //Phase2
    uint32 m_uiSoulFlyTimer;
    uint32 m_uiLegionLightingTimer;
    uint32 m_uiFireBloomCheck;
    uint32 m_uiFireBloomTimer;
    uint32 m_uiFireBloomCount;
    uint32 m_uiShieldOrbTimer;

    //Phase3
    bool m_bPhase3;
    uint32 m_uiShadowSpikeTimer;
    uint32 m_uiFlameDartTimer;
    uint32 m_uiDarknessOfSoulsTimer;
    uint32 m_uiDarknessExplosionTimer;
    uint32 m_uiShadowSpikeEndsTimer;
    uint32 m_uiSpikesCount;
    bool m_bShadowSpikeEnds;

    //Phase4
    bool m_bPhase4;
    bool m_bDarknessOfSoulsCasting;
    uint32 m_uiAramageddonTimer;
    
    //Phase5
    bool m_bPhase5;

    //Sinister Reflections
    uint64 m_uiSinisterGUID[12][2];
    uint32 m_uiSinnisterCastTimer;
 
    void Reset()
    {
        //creatures
        m_uiKalecgosGUID = 0;
        m_uiAnveenaGUID = 0;

        //SpeachTimers
        m_uiKJOrdersTimer = 10000;

        m_bIsAnvena = false;

        //Kalecgos Event
        m_uiKalecgosTimer = 25000;
        m_bIsKalecgosSpawned = false;

        m_bBoolOrb = true;

        //Phase2
        m_uiSoulFlyTimer        = 1000;
        m_uiLegionLightingTimer = 15000;
        m_uiFireBloomCheck      = 2000;
        m_uiFireBloomTimer      = 30000;
        m_uiFireBloomCount      = 10;
        m_uiShieldOrbTimer      = 25000;

        //Phase3
        m_bShadowSpikeEnds     = false;
        m_uiSpikesCount         = 9;

        m_bDarknessOfSoulsCasting  = false;
        m_uiDarknessExplosionTimer = 120000;

        m_bPhase3 = false;
        m_bPhase4 = false;
        m_bPhase5 = false;

        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->setFaction(35);

        if(!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_creature->HasAura(SPELL_SACRIFICE_OF_ANVEENA, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_SACRIFICE_OF_ANVEENA,0);

        for(uint8 i=0; i<4; ++i)
            m_uiDragonGUID[i] = 0;

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_KILJAEDEN, NOT_STARTED);
        }
    }
    
    void Aggro(Unit *who) 
    {
        m_creature->SetInCombatWithZone();
        
        if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
            pAnveena->SetVisibility(VISIBILITY_OFF);
    }

    void JustReachedHome()
    {
        if(Creature* pKalecgnos = m_pInstance->instance->GetCreature(m_uiKalecgosGUID))
            pKalecgnos->ForcedDespawn();
    }

    void KilledUnit(Unit *Victim) 
    {
        switch (rand()%2)
        {  
            case 0: DoScriptText(SAY_KJ_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_KJ_SLAY2, m_creature); break;
        }
    }

    void Sinister(Player* victim,uint8 i,uint8 k)
    {
        uint32 spell;
        switch(victim->getClass())
        {
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

        for(i=i; i<k; ++i)
        {
            /*Creature* Sinister = m_creature->SummonCreature(ID_SINISTER, victim->GetPositionX()+rand()%3, victim->GetPositionY()+rand()%3, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            if(Sinister)
            {
                Sinister->setFaction(14);
                Sinister->SetDisplayId(victim->GetDisplayId());
                Sinister->AI()->AttackStart(Sinister->getVictim());
                m_uiSinisterGUID[i][0] = Sinister->GetGUID();
                m_uiSinisterGUID[i][1] = spell;
            }*/
        }
    }

    void JustDied(Unit* Killer) 
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        //Sounds OOC, Kiljaeden Orders
        if(!m_creature->getVictim())
        {
            if(m_uiKJOrdersTimer < diff)
            {
		        switch (rand()%5)
		        {
			        case 0: DoScriptText(SAY_KJ_OFFCOMBAT1, m_creature); break;
			        case 1: DoScriptText(SAY_KJ_OFFCOMBAT2, m_creature); break;
			        case 2: DoScriptText(SAY_KJ_OFFCOMBAT3, m_creature); break;
                    case 3: DoScriptText(SAY_KJ_OFFCOMBAT4, m_creature); break;
                    case 4: DoScriptText(SAY_KJ_OFFCOMBAT5, m_creature); break;
		        }
                m_uiKJOrdersTimer = 30000;
            }else m_uiKJOrdersTimer -= diff;
        }

        //Rebirth After Phase1
		if(m_pInstance && m_pInstance->GetData(TYPE_KILJAEDEN) == SPECIAL)
        {
            m_creature->setFaction(14);
            m_creature->CastSpell(m_creature, SPELL_REBIRTH, true);
            DoScriptText(SAY_KJ_EMERGE, m_creature);
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);
            //pInstance->SetData(TYPE_DECIVER, NOT_STARTED); 
        }

        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
               return;

        // After Each Phase Dragons Are Spawned
        if((m_uiOrbTimer < diff) && !m_bBoolOrb)
        {
            if(Creature* pKalec = m_pInstance->instance->GetCreature(m_uiKalecgosGUID))
            {
                switch (rand()%4)
                {
                case 0: DoScriptText(SAY_KALECGOS_ORB1,pKalec); break;
                case 1: DoScriptText(SAY_KALECGOS_ORB2,pKalec); break;
                case 2: DoScriptText(SAY_KALECGOS_ORB3,pKalec); break;
                case 3: DoScriptText(SAY_KALECGOS_ORB4,pKalec); break;
                }
            }
            uint8 m_uiMaxDragons = 1;
            if(m_bPhase5)
                m_uiMaxDragons = 4;
            for(uint8 i=0; i<m_uiMaxDragons; ++i)
            {
                //Creature* Dragon = m_creature->SummonCreature(ID_DRAGON, m_creature->GetPositionX()+urand(20,35), m_creature->GetPositionY()+urand(20,35), m_creature->GetPositionZ()+1, 0, TEMPSUMMON_CORPSE_DESPAWN, 20000);
                //m_uiDragonGUID[i] = Dragon->GetGUID();
            }
            m_bBoolOrb = true;
        }else m_uiOrbTimer -= diff;

        //Kalecgos and Anvena Event
        if((m_uiKalecgosAnvenaTimer < diff) && m_bIsAnvena)
        {
            if(Creature* pAnveena = m_pInstance->instance->GetCreature(m_uiAnveenaGUID))
                if(Creature* pKalec = m_pInstance->instance->GetCreature(m_uiKalecgosGUID))
                {
                    switch(m_uiKalecgosAnvenaCount)
                    {
                    case 0:
                        DoScriptText(SAY_KALECGOS_AWAKEN, pKalec); m_uiKalecgosAnvenaTimer = 5000; break;
                    case 1:
                        DoScriptText(SAY_ANVEENA_IMPRISONED, pAnveena); m_uiKalecgosAnvenaTimer = 3000; break;
                    case 2:
                        DoScriptText(SAY_KALECGOS_LETGO, pKalec); m_uiKalecgosAnvenaTimer = 6000; break;
                    case 5:
                        DoScriptText(SAY_ANVEENA_LOST,pAnveena); m_uiKalecgosAnvenaTimer = 4000; break;
                    case 4:
                        DoScriptText(SAY_KALECGOS_FOCUS,pKalec); m_uiKalecgosAnvenaTimer = 8000; break;
                    case 3:
                        DoScriptText(SAY_ANVEENA_KALEC,pAnveena); m_uiKalecgosAnvenaTimer = 5000; break;
                    case 6:
                        DoScriptText(SAY_KALECGOS_FATE,pKalec); m_uiKalecgosAnvenaTimer = 5000; break;
                    case 7:
                        DoScriptText(SAY_ANVEENA_GOODBYE, pAnveena); 
                        //pAnveena->CastSpell(pAnveena, SPELL_ANVEENA_EXPLODE, false);
                        m_creature->CastSpell(m_creature, SPELL_SACRIFICE_OF_ANVEENA, false);
                        if(Creature* Anveena = m_creature->GetMap()->GetCreature(m_uiAnveenaGUID))
                            if(Anveena && Anveena->isAlive())
                                Anveena->SetVisibility(VISIBILITY_OFF);
                        m_uiKalecgosAnvenaTimer = 5000;
                        break;
                    case 9:
                        DoScriptText(SAY_KALECGOS_GOODBYE,pKalec);
                        pAnveena->CastStop();
                        m_bIsAnvena = false; 
                        break;
                    case 8:
                        DoScriptText(SAY_KALECGOS_ENCOURAGE, pKalec); m_uiKalecgosAnvenaTimer = 14000; break;         
                    }
                }
                ++m_uiKalecgosAnvenaCount;
        }m_uiKalecgosAnvenaTimer -= diff;

        //Kalecgos Event
        if((m_uiKalecgosTimer < diff) && !m_bIsKalecgosSpawned)
        {
            //if(!GetClosestCreatureWithEntry(m_creature, ID_KALECGOS, 80.0f))
            {
                /*if(Creature* cKalecgos = m_creature->SummonCreature(ID_KALECGOS, m_creature->GetPositionX()-25, m_creature->GetPositionY()-25, m_creature->GetPositionZ() + 5, 0.686f, TEMPSUMMON_TIMED_DESPAWN, 600000))
                {
                    DoScriptText(SAY_KALECGOS_INTRO, cKalecgos);
                    m_uiKalecgosGUID = cKalecgos->GetGUID();
                    cKalecgos->setFaction(35);
                    cKalecgos->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    //Kalecgos need to start shhooting arcane bolt into Kiljaeden
                    //Dragon->AI()->AttackStart(m_creature);
                }*/
            }
            m_bIsKalecgosSpawned = true;
        }m_uiKalecgosTimer -= diff;

        //Shield Orb At Start each phases
        if(m_uiShieldOrbTimer < diff && !m_bPhase5)
        {
            uint8 l=1;
            if(m_bPhase3)
                l=2;
            if(m_bPhase4)
                l=3;
            for(uint8 k=0; k<l; ++k)
            {
                //Creature* ShieldOrb = m_creature->SummonCreature(ID_SHIELDORB, m_creature->GetPositionX()+urand(1,15), m_creature->GetPositionY()+urand(1,15), m_creature->GetPositionZ()+10, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                //if(ShieldOrb)
                //    ShieldOrb->AI()->AttackStart(m_creature->getVictim()); 
            }
            
            m_uiShieldOrbTimer = 50000; 
        }else m_uiShieldOrbTimer -= diff;

        //Sinister Reflects Attack
        if(m_uiSinnisterCastTimer < diff)
        {
            uint8 m_uiSinisterCount = 0;
            if(m_bPhase3)
                m_uiSinisterCount = 4;
            if(m_bPhase4)
                m_uiSinisterCount = 8;
            if(m_bPhase5)
                m_uiSinisterCount = 12;
            for(uint8 i=0; i<m_uiSinisterCount; ++i)
			{
                if(Unit* Sinister = m_creature->GetMap()->GetUnit(m_uiSinisterGUID[i][0]))
				{
					if(!Sinister->isDead())
					{
						if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
							Sinister->CastSpell(target, m_uiSinisterGUID[i][1], true);
					}
				}
			}
            m_uiSinnisterCastTimer = 8000;
        }else m_uiSinnisterCastTimer -= diff;

        //Phase4
        if((m_uiAramageddonTimer < diff) && m_bPhase4)
        {
            uint8 h=3;
            if(m_bPhase5)
                h=5;
            if(!m_bPhase5 && m_bDarknessOfSoulsCasting)
                h=0;
            
            for(uint8 i=0; i<h; ++i)
            {
                //if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                //    Creature* Armagedon = m_creature->SummonCreature(ID_ARMAGEDON, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            }
            m_uiAramageddonTimer = 14000;
        }else m_uiAramageddonTimer -= diff;

        if((m_uiShadowSpikeEndsTimer < diff) && m_bShadowSpikeEnds && m_bPhase3)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                //if(Creature* cShadowSpike = m_creature->SummonCreature(ID_SHADOWSPIKE, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 6000))
                {
                    //cShadowSpike->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    //cShadowSpike->setFaction(14);
                }

                //target->CastSpell(target, SPELL_SHADOWSPIKE_EXP, false);
            }
            ++m_uiSpikesCount;
            if(m_uiSpikesCount > 9)
                m_bShadowSpikeEnds = false;
            m_uiShadowSpikeEndsTimer = 3000;
        }else m_uiShadowSpikeEndsTimer -= diff;

        if(m_bShadowSpikeEnds)
            return;

        if(m_uiDarknessExplosionTimer < diff && m_bDarknessOfSoulsCasting)
        {
            //m_creature->CastSpell(m_creature->getVictim(), SPELL_DARKNESS_EXPLOSION, true);
            m_bDarknessOfSoulsCasting = false;
            m_uiDarknessExplosionTimer = 600000;
            m_uiDarknessOfSoulsTimer = 45000;
            m_uiFireBloomTimer = 25000;
            if(m_bPhase5)
                m_uiDarknessOfSoulsTimer = 25000;
        }else m_uiDarknessExplosionTimer -= diff;

        if(m_bDarknessOfSoulsCasting)
            return;

        if(m_uiDarknessOfSoulsTimer < diff && m_bPhase3)
        {
            switch (rand()%3)
		        {
			        case 0: DoScriptText(SAY_KJ_DARKNESS1, m_creature); break;
			        case 1: DoScriptText(SAY_KJ_DARKNESS2, m_creature); break;
			        case 2: DoScriptText(SAY_KJ_DARKNESS3, m_creature); break;
		        }
            m_creature->CastSpell(m_creature, SPELL_DARKNESS_OF_SOULS, false);
            m_bDarknessOfSoulsCasting = true;
            m_uiDarknessExplosionTimer = 8500;
        }else m_uiDarknessOfSoulsTimer -= diff;

        //Phases
        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 85) && !m_bPhase3)
        {      
            DoScriptText(SAY_KJ_DENINE, m_creature);
            if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)) 
                if (victim && (victim->GetTypeId() == TYPEID_PLAYER))
                    Sinister(((Player*)victim),0,4);

            //Start Timerow  Dochodzacych w 3 Fazie
            m_uiSinnisterCastTimer     = 10000;
            m_uiShadowSpikeTimer       = 30000;
            m_uiFlameDartTimer         = 40000;
            m_uiDarknessOfSoulsTimer   = 45000;
            m_bShadowSpikeEnds         = false;
            m_bDarknessOfSoulsCasting  = false;
            m_bPhase3 = true;
            //DragonsTimer
            m_uiOrbTimer = 35000;
            m_bBoolOrb = false;
        }

        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 55) && !m_bPhase4)
        {
            DoScriptText(SAY_KJ_CANNOT_WIN, m_creature);
            if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)) 
                if (victim && (victim->GetTypeId() == TYPEID_PLAYER))
                    Sinister(((Player*)victim),4,8);

            //Start Timerow  Dochodzacych w 4 Fazie
            m_uiAramageddonTimer       = 2000;  //100% ok
            m_bPhase4 = true;
            m_uiDarknessOfSoulsTimer   = 45000;
            //DragonsTimer
            m_uiOrbTimer = 35000;
            m_bBoolOrb = false;
        }

        if(((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 25) && !m_bPhase5)
        {
            DoScriptText(SAY_KJ_LOST_POWER, m_creature);
            if(Unit* victim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)) 
                if (victim && (victim->GetTypeId() == TYPEID_PLAYER))
                    Sinister(((Player*)victim),8,12);
            //Start Timerow  Dochodzacych w 5 Fazie
            m_uiShadowSpikeTimer = 1000;
            m_uiDarknessOfSoulsTimer   = 45000;
            m_bPhase5 = true;
            //DragonsTimer
            m_uiOrbTimer = 35000;
            m_bBoolOrb = false;
            //Kalecgos and Anvena Event
            m_uiKalecgosAnvenaTimer = 20000;
            m_bIsAnvena = true;
            m_uiKalecgosAnvenaCount = 0;

            if(Creature* cAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
            //if(Creature* cAnveena = m_creature->SummonCreature(ID_ANVEENA, m_creature->GetPositionX()+urand(20,30), m_creature->GetPositionY()+urand(20,30), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 120000))
            {
                m_uiAnveenaGUID = cAnveena->GetGUID();
                cAnveena->SetSplineFlags(SPLINEFLAG_FLYING);
                cAnveena->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                cAnveena->setFaction(35);
                cAnveena->SetVisibility(VISIBILITY_ON);
                cAnveena->GetMap()->CreatureRelocation(cAnveena, cAnveena->GetPositionX(), cAnveena->GetPositionY(), cAnveena->GetPositionZ()+20, cAnveena->GetOrientation());
            }
        }

        //Phase3
        if(m_bPhase3)
        {
            if(m_uiShadowSpikeTimer < diff)
            {
                //DoCast(m_creature->getVictim(), SPELL_SHADOWSPIKE);

                m_bShadowSpikeEnds         = true;
                m_uiShadowSpikeEndsTimer    = 500;
                m_uiSpikesCount             = 0;
                m_uiShadowSpikeTimer        = 90000;
            }else m_uiShadowSpikeTimer -= diff;

            if(m_uiFlameDartTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_FLAMEDART);
                m_uiFlameDartTimer = 33000;
            }else m_uiFlameDartTimer -= diff;
        }

        //Phase2
        if(m_uiLegionLightingTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_LEGION_LIGHTING);
            m_uiLegionLightingTimer = 11000;
        }else m_uiLegionLightingTimer -= diff;

        if(m_uiFireBloomTimer < diff)
        {
            for(uint8 i=0; i<5; ++i)
            {
                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                m_uiFireBloomTarget[i] = target->GetGUID();
                m_uiFireBloomCount = 0;
                //DoCast(target, SPELL_FIREBLOOM, true);
            }
            m_uiFireBloomTimer = 25000;
        }else m_uiFireBloomTimer -= diff;

        if(m_uiSoulFlyTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SOULFLY);
            m_uiSoulFlyTimer = 7000;
        }else m_uiSoulFlyTimer -= diff;

        DoMeleeAttackIfReady();
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
        m_uiPortalTimer		= 25000;
        m_uiShadowBoltTimer = 10000;
		m_bHasPortal		= false;
		m_uiPortalGUID		= 0;
    }
    
    void Aggro(Unit* pWho) 
    {
		if (m_pInstance)
            m_pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);

        m_creature->CastStop();

		std::list<Creature*> lDecievers;
		GetCreatureListWithEntryInGrid(lDecievers, m_creature, NPC_DECIVER, DEFAULT_VISIBILITY_INSTANCE);
		if (!lDecievers.empty())
		{
			for(std::list<Creature*>::iterator iter = lDecievers.begin(); iter != lDecievers.end(); ++iter)
			{
				if ((*iter) && (*iter)->isAlive())
					(*iter)->AI()->AttackStart(pWho);
			}
		}
	}

    void JustDied(Unit* Killer) 
    {
        // Start phase 2
		if(Creature* pPortal = m_creature->GetMap()->GetCreature(m_uiPortalGUID))
			pPortal->ForcedDespawn();
    }

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_KILJAEDEN) != NOT_STARTED)
				m_pInstance->SetData(TYPE_KILJAEDEN, NOT_STARTED);
		}
	}

	void JustSummoned(Creature* pSummon)
	{
		if(pSummon->GetEntry() == NPC_FELFIRE_PORTAL)
			m_uiPortalGUID = pSummon->GetGUID();
	}
     
	void UpdateAI(const uint32 uiDiff)
	{
		//Return since we have no target
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
struct MANGOS_DLL_DECL mob_shield_orbAI : public Scripted_NoMovementAI
{
    mob_shield_orbAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
		m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

	ScriptedInstance* m_pInstance;
 
    void Reset()
    {
		DoCast(m_creature, SPELL_SHADOW_ORB_BOLT_TRIGG);
		m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    { 
		if(m_pInstance->GetData(TYPE_KILJAEDEN) != IN_PROGRESS)
			m_creature->ForcedDespawn();
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
 
    void Reset()
    {
        m_uiVisualTimer	= 3000;
		m_uiDamageTimer	= 4000;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}

    void UpdateAI(const uint32 uiDiff)
    {
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
	    Reset(); 
    }
    ScriptedInstance* m_pInstance; 

    uint32 m_uiProphetsTimer;
    uint32 m_uiProphetsCount;
    bool m_bIsOutroEnd;
    bool m_bIsProhetSpawned;
    bool m_bIsOnce;

    uint64 m_uiVelenGUID;
    uint64 m_uiLiadrinGUID;

    std::list<Creature*> lDecievers;

    void Reset() 
    { 
		DoCast(m_creature, SPELL_ANVEENA_DRAIN);
        m_bIsProhetSpawned = true;
        m_bIsOutroEnd = true;
        m_creature->setFaction(14);
        m_uiProphetsCount = 10;
        m_bIsOnce = true;

        m_uiVelenGUID       = 0;
        m_uiLiadrinGUID    = 0;

        if(m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_KILJAEDEN) != DONE)
            {
                if(Creature* pAnveena = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ANVEENA)))
                    pAnveena->CastSpell(pAnveena, SPELL_ANVEENA_PRISON, false);

                if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_KILJAEDEN)))
                    pKiljaeden->AI()->EnterEvadeMode();

                GetCreatureListWithEntryInGrid(lDecievers, m_creature, NPC_DECIVER, DEFAULT_VISIBILITY_INSTANCE);
                if (!lDecievers.empty())
                {
                    for(std::list<Creature*>::iterator iter = lDecievers.begin(); iter != lDecievers.end(); ++iter)
                    {
                        if ((*iter) && !(*iter)->isAlive())
                            (*iter)->Respawn();
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_KILJAEDEN) == DONE && m_bIsOnce)
        {
            m_bIsProhetSpawned = false;
            m_uiProphetsCount = 0;
            m_bIsOutroEnd = false;
            m_uiProphetsTimer = 20000;
            m_bIsOnce = false;
            m_creature->AttackStop();
            m_creature->DeleteThreatList();
        }

        if(!m_bIsProhetSpawned)
        {   
            //if(!GetClosestCreatureWithEntry(m_creature, ID_VELEN, 80.0f))
            {
                //Creature* Velen = m_creature->SummonCreature(ID_VELEN, m_creature->GetPositionX()+20, m_creature->GetPositionY()+20, m_creature->GetPositionZ(), 3.874f, TEMPSUMMON_TIMED_DESPAWN, 360000);
                //Velen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                //Velen->setFaction(35);
                //m_uiVelenGUID = Velen->GetGUID();
            }
            //else
            {
                 //if(Creature* Velen = GetClosestCreatureWithEntry(m_creature, ID_VELEN, 80.0f))
                 //    m_uiVelenGUID = Velen->GetGUID();
            }
            //if(!GetClosestCreatureWithEntry(m_creature, ID_LIADRIN, 80.0f))
            {
                //Creature* Liadrin = m_creature->SummonCreature(ID_LIADRIN, m_creature->GetPositionX()+20, m_creature->GetPositionY()+17, m_creature->GetPositionZ(), 3.874f, TEMPSUMMON_TIMED_DESPAWN, 360000);
                //Liadrin->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                //Liadrin->setFaction(35);
                //m_uiLiadrinGUID = Liadrin->GetGUID();
            }
            //else
            {
                //if(Creature* Liadrin = GetClosestCreatureWithEntry(m_creature, ID_LIADRIN, 80.0f))
                //    m_uiLiadrinGUID = Liadrin->GetGUID();
            }
            m_bIsProhetSpawned = true;
        }

        if((m_uiProphetsTimer < diff) && !m_bIsOutroEnd)
        {
            switch(m_uiProphetsCount)
            {
                case 0:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
						DoScriptText(SAY_OUTRO_01_VELEN, pVelen); 
                    m_uiProphetsTimer = 25000; 
                    break;
                case 1:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_02_VELEN, pVelen); 
                    m_uiProphetsTimer = 15000; 
                    break;
                case 2:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_03_VELEN, pVelen); 
                    m_uiProphetsTimer = 10000; 
                    break;
                case 3:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_04_VELEN, pVelen); 
                    m_uiProphetsTimer = 26000; 
                    break;
                case 9:
                    if(Creature* pLiadrin = m_pInstance->instance->GetCreature(m_uiLiadrinGUID))
						DoScriptText(SAY_OUTRO_05_LIADRIN, pLiadrin); 
                    m_uiProphetsTimer = 20000; 
                    break;
                case 5:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_06_VELEN, pVelen); 
                    m_uiProphetsTimer = 16000;
                    break;
                case 6:
                    if(Creature* pLiadrin = m_pInstance->instance->GetCreature(m_uiLiadrinGUID))
                        DoScriptText(SAY_OUTRO_07_LIADRIN,pLiadrin); 
                    m_uiProphetsTimer = 3000; 
                    break;
                case 7:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_08_VELEN,pVelen); 
                    m_uiProphetsTimer = 17000; 
                    break;
                case 8:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_09_VELEN, pVelen); 
                    m_uiProphetsTimer = 16000; 
                    break;
                case 4:
                    if(Creature* pLiadrin = m_pInstance->instance->GetCreature(m_uiLiadrinGUID))
                        DoScriptText(SAY_OUTRO_10_LIADRIN, pLiadrin); 
                    m_uiProphetsTimer = 11000; 
                    break;
                case 10:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_11_VELEN, pVelen); 
                    m_uiProphetsTimer = 7000; 
                    break;
                case 11:
                    if(Creature* pVelen = m_pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_OUTRO_12_VELEN, pVelen); 
                    break;
                m_bIsOutroEnd = true; 
                m_creature->setFaction(35);           
            }
            ++m_uiProphetsCount;
        }m_uiProphetsTimer -= diff;
    }
};

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