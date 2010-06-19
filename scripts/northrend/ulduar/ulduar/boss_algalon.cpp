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
SDName: boss_algalon
SD%Complete: 
SDComment:
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_INTRO1                      = -1603270,
    SAY_INTRO2                      = -1603271,
    SAY_INTRO3                      = -1603272,
    SAY_ENGAGE                      = -1603141,
    SAY_AGGRO                       = -1603140,
    SAY_SLAY1                       = -1603145,
    SAY_SLAY2                       = -1603146,
    SAY_SUMMON_STAR                 = -1603148,
    SAY_BIGBANG1                    = -1603142,
    SAY_BIGBANG2                    = -1603143,
    SAY_PHASE2                      = -1603144, 
    SAY_BERSERK                     = -1603147,
    SAY_DESPAWN1                    = -1603273,
    SAY_DESPAWN2                    = -1603274,
    SAY_DESPAWN3                    = -1603275,
    SAY_OUTRO1                      = -1603276,
    SAY_OUTRO2                      = -1603277,
    SAY_OUTRO3                      = -1603278,
    SAY_OUTRO4                      = -1603149,
    SAY_OUTRO5                      = -1603279,

    ITEM_PLANETARIUM_KEY            = 45796,
    ITEM_PLANETARIUM_KEY_H          = 45798,

    //spells to be casted
    SPELL_QUANTUM_STRIKE            = 64395, //Normal Quantum Strike
    SPELL_QUANTUM_STRIKE_H          = 64592, //Heroic Quantum Strike
    SPELL_PHASE_PUNCH               = 64412, //Phase punch 
    SPELL_PHASE_PUNCH_SHIFT         = 64417,
    SPELL_PHASE_PUNCH_TRIGGER       = 65508, 
    SPELL_COSMIC_SMASH              = 62311, //62301, //Normal Cosmic Smash
    SPELL_COSMIC_SMASH_H            = 64569, //64598, //Heroic Cosmic Smash
    SPELL_BIG_BANG                  = 64443, //Normal Big Bang
    SPELL_BIG_BANG_H                = 64584, //Heroic Big Bang
    SPELL_ASCEND                    = 70063, //64487, //Ascend to the Heavens -> changed to "the fury of frostmourne" because is's broken
    SPELL_BERSERK                   = 47008, //Berserk

    // mobs
    CREATURE_COLLAPSING_STAR        = 32955,    // they lose 1%hp per sec & cast black hole explosion when they die -> leave a black hole
    SPELL_BLACK_HOLE_EXPLOSION      = 64122,
    SPELL_BLACK_HOLE_EXPLOSION_H    = 65108,

    CREATURE_BLACK_HOLE             = 32953,    // players must stay inside to avoid big bang

    CREATURE_LIVING_CONSTELLATION   = 33052,    // if one enters a black hole they are despawned
    SPELL_ARCANE_BARRAGE            = 64599, //Arcane Barage
    SPELL_ARCANE_BARRAGE_H          = 64607, //Heroic Arcane Barage?

    CREATURE_DARK_MATTER            = 33089,    // populates the black holes = 7
    CREATURE_COSMIC_SMASH           = 33104,

    ACHIEV_FEED_TEARS               = 3004,
    ACHIEV_FEED_TEARS_H             = 3005,     // nobody dies in the raid lockout
    ACHIEV_HERALD_OF_TITANS         = 3316,
    ACHIEV_OBSERVED                 = 3036,
    ACHIEV_OBSERVED_H               = 3037,
    ACHIEV_SUPERMASSIVE             = 3003,
    ACHIEV_SUPERMASSIVE_H           = 3002,
};

//Positional defines
#define LOC_Z                       417.32f  
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {1659.420f, -317.325f},
    {1641.782f, -329.481f},
    {1625.716f, -325.142f},
    {1610.169f, -312.011f},
    {1617.442f, -286.099f},
    {1640.035f, -279.357f},
};

