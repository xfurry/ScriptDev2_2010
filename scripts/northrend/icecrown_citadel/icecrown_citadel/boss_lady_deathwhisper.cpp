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
SDName: boss_lady_deathwhisper
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_INTRO1                 = -1609407,
    SAY_INTRO2                 = -1609408,
    SAY_INTRO3                 = -1609402,
    SAY_INTRO4                 = -1609403,
    SAY_INTRO5                 = -1609404,
    SAY_INTRO6                 = -1609405,
    SAY_INTRO7                 = -1609406,

    SAY_AGGRO                  = -1609409,
    SAY_PHASE2                 = -1609410,
    SAY_DOMINATEMIND           = -1609414,
    SAY_DARKEMPOWERMENT        = -1609411,
    SAY_DARKTRANSFORMATION     = -1609412,
    SAY_ANIMATEDEAD            = -1609413,
    SAY_KILL1                  = -1609417,
    SAY_KILL2                  = -1609418,
    SAY_BERSERK                = -1609415,
    SAY_DEATH                  = -1609416,

    SPELL_MANA_BARRIER          = 70842,
    SPELL_SHADOW_BOLT_10        = 71254,
    SPELL_SHADOW_BOLT_25        = 72008,
    SPELL_SHADOW_BOLT_10HC      = 72503,
    SPELL_SHADOW_BOLT_25HC      = 72504,
    SPELL_DEATH_AND_DECAY_10    = 71001,
    SPELL_DEATH_AND_DECAY_25    = 72108,
    SPELL_DEATH_AND_DECAY_10HC  = 72109,
    SPELL_DEATH_AND_DECAY_25HC  = 72110,
    SPELL_DARK_EMPOWERMENT      = 70901,    // on adherents
    SPELL_DARK_TRANSFORMATION   = 70900,    // on fanatics
    SPELL_DOMINATE_MIND         = 71289,
    SPELL_FROSTBOLT_10          = 71420,
    SPELL_FROSTBOLT_25          = 72007,
    SPELL_FROSTBOLT_10HC        = 72501,
    SPELL_FROSTBOLT_25HC        = 72502,
    SPELL_INSIGNIFICANCE        = 71204,
    SPELL_FROSTBOLT_VOLLEY_10   = 72905,
    SPELL_FROSTBOLT_VOLLEY_25   = 72906,
    SPELL_FROSTBOLT_VOLLEY_10HC = 72907,
    SPELL_FROSTBOLT_VOLLEY_25HC = 72908,
    SPELL_SUMMON_SHADE          = 71426,
    SPELL_BERSERK               = 26662,

    // vengefull shade
    SPELL_VENGEFUL_BLAST_10     = 71544,
    SPELL_VENGEFUL_BLAST_25     = 72010,
    SPELL_VENGEFUL_BLAST_10HC   = 72011,
    SPELL_VENGEFUL_BLAST_25HC   = 72012,

    // adherent
    SPELL_SHROUD_OF_THE_OCCULT      = 70768,
    SPELL_CURSE_OF_TORPOR           = 71237,
    SPELL_DARK_MARTYRDOM_10         = 70903,
    SPELL_DARK_MARTYRDOM_25         = 72498,
    SPELL_DARK_MARTYRDOM_10HC       = 72499,
    SPELL_DARK_MARTYRDOM_25HC       = 72500,
    SPELL_DEATHCHILL_BLAST_10       = 70906,
    SPELL_DEATHCHILL_BLAST_25       = 72485,
    SPELL_DEATHCHILL_BLAST_10HC     = 72486,
    SPELL_DEATHCHILL_BLAST_25HC     = 72487,
    SPELL_DEATHCHILL_BOLT_10        = 70594,
    SPELL_DEATHCHILL_BOLT_25        = 72005,
    SPELL_DEATHCHILL_BOLT_10HC      = 72488,
    SPELL_DEATHCHILL_BOLT_25HC      = 72489,
    SPELL_ADHERENTS_DETERMINATION   = 71234,
    SPELL_FROST_FEVER_10            = 67719,
    SPELL_FROST_FEVER_25            = 67934,
    SPELL_FROST_FEVER_10HC          = 71129,
    SPELL_FROST_FEVER_25HC          = 67878,

    // fanatic
    SPELL_NECROTIC_STRIKE_10        = 70659,
    SPELL_NECROTIC_STRIKE_25        = 72490,
    SPELL_NECROTIC_STRIKE_10HC      = 72491,
    SPELL_NECROTIC_STRIKE_25HC      = 72492,
    SPELL_FANATICS_DETERMINATION    = 71235,
    SPELL_VAMPIRIC_MIGHT            = 70674,
    SPELL_SHADOW_CLEAVE_10          = 70670,
    SPELL_SHADOW_CLEAVE_25          = 72006,
    SPELL_SHADOW_CLEAVE_10HC        = 72493,
    SPELL_SHADOW_CLEAVE_25HC        = 72494,

    // npc
    NPC_CULT_ADHERENT          = 37949,
    NPC_CULT_FANATIC           = 37890,
    NPC_VENGEFUL_SHADE         = 38222,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};
 
