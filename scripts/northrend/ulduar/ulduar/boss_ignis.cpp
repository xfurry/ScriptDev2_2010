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
SDName: boss_ignis
SD%Complete: 85%
SDComment: slag pot damage missing. missing bringing adds to water part
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO       = -1603010,
    SAY_SCORCH1     = -1603011,
    SAY_SCORCH2     = -1603012,
    SAY_SLAGPOT     = -1603013,
    SAY_FLAMEJETS   = -1603014,
    SAY_SUMMON      = -1603015,
    SAY_SLAY1       = -1603016,
    SAY_SLAY2       = -1603017,
    SAY_BERSERK     = -1603018,
    SAY_DEATH       = -1603019,

    //ignis the furnace master
    SPELL_FLAME_JETS			= 62680,
    SPELL_FLAME_JETS_H			= 63472,
    SPELL_SLAG_POT				= 62717,
    SPELL_SLAG_POT_H			= 63477,
    SPELL_SLAG_POT_DMG			= 65722,
    SPELL_SLAG_POT_DMG_H		= 65723,
    SPELL_SCORCH				= 62546,
    SPELL_SCORCH_H				= 63474,
    BUFF_STRENGHT_OF_CREATOR	= 64473,
    SPELL_HASTE					= 66045,
    SPELL_ENRAGE                = 26662,
    //iron construct
    SPELL_HEAT					= 65667,
    SPELL_MOLTEN				= 62373,
    SPELL_BRITTLE				= 62382,
    SPELL_SHATTER				= 62383,
    //scorch target
    AURA_SCORCH					= 62548,
    AURA_SCORCH_H				= 63476,
    AURA_HEAT                   = 65667,
    //NPC ids
    MOB_IRON_CONSTRUCT			= 33121,
    MOB_SCORCH_TARGET			= 33221,

    ACHIEV_STOKIN_THE_FURNACE   = 2930,
    ACHIEV_STOKIN_THE_FURNACE_H = 2929,
};

#define HOME_X                      586.747009f
#define HOME_Y                      381.997986f

// scorch target
struct MANGOS_DLL_DECL mob_scorch_targetAI : public ScriptedAI
{
    mob_scorch_targetAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Death_Timer;
    uint32 Range_Check_Timer;

    void Reset()
    {
        Death_Timer = 55000;
        Range_Check_Timer = 1000;
        m_creature->SetDisplayId(11686);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature,  m_bIsRegularMode ? AURA_SCORCH : AURA_SCORCH_H);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (Death_Timer < diff)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else Death_Timer -= diff;
    }
};

CreatureAI* GetAI_mob_scorch_target(Creature* pCreature)
{
    return new mob_scorch_targetAI(pCreature);
}

// iron construct
struct MANGOS_DLL_DECL mob_iron_constructAI : public ScriptedAI
{
    mob_iron_constructAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    uint32 Death_Timer;
    uint32 Aura_Check_Timer;
    uint32 ScorchTimer;
    uint32 MoltenTimer;
    uint32 BrittleTimer;
    bool brittle;
    bool shatter;
    bool molten;

    uint32 WaterCheckTimer;

