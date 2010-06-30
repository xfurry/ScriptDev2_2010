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
SDName: boss_krick_and_ick
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    // ick
    SPELL_POISON_NOVA             = 68989,
    SPELL_POISON_NOVA_H           = 70434,
    SPELL_MIGHTY_KICK             = 69021,
    SPELL_PURSUED                 = 68987,
    SPELL_CONFUSION               = 69029,
    // krick
    SPELL_TOXIC_WASTE             = 69024,
    SPELL_TOXIC_WASTE_H           = 70436,
    SPELL_STRANGULATING           = 69413,
    SPELL_SHADOW_BOLT             = 69028,
    SPELL_EXPLOSIVE_BARRAGE       = 69263,
    NPC_EXPLOSIVE_ORB             = 36610,
    SPELL_EXPLOSIVE_BARRAGE_ORB   = 69019,
    SPELL_EXPLOSIVE_BARRAGE_ORB_H = 70433,

    SAY_KRICK_CHASE_1             = -1610064,
    SAY_KRICK_CHASE_2             = -1610065,
    SAY_KRICK_CHASE_3             = -1610066,
    SAY_ICK_CHASE                 = -1609334, // invalid
    SAY_KRICK_POISON              = -1610061,
    SAY_ICK_POISON                = -1609336, // invalid
    SAY_KRICK_BARRAGE             = -1610062,
    SAY_KRICK_BARRAGE_EMOTE       = -1610063,
    SAY_AGGRO                     = -1610060,
    SAY_SLAY1                     = -1610067,
    SAY_SLAY2                     = -1610068,

    SAY_OUTRO1              = -1610069,
    SAY_OUTRO2_ALY          = -1610070,
    SAY_OUTRO2_HORDE        = -1610071,
    SAY_OUTRO3              = -1610072,
    SAY_OUTRO4_ALY          = -1610073,
    SAY_OUTRO4_HORDE        = -1610074,
    SAY_OUTRO5              = -1610075,
    SAY_OUTRO6_TYRANNUS     = -1610076,
    SAY_OUTRO7              = -1610077,
    SAY_OUTRO8_TYRANNUS     = -1610078,
    SAY_OUTRO9_ALY          = -1610079,
    SAY_OUTRO9_HORDE        = -1610080,
};

const float KrickPos[4] = {856.237f, 120.484f, 510.01f, 3.48f};

#define HOME_X                      852.322f
#define HOME_Y                      127.969f

struct MANGOS_DLL_DECL boss_IckAI : public ScriptedAI
{
    boss_IckAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiKrickGUID;

    uint32 m_uiPoisonNovaTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiPursueDelayTimer;
    uint32 m_uiMightKickTimer;

    void Reset()
    {
        m_uiKrickGUID       = 0;
        m_uiPoisonNovaTimer = 30000;
        m_uiPursueTimer     = 10000;
        m_uiPursueDelayTimer= 30000;
        m_uiMightKickTimer  = 20000;
    }

    void KilledUnit(Unit *victim)
    {
        if(Creature* pKrick = m_pInstance->instance->GetCreature(m_uiKrickGUID))
        {
            if(irand(0,1))
                DoScriptText(SAY_SLAY1, pKrick);
            else
                DoScriptText(SAY_SLAY2, pKrick);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK_AND_ICK, IN_PROGRESS);

        if(!GetClosestCreatureWithEntry(m_creature, NPC_KRICK, 50.0f))
        {
            if(Creature* pKrick = m_creature->SummonCreature(NPC_KRICK, KrickPos[0], KrickPos[1], KrickPos[2], KrickPos[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
            {
                if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pKrick->AddThreat(pTarget, 0.0f);
                DoScriptText(SAY_AGGRO, pKrick);
                m_uiKrickGUID = pKrick->GetGUID();
            }
        }
        else
        {
            if(Creature* pKrick = GetClosestCreatureWithEntry(m_creature, NPC_KRICK, 80.0f))
            {
                if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pKrick->AddThreat(pTarget, 0.0f);
                DoScriptText(SAY_AGGRO, pKrick);
                m_uiKrickGUID = pKrick->GetGUID();
            }
        }
    }

    void JustDied(Unit *victim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK_AND_ICK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPoisonNovaTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->instance->GetCreature(m_uiKrickGUID))
                DoScriptText(SAY_KRICK_POISON, pKrick);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_POISON_NOVA : SPELL_POISON_NOVA_H);
            m_uiPoisonNovaTimer = 30000;
        }
        else
            m_uiPoisonNovaTimer -= uiDiff;

        if (m_uiPursueTimer < uiDiff)
        {
            if (Creature* pKrick = m_pInstance->instance->GetCreature(m_uiKrickGUID))
            {
                switch (urand(0, 2))
                {
                case 0: 
                    DoScriptText(SAY_KRICK_CHASE_1, pKrick);
                    break;
                case 1: 
                    DoScriptText(SAY_KRICK_CHASE_2, pKrick);
                    break;
                case 2: 
                    DoScriptText(SAY_KRICK_CHASE_3, pKrick);
                    break;
                }
            }

            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_PURSUED);
            
            m_uiPursueTimer = 13000;
            m_uiPursueDelayTimer = 1000;
        }
        else
            m_uiPursueTimer -= uiDiff;

