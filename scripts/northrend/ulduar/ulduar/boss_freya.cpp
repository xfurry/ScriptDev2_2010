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
SDName: boss_freya
SD%Complete: 80%
SDComment: fix outro, makes server crash :|
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    /* YELLS */
    // freya
    SAY_AGGRO               = -1603000,
    SAY_AGGRO_HARD          = -1603001,
    SAY_SUMMON1             = -1603002,
    SAY_SUMMON2             = -1603003,
    SAY_SUMMON3             = -1603004,
    SAY_SLAY1               = -1603005,
    SAY_SLAY2               = -1603006,
    SAY_DEATH               = -1603007,
    SAY_BERSERK             = -1603008,
    //brightleaf
    SAY_BRIGHTLEAF_AGGRO    = -1603160,
    SAY_BRIGHTLEAF_SLAY1    = -1603161,
    SAY_BRIGHTLEAF_SLAY2    = -1603162,
    SAY_BRIGHTLEAF_DEATH    = -1603163,
    //Ironbranch
    SAY_IRONBRANCH_AGGRO    = -1603170,
    SAY_IRONBRANCH_SLAY1    = -1603171,
    SAY_IRONBRANCH_SLAY2    = -1603172,
    SAY_IRONBRANCH_DEATH    = -1603173,
    //Stonebark
    SAY_STONEBARK_AGGRO     = -1603180,
    SAY_STONEBARK_SLAY1     = -1603181,
    SAY_STONEBARK_SLAY2     = -1603182,
    SAY_STONEBARK_DEATH     = -1603183,

    /* BOSS SPELLS */
    SPELL_ATTUNED_TO_NATURE        = 62519, //increases healing, start at 150 stacks
    SPELL_TOUCH_OF_EONAR           = 62528, //heals Freya, 6k per second
    SPELL_TOUCH_OF_EONAR_H         = 62892, //heals Freya, 24k per second
    SPELL_SUNBEAM                  = 62623,
    SPELL_SUNBEAM_H                = 62872,
    SPELL_BERSERK                  = 47008, // 10 min

    /* HARD MODE SPELLS */
    SPELL_DRAINED_OF_POWER      = 62467,
    // brightleaf
    SPELL_UNSTABLE_ENERGY_FREYA = 62451,
    SPELL_UNSTABLE_ENERGY_FREYA_H=62865,
    SPELL_BRIGHTLEAFS_ESSENCE   = 62968, //62385,
    // ironbrach
    SPELL_STRENGHTEN_IRON_ROOTS = 63601,
    MOB_STRENGHTEN_IRON_ROOTS   = 33168,
    SPELL_IRON_ROOTS_FREYA      = 62283,
    SPELL_IRON_ROOTS_FREYA_H    = 62930,
    SPELL_IRONBRACHS_ESSENCE    = 62713, //62387,
    // stonebark
    SPELL_GROUND_TREMOR_FREYA   = 62437,
    SPELL_GROUND_TREMOR_FREYA_H = 62859,
    SPELL_STONEBARKS_ESSENCE    = 65590, //62386,


    // sanctuary adds 
    MOB_EONARS_GIFT             = 33228,
    SPELL_LIFEBINDERS_GIFT      = 62584,    // after 12 secs, heals Freya & her allies for 30%
    SPELL_LIFEBINDERS_GIFT_H    = 64185,    // the same but for 60%
    SPELL_PHEROMONES            = 62619,    // protects from conservators grip
    MOB_HEALTHY_SPORE           = 33215,

    /* ADDS */
    // 6 waves of adds. 1 of the 3 each min 
    MOB_DETONATING_LASHER       = 32918,    // recude 2 stacks
    // spells
    SPELL_FLAME_LASH            = 62608,
    SPELL_DETONATE              = 62598,
    SPELL_DETONATE_H            = 62937, 

    MOB_ANCIENT_CONSERVATOR     = 33203,    // reduce 30 stacks
    //spells
    SPELL_CONSERVATORS_GRIP     = 62532,
    SPELL_NATURES_FURRY         = 62589,
    SPELL_NATURES_FURRY_H       = 63571,

    /* elemental adds */                    // each one reduces 10 stacks
    MOB_WATER_SPIRIT            = 33202,
    // spells
    SPELL_TIDAL_WAVE            = 62653,

    MOB_STORM_LASHER            = 32919,
    // spells
    SPELL_STORMBOLT             = 62649,
    SPELL_STORMBOLT_H           = 62938,
    SPELL_LIGHTNING_LASH        = 62648,    // 3 targets
    SPELL_LIGHTNING_LASH_H      = 62939,    // 5 targets

    MOB_SNAPLASHER              = 32916,
    // spells
    SPELL_HARDEND_BARK          = 62663,
    SPELL_HARDEND_BARK_H        = 64190,

    // nature bomb
    MOB_NATURE_BOMB             = 34129,
    SPELL_NATURE_BOMB           = 64587,
    SPELL_NATURE_BOMB_H         = 64650,

    /* ELDERS */                            // used in phase 1
    ELDER_BRIGHTLEAF            = 32915,
    ELDER_IRONBRANCH             = 32913,
    ELDER_STONEBARK             = 32914,

    // brightleaf spells
    SPELL_BRIGHTLEAF_FLUX       = 62262,
    SPELL_SOLAR_FLARE           = 62240,
    SPELL_SOLAR_FLARE_H         = 62920,
    SPELL_UNSTABLE_SUN_BEAN     = 62243,
    SPELL_UNSTABLE_ENERGY       = 62217,
    SPELL_UNSTABLE_ENERGY_H     = 62922,
    SPELL_PHOTOSYNTESIS         = 62209,

    // ironbrach spells
    SPELL_IMPALE                = 62310,
    SPELL_IMPALE_H              = 62928,
    SPELL_IRON_ROOTS            = 62438,
    SPELL_IRON_ROOTS_H          = 62861,
    MOB_IRON_ROOTS              = 33088,
    SPELL_THORM_SWARM           = 62285,
    SPELL_THORM_SWARM_H         = 62931,
    SPELL_STUN				    = 56,

    // stonebark spells
    SPELL_FIST_OF_STONE         = 62344,
    SPELL_BROKEN_BONES          = 62356,
    SPELL_GROUND_TREMOR         = 62325,
    SPELL_GROUND_TREMOR_H       = 62932,
    SPELL_PETRIFIED_BARK        = 62337,
    SPELL_PETRIFIED_BARK_H      = 62933,
};

