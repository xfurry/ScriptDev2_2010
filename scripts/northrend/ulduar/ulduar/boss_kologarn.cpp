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
SDName: boss_kologarn
SD%Complete: 85%
SDComment: stone grip
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603150,
    SAY_SHOCKWEAVE      = -1603151,
    SAY_GRAB            = -1603152,
    SAY_LEFT_ARM_LOST   = -1603153,
    SAY_RIGHT_ARM_LOST  = -1603154,
    SAY_SLAY1           = -1603155,
    SAY_SLAY2           = -1603156,
    SAY_BERSERK         = -1603157,
    SAY_DEATH           = -1603158,

    //kologarn
    SPELL_OVERHEAD_SMASH		= 63356,
    SPELL_OVERHEAD_SMASH_H		= 64003,
    SPELL_ONE_ARMED_SMASH		= 63573,
    SPELL_ONE_ARMED_SMASH_H		= 64006,
    SPELL_STONE_SHOUT			= 63716,
    SPELL_STONE_SHOUT_H			= 64005,
    SPELL_PETRIFYING_BREATH		= 62030,
    SPELL_PETRIFYING_BREATH_H	= 63980,
    SPELL_FOCUSED_EYEBEAN       = 63346,
    SPELL_FOCUSED_EYEBEAN_H     = 63976,
    SPELL_FOCUSED_EYEBEAN_VISUAL= 63368,
    SPELL_ENRAGE                = 26662,
    //left arm
    SPELL_SHOCKWAVE				= 63783,
    SPELL_SHOCKWAVE_H			= 63982,
    //right arm
    SPELL_STONE_GRIP			= 62166,
    SPELL_STONE_GRIP_H			= 63981,
    //both
    SPELL_ARM_VISUAL			= 64753,
    //rubble
    SPELL_RUMBLE				= 63818,
    SPELL_STONE_NOVA			= 63978,
    //NPC ids
    MOB_RUBBLE					= 33768, 

    ACHIEV_RUBBLE_AND_ROLL      = 2959,
    ACHIEV_RUBBLE_AND_ROLL_H    = 2960,
    ACHIEV_WITH_OPEN_ARMS       = 2951,
    ACHIEV_WITH_OPEN_ARMS_H     = 2952,
    ACHIEV_DISARMED             = 2953,
    ACHIEV_DISARMED_H           = 2954,
};

float LeftArm[3] = {1767.636f, -47.970f, 448.805f};
float RightArm[3] = {1769.671f, 1.514f, 448.810f};
const float KoloFront[3] = {1771.683f, -24.230f, 448.806f};

bool m_bIsLeftDead;
bool m_bIsRightDead;

// Rubble
struct MANGOS_DLL_DECL mob_ulduar_rubbleAI : public ScriptedAI
{
    mob_ulduar_rubbleAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    uint32 Stone_Nova_Timer;
    uint32 Death_Timer;

    bool die;

    void Reset()
    {
        die = false;
        Stone_Nova_Timer = urand(8000, 12000);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (!die)
            {
                uiDamage = 0;
                DoCast(m_creature, SPELL_RUMBLE);
                Death_Timer = 500;
                die = true;
            }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Death_Timer < diff && die)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else Death_Timer -= diff;

        if (Stone_Nova_Timer < diff && !die)
        {
            DoCast(m_creature, SPELL_STONE_NOVA);
            Stone_Nova_Timer = urand(7000, 9000);
        }else Stone_Nova_Timer -= diff;

        if (!die)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ulduar_rubble(Creature* pCreature)
{
    return new mob_ulduar_rubbleAI(pCreature);
}

// Left Arm
struct MANGOS_DLL_DECL boss_left_armAI : public ScriptedAI
{
    boss_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 Shockwave_Timer;

    void Reset()
    {
        Shockwave_Timer = 30000;
        DoCast(m_creature, SPELL_ARM_VISUAL);

        m_bIsLeftDead = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_KOLOGARN))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_LEFT_ARM_LOST, m_creature);
        if (!m_pInstance)
            return;
        if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_KOLOGARN))))
        {
            if (pTemp->isAlive())
                pTemp->DealDamage(pTemp, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        m_bIsLeftDead = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Shockwave_Timer < diff)
        {
            DoScriptText(SAY_SHOCKWEAVE, m_creature);
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHOCKWAVE : SPELL_SHOCKWAVE_H);
            Shockwave_Timer = 30000;
        }else Shockwave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_left_arm(Creature* pCreature)
{
    return new boss_left_armAI(pCreature);
}

