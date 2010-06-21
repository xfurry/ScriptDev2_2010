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
SDName: boss_anubarak_trial
SD%Complete: 0
SDComment:
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    SAY_INTRO       = -1605076,
    SAY_AGGRO       = -1605067,
    SAY_SUBMERGE    = -1605068,
    SAY_SWARM       = -1605069,
    SAY_SLAY1       = -1605070,
    SAY_SLAY2       = -1605071,
    SAY_DEATH       = -1605072,
    SAY_BERSERK     = -1605073,
    SAY_OUTRO       = -1605074,
    EMOTE_BURROW    = -1605136,
    EMOTE_PURSUE    = -1605137,

    SPELL_FREEZING_SLASH        = 66012,
    SPELL_PENETRATING_COLD_10   = 67700,
    SPELL_PENETRATING_COLD_25   = 68509,

    SPELL_PURSUING_SPIKES       = 65922,
    SPELL_PURSUED_BY_ANUB       = 67574,
    SPELL_IMPALE_10             = 67858,
    SPELL_IMPALE_25             = 67859,
    SPELL_SPIKES_ICE            = 66181,    // used when hit ice
    SPELL_SPIKES_SUMMON         = 66169,    // summon a spike
    SPELL_SPIKES_AIR            = 66170,    // used to throw target in the air
    SPELL_SPIKES_GROUND         = 65921,    // aura that makes the ground shake

    NPC_SPIKE                   = 34660,

    SPELL_LEECHING_SWARM_10     = 67630,
    SPELL_LEECHING_SWARM_25     = 66118,
    SPELL_LEECHING_SWARM_10HC   = 68646,
    SPELL_LEECHING_SWARM_25HC   = 68647,
    SPELL_LEECHING_SWARM_DUMMY  = 66240,
    SPELL_LEECHING_SWARM_HEAL   = 66125,

    NPC_FROST_SPHERE            = 34606,
    SPELL_PERMAFROST            = 66193,

    NPC_NERUBIAN_BURROWER       = 34607,
    SPELL_SPIDER_FRENZY         = 66129,
    SPELL_SPIDER_FRENZY_TRIG    = 66128,
    SPELL_EXPOSE_WEAKNESS       = 67847,
    SPELL_SUBMERGE              = 67322,
    SPELL_SHADOW_STRIKE         = 66134,

    NPC_SWARM_SCARAB            = 34605,
    SPELL_ACIC_MANDIBLES        = 67861,
    SPELL_DETERMINATION         = 66092,

    SPELL_BERSERK               = 26662,
    SPELL_SUBMERGE_ANUB         = 53421,

    ACHIEV_TRAITOR_KING         = 3800,
    ACHIEV_TRAITOR_KING_H       = 3816,
};

struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY AnubLoc[]=
{
    {693.561f, 188.314f}, 
    {686.165f,  97.553f},  
    {734.034f, 191.537f},
    {731.950f, 75.8964f},
};

bool m_bStartAchiev;
uint8 m_uiAchievCounter;

#define LOC_Z           142.12f

#define HOME_X          786.746f
#define HOME_Y          133.428f  

struct MANGOS_DLL_DECL boss_anubarak_trialAI : public ScriptedAI
{
    boss_anubarak_trialAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bHasTaunted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    bool m_bIsPhase3;
    bool m_bStopSummoning;
    uint8 m_uiPhase;

    uint32 m_uiFreezingSlashTimer;
    uint32 m_uiPenetratingColdTimer;
    uint32 m_uiPursuingSpikesTimer;
    uint32 m_uiPhaseTimer;
    uint32 m_uiFrostSphereTimer;
    uint32 m_uiBorrowerTimer;
    uint32 m_uiScarabTimer;

    uint32 SwarmDamage;
    uint32 SwarmDamageTotal;
    uint32 SwarmTickTimer;
    double SwarmDamagePercent;

    bool m_bHasTaunted;

    uint32 m_uiBerserkTimer;
    uint32 m_uiAchievTimer;

    std::list<Creature*> lSpheres;

