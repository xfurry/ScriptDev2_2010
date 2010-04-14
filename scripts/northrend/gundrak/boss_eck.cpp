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
SDName: Boss_Eck
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum Spells
{
    SPELL_ECK_BERSERK                       = 55816, //Eck goes berserk, increasing his attack speed by 150% and all damage he deals by 500%.
    SPELL_ECK_BITE                          = 55813, //Eck bites down hard, inflicting 150% of his normal damage to an enemy.
    SPELL_ECK_SPIT                          = 55814, //Eck spits toxic bile at enemies in a cone in front of him, inflicting 2970 Nature damage and draining 220 mana every 1 sec for 3 sec.
    SPELL_ECK_SPRING_1                      = 55815, //Eck leaps at a distant target.  --> Drops aggro and charges a random player. Tank can simply taunt him back.
    SPELL_ECK_SPRING_2                      = 55837,  //Eck leaps at a distant target.
    SPELL_ECK_RESIDUE                       = 55817
};

/*######
## boss_eck
######*/

struct MANGOS_DLL_DECL boss_eckAI : public ScriptedAI
{
    boss_eckAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiBerserkTimer;
    uint32 uiBiteTimer;
    uint32 uiSpitTimer;
    uint32 uiSpringTimer;
    bool bBerserk;

    void Reset()
    {
        uiBerserkTimer = 60000 + rand()%30000; //60-90 secs according to wowwiki
        uiBiteTimer = 5000;
        uiSpitTimer = 10000;
        uiSpringTimer = 8000;

        bBerserk = false;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_ECK, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_ECK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiBiteTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ECK_BITE);
            uiBiteTimer = 8000 + rand()%4000;
        } else uiBiteTimer -= uiDiff;

        if (uiSpitTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ECK_SPIT);
            uiSpitTimer = 8000 + rand()%8000;
        } else uiSpitTimer -= uiDiff;

        if (uiSpringTimer <= uiDiff)
        {
            Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1);
            if(pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
            {
                switch(urand(0, 1))
                {
                    case 0: DoCast(pTarget, SPELL_ECK_SPRING_1); break;
                    case 1: DoCast(pTarget, SPELL_ECK_SPRING_2); break;
                }
                uiSpringTimer = 5000 + rand()%10000;
            }
        } else uiSpringTimer -= uiDiff;

        //Berserk on timer or 20% of health
        if (!bBerserk)
        {
            if (uiBerserkTimer <= uiDiff)
            {
                DoCast(m_creature, SPELL_ECK_BERSERK);
                bBerserk = true;
            }
            else
            {
                uiBerserkTimer -= uiDiff;
                if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 20)
                {
                    DoCast(m_creature, SPELL_ECK_BERSERK);
                    bBerserk = true;
                }
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eck(Creature* pCreature)
{
    return new boss_eckAI(pCreature);
}

void AddSC_boss_eck()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_eck";
    newscript->GetAI = &GetAI_boss_eck;
    newscript->RegisterSelf();
}
