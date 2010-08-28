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
SDName: gunship_battle
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
	// saurfang & bronzebeard
	SPELL_BATTLE_FURY		= 69637,
	SPELL_CLEAVE			= 15284,
	SPELL_RENDING_THROW		= 70309,
	SPELL_TASTE_BLOOD		= 69634,
	NPC_BRONZEBEARD			= 36948,
	NPC_SAURFANG			= 36939,

	// all troops
	SPELL_EXPERIENCED		= 71188,
	SPELL_VETERAN			= 71193,
	SPELL_ELITE				= 71195,
	SPELL_DESPERATE_RESOLVE	= 72536,

	// sergeant
	SPELL_BLADESTORM		= 69652,
	SPELL_WOUNDING_STRIKE	= 69651,
	NPC_KORKRON_SERGEANT	= 36960,
	NPC_SKYBREAKER_SERGEANT	= 36961,

	// soldiers
	SPELL_BURNING_PITCH		= 71335,
	NPC_KORKRON_REAVER		= 36957,
	NPC_SKYBREKER_MARINE	= 36950,

	// axethrower / rifleman
	SPELL_HURL_AXE			= 72539,
	NPC_KORKRON_AXETHROWER	= 36968,
	SPELL_SHOOT				= 70162,
	NPC_SKYBREAKER_RIFLEMAN	= 36969,

	// rocketeer / mortar soldier
	SPELL_ROCKET_ARTILLERY	= 69679,
	NPC_KORKRON_ROCKETEER	= 36982,
	NPC_SKYBREAKER_MORTAR	= 36978,

	SPELL_BELOW_ZERO		= 69705,
	NPC_KORKRON_BATTLEMAGE	= 37117,
	NPC_SKYBREAKER_SORCERER	= 37116,

	// cannons
	NPC_ALY_GUNSHIP_CANNON	= 36838,
	NPC_HORDE_GUNSHIP_CANNON= 36839,
};

enum AI
{
    AI_MELEE    = 0,
    AI_RANGED   = 1,
};

#define GOSSIP_ITEM "Start the Gunship event!"

// common parts for npcs
struct MANGOS_DLL_DECL gunship_battle_soldierAI: public ScriptedAI
{
    gunship_battle_soldierAI(Creature* pCreature, uint32 aitype): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        mAIType = aitype;
        Init();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    uint32 mAIType;
    uint32 m_uiThreatTimer;
    uint32 TeamInInstance;
	uint32 m_uiBuffTimer;
	uint8 m_uiBuffStage;
	bool m_bHasResolved;

    void Init()
    {
        m_uiThreatTimer = 15000;
        TeamInInstance	= GetFaction();
		m_uiBuffTimer	= 20000;
		m_uiBuffStage	= 0;
		m_bHasResolved	= false;
        m_creature->SetRespawnDelay(DAY);
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if(mAIType==AI_MELEE)
                DoStartMovement(pWho);
            else
                DoStartMovement(pWho, 20.0f);
        }
    }

    void Aggro(Unit *who)
    {
    }

    void JustReachedHome()
    {
    }

    void UpdatePower()
    {
        if(m_creature->getPowerType() == POWER_MANA)
            m_creature->ModifyPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA) / 3);
    }

    float CalculateThreat(float distance, float armor, uint32 health)
    {
        float dist_mod = (mAIType == AI_MELEE) ? 15.0f/(15.0f + distance) : 1.0f;
        float armor_mod = (mAIType == AI_MELEE) ? armor / 16635.0f : 0.0f;
        float eh = (health+1) * (1.0f + armor_mod);
        return dist_mod * 30000.0f / eh;
    }

    void UpdateThreat()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator itr;
        bool empty = true;
        for(itr = tList.begin(); itr!=tList.end(); ++itr)
        {
            Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
            if (pUnit && m_creature->getThreatManager().getThreat(pUnit))
            {
                if(pUnit->GetTypeId()==TYPEID_PLAYER)
                {
                    float threat = CalculateThreat(m_creature->GetDistance2d(pUnit), (float)pUnit->GetArmor(), pUnit->GetHealth());
                    m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
                    m_creature->AddThreat(pUnit, 1000000.0f * threat);
                    empty = false;
                }
            }
        }
        //if(empty) 
            //EnterEvadeMode();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiThreatTimer < uiDiff)
        {
            UpdatePower();
            UpdateThreat();
            m_uiThreatTimer = 4000;
        }
        else m_uiThreatTimer -= uiDiff;

		if(m_uiBuffTimer < uiDiff)
		{
			if(m_uiBuffStage == 0)
				DoCast(m_creature, SPELL_EXPERIENCED);
			else if(m_uiBuffStage == 1)
				DoCast(m_creature, SPELL_VETERAN);
			else if(m_uiBuffStage == 2)
				DoCast(m_creature, SPELL_ELITE);

			m_uiBuffStage += 1;
			m_uiBuffTimer = 20000;
		}
		else m_uiBuffTimer -= uiDiff;

		if(m_creature->GetHealthPercent() < 20.0f && !m_bHasResolved)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_DESPERATE_RESOLVE);
			m_bHasResolved = true;
		}

		DoMeleeAttackIfReady();
    }
};

