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
SDName: boss_flame_leviathan
SD%Complete: 0%
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                               = -1603159,
    SAY_SLAY                                = -1603160,
    SAY_DEATH                               = -1603161,

    SAY_CHANGE_1                            = -1603162,
    SAY_CHANGE_2                            = -1603163,
    SAY_CHANGE_3                            = -1603164,
    SAY_PLAYER_RIDE                         = -1603165,
    SAY_OVERLOAD_1                          = -1603166,
    SAY_OVERLOAD_2                          = -1603167,
    SAY_OVERLOAD_3                          = -1603168,

    SAY_HARD_MODE                           = -1603169,

    SAY_TOWER_FROST                         = -1603170,
    SAY_TOWER_FIRE                          = -1603171,
    SAY_TOWER_ENERGY                        = -1603172,
    SAY_TOWER_NATURE                        = -1603173,
    SAY_TOWER_DOWN                          = -1603174,

    EMOTE_PURSUE                            = -1603175,
};

enum spells
{
    SPELL_PURSUED           = 62374,

    SPELL_MISSILE_BARRAGE   = 62400,
    SPELL_FLAME_VENTS       = 62396,
    SPELL_BATTERING_RAM     = 62376,

    SPELL_GATHERING_SPEED   = 62375,
    // interupted by
    SPELL_OVERLOAD_CIRCUIT  = 62399,

    SPELL_SEARING_FLAME     = 62402, // used by defense turret
    // interupted by
    SPELL_SYSTEMS_SHUTDOWN  = 62475,

    SPELL_FLAME_CANNON      = 62395,
    //SPELL_FLAME_CANNON    = 64692, trigger the same spell
    SPELL_BLAZE             = 62292,

    // used by players -> to be added later
    SPELL_ELECTROSHOCK      = 62522,
    SPELL_SMOKE_TRAIL       = 63575,

    // tower of nature
    SPELL_FREYAS_WARD       = 62906,
    SPELL_TOWER_OF_LIFE     = 64482,
    // tower of flames
    SPELL_MIMIRON_INFERNO   = 62910,
    SPELL_TOWER_OF_FLAMES   = 65075,
    // tower of frost
    SPELL_HODIR_FURY        = 62297, // also + 10% hp
    // tower of storms
    SPELL_THORIMS_HAMMER    = 62912,
    SPELL_TOWER_OF_STORMS   = 65076,

	//TOWER Additional SPELLS
    SPELL_THORIM_S_HAMMER                       = 62911, // Tower of Storms
    SPELL_MIMIRON_S_INFERNO                     = 62909, // Tower of Flames
    SPELL_HODIR_S_FURY                          = 62533, // Tower of Frost
    SPELL_FREYA_S_WARD                          = 62906, // Tower of Nature
    SPELL_FREYA_SUMMONS                         = 62947, // Tower of Nature
    //TOWER ap & health spells
    SPELL_BUFF_TOWER_OF_STORMS                  = 65076,
    SPELL_BUFF_TOWER_OF_FLAMES                  = 65075,
    SPELL_BUFF_TOWER_OF_FR0ST                   = 65077,
    SPELL_BUFF_TOWER_OF_LIFE                    = 64482,
    //Additional Spells
    SPELL_LASH                                  = 65062,
    SPELL_FREYA_S_WARD_EFFECT_1                 = 62947,
    SPELL_FREYA_S_WARD_EFFECT_2                 = 62907,
    SPELL_AUTO_REPAIR                           = 62705,
    AURA_DUMMY_BLUE                             = 63294,
    AURA_DUMMY_GREEN                            = 63295,
    AURA_DUMMY_YELLOW                           = 63292,
    SPELL_LIQUID_PYRITE                         = 62494,
};

enum Mobs
{
    DEFENSE_TURRET      = 33142,
    KEEPER_OF_NORGANNON = 33686
};

enum Creatures
{
    MOB_MECHANOLIFT                             = 33214,
    MOB_LIQUID                                  = 33189,
    MOB_CONTAINER                               = 33218,
    MOB_THORIM_BEACON                           = 33365,
    MOB_MIMIRON_BEACON                          = 33370,
    MOB_HODIR_BEACON                            = 33212,
    MOB_FREYA_BEACON                            = 33367,
    NPC_THORIM_TARGET_BEACON                    = 33364,
    NPC_MIMIRON_TARGET_BEACON                   = 33369,
    NPC_HODIR_TARGET_BEACON                     = 33108,
    NPC_FREYA_TARGET_BEACON                     = 33366,
    NPC_LOREKEEPER                              = 33686, //Hard mode starter
    NPC_BRANZ_BRONZBEARD                        = 33579,
    NPC_DELORAH                                 = 33701,
};

enum Towers
{
    GO_TOWER_OF_STORMS = 194377,
    GO_TOWER_OF_FLAMES = 194371,
    GO_TOWER_OF_FROST = 194370,
    GO_TOWER_OF_LIFE = 194375,
};

