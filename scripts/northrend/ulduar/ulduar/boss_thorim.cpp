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
SDName: boss_thorim
SD%Complete: 90%
SDComment: Implement lightning orbs, summon Sit on the platform in the first 3 min.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO1          = -1603221,
    SAY_AGGRO2          = -1603222,
    SAY_SPECIAL1        = -1603223,
    SAY_SPECIAL2        = -1603224,
    SAY_SPECIAL3        = -1603225,
    SAY_JUMP            = -1603226,
    SAY_SLAY1           = -1603227,
    SAY_SLAY2           = -1603228,
    SAY_BERSERK         = -1603229,
    SAY_ARENA_WIPE      = -1603230,
    SAY_DEATH           = -1603231,
    SAY_OUTRO1          = -1603232,
    SAY_OUTRO2          = -1603233,
    SAY_OUTRO3          = -1603234,
    SAY_OUTRO_HARD1     = -1603235,
    SAY_OUTRO_HARD2     = -1603236,
    SAY_OUTRO_HARD3     = -1603237,

    // Sif
    SAY_SIF_INTRO           = -1603185,
    SAY_SIF_EVENT           = -1603186,
    SAY_SIF_DESPAWN         = -1603187,

    // spells
    // phase1
    SPELL_SHEAT_OF_LIGHTNING        = 62276,
    SPELL_STORMHAMMER               = 62042,
    SPELL_DEAFENING_THUNDER         = 62470,
    SPELL_LIGHTNING_SHOCK           = 62017,
    SPELL_CHARGE_ORB                = 62016,
    SPELL_BERSERK_ADDS              = 62560,    // 5 min phase 1 -> for adds
    SPELL_SUMMON_LIGHTNING_ORB      = 62391,
    MOB_LIGHTNING_ORB               = 33138,
    // phase2
    SPELL_TOUTCH_OF_DOMINION        = 62565,    // not available in hard mode
    SPELL_CHAIN_LIGHTNING           = 62131,
    SPELL_CHAIN_LIGHTNING_H         = 64390,
    SPELL_LIGHTNING_CHARGE          = 62279,
    SPELL_UNBALANCING_STRIKE        = 62130,
    SPELL_BERSERK                   = 26662,    // 5 min phase 2

    // hard mode
    SPELL_FROSTBOLT_VOLLEY          = 62580,
    SPELL_FROSTBOLT_VOLLEY_H        = 62604,
    SPELL_FROST_NOVA                = 62597,
    SPELL_FROST_NOVA_H              = 62605,
    SPELL_BLIZZARD                  = 62576,
    SPELL_BLIZZARD_H                = 62602,
    NPC_SIF                         = 33196,
    SPELL_SOUL_CHANNEL              = 40401,

    // arena
    MOB_DARK_RUNE_CHAMPION          = 32876,
    MOB_DARK_RUNE_COMMONER          = 32904,
    MOB_DARK_RUNE_EVOKER            = 32878,
    MOB_DARK_RUNE_WARBRINGER        = 32877,

    // mobs spells
    // acolyte
    SPELL_GREATER_HEAL              = 62334,
    SPELL_GREATER_HEAL_H            = 62442,
    SPELL_RENEW                     = 62333,
    SPELL_RENEW_H                   = 62441,
    SPELL_HOLY_SMITE                = 62335,
    SPELL_HOLY_SMITE_H              = 62443,
    // champion
    SPELL_MORTAL_STRIKE             = 35054,
    SPELL_CHARGE_CHAMPION           = 32323,
    SPELL_WHIRLWIND                 = 15578,
    // commoner         
    SPELL_LOW_BLOW                  = 62326,
    SPELL_PUMMEL                    = 38313,
    // evoker
    SPELL_RUNIC_LIGHTNING           = 62327,
    SPELL_RUNIC_LIGHTNING_H         = 62445,
    SPELL_RUNIC_MENDING             = 62328,
    SPELL_RUNIC_MENDING_H           = 62446,
    SPELL_RUNIC_SHIELD              = 62321,
    SPELL_RUNIC_SHIELD_H            = 62529,
    // warbringer
    SPELL_RUNIC_STRIKE              = 62322,
    SPELL_AURA_CELERITY             = 62320,

    // ring guard
    SPELL_WHIRLING_TRIP             = 64151,
    SPELL_IMPALE                    = 62331,
    SPELL_IMPALE_H                  = 62418,
    // honor guard
    SPELL_CLEAVE                    = 42724, 
    SPELL_HAMSTRING                 = 48639,
    SPELL_SHIELD_SMASH              = 62332,
    SPELL_SHIELD_SMASH_H            = 62420,

    // hallway
    MOB_DARK_RUNE_ACOLYTE           = 33110,
    MOB_IRON_RING_GUARD             = 32874,
    MINIBOSS_RUNIC_COLOSSUS         = 32872,
    SPELL_SMASH                     = 62339,
    SPELL_RUNIC_BARRIER             = 62338,
    SPELL_CHARGE                    = 62613,
    SPELL_CHARGE_H                  = 62614,

    MOB_IRON_HOHOR_GUARD            = 32875,
    MINIBOSS_ANCIENT_RUNE_GIANT     = 32873,
    SPELL_RUNIC_FORTIFICATION       = 62942,
    SPELL_STOMP                     = 62411,
    SPELL_STOMP_H                   = 62413,
    SPELL_RUNE_DETONATION           = 62526,

    ACHIEV_LOSE_ILLUSION            = 3176,
    ACHIEV_LOSE_ILLUSION_H          = 3183,
    ACHIEV_SIFFED                   = 2977,
    ACHIEV_SIFFED_H                 = 2978,
};
#define LOC_Z                       419.5f  
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY ArenaLoc[]=
{
    {2158.082f, -240.572f},
    {2111.883f, -240.561f},
    {2105.243f, -274.499f},
    {2163.927f, -277.834f},
    {2104.865f, -251.027f},
    {2167.612f, -262.128f},
};

