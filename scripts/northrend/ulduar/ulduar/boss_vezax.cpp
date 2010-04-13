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
SDName: boss_vezax
SD%Complete:
SDComment: despawn vapors in hard mode, implement mark of the faceless
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO       = -1603120,
    SAY_SURGE       = -1603123,
    SAY_HARD        = -1603126,
    SAY_SLAY1       = -1603121,
    SAY_SLAY2       = -1603122,
    SAY_BERSERK     = -1603125,
    SAY_DEATH       = -1603124,


    SPELL_AURA_OF_DESPAIR       = 62692,
    SPELL_SHADOW_CRASH          = 62660,
    SPELL_MARK_OF_FACELESS      = 63276,
    SPELL_SEARING_FLAMES        = 62661,
    SPELL_SURGE_OF_DARKNESS     = 62662,
    SPELL_BERSERK               = 26662, 
    SPELL_SARONITE_BARRIER      = 63364,

    NPC_SARONITE_VAPOR          = 33488,
    SPELL_SARONITE_VAPORS       = 63322, //63323, //not working yet

    NPC_SARONITE_ANIMUS         = 33524,
    SPELL_PROFOUND_DARKNESS     = 63420, 

    ACHIEV_MORNING_SARONITE     = 3181,
    ACHIEV_MORNING_SARONITE_H   = 3188,
};

struct VaporsPositions
{
    float x, y, z;
};
 
// Saronite Vapors Spawn Positions
static VaporsPositions VaporsSP[] =
{
    {1888.806396f,128.965668f,342.380219f},
    {1876.695557f,149.551529f,342.354980f},
    {1827.845093f,147.315857f,341.311432f},
    {1809.365112f,115.901085f,342.377747f},
    {1850.341309f,151.806229f,342.377869f},
    {1872.992554f,106.913689f,342.377960f},
    {1813.253906f,97.265800f,342.377960f},
    {1845.484863f,124.527130f,341.802368f}
};

uint8 SaroniteVaporCount;

struct MANGOS_DLL_DECL boss_vezaxAI : public ScriptedAI
{
    boss_vezaxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 EnrageTimer;
    uint32 CrashTimer;
    uint32 MarkTimer;
    uint32 FlamesTimer;
    uint32 SurgeTimer;
    uint32 SaroniteVaporTimer;
    uint32 AddsCheckTimer;
    uint32 SimphonTimer;
    uint32 EndSimphonTimer;

    bool isHardMode;
    bool hasMark;
    float zLoc;

    bool m_bHasCheckedKeepers;