// Right Arm
struct MANGOS_DLL_DECL boss_right_armAI : public ScriptedAI
{
    boss_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 Stone_Grip_Timer;
    uint32 gripdmg;
    uint32 freedmg;
    uint64 GripTarget;

    bool grip;

    void Reset()
    {
        Stone_Grip_Timer = 20000;
        GripTarget = 0;
        gripdmg = 0;
        freedmg = 0;
        grip = false;
        DoCast(m_creature, SPELL_ARM_VISUAL);

        m_bIsRightDead = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_KOLOGARN))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (grip)
        {
            gripdmg += uiDamage;
            freedmg = m_bIsRegularMode ? 100000 : 480000;
            if (gripdmg > freedmg || uiDamage > m_creature->GetHealth())
            {
                if (Unit* pGripTarget = Unit::GetUnit(*m_creature, GripTarget)){
                    if (pGripTarget->HasAura(SPELL_STONE_GRIP))
                        pGripTarget->RemoveAurasDueToSpell(SPELL_STONE_GRIP);
                    if (pGripTarget->HasAura(SPELL_STONE_GRIP_H))
                        pGripTarget->RemoveAurasDueToSpell(SPELL_STONE_GRIP_H);
                }
                grip = false;
                gripdmg = 0;
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_RIGHT_ARM_LOST, m_creature);
        if (!m_pInstance)
            return;
        if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_KOLOGARN))))
        {
            if (pTemp->isAlive())
                pTemp->DealDamage(pTemp, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }

        m_bIsRightDead = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Stone_Grip_Timer < diff)
        {
            DoScriptText(SAY_GRAB, m_creature);
            //stone grip emote
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)){
                //DoCast(target, m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
                GripTarget = target->GetGUID();
                grip = true;
                gripdmg = 0;
            }
            Stone_Grip_Timer = 30000;
        }else Stone_Grip_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_right_arm(Creature* pCreature)
{
    return new boss_right_armAI(pCreature);
}

// Kologarn
struct MANGOS_DLL_DECL boss_kologarnAI : public ScriptedAI
{
    boss_kologarnAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Spell_Timer;
    uint32 Check_Timer;
    uint32 Eyebeah_Timer;
    uint32 respawnright;
    uint32 respawnleft;
    uint32 EnrageTimer;

    uint32 m_uiRubbleNo;
    bool m_bHasLeftDied;
    bool m_bHasRightDied;
    bool m_bDisarmedCheck;
    uint32 m_uiDisarmedTimer;

    bool right;
    bool left;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        Spell_Timer = 10000;
        Check_Timer = 6300;
        EnrageTimer = 600000;
        Eyebeah_Timer = 10000 + urand(1000, 5000);
        right = true;
        left = true;