class MANGOS_DLL_DECL AttunedToNatureAura : public Aura
{
public:
    AttunedToNatureAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};

// Brightleaf
struct MANGOS_DLL_DECL boss_elder_brightleafAI : public ScriptedAI
{
    boss_elder_brightleafAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 BrightleafFluxTimer;
    uint32 SolarFlareTimer;
    uint32 UnstableSunBeanTimer;
    uint32 UnstabelEnergyTimer;
    uint32 PhotosyntesisTimer;
    uint32 PhotosyntesisEndTimer;

    void Reset()
    {
        BrightleafFluxTimer     = 5000;
        SolarFlareTimer         = 10000 + urand(1000, 5000);
        UnstableSunBeanTimer    = 15000;
        UnstabelEnergyTimer     = 20000;
        PhotosyntesisTimer      = 30000;
        PhotosyntesisEndTimer   = 5000;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_BRIGHTLEAF_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_BRIGHTLEAF_SLAY1, m_creature);
        else
            DoScriptText(SAY_BRIGHTLEAF_SLAY2, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_BRIGHTLEAF_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(BrightleafFluxTimer < diff)
        {
            DoCast(m_creature, SPELL_BRIGHTLEAF_FLUX);
            BrightleafFluxTimer = 5000;
        }
        else BrightleafFluxTimer -= diff;

        if(SolarFlareTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SOLAR_FLARE : SPELL_SOLAR_FLARE_H);
            SolarFlareTimer = 10000 + urand(1000, 5000);
        }
        else SolarFlareTimer -= diff;

