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
SDName: boss_thorim
SD%Complete: 90%
SDComment: Implement lightning orbs, summon Sit on the platform in the first 3 min.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO1          = -1603221,
    SAY_AGGRO2          = -1603222,
    SAY_SPECIAL1        = -1603223,
    SAY_SPECIAL2        = -1603224,
    SAY_SPECIAL3        = -1603225,
    SAY_JUMP            = -1603226,
    SAY_SLAY1           = -1603227,
    SAY_SLAY2           = -1603228,
    SAY_BERSERK         = -1603229,
    SAY_ARENA_WIPE      = -1603230,
    SAY_DEATH           = -1603231,
    SAY_OUTRO1          = -1603232,
    SAY_OUTRO2          = -1603233,
    SAY_OUTRO3          = -1603234,
    SAY_OUTRO_HARD1     = -1603235,
    SAY_OUTRO_HARD2     = -1603236,
    SAY_OUTRO_HARD3     = -1603237,

    // Sif
    SAY_SIF_INTRO           = -1603185,
    SAY_SIF_EVENT           = -1603186,
    SAY_SIF_DESPAWN         = -1603187,

    // spells
    // phase1
    SPELL_SHEAT_OF_LIGHTNING        = 62276,
    SPELL_STORMHAMMER               = 62042,
    SPELL_DEAFENING_THUNDER         = 62470,
    SPELL_LIGHTNING_SHOCK           = 62017,
    SPELL_CHARGE_ORB                = 62016,
    SPELL_BERSERK_ADDS              = 62560,    // 5 min phase 1 -> for adds
    SPELL_SUMMON_LIGHTNING_ORB      = 62391,
    MOB_LIGHTNING_ORB               = 33138,
    // phase2
    SPELL_TOUTCH_OF_DOMINION        = 62565,    // not available in hard mode
    SPELL_CHAIN_LIGHTNING           = 62131,
    SPELL_CHAIN_LIGHTNING_H         = 64390,
    SPELL_LIGHTNING_CHARGE          = 62279,
    SPELL_UNBALANCING_STRIKE        = 62130,
    SPELL_BERSERK                   = 26662,    // 5 min phase 2

    // hard mode
    SPELL_FROSTBOLT_VOLLEY          = 62580,
    SPELL_FROSTBOLT_VOLLEY_H        = 62604,
    SPELL_FROST_NOVA                = 62597,
    SPELL_FROST_NOVA_H              = 62605,
    SPELL_BLIZZARD                  = 62576,
    SPELL_BLIZZARD_H                = 62602,
    NPC_SIF                         = 33196,
    SPELL_SOUL_CHANNEL              = 40401,

    // pre phase adds
    MOB_JORMUNGAR_BEHEMOTH          = 32882,
    SPELL_ACID_BREATH               = 62315,
    SPELL_ACID_BREATH_H             = 62415,
    SPELL_SWEEP                     = 62316,
    SPELL_SWEEP_H                   = 62417,

    // arena
    MOB_DARK_RUNE_CHAMPION          = 32876,
    MOB_DARK_RUNE_COMMONER          = 32904,
    MOB_DARK_RUNE_EVOKER            = 32878,
    MOB_DARK_RUNE_WARBRINGER        = 32877,

    // hallway
    MOB_DARK_RUNE_ACOLYTE           = 33110,
    MOB_IRON_RING_GUARD             = 32874,
    MINIBOSS_RUNIC_COLOSSUS         = 32872,
    SPELL_SMASH                     = 62339,
    SPELL_RUNIC_BARRIER             = 62338,
    SPELL_CHARGE                    = 62613,
    SPELL_CHARGE_H                  = 62614,

    MOB_IRON_HOHOR_GUARD            = 32875,
    MINIBOSS_ANCIENT_RUNE_GIANT     = 32873,
    SPELL_RUNIC_FORTIFICATION       = 62942,
    SPELL_STOMP                     = 62411,
    SPELL_STOMP_H                   = 62413,
    SPELL_RUNE_DETONATION           = 62526,

};
#define LOC_Z                       419.5f  
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY ArenaLoc[]=
{
    {2158.082f, -240.572f},
    {2111.883f, -240.561f},
    {2105.243f, -274.499f},
    {2163.927f, -277.834f},
    {2104.865f, -251.027f},
    {2167.612f, -262.128f},
};

