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
SDName: Boss_Sjonnir
SD%Complete: 20%
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"
#include "halls_of_stone.h"

enum
{
    SAY_AGGRO                       = -1599000,
    SAY_SLAY_1                      = -1599001,
    SAY_SLAY_2                      = -1599002,
    SAY_SLAY_3                      = -1599003,
    SAY_DEATH                       = -1599004,
    EMOTE_GENERIC_FRENZY            = -1000002,

    SPELL_FRENZY                    = 28747,

    SPELL_CHAIN_LIGHTNING           = 50830,
    SPELL_CHAIN_LIGHTNING_H         = 59844,

    SPELL_STATIC_CHARGE             = 50834,
    SPELL_STATIC_CHARGE_H           = 59846,

    SPELL_LIGHTNING_SHIELD          = 50831,
    SPELL_LIGHTNING_SHIELD_H        = 59845,

    SPELL_LIGHTNING_RING            = 50840,
    SPELL_LIGHTNING_RING_H          = 59848,

    SPELL_SUMMON_IRON_DWARF         = 50789,                // periodic dummy aura, tick each 30sec or each 20sec in heroic
    SPELL_SUMMON_IRON_DWARF_H       = 59860,                // left/right 50790,50791

    SPELL_SUMMON_IRON_TROGG         = 50792,                // periodic dummy aura, tick each 10sec or each 7sec in heroic
    SPELL_SUMMON_IRON_TROGG_H       = 59859,                // left/right 50793,50794

    SPELL_SUMMON_MALFORMED_OOZE     = 50801,                // periodic dummy aura, tick each 5sec or each 3sec in heroic
    SPELL_SUMMON_MALFORMED_OOZE_H   = 59858,                // left/right 50802,50803

    SPELL_SUMMON_IRON_SLUDGE        = 50747,                // instakill TARGET_SCRIPT
    SPELL_IRON_SLUDGE_SPAWN_VISUAL  = 50777,

    NPC_IRON_TROGG                  = 27979,
    NPC_IRON_DWARF                  = 27982,
    NPC_MALFORMED_OOZE              = 27981,
    NPC_IRON_SLUDGE                 = 28165,

    SPELL_TOXIC_VOLLEY              = 50838,
    SPELL_TOXIC_VOLLEY_H            = 59853,

    ACHIEV_HOS                      = 485,
    ACHIEV_HOS_H                    = 496,
    ACHIEV_ABUSE_THE_OOZE           = 2155,         // needs script support
};

uint32 m_uiSludgeCounter;

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations PipeLoc[]=
{
    {1295.44f, 734.07f, 200.3f}, // left
    {1297.7f,  595.6f,  199.9f}, // right
};

/*######
## boss_sjonnir
######*/

struct MANGOS_DLL_DECL boss_sjonnirAI : public ScriptedAI
{
    boss_sjonnirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsFrenzy;

    std::list<uint64> m_lDwarfGUIDList;
    uint32 m_uiChainLightning_Timer;
    uint32 m_uiLightningShield_Timer;
    uint32 m_uiStaticCharge_Timer;
    uint32 m_uiLightningRing_Timer;
    uint32 m_uiSummon_Timer;
    uint32 m_uiFrenzy_Timer;

    uint32 m_uiOozeCounter;

