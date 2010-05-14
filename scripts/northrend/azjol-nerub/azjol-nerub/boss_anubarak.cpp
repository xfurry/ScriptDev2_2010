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
SDName: Boss_Anubarak
SD%Complete: 20%
SDComment:
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SAY_INTRO                       = -1601014,
    SAY_AGGRO                       = -1601015,
    SAY_KILL_1                      = -1601016,
    SAY_KILL_2                      = -1601017,
    SAY_KILL_3                      = -1601018,
    SAY_SUBMERGE_1                  = -1601019,
    SAY_SUBMERGE_2                  = -1601020,
    SAY_LOCUST_1                    = -1601021,
    SAY_LOCUST_2                    = -1601022,
    SAY_LOCUST_3                    = -1601023,
    SAY_DEATH                       = -1601024, 

    ACHIEV_GOTTA_GO                 = 1860,
};

enum Spells
{
    SPELL_CARRION_BEETLES                  =       53520,
    SPELL_SUMMON_CARRION_BEETLES           =       53521,
    SPELL_LEECHING_SWARM                   =       53467,

    SPELL_IMPALE                           =       53454,
    H_SPELL_IMPALE                         =       59446,

    SPELL_POUND                            =       53472,
    H_SPELL_POUND                          =       59433,

    SPELL_SUBMERGE                         =       53421,
};

enum Creatures
{
    CREATURE_GUARDIAN                      =       29216,
    CREATURE_VENOMANCER                    =       29217,
    CREATURE_DATTER                        =       29213
};

#define SPAWNPOINT_Z                               224.3f

float SpawnPoint[2][2] =
{
    {550.7f, 282.8f},
    {551.1f, 229.4f},
};

#define HOME_X                      551.776978f
#define HOME_Y                      254.246994f

/*######
## boss_anubarak
######*/

struct MANGOS_DLL_DECL boss_anubarakAI : public ScriptedAI
{
    boss_anubarakAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool Channeling;
    bool Summoned_Guardian;
    bool Summoned_Venomancer;
    bool Summoned_Datter;
    uint32 Phase;
    uint32 Phase_Time;

    uint32 AchievTimer;

    uint32 SPELL_CARRION_BEETLES_Timer;
    uint32 SPELL_LEECHING_SWARM_Timer;
    uint32 SPELL_IMPALE_Timer;
    uint32 SPELL_POUND_Timer;
    uint32 SPELL_SUBMERGE_Timer;
    uint32 UNDERGROUND_Timer;
    uint32 VENOMANCER_Timer;
    uint32 DATTER_Timer;

    void Reset()
    {
        SPELL_CARRION_BEETLES_Timer = 8000;
        SPELL_LEECHING_SWARM_Timer = 20000;
        SPELL_IMPALE_Timer = 9000;
        SPELL_POUND_Timer = 15000;

        Phase = 0;
        Phase_Time = 0;
        Channeling = false;

        AchievTimer = 0;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
        
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);

        if (!m_bIsRegularMode && AchievTimer < 240000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_GOTTA_GO);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Achievement counter
        AchievTimer += uiDiff;

        if (Channeling == true)
        {
            for (uint8 i = 0; i < 4; ++i)
                DoCast(m_creature->getVictim(), SPELL_SUMMON_CARRION_BEETLES, true);
            Channeling = false;
        }

