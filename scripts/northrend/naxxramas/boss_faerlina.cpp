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
SDName: Boss_Faerlina
SD%Complete: 50
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                 = -1533009,
    SAY_AGGRO1                = -1533010,
    SAY_AGGRO2                = -1533011,
    SAY_AGGRO3                = -1533012,
    SAY_AGGRO4                = -1533013,
    SAY_SLAY1                 = -1533014,
    SAY_SLAY2                 = -1533015,
    SAY_DEATH                 = -1533016,

    //SOUND_RANDOM_AGGRO        = 8955,                              //soundId containing the 4 aggro sounds, we not using this

    SPELL_POSIONBOLT_VOLLEY   = 28796,
    H_SPELL_POSIONBOLT_VOLLEY = 54098,
    SPELL_ENRAGE              = 28798,
    H_SPELL_ENRAGE            = 54100,
    SPELL_WIDOWS_EMBRACE       = 28732,
    SPELL_FIREBALL            = 54095,
    SPELL_FIREBALL_H          = 54096,

    SPELL_RAINOFFIRE          = 28794,                       //Not sure if targeted AoEs work if casted directly upon a pPlayer

    NPC_NAXX_FOLLOWER           = 16505,
    NPC_NAXX_WORSHIPER          = 16506,

    ACHIEV_KNOCK_YOU_OUT        = 1997,
    ACHIEV_KNOCK_YOU_OUT_H      = 2140,
};
struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPoisonBoltVolleyTimer;
    uint32 m_uiRainOfFireTimer;
    uint32 m_uiEnrageTimer;
    bool   m_bHasTaunted;

    bool m_bHasFrenzy;

    std::list<Creature*> lFollowers;
    std::list<Creature*> lWorshipers;

    void Reset()
    {
        m_uiPoisonBoltVolleyTimer = 8000;
        m_uiRainOfFireTimer = 16000;
        m_uiEnrageTimer = 60000;

        m_bHasFrenzy = false;

        GetCreatureListWithEntryInGrid(lFollowers, m_creature, NPC_NAXX_FOLLOWER, DEFAULT_VISIBILITY_INSTANCE);
        if (!lFollowers.empty())
        {
            for(std::list<Creature*>::iterator iter = lFollowers.begin(); iter != lFollowers.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }

        GetCreatureListWithEntryInGrid(lWorshipers, m_creature, NPC_NAXX_WORSHIPER, DEFAULT_VISIBILITY_INSTANCE);
        if (!lWorshipers.empty())
        {
            for(std::list<Creature*>::iterator iter = lWorshipers.begin(); iter != lWorshipers.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
            case 3: DoScriptText(SAY_AGGRO4, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            DoScriptText(SAY_GREET, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, DONE);

        if(!m_bHasFrenzy)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_YOU_OUT : ACHIEV_KNOCK_YOU_OUT_H);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Poison Bolt Volley
        if (m_uiPoisonBoltVolleyTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), !m_bIsRegularMode ? H_SPELL_POSIONBOLT_VOLLEY : SPELL_POSIONBOLT_VOLLEY);
            m_uiPoisonBoltVolleyTimer = 11000;
        }
        else
            m_uiPoisonBoltVolleyTimer -= uiDiff;

        // Rain Of Fire
        if (m_uiRainOfFireTimer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_RAINOFFIRE);

            m_uiRainOfFireTimer = 16000;
        }
        else
            m_uiRainOfFireTimer -= uiDiff;

        //Enrage_Timer
        if (m_uiEnrageTimer < uiDiff)
        {
            m_bHasFrenzy = true;
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : H_SPELL_ENRAGE);
            m_uiEnrageTimer = 61000;
        }
        else 
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_worshippersAI : public ScriptedAI
{
    mob_worshippersAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsDead;

    uint32 m_uiFireball_Timer;
    uint32 m_uiDeathDelay_Timer;

    void Reset()
    {
        m_bIsDead = false;
        m_uiFireball_Timer = 0;
        m_uiDeathDelay_Timer = 0;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_bIsDead)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage > m_creature->GetHealth())
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            m_creature->RemoveAllAuras();
            m_creature->AttackStop();

            //DoCast(m_creature, SPELL_WIDOWS_EMBRACE);

            if (m_pInstance)
                if (Creature* pFaerlina = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_FAERLINA))))
                {
                    if (m_creature->GetDistance2d(pFaerlina) <= 5 && pFaerlina->HasAura(m_bIsRegularMode ? H_SPELL_ENRAGE : SPELL_ENRAGE))
                        pFaerlina->RemoveAurasDueToSpell(m_bIsRegularMode ? H_SPELL_ENRAGE : SPELL_ENRAGE);

                    //pFaerlina->CastSpell(pFaerlina, SPELL_WIDOWS_EMBRACE, false);
                }

            m_bIsDead = true;
            m_uiDeathDelay_Timer = 500;

            uiDamage = 0;
            return;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsDead)
            if (m_uiDeathDelay_Timer < uiDiff)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                m_uiDeathDelay_Timer = 0;
            }
            else m_uiDeathDelay_Timer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_bIsDead)
            return;

        if (m_uiFireball_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FIREBALL_H : SPELL_FIREBALL);
            m_uiFireball_Timer = 7000 + rand()%4000;
        }
        else m_uiFireball_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_worshippers(Creature* pCreature)
{
    return new mob_worshippersAI(pCreature);
}

CreatureAI* GetAI_boss_faerlina(Creature* pCreature)
{
    return new boss_faerlinaAI(pCreature);
}

void AddSC_boss_faerlina()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_faerlina";
    NewScript->GetAI = &GetAI_boss_faerlina;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_worshippers";
    NewScript->GetAI = &GetAI_mob_worshippers;
    NewScript->RegisterSelf();
}
