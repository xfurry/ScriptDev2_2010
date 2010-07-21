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
	SPELL_BONE_STOMR_AURA	= 69076,
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
    uint32 m_uiBoneSpike_Timer;
    uint32 m_uiBoneStorm_Timer;
    uint32 m_uiBerserkTimer;
    bool m_bHasTaunted;

    void Reset()
    {
        m_uiSaberLash_Timer = 1000;
        m_uiColdFlame_Timer = 15000;
        m_uiBoneSpike_Timer = 15000;
        m_uiBoneStorm_Timer = 45000;
        m_uiBerserkTimer    = 600000;  // 10 min
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

	void SummonColdFlame()
	{
		// coordonatele de baza
        /*float startX = m_creature->GetPositionX();  // pozitia X a bossului
        float startY = m_creature->GetPositionY();  // pozitia Y a bossului
        float endX = target->GetPositionX();        // pozitia X a targetului
        float endY = target->GetPositionY();        // pozitia Y a targetului
        float localZ = m_creature->GetPositionZ();  // pozitia pe axa Z
        float defDist = 10.0f;              // distanta definita dintre spikes
        if(startX > endX || startY > endY)  // to be removed when formula implemented!!!
            defDist = -10.0f;
        float tarDist = sqrtf(pow((startX - startY), 2) + pow((endX - endY), 2));   // distanta dintre boss si target
        float maxSpikes = tarDist/defDist;  // numarul de spikes calculat ca raport intre dinstanta dintre boss is target / distanta dintre spikes

        // definim un triunghi virtual intre boss si axe
        float dYa = sqrtf(pow((startX - startY), 2) + pow((startX - 0), 2)); // distanta de la boss la axa OX
        float dYb = sqrtf(pow((endX - endY), 2) + pow((endX - 0), 2));      // distanta de la target la axa OX
        float dXa = sqrtf(pow((startX - startY), 2) + pow((0 - startY), 2));// distnata de la boss la axa OY
        float dXb = sqrtf(pow((endX - endY), 2) + pow((0 - endY), 2));      // distanta de la target la axa OY
        float dX = dXb - dXa;   // distanta pe X dintre target si boss -> semnul determina directia
        float dY = dYb - dYa;   // distanta pe Y dintre target si boss

        // calculam sin de unghiul dintre segmentul AB si axa ox
        float sinY = dY/tarDist;
        float cosX = dX/tarDist;

        // in functie de sin si cos se calculeaza cat trebuie adaugat pe fiecare axa pentru a obtine coordonatele punctului
        float xAdd = sinY*defDist;
        float yAdd = cosX*defDist;

        // summon
        for(uint8 i = 0; i < (uint8)maxSpikes; i++)
        {
            //if(Creature* pSpike = m_creature->SummonCreature(NPC_SPIKE, startX + i*xAdd, startY + i*yAdd, localZ, 0, TEMPSUMMON_TIMED_DESPAWN, 1000))
            if(Creature* pSpike = m_creature->SummonCreature(NPC_SPIKE, startX + i*defDist, startY + i*defDist, localZ, 0, TEMPSUMMON_TIMED_DESPAWN, 1000))
            {
                pSpike->AddThreat(target, 0.0f);
                if(pSpike->HasAura(SPELL_PERMAFROST, EFFECT_INDEX_0))
                    return;
            }
        }*/
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBoneStorm_Timer < uiDiff)
        {
            DoScriptText(SAY_BONESTORM, m_creature);
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_BONE_STOMR_AURA);
            m_uiBoneStorm_Timer = 90000;
        }
        else m_uiBoneStorm_Timer -= uiDiff;

        if (m_uiBoneSpike_Timer < uiDiff)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_BONESPIKE1, m_creature); break;
                case 1: DoScriptText(SAY_BONESPIKE2, m_creature); break;
                case 2: DoScriptText(SAY_BONESPIKE3, m_creature); break;
            }

            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_BONE_SPIKE);
				m_creature->SummonCreature(NPC_BONESPIKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
            }
            m_uiSaberLash_Timer = 4000;
            m_uiBoneSpike_Timer = 20000;
        }
        else m_uiBoneSpike_Timer -= uiDiff;

        if (m_uiColdFlame_Timer < uiDiff)
        {
			//DoCast(m_creature, SPELL_COLDFLAME_SUMMON);
			SummonColdFlame();
			// fix this!!
            /*if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                float fPosX, fPosY, fPosZ;
                target->GetPosition(fPosX, fPosY, fPosZ);

                m_creature->SummonCreature(NPC_COLDFLAME, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            }*/
            m_uiColdFlame_Timer = 15000;
        }
        else m_uiColdFlame_Timer -= uiDiff;

        if (m_uiSaberLash_Timer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_25);
            m_uiSaberLash_Timer = urand(2000, 4000);
        }
        else m_uiSaberLash_Timer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_ENRAGE, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

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

