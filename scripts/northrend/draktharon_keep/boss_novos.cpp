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
SDName: Boss_Novos
SD%Complete: 20%
SDComment:
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_AGGRO                       = -1600005,
    SAY_DEATH                       = -1600006,
    SAY_KILL                        = -1600007,
    SAY_ADDS                        = -1600008,
    SAY_BUBBLE_1                    = -1600009,
    SAY_BUBBLE_2                    = -1600010,

    EMOTE_ASSISTANCE                = -1600011,

    POS                                 = 3,
    
    SPELL_ARCANE_FIELD                  = 47346,
    SPELL_FROSTBOLT                     = 49037,
    H_SPELL_FROSTBOLT                   = 59855,
    SPELL_ARCANE_BLAST                  = 49198,
    H_SPELL_ARCANE_BLAST                = 59909,
    SPELL_BLIZZARD                      = 49034,
    H_SPELL_BLIZZARD                    = 59854,
    SPELL_WRATH_OF_MISERY               = 50089,
    H_SPELL_WRATH_OF_MISERY             = 59856,

    SPELL_RITUAL_CRYSTAL_KEY            = 51404,
    SPELL_EFFECT                        = 52106,
    SPELL_DEAD_EFFECT                   = 47336,

    SPELL_SHADOW_BOLT                   = 51363,
    H_SPELL_SHADOW_BOLT                 = 59016,

    ACHIEV_OH_NOVOS                     = 2057,
};

const float AddSpawnPoint[3] = { -379.20f, -816.76f, 59.70f };
const float CrystalHandlerSpawnPoint[3] = { -314.00f, -699.17f, 27.2914f };
const float AddDestinyPoint[3] = { -379.314545f, -770.577637f, 28.58837f };

bool m_bIsAchiev;   // needed for achiev

/*######
## boss_novos
######*/

struct MANGOS_DLL_DECL boss_novosAI : public ScriptedAI
{
    boss_novosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool Phase1;
    bool Phase2;
    uint32 Start_Check;
    uint32 Handler_Spawn;
    uint32 Handler_Count;
    uint32 Cast_Timer;
    uint32 ArcaneCount;
    uint32 SpecialCast;
    uint32 SummonMinion_Timer;
    uint32 m_uiWaveTimer;

    std::list<uint64> luiCrystals;

    void Reset()
    {
        Start_Check = 1;
        Handler_Spawn = 5000;
        Handler_Count = 0;
        ArcaneCount = 0;
        SpecialCast = 0;
        Phase1 = false;
        Phase2 = false;

        m_uiWaveTimer   = 1000;
        m_bIsAchiev     = true;

        luiCrystals.clear();

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_NOVOS, NOT_STARTED);
            luiCrystals.push_back(m_pInstance->GetData64(TYPE_CRYSTAL_1));
            luiCrystals.push_back(m_pInstance->GetData64(TYPE_CRYSTAL_2));
            luiCrystals.push_back(m_pInstance->GetData64(TYPE_CRYSTAL_3));
            luiCrystals.push_back(m_pInstance->GetData64(TYPE_CRYSTAL_4));
            for (std::list<uint64>::const_iterator itr = luiCrystals.begin(); itr != luiCrystals.end(); ++itr)
            {
                if (GameObject* pTemp = m_pInstance->instance->GetGameObject(*itr))
                    pTemp->SetGoState(GO_STATE_READY);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_NOVOS, IN_PROGRESS);

