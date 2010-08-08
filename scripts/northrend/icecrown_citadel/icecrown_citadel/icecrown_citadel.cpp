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
SDName: ICC teleporter & minibosses
SD%Complete:
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

/*######
## icecrown_teleporters
######*/
enum locations
{
    LIGHTS_HAMMER       = 200,
    ORATORY_OF_DAMMED   = 201,
    RAMPART_OF_SKULLS   = 202,
    DEATHBRINGERS_RISE  = 203,
    UPPER_SPIRE         = 204,
    SINDRAGOSA          = 205,
    FROZEN_THRONE       = 206,
};

// spells
enum teleport
{
    SPELL_TELEPORT_LIGHTS_HAMMER    = 70781,
    SPELL_TELEPORT_ORATORY          = 70856,
    SPELL_TELEPORT_SKULLS_RAMPART   = 70857,
    SPELL_TELEPORT_DEATHBRINGER     = 70858,
    SPELL_TELEPORT_UPPER_SPIRE      = 70859,
    SPELL_TELEPORT_SINDRAGOSAS_LAIR = 70861,
    SPELL_TELEPORT_FROZEN_THRONE    = 70860,
};

bool GoHello_icecrown_citadel_teleporter( Player *pPlayer, GameObject *pGO )
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    // Lights Hammer
    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Light's Hammer", GOSSIP_SENDER_MAIN, LIGHTS_HAMMER);

    // Oratory of the dammed
    if(pInstance->GetData(TYPE_MARROWGAR) == DONE)
    {
        pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Oratory of the Dammed", GOSSIP_SENDER_MAIN, ORATORY_OF_DAMMED);

        // Rampart of Skulls
        if(pInstance->GetData(TYPE_DEATHWHISPER) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Rampart of Skulls", GOSSIP_SENDER_MAIN, RAMPART_OF_SKULLS);

            // Deathbringer's rise
            if(pInstance->GetData(TYPE_GUNSHIP_BATTLE) == DONE)
            {
                pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Deathbringer's Rise", GOSSIP_SENDER_MAIN, DEATHBRINGERS_RISE);

                if(pInstance->GetData(TYPE_SAURFANG) == DONE)
                    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport to the Upper Spire", GOSSIP_SENDER_MAIN, UPPER_SPIRE);
            }
        }
    }

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pGO->GetGUID());

    return true;
}

bool GoSelect_icecrown_citadel_teleporter( Player *pPlayer, GameObject *pGO, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!pPlayer->getAttackers().empty()) return true;

    ScriptedInstance *pInstance = (ScriptedInstance *) pGO->GetInstanceData();
    if(!pInstance) return true;

    switch(action)
    {
    case LIGHTS_HAMMER:
        //pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_LIGHTS_HAMMER, false);
        pPlayer->TeleportTo(631, -16.729f, 2211.418f, 30.115f, 3.123f);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case ORATORY_OF_DAMMED:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_ORATORY, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case RAMPART_OF_SKULLS:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_SKULLS_RAMPART, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case DEATHBRINGERS_RISE:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_DEATHBRINGER, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case UPPER_SPIRE:
        //pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_UPPER_SPIRE, false);
        if(pInstance->GetData(TYPE_FESTERGUT) == DONE || pInstance->GetData(TYPE_ROTFACE) == DONE)
            pPlayer->TeleportTo(631, 4199.149f, 2769.339f, 351.064f, 6.275f);
        else
            pPlayer->TeleportTo(631, 4199.149f, 2769.339f, 351.064f, 6.275f);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case SINDRAGOSA:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_SINDRAGOSAS_LAIR, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    case FROZEN_THRONE:
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_FROZEN_THRONE, false);
        pPlayer->CLOSE_GOSSIP_MENU(); break;
    }
    return true;
}

/*######
## stinky & precious
######*/
enum stinky
{
    SAY_DEATH_STINKY        = -1609471,
    SAY_DEATH_PRECIOUS      = -1609460,

    SPELL_DECIMATE          = 71123,
    SPELL_MORTAL_WOUND      = 71127,
    SPELL_PLAGUE_STENCH     = 71805,
    SPELL_PRECIOUS_RIBBON   = 70404,
};

struct MANGOS_DLL_DECL miniboss_stinkyAI : public ScriptedAI
{
    miniboss_stinkyAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;

    void Reset()
    {
        m_uiMortalWoundTimer    = 3000;
        m_uiDecimateTimer       = 10000;

        DoCast(m_creature, SPELL_PLAGUE_STENCH);
    }

    void JustDied(Unit *killer)
    {
        if (Creature* pFestergut = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_FESTERGUT))))
            DoScriptText(SAY_DEATH_STINKY, pFestergut);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_WOUND);
            m_uiMortalWoundTimer = urand(5000, 10000);
        }
        else
            m_uiMortalWoundTimer -= uiDiff;

        if (m_uiDecimateTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DECIMATE);
            m_uiDecimateTimer = 30000;
        }
        else
            m_uiDecimateTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_miniboss_stinky(Creature* pCreature)
{
    return new miniboss_stinkyAI(pCreature);
}

struct MANGOS_DLL_DECL miniboss_preciousAI : public ScriptedAI
{
    miniboss_preciousAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;

    void Reset()
    {
        m_uiMortalWoundTimer    = 3000;
        m_uiDecimateTimer       = 10000;

        DoCast(m_creature, SPELL_PRECIOUS_RIBBON);
    }

    void JustDied(Unit *killer)
    {
        if (Creature* pRotface = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_ROTFACE))))
            DoScriptText(SAY_DEATH_PRECIOUS, pRotface);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_WOUND);
            m_uiMortalWoundTimer = urand(5000, 10000);
        }
        else
            m_uiMortalWoundTimer -= uiDiff;

        if (m_uiDecimateTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_DECIMATE);
            m_uiDecimateTimer = 30000;
        }
        else
            m_uiDecimateTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_miniboss_precious(Creature* pCreature)
{
    return new miniboss_preciousAI(pCreature);
}



/*######
## trash mobs
######*/
enum
{
    // spire frostwyrm
    SPELL_BERSERK                           = 47008,
    SPELL_FROST_BREATH                      = 70116,
    SPELL_FROST_BREATH_H                    = 72641,
    SPELL_BLIZZARD                          = 70362,
    SPELL_BLIZZARD_H                        = 71118,
    SPELL_CLEAVE                            = 70361,

    // rotting frost giant
    SPELL_STOMP                             = 64652,
    SPELL_DEATH_PLAGUE                      = 72865,
};

struct MANGOS_DLL_DECL mob_spire_frostwyrmAI : public ScriptedAI
{
    mob_spire_frostwyrmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCleaveTimer;
    uint32 m_uiBreathTimer;
    uint32 m_uiBlizzardTimer;
    bool m_bHasEnraged;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_uiCleaveTimer     = 5000;
        m_uiBreathTimer     = 12000;
        m_uiBlizzardTimer   = 15000;
        m_bHasEnraged       = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(20000, 25000);
        }
        else m_uiCleaveTimer -= uiDiff;

        if(m_uiBreathTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FROST_BREATH : SPELL_FROST_BREATH_H);
            m_uiBreathTimer = urand(20000, 25000);
        }
        else m_uiBreathTimer -= uiDiff;

        if(m_uiBlizzardTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, m_bIsRegularMode ? SPELL_BLIZZARD : SPELL_BLIZZARD_H);
            m_uiBlizzardTimer = urand(20000, 25000);
        }
        else m_uiBlizzardTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 2.0f && !m_bHasEnraged)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasEnraged = true;
        }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_spire_frostwyrm(Creature* pCreature)
{
    return new mob_spire_frostwyrmAI(pCreature);
}

struct MANGOS_DLL_DECL mob_rotting_frost_giantAI : public ScriptedAI
{
    mob_rotting_frost_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStompTimer;
    uint32 m_uiDeathPlagueTimer;
    bool m_bHasEnraged;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_uiStompTimer          = 10000;
        m_uiDeathPlagueTimer    = 20000;
        m_bHasEnraged           = false;
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance) 
            m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiStompTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_STOMP);
            m_uiStompTimer = urand(10000, 15000);
        }
        else m_uiStompTimer -= uiDiff;

        if(m_uiDeathPlagueTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_DEATH_PLAGUE);
            m_uiDeathPlagueTimer = urand(20000, 25000);
        }
        else m_uiDeathPlagueTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 2.0f && !m_bHasEnraged)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasEnraged = true;
        }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_rotting_frost_giant(Creature* pCreature)
{
    return new mob_rotting_frost_giantAI(pCreature);
}

/*########################################
##########################################
########## Lower spire trash #############
##########################################
########################################*/

enum
{
	SPELL_SHATTERED_BONES                   = 70961,
	SPELL_BONE_FLURRY                       = 70960,
};

/*######
## The Damned 
######*/
struct MANGOS_DLL_DECL mob_the_damnedAI : public ScriptedAI
{
	mob_the_damnedAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiBoneFlurryTimer;
	uint32 m_uiDieTimer;
	bool m_bMustDie;

