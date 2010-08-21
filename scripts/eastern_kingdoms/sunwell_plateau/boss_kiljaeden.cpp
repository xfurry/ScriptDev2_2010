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
SDAuthor: ScrappyDoo
SDTestTeam: Legion Of War (c) Andeeria
EndScriptData */
 
/*
Phase1 100%
Phase2 100%
Phase3 100%
Phase4 100%
Phase5 100%

DragonOrbs Event            = in progress
Kalecgos Event              = Implemented
Kalecgos And Anvena Event   = Implemented
Outro Event                 = Implemented
Shield Orb Event            = Implemented
Shadow Spikes               = Implemented
*/

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

    // Encounter speech and sounds
    EMOTE_KJ_DARKNESS   = 0,

    /*** Outro Sounds***/
    SAY_VELEN_01            = -1580087,
    SAY_VELEN_02            = -1580088,
    SAY_VELEN_03            = -1580089,
    SAY_VELEN_04            = -1580090,
    SAY_VELEN_05            = -1580091,
    SAY_VELEN_06            = -1580092,
    SAY_VELEN_07            = -1580093,
    SAY_VELEN_08            = -1580094,
    SAY_VELEN_09            = -1580095,
    SAY_LIADRIN_01          = -1580096,
    SAY_LIADRIN_02          = -1580097,
    SAY_LIADRIN_03          = -1580098,
};

enum spells
{
    //Kilajden Spells
    AURA_SUNWELL_RADIANCE       = 45769, // NOT WORKING
    SPELL_REBIRTH               = 44200, // Emerge from the Sunwell Epick :D

    SPELL_SOULFLY               = 45442, // 100%
    SPELL_DARKNESS_OF_SOULS     = 46605, // 100% Visual Effect
    SPELL_DARKNESS_EXPLOSION    = 45657, // 100% Damage
    SPELL_DESTROY_DRAKES        = 46707, // ?
    SPELL_FIREBLOOM             = 45641, // NOT WORKING
    SPELL_FIREBLOOM_EFF         = 45642, // 100% Damage
    SPELL_FLAMEDARTS            = 45737, // 100%
    SPELL_LEGION_LIGHTING       = 45664, // 100%

    SPELL_SHADOWSPIKE           = 46680, // NOT WORKING -> WorkArround 99%
    SPELL_SHADOWSPIKE_EXP       = 45885, // 100% damage -> target self cast long range
    SPELL_SHADOWSPIKE_DEV       = 60851, // aoe 6k dmg
    SPELL_SHADOWSPIKE_VIS       = 33125, // visual effect

    SPELL_ARMAGEDDON_EXPLOSION  = 45909, // 100% Damage
    SPELL_ARMAGEDDON_EFFECT     = 24207, // 100% Visual

    SPELL_SINISTER_REFLECTION   = 45892, // NOT WORKING -> WorkArround 99%
    SPELL_SACRIFICE_OF_ANVEENA  = 46474, // 100%

    SPELL_KNOCKBACK             = 0,     // ?

    //Hand of the Deceiver Spells
    SPELL_FELLFIRE_PORTAL       = 46875, // NOT WORKING -> WorkArround 99%
    SPELL_SHADOWBOLT_VOLLEY     = 45770, // 100%
    SPELL_SHADOWINFUSION        = 45772, // 100%
    SPELL_IMPFUSION             = 45779, // 100%
    SPELL_SHADOW_CHANNELING     = 46757, // Channeling animation out of combat
    SPELL_ENERGY_DRAIN          = 60309, // visual on anveena

    // Anveena
    SPELL_ANVEENAS_PRISON       = 46367,
    SPELL_ANVEENA_EXPLODE       = 46410,

    //Orb Spells
    SPELL_ORB_BOLT_DEV          = 45680, // NOT WORKING
    SPELL_ORB_BOLT              = 53086, // 100%

