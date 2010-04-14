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
SDName: Boss_Koralon
SD%Complete: 
SDComment: 
EndScriptData */

/*** SQL START ***
UPDATE `creature_template` SET `ScriptName`='boss_koralon' WHERE `entry`='35013';
UPDATE `creature_template` SET `ScriptName`='mob_flame_warder' WHERE `entry`='35143';
*** SQL END ***/

#include "precompiled.h"
#include "vault_of_archavon.h"

enum Spells
{
    // Spells Koralon
    SPELL_BURNING_BREATH                        = 66665,
    SPELL_BURNING_BREATH_H                      = 67328,
    SPELL_BURNING_FURY                          = 66721,
    SPELL_FLAME_CINDER_A                        = 66684,
    SPELL_FLAME_CINDER_A_H                      = 67332,
    SPELL_FLAME_CINDER_B                        = 66681, // don't know the real relation to SPELL_FLAME_CINDER_A atm.
    SPELL_METEOR_FISTS_A                        = 66725,
    SPELL_METEOR_FISTS_A_H                      = 66765,
    SPELL_METEOR_FISTS_B                        = 67333,
    SPELL_METEOR_FISTS_B_H                      = 68161,

    // Spells Flame Warder
    SPELL_FW_LAVA_BIRST                         = 66813,
    SPELL_FW_LAVA_BIRST_H                       = 67330,
    SPELL_FW_METEOR_FISTS_A                     = 66808,
    SPELL_FW_METEOR_FISTS_A_H                   = 66809,
    SPELL_FW_METEOR_FISTS_B                     = 67331,
    SPELL_FW_METEOR_FISTS_B_H                   = 68160,
};


struct MANGOS_DLL_DECL boss_koralonAI : public ScriptedAI
{
    boss_koralonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* pInstance;
    bool Regular;
    uint32 BurningBreathTimer;
    uint32 m_uiMeteorFistsATimer;
    uint32 m_uiMeteorFistsBTimer;
    uint32 FlamesTimer;

    uint32 BBTickTimer;
    uint32 BBTicks;
    bool BB;

    void Reset()
    {
        BurningBreathTimer = 15000;
        m_uiMeteorFistsATimer   = 75000;
        m_uiMeteorFistsBTimer   = 80000;
        FlamesTimer = 30000;

        BB = false;

        if(pInstance) 
            pInstance->SetData(TYPE_KORALON, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_BURNING_FURY);

        if(pInstance) 
            pInstance->SetData(TYPE_KORALON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) 
            pInstance->SetData(TYPE_KORALON, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(BurningBreathTimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_BURNING_BREATH : SPELL_BURNING_BREATH_H);
            BurningBreathTimer = 45000;

            BB = true;
            BBTickTimer = 1000;
            BBTicks = 0;
        }
        else BurningBreathTimer -= diff;

        /*if(BB)
        needs to be rewritten
        {
            if(BBTickTimer < diff)
            {
                DoCast(NULL, Regular ? SP_BB_EFFECT : H_SP_BB_EFFECT, true);
                BBTickTimer = 1000;
                ++BBTicks;
                if(BBTicks > 2) BB = false;
            }
            else BBTickTimer -= diff;
        }*/

        if(FlamesTimer < diff)
        {
            int flames = Regular ? 3 : 5;
            int i;
            for(i=0; i< flames; ++i)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target) 
                    DoCast(target, Regular ? SPELL_FLAME_CINDER_A : SPELL_FLAME_CINDER_A_H);
            }
            FlamesTimer = 20000;
        }
        else FlamesTimer -= diff;

        if(m_uiMeteorFistsATimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_METEOR_FISTS_A : SPELL_METEOR_FISTS_A_H);
            m_uiMeteorFistsBTimer = 1500;
            m_uiMeteorFistsATimer = 45000;
        }
        else m_uiMeteorFistsATimer -= diff;

        if(m_uiMeteorFistsBTimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_METEOR_FISTS_B : SPELL_METEOR_FISTS_B_H);
            m_uiMeteorFistsATimer = 45000;
        }
        else m_uiMeteorFistsBTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

/*######
##  Mob Flame Warder
######*/
struct mob_flame_warderAI : public ScriptedAI
{
    mob_flame_warderAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    ScriptedInstance* pInstance;
    bool Regular;

    uint32 m_uiLavaBurstTimer;
    uint32 m_uiMeteorFistsATimer;
    uint32 m_uiMeteorFistsBTimer;

    void Reset()
    {
        m_uiLavaBurstTimer      = 5000;
        m_uiMeteorFistsATimer   = 10000;
        m_uiMeteorFistsBTimer   = 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiLavaBurstTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SPELL_FW_LAVA_BIRST : SPELL_FW_LAVA_BIRST_H);
            m_uiLavaBurstTimer = 15000;
        }
        else m_uiLavaBurstTimer -= diff;

        if(m_uiMeteorFistsATimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_FW_METEOR_FISTS_A : SPELL_FW_METEOR_FISTS_A_H);
            m_uiMeteorFistsBTimer = 1500;
            m_uiMeteorFistsATimer = 45000;
        }
        else m_uiMeteorFistsATimer -= diff;

        if(m_uiMeteorFistsBTimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_FW_METEOR_FISTS_B : SPELL_FW_METEOR_FISTS_B_H);
            m_uiMeteorFistsATimer = 20000;
        }
        else m_uiMeteorFistsBTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_koralonAI(Creature* pCreature)
{
    return new boss_koralonAI(pCreature);
}

CreatureAI* GetAI_mob_flame_warder(Creature* pCreature)
{
    return new mob_flame_warderAI (pCreature);
}

void AddSC_boss_koralon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_koralon";
    newscript->GetAI = &GetAI_boss_koralonAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_warder";
    newscript->GetAI = &GetAI_mob_flame_warder;
    newscript->RegisterSelf();
}