        if (Phase == 1)
        {
            if (SPELL_IMPALE_Timer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_IMPALE : H_SPELL_IMPALE) == CAST_OK)
                    SPELL_IMPALE_Timer = 9000;
                }
            } else SPELL_IMPALE_Timer -= uiDiff;

            if (!Summoned_Guardian)
            {
                for (uint8 i = 0; i < 2; ++i)
                {
                    if (Creature *Guardian = m_creature->SummonCreature(CREATURE_GUARDIAN,SpawnPoint[i][0],SpawnPoint[i][1],SPAWNPOINT_Z,0,TEMPSUMMON_CORPSE_DESPAWN,0))
                    {
                        Guardian->AddThreat(m_creature->getVictim(), 0.0f);
                    }
                }
                Summoned_Guardian = true;
            }

            if(!Summoned_Venomancer)
            {
                if (VENOMANCER_Timer <= uiDiff)
                {
                    if (Phase_Time > 1)
                    {
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            if (Creature *Venomancer = m_creature->SummonCreature(CREATURE_VENOMANCER,SpawnPoint[i][0],SpawnPoint[i][1],SPAWNPOINT_Z,0,TEMPSUMMON_CORPSE_DESPAWN,0))
                            {
                                Venomancer->AddThreat(m_creature->getVictim(), 0.0f);
                            }
                        }
                        Summoned_Venomancer = true;
                    }
                } else VENOMANCER_Timer -= uiDiff;
            }

            if(!Summoned_Datter)
            {
                if (DATTER_Timer <= uiDiff)
                {
                    if (Phase_Time > 2)
                    {
                        for (uint8 i = 0; i < 2; ++i)
                        {
                            if (Creature *Datter = m_creature->SummonCreature(CREATURE_DATTER,SpawnPoint[i][0],SpawnPoint[i][1],SPAWNPOINT_Z,0,TEMPSUMMON_CORPSE_DESPAWN,0))
                            {
                                Datter->AddThreat(m_creature->getVictim(), 0.0f);
                            }
                        }
                        Summoned_Datter = true;
                    }
                } else DATTER_Timer -= uiDiff;
            }

            if (UNDERGROUND_Timer <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                Phase = 0;
            } else UNDERGROUND_Timer -= uiDiff;
        }

        if (Phase == 0)
        {
            if (SPELL_LEECHING_SWARM_Timer <= uiDiff)
            {
			switch(urand(0, 2))
                {
                case 0: DoScriptText(SAY_LOCUST_1, m_creature); break;
                case 1: DoScriptText(SAY_LOCUST_2, m_creature); break;
                case 2: DoScriptText(SAY_LOCUST_3, m_creature); break;
                }
                DoCast(m_creature, SPELL_LEECHING_SWARM, true);
                SPELL_LEECHING_SWARM_Timer = 19000;
            } else SPELL_LEECHING_SWARM_Timer -= uiDiff;

            if (SPELL_CARRION_BEETLES_Timer <= uiDiff)
            {
                Channeling = true;
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CARRION_BEETLES : SPELL_CARRION_BEETLES);
                SPELL_CARRION_BEETLES_Timer = 25000;
            } else SPELL_CARRION_BEETLES_Timer -= uiDiff;

            if (SPELL_POUND_Timer <= uiDiff)
            {
                 if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_POUND : H_SPELL_POUND) == CAST_OK)
                    SPELL_POUND_Timer = 16500;
            } else SPELL_POUND_Timer -= uiDiff;
        }

        if ((Phase_Time == 0 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 75)
            || (Phase_Time == 1 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 50)
            || (Phase_Time == 2 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 25))
        {
            ++Phase_Time;

            Summoned_Guardian = false;
            Summoned_Venomancer = false;
            Summoned_Datter = false;

            UNDERGROUND_Timer = 40000;
            VENOMANCER_Timer = 25000;
            DATTER_Timer = 32000;

            DoCast(m_creature, SPELL_SUBMERGE, false);

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);

            Phase = 1;

            switch(urand(0, 1))
                {
                case 0: DoScriptText(SAY_SUBMERGE_1, m_creature); break;
                case 1: DoScriptText(SAY_SUBMERGE_2, m_creature); break;
                }
        }

        if (Phase != 1)
            DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 30)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_anubarak(Creature* pCreature)
{
    return new boss_anubarakAI(pCreature);
}

void AddSC_boss_anubarak()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak";
    newscript->GetAI = &GetAI_boss_anubarak;
    newscript->RegisterSelf();
}
