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
SDName: boss_festergut
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // yells
    SAY_AGGRO               = -1609472,
    SAY_GAS_CLOUD           = -1609473,     // by putricide
    SAY_SPORE               = -1609474,
    SAY_PUNGUENT_BLIGHT     = -1609475,
    SAY_KILL1               = -1609476,
    SAY_KILL2               = -1609477,
    SAY_BERSERK             = -1609478,
    SAY_DEATH1              = -1609479,
    SAY_DEATH2              = -1609480,     // by putricide

    // spells
	SPELL_GAS_VISUAL_0			= 69126,	// full cloud
	SPELL_GAS_VISUAL_1			= 69152,
	SPELL_GAS_VISUAL_2			= 69154,	// lesser cloud
	// gas cloud lvl 1:
	SPELL_GASEOUS_BLIGHT_0		= 69157, 
	SPELL_GASEOUS_BLIGHT_1		= 69162, 
	SPELL_GASEOUS_BLIGHT_2		= 69164,

    //SPELL_GASEOUS_BLIGHT_2      = 70468,	// auras: 69164, 69162, 69157, 
    //SPELL_GASEOUS_BLIGHT_1      = 69161,
    //SPELL_GASEOUS_BLIGHT_0      = 70138,
    //SPELL_GASEOUS_BLIGHT_25     = 70140,
    //SPELL_GASEOUS_BLIGHT_25HC   = 70137,
	// others
    SPELL_INHALE_BLIGHT         = 69165,
    SPELL_INHALED_BLIGHT        = 71912,
    SPELL_BERSERK               = 47008,
    SPELL_PUNGENT_BLIGHT_10     = 69195,
    SPELL_PUNGENT_BLIGHT_25     = 71219,
    SPELL_PUNGENT_BLIGHT_10HC   = 73031,
    SPELL_PUNGENT_BLIGHT_25HC   = 73032,
    SPELL_VILE_GAS_10           = 69240,
    SPELL_VILE_GAS_25           = 71218,
    SPELL_VILE_GAS_10HC         = 73019,
    SPELL_VILE_GAS_25HC         = 73020,
	NPC_VILE_GAS_STALKER		= 38548,
    SPELL_GAS_SPORE             = 69278,
    SPELL_GASTRIC_BLOAT_10      = 72219,
    SPELL_GASTRIC_BLOAT_10HC    = 72552,
    SPELL_GASTRIC_BLOAT_25      = 72551,
    SPELL_GASTRIC_BLOAT_25HC    = 72553,
    SPELL_GASTRIC_EXPLOSION_10  = 72227,
    SPELL_GASTRIC_EXPLOSION_10HC= 72229,
    SPELL_GASTRIC_EXPLOSION_25  = 72228,
    SPELL_GASTRIC_EXPLOSION_25HC= 72230,
    SPELL_INOCULATE             = 72103,
    SPELL_VILE_GAS              = 72272,
};

struct MANGOS_DLL_DECL boss_festergutAI : public ScriptedAI
{
    boss_festergutAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiGasSporeTimer;
    uint32 m_uiGastricBloatTimer;
    uint32 m_uiVileGasTimer;
    uint32 m_uiInhaleBlightTimer;
    uint8 m_uiInhaleStage;
	uint8 m_uiCloudStage;
    uint32 m_uiBerserkTimer;
	uint32 m_uiAuraTimer;
	bool m_bHasAura;

    void Reset()
    {
		m_uiAuraTimer			= 1000;
		m_bHasAura				= false;
        m_uiGasSporeTimer       = 20000;
        m_uiGastricBloatTimer   = 15000;
        m_uiVileGasTimer        = 12000;
        m_uiInhaleBlightTimer   = 33000;
        m_uiInhaleStage         = 0;
		m_uiCloudStage			= 0;

        m_uiBerserkTimer        = 300000;  // 5 min 
    }

