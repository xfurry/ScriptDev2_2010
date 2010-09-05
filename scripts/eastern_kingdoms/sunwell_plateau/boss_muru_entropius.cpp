/* Copyright  2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_muru_entropius
SD%Complete:
SDComment:
SDAuthor:
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"
 
enum
{
	SPELL_NEGATIVE_ENERGY       = 46009,
	SPELL_DARKNESS				= 45996,
	SPELL_SUMMON_BERSERKER		= 46037,
	SPELL_SUMMON_FURY_MAGE		= 46038,
	SPELL_SUMMON_VOID_SENTINEL	= 45988,

	// transition
	SPELL_OPEN_ALL_PORTALS		= 46177,
	SPELL_ENTROPIUS_SUMMON		= 46217,
	SPELL_ENTROPIUS_SPAWN		= 46223,

	// entropius spells
	SPELL_NEGATIVE_ENERGY_ENT	= 46284,
	SPELL_NEGATIVE_ENERGY_DMG	= 46285,
	SPELL_SUMMON_BLACK_HOLE		= 46282,
	SPELL_SUMMON_DARKNESS		= 46269,

    SPELL_ENRAGE                = 26662,

	// black hole
    SPELL_BLACK_HOLE_PASSIVE    = 46228,
	SPELL_VOID_ZONE_PERIODIC	= 46262,
	SPELL_VOID_ZONE_PREEFFECT	= 46265,

    //Dark Fiend
    SPELL_DARK_FIEND_VISUAL     = 45934,
    SPELL_DARK_FIEND_DMG        = 45944,

	// void sentinel
	SPELL_SHADOW_PULSE			= 46086,
    SPELL_VOID_BLAST            = 46161,

	// portals
	SPELL_OPEN_PORTAL_VISUAL	= 45977,
	NPC_PORTAL_TARGET			= 25770,

	NPC_ENTROPIUS				= 25840,
    NPC_SHADOW_FURY_MAGE        = 25799, 
    NPC_SHADOW_BERSERKER        = 25798, 
    NPC_VOID_SENTINEL           = 25772,
	NPC_VOID_SPAWN              = 25824,
	NPC_DARK_FIEND              = 25744, 
    NPC_SINGULARITY             = 25855, 
	NPC_DARKNESS				= 25879,
};
 
//Positional defines 
struct LocationsXY
{
    float x, y, z;
    uint32 id;
};

// Sumoned trash coordinates
static LocationsXY TrashLoc[]=
{
	{1753.964f, 695.484f, 71.19f},	// inside
	{1865.045f, 585.158f, 71.29f},	// outside
};

/*######
## boll_entropius
######*/
struct MANGOS_DLL_DECL boss_entropiusAI : public ScriptedAI
{
    boss_entropiusAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
	    m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
	    Reset(); 
    }

    ScriptedInstance* m_pInstance; 

	uint32 m_uiNegativeEnergyTimer;
	uint32 m_uiBlackHoleTimer;
	uint32 m_uiDarknessTimer;
	uint32 m_uiBerserkTimer;

	void Reset()
	{
		m_creature->SetRespawnDelay(DAY);
		DoCast(m_creature, SPELL_NEGATIVE_ENERGY_ENT);	// not working

		m_uiNegativeEnergyTimer = 1000;
		m_uiBlackHoleTimer		= 15000;
		m_uiDarknessTimer		= 20000;
		m_uiBerserkTimer		= 300000;
	}

	void JustReachedHome()
	{
		if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, NOT_STARTED);

		if(Creature* pMuru = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_MURU)))
			pMuru->Respawn();
		m_creature->ForcedDespawn();
	}

	void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, DONE);
    }

	void UpdateAI(const uint32 uiDiff)
	{ 
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiBerserkTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ENRAGE);
			m_uiBerserkTimer = 60000;
		}
		else m_uiBerserkTimer -= uiDiff;

		if(m_uiNegativeEnergyTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_NEGATIVE_ENERGY_DMG);
			m_uiNegativeEnergyTimer = 1000;
		}
		else m_uiNegativeEnergyTimer -= uiDiff;

		if(m_uiBlackHoleTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				DoCast(pTarget, SPELL_SUMMON_BLACK_HOLE);
			m_uiBlackHoleTimer = 30000;
		}
		else m_uiBlackHoleTimer -= uiDiff;

		if(m_uiDarknessTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
				DoCast(pTarget, SPELL_SUMMON_DARKNESS);
			m_uiDarknessTimer = 20000;
		}
		else m_uiDarknessTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

