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
SDName: boss_bronjahm
SD%Complete: 70%
SDComment: Soulstorm, Corrupt soul
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum Spells
{
    SPELL_CORRUPT_SOUL          = 68839,
    SPELL_MAGICSBANE            = 68793,
    SPELL_MAGICSBANE_H          = 69050,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_SHADOW_BOLT_H         = 70043,
    SPELL_FEAR                  = 30584,//68950,
    SPELL_SOULSTORM             = 68872,
    SPELL_TELEPORT              = 68988,
    SPELL_SOULSTORM_EFFECT      = 68925,
    SPELL_SOULSTORM_DMG         = 68921,

    NPC_CORRUPT_SOUL_FRAGMENT   = 36535,
    EQUIP_ID                    = 35514,

    ACHIEV_SOUL_POWER           = 4522,
};

enum Yells
{
    SAY_AGGRO                               = -1610017,
    SAY_SOULSTORM                           = -1610018,
    SAY_SLAY1                               = -1610019,
    SAY_SLAY2                               = -1610020,
    SAY_CORRUPT_SOUL                        = -1610021,
    SAY_DEATH                               = -1610022,
};

#define HOME_X                      5297.33f
#define HOME_Y                      2506.64f

struct MANGOS_DLL_DECL boss_BronjahnAI : public ScriptedAI
{
    boss_BronjahnAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCorruptSoulTimer;
    uint32 m_uiMagicsBaneTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiSoulStormTimer;

    bool m_bIsSoulStorm;
    bool m_bHasTeleported;

    void Reset()
    {        
        m_uiCorruptSoulTimer    = urand(15000, 25000);
        m_uiMagicsBaneTimer     = urand(8000, 15000);
        m_uiShadowBoltTimer     = 2000;
        m_uiFearTimer           = 1000;

        m_bIsSoulStorm          = false;
        m_bHasTeleported        = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHN, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHN, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit *pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)  
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiCorruptSoulTimer < uiDiff && !m_bIsSoulStorm)
        {
            DoScriptText(SAY_CORRUPT_SOUL, m_creature);
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_CORRUPT_SOUL);
                /*if(Creature *pTemp = m_creature->SummonCreature(NPC_CORRUPT_SOUL_FRAGMENT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                {
                    pTemp->SetSplineFlags(SPLINEFLAG_WALKMODE);
                    pTemp->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                }*/
            }
            m_uiCorruptSoulTimer = urand(15000, 25000);
        }
        else
            m_uiCorruptSoulTimer -= uiDiff;

        if (m_uiMagicsBaneTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGICSBANE : SPELL_MAGICSBANE_H);
            m_uiMagicsBaneTimer = urand(8000, 15000);
        }
        else
            m_uiMagicsBaneTimer -= uiDiff;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
            
            if(m_bIsSoulStorm)
                m_uiShadowBoltTimer = 3000;
            else
                m_uiShadowBoltTimer = 5000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        if (m_uiFearTimer < uiDiff && m_bIsSoulStorm)
        {
            m_creature->CastStop();
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(m_creature, SPELL_FEAR);
            m_uiFearTimer = urand(8000, 12000);
        }
        else
            m_uiFearTimer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 30.0f && !m_bHasTeleported)
        {
            m_creature->CastStop();
            m_uiSoulStormTimer = 2000;
            m_bHasTeleported = true;
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(0, 5297.33f, 2506.64f, 686.068f);     
        }

        if (m_uiSoulStormTimer < uiDiff && !m_bIsSoulStorm && m_bHasTeleported)
        {
            m_creature->CastStop();
            m_uiFearTimer = 7000;
            //DoCast(m_creature, SPELL_SOULSTORM); // spell broken!!!
            DoCast(m_creature, SPELL_SOULSTORM_EFFECT);
            DoScriptText(SAY_SOULSTORM, m_creature);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_bIsSoulStorm = true;;
        }
        else
            m_uiSoulStormTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 30)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_Bronjahn(Creature* pCreature)
{
    return new boss_BronjahnAI (pCreature);
}

struct MANGOS_DLL_DECL mob_corrupted_soul_fragmentAI : public ScriptedAI
{
    mob_corrupted_soul_fragmentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if (Creature* pBronjahm = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BRONJAHM))))
                {
                    float fDistance = m_creature->GetDistance2d(pBronjahm);
                    if (fDistance <= 2)
                    {
                        //((boss_bronjahmAI*)pBronjahm->AI())->SoulFragmentHit();
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_corrupted_soul_fragment(Creature* pCreature)
{
    return new mob_corrupted_soul_fragmentAI (pCreature);
}

void AddSC_boss_Bronjahn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_Bronjahn";
    newscript->GetAI = &GetAI_boss_Bronjahn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_corrupted_soul_fragment";
    newscript->GetAI = &GetAI_mob_corrupted_soul_fragment;
    newscript->RegisterSelf();
}