    //Reflections Spells
    SPELL_PRIEST                = 47077, // 100%
    SPELL_PALADIN               = 37369, // 100%
    SPELL_PALADIN2              = 37369, // 100%
    SPELL_WARLOCK               = 46190, // 100%
    SPELL_WARLOCK2              = 47076, // 100%
    SPELL_MAGE                  = 47074, // 100%
    SPELL_ROGUE                 = 45897, // 100%
    SPELL_WARRIOR               = 17207, // 100%
    SPELL_DRUID                 = 47072, // 100%
    SPELL_SHAMAN                = 47071, // 100%
    SPELL_HUNTER                = 48098, // 100%

    //Orbs of DragonFligth
    SPELL_REVITALIZE            = 45027, // 100%
    SPELL_SHIELD_OF_BLUE        = 45848, // 100%
    AURA_BLUESHIELD             = 44867, // :D
    SPELL_HASTE                 = 45856, // NOT WORKING

    //Other Spells (used by players, etc)
    SPELL_VENGEANCE_OF_THE_BLUE_FLIGHT  = 45839, // Possess the blue dragon from the orb to help the raid.
};

enum creature_ids
{
    ID_ANVEENA      = 26046, // Embodiment of the Sunwell
    ID_KALECGOS     = 25319, // Helps the raid throughout the fight
    ID_VELEN        = 26246, // Outro
    ID_LIADRIN      = 26247, // outro

    IF_KILJAEDEN_CONTROLLER = 25608,
	ID_KILJADEN		= 25315,
    ID_DECIVER      = 25588,
    ID_SHIELDORB    = 25502,
    ID_SINISTER     = 25708, // Sinister Reflection
    ID_ARMAGEDON    = 25735,
    ID_IMP          = 25598,
    ID_PORTAL       = 25603, //Imp Portal
    ID_DRAGON       = 25653,
    ID_SHADOWSPIKE  = 30598,

    ID_THE_CORE_OF_ENTROPIUS  = 26262, // Used in the ending cinematic?
};
 
float OrbSpawn [4][2] =
{
    {1853.300f,588.653f},
    {1698.900f,627.870f},
    {1781.502f,659.254f},
    {1853.300f,588.653f},
};

float DragonSpawnCoord[4][2] =
{
    (1668.711f, 643.723f),
    (1745.68f, 621.823f),
    (1704.14f, 583.591f),
    (1653.12f, 635.41f),
};

float DeceiverPos[3][2] =
{
    (1707.285f, 612.927f),
    (1684.278f, 614.128f),
    (1682.442f, 638.069f),
};

#define GAMEOBJECT_ORB_OF_THE_BLUE_DRAGONFLIGHT 188415

#define GOSSIP_ITEM_1 "cast on me Shield of the Blue Dragon Flight ! Quikly !"
#define GOSSIP_ITEM_2 "cast on me Dragon Breath: Revitalize !"
#define GOSSIP_ITEM_3 "cast on me Dragon Breath: Haste !"
#define GOSSIP_ITEM_4 "cast on me Blink !"
#define GOSSIP_ITEM_5 "Fight with our Enemy !"

/* --- Kiljaden --- */
struct MANGOS_DLL_DECL boss_kiljadenAI : public Scripted_NoMovementAI
{
    boss_kiljadenAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
 
    ScriptedInstance* pInstance;
    
    uint64 m_uiKalecgosGUID;
    uint64 m_uiAnveenaGUID;

    //BlueShield WorkArround
    uint32 m_uiCancelShieldTimer;

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
        //BlueShield WorkArround
        m_uiCancelShieldTimer = 300000;

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

        if(!m_creature->HasAura(AURA_SUNWELL_RADIANCE, EFFECT_INDEX_0))
            m_creature->CastSpell(m_creature, AURA_SUNWELL_RADIANCE, true);

        if(m_creature->HasAura(SPELL_SACRIFICE_OF_ANVEENA, EFFECT_INDEX_0))
            m_creature->RemoveAurasDueToSpell(SPELL_SACRIFICE_OF_ANVEENA,0);

        for(uint8 i=0; i<4; ++i)
            m_uiDragonGUID[i] = 0;

