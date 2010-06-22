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
SDName: boss_mimiron
SD%Complete: 75%
SDComment: Put the robot's pieces together; Implement fires in hard mode
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO           = -1603241,
    SAY_HARD_MODE       = -1603242,
    SAY_BERSERK         = -1603243,
    SAY_TANK_ACTIVE     = -1603244,
    SAY_TANK_SLAY1      = -1603245,
    SAY_TANK_SLAY2      = -1603246,
    SAY_TANK_DEATH      = -1603247,
    SAY_TORSO_ACTIVE    = -1603248,
    SAY_TORSO_SLAY1     = -1603249,
    SAY_TORSO_SLAY2     = -1603250,
    SAY_TORSO_DEATH     = -1603251,
    SAY_HEAD_ACTIVE     = -1603252,
    SAY_HEAD_SLAY1      = -1603253,
    SAY_HEAD_SLAY2      = -1603254,
    SAY_HEAD_DEATH      = -1603255,
    SAY_ROBOT_ACTIVE    = -1603256,
    SAY_ROBOT_SLAY1     = -1603257,
    SAY_ROBOT_SLAY2     = -1603258,
    SAY_ROBOT_DEATH     = -1603259,

    //spells
    //leviathan
    SPELL_PROXIMITY_MINES   = 63016, // also in phase 4
    MOB_PROXIMITY_MINE      = 34362,
    SPELL_EXPLOSION         = 66351,
    SPELL_EXPLOSION_H       = 63009,
    SPELL_NAPALM_SHELL      = 63666,
    SPELL_NAPALM_SHELL_H    = 65026,
    SPELL_PLASMA_BLAST      = 62997,
    SPELL_PLASMA_BLAST_H    = 64529,
    SPELL_SHOCK_BLAST       = 63631, // also in phase 4
    LEVIATHAN_TURRET        = 34071,

    //vx001
    SPELL_RAPID_BURST       = 63387,
    SPELL_RAPID_BURST_H     = 64531,
    SPELL_LASER_BARRAGE     = 63293, // also in phase 4
    SPELL_ROCKET_STRIKE     = 63041, // also in phase 4
    SPELL_HEAT_WAVE         = 63677,
    SPELL_HEAT_WAVE_H       = 64533,
    SPELL_HAND_PULSE        = 64348, // only in phase 4
    SPELL_HAND_PULSE_H      = 64536,

    //aerial unit
    SPELL_PLASMA_BALL       = 63689, // also in phase 4
    SPELL_PLASMA_BALL_H     = 64535, // also in phase 4
    MOB_ASSALT_BOT          = 34057,
    MOB_BOMB_BOT            = 33836,
    MOB_BOMB_BOT_321        = 33346,
    MOB_BOMB_BOT_500        = 34192,
    MOB_JUNK_BOT            = 33855,
    SPELL_MAGNETIC_CORE     = 64444,
    MOB_MAGNETIC_CORE       = 34068,
    ITEM_MAGNETIC_CORE      = 46029,

    //hard mode
    // summons fires
    SPELL_EMERGENCY_MODE    = 64582,
    SPELL_FROST_BOMB        = 64627,
    MOB_FROST_BOMB          = 34149,
    MOB_EMERGENCY_FIRE_BOT  = 34147,
    SPELL_DEAFENING_SIREN   = 64616,
    SPELL_WATER_SPRAY       = 64619,

    SPELL_BERSERK           = 26662,

    ACHIEV_FIREFIGHTER      = 3180,
    ACHIEV_FIREFIGHTER_H    = 3189,
};

#define CENTER_X            2744.732f
#define CENTER_Y            2569.479f
#define CENTER_Z            364.312f 

const float PosTankHome[2]= {2794.86f, 2597.83f};
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {2753.665f, 2584.712f},
    {2754.150f, 2554.445f},
    {2726.966f, 2569.032f},
    {2759.085f, 2594.249f},
    {2759.977f, 2544.345f},
    {2715.542f, 2569.160f},
    {2765.070f, 2604.337f},
    {2765.676f, 2534.558f},
    {2703.810f, 2569.132f},
};

struct MANGOS_DLL_DECL boss_leviathan_mkAI : public ScriptedAI
{   
    boss_leviathan_mkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->setFaction(35);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    bool isPhase4;

    uint32 minesTimer;
    uint32 napalmTimer;
    uint32 plasmaBlastTimer;
    uint32 shockBlastTimer;

    // intro & outro
    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;

    bool isEventFinished;

    // dist for mines
    double distX;
    double distY;