// dark rune acolyte
struct MANGOS_DLL_DECL mob_dark_rune_acolyteAI : public ScriptedAI
{
    mob_dark_rune_acolyteAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

    bool m_bIsRegularMode;
	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 4))
            {
                case 0:
                case 1:
                    if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_GREATER_HEAL : SPELL_GREATER_HEAL_H);
				break;
				case 2:
                case 3:
                    if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_RENEW : SPELL_RENEW_H);
				break;
                case 4:
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_HOLY_SMITE : SPELL_HOLY_SMITE_H);
                break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_acolyte(Creature* pCreature)
{
    return new mob_dark_rune_acolyteAI(pCreature);
}

// dark rune champion
struct MANGOS_DLL_DECL mob_dark_rune_championAI : public ScriptedAI
{
    mob_dark_rune_championAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
	}

	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 2))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
				break;
				case 1:
                    DoCast(m_creature->getVictim(), SPELL_CHARGE_CHAMPION);
				break;
                case 2:
                    DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
				break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_champion(Creature* pCreature)
{
    return new mob_dark_rune_championAI(pCreature);
}

// dark rune commoner
struct MANGOS_DLL_DECL mob_dark_rune_commonerAI : public ScriptedAI
{
    mob_dark_rune_commonerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
	}

	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 1))
            {
                case 0:
					DoCast(m_creature->getVictim(), SPELL_LOW_BLOW);
				break;
				case 1:
					DoCast(m_creature->getVictim(), SPELL_PUMMEL);
				break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_commoner(Creature* pCreature)
{
    return new mob_dark_rune_commonerAI(pCreature);
}

// dark rune evoker
struct MANGOS_DLL_DECL mob_dark_rune_evokerAI : public ScriptedAI
{
    mob_dark_rune_evokerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

    bool m_bIsRegularMode;
	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 4))
            {
                case 0:
                case 1:
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_RUNIC_LIGHTNING : SPELL_RUNIC_LIGHTNING_H);
				break;
				case 2:
                case 3:
                    if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_RUNIC_MENDING : SPELL_RUNIC_MENDING_H);
				break;
                case 4:
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_RUNIC_SHIELD : SPELL_RUNIC_SHIELD_H);
                break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_evoker(Creature* pCreature)
{
    return new mob_dark_rune_evokerAI(pCreature);
}

// dark rune warbringer
struct MANGOS_DLL_DECL mob_dark_rune_warbringerAI : public ScriptedAI
{
    mob_dark_rune_warbringerAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        Reset();
	}

	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(4000, 7000);
        DoCast(m_creature, SPELL_AURA_CELERITY);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_RUNIC_STRIKE);
            m_uiSpell_Timer = urand(4000, 7000);
        }else m_uiSpell_Timer -= uiDiff;        

        DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_warbringer(Creature* pCreature)
{
    return new mob_dark_rune_warbringerAI(pCreature);
}