        if(m_uiPursueDelayTimer < uiDiff)
        {
            //DoCast(m_creature, SPELL_CONFUSION);
            m_uiPursueDelayTimer = 30000;
        }
        else m_uiPursueDelayTimer -= uiDiff;

        if (m_uiMightKickTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIGHTY_KICK);
            m_uiMightKickTimer = 25000;
        }
        else
            m_uiMightKickTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 80)
            EnterEvadeMode();
    }
};

struct MANGOS_DLL_DECL boss_KrickAI : public ScriptedAI
{
    boss_KrickAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiToxicWasteTimer;
    uint32 m_uiShadowboltTimer;
    uint32 m_uiExplosivBarrageTimer;

    uint32 m_uiSummonOrbsTimer;
    uint32 m_uiSummonOverTimer;
    bool m_bIsSummoning;

    bool m_bIsOutro;
    uint32 OutroTimer;
    uint32 Step;

    uint32 TeamInInstance;

    uint64 m_uiJainaGuid;
    uint64 m_uiSylvanasGuid;
    uint64 m_uiTyrannusGuid;

    void Reset()
    {
        m_uiJainaGuid       = 0;
        m_uiSylvanasGuid    = 0;
        m_uiTyrannusGuid    = 0;

        m_uiToxicWasteTimer      = 5000;
        m_uiShadowboltTimer      = 15000;
        m_uiExplosivBarrageTimer = 35000;
        m_uiSummonOrbsTimer     = 600000;
        m_uiSummonOverTimer     = 600000;
        m_bIsSummoning          = false;

        TeamInInstance = GetFaction();

        m_bIsOutro = false;
        OutroTimer = 10000;
        Step       = 1;
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bIsOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if (m_uiToxicWasteTimer < uiDiff)
            {
                if(Creature* pIck = GetClosestCreatureWithEntry(m_creature, NPC_ICK, 100.0f))
                {
                    if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_TOXIC_WASTE : SPELL_TOXIC_WASTE_H);
                }
                m_uiToxicWasteTimer = 10000;
            }
            else
                m_uiToxicWasteTimer -= uiDiff;