        if(pInstance)
        {
            pInstance->SetData(TYPE_DECIVER, NOT_STARTED); 
            pInstance->SetData(TYPE_KILJAEDEN, NOT_STARTED);
        }
    }
    
    void Aggro(Unit *who) 
    {
        m_creature->SetInCombatWithZone();
        
        if(Creature* pAnveena = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_ANVEENA)))
            pAnveena->SetVisibility(VISIBILITY_OFF);
    }

    void JustReachedHome()
    {
        if(Creature* pKalecgnos = pInstance->instance->GetCreature(m_uiKalecgosGUID))
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
            Creature* Sinister = m_creature->SummonCreature(ID_SINISTER, victim->GetPositionX()+rand()%3, victim->GetPositionY()+rand()%3, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            if(Sinister)
            {
                Sinister->setFaction(14);
                Sinister->SetDisplayId(victim->GetDisplayId());
                Sinister->AI()->AttackStart(Sinister->getVictim());
                m_uiSinisterGUID[i][0] = Sinister->GetGUID();
                m_uiSinisterGUID[i][1] = spell;
            }
        }
    }

    void JustDied(Unit* Killer) 
    {
        if(pInstance)
            pInstance->SetData(TYPE_KILJAEDEN, DONE);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if(pDoneTo->HasAura(AURA_BLUESHIELD,EFFECT_INDEX_0))
            uiDamage = uiDamage * 0.05;
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
        if(pInstance && pInstance->GetData(TYPE_DECIVER) == SPECIAL)
        {
            m_creature->setFaction(14);
            m_creature->CastSpell(m_creature, SPELL_REBIRTH, true);
            DoScriptText(SAY_KJ_EMERGE, m_creature);
            m_creature->SetVisibility(VISIBILITY_ON);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);
            pInstance->SetData(TYPE_DECIVER, NOT_STARTED); 
        }

        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
               return;

        //FireBloom Damage WorkArround
        if (m_uiFireBloomCheck < diff)
        {
            if(m_uiFireBloomCount < 10)
                for(uint8 i=0; i<5; ++i)
                {
                    if(Unit* FireTarget = m_creature->GetMap()->GetUnit(m_uiFireBloomTarget[i]))
                        FireTarget->CastSpell(FireTarget, SPELL_FIREBLOOM_EFF, true);
                }
            ++m_uiFireBloomCount;
            m_uiFireBloomCheck = 2000;
        }else m_uiFireBloomCheck -= diff;

        // After Each Phase Dragons Are Spawned
        if((m_uiOrbTimer < diff) && !m_bBoolOrb)
        {
            if(Creature* pKalec = pInstance->instance->GetCreature(m_uiKalecgosGUID))
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
                Creature* Dragon = m_creature->SummonCreature(ID_DRAGON, m_creature->GetPositionX()+urand(20,35), m_creature->GetPositionY()+urand(20,35), m_creature->GetPositionZ()+1, 0, TEMPSUMMON_CORPSE_DESPAWN, 20000);
                m_uiDragonGUID[i] = Dragon->GetGUID();
            }
            m_bBoolOrb = true;
        }else m_uiOrbTimer -= diff;

        for(uint8 i=0; i<4; ++i)
        {
            if(Unit* Dragon = m_creature->GetMap()->GetUnit(m_uiDragonGUID[i]))
                if(Dragon && Dragon->HasAura(SPELL_SHIELD_OF_BLUE))
                {
                    m_uiCancelShieldTimer = 5000;
                    std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                    {
                        Unit *TargetedPlayer = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());  
                        if (TargetedPlayer && TargetedPlayer->GetTypeId() == TYPEID_PLAYER && TargetedPlayer->IsWithinDistInMap(Dragon, 10) && !TargetedPlayer->HasAura(AURA_BLUESHIELD))
                            TargetedPlayer->CastSpell(TargetedPlayer,AURA_BLUESHIELD,true);
                    }
                }
        }

        if(m_uiCancelShieldTimer < diff)
        {
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *ShieldedPlayer1 = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                if (ShieldedPlayer1 && ShieldedPlayer1->GetTypeId() == TYPEID_PLAYER && ShieldedPlayer1->HasAura(AURA_BLUESHIELD))
                {
                    ShieldedPlayer1->RemoveAurasDueToSpell(AURA_BLUESHIELD);
                } 
            }
            m_uiCancelShieldTimer = 300000;
        }else m_uiCancelShieldTimer -= diff;

        //Kalecgos and Anvena Event
        if((m_uiKalecgosAnvenaTimer < diff) && m_bIsAnvena)
        {
            if(Creature* pAnveena = pInstance->instance->GetCreature(m_uiAnveenaGUID))
                if(Creature* pKalec = pInstance->instance->GetCreature(m_uiKalecgosGUID))
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
            if(!GetClosestCreatureWithEntry(m_creature, ID_KALECGOS, 80.0f))
            {
                if(Creature* cKalecgos = m_creature->SummonCreature(ID_KALECGOS, m_creature->GetPositionX()-25, m_creature->GetPositionY()-25, m_creature->GetPositionZ() + 5, 0.686f, TEMPSUMMON_TIMED_DESPAWN, 600000))
                {
                    DoScriptText(SAY_KALECGOS_INTRO, cKalecgos);
                    m_uiKalecgosGUID = cKalecgos->GetGUID();
                    cKalecgos->setFaction(35);
                    cKalecgos->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    //Kalecgos need to start shhooting arcane bolt into Kiljaeden
                    //Dragon->AI()->AttackStart(m_creature);
                }
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
                Creature* ShieldOrb = m_creature->SummonCreature(ID_SHIELDORB, m_creature->GetPositionX()+urand(1,15), m_creature->GetPositionY()+urand(1,15), m_creature->GetPositionZ()+10, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if(ShieldOrb)
                    ShieldOrb->AI()->AttackStart(m_creature->getVictim()); 
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
                if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    Creature* Armagedon = m_creature->SummonCreature(ID_ARMAGEDON, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            }
            m_uiAramageddonTimer = 14000;
        }else m_uiAramageddonTimer -= diff;

        if((m_uiShadowSpikeEndsTimer < diff) && m_bShadowSpikeEnds && m_bPhase3)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Creature* cShadowSpike = m_creature->SummonCreature(ID_SHADOWSPIKE, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 6000))
                {
                    cShadowSpike->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    cShadowSpike->setFaction(14);
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

            if(Creature* cAnveena = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_ANVEENA)))
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
                DoCast(m_creature->getVictim(), SPELL_SHADOWSPIKE);

                m_bShadowSpikeEnds         = true;
                m_uiShadowSpikeEndsTimer    = 500;
                m_uiSpikesCount             = 0;
                m_uiShadowSpikeTimer        = 90000;
            }else m_uiShadowSpikeTimer -= diff;

            if(m_uiFlameDartTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_FLAMEDARTS);
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