	void Reset()
	{
		m_uiBoneFlurryTimer = 10000;
		m_bMustDie = false;
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(uiDamage > m_creature->GetHealth() && !m_bMustDie)
		{
			DoCast(m_creature, SPELL_SHATTERED_BONES);
			m_uiDieTimer = 1000;
			m_bMustDie = true;
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiDieTimer < uiDiff && m_bMustDie)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;

		if (m_uiBoneFlurryTimer < uiDiff)
		{
			// Spell bugged, shouldn't chanell
			DoCast(m_creature, SPELL_BONE_FLURRY);				
			m_uiBoneFlurryTimer = 20000;
		}
		else m_uiBoneFlurryTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_the_damned(Creature* pCreature)
{
	return new mob_the_damnedAI(pCreature);
}

enum
{
	SPELL_SHIELD_BASH							= 70964,
};

/*######
## Ancient Skeletal Soldier
######*/
struct MANGOS_DLL_DECL mob_ancient_skeletal_soldierAI : public ScriptedAI
{
	mob_ancient_skeletal_soldierAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiShield_BashTimer;

	void Reset()
	{
		m_uiShield_BashTimer = urand(7000, 10000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShield_BashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHIELD_BASH);				
			m_uiShield_BashTimer = urand(5000, 7000);
		}
		else m_uiShield_BashTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_ancient_skeletal_soldier(Creature* pCreature)
{
	return new  mob_ancient_skeletal_soldierAI(pCreature);
}

enum
{
	SPELL_GLACIAL_BLAST						= 71029,
};

/*######
## Servant of the Throne
######*/
struct MANGOS_DLL_DECL mob_servant_of_the_throneAI : public ScriptedAI
{
	mob_servant_of_the_throneAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiGlacial_BlastTimer;

	void Reset()
	{
		m_uiGlacial_BlastTimer = urand(3000, 6000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiGlacial_BlastTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_GLACIAL_BLAST);
			m_uiGlacial_BlastTimer = urand(3000, 7000);
		}
		else m_uiGlacial_BlastTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_servant_of_the_throne(Creature* pCreature)
{
	return new mob_servant_of_the_throneAI(pCreature);
}

enum
{
	SPELL_CRYPT_SCARABS				= 70965,
	SPELL_DARK_MENDING				= 71020,
	SPELL_WEB_WRAP					= 70980,
};

/*######
## Nerub'ar Broodkeeper
######*/
struct MANGOS_DLL_DECL mob_nerubar_broodkeeperAI : public ScriptedAI
{
	mob_nerubar_broodkeeperAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiCryptScarabsTimer;
	uint32 m_uiDarkMendingTimer;
	uint32 m_uiWebWrapTimer;
	bool m_bStartAttack;

	void Reset()
	{
		m_uiCryptScarabsTimer	= urand(5000, 7000);
		m_uiDarkMendingTimer	= urand(6000, 13000);
		m_uiWebWrapTimer		= urand(7000, 11000);
		m_bStartAttack			= false;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
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
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
    }

	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && !m_bStartAttack && 
			pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDist3d(pWho->GetPositionX(), pWho->GetPositionY(), pWho->GetPositionZ(), 70) && m_creature->IsWithinLOSInMap(pWho))
		{
			m_bStartAttack = true;
			m_creature->SetInCombatWithZone();
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiCryptScarabsTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_CRYPT_SCARABS);
			m_uiCryptScarabsTimer = urand(5000, 7000);
		}
		else m_uiCryptScarabsTimer -= uiDiff;

		if(m_uiDarkMendingTimer < uiDiff)
		{
			if(Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
				DoCast(pTarget, SPELL_DARK_MENDING);
			m_uiDarkMendingTimer = urand(6000, 13000);
		}
		else m_uiDarkMendingTimer -= uiDiff;

		if(m_uiWebWrapTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_WEB_WRAP);
			m_uiWebWrapTimer = urand(7000, 11000);
		}
		else m_uiWebWrapTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_nerubar_broodkeeper(Creature* pCreature)
{
	return new mob_nerubar_broodkeeperAI(pCreature);
}

enum
{
	SPELL_SABER_LASH					= 71021,
	SPELL_DISRUPTING_SHOUT				= 71022,
	SPELL_SPIRIT_ALARM					= 70547,
	SPELL_SPIRIT_ALARM2					= 70536,
	SPELL_SPIRIT_ALARM3					= 70545,
	SPELL_SPIRIT_ALARM4					= 70546,
};

/*######
## Deathbound Ward
######*/
struct MANGOS_DLL_DECL mob_deathbound_wardAI : public ScriptedAI
{
	mob_deathbound_wardAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiDisrupting_ShoutTimer;
	uint32 m_uiSaber_LashTimer;
	bool m_bStartAttack;

	void Reset()
	{
		m_uiDisrupting_ShoutTimer = 15000;
		m_uiSaber_LashTimer = urand(5000, 7000);
		m_bStartAttack = false;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
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
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }
    }

	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && !m_bStartAttack && (pWho->HasAura(SPELL_SPIRIT_ALARM) || pWho->HasAura(SPELL_SPIRIT_ALARM2) || pWho->HasAura(SPELL_SPIRIT_ALARM3) || pWho->HasAura(SPELL_SPIRIT_ALARM4)) && 
			pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80) && m_creature->IsWithinLOSInMap(pWho))
		{
			m_creature->CallForHelp(60.0f);
			m_bStartAttack = true;
			m_creature->SetInCombatWithZone();
		}
	}

	void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
	{
		if(pSpell->Id == SPELL_SPIRIT_ALARM)
		{
			m_bStartAttack = true;
			m_creature->SetInCombatWithZone();
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiDisrupting_ShoutTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_DISRUPTING_SHOUT);
			m_uiDisrupting_ShoutTimer = urand(10000, 15000);
		}
		else m_uiDisrupting_ShoutTimer -= uiDiff;

		if(m_uiSaber_LashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SABER_LASH);
			m_uiSaber_LashTimer = urand(5000, 10000);
		}
		else m_uiSaber_LashTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathbound_ward(Creature* pCreature)
{
	return new mob_deathbound_wardAI(pCreature);
}

/*########################################
##########################################
############ Oratory trash ###############
##########################################
########################################*/

enum
{
	SPELL_AURA_OF_DARKNESS				= 69491,
	SPELL_DARK_RECKONING				= 69483,
};

/*######
## Deathspeaker High Priest
######*/
struct MANGOS_DLL_DECL mob_deathspeaker_high_priestAI : public ScriptedAI
{
	mob_deathspeaker_high_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiDark_ReckoningTimer;

	void Reset()
	{
		m_uiDark_ReckoningTimer = 15000;
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_AURA_OF_DARKNESS);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiDark_ReckoningTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_DARK_RECKONING);
			m_uiDark_ReckoningTimer = urand(10000, 15000);
		}
		else m_uiDark_ReckoningTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathspeaker_high_priest(Creature* pCreature)
{
	return new mob_deathspeaker_high_priestAI(pCreature);
}

enum
{
	SPELL_SHADOW_BOLT					= 69387,
	SPELL_SHADOW_NOVA_10				= 69355,
	SPELL_SHADOW_NOVA_25				= 71106,
};

/*######
## Deathspeaker Attendant
######*/
struct MANGOS_DLL_DECL mob_deathspeaker_attendantAI : public ScriptedAI
{
	mob_deathspeaker_attendantAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiShadow_BoltTimer;
	uint32 m_uiShadow_NovaTimer;

	void Reset()
	{
		m_uiShadow_BoltTimer = urand(3000, 6000);
		m_uiShadow_NovaTimer = urand(9000, 15000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShadow_BoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SHADOW_BOLT);
			m_uiShadow_BoltTimer = urand(7000, 10000);
		}
		else m_uiShadow_BoltTimer -= uiDiff;

		if(m_uiShadow_NovaTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_SHADOW_NOVA_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_SHADOW_NOVA_25);
			m_uiShadow_NovaTimer = urand(10000, 15000);
		}
		else m_uiShadow_NovaTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathspeaker_attendant(Creature* pCreature)
{
	return new mob_deathspeaker_attendantAI(pCreature);
}

enum
{
	SPELL_DARK_BLESSING			= 69391,
	//SPELL_SHADOW_BOLT			= 69387,
	SPELL_SHADOW_MEND_10		= 69389,
	SPELL_SHADOW_MEND_25		= 71107,
};

/*######
## Deathspeaker Disciple
######*/
struct MANGOS_DLL_DECL mob_deathspeaker_discipleAI : public ScriptedAI
{
	mob_deathspeaker_discipleAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiShadow_BoltTimer;
	uint32 m_uiShadowMendTimer;
	uint32 m_uiDarkBlessingTimer;

	void Reset()
	{
		m_uiShadow_BoltTimer	= urand(3000, 6000);
		m_uiShadowMendTimer		= urand(7000, 11000);
		m_uiDarkBlessingTimer	= urand(5000, 9000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShadow_BoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SHADOW_BOLT);
			m_uiShadow_BoltTimer = urand(7000, 10000);
		}
		else m_uiShadow_BoltTimer -= uiDiff;

		if(m_uiShadowMendTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			if(Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(pTarget, SPELL_SHADOW_MEND_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_SHADOW_MEND_25);
			}
			m_uiShadowMendTimer = urand(10000, 15000);
		}
		else m_uiShadowMendTimer -= uiDiff;

		if(m_uiDarkBlessingTimer < uiDiff)
		{
			if(Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
				DoCast(pTarget, SPELL_DARK_BLESSING);
			m_uiDarkBlessingTimer = urand(5000, 9000);
		}
		else m_uiDarkBlessingTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathspeaker_disciple(Creature* pCreature)
{
	return new mob_deathspeaker_discipleAI(pCreature);
}

enum
{
	SPELL_CHAOS_BOLT_10					= 69576,
	SPELL_CHAOS_BOLT_25					= 71108,
	SPELL_CONSUMING_SHADOWS				= 69405,
	SPELL_CURSE_OF_AGONY_10				= 69404,
	SPELL_CURSE_OF_AGONY_25				= 71112,
};

/*######
## Deathspeaker Servant
######*/
struct MANGOS_DLL_DECL mob_deathspeaker_servantAI : public ScriptedAI
{
	mob_deathspeaker_servantAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiChaos_BoltTimer;
	uint32 m_uiConsuming_ShadowTimer;
	uint32 m_uiCurse_of_AgonyTimer;

	void Reset()
	{
		m_uiChaos_BoltTimer = urand(4000, 7000);
		m_uiConsuming_ShadowTimer = urand(10000, 15000);
		m_uiCurse_of_AgonyTimer = urand(7000, 13000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiConsuming_ShadowTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CONSUMING_SHADOWS);
			m_uiConsuming_ShadowTimer = urand(7000, 10000);
		}
		else m_uiConsuming_ShadowTimer -= uiDiff;

		if (m_uiChaos_BoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_CHAOS_BOLT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_CHAOS_BOLT_25);
			}
			m_uiChaos_BoltTimer = urand(4000, 7000);
		}
		else m_uiChaos_BoltTimer -= uiDiff;

		if(m_uiCurse_of_AgonyTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_CURSE_OF_AGONY_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_CURSE_OF_AGONY_25);
			}
			m_uiCurse_of_AgonyTimer = urand(10000, 17000);
		}
		else m_uiCurse_of_AgonyTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathspeaker_servant(Creature* pCreature)
{
	return new mob_deathspeaker_servantAI(pCreature);
}

enum
{
	SPELL_SHADOW_CLEAVE					= 69492,
};

/*######
## Deathspeaker Zealot
######*/
struct MANGOS_DLL_DECL mob_deathspeaker_zealotAI : public ScriptedAI
{
	mob_deathspeaker_zealotAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiShadow_cleaveTimer;

	void Reset()
	{
		m_uiShadow_cleaveTimer = urand(3000, 7000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShadow_cleaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHADOW_CLEAVE);
			m_uiShadow_cleaveTimer = urand(4000, 7000);
		}
		else m_uiShadow_cleaveTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_deathspeaker_zealot(Creature* pCreature)
{
	return new mob_deathspeaker_zealotAI(pCreature);
}

/*########################################
##########################################
############ Rampart trash ###############
##########################################
########################################*/

enum
{
	SPELL_ABOMINATION_CLEAVE			= 70191,
	SPELL_ENRAGE						= 70371,
};

/*######
## Frenzied Abomination
######*/
struct MANGOS_DLL_DECL mob_frenzied_abominationAI : public ScriptedAI
{
	mob_frenzied_abominationAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiCleaveTimer;
	bool m_bHasEnraged;

	void Reset()
	{
		m_uiCleaveTimer = urand(3000, 7000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiCleaveTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_ABOMINATION_CLEAVE);
			m_uiCleaveTimer = urand(3000, 7000);
		}
		else m_uiCleaveTimer -= uiDiff;

		if(m_creature->GetHealthPercent() <= 10.0f && !m_bHasEnraged)
		{
			DoCast(m_creature, SPELL_ENRAGE);
			m_bHasEnraged = true;
		}	

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_frenzied_abomination(Creature* pCreature)
{
	return new mob_frenzied_abominationAI(pCreature);
}

enum
{
	SPELL_POSION_SPIT					= 70189,
};

/*######
## Spire Gargoyle
######*/
struct MANGOS_DLL_DECL mob_spire_gargoyleAI : public ScriptedAI
{
	mob_spire_gargoyleAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiPoison_SpitTimer;

