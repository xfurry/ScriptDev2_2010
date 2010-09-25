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
SDName: Boss_Anubrekhan
SD%Complete: 70
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                   = -1533000,
    SAY_AGGRO1                  = -1533001,
    SAY_AGGRO2                  = -1533002,
    SAY_AGGRO3                  = -1533003,
    SAY_TAUNT1                  = -1533004,
    SAY_TAUNT2                  = -1533005,
    SAY_TAUNT3                  = -1533006,
    SAY_TAUNT4                  = -1533007,
    SAY_SLAY                    = -1533008,

    SPELL_IMPALE                = 28783,                    //May be wrong spell id. Causes more dmg than I expect
    SPELL_IMPALE_H              = 56090,
    SPELL_LOCUSTSWARM           = 28785,                    //This is a self buff that triggers the dmg debuff
    SPELL_LOCUSTSWARM_H         = 54021,

    //spellId invalid
    SPELL_SUMMONGUARD           = 29508,                    //Summons 1 crypt guard at targeted location

    SPELL_SELF_SPAWN_5          = 29105,                    //This spawns 5 corpse scarabs ontop of us (most likely the pPlayer casts this on death)
    SPELL_SELF_SPAWN_10         = 28864,                    //This is used by the crypt guards when they die

    NPC_CRYPT_GUARD             = 16573,
    NPC_CORPSE_SCARAB           = 16698,
};

static const uint32 MAX_CRYPT_GUARDS = 8;

struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiImpaleTimer;
    uint32 m_uiLocustSwarmTimer;
    uint32 m_uiSummonTimer;
    bool   m_bHasTaunted;

    uint32 RiseFromCorpse_Timer;

    uint64 guidCryptGuards[MAX_CRYPT_GUARDS];
    uint32 CryptGuard_count;

    bool swarm;                        //is active spell LocustSwarm or not, when active he cannot use other spels

    void Reset()
    {
        m_uiImpaleTimer = 15000;                            // 15 seconds
        m_uiLocustSwarmTimer = urand(80000, 120000);        // Random time between 80 seconds and 2 minutes for initial cast
        m_uiSummonTimer = m_uiLocustSwarmTimer + 45000;     // 45 seconds after initial locust swarm

        swarm = false;
        CryptGuard_count = 0;

        // Remove all Crypt Guards
        for (int i = 0; i < MAX_CRYPT_GUARDS; i++)
        {
            //delete creature
            Unit* pUnit = m_creature->GetMap()->GetUnit(guidCryptGuards[i]);
            if (pUnit)
                pUnit->AddObjectToRemoveList();
            guidCryptGuards[i] = 0;
        }
        
        //Remove all corpse scarabs
        /*std::list<Creature*> CorpseScarabs = GetCreaturesByEntry(NPC_CORPSE_SCARAB);
        if (!CorpseScarabs.empty())
            for(std::list<Creature*>::iterator itr = CorpseScarabs.begin(); itr != CorpseScarabs.end(); ++itr)
                (*itr)->AddObjectToRemoveList();*/

        //if anubrekhan is alive -> this must be first time we entered Arachnid Quarter -> close all other doors
    }

    void KilledUnit(Unit* pVictim)
    {
        //Force the player to spawn corpse scarabs via spell
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, true);

        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            switch(urand(0, 4))
            {
                case 0: DoScriptText(SAY_GREET, m_creature); break;
                case 1: DoScriptText(SAY_TAUNT1, m_creature); break;
                case 2: DoScriptText(SAY_TAUNT2, m_creature); break;
                case 3: DoScriptText(SAY_TAUNT3, m_creature); break;
                case 4: DoScriptText(SAY_TAUNT4, m_creature); break;
            }
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    bool IsVisible(Unit* who) const
    {
        if (!who)
            return false;
        return m_creature->IsWithinDistInMap(who, 100.0f);
    }

    /*std::list<Creature*> GetCreaturesByEntry(uint32 entry)
    {
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        std::list<Creature*> creatureList;

        AllCreaturesOfEntryInRange check(m_creature, entry, 100);
        MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange> searcher(m_creature, creatureList, check);
        TypeContainerVisitor<MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

        cell.Visit(pair, visitor, *(m_creature->GetMap()));

        return creatureList;
    }*/

    void JustSummoned(Creature* temp) 
    {
        if (!temp)
            return;

        //Summoned Crypt Guard will target random player
        guidCryptGuards[CryptGuard_count++] = temp->GetGUID();
        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
        {
            temp->AddThreat(target);
            m_creature->SetInCombatWithZone();
        }

        switch (rand()%4)
        {
            case 0: DoScriptText(SAY_TAUNT1, m_creature); break;
            case 1: DoScriptText(SAY_TAUNT2, m_creature); break;
            case 2: DoScriptText(SAY_TAUNT3, m_creature); break;
            case 3: DoScriptText(SAY_TAUNT4, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //SumonFirstCryptGuard_Timer
        if (m_uiSummonTimer< uiDiff)
        {
            if (CryptGuard_count < MAX_CRYPT_GUARDS)
                DoSpawnCreature(NPC_CRYPT_GUARD,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000);
            m_uiSummonTimer = 2000000;
        }else m_uiSummonTimer -= uiDiff;

        /*RiseFromCorpse_Timer
        if (RiseFromCorpse_Timer < uiDiff)
        {
            RiseFromCorpse_Timer = 60000 + (rand()%10000);
            std::list<Creature*> CryptGuards = GetCreaturesByEntry(NPC_CRYPT_GUARD);
            if (!CryptGuards.empty())
                for(std::list<Creature*>::iterator itr = CryptGuards.begin(); itr != CryptGuards.end(); ++itr)
                    if ((*itr)->isDead())
                    {
                        (*itr)->CastSpell((*itr),SPELL_SELF_SPAWN_10,true);
                        (*itr)->AddObjectToRemoveList();
                    }

            const Map::PlayerList &players = m_creature->GetMap()->GetPlayers();
            if (players.isEmpty())
                return;

            for(Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                if (Player* pPlayer = i->getSource())
                {
                    if (pPlayer->isGameMaster())
                        continue;

                    if (pPlayer->isDead())
                        pPlayer->CastSpell(pPlayer,SPELL_SELF_SPAWN_5,true);
                }
            CryptGuards.clear();
        }else RiseFromCorpse_Timer -= uiDiff; */

        if(!swarm)
        {
            //Impale_Timer
            if (m_uiImpaleTimer < uiDiff)
            {
                //Cast Impale on a random target
                //Do NOT cast it when we are afflicted by locust swarm
                if (!m_creature->HasAura(m_bIsRegularMode ? SPELL_LOCUSTSWARM : SPELL_LOCUSTSWARM_H, EFFECT_INDEX_1))
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                        DoCast(target,m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
                m_uiImpaleTimer = 15000;
            }else m_uiImpaleTimer -= uiDiff;

            //LocustSwarm_Timer
            if (m_uiLocustSwarmTimer < uiDiff)
            {
                //Cast Locust Swarm buff on ourselves
                DoCast(m_creature, m_bIsRegularMode ? SPELL_LOCUSTSWARM : SPELL_LOCUSTSWARM_H);
                swarm = true;
                //Summon Crypt Guard immidietly after Locust Swarm
                if (CryptGuard_count < MAX_CRYPT_GUARDS)
                    DoSpawnCreature(NPC_CRYPT_GUARD,0,0,0,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000);
                m_uiLocustSwarmTimer = 20000;
            }else m_uiLocustSwarmTimer -= uiDiff;
        }
        else
        {
            if (m_uiLocustSwarmTimer < uiDiff)
            {            
                swarm = false;
                m_uiLocustSwarmTimer = 60000 + rand()%20000;
            }else m_uiLocustSwarmTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubrekhan(Creature* pCreature)
{
    return new boss_anubrekhanAI(pCreature);
}

void AddSC_boss_anubrekhan()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_anubrekhan";
    NewScript->GetAI = &GetAI_boss_anubrekhan;
    NewScript->RegisterSelf();
}