struct MANGOS_DLL_DECL boss_thorimAI : public ScriptedAI
{
    boss_thorimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint8 phase;
    bool hasStarted;

    uint32 arenaBerserkTimer;
    uint32 berserkTimer;
    uint32 arenaYellTimer;
    uint32 stormHammerTimer;
    uint32 deafeningThunderTimer;
    uint32 chargeOrbTimer;
    uint32 summonWavesTimer;
    uint32 minibossCheckTimer;

    uint32 chainLightningTimer;
    uint32 lightningChargeTimer;
    uint32 unbalancingStrikeTimer;

    uint32 phase2Timer;
    uint32 hardModeTimer;
    uint8 i;
    uint8 k;

    bool phaseEnd;
    bool isHardMode;

    Creature* Thorim;
    Creature* Sif;

    // intro & outro
    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;
    bool isIntro;
    uint32 IntroTimer;
    uint32 IntroStep;

    // mob list check
    std::list<Creature*> lIronRingGuards;
    std::list<Creature*> lDarkRuneAcolytes;
    std::list<Creature*> lIronHonorGuards;

    void Reset()
    {
        phase = 10;
        hasStarted = false;
        SetCombatMovement(false);

        isHardMode          = true;
        phaseEnd            = false;

        Sif                 = NULL;

        arenaBerserkTimer   = 300000; // 5 min
        berserkTimer        = 300000; // 5 min
        hardModeTimer       = 180000; // 3 min
        arenaYellTimer      = 30000;
        summonWavesTimer    = 10000;

        stormHammerTimer        = 20000;
        deafeningThunderTimer   = 22000;
        chargeOrbTimer          = 15000;

        chainLightningTimer     = 15000;
        lightningChargeTimer    = 50000;
        unbalancingStrikeTimer  = 35000;

        OutroTimer          = 10000;
        Step                = 1;
        isIntro             = true;
        IntroTimer          = 10000;
        IntroStep           = 1;
        isOutro             = false;

        if(GameObject* pLever = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_THORIM_LEVER)))
            pLever->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1); 

        if(pInstance) 
        {
            pInstance->SetData(TYPE_THORIM, NOT_STARTED);

            if(Sif && !Sif->isAlive())
                Sif->ForcedDespawn();

            // respawn ring guards
            GetCreatureListWithEntryInGrid(lIronRingGuards, m_creature, MOB_IRON_RING_GUARD, DEFAULT_VISIBILITY_INSTANCE);
            if (!lIronRingGuards.empty())
            {
                for(std::list<Creature*>::iterator iter = lIronRingGuards.begin(); iter != lIronRingGuards.end(); ++iter)
                {
                    if ((*iter) && !(*iter)->isAlive())
                        (*iter)->Respawn();
                }
            }

            // respawn dark rune acolytes
            GetCreatureListWithEntryInGrid(lDarkRuneAcolytes, m_creature, MOB_DARK_RUNE_ACOLYTE, DEFAULT_VISIBILITY_INSTANCE);
            if (!lDarkRuneAcolytes.empty())
            {
                for(std::list<Creature*>::iterator iter = lDarkRuneAcolytes.begin(); iter != lDarkRuneAcolytes.end(); ++iter)
                {
                    if ((*iter) && !(*iter)->isAlive())
                        (*iter)->Respawn();
                }
            }

            // respawn honor guards
            GetCreatureListWithEntryInGrid(lIronHonorGuards, m_creature, MOB_IRON_HOHOR_GUARD, DEFAULT_VISIBILITY_INSTANCE);
            if (!lIronHonorGuards.empty())
            {
                for(std::list<Creature*>::iterator iter = lIronHonorGuards.begin(); iter != lIronHonorGuards.end(); ++iter)
                {
                    if ((*iter) && !(*iter)->isAlive())
                        (*iter)->Respawn();
                }
            }

            // respawn runic colossus
            if (Creature* pColossus = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_RUNIC_COLOSSUS))))
            {
                if (!pColossus->isAlive())
                    pColossus->Respawn();
            }

            // respawn ancient rune giant
            if (Creature* pRuneGiant = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_RUNE_GIANT))))
            {
                if (!pRuneGiant->isAlive())
                    pRuneGiant->Respawn();
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void DoOutro()
    {
        if(pInstance) 
        {
            pInstance->SetData(TYPE_THORIM, DONE);
            if(isHardMode)
                pInstance->SetData(TYPE_THORIM_HARD, DONE);
        }

        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(pInstance) 
        {
            pInstance->SetData(TYPE_THORIM, DONE);
            if(isHardMode)
                pInstance->SetData(TYPE_THORIM_HARD, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            isOutro = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!isOutro)
        {
            if(pInstance && pInstance->GetData(TYPE_THORIM) == SPECIAL && !hasStarted)
            {
                m_creature->SetInCombatWithZone();
                hasStarted = true;
                if(GameObject* pLever = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_THORIM_LEVER)))
                    pLever->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                phase = 0;
            }
            // phase 0, intro
            if(phase == 0)
            {
                // intro
                if(isIntro)
                {
                    switch(IntroStep)
                    {
                    case 1:
                        DoScriptText(SAY_AGGRO1, m_creature);
                        DoCast(m_creature, SPELL_SHEAT_OF_LIGHTNING);
                        ++IntroStep;
                        IntroTimer = 10000;
                        break;
                    case 3:
                        DoScriptText(SAY_AGGRO2, m_creature);
                        if (pInstance)
                            pInstance->SetData(TYPE_THORIM, IN_PROGRESS);
                        Sif = m_creature->SummonCreature(NPC_SIF, m_creature->GetPositionX() + 10, m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 200000);
                        Sif->setFaction(35);
                        ++IntroStep;
                        IntroTimer = 9000;
                        break;
                    case 5:
                        DoScriptText(SAY_SIF_INTRO, Sif);
                        phase = 1;
                        isIntro = false;
                        ++IntroStep;
                        IntroTimer = 9000;
                        break;
                    }
                }
                else return;

                if (IntroTimer <= diff)
                {
                    ++IntroStep;
                    IntroTimer = 330000;
                } IntroTimer -= diff;
            }

            // phase 1, fight in arena & hallway
            if(phase == 1)
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // hard mode check
                if (hardModeTimer <= diff && isHardMode)
                {
                    isHardMode = false;
                    if(Sif && Sif->isAlive())
                    {
                        DoScriptText(SAY_SIF_DESPAWN, Sif);
                        Sif->ForcedDespawn();
                    }
                    hardModeTimer = 330000;
                } hardModeTimer -= diff;

                // spawn adds in arena
                if(summonWavesTimer < diff)
                {
                    // 1-2 warbringer
                    // 1 evoker
                    // 5-6 commoners
                    // 1 champion
                    // 1 acolyte
                    switch(urand(0, 4))
                    {
                    case 0:
                        i = urand(0, 5);
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_CHAMPION, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                        {
                            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                            {
                                pTemp->AddThreat(pTarget,0.0f);
                                pTemp->AI()->AttackStart(pTarget);
                                pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            }
                        }
                        break;
                    case 1:
                        i = urand(0, 5);
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_EVOKER, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                        {
                            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                            {
                                pTemp->AddThreat(pTarget,0.0f);
                                pTemp->AI()->AttackStart(pTarget);
                                pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            }
                        }
                        break;
                    case 2:
                        i = urand(5, 6);
                        for(uint8 j = 0; j < i; j++)
                        {
                            if(Creature *pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_COMMONER, ArenaLoc[j].x, ArenaLoc[j].y, LOC_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                            {
                                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                                {
                                    pTemp->AddThreat(pTarget,0.0f);
                                    pTemp->AI()->AttackStart(pTarget);
                                    pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                                }
                            }
                        }
                        break;
                    case 3:
                        k = urand(0, 3);
                        i = urand(k + 1, k + 2);
                        for(uint8 j = k; j < i; j++)
                        {
                            if(Creature *pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_WARBRINGER, ArenaLoc[j].x, ArenaLoc[j].y, LOC_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                            {
                                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                                {
                                    pTemp->AddThreat(pTarget,0.0f);
                                    pTemp->AI()->AttackStart(pTarget);
                                    pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                                }
                            }
                        }
                        break;
                    case 4:
                        i = urand(0, 5);
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_DARK_RUNE_ACOLYTE, ArenaLoc[i].x, ArenaLoc[i].y, LOC_Z, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                        {
                            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                            {
                                pTemp->AddThreat(pTarget,0.0f);
                                pTemp->AI()->AttackStart(pTarget);
                                pTemp->GetMotionMaster()->MovePoint(0, 2134.72f, -263.148f, 419.846f);
                            }
                        }
                        break;
                    }
                    summonWavesTimer = urand (7000, 10000);
                }
                else summonWavesTimer -= diff; 

                // phase 1 spells
                // charge orb
                if(chargeOrbTimer < diff)
                {
                    // missing orbs from DB :(
                    //if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    //DoCast(pTarget, SPELL_CHARGE_ORB);
                    chargeOrbTimer = 10000;
                }
                else chargeOrbTimer -= diff; 

                // storm hammer
                if(stormHammerTimer < diff)
                {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(pTarget, SPELL_STORMHAMMER);
                    stormHammerTimer = 15000;
                    deafeningThunderTimer = 3000;
                }
                else stormHammerTimer -= diff; 

                // deafening thunder
                if(deafeningThunderTimer < diff)
                {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(pTarget, SPELL_DEAFENING_THUNDER);
                    deafeningThunderTimer = 20000;
                }
                else deafeningThunderTimer -= diff; 

                if(arenaYellTimer < diff)
                {
                    switch(urand(0, 2))
                    {
                    case 0: DoScriptText(SAY_SPECIAL1, m_creature); break;
                    case 1: DoScriptText(SAY_SPECIAL2, m_creature); break;
                    case 2: DoScriptText(SAY_SPECIAL3, m_creature); break;
                    }
                    arenaYellTimer = 30000;
                }
                else arenaYellTimer -= diff;

                // phase 1 berserk
                if(arenaBerserkTimer < diff)
                {
                    DoScriptText(SAY_ARENA_WIPE, m_creature);
                    // disabled because is cast on players
                    //DoCast(m_creature, SPELL_BERSERK_ADDS);
                    DoCast(m_creature, SPELL_BERSERK);
                    if(Creature *pOrb = m_creature->SummonCreature(MOB_LIGHTNING_ORB, 2123.389f, -440.639f, 438.247f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        pOrb->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        pOrb->SetSpeedRate(MOVE_RUN, 4.0f);
                        pOrb->GetMotionMaster()->MovePoint(0, 2166.806f, -441.160f, 438.246f);
                        pOrb->GetMotionMaster()->MovePoint(1, 2227.267f, -431.599f, 412.177f);
                        pOrb->GetMotionMaster()->MovePoint(2, 2228.072f, -266.417f, 412.177f);

                        // dmg
                        Map *map = m_creature->GetMap();
                        if (map->IsDungeon())
                        {
                            Map::PlayerList const &PlayerList = map->GetPlayers();

                            if (PlayerList.isEmpty())
                                return;

                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                                    i->getSource()->DealDamage(i->getSource(), i->getSource()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, NULL, false);
                            }
                        } 
                    }
                    arenaBerserkTimer = 30000;
                }
                else arenaBerserkTimer -= diff;

                //enter phase 2
                if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 10) && !phaseEnd)
                {
                    if (Creature* pColossus = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_RUNIC_COLOSSUS))))
                    {
                        // check colossus for exploit
                        if(!pColossus->isAlive())
                        {
                            if(Creature* pGiant = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_RUNE_GIANT))))
                            {
                                // check giant for exploit
                                if(!pGiant->isAlive())
                                {
                                    // say
                                    DoScriptText(SAY_JUMP, m_creature);

                                    // move in arena
                                    m_creature->GetMotionMaster()->MovePoint(0, 2134.719f, -263.148f, 419.846f);
                                    m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                                    m_creature->SetSplineFlags(SPLINEFLAG_FALLING); 
                                    phaseEnd = true;
                                    phase2Timer = 9000;
                                }
                                // disable for debug
                                else
                                    EnterEvadeMode();
                            }
                        }
                        // disable for debug
                        else
                            EnterEvadeMode();
                    }
                }

                if(phase2Timer < diff && phaseEnd)
                {
                    m_creature->RemoveSplineFlag(SPLINEFLAG_FALLING);
                    m_creature->RemoveAurasDueToSpell(SPELL_SHEAT_OF_LIGHTNING);
                    if(!isHardMode)
                    {
                        // not working: workaround
                        DoCast(m_creature, SPELL_TOUTCH_OF_DOMINION);
                        m_creature->SetHealth(m_creature->GetHealth() - (m_creature->GetHealth() * 0.25));
                    }
                    if(isHardMode)
                    {
                        //Sif = m_creature->SummonCreature(NPC_SIF, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        Sif->setFaction(14);
                        DoScriptText(SAY_SIF_EVENT, Sif);
                        Sif->SetInCombatWithZone();
                    }
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    SetCombatMovement(true);
                    phase = 2;
                    phaseEnd = false;
                }
                else phase2Timer -= diff;
            }

            if(phase == 2)
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // all spells
                // chain lightning
                if(chainLightningTimer < diff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
                    chainLightningTimer = 10000 + rand()%5000;
                }
                else chainLightningTimer -= diff; 

                // lightning charge
                if(lightningChargeTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_LIGHTNING_CHARGE);
                    lightningChargeTimer = 30000;
                }
                else lightningChargeTimer -= diff; 

                // unbalancing strike
                if(unbalancingStrikeTimer < diff)
                {
                    DoCast(m_creature->getVictim(), SPELL_UNBALANCING_STRIKE);
                    unbalancingStrikeTimer = 30000 + urand(2000, 7000);
                }
                else unbalancingStrikeTimer -= diff; 

                // phase 2 berserk
                if(berserkTimer < diff)
                {
                    DoScriptText(SAY_BERSERK, m_creature);
                    DoCast(m_creature, SPELL_BERSERK);
                    berserkTimer = 30000;
                }
                else berserkTimer -= diff;

                DoMeleeAttackIfReady();
            }
        }
        // outro
        if(isOutro)
        {
            switch(Step)
            {
            case 1:
                Thorim = m_creature;
                Thorim->setFaction(35);
                Thorim->RemoveAllAuras();
                Thorim->DeleteThreatList();
                Thorim->CombatStop(true);
                Thorim->InterruptNonMeleeSpells(false);
                Thorim->SetHealth(Thorim->GetMaxHealth());
                Thorim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++Step;
                OutroTimer = 1000;
                break;
            case 3:
                Thorim->SetOrientation(4.99f);
                DoScriptText(SAY_DEATH, Thorim);
                ++Step;
                OutroTimer = 3000;
                break;
            case 5:
                if(isHardMode)
                {
                    DoScriptText(SAY_OUTRO_HARD1, Thorim);
                    DoCast(Sif, SPELL_STORMHAMMER);
                }
                else
                    DoScriptText(SAY_OUTRO1, Thorim);
                ++Step;
                OutroTimer = 1000;
                break;
            case 7:
                if(isHardMode)
                {
                    //summon a tentacule
                    if(Creature *tentacule = m_creature->SummonCreature(34266, Sif->GetPositionX(), Sif->GetPositionY(), Sif->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 7000))
                    {
                        tentacule->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        tentacule->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                    Sif->SetVisibility(VISIBILITY_OFF);
                    Sif->setFaction(35);
                    Sif->RemoveAllAuras();
                    Sif->DeleteThreatList();
                    Sif->CombatStop(true);
                    Sif->InterruptNonMeleeSpells(false);
                }
                ++Step;
                OutroTimer = 7000;
                break;
            case 9:
                if(isHardMode)
                    DoScriptText(SAY_OUTRO_HARD2, Thorim);
                else
                    DoScriptText(SAY_OUTRO2, Thorim);
                ++Step;
                OutroTimer = 13000;
                break;
            case 11:
                if(isHardMode)
                    DoScriptText(SAY_OUTRO_HARD3, Thorim);
                else
                    DoScriptText(SAY_OUTRO3, Thorim);
                ++Step;
                OutroTimer = 15000;
                break;
            case 13:
                DoOutro();
                ++Step;
                OutroTimer = 10000;
                break;
            }
        }
        else return;

        if (OutroTimer <= diff)
        {
            ++Step;
            OutroTimer = 330000;
        } OutroTimer -= diff;
    }
};

