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
SDName: Boss_Ymiron
SD%Complete: 20%
SDComment:
SDCategory: Utgarde Pinnacle
EndScriptData */

#include "precompiled.h"
#include "utgarde_pinnacle.h"

enum
{
    SAY_AGGRO                   = -1575031,
    SAY_SUMMON_BJORN            = -1575032,
    SAY_SUMMON_HALDOR           = -1575033,
    SAY_SUMMON_RANULF           = -1575034,
    SAY_SUMMON_TORGYN           = -1575035,
    SAY_SLAY_1                  = -1575036,
    SAY_SLAY_2                  = -1575037,
    SAY_SLAY_3                  = -1575038,
    SAY_SLAY_4                  = -1575039,
    SAY_DEATH                   = -1575040,

    ACHIEV_KINGS_BANE           = 2157,
    SPELL_BANE_N                            = 48294,
    SPELL_BANE_H                            = 59301,
    SPELL_DARK_SLASH                        = 48292,
    SPELL_FETID_ROT_N                       = 48291,
    SPELL_FETID_ROT_H                       = 59300,
    SPELL_SCREAMS_OF_THE_DEAD               = 51750,
    SPELL_SPIRIT_BURST_N                    = 48529, // when Ranulf
    SPELL_SPIRIT_BURST_H                    = 59305, // when Ranulf
    SPELL_SPIRIT_STRIKE_N                   = 48423, // when Haldor
    SPELL_SPIRIT_STRIKE_H                   = 59304, // when Haldor
    SPELL_ANCESTORS_VENGEANCE               = 16939, // 2 normal, 4 heroic

    SPELL_SUMMON_AVENGING_SPIRIT            = 48592,
    SPELL_SUMMON_SPIRIT_FOUNT               = 48386,

    SPELL_CHANNEL_SPIRIT_TO_YMIRON          = 48316,
    SPELL_CHANNEL_YMIRON_TO_SPIRIT          = 48307,

    SPELL_SPIRIT_FOUNT_N                    = 48380,
    SPELL_SPIRIT_FOUNT_H                    = 59320,

    NPC_BJORN                               = 27303, // Near Right Boat, summon Spirit Fount
    NPC_BJORN_VISUAL                        = 27304,
    NPC_HALDOR                              = 27307, // Near Left Boat, debuff Spirit Strike on player
    NPC_HALDOR_VISUAL                       = 27310,
    NPC_RANULF                              = 27308, // Far Left Boat, ability to cast spirit burst
    NPC_RANULF_VISUAL                       = 27311,
    NPC_TORGYN                              = 27309, // Far Right Boat, summon 4 Avenging Spirit
    NPC_TORGYN_VISUAL                       = 27312,

    NPC_SPIRIT_FOUNT                        = 27339,
    NPC_AVENGING_SPIRIT                     = 27386
};

struct ActiveBoatStruct
{
    uint32 npc;
    uint32 say;
    float MoveX,MoveY,MoveZ,SpawnX,SpawnY,SpawnZ,SpawnO;
};

static ActiveBoatStruct ActiveBot[4] =
{
    {NPC_BJORN_VISUAL,  SAY_SUMMON_BJORN,  404.379f, -335.335f, 104.756f, 413.594f, -335.408f, 107.995f, 3.157f},
    {NPC_HALDOR_VISUAL, SAY_SUMMON_HALDOR, 380.813f, -335.069f, 104.756f, 369.994f, -334.771f, 107.995f, 6.232f},
    {NPC_RANULF_VISUAL, SAY_SUMMON_RANULF, 381.546f, -314.362f, 104.756f, 370.841f, -314.426f, 107.995f, 6.232f},
    {NPC_TORGYN_VISUAL, SAY_SUMMON_TORGYN, 404.310f, -314.761f, 104.756f, 413.992f, -314.703f, 107.995f, 3.157f}
};

/*######
## boss_ymiron
######*/

struct MANGOS_DLL_DECL boss_ymironAI : public ScriptedAI
{
    boss_ymironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();