/*######
## boll_muru
######*/
struct MANGOS_DLL_DECL boss_muruAI : public ScriptedAI
{
    boss_muruAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
	    m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		SetCombatMovement(false);
	    Reset(); 
    }

    ScriptedInstance* m_pInstance; 

    uint32 m_uiSummonTrashTimer;
	bool m_bIsTrashSummoning;
	uint32 m_uiSummonTimer;
	uint32 m_uiTrashCount;
    uint32 m_uiSummonVoidTimer;
    uint32 m_uiDarknessTimer;
	uint32 m_uiDarkFiendTimer;
    uint32 m_uiEnrageTimer;
	uint64 m_uiPortalGUID;
	uint32 m_uiPortalDelayTimer;
	uint32 m_uiEndPhaseTimer;
	uint32 m_uiEndPhaseStage;
	bool m_bIsEndPhase;
    
    void Reset()
    {
        m_uiSummonTrashTimer	= 10000;
		m_bIsTrashSummoning		= false;
		m_uiSummonTimer			= 0;
		m_uiTrashCount			= 0;
        m_uiSummonVoidTimer		= 30000;
        m_uiDarknessTimer		= 45000;
		m_uiDarkFiendTimer		= 50000;
        m_uiEnrageTimer			= 600000;
		m_uiPortalGUID			= 0;
		m_uiPortalDelayTimer	= 60000;
		m_uiEndPhaseTimer		= 1000;
		m_uiEndPhaseStage		= 0;
		m_bIsEndPhase			= false;
    }
    
    void Aggro(Unit* pWho) 
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, IN_PROGRESS);
		DoCast(m_creature, SPELL_NEGATIVE_ENERGY);
    }  

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, DONE);
    }

	void JustReachedHome()
	{
		if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, NOT_STARTED);
	}

	void JustSummoned(Creature* pSummon)
	{
		pSummon->SetInCombatWithZone();
		if(pSummon->GetEntry() == NPC_ENTROPIUS)
		{
			pSummon->CastSpell(pSummon, SPELL_ENTROPIUS_SPAWN, false);
			((boss_entropiusAI*)pSummon->AI())->m_uiBerserkTimer = m_uiEnrageTimer;
		}
	}

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		if(uiDamage > m_creature->GetHealth() || m_creature->GetHealthPercent() < 1.0f)
		{
			uiDamage = 0;
			DoCast(m_creature, SPELL_OPEN_ALL_PORTALS);
			OpenAllPortals();
			m_bIsEndPhase = true;
			m_uiEndPhaseTimer = 5000;
		}
	}

	void OpenAllPortals()
	{
		std::list<Creature*> lPortals;
		GetCreatureListWithEntryInGrid(lPortals, m_creature, NPC_PORTAL_TARGET, 40.0f);
        if (!lPortals.empty())
        {
            for(std::list<Creature*>::iterator iter = lPortals.begin(); iter != lPortals.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->CastSpell((*iter), SPELL_OPEN_PORTAL_VISUAL, false);
            }
        }
	}

	Creature* SelectRandomPortal()
    {
        std::list<Creature* > lPortalList;
		GetCreatureListWithEntryInGrid(lPortalList, m_creature, NPC_PORTAL_TARGET, 40.0f);

        if (lPortalList.empty()){
            m_uiSummonVoidTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lPortalList.begin();
        advance(iter, urand(0, lPortalList.size()-1));

        return *iter;
    }
    
    void UpdateAI(const uint32 uiDiff)
    {       
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiEndPhaseTimer < uiDiff && m_bIsEndPhase)
		{
			switch(m_uiEndPhaseStage)
			{
			case 0:
				DoCast(m_creature, SPELL_ENTROPIUS_SUMMON);
				++m_uiEndPhaseStage;
				m_uiEndPhaseTimer = 2500;
				break;
			case 1:
				m_creature->SetVisibility(VISIBILITY_OFF);
				m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
				++m_uiEndPhaseStage;
				m_uiEndPhaseTimer = 10000;
				break;
			}
		}
		else m_uiEndPhaseTimer -= uiDiff;
        
		if(m_uiEnrageTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_ENRAGE);
			m_uiEnrageTimer = 60000;
		}
		else m_uiEnrageTimer -= uiDiff;

		if(m_bIsEndPhase)
			return;

		if(m_uiDarknessTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_DARKNESS);
			m_uiDarkFiendTimer = 5000;
			m_uiDarknessTimer = 45000;
		}
		else m_uiDarknessTimer -= uiDiff;

		if(m_uiDarkFiendTimer < uiDiff)
		{
			for(int i = 0; i < 8; i++)
            {
                float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
                float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
				if(Creature* pTemp = m_creature->SummonCreature(NPC_DARK_FIEND, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 11000))
					pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
            }
			m_uiDarkFiendTimer = 60000;
		}
		else m_uiDarkFiendTimer -= uiDiff;

		if(m_uiSummonVoidTimer < uiDiff)
		{
			if(Creature* pPortal = SelectRandomPortal())
			{
				m_uiPortalGUID = pPortal->GetGUID();
				pPortal->CastSpell(pPortal, SPELL_OPEN_PORTAL_VISUAL, false);
			}
			m_uiPortalDelayTimer = 5000;
			m_uiSummonVoidTimer = 30000;
		}
		else m_uiSummonVoidTimer -= uiDiff;

		if(m_uiPortalDelayTimer < uiDiff)
		{
			if(Creature* pPortal = m_creature->GetMap()->GetCreature(m_uiPortalGUID))
				m_creature->SummonCreature(NPC_VOID_SENTINEL, pPortal->GetPositionX(), pPortal->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
			m_uiPortalDelayTimer = 60000;
		}
		else m_uiPortalDelayTimer -= uiDiff;

		if(m_bIsTrashSummoning)
		{
			if(m_uiTrashCount < 3)
			{
				if(m_uiSummonTimer < uiDiff)
				{
					for(uint8 i = 0; i < 2; i++)
						m_creature->SummonCreature(m_uiTrashCount == 1 ? NPC_SHADOW_FURY_MAGE : NPC_SHADOW_BERSERKER, TrashLoc[i].x, TrashLoc[i].y, TrashLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
					m_uiSummonTimer = 1000;
					++m_uiTrashCount;
				}
				else m_uiSummonTimer -= uiDiff;
			}
			else
			{
				m_uiTrashCount = 0;
				m_uiSummonTrashTimer = 45000;
				m_bIsTrashSummoning = false;
			}
		}
		else
		{
			if(m_uiSummonTrashTimer < uiDiff)
				m_bIsTrashSummoning = true;
			else m_uiSummonTrashTimer -= uiDiff;
		}
	}
};
 