//Algalon
struct MANGOS_DLL_DECL boss_algalonAI : public ScriptedAI
{
    boss_algalonAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        DespawnTimer = 6000000;  // 1h;
        pCreature->setFaction(35);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    std::list<uint64> m_lCollapsingStarGUIDList;
    std::list<uint64> m_lLivingConstelationGUIDList;

    bool m_bIsRegularMode;

    uint64 BlackHoleGUID;

    uint32 Ascend_Timer;
    uint32 Berserk_Timer;
    uint32 BigBang_Timer;
    uint32 CosmicSmash_Timer;
    uint32 PhasePunch_Timer;
    uint32 QuantumStrike_Timer;
    uint32 CollapsingStar_Timer;
    uint32 LivingConstellationTimer;
    uint32 PhasePunchCheckTimer;
    uint32 DarkMaterTimer;
    uint32 DespawnTimer;

    // intro & outro
    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;
    bool isIntro;
    uint32 IntroTimer;
    uint32 IntroStep;

    uint8 phase;
    bool isPhase2;

    bool isDespawned;
    bool isFirstTime;

    void Reset()
    {
        BlackHoleGUID = 0;

        Ascend_Timer        = 480000; //8 minutes
        QuantumStrike_Timer = 4000 + rand()%10000;
        Berserk_Timer       = 360000; //6 minutes
        CollapsingStar_Timer = urand(15000, 20000); //Spawns between 15 to 20 seconds
        LivingConstellationTimer = 60000;
        BigBang_Timer       = 90000;
        DarkMaterTimer      = 90000;
        PhasePunch_Timer    = 8000;
        CosmicSmash_Timer   = urand(30000, 60000);
        PhasePunchCheckTimer = 40000;

        phase = 10;
        isPhase2 = false;

        OutroTimer          = 10000;
        Step                = 1;
        isIntro             = true;
        IntroTimer          = 10000;
        IntroStep           = 1;
        isOutro             = false;  

        isDespawned         = false;
        isFirstTime         = false;

        if(!isFirstTime)
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        DespawnCollapsingStar();
    }