        srand(time(NULL));
        for (int i = 0; i < 4; ++i)
            m_uiActiveOrder[i] = i;
        for (int i = 0; i < 3; ++i)
        {
            int r = i + (rand()%(4-i));
            int temp = m_uiActiveOrder[i];
            m_uiActiveOrder[i] = m_uiActiveOrder[r];
            m_uiActiveOrder[r] = temp;
        }

        SpellEntry* TempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(SPELL_CHANNEL_SPIRIT_TO_YMIRON);
        if (TempSpell && TempSpell->EffectImplicitTargetB[0] != 16)
        {
               TempSpell->EffectImplicitTargetA[0] = 53;
               TempSpell->EffectImplicitTargetA[1] = 0;
               TempSpell->EffectImplicitTargetB[0] = 16;
               TempSpell->EffectImplicitTargetB[1] = 0;
        }
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsWalking;
    bool m_bIsPause;
    bool m_bIsActiveWithBJORN;
    bool m_bIsActiveWithHALDOR;
    bool m_bIsActiveWithRANULF;
    bool m_bIsActiveWithTORGYN;

    uint8 m_uiActiveOrder[4];
    uint8 m_uiActivedNumber;

    uint32 m_uiFetidRot_Timer;
    uint32 m_uiBane_Timer;
    uint32 m_uiDarkSlash_Timer;
    uint32 m_uiAncestors_Vengeance_Timer;

    uint32 m_uiAbility_BJORN_Timer;
    uint32 m_uiAbility_HALDOR_Timer;
    uint32 m_uiAbility_RANULF_Timer;
    uint32 m_uiAbility_TORGYN_Timer;

    uint32 m_uiPause_Timer;
    uint32 m_uiHealthAmountModifier;
    uint32 m_uiHealthAmountMultipler;

    uint64 m_uiActivedCreatureGUID;
    uint64 m_uiOrbGUID;

    bool m_bHasBane;

    void Reset()
    {
        // exploit check
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_bIsPause = false;
        m_bIsActiveWithBJORN = false;
        m_bIsActiveWithHALDOR = false;
        m_bIsActiveWithRANULF = false;
        m_bIsActiveWithTORGYN = false;

        m_uiFetidRot_Timer            = 8000+rand()%5000;
        m_uiBane_Timer                = 18000+rand()%5000;
        m_uiDarkSlash_Timer           = 28000+rand()%5000;
        m_uiAncestors_Vengeance_Timer = (m_bIsRegularMode ? 45000 : 60000);
        m_uiPause_Timer               = 0;

        m_uiAbility_BJORN_Timer  = 0;
        m_uiAbility_HALDOR_Timer = 0;
        m_uiAbility_RANULF_Timer = 0;
        m_uiAbility_TORGYN_Timer = 0;

        m_uiActivedNumber        = 0;
        m_uiHealthAmountModifier = 1;
        m_uiHealthAmountMultipler = (m_bIsRegularMode ? 25 : 20);

        DespawnBoatGhosts(m_uiActivedCreatureGUID);
        DespawnBoatGhosts(m_uiOrbGUID);

		if(m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, NOT_STARTED);

        m_bHasBane = false;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (m_bIsPause)
            return;

        if (!pWho || pWho == m_creature)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void DespawnBoatGhosts(uint64& m_uiCreatureGUID)
    {
        if (m_uiCreatureGUID)
            if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, m_uiCreatureGUID))
                //pTemp->ForcedDespawn();
                pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        m_uiCreatureGUID = 0;
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
            case 3: DoScriptText(SAY_SLAY_4, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_YMIRON, DONE);

        if(!m_bIsRegularMode && !m_bHasBane)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(ACHIEV_KINGS_BANE);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // exploit check
        if(m_pInstance && m_pInstance->GetData(TYPE_SKADI) == DONE)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        if(m_creature->HasAura(m_bIsRegularMode ? SPELL_BANE_N : SPELL_BANE_H, EFFECT_INDEX_0) && !m_bHasBane)
            m_bHasBane = true;