CreatureAI* GetAI_boss_thorim(Creature* pCreature)
{
    return new boss_thorimAI(pCreature);
}

struct MANGOS_DLL_DECL boss_runic_colossusAI : public ScriptedAI
{
    boss_runic_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint32 spellTimer;
    uint32 runicBarrierTimer;

    void Reset()
    {
        spellTimer = 10000;
        runicBarrierTimer = 15000;

        if(pInstance) 
            pInstance->SetData(TYPE_RUNIC_COLOSSUS, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) 
            pInstance->SetData(TYPE_RUNIC_COLOSSUS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) 
            pInstance->SetData(TYPE_RUNIC_COLOSSUS, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                DoCast(m_creature->getVictim(), SPELL_SMASH);
                break;
            case 1:
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_CHARGE : SPELL_CHARGE_H);
                break;
            }
            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        if (runicBarrierTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_RUNIC_BARRIER);
            runicBarrierTimer = 15000 + urand(1000, 5000);
        }else runicBarrierTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_runic_colossus(Creature* pCreature)
{
    return new boss_runic_colossusAI(pCreature);
}

struct MANGOS_DLL_DECL boss_ancient_rune_giantAI : public ScriptedAI
{
    boss_ancient_rune_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 10000;

        if(pInstance) 
            pInstance->SetData(TYPE_RUNE_GIANT, NOT_STARTED);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) 
            pInstance->SetData(TYPE_RUNE_GIANT, DONE);
    }

    void Aggro(Unit *who) 
    {
        // should be cast on adds!!!
        //DoCast(m_creature, SPELL_RUNIC_FORTIFICATION);

        if(pInstance) 
            pInstance->SetData(TYPE_RUNE_GIANT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                DoCast(m_creature->getVictim(), m_bIsRegularMode? SPELL_STOMP : SPELL_STOMP_H);
                break;
            case 1:
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, SPELL_RUNE_DETONATION);
                break;
            }
            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ancient_rune_giant(Creature* pCreature)
{
    return new boss_ancient_rune_giantAI(pCreature);
}

struct MANGOS_DLL_DECL npc_sifAI : public ScriptedAI
{
    npc_sifAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 2))
            {
            case 0:
                DoCast(m_creature->getVictim(), m_bIsRegularMode? SPELL_FROSTBOLT_VOLLEY : SPELL_FROSTBOLT_VOLLEY_H);
                break;
            case 1:
                DoCast(m_creature, m_bIsRegularMode? SPELL_FROST_NOVA : SPELL_FROST_NOVA_H);
                break;
            case 2:
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, m_bIsRegularMode? SPELL_BLIZZARD : SPELL_BLIZZARD_H);
                break;
            }

            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_sif(Creature* pCreature)
{
    return new npc_sifAI(pCreature);
}

void AddSC_boss_thorim()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_thorim";
    newscript->GetAI = &GetAI_boss_thorim;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_runic_colossus";
    newscript->GetAI = &GetAI_boss_runic_colossus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ancient_rune_giant";
    newscript->GetAI = &GetAI_boss_ancient_rune_giant;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sif";
    newscript->GetAI = &GetAI_npc_sif;
    newscript->RegisterSelf();
}