struct MANGOS_DLL_DECL mob_deceiverAI : public ScriptedAI
{
    mob_deceiverAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }
 
    ScriptedInstance* pInstance;

    uint32 m_uiBoltTimer;
    uint32 m_uiPortalTimer;
    uint32 m_uiImpTimer;
    uint8 m_uiImpCount;
    float m_fxx;
    float m_fyy;
 
    void Reset()
    {
        m_uiPortalTimer = 25000;
        m_uiBoltTimer = 10000;
        m_uiImpTimer = 60000;
        m_uiImpCount = 0;
        m_fxx = 0;
        m_fyy = 0;

        if(pInstance)
            pInstance->SetData(TYPE_DECIVER, NOT_STARTED);

        if(Creature* pAnveena = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_ANVEENA)))
            DoCast(pAnveena, SPELL_ENERGY_DRAIN);
    }
    
    void Aggro(Unit *who) 
    {
		if (pInstance)
            pInstance->SetData(TYPE_KILJAEDEN, IN_PROGRESS);

        if(!m_creature->getVictim())
			m_creature->AI()->AttackStart(who);

        if(Creature* pAnveena = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_ANVEENA)))
            pAnveena->RemoveAurasDueToSpell(SPELL_ANVEENAS_PRISON);

        m_creature->CastStop();
    }

    void JustDied(Unit* Killer) 
    {
        if(pInstance && pInstance->GetData(TYPE_DECIVER) == NOT_STARTED)
            pInstance->SetData(TYPE_DECIVER, IN_PROGRESS);
        else 
            if(pInstance && pInstance->GetData(TYPE_DECIVER) == IN_PROGRESS)
                pInstance->SetData(TYPE_DECIVER, DONE);
            else
                if(pInstance && pInstance->GetData(TYPE_DECIVER) == DONE)
                    pInstance->SetData(TYPE_DECIVER, SPECIAL);
    }

    void KilledUnit(Unit *Victim) {}
     
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHADOWBOLT_VOLLEY);
            m_uiBoltTimer = 10000;
        }else m_uiBoltTimer -= diff;

        if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 20)
            if(!m_creature->HasAura(SPELL_SHADOWINFUSION,EFFECT_INDEX_0))
                m_creature->CastSpell(m_creature, SPELL_SHADOWINFUSION, true);

        if(m_uiPortalTimer < diff)
        {
            m_fxx = m_creature->GetPositionX();
            m_fyy = m_creature->GetPositionY();

            Creature* Portal = m_creature->SummonCreature(ID_PORTAL, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20500);
            m_uiPortalTimer = 30000;
            m_uiImpTimer = 500;
            m_uiImpCount = 0;
        }else m_uiPortalTimer -= diff;

        if(m_uiImpTimer < diff)
        {
            if(m_uiImpCount < 4)
            {
                Creature* Imp = m_creature->SummonCreature(ID_IMP, m_fxx, m_fyy, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                if(Imp)
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        Imp->AI()->AttackStart(target);  
            }
            m_uiImpTimer = 5000;
            ++m_uiImpCount;
        }else m_uiImpTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Shield Orb AI
struct MANGOS_DLL_DECL mob_orbAI : public Scripted_NoMovementAI
{
    mob_orbAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        Reset();
    }
 
    uint32 m_uiSpellTimer;
 
    void Reset()
    {
        m_uiSpellTimer = 1000;
        m_creature->setFaction(14);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if(pDoneTo->HasAura(AURA_BLUESHIELD,EFFECT_INDEX_0))
            uiDamage = uiDamage * 0.05;

    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        if(m_uiSpellTimer < diff)
        {
            for(uint8 i=0; i<3; ++i)
            {
                if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->CastSpell(target, SPELL_ORB_BOLT, true);
            }
            m_uiSpellTimer = 1000;
        }else m_uiSpellTimer -= diff;
    }
};