// dark rune ring guard
struct MANGOS_DLL_DECL mob_dark_rune_ring_guardAI : public ScriptedAI
{
    mob_dark_rune_ring_guardAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

    bool m_bIsRegularMode;
	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 1))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_WHIRLING_TRIP);
				break;
				case 1:
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
				break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_ring_guard(Creature* pCreature)
{
    return new mob_dark_rune_ring_guardAI(pCreature);
}

// dark rune honor guard
struct MANGOS_DLL_DECL mob_dark_rune_honor_guardAI : public ScriptedAI
{
    mob_dark_rune_honor_guardAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
	}

    bool m_bIsRegularMode;
	uint32 m_uiSpell_Timer;

    void Reset()
    {
		m_uiSpell_Timer = urand(3000, 6000);
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiSpell_Timer < uiDiff)
        {
			switch(urand(0, 2))
            {
                case 0:
					DoCast(m_creature->getVictim(), SPELL_CLEAVE);
				break;
				case 1:
                    DoCast(m_creature->getVictim(), SPELL_HAMSTRING);
				break;
                case 2:
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHIELD_SMASH : SPELL_SHIELD_SMASH_H);
				break;
			}
            m_uiSpell_Timer = urand(3000, 6000);
        }else m_uiSpell_Timer -= uiDiff;        
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_dark_rune_honor_guard(Creature* pCreature)
{
    return new mob_dark_rune_honor_guardAI(pCreature);
}

struct MANGOS_DLL_DECL boss_thorimAI : public ScriptedAI
{
    boss_thorimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint8 phase;

    uint32 arenaBerserkTimer;
    uint32 berserkTimer;
    uint32 arenaYellTimer;
    uint32 stormHammerTimer;
    uint32 deafeningThunderTimer;
    uint32 chargeOrbTimer;
    uint32 summonWavesTimer;
    uint32 minibossCheckTimer;

    uint32 chainLightningTimer;
    uint32 lightningChargeTimer;
    uint32 unbalancingStrikeTimer;

    uint32 phase2Timer;
    uint32 hardModeTimer;
    uint8 i;
    uint8 k;

    bool phaseEnd;
    bool isHardMode;

    uint64 m_uiSifGUID;

    // intro & outro
    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;
    bool isIntro;
    uint32 IntroTimer;
    uint32 IntroStep;

    // mob list check
    std::list<Creature*> lIronDwarfes;