    void Reset()
    {
        isPhase4 = false;

        minesTimer      = 10000;
        napalmTimer     = 20000;
        plasmaBlastTimer= 10000;
        shockBlastTimer = 30000;

        isEventFinished = false;

        OutroTimer          = 10000;
        Step                = 1;
        isOutro             = false;
        m_creature->setFaction(35);

        distX = 10;
        distY = 0;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, IN_PROGRESS);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void DoOutro()
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, DONE);
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 1.0f)
        {
            if(!isPhase4)
            {
                uiDamage = 0;
                isOutro = true;
            }
            else
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_LEVIATHAN_MK, SPECIAL);

                isEventFinished = true;
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
        {
            if(!isPhase4)
            {
                if(irand(0,1))
                    DoScriptText(SAY_TANK_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_TANK_SLAY2, pMimiron);
            }
            else
            {
                if(!isEventFinished)
                {
                    if(irand(0,1))
                        DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                    else
                        DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
                }
            }
        }
    }

    void SetPhase()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        isPhase4 = true;
        isOutro = false;
        minesTimer      = 10000;
        shockBlastTimer = 30000;
        // look like a robot
        SetCombatMovement(false);
    }

    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_LEVIATHAN_MK, IN_PROGRESS);

        isEventFinished = false;
        SetPhase();
    }

    void SummonProximityMinesFirst()
    {
        for(uint8 i = 0; i < 4; i++)
        {
            m_creature->SummonCreature(MOB_PROXIMITY_MINE, m_creature->GetPositionX() + distX, m_creature->GetPositionY() + distY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            distX -= 2.5;
            distY += 2.5;
        }
    }

    void SummonProximityMinesSecond()
    {
        for(uint8 i = 0; i < 4; i++)
        {
            m_creature->SummonCreature(MOB_PROXIMITY_MINE, m_creature->GetPositionX() - distX, m_creature->GetPositionY() + distY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            distX -= 2.5;
            distY += 2.5;
        }
    }

    void SummonProximityMinesThird()
    {
        for(uint8 i = 0; i < 4; i++)
        {
            m_creature->SummonCreature(MOB_PROXIMITY_MINE, m_creature->GetPositionX() + distX, m_creature->GetPositionY() - distY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            distX -= 2.5;
            distY += 2.5;
        }
    }

    void SummonProximityMinesForth()
    {
        for(uint8 i = 0; i < 4; i++)
        {
            m_creature->SummonCreature(MOB_PROXIMITY_MINE, m_creature->GetPositionX() - distX, m_creature->GetPositionY() - distY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            distX -= 2.5;
            distY += 2.5;
        }
    }


    void UpdateAI(const uint32 diff)
    {
        if(!isOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            if(minesTimer < diff && !isEventFinished)
            {
                //DoCast(m_creature, SPELL_PROXIMITY_MINES);
                distX = 10; distY = 0;
                SummonProximityMinesFirst();
                distX = 10; distY = 0;
                SummonProximityMinesSecond();
                distX = 10; distY = 0;
                SummonProximityMinesThird();
                distX = 10; distY = 0;
                SummonProximityMinesForth();
                minesTimer = 30000;
            }
            else minesTimer -= diff;

            if(plasmaBlastTimer < diff && !isPhase4)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BLAST : SPELL_PLASMA_BLAST_H);
                plasmaBlastTimer = 35000;
            }
            else plasmaBlastTimer -= diff;

            if(napalmTimer < diff && !isPhase4)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if(!m_creature->IsWithinDistInMap(pTarget, 15))
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_NAPALM_SHELL : SPELL_NAPALM_SHELL_H);
                napalmTimer = urand (5000, 10000);
            }
            else napalmTimer -= diff;

            if(shockBlastTimer < diff  && !isEventFinished)
            {
                DoCast(m_creature, SPELL_SHOCK_BLAST);
                shockBlastTimer = 40000;
            }
            else shockBlastTimer -= diff;

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
                m_creature->GetMotionMaster()->MovePoint(0, PosTankHome[0], PosTankHome[1], CENTER_Z); 
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
                    DoScriptText(SAY_TANK_DEATH, pMimiron);
                ++Step;
                OutroTimer = 12000;
                break;
            case 3:
                DoOutro();
                ++Step;
                OutroTimer = 3000;
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

struct MANGOS_DLL_DECL boss_vx001AI : public ScriptedAI
{   
    boss_vx001AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 rapidBurstTimer;
    uint32 laserBarrageTimer;
    uint32 rocketStrikeTimer;
    uint32 heatWaveTimer;
    uint32 handPulseTimer;

    bool isPhase4;
    bool isEventFinished;

    void Reset()
    {
        isPhase4 = false;
        isEventFinished = false;

        rapidBurstTimer     = 1000;
        laserBarrageTimer   = 60000;
        rocketStrikeTimer   = 25000;
        heatWaveTimer       = 20000;
        handPulseTimer      = 1000;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VX001, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VX001, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
        {
            if(!isPhase4)
            {
                if(irand(0,1))
                    DoScriptText(SAY_TORSO_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_TORSO_SLAY2, pMimiron);
            }
            else
            {
                if(!isEventFinished)
                {
                    if(irand(0,1))
                        DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                    else
                        DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
                }
            }
        }
    }

    void SetPhase()
    {
        //SetCombatMovement(true);
        isPhase4 = true;
        laserBarrageTimer   = 60000;
        rocketStrikeTimer   = 25000;
        handPulseTimer      = 1000;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VX001, IN_PROGRESS);

        // look like a robot
        m_creature->GetMotionMaster()->MoveIdle();
        SetCombatMovement(false);
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  3, 0.0f);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  3, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void JustDied(Unit* pKiller)
    {
        if(!isEventFinished)
        {
            if(m_pInstance) 
                m_pInstance->SetData(TYPE_VX001, DONE);

            if(!isPhase4)
            {
                if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
                    DoScriptText(SAY_TORSO_DEATH, pMimiron);
            }
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(isPhase4 && !isEventFinished)
        {
            if(uiDamage > m_creature->GetHealth())
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);
                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_VX001, SPECIAL);

                isEventFinished = true;
            }
        }
    }

    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VX001, IN_PROGRESS);
        isEventFinished = false;
        SetPhase();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(rapidBurstTimer < diff && !isPhase4)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_RAPID_BURST : SPELL_RAPID_BURST_H);
            rapidBurstTimer = 1000;
        }
        else rapidBurstTimer -= diff;

        if(laserBarrageTimer < diff && !isEventFinished)
        {
            DoCast(m_creature, SPELL_LASER_BARRAGE);
            laserBarrageTimer = urand(50000, 60000);
        }
        else laserBarrageTimer -= diff;

        if(rocketStrikeTimer < diff && !isEventFinished)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_ROCKET_STRIKE);
            rocketStrikeTimer = urand(25000, 30000);
        }
        else rocketStrikeTimer -= diff;

        if(heatWaveTimer < diff  && !isPhase4)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_HEAT_WAVE : SPELL_HEAT_WAVE_H);
            heatWaveTimer = 10000;
        }
        else heatWaveTimer -= diff;

        if(handPulseTimer < diff && isPhase4 && !isEventFinished)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_HAND_PULSE : SPELL_HAND_PULSE_H);
            handPulseTimer = 1000;
        }
        else handPulseTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_aerial_command_unitAI : public ScriptedAI
{   
    boss_aerial_command_unitAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    uint32 plasmaBallTimer;
    uint32 summonWavesTimer;
    uint32 groundTimer;
    uint32 timeGround;
    bool isGrounded;
    uint8 i;

    bool isPhase4;
    bool isEventFinished;
    uint32 checkEventTimer;

    bool mustDie;
    uint32 dieTimer;

    void Reset()
    {
        plasmaBallTimer = 3000;
        summonWavesTimer = 15000;
        timeGround = 40000;
        isGrounded = false;

        isPhase4 = false;
        isEventFinished = false;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_AERIAL_UNIT, NOT_STARTED);

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_AERIAL_UNIT, IN_PROGRESS);

        if(!isPhase4)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
            m_creature->GetMotionMaster()->MoveIdle();
            SetCombatMovement(false);
            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, 0.0f);
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(isPhase4 && !isEventFinished)
        {
            if(uiDamage > m_creature->GetHealth())
            {
                uiDamage = 0;
                m_creature->SetHealth(0);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->RemoveAllAuras();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovementExpired(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->SetStandState(UNIT_STAND_STATE_DEAD);

                if(m_pInstance) 
                    m_pInstance->SetData(TYPE_AERIAL_UNIT, SPECIAL);
                isEventFinished = true;
            }
        }
    }

    void Repair()
    {
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->SetHealth(m_creature->GetMaxHealth() * 0.5);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_creature->AI()->AttackStart(m_creature->getVictim());
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_AERIAL_UNIT, IN_PROGRESS);
        isEventFinished = false;
        SetPhase();
    }

    void JustDied(Unit *killer)
    {
        if(!isEventFinished)
        {
            if(!isPhase4)
            {
                if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
                    DoScriptText(SAY_HEAD_DEATH, pMimiron);
            }

            if(m_pInstance) 
                m_pInstance->SetData(TYPE_AERIAL_UNIT, DONE);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_MIMIRON))))
        {
            if(!isPhase4)
            {
                if(irand(0,1))
                    DoScriptText(SAY_HEAD_SLAY1, pMimiron);
                else
                    DoScriptText(SAY_HEAD_SLAY2, pMimiron);
            }
            else
            {
                if(!isEventFinished)
                {
                    if(irand(0,1))
                        DoScriptText(SAY_ROBOT_SLAY1, pMimiron);
                    else
                        DoScriptText(SAY_ROBOT_SLAY2, pMimiron);
                }
            }
        }
    }

    void SetPhase()
    {
        isPhase4 = true;
        plasmaBallTimer = 3000;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_AERIAL_UNIT, IN_PROGRESS);

        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        SetCombatMovement(false);
        m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  6.5f, 0.0f);
        m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 6.5f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(plasmaBallTimer < diff && !isEventFinished)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_PLASMA_BALL : SPELL_PLASMA_BALL_H);
            plasmaBallTimer = 3000;
        }
        else plasmaBallTimer -= diff;

        if(timeGround < diff && !isPhase4)
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), CENTER_Z, 0);
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), CENTER_Z, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            isGrounded = true;
            // make boss land
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
            timeGround = 50000;
            groundTimer = 10000;
        }
        else timeGround -= diff;

        if (groundTimer < diff && isGrounded)
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, 0.0f);
            m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() +  7, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
            isGrounded = false;
            // make boss fly
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        }else groundTimer -= diff;

        // spawn adds in arena
        if(summonWavesTimer < diff && !isPhase4)
        {
            // 1 junk bot
            // 1 bomb bot
            // 1 assalt bot
            switch(urand(0, 2))
            {
            case 0:
                i = urand(0, 8);
                if(Creature *pTemp = m_creature->SummonCreature(MOB_BOMB_BOT, m_creature->GetPositionX(), m_creature->GetPositionY(), CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                    }
                }
                break;
            case 1:
                i = urand(0, 8);
                if(Creature *pTemp = m_creature->SummonCreature(MOB_JUNK_BOT, SummonLoc[i].x, SummonLoc[i].y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                    }
                }
                break;
            case 2:
                i = urand(0, 8);
                if(Creature *pTemp = m_creature->SummonCreature(MOB_ASSALT_BOT, SummonLoc[i].x, SummonLoc[i].y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        pTemp->AddThreat(pTarget,0.0f);
                        pTemp->AI()->AttackStart(pTarget);
                    }
                }
                break;
            }
            summonWavesTimer = urand (7000, 10000);
        }
        else summonWavesTimer -= diff;         

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_mimironAI : public ScriptedAI
{   
    boss_mimironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    bool isHardMode;
    uint8 phase;
    uint32 phaseDelayTimer;
    uint32 robotDelayTimer;
    uint32 endCheckTimer;
    uint32 selfRepairTimer;
    uint32 berserkTimer;
    bool hasChecked;
    uint32 outroTimer;
    uint32 phaseCheckTimer;
    uint32 hardModeCheckTimer;
    uint32 hpCheckTimer;
    bool hasMoreHp;

    //hard mode
    uint32 fireTimer;
    uint32 supresionTimer;  // extinguish the fires in phase 1 -> just once
    bool hasSupresed;
    uint32 frostBombTimer;  // 50k dmg in 15y ->extinguish fires in phase 2
    uint32 extinguishTimer; // extinguish fires in 5y in phase 2
    uint32 emergencyBotTimer;   //extinguish fires in phase 3

    uint32 IntroTimer;
    uint32 IntroStep;
    bool isIntro;
    bool isOutro;

    bool isTankDone;
    bool isTorsoDone;
    bool isHeadDone;
    bool isRobotReady;

    uint64 m_uiTankGUID;
    uint64 m_uiTorsoGUID;
    uint64 m_uiHeadGUID;

    bool isTankDead;
    bool isTorsoDead;
    bool isHeadDead;

    bool m_bIsTaunted;

    void Reset()
    {
        phase               = 0;
        isHardMode          = false;
        isIntro             = true;
        phaseDelayTimer     = 10000;
        endCheckTimer       = 1000;
        berserkTimer        = 900000;   // 15 min
        hasChecked          = false;
        isOutro             = false;
        phaseCheckTimer     = 1000;
        hardModeCheckTimer  = 1000;
        hasMoreHp           = false;

        // hard
        fireTimer           = 5000;     //5k dmg /sec
        supresionTimer      = 60000;    // just once in phase 1
        hasSupresed         = false;
        extinguishTimer     = 30000;    //in range of 5y
        frostBombTimer      = 60000;
        emergencyBotTimer   = 10000;    //40 secs after -> 3 robots

        isTankDone          = false;
        isTorsoDone         = false;
        isHeadDone          = false;
        isRobotReady        = false;

        IntroTimer          = 10000;
        IntroStep           = 1;

        isTankDead          = false;
        isTorsoDead         = false;
        isHeadDead          = false;

        m_bIsTaunted        = false;

        m_uiTankGUID        = 0;
        m_uiTorsoGUID       = 0;
        m_uiHeadGUID        = 0;

        phase = 10;
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_MIMIRON, NOT_STARTED);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(GameObject* pButton = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(G0_MIMIRON_BUTTON)))
            pButton->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);

        if(Creature* Torso = GetClosestCreatureWithEntry(m_creature, NPC_VX001, 80.0f))
            m_creature->DealDamage(Torso, Torso->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if(Creature* Head = GetClosestCreatureWithEntry(m_creature, NPC_AERIAL_UNIT, 80.0f))
            m_creature->DealDamage(Head, Head->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if (Creature* Tank = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEVIATHAN_MK))))
             ((boss_leviathan_mkAI*)Tank->AI())->Reset();
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        phase = 0;
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(isHardMode)
            {
                m_pInstance->SetData(TYPE_MIMIRON_HARD, DONE);
                //m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_FIREFIGHTER : ACHIEV_FIREFIGHTER_H);
            }
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_MIMIRON, DONE);
            if(isHardMode)
                m_pInstance->SetData(TYPE_MIMIRON_HARD, DONE);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(phase == 0)
        {
            if(isIntro)
            {
                //hard mode check
                switch(IntroStep)
                {
                case 1:
                    ++IntroStep;
                    IntroTimer = 15000;
                    break;
                case 3:
                    if(isHardMode)
                    {
                        if(GameObject* pButton = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(G0_MIMIRON_BUTTON)))
                            pButton->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1); 

                        berserkTimer = 460000;  // 8 min
                        isHardMode = true;

                        DoScriptText(SAY_HARD_MODE, m_creature);
                        ++IntroStep;
                        IntroTimer = 15000;
                    }
                    else
                    {
                        ++IntroStep;
                        IntroTimer = 1000;
                    }
                    break;
                case 5:
                    if (Creature* pTank = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEVIATHAN_MK))))
                    {
                        if(pTank->isAlive())
                        {
                            DoScriptText(SAY_TANK_ACTIVE, m_creature);
                            pTank->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
                        }
                        else
                        {
                            IntroStep = 1;
                            pTank->Respawn();
                        }
                        m_uiTankGUID = pTank->GetGUID();
                    }
                    else
                        return;
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_MIMIRON, IN_PROGRESS);
                    ++IntroStep;
                    IntroTimer = 15000;
                    break;
                case 7:
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                    {
                        pTank->setFaction(14);
                        if(isHardMode)
                        {
                            pTank->CastSpell(pTank, SPELL_EMERGENCY_MODE, false);
                            ++IntroStep;
                            IntroTimer = 1000;
                        }
                        else
                        {
                            phase = 1;
                            isIntro = false;
                            ++IntroStep;
                            IntroTimer = 9000;
                        }
                    }
                    break;
                case 9:
                    if(isHardMode)
                    {
                        if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                            pTank->SetHealth(m_creature->GetMaxHealth()+ (m_creature->GetMaxHealth() * 0.3));                  
                        phase = 1;
                        isIntro = false;
                        ++IntroStep;
                        IntroTimer = 9000;
                    }
                    break;
                }
            }
            else return;

            if (IntroTimer <= diff)
            {
                ++IntroStep;
                IntroTimer = 330000;
            } IntroTimer -= diff;
        }
        if(phase == 1)
        {
            if (phaseCheckTimer <= diff && !isTankDone)
            {
                if(m_pInstance && m_pInstance->GetData(TYPE_LEVIATHAN_MK) == DONE)
                {
                    isTankDone = true;
                    phaseDelayTimer = 15000;
                }
                phaseCheckTimer = 1000;
            }
            else phaseCheckTimer -= diff;

            if(isTankDone)
            {
                if(phaseDelayTimer < diff && !hasMoreHp)
                {
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_VX001, SPECIAL);
                    DoScriptText(SAY_TORSO_ACTIVE, m_creature);
                    if(Creature* pTorso = m_creature->SummonCreature(NPC_VX001, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                    {
                        if(isHardMode)
                        {
                            pTorso->CastSpell(pTorso, SPELL_EMERGENCY_MODE, false);
                            hasMoreHp = true;
                            hpCheckTimer = 1000;
                        }
                        else
                        {
                            phase = 2;
                            phaseDelayTimer = 10000;
                            phaseCheckTimer = 1000;
                        }
                        m_uiTorsoGUID = pTorso->GetGUID();
                    }
                }
                else phaseDelayTimer -= diff;

                if (hpCheckTimer <= diff && hasMoreHp)
                {
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        pTorso->SetHealth(m_creature->GetMaxHealth()+ (m_creature->GetMaxHealth() * 0.3));
                    phase = 2;
                    hasMoreHp = false;
                    phaseDelayTimer = 10000;
                    phaseCheckTimer = 1000;
                    hpCheckTimer = 10000;
                }
                else hpCheckTimer -= diff;
            }
        }
        if(phase == 2)
        {
            if (phaseCheckTimer <= diff && !isTorsoDone)
            {
                if(m_pInstance && m_pInstance->GetData(TYPE_VX001) == DONE)
                {
                    phaseDelayTimer = 20000;
                    hasMoreHp = false;
                    isTorsoDone = true;
                }
                phaseCheckTimer = 1000;
            }
            else phaseCheckTimer -= diff;

            if(isTorsoDone)
            {
                if(phaseDelayTimer < diff && !hasMoreHp)
                {
                    DoScriptText(SAY_HEAD_ACTIVE, m_creature);
                    if(Creature* pHead = m_creature->SummonCreature(NPC_AERIAL_UNIT, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                    {
                        if(isHardMode)
                        {
                            pHead->CastSpell(pHead, SPELL_EMERGENCY_MODE, false);
                            hasMoreHp = true;
                            hpCheckTimer = 1000;
                        }
                        else
                        {
                            phase = 3;
                            phaseCheckTimer = 1000;
                            phaseDelayTimer = 10000;
                        }
                        m_uiHeadGUID = pHead->GetGUID();
                    }
                }
                else phaseDelayTimer -= diff;

                if (hpCheckTimer <= diff && hasMoreHp)
                {
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        pHead->SetHealth(m_creature->GetMaxHealth()+ (m_creature->GetMaxHealth() * 0.3));
                    phase = 3;
                    hasMoreHp = false;
                    phaseDelayTimer = 10000;
                    phaseCheckTimer = 1000;
                    hpCheckTimer = 10000;
                }
                else hpCheckTimer -= diff;
            }
        }
        if(phase == 3)
        {
            if (phaseCheckTimer <= diff && !isHeadDone)
            {
                if(m_pInstance && m_pInstance->GetData(TYPE_AERIAL_UNIT) == DONE)
                {
                    phaseDelayTimer = 10000;
                    hasMoreHp = false;
                    isHeadDone = true;
                }
                phaseCheckTimer = 1000;
            }
            else phaseCheckTimer -= diff;

            if(isHeadDone)
            {
                if(phaseDelayTimer < diff && !isRobotReady)
                {
                    if (Creature* pTank = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_LEVIATHAN_MK))))
                    {
                        if(pTank->isAlive())
                        {
                            pTank->GetMotionMaster()->MovePoint(0, CENTER_X, CENTER_Y, CENTER_Z);
                            m_uiTankGUID = pTank->GetGUID();
                        }

                        isRobotReady = true;
                        robotDelayTimer = 10000;
                    }
                    else
                        return;
                    phaseDelayTimer = 100000;
                }
                else phaseDelayTimer -= diff;
            }

            if(robotDelayTimer < diff && isRobotReady && !hasMoreHp)
            {
                DoScriptText(SAY_ROBOT_ACTIVE, m_creature);
                if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                {
                    pTank->setFaction(14);
                    ((boss_leviathan_mkAI*)pTank->AI())->SetPhase();
                }

                if(Creature* pTorso = m_creature->SummonCreature(NPC_VX001, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    ((boss_vx001AI*)pTorso->AI())->SetPhase();
                    m_uiTorsoGUID = pTorso->GetGUID();
                }

                if(Creature* pHead = m_creature->SummonCreature(NPC_AERIAL_UNIT, CENTER_X, CENTER_Y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    ((boss_aerial_command_unitAI*)pHead->AI())->SetPhase();
                    m_uiHeadGUID = pHead->GetGUID();
                }                
                
                if(isHardMode)
                {
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        pTorso->CastSpell(pTorso, SPELL_EMERGENCY_MODE, false);
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        pHead->CastSpell(pHead, SPELL_EMERGENCY_MODE, false);
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        pTank->CastSpell(pTank, SPELL_EMERGENCY_MODE, false);
                    hasMoreHp = true;
                    hpCheckTimer = 1000;
                }
                else
                {
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        pTorso->SetHealth(pTorso->GetMaxHealth() * 0.5);
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        pHead->SetHealth(pHead->GetMaxHealth() * 0.5);
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        pTank->SetHealth(pTank->GetMaxHealth() * 0.5);
                    phase = 4;
                    endCheckTimer = 2000;
                    robotDelayTimer = 100000;
                }
            }
            else robotDelayTimer -= diff;

            if (hpCheckTimer <= diff && hasMoreHp)
            {
                if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                    pHead->SetHealth(pHead->GetMaxHealth() * 0.5);
                if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                    pTorso->SetHealth(pTorso->GetMaxHealth()* 0.5);
                if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                    pTank->SetHealth(pTank->GetMaxHealth()* 0.5);   
                phase = 4;
                endCheckTimer = 1000;
                robotDelayTimer = 100000;
                hpCheckTimer = 10000;
            }
            else hpCheckTimer -= diff;
        }
        if(phase == 4)
        {
            if(!isOutro)
            {
                if(endCheckTimer < diff && !hasChecked)
                {
                    if(m_pInstance && m_pInstance->GetData(TYPE_LEVIATHAN_MK) == SPECIAL)
                    {
                        selfRepairTimer = 15000;
                        isTankDead = true;
                        hasChecked = true;
                    }
                    if(m_pInstance && m_pInstance->GetData(TYPE_VX001) == SPECIAL)
                    {
                        selfRepairTimer = 15000;
                        isTorsoDead = true;
                        hasChecked = true;
                    }
                    if(m_pInstance && m_pInstance->GetData(TYPE_AERIAL_UNIT) == SPECIAL)
                    {
                        selfRepairTimer = 15000;
                        isHeadDead = true;
                        hasChecked = true;
                    }
                    endCheckTimer = 1000;
                }
                else endCheckTimer -= diff;

                if(selfRepairTimer < diff && hasChecked)
                {
                    if(m_pInstance && m_pInstance->GetData(TYPE_LEVIATHAN_MK) == SPECIAL)
                        isTankDead = true;
                    if(m_pInstance && m_pInstance->GetData(TYPE_VX001) == SPECIAL)
                        isTorsoDead = true;
                    if(m_pInstance && m_pInstance->GetData(TYPE_AERIAL_UNIT) == SPECIAL)
                        isHeadDead = true;

                    if(isTankDead && isTorsoDead && isHeadDead)
                    {
                        DoScriptText(SAY_ROBOT_DEATH, m_creature);
                        outroTimer = 15000;
                        isOutro = true;
                    }
                    else
                    {
                        if(isTankDead)
                        {
                            if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                                ((boss_leviathan_mkAI*)pTank->AI())->Repair();
                            endCheckTimer = 1000;
                            hasChecked = false;
                        }
                        if(isTorsoDead)
                        {
                            if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                                ((boss_vx001AI*)pTorso->AI())->Repair();
                            endCheckTimer = 1000;
                            hasChecked = false;
                        }
                        if(isHeadDead)
                        {
                            if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                                ((boss_aerial_command_unitAI*)pHead->AI())->Repair();
                            endCheckTimer = 1000;
                            hasChecked = false;
                        }
                    }
                    selfRepairTimer = 1000;
                }
                else selfRepairTimer -= diff;
            }
            else
            {
                if(outroTimer < diff)
                {
                    if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
                        m_creature->DealDamage(pTank, pTank->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
                        m_creature->DealDamage(pHead, pHead->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                        m_creature->DealDamage(pTorso, pTorso->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    DoOutro();
                }
                else outroTimer -= diff;
            }
        }

        // berserk
        if (berserkTimer <= diff && (phase == 1 || phase == 2 || phase == 3 || phase == 4))
        {
            if(Creature* pTank = m_pInstance->instance->GetCreature(m_uiTankGUID))
            {
                if(pTank && pTank->isAlive())
                    pTank->CastSpell(pTank, SPELL_BERSERK, false);
            }

            if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
            {
                if(pTorso && pTorso->isAlive())
                    pTorso->CastSpell(pTorso, SPELL_BERSERK, false);
            }

            if(Creature* pHead = m_pInstance->instance->GetCreature(m_uiHeadGUID))
            {
                if(pHead && pHead->isAlive())
                    pHead->CastSpell(pHead, SPELL_BERSERK, false);
            }

            berserkTimer = 330000;
        } 
        else
            berserkTimer -= diff;

        // hard mode
        if(isHardMode)
        {
            // TODO: implement hard mode
            if (fireTimer <= diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))

                    fireTimer = 5000;
            } 
            else fireTimer -= diff;

            // supres fires in phase 1
            if(phase == 1)
            {
                if (supresionTimer <= diff && !hasSupresed)
                {

                    supresionTimer = 60000;
                    hasSupresed = true;
                } 
                else supresionTimer -= diff;
            }

            // supress fires in phase 2 & 4
            if(phase == 2 || phase == 4)
            {
                if (frostBombTimer <= diff)
                {
                    if(Creature* pTorso = m_pInstance->instance->GetCreature(m_uiTorsoGUID))
                    {
                        //if(Creature *pTemp = GetClosestCreatureWithEntry(Torso, MOB_BOMB_BOT, 10))  // fix id!!!
                        //m_creature->SummonCreature(MOB_FROST_BOMB, pTemp->GetPositionX(), pTemp->GetPositionY(), CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
                    }

                    frostBombTimer = 60000;
                } 
                else frostBombTimer -= diff;
            }

            // supress fires in phase 2
            if(phase == 2)
            {
                if (extinguishTimer <= diff)
                {

                    extinguishTimer = 30000;
                } 
                else extinguishTimer -= diff;
            }

            // supress fires in phase 3
            if(phase == 3)
            {
                if (emergencyBotTimer <= diff)
                {
                    for(uint8 i = 0; i < 3; i++)
                        if(Creature *pTemp = m_creature->SummonCreature(MOB_EMERGENCY_FIRE_BOT, SummonLoc[i].x, SummonLoc[i].y, CENTER_Z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                        {
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                pTemp->AddThreat(pTarget,0.0f);
                                pTemp->AI()->AttackStart(pTarget);
                            }
                        }
                        emergencyBotTimer = 40000;
                } 
                else emergencyBotTimer -= diff;
            }

        }
    }
};

struct MANGOS_DLL_DECL leviathan_turretAI : public ScriptedAI
{   
    leviathan_turretAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetVisibility(VISIBILITY_OFF);
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_proximity_mineAI : public ScriptedAI
{   
    mob_proximity_mineAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *pInstance;
    bool m_bIsRegularMode;

    uint32 explosionTimer;
    uint32 rangeCheckTimer;

    void Reset()
    {
        explosionTimer = 60000;
        rangeCheckTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(explosionTimer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLOSION : SPELL_EXPLOSION_H);
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            explosionTimer = 20000;
        }
        else explosionTimer -= diff;

        if (rangeCheckTimer < diff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 1)
                        explosionTimer = 500;
                }
            } 
            rangeCheckTimer = 100000;
        }
        else rangeCheckTimer -= diff;
    }
};

