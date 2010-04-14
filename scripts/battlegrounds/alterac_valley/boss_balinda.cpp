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
SDName: Boss_Balinda
SD%Complete: 
SDComment: Timers should be adjusted
EndScriptData */

#include "precompiled.h"

enum Spells
{
    SPELL_ARCANE_EXPLOSION                        = 46608,
    SPELL_CONE_OF_COLD                            = 38384,
    SPELL_FIREBALL                                = 46988,
    SPELL_FROSTBOLT                               = 46987
};

enum Yells
{
    YELL_AGGRO                                    = -2100019,
    YELL_EVADE                                    = -2100020
};

enum Creatures
{
    NPC_WATER_ELEMENTAL                           = 25040
};

enum WaterElementalSpells
{
    SPELL_WATERBOLT                               = 46983
};

struct MANGOS_DLL_DECL mob_water_elementalAI : public ScriptedAI
{
    mob_water_elementalAI(Creature *c) : ScriptedAI(c) {}

    uint32 uiWaterBoltTimer;
    uint64 uiBalindaGUID;
    uint32 uiResetTimer;

    void Reset()
    {
        uiWaterBoltTimer            = 3*IN_MILLISECONDS;
        uiResetTimer                = 5*IN_MILLISECONDS;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiWaterBoltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_WATERBOLT);
            uiWaterBoltTimer = 5*IN_MILLISECONDS;
        } else uiWaterBoltTimer -= diff;

        // check if creature is not outside of building
        if (uiResetTimer < diff)
        {
            if (Creature *pBalinda = ((Creature*)Unit::GetUnit((*m_creature), uiBalindaGUID)))
            {
                float x, y, z;
                pBalinda->GetPosition(x, y, z);
                if(x > -6)
                    EnterEvadeMode();
                uiResetTimer = 5*IN_MILLISECONDS;
            }
        } else uiResetTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_balindaAI : public ScriptedAI
{
    boss_balindaAI(Creature *c) : ScriptedAI(c) {Reset();}


    uint32 ArcaneExplosionTimer;
    uint32 ConeofcoldTimer;
    uint32 FireboltTimer;
    uint32 FrostboltTimer;
    uint32 ResetTimer;
    uint32 uiWaterElementalTimer;

    std::list<uint64> Summons;

    void Reset()
    {
        ArcaneExplosionTimer		= (10+rand()%5)*1000;
        ConeofcoldTimer			= 8000;
        FireboltTimer			= 1000;
        FrostboltTimer			= 4000;
        ResetTimer			    = 5000;
        uiWaterElementalTimer   = 0;

        Summons.clear();
    }

    void Aggro(Unit *who)
    {
        DoScriptText(YELL_AGGRO, m_creature);
    }

    void KilledUnit(Unit* victim){}

    void JustDied(Unit* Killer){}

    void JustSummoned(Creature* summoned)
    {
        ((mob_water_elementalAI*)summoned->AI())->uiBalindaGUID = m_creature->GetGUID();
        summoned->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM,0));
        summoned->setFaction(m_creature->getFaction());
        Summons.push_back(summoned->GetGUID());
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (uiWaterElementalTimer < diff)
        {
            if (Summons.empty())
                m_creature->SummonCreature(NPC_WATER_ELEMENTAL, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 45*IN_MILLISECONDS);
            uiWaterElementalTimer = 50*IN_MILLISECONDS;
        } else uiWaterElementalTimer -= diff;

        if (ArcaneExplosionTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_ARCANE_EXPLOSION);
            ArcaneExplosionTimer =  (10+rand()%5)*1000;
        }else ArcaneExplosionTimer -= diff;

        if (ConeofcoldTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_CONE_OF_COLD);
            ConeofcoldTimer = (10+rand()%10)*1000;
        }else ConeofcoldTimer -= diff;

        if (FireboltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FIREBALL);
            FireboltTimer = (5+rand()%4)*1000;
        }else FireboltTimer -= diff;

        if (FrostboltTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FROSTBOLT);
            FrostboltTimer = (4+rand()%8)*1000;
        }else FrostboltTimer -= diff;

        // check if creature is not outside of building
        if(ResetTimer < diff)
        {
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            if(x > -6)
            {
                DoScriptText(YELL_EVADE, m_creature);
                EnterEvadeMode();
            }
            ResetTimer = 5000;
        }else ResetTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_balinda(Creature *_Creature)
{
    return new boss_balindaAI (_Creature);
}

CreatureAI* GetAI_mob_water_elemental(Creature *_Creature)
{
    return new mob_water_elementalAI (_Creature);
}

void AddSC_boss_balinda()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_balinda";
    newscript->GetAI = &GetAI_boss_balinda;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_water_elemental";
    newscript->GetAI = &GetAI_mob_water_elemental;
    newscript->RegisterSelf();
}