        if(UnstableSunBeanTimer < diff)
        {
            DoCast(m_creature, SPELL_UNSTABLE_SUN_BEAN);
            UnstableSunBeanTimer = urand(15000, 20000);
        }
        else UnstableSunBeanTimer -= diff;

        if(UnstabelEnergyTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY: SPELL_UNSTABLE_ENERGY_H);
            UnstabelEnergyTimer = urand(17000, 23000);
        }
        else UnstabelEnergyTimer -= diff;

        if(PhotosyntesisTimer < diff)
        {
            DoCast(m_creature, SPELL_PHOTOSYNTESIS);
            PhotosyntesisTimer = 30000;
            PhotosyntesisEndTimer = 5000;
        }
        else PhotosyntesisTimer -= diff;

        if(PhotosyntesisEndTimer < diff)
        {
            m_creature->RemoveAurasDueToSpell(SPELL_PHOTOSYNTESIS);
            PhotosyntesisEndTimer = 35000;
        }
        else PhotosyntesisEndTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_elder_brightleaf(Creature* pCreature)
{
    return new boss_elder_brightleafAI(pCreature);
}

// Ironbranch
struct MANGOS_DLL_DECL boss_elder_ironbranchAI : public ScriptedAI
{
    boss_elder_ironbranchAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 ImpaleTimer;
    uint32 IronrootsTimer;
    uint32 ThornSwarmTimer;

    void Reset()
    {
        ImpaleTimer         = 10000 + urand (1000, 5000);
        IronrootsTimer      = 20000 + urand (1000, 7000);
        ThornSwarmTimer     = 30000;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_IRONBRANCH_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_IRONBRANCH_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_IRONBRANCH_SLAY1, m_creature);
        else
            DoScriptText(SAY_IRONBRANCH_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(ImpaleTimer < diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
            ImpaleTimer = 10000 + urand (1000, 5000);
        }
        else ImpaleTimer -= diff;

        if(IronrootsTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                m_creature->SummonCreature(MOB_IRON_ROOTS, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
            IronrootsTimer = 20000 + urand (1000, 7000);
        }
        else IronrootsTimer -= diff;

        if(ThornSwarmTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_THORM_SWARM : SPELL_THORM_SWARM_H);
            ThornSwarmTimer = 30000;
        }
        else ThornSwarmTimer -= diff;

        DoMeleeAttackIfReady(); 
    }
};

CreatureAI* GetAI_boss_elder_ironbranch(Creature* pCreature)
{
    return new boss_elder_ironbranchAI(pCreature);
}

// Stonebark
struct MANGOS_DLL_DECL boss_elder_stonebarkAI : public ScriptedAI
{
    boss_elder_stonebarkAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 FistsOfStoneTimer;
    uint32 GroundTremorTimer;
    uint32 PetrifiedBarkTimer;

    void Reset()
    {
        FistsOfStoneTimer   = 20000;
        GroundTremorTimer   = 15000;
        PetrifiedBarkTimer  = 25000;
    }

    void Aggro(Unit* pWho)
    {    
        DoScriptText(SAY_STONEBARK_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_STONEBARK_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_STONEBARK_SLAY1, m_creature);
        else
            DoScriptText(SAY_STONEBARK_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(FistsOfStoneTimer < diff)
        {
            DoCast(m_creature, SPELL_FIST_OF_STONE);
            FistsOfStoneTimer = 30000;
        }
        else FistsOfStoneTimer -= diff;

        if(GroundTremorTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR : SPELL_GROUND_TREMOR_H);
            GroundTremorTimer = 15000 + urand (1000, 5000);
        }
        else GroundTremorTimer -= diff;

        if(PetrifiedBarkTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_PETRIFIED_BARK : SPELL_PETRIFIED_BARK_H);
            PetrifiedBarkTimer = 20000 + urand (1000, 5000);
        }
        else PetrifiedBarkTimer -= diff;

