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
SDName: Boss_Broggok
SD%Complete: 70
SDComment: pre-event not made
SDCategory: Hellfire Citadel, Blood Furnace
EndScriptData */

#include "precompiled.h"
#include "blood_furnace.h"

enum
{
    SAY_AGGRO               = -1542008,

    SPELL_SLIME_SPRAY       = 30913,
    H_SPELL_SLIME_SPRAY     = 38458,
    SPELL_POISON_CLOUD      = 30916,
    SPELL_POISON_BOLT       = 30917,
    H_SPELL_POISON_BOLT     = 38459,

	NPC_NASCENT_ORC			= 17398,

    SPELL_POISON            = 30914
};

struct MANGOS_DLL_DECL boss_broggokAI : public ScriptedAI
{
    boss_broggokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 AcidSpray_Timer;
    uint32 PoisonSpawn_Timer;
    uint32 PoisonBolt_Timer;

	uint32 m_uiOrcPrisonerTimer;
	uint32 m_uiOrcPrisonerStage;

    void Reset()
    {
        AcidSpray_Timer = 10000;
        PoisonSpawn_Timer = 5000;
        PoisonBolt_Timer = 7000;
		m_uiOrcPrisonerTimer	= 1000;
		m_uiOrcPrisonerStage	= 0;
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BROGGOK_EVENT,FAIL);

		if(GameObject* pLever = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_PRISON_CELL_LEVER)))
			pLever->Respawn();//RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK1)))
			pDoor->SetGoState(GO_STATE_READY);
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK2)))
			pDoor->SetGoState(GO_STATE_READY);
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK3)))
			pDoor->SetGoState(GO_STATE_READY);
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK4)))
			pDoor->SetGoState(GO_STATE_READY);
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_DOOR_BROGGOK_REAR)))
			pDoor->SetGoState(GO_STATE_READY);

		std::list<Creature*> lOrcs;
		GetCreatureListWithEntryInGrid(lOrcs, m_creature, NPC_NASCENT_ORC, DEFAULT_VISIBILITY_INSTANCE);
		if (!lOrcs.empty())
        {
            for(std::list<Creature*>::iterator iter = lOrcs.begin(); iter != lOrcs.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
				{
                    (*iter)->Respawn();
					(*iter)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					(*iter)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
				}
            }
        }
    }

	void AttackStart(Unit* pWho)
	{
		if (m_uiOrcPrisonerStage < 5) 
			return;

		if (m_creature->Attack(pWho, true)) 
		{
			m_creature->AddThreat(pWho);
			m_creature->SetInCombatWith(pWho);
			pWho->SetInCombatWith(m_creature);
			DoStartMovement(pWho);
		}
	}

    void JustSummoned(Creature *summoned)
    {
        summoned->setFaction(16);
        summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        summoned->CastSpell(summoned,SPELL_POISON,false,0,0,m_creature->GetGUID());
    }

    void JustDied(Unit *who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BROGGOK_EVENT,DONE);
    }

	void OpenPrison(uint64 uiCellDoorGUID)
	{
		if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(uiCellDoorGUID))
		{
			pDoor->SetGoState(GO_STATE_ACTIVE);

			std::list<Creature*> lOrcs;
			GetCreatureListWithEntryInGrid(lOrcs, pDoor, NPC_NASCENT_ORC, 15.0f);
			if (!lOrcs.empty())
			{
				for(std::list<Creature*>::iterator iter = lOrcs.begin(); iter != lOrcs.end(); ++iter)
				{
					if ((*iter) && (*iter)->isAlive())
					{
						(*iter)->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						(*iter)->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
						(*iter)->SetInCombatWithZone();
					}
				}
			}
		}
	}

    void UpdateAI(const uint32 diff)
    {
		if(m_pInstance->GetData(TYPE_BROGGOK_EVENT) == IN_PROGRESS && m_uiOrcPrisonerStage < 5)
		{
			if(m_uiOrcPrisonerTimer < diff)
			{
				switch(m_uiOrcPrisonerStage)
				{
				case 0:
					OpenPrison(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK2));
					++m_uiOrcPrisonerStage;
					break;
				case 1:
					OpenPrison(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK4));
					++m_uiOrcPrisonerStage;
					break;
				case 2:
					OpenPrison(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK1));
					++m_uiOrcPrisonerStage;
					break;
				case 3:
					OpenPrison(m_pInstance->GetData64(DATA_PRISON_CELL_BROGGOK3));
					++m_uiOrcPrisonerStage;
					break;
				case 4:
					if(GameObject* pDoor = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_DOOR_BROGGOK_REAR)))
						pDoor->SetGoState(GO_STATE_ACTIVE);
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_creature->GetMotionMaster()->MovePoint(0, 456.633f, 62.077f, 9.615f);
					m_creature->SetInCombatWithZone();
					++m_uiOrcPrisonerStage;
					break;
				}
				m_uiOrcPrisonerTimer = 30000;
			}
			else m_uiOrcPrisonerTimer -= diff;
		}

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (AcidSpray_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? H_SPELL_SLIME_SPRAY : SPELL_SLIME_SPRAY);
            AcidSpray_Timer = urand(4000, 12000);
        }else AcidSpray_Timer -=diff;

        if (PoisonBolt_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? H_SPELL_POISON_BOLT : SPELL_POISON_BOLT);
            PoisonBolt_Timer = urand(4000, 12000);
        }else PoisonBolt_Timer -=diff;

        if (PoisonSpawn_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_POISON_CLOUD);
            PoisonSpawn_Timer = 20000;
        }else PoisonSpawn_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_broggok_poisoncloudAI : public ScriptedAI
{
    mob_broggok_poisoncloudAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }
    void MoveInLineOfSight(Unit *who) { }
    void AttackStart(Unit *who) { }
};

bool GOHello_go_prison_cell_lever(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (m_pInstance->GetData(TYPE_BROGGOK_EVENT) != DONE)
    {
		m_pInstance->SetData(TYPE_BROGGOK_EVENT, IN_PROGRESS);
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    }

    return false;
}


CreatureAI* GetAI_boss_broggok(Creature* pCreature)
{
    return new boss_broggokAI(pCreature);
}

CreatureAI* GetAI_mob_broggok_poisoncloud(Creature* pCreature)
{
    return new mob_broggok_poisoncloudAI(pCreature);
}

void AddSC_boss_broggok()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_broggok";
    newscript->GetAI = &GetAI_boss_broggok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_broggok_poisoncloud";
    newscript->GetAI = &GetAI_mob_broggok_poisoncloud;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "go_prison_cell_lever";
    newscript->pGOHello = &GOHello_go_prison_cell_lever;
    newscript->RegisterSelf();
}