    void KilledUnit(Unit *victim)
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
            m_pInstance->SetData(TYPE_ALGALON, NOT_STARTED);
    }

    void DoOutro()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ALGALON, DONE);

        m_creature->ForcedDespawn();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(pWho) &&
            pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40) && phase > 2)
        {
            SetCombatMovement(false);
            phase = 0;
        }
    }

    void StartEncounter()
    {
        m_creature->setFaction(14);
        isFirstTime = true;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            isOutro = true;
        }
    }

    void Aggro(Unit* pWho)
    {
        phase = 1;
        if(isFirstTime)
            DoScriptText(SAY_ENGAGE, m_creature);
        else
            DoScriptText(SAY_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
        SetCombatMovement(true);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ALGALON, IN_PROGRESS);
    }

    void DespawnCollapsingStar()
    {
        if (m_lCollapsingStarGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lCollapsingStarGUIDList.begin(); itr != m_lCollapsingStarGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }
        m_lCollapsingStarGUIDList.clear();

        if (m_lLivingConstelationGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lLivingConstelationGUIDList.begin(); itr != m_lLivingConstelationGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }
        m_lLivingConstelationGUIDList.clear();
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == CREATURE_COLLAPSING_STAR)
            m_lCollapsingStarGUIDList.push_back(pSummoned->GetGUID());

        if (pSummoned->GetEntry() == CREATURE_LIVING_CONSTELLATION)
            m_lLivingConstelationGUIDList.push_back(pSummoned->GetGUID());
    }

    void SummonCollapsingStar()
    {
        uint8 i = urand(0, 2);
        for(uint8 j = i; j < i + urand(3, 4); j++)
            m_creature->SummonCreature(CREATURE_COLLAPSING_STAR, SummonLoc[j].x, SummonLoc[j].y, LOC_Z, 0, TEMPSUMMON_CORPSE_DESPAWN, 10000);  
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // despawn timer
        if(DespawnTimer < uiDiff && !isDespawned)
        {
            isDespawned = true;
            isOutro = true;
        }
        else DespawnTimer -= uiDiff;

        if(!isOutro)
        {
            // intro
            if(phase == 0)
            {
                if(isIntro  && isFirstTime)
                {
                    switch(IntroStep)
                    {
                    case 1:
                        DoScriptText(SAY_INTRO1, m_creature);
                        ++IntroStep;
                        IntroTimer = 7000;
                        break;
                    case 3:
                        DoScriptText(SAY_INTRO2, m_creature);
                        ++IntroStep;
                        IntroTimer = 7000;
                        break;
                    case 5:
                        DoScriptText(SAY_INTRO3, m_creature);
                        ++IntroStep;
                        IntroTimer = 10000;
                        break;
                    case 7:
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        isIntro = false;
                        ++IntroStep;
                        IntroTimer = 10000;
                        break;
                    }
                }
                else return;

                if (IntroTimer <= uiDiff)
                {
                    ++IntroStep;
                    IntroTimer = 330000;
                } IntroTimer -= uiDiff;
            }

            if(phase == 1)
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // spells
                if(QuantumStrike_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_QUANTUM_STRIKE : SPELL_QUANTUM_STRIKE_H);
                    QuantumStrike_Timer = 4000 + rand()%10000;
                }else QuantumStrike_Timer -= uiDiff;

                if(BigBang_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_BIG_BANG : SPELL_BIG_BANG_H, true);
                    BigBang_Timer = 90000;
                    DarkMaterTimer = 10000;
                }else BigBang_Timer -= uiDiff;

                if(CosmicSmash_Timer < uiDiff)
                {
                    DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_COSMIC_SMASH : SPELL_COSMIC_SMASH_H, true);
                    CosmicSmash_Timer = urand(30000, 60000);
                }else CosmicSmash_Timer -= uiDiff;

                if(PhasePunch_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(),SPELL_PHASE_PUNCH);
                    PhasePunch_Timer = 8000;
                }else PhasePunch_Timer -= uiDiff;

                if(PhasePunchCheckTimer < uiDiff)
                {
                    Map *map = m_creature->GetMap();
                    if (map->IsDungeon())
                    {
                        Map::PlayerList const &PlayerList = map->GetPlayers();

                        if (PlayerList.isEmpty())
                            return;

                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        {
                            if (i->getSource()->isAlive() && i->getSource()->HasAura(SPELL_PHASE_PUNCH, EFFECT_INDEX_0))
                            {
                                Aura *phasePunch = i->getSource()->GetAura(SPELL_PHASE_PUNCH, EFFECT_INDEX_0);
                                if(phasePunch->GetStackAmount() > 4)
                                {
                                    i->getSource()->RemoveAurasDueToSpell(SPELL_PHASE_PUNCH);
                                    DoCast(i->getSource(), SPELL_PHASE_PUNCH_SHIFT);
                                }
                            }
                        }
                    }
                    PhasePunchCheckTimer = 1000;
                }else PhasePunchCheckTimer -= uiDiff;

                // ascent
                if(Ascend_Timer < uiDiff)
                {
                    DoCast(m_creature->getVictim(),SPELL_ASCEND, true);
                    Ascend_Timer = 480000;
                }else Ascend_Timer -= uiDiff;

                // berserk
                if(Berserk_Timer < uiDiff)
                {
                    DoScriptText(SAY_BERSERK, m_creature);	
                    //DoCast(m_creature->getVictim(),SPELL_BERSERK, true);
                    DoCast(m_creature ,SPELL_ASCEND);
                    Berserk_Timer = 360000;
                }else Berserk_Timer -= uiDiff;

                // summons
                if(CollapsingStar_Timer < uiDiff && !isPhase2)
                {
                    DoScriptText(SAY_SUMMON_STAR, m_creature);
                    SummonCollapsingStar();
                    CollapsingStar_Timer = 90000;
                }else CollapsingStar_Timer -= uiDiff;

                if(LivingConstellationTimer < uiDiff && !isPhase2)
                {
                    for(uint8 i = 0; i < urand (1, 3); i++)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            m_creature->SummonCreature(CREATURE_LIVING_CONSTELLATION, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                    }
                    LivingConstellationTimer = 30000;
                }else LivingConstellationTimer -= uiDiff;

                if(DarkMaterTimer < uiDiff && !isPhase2)
                {
                    for(uint8 i = 0; i < 7; i++)
                    {
                        Creature *darkMatter = m_creature->SummonCreature(CREATURE_DARK_MATTER, SummonLoc[i].x, SummonLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);  
                        if(m_bIsRegularMode)
                            darkMatter->SetMaxHealth(19550);
                    }
                    DarkMaterTimer = 90000;
                }else DarkMaterTimer -= uiDiff;

                // hp check
                if(!isPhase2 && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 20)
                {
                    DoScriptText(SAY_PHASE2, m_creature);
                    isPhase2 = true;
                }

                DoMeleeAttackIfReady();

                EnterEvadeIfOutOfCombatArea(uiDiff);
            }
        }
        // outro
        if(isOutro)
        {
            switch(Step)
            {
            case 1:
                m_creature->setFaction(35);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->GetMotionMaster()->MovePoint(0, 1631.970f, -302.635f, 417.321f);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++Step;
                OutroTimer = 5000;
                break;
            case 3:
                // make boss kneel
                m_creature->SetSplineFlags(SPLINEFLAG_UNKNOWN12);
                if(isDespawned)
                {
                    DoScriptText(SAY_DESPAWN1, m_creature);
                    ++Step;
                    OutroTimer = 15000;
                }
                else
                {
                    DoScriptText(SAY_OUTRO1, m_creature);
                    ++Step;
                    OutroTimer = 37000;
                }
                break;
            case 5:
                if(isDespawned)
                {
                    DoScriptText(SAY_DESPAWN2, m_creature);
                    ++Step;
                    OutroTimer = 8000;
                }
                else
                {
                    DoScriptText(SAY_OUTRO2, m_creature);
                    ++Step;
                    OutroTimer = 17000;
                }
                break;
            case 7:
                if(isDespawned)
                {
                    DoScriptText(SAY_DESPAWN3, m_creature);
                    ++Step;
                    OutroTimer = 7000;
                }
                else
                {
                    DoScriptText(SAY_OUTRO3, m_creature);
                    ++Step;
                    OutroTimer = 12000;
                }
                break;
            case 9:
                if(isDespawned)
                {
                    DoCast(m_creature, SPELL_ASCEND);
                    ++Step;
                    OutroTimer = 5000;
                }
                else
                {
                    DoScriptText(SAY_OUTRO4, m_creature);
                    ++Step;
                    OutroTimer = 11000;
                }
                break;
            case 11:
                if(isDespawned)
                {
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_YOGGSARON, DONE);
                    m_creature->ForcedDespawn();
                }
                else
                {
                    DoScriptText(SAY_OUTRO5, m_creature);
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_YOGGSARON, DONE);
                    ++Step;
                    OutroTimer = 13000;
                }
                break;
            case 13:
                DoOutro();
                ++Step;
                OutroTimer = 10000;
                break;
            }
        }
        else 
            return;

        if (OutroTimer <= uiDiff)
        {
            ++Step;
            OutroTimer = 330000;
        } OutroTimer -= uiDiff;
    }	
};	