        DoMeleeAttackIfReady(); 
    }
};

CreatureAI* GetAI_boss_elder_stonebark(Creature* pCreature)
{
    return new boss_elder_stonebarkAI(pCreature);
}

// Freya
struct MANGOS_DLL_DECL boss_freyaAI : public ScriptedAI
{
    boss_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        if(pCreature->isAlive())
        {
            sp = (SpellEntry *)GetSpellStore()->LookupEntry(SPELL_ATTUNED_TO_NATURE);
            bp = 8;
            if(!pCreature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
                pCreature->AddAura(new AttunedToNatureAura(sp, EFFECT_INDEX_0, &bp, pCreature, pCreature));
            pCreature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->SetStackAmount(150);
        }
        Reset();
    }

    uint32 SummonTimer;
    uint32 WaveNumber;
    uint32 WaveType;
    uint32 WaveTypeInc;
    uint32 SunbeamTimer;
    uint32 EnrageTimer;

    bool Regular;
    bool isHardMode;
    ScriptedInstance *pInstance;
    SpellEntry const *sp;
    int32 bp;

    uint32 NatureBombTimer;
    uint32 LifebindersGiftTimer;

    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;

    uint32 UnstableEnergyTimer;
    uint32 StrenghtenIronRootsTimer;
    uint32 GroundTremorTimer;

    uint32 AuraCheckTimer;

    bool isBrightleafAlive;
    bool isIronbranchAlive;
    bool isStonebarkAlive;