/*######
## mob_dark_fiend
######*/
struct MANGOS_DLL_DECL mob_dark_fiendAI : public ScriptedAI
{
    mob_dark_fiendAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
		SetCombatMovement(false);
	    Reset(); 
    } 

	uint32 m_uiSpellTimer;

    void Reset() 
    {
		m_uiSpellTimer = 10000;
        DoCast(m_creature, SPELL_DARK_FIEND_VISUAL);
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
	{
		uiDamage = 0;
	}

	void AttackStart(Unit* pWho)
	{
		return;
	}

	void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
	{
		// dispell
		switch(pSpell->Id)
		{
		case 32375:
		case 72734:
		case 32592:
		case 39897:
		case 988:
		case 8012:
			m_creature->ForcedDespawn();
		}
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiSpellTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_DARK_FIEND_DMG);
			m_uiSpellTimer = 10000;
		}
		else m_uiSpellTimer -= uiDiff;
    }
};

/*######
## mob_voidsentinel
######*/
struct MANGOS_DLL_DECL mob_voidsentinelAI : public ScriptedAI
{
    mob_voidsentinelAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{ 
		Reset(); 
	}
 
    uint32 m_uiVoidBlastTimer;

    void Reset() 
    {   
		DoCast(m_creature, SPELL_SHADOW_PULSE);
        m_uiVoidBlastTimer = 15000;
    }
    