	void Reset()
	{
		m_uiPoison_SpitTimer = urand(3000, 5000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiPoison_SpitTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_POSION_SPIT);
			m_uiPoison_SpitTimer = urand(3000, 5000);
		}
		else m_uiPoison_SpitTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_spire_gargoyle(Creature* pCreature)
{
	return new mob_spire_gargoyleAI(pCreature);
}

enum
{
	SPELL_GHOUL_SLASH					= 70396,
	SPELL_CANNIBALIZE					= 70363,
};

/*######
## Spire Minion
######*/
struct MANGOS_DLL_DECL mob_spire_minionAI : public ScriptedAI
{
	mob_spire_minionAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiGhoul_SlashTimer;
	uint32 m_uiCannibalizeTimer;

	void Reset()
	{
		m_uiGhoul_SlashTimer = 7000;
		m_uiCannibalizeTimer = 30000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiGhoul_SlashTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_GHOUL_SLASH);
			m_uiGhoul_SlashTimer = urand(3000, 7000);
		}
		else m_uiGhoul_SlashTimer -= uiDiff;

		if (m_uiCannibalizeTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_CANNIBALIZE);	
			m_uiCannibalizeTimer = urand(20000, 25000);
		}
		else m_uiCannibalizeTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_spire_minion(Creature* pCreature)
{
	return new mob_spire_minionAI(pCreature);
}

/*########################################
##########################################
########### Rampart soldiers #############
##########################################
########################################*/

enum death_knight
{
	SPELL_ICY_TOUCH						= 69916,
	SPELL_PLAGUE_STRIKE					= 69912,
};

/*######
## Gunship Dreadblade / Reaver -- death knight
######*/
struct MANGOS_DLL_DECL mob_gunship_dreadbladeAI : public ScriptedAI
{
	mob_gunship_dreadbladeAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiIcy_TouchTimer;
	uint32 m_uiPlague_StrykeTimer;

	void Reset()
	{
		m_uiIcy_TouchTimer = 7000;
		m_uiPlague_StrykeTimer = 8000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiIcy_TouchTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_ICY_TOUCH);
			m_uiIcy_TouchTimer = urand(7000, 10000);
		}
		else m_uiIcy_TouchTimer -= uiDiff;

		if (m_uiPlague_StrykeTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_PLAGUE_STRIKE);
			m_uiPlague_StrykeTimer = urand(4000, 7000);
		}
		else m_uiPlague_StrykeTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_gunship_dreadblade(Creature* pCreature)
{
	return new mob_gunship_dreadbladeAI(pCreature);
}

enum hunter
{
	SPELL_ARCANE_SHOT_10				= 69989,
	SPELL_ARCANE_SHOT_25				= 71116,
	SPELL_EXPLOSIVE_SHOT_10				= 69975,
	SPELL_EXPLOSIVE_SHOT_25				= 71126,
	SPELL_SHOOT_10						= 69974,
	SPELL_SHOOT_25						= 71144,
};

/*######
## Skybreaker Marksman / Sniper -- hunter
######*/
struct MANGOS_DLL_DECL mob_gunship_marksmanAI : public ScriptedAI
{
	mob_gunship_marksmanAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiArcane_ShotTimer;
	uint32 m_uiExplosive_ShotTimer;
	uint32 m_uiShotTimer;

	void Reset()
	{
		m_uiShotTimer = urand(3000, 7000);
		m_uiArcane_ShotTimer = urand(5000, 8000);
		m_uiExplosive_ShotTimer = urand(9000, 12000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShotTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_SHOOT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_SHOOT_25);
			}
			m_uiShotTimer = urand(4000, 7000);
		}
		else m_uiShotTimer -= uiDiff;

		if (m_uiArcane_ShotTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_ARCANE_SHOT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_ARCANE_SHOT_25);
			}
			m_uiArcane_ShotTimer = urand(5000, 9000);
		}
		else m_uiArcane_ShotTimer -= uiDiff;

		if (m_uiExplosive_ShotTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_EXPLOSIVE_SHOT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_EXPLOSIVE_SHOT_25);
			}
			m_uiExplosive_ShotTimer = urand(9000, 12000);
		}
		else m_uiExplosive_ShotTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_marksman(Creature* pCreature)
{
	return new mob_gunship_marksmanAI(pCreature);
}

enum warrior
{
	SPELL_DEVASTATE						= 69902,
	SPELL_SHIELD_SLAM_10				= 69903,
	SPELL_SHIELD_SLAM_25				= 72645,
	SPELL_THUNDERCLAP_10				= 69965,
	SPELL_THUNDERCLAP_25				= 71147,
	SPELL_SPELL_REFLECT					= 69901,
};

/*######
## Gunship Protector / Defender -- prot warr
######*/
struct MANGOS_DLL_DECL mob_gunship_protectorAI : public ScriptedAI
{
	mob_gunship_protectorAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiDevastateTimer;
	uint32 m_uiSpell_ReflectTimer;
	uint32 m_uiShield_SlamTimer;
	uint32 m_uiThunderclapTimer;

	void Reset()
	{
		m_uiDevastateTimer = urand(3000, 7000);
		m_uiSpell_ReflectTimer = 15000;
		m_uiShield_SlamTimer = 10000;
		m_uiThunderclapTimer = 16000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShield_SlamTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_SHIELD_SLAM_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_SHIELD_SLAM_25);
			m_uiShield_SlamTimer = urand(10000, 15000);
		}
		else m_uiShield_SlamTimer -= uiDiff;

		if (m_uiThunderclapTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_THUNDERCLAP_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_THUNDERCLAP_25);
			m_uiThunderclapTimer = urand(16000, 18000);														
		}
		else m_uiThunderclapTimer -= uiDiff;

		if (m_uiDevastateTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				DoCast(pTarget, SPELL_DEVASTATE);
			m_uiDevastateTimer = urand(3000, 7000);													
		}
		else m_uiDevastateTimer -= uiDiff;

		if (m_uiSpell_ReflectTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SPELL_REFLECT);
			m_uiSpell_ReflectTimer = urand(15000, 20000);													
		}
		else m_uiSpell_ReflectTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_protector(Creature* pCreature)
{
	return new mob_gunship_protectorAI(pCreature);
}

enum shaman
{
	SPELL_CHAIN_HEAL_10			= 69923,
	SPELL_CHAIN_HEAL_25			= 71120,
	SPELL_EARTH_SHIELD			= 69926,
	SPELL_HEALING_WAVE_10		= 69958,
	SPELL_HEALING_WAVE_25		= 71133,
	SPELL_LIGHTNING_BOLT_10		= 69970,
	SPELL_LIGHTNING_BOLT_25		= 71136,
};
/*######
## Gunship Luminary / Oracle -- resto shaman
######*/
struct MANGOS_DLL_DECL mob_gunship_luminaryAI : public ScriptedAI
{
	mob_gunship_luminaryAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiLightningBoltTimer;
	uint32 m_uiHealingWaveTimer;
	uint32 m_uiChainHealTimer;
	uint32 m_uiEarthShieldTimer;

	void Reset()
	{
		m_uiLightningBoltTimer = urand(4000, 7000);
		m_uiHealingWaveTimer = urand(6000, 10000);
		m_uiChainHealTimer = urand(7000, 12000);
		m_uiEarthShieldTimer = urand(10000, 15000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiLightningBoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_LIGHTNING_BOLT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_LIGHTNING_BOLT_25);
			}
			m_uiLightningBoltTimer = urand(4000, 7000);
		}
		else m_uiLightningBoltTimer -= uiDiff;

		if (m_uiHealingWaveTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_HEALING_WAVE_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_HEALING_WAVE_25);
			}
			m_uiHealingWaveTimer = urand(6000, 12000);														
		}
		else m_uiHealingWaveTimer -= uiDiff;

		if (m_uiChainHealTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_CHAIN_HEAL_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_CHAIN_HEAL_25);
			}
			m_uiChainHealTimer = urand(8000, 13000);													
		}
		else m_uiChainHealTimer -= uiDiff;

		if (m_uiEarthShieldTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
				DoCast(pTarget, SPELL_EARTH_SHIELD);
			m_uiEarthShieldTimer = urand(10000, 17000);													
		}
		else m_uiEarthShieldTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_luminary(Creature* pCreature)
{
	return new mob_gunship_luminaryAI(pCreature);
}

enum mage
{
	SPELL_BLINK					= 69904,
	SPELL_FROSTFIRE_BOLT_10		= 69869,
	SPELL_FROSTFIRE_BOLT_25		= 71130, 
	SPELL_SKYBREAKER_FLAG		= 69810,	// aly
	SPELL_KORKRON_FLAG			= 69811,	// horde
	SPELL_SKYBREAKER_FLAG_AURA	= 69808,
	SPELL_KORKRON_FLAG_AURA		= 69809,
	NPC_ALY_FLAG				= 37041,
	NPC_HORDE_FLAG				= 37044,
};

/*######
## Gunship Sorcerer / Invoker -- mage
######*/
struct MANGOS_DLL_DECL mob_gunship_sorcererAI : public ScriptedAI
{
	mob_gunship_sorcererAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiBlinkTimer;
	uint32 m_uiFrostfireBoltTimer;
	uint32 m_uiFlagTimer;

	void Reset()
	{
		m_uiBlinkTimer = urand(10000, 15000);
		m_uiFlagTimer = 10000;
		m_uiFrostfireBoltTimer = urand(3000, 7000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void JustSummoned(Creature* pSummoned)
	{
		switch(pSummoned->GetEntry())
		{
		case NPC_ALY_FLAG:
			pSummoned->CastSpell(pSummoned, SPELL_SKYBREAKER_FLAG_AURA, false);
			break;
		case NPC_HORDE_FLAG:
			pSummoned->CastSpell(pSummoned, SPELL_KORKRON_FLAG_AURA, false);
			break;
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiFrostfireBoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_FROSTFIRE_BOLT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_FROSTFIRE_BOLT_25);
			}
			m_uiFrostfireBoltTimer = urand(4000,7000);														
		}
		else m_uiFrostfireBoltTimer -= uiDiff;

		if (m_uiBlinkTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_BLINK);
			m_uiBlinkTimer = urand(15000, 20000);													
		}
		else m_uiBlinkTimer -= uiDiff;

		if (m_uiFlagTimer < uiDiff)
		{
			switch(m_creature->GetEntry())
			{
			case 37026:
				DoCast(m_creature, SPELL_SKYBREAKER_FLAG);
				break;
			case 37033:
				DoCast(m_creature, SPELL_KORKRON_FLAG);
				break;
			}
			m_uiFlagTimer = 30000;													
		}
		else m_uiFlagTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_sorcerer(Creature* pCreature)
{
	return new mob_gunship_sorcererAI(pCreature);
}

enum druid
{
	SPELL_HEALING_TOUCH_10		= 69899,
	SPELL_HEALING_TOUCH_25		= 71121,
	SPELL_REGROWTH_10			= 69882,
	SPELL_REGROWTH_25			= 71141,
	SPELL_REJUVENATION_10		= 69898,
	SPELL_REJUVENATION_25		= 71142,
	SPELL_WRATH_10				= 69968,
	SPELL_WRATH_25				= 71148,
};

/*######
## Gunship Hierophant / Primalist -- resto druid
######*/
struct MANGOS_DLL_DECL mob_gunship_primalistAI : public ScriptedAI
{
	mob_gunship_primalistAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiWrathTimer;
	uint32 m_uiRejuvenationTimer;
	uint32 m_uiRegrowthTimer;
	uint32 m_uiHealingTouchTimer;

