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
SDName: boss_halion
SD%Complete: 20%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

//TODO: Add meteor/portal mechanics, twilight cutter (2 orbs), sql spells, sql npcs, TEST

#include "precompiled.h"
#include "ruby_sanctum.h"

enum 
{
    //SPELLS
    //All
    SPELL_TWILIGHT_PRECISION                    = 78243, // Increases Halion's chance to hit by 5% and decreases all players' chance to dodge by 20%
    SPELL_BERSERK                               = 26663, // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500% for 5 min.  Also grants immunity to Taunt effects.
    SPELL_START_PHASE2                          = 74808, // Phases the caster into the Twilight realm, leaving behind a large rift.
    SPELL_TWILIGHT_ENTER                        = 74807, // Phases the caster into the Twilight realm

    //NEED SCRIPT
    SPELL_TAIL_LASH                             = 74531, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TWILIGHT_DIVISION                     = 75063, // Phases the caster, allowing him to exist and act simultaneously in both the material and Twilight realms.
    SPELL_FIERY_COMBUSTION                      = 74562, // Inflicts 4,000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
    SPELL_SOUL_CONSUMPTION                      = 74792, // Inflicts 4,000 Shadow damage every 2 seconds for 30 seconds to a random raider. Every time Soul Consumption does damage, it applies a stackable Mark of Consumption.
    SPELL_TWILIGHT_CUTTER                       = 77844, // Inflicts 13,875 to 16,125 Shadow damage every second to players touched by the shadow beam
    //CORPOREALITY
    SPELL_CORPOREALITY_EVEN                     = 74826, // Deals & receives normal damage
    SPELL_CORPOREALITY_20I                      = 74827, // Damage dealt increased by 10% & Damage taken increased by 15%
    SPELL_CORPOREALITY_40I                      = 74828, // Damage dealt increased by 30% & Damage taken increased by 50%
    SPELL_CORPOREALITY_60I                      = 74829, // Damage dealt increased by 60% & Damage taken increased by 100%
    SPELL_CORPOREALITY_80I                      = 74830, // Damage dealt increased by 100% & Damage taken increased by 200%
    SPELL_CORPOREALITY_100I                     = 74831, // Damage dealt increased by 200% & Damage taken increased by 400%
    SPELL_CORPOREALITY_20D                      = 74832, // Damage dealt reduced by 10% & Damage taken reduced by 15%
    SPELL_CORPOREALITY_40D                      = 74833, // Damage dealt reduced by 30% & Damage taken reduced by 50%
    SPELL_CORPOREALITY_60D                      = 74834, // Damage dealt reduced by 60% & Damage taken reduced by 100%
    SPELL_CORPOREALITY_80D                      = 74835, // Damage dealt reduced by 100% & Damage taken reduced by 200%
    SPELL_CORPOREALITY_100D                     = 74836, // Damage dealt reduced by 200% & Damage taken reduced by 400% 
    //METEOR STRIKE
    SPELL_METEOR_STRIKE                         = 75877, // Script Start
    SPELL_METEOR                                = 74637, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_IMPACT                      = 74641, // IMPACT ZONE FOR METEOR
    SPELL_METEOR_LAND                           = 74648, // LANDING EFFECT
    SPELL_METEOR_FLAME                          = 74718, // FLAME FROM METEOR
    //N10
    SPELL_FLAME_BREATH_0                        = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_0                         = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_0                         = 75484, // Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //H10 & N25
    SPELL_FLAME_BREATH_1                        = 74527, // Inflicts 26,250 to 33,750 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_1                         = 74806, // Inflicts 26,250 to 33,750 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_1                         = 75485, // Inflicts 4,500 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //H25
    SPELL_FLAME_BREATH_2                        = 74404, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_2                         = 75956, // Inflicts 33,688 to 43,312 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_2                         = 75486, // Inflicts 6,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //NPC
    NPC_LIVING_EMBER                            = 40683,
    NPC_LIVING_INFERNO                          = 40681,
    //Summons
    NPC_METEOR_STRIKE                           = 40029, //casts "impact zone" then meteor
    NPC_METEORFLAME                             = 36673, //meteor flame
    NPC_SHADOW_PULSAR_N                         = 40083, //spinning orb N spawn
    NPC_SHADOW_PULSAR_S                         = 40100, //spinning orb S spawn