            for (std::list<uint64>::const_iterator itr = luiCrystals.begin(); itr != luiCrystals.end(); ++itr)
            {
                if (GameObject *pTemp = m_pInstance->instance->GetGameObject(*itr))
                    pTemp->SetGoState(GO_STATE_ACTIVE);
            }
        }

        m_creature->SummonCreature(NPC_CRYSTAL_CHANNEL_TARGET, -379.269f, -737.728f, 39.313f, 0 , TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
        m_creature->CallForHelp(50.0f);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_NOVOS, DONE);

        if(!m_bIsRegularMode && m_bIsAchiev)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_OH_NOVOS);
        }
    }

    void EnterPhase1()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        Phase1 = true;
    }

    void EnterPhase2()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
        SummonMinion_Timer = urand (15000,20000);
        Phase1 = false;
        Phase2 = true;
    }

    void RemoveCrystal()
    {
        if (!luiCrystals.empty())
        {
            if (m_pInstance)
                if (GameObject *pTemp = m_pInstance->instance->GetGameObject(luiCrystals.back()))
                    pTemp->SetGoState(GO_STATE_READY);
            luiCrystals.pop_back();
        }
        if (luiCrystals.empty())
        {
            m_creature->CastStop();
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            EnterPhase2();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Phase2 == true)
        {
               if (Cast_Timer < uiDiff)
            {     
                if (ArcaneCount == 2)
                {
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_ARCANE_BLAST : H_SPELL_ARCANE_BLAST);
                    ArcaneCount = 0;
                }

                if (ArcaneCount < 2)
                {
                    if (SpecialCast == 3)
                    {
                        switch(urand(0, 1))
                        {
                            case 0: 
                                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_WRATH_OF_MISERY : H_SPELL_WRATH_OF_MISERY);
                            case 1: 
                                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_BLIZZARD : H_SPELL_BLIZZARD);
                        }
                        SpecialCast = 0;
                    }
                    if (SpecialCast < 3)
                    {
                        DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0),  m_bIsRegularMode ? SPELL_FROSTBOLT : H_SPELL_FROSTBOLT);
                        SpecialCast ++;
                    }
                    ArcaneCount ++;
                }
                Cast_Timer = 1000;
            }else Cast_Timer -= uiDiff;
        }

        if ( m_creature->getVictim() && Start_Check == 1)
        {
            EnterPhase1();
            DoCast(m_creature, SPELL_ARCANE_FIELD);
            Start_Check = 0;
        }
        
        if(!m_bIsRegularMode && Phase2)
        {
            if (SummonMinion_Timer < uiDiff )
            {    
                for(uint8 i = 0; i < 5; i++)
                {
                    if (Creature* pAdd = m_creature->SummonCreature(NPC_FETID_TROLL_CORPSE, m_creature->GetPositionX() + urand(0, 3), m_creature->GetPositionY() + urand(0, 3),m_creature->GetPositionZ(),0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        pAdd->AI()->AttackStart(m_creature->getVictim());
                }
                SummonMinion_Timer = urand (25000,30000);
            }else SummonMinion_Timer -= uiDiff;
        }
        
        if (Handler_Spawn < uiDiff && Phase1 == true)
        {    
            Handler_Count ++;
            if(Handler_Count < 5)
            {
                if(Creature* pAdd = m_creature->SummonCreature(NPC_CRYSTAL_HANDLER, CrystalHandlerSpawnPoint[0],CrystalHandlerSpawnPoint[1],CrystalHandlerSpawnPoint[2],0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    pAdd->GetMotionMaster()->MovePoint(0, AddDestinyPoint[0], AddDestinyPoint[1], AddDestinyPoint[2]);
                Handler_Spawn = urand(20000, 30000);
            }
            if(Handler_Count == 5)
            {            
                EnterPhase2();
                if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
                    pTrigger->ForcedDespawn();
                Phase2 = true;
                Cast_Timer = 500;
            }
        }else Handler_Spawn -= uiDiff;

        if (m_uiWaveTimer < uiDiff && Phase1 == true)
        {
            uint32 m_uiSummonEntry = 0;
            switch (urand(0, 2))
            {
                case 0:
                    m_uiSummonEntry = NPC_HULKING_CORPSE;
                    break;
                case 1:
                    m_uiSummonEntry = NPC_RISEN_SHADOWCASTER;
                    break;
                case 2:
                    m_uiSummonEntry = NPC_FETID_TROLL_CORPSE;
                    break;
            }
            if (Creature* pAdd = m_creature->SummonCreature(m_uiSummonEntry, AddSpawnPoint[0],AddSpawnPoint[1],AddSpawnPoint[2],0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                pAdd->GetMotionMaster()->MovePoint(0, AddDestinyPoint[0], AddDestinyPoint[1], AddDestinyPoint[2]);

            m_uiWaveTimer = 3000;
        }else m_uiWaveTimer -= uiDiff;
    }
};

enum CrystalHandlerSpells
{
    SPELL_FLASH_OF_DARKNESS                       = 49668,
    H_SPELL_FLASH_OF_DARKNESS                     = 59004
};

struct MANGOS_DLL_DECL crystal_handlerAI : public ScriptedAI
{
    crystal_handlerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 uiFlashOfDarknessTimer;

    void Reset()
    {
        uiFlashOfDarknessTimer = 5000;
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (Unit* pNovos = GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f))
            m_creature->AI()->AttackStart(pNovos->getVictim());
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiFlashOfDarknessTimer <= diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FLASH_OF_DARKNESS : H_SPELL_FLASH_OF_DARKNESS);
            uiFlashOfDarknessTimer = 5000;
        } else uiFlashOfDarknessTimer -= diff;
    }

    void JustDied()
    {
        if (Creature* pDeadTrigger = m_creature->SummonCreature(NPC_TRIGGER_TARGET,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000))
        {
            if (Unit* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
            {
                pDeadTrigger->CastSpell(pTarget, SPELL_DEAD_EFFECT, true);
            }
        }
        if (Creature* pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL, 85.0f))
            pTrigger->ForcedDespawn();

        if (Creature* pNovos = GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f))
            ((boss_novosAI*)pNovos->AI())->RemoveCrystal();
    }
};

