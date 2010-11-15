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
SDName: boss_black_knight
SD%Complete: 92%
SDComment: missing yells. not sure about timers.
SDCategory: Trial Of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum
{
    // yells
    SAY_INTRO1                  = -1614032,
    SAY_KILL_HERALD             = -1614033,
    SAY_TIRION_INTRO            = -1614034,
    SAY_INTRO2                  = -1614035,
    SAY_INTRO3                  = -1614036,
    SAY_AGGRO                   = -1614037,
    SAY_PHASE2                  = -1614039,
    SAY_PHASE3                  = -1614040,
    SAY_KILL1                   = -1614041,
    SAY_KILL2                   = -1614042,
    SAY_DEATH                   = -1614043,
    SAY_TIRION_OUTRO1           = -1614044,
    SAY_TIRION_OUTRO2           = -1614045,

    //undead
    SPELL_PLAGUE_STRIKE         = 67724,
    SPELL_PLAGUE_STRIKE_H       = 67884,
    SPELL_ICY_TOUCH             = 67718,
    SPELL_ICY_TOUCH_H           = 67881,
    SPELL_OBLITERATE            = 67725,
    SPELL_OBLITERATE_H          = 67883,
    SPELL_DEATHS_RESPITE        = 68306,
    //skeleton
    SPELL_ARMY                  = 67761, 
    SPELL_DESECRATION           = 67782,
    SPELL_DESECRATION_SUMMON    = 67779,
    NPC_DESECRATION_STALKER     = 35614,
    SPELL_GHOUL_EXPLODE         = 67751,
    //ghost
    SPELL_DEATHS_BITE           = 67808,
    SPELL_DEATHS_BITE_H         = 67875,
    SPELL_MARKED_DEATH          = 67823,

    //risen ghoul
    SPELL_CLAW                  = 67879,
    SPELL_EXPLODE               = 67729,
    SPELL_EXPLODE_H             = 67886,
    SPELL_LEAP                  = 67749,
    SPELL_LEAP_H                = 67880,

    //sword ID
    EQUIP_SWORD                 = 40343,

    ACHIEV_HAD_WORSE            = 3804,
};

// Risen Ghoul
struct MANGOS_DLL_DECL mob_toc_risen_ghoulAI: public ScriptedAI
{
    mob_toc_risen_ghoulAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpellTimer;
    uint32 m_uiCreatureEntry;
    bool m_bHasExploded;

    void Reset()
    {
        m_uiCreatureEntry = m_creature->GetEntry();
        m_creature->SetRespawnDelay(DAY);
        m_uiSpellTimer = 2500;
        m_bHasExploded = false;
    }

    void DoExplode()
    {
        DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLODE : SPELL_EXPLODE_H);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_BLACK_KNIGHT) != IN_PROGRESS)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, IN_PROGRESS);
        if(Creature* pKnight = GetClosestCreatureWithEntry(m_creature, NPC_BLACK_KNIGHT, 180.0f))
            pKnight->AI()->AttackStart(m_creature->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCreatureEntry == NPC_RISEN_CHAMPION)
        {
            if (m_pInstance && m_pInstance->GetData(TYPE_BLACK_KNIGHT) != IN_PROGRESS) 
                m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiCreatureEntry == NPC_RISEN_CHAMPION)
        {
            if(m_uiSpellTimer < uiDiff)
            {
                if (Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BLACK_KNIGHT, 180.0f))
                {
                    if (pTemp->isAlive() && pTemp->GetHealthPercent() < 25.0f && !m_bHasExploded)
                    {
                        DoExplode();
                        m_bHasExploded = true;
                        return;
                    }
                }

                if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 30.0f))
                {
                    DoCast(m_creature->getVictim(), SPELL_CLAW);
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->AddThreat(pTarget, 100.0f);
                    m_uiSpellTimer = urand(2000, 4000);
                }
            }
            else
                m_uiSpellTimer -= uiDiff;
        }
        else
        {
            if(m_uiSpellTimer < uiDiff)
            {
                if (Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BLACK_KNIGHT, 180.0f))
                {
                    if (pTemp->isAlive() && pTemp->GetHealthPercent() < 25.0f && !m_bHasExploded)
                    {
                        DoExplode();
                        m_bHasExploded = true;
                        return;
                    }
                }

                if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 4.0f))
                {
                    DoCast(m_creature->getVictim(), SPELL_CLAW);
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->AddThreat(pTarget, 100.0f);
                    m_uiSpellTimer = urand(2000, 4000);
                }
                else if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 30.0f))
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LEAP : SPELL_LEAP_H);
                    m_uiSpellTimer = urand(2000, 4000);
                } 
            }
            else 
                m_uiSpellTimer -= uiDiff;
        }

        if (m_creature->GetHealthPercent() < 25.0f && !m_bHasExploded)
        {
            DoExplode();
            m_bHasExploded = true;
        }

        DoMeleeAttackIfReady();
    }
};

// The Black Knight
struct MANGOS_DLL_DECL boss_black_knightAI: public ScriptedAI
{
    boss_black_knightAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsIntro = true;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;