    //SAYS
    SAY_HALION_SPAWN                = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
    SAY_HALION_AGGRO                = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1               = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2               = -1666103, //17502 Ha Ha Ha!
    SAY_HALION_DEATH                = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
    SAY_HALION_BERSERK              = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1            = -1666106, //17505 The heavens burn!
    SAY_HALION_SPECIAL_2            = -1666107, //17506 Beware the shadow!
    SAY_HALION_PHASE_2              = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
    SAY_HALION_PHASE_3              = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!

 EMOTE_WARNING     = -1666110, //orbs charge warning
};

static float m_xflame[12][8]=
{
    //RND0 RND1 RND2 RND3
    //x y x y x y x y
    {2.0f,0.0f,2.0f,1.0f,1.5f,1.5f,1.0f,2.0f}, //NPC0 SPAWN1
    {4.5f,0.0f,4.0f,2.0f,3.0f,3.0f,2.0f,4.0f}, //NPC0 SPAWN2
    {7.5f,0.0f,6.5f,3.5f,4.5f,4.5f,3.5f,6.5f}, //NPC0 SPAWN3
    {0.0f,2.0f,-2.0f,-1.0f,-1.5f,1.5f,-1.0f,2.0f}, //NPC1 SPAWN1
    {0.0f,4.5f,-4.0f,-2.0f,-3.0f,3.0f,-2.0f,4.0f}, //NPC1 SPAWN2
    {0.0f,7.5f,-6.5f,-3.5f,-4.5f,4.5f,-3.5f,6.5f}, //NPC1 SPAWN3
    {-2.0f, 0.0f,1.0f,2.0f,-1.5f,-1.5f,-1.0f,-2.0f}, //NPC2 SPAWN1
    {-4.5f, 0.0f,2.0f,4.0f,-3.0f,-3.0f,-2.0f,-4.0f}, //NPC2 SPAWN2
    {-7.5f,0.0f,3.5f,6.5f,-4.5f,-4.5f,-3.5f,-6.5f}, //NPC2 SPAWN3
    {0.0f,-2.0f,-1.0f,-2.0f,1.5f,-1.5f,1.0f,-2.0f}, //NPC3 SPAWN1
    {0.0f,-4.5f,-2.0f,-4.0f,3.0f,-3.0f,2.5f,-4.0f}, //NPC3 SPAWN2
    {0.0f,-7.5f,-3.5f,-6.5f,4.5f,-4.5f,3.5f,-6.5f}, //NPC3 SPAWN3
};

static float m_pulsar[16][2]=
{
    {0.0f, 0.0f}, //S
    {0.0f, 0.0f}, //SSW
    {0.0f, 0.0f}, //SW
    {0.0f, 0.0f}, //SWW
    {0.0f, 0.0f}, //W
    {0.0f, 0.0f}, //NWW
    {0.0f, 0.0f}, //NW
    {0.0f, 0.0f}, //NNW
    {0.0f, 0.0f}, //N
    {0.0f, 0.0f}, //NNE
    {0.0f, 0.0f}, //NE
    {0.0f, 0.0f}, //NEE
    {0.0f, 0.0f}, //E
    {0.0f, 0.0f}, //SEE
    {0.0f, 0.0f}, //SE
    {0.0f, 0.0f}, //SSE
};

/*######
## boss_halion_real (Physical version)
######*/
struct MANGOS_DLL_DECL boss_halion_realAI : public ScriptedAI
{
    boss_halion_realAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint8 p_phase;
    bool intro;

    void Reset()
    {
        if(!pInstance)
            return;

        p_phase = 0;
        if (m_creature->isAlive()) pInstance->SetData(TYPE_HALION, NOT_STARTED);
       // resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);

        if (intro || !pInstance) return;
            else
            {
                DoScriptText(-1666100,m_creature);
                intro = true;
            }
    }