//Collapsing Star
struct MANGOS_DLL_DECL mob_collapsing_starAI : public ScriptedAI
{
    mob_collapsing_starAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetSpeedRate(MOVE_RUN, 0.5f);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 healthTimer;

    void Reset()
    {
        healthTimer = 1000;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit *victim)
    {
        m_creature->SummonCreature(CREATURE_BLACK_HOLE, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ALGALON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(healthTimer < uiDiff)
        {
            m_creature->DeleteThreatList();
            m_creature->GetMotionMaster()->MoveConfused();
            m_creature->DealDamage(m_creature, (m_creature->GetMaxHealth() * 0.1), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            healthTimer = 1000;
        }else healthTimer -= uiDiff;
    }	
};

//Living constellation
struct MANGOS_DLL_DECL mob_living_constellationAI : public ScriptedAI
{
    mob_living_constellationAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetSpeedRate(MOVE_RUN, 0.5f);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 ArcaneBarrageTimer;

    void Reset()
    {
        ArcaneBarrageTimer = 15000;
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ALGALON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ArcaneBarrageTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)){
                DoCast(target, m_bIsRegularMode ? SPELL_ARCANE_BARRAGE : SPELL_ARCANE_BARRAGE_H);
            }
            ArcaneBarrageTimer = 15000;
        }else ArcaneBarrageTimer -= uiDiff;
    }	
};