// Saurfang & Bronzebeard
struct MANGOS_DLL_DECL boss_gunship_commanderAI: public gunship_battle_soldierAI
{
    boss_gunship_commanderAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_MELEE)
    {
        Reset();
    }

    uint32 m_uiCleaveTimer;
    uint32 m_uiRendingThrowTimer;
 
    void Reset()
    {
        m_uiCleaveTimer         = 3000;
        m_uiRendingThrowTimer   = 30000;
		DoCast(m_creature, SPELL_BATTLE_FURY);
    }

	void Aggro(Unit* pWho)
	{
		DoCast(m_creature, SPELL_TASTE_BLOOD);
	}

	void StartEvent()
	{
		// start gunship battle event here
		if(m_pInstance)
			m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, IN_PROGRESS);
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCleaveTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = 4000;
        }
        else
            m_uiCleaveTimer -= uiDiff;  

        if (m_uiRendingThrowTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_RENDING_THROW);
            m_uiRendingThrowTimer = 10000;
        }
        else
            m_uiRendingThrowTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_gunship_commander(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance *m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!m_pInstance) return true;

	if(m_pInstance->GetData(TYPE_GUNSHIP_BATTLE) != DONE && m_pInstance->GetData(TYPE_GUNSHIP_BATTLE) != IN_PROGRESS)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}

bool GossipSelect_gunship_commander(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        ((boss_gunship_commanderAI*)pCreature->AI())->StartEvent();
        pPlayer->CLOSE_GOSSIP_MENU();
        break;
    }

    return true;
}

// Sergeant
struct MANGOS_DLL_DECL mob_sergeantAI: public gunship_battle_soldierAI
{
    mob_sergeantAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_MELEE)
    {
        Reset();
    }

    uint32 m_uiBladestorm_Timer;
    uint32 m_uiWounding_Strike_Timer;

    void Reset()
    {
        m_uiBladestorm_Timer         = 10000;
        m_uiWounding_Strike_Timer    = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBladestorm_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BLADESTORM);
            m_uiBladestorm_Timer = 10000;
        }
        else m_uiBladestorm_Timer -= uiDiff;  

        if (m_uiWounding_Strike_Timer < uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_WOUNDING_STRIKE);
            m_uiWounding_Strike_Timer = 20000;
        }
        else m_uiWounding_Strike_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// R/M Soldiers
struct MANGOS_DLL_DECL mob_rm_soldiersAI: public gunship_battle_soldierAI
{
    mob_rm_soldiersAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiRocket_Artillery_Timer;

	void Reset()
	{
		m_uiRocket_Artillery_Timer      = 5000;
	}