    void Reset()
    {
        phase = -1;
        SetCombatMovement(false);

        isHardMode          = true;
        phaseEnd            = false;

        arenaBerserkTimer   = 300000; // 5 min
        berserkTimer        = 300000; // 5 min
        hardModeTimer       = 180000; // 3 min
        arenaYellTimer      = 30000;
        summonWavesTimer    = 10000;

        stormHammerTimer        = 20000;
        deafeningThunderTimer   = 22000;
        chargeOrbTimer          = 15000;

        chainLightningTimer     = 15000;
        lightningChargeTimer    = 50000;
        unbalancingStrikeTimer  = 35000;

        OutroTimer          = 10000;
        Step                = 1;
        isIntro             = true;
        IntroTimer          = 10000;
        IntroStep           = 1;
        isOutro             = false;
        m_uiSifGUID         = 0;
        lIronDwarfes.clear();

        // exploit check
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(GameObject* pLever = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_DOOR_LEVER)))
            pLever->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1); 

        GetCreatureListWithEntryInGrid(lIronDwarfes, m_creature, MOB_IRON_RING_GUARD, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lIronDwarfes, m_creature, MOB_DARK_RUNE_ACOLYTE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lIronDwarfes, m_creature, MOB_IRON_HOHOR_GUARD, DEFAULT_VISIBILITY_INSTANCE);
        if (!lIronDwarfes.empty())
        {
            for(std::list<Creature*>::iterator iter = lIronDwarfes.begin(); iter != lIronDwarfes.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }

        if(m_pInstance) 
        {
            // respawn runic colossus
            if (Creature* pColossus = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RUNIC_COLOSSUS))))
            {
                if (!pColossus->isAlive())
                    pColossus->Respawn();
            }

            // respawn ancient rune giant
            if (Creature* pRuneGiant = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RUNE_GIANT))))
            {
                if (!pRuneGiant->isAlive())
                    pRuneGiant->Respawn();
            }
        }
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_THORIM, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(isHardMode)
            {
                m_pInstance->SetData(TYPE_THORIM_HARD, DONE);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_LOSE_ILLUSION : ACHIEV_LOSE_ILLUSION_H);
            }
            m_pInstance->SetData(TYPE_THORIM, DONE);
        }

        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(m_pInstance) 
        {
            if(isHardMode)
                m_pInstance->SetData(TYPE_THORIM_HARD, DONE);
            m_pInstance->SetData(TYPE_THORIM, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 1.0f)
        {
            uiDamage = 0;
            isOutro = true;
        }
    }

    void StartEncounter()
    {
        m_creature->SetInCombatWithZone();
        if(GameObject* pLever = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_DOOR_LEVER)))
            pLever->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        phase = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!isOutro)
        {
            // phase 0, intro
            if(phase == 0)
            {
                // intro
                if(isIntro)
                {
                    switch(IntroStep)
                    {
                    case 1:
                        DoScriptText(SAY_AGGRO1, m_creature);
                        DoCast(m_creature, SPELL_SHEAT_OF_LIGHTNING);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        ++IntroStep;
                        IntroTimer = 10000;
                        break;
                    case 3:
                        DoScriptText(SAY_AGGRO2, m_creature);
                        if (m_pInstance)
                            m_pInstance->SetData(TYPE_THORIM, IN_PROGRESS);
                        if(Creature* pSif = m_creature->SummonCreature(NPC_SIF, m_creature->GetPositionX() + 10, m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 200000))
                        {
                            pSif->setFaction(35);
                            m_uiSifGUID = pSif->GetGUID();
                        }
                        ++IntroStep;
                        IntroTimer = 9000;
                        break;
                    case 5:
                        if(Creature* pSif = m_pInstance->instance->GetCreature(m_uiSifGUID))
                            DoScriptText(SAY_SIF_INTRO, pSif);
                        phase = 1;
                        isIntro = false;
                        ++IntroStep;
                        IntroTimer = 9000;
                        break;
                    }
                }
                else return;

                if (IntroTimer <= uiDiff)
                {
                    ++IntroStep;
                    IntroTimer = 330000;
                } IntroTimer -= uiDiff;
            }

            // phase 1, fight in arena & hallway
            if(phase == 1)
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // hard mode check
                if (hardModeTimer <= uiDiff && isHardMode)
                {
                    isHardMode = false;
                    if(Creature* Sif = m_pInstance->instance->GetCreature(m_uiSifGUID))
                    {
                        if(Sif && Sif->isAlive())
                        {
                            DoScriptText(SAY_SIF_DESPAWN, Sif);
                            Sif->ForcedDespawn();
                        }
                    }
                    hardModeTimer = 330000;
                } hardModeTimer -= uiDiff;

                // spawn adds in arena
                if(summonWavesTimer < uiDiff)
                {
                    // 1-2 warbringer
                    // 1 evoker
                    // 5-6 commoners
                    // 1 champion
                    // 1 acolyte
                    switch(urand(0, 4))
                    {
                    case 0:
                        i = urand(0, 5);
                        if(Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_CHAMPION, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            if(pTemp->IsWithinLOSInMap(m_creature->getVictim()))
                            {
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                pTemp->AddThreat(m_creature->getVictim(), 100.0f);
                            }
                        }
                        break;
                    case 1:
                        i = urand(0, 5);
                        if(Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_EVOKER, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            if(pTemp->IsWithinLOSInMap(m_creature->getVictim()))
                            {
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                pTemp->AddThreat(m_creature->getVictim(), 100.0f);
                            }
                        }
                        break;
                    case 2:
                        i = urand(5, 6);
                        for(uint8 j = 0; j < i; j++)
                        {
                            if(Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_COMMONER, ArenaLoc[j].x, ArenaLoc[j].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                                if(pTemp->IsWithinLOSInMap(m_creature->getVictim()))
                                {
                                    pTemp->AI()->AttackStart(m_creature->getVictim());
                                    pTemp->AddThreat(m_creature->getVictim(), 100.0f);
                                }
                            }
                        }
                        break;
                    case 3:
                        k = urand(0, 3);
                        i = urand(k + 1, k + 2);
                        for(uint8 j = k; j < i; j++)
                        {
                            if(Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_WARBRINGER, ArenaLoc[j].x, ArenaLoc[j].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                                if(pTemp->IsWithinLOSInMap(m_creature->getVictim()))
                                {
                                    pTemp->AI()->AttackStart(m_creature->getVictim());
                                    pTemp->AddThreat(m_creature->getVictim(), 100.0f);
                                }
                            }
                        }
                        break;
                    case 4:
                        i = urand(0, 5);
                        if(Creature* pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_ACOLYTE, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            if(pTemp->IsWithinLOSInMap(m_creature->getVictim()))
                            {
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                pTemp->AddThreat(m_creature->getVictim(), 100.0f);
                            }
                        }
                        break;
                    }
                    summonWavesTimer = urand (7000, 10000);
                }
                else summonWavesTimer -= uiDiff; 

                // phase 1 spells
                // charge orb
                if(chargeOrbTimer < uiDiff)
                {
                    // missing orbs from DB :(
                    //if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    //DoCast(pTarget, SPELL_CHARGE_ORB);
                    chargeOrbTimer = 10000;
                }
                else chargeOrbTimer -= uiDiff; 

                // storm hammer
                if(stormHammerTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if(pTarget->IsWithinLOSInMap(m_creature))
                        {
                            DoCast(pTarget, SPELL_STORMHAMMER);
                            stormHammerTimer = 15000;
                            deafeningThunderTimer = 3000;
                        }
                    }
                }
                else stormHammerTimer -= uiDiff; 

                // deafening thunder
                if(deafeningThunderTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        if(pTarget->IsWithinLOSInMap(m_creature))
                        {
                            DoCast(pTarget, SPELL_DEAFENING_THUNDER);
                            deafeningThunderTimer = 20000;
                        }
                    }
                }
                else deafeningThunderTimer -= uiDiff; 

                if(arenaYellTimer < uiDiff)
                {
                    switch(urand(0, 2))
                    {
                    case 0: DoScriptText(SAY_SPECIAL1, m_creature); break;
                    case 1: DoScriptText(SAY_SPECIAL2, m_creature); break;
                    case 2: DoScriptText(SAY_SPECIAL3, m_creature); break;
                    }
                    arenaYellTimer = 30000;
                }
                else arenaYellTimer -= uiDiff;

                // phase 1 berserk
                if(arenaBerserkTimer < uiDiff)
                {
                    DoScriptText(SAY_ARENA_WIPE, m_creature);
                    // disabled because is cast on players
                    //DoCast(m_creature, SPELL_BERSERK_ADDS);
                    DoCast(m_creature, SPELL_BERSERK);
                    if(Creature *pOrb = m_creature->SummonCreature(MOB_LIGHTNING_ORB, 2123.389f, -440.639f, 438.247f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        // fix this!
                        pOrb->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pOrb->SetSpeedRate(MOVE_RUN, 4.0f);
                        pOrb->GetMotionMaster()->MovePoint(0, 2166.806f, -441.160f, 438.246f);
                        pOrb->GetMotionMaster()->MovePoint(1, 2227.267f, -431.599f, 412.177f);
                        pOrb->GetMotionMaster()->MovePoint(2, 2228.072f, -266.417f, 412.177f);

                        // dmg
                        Map *map = m_creature->GetMap();
                        if (map->IsDungeon())
                        {
                            Map::PlayerList const &PlayerList = map->GetPlayers();

                            if (PlayerList.isEmpty())
                                return;

                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                                    m_creature->DealDamage(i->getSource(), i->getSource()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, NULL, false);
                            }
                        } 
                    }
                    arenaBerserkTimer = 30000;
                }
                else arenaBerserkTimer -= uiDiff;

                //enter phase 2
                if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 10) && !phaseEnd)
                {
                    if (Creature* pColossus = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RUNIC_COLOSSUS))))
                    {
                        // check colossus for exploit
                        if(!pColossus->isAlive())
                        {
                            if(Creature* pGiant = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_RUNE_GIANT))))
                            {
                                // check giant for exploit
                                if(!pGiant->isAlive())
                                {
                                    // say
                                    DoScriptText(SAY_JUMP, m_creature);

                                    // move in arena
                                    m_creature->GetMotionMaster()->MovePoint(0, 2134.719f, -263.148f, 419.846f);
                                    m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                    m_creature->SetSplineFlags(SPLINEFLAG_FALLING); 
                                    phaseEnd = true;
                                    phase2Timer = 9000;
                                }
                                // disable for debug
                                else
                                    EnterEvadeMode();
                            }
                        }
                        // disable for debug
                        else
                            EnterEvadeMode();
                    }
                }

                if(phase2Timer < uiDiff && phaseEnd)
                {
                    m_creature->RemoveSplineFlag(SPLINEFLAG_FALLING);
                    m_creature->RemoveAurasDueToSpell(SPELL_SHEAT_OF_LIGHTNING);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    if(!isHardMode)
                    {
                        // not working: workaround
                        DoCast(m_creature, SPELL_TOUTCH_OF_DOMINION);
                        m_creature->SetHealth(m_creature->GetHealth() - (m_creature->GetHealth() * 0.25));
                    }
                    if(isHardMode)
                    {
                        if(Creature* Sif = m_pInstance->instance->GetCreature(m_uiSifGUID))
                        {
                            Sif->setFaction(14);
                            DoScriptText(SAY_SIF_EVENT, Sif);
                            Sif->SetInCombatWithZone();
                            m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_SIFFED : ACHIEV_SIFFED_H);
                        }
                    }
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    SetCombatMovement(true);
                    phase = 2;
                    phaseEnd = false;
                }
                else phase2Timer -= uiDiff;
            }

            if(phase == 2)
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // all spells
                // chain lightning
                if(chainLightningTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
                    chainLightningTimer = 10000 + rand()%5000;
                }
                else chainLightningTimer -= uiDiff; 

                // lightning charge
                if(lightningChargeTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_LIGHTNING_CHARGE);
                    lightningChargeTimer = 30000;
                }
                else lightningChargeTimer -= uiDiff; 

                // unbalancing strike
                if(unbalancingStrikeTimer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), SPELL_UNBALANCING_STRIKE);
                    unbalancingStrikeTimer = 30000 + urand(2000, 7000);
                }
                else unbalancingStrikeTimer -= uiDiff; 

                // phase 2 berserk
                if(berserkTimer < uiDiff)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    DoCast(m_creature, SPELL_BERSERK);
                    berserkTimer = 30000;
                }
                else berserkTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        }
        // outro
        if(isOutro)
        {
            switch(Step)
            {
            case 1:
                m_creature->setFaction(35);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++Step;
                OutroTimer = 1000;
                break;
            case 3:
                m_creature->SetOrientation(4.99f);
                DoScriptText(SAY_DEATH, m_creature);
                ++Step;
                OutroTimer = 3000;
                break;
            case 5:
                if(isHardMode)
                {
                    DoScriptText(SAY_OUTRO_HARD1, m_creature);
                    if(Creature* Sif = m_pInstance->instance->GetCreature(m_uiSifGUID))
                        DoCast(Sif, SPELL_STORMHAMMER);
                }
                else
                    DoScriptText(SAY_OUTRO1, m_creature);
                ++Step;
                OutroTimer = 1000;
                break;
            case 7:
                if(isHardMode)
                {
                    if(Creature* Sif = m_pInstance->instance->GetCreature(m_uiSifGUID))
                    {
                        //summon a tentacule
                        if(Creature* tentacule = m_creature->SummonCreature(34266, Sif->GetPositionX(), Sif->GetPositionY(), Sif->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 7000))
                        {
                            tentacule->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            tentacule->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        }
                        Sif->ForcedDespawn();
                    }
                }
                ++Step;
                OutroTimer = 7000;
                break;
            case 9:
                if(isHardMode)
                    DoScriptText(SAY_OUTRO_HARD2, m_creature);
                else
                    DoScriptText(SAY_OUTRO2, m_creature);
                ++Step;
                OutroTimer = 13000;
                break;
            case 11:
                if(isHardMode)
                    DoScriptText(SAY_OUTRO_HARD3, m_creature);
                else
                    DoScriptText(SAY_OUTRO3, m_creature);
                ++Step;
                OutroTimer = 15000;
                break;
            case 13:
                DoOutro();
                ++Step;
                OutroTimer = 10000;
                break;
            }
        }
        else return;

        if (OutroTimer <= uiDiff)
        {
            ++Step;
            OutroTimer = 330000;
        } OutroTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_thorim(Creature* pCreature)
{
    return new boss_thorimAI(pCreature);
}

