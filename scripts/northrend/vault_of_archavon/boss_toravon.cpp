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
SDName: Boss_Toravon
SD%Complete: 
SDComment: Timers should be adjusted
EndScriptData */

/*** SQL START ***
UPDATE `creature_template` SET `ScriptName`='boss_toravon' WHERE `entry`='38433';
UPDATE `creature_template` SET `ScriptName`='mob_frost_warder' WHERE `entry`='38482';
UPDATE `creature_template` SET `ScriptName`='mob_frozen_orb_stalker' WHERE `entry`='38461';
UPDATE `creature_template` SET `ScriptName`='mob_frozen_orb' WHERE `entry`='38456';
*** SQL END ***/

#include "precompiled.h"
#include "vault_of_archavon.h"

enum Spells
{
    // Spells Toravon
    SPELL_FREEZING_GROUND       = 72090,  
    SPELL_FREEZING_GROUND_H     = 72104,
    SPELL_FROZEN_ORB            = 72091,
    SPELL_FROZEN_ORB_H          = 72095,
    SPELL_WHITEOUT              = 72034,  // Every 38 sec. cast. (after SPELL_FROZEN_ORB)
    SPELL_WHITEOUT_H            = 72096,
    SPELL_WHITEOUT_VISUAL       = 72036,
    SPELL_FROZEN_MALLET         = 71993,

    // Spells Frost Warder
    SPELL_FROST_BLAST           = 72123,
    SPELL_FROST_BLAST_H         = 72124,
    SPELL_FROZEN_MALLET_FW      = 72122,

    // Spell Frozen Orb
    SPELL_FROZEN_ORB_DMG        = 72081,   // priodic dmg aura
    SPELL_FROZEN_ORB_VISUAL     = 72067,   // make visible

    // Spell Frozen Orb Stalker
    SPELL_ORB_SUMMON_TRIG       = 72094,
    SPELL_ORB_SUMMON            = 72093,   // summon orb

    // Mob Frozen Orb
    MOB_FROZEN_ORB_STALKER      = 38461,    // 1 in 10 mode and 3 in 25 mode
};

struct MANGOS_DLL_DECL boss_toravonAI : public ScriptedAI
{
    boss_toravonAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    
    uint32 m_uiFrozenOrbTimer;
    uint32 m_uiWhiteOutTimer;
    uint8 m_uiMaxOrbs;
    uint32 m_uiFreezingGroundTimer;

    void Reset()
    {
        m_uiFrozenOrbTimer      = 11000;
        m_uiWhiteOutTimer       = 13000;
        m_uiFreezingGroundTimer = 15000; 
        m_uiMaxOrbs             = m_bIsRegularMode ? 1 : 3;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TORAVON, NOT_STARTED);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TORAVON, DONE);
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_FROZEN_MALLET);

        if (m_pInstance) 
            m_pInstance->SetData(TYPE_TORAVON, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFrozenOrbTimer < uiDiff)
        {
            for(int i = 0; i < m_uiMaxOrbs; i++)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(MOB_FROZEN_ORB_STALKER, pTarget->GetPositionX(), pTarget->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 10000);
            }
            //DoCast(m_creature, m_bIsRegularMode ? SPELL_FROZEN_ORB : SPELL_FROZEN_ORB_H); // spell broken
            m_uiFrozenOrbTimer = 38000;
        }
        else m_uiFrozenOrbTimer -= uiDiff;

        if(m_uiWhiteOutTimer < uiDiff)
        {
			DoCast(m_creature, m_bIsRegularMode ? SPELL_WHITEOUT : SPELL_WHITEOUT_H);
            DoCast(m_creature, SPELL_WHITEOUT_VISUAL);
            m_uiWhiteOutTimer = 38000;
        }
        else m_uiWhiteOutTimer -= uiDiff;

        if(m_uiFreezingGroundTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FREEZING_GROUND : SPELL_FREEZING_GROUND_H);
            m_uiFreezingGroundTimer = 20000;
        }
        else m_uiFreezingGroundTimer -= uiDiff;

        if (m_creature->GetDistance2d(-43.12f, -290.56f) > 80.0f)
            EnterEvadeMode();

        DoMeleeAttackIfReady();
    }
};

/*######
##  Mob Frost Warder
######*/
struct MANGOS_DLL_DECL mob_frost_warderAI : public ScriptedAI
{
    mob_frost_warderAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    bool m_bIsRegularMode;

    uint32 m_uiFrostBlastTimer;

    void Reset()
    {
        m_uiFrostBlastTimer = 5000;
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_FROZEN_MALLET_FW);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFrostBlastTimer < uiDiff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FROST_BLAST : SPELL_FROST_BLAST_H);
            m_uiFrostBlastTimer = urand(3000, 6000);
        }
        else m_uiFrostBlastTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};


/*######
##  Mob Frozen Orb
######*/
struct MANGOS_DLL_DECL mob_frozen_orbAI : public ScriptedAI
{
    mob_frozen_orbAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpellTimer;
    bool m_bHasSpell;
    uint32 m_uiChangeTargetTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_FROZEN_ORB_VISUAL);
        m_creature->SetSplineFlags(SPLINEFLAG_WALKMODE);
        m_creature->SetRespawnDelay(DAY);
        m_uiSpellTimer = 500;
        m_bHasSpell = false;
        m_uiChangeTargetTimer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_TORAVON) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(m_uiSpellTimer < uiDiff && !m_bHasSpell)
        {
            DoCast(m_creature, SPELL_FROZEN_ORB_DMG);
            m_bHasSpell = true;
        }
        else m_uiSpellTimer -= uiDiff;

        if(m_uiChangeTargetTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_creature->AddThreat(pTarget, 100.0f);
                m_creature->GetMotionMaster()->MoveChase(pTarget);
            }
            m_uiChangeTargetTimer = 3000;
        }
        else m_uiChangeTargetTimer -= uiDiff;
    }
};

/*######
##  Mob Frozen Orb Stalker
######*/
struct MANGOS_DLL_DECL mob_frozen_orb_stalkerAI : public ScriptedAI
{
    mob_frozen_orb_stalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(11686);     // make invisible    
        Reset();
    }

    void Reset()
    {
        DoCast(m_creature, SPELL_ORB_SUMMON_TRIG);
    }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    { }
};

CreatureAI* GetAI_boss_toravon(Creature* pCreature)
{
    return new boss_toravonAI (pCreature);
}

CreatureAI* GetAI_mob_frost_warder(Creature* pCreature)
{
    return new mob_frost_warderAI (pCreature);
}

CreatureAI* GetAI_mob_frozen_orb(Creature* pCreature)
{
    return new mob_frozen_orbAI (pCreature);
}

CreatureAI* GetAI_mob_frozen_orb_stalker(Creature* pCreature)
{
    return new mob_frozen_orb_stalkerAI (pCreature);
}

void AddSC_boss_toravon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_toravon";
    newscript->GetAI = &GetAI_boss_toravon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_warder";
    newscript->GetAI = &GetAI_mob_frost_warder;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frozen_orb";
    newscript->GetAI = &GetAI_mob_frozen_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frozen_orb_stalker";
    newscript->GetAI = &GetAI_mob_frozen_orb_stalker;
    newscript->RegisterSelf();
}