        if (m_bIsWalking)
        {
            if (m_uiPause_Timer < uiDiff)
            {
                DoScriptText(ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].say, m_creature);
                DoCast(m_creature, SPELL_CHANNEL_YMIRON_TO_SPIRIT); // should be on spirit
                if (Creature* pTemp = m_creature->SummonCreature(ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].npc, ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].SpawnX, ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].SpawnY, ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].SpawnZ, ActiveBot[m_uiActiveOrder[m_uiActivedNumber]].SpawnO, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    m_uiActivedCreatureGUID = pTemp->GetGUID();
                    pTemp->CastSpell(m_creature, SPELL_CHANNEL_SPIRIT_TO_YMIRON, true);
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pTemp->SetSplineFlags(SPLINEFLAG_FLYING);
                    switch(m_uiActiveOrder[m_uiActivedNumber])
                    {
                        case 0: m_bIsActiveWithBJORN  = true; break;
                        case 1: m_bIsActiveWithHALDOR = true; break;
                        case 2: m_bIsActiveWithRANULF = true; break;
                        case 3: m_bIsActiveWithTORGYN = true; break;
                    }
                }

                m_bIsPause = true;
                m_bIsWalking = false;
                m_uiPause_Timer = 3000;
            } else m_uiPause_Timer -= uiDiff;
            return;
        }
        else if (m_bIsPause)
        {
            if (m_uiPause_Timer < uiDiff)
            {
                m_uiAbility_BJORN_Timer = 5000;
                m_uiAbility_HALDOR_Timer = 5000;
                m_uiAbility_RANULF_Timer = 5000;
                m_uiAbility_TORGYN_Timer = 5000;

                m_bIsPause = false;
                m_uiPause_Timer = 0;
            } else m_uiPause_Timer -= uiDiff;
            return;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
        if (!m_bIsPause)
        {
            // Normal spells ------------------------------------------------------------------------
            if (m_uiBane_Timer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_BANE_N : SPELL_BANE_H);
                m_uiBane_Timer = 20000+rand()%5000;
            } else m_uiBane_Timer -= uiDiff;

            if (m_uiFetidRot_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FETID_ROT_N : SPELL_FETID_ROT_H);
                m_uiFetidRot_Timer = 10000+rand()%5000;
            } else m_uiFetidRot_Timer -= uiDiff;

            if (m_uiDarkSlash_Timer < uiDiff)
            {
                //DoCast(m_creature->getVictim(), SPELL_DARK_SLASH); // not working
                int32 dmg = m_creature->getVictim()->GetHealth() / 50; // workaround for damage
                m_creature->CastCustomSpell(m_creature->getVictim(), SPELL_DARK_SLASH, &dmg, 0, 0, false);
                m_uiDarkSlash_Timer = 30000+rand()%5000;
            } else m_uiDarkSlash_Timer -= uiDiff;

            if (m_uiAncestors_Vengeance_Timer < uiDiff)
            {
                DoCast(m_creature, SPELL_ANCESTORS_VENGEANCE);
				m_uiAncestors_Vengeance_Timer = (m_bIsRegularMode ? 45000+rand()%5000 : 60000+rand()%5000);
            } else m_uiAncestors_Vengeance_Timer -= uiDiff;

            // Abilities ------------------------------------------------------------------------------
            if (m_bIsActiveWithBJORN && m_uiAbility_BJORN_Timer < uiDiff)
            {
                //DoCast(m_creature, SPELL_SUMMON_SPIRIT_FOUNT); // works fine, but using summon has better control
                if (Creature* pTemp = m_creature->SummonCreature(NPC_SPIRIT_FOUNT, 385+rand()%10, -330+rand()%10, 104.756f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 180000))
                {
                    pTemp->SetSpeedRate(MOVE_RUN, 0.4f);
                    pTemp->CastSpell(pTemp, m_bIsRegularMode ? SPELL_SPIRIT_FOUNT_N : SPELL_SPIRIT_FOUNT_H, true);
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    pTemp->SetDisplayId(11686);
                    //pTemp->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_uiOrbGUID = pTemp->GetGUID();
                }
                m_bIsActiveWithBJORN = false; // only one orb
            } else m_uiAbility_BJORN_Timer -= uiDiff;

            if (m_bIsActiveWithHALDOR && m_uiAbility_HALDOR_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SPIRIT_STRIKE_N : SPELL_SPIRIT_STRIKE_H);
                m_uiAbility_HALDOR_Timer = 5000; // overtime
            } else m_uiAbility_HALDOR_Timer -= uiDiff;

            if (m_bIsActiveWithRANULF && m_uiAbility_RANULF_Timer < uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_SPIRIT_BURST_N : SPELL_SPIRIT_BURST_H);
                m_uiAbility_RANULF_Timer = 10000; // overtime
            } else m_uiAbility_RANULF_Timer -= uiDiff;

            if (m_bIsActiveWithTORGYN && m_uiAbility_TORGYN_Timer < uiDiff)
            {
                float x,y,z;
                x = m_creature->GetPositionX()-5;
                y = m_creature->GetPositionY()-5;
                z = m_creature->GetPositionZ();
                for(uint8 i = 0; i < 4; ++i)
                {
                    //DoCast(m_creature, SPELL_SUMMON_AVENGING_SPIRIT); // works fine, but using summon has better control
                    if (Creature* pTemp = m_creature->SummonCreature(NPC_AVENGING_SPIRIT, x+rand()%10, y+rand()%10, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        {
                            pTemp->AddThreat(pTarget, 0.0f);
                            pTemp->AI()->AttackStart(pTarget);
                        }
                    }
                }
                m_uiAbility_TORGYN_Timer = 15000; // overtime
            } else m_uiAbility_TORGYN_Timer -= uiDiff;

            // Health check -----------------------------------------------------------------------------
            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < (100-(m_uiHealthAmountMultipler * m_uiHealthAmountModifier)))
            {
            	uint8 m_uiOrder = m_uiHealthAmountModifier - 1;
            	++m_uiHealthAmountModifier;

                m_creature->InterruptNonMeleeSpells(true);
                DoCast(m_creature, SPELL_SCREAMS_OF_THE_DEAD);
                m_creature->GetMotionMaster()->Clear();
                m_creature->StopMoving();
                m_creature->AttackStop();
                //m_creature->GetMotionMaster()->MovePoint(0, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveX, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveY, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveZ);
                m_creature->GetMap()->CreatureRelocation(m_creature, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveX, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveY, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveZ, m_creature->GetOrientation());
                m_creature->SendMonsterMove(ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveX, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveY, ActiveBot[m_uiActiveOrder[m_uiOrder]].MoveZ, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);

                DespawnBoatGhosts(m_uiActivedCreatureGUID);
                DespawnBoatGhosts(m_uiOrbGUID);

                m_bIsActiveWithBJORN  = false;
                m_bIsActiveWithHALDOR = false;
                m_bIsActiveWithRANULF = false;
                m_bIsActiveWithTORGYN = false;

                m_uiBane_Timer                += 8000;
                m_uiFetidRot_Timer            += 8000;
                m_uiDarkSlash_Timer           += 8000;
                m_uiAncestors_Vengeance_Timer += 8000;

                m_uiActivedNumber = m_uiOrder;
                m_bIsWalking = true;
                //m_bIsPause = true;
                m_uiPause_Timer = 2000;
                return;
            }

            DoMeleeAttackIfReady();
            }
    }
};

CreatureAI* GetAI_boss_ymiron(Creature* pCreature)
{
    return new boss_ymironAI(pCreature);
}

void AddSC_boss_ymiron()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ymiron";
    newscript->GetAI = &GetAI_boss_ymiron;
    newscript->RegisterSelf();
}