        m_uiRubbleNo = 0;
        m_bHasLeftDied  = false;
        m_bHasRightDied = false;
        m_bDisarmedCheck = false;
        m_uiDisarmedTimer = 0;
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        // Rubble and roll
        if (m_uiRubbleNo >= 25)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_RUBBLE_AND_ROLL : ACHIEV_RUBBLE_AND_ROLL_H);
        }

        // With open arms
        if (!m_bHasRightDied && !m_bHasLeftDied)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_WITH_OPEN_ARMS : ACHIEV_WITH_OPEN_ARMS_H);
        }

        // Disarmed
        if (m_bIsLeftDead && m_bIsRightDead && m_uiDisarmedTimer <= 12000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_DISARMED : ACHIEV_DISARMED_H);
        }

        //death yell
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, DONE);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
                if (pTemp->isAlive())
                    pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
                if (pTemp->isAlive())
                    pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
        }
        //aggro yell
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_KOLOGARN, FAIL);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
                if (!pTemp->isAlive())
                    pTemp->Respawn();
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
                if (!pTemp->isAlive())
                    pTemp->Respawn();
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Spell_Timer < diff)
        {
            if (right && left)
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OVERHEAD_SMASH : SPELL_OVERHEAD_SMASH_H);
            else
                if (!right && !left)
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STONE_SHOUT : SPELL_STONE_SHOUT_H);
                else
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ONE_ARMED_SMASH : SPELL_ONE_ARMED_SMASH_H);
            Spell_Timer = 20000;
        }else Spell_Timer -= diff;   

        /* to be fixed!
        if (Eyebeah_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, SPELL_FOCUSED_EYEBEAN_VISUAL);
                DoCast(target, m_bIsRegularMode ? SPELL_FOCUSED_EYEBEAN : SPELL_FOCUSED_EYEBEAN_H, true);
            }
            Eyebeah_Timer = 20000;
        }else Eyebeah_Timer -= diff;
        */

        if (respawnleft < diff && !left)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
                if (!pTemp->isAlive())
                {
                    pTemp->Respawn();
                    left = true;
                    m_bDisarmedCheck = false;
                    m_bIsLeftDead = false;
                }
        }else respawnleft -= diff;  

        if (respawnright < diff && !right)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
                if (!pTemp->isAlive())
                {
                    pTemp->Respawn();
                    right = true;
                    m_bDisarmedCheck = false;
                    m_bIsRightDead = false;
                }
        }else respawnright -= diff; 

        if (Check_Timer < diff)
        {
            if (Creature* lArm = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEFT_ARM))))
            {
                if (!lArm->isAlive() && left)
                {
                    m_bHasLeftDied = true;
                    m_bDisarmedCheck = true;
                    m_uiDisarmedTimer = 0;

                    for(uint8 i = 0; i < 5; i ++)
                    {
                        if(Creature* pRubble = m_creature->SummonCreature(MOB_RUBBLE, LeftArm[0] + urand(5, 15), LeftArm[1] + urand(0, 10), LeftArm[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                        {
                            pRubble->GetMotionMaster()->MovePoint(0, KoloFront[0], KoloFront[1], KoloFront[2]);

                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pRubble->AddThreat(pTarget,0.0f);
                                pRubble->AI()->AttackStart(pTarget);
                                pRubble->SetInCombatWithZone();
                            }

                            m_uiRubbleNo++;
                        }
                    }
                    left = false;
                    respawnleft = 50000;
                }
            }
            if (Creature* rArm = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_RIGHT_ARM))))
            {
                if (!rArm->isAlive() && right)
                {
                    m_bHasRightDied = true;
                    m_bDisarmedCheck = true;
                    m_uiDisarmedTimer = 0;

                    for(uint8 i = 0; i < 5; i ++)
                    {
                        if(Creature* pRubble = m_creature->SummonCreature(MOB_RUBBLE, RightArm[0] + urand(5, 15), RightArm[1] + urand(0, 10), RightArm[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                        {
                            pRubble->GetMotionMaster()->MovePoint(0, KoloFront[0], KoloFront[1], KoloFront[2]);

                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pRubble->AddThreat(pTarget,0.0f);
                                pRubble->AI()->AttackStart(pTarget);
                                pRubble->SetInCombatWithZone();
                            }

                            m_uiRubbleNo++;
                        }
                    }
                    right = false;
                    respawnright = 50000;
                }
            }

            //Petrifying breath
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), 10))
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PETRIFYING_BREATH : SPELL_PETRIFYING_BREATH_H);

            Check_Timer = 500;
        }else Check_Timer -= diff;

        // disarmed achiev check
        if (m_bIsRightDead || m_bIsLeftDead)
            m_uiDisarmedTimer += diff;

        if(EnrageTimer < diff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_ENRAGE);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kologarn";
    NewScript->GetAI = GetAI_boss_kologarn;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_ulduar_rubble";
    NewScript->GetAI = &GetAI_mob_ulduar_rubble;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_left_arm";
    NewScript->GetAI = &GetAI_boss_left_arm;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_right_arm";
    NewScript->GetAI = &GetAI_boss_right_arm;
    NewScript->RegisterSelf();
}