//Armageddon AI
struct MANGOS_DLL_DECL mob_armagedonAI : public Scripted_NoMovementAI
{
    mob_armagedonAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        Reset();
    }
 
    uint32 m_uiExplosionTimer;
    uint32 m_uiEffectTimer;
    bool m_bIsExplosion;
 
    void Reset()
    {
        m_creature->SetDisplayId(25206);

        m_uiExplosionTimer = 8000;
        m_uiEffectTimer = 100;
        m_bIsExplosion = false;

        m_creature->setFaction(14);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if(pDoneTo->HasAura(AURA_BLUESHIELD,EFFECT_INDEX_0))
            uiDamage = uiDamage * 0.05;
    }

    void UpdateAI(const uint32 diff)
    {
        if(m_bIsExplosion)
            return;

        if((m_uiExplosionTimer < diff) && !m_bIsExplosion)
        {
            m_creature->CastSpell(m_creature, SPELL_ARMAGEDDON_EXPLOSION, true);
            m_bIsExplosion = true;
        }else m_uiExplosionTimer -= diff;

        if(m_uiEffectTimer < diff && !m_bIsExplosion)
        {
            m_creature->CastSpell(m_creature,SPELL_ARMAGEDDON_EFFECT,false);
            m_uiEffectTimer = 1000;
        }else m_uiEffectTimer -= diff;
    }
};

