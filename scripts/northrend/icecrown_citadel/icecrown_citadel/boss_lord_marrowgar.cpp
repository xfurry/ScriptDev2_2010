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
SDName: boss_lord_marrowgar
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_INTRO            = -1609392,
    SAY_AGGRO            = -1609393,
    SAY_BONESTORM        = -1609394,
    SAY_BONESPIKE1       = -1609395,
    SAY_BONESPIKE2       = -1609396,
    SAY_BONESPIKE3       = -1609397,
    SAY_KILL1            = -1609398,
    SAY_KILL2            = -1609399,
    SAY_ENRAGE           = -1609401,
    SAY_DEATH            = -1609400,

    SPELL_BONE_SLICE_10     = 69055,
    SPELL_BONE_SLICE_25     = 70814,
    SPELL_COLDFLAME_10      = 69146,
    SPELL_COLDFLAME_25      = 70823,
    SPELL_COLDFLAME_10HC    = 70824,
    SPELL_COLDFLAME_25HC    = 70825,
	SPELL_COLDFLAME_SUMMON	= 69138,
    SPELL_COLDFLAME         = 69147,
	SPELL_COLDFLAME_TRIG	= 69145,
    NPC_COLDFLAME           = 36672,
    SPELL_BONE_SPIKE        = 69057,    // just 1 spell??
    NPC_BONESPIKE           = 38711,
    SPELL_IMPALED           = 69065,
	SPELL_BONE_STORM_AURA	= 69076,
    SPELL_BONE_STORM_10     = 69075,
    SPELL_BONE_STORM_25     = 70834,
    SPELL_BONE_STORM_10HC   = 70835,
    SPELL_BONE_STORM_25HC   = 70836,
    SPELL_BERSERK           = 26662,
};

struct MANGOS_DLL_DECL boss_marrowgarAI : public ScriptedAI
{
    boss_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiSaberLash_Timer;
    uint32 m_uiColdFlame_Timer;
	uint32 m_uiColdFlames;
	uint32 m_uiColdFlameTickTimer;
    uint32 m_uiBoneSpike_Timer;
    uint32 m_uiBoneStorm_Timer;
	uint32 m_uiStormEndTimer;
    uint32 m_uiBerserkTimer;
    bool m_bHasTaunted;
	bool m_bIsColdFlame;

	// coords for coldflame summon
	float m_fTargetX;
	float m_fTargetY;

	float m_fCurrentX;
	float m_fCurrentY;

