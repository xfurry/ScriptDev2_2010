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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    // collosus' abilities
    SPELL_EMERGE                = 54850,
    SPELL_MIGHTY_BLOW           = 54719,
    SPELL_MORTAL_STRIKES        = 54715,
    SPELL_MORTAL_STRIKES_H      = 59454,

    // elemental's abilities
    SPELL_MERGE                 = 54878,
    SPELL_SURGE                 = 54801,
	SPELL_MOJO_PUDDLE           = 55627,
    SPELL_MOJO_PUDDLE_H         = 58994,

	NPC_DRAKKARI_ELEMENTAL      = 29573,
    NPC_LIVING_MOJO             = 29830
};

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMightyBlowTimer;
	uint32 m_uiEmergeTimer;
	bool m_bHasEmerged;
	uint8 m_uiPhase;
    
    void Reset()
    {
        m_uiMightyBlowTimer = 10000;
		m_uiEmergeTimer		= 3000;
		m_bHasEmerged		= false;
		m_uiPhase			= 1;

		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Agrro()
    {
        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MORTAL_STRIKES : SPELL_MORTAL_STRIKES_H);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

	void JustDied(Unit* killer)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
		if(m_uiPhase == 1 && m_creature->GetHealthPercent() < 50.0f)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature,SPELL_EMERGE);
			m_bHasEmerged	= true;
			m_uiEmergeTimer = 3000;
			m_uiPhase		= 2;
			SetCombatMovement(false);
			m_creature->GetMotionMaster()->MoveIdle();
			m_creature->RemoveAllAuras();
			m_creature->AttackStop();
            m_creature->CombatStop(true);
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}

		if(m_uiPhase == 3 && uiDamage > m_creature->GetHealth())
		{
			DoCast(m_creature,SPELL_EMERGE);
			uiDamage		= 0;
			m_uiEmergeTimer = 3000;
			m_bHasEmerged	= true;
		}
    }

    void StartSecondPhase()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_uiPhase = 3;
		SetCombatMovement(true);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiEmergeTimer < uiDiff && m_bHasEmerged)
		{
			if(m_uiPhase == 2)
			{
				if(Creature* pElemental = m_creature->SummonCreature(NPC_DRAKKARI_ELEMENTAL, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000))
                    pElemental->SetInCombatWithZone();
			}
			else if(m_uiPhase == 3)
				m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			m_bHasEmerged = false;
		}
		else m_uiEmergeTimer -= uiDiff;

		if(m_uiPhase == 2)
			return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;
        
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_drakkari_elementalAI : public ScriptedAI
{
    boss_drakkari_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSurgeTimer;
    uint32 m_uiMojoPuddleTimer;
	uint8 m_uiPhase;
	uint32 m_uiMergeTimer;
	bool m_bHasMerged;

    void Reset()
    {
        m_uiSurgeTimer      = 7000;
        m_uiMojoPuddleTimer = 2000;
		m_uiPhase			= 2;
		m_uiMergeTimer		= 3000;
		m_bHasMerged		= false;
		m_creature->SetRespawnDelay(DAY);
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
		if(m_uiPhase == 2 && m_creature->GetHealthPercent() < 50.0f)
		{
			m_creature->InterruptNonMeleeSpells(true);
			//DoCast(m_creature, SPELL_MERGE);
			m_bHasMerged	= true;
			m_uiMergeTimer  = 2000;
			m_uiPhase		= 3;
			SetCombatMovement(false);
			if(Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
				m_creature->GetMotionMaster()->MovePoint(0, pColossus->GetPositionX(), pColossus->GetPositionY(), pColossus->GetPositionZ());
			m_creature->RemoveAllAuras();
            m_creature->CombatStop(true);
			m_creature->AttackStop();
			m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		}
    }

	void JustDied(Unit* killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);

		// TEMP HACK!!!
		if(Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
		{
			if(pColossus->isAlive())
				pColossus->DealDamage(pColossus, pColossus->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		}
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
        if(Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
        {
            if(!pColossus->isAlive())
                pColossus->Respawn();
            else
                pColossus->AI()->EnterEvadeMode();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiMergeTimer < uiDiff && m_bHasMerged)
		{
			if(Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                if(pColossus->isAlive())
                    ((boss_colossusAI*)pColossus->AI())->StartSecondPhase();
            }
			m_creature->SetVisibility(VISIBILITY_OFF);
			m_creature->GetMotionMaster()->MoveIdle();
			m_bHasMerged = false;
		}
		else m_uiMergeTimer -= uiDiff;

		if(m_uiPhase == 3)
		{
			if(Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
			{
				if(!pColossus->isAlive())
				{
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					m_uiPhase = 4;
					SetCombatMovement(true);
					m_creature->SetVisibility(VISIBILITY_ON);
					m_creature->GetMotionMaster()->Clear();
					m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				}
				else
					return;
			}
		}

        if (m_uiSurgeTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SURGE);
            m_uiSurgeTimer = 7000;
        } 
		else m_uiSurgeTimer -= uiDiff;

        if (m_uiMojoPuddleTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);
            m_uiMojoPuddleTimer = 2000;
        } 
		else m_uiMojoPuddleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

CreatureAI* GetAI_boss_drakkari_elemental(Creature* pCreature)
{
    return new boss_drakkari_elementalAI (pCreature);
}

void AddSC_boss_colossus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_colossus";
    pNewScript->GetAI = &GetAI_boss_colossus;
    pNewScript->RegisterSelf();

	pNewScript = new Script;
    pNewScript->Name = "boss_drakkari_elemental";
    pNewScript->GetAI = &GetAI_boss_drakkari_elemental;
    pNewScript->RegisterSelf();
}
