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
SDName: Boss_Garr
SD%Complete: 50
SDComment: Adds NYI
SDCategory: Molten Core
EndScriptData */

#include "precompiled.h"
#include "molten_core.h"

// Garr spells
#define SPELL_ANTIMAGICPULSE        19492
#define SPELL_MAGMASHACKLES         19496
#define SPELL_ENRAGE                19516                   //Stacking enrage (stacks to 10 times)

//Add spells
#define SPELL_ERUPTION              19497
#define SPELL_IMMOLATE              20294

struct MANGOS_DLL_DECL boss_garrAI : public ScriptedAI
{
    boss_garrAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 AntiMagicPulse_Timer;
    uint32 MagmaShackles_Timer;
    uint32 CheckAdds_Timer;
    uint64 Add[8];
    bool Enraged[8];

    void Reset()
    {
        AntiMagicPulse_Timer = 25000;                       //These times are probably wrong
        MagmaShackles_Timer = 15000;
        CheckAdds_Timer = 2000;

        if (m_pInstance && m_pInstance->GetData(TYPE_GARR) != DONE)
			m_pInstance->SetData(TYPE_GARR, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {	
		if (m_pInstance)
			m_pInstance->SetData(TYPE_GARR, IN_PROGRESS);
    }

	void JustDied(Unit* pKiller)
    {
		if (m_pInstance)
			m_pInstance->SetData(TYPE_GARR, DONE);
	}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //AntiMagicPulse_Timer
        if (AntiMagicPulse_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_ANTIMAGICPULSE);
            AntiMagicPulse_Timer = urand(10000, 15000);
        }else AntiMagicPulse_Timer -= diff;

        //MagmaShackles_Timer
        if (MagmaShackles_Timer < diff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MAGMASHACKLES);
            MagmaShackles_Timer = urand(8000, 12000);
        }else MagmaShackles_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_fireswornAI : public ScriptedAI
{
    mob_fireswornAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Immolate_Timer;

    void Reset()
    {
        Immolate_Timer = 4000;                              //These times are probably wrong
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Immolate_Timer
        if (Immolate_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCastSpellIfCan(target,SPELL_IMMOLATE);

            Immolate_Timer = urand(5000, 10000);
        }else Immolate_Timer -= diff;

        //Cast Erruption and let them die
        if (m_creature->GetHealthPercent() <= 10.0f)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ERUPTION);
            m_creature->SetDeathState(JUST_DIED);
            m_creature->RemoveCorpse();
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_garr(Creature* pCreature)
{
    return new boss_garrAI(pCreature);
}

CreatureAI* GetAI_mob_firesworn(Creature* pCreature)
{
    return new mob_fireswornAI(pCreature);
}

void AddSC_boss_garr()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_garr";
    newscript->GetAI = &GetAI_boss_garr;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_firesworn";
    newscript->GetAI = &GetAI_mob_firesworn;
    newscript->RegisterSelf();
}