// ShadowSpike AI
struct MANGOS_DLL_DECL mob_shadowspikeAI : public Scripted_NoMovementAI
{
    mob_shadowspikeAI(Creature *c) : Scripted_NoMovementAI(c)
    {
        Reset();
    }
 
    uint32 m_uiExplosionTimer;
    bool m_bIsExplosion;
    uint32 m_uiEffectTimer;
 
    void Reset()
    {
        m_creature->SetDisplayId(25206);

        m_uiExplosionTimer = 5000;
        m_bIsExplosion = false;
        m_uiEffectTimer = 500;

        m_creature->setFaction(14);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if(pDoneTo->HasAura(AURA_BLUESHIELD,EFFECT_INDEX_0))
            uiDamage = uiDamage * 0.05;

    }

    void UpdateAI(const uint32 diff)
    {
        if(m_bIsExplosion)
            return;

        if((m_uiExplosionTimer < diff) && !m_bIsExplosion)
        {
            m_creature->CastSpell(m_creature, SPELL_SHADOWSPIKE_DEV, true);
            m_bIsExplosion = true;
        }else m_uiExplosionTimer -= diff;

        if(m_uiEffectTimer < diff && !m_bIsExplosion)
        {
            m_creature->CastSpell(m_creature,SPELL_SHADOWSPIKE_VIS,false);
            m_uiEffectTimer = 1000;
        }else m_uiEffectTimer -= diff;
    }
};

// Imp AI
struct MANGOS_DLL_DECL mob_killimpAI : public ScriptedAI
{
    mob_killimpAI(Creature *c) : ScriptedAI(c) 
    {
	    Reset(); 
    }

    bool m_bIsReached;
    uint32 m_uiVisibilityTimer ;

    void Reset() 
    {
        m_bIsReached = false;
    }

    void JustDied(Unit* Killer) 
    {
        if(!m_bIsReached)
        {
            m_creature->CastSpell(m_creature->getVictim(), SPELL_IMPFUSION, true);
            m_bIsReached = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
            if(m_bIsReached)
                if(m_uiVisibilityTimer < diff)
                {
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    m_uiVisibilityTimer = 60000;
                }else m_uiVisibilityTimer -= diff;

            //If we are within range melee the target
            if(!m_bIsReached)
                if(m_creature->IsWithinDistInMap(m_creature->getVictim(), 3.0f))
                {
                    m_creature->CastSpell(m_creature->getVictim(), SPELL_IMPFUSION, true);
                    m_uiVisibilityTimer = 2000;
                    m_bIsReached = true;
                }
        }
    }
};          

// Kiljaeden Controller Event AI
struct MANGOS_DLL_DECL mob_kiljaeden_controllerAI : public Scripted_NoMovementAI
{
    mob_kiljaeden_controllerAI(Creature *c) : Scripted_NoMovementAI(c) 
    {
	    pInstance = ((ScriptedInstance*)c->GetInstanceData());
        SetCombatMovement(false);
	    Reset(); 
    }
    ScriptedInstance* pInstance; 

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
        m_bIsProhetSpawned = true;
        m_bIsOutroEnd = true;
        m_creature->setFaction(14);
        m_uiProphetsCount = 10;
        m_bIsOnce = true;

        m_uiVelenGUID       = 0;
        m_uiLiadrinGUID    = 0;

        if(pInstance)
        {
            pInstance->SetData(TYPE_DECIVER, NOT_STARTED); 

            if(pInstance->GetData(TYPE_KILJAEDEN) != DONE)
            {
                if(Creature* pAnveena = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_ANVEENA)))
                {
                    pAnveena->CastSpell(pAnveena, SPELL_ANVEENAS_PRISON, false);
                    pAnveena->SetVisibility(VISIBILITY_ON);
                    pAnveena->GetMap()->CreatureRelocation(pAnveena, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation());
                }

                if(Creature* pKiljaeden = m_creature->GetMap()->GetCreature(pInstance->GetData64(DATA_KILJAEDEN)))
                {
                    pKiljaeden->AI()->EnterEvadeMode();
                    pKiljaeden->SetHealth(pKiljaeden->GetMaxHealth());
                }

