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
SDName: Boss_Kurinnaxx
SD%Complete: 100
SDComment: VERIFY SCRIPT AND SQL
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

enum
{
    GO_TRAP                 =   180647,
 
    SPELL_MORTALWOUND       =   25646,
    SPELL_SUMMON_SANDTRAP   =   25648,
    SPELL_SANDTRAP_EFFECT   =   25656,
    SPELL_ENRAGE            =   26527,
    SPELL_SUMMON_PLAYER     =   26446,
    SPELL_WIDE_SLASH        =   25814,
    SPELL_TRASH             =   3391,

    SAY_BREACHED            =   -1509022
};

struct SpawnLocations
{
    float x, y, z, an;
    uint32 id;
};

static SpawnLocations NPCs[]=
{
    {-8873.42f,1647.67f,21.386f,5.69141f, NPC_GENERAL_ANDOROV}
};

struct MANGOS_DLL_DECL boss_kurinnaxxAI : public ScriptedAI
{
    boss_kurinnaxxAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Unit *pTarget;
    uint32 m_uiMortalWound_Timer;
    uint32 m_uiSandTrap_Timer;
    uint32 m_uiTrash_Timer;
    uint32 m_uiWideSlash_Timer;

    bool m_bHasEnraged;
    bool m_bHasSummonedTrap;

    void Reset()
    {
        pTarget = NULL;

        m_uiMortalWound_Timer = 7000;
        m_uiSandTrap_Timer = 30000;
        m_uiTrash_Timer = 10000;
        m_uiWideSlash_Timer = 15000;

        m_bHasEnraged = false;
        m_bHasSummonedTrap = false;
    }

    void Aggro(Unit *who)
    {
        pTarget = who;
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        //tempsummon since ossirian is not created when this event occurs
        if (Unit* pOssirian = m_creature->SummonCreature(NPC_OSSIRIAN,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ() - 40.0f,0,TEMPSUMMON_TIMED_DESPAWN,1000))
            DoScriptText(SAY_BREACHED, pOssirian);

        m_pInstance->SetData(TYPE_KURINNAXX,DONE);

       Creature* pAndorov = m_creature->SummonCreature(NPC_GENERAL_ANDOROV,NPCs[0].x,NPCs[0].y,NPCs[0].z,NPCs[0].an,TEMPSUMMON_CORPSE_DESPAWN,0);
        if (pAndorov)
        {
            pAndorov->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pAndorov->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
     {
        if (!m_bHasEnraged && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 30 && !m_creature->IsNonMeleeSpellCasted(false))
        {
            DoCast(m_creature->getVictim(),SPELL_ENRAGE);
            m_bHasEnraged = true;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //m_uiMortalWound_Timer
        if (m_uiMortalWound_Timer < uiDiff)
         {
            DoCast(m_creature->getVictim(),SPELL_MORTALWOUND);
            m_uiMortalWound_Timer = 30000;
         }
        else 
            m_uiMortalWound_Timer -= uiDiff;

        //Summon our trap and wait 5 seconds
        if (!m_bHasSummonedTrap && m_uiSandTrap_Timer < 10000)
         {
            if (Unit* pUnit = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                pUnit->CastSpell(pUnit,SPELL_SUMMON_SANDTRAP,false);
                m_bHasSummonedTrap = true;
            }
        }

        //if random target gets close to the trap, trap will cast
        if (m_uiSandTrap_Timer < 5000)
         {

            if (Unit* pUnit = SelectUnit(SELECT_TARGET_RANDOM,0))
                if (GameObject *pTrap = GetClosestGameObjectWithEntry(pUnit,GO_TRAP,ATTACK_DISTANCE))
                {
                    DoCast(pUnit, SPELL_SANDTRAP_EFFECT);
                    pTrap->Delete();
                    m_bHasSummonedTrap = false;
                    m_uiSandTrap_Timer = 30000;
                }
        }

        if (m_uiSandTrap_Timer < uiDiff)
        {
            if (GameObject *pTrap = GetClosestGameObjectWithEntry(m_creature,GO_TRAP,DEFAULT_VISIBILITY_DISTANCE))
            {
                m_creature->SendObjectDeSpawnAnim(pTrap->GetGUID());
                pTrap->Delete();
                m_bHasSummonedTrap = false;
                m_uiSandTrap_Timer = 30000;
            }
        }
        else
            m_uiSandTrap_Timer -= uiDiff;

        if (Player* pPlayer = GetPlayerAtMinimumRange(DEFAULT_VISIBILITY_DISTANCE))
            DoCast(pPlayer, SPELL_SUMMON_PLAYER);

        if (m_uiWideSlash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_WIDE_SLASH);
            m_uiWideSlash_Timer = 10000 + rand()%10000;
        }
        else
            m_uiWideSlash_Timer -= uiDiff;

        if (m_uiTrash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_TRASH);
            m_uiTrash_Timer = 10000 + rand()%10000;
        }
        else
            m_uiTrash_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kurinnaxx(Creature* pCreature)
{
    return new boss_kurinnaxxAI(pCreature);
}

void AddSC_boss_kurinnaxx()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "boss_kurinnaxx";
    newscript->GetAI = &GetAI_boss_kurinnaxx;
    newscript->RegisterSelf();
}