	void Reset()
	{
		m_uiWrathTimer = urand(4000, 8000);
		m_uiRejuvenationTimer = urand(6000, 10000);
		m_uiRegrowthTimer = urand(12000, 15000);
		m_uiHealingTouchTimer = urand(5000, 10000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiWrathTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_WRATH_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_WRATH_25);
			}
			m_uiWrathTimer = urand(4000, 8000);
		}
		else m_uiWrathTimer -= uiDiff;

		if (m_uiRejuvenationTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_REJUVENATION_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_REJUVENATION_25);
			}
			m_uiRejuvenationTimer = urand(6000, 10000);													
		}
		else m_uiRejuvenationTimer -= uiDiff;

		if (m_uiRegrowthTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_REGROWTH_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_REGROWTH_25);
			}
			m_uiRegrowthTimer = urand(12000, 15000);												
		}
		else m_uiRegrowthTimer -= uiDiff;

		if (m_uiHealingTouchTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_HEALING_TOUCH_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_HEALING_TOUCH_25);
			}
			m_uiHealingTouchTimer = urand(5000, 10000);													
		}
		else m_uiHealingTouchTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_primalist(Creature* pCreature)
{
	return new mob_gunship_primalistAI(pCreature);
}

enum priest
{
	SPELL_GREATER_HEAL_10		= 69963,
	SPELL_GREATER_HEAL_25		= 71131,
	SPELL_PAIN_SUPPRESSION		= 69910,
	SPELL_SMITE_10				= 69967,
	SPELL_SMITE_25				= 71146,
};
/*######
## Gunship Vicar / Templar -- holy priest
######*/
struct MANGOS_DLL_DECL mob_gunship_templarAI : public ScriptedAI
{
	mob_gunship_templarAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiGreatHealTimer;
	uint32 m_uiPainSuppressionTimer;
	uint32 m_uiSmiteTimer;

	void Reset()
	{
		m_uiSmiteTimer = urand(4000, 7000);
		m_uiPainSuppressionTimer = 15000;
		m_uiGreatHealTimer = urand(9000, 14000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiSmiteTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_SMITE_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_SMITE_25);
			}
			m_uiSmiteTimer = urand(4000, 7000);
		}
		else m_uiSmiteTimer -= uiDiff;

		if (m_uiGreatHealTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_GREATER_HEAL_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_GREATER_HEAL_25);
			}
			m_uiGreatHealTimer = urand(9000, 14000);														
		}
		else m_uiGreatHealTimer -= uiDiff;

		if (m_uiPainSuppressionTimer < uiDiff)
		{
			if (Unit* pTarget = DoSelectLowestHpFriendly(50.0f))
				DoCast(pTarget, SPELL_PAIN_SUPPRESSION);
			m_uiPainSuppressionTimer = 15000;												
		}
		else m_uiPainSuppressionTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_templar(Creature* pCreature)
{
	return new mob_gunship_templarAI(pCreature);
}

enum warlock
{
	SPELL_CURSE_DOOM_10			= 69969,
	SPELL_CURSE_DOOM_25			= 71124,
	SPELL_INCINERATE_10			= 69973,
	SPELL_INCINERATE_25			= 71135,
	SPELL_SHADOW_BOLT_10		= 69972,
	SPELL_SHADOW_BOLT_25		= 71143,
};

/*######
## Gunship Summoner / Necrolyte -- warlock
######*/
struct MANGOS_DLL_DECL mob_gunship_necrolyteAI : public ScriptedAI
{
	mob_gunship_necrolyteAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiShadowBoltTimer;
	uint32 m_uiIncinerateTimer;
	uint32 m_uiCurseDoomTimer;

	void Reset()
	{
		m_uiShadowBoltTimer = urand(3000, 6000);
		m_uiIncinerateTimer = urand(7000, 13000);
		m_uiCurseDoomTimer = urand(8000, 15000);
	}

	void AttackStart(Unit* pWho)
	{
		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho, 20.0f);
		}
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiShadowBoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_BOLT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_BOLT_25);
			}
			m_uiShadowBoltTimer = urand(3000, 6000);
		}
		else m_uiShadowBoltTimer -= uiDiff;

		if (m_uiIncinerateTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_INCINERATE_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_INCINERATE_25);
			}
			m_uiIncinerateTimer = urand(7000, 13000);													
		}
		else m_uiIncinerateTimer -= uiDiff;

		if (m_uiCurseDoomTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_CURSE_DOOM_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_CURSE_DOOM_25);
			}
			m_uiCurseDoomTimer = urand(8000, 15000);													
		}
		else m_uiCurseDoomTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_necrolyte(Creature* pCreature)
{
	return new mob_gunship_necrolyteAI(pCreature);
}

enum paladin
{
	SPELL_AVENGERS_SHIELD_10		= 69927,
	SPELL_AVENGERS_SHIELD_25		= 71117,
	SPELL_CONSECRATION_10			= 69930,
	SPELL_CONSECRATION_25			= 71122,
	SPELL_HOLY_WRATH_10				= 69934,
	SPELL_HOLY_WRATH_25				= 71134,
};

/*######
## Gunship Vindicator / Vanquisher -- retry pala
######*/
struct MANGOS_DLL_DECL mob_gunship_vindicatorAI : public ScriptedAI
{
	mob_gunship_vindicatorAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiConsecrationTimer;
	uint32 m_uiAvengersShieldTimer;
	uint32 m_uiHolyWrathTimer;

	void Reset()
	{
		m_uiConsecrationTimer = urand(9000, 13000);
		m_uiAvengersShieldTimer = urand(5000, 9000);
		m_uiHolyWrathTimer	= urand(7000, 14000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiAvengersShieldTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_AVENGERS_SHIELD_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_AVENGERS_SHIELD_25);
			}
			m_uiAvengersShieldTimer = urand(5000, 10000);
		}
		else m_uiAvengersShieldTimer -= uiDiff;

		if (m_uiConsecrationTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_CONSECRATION_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_CONSECRATION_25);
			m_uiConsecrationTimer = urand(14000, 18000);														
		}
		else m_uiConsecrationTimer -= uiDiff;

		if (m_uiHolyWrathTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_HOLY_WRATH_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_HOLY_WRATH_25);
			}
			m_uiHolyWrathTimer = urand(8000, 12000);													
		}
		else m_uiHolyWrathTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_vindicator(Creature* pCreature)
{
	return new mob_gunship_vindicatorAI(pCreature);
}

enum rogue
{
	SPELL_FAN_KNIVES_10			= 69921,
	SPELL_FAN_KNIVES_25			= 71128,
	SPELL_SINISTER_STRIKE_10	= 69920,
	SPELL_SINISTER_STRIKE_25	= 71145,
};

/*######
## Gunship Assasin / Stalker -- rogue
######*/
struct MANGOS_DLL_DECL mob_gunship_assasinAI : public ScriptedAI
{
	mob_gunship_assasinAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiFanKnivesTimer;
	uint32 m_uiSinisterStrikeTimer;

	void Reset()
	{
		m_uiFanKnivesTimer = urand(7000, 15000);
		m_uiSinisterStrikeTimer = urand(4000, 8000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiSinisterStrikeTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_SINISTER_STRIKE_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_SINISTER_STRIKE_25);
			m_uiSinisterStrikeTimer = urand(4000, 8000);
		}
		else m_uiSinisterStrikeTimer -= uiDiff;

		if (m_uiFanKnivesTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_FAN_KNIVES_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_FAN_KNIVES_25);
			m_uiFanKnivesTimer = urand(7000, 15000);														
		}
		else m_uiFanKnivesTimer -= uiDiff;

		DoMeleeAttackIfReady();																				
	}
};

CreatureAI* GetAI_mob_gunship_assasin(Creature* pCreature)
{
	return new mob_gunship_assasinAI(pCreature);
}

/*########################################
##########################################
########### Plagueworks trash ############
##########################################
########################################*/

enum
{
	SPELL_ABOM_CLEAVE			= 40504,
	SPELL_PLAGUE_CLOUD			= 71150,
	SPELL_SCOURGE_HOOK			= 71140,
};

/*######
## Blighted Abomination
######*/
struct MANGOS_DLL_DECL mob_blighted_abominationAI : public ScriptedAI
{
    mob_blighted_abominationAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiCleaveTimer;
    uint32 m_uiPlagueCloudTimer;
    uint32 m_uiScourgeHookTimer;

    void Reset()
    {
        m_uiCleaveTimer			= urand(2000, 5000);
		m_uiPlagueCloudTimer	= urand(7000, 15000);
		m_uiScourgeHookTimer	= urand(5000, 10000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiScourgeHookTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_SCOURGE_HOOK);
            m_uiScourgeHookTimer = urand(10000, 17000);
        }
        else m_uiScourgeHookTimer -= uiDiff;

        if (m_uiPlagueCloudTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_PLAGUE_CLOUD);
            m_uiPlagueCloudTimer = urand(20000, 30000);
        }
        else m_uiPlagueCloudTimer -= uiDiff;

        if (m_uiCleaveTimer	 < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ABOM_CLEAVE);
            m_uiCleaveTimer	 = urand(3000, 6000);
        }
        else m_uiCleaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blighted_abomination(Creature* pCreature)
{
    return new mob_blighted_abominationAI(pCreature);
}

enum
{
    SPELL_COMBOBULATING_SPRAY	= 71103,
    SPELL_PLAGUE_BLAST			= 73079,
    SPELL_PLAGUE_STREAM			= 69871,
};

/*######
## Plague Scientist
######*/
struct MANGOS_DLL_DECL mob_plague_scientistAI : public ScriptedAI
{
    mob_plague_scientistAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiCombobulatingSprayTimer;
    uint32 m_uiPlagueBlastTimer;
    uint32 m_uiPlagueStreamTimer;

    void Reset()
    {
        m_uiCombobulatingSprayTimer = 10000;
        m_uiPlagueBlastTimer        = urand(8000, 11000);
        m_uiPlagueStreamTimer       = urand(12000, 15000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCombobulatingSprayTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_COMBOBULATING_SPRAY);
            m_uiCombobulatingSprayTimer = 10000;
        }
        else m_uiCombobulatingSprayTimer -= uiDiff;

        if (m_uiPlagueBlastTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget,SPELL_PLAGUE_BLAST );
            m_uiPlagueBlastTimer = urand(8000, 11000);
        }
        else m_uiPlagueBlastTimer -= uiDiff;

        if (m_uiPlagueStreamTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_PLAGUE_STREAM );
            m_uiPlagueStreamTimer = urand(12000, 15000);
        }
        else m_uiPlagueStreamTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_plague_scientist(Creature* pCreature)
{
    return new mob_plague_scientistAI(pCreature);
}

enum
{
	SPELL_LEAPING_FACE_MAUL			= 71164,
};
/*######
## Vengefull fleshreaper
######*/
struct MANGOS_DLL_DECL mob_vengefull_fleshreaperAI : public ScriptedAI
{
    mob_vengefull_fleshreaperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiFacemaulTimer;