struct MANGOS_DLL_DECL mob_bomb_botAI : public ScriptedAI
{   
    mob_bomb_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {}

    void JustDied(Unit* pKiller)
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                    i->getSource()->DealDamage(i->getSource(), urand(9425, 10575), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FIRE, NULL, false);
            }
        } 
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_emergency_botAI : public ScriptedAI
{   
    mob_emergency_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool m_bIsRegularMode;

    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                if(!m_bIsRegularMode)
                    DoCast(m_creature->getVictim(), SPELL_DEAFENING_SIREN);
                break;
            case 1:
                DoCast(m_creature, SPELL_WATER_SPRAY);
                break;
            }
            spellTimer = 10000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_frost_bombAI : public ScriptedAI
{   
    mob_frost_bombAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance *pInstance;
    bool m_bIsRegularMode;

    uint32 explosionTimer;

    void Reset()
    {
        explosionTimer = 10000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (explosionTimer < diff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 15)
                        i->getSource()->DealDamage(i->getSource(), 50000, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FROST, NULL, false);
                }
            } 
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            explosionTimer = 100000;
        }
        else explosionTimer -= diff;
    }
};

bool GOHello_go_red_button(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;

    if (Creature* pMimiron = ((Creature*)Unit::GetUnit((*pGo), m_pInstance->GetData64(NPC_MIMIRON))))
    {
        if(pMimiron->isAlive())
            ((boss_mimironAI*)pMimiron->AI())->isHardMode = true;
    }

    return false;
}

