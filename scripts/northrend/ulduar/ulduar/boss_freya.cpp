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
SDComment: 
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
    SPELL_DRAINED_OF_POWER          = 62467,
    // brightleaf
    SPELL_UNSTABLE_ENERGY_FREYA     = 62451,
    SPELL_UNSTABLE_ENERGY_FREYA_H   = 62865,
    SPELL_BRIGHTLEAFS_ESSENCE       = 62968, //62385,
    SPELL_EFFECT_BRIGHTLEAF         = 63294,
    // ironbrach
    SPELL_STRENGHTEN_IRON_ROOTS     = 63601,
    NPC_STRENGHENED_IRON_ROOTS      = 33168,
    SPELL_IRON_ROOTS_FREYA          = 62283,
    SPELL_IRON_ROOTS_FREYA_H        = 62930,
    SPELL_IRONBRANCH_ESSENCE        = 62713, //62387,
    SPELL_EFFECT_IRONBRANCH         = 63292,
    // stonebark
    SPELL_GROUND_TREMOR_FREYA       = 62437,
    SPELL_GROUND_TREMOR_FREYA_H     = 62859,
    SPELL_STONEBARKS_ESSENCE        = 65590, //62386,
    SPELL_EFFECT_STONEBARK          = 63295,

    NPC_SUN_BEAM                    = 33170,
    NPC_UNSTABLE_SUN_BEAM           = 33050,

    // sanctuary adds 
    NPC_EONARS_GIFT             = 33228,
    SPELL_LIFEBINDERS_GIFT      = 62584,    // after 12 secs, heals Freya & her allies for 30%
    SPELL_LIFEBINDERS_GIFT_H    = 64185,    // the same but for 60%
    SPELL_PHEROMONES            = 62619,    // protects from conservators grip
    NPC_HEALTHY_SPORE           = 33215,

    /* ADDS */
    // 6 waves of adds. 1 of the 3 each min 
    NPC_DETONATING_LASHER       = 32918,    // recude 2 stacks
    // spells
    SPELL_FLAME_LASH            = 62608,
    SPELL_DETONATE              = 62598,
    SPELL_DETONATE_H            = 62937, 

    NPC_ANCIENT_CONSERVATOR     = 33203,    // reduce 30 stacks
    //spells
    SPELL_CONSERVATORS_GRIP     = 62532,
    SPELL_NATURES_FURY          = 62589,
    SPELL_NATURES_FURY_H        = 63571,

    /* elemental adds */                    // each one reduces 10 stacks
    NPC_WATER_SPIRIT            = 33202,
    // spells
    SPELL_TIDAL_WAVE            = 62653,
    SPELL_TIDAL_WAVE_H          = 62935,

    NPC_STORM_LASHER            = 32919,
    // spells
    SPELL_STORMBOLT             = 62649,
    SPELL_STORMBOLT_H           = 62938,
    SPELL_LIGHTNING_LASH        = 62648,    // 3 targets
    SPELL_LIGHTNING_LASH_H      = 62939,    // 5 targets

    NPC_SNAPLASHER              = 32916,
    // spells
    SPELL_HARDENED_BARK         = 62663,
    SPELL_HARDENED_BARK_H       = 64190,

    // nature bomb
    NPC_NATURE_BOMB             = 34129,
    GO_NATURE_BOMB              = 194902,
    SPELL_NATURE_BOMB           = 64587,
    SPELL_NATURE_BOMB_H         = 64650,

    /* ELDERS */                            // used in phase 1
    ELDER_BRIGHTLEAF            = 32915,
    ELDER_IRONBRANCH            = 32913,
    ELDER_STONEBARK             = 32914,

    // brightleaf spells
    SPELL_BRIGHTLEAF_FLUX       = 62262,
    SPELL_SOLAR_FLARE           = 62240,
    SPELL_SOLAR_FLARE_H         = 62920,
    SPELL_UNSTABLE_SUN_BEAN     = 62243,
    SPELL_UNSTABLE_ENERGY       = 62217,
    SPELL_UNSTABLE_ENERGY_H     = 62922,
    SPELL_PHOTOSYNTHESIS        = 62209,

    // ironbrach spells
    SPELL_IMPALE                = 62310,
    SPELL_IMPALE_H              = 62928,
    SPELL_IRON_ROOTS            = 62438,
    SPELL_IRON_ROOTS_H          = 62861,
    NPC_IRON_ROOTS              = 33088,
    SPELL_THORM_SWARM           = 62285,
    SPELL_THORM_SWARM_H         = 62931,

    // stonebark spells
    SPELL_FIST_OF_STONE         = 62344,
    SPELL_BROKEN_BONES          = 62356,
    SPELL_GROUND_TREMOR         = 62325,
    SPELL_GROUND_TREMOR_H       = 62932,
    SPELL_PETRIFIED_BARK        = 62337,
    SPELL_PETRIFIED_BARK_H      = 62933,

    // not used because summoned chest doesn't despawn after looted
    SPELL_SUMMON_CHEST_1                = 62950,
    SPELL_SUMMON_CHEST_2                = 62952,
    SPELL_SUMMON_CHEST_3                = 62953,
    SPELL_SUMMON_CHEST_4                = 62954,
    SPELL_SUMMON_CHEST_5                = 62955,
    SPELL_SUMMON_CHEST_6                = 62956,
    SPELL_SUMMON_CHEST_7                = 62957,
    SPELL_SUMMON_CHEST_8                = 62958,

    SPELL_SUMMON_ALLIES_OF_NATURE       = 62678, //better do that in sd2
    SPELL_SUMMON_WAVE_10                = 62688,
    SPELL_SUMMON_WAVE_3                 = 62686,
    SPELL_SUMMON_WAVE_1                 = 62685,
    SPELL_LIFEBINDERS_GIFT_SUMMON       = 62869,
    SPELL_NATURE_BOMB_SUMMON            = 64606,

    SPELL_SPORE_SUMMON_NE               = 62591,
    SPELL_SPORE_SUMMON_SE               = 62592,
    SPELL_SPORE_SUMMON_SW               = 62593,
    SPELL_SPORE_SUMMON_NW               = 62582,

    SPELL_HEALTHY_SPORE_VISUAL          = 62538,
    SPELL_NATURE_BOMB_VISUAL            = 64604,
    SPELL_LIFEBINDERS_VISUAL            = 62579,
    SPELL_LIFEBINDER_GROW               = 44833,

    SPELL_PHEROMONES_LG                 = 62619,
    SPELL_POTENT_PHEROMONES             = 62541,

    ACHIEV_BACK_TO_NATURE           = 2982,
    ACHIEV_BACK_TO_NATURE_H         = 2983,
    ACHIEV_KNOCK_WOOD               = 3177,
    ACHIEV_KNOCK_WOOD_H             = 3185,
    ACHIEV_KNOCK_KNOCK_WOOD         = 3178,
    ACHIEV_KNOCK_KNOCK_WOOD_H       = 3186,
    ACHIEV_KNOCK_KNOCK_KNOCK_WOOD   = 3179,
    ACHIEV_KNOCK_KNOCK_KNOCK_WOOD_H = 3187, 
};