    void JustReachedHome()
    {
        if (!pInstance)
            return;

        pInstance->SetData(TYPE_HALION, FAIL);
        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;

        DoScriptText(-1666104,m_creature);
        if (Creature* pclone = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_HALION_TWILIGHT)))
            if (!pclone->isAlive())
                pInstance->SetData(TYPE_HALION, DONE);
        else
            pInstance->SetData(TYPE_HALION, SPECIAL);
        pInstance->SetData(DATA_HEALTH_HALION_P, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_HALION, IN_PROGRESS);     
        DoScriptText(-1666101,m_creature);
        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance)
            return;
        if (!m_creature || !m_creature->isAlive())
            return;
        if (pDoneBy->GetGUID() == m_creature->GetGUID())
            return;

        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
  pInstance->SetData(DATA_P_1, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance)
            return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_T) && pInstance->GetData(DATA_HEALTH_HALION_T) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_T));

        if (m_creature->GetHealthPercent() < 99.5f && p_phase == 0)
        {
            pInstance->DoRespawnGameObject(pInstance->GetData64(GO_HALION_FIRE_RING),MINUTE*5);
            p_phase = 1;
        }

        if (m_creature->GetHealthPercent() < 75.0f && p_phase == 1)
        {
            DoScriptText(-1666108,m_creature);
            p_phase = 2;
        }

        if (m_creature->GetHealthPercent() < 50.0f && p_phase == 2)
        {
            DoScriptText(-1666109,m_creature);
            p_phase = 3;
        }

        switch (p_phase)
        {
            case 0: //SPAWNED
                break;
            case 1: //PHASE 1 PHYSICAL REALM
                //timedCast(SPELL_FLAME_BREATH_0, uiDiff);
                //timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                /* Meteor Needs Test */
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    DoCast(m_creature, SPELL_METEOR);
                    {
                        DoScriptText(-1666106,m_creature,pTarget);
                        float fPosX, fPosY, fPosZ;
                        m_creature->GetPosition(fPosX, fPosY, fPosZ);
      pInstance->SetData(DATA_X, fPosX);
      pInstance->SetData(DATA_Y, fPosY);
      pInstance->SetData(DATA_Z, fPosZ);

                        //if (Unit* pMeteor = doSummon(NPC_METEOR_STRIKE, fPosX, fPosY, fPosZ))
                            //pMeteor->AddThreat(pTarget, 100.0f);
                    };
                }
                break;
            case 2: //DEPHASE HALION FORCE TO TWILIGHT REALM
                //setflag unattackable, unselectable, remove combat
                break;
            case 3: //PHASE 3 BOTH REALMS
                //timedCast(SPELL_FLAME_BREATH_0, uiDiff);
                //timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                DoCast(m_creature, SPELL_TWILIGHT_DIVISION);
                // Needs Script
               // timedCast(SPELL_METEOR_STRIKE, uiDiff); 
                //needs test
    switch (pInstance->GetData(DATA_P_BUFF))
    {
     case 0:
      DoCast(m_creature, SPELL_CORPOREALITY_EVEN);
     break;
     case 1:
      DoCast(m_creature, SPELL_CORPOREALITY_20I);
     break;
     case 2:
      DoCast(m_creature, SPELL_CORPOREALITY_40I);
     break;
     case 3:
      DoCast(m_creature, SPELL_CORPOREALITY_60I);
     break;
     case 4:
      DoCast(m_creature, SPELL_CORPOREALITY_80I);
     break;
     case 5:
      DoCast(m_creature, SPELL_CORPOREALITY_100I);
     break;
     case 6:
      DoCast(m_creature, SPELL_CORPOREALITY_20D);
     break;
     case 7:
      DoCast(m_creature, SPELL_CORPOREALITY_40D);
     break;
     case 8:
      DoCast(m_creature, SPELL_CORPOREALITY_60D);
     break;
     case 9:
      DoCast(m_creature, SPELL_CORPOREALITY_80D);
     break;
     case 10:
      DoCast(m_creature, SPELL_CORPOREALITY_100D);
     break;
    }

    break;
            default:
                break;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_real(Creature* pCreature)
{
    return new boss_halion_realAI(pCreature);
}

/*######
## boss_halion_twilight (Twilight version)
######*/