    void Reset()
    {
        shatter = false;
        brittle = false;
        molten = false;
        WaterCheckTimer = 2000;
        ScorchTimer = 5000;
        Aura_Check_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->setFaction(35);
        m_creature->GetMotionMaster()->Clear();
        SetCombatMovement(true);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;
        if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_IGNIS))))
            if (pTemp->isAlive())
                if (pTemp->HasAura(BUFF_STRENGHT_OF_CREATOR))
                {
                    if (pTemp->GetAura(BUFF_STRENGHT_OF_CREATOR, EFFECT_INDEX_0)->GetStackAmount() == 1)
                        pTemp->RemoveAurasDueToSpell(BUFF_STRENGHT_OF_CREATOR);
                    else
                        pTemp->GetAura(BUFF_STRENGHT_OF_CREATOR, EFFECT_INDEX_0)->modStackAmount(-1);
                }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (brittle)
            if (damage>5000){
                DoCast(m_creature, SPELL_SHATTER);
                shatter = true;
                brittle = false;
                Death_Timer = 1000;
            }
    }

    void GetInCombat()
    { 
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_IGNIS))))
        {
            if (pTemp->isAlive())
            {
                m_creature->setFaction(14);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MovePoint(0, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ());

                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->AddThreat(pTarget,0.0f);
                    m_creature->AI()->AttackStart(pTarget);
                    m_creature->SetInCombatWithZone();
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Death_Timer < diff && shatter)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else Death_Timer -= diff;   

        if (m_creature->HasAura(SPELL_BRITTLE,EFFECT_INDEX_0))
            brittle = true;
        else
            brittle = false;

        if (Aura_Check_Timer < diff && !molten)
        {
            if(Aura* aura = m_creature->GetAura(SPELL_HEAT,EFFECT_INDEX_0))
                if(aura->GetStackAmount() > 19)
                {
                    DoCast(m_creature, SPELL_MOLTEN);
                    MoltenTimer = 30000;
                    molten = true;
                }
                Aura_Check_Timer = 1000;
        }else Aura_Check_Timer -= diff;

        //Water checks
        if(molten)
        {
            //timed molten
            if (MoltenTimer <= diff)
            {
                DoCast(m_creature, SPELL_BRITTLE);
                SetCombatMovement(false);
                m_creature->StopMoving();
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                MoltenTimer = 30000;
                BrittleTimer = 20000;
                brittle = true;
                molten = false;
            }else MoltenTimer -= diff;

            //TODO: implement water event!
            /*
            if (WaterCheckTimer <= diff)
            {
                if(m_creature->IsInWater())
                {
                    DoCast(m_creature, SPELL_BRITTLE);
                    brittle = true;
                    molten = false;
                }
                WaterCheckTimer = 2000;
            }else WaterCheckTimer -= diff;
            */
        } 

        if (BrittleTimer < diff && brittle)
        {
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            brittle = false;
            
            BrittleTimer = 20000;
        }else BrittleTimer -= diff;

        if (!shatter && !brittle)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_iron_construct(Creature* pCreature)
{
    return new mob_iron_constructAI(pCreature);
}

//ignis the furnace master
struct MANGOS_DLL_DECL boss_ignisAI : public ScriptedAI
{
    boss_ignisAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lIronConstructGUIDList;

    uint32 Flame_Jets_Timer;
    uint32 Slag_Pot_Timer;
    uint32 Slag_Pot_Dmg_Timer;
    uint32 Scorch_Timer;
    uint32 Summon_Timer;
    uint32 PotDmgCount;
    uint32 EnrageTimer;

    uint64 m_uiPotTarget;
    std::list<Creature*> lConstructs;
    Creature *pConstruct;

    uint32 uiEncounterTimer;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        Flame_Jets_Timer = 20000;
        Slag_Pot_Timer = 25000;
        Slag_Pot_Dmg_Timer = 26000;
        Scorch_Timer = 14000;
        Summon_Timer = 10000;
        EnrageTimer = 600000;   // 10 MIN
        PotDmgCount = 0;
        m_uiPotTarget = 0;
        m_lIronConstructGUIDList.clear();

        uiEncounterTimer = 0;

        GetCreatureListWithEntryInGrid(lConstructs, m_creature, MOB_IRON_CONSTRUCT, DEFAULT_VISIBILITY_INSTANCE);

        if (!lConstructs.empty())
        {
            for(std::list<Creature*>::iterator iter = lConstructs.begin(); iter != lConstructs.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        //death yell
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, DONE);

        if (!m_lIronConstructGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lIronConstructGUIDList.begin(); itr != m_lIronConstructGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                    pTemp->ForcedDespawn();
        }

        DoScriptText(SAY_DEATH, m_creature);

        if (uiEncounterTimer < 240000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_STOKIN_THE_FURNACE : ACHIEV_STOKIN_THE_FURNACE_H);
        }
    }

    Creature* SelectRandomConstruct(float fRange)
    {
        std::list<Creature* > lConstructList;
        GetCreatureListWithEntryInGrid(lConstructList, m_creature, MOB_IRON_CONSTRUCT, fRange);

        //This should not appear!
        if (lConstructList.empty()){
            EnterEvadeMode();
            debug_log("SD2: AhnKahet: No volunteer to sacriface!");
            return NULL;
        }

        std::list<Creature* >::iterator iter = lConstructList.begin();
        advance(iter, urand(0, lConstructList.size()-1));

        return *iter;
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_IGNIS, FAIL);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        uiEncounterTimer += diff;

        // enrage
        if(EnrageTimer < diff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_ENRAGE);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        if (Flame_Jets_Timer < diff)
        {
            //flame jets yell
            DoScriptText(SAY_FLAMEJETS, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_JETS : SPELL_FLAME_JETS_H);
            Flame_Jets_Timer = 30000;
        }else Flame_Jets_Timer -= diff;   

        if (Slag_Pot_Timer < diff)
        {
            //slag pot yell
            DoScriptText(SAY_SLAGPOT, m_creature);
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1)){
                DoCast(target, m_bIsRegularMode ? SPELL_SLAG_POT : SPELL_SLAG_POT_H);
                m_uiPotTarget = target->GetGUID();
            }
            Slag_Pot_Timer = 30000;
            Slag_Pot_Dmg_Timer = 1000;
            PotDmgCount = 0;
        }else Slag_Pot_Timer -= diff;  

        if (Slag_Pot_Dmg_Timer < diff)
        {
            if (Unit* pPotTarget = Unit::GetUnit(*m_creature, m_uiPotTarget)){
                if (PotDmgCount < 10)
                    DoCast(pPotTarget, m_bIsRegularMode ? SPELL_SLAG_POT_DMG : SPELL_SLAG_POT_DMG_H);
                else
                    if (PotDmgCount == 10)
                        DoCast(pPotTarget, SPELL_HASTE);
            }
            ++PotDmgCount;
            Slag_Pot_Dmg_Timer = 1000;
        }else Slag_Pot_Dmg_Timer -= diff;

        if (Summon_Timer < diff)
        {
            //summon yell
            DoScriptText(SAY_SUMMON, m_creature);

            /*
            if (Creature* pTemp = m_creature->SummonCreature(MOB_IRON_CONSTRUCT, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    pTemp->AddThreat(pTarget,0.0f);
                    pTemp->AI()->AttackStart(pTarget);
                    m_lIronConstructGUIDList.push_back(pTemp->GetGUID());
                }
            */

            pConstruct = SelectRandomConstruct(200.0f);
            if(pConstruct)
            {
                ((mob_iron_constructAI*)pConstruct->AI())->GetInCombat();
            }

            Summon_Timer = 40000;
            if (m_creature->HasAura(BUFF_STRENGHT_OF_CREATOR))
                m_creature->GetAura(BUFF_STRENGHT_OF_CREATOR, EFFECT_INDEX_0)->modStackAmount(+1);
            else
                DoCast(m_creature, BUFF_STRENGHT_OF_CREATOR);
        }else Summon_Timer -= diff;

        if (Scorch_Timer < diff)
        {
            if(irand(0,1))
                DoScriptText(SAY_SCORCH1, m_creature);
            else
                DoScriptText(SAY_SCORCH2, m_creature);

            DoCast(m_creature->getVictim(), SPELL_SCORCH);
            if (Creature* pTemp = m_creature->SummonCreature(MOB_SCORCH_TARGET, m_creature->getVictim()->GetPositionX(), m_creature->getVictim()->GetPositionY(), m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
            {
                pTemp->AddThreat(m_creature->getVictim(),0.0f);
                pTemp->AI()->AttackStart(m_creature->getVictim());
            }
            Scorch_Timer = 28000;
        }else Scorch_Timer -= diff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 200)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_ignis(Creature* pCreature)
{
    return new boss_ignisAI(pCreature);
}

void AddSC_boss_ignis()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_ignis";
    NewScript->GetAI = GetAI_boss_ignis;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_scorch_target";
    NewScript->GetAI = &GetAI_mob_scorch_target;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_iron_construct";
    NewScript->GetAI = &GetAI_mob_iron_construct;
    NewScript->RegisterSelf();
}