	void JustReachedHome()
	{
		if(m_pInstance)
            m_pInstance->SetData(TYPE_FESTERGUT, NOT_STARTED);
	}

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (Creature* pPutricide = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_PUTRICIDE)))
            DoScriptText(SAY_GAS_CLOUD, pPutricide);
        // cast gas dmg aura
        DoCast(m_creature, SPELL_GASEOUS_BLIGHT_0);

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_FESTERGUT, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH1, m_creature);
        if (Creature* pPutricide = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_PUTRICIDE)))
            DoScriptText(SAY_DEATH2, pPutricide);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_FESTERGUT, DONE);
    }

    void GastricExplosionCheck()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive())
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    {
                        if(i->getSource()->HasAura(SPELL_GASTRIC_BLOAT_10))
                        {
                            if(i->getSource()->GetAura(SPELL_GASTRIC_BLOAT_10, EFFECT_INDEX_0)->GetStackAmount() == 10)
                                i->getSource()->CastSpell(i->getSource(), SPELL_GASTRIC_EXPLOSION_10, false);
                        }
                    }
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    {
                        if(i->getSource()->HasAura(SPELL_GASTRIC_BLOAT_25))
                        {
                            if(i->getSource()->GetAura(SPELL_GASTRIC_BLOAT_25, EFFECT_INDEX_0)->GetStackAmount() == 10)
                                i->getSource()->CastSpell(i->getSource(), SPELL_GASTRIC_EXPLOSION_25, false);
                        }
                    }
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    {
                        if(i->getSource()->HasAura(SPELL_GASTRIC_BLOAT_10HC)) 
                        {
                            if(i->getSource()->GetAura(SPELL_GASTRIC_BLOAT_10HC, EFFECT_INDEX_0)->GetStackAmount() == 10)
                                i->getSource()->CastSpell(i->getSource(), SPELL_GASTRIC_EXPLOSION_10HC, false);
                        }
                    }
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    {
                        if(i->getSource()->HasAura(SPELL_GASTRIC_BLOAT_25HC))
                        {
                            if(i->getSource()->GetAura(SPELL_GASTRIC_BLOAT_25HC, EFFECT_INDEX_0)->GetStackAmount() == 10)
                                i->getSource()->CastSpell(i->getSource(), SPELL_GASTRIC_EXPLOSION_25HC, false);
                        }
                    }
                }
            }
        } 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiAuraTimer < uiDiff && !m_bHasAura)
		{
			DoCast(m_creature, SPELL_GAS_VISUAL_0);
			m_bHasAura = true;
		}
		else m_uiAuraTimer -= uiDiff;

        if (m_uiGasSporeTimer < uiDiff)
        {
            DoScriptText(SAY_SPORE, m_creature);
            uint8 m_uiMaxPlayers = 0;
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                m_uiMaxPlayers = 2;
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                m_uiMaxPlayers = 3;
            for(uint8 i = 0; i < m_uiMaxPlayers; i++)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target,SPELL_GAS_SPORE);
            }
            m_uiGasSporeTimer = 40000;
        }
        else m_uiGasSporeTimer -= uiDiff;

        if (m_uiGastricBloatTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_GASTRIC_BLOAT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_GASTRIC_BLOAT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_GASTRIC_BLOAT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_GASTRIC_BLOAT_25HC);
            }
            m_uiGastricBloatTimer = 15000;
        }
        else m_uiGastricBloatTimer -= uiDiff;

        if (m_uiVileGasTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				m_creature->SummonCreature(NPC_VILE_GAS_STALKER, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 6000);
            m_uiVileGasTimer = 30000;
        }
        else m_uiVileGasTimer -= uiDiff;

        if (m_uiInhaleBlightTimer < uiDiff)
        {
			m_creature->InterruptNonMeleeSpells(true);
			switch (m_uiInhaleStage)
			{
				// cast inhale blight
			case 0:
				if(m_uiCloudStage != 3)
				{
					DoCast(m_creature, SPELL_INHALE_BLIGHT);
					m_uiInhaleBlightTimer = 3600;
				}
				else
					m_uiInhaleBlightTimer = 100;
				++m_uiInhaleStage;
				break;

				// switch cloud aura
			case 1:
				switch(m_uiCloudStage)
				{
					// full cloud to half cloud
				case 0:
					// remove the 0 inhale aura and cast the 1 inhale aura
					/*if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_0, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_0);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_1);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_25, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_25);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_0);
					}
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_25, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_25);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_0);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_25HC, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_25HC);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_25);
					}*/
					m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_0);
					DoCast(m_creature, SPELL_GASEOUS_BLIGHT_1);
					++m_uiCloudStage;
					break;
					// half cloud to quater cloud
				case 1:
					// remove the 1 inhale aura and cast the 2 inhale aura
					/*if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_1, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_1);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_2);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_0, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_0);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_1);
					}
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_0, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_0);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_1);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_25, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_25);
						DoCast(m_creature, SPELL_GASEOUS_BLIGHT_0);
					}*/
					m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_1);
					DoCast(m_creature, SPELL_GASEOUS_BLIGHT_2);
					++m_uiCloudStage;
					break;
					// quater cloud to no cloud
				case 2:
					/*if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_2, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_2);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_1, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_1);
					}
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_1, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_1);
					}
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					{
						if(m_creature->HasAura(SPELL_GASEOUS_BLIGHT_0, EFFECT_INDEX_0))
							m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_0);
					}*/
					m_creature->RemoveAurasDueToSpell(SPELL_GASEOUS_BLIGHT_2);
					++m_uiCloudStage;
					break;
					// punguent blight
				case 3:
					m_creature->RemoveAurasDueToSpell(SPELL_INHALED_BLIGHT);
					DoScriptText(SAY_PUNGUENT_BLIGHT, m_creature);

					if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
						DoCast(m_creature, SPELL_PUNGENT_BLIGHT_10);
					if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
						DoCast(m_creature, SPELL_PUNGENT_BLIGHT_25);
					if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
						DoCast(m_creature, SPELL_PUNGENT_BLIGHT_10HC);
					if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
						DoCast(m_creature, SPELL_PUNGENT_BLIGHT_25HC);
					m_uiCloudStage = 0;
					break;
				}
				if(m_uiCloudStage == 0)
					m_uiInhaleBlightTimer = 3100;
				else
					m_uiInhaleBlightTimer = 200;
                ++m_uiInhaleStage;
                break;

				// adjust visual cloud
            case 2:
				switch(m_uiCloudStage)
				{
					// set full cloud
				case 0:
					m_creature->RemoveAurasDueToSpell(SPELL_GAS_VISUAL_2);
					DoCast(m_creature, SPELL_GAS_VISUAL_0);
					break;
					// set half cloud
				case 1:
					m_creature->RemoveAurasDueToSpell(SPELL_GAS_VISUAL_0);
					DoCast(m_creature, SPELL_GAS_VISUAL_1);
					// set quater cloud
				case 2:
					m_creature->RemoveAurasDueToSpell(SPELL_GAS_VISUAL_1);
					DoCast(m_creature, SPELL_GAS_VISUAL_2);
					break;
					// set no cloud
				case 3:
					m_creature->RemoveAurasDueToSpell(SPELL_GAS_VISUAL_2);
					break;
				}
				m_uiInhaleBlightTimer = 33000;
                m_uiInhaleStage = 0;
                break;
            }
            
        }
        else m_uiInhaleBlightTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_festergut(Creature* pCreature)
{
    return new boss_festergutAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_vile_gas_stalkerAI : public ScriptedAI
{
	mob_vile_gas_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature) 
	{
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
		Difficulty = pCreature->GetMap()->GetDifficulty();
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pCreature->SetDisplayId(11686);     // make invisible
		pCreature->setFaction(14);
		SetCombatMovement(false);
		Reset();
	}

	ScriptedInstance* m_pInstance;
	uint32 Difficulty;

	void Reset()
	{
		if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
			DoCast(m_creature, SPELL_VILE_GAS_10);
		if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			DoCast(m_creature, SPELL_VILE_GAS_25);
		if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
			DoCast(m_creature, SPELL_VILE_GAS_10HC);
		if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
			DoCast(m_creature, SPELL_VILE_GAS_25HC);
	}

	void UpdateAI(const uint32 uiDiff)
	{
	}
};

CreatureAI* GetAI_mob_vile_gas_stalker(Creature* pCreature)
{
    return new mob_vile_gas_stalkerAI(pCreature);
}

void AddSC_boss_festergut()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_festergut";
    newscript->GetAI = &GetAI_boss_festergut;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "mob_vile_gas_stalker";
    newscript->GetAI = &GetAI_mob_vile_gas_stalker;
    newscript->RegisterSelf();
}