	void UpdateAI(const uint32 uiDiff)
	{
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if (m_uiRocket_Artillery_Timer < uiDiff)
		{
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_ROCKET_ARTILLERY);
			m_uiRocket_Artillery_Timer = 10000;
		}
		else
			m_uiRocket_Artillery_Timer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

// Rifleman
struct MANGOS_DLL_DECL mob_riflemanAI: public gunship_battle_soldierAI
{
    mob_riflemanAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiShoot_Timer;

    void Reset()
    {
        m_uiShoot_Timer             = 4000;        
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShoot_Timer < uiDiff)
        {
			if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				if(m_creature->GetEntry() == NPC_KORKRON_AXETHROWER)
					DoCast(pTarget, SPELL_HURL_AXE);
				else if(m_creature->GetEntry() == NPC_SKYBREAKER_RIFLEMAN)
					DoCast(pTarget, SPELL_SHOOT);
			}
            m_uiShoot_Timer = 2000;
        }
        else m_uiShoot_Timer -= uiDiff;  

        DoMeleeAttackIfReady();
    }
};

// Battle Mage
struct MANGOS_DLL_DECL mob_battle_mageAI: public gunship_battle_soldierAI
{
    mob_battle_mageAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiFreezeTimer;

    void Reset()
    {
        m_uiFreezeTimer             = 4000;        
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiFreezeTimer < uiDiff)
		{
			if(m_creature->GetEntry() == NPC_KORKRON_BATTLEMAGE)
			{
				if(Unit* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_ALY_GUNSHIP_CANNON, 50.0f))
					DoCast(pTarget, SPELL_BELOW_ZERO);
			}
			else if(m_creature->GetEntry() == NPC_SKYBREAKER_RIFLEMAN)
			{
				if(Unit* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_HORDE_GUNSHIP_CANNON, 50.0f))
					DoCast(pTarget, SPELL_BELOW_ZERO);
			}
			m_uiFreezeTimer = 30000;
		}
		else m_uiFreezeTimer -= uiDiff;  

		DoMeleeAttackIfReady();
	}
};

// Marine / Reaver
struct MANGOS_DLL_DECL mob_marineAI: public gunship_battle_soldierAI
{
    mob_marineAI(Creature* pCreature): gunship_battle_soldierAI(pCreature, AI_MELEE)
	{
		Reset();
	}

	void Reset()
	{
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return; 

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_gunship_commander(Creature* pCreature)
{
    return new boss_gunship_commanderAI(pCreature);
}

CreatureAI* GetAI_mob_sergeant(Creature* pCreature)
{
    return new mob_sergeantAI(pCreature);
}

CreatureAI* GetAI_mob_rm_soldiers(Creature* pCreature)
{
    return new mob_rm_soldiersAI(pCreature);
}

CreatureAI* GetAI_mob_rifleman(Creature* pCreature)
{
    return new mob_riflemanAI(pCreature);
}

CreatureAI* GetAI_mob_battle_mage(Creature* pCreature)
{
    return new mob_battle_mageAI(pCreature);
}

CreatureAI* GetAI_mob_marine(Creature* pCreature)
{
    return new mob_marineAI(pCreature);
}

void AddSC_gunship_battle()
{
    Script* pNewScript;

    //pNewScript = new Script;
    //pNewScript->Name = "boss_gunship_commander";
    //pNewScript->GetAI = &GetAI_boss_gunship_commander;
	//pNewScript->pGossipHello = &GossipHello_gunship_commander;
    //pNewScript->pGossipSelect = &GossipSelect_gunship_commander;
    //pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "mob_sergeant";
    pNewScript->GetAI = &GetAI_mob_sergeant;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "mob_rm_soldiers";
    pNewScript->GetAI = &GetAI_mob_rm_soldiers;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "mob_rifleman";
    pNewScript->GetAI = &GetAI_mob_rifleman;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "mob_battle_mage";
    pNewScript->GetAI = &GetAI_mob_battle_mage;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "mob_marine";
    pNewScript->GetAI = &GetAI_mob_marine;
    pNewScript->RegisterSelf();
}