static Locations SpawnLoc[]=
{
    {-619.006f, 2158.104f, 50.848f},
    {-598.697f, 2157.767f, 50.848f},
    {-577.992f, 2156.989f, 50.848f},
    {-618.748f, 2266.648f, 50.849f},
    {-598.573f, 2266.870f, 50.849f},
    {-578.360f, 2267.210f, 50.849f},
    {-526.018f, 2211.791f, 62.746f},
};

struct MANGOS_DLL_DECL boss_deathwhisperAI : public ScriptedAI
{
    boss_deathwhisperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsIntroDone = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiShadowBolt_Timer;
    uint32 m_uiDeathAndDecay_Timer;
    uint32 m_uiFrostbolt_Timer;
    uint32 m_uiFrostboltVolley_Timer;
    uint32 m_uiInsignificance_Timer;
    uint32 m_uiSummon_Cult_Timer;
    uint32 m_uiSummon_Shade_Timer;
    uint32 m_uiDeterminationTimer;
    uint32 m_uiDominateTimer;
    uint32 m_uiBerserkTimer;

    uint8 m_uiPhase;
    uint8 m_uiSummon_Cult_Count;
    uint8 m_uiSummon_Shade_Count;
    bool m_bSpawnLeft;
    uint32 m_uiIntroTimer;
    uint32 m_uiIntroStep;
    bool m_bIsIntroDone;
    bool m_bIsIntro;