enum Seats
{
    SEAT_PLAYER = 0,
    SEAT_TURRET = 1,
    SEAT_DEVICE = 2,
};

enum eAchievementData
{
    //ACHIEV_CHAMPION_OF_ULDUAR                   = 2903,
    //ACHIEV_CONQUEROR_OF_ULDUAR                  = 2904,
    ACHIEV_10_NUKED_FROM_ORBIT                  = 2915,
    ACHIEV_25_NUKED_FROM_ORBIT                  = 2917,
    ACHIEV_10_ORBITAL_BOMBARDMENT               = 2913,
    ACHIEV_25_ORBITAL_BOMBARDMENT               = 2918,
    ACHIEV_10_ORBITAL_DEVASTATION               = 2914,
    ACHIEV_25_ORBITAL_DEVASTATION               = 2916,
    ACHIEV_10_ORBIT_UARY                        = 3056,
    ACHIEV_25_ORBIT_UARY                        = 3057,
    ACHIEV_10_SHUTOUT                           = 2911,
    ACHIEV_25_SHUTOUT                           = 2912,
    ACHIEV_10_SIEGE_OF_ULDUAR                   = 2886,
    ACHIEV_25_SIEGE_OF_ULDUAR                   = 2887,
    ACHIEV_10_THREE_CAR_GARAGE                  = 2907, //no core support for using a vehicle
    ACHIEV_25_THREE_CAR_GARAGE                  = 2908, //no core support for using a vehicle
    ACHIEV_10_UNBROKEN                          = 2905,
    ACHIEV_25_UNBROKEN                          = 2906,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};

static LocationsXY Center[]=
{
    {354.8771f, -12.90240f, 409.803650f},
};

const LocationsXY PosSiege[5] =
{
    {-814.59f,-64.54f,429.92f,5.969f},
    {-784.37f,-33.31f,429.92f,5.096f},
    {-808.99f,-52.10f,429.92f,5.668f},
    {-798.59f,-44.00f,429.92f,5.663f},
    {-812.83f,-77.71f,429.92f,0.046f},
};

const LocationsXY PosChopper[5] =
{
    {-717.83f,-106.56f,430.02f,0.122f},
    {-717.83f,-114.23f,430.44f,0.122f},
    {-717.83f,-109.70f,430.22f,0.122f},
    {-718.45f,-118.24f,430.26f,0.052f},
    {-718.45f,-123.58f,430.41f,0.085f},
};

const LocationsXY PosDemolisher[5] =
{
    {-724.12f,-176.64f,430.03f,2.543f},
    {-766.70f,-225.03f,430.50f,1.710f},
    {-729.54f,-186.26f,430.12f,1.902f},
    {-756.01f,-219.23f,430.50f,2.369f},
    {-798.01f,-227.24f,429.84f,1.446f},
};