class MANGOS_DLL_DECL AttunedToNatureAura : public Aura
{
public:
    AttunedToNatureAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
    {}
};

// Iron roots
struct MANGOS_DLL_DECL mob_iron_rootsAI : public ScriptedAI
{
    mob_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
        SetCombatMovement(false);
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }

    void SetVictim(uint64 victim)
    {
        if (victim)
        {
            m_uiVictimGUID = victim;
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            {
                pVictim->CastSpell(pVictim, m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H, true);
                pVictim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
    }
};

struct MANGOS_DLL_DECL mob_str_iron_rootsAI : public ScriptedAI
{
    mob_str_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }
    
    void SetVictim(uint64 victim)
    {
        if (victim)
        {
            m_uiVictimGUID = victim;
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                pVictim->CastSpell(pVictim, m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H, true);
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : SPELL_IRON_ROOTS_FREYA_H);
    }
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SOLAR_FLARE : SPELL_SOLAR_FLARE_H);
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
            DoCast(m_creature, SPELL_PHOTOSYNTHESIS);
            PhotosyntesisTimer = 30000;
            PhotosyntesisEndTimer = 5000;
        }
        else PhotosyntesisTimer -= diff;

        if(PhotosyntesisEndTimer < diff)
        {
            m_creature->RemoveAurasDueToSpell(SPELL_PHOTOSYNTHESIS);
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
            ImpaleTimer = 10000 + urand (1000, 5000);
        }
        else ImpaleTimer -= diff;

        if(IronrootsTimer < diff)
        {
            //DoCast(m_creature, m_bIsRegularMode ? SPELL_IRON_ROOTS : SPELL_IRON_ROOTS_H);
            int8 times = m_bIsRegularMode ? 1 : 2;
            for(int8 i = 0; i < times; )
            {
                if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
                {
                    if(target->HasAura(SPELL_IRON_ROOTS || SPELL_IRON_ROOTS_H))
                        return;

                    float x = target->GetPositionX();
                    float y = target->GetPositionY();
                    float z = target->GetPositionZ();
                    if(Creature* pRoots = m_creature->SummonCreature(NPC_IRON_ROOTS, x, y, z, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                        ((mob_iron_rootsAI*)pRoots->AI())->SetVictim(target->GetGUID());
                    DoTeleportPlayer(target, x, y, z, target->GetOrientation());
                    ++i;
                }else break;
            }
            IronrootsTimer = 20000 + urand (1000, 7000);
        }
        else IronrootsTimer -= diff;

        if(ThornSwarmTimer < diff)
        {
            if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(target, m_bIsRegularMode ? SPELL_THORM_SWARM : SPELL_THORM_SWARM_H);
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
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
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

    bool m_bIsRegularMode;
    bool isHardMode;
    ScriptedInstance* m_pInstance;
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

    uint32 m_uiThreeWaveDeath_Timer;
    bool m_bWaveCheck;
    bool m_bRespawnAdds;

    bool isBrightleafAlive;
    bool isIronbranchAlive;
    bool isStonebarkAlive;

    uint32 m_uiAchievProgress;
    bool m_bNature;

    void Reset()
    {
        SummonTimer                 = 15000;
        WaveNumber                  = 0;
        WaveType                    = irand(0,2);
        WaveTypeInc                 = irand(1,2);
        SunbeamTimer                = rand()%10000;
        EnrageTimer                 = 600000; //10 minutes
        isHardMode                  = false;
        LifebindersGiftTimer        = 30000;
        UnstableEnergyTimer         = 25000;
        StrenghtenIronRootsTimer    = 25000 + urand(1000, 5000);
        GroundTremorTimer           = 20000;

        NatureBombTimer             = 7000;
        m_uiThreeWaveDeath_Timer    = 10000;
        m_uiAchievProgress          = 10000;
        m_bWaveCheck                = false;
        m_bRespawnAdds              = false;

        OutroTimer                  = 10000;
        Step                        = 1;

        isOutro                     = false;

        m_uiAchievProgress          = 0;
        m_bNature                   = false;

        if(m_creature->isAlive())
        {
            if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
                m_creature->AddAura(new AttunedToNatureAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
            m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->SetStackAmount(150);
        }

        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA, NOT_STARTED);

            // remove elder auras
            if (Creature* pBrightleaf = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_BRIGHTLEAF))))
            {
                if (pBrightleaf->isAlive())
                    pBrightleaf->RemoveAllAuras();
            }
            if (Creature* pIronbranch = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_IRONBRACH))))
            {
                if (pIronbranch->isAlive())
                    pIronbranch->RemoveAllAuras();
            }
            if (Creature* pStonebark = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_STONEBARK))))
            {
                if (pStonebark->isAlive())
                    pStonebark->RemoveAllAuras();
            }
        }

        std::list<Creature*> lAddsList;
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_SNAPLASHER, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_STORM_LASHER, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_WATER_SPIRIT, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_ANCIENT_CONSERVATOR, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_DETONATING_LASHER, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_EONARS_GIFT, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_NATURE_BOMB, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_HEALTHY_SPORE, DEFAULT_VISIBILITY_INSTANCE);
        if (!lAddsList.empty())
        {
            for(std::list<Creature*>::iterator iter = lAddsList.begin(); iter != lAddsList.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void Aggro(Unit *who) 
    {
        if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
            m_creature->AddAura(new AttunedToNatureAura(sp, EFFECT_INDEX_0, &bp, m_creature, m_creature));
        m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->SetStackAmount(150);

        DoCast(m_creature, m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : SPELL_TOUCH_OF_EONAR_H);

        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA, IN_PROGRESS);

            // check brightleaf
            if (Creature* pBrightleaf = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_BRIGHTLEAF))))
            {
                if (pBrightleaf->isAlive())
                {
                    pBrightleaf->CastSpell(pBrightleaf, SPELL_DRAINED_OF_POWER, true);
                    m_creature->CastSpell(m_creature, SPELL_BRIGHTLEAFS_ESSENCE, true);
                    pBrightleaf->CastSpell(m_creature, SPELL_EFFECT_BRIGHTLEAF, true);
                    isBrightleafAlive = true;
                    m_uiAchievProgress += 1;
                }
                else
                    isBrightleafAlive = false;
            }

            // check ironbranch
            if (Creature* pIronbranch = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_IRONBRACH))))
            {
                if (pIronbranch->isAlive())
                {
                    pIronbranch->CastSpell(pIronbranch, SPELL_DRAINED_OF_POWER, true);
                    m_creature->CastSpell(m_creature, SPELL_IRONBRANCH_ESSENCE, true);
                    pIronbranch->CastSpell(m_creature, SPELL_EFFECT_IRONBRANCH, true);
                    isIronbranchAlive = true;
                    m_uiAchievProgress += 1;
                }
                else
                    isIronbranchAlive = false;
            }

            // check stonebark
            if (Creature* pStonebark = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_STONEBARK))))
            {
                if (pStonebark->isAlive())
                {
                    pStonebark->CastSpell(pStonebark, SPELL_DRAINED_OF_POWER, true);
                    m_creature->CastSpell(m_creature, SPELL_STONEBARKS_ESSENCE, true);
                    pStonebark->CastSpell(m_creature, SPELL_EFFECT_STONEBARK, true);
                    isStonebarkAlive = true;
                    m_uiAchievProgress += 1;
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
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA_HARD, 0);

            if(m_uiAchievProgress == 1)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_WOOD : ACHIEV_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 1);
            }
            else if (m_uiAchievProgress == 2)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_KNOCK_WOOD : ACHIEV_KNOCK_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 2);
            }
            else if (m_uiAchievProgress == 3)
            {
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_KNOCK_KNOCK_KNOCK_WOOD : ACHIEV_KNOCK_KNOCK_KNOCK_WOOD_H);
                m_pInstance->SetData(TYPE_FREYA_HARD, 3);
            }

            if (m_bNature)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_BACK_TO_NATURE : ACHIEV_BACK_TO_NATURE_H);

            m_pInstance->SetData(TYPE_FREYA, DONE);
        }

        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FREYA, DONE);
            if(isHardMode)
                m_pInstance->SetData(TYPE_FREYA_HARD, DONE);
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
            Creature *lasher = DoSpawnCreature(NPC_DETONATING_LASHER, x, y, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
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
        Creature *add = DoSpawnCreature(NPC_ANCIENT_CONSERVATOR, x, y, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);
    }

    void SummonElementals()
    {
        DoScriptText(SAY_SUMMON2, m_creature);
        Creature *add;
        Unit *target;
        add = DoSpawnCreature(NPC_WATER_SPIRIT, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);

        add = DoSpawnCreature(NPC_STORM_LASHER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);

        add = DoSpawnCreature(NPC_SNAPLASHER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000);
        target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if(add && target)
            add->AddThreat(target, 1.0f);
    }

    void SummonedCreatureDespawn(Creature* mob)
    {
        if(!m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)) return;

        switch(mob->GetEntry())
        {
        case NPC_DETONATING_LASHER:
            if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->modStackAmount(-2))
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
            break;
        case NPC_ANCIENT_CONSERVATOR:
            if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->modStackAmount(-25))
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
            break;
        case NPC_SNAPLASHER:
        case NPC_STORM_LASHER:
        case NPC_WATER_SPIRIT:
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

            if(m_bWaveCheck)
            {
                if(m_uiThreeWaveDeath_Timer < diff)
                {
                    std::list<Creature*> lAddsList;
                    GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_STORM_LASHER, DEFAULT_VISIBILITY_INSTANCE);
                    GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_SNAPLASHER, DEFAULT_VISIBILITY_INSTANCE);
                    GetCreatureListWithEntryInGrid(lAddsList, m_creature, NPC_WATER_SPIRIT, DEFAULT_VISIBILITY_INSTANCE);
                    if (!lAddsList.empty())
                    {
                        for(std::list<Creature*>::iterator iter = lAddsList.begin(); iter != lAddsList.end(); ++iter)
                        {
                            if ((*iter) && (*iter)->isAlive())
                                m_bRespawnAdds = true;
                        }
                    }

                    if(!m_bRespawnAdds)
                        m_bWaveCheck = false;
                    else
                    {
                        if (!lAddsList.empty())
                        {
                            for(std::list<Creature*>::iterator iter = lAddsList.begin(); iter != lAddsList.end(); ++iter)
                            {
                                if ((*iter) && !(*iter)->isAlive())
                                    (*iter)->Respawn();
                            }
                        }
                        m_bWaveCheck = false;
                        m_bRespawnAdds = false;
                    }
                }
                else
                    m_uiThreeWaveDeath_Timer -= diff;
            }
            else
                m_uiThreeWaveDeath_Timer = 10000;

            // Hardmode
            if(isBrightleafAlive)
            {
                if(UnstableEnergyTimer < diff)
                {
                    //DoCast(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_FREYA : SPELL_UNSTABLE_ENERGY_FREYA_H);
                    for(int8 i = 0; i < 3; ++i)
                    {
                        if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        {
                            float x = target->GetPositionX();
                            float y = target->GetPositionY();
                            float z = target->GetPositionZ();
                            m_creature->SummonCreature(NPC_SUN_BEAM, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                        }
                    }
                    UnstableEnergyTimer = urand(25000, 30000);
                }
                else UnstableEnergyTimer -= diff;
            }

            if(isIronbranchAlive)
            {
                if(StrenghtenIronRootsTimer < diff)
                {
                    int8 times = m_bIsRegularMode ? 1 : 2;
                    for(int8 i = 0; i < times; )
                    {
                        if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
                        {
                            if(target->HasAura(SPELL_IRON_ROOTS_FREYA || SPELL_IRON_ROOTS_FREYA_H))
                                return;

                            float x = target->GetPositionX();
                            float y = target->GetPositionY();
                            float z = target->GetPositionZ();
                            if(Creature* pRoots = m_creature->SummonCreature(NPC_STRENGHENED_IRON_ROOTS, x, y, z, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                                ((mob_str_iron_rootsAI*)pRoots->AI())->SetVictim(target->GetGUID());
                            DoTeleportPlayer(target, x, y, z, target->GetOrientation());
                            ++i;
                        }else break;
                    }
                    StrenghtenIronRootsTimer = 50000 + urand(10000, 20000);
                }
                else StrenghtenIronRootsTimer -= diff;
            }

            if(isStonebarkAlive)
            {
                if(GroundTremorTimer < diff)
                {
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR_FREYA : SPELL_GROUND_TREMOR_FREYA_H);
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
                    m_creature->CastSpell(m_creature, SPELL_NATURE_BOMB_VISUAL, true);
                    m_creature->CastSpell(m_creature, SPELL_NATURE_BOMB_SUMMON, true);

                    int8 count = urand(8,10);
                    for(int8 i = 0; i < count; ++i)
                    {
                        float radius = 30* rand_norm_f();
                        float angle = 2.0f * M_PI_F * rand_norm_f();
                        float x = m_creature->GetPositionX() + cos(angle) * radius;
                        float y = m_creature->GetPositionY() + sin(angle) * radius;
                        float z = m_creature->GetMap()->GetHeight(x, y, MAX_HEIGHT);
                        m_creature->SummonCreature(NPC_NATURE_BOMB, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                        m_creature->SummonGameobject(GO_NATURE_BOMB, x, y, z, 0, 25000);
                    }
                    NatureBombTimer = urand(7000, 12000);
                }
                else NatureBombTimer -= diff;
            }

            //All phases
            if(SunbeamTimer < diff)
            {
                if( Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, m_bIsRegularMode ? SPELL_SUNBEAM : SPELL_SUNBEAM_H);
                SunbeamTimer = 6000 + rand()%10000;
            }
            else SunbeamTimer -= diff;

            if(LifebindersGiftTimer < diff)
            {
                if( Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_LIFEBINDERS_GIFT_SUMMON);
                    //m_creature->SummonCreature(MOB_EONARS_GIFT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
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

            DoMeleeAttackIfReady();
        }

        // outro
        if(isOutro)
        {
            switch(Step)
            {
            case 1:
                if(m_creature->HasAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
                {
                    if(m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0)->GetStackAmount() >= 25)
                        m_bNature = true;
                }
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
struct MANGOS_DLL_DECL mob_freya_groundAI : public ScriptedAI
{
     mob_freya_groundAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 NatureBomb_Timer;
    uint32 EonarsGift_Timer;
    uint32 NonSelectable_Timer;
    uint32 Grow_Timer;
    uint32 SunBeamDespawn_Timer;
    uint32 UnstableEnergy_Timer;
    uint32 HealthyGrow_Timer;
    float size;

    bool NpcNatureBomb;
    bool NpcEonarsGift;
    bool NpcHealthySpore;
    bool NpcSunBeamFreya;
    bool NpcSunBeamBright;

    bool Grow;

    GameObject* Bomb;

    void Reset()
    {
        NatureBomb_Timer = urand(9,11)*IN_MILLISECONDS;
        EonarsGift_Timer = urand(11,13)*IN_MILLISECONDS;
        NonSelectable_Timer = 5*IN_MILLISECONDS;
        UnstableEnergy_Timer = IN_MILLISECONDS;
        Grow_Timer = 0;
        SunBeamDespawn_Timer = urand(10,11)*IN_MILLISECONDS;
        Grow = true;
        HealthyGrow_Timer = urand(3,12)*IN_MILLISECONDS;
        NpcNatureBomb = false;
        NpcEonarsGift = false;
        NpcHealthySpore = false;
        NpcSunBeamFreya = false;
        NpcSunBeamBright = false;
        Bomb = NULL;
        switch(m_creature->GetEntry())
        {
            case NPC_NATURE_BOMB:
                NpcNatureBomb = true;
                size = 1;
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                Bomb = m_creature->SummonGameobject(GO_NATURE_BOMB, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+1, 0, 25000);
                break;
            case NPC_EONARS_GIFT:
                NpcEonarsGift = true;
                size = float(0.1);
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                break;
            case NPC_HEALTHY_SPORE:
                NpcHealthySpore = true; 
                m_creature->CastSpell(m_creature, SPELL_HEALTHY_SPORE_VISUAL, true);
                m_creature->CastSpell(m_creature, SPELL_POTENT_PHEROMONES, true);
                break;
            case NPC_SUN_BEAM:
                NpcSunBeamFreya = true;
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_FREYA : SPELL_UNSTABLE_ENERGY_FREYA_H, true);
                break;

            case NPC_UNSTABLE_SUN_BEAM:
                NpcSunBeamBright = true; 
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                m_creature->CastSpell(m_creature, SPELL_PHOTOSYNTHESIS, true);
                break;
        }

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetRespawnDelay(DAY);           
    }

    void AttackStart(Unit* pWho){return;}

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_FREYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(!m_creature->isAlive())
            return;

        if(NpcNatureBomb)
            if(NatureBomb_Timer < diff)
            {
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB : SPELL_NATURE_BOMB_H, true);
                m_creature->ForcedDespawn();
                if(Bomb)
                    Bomb->Delete();
            }else NatureBomb_Timer -= diff;

        if(NpcEonarsGift)
        {
            if (Grow_Timer > 500 && size < 1.5)
            {
                size += float(Grow_Timer)/8000;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, size);
                Grow_Timer = 0;
            }else Grow_Timer += diff;

            if(EonarsGift_Timer < diff)
            {
                if (Creature* pFreya = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_FREYA))))
                    m_creature->CastSpell(pFreya, m_bIsRegularMode ? SPELL_LIFEBINDERS_GIFT : SPELL_LIFEBINDERS_GIFT_H, true);
                EonarsGift_Timer = IN_MILLISECONDS;
            }else EonarsGift_Timer -= diff;

            if(NonSelectable_Timer < diff && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->CastSpell(m_creature, SPELL_PHEROMONES_LG, true);
            }else NonSelectable_Timer -= diff;
        }
        if(NpcHealthySpore)
        {
            if(!Grow && size < 0.25)
                m_creature->ForcedDespawn();

            if(HealthyGrow_Timer < diff)
            {
                if(Grow)
                {
                    size = float(urand(150,225))/100;
                    Grow = false;
                }
                else
                    size = float(urand(1,300))/100;
                if(size < 1)
                    size = 0.1f;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, size);
                HealthyGrow_Timer = urand(3,5)*IN_MILLISECONDS;
            }else HealthyGrow_Timer -= diff;

        }
        if(NpcSunBeamBright)
        {
            if(UnstableEnergy_Timer < diff)
            {
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY : SPELL_UNSTABLE_ENERGY_H, true);
                UnstableEnergy_Timer = IN_MILLISECONDS;
            }else UnstableEnergy_Timer -= diff;
        
        }
        if(NpcSunBeamFreya || NpcSunBeamBright)
        {
            if(SunBeamDespawn_Timer < diff)
                m_creature->ForcedDespawn();
            else SunBeamDespawn_Timer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL mob_freya_spawnedAI : public ScriptedAI
{
    mob_freya_spawnedAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool AncientConservator;
    bool DetonatingLasher;
    bool AncientWaterSpirit;
    bool StormLasher;
    bool Snaplasher;

    uint32 DeathCountdown;
    uint32 TidalWave_Timer;
    uint32 Stormbolt_Timer;
    uint32 LightningLash_Timer;
    uint32 FlameLash_Timer;
    uint32 NaturesFury_Timer;
    uint32 Wave3_DeathCountdown;
    uint32 RespawnSpores_Timer;

    void Reset()
    {
        AttackStart(SelectUnit(SELECT_TARGET_RANDOM,0));
        AncientWaterSpirit = false;
        StormLasher = false;
        Snaplasher = false;
        AncientConservator = false;
        DetonatingLasher = false;
        DeathCountdown = 10*IN_MILLISECONDS;
        TidalWave_Timer = urand(2,4)*IN_MILLISECONDS;
        Stormbolt_Timer = IN_MILLISECONDS;
        LightningLash_Timer = urand(11,14)*IN_MILLISECONDS;        
        FlameLash_Timer = urand(5,10)*IN_MILLISECONDS;
        NaturesFury_Timer = urand(8,10)*IN_MILLISECONDS;
        RespawnSpores_Timer = 5*IN_MILLISECONDS;

        switch(m_creature->GetEntry())
        {
        case NPC_ANCIENT_CONSERVATOR:
            AncientConservator = true;
            //m_creature->CastSpell(m_creature, SPELL_CONSERVATORS_GRIP, true); //spell disabled because it isn't negated by pheronomes
            DoSpores(10);
            break;
        case NPC_DETONATING_LASHER:
            DetonatingLasher = true;
            break;
        case NPC_WATER_SPIRIT:
            AncientWaterSpirit = true;
            break;
        case NPC_SNAPLASHER:
            Snaplasher = true;
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_HARDENED_BARK : SPELL_HARDENED_BARK_H, true);
            break;
        case NPC_STORM_LASHER:
            StormLasher = true;
            break;
        }

        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit* Killer)
    {
        if(AncientWaterSpirit || Snaplasher || StormLasher)
        {
            if (Creature* pFreya = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_FREYA))))
                ((boss_freyaAI*)pFreya->AI())->m_bWaveCheck = true;
        }
    }
        /*if (AncientConservator)
            ReduceStack(30);
        if (DetonatingLasher)
            ReduceStack(2);
        if(AncientWaterSpirit || Snaplasher || StormLasher)
            ReduceStack(10);
    }
    
    void ReduceStack(uint8 count)
    {
        if(Aura *pAura = Freya->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
        {
            if(count > pAura->GetStackAmount())
            {
                pAura->SetStackAmount(1);
                pAura->DropAuraCharge();
            }else pAura->SetStackAmount(pAura->GetStackAmount()-count);
        }
    }*/

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (DetonatingLasher && damage > m_creature->GetHealth())
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_DETONATE : SPELL_DETONATE_H, true);
    }

    void DoSpores(int8 times)
    {
        for(int8 i = 0; i < times; ++i)
        {
            for(int8 itr = 0; i < 3; ++i)
                m_creature->CastSpell(m_creature, SPELL_SPORE_SUMMON_NE+itr, true);
            m_creature->CastSpell(m_creature, SPELL_SPORE_SUMMON_NW, true);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_FREYA) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(!m_creature->isAlive())
            return;
        
        if(DetonatingLasher && FlameLash_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_LASH);
            FlameLash_Timer = urand(5,10)*IN_MILLISECONDS;
        }else FlameLash_Timer -= diff;

        if(AncientConservator)
        {
            if(NaturesFury_Timer < diff)
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM,0), m_bIsRegularMode ? SPELL_NATURES_FURY : SPELL_NATURES_FURY_H);
                NaturesFury_Timer = urand(5,6)*IN_MILLISECONDS;
            }else NaturesFury_Timer -= diff;

            if(RespawnSpores_Timer < diff)
            {
                DoSpores(3);
                RespawnSpores_Timer = 5*IN_MILLISECONDS;
            }else RespawnSpores_Timer -= diff;
        }

        if(AncientWaterSpirit && TidalWave_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TIDAL_WAVE : SPELL_TIDAL_WAVE_H);
            TidalWave_Timer = urand(7,9)*IN_MILLISECONDS;
        }else TidalWave_Timer -= diff;

        if(StormLasher)
        {
            if (LightningLash_Timer < diff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTNING_LASH : SPELL_LIGHTNING_LASH_H);
                LightningLash_Timer = urand(11,14)*IN_MILLISECONDS;
            }else
            {
                LightningLash_Timer -= diff;
                if (Stormbolt_Timer <diff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STORMBOLT : SPELL_STORMBOLT_H);
                    Stormbolt_Timer = 2*IN_MILLISECONDS;
                }else Stormbolt_Timer -= diff;
            }
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_freya_ground(Creature* pCreature)
{
    return new mob_freya_groundAI(pCreature);
}

CreatureAI* GetAI_mob_freya_spawned(Creature* pCreature)
{
    return new mob_freya_spawnedAI(pCreature);
}

CreatureAI* GetAI_mob_iron_roots(Creature* pCreature)
{
    return new mob_iron_rootsAI(pCreature);
}

CreatureAI* GetAI_mob_str_iron_roots(Creature* pCreature)
{
    return new mob_str_iron_rootsAI(pCreature);
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
    newscript->Name = "mob_freya_ground";
    newscript->GetAI = &GetAI_mob_freya_ground;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_freya_spawned";
    newscript->GetAI = &GetAI_mob_freya_spawned;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_str_iron_roots";
    newscript->GetAI = &GetAI_mob_str_iron_roots;
    newscript->RegisterSelf();
}