    void Reset() 
    {
        m_bIsPhase3         = false;
        m_bStopSummoning    = false;
        m_uiPhase           = 0;

        m_uiFreezingSlashTimer      = 20000 + rand()%5000;
        m_uiPenetratingColdTimer    = 10000;
        m_uiPursuingSpikesTimer     = 30000;
        m_uiPhaseTimer              = 90000;
        m_uiFrostSphereTimer        = 30000;
        m_uiBorrowerTimer           = 10000;
        m_uiScarabTimer             = 5000;
        SwarmTickTimer              = 3000;

        m_uiBerserkTimer    = 600000;  // 10 min

        m_creature->SetVisibility(VISIBILITY_ON);

        m_bStartAchiev      = false;
        m_uiAchievCounter   = 0;
        m_uiAchievTimer     = 0;

        lSpheres.clear();
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
            if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
            }
        }
        m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_ANUB);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);

        DoScriptText(SAY_DEATH, m_creature);
        if(Creature* pTirion = m_creature->SummonCreature(NPC_TIRION_END, 679.531f, 142.507f, 142.129f, 0, TEMPSUMMON_TIMED_DESPAWN, 600000))
            DoScriptText(SAY_OUTRO, pTirion);

        GetCreatureListWithEntryInGrid(lSpheres, m_creature, NPC_FROST_SPHERE, DEFAULT_VISIBILITY_INSTANCE);

        if (!lSpheres.empty())
        {
            for(std::list<Creature*>::iterator iter = lSpheres.begin(); iter != lSpheres.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_bHasTaunted)
        {
            if(m_pInstance->GetData(TYPE_TWIN_VALKYR) == DONE)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoScriptText(SAY_INTRO, m_creature);
                m_bHasTaunted = true;
            }
        }

        ScriptedAI::MoveInLineOfSight(who);   
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bStartAchiev)
            m_uiAchievTimer += uiDiff;

        if (m_uiAchievTimer < 30000)
        {
            if(m_uiAchievCounter >= 40)
            {
                if(m_pInstance)
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        m_pInstance->DoCompleteAchievement(ACHIEV_TRAITOR_KING);
                    else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        m_pInstance->DoCompleteAchievement(ACHIEV_TRAITOR_KING_H);
                }
                m_bStartAchiev = false;
            }
        }

        if (m_uiAchievTimer > 30000 && m_bStartAchiev)
        {
            m_bStartAchiev = false;
            m_uiAchievCounter = 0;
            m_uiAchievTimer = 0;
        }

        if(m_uiPhase == 0)
        {
            // spells
            if (m_uiFreezingSlashTimer < uiDiff && !m_bIsPhase3)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_FREEZING_SLASH);

                m_uiFreezingSlashTimer = 20000 + rand()%5000;
            }
            else
                m_uiFreezingSlashTimer -= uiDiff;

            if (m_uiPenetratingColdTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature, SPELL_PENETRATING_COLD_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature, SPELL_PENETRATING_COLD_25);
                m_uiPenetratingColdTimer = urand(20000, 25000);
            }
            else
                m_uiPenetratingColdTimer -= uiDiff;

            if (m_uiBorrowerTimer < uiDiff && !m_bStopSummoning)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    uint8 i = urand(0, 2);
                    if(Creature *pTemp = m_creature->SummonCreature(NPC_NERUBIAN_BURROWER, AnubLoc[i].x, AnubLoc[i].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                        pTemp->SetInCombatWithZone();
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    for(uint8 j = 0; j < 2; j++)
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(NPC_NERUBIAN_BURROWER, AnubLoc[j].x, AnubLoc[j].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                            pTemp->SetInCombatWithZone();
                    }
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    for(uint8 j = 0; j < 4; j++)
                    {
                        if(Creature *pTemp = m_creature->SummonCreature(NPC_NERUBIAN_BURROWER, AnubLoc[j].x, AnubLoc[j].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                            pTemp->SetInCombatWithZone();
                    }
                }
                m_uiBorrowerTimer = 50000 + urand(1000, 10000);
            }
            else
                m_uiBorrowerTimer -= uiDiff;

            if (m_uiFrostSphereTimer < uiDiff)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->SummonCreature(NPC_FROST_SPHERE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 30000);
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    for(uint8 j = 0; j < 2; j++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            m_creature->SummonCreature(NPC_FROST_SPHERE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 30000);
                    }
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    for(uint8 j = 0; j < 4; j++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            m_creature->SummonCreature(NPC_FROST_SPHERE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 30000);
                    }
                }
                m_uiFrostSphereTimer = 20000;
            }
            else
                m_uiFrostSphereTimer -= uiDiff;

            if (m_uiPhaseTimer < uiDiff && !m_bIsPhase3)
            {
                m_uiPhase = 1;
                m_uiPursuingSpikesTimer     = 4000;
                DoScriptText(EMOTE_BURROW, m_creature);
                DoCast(m_creature, SPELL_SUBMERGE_ANUB);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                DoScriptText(SAY_SUBMERGE, m_creature);
                m_uiScarabTimer = 10000;
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiPhaseTimer = 60000;
            }
            else
                m_uiPhaseTimer -= uiDiff;

            if(m_creature->GetHealthPercent() < 30.0f && !m_bIsPhase3)
            {
                DoScriptText(SAY_SWARM, m_creature);

                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    DoCast(m_creature, SPELL_LEECHING_SWARM_10);
                    SwarmDamagePercent = 0.1;
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                {
                    DoCast(m_creature, SPELL_LEECHING_SWARM_25);
                    SwarmDamagePercent = 0.2;
                }
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    DoCast(m_creature, SPELL_LEECHING_SWARM_10HC);
                    SwarmDamagePercent = 0.2;
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    DoCast(m_creature, SPELL_LEECHING_SWARM_25HC);
                    SwarmDamagePercent = 0.3;
                }

                m_bIsPhase3 = true;
                SwarmTickTimer = 3000;
                // stop summoning in not on Hc
                if(Difficulty != RAID_DIFFICULTY_10MAN_NORMAL || Difficulty != RAID_DIFFICULTY_25MAN_NORMAL)
                    m_bStopSummoning = true; 
            }

            // workaround for leeching swarm
            if(SwarmTickTimer < uiDiff && m_bIsPhase3)
            {
                // check for players
                uint32 m_uiSwarmDmgTotal = 0;
                Map *map = m_creature->GetMap();
                if (map->IsDungeon())
                {
                    Map::PlayerList const &PlayerList = map->GetPlayers();

                    if (PlayerList.isEmpty())
                        return;

                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if(i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()) < 180.0f)
                        {
                            uint32 m_uiSwarmDmg = i->getSource()->GetHealth()*SwarmDamagePercent;
                            if(i->getSource()->GetHealth()*SwarmDamagePercent < 250)
                                m_uiSwarmDmg = 250;
                            m_uiSwarmDmgTotal += m_uiSwarmDmg;
                            i->getSource()->DealDamage(i->getSource(), m_uiSwarmDmg, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
                    }
                    m_creature->DealHeal(m_creature, m_uiSwarmDmgTotal/2, NULL);
                }
                SwarmTickTimer = 1000;
            }
            else SwarmTickTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        // submerged
        if(m_uiPhase == 1)
        {
            if (m_uiPhaseTimer < uiDiff)
            {
                m_uiPhase = 0;
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_ANUB);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                //m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                m_uiBorrowerTimer           = 10000;
                m_uiFreezingSlashTimer      = 20000 + rand()%5000;
                m_uiPenetratingColdTimer    = 10000;
                m_uiFrostSphereTimer        = 30000;
                m_uiPhaseTimer              = 90000;
            }
            else
                m_uiPhaseTimer -= uiDiff;

            if (m_uiPursuingSpikesTimer < uiDiff)
            {
                if(Creature* pSpike = GetClosestCreatureWithEntry(m_creature, NPC_SPIKE, 200.0f))
                {
                    if(pSpike->isAlive())
                    {
                        m_uiPursuingSpikesTimer = 1000;
                        return;
                    }
                }

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoScriptText(EMOTE_PURSUE, m_creature, pTarget);
                    DoCast(pTarget, SPELL_PURSUED_BY_ANUB);
                    //DoCast(pTarget, SPELL_PURSUING_SPIKES);
                    if(Creature* pSpike = m_creature->SummonCreature(NPC_SPIKE, pTarget->GetPositionX() + urand(5, 10), pTarget->GetPositionY() + urand(5, 10), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000))
                    {
                        pSpike->AddThreat(pTarget, 1000.0f);
                        pSpike->GetMotionMaster()->MoveChase(pTarget);
                    }
                }
                m_uiPursuingSpikesTimer = 1000;
            }
            else
                m_uiPursuingSpikesTimer -= uiDiff;

            if (m_uiScarabTimer < uiDiff)
            {
                uint8 i = 0;
                if(Creature *pTemp = m_creature->SummonCreature(NPC_SWARM_SCARAB, AnubLoc[i].x, AnubLoc[i].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    pTemp->SetInCombatWithZone();
                if(Creature *pTemp = m_creature->SummonCreature(NPC_SWARM_SCARAB, AnubLoc[i+1].x, AnubLoc[i+1].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                    pTemp->SetInCombatWithZone();

                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    if(Creature *pTemp = m_creature->SummonCreature(NPC_SWARM_SCARAB, AnubLoc[i+2].x, AnubLoc[i+2].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                        pTemp->SetInCombatWithZone();
                    if(Creature *pTemp = m_creature->SummonCreature(NPC_SWARM_SCARAB, AnubLoc[i+3].x, AnubLoc[i+3].y, LOC_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                        pTemp->SetInCombatWithZone();
                }
                m_uiScarabTimer = urand(7000, 13000);
            }
            else
                m_uiScarabTimer -= uiDiff;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 80)
            EnterEvadeMode();

        if(m_uiPhase == 0)
            DoMeleeAttackIfReady();
    }

    // needs update!
    void DoSpikes(Unit* target)
    {

        if(Creature* pSpike = m_creature->SummonCreature(NPC_SPIKE, target->GetPositionX() + urand(5, 10), target->GetPositionY() + urand(5, 10), target->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000))
            pSpike->AddThreat(target, 1000.0f);

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
};

CreatureAI* GetAI_boss_anubarak_trial(Creature* pCreature)
{
    return new boss_anubarak_trialAI(pCreature);
}

struct MANGOS_DLL_DECL mob_frost_sphereAI : public ScriptedAI
{
    mob_frost_sphereAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        SetCombatMovement(false);
        pCreature->SetDisplayId(25144);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
        m_creature->SetSpeedRate(MOVE_RUN, 0.1f);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MoveRandom();
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth() && GetClosestCreatureWithEntry(m_creature, NPC_ANUBARAK, 150.0f))
        {
            uiDamage = 0;
            DoCast(m_creature, SPELL_PERMAFROST);
            spellTimer = 1000;
            m_creature->SetHealth(0);
            m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*
        if (spellTimer < uiDiff)
        {
        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        spellTimer = 10000;
        }else spellTimer -= uiDiff;
        */
    }
};

CreatureAI* GetAI_mob_frost_sphere(Creature* pCreature)
{
    return new mob_frost_sphereAI (pCreature);
}

class MANGOS_DLL_DECL SpiderFrenzyAura : public Aura
{
public:
    SpiderFrenzyAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};

struct MANGOS_DLL_DECL mob_nerubian_burrowerAI : public ScriptedAI
{
    mob_nerubian_burrowerAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 spellTimer;
    uint32 checkTimer;
    uint32 submergeTimer;
    bool isSubmerged;

    std::list<Creature*> lBorrower;
    uint8 m_uiBorrowerCount;

    void Reset()
    {
        spellTimer = 7000;
        checkTimer = 2000;
        isSubmerged = false;
        lBorrower.clear();
        m_uiBorrowerCount = 0;
        DoCast(m_creature, SPELL_SPIDER_FRENZY);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_creature->GetHealthPercent() < 30.0f && !isSubmerged)
        {
            // check for permafrost
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if(i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()) < 1.0f)
                    {
                        if(i->getSource()->HasAura(SPELL_PERMAFROST, EFFECT_INDEX_0))
                            return;
                    }
                }
            }

            DoCast(m_creature, SPELL_SUBMERGE_ANUB);
            m_creature->SetHealth(m_creature->GetMaxHealth());
            submergeTimer = 10000;
            isSubmerged = true;
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        }

        if (spellTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                DoCast(pTarget, SPELL_EXPOSE_WEAKNESS);
            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        if (checkTimer < uiDiff)
        {
            lBorrower.clear();
            m_uiBorrowerCount = 0;
            GetCreatureListWithEntryInGrid(lBorrower, m_creature, NPC_NERUBIAN_BURROWER, 12.0f);
            if(!lBorrower.empty())
            {
                for(std::list<Creature*>::iterator iter = lBorrower.begin(); iter != lBorrower.end(); ++iter)
                {
                    if ((*iter) && (*iter)->isAlive())
                        m_uiBorrowerCount += 1;
                }
            }

            if(m_uiBorrowerCount > 1)
            {
                SpellEntry* spell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_SPIDER_FRENZY);
                if(m_creature->AddAura(new SpiderFrenzyAura(spell, EFFECT_INDEX_0, NULL, m_creature, m_creature)))
                    m_creature->GetAura(SPELL_SPIDER_FRENZY, EFFECT_INDEX_0)->SetStackAmount(m_uiBorrowerCount - 1);
            }
            checkTimer = 2000;
        }else checkTimer -= uiDiff;

        if (submergeTimer < uiDiff && isSubmerged)
        {
            if (m_creature->HasAura(SPELL_SUBMERGE_ANUB, EFFECT_INDEX_0))
            {
                m_creature->RemoveAurasDueToSpell(SPELL_SUBMERGE_ANUB);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            }
            submergeTimer = 600000;
        }else submergeTimer -= uiDiff;

        if(!m_creature->HasAura(SPELL_SUBMERGE_ANUB, EFFECT_INDEX_0))
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_nerubian_burrower(Creature* pCreature)
{
    return new mob_nerubian_burrowerAI (pCreature);
}

struct MANGOS_DLL_DECL mob_swarm_scarabAI : public ScriptedAI
{
    mob_swarm_scarabAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* pKiller)
    {
        m_uiAchievCounter += 1;
        m_bStartAchiev = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                DoCast(m_creature, SPELL_DETERMINATION);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_ACIC_MANDIBLES);
                break;
            }
            spellTimer = urand(5000, 8000);
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_swarm_scarab(Creature* pCreature)
{
    return new mob_swarm_scarabAI (pCreature);
}

struct MANGOS_DLL_DECL mob_anub_spikeAI : public ScriptedAI
{
    mob_anub_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
        pCreature->SetDisplayId(31124); // look like a spike :)
        pCreature->setFaction(14);
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 spellTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        spellTimer = 1000;
        m_uiDieTimer = 600000;
        m_creature->SetRespawnDelay(DAY);
        DoCast(m_creature, SPELL_SPIKES_GROUND);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(spellTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if(i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()) <= 2.0f /*&& i->getSource()->HasAura(SPELL_PURSUED_BY_ANUB, EFFECT_INDEX_0)*/)
                    {
                        // check permafrost
                        if(i->getSource()->HasAura(SPELL_PERMAFROST, EFFECT_INDEX_0))
                        {
                            DoCast(m_creature, SPELL_SPIKES_ICE);
                            spellTimer  = 5000;
                            m_uiDieTimer = 1000;
                            return;
                        }
                        // do impale
                        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                            DoCast(i->getSource(), SPELL_IMPALE_10);
                        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                            DoCast(i->getSource(), SPELL_IMPALE_25); 
                        // make player jump into the air
                        //DoCast(m_creature, 56090);
                        //i->getSource()->RemoveAurasDueToSpell(SPELL_PURSUED_BY_ANUB);
                        //i->getSource()->TeleportTo(i->getSource()->GetMapId(), i->getSource()->GetPositionX(), i->getSource()->GetPositionY(), i->getSource()->GetPositionZ() + 20, i->getSource()->GetOrientation());
                        m_uiDieTimer = 1000;
                    }
                }
            }
            spellTimer = 1000;
        }
        else
            spellTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_uiDieTimer = 10000;
        }else m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_anub_spike(Creature* pCreature)
{
    return new mob_anub_spikeAI (pCreature);
}

bool GossipHello_npc_tirion_end(Player* pPlayer, Creature* pCreature)
{

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport me to the surface", GOSSIP_SENDER_MAIN,GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tirion_end(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    pPlayer->CLOSE_GOSSIP_MENU();
    if(uiAction == GOSSIP_ACTION_INFO_DEF+1)
        pPlayer->TeleportTo(571, 8516.319f, 634.558f, 547.393f, 1.62f);
    return true;
}

void AddSC_boss_anubarak_trial()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak_trial";
    newscript->GetAI = &GetAI_boss_anubarak_trial;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_sphere";
    newscript->GetAI = &GetAI_mob_frost_sphere;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nerubian_burrower";
    newscript->GetAI = &GetAI_mob_nerubian_burrower;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarm_scarab";
    newscript->GetAI = &GetAI_mob_swarm_scarab;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_anub_spike";
    newscript->GetAI = &GetAI_mob_anub_spike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tirion_end";
    newscript->pGossipHello = &GossipHello_npc_tirion_end;
    newscript->pGossipSelect = &GossipSelect_npc_tirion_end;
    newscript->RegisterSelf();
}