    void Reset()
    {
        m_uiFacemaulTimer = 10000;
    }

	void Aggro(Unit* pWho)
	{
		DoCast(pWho, SPELL_LEAPING_FACE_MAUL);
	}

	// not sure if I must use this
	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && (pWho->HasAura(SPELL_SPIRIT_ALARM) || pWho->HasAura(SPELL_SPIRIT_ALARM2) || pWho->HasAura(SPELL_SPIRIT_ALARM3) || pWho->HasAura(SPELL_SPIRIT_ALARM4)) && 
			pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 80) && m_creature->IsWithinLOSInMap(pWho))
		{
			m_creature->CallForHelp(60.0f);
			m_creature->SetInCombatWithZone();
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFacemaulTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_LEAPING_FACE_MAUL);
            m_uiFacemaulTimer = 10000;
        }
        else m_uiFacemaulTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vengefull_fleshreaper(Creature* pCreature)
{
    return new mob_vengefull_fleshreaperAI(pCreature);
}


enum
{
	SPELL_MASSIVE_STOMP_10			= 71114,
	SPELL_MASSIVE_STOMP_25			= 71115,
};
/*######
## Decaying colossus
######*/
struct MANGOS_DLL_DECL mob_decaying_colossusAI : public ScriptedAI
{
    mob_decaying_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiStompTimer;

    void Reset()
    {
        m_uiStompTimer = urand(5000, 9000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStompTimer < uiDiff)
        {
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_MASSIVE_STOMP_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_MASSIVE_STOMP_25);
            m_uiStompTimer = urand(10000, 20000);
        }
        else m_uiStompTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_decaying_colossus(Creature* pCreature)
{
    return new mob_decaying_colossusAI(pCreature);
}

/*########################################
##########################################
########## Crimson halls trash ###########
##########################################
########################################*/

enum
{
	SPELL_SEVERED_ESSENCE_10		= 71906,
	SPELL_SEVERED_ESSENCE_25		= 71942,
};

/*######
## Val'kyr Herald
######*/
struct MANGOS_DLL_DECL mob_valkyr_heraldAI : public ScriptedAI
{
	mob_valkyr_heraldAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiSeveredEssenceTimer;

	void Reset()
	{
		m_uiSeveredEssenceTimer = urand(5000, 9000);
	}

	void MoveInLineOfSight(Unit* pWho)
	{
		if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && pWho->GetTypeId() == TYPEID_PLAYER && 
			m_creature->IsWithinDist3d(pWho->GetPositionX(), pWho->GetPositionY(), pWho->GetPositionZ(), 100) && m_creature->IsWithinLOSInMap(pWho))
			m_creature->SetInCombatWithZone();
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiSeveredEssenceTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_SEVERED_ESSENCE_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_SEVERED_ESSENCE_25);
			m_uiSeveredEssenceTimer = urand(5000, 9000);
		}
		else m_uiSeveredEssenceTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_valkyr_herald(Creature* pCreature)
{
	return new mob_valkyr_heraldAI(pCreature);
}
enum
{
	SPELL_AMPLIFY_MAGIC_10		    = 70408,
	SPELL_AMPLIFY_MAGIC_25	   	    = 72336,
	SPELL_BLAST_WAVE_10		        = 70407,
	SPELL_BLAST_WAVE_25	   			= 71151,
	SPELL_FIREBALL_10				= 70409,
	SPELL_FIREBALL_25	   			= 71153,
	SPELL_POLYMORPH_SPIDER		    = 70410,
	SPELL_SIPHON_ESSENCE	   	    = 70299,
};

/*######
## Darkfallen Archmage
######*/
struct MANGOS_DLL_DECL mob_darkfallen_archmageAI : public ScriptedAI
{
	mob_darkfallen_archmageAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiAmplifyMagicTimer;
	uint32 m_uiBlastWaveTimer;
	uint32 m_uiFireballTimer;
	uint32 m_uiPolymorphSpiderTimer;
	uint32 m_uiSiphonEssenceTimer;

	void Reset()
	{
		m_uiAmplifyMagicTimer     = urand(5000, 9000);
		m_uiBlastWaveTimer        = urand(7000, 11000);
		m_uiFireballTimer         = urand(9000, 13000);
		m_uiPolymorphSpiderTimer  = 10000;
		m_uiSiphonEssenceTimer    = 15000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiAmplifyMagicTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_AMPLIFY_MAGIC_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_AMPLIFY_MAGIC_25);
			m_uiAmplifyMagicTimer = urand(5000, 9000);
		}
		else m_uiAmplifyMagicTimer -= uiDiff;

		if (m_uiBlastWaveTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_BLAST_WAVE_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_BLAST_WAVE_25);
			m_uiBlastWaveTimer = urand(7000, 10000);
		}
		else m_uiBlastWaveTimer -= uiDiff;

		if (m_uiFireballTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_FIREBALL_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_FIREBALL_25);
			}
			m_uiFireballTimer = urand(9000, 13000);
		}
		else m_uiFireballTimer -= uiDiff;

		if (m_uiPolymorphSpiderTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_POLYMORPH_SPIDER);
			m_uiPolymorphSpiderTimer = 10000;
		}
		else m_uiPolymorphSpiderTimer -= uiDiff;

		if (m_uiSiphonEssenceTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCastSpellIfCan(pTarget, SPELL_SIPHON_ESSENCE);
			m_uiSiphonEssenceTimer = 15000;
		}
		else m_uiSiphonEssenceTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_archmage(Creature* pCreature)
{
	return new mob_darkfallen_archmageAI(pCreature);	
}

enum
{
	SPELL_BLOOD_MIRROR   		    = 70450,
	SPELL_UNHOLY_STRIKE		        = 70437,
	SPELL_VAMPIRIC_AURA	   			= 71736,
};

/*######
## Darkfallen Blood Knight
######*/
struct MANGOS_DLL_DECL mob_darkfallen_blood_knightAI : public ScriptedAI
{
	mob_darkfallen_blood_knightAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiBloodMirrorTimer;
	uint32 m_uiSiphonEssenceTimer;
	uint32 m_uiUnholyStrikeTimer;

	void Reset()
	{
		m_uiBloodMirrorTimer      = urand(5000, 9000);
		m_uiSiphonEssenceTimer    = urand(7000, 11000);
		m_uiUnholyStrikeTimer     = urand(9000, 13000);
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_VAMPIRIC_AURA);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiBloodMirrorTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_BLOOD_MIRROR);
			m_uiBloodMirrorTimer = urand(5000, 9000);
		}
		else m_uiBloodMirrorTimer -= uiDiff;

		if (m_uiSiphonEssenceTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SIPHON_ESSENCE);
			m_uiSiphonEssenceTimer = urand(7000, 10000);
		}
		else m_uiSiphonEssenceTimer -= uiDiff;

		if (m_uiUnholyStrikeTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_UNHOLY_STRIKE);
			m_uiUnholyStrikeTimer = urand(9000, 13000);
		}
		else m_uiUnholyStrikeTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_blood_knight(Creature* pCreature)
{
	return new mob_darkfallen_blood_knightAI(pCreature);	
}

enum
{
	SPELL_CHAINS_OF_SHADOW	   	= 70645,
	SPELL_SHADOW_B_10	        = 72960,
	SPELL_SHADOW_B_25 			= 72961,
};

/*######
## Darkfallen Noble
######*/
struct MANGOS_DLL_DECL mob_darkfallen_nobleAI : public ScriptedAI
{
	mob_darkfallen_nobleAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiChainsOfShadowTimer;
	uint32 m_uiShadowBoltTimer;
	uint32 m_uiSiphonEssenceTimer;

	void Reset()
	{
		m_uiChainsOfShadowTimer   = urand(5000, 9000);
		m_uiShadowBoltTimer       = urand(7000, 11000);
		m_uiSiphonEssenceTimer    = urand(9000, 13000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiChainsOfShadowTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CHAINS_OF_SHADOW);
			m_uiChainsOfShadowTimer = urand(5000, 9000);
		}
		else m_uiChainsOfShadowTimer -= uiDiff;

		if (m_uiShadowBoltTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_B_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_B_25);
			}
			m_uiShadowBoltTimer = urand(7000, 10000);
		}
		else m_uiShadowBoltTimer -= uiDiff;

		if (m_uiSiphonEssenceTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SIPHON_ESSENCE);
			m_uiSiphonEssenceTimer = urand(9000, 13000);
		}
		else m_uiSiphonEssenceTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_noble(Creature* pCreature)
{
	return new mob_darkfallen_nobleAI(pCreature);	
}
enum
{
	SPELL_LICH_SLAP_10  	        = 72057,
	SPELL_LICH_SLAP_25 	     		= 72421,
	SPELL_SHROUD_OF_PROTECTION	    = 72065,
	SPELL_SHROUD_OF_SPELL_WARDING   = 72066,
};

/*######
## Darkfallen Advisor
######*/
struct MANGOS_DLL_DECL mob_darkfallen_advisorAI : public ScriptedAI
{
	mob_darkfallen_advisorAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiLichSlapTimer;
	bool m_bHasProtection;
	uint32 m_uiShroudOfSpellWardingTimer;

	void Reset()
	{
		m_uiLichSlapTimer                = urand(5000, 9000);
		m_bHasProtection				 = false;
		m_uiShroudOfSpellWardingTimer    = urand(9000, 13000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (!m_bHasProtection && m_creature->GetHealthPercent() < 10.0f)
		{
			m_bHasProtection = true;
			DoCast(m_creature, SPELL_SHROUD_OF_PROTECTION);
		}

		if (m_uiLichSlapTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_LICH_SLAP_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_LICH_SLAP_25);
			}
			m_uiLichSlapTimer = urand(5000, 9000);
		}
		else m_uiLichSlapTimer -= uiDiff;

		if (m_uiShroudOfSpellWardingTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SHROUD_OF_SPELL_WARDING);
			m_uiShroudOfSpellWardingTimer = 30000;
		}
		else m_uiShroudOfSpellWardingTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_advisor(Creature* pCreature)
{
	return new mob_darkfallen_advisorAI(pCreature);	
}

enum
{
	SPELL_REND_FLASH_10  	        = 70435,
	SPELL_REND_FLASH_25      		= 71154,
	SPELL_VAMPIRIC_CURSE    	    = 70423,
};

/*######
## Darkfallen Lieutenant
######*/
struct MANGOS_DLL_DECL mob_darkfallen_lieutenantAI : public ScriptedAI
{
	mob_darkfallen_lieutenantAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;

	uint32 m_uiRendFlashTimer;
	uint32 m_uiVampiricCurseTimer;