    void Reset()
    {
        EnrageTimer = 600000; //10 minutes
        FlamesTimer = 10000 + rand()%10000;
        SaroniteVaporTimer = 40000;

        isHardMode = false;
        hasMark = false;

        SurgeTimer = urand(60000, 70000);
        MarkTimer = 30000;
        CrashTimer = 10000;
        AddsCheckTimer = 2000;
        SimphonTimer = 1000;
        EndSimphonTimer = 10000;

        SaroniteVaporCount = 0;

        zLoc = 342.188f;

        m_bHasCheckedKeepers = false;
        
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VEZAX, NOT_STARTED);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VEZAX, IN_PROGRESS);
        DoCast(NULL, SPELL_AURA_OF_DESPAIR, true);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_VEZAX, DONE);

            if(isHardMode)
            {
                m_pInstance->SetData(TYPE_VEZAX_HARD, DONE);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_MORNING_SARONITE : ACHIEV_MORNING_SARONITE_H);
            }
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void CheckAdds()
    {
        if(SaroniteVaporCount == 6)
        {
            DoCast(m_creature, SPELL_SARONITE_BARRIER);
            DoScriptText(SAY_HARD, m_creature);
            Creature *Animus = m_creature->SummonCreature(NPC_SARONITE_ANIMUS, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            Animus->SetInCombatWithZone();
            isHardMode = true;
            SaroniteVaporCount = 0;        
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!m_bHasCheckedKeepers)
        {
            // hodir doesn't save!!! fix it
            if(m_pInstance && /*m_pInstance->GetData(TYPE_HODIR) == DONE &&*/ m_pInstance->GetData(TYPE_FREYA) == DONE && m_pInstance->GetData(TYPE_THORIM) == DONE && m_pInstance->GetData(TYPE_MIMIRON) == DONE)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bHasCheckedKeepers = true;
            }
            else
            {
                //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bHasCheckedKeepers = true;
            }
        }

        // check for adds
        if(!isHardMode)
        {
            if(AddsCheckTimer < diff)
            {
                CheckAdds();
                AddsCheckTimer = 2000;
            }
            else AddsCheckTimer -= diff;

            // saronite vapor
            if(SaroniteVaporTimer < diff && (SaroniteVaporCount < 8))
            {      
                m_creature->SummonCreature(NPC_SARONITE_VAPOR, VaporsSP[SaroniteVaporCount].x, VaporsSP[SaroniteVaporCount].y, VaporsSP[SaroniteVaporCount].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 25000);
                SaroniteVaporCount += 1;
                if(SaroniteVaporCount == 8)
                    SaroniteVaporCount = 0;
                SaroniteVaporTimer = 40000;
            }
            else SaroniteVaporTimer -= diff;

        }

        // searing flames
        if(FlamesTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SEARING_FLAMES);
            FlamesTimer = 10000 + rand()%10000;
        }
        else FlamesTimer -= diff; 

        // surge of darkness
        if(SurgeTimer < diff)
        {
            DoScriptText(SAY_SURGE, m_creature);
            DoCast(m_creature->getVictim(), SPELL_SURGE_OF_DARKNESS);
            SurgeTimer = urand(60000, 70000);
        }
        else SurgeTimer -= diff; 

        // mark of faceless
        if(MarkTimer < diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_MARK_OF_FACELESS);

            hasMark = true;
            MarkTimer = 30000;
            EndSimphonTimer = 10000;
        }
        else MarkTimer -= diff;

        if(hasMark)
        {
            if(SimphonTimer < diff)
            {
                m_creature->SetHealth(m_creature->GetHealth() + 5000);

                // drain life from target -> workaround
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                {
                    Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                    {
                        if (pUnit->HasAura(SPELL_MARK_OF_FACELESS))
                        {
                            pUnit->SetHealth(pUnit->GetHealth() - 5000);
                        }
                    }
                }
                SimphonTimer = 1000;
            }
            else SimphonTimer -= diff; 

            // end simphon
            if(EndSimphonTimer < diff)
            {
                hasMark = false;  
            }
            else EndSimphonTimer -= diff;
        }

        // shadow crash
        if(CrashTimer < diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_SHADOW_CRASH);
            CrashTimer = 10000;
        }
        else CrashTimer -= diff;

        // enrage 10 min
        if(EnrageTimer < diff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_saronite_animusAI : public ScriptedAI
{
    mob_saronite_animusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 ProfoundDarknessTimer;

    void Reset()
    {
        ProfoundDarknessTimer = 5000;
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
        {
            if (Creature* pVezax = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_VEZAX))))
            {
                if (pVezax->isAlive())
                    pVezax->RemoveAurasDueToSpell(SPELL_SARONITE_BARRIER);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(ProfoundDarknessTimer < diff)
        {
            DoCast(m_creature, SPELL_PROFOUND_DARKNESS);
            ProfoundDarknessTimer = 5000;
        }
        else ProfoundDarknessTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_saronite_vaporAI : public ScriptedAI
{
    mob_saronite_vaporAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {}

    void JustDied(Unit *killer)
    {
        SaroniteVaporCount -= 1;
        DoCast(killer, SPELL_SARONITE_VAPORS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_boss_vezax(Creature* pCreature)
{
    return new boss_vezaxAI(pCreature);
}

CreatureAI* GetAI_mob_saronite_animus(Creature* pCreature)
{
    return new mob_saronite_animusAI(pCreature);
}

CreatureAI* GetAI_mob_saronite_vapor(Creature* pCreature)
{
    return new mob_saronite_vaporAI(pCreature);
}

void AddSC_boss_vezax()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_vezax";
    newscript->GetAI = &GetAI_boss_vezax;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_saronite_animus";
    newscript->GetAI = &GetAI_mob_saronite_animus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_saronite_vapor";
    newscript->GetAI = &GetAI_mob_saronite_vapor;
    newscript->RegisterSelf();
}


/* check this:

enum Enums
{
    //General Vezax yells
    SAY_VEZAX_AGGRO                     = -1999925,
    SAY_VEZAX_KILL_PLAYER_1             = -1999926,
    SAY_VEZAX_KILL_PLAYER_2             = -1999927,
    SAY_VEZAX_SUMMON                    = -1999928,
    SAY_VEZAX_ENRAGE                    = -1999929,
    SAY_VEZAX_DEATH                     = -1999930,
    SAY_VEZAX_SURGE                     = -1999931,
 
    //Vezax spells
    SPELL_ENRAGE                        = 61632, // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_AURA_OF_DESPAIR               = 62692, // Unable to regenerate or gain mana from most natural sources. Melee attack speed reduced by 20%.
    SPELL_SHADOW_CRASH                  = 60835, // Fires a missile towards a random target. When this missile lands, it deals 5655 to 6345 Shadow damage to all enemies within 10 yards of that location.
    SPELL_SHADOW_CRASH_H                = 63721, // deals 11310 to 12690 Shadow damage
    SPELL_SEARING_FLAMES                = 62661, // Deals 13875 to 16125 Fire damage to all enemies within 100 yards, Armor reduced by 75% for 10 seconds.
    SPELL_SURGE_OF_DARKNESS             = 62662, // Physical damage increased by 100%. Movement speed reduced by 55%.
    SPELL_MARK_OF_FACELESS              = 63276, // Siphoning 5000 health from nearby allies every 1 sec., healing General Vezax.
    SPELL_CORRUPTED_RAGE                = 68415, // Your insight into Shamanistic Rage is intensified but corrupted by General Vezax's Aura of Despair, hindering your ability to heal.
    SPELL_SARONITE_BARRIER              = 63364,
 
    //summons
    BOSS_SARONITE_ANIMUS                = 33524, //only hardmode (if at least 6 saronite vapors arent destroyed)
    NPC_SARONITE_VAPORS                 = 33488,
 
    //summons spells
    SPELL_PROFOUND_OF_DARKNESS          = 63420, //saronite animus
    SPELL_SARONITE_VAPORS               = 63323, //not working yet
 
    //phases
    PHASE_NORMAL                        = 1,
    PHASE_ANIMUS_SPAWNED                = 2,
    PHASE_HARD_MODE_ACTIVE              = 3
};
 
struct VaporsPositions
{
    float x, y, z;
};
 
// Saronite Vapors Spawn Positions
static VaporsPositions VaporsSP[] =
{
    {1888.806396,128.965668,342.380219},
    {1876.695557,149.551529,342.354980},
    {1827.845093,147.315857,341.311432},
    {1809.365112,115.901085,342.377747},
    {1850.341309,151.806229,342.377869},
    {1872.992554,106.913689,342.377960},
    {1813.253906,97.265800,342.377960},
    {1845.484863,124.527130,341.802368}
};
 
struct TRINITY_DLL_DECL boss_general_vezaxAI : public BossAI
{
    boss_general_vezaxAI(Creature* pCreature) : BossAI(pCreature, TYPE_VEZAX) {}
 
    uint32 EnrageTimer;
    uint32 ShadowCrashTimer;
    uint32 SearingFlamesTimer;
    uint32 SurgeOfDarknessTimer;
    uint32 MarkOfFacelessTimer;
    uint32 SummonVaporsTimer;
 
    uint8 Phase;
    uint8 VaporsCounter;
    bool HardMode;
    bool AchievHardModeKill;
    bool AchievShadowdodger;
    bool AnimusSummoned;
 
    void Reset()
    {
        EnrageTimer = 10*MINUTE*IN_MILISECONDS;
        ShadowCrashTimer = 10000;
        SearingFlamesTimer = urand(5000,10000);
        SurgeOfDarknessTimer = 65000;
        MarkOfFacelessTimer = urand(15000,25000);
        SummonVaporsTimer = 30000;
 
        Phase = PHASE_NORMAL;
        VaporsCounter = 0;
        HardMode = true;
        AchievHardModeKill = true;
        AchievShadowdodger = true;
        AnimusSummoned = false;
 
        m_creature->ResetLootMode();
        m_creature->SetMaxHealth(HEROIC(5000000,20000000));
    }
 
    void EnterCombat(Unit* pWho)
    {
        DoScriptText(SAY_VEZAX_AGGRO, m_creature);
        DoZoneInCombat();
 
        m_creature->ApplySpellImmune(0, IMMUNITY_ID, SPELL_AURA_OF_DESPAIR, true);
        DoCastAOE(SPELL_AURA_OF_DESPAIR, true);
        m_creature->AddLootMode(2); // emblem of valor drop
    }
 
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_VEZAX_DEATH, m_creature);
    }
 
    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(RAND(SAY_VEZAX_KILL_PLAYER_1,SAY_VEZAX_KILL_PLAYER_2), m_creature);
    }
 
    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;
 
        if (diff > EnrageTimer) {
            DoCast(m_creature, SPELL_ENRAGE);
            DoScriptText(SAY_VEZAX_ENRAGE, m_creature);
        } else EnrageTimer -= diff;
 
        if (Phase != PHASE_ANIMUS_SPAWNED) {
            if (diff > SearingFlamesTimer) {
                DoCastVictim(SPELL_SEARING_FLAMES);
                SearingFlamesTimer = urand(5000,10000);
            } else SearingFlamesTimer -= diff;
        }
 
        if (diff > SurgeOfDarknessTimer) {
            DoCast(m_creature, SPELL_SURGE_OF_DARKNESS);
            DoScriptText(SAY_VEZAX_SURGE, m_creature);
            SurgeOfDarknessTimer = 65000;
        } else SurgeOfDarknessTimer -= diff;
 
        if ((Phase == PHASE_NORMAL) && (VaporsCounter<8)) {
            if (diff > SummonVaporsTimer) {
                m_creature->SummonCreature(NPC_SARONITE_VAPORS, VaporsSP[VaporsCounter].x, VaporsSP[VaporsCounter].y, VaporsSP[VaporsCounter].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 25000);
                m_creature->SummonCreature(NPC_SARONITE_VAPORS, VaporsSP[VaporsCounter+1].x, VaporsSP[VaporsCounter+1].y, VaporsSP[VaporsCounter+1].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 25000);
                VaporsCounter+=2;
                if (VaporsCounter == 6) {
                    Phase = PHASE_ANIMUS_SPAWNED;
                    DoCast(m_creature, SPELL_SARONITE_BARRIER);
                    m_creature->SummonCreature(BOSS_SARONITE_ANIMUS, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN);
                    m_creature->RemoveLootMode(2);
                    m_creature->AddLootMode(4); // change emblem of valor for emblem of conquest and add hard mode loot items
                }
            } else SummonVaporsTimer -= diff;
        }
    }
};
*/