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
    SPELL_FLAME_CINDER                          = 66684,
    SPELL_FLAME_CINDER_H                        = 67332,
    SPELL_FLAME_CINDER_DUMMY                    = 66681, // don't know the real relation to SPELL_FLAME_CINDER_A atm.
    SPELL_METEOR_FISTS                          = 66725,
    SPELL_METEOR_FISTS_H                        = 66765,
    SPELL_METEOR_FISTS_DMG                      = 67333,
    SPELL_METEOR_FISTS_DMG_H                    = 68161,

    // Spells Flame Warder
    SPELL_LAVA_BURST                            = 66813,
    SPELL_LAVA_BURST_H                          = 67330,
    SPELL_FW_METEOR_FISTS                       = 66808,
    SPELL_FW_METEOR_FISTS_H                     = 66809,
    SPELL_FW_METEOR_FISTS_DMG                   = 66809,
    SPELL_FW_METEOR_FISTS_DMG_H                 = 67331,
};


struct MANGOS_DLL_DECL boss_koralonAI : public ScriptedAI
{
    boss_koralonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBurningBreathTimer;
    uint32 m_uiMeteorFistsTimer;
    uint32 m_uiMeteorFistsDmgTimer;
    uint32 m_uiMeteorFistsEndTimer;
    bool m_bHasFists;
    uint32 m_uiFlameCinderTimer;
    uint8 m_uiMaxFlames;
    uint32 m_uiBurningFuryTimer;

    void Reset()
    {
        m_uiBurningBreathTimer  = 15000;
        m_uiMeteorFistsTimer    = 75000;
        m_uiMeteorFistsDmgTimer = 80000;
        m_bHasFists             = false;
        m_uiFlameCinderTimer    = 30000;
        m_uiMaxFlames           = m_bIsRegularMode ? 2 : 5;
        m_uiBurningFuryTimer    = 20000;
    }

    void JustReachedHome()
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_KORALON, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_KORALON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_KORALON, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiBurningFuryTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BURNING_FURY);
            m_uiBurningFuryTimer = 20000;
        }
        else 
            m_uiBurningFuryTimer -= uiDiff;

        if(m_uiBurningBreathTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_BURNING_BREATH : SPELL_BURNING_BREATH_H);
            m_uiBurningBreathTimer = 45000;
        }
        else m_uiBurningBreathTimer -= uiDiff;

        if(m_uiFlameCinderTimer < uiDiff)
        {
            for(int i=0; i < m_uiMaxFlames; ++i)
            {
                if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_FLAME_CINDER : SPELL_FLAME_CINDER_H);
            }
            m_uiFlameCinderTimer = 20000;
        }
        else m_uiFlameCinderTimer -= uiDiff;

        if(m_uiMeteorFistsTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_METEOR_FISTS : SPELL_METEOR_FISTS_H);
            m_uiMeteorFistsDmgTimer = urand(1000, 2000);
            m_bHasFists             = true;
            m_uiMeteorFistsEndTimer = 15000;
            m_uiMeteorFistsTimer    = 45000;
        }
        else m_uiMeteorFistsTimer -= uiDiff;

        if(m_uiMeteorFistsDmgTimer < uiDiff && m_bHasFists)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_METEOR_FISTS_DMG : SPELL_METEOR_FISTS_DMG_H);
            m_uiMeteorFistsDmgTimer = urand(1000, 2000);
        }
        else m_uiMeteorFistsDmgTimer -= uiDiff;

        if(m_uiMeteorFistsEndTimer < uiDiff && m_bHasFists)
            m_bHasFists = false;
        else m_uiMeteorFistsEndTimer -= uiDiff;

        if (m_creature->GetDistance2d(-218.58f, 103.73f) > 80.0f)
            EnterEvadeMode();

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
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    uint32 m_uiLavaBurstTimer;
    uint32 m_uiMeteorFistsTimer;
    uint32 m_uiMeteorDmgTimer;
    uint32 m_uiMeteorFistsEndTimer;
    bool m_bHasFists;

    void Reset()
    {
        m_uiLavaBurstTimer      = 5000;
        m_uiMeteorFistsTimer    = 10000;
        m_uiMeteorDmgTimer      = 17000;
        m_bHasFists             = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiLavaBurstTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_LAVA_BURST : SPELL_LAVA_BURST_H);
            m_uiLavaBurstTimer = urand(7000, 13000);
        }
        else m_uiLavaBurstTimer -= uiDiff;

        if(m_uiMeteorFistsTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FW_METEOR_FISTS : SPELL_FW_METEOR_FISTS_H);
            m_bHasFists = true;
            m_uiMeteorDmgTimer = 1000;
            m_uiMeteorFistsEndTimer = 15000;
            m_uiMeteorFistsTimer = urand(20000, 25000);
        }
        else m_uiMeteorFistsTimer -= uiDiff;

        if(m_uiMeteorDmgTimer < uiDiff && m_bHasFists)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FW_METEOR_FISTS_DMG : SPELL_FW_METEOR_FISTS_DMG_H);
            m_uiMeteorDmgTimer = urand(1000, 2000);
        }
        else m_uiMeteorDmgTimer -= uiDiff;

        if(m_uiMeteorFistsEndTimer < uiDiff && m_bHasFists)
            m_bHasFists = false;
        else m_uiMeteorFistsEndTimer -= uiDiff;

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
