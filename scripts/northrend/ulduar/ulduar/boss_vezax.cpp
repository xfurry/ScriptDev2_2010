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
            // Todo: add hard loot mode
            //m_creature->RemoveLootMode(2);
            //m_creature->AddLootMode(4)
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
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
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
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
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
*/