    void Reset()
    {
        m_uiSaberLash_Timer		= 1000;
        m_uiColdFlame_Timer		= urand(7000, 13000);
        m_uiBoneSpike_Timer		= 15000;
        m_uiBoneStorm_Timer		= 45000;
        m_uiBerserkTimer		= 600000;  // 10 min

		m_bIsColdFlame			= false;
		m_uiColdFlameTickTimer	= 1000;
		m_uiColdFlames			= 0;

		m_fTargetX				= 0;
		m_fTargetY				= 0;
		m_fCurrentX				= 0;
		m_fCurrentY				= 0;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_bHasTaunted)
        {
            DoScriptText(SAY_INTRO, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(who);   
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

	void SummonColdFlame(float fDist)
	{
		// coordonatele de baza
        /*if(m_fCurrentX > m_fTargetX || m_fCurrentY > m_fTargetY)  // to be removed when formula implemented!!!
            fDist = -fDist;
        float tarDist = sqrtf(pow((m_fCurrentX - m_fTargetX), 2) + pow((m_fCurrentY - m_fTargetY), 2));   // distanta dintre boss si target
        float maxSpikes = tarDist/fDist;  // numarul de spikes calculat ca raport intre dinstanta dintre boss is target / distanta dintre spikes

        // definim un triunghi virtual intre boss si axe
		float dYa = sqrtf(pow((m_fCurrentX - m_fCurrentY), 2) + pow((m_fCurrentX - 0), 2)); // distanta de la boss la axa OX
		float dYb = sqrtf(pow((m_fTargetX - m_fTargetY), 2) + pow((m_fTargetX - 0), 2));      // distanta de la target la axa OX
        float dXa = sqrtf(pow((m_fCurrentX - m_fCurrentY), 2) + pow((0 - m_fCurrentY), 2));// distnata de la boss la axa OY
        float dXb = sqrtf(pow((m_fTargetX - m_fTargetY), 2) + pow((0 - m_fTargetY), 2));      // distanta de la target la axa OY
        float dX = dXb - dXa;   // distanta pe X dintre target si boss -> semnul determina directia
        float dY = dYb - dYa;   // distanta pe Y dintre target si boss

        // calculam sin de unghiul dintre segmentul AB si axa ox
        float sinY = dY/tarDist;
        float cosX = dX/tarDist;

        // in functie de sin si cos se calculeaza cat trebuie adaugat pe fiecare axa pentru a obtine coordonatele punctului
        float xAdd = sinY*fDist;
        float yAdd = cosX*fDist;

        // summon
		m_creature->SummonCreature(NPC_COLDFLAME, m_fCurrentX, m_fCurrentX, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
		m_uiColdFlames += 1;

		// add new coords
		m_fCurrentX = m_fCurrentX + xAdd;
		m_fCurrentY = m_fCurrentY + yAdd;*/


		///////////////////////////////////////////////////

		/** mutate in functia principala */
		// distanta dintre boss si target
		float tarDist = sqrtf(pow((m_fCurrentX - m_fTargetX), 2) + pow((m_fCurrentY - m_fTargetY), 2));
		// panta dreptei ce uneste boss-ul cu target-ul
		float m = (m_fTargetY - m_fCurrentY)/(m_fTargetX - m_fCurrentX);
		// pentru a intelege ce se intampla mai jos imaginati-va ca dreapta ce uneste boss-ul cu targetul face parte dintr-un triunghi dreptunghic
		float cateta_alaturata = m_fTargetX - m_fCurrentX;
		float cos = cateta_alaturata/tarDist; // este constant si ne vom folosi de el pentru a calcula urmatoarele pct de spawn in functie de fDist
		/**
		* Am ales ca factor de incrementare 5. Adica am presupus 
		* ca npc-ul NPC_COLDFLAME va fi spawnat din 5 in 5 yards.
		* Totusi mai corect ar fi ca factorul de incrementare sa
		* fie lungimea npc-ului NPC_COLDFLAME. Si-n plus nu sunt
		* sigur daca 1 unitate float = 1 yard??? Probabil ca trebuie
		* adaugat si-un timer.
		*/
		for(float fDist=5; fDist<=50; fDist+=5)
			SummonColdFlame(cos, fDist, m);
		/** mutate in functia principala */
	}

	void SummonColdFlame(float cos, float fDist, float m)
	{
		float cateta_alaturata_calculata = cos * fDist;
		float x_to_spawn = cateta_alaturata_calculata + m_fCurrentX;
		float y_to_spawn = m*(x_to_spawn - m_fCurrentX) + m_fCurrentY;

		// summon
		m_creature->SummonCreature(NPC_COLDFLAME, x_to_spawn, y_to_spawn, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);
	}

	/*
	void SummonColdFlame(float fDist)
	{
		// distanta dintre boss si target
		float tarDist = sqrtf(pow((m_fCurrentX - m_fTargetX), 2) + pow((m_fCurrentY - m_fTargetY), 2));
		// panta dreptei ce uneste boss-ul cu target-ul
		float m = (m_fTargetY - m_fCurrentY)/(m_fTargetX - m_fCurrentX);

		// pentru a intelege ce se intampla mai jos imaginati-va ca dreapta ce uneste boss-ul cu targetul face parte dintr-untriunghi dreptunghic
		float cateta_alaturata = m_fTargetX - m_fCurrentX;
		float cos = cateta_alaturata/tarDist; // este constant si ne vom folosi de el pentru a calcula urmatoarele pct de spawn in functie de fDist
		float cateta_alaturata_calculata = cos * fDist;
		float x_to_spawn = cateta_alaturata_calculata + m_fCurrentX;
		float y_to_spawn = m*x_to_spawn - m*m_fCurrentX + m_fCurrentY;

		// summon
		m_creature->SummonCreature(NPC_COLDFLAME, x_to_spawn, y_to_spawn, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 5000);

		// add new coords
		m_fCurrentX = x_to_spawn;
		m_fCurrentY = y_to_spawn;
	}*/

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		// berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_ENRAGE, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_uiBoneStorm_Timer < uiDiff)
        {
            DoScriptText(SAY_BONESTORM, m_creature);
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_BONE_STORM_AURA);
            m_uiBoneStorm_Timer = 90000;
			m_uiStormEndTimer	= 23000;
			m_uiColdFlame_Timer = 5000;
			//m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, IMMUNE_TO_SILENCE_AND_STUN_AND_FEAR_MASK, true);
        }
        else m_uiBoneStorm_Timer -= uiDiff;

		if (m_uiStormEndTimer < uiDiff && m_creature->HasAura(SPELL_BONE_STORM_AURA, EFFECT_INDEX_0))
			m_creature->RemoveAurasDueToSpell(SPELL_BONE_STORM_AURA);
		else m_uiStormEndTimer -= uiDiff;

		// start the line of flame
        if (m_uiColdFlame_Timer < uiDiff)
        {
			//DoCast(m_creature, SPELL_COLDFLAME_SUMMON);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
				m_fTargetX = pTarget->GetPositionX();
				m_fTargetY = pTarget->GetPositionY();
				m_fCurrentX = m_creature->GetPositionX();
				m_fCurrentY = m_creature->GetPositionY();
				SummonColdFlame(5.0f);
				m_uiColdFlames			= 0;
				m_uiColdFlameTickTimer	= 1000;
				m_bIsColdFlame			= true;
			}
			if(m_creature->HasAura(SPELL_BONE_STORM_AURA, EFFECT_INDEX_0))
				m_uiColdFlame_Timer = 1000;
			else
				m_uiColdFlame_Timer	= urand(8000, 10000);
        }
        else m_uiColdFlame_Timer -= uiDiff;