struct MANGOS_DLL_DECL boss_halion_twilightAI : public ScriptedAI
{
    boss_halion_twilightAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint8 t_phase;

    void Reset() 
    {
        if(!pInstance)
            return;

        t_phase = 0;
        pInstance->SetData(TYPE_HALION, NOT_STARTED);
        //resetTimers();
    }

    void JustReachedHome()
    {
        if (!pInstance)
            return;

        pInstance->SetData(TYPE_HALION, FAIL);
        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;

        DoScriptText(-1666104,m_creature);
        pInstance->SetData(DATA_HEALTH_HALION_T, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());

        if (t_phase == 0)
            t_phase = 2;

  //turn on the orb cutting
  pInstance->SetData(DATA_ORB, 1);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance)
            return;
        if (!m_creature || !m_creature->isAlive())
            return;
        if(pDoneBy->GetGUID() == m_creature->GetGUID())
            return;

        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
  pInstance->SetData(DATA_T_1, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_P) && pInstance->GetData(DATA_HEALTH_HALION_P) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_P));

        if (m_creature->GetHealthPercent() < 50.0f && t_phase == 2)
        {
            DoScriptText(-1666109,m_creature);
            t_phase = 3;
        }

        DoCast(m_creature, SPELL_TWILIGHT_PRECISION);

  switch (t_phase)
        {
            case 0: //SPAWNED
                break;
            case 1: //FAIL
                //HOW DID U GET HERE
                break;
            case 2: //PHASE 2 TWILIGHT REALM
                DoCast(m_creature, SPELL_TWILIGHT_DIVISION);
                DoCast(m_creature, SPELL_DUSK_SHROUD_0);
                //timedCast(SPELL_DARK_BREATH_0, uiDiff);
                //timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                break;
            case 3: //PHASE 3 BOTH REALMS
                DoCast(m_creature, SPELL_TWILIGHT_DIVISION);
                DoCast(m_creature, SPELL_DUSK_SHROUD_0);
                //timedCast(SPELL_DARK_BREATH_0, uiDiff);
                //timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                //needs test
    switch (pInstance->GetData(DATA_P_BUFF))
    {
     case 0:
      DoCast(m_creature, SPELL_CORPOREALITY_EVEN);
     break;
     case 1:
      DoCast(m_creature, SPELL_CORPOREALITY_20I);
     break;
     case 2:
      DoCast(m_creature, SPELL_CORPOREALITY_40I);
     break;
     case 3:
      DoCast(m_creature, SPELL_CORPOREALITY_60I);
     break;
     case 4:
      DoCast(m_creature, SPELL_CORPOREALITY_80I);
     break;
     case 5:
      DoCast(m_creature, SPELL_CORPOREALITY_100I);
     break;
     case 6:
      DoCast(m_creature, SPELL_CORPOREALITY_20D);
     break;
     case 7:
      DoCast(m_creature, SPELL_CORPOREALITY_40D);
     break;
     case 8:
      DoCast(m_creature, SPELL_CORPOREALITY_60D);
     break;
     case 9:
      DoCast(m_creature, SPELL_CORPOREALITY_80D);
     break;
     case 10:
      DoCast(m_creature, SPELL_CORPOREALITY_100D);
     break;
    }
    
    break;
            default:
                break;
        }
        
  //timedCast(SPELL_BERSERK, uiDiff);

  DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_twilight(Creature* pCreature)
{
    return new boss_halion_twilightAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_meteorAI : public ScriptedAI
{
    mob_halion_meteorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 timer;
 uint8 flames;
 uint8 tick;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
  flames = 0;
  tick = 0;
  timer = 1000;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance)
            return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

  /*TIME - SPELL - INFO
    0000 -       - spawned
    0000 - 74637 - cast meteor ball start
    1000 -       - cast meteor ball finished & actual ball appears and begins fall to ground
    1000 - 74641 - on ground impact zone appears
    6000 - 74648 - ball hits ground
    6000 -       - start timer of flame spawn
  */
        if (timer < diff)
        {
   tick ++;
   timer = 1000;
        }else timer -= diff;
  
  if (tick == 0)
  {
   DoCast(m_creature, SPELL_METEOR);
  }
   
  if (tick == 1)
  {
   DoCast(m_creature, SPELL_METEOR_IMPACT);
  }
  
  if (tick == 6) //spawn em all in one go doesn't seam like the flame creeps like marrowgars flame
  {
   DoCast(m_creature, SPELL_METEOR_LAND);
   
   switch (DATA_RND) 
   {
    case 0:
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[0][0], DATA_Y + m_xflame[0][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[1][0], DATA_Y + m_xflame[1][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[2][0], DATA_Y + m_xflame[2][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[3][0], DATA_Y + m_xflame[3][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[4][0], DATA_Y + m_xflame[4][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[5][0], DATA_Y + m_xflame[5][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[6][0], DATA_Y + m_xflame[6][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[7][0], DATA_Y + m_xflame[7][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[8][0], DATA_Y + m_xflame[8][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[9][0], DATA_Y + m_xflame[9][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[10][0], DATA_Y + m_xflame[10][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[11][0], DATA_Y + m_xflame[11][1], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
     break;
    case 1:
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[0][2], DATA_Y + m_xflame[0][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[1][2], DATA_Y + m_xflame[1][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[2][2], DATA_Y + m_xflame[2][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[3][2], DATA_Y + m_xflame[3][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[4][2], DATA_Y + m_xflame[4][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[5][2], DATA_Y + m_xflame[5][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[6][2], DATA_Y + m_xflame[6][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[7][2], DATA_Y + m_xflame[7][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[8][2], DATA_Y + m_xflame[8][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[9][2], DATA_Y + m_xflame[9][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[10][2], DATA_Y + m_xflame[10][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[11][2], DATA_Y + m_xflame[11][3], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
     break;
    case 2:
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[0][4], DATA_Y + m_xflame[0][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[1][4], DATA_Y + m_xflame[1][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[2][4], DATA_Y + m_xflame[2][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[3][4], DATA_Y + m_xflame[3][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[4][4], DATA_Y + m_xflame[4][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[5][4], DATA_Y + m_xflame[5][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[6][4], DATA_Y + m_xflame[6][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[7][4], DATA_Y + m_xflame[7][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[8][4], DATA_Y + m_xflame[8][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[9][4], DATA_Y + m_xflame[9][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[10][4], DATA_Y + m_xflame[10][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[11][4], DATA_Y + m_xflame[11][5], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
     break;
    case 3:
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[0][6], DATA_Y + m_xflame[0][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[1][6], DATA_Y + m_xflame[1][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[2][6], DATA_Y + m_xflame[2][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[3][6], DATA_Y + m_xflame[3][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[4][6], DATA_Y + m_xflame[4][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[5][6], DATA_Y + m_xflame[5][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[6][6], DATA_Y + m_xflame[6][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[7][6], DATA_Y + m_xflame[7][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[8][6], DATA_Y + m_xflame[8][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[9][6], DATA_Y + m_xflame[9][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[10][6], DATA_Y + m_xflame[10][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
      m_creature->SummonCreature(NPC_METEORFLAME, DATA_X + m_xflame[11][6], DATA_Y + m_xflame[11][7], DATA_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
     break;
             };
   
    }  
     }
};

CreatureAI* GetAI_mob_halion_meteor(Creature* pCreature)
{
    return new mob_halion_meteorAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_flameAI : public ScriptedAI
{
    mob_halion_flameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoCast(m_creature, SPELL_METEOR_FLAME);
    }
};
CreatureAI* GetAI_mob_halion_flame(Creature* pCreature)
{
    return new mob_halion_flameAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_orb_0AI : public ScriptedAI
{
    mob_halion_orb_0AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
 uint32 timer;
 uint32 tick;
 uint64 orb_targetGUID;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

  timer = 1000;
  tick =0;
  orb_targetGUID = 0;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //TODO
  //MOVEMENT 16 point circle
  if (timer < uiDiff)
        {
   tick ++;
   timer = 1000;
        }else timer -= uiDiff;
  
  if (tick == 25) //WARNING 5 sec pre warn on cutting
  {
   DoScriptText(EMOTE_WARNING, m_creature);
  }
   
  if (tick == 30) // cutting using other orb as target
  {
   if (pInstance->GetData(DATA_ORB) == 1)
   {
    Creature *temp = m_creature->SummonCreature(NPC_ORB1, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 1200000);

    if (temp)
    {
     orb_targetGUID = temp->GetGUID();

     if (Unit *orb_target = Unit::GetUnit(*m_creature, orb_targetGUID))
     {
      orb_target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
      orb_target->SetDisplayId(11686);
      DoCastSpellIfCan(orb_target, SPELL_TWILIGHT_CUTTER);
     }

     if (Unit *orb_target = Unit::GetUnit(*m_creature, orb_targetGUID))
     {
      orb_target->CastSpell(m_creature, SPELL_TWILIGHT_CUTTER, true);
     }
   }
  }
   tick = 0;
  }

    }
};
CreatureAI* GetAI_mob_halion_orb_0(Creature* pCreature)
{
    return new mob_halion_orb_0AI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_orb_1AI : public ScriptedAI
{
    mob_halion_orb_1AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //TODO
  //MOVEMENT 16 point circle
  //WARNING 5 sec pre warn on cutting (EMOTE)
  //CAST cutter on other orb  with 
  //REPEAT every 30 sec
    }
};
CreatureAI* GetAI_mob_halion_orb_1(Creature* pCreature)
{
    return new mob_halion_orb_1AI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_portal_inAI : public ScriptedAI
{
    mob_halion_portal_inAI(Creature* pCreature) : ScriptedAI(pCreature) {
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }
    ScriptedInstance* pInstance;

    void Reset() 
    {
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }
 
 void UpdateAI(const uint32 uiDiff)
    {
  //none
    }
};

CreatureAI* GetAI_mob_halion_portal_in(Creature* pCreature)
{
    return new mob_halion_portal_inAI(pCreature);
};

bool GossipHello_mob_halion_portal_in(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) 
  return true;

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    player->CastSpell(player,SPELL_TWILIGHT_ENTER,false);
    player->CLOSE_GOSSIP_MENU();
    return true;
};

struct MANGOS_DLL_DECL mob_halion_portal_outAI : public ScriptedAI
{
    mob_halion_portal_outAI(Creature* pCreature) : ScriptedAI(pCreature) {
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }
    ScriptedInstance* pInstance;

    void Reset() 
    {
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }
 
 void UpdateAI(const uint32 uiDiff)
    {
  //none
    }
};

CreatureAI* GetAI_mob_halion_portal_out(Creature* pCreature)
{
    return new mob_halion_portal_outAI(pCreature);
};

bool GossipHello_mob_halion_portal_out(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) 
  return true;

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    //PLEASE FIX THIS
 //player->RemoveAurasDueToSpell(SPELL_TWILIGHT_ENTER);
    player->CLOSE_GOSSIP_MENU();
    return true;
}
struct MANGOS_DLL_DECL mob_halion_controlAI : public ScriptedAI
{
    mob_halion_controlAI(Creature* pCreature) : ScriptedAI(pCreature) {
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }
    ScriptedInstance* pInstance;
 uint32 timer;
 uint32 timer2;
 uint32 timer3;

    void Reset() 
    {
  timer = 1000;
  timer2 = 5000;
  timer3 = 3000;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }
 
 void UpdateAI(const uint32 diff)
    {
  //corporeality get damage every second
  if (timer < diff)
        {
   //CORPOREALITY
   pInstance->SetData(DATA_P_0, (pInstance->GetData(DATA_P_0) + (pInstance->GetData(DATA_P_1))));
   pInstance->SetData(DATA_T_0, (pInstance->GetData(DATA_T_0) + (pInstance->GetData(DATA_T_1))));
   timer = 1000;
        }else timer -= diff;

  //average it out
  if (timer2 < diff)
        {
   //CORPOREALITY
   pInstance->SetData(DATA_P_2, (pInstance->GetData(DATA_P_0) / 5));
   pInstance->SetData(DATA_T_2, (pInstance->GetData(DATA_T_0) / 5));
   //clean up
   pInstance->SetData(DATA_P_0, 0);
   pInstance->SetData(DATA_T_0, 0);
   pInstance->SetData(DATA_P_1, 0);
   pInstance->SetData(DATA_T_1, 0);
   //the buff Physical side
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 1500) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 2999))
   {
    //BUFF LVL 1
    pInstance->SetData(DATA_P_BUFF, 1);
    pInstance->SetData(DATA_T_BUFF, 6);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 3000) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 4499))
   {
    //BUFF LVL 2
    pInstance->SetData(DATA_P_BUFF, 2);
    pInstance->SetData(DATA_T_BUFF, 7);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 4500) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 5999))
   {
    //BUFF LVL 3
    pInstance->SetData(DATA_P_BUFF, 3);
    pInstance->SetData(DATA_T_BUFF, 8);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 6000) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 7499))
   {
    //BUFF LVL 4PT_BUFF, 1);
    pInstance->SetData(DATA_P_BUFF, 4);
    pInstance->SetData(DATA_T_BUFF, 9);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 7500))
   {
    //BUFF LVL 5
    pInstance->SetData(DATA_P_BUFF, 5);
    pInstance->SetData(DATA_T_BUFF, 10);
   }
   else
    //NORMAL BUFF
    pInstance->SetData(DATA_P_BUFF, 0);

   //the buff twilight side
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 1500) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 2999))
   {
    //BUFF LVL 1
    pInstance->SetData(DATA_P_BUFF, 6);
    pInstance->SetData(DATA_T_BUFF, 1);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 3000) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 4499))
   {
    //BUFF LVL 2
    pInstance->SetData(DATA_P_BUFF, 7);
    pInstance->SetData(DATA_T_BUFF, 2);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 4500) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 5999))
   {
    //BUFF LVL 3
    pInstance->SetData(DATA_P_BUFF, 8);
    pInstance->SetData(DATA_T_BUFF, 3);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 6000) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 7499))
   {
    //BUFF LVL 4
    pInstance->SetData(DATA_P_BUFF, 9);
    pInstance->SetData(DATA_T_BUFF, 4);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 7500))
   {
    //BUFF LVL 5
    pInstance->SetData(DATA_P_BUFF, 10);
    pInstance->SetData(DATA_T_BUFF, 5);
   }
   else
    //NORMAL BUFF
    pInstance->SetData(DATA_T_BUFF, 0);
   
    
    
    
   timer2 = 5000;
        }else timer2 -= diff;

  if (timer3 < diff)
        {
   //RND FLAME DIRECTION
   switch (urand(0,3))
   {
    case 0:
     pInstance->SetData(DATA_RND, 0);
     break;
    case 1:
     pInstance->SetData(DATA_RND, 1);
     break;
    case 2:
     pInstance->SetData(DATA_RND, 2);
     break;
    case 3:
     pInstance->SetData(DATA_RND, 3);
     break;
   };

   timer3 = 3000;
        }else timer3 -= diff;
    }
};

CreatureAI* GetAI_mob_halion_control(Creature* pCreature)
{
    return new mob_halion_controlAI(pCreature);
};

void AddSC_boss_halion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_halion_real";
    newscript->GetAI = &GetAI_boss_halion_real;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_halion_twilight";
    newscript->GetAI = &GetAI_boss_halion_twilight;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_meteor";
    newscript->GetAI = &GetAI_mob_halion_meteor;
    newscript->RegisterSelf();
 
 newscript = new Script;
    newscript->Name = "mob_halion_flame";
    newscript->GetAI = &GetAI_mob_halion_flame;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_orb_0";
    newscript->GetAI = &GetAI_mob_halion_orb_0;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_orb_1";
    newscript->GetAI = &GetAI_mob_halion_orb_1;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_portal_in";
    newscript->GetAI = &GetAI_mob_halion_portal_in;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_portal_out";
    newscript->GetAI = &GetAI_mob_halion_portal_out;
    newscript->RegisterSelf();

 newscript = new Script;
    newscript->Name = "mob_halion_control";
    newscript->GetAI = &GetAI_mob_halion_control;
    newscript->RegisterSelf();
}