    void Reset()
    {
        m_uiShadowBolt_Timer    = 5000;
        m_uiDeathAndDecay_Timer = 30000;
        m_uiSummon_Cult_Timer   = 20000;
        m_uiFrostbolt_Timer     = 15000;
        m_uiFrostboltVolley_Timer = 40000;
        m_uiInsignificance_Timer = 5000+rand()%40000;
        m_uiSummon_Shade_Timer  = 25000;
        m_uiDeterminationTimer  = 30000;
        m_uiDominateTimer       = 40000;
        m_uiBerserkTimer        = 600000;  // 10 min
        m_uiPhase               = 1;
        m_uiSummon_Cult_Count   = 0;
        m_uiSummon_Shade_Count  = 0;
        m_bSpawnLeft            = true;
        m_uiIntroTimer          = 1000;
        m_uiIntroStep           = 0;
        m_bIsIntro              = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHWHISPER, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
        DoCast(m_creature, SPELL_MANA_BARRIER);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_DEATHWHISPER, DONE);
            //m_pInstance->SetData(TYPE_GUNSHIP_BATTLE, DONE);    // temp until event implemented
        }
        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHWHISPER, FAIL);

        if(m_creature->HasAura(SPELL_MANA_BARRIER))
            m_creature->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if(!m_bIsIntro && !m_bIsIntroDone)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_bIsIntro = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);  
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    //Mana Barrier is bugged so we override it for now
    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if (m_creature->HasAura(SPELL_MANA_BARRIER))
        {
            m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA) - uiDamage);
            m_creature->SetHealth(m_creature->GetMaxHealth());
            uiDamage = 0;
        }
    }

    Creature* SelectRandomFanatic(float fRange)
    {
        std::list<Creature* > lCultistList;
        GetCreatureListWithEntryInGrid(lCultistList, m_creature, NPC_CULT_FANATIC, fRange);

        //This should not appear!
        if (lCultistList.empty()){
            m_uiDeterminationTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lCultistList.begin();
        advance(iter, urand(0, lCultistList.size()-1));

        return *iter;
    }

    Creature* SelectRandomAdherent(float fRange)
    {
        std::list<Creature* > lCultistList;
        GetCreatureListWithEntryInGrid(lCultistList, m_creature, NPC_CULT_ADHERENT, fRange);

        //This should not appear!
        if (lCultistList.empty()){
            m_uiDeterminationTimer = 30000;
            return NULL;
        }

        std::list<Creature* >::iterator iter = lCultistList.begin();
        advance(iter, urand(0, lCultistList.size()-1));

        return *iter;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro && !m_bIsIntroDone)
        {
            switch(m_uiIntroStep)
            {
            case 1:
                DoScriptText(SAY_INTRO1, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 15000;
                break;
            case 3:
                DoScriptText(SAY_INTRO2, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 5:
                DoScriptText(SAY_INTRO3, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 7:
                DoScriptText(SAY_INTRO4, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 9:
                DoScriptText(SAY_INTRO5, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 11:
                DoScriptText(SAY_INTRO6, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            case 13:
                DoScriptText(SAY_INTRO7, m_creature);
                ++m_uiIntroStep;
                m_uiIntroTimer = 17000;
                break;
            case 15:
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_uiPhase = 1;
                m_bIsIntro = false;
                m_bIsIntroDone = true;
                ++m_uiIntroStep;
                m_uiIntroTimer = 10000;
                break;
            }

            if (m_uiIntroTimer <= uiDiff)
            {
                ++m_uiIntroStep;
                m_uiIntroTimer = 330000;
            } m_uiIntroTimer -= uiDiff;
        }

        if (!m_bIsIntroDone) 
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPhase == 1)
        {
            if ((m_creature->GetPower(POWER_MANA)*100 / m_creature->GetMaxPower(POWER_MANA)) < 1)
            {
                DoScriptText(SAY_PHASE2, m_creature);
                if (m_creature->HasAura(SPELL_MANA_BARRIER))
                    m_creature->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
                DoStartMovement(m_creature->getVictim());
                m_uiSummon_Cult_Timer = 20000;
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, CREATURE_FLAG_EXTRA_NOT_TAUNTABLE);   // not sure if it's right
                m_uiPhase = 2;
                return;
            }

            if (m_uiSummon_Cult_Timer < uiDiff)
            {
                // summon adds only on the sides of the room
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    if (m_bSpawnLeft)
                    {
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[0].x,SpawnLoc[0].y,SpawnLoc[0].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[1].x,SpawnLoc[1].y,SpawnLoc[1].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[2].x,SpawnLoc[2].y,SpawnLoc[2].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_bSpawnLeft = false;
                    }
                    else
                    {
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[3].x,SpawnLoc[3].y,SpawnLoc[3].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[4].x,SpawnLoc[4].y,SpawnLoc[4].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[5].x,SpawnLoc[5].y,SpawnLoc[5].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_bSpawnLeft = true;
                    }
                    m_uiSummon_Cult_Count += 3;
                }
                // summon adds on both sides of the room + one at the entrance
                else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                {
                    m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[0].x,SpawnLoc[0].y,SpawnLoc[0].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[1].x,SpawnLoc[1].y,SpawnLoc[1].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[2].x,SpawnLoc[2].y,SpawnLoc[2].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[3].x,SpawnLoc[3].y,SpawnLoc[3].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[4].x,SpawnLoc[4].y,SpawnLoc[4].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[5].x,SpawnLoc[5].y,SpawnLoc[5].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                    switch (urand(0, 1))
                    {
                    case 0:
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[6].x,SpawnLoc[6].y,SpawnLoc[6].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        break;
                    case 1:
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[6].x,SpawnLoc[6].y,SpawnLoc[6].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        break;
                    }
                    m_uiSummon_Cult_Count += 7;
                }
                m_uiSummon_Cult_Timer = 60000;
            }
            else m_uiSummon_Cult_Timer -= uiDiff;

            if(m_uiDeterminationTimer < uiDiff)
            {
                switch (urand(0,1))
                {
                case 0:
                    if (Unit* pCultist = SelectRandomAdherent(80.0f))
                    {
                        pCultist->CastSpell(pCultist, SPELL_DARK_EMPOWERMENT, false);
                        DoScriptText(SAY_DARKEMPOWERMENT, m_creature);
                    }
                    break;
                case 1:
                    if (Unit* pCultist = SelectRandomFanatic(80.0f))
                    {
                        pCultist->CastSpell(pCultist, SPELL_DARK_TRANSFORMATION, false);
                        DoScriptText(SAY_DARKTRANSFORMATION, m_creature);
                    }
                    break;
                }
                m_uiDeterminationTimer = 30000;
            }
            else m_uiDeterminationTimer -= uiDiff;

            if (m_uiShadowBolt_Timer < uiDiff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(target, SPELL_SHADOW_BOLT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(target, SPELL_SHADOW_BOLT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(target, SPELL_SHADOW_BOLT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(target, SPELL_SHADOW_BOLT_25HC);
                }
                m_uiShadowBolt_Timer = 5000;
            }
            else m_uiShadowBolt_Timer -= uiDiff;

            DoStartNoMovement(m_creature->getVictim());
        }
        else if (m_uiPhase == 2)
        {
            if (m_uiInsignificance_Timer < uiDiff)
            {
                 DoCastSpellIfCan(m_creature->getVictim(), SPELL_INSIGNIFICANCE);
                m_uiInsignificance_Timer = 5000+rand()%40000;
            }
            else m_uiInsignificance_Timer -= uiDiff;

            if (m_uiFrostboltVolley_Timer < uiDiff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(target, SPELL_FROSTBOLT_VOLLEY_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(target, SPELL_FROSTBOLT_VOLLEY_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(target, SPELL_FROSTBOLT_VOLLEY_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(target, SPELL_FROSTBOLT_VOLLEY_25HC);
                }
                m_uiFrostboltVolley_Timer = 40000;
            }
            else m_uiFrostboltVolley_Timer -= uiDiff;

            if (m_uiSummon_Shade_Count < m_uiSummon_Cult_Count)
            {
                if (m_uiSummon_Shade_Timer < uiDiff)
                {
                    DoScriptText(SAY_ANIMATEDEAD, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_SUMMON_SHADE);
                    m_uiSummon_Shade_Count++;
                    m_uiSummon_Shade_Timer = 25000;
                }
                else m_uiSummon_Shade_Timer -= uiDiff;
            }

            if (m_uiFrostbolt_Timer < uiDiff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(target, SPELL_FROSTBOLT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(target, SPELL_FROSTBOLT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(target, SPELL_FROSTBOLT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(target, SPELL_FROSTBOLT_25HC);
                }
                m_uiFrostbolt_Timer = 15000;
            }
            else m_uiFrostbolt_Timer -= uiDiff;

            // summon 3 cultist on 25 heroic
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                if (m_uiSummon_Cult_Timer < uiDiff)
                {
                    if (m_bSpawnLeft)
                    {
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[0].x,SpawnLoc[0].y,SpawnLoc[0].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[1].x,SpawnLoc[1].y,SpawnLoc[1].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[2].x,SpawnLoc[2].y,SpawnLoc[2].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_bSpawnLeft = false;
                    }
                    else
                    {
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[3].x,SpawnLoc[3].y,SpawnLoc[3].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[4].x,SpawnLoc[4].y,SpawnLoc[4].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[5].x,SpawnLoc[5].y,SpawnLoc[5].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        m_bSpawnLeft = true;
                    }

                    m_uiSummon_Cult_Count += 3;
                    m_uiSummon_Cult_Timer = 60000;
                }
                else m_uiSummon_Cult_Timer -= uiDiff;
            }
            // summon only 1 cultist on heroic mode at entrance
            else if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            {
                if (m_uiSummon_Cult_Timer < uiDiff)
                {
                    switch (urand(0, 1))
                    {
                    case 0:
                        m_creature->SummonCreature(NPC_CULT_ADHERENT,SpawnLoc[6].x,SpawnLoc[6].y,SpawnLoc[6].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        break;
                    case 1:
                        m_creature->SummonCreature(NPC_CULT_FANATIC,SpawnLoc[6].x,SpawnLoc[6].y,SpawnLoc[6].z,0,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
                        break;
                    }
                    m_uiSummon_Cult_Count++;
                    m_uiSummon_Cult_Timer = 60000;
                }
                else m_uiSummon_Cult_Timer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }

        if (m_uiDeathAndDecay_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(target, SPELL_DEATH_AND_DECAY_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(target, SPELL_DEATH_AND_DECAY_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(target, SPELL_DEATH_AND_DECAY_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(target, SPELL_DEATH_AND_DECAY_25HC);
            }
            m_uiDeathAndDecay_Timer = 30000;
        }
        else m_uiDeathAndDecay_Timer -= uiDiff;

        if(Difficulty != RAID_DIFFICULTY_10MAN_NORMAL)
        {
            if(m_uiDominateTimer < uiDiff)
            {
                uint8 m_uiMaxPlayers = 0;
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || RAID_DIFFICULTY_25MAN_NORMAL)
                    m_uiMaxPlayers = 1;
                else if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    m_uiMaxPlayers = 3;
                DoScriptText(SAY_DOMINATEMIND, m_creature);
                for(uint8 i = 0; i < m_uiMaxPlayers; i++)
                {
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_DOMINATE_MIND);
                }
                m_uiDominateTimer = 30000;
            }
            else m_uiDominateTimer -= uiDiff;
        }

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_deathwhisper(Creature* pCreature)
{
    return new boss_deathwhisperAI(pCreature);
}

struct MANGOS_DLL_DECL mob_vengeful_shadeAI : public ScriptedAI
{
    mob_vengeful_shadeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiDeathTimer;
	bool m_bHasCasted;

    void Reset()
    {
        m_uiDeathTimer	= 600000;
		m_bHasCasted	= false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 5) && !m_bHasCasted)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_VENGEFUL_BLAST_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_VENGEFUL_BLAST_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_VENGEFUL_BLAST_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_VENGEFUL_BLAST_25HC);

            m_uiDeathTimer = 500;
			m_bHasCasted = true;
        }

        if (m_uiDeathTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDeathTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_vengeful_shade(Creature* pCreature)
{
    return new mob_vengeful_shadeAI (pCreature);
}

struct MANGOS_DLL_DECL mob_cult_adherentAI : public ScriptedAI
{
    mob_cult_adherentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiFrostFeverTimer;
    uint32 m_uiDeathchillBoltTimer;
    uint32 m_uiDeathchillBlastTimer;
    uint32 m_uiMartyrdomTimer;
    uint32 m_uiTorporTimer;
    uint32 m_uiShroudOfOccultTimer;
    uint32 m_uiAdherentsDeterminationTimer;

    void Reset()
    {
        m_uiFrostFeverTimer         = 5000;
        m_uiDeathchillBoltTimer     = 3000;
        m_uiDeathchillBlastTimer    = 25000;
        m_uiMartyrdomTimer          = 50000;
        m_uiTorporTimer             = 10000;
        m_uiShroudOfOccultTimer     = 30000;
        m_uiAdherentsDeterminationTimer = 20000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostFeverTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(target, SPELL_FROST_FEVER_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(target, SPELL_FROST_FEVER_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(target, SPELL_FROST_FEVER_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(target, SPELL_FROST_FEVER_25HC);
            }
            m_uiFrostFeverTimer = 10000;
        }
        else
            m_uiFrostFeverTimer -= uiDiff;

        if (m_uiDeathchillBoltTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(target, SPELL_DEATHCHILL_BOLT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(target, SPELL_DEATHCHILL_BOLT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(target, SPELL_DEATHCHILL_BOLT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(target, SPELL_DEATHCHILL_BOLT_25HC);
            }
            m_uiDeathchillBoltTimer = 11000;
        }
        else
            m_uiDeathchillBoltTimer -= uiDiff;

        if (m_uiDeathchillBlastTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_DEATHCHILL_BLAST_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_DEATHCHILL_BLAST_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_DEATHCHILL_BLAST_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_DEATHCHILL_BLAST_25HC);
            m_uiDeathchillBlastTimer = 15000;
        }
        else
            m_uiDeathchillBlastTimer -= uiDiff;

        if (m_uiMartyrdomTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_25HC);
            m_uiMartyrdomTimer = 60000;
        }
        else
            m_uiMartyrdomTimer -= uiDiff;

        if (m_uiTorporTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_CURSE_OF_TORPOR);
            m_uiTorporTimer = 15000;
        }
        else
            m_uiTorporTimer -= uiDiff;

        if (m_uiShroudOfOccultTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_SHROUD_OF_THE_OCCULT);
            m_uiShroudOfOccultTimer = 30000;
        }
        else
            m_uiShroudOfOccultTimer -= uiDiff;

        if (m_uiAdherentsDeterminationTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_ADHERENTS_DETERMINATION);
            m_uiAdherentsDeterminationTimer = 30000;
        }
        else
            m_uiAdherentsDeterminationTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_cult_adherent(Creature* pCreature)
{
    return new mob_cult_adherentAI (pCreature);
}

struct MANGOS_DLL_DECL mob_cult_fanaticAI : public ScriptedAI
{
    mob_cult_fanaticAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiShadowCleaveTimer;
    uint32 m_uiVampiricMightTimer;
    uint32 m_uiNecroticStrikeTimer;
    uint32 m_uiFanaticDeterminationTimer;
    uint32 m_uiMartyrdomTimer;

    void Reset()
    {
        m_uiShadowCleaveTimer           = 5000;
        m_uiVampiricMightTimer          = 13000;
        m_uiNecroticStrikeTimer         = 10000;
        m_uiFanaticDeterminationTimer   = 20000;
        m_uiMartyrdomTimer              = 50000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShadowCleaveTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CLEAVE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CLEAVE_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CLEAVE_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CLEAVE_25HC);
            m_uiShadowCleaveTimer = 6000;
        }
        else
            m_uiShadowCleaveTimer -= uiDiff;

        if (m_uiNecroticStrikeTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_NECROTIC_STRIKE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_NECROTIC_STRIKE_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_NECROTIC_STRIKE_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_NECROTIC_STRIKE_25HC);
            m_uiNecroticStrikeTimer = 10000;
        }
        else
            m_uiNecroticStrikeTimer -= uiDiff;

        if (m_uiMartyrdomTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_DARK_MARTYRDOM_25HC);
            m_uiMartyrdomTimer = 60000;
        }
        else
            m_uiMartyrdomTimer -= uiDiff;

        if (m_uiVampiricMightTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_VAMPIRIC_MIGHT);
            m_uiVampiricMightTimer = 20000;
        }
        else
            m_uiVampiricMightTimer -= uiDiff;

        if (m_uiFanaticDeterminationTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_FANATICS_DETERMINATION);
            m_uiFanaticDeterminationTimer = 30000;
        }
        else
            m_uiFanaticDeterminationTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_cult_fanatic(Creature* pCreature)
{
    return new mob_cult_fanaticAI (pCreature);
}

void AddSC_boss_deathwhisper()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_deathwhisper";
    NewScript->GetAI = &GetAI_boss_deathwhisper;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_vengeful_shade";
    NewScript->GetAI = &GetAI_mob_vengeful_shade;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_cult_adherent";
    NewScript->GetAI = &GetAI_mob_cult_adherent;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_cult_fanatic";
    NewScript->GetAI = &GetAI_mob_cult_fanatic;
    NewScript->RegisterSelf();
}