		// summon a line of flame periodicaly
		if(m_uiColdFlameTickTimer < uiDiff && m_bIsColdFlame && m_uiColdFlames < 10)
		{
			//SummonColdFlame(10.0f);
			if(m_creature->HasAura(SPELL_BONE_STORM_AURA, EFFECT_INDEX_0))
				m_uiColdFlameTickTimer = 500;
			else
				m_uiColdFlameTickTimer = 1000;
		}
		else m_uiColdFlameTickTimer -= uiDiff;

		// stop summning cold flame
		if(m_uiColdFlames == 10 && m_bIsColdFlame)
		{
			m_uiColdFlames = 0;
			m_bIsColdFlame = false;
		}

		// return in bonestorm
		if(m_creature->HasAura(SPELL_BONE_STORM_AURA))
			return;

        if (m_uiBoneSpike_Timer < uiDiff)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_BONESPIKE1, m_creature); break;
                case 1: DoScriptText(SAY_BONESPIKE2, m_creature); break;
                case 2: DoScriptText(SAY_BONESPIKE3, m_creature); break;
            }

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                DoCast(pTarget, SPELL_BONE_SPIKE);
				m_creature->SummonCreature(NPC_BONESPIKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            }
            m_uiSaberLash_Timer = 4000;
            m_uiBoneSpike_Timer = 18000;
        }
        else m_uiBoneSpike_Timer -= uiDiff;

        if (m_uiSaberLash_Timer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_25);
            m_uiSaberLash_Timer = urand(1000, 2000);
        }
        else m_uiSaberLash_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_marrowgar(Creature* pCreature)
{
    return new boss_marrowgarAI(pCreature);
}

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		pCreature->SetDisplayId(11686);     // make invisible
		pCreature->setFaction(14);
		SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *m_pInstance;

    void Reset()
	{
		DoCast(m_creature, SPELL_COLDFLAME_TRIG);
	}

    void AttackStart(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint64 m_uiVictimGUID;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        DoCast(pWho, SPELL_IMPALED);
        m_uiVictimGUID = pWho->GetGUID();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
            pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);

        if (Killer)
            Killer->RemoveAurasDueToSpell(SPELL_IMPALED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
            m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

void AddSC_boss_marrowgar()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_marrowgar";
    NewScript->GetAI = &GetAI_boss_marrowgar;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_coldflame";
    NewScript->GetAI = &GetAI_mob_coldflame;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_bone_spike";
    NewScript->GetAI = &GetAI_mob_bone_spike;
    NewScript->RegisterSelf();
}