	void Reset()
	{
		m_uiRendFlashTimer               = urand(5000, 9000);
		m_uiVampiricCurseTimer           = urand(7000, 11000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiVampiricCurseTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				DoCast(pTarget, SPELL_VAMPIRIC_CURSE);
			m_uiVampiricCurseTimer = urand(7000, 11000);
		}
		else m_uiVampiricCurseTimer -= uiDiff;

		if (m_uiRendFlashTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_REND_FLASH_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature->getVictim(), SPELL_REND_FLASH_25);
			m_uiRendFlashTimer = urand(5000, 9000);
		}
		else m_uiRendFlashTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_lieutenant(Creature* pCreature)
{
	return new mob_darkfallen_lieutenantAI(pCreature);	
}
enum
{
	SPELL_BLOOD_SAP      	        = 70432,
	SPELL_SHADOWSTEP        		= 70431,
};

/*######
## Darkfallen Tactician
######*/
struct MANGOS_DLL_DECL mob_darkfallen_tacticianAI : public ScriptedAI
{
	mob_darkfallen_tacticianAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}

	uint32 m_uiBloodSapTimer;
	uint32 m_uiUnholyStrikeTimer;
	uint32 m_uiShadowstepTimer;

	void Reset()
	{
		m_uiBloodSapTimer                = urand(5000, 9000);
		m_uiShadowstepTimer              = urand(7000, 11000);
		m_uiUnholyStrikeTimer            = urand(9000, 15000);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiBloodSapTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_BLOOD_SAP);
			m_uiBloodSapTimer = urand(5000, 9000);
		}
		else m_uiBloodSapTimer -= uiDiff;

		if (m_uiShadowstepTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SHADOWSTEP);
			m_uiShadowstepTimer = urand(7000, 11000);
		}
		else m_uiShadowstepTimer -= uiDiff;

		if (m_uiUnholyStrikeTimer < uiDiff)
		{
			DoCast(m_creature->getVictim(), SPELL_UNHOLY_STRIKE);
			m_uiUnholyStrikeTimer = urand(9000, 15000);
		}
		else m_uiUnholyStrikeTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_tactician(Creature* pCreature)
{
	return new mob_darkfallen_tacticianAI(pCreature);	
}

enum
{
	SPELL_BATTLE_SHOUT     	        = 70750,
	SPELL_VAMPIRE_RUSH_10        	= 70449,
	SPELL_VAMPIRE_RUSH_25      	    = 71155,
};

/*######
## Darkfallen Commander
######*/
struct MANGOS_DLL_DECL mob_darkfallen_commanderAI : public ScriptedAI
{
	mob_darkfallen_commanderAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Difficulty = pCreature->GetMap()->GetDifficulty();
		Reset();
	}

	uint32 Difficulty;
	uint32 m_uiVampireRushTimer;

	void Reset()
	{
		m_uiVampireRushTimer                = urand(7000, 11000);
	}

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_BATTLE_SHOUT);
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiVampireRushTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_VAMPIRE_RUSH_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_VAMPIRE_RUSH_25);
			}
			m_uiVampireRushTimer = urand(7000, 11000);
		}
		else m_uiVampireRushTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_darkfallen_commander(Creature* pCreature)
{
	return new mob_darkfallen_commanderAI(pCreature);
}

/*########################################
##########################################
############## Svalna event ##############
##########################################
########################################*/

enum
{
	// event yells
	SAY_CROCK_INTRO					= -1609520,
	SAY_ARNATH_INTRO				= -1609521,
	SAY_CROCK_START					= -1609522,
	SAY_SVALNA_TAUNT				= -1609523,
	SAY_SVALNA_WIPE					= -1609524,
	SAY_RESSURECT_UNDEAD			= -1609525,
	SAY_AGGRO						= -1609526,
	SAY_KILL_CAPTAIN				= -1609527,
	SAY_KILL_PLAYER					= -1609528,
	SAY_DEATH						= -1609529,

	SPELL_AETHER_BURST_10     	    = 71468,
    SPELL_AETHER_BURST_25        	= 71469,
    SPELL_AETHER_SHIELD      	    = 71463,
	SPELL_CARESS_OF_DEATH           = 70078,
	SPELL_DIVINE_SURGE              = 71465,
	SPELL_IMPALING_SPEAR1           = 70169,
	SPELL_IMPALING_SPEAR2           = 71443,
	SPELL_REVIVE                    = 70053,
};

/*######
## Sister Svalna 
######*/
struct MANGOS_DLL_DECL miniboss_sister_svalnaAI : public ScriptedAI
{
    miniboss_sister_svalnaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    uint32 Difficulty;
    uint32 m_uiAetherBurstTimer;
	uint32 m_uiAetherShieldTimer;
	uint32 m_uiCaressOfDeathTimer;
	uint32 m_uiDivineSurgeTimer;
	uint32 m_uiImpaling1Timer;
	uint32 m_uiImpaling2Timer;
	uint32 m_uiReviveTimer;
	
	   void Reset()
    {
        m_uiAetherBurstTimer                = urand(5000, 9000);
		m_uiAetherShieldTimer               = urand(7000, 11000);
		m_uiCaressOfDeathTimer              = urand(10000, 15000);
		m_uiDivineSurgeTimer                = 15000;
		m_uiImpaling1Timer                  = urand(9000, 14000);
		m_uiImpaling2Timer                  = urand(7000, 15000);
		m_uiReviveTimer                     = 16000;
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAetherShieldTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(m_creature, SPELL_AETHER_SHIELD);
			m_uiAetherShieldTimer = urand(7000, 11000);
        }
        else m_uiAetherShieldTimer -= uiDiff;

		if (m_uiAetherBurstTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(pTarget, SPELL_AETHER_BURST_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_AETHER_BURST_25);
			}
			m_uiAetherBurstTimer = urand(5000, 9000);
        }
        else m_uiAetherBurstTimer -= uiDiff;


        if (m_uiCaressOfDeathTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CARESS_OF_DEATH);
			m_uiCaressOfDeathTimer = urand(10000, 15000);
        }
        else m_uiCaressOfDeathTimer -= uiDiff;



        if (m_uiDivineSurgeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_DIVINE_SURGE);
			m_uiAetherShieldTimer = 15000;
        }
        else m_uiDivineSurgeTimer -= uiDiff;

	

        if (m_uiImpaling1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_IMPALING_SPEAR1);
			m_uiImpaling1Timer = urand(9000, 14000);
        }
        else m_uiImpaling1Timer -= uiDiff;

		if (m_uiImpaling2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_IMPALING_SPEAR2);
			m_uiImpaling2Timer = urand(7000, 15000);
        }
        else m_uiImpaling2Timer -= uiDiff;

		if (m_uiReviveTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_REVIVE);
			m_uiReviveTimer = 16000;
        }
        else m_uiReviveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_miniboss_sister_svalna(Creature* pCreature)
{
    return new miniboss_sister_svalnaAI(pCreature);
}

enum
{
	SPELL_ICEBOUND_ARMOR	   	    = 70714,
	SPELL_DEATHSTRIKE	            = 71489,
	SPELL_DEATH_COIL_25 			= 71490,
	SPELL_SCOURGE_STRIKE            = 71488,
};

/*######
## // Crok Scourgebane
######*/
struct MANGOS_DLL_DECL mob_crok_scourgebaneAI : public ScriptedAI
{
    mob_crok_scourgebaneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiIceboundTimer;
	uint32 m_uiDeathstrikeTimer;
	uint32 m_uiDeathcoilTimer;
	uint32 m_uiScourgeStrikeTimer;
	
	   void Reset()
    {
        m_uiIceboundTimer          = 20000;
		m_uiDeathstrikeTimer       = urand(7000, 11000);
		m_uiDeathcoilTimer         = urand(9000, 13000);
		m_uiScourgeStrikeTimer     = urand(10000, 15000);
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiIceboundTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(m_creature, SPELL_ICEBOUND_ARMOR);
			m_uiIceboundTimer = 20000;
        }
        else m_uiIceboundTimer -= uiDiff;

		if (m_uiDeathcoilTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_DEATH_COIL_25);
			}
            m_uiDeathcoilTimer = urand(9000, 13000);
        }
        else m_uiDeathcoilTimer -= uiDiff;


		if (m_uiDeathstrikeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_DEATHSTRIKE);
            m_uiDeathstrikeTimer = urand(7000, 11000);
        }
        else m_uiDeathstrikeTimer -= uiDiff;

		if (m_uiScourgeStrikeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SCOURGE_STRIKE);
            m_uiScourgeStrikeTimer = urand(11000, 15000);
        }
        else m_uiScourgeStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_crok_scourgebane(Creature* pCreature)
{
    return new mob_crok_scourgebaneAI(pCreature);	
}

	enum
{

	SPELL_FLASH_HEAL	   	    = 71595,
	SPELL_FLASH_HEAL_10	        = 71782,
	SPELL_FLASH_HEAL_25         = 71783,
	SPELL_DOMINATE_MIND_25 		= 14515,
	SPELL_POWER_SHIELD          = 71548,
	SPELL_POWER_SHIELD_10       = 71780,
	SPELL_POWER_SHIELD_25       = 71781,
	SPELL_SMITE1_10             = 71546,
	SPELL_SMITE1_25             = 71547,
	SPELL_SMITE2_10             = 71778,
	SPELL_SMITE2_25             = 71779,
	SPELL_UNDEATH               = 70089,
};

/*######
## // Captain Arnath
######*/
struct MANGOS_DLL_DECL mob_captain_arnathAI : public ScriptedAI
{
    mob_captain_arnathAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiFlash1Timer;
	uint32 m_uiFlash2Timer;
	uint32 m_uiShield1Timer;
	uint32 m_uiShield2Timer;
	uint32 m_uiDominateTimer;
	uint32 m_uiSmite1Timer;
	uint32 m_uiSmite2Timer;
	uint32 m_uiUndeathTimer;
	
	   void Reset()
    {
        m_uiFlash1Timer            = 10000;
		m_uiFlash2Timer            = urand(7000, 11000);
		m_uiSmite1Timer            = urand(9000, 13000);
		m_uiSmite2Timer            = urand(10000, 15000);
		m_uiShield1Timer           = 9000;
		m_uiShield2Timer           = urand(10000, 17000);
		m_uiDominateTimer          = 14000;
		m_uiUndeathTimer           = urand(12000, 19000);
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFlash1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(m_creature, SPELL_FLASH_HEAL);
			m_uiFlash1Timer = 10000;
        }
        else m_uiFlash1Timer -= uiDiff;

		if (m_uiDominateTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_DOMINATE_MIND_25);
			}
            m_uiDominateTimer = 14000;
        }
        else m_uiDominateTimer -= uiDiff;


		if (m_uiFlash2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_FLASH_HEAL_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_FLASH_HEAL_25);
			}
			m_uiFlash2Timer = urand(7000, 11000);
        }
        else m_uiFlash2Timer -= uiDiff;

		if (m_uiShield2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_POWER_SHIELD_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_POWER_SHIELD_25);
			}
			m_uiShield2Timer = urand(10000, 17000);
        }
        else m_uiShield2Timer -= uiDiff;

		if (m_uiSmite1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(pTarget, SPELL_SMITE1_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_SMITE1_25);
			}
			m_uiSmite1Timer = urand(9000, 13000);
        }
        else m_uiSmite1Timer -= uiDiff;

		if (m_uiSmite2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(pTarget, SPELL_SMITE2_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_SMITE2_25);
			}
			m_uiSmite2Timer = urand(10000, 15000);
        }
        else m_uiSmite2Timer -= uiDiff;


		if (m_uiShield1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(m_creature, SPELL_POWER_SHIELD);
            m_uiShield1Timer = 9000;
        }
        else m_uiShield1Timer -= uiDiff;

		if (m_uiUndeathTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_UNDEATH);
            m_uiUndeathTimer = urand(12000, 19000);
        }
        else m_uiUndeathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_captain_arnath(Creature* pCreature)
{
    return new mob_captain_arnathAI(pCreature);
}

	enum
{