            if (m_uiShadowboltTimer < uiDiff)
            {
                if(Creature* pIck = GetClosestCreatureWithEntry(m_creature, NPC_ICK, 100.0f))
                {
                    if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_SHADOW_BOLT);
                }
                m_uiShadowboltTimer = 15000;
            }
            else
                m_uiShadowboltTimer -= uiDiff;

            if (m_uiExplosivBarrageTimer < uiDiff)
            {
                DoScriptText(SAY_KRICK_BARRAGE, m_creature);
                DoScriptText(SAY_KRICK_BARRAGE_EMOTE, m_creature);

                DoCast(m_creature, SPELL_EXPLOSIVE_BARRAGE);
                m_uiSummonOrbsTimer = 3000;
                m_uiSummonOverTimer = 18000;
                m_bIsSummoning = true;
                m_uiExplosivBarrageTimer = 45000;
            }
            else
                m_uiExplosivBarrageTimer -= uiDiff;

            if (m_uiSummonOrbsTimer < uiDiff && m_bIsSummoning)
            {
                for(uint8 i = 0; i < 4; ++i)
                {
                    if(Creature* pIck = GetClosestCreatureWithEntry(m_creature, NPC_ICK, 100.0f))
                    {
                        if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            m_creature->SummonCreature(NPC_EXPLOSIVE_ORB, pTarget->GetPositionX() + urand(0, 3), pTarget->GetPositionY() + urand(0, 3), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                    }
                }
                m_uiSummonOrbsTimer = 1500;
            }
            else
                m_uiSummonOrbsTimer -= uiDiff;

            if (m_uiSummonOverTimer < uiDiff && m_bIsSummoning)
            {
                m_bIsSummoning = false;
                m_uiSummonOverTimer = 60000;
            }
            else
                m_uiSummonOverTimer -= uiDiff;

            if(!GetClosestCreatureWithEntry(m_creature, NPC_ICK, 100.0f))
                m_bIsOutro = true;

            if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 80)
                EnterEvadeMode();
        }
        if(m_bIsOutro)
        {
            switch(Step)
            {
            case 1:
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->GetMotionMaster()->MoveIdle();
                DoScriptText(SAY_OUTRO1, m_creature);

                if(TeamInInstance == ALLIANCE)
                {
                    if(Creature* pJaina = m_creature->SummonCreature(NPC_JAINA_START, 783.565f, 112.559f, 509.461f, 0, TEMPSUMMON_TIMED_DESPAWN, 80000))
                    {
                        pJaina->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX() - 15, m_creature->GetPositionY(), m_creature->GetPositionZ());
                        m_uiJainaGuid = pJaina->GetGUID();
                    }
                }

                if(TeamInInstance == HORDE)
                {
                    if(Creature* pSylvanas = m_creature->SummonCreature(NPC_SYLVANAS_START, 783.565f, 112.559f, 509.461f, 0, TEMPSUMMON_TIMED_DESPAWN, 80000))
                    {
                        pSylvanas->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX() - 15, m_creature->GetPositionY(), m_creature->GetPositionZ());
                        m_uiSylvanasGuid = pSylvanas->GetGUID();
                    }
                }
                ++Step;
                OutroTimer = 15000;
                break;
            case 3:
                if(TeamInInstance == ALLIANCE)
                {
                    if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGuid))
                    {
                        DoScriptText(SAY_OUTRO2_ALY, pJaina);
                        pJaina->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                    }
                }
                if(TeamInInstance == HORDE)
                {
                    if(Creature* pSylvanas = m_pInstance->instance->GetCreature(m_uiSylvanasGuid))
                    {
                        DoScriptText(SAY_OUTRO2_HORDE, pSylvanas);
                        pSylvanas->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                    }
                }
                ++Step;
                OutroTimer = 5000;
                break;
            case 5:
                DoScriptText(SAY_OUTRO3, m_creature);
                ++Step;
                OutroTimer = 15000;
                break;
            case 7:
                if(TeamInInstance == ALLIANCE)
                    if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGuid))
                        DoScriptText(SAY_OUTRO4_ALY, pJaina);
                if(TeamInInstance == HORDE)
                    if(Creature* pSylvanas = m_pInstance->instance->GetCreature(m_uiSylvanasGuid))
                        DoScriptText(SAY_OUTRO4_HORDE, pSylvanas);
                ++Step;
                OutroTimer = 5000;
                break;
            case 9:
                DoScriptText(SAY_OUTRO5, m_creature);
                if(Creature* pTyrannus = m_creature->SummonCreature(NPC_TYRANNUS_INTRO, 860.649f, 124.863f, 536.019f, 3.43f, TEMPSUMMON_TIMED_DESPAWN, 20000))
                {
                    pTyrannus->GetMotionMaster()->MoveIdle();
                    pTyrannus->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                    pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    pTyrannus->GetMotionMaster()->MoveIdle();
                    pTyrannus->GetMap()->CreatureRelocation(pTyrannus, 860.649f, 124.863f, 536.019f, 3.43f);
                    pTyrannus->SendMonsterMove(860.649f, 124.863f, 536.019f, SPLINETYPE_NORMAL, pTyrannus->GetSplineFlags(), 1);
                    m_uiTyrannusGuid = pTyrannus->GetGUID();
                }
                ++Step;
                OutroTimer = 4000;
                break;
            case 11:
                if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                    DoScriptText(SAY_OUTRO6_TYRANNUS, pTyrannus);
                ++Step;
                OutroTimer = 4000;
                break;
            case 13:
                DoScriptText(SAY_OUTRO7, m_creature);
                ++Step;
                OutroTimer = 3000;
                break;
            case 15:
                m_creature->SetHealth(0);
                m_creature->SetStandFlags(UNIT_STAND_STATE_DEAD);
                if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                    DoScriptText(SAY_OUTRO8_TYRANNUS, pTyrannus);
                ++Step;
                OutroTimer = 10000;
                break;
            case 17:
                if(TeamInInstance == ALLIANCE)
                    if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGuid))
                        DoScriptText(SAY_OUTRO9_ALY, pJaina);
                if(TeamInInstance == HORDE)
                    if(Creature* pSylvanas = m_pInstance->instance->GetCreature(m_uiSylvanasGuid))
                        DoScriptText(SAY_OUTRO9_HORDE, pSylvanas);
                m_creature->DealDamage(m_creature, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                ++Step;
                OutroTimer = 3000;
                break;
            }
            if (OutroTimer <= uiDiff)
            {
                ++Step;
                OutroTimer = 330000;
            } OutroTimer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL mob_explosive_orbAI : public ScriptedAI
{
    mob_explosive_orbAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        pCreature->SetDisplayId(11686);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiExplosiveBarrageTimer;

    void Reset()
    {
        m_uiExplosiveBarrageTimer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (m_uiExplosiveBarrageTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLOSIVE_BARRAGE_ORB : SPELL_EXPLOSIVE_BARRAGE_ORB_H);
            m_uiExplosiveBarrageTimer = 10000;
        }
        else
            m_uiExplosiveBarrageTimer -= uiDiff;      
    }
};

CreatureAI* GetAI_mob_explosive_orb(Creature* pCreature)
{
    return new mob_explosive_orbAI (pCreature);
}

CreatureAI* GetAI_boss_Ick(Creature* pCreature)
{
    return new boss_IckAI (pCreature);
}

CreatureAI* GetAI_boss_Krick(Creature* pCreature)
{
    return new boss_KrickAI (pCreature);
}

void AddSC_boss_Ick_and_Krick()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_Ick";
    newscript->GetAI = &GetAI_boss_Ick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_Krick";
    newscript->GetAI = &GetAI_boss_Krick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_explosive_orb";
    newscript->GetAI = &GetAI_mob_explosive_orb;
    newscript->RegisterSelf();
}