struct MANGOS_DLL_DECL boss_runic_colossusAI : public ScriptedAI
{
    boss_runic_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 m_uiSpellTimer;
    uint32 m_uiRunicBarrierTimer;

    void Reset()
    {
        m_uiSpellTimer = urand(5000, 10000);
        m_uiRunicBarrierTimer = 15000;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_RUNIC_COLOSSUS, NOT_STARTED);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_RUNIC_COLOSSUS, DONE);
    }

    void UpdateAI(const uint32 uiuiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiuiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                DoCast(m_creature->getVictim(), SPELL_SMASH);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_CHARGE : SPELL_CHARGE_H);
                break;
            }
            m_uiSpellTimer = urand(5000, 10000);
        }else m_uiSpellTimer -= uiuiDiff;

        if (m_uiRunicBarrierTimer < uiuiDiff)
        {
            DoCast(m_creature, SPELL_RUNIC_BARRIER);
            m_uiRunicBarrierTimer = urand(25000, 30000);
        }else m_uiRunicBarrierTimer -= uiuiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_runic_colossus(Creature* pCreature)
{
    return new boss_runic_colossusAI(pCreature);
}

struct MANGOS_DLL_DECL boss_ancient_rune_giantAI : public ScriptedAI
{
    boss_ancient_rune_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiSpellTimer = urand(5000, 10000);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_RUNE_GIANT, NOT_STARTED);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_RUNE_GIANT, DONE);
    }

    void Aggro(Unit *who) 
    {
        // should be cast on adds!!!
        DoCast(m_creature, SPELL_RUNIC_FORTIFICATION);
    }

    void UpdateAI(const uint32 uiuiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiuiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                DoCast(m_creature->getVictim(), m_bIsRegularMode? SPELL_STOMP : SPELL_STOMP_H);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_RUNE_DETONATION);
                break;
            }
            m_uiSpellTimer = urand(5000,10000);
        }else m_uiSpellTimer -= uiuiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ancient_rune_giant(Creature* pCreature)
{
    return new boss_ancient_rune_giantAI(pCreature);
}