    bool m_bIsIntro;

    uint32 m_uiMinionId;

    uint32 m_uiPlague_Strike_Timer;
    uint32 m_uiIcy_Touch_Timer;
    uint32 m_uiObliterate_Timer;
    uint32 m_uiChoke_Timer;
    uint32 m_uiDeath_Timer;
    uint32 m_uiMark_Timer;
    uint32 m_uiPhase_Delay;
    uint32 m_uiDesecrationTimer;
    uint32 m_uiGhoulExplodeTimer;

    uint32 TeamInInstance;

    bool m_bIsPhase1;
    bool m_bIsPhase2;
    bool m_bIsPhase3;

    void Reset()
    {
        m_uiIntroTimer              = 3000;
        m_uiIntroStep               = 0;
        TeamInInstance              = GetFaction();
        m_uiPlague_Strike_Timer     = 5000;
        m_uiIcy_Touch_Timer         = 10000;
        m_uiObliterate_Timer        = 16000;
        m_uiChoke_Timer             = 15000;
        m_bIsPhase1                 = true;      //FIXME: use one numeric value instead
        m_bIsPhase2                 = false;
        m_bIsPhase3                 = false;
        m_creature->SetDisplayId(29837);
        SetEquipmentSlots(false, EQUIP_SWORD, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
        m_creature->SetRespawnDelay(DAY);
        // set ghoul
        if(TeamInInstance == HORDE)
            m_uiMinionId = NPC_RISEN_ARELAS;
        else if(TeamInInstance == ALLIANCE)
            m_uiMinionId = NPC_RISEN_JAEREN;
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_BLACK_KNIGHT) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_KILL1, m_creature); break;
        case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance && m_pInstance->GetData(TYPE_BLACK_KNIGHT) != IN_PROGRESS)
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
        {
            pSummoned->AddThreat(pTarget, 100.0f);
            pSummoned->AI()->AttackStart(pTarget);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth() && !m_bIsPhase3)
        {
            uiDamage = 0;
            if (m_bIsPhase2)
                StartPhase3();
            if (m_bIsPhase1)
                StartPhase2();
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;
        if (m_bIsPhase3)
        {
            DoScriptText(SAY_DEATH, m_creature);
            if(Creature* pTirion = GetClosestCreatureWithEntry(m_creature, NPC_TIRION, 180.0f))
                DoScriptText(SAY_TIRION_OUTRO1, pTirion);
            m_pInstance->SetData(TYPE_BLACK_KNIGHT, DONE);
        }
        if (m_bIsPhase2 && !m_creature->isAlive())
        {
            m_creature->Respawn();
            StartPhase3();
        }
        if (m_bIsPhase1 && !m_creature->isAlive())
        {
            m_creature->Respawn();
            StartPhase2();
        }
    }

    void StartPhase2()
    {
        m_bIsPhase1             = false;
        m_bIsPhase2             = true;
        m_bIsPhase3             = false;
        m_uiPlague_Strike_Timer = 14000;
        m_uiIcy_Touch_Timer     = 12000;
        m_uiObliterate_Timer    = 18000;
        m_uiDesecrationTimer    = 15000;
        m_uiGhoulExplodeTimer   = 13000;
        m_creature->SetHealthPercent(100.0f);
        m_creature->SetDisplayId(27550);
        DoCast(m_creature, SPELL_ARMY);
        DoScriptText(SAY_PHASE2, m_creature);
    }

    void StartPhase3()
    {
        m_bIsPhase1     = false;
        m_bIsPhase2     = false;
        m_bIsPhase3     = true;
        m_uiDeath_Timer = 5000;
        m_uiMark_Timer  = 9000;
        m_creature->SetHealthPercent(100.0f);
        m_creature->SetDisplayId(14560);
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
        DoScriptText(SAY_PHASE3, m_creature);
        ExplodeZombies();
    }

    void ExplodeZombies()
    {
        std::list<Creature* > lZombies;
        GetCreatureListWithEntryInGrid(lZombies, m_creature, NPC_RISEN_CHAMPION, 75.0f);

        if (!lZombies.empty())
        {
            for(std::list<Creature*>::iterator iter = lZombies.begin(); iter != lZombies.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    ((mob_toc_risen_ghoulAI*)(*iter)->AI())->DoExplode();
            }
        }
    }

    Creature* SelectRandomZombie(float fRange)
    {
        std::list<Creature* > lZombies;
        GetCreatureListWithEntryInGrid(lZombies, m_creature, NPC_RISEN_CHAMPION, fRange);

        //This should not appear!
        if (lZombies.empty()){
            m_uiGhoulExplodeTimer = urand(7000, 15000);
            return NULL;
        }

        std::list<Creature* >::iterator iter = lZombies.begin();
        advance(iter, urand(0, lZombies.size()-1));

        if((*iter)->isAlive())
            return *iter;
        else
        {
            m_uiGhoulExplodeTimer = urand(7000, 15000);
            return NULL;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if(m_uiIntroTimer < uiDiff)
            {
                switch(m_uiIntroStep)
                {
                case 0:
                    DoScriptText(SAY_INTRO1, m_creature);
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 8000;
                    break;
                case 1:
                    DoScriptText(SAY_KILL_HERALD, m_creature);
                    if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_JAEREN, 180.0f))
                        pAnnoucer->DealDamage(pAnnoucer, pAnnoucer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pAnnoucer = GetClosestCreatureWithEntry(m_creature, NPC_ARELAS, 180.0f))
                        pAnnoucer->DealDamage(pAnnoucer, pAnnoucer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 3000;
                    break;
                case 2:
                    // tirion
                    if(Creature* pTirion = GetClosestCreatureWithEntry(m_creature, NPC_TIRION, 180.0f))
                        DoScriptText(SAY_TIRION_INTRO, pTirion);
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 3000;
                    break;
                case 3:
                    DoScriptText(SAY_INTRO2, m_creature);
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 9000;
                    break;
                case 4:
                    DoScriptText(SAY_INTRO3, m_creature);
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 4000;
                    break;
                case 5:
                    // summmon 
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_BLACK_KNIGHT, IN_PROGRESS);
                    if(Creature* pGhoul = m_creature->SummonCreature(m_uiMinionId, 0, 0, 0, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                        pGhoul->SetInCombatWithZone();
                    m_creature->SetInCombatWithZone();
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_bIsIntro = false;
                    ++m_uiIntroStep;
                    m_uiIntroTimer = 3000;
                    break;
                }
            }else m_uiIntroTimer -= uiDiff;
        }

        if(m_bIsIntro)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // phase 1 spells
        if (m_uiPlague_Strike_Timer < uiDiff && !m_bIsPhase3)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PLAGUE_STRIKE : SPELL_PLAGUE_STRIKE_H);
            m_uiPlague_Strike_Timer = 10500;
        }
        else
            m_uiPlague_Strike_Timer -= uiDiff;  

        if (m_uiIcy_Touch_Timer < uiDiff && !m_bIsPhase3)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ICY_TOUCH : SPELL_ICY_TOUCH_H);
            m_uiIcy_Touch_Timer = 10000;
        }
        else
            m_uiIcy_Touch_Timer -= uiDiff;

        if (m_uiObliterate_Timer < uiDiff && !m_bIsPhase3)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OBLITERATE : SPELL_OBLITERATE_H);
            m_uiObliterate_Timer = 11000;
        }
        else
            m_uiObliterate_Timer -= uiDiff;

        // only in phase 1
        if (m_uiChoke_Timer < uiDiff && m_bIsPhase1)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_DEATHS_RESPITE);
            m_uiChoke_Timer = 15000;
        }
        else
            m_uiChoke_Timer -= uiDiff;

        // phase 2 spells
        if(m_uiDesecrationTimer < uiDiff && m_bIsPhase2)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_DESECRATION_SUMMON);
            m_uiDesecrationTimer = urand(14000, 17000);
        }
        else m_uiDesecrationTimer -= uiDiff;

        if(m_uiGhoulExplodeTimer < uiDiff && m_bIsPhase2)
        {
            if(Creature* pZombie = SelectRandomZombie(75.0f))
                ((mob_toc_risen_ghoulAI*)pZombie->AI())->DoExplode();
            m_uiGhoulExplodeTimer = urand(7000, 15000);
        }
        else m_uiGhoulExplodeTimer -= uiDiff;

        // phase 3 spells
        if (m_uiMark_Timer < uiDiff && m_bIsPhase3)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_MARKED_DEATH);
            m_uiMark_Timer = 15000;
        }
        else
            m_uiMark_Timer -= uiDiff;

        if (m_uiDeath_Timer < uiDiff && m_bIsPhase3)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_DEATHS_BITE : SPELL_DEATHS_BITE_H);
            m_uiDeath_Timer = 3500;
        }
        else
            m_uiDeath_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Desecration bunny
struct MANGOS_DLL_DECL mob_toc_desecration_bunnyAI: public ScriptedAI
{
    mob_toc_desecration_bunnyAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        pCreature->setFaction(14);
        pCreature->SetDisplayId(11686);     // make invisible
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        DoCast(m_creature, SPELL_DESECRATION);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_BLACK_KNIGHT) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_toc_risen_ghoul(Creature* pCreature)
{
    return new mob_toc_risen_ghoulAI(pCreature);
}

CreatureAI* GetAI_boss_black_knight(Creature* pCreature)
{
    return new boss_black_knightAI(pCreature);
}

CreatureAI* GetAI_mob_toc_desecration_bunny(Creature* pCreature)
{
    return new mob_toc_desecration_bunnyAI(pCreature);
}

void AddSC_boss_black_knight()
{
    Script *NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_black_knight";
    NewScript->GetAI = &GetAI_boss_black_knight;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc_risen_ghoul";
    NewScript->GetAI = &GetAI_mob_toc_risen_ghoul;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc_desecration_bunny";
    NewScript->GetAI = &GetAI_mob_toc_desecration_bunny;
    NewScript->RegisterSelf();
}