    void JustDied(Unit* pKiller) 
    {
        for(uint8 i = 0; i < 8; ++i)
        {
            if(Creature* pTrash = m_creature->SummonCreature(NPC_VOID_SPAWN, m_creature->GetPositionX()+rand()%2, m_creature->GetPositionY()+rand()%2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000))
				pTrash->SetInCombatWithZone();
        }
    }
      
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        if(m_uiVoidBlastTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_VOID_BLAST);
            m_uiVoidBlastTimer = urand(1000, 20000);
        }
		else m_uiVoidBlastTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## mob_singularity
######*/
struct MANGOS_DLL_DECL mob_singularityAI : public ScriptedAI
{
    mob_singularityAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{ 
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->setFaction(14);
		Reset(); 
	}
 
    uint32 m_uiChangeTargetTimer;

    void Reset()                    
    {
		DoCast(m_creature, SPELL_BLACK_HOLE_PASSIVE);
        m_uiChangeTargetTimer = 5000;
    }
      
    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiChangeTargetTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			{
				m_creature->AddThreat(pTarget, 1000.0f);
				m_creature->GetMotionMaster()->MoveChase(pTarget);
				m_creature->Attack(pTarget, true);
			}
			m_uiChangeTargetTimer = 5000;
		}
		else m_uiChangeTargetTimer -= uiDiff;
    }
};

/*######
## mob_darkness
######*/
struct MANGOS_DLL_DECL mob_darknessAI : public ScriptedAI
{
    mob_darknessAI(Creature* pCreature) : ScriptedAI(pCreature) 
	{ 
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		SetCombatMovement(false);
		Reset(); 
	}
 
    uint32 m_uiSummonTimer;
	uint32 m_uiVisualTimer;
	uint32 m_uiDieTimer;

    void Reset()                    
    {
		DoCast(m_creature, SPELL_VOID_ZONE_PREEFFECT);
		m_uiVisualTimer = 3000;
        m_uiSummonTimer = 5000;
		m_uiDieTimer = 16000;
    }

	void AttackStart(Unit* pWho)
	{
		return;
	}
      
    void UpdateAI(const uint32 uiDiff)
    {
		if(m_uiVisualTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_VOID_ZONE_PERIODIC);
			m_uiVisualTimer = 30000;
		}
		else m_uiVisualTimer -= uiDiff;

		if(m_uiSummonTimer < uiDiff)
		{
			m_creature->SummonCreature(NPC_DARK_FIEND, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 11000);
			m_uiSummonTimer = 60000;
		}
		else m_uiSummonTimer -= uiDiff;

		if(m_uiDieTimer < uiDiff)
			m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
		else m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_muru(Creature *pCreature)
{
    return new boss_muruAI(pCreature);
}

CreatureAI* GetAI_boss_entropius(Creature *pCreature)
{
    return new boss_entropiusAI(pCreature);
}
 
CreatureAI* GetAI_mob_dark_fiend(Creature *pCreature)
{
    return new mob_dark_fiendAI(pCreature);
}
 
CreatureAI* GetAI_mob_voidsentinel(Creature *pCreature)
{
    return new mob_voidsentinelAI(pCreature);
}

CreatureAI* GetAI_mob_singularity(Creature *pCreature)
{
    return new mob_singularityAI(pCreature);
}

CreatureAI* GetAI_mob_darkness(Creature *pCreature)
{
    return new mob_darknessAI(pCreature);
}

void AddSC_boss_muru_entropius()
{
    Script *newscript;
        
    newscript = new Script;
    newscript->Name="boss_muru";
    newscript->GetAI = &GetAI_boss_muru;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="boss_entropius";
    newscript->GetAI = &GetAI_boss_entropius;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_dark_fiend";
    newscript->GetAI = &GetAI_mob_dark_fiend;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_voidsentinel";
    newscript->GetAI = &GetAI_mob_voidsentinel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_singularity";
    newscript->GetAI = &GetAI_mob_singularity;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="mob_darkness";
    newscript->GetAI = &GetAI_mob_singularity;
    newscript->RegisterSelf();
}