                GetCreatureListWithEntryInGrid(lDecievers, m_creature, ID_DECIVER, DEFAULT_VISIBILITY_INSTANCE);
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

    void Aggro(Unit *who)
    { 
        m_creature->SetInCombatWithZone();

        /*for(uint8 i=0; i<3; ++i)
        {
            Creature* Deveiver = m_creature->SummonCreature(ID_DECIVER, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 10000);
                if(Deveiver)
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        Deveiver->AI()->AttackStart(target);
        }*/
    }

    void UpdateAI(const uint32 diff)
    {
        /*if(pInstance && pInstance->GetData(TYPE_KILJAEDEN) == NOT_STARTED)
        {
            Creature* pDeciever = GetClosestCreatureWithEntry(m_creature, ID_DECIVER, 80.0f);

            if(!pDeciever || !pDeciever->isAlive())
            {
                if(Creature* pKiljaeden = ((Creature*)Unit::GetUnit(*m_creature, pInstance->GetData64(DATA_KILJAEDEN))))
                    pKiljaeden->setFaction(14);
            }
        }*/

        if(pInstance && pInstance->GetData(TYPE_KILJAEDEN) == DONE && m_bIsOnce)
        {
            m_bIsProhetSpawned = false;
            m_uiProphetsCount = 0;
            m_bIsOutroEnd = false;
            m_uiProphetsTimer = 20000;
            m_bIsOnce = false;
            // make out of combat
            //m_creature->setFaction(35);
            m_creature->AttackStop();
            m_creature->DeleteThreatList();
        }

        if(!m_bIsProhetSpawned)
        {   
            if(!GetClosestCreatureWithEntry(m_creature, ID_VELEN, 80.0f))
            {
                Creature* Velen = m_creature->SummonCreature(ID_VELEN, m_creature->GetPositionX()+20, m_creature->GetPositionY()+20, m_creature->GetPositionZ(), 3.874f, TEMPSUMMON_TIMED_DESPAWN, 360000);
                Velen->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Velen->setFaction(35);
                m_uiVelenGUID = Velen->GetGUID();
            }
            else
            {
                 if(Creature* Velen = GetClosestCreatureWithEntry(m_creature, ID_VELEN, 80.0f))
                     m_uiVelenGUID = Velen->GetGUID();
            }
            if(!GetClosestCreatureWithEntry(m_creature, ID_LIADRIN, 80.0f))
            {
                Creature* Liadrin = m_creature->SummonCreature(ID_LIADRIN, m_creature->GetPositionX()+20, m_creature->GetPositionY()+17, m_creature->GetPositionZ(), 3.874f, TEMPSUMMON_TIMED_DESPAWN, 360000);
                Liadrin->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                Liadrin->setFaction(35);
                m_uiLiadrinGUID = Liadrin->GetGUID();
            }
            else
            {
                if(Creature* Liadrin = GetClosestCreatureWithEntry(m_creature, ID_LIADRIN, 80.0f))
                    m_uiLiadrinGUID = Liadrin->GetGUID();
            }
            m_bIsProhetSpawned = true;
        }

        if((m_uiProphetsTimer < diff) && !m_bIsOutroEnd)
        {
            switch(m_uiProphetsCount)
            {
                case 0:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_01, pVelen); 
                    m_uiProphetsTimer = 25000; 
                    break; //1
                case 1:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_02, pVelen); 
                    m_uiProphetsTimer = 15000; 
                    break; //2
                case 2:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_03, pVelen); 
                    m_uiProphetsTimer = 10000; 
                    break;  //2
                case 3:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_04, pVelen); 
                    m_uiProphetsTimer = 26000; 
                    break; //3
                case 9:
                    if(Creature* pLiadrin = pInstance->instance->GetCreature(m_uiLiadrinGUID))
                        DoScriptText(SAY_LIADRIN_01, pLiadrin); 
                    m_uiProphetsTimer = 20000; 
                    break; //9
                case 5:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_05, pVelen); 
                    m_uiProphetsTimer = 16000;
                    break; //5
                case 6:
                    if(Creature* pLiadrin = pInstance->instance->GetCreature(m_uiLiadrinGUID))
                        DoScriptText(SAY_LIADRIN_02,pLiadrin); 
                    m_uiProphetsTimer = 3000; 
                    break; //6
                case 7:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_06,pVelen); 
                    m_uiProphetsTimer = 17000; 
                    break; //7
                case 8:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_07, pVelen); 
                    m_uiProphetsTimer = 16000; 
                    break; //8
                case 4:
                    if(Creature* pLiadrin = pInstance->instance->GetCreature(m_uiLiadrinGUID))
                        DoScriptText(SAY_LIADRIN_03, pLiadrin); 
                    m_uiProphetsTimer = 11000; 
                    break; //4
                case 10:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_08, pVelen); 
                    m_uiProphetsTimer = 7000; 
                    break; //10
                case 11:
                    if(Creature* pVelen = pInstance->instance->GetCreature(m_uiVelenGUID))
                        DoScriptText(SAY_VELEN_09, pVelen); 
                    break; //11
                m_bIsOutroEnd = true; 
                m_creature->setFaction(35);           
            }
            ++m_uiProphetsCount;
        }m_uiProphetsTimer -= diff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
    }
};