CreatureAI* GetAI_boss_mimiron(Creature* pCreature)
{
    return new boss_mimironAI(pCreature);
}

CreatureAI* GetAI_boss_leviathan_mk(Creature* pCreature)
{
    return new boss_leviathan_mkAI(pCreature);
}

CreatureAI* GetAI_boss_vx001(Creature* pCreature)
{
    return new boss_vx001AI(pCreature);
}

CreatureAI* GetAI_boss_aerial_command_unit(Creature* pCreature)
{
    return new boss_aerial_command_unitAI(pCreature);
}

CreatureAI* GetAI_leviathan_turret(Creature* pCreature)
{
    return new leviathan_turretAI(pCreature);
}

CreatureAI* GetAI_mob_proximity_mine(Creature* pCreature)
{
    return new mob_proximity_mineAI(pCreature);
}

CreatureAI* GetAI_mob_bomb_bot(Creature* pCreature)
{
    return new mob_bomb_botAI(pCreature);
}
CreatureAI* GetAI_mob_emergency_bot(Creature* pCreature)
{
    return new mob_emergency_botAI(pCreature);
}

CreatureAI* GetAI_mob_frost_bomb(Creature* pCreature)
{
    return new mob_frost_bombAI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mimiron";
    newscript->GetAI = &GetAI_boss_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk";
    newscript->GetAI = &GetAI_boss_leviathan_mk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "leviathan_turret";
    newscript->GetAI = &GetAI_leviathan_turret;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vx001";
    newscript->GetAI = &GetAI_boss_vx001;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_aerial_command_unit";
    newscript->GetAI = &GetAI_boss_aerial_command_unit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_proximity_mine";
    newscript->GetAI = &GetAI_mob_proximity_mine;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bomb_bot";
    newscript->GetAI = &GetAI_mob_bomb_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_emergency_bot";
    newscript->GetAI = &GetAI_mob_emergency_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_bomb";
    newscript->GetAI = &GetAI_mob_bomb_bot;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_red_button";
    newscript->pGOHello = &GOHello_go_red_button;
    newscript->RegisterSelf();
}