struct MANGOS_DLL_DECL npc_sifAI : public ScriptedAI
{
    npc_sifAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiSpellTimer = urand(5000, 10000);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiuiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_THORIM) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiuiDiff)
        {
            switch(urand(0, 2))
            {
            case 0:
                DoCast(m_creature, m_bIsRegularMode? SPELL_FROSTBOLT_VOLLEY : SPELL_FROSTBOLT_VOLLEY_H);
                break;
            case 1:
                DoCast(m_creature, m_bIsRegularMode? SPELL_FROST_NOVA : SPELL_FROST_NOVA_H);
                break;
            case 2:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode? SPELL_BLIZZARD : SPELL_BLIZZARD_H);
                break;
            }

            m_uiSpellTimer = urand(5000, 7000);
        }else m_uiSpellTimer -= uiuiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_sif(Creature* pCreature)
{
    return new npc_sifAI(pCreature);
}

bool GOHello_go_door_lever(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (Creature* pThorim = ((Creature*)Unit::GetUnit((*pGo), m_pInstance->GetData64(NPC_THORIM))))
    {
        if(pThorim->isAlive())
            ((boss_thorimAI*)pThorim->AI())->StartEncounter();
    }

    return false;
}

void AddSC_boss_thorim()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_thorim";
    newscript->GetAI = &GetAI_boss_thorim;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_runic_colossus";
    newscript->GetAI = &GetAI_boss_runic_colossus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ancient_rune_giant";
    newscript->GetAI = &GetAI_boss_ancient_rune_giant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sif";
    newscript->GetAI = &GetAI_npc_sif;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_acolyte";
    newscript->GetAI = &GetAI_mob_dark_rune_acolyte;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_champion";
    newscript->GetAI = &GetAI_mob_dark_rune_champion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_commoner";
    newscript->GetAI = &GetAI_mob_dark_rune_commoner;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_evoker";
    newscript->GetAI = &GetAI_mob_dark_rune_evoker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_warbringer";
    newscript->GetAI = &GetAI_mob_dark_rune_warbringer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_ring_guard";
    newscript->GetAI = &GetAI_mob_dark_rune_ring_guard;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_rune_honor_guard";
    newscript->GetAI = &GetAI_mob_dark_rune_honor_guard;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_door_lever";
    newscript->pGOHello = &GOHello_go_door_lever;
    newscript->RegisterSelf();
}