struct MANGOS_DLL_DECL boss_flame_leviathan : public ScriptedAI
{
    boss_flame_leviathan(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->setFaction(35);  // remove this when vehicules fixed!
        pCreature->SetVisibility(VISIBILITY_OFF);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBatteringRamTimer;
    uint32 m_uiFlameVentsTimer;
    uint32 m_uiMissileBarrageTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiGatheringSpeedTimer;
    uint32 m_uiSummonFlyerTimer;
    uint8 maxFlyers;

    bool isHardMode;
    bool isHodirsTower;
    bool isFreyasTower;
    bool isMimironsTower;
    bool isThorimsTower;

    uint32 m_uiFreyaWardTimer;
    uint32 m_uiMimironInfernoTimer;
    uint32 m_uiHodirFuryTimer;
    uint32 m_uiThorimHammerTimer;

    void Reset()
    {
        m_uiBatteringRamTimer   = 15000 + rand()%20000;
        m_uiFlameVentsTimer     = 15000 + rand()%10000;
        m_uiMissileBarrageTimer = 1000;
        m_uiPursueTimer         = 30000;
        m_uiGatheringSpeedTimer = 50000;
        m_uiSummonFlyerTimer    = 2000;
        maxFlyers = 10;

        isHardMode      = false;
        isHodirsTower   = false;
        isFreyasTower   = false;
        isMimironsTower = false;
        isThorimsTower  = false;

        m_uiFreyaWardTimer      = 40000 + urand(1000, 10000);
        m_uiMimironInfernoTimer = 40000 + urand(1000, 10000);
        m_uiHodirFuryTimer      = 40000 + urand(1000, 10000);
        m_uiThorimHammerTimer   = 40000 + urand(1000, 10000);

        m_creature->SetSpeedRate(MOVE_RUN, 0.3f);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_LEVIATHAN, IN_PROGRESS);
            if(m_pInstance->GetData(TYPE_LEVIATHAN_TP) != DONE)
                m_pInstance->SetData(TYPE_LEVIATHAN_TP, DONE);
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_LEVIATHAN, DONE);
            if(isHardMode)
                m_pInstance->SetData(TYPE_LEVIATHAN_HARD, DONE);
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, FAIL);
    }

    void KilledUnit(Unit *who)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    // TODO: effect 0 and effect 1 may be on different target
    void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_PURSUED)
            AttackStart(pTarget);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        /*if(spell->Id == 62472)
        vehicle->InstallAllAccessories();
        else if(spell->Id == SPELL_ELECTROSHOCK)
        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);*/
    }

    void DamageTaken(Unit *pDoneBy, uint32 &uiDamage)
    {
        if(m_creature->HasAura(SPELL_SYSTEMS_SHUTDOWN, EFFECT_INDEX_0))
            uiDamage += uiDamage/2;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // pursue
        if(m_uiPursueTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_CHANGE_1, m_creature); break;
                case 1: DoScriptText(SAY_CHANGE_2, m_creature); break;
                case 2: DoScriptText(SAY_CHANGE_3, m_creature); break;
            }
            DoScriptText(EMOTE_PURSUE, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_creature->AddThreat(pTarget, 100.0f);
                DoCast(pTarget, SPELL_PURSUED);
            }

            m_uiPursueTimer = 30000;
        }
        else m_uiPursueTimer -= uiDiff;

        // flame vents
        if(m_uiFlameVentsTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_VENTS);
            m_uiFlameVentsTimer = 30000 + rand()%20000;
        }
        else m_uiFlameVentsTimer -= uiDiff;

        // battering ram
        if(m_uiBatteringRamTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_BATTERING_RAM);
            m_uiBatteringRamTimer = 25000 + rand()%15000;
        }
        else m_uiBatteringRamTimer -= uiDiff;

        /* flyers
        if(m_uiSummonFlyerTimer < uiDiff)
        {
            m_creature->SummonCreature(MOB_MECHANOLIFT, m_creature->GetPositionX() + rand()%20, m_creature->GetPositionY() + rand()%20, m_creature->GetPositionZ() + 50, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 40000);
            m_uiSummonFlyerTimer = 2000;
        }
        else m_uiSummonFlyerTimer -= uiDiff;*/

        // missile barrage
        if(m_uiMissileBarrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_MISSILE_BARRAGE);
            m_uiMissileBarrageTimer = 3000 + rand()%2000;
        }
        else m_uiMissileBarrageTimer -= uiDiff;

        if(m_uiGatheringSpeedTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_GATHERING_SPEED);
            m_uiGatheringSpeedTimer = urand(50000, 60000);
        }
        else m_uiGatheringSpeedTimer -= uiDiff;

        // tower of freya
        if(isFreyasTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_LIFE);

            if(m_uiFreyaWardTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_FREYAS_WARD);
                m_uiFreyaWardTimer = 40000 + urand(1000, 10000);
            }
            else m_uiFreyaWardTimer -= uiDiff;
        }

        // tower of mimiron
        if(isMimironsTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_FLAMES);

            if(m_uiMimironInfernoTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_FREYAS_WARD);
                m_uiMimironInfernoTimer = 40000 + urand(1000, 10000);
            }
            else m_uiMimironInfernoTimer -= uiDiff;
        }

        // tower of hodir
        if(isHodirsTower)
        {
            m_creature->SetHealth(m_creature->GetHealth() + 0.1* m_creature->GetHealth());

            if(m_uiHodirFuryTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_HODIR_FURY);
                m_uiHodirFuryTimer = 40000 + urand(1000, 10000);
            }
            else m_uiHodirFuryTimer -= uiDiff;
        }

        // tower of thorim
        if(isThorimsTower)
        {
            DoCast(m_creature, SPELL_TOWER_OF_STORMS);

            if(m_uiThorimHammerTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_THORIMS_HAMMER);
                m_uiThorimHammerTimer = 40000 + urand(1000, 10000);
            }
            else m_uiThorimHammerTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_defense_turretAI : public ScriptedAI
{
    mob_defense_turretAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSpell_Timer;

    void Reset()
    {
        m_uiSpell_Timer = urand(10000, 15000);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SYSTEMS_SHUTDOWN)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_SEARING_FLAME);
            m_uiSpell_Timer = urand(10000, 15000);
        }else m_uiSpell_Timer -= uiDiff;        
    }
};

CreatureAI* GetAI_mob_defense_turret(Creature* pCreature)
{
    return new mob_defense_turretAI(pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathan(pCreature);
}

void AddSC_boss_flame_leviathan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flame_leviathan";
    newscript->GetAI = &GetAI_boss_flame_leviathan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_defense_turret";
    newscript->GetAI = &GetAI_mob_defense_turret;
    newscript->RegisterSelf();
}