	SPELL_CRUSADER   	   	    = 71549,
	SPELL_DIVINE                = 71550,
	SPELL_HAMMER                = 71784,
	SPELL_JUDGE                 = 71551,	
};

/*######
## /// Captain Brandon
######*/
struct MANGOS_DLL_DECL mob_captain_brandonAI : public ScriptedAI
{
    mob_captain_brandonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiCrusaderTimer;
	uint32 m_uiDivineTimer;
	uint32 m_uiHammerTimer;
	uint32 m_uiJudgeTimer;
	uint32 m_uiUndeathTimer;
	
	   void Reset()
    {
        m_uiCrusaderTimer           = 10000;
		m_uiDivineTimer             = urand(7000, 11000);
		m_uiHammerTimer             = urand(9000, 13000);
		m_uiJudgeTimer              = urand(10000, 15000);
		m_uiUndeathTimer            = urand(12000, 19000);
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCrusaderTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CRUSADER);
			m_uiCrusaderTimer = 10000;
        }
        else m_uiCrusaderTimer -= uiDiff;

		if (m_uiDivineTimer < uiDiff)
        {
			DoCast(m_creature, SPELL_DIVINE);
			m_uiDivineTimer = urand(7000,11000);
        }
        else m_uiDivineTimer -= uiDiff;
    
		if (m_uiHammerTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_HAMMER);
            m_uiHammerTimer = urand(9000, 13000);
        }
        else m_uiHammerTimer -= uiDiff;

		if (m_uiJudgeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_JUDGE);
            m_uiJudgeTimer = urand(10000, 15000);
        }
        else m_uiJudgeTimer -= uiDiff;

		if (m_uiUndeathTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_UNDEATH);
            m_uiUndeathTimer = urand(12000, 19000);
        }
        else m_uiUndeathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_captain_brandon(Creature* pCreature)
{
    return new mob_captain_brandonAI(pCreature);
}

enum
{

	SPELL_CHARGE     	   	    = 71553,
	SPELL_CONFLAGRATION         = 71785,
	SPELL_MORTAL_STRIKE         = 71552,
	SPELL_SUNDER_ARMOR  		= 71554,	
};

/*######
## // Captain Grondel
######*/
struct MANGOS_DLL_DECL mob_captain_grondelAI : public ScriptedAI
{
    mob_captain_grondelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiChargeTimer;
	uint32 m_uiConflagrationTimer;
	uint32 m_uiMstrikeTimer;
	uint32 m_uiSunderTimer;
	uint32 m_uiUndeathTimer;
	
	   void Reset()
    {
        m_uiChargeTimer            = 10000;
		m_uiConflagrationTimer     = urand(7000, 11000);
		m_uiMstrikeTimer           = urand(9000, 13000);
		m_uiSunderTimer            = urand(10000, 15000);
		m_uiUndeathTimer           = urand(12000, 19000);
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChargeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CHARGE);
			m_uiChargeTimer = 10000;
        }
        else m_uiChargeTimer -= uiDiff;

		if (m_uiConflagrationTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CONFLAGRATION);
			
            m_uiConflagrationTimer = 14000;
        }
        else m_uiConflagrationTimer -= uiDiff;


		if (m_uiMstrikeTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			    DoCast(pTarget, SPELL_MORTAL_STRIKE);
			m_uiMstrikeTimer = urand(7000, 11000);
        }
        else m_uiMstrikeTimer -= uiDiff;

		if (m_uiSunderTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			    DoCast(pTarget, SPELL_SUNDER_ARMOR);
			m_uiSunderTimer = urand(10000, 17000);
        }
        else m_uiSunderTimer -= uiDiff;

       if (m_uiUndeathTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_UNDEATH);
            m_uiUndeathTimer = urand(12000, 19000);
        }
        else m_uiUndeathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_captain_grondel(Creature* pCreature)
{
    return new mob_captain_grondelAI(pCreature);
}

enum
{

	SPELL_FEL_BOMB1  	   	    = 71592,
	SPELL_FEL_BOMB2             = 71787,
	SPELL_MACHINE_GUN           = 71594,
	SPELL_MACHINE_GUN_10        = 71780,
	SPELL_MACHINE_GUN_25        = 71781,
	SPELL_ROCKET1               = 71590,
	SPELL_ROCKET2               = 71786,
};

/*######
## // Captain Rupert
######*/
struct MANGOS_DLL_DECL mob_captain_rupertAI : public ScriptedAI
{
    mob_captain_rupertAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiFel1Timer;
	uint32 m_uiFel2Timer;
	uint32 m_uiMg1Timer;
	uint32 m_uiMg2Timer;
	uint32 m_uiRocket1Timer;
	uint32 m_uiRocket2Timer;
	uint32 m_uiUndeathTimer;
	
	   void Reset()
    {
        m_uiFel1Timer            = 10000;
		m_uiFel2Timer            = urand(7000, 11000);
		m_uiMg1Timer             = urand(9000, 13000);
		m_uiMg2Timer             = urand(10000, 15000);
		m_uiRocket1Timer         = 9000;
		m_uiRocket2Timer         = urand(10000, 17000);
		m_uiUndeathTimer         = urand(12000, 19000);
	 }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFel1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_FEL_BOMB1);
			m_uiFel1Timer = 10000;
        }
        else m_uiFel1Timer -= uiDiff;

		if (m_uiFel2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
		        DoCast(pTarget, SPELL_FEL_BOMB2);
		
            m_uiFel2Timer = urand(7000, 11000);
        }
        else m_uiFel2Timer -= uiDiff;

		if (m_uiMg1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
		        DoCast(pTarget, SPELL_MACHINE_GUN);
		
            m_uiMg1Timer = urand(7000, 11000);
        }
        else m_uiMg1Timer -= uiDiff;


		if (m_uiMg2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(pTarget, SPELL_MACHINE_GUN_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(pTarget, SPELL_MACHINE_GUN_25);
			}
			m_uiMg2Timer = urand(7000, 11000);
        }
        else m_uiMg2Timer -= uiDiff;

		if (m_uiRocket1Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ROCKET1);
            m_uiRocket1Timer = 9000;
        }
        else m_uiRocket1Timer -= uiDiff;

		if (m_uiRocket2Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ROCKET2);
            m_uiRocket2Timer = 9000;
        }
        else m_uiRocket2Timer -= uiDiff;

		if (m_uiUndeathTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_UNDEATH);
            m_uiUndeathTimer = urand(12000, 19000);
        }
        else m_uiUndeathTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_captain_rupert(Creature* pCreature)
{
    return new mob_captain_rupertAI(pCreature);
}



enum
{
	SPELL_ADRENALINE_RUSH 	   	    = 71258,
	SPELL_BARBARIC_STRIKE           = 71257,
};

/*######
## // Ymirjar battle maiden
######*/
struct MANGOS_DLL_DECL mob_ymirjar_battlemaidenAI : public ScriptedAI
{
    mob_ymirjar_battlemaidenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiAdrenalineTimer;
	uint32 m_uiBarbaricTimer;
	
	
	void Reset()
	{
		m_uiAdrenalineTimer            = 10000;
		m_uiBarbaricTimer              = urand(7000, 11000);
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAdrenalineTimer < uiDiff)
        {
			DoCast(m_creature, SPELL_ADRENALINE_RUSH);
			m_uiAdrenalineTimer = 10000;
        }
        else m_uiAdrenalineTimer -= uiDiff;

		if (m_uiBarbaricTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
		        DoCast(pTarget, SPELL_BARBARIC_STRIKE);
            m_uiBarbaricTimer = urand(7000, 11000);
        }
        else m_uiBarbaricTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ymirjar_battlemaiden(Creature* pCreature)
{
    return new mob_ymirjar_battlemaidenAI(pCreature);
}

enum
{
	SPELL_BANISH	   	            = 71298,
	SPELL_DEATH_10                  = 71299,
	SPELL_DEATH_25                  = 71300,
	SPELL_SHADOW_10                 = 71296,
	SPELL_SHADOW_25                 = 71297,
	SPELL_SPIRIT_STREAM             = 69929,
	SPELL_SUMM_Y                    = 71303,
};

/*######
## // Ymirjar deathbringer
######*/
struct MANGOS_DLL_DECL mob_ymirjar_deathbringerAI : public ScriptedAI
{
    mob_ymirjar_deathbringerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiBanishTimer;
	uint32 m_uiDeathTimer;
	uint32 m_uiShadowTimer;
	uint32 m_uiSpiritTimer;
	uint32 m_uiSummonTimer;

	void Reset()
	{
		m_uiBanishTimer            = 10000;
		m_uiDeathTimer             = urand(7000, 11000);
		m_uiShadowTimer            = 14000;
		m_uiSpiritTimer            = 15000;
		m_uiSummonTimer            = 20000;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBanishTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				DoCast(pTarget, SPELL_BANISH);
			m_uiBanishTimer = 10000;
        }
        else m_uiBanishTimer -= uiDiff;

		if (m_uiShadowTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_SHADOW_25);
			}
			m_uiShadowTimer = urand(7000, 11000);
		}
        else m_uiShadowTimer -= uiDiff;

		if (m_uiDeathTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_DEATH_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_DEATH_25);
			m_uiDeathTimer = urand(7000, 11000);
		}
        else m_uiDeathTimer -= uiDiff;

		if (m_uiSpiritTimer < uiDiff)
        {
			switch(urand(0, 1))
			{
			case 0:
				if(Unit* pTarget = GetClosestCreatureWithEntry(m_creature, 37132, 50.0f))
					DoCast(pTarget, SPELL_SPIRIT_STREAM);
				break;
			case 1:
				if(Unit* pTarget = GetClosestCreatureWithEntry(m_creature, 37134, 50.0f))
					DoCast(pTarget, SPELL_SPIRIT_STREAM);
				break;
			}		
            m_uiSpiritTimer = urand(7000, 11000);
        }
		else m_uiSpiritTimer -= uiDiff;

		if (m_uiSummonTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_SUMM_Y);
			m_uiSummonTimer = urand(7000, 11000);
		}
		else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ymirjar_deathbringer(Creature* pCreature)
{
    return new mob_ymirjar_deathbringerAI(pCreature);
}


enum
{
	SPELL_WHIRL1	   	            = 41057,
	SPELL_WHIRL2                    = 41056,
};

/*######
## // Ymirjar warlord
######*/
struct MANGOS_DLL_DECL mob_ymirjar_warlordAI : public ScriptedAI
{
    mob_ymirjar_warlordAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiWhirl1Timer;
	uint32 m_uiWhirl2Timer;
	
	void Reset()
	{
		m_uiWhirl1Timer            = 10000;
		m_uiWhirl2Timer            = urand(7000, 11000);
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiWhirl1Timer < uiDiff)
        {
			DoCast(m_creature, SPELL_WHIRL1);
			m_uiWhirl1Timer = 10000;
        }
        else m_uiWhirl1Timer -= uiDiff;