    void Reset()
    {
        SummonTimer = 15000;
        WaveNumber = 0;
        WaveType = irand(0,2);
        WaveTypeInc = irand(1,2);
        SunbeamTimer = rand()%10000;
        EnrageTimer = 600000; //10 minutes
        isHardMode = false;
        LifebindersGiftTimer = 30000;
        UnstableEnergyTimer = 25000;
        StrenghtenIronRootsTimer = 25000 + urand(1000, 5000);
        GroundTremorTimer = 20000;

        NatureBombTimer = 7000;

        AuraCheckTimer = 5000;

        OutroTimer = 10000;
        Step = 1;

        isOutro = false;

        if(m_creature->isAlive())
        {
            if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
                m_creature->AddAura(new AttunedToNatureAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
            m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->SetStackAmount(150);
        }

        if(pInstance) pInstance->SetData(TYPE_FREYA, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
            m_creature->AddAura(new AttunedToNatureAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
        m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->SetStackAmount(150);

        DoCast(m_creature, Regular ? SPELL_TOUCH_OF_EONAR : SPELL_TOUCH_OF_EONAR_H);

        if(pInstance) 
        {
            pInstance->SetData(TYPE_FREYA, IN_PROGRESS);

            // check brightleaf
            if (Creature* pBrightleaf = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_BRIGHTLEAF))))
            {
                if (pBrightleaf->isAlive())
                {
                    pBrightleaf->CastSpell(pBrightleaf, SPELL_DRAINED_OF_POWER, false);
                    isBrightleafAlive = true;
                }
                else
                    isBrightleafAlive = false;
            }

            // check ironbranch
            if (Creature* pIronbranch = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_IRONBRACH))))
            {
                if (pIronbranch->isAlive())
                {
                    pIronbranch->CastSpell(pIronbranch, SPELL_DRAINED_OF_POWER, false);
                    isIronbranchAlive = true;
                }
                else
                    isIronbranchAlive = false;
            }

            // check stonebark
            if (Creature* pStonebark = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_STONEBARK))))
            {
                if (pStonebark->isAlive())
                {
                    pStonebark->CastSpell(pStonebark, SPELL_DRAINED_OF_POWER, false);
                    isStonebarkAlive = true;
                }
                else
                    isStonebarkAlive = false;
            }
        }


        isHardMode = checkHardMode();

        if(!isHardMode)
            DoScriptText(SAY_AGGRO, m_creature);
        else
            DoScriptText(SAY_AGGRO_HARD, m_creature);
    }

    void DoOutro()
    {
        if(pInstance) 
        {
            pInstance->SetData(TYPE_FREYA, DONE);
            if(isHardMode)
                pInstance->SetData(TYPE_FREYA_HARD, DONE);
        }

        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(pInstance) 
        {
            pInstance->SetData(TYPE_FREYA, DONE);
            if(isHardMode)
                pInstance->SetData(TYPE_FREYA_HARD, DONE);
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

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void SummonLashers()
    {
        DoScriptText(SAY_SUMMON3, m_creature);
        int i;
        float x,y;
        for(i=0; i<10; ++i)
        {
            x = (rand_norm() * 30.0f) - 15.0f;
            y = (rand_norm() * 30.0f) - 15.0f;
            Creature *lasher = DoSpawnCreature(MOB_DETONATING_LASHER, x, y, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(lasher && target)
                lasher->AddThreat(target, 1.0f);
        }
    }

    void SummonConservator()
    {
        DoScriptText(SAY_SUMMON1, m_creature);
        float x = (rand_norm() * 30.0f) - 15.0f;
        float y = (rand_norm() * 30.0f) - 15.0f;
        Creature *add = DoSpawnCreature(MOB_ANCIENT_CONSERVATOR, x, y, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);
    }

    void SummonElementals()
    {
        DoScriptText(SAY_SUMMON2, m_creature);
        Creature *add;
        Unit *target;
        add = DoSpawnCreature(MOB_WATER_SPIRIT, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);

        add = DoSpawnCreature(MOB_STORM_LASHER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);

        add = DoSpawnCreature(MOB_SNAPLASHER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);
    }

    void SummonedCreatureDespawn(Creature* mob)
    {
        if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)) return;

        switch(mob->GetEntry())
        {
        case MOB_DETONATING_LASHER:
            if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->modStackAmount(-2))
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
            break;
        case MOB_ANCIENT_CONSERVATOR:
            if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->modStackAmount(-25))
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
            break;
        case MOB_SNAPLASHER:
        case MOB_STORM_LASHER:
        case MOB_WATER_SPIRIT:
            if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->modStackAmount(-10))
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);

        }
    }

    bool checkHardMode()
    {
        if(isBrightleafAlive && isIronbranchAlive && isStonebarkAlive)
            return true;
        return false;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!isOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // Hardmode
            if(isBrightleafAlive)
            {
                if(AuraCheckTimer < diff)
                {
                    DoCast(m_creature, SPELL_BRIGHTLEAFS_ESSENCE);
                    AuraCheckTimer = 5000;
                }
                else AuraCheckTimer -= diff;

                if(UnstableEnergyTimer < diff)
                {
                    DoCast(m_creature, Regular ? SPELL_UNSTABLE_ENERGY_FREYA : SPELL_UNSTABLE_ENERGY_FREYA_H);
                    UnstableEnergyTimer = 25000;
                }
                else UnstableEnergyTimer -= diff;
            }

            if(isIronbranchAlive)
            {
                if(AuraCheckTimer < diff)
                {
                    DoCast(m_creature, SPELL_IRONBRACHS_ESSENCE);
                    AuraCheckTimer = 5000;
                }
                else AuraCheckTimer -= diff;

                if(StrenghtenIronRootsTimer < diff)
                {
                    if( Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        m_creature->SummonCreature(MOB_STRENGHTEN_IRON_ROOTS, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                    StrenghtenIronRootsTimer = 25000 + urand(1000, 5000);
                }
                else StrenghtenIronRootsTimer -= diff;

            }

            if(isStonebarkAlive)
            {
                if(AuraCheckTimer < diff)
                {
                    DoCast(m_creature, SPELL_STONEBARKS_ESSENCE);
                    AuraCheckTimer = 5000;
                }
                else AuraCheckTimer -= diff;

                if(GroundTremorTimer < diff)
                {
                    DoCast(m_creature, Regular ? SPELL_GROUND_TREMOR_FREYA : SPELL_GROUND_TREMOR_FREYA_H);
                    GroundTremorTimer = 20000;
                }
                else GroundTremorTimer -= diff;
            }

            //Phase 1, waves of adds
            if(WaveNumber < 6)
            {
                if(SummonTimer < diff)
                {
                    switch(WaveType)
                    {
                    case 0: SummonLashers(); break;
                    case 1: SummonConservator(); break;
                    case 2: SummonElementals(); break;
                    }
                    WaveType = (WaveType + WaveTypeInc) % 3;
                    ++WaveNumber;
                    SummonTimer = 60000;
                }
                else SummonTimer -= diff;
            }
            // Phase 2
            else
            {
                // nature bomb
                if(NatureBombTimer < diff)
                {
                    for(uint8 i = 0; i < 7; i++)
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                            m_creature->SummonCreature(MOB_NATURE_BOMB, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 6000);
                    }
                    NatureBombTimer = 7000;
                }
                else NatureBombTimer -= diff;
            }

            //All phases
            if(SunbeamTimer < diff)
            {
                if( Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, Regular ? SPELL_SUNBEAM : SPELL_SUNBEAM_H);
                SunbeamTimer = 6000 + rand()%10000;
            }
            else SunbeamTimer -= diff;

            if(LifebindersGiftTimer < diff)
            {
                if( Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(MOB_EONARS_GIFT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                LifebindersGiftTimer = 30000;
            }
            else LifebindersGiftTimer -= diff;

            if(EnrageTimer < diff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_BERSERK);
                EnrageTimer = 30000;
            }
            else EnrageTimer -= diff;

            // Hp check
            //if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 1)
                //isOutro = true;

            DoMeleeAttackIfReady();
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
                m_creature->GetMotionMaster()->MovePoint(0, 2359.40f, -52.39f, 425.64f);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++Step;
                OutroTimer = 7000;
                break;
            case 3:
                m_creature->SetOrientation(4.99f);
                DoScriptText(SAY_DEATH, m_creature);
                ++Step;
                OutroTimer = 10000;
                break;
            case 5:
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

CreatureAI* GetAI_boss_freya(Creature* pCreature)
{
    return new boss_freyaAI(pCreature);
}

// Nature bomb
struct MANGOS_DLL_DECL mob_nature_bombAI : public ScriptedAI
{
    mob_nature_bombAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 ExplodeTimer;
    uint32 dieTimer;

    void Reset()
    {
        ExplodeTimer = 5000;
        dieTimer = 5500;
    }

    void UpdateAI(const uint32 diff)
    {
        if(ExplodeTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB : SPELL_NATURE_BOMB_H);
        }
        else ExplodeTimer -= diff;

        if(dieTimer < diff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
        else dieTimer -= diff;
    }
};

CreatureAI* GetAI_mob_nature_bomb(Creature* pCreature)
{
    return new mob_nature_bombAI(pCreature);
}

// Iron roots
struct MANGOS_DLL_DECL mob_iron_rootsAI : public ScriptedAI
{
    mob_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 rootTimer;

    void Reset()
    {
        rootTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(rootTimer < diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                DoCast(pTarget, SPELL_STUN);
                m_creature->DealDamage(pTarget, m_bIsRegularMode ? urand(6013, 6987) : urand(7863, 9137), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NATURE, NULL, false);
            }
            rootTimer = 1000;
        }
        else rootTimer -= diff;
    }
};

CreatureAI* GetAI_mob_iron_roots(Creature* pCreature)
{
    return new mob_iron_rootsAI(pCreature);
}

void AddSC_boss_freya()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_freya";
    newscript->GetAI = &GetAI_boss_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_brightleaf";
    newscript->GetAI = &GetAI_boss_elder_brightleaf;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_ironbranch";
    newscript->GetAI = &GetAI_boss_elder_ironbranch;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_stonebark";
    newscript->GetAI = &GetAI_boss_elder_stonebark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nature_bomb";
    newscript->GetAI = &GetAI_mob_nature_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();

}