struct MANGOS_DLL_DECL crystal_channelAI : public ScriptedAI
{
    crystal_channelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 Check_Timer;

    void Reset()
    {
        Check_Timer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (Check_Timer < uiDiff)
        {
            Creature* pNovos =  GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f);
            if (Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_CRYSTAL_CHANNEL_TARGET , 85.0f))
                if (((boss_novosAI*)pNovos->AI())->Phase1 == true)
                    DoCast(pTarget, SPELL_EFFECT, true);
                else
                    pTarget->ForcedDespawn();
            Check_Timer = 1000;
        }else Check_Timer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_novos_minionAI : public ScriptedAI
{
    mob_novos_minionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 Check_Timer;
    uint32 creatureEntry;

    void Reset()
    {
        Check_Timer = 1000;
        creatureEntry = m_creature->GetEntry();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // achiev check
        if (Creature* pNovos = GetClosestCreatureWithEntry(m_creature, NPC_NOVOS, 85.0f))
            if(((boss_novosAI*)pNovos->AI())->Phase1 == true && ((boss_novosAI*)pNovos->AI())->Phase2 == false)
                if(m_bIsAchiev && m_creature->GetPositionY() > -770.0f)
                    m_bIsAchiev = false;

        switch (creatureEntry)
        {
        case NPC_RISEN_SHADOWCASTER:
            if (Check_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
                Check_Timer = 1000;
            }else Check_Timer -= uiDiff;
            break;
        default:
            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_novos(Creature* pCreature)
{
    return new boss_novosAI(pCreature);
}

CreatureAI* GetAI_mob_novos_minionAI(Creature* pCreature)
{
    return new mob_novos_minionAI(pCreature);
}

CreatureAI* GetAI_crystal_handler(Creature* pCreature)
{
    return new crystal_handlerAI(pCreature);
}

CreatureAI* GetAI_crystal_channel(Creature* pCreature)
{
    return new crystal_channelAI(pCreature);
}

void AddSC_boss_novos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_novos";
    newscript->GetAI = &GetAI_boss_novos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "crystal_handler";
    newscript->GetAI = &GetAI_crystal_handler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "crystal_channel";
    newscript->GetAI = &GetAI_crystal_channel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_novos_minionAI";
    newscript->GetAI = &GetAI_mob_novos_minionAI;
    newscript->RegisterSelf();
}