		if (m_uiWhirl2Timer < uiDiff)
        {
			DoCast(m_creature, SPELL_WHIRL2);
			m_uiWhirl2Timer = 10000;
        }
        else m_uiWhirl2Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ymirjar_warlord(Creature* pCreature)
{
    return new mob_ymirjar_warlordAI(pCreature);
}


enum
{
	SPELL_ICE_TRAP	   	            = 71249,
	SPELL_RAPID_SHOT                = 71251,
	SPELL_SHOOT                     = 71253,
	SPELL_SUMMON_W                  = 71705,
	SPELL_VOLLEY                    = 71252,
};

/*######
## // Ymirjar huntress
######*/
struct MANGOS_DLL_DECL mob_ymirjar_huntressAI : public ScriptedAI
{
    mob_ymirjar_huntressAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;

    uint32 m_uiIcetrapTimer;
	uint32 m_uiRapidshotTimer;
	uint32 m_uiShootTimer;
	uint32 m_uiSummonTimer;
	uint32 m_uiVolleyTimer;
	
	void Reset()
	{
		m_uiIcetrapTimer            = 10000;
		m_uiRapidshotTimer          = urand(7000, 11000);
		m_uiShootTimer              = 14000;
		m_uiSummonTimer             = 15000;
		m_uiVolleyTimer             = 20000;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiIcetrapTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ICE_TRAP);
			m_uiIcetrapTimer = 10000;
        }
        else m_uiIcetrapTimer -= uiDiff;

		if (m_uiSummonTimer < uiDiff)
        {
			DoCast(m_creature, SPELL_SUMMON_W);
			m_uiSummonTimer = 15000;
        }
        else m_uiSummonTimer -= uiDiff;

		if (m_uiRapidshotTimer < uiDiff)
        {
			DoCast(m_creature, SPELL_RAPID_SHOT);
			m_uiRapidshotTimer = urand(7000, 11000);
        }
        else m_uiRapidshotTimer -= uiDiff;

		if (m_uiShootTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_SHOOT);
			m_uiShootTimer = 14000;
        }
        else m_uiShootTimer -= uiDiff;

		if (m_uiVolleyTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_VOLLEY);
			m_uiVolleyTimer = 20000;
		}
		else m_uiVolleyTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_ymirjar_huntress(Creature* pCreature)
{
    return new mob_ymirjar_huntressAI(pCreature);
}

/*########################################
##########################################
############ Frostwing trash #############
##########################################
########################################*/

enum
{
	SPELL_CONC_SHOT_10               = 71337,
	SPELL_CONC_SHOT_25               = 71338,
	SPELL_ORDER_WHELP                = 71357,
};

/*######
## // Frostwarden handler
######*/
struct MANGOS_DLL_DECL mob_frostwarden_handlerAI : public ScriptedAI
{
    mob_frostwarden_handlerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;
	uint32 m_uiConcShotTimer;
	uint32 m_uiOrderTimer;

	void Reset()
	{
		m_uiConcShotTimer                = urand(7000, 11000);
		m_uiOrderTimer                   = 15000 ;		
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiConcShotTimer < uiDiff)
		{
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
				DoCast(m_creature, SPELL_CONC_SHOT_10);
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
				DoCast(m_creature, SPELL_CONC_SHOT_25);
			m_uiConcShotTimer = 14000;
		}
		else m_uiConcShotTimer -= uiDiff;

		if (m_uiOrderTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			    DoCast(pTarget, SPELL_ORDER_WHELP);
			m_uiOrderTimer = 15000;
        }
        else m_uiOrderTimer -= uiDiff;
		    
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_frostwarden_handler(Creature* pCreature)
{
    return new mob_frostwarden_handlerAI(pCreature);
}

enum
{
	SPELL_FROST_BLAST_10               = 71361,
	SPELL_FROST_BLAST_25               = 71362,
	SPELL_FOCUS_FIRE                   = 71350,
};

/*######
## // Frostwing whelp
######*/
struct MANGOS_DLL_DECL mob_frostwarden_whelpAI : public ScriptedAI
{
    mob_frostwarden_whelpAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
		Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

	uint32 Difficulty;
	uint32 m_uiFrostBTimer;
	uint32 m_uiFocusTimer;
	
	void Reset()
	{
		m_uiFrostBTimer                      = urand(7000, 11000);
		m_uiFocusTimer                       = 15000 ;
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiFrostBTimer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(pTarget, SPELL_FROST_BLAST_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(pTarget, SPELL_FROST_BLAST_25);
			}
			m_uiFrostBTimer = 14000;
		}
		else m_uiFrostBTimer -= uiDiff;

		if (m_uiFocusTimer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				m_creature->AddThreat(pTarget, 10000.0f);
			    DoCast(pTarget, SPELL_FOCUS_FIRE);
			}		
			m_uiFocusTimer = 15000;
        }
        else m_uiFocusTimer -= uiDiff;
		    
		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_frostwarden_whelp(Creature* pCreature)
{
    return new mob_frostwarden_whelpAI(pCreature);
}

void AddSC_icecrown_citadel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "icecrown_citadel_teleporter";
    newscript->pGOHello = &GoHello_icecrown_citadel_teleporter;
    newscript->pGOGossipSelect = &GoSelect_icecrown_citadel_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "miniboss_stinky";
    newscript->GetAI = &GetAI_miniboss_stinky;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "miniboss_precious";
    newscript->GetAI = &GetAI_miniboss_precious;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spire_frostwyrm";
    newscript->GetAI = &GetAI_mob_spire_frostwyrm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_rotting_frost_giant";
    newscript->GetAI = &GetAI_mob_rotting_frost_giant;
    newscript->RegisterSelf();

	// ### Entrance Trash ###
	newscript = new Script;
    newscript->Name = "mob_the_damned";
    newscript->GetAI = &GetAI_mob_the_damned;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = " mob_ancient_skeletal_soldier";
    newscript->GetAI = &GetAI_mob_ancient_skeletal_soldier;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_servant_of_the_throne";
    newscript->GetAI = &GetAI_mob_servant_of_the_throne;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_nerubar_broodkeeper";
	newscript->GetAI = &GetAI_mob_nerubar_broodkeeper;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_deathbound_ward";
    newscript->GetAI = &GetAI_mob_deathbound_ward;
    newscript->RegisterSelf();

	// ### Oratory Trash ###
	newscript = new Script;
    newscript->Name = "mob_deathspeaker_high_priest";
    newscript->GetAI = &GetAI_mob_deathspeaker_high_priest;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_deathspeaker_attendant";
    newscript->GetAI = &GetAI_mob_deathspeaker_attendant;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_deathspeaker_servant";
    newscript->GetAI = &GetAI_mob_deathspeaker_servant;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_deathspeaker_zealot";
    newscript->GetAI = &GetAI_mob_deathspeaker_zealot;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_deathspeaker_disciple";
    newscript->GetAI = &GetAI_mob_deathspeaker_disciple;
    newscript->RegisterSelf();

	// ### Rampart Trash ###
	newscript = new Script;
    newscript->Name = "mob_frenzied_abomination";
    newscript->GetAI = &GetAI_mob_frenzied_abomination;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_spire_gargoyle";
    newscript->GetAI = &GetAI_mob_spire_gargoyle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spire_minion";
    newscript->GetAI = &GetAI_mob_spire_minion;
    newscript->RegisterSelf();

	// ### Soldiers Trash ###
    newscript = new Script;
    newscript->Name = "mob_gunship_dreadblade";
    newscript->GetAI = &GetAI_mob_gunship_dreadblade;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_gunship_marksman";
    newscript->GetAI = &GetAI_mob_gunship_marksman;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name = "mob_gunship_protector";
    newscript->GetAI = &GetAI_mob_gunship_protector;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_luminary";
	newscript->GetAI = &GetAI_mob_gunship_luminary;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_sorcerer";
	newscript->GetAI = &GetAI_mob_gunship_sorcerer;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_primalist";
	newscript->GetAI = &GetAI_mob_gunship_primalist;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_templar";
	newscript->GetAI = &GetAI_mob_gunship_templar;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_necrolyte";
	newscript->GetAI = &GetAI_mob_gunship_necrolyte;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_vindicator";
	newscript->GetAI = &GetAI_mob_gunship_vindicator;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_gunship_assasin";
	newscript->GetAI = &GetAI_mob_gunship_assasin;
    newscript->RegisterSelf();

	// ### Plagueworks Trash ###
	newscript = new Script;
    newscript->Name = "mob_blighted_abomination";
    newscript->GetAI = &GetAI_mob_blighted_abomination;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_plague_scientist";
    newscript->GetAI = &GetAI_mob_plague_scientist;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_vengefull_fleshreaper";
    newscript->GetAI = &GetAI_mob_vengefull_fleshreaper;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_decaying_colossus";
    newscript->GetAI = &GetAI_mob_decaying_colossus;
    newscript->RegisterSelf();

	// ### Crimson Halls ###
	newscript = new Script;
    newscript->Name = "mob_valkyr_herald";
    newscript->GetAI = &GetAI_mob_valkyr_herald;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_archmage";
    newscript->GetAI = &GetAI_mob_darkfallen_archmage;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_blood_knight";
    newscript->GetAI = &GetAI_mob_darkfallen_blood_knight;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_noble";
    newscript->GetAI = &GetAI_mob_darkfallen_noble;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_advisor";
    newscript->GetAI = &GetAI_mob_darkfallen_advisor;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_lieutenant";
    newscript->GetAI = &GetAI_mob_darkfallen_lieutenant;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_tactician";
    newscript->GetAI = &GetAI_mob_darkfallen_tactician;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_darkfallen_commander";
    newscript->GetAI = &GetAI_mob_darkfallen_commander;
    newscript->RegisterSelf();

	//   ### Frostwing Trash ###
	newscript = new Script;
    newscript->Name = "miniboss_sister_svalna";
    newscript->GetAI = &GetAI_miniboss_sister_svalna;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_crok_scourgebane";
    newscript->GetAI = &GetAI_mob_crok_scourgebane;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_captain_arnath";
    newscript->GetAI = &GetAI_mob_captain_arnath;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_captain_brandon";
    newscript->GetAI = &GetAI_mob_captain_brandon;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_captain_grondel";
    newscript->GetAI = &GetAI_mob_captain_grondel;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_captain_rupert";
    newscript->GetAI = &GetAI_mob_captain_rupert;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ymirjar_battlemaiden";
    newscript->GetAI = &GetAI_mob_ymirjar_battlemaiden;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ymirjar_deathbringer";
    newscript->GetAI = &GetAI_mob_ymirjar_deathbringer;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ymirjar_warlord";
    newscript->GetAI = &GetAI_mob_ymirjar_warlord;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_ymirjar_huntress";
    newscript->GetAI = &GetAI_mob_ymirjar_huntress;
    newscript->RegisterSelf();
	
	// ### Sindragosa Event ###
	newscript = new Script;
    newscript->Name = "mob_frostwarden_handler";
    newscript->GetAI = &GetAI_mob_frostwarden_handler;
    newscript->RegisterSelf();
	
	newscript = new Script;
    newscript->Name = "mob_frostwarden_whelp";
    newscript->GetAI = &GetAI_mob_frostwarden_whelp;
    newscript->RegisterSelf();
}