//Dragon Gossip Menu
//This function is called when the player opens the gossip menubool
bool GossipHello_dragon(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
    pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());
    return true;
}

bool GossipSelect_dragon(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiSender == GOSSIP_SENDER_MAIN)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pCreature->CastSpell(pPlayer, SPELL_SHIELD_OF_BLUE, false);
                pPlayer->CLOSE_GOSSIP_MENU();
                return true;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->CastSpell(pPlayer, SPELL_REVITALIZE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                return true;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->CastSpell(pPlayer, SPELL_HASTE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                return true;
            case GOSSIP_ACTION_INFO_DEF+4:
                pPlayer->TeleportTo(pPlayer->GetMapId(), pPlayer->GetPositionX()+10, pPlayer->GetPositionY()+10, pPlayer->GetPositionZ(), pPlayer->GetOrientation());
                pPlayer->CLOSE_GOSSIP_MENU();
                return true;
            case GOSSIP_ACTION_INFO_DEF+5:
                pPlayer->CLOSE_GOSSIP_MENU();
        }
    }
    return true;
}

CreatureAI* GetAI_mob_killimp(Creature *_Creature)
{
    return new mob_killimpAI(_Creature);
}

CreatureAI* GetAI_mob_shadowspike(Creature *_Creature)
{
    return new mob_shadowspikeAI(_Creature);
}

CreatureAI* GetAI_mob_deceiver(Creature *_Creature)
{
    return new mob_deceiverAI(_Creature);
}

CreatureAI* GetAI_mob_armagedon(Creature *_Creature)
{
    return new mob_armagedonAI(_Creature);
}

CreatureAI* GetAI_boss_kiljaden(Creature *_Creature)
{
    return new boss_kiljadenAI(_Creature);
}

CreatureAI* GetAI_mob_orb(Creature *_Creature)
{
    return new mob_orbAI(_Creature);
}

CreatureAI* GetAI_mob_kiljaeden_controller(Creature *_Creature)
{
    return new mob_kiljaeden_controllerAI(_Creature);
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
    newscript->GetAI = &GetAI_boss_kiljaden;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_shadowspike";
    newscript->GetAI = &GetAI_mob_shadowspike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_killimp";
    newscript->GetAI = &GetAI_mob_killimp;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_orb";
    newscript->GetAI = &GetAI_mob_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_armagedon";
    newscript->GetAI = &GetAI_mob_armagedon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_deceiver";
    newscript->GetAI = &GetAI_mob_deceiver;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "blue_dragon";
    newscript->pGossipHello = &GossipHello_dragon;
    newscript->pGossipSelect = &GossipSelect_dragon;
    newscript->RegisterSelf();
}