    void Reset()
    {
        // exploit check
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_uiChainLightning_Timer = 3000 + rand()%5000;
        m_uiLightningShield_Timer = 20000 + rand()%5000;
        m_uiStaticCharge_Timer = 20000 + rand()%5000;
        m_uiLightningRing_Timer = 30000 + rand()%5000;
        m_uiSummon_Timer = 5000;
        m_uiFrenzy_Timer = 300000;

        DespawnDwarf();
        m_uiSludgeCounter = 0;
        m_uiOozeCounter = 0;

        if (m_creature->isAlive())
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_SHIELD : SPELL_LIGHTNING_SHIELD_H, false);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_SJONNIR, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_SUMMON_IRON_DWARF : SPELL_SUMMON_IRON_DWARF_H, true);
        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_SUMMON_IRON_TROGG : SPELL_SUMMON_IRON_TROGG_H, true);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_SJONNIR, IN_PROGRESS);
    }


    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_IRON_TROGG || pSummoned->GetEntry() == NPC_IRON_DWARF || pSummoned->GetEntry() == NPC_MALFORMED_OOZE)
        {
            float fX, fY, fZ;
            pSummoned->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 10.0f, fX, fY, fZ);

            pSummoned->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            pSummoned->GetMotionMaster()->MovePoint(0, fX, fY, fZ);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance && m_pInstance->GetData(TYPE_KRYSTALLUS) == DONE && m_pInstance->GetData(TYPE_MAIDEN) == DONE && m_pInstance->GetData(TYPE_TRIBUNAL) == DONE)
        {
            m_pInstance->SetData(TYPE_SJONNIR, DONE);

            // Complete achiev for Tribunal
            m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_HOS : ACHIEV_HOS_H);

            if(!m_bIsRegularMode && m_uiSludgeCounter >=5)
                m_pInstance->DoCompleteAchievement(ACHIEV_ABUSE_THE_OOZE);
        }
    }

    void DespawnDwarf()
    {
        if (m_lDwarfGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lDwarfGUIDList.begin(); itr != m_lDwarfGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
            {
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
            }
        }

        m_lDwarfGUIDList.clear();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // exploit check
        if(m_pInstance && m_pInstance->GetData(TYPE_TRIBUNAL) == DONE)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChainLightning_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, !m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING_H : SPELL_CHAIN_LIGHTNING);
            m_uiChainLightning_Timer = 10000 + rand()%5000;
        }
        else
            m_uiChainLightning_Timer -= uiDiff;

        if (m_uiLightningShield_Timer < uiDiff)
        {
            DoCast(m_creature, !m_bIsRegularMode ? SPELL_LIGHTNING_SHIELD_H : SPELL_LIGHTNING_SHIELD);
            m_uiLightningShield_Timer = 20000 + rand()%5000;
        }
        else
            m_uiLightningShield_Timer -= uiDiff;

        if (m_uiStaticCharge_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), !m_bIsRegularMode ? SPELL_STATIC_CHARGE_H : SPELL_STATIC_CHARGE);
            m_uiStaticCharge_Timer = 20000 + rand()%5000;
        }
        else
            m_uiStaticCharge_Timer -= uiDiff;

        if (m_uiLightningRing_Timer < uiDiff)
        {
            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);
            DoCast(m_creature, !m_bIsRegularMode ? SPELL_LIGHTNING_RING_H : SPELL_LIGHTNING_RING);
            m_uiLightningRing_Timer = 30000 + rand()%5000;
        }
        else
            m_uiLightningRing_Timer -= uiDiff;

        if (m_uiSummon_Timer < uiDiff)
        {
            uint32 SummonPipe = rand()%2;
            uint32 SummonEntry = 0;
            switch(rand()%3)
            {
            case 0: SummonEntry = NPC_IRON_TROGG; break;
            case 1: SummonEntry = NPC_MALFORMED_OOZE;    break;
            case 2: SummonEntry = NPC_IRON_DWARF; break;
            }
            m_creature->SummonCreature(SummonEntry, PipeLoc[SummonPipe].x, PipeLoc[SummonPipe].y, PipeLoc[SummonPipe].z, 0.0f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);
            m_uiSummon_Timer = 20000;
        }
        else
            m_uiSummon_Timer -= uiDiff;

        if (!m_bIsFrenzy && m_uiFrenzy_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_FRENZY);
            m_bIsFrenzy = true;
            m_uiFrenzy_Timer = 0;
        }
        else
            m_uiFrenzy_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sjonnir(Creature* pCreature)
{
    return new boss_sjonnirAI(pCreature);
}

/*######
## mob_iron_sludge
######*/

struct MANGOS_DLL_DECL mob_iron_sludgeAI : public ScriptedAI
{
    mob_iron_sludgeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiSpellTimer = 5000;
    }

    void JustDied(Unit* pKiller)
    {
        m_uiSludgeCounter += 1;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_TOXIC_VOLLEY : SPELL_TOXIC_VOLLEY_H);
            m_uiSpellTimer = 5000;
        }
        else
            m_uiSpellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_iron_sludge(Creature* pCreature)
{
    return new mob_iron_sludgeAI(pCreature);
}

void AddSC_boss_sjonnir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sjonnir";
    newscript->GetAI = &GetAI_boss_sjonnir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_sludge";
    newscript->GetAI = &GetAI_mob_iron_sludge;
    newscript->RegisterSelf();
}