//Black hole
struct MANGOS_DLL_DECL mob_black_holeAI : public ScriptedAI
{
    mob_black_holeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 playersCheckTimer;

    void Reset()
    {
        playersCheckTimer = 1000;
        DoCast(m_creature, m_bIsRegularMode ? SPELL_BLACK_HOLE_EXPLOSION : SPELL_BLACK_HOLE_EXPLOSION_H);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_ALGALON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (playersCheckTimer < uiDiff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                        DoCast(i->getSource(), SPELL_PHASE_PUNCH_SHIFT);
                }
            } 

            if(Creature *pConstellation = GetClosestCreatureWithEntry(m_creature, CREATURE_LIVING_CONSTELLATION, 2))
            {
                m_creature->DealDamage(pConstellation, pConstellation->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                m_creature->ForcedDespawn();
            }
            playersCheckTimer = 1000;
        }
        else playersCheckTimer -= uiDiff;
    }	
};

bool GOHello_go_celestial_acces(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();
    bool m_bHasItem = false;

    if (m_bIsRegularMode)
    {
        if(pPlayer->HasItemCount(ITEM_PLANETARIUM_KEY, 1) || pPlayer->HasItemCount(ITEM_PLANETARIUM_KEY_H, 1)) 
            m_bHasItem = true;
    }
    else
    {
        if(pPlayer->HasItemCount(ITEM_PLANETARIUM_KEY_H, 1))
            m_bHasItem = true;        
    }

    if(!m_bHasItem)        
        return false;

    if (!m_pInstance)
        return false;

    if (m_pInstance->GetData(TYPE_ALGALON) == DONE)
    {
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
        return false;
    }

    if (Creature* pAlgalon = ((Creature*)Unit::GetUnit((*pGo), m_pInstance->GetData64(NPC_ALGALON))))
    {
        if(pAlgalon->isAlive())
        {
            ((boss_algalonAI*)pAlgalon->AI())->StartEncounter();
            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

            // open celestial door
            if(GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CELESTIAL_DOOR)))
                m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
        }
    }

    return false;
}

CreatureAI* GetAI_boss_algalon(Creature* pCreature)
{
    return new boss_algalonAI(pCreature);
}

CreatureAI* GetAI_mob_collapsing_star(Creature* pCreature)
{
    return new mob_collapsing_starAI(pCreature);
}

CreatureAI* GetAI_mob_living_constellation(Creature* pCreature)
{
    return new mob_living_constellationAI(pCreature);
}

CreatureAI* GetAI_mob_black_hole(Creature* pCreature)
{
    return new mob_black_holeAI(pCreature);
}

void AddSC_boss_algalon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_algalon";
    newscript->GetAI = &GetAI_boss_algalon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_collapsing_star";
    newscript->GetAI = &GetAI_mob_collapsing_star;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_living_constellation";
    newscript->GetAI = &GetAI_mob_living_constellation;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_black_hole";
    newscript->GetAI = &GetAI_mob_black_hole;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_celestial_acces";
    newscript->pGOHello = &GOHello_go_celestial_acces;
    newscript->RegisterSelf();
}