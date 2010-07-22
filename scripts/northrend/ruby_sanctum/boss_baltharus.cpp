/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_baltharus
SD%Complete: 90%
SDComment: by notagain and /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */
// Not fully offlike clone work, but Blizz idea is intact.
// Need correct timers

#include "precompiled.h"
#include "ruby_sanctum.h"

static Locations SpawnLoc[]=
{
    {3152.329834f, 359.41757f, 85.301605f},    // Baltharus target point
    {3153.06f, 389.486f, 86.2596f},            // Baltharus initial point
};

enum Equipment
{
    EQUIP_MAIN           = 49888,
    EQUIP_OFFHAND        = EQUIP_NO_CHANGE,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum BossSpells
{
    SPELL_BLADE_TEMPEST              = 75125, //every 22 secs
    SPELL_ENERVATING_BRAND           = 74502, //friendlys in 12yards = 74505
    SPELL_REPELLING_WAVE             = 74509, //every 10-15 secs
    SPELL_SUMMON_CLONE               = 74511, //summons npc 39899 (Clone)
    SPELL_CHANNEL_SPELL              = 76221, //Channeling dummy spell
};

/*######
## boss_baltharus
######*/

struct MANGOS_DLL_DECL boss_baltharusAI : public ScriptedAI
{
    boss_baltharusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    Creature* Baltharus_Target;
    Creature* Clone;
    bool inCombat;
    bool intro;

    void Reset()
    {
        if(!pInstance)
            return;

        if (m_creature->isAlive()) pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
        m_creature->SetRespawnDelay(7*DAY);
        //resetTimers();
        stage = 0;
        Clone = NULL;
        inCombat = false;
        intro = false;
        if (Baltharus_Target = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_BALTHARUS_TARGET))))
        {
            Baltharus_Target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            Baltharus_Target->GetMotionMaster()->MoveIdle();
        } else
               if (Baltharus_Target = m_creature->SummonCreature(NPC_BALTHARUS_TARGET, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 1000))
               {
                   Baltharus_Target->GetMotionMaster()->MoveIdle();
               }

        if(Creature* pTarget = ((Creature*)Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_XERESTRASZA))))
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        if (Baltharus_Target) Baltharus_Target->ForcedDespawn();
        pInstance->SetData(TYPE_BALTHARUS, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);
        if(!pInstance || intro ||
            pWho->GetTypeId() != TYPEID_PLAYER ||
            !pWho->IsWithinDistInMap(m_creature, 60.0f)) return;

        pInstance->SetData(TYPE_EVENT, 10);
        DoScriptText(-1666305,m_creature);
        intro = true;
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;

        if (Baltharus_Target) Baltharus_Target->ForcedDespawn();
        DoScriptText(-1666303,m_creature);
        pInstance->SetData(TYPE_BALTHARUS, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned) return;

        if ( summoned->GetEntry() != NPC_BALTHARUS_TARGET ) 
        {
             if (!Clone) Clone = summoned;
             else if (!Clone->isAlive()) Clone = summoned;
             summoned->SetInCombatWithZone();
        }
    }

    void SummonedCreatureJustDied(Creature* summoned)
    {
         if (!pInstance || !summoned) return;

         if (summoned == Clone) Clone = NULL;
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;
        if (pWho->GetTypeId() != TYPEID_PLAYER) return;

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        inCombat = true;
        m_creature->InterruptNonMeleeSpells(true);
        m_creature->SetInCombatWithZone();
        SetCombatMovement(true);
        pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
        DoScriptText(-1666300,m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance) return;

        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
          return;

        if (Clone && Clone->isAlive())
        {
            pDoneBy->DealDamage(Clone, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            uiDamage = 0;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance) return;

        if (!inCombat && !m_creature->IsNonMeleeSpellCasted(false))
            //timedCast(SPELL_CHANNEL_SPELL, uiDiff, Baltharus_Target);

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (stage)
        {
            case 0:
                 if ( m_creature->GetHealthPercent() <= 66.0f) stage = 1;
                 break;

            case 1:
                 m_creature->InterruptNonMeleeSpells(true);
                 //if (currentDifficulty == RAID_DIFFICULTY_25MAN_NORMAL
                     //|| currentDifficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                     {
                        DoCast(m_creature, SPELL_SUMMON_CLONE);
                     };
                 stage = 2;
                 break;

            case 2:
                 if ( m_creature->GetHealthPercent() <= 50.0f) stage = 3;
                 break;

            case 3:
                 m_creature->InterruptNonMeleeSpells(true);
                 //if (currentDifficulty == RAID_DIFFICULTY_10MAN_NORMAL
                     //|| currentDifficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                     {
                        DoCast(m_creature, SPELL_SUMMON_CLONE);
                     };
                 stage = 4;
                 break;

            case 4:
                 if ( m_creature->GetHealthPercent() <= 33.0f) stage = 5;
                 break;

            case 5:
                 m_creature->InterruptNonMeleeSpells(true);
                 //if (currentDifficulty == RAID_DIFFICULTY_25MAN_NORMAL
                     //|| currentDifficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                     {
                        DoCast(m_creature, SPELL_SUMMON_CLONE);
                     };
                 stage = 6;
                 break;

            case 6:
            default:
                 break;
        }

        if (m_creature->IsNonMeleeSpellCasted(false)) return;

        //timedCast(SPELL_BLADE_TEMPEST, uiDiff);
        //timedCast(SPELL_ENERVATING_BRAND, uiDiff);
        //timedCast(SPELL_REPELLING_WAVE, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

/*######
## mob_baltharus_clone
######*/

struct MANGOS_DLL_DECL mob_baltharus_cloneAI : public ScriptedAI
{
    mob_baltharus_cloneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(!pInstance) return;
        //resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);

        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!pInstance) return;

        if (pInstance->GetData(TYPE_BALTHARUS) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        //timedCast(SPELL_BLADE_TEMPEST, uiDiff);

        //timedCast(SPELL_ENERVATING_BRAND, uiDiff);
    }
};

CreatureAI* GetAI_mob_baltharus_clone(Creature* pCreature)
{
    return new mob_baltharus_cloneAI(pCreature);
}


void AddSC_boss_baltharus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baltharus";
    newscript->GetAI = &GetAI_boss_baltharus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_baltharus_clone";
    newscript->GetAI = &GetAI_mob_baltharus_clone;
    newscript->RegisterSelf();
}
