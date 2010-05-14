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
    SPELL_FREEZING_GROUND       = 72090,  // don't know cd... using 20 secs.
    SPELL_FREEZING_GROUND_H     = 72104,
    SPELL_FROZEN_ORB            = 72091,
    SPELL_FROZEN_ORB_H          = 72095,
    SPELL_WHITEOUT              = 72034,  // Every 38 sec. cast. (after SPELL_FROZEN_ORB)
    SPELL_WHITE0UT_H            = 72096,
    SPELL_FROZEN_MALLET         = 71993,

    // Spells Frost Warder
    SPELL_FROST_BLAST           = 72123,      // don't know cd... using 20 secs.
    SPELL_FROST_BLAST_H         = 72124,
    SPELL_FROZEN_MALLET_FW      = 72122,

    // Spell Frozen Orb
    SPELL_FROZEN_ORB_DMG        = 72081,   // priodic dmg aura
    SPELL_FROZEN_ORB_AURA       = 72067,   // make visible

    // Spell Frozen Orb Stalker
    SPELL_FROZEN_ORB_SUMMON     = 72093,   // summon orb

    // Mob Frozen Orb
    MOB_FROZEN_ORB              = 38456,    // 1 in 10 mode and 3 in 25 mode
};

struct MANGOS_DLL_DECL boss_toravonAI : public ScriptedAI
{
    boss_toravonAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    
    uint32 m_uiFrozenOrbTimer;
    uint32 m_uiWhiteOutTimer;
    uint32 m_uiFreezingGroundTimer;

    void Reset()
    {
        m_uiFrozenOrbTimer      = 11000;
        m_uiWhiteOutTimer       = 13000;
        m_uiFreezingGroundTimer = 15000;

        if (pInstance)
            pInstance->SetData(TYPE_TORAVON, NOT_STARTED);
    }

    void KilledUnit(Unit* Victim) {}

    void JustDied(Unit* Killer)
    {
        if (pInstance)
            pInstance->SetData(TYPE_TORAVON, DONE);
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_FROZEN_MALLET);

        if(pInstance) 
            pInstance->SetData(TYPE_TORAVON, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFrozenOrbTimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_FROZEN_ORB : SPELL_FROZEN_ORB_H);
            m_uiFrozenOrbTimer = 38000;
        }
        else m_uiFrozenOrbTimer -= diff;

        if(m_uiWhiteOutTimer < diff)
        {
            DoCast(m_creature, Regular ? SPELL_WHITEOUT : SPELL_WHITE0UT_H);
            m_uiWhiteOutTimer = 38000;
        }
        else m_uiWhiteOutTimer -= diff;

        if(m_uiFreezingGroundTimer < diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, Regular ? SPELL_FREEZING_GROUND : SPELL_FREEZING_GROUND_H);
            m_uiFreezingGroundTimer = 20000;
        }
        else m_uiFreezingGroundTimer -= diff;

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
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* pInstance;
    bool Regular;

    uint32 m_uiFrostBlastTimer;

    void Reset()
    {
        m_uiFrostBlastTimer = 5000;
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_FROZEN_MALLET_FW);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiFrostBlastTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FROST_BLAST);
            m_uiFrostBlastTimer = 2000;
        }
        else m_uiFrostBlastTimer -= diff;

        DoMeleeAttackIfReady();
    }
};


/*######
##  Mob Frozen Orb
######*/
struct MANGOS_DLL_DECL mob_frozen_orbAI : public ScriptedAI
{
    mob_frozen_orbAI(Creature *pCreature) : ScriptedAI(pCreature) {}

    bool done;
    uint32 killtimer;

    void Reset()
    {
        done = false;
        killtimer = 60000; // if after this time there is no victim -> destroy!
    }

    void UpdateAI(const uint32 diff)
    {
        if (!done)
        {
            DoCast(m_creature, SPELL_FROZEN_ORB_AURA, true);
            DoCast(m_creature, SPELL_FROZEN_ORB_DMG, true);
            done = true;
        }

        if (killtimer <= diff)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                m_creature->ForcedDespawn();
            killtimer = 10000;
        }
        else
            killtimer -= diff;
    }
};

/*######
##  Mob Frozen Orb Stalker
######*/
struct MANGOS_DLL_DECL mob_frozen_orb_stalkerAI : public ScriptedAI
{
    mob_frozen_orb_stalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pCreature->SetVisibility(VISIBILITY_OFF);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        spawned = false;
    }

    ScriptedInstance *pInstance;
    bool spawned;
    bool Regular;

    void UpdateAI(const uint32 diff)
    {
        if (!spawned)
        {
            Creature* pToravon;
            if (pInstance && (pToravon = (Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(DATA_TORAVON))))
            {
                uint8 num_orbs = Regular ? 1 : 3;
                for (uint8 i=0; i<num_orbs; ++i)
                {
                    m_creature->GetMotionMaster()->MovePoint(0, pToravon->GetPositionX(), pToravon->GetPositionY() + 10, pToravon->GetPositionZ());
                    DoCast(m_creature, SPELL_FROZEN_ORB_SUMMON);
                }
            }
            spawned = true;
        }
    }
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

/*CreatureAI* GetAI_mob_frozen_orb_stalker(Creature* pCreature)
{
    return new mob_frozen_orb_stalkerAI (pCreature);
}*/

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

    /*newscript = new Script;
    newscript->Name = "mob_frozen_orb_stalker";
    newscript->GetAI = &GetAI_mob_frozen_orb_stalker;
    newscript->RegisterSelf();*/
}
