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
SDName: Violet_Hold
SD%Complete: 40
SDComment:
SDCategory: Violet Hold
EndScriptData */

/* ContentData
go_activation_crystal
npc_door_seal
npc_sinclari
npc_teleportation_portal
EndContentData */

#include "precompiled.h"
#include "violet_hold.h"
#include "escort_ai.h"

enum
{
    SPELL_TELEPORT_INSIDE                 = 62139, // maybe 62138 ?

    //DRAGONS SPELLS
    //Azure Captain
    SPELL_MORTAL_STRIKE                   = 32736,
    SPELL_WHIRLWIND                       = 41057,

    //Azure Raider
    SPELL_CONCUSSION_BLOW                 = 52719,
    SPELL_MAGIC_REFLECTION                = 60158,

    //Azure Sorceror
    SPELL_ARCANE_STREAM                   = 60181,
    SPELL_ARCANE_STREAM_H                 = 60204,
    SPELL_MANA_DETONATION                 = 60182,
    SPELL_MANA_DETONATION_H               = 60205,

    //Azure stalker
    SPELL_BACKSTAB                        = 58471,
    SPELL_TACTICAL_BLINK                  = 58470,

    //Azure guardian
    SPELL_AGONISING_STRIKE                  = 58504,
    SPELL_SIDE_SWIPE                        = 58508,

    //Azure keeper
    SPELL_ARCANE_MISSILES                   = 58531,
    SPELL_ARCANE_MISSILES_H                 = 61593,
    SPELL_DEEP_FREEZE                       = 58534,
    SPELL_FROSTBOLT_VOLLEY                  = 58532,
    SPELL_FROSTBOLT_VOLLEY_H                = 61594,

    //Azure binder
    SPELL_ARCANE_BARRAGE                    = 58456,
    SPELL_ARCANE_BARRAGE_H                  = 59248,
    SPELL_ARCANE_EXPLOSION                  = 58455,
    SPELL_ARCANE_EXPLOSION_H                = 59245,

    //Azure invader
    SPELL_CLEAVE                            = 15496,
    SPELL_IMPALE                            = 58459,
    SPELL_IMPALE_H                          = 59256,

    //Azure mageslayer
    SPELL_ARCANE_EMPOWERMENT                = 58469,

    //Azure spellbreaker
    SPELL_ARCANE_BLAST                      = 58462,
    SPELL_ARCANE_BLAST_H                    = 59257,
    SPELL_SLOW                              = 25603,

    //Veteran Mage Hunter
    SPELL_ARCANE_BOLT                       = 13748,
    SPELL_ARCANE_BOLT_H                     = 20829,
    SPELL_FIREBALL                          = 12466,
    SPELL_FIREBALL_H                        = 20823,
    SPELL_FROSTBOLT                         = 15043,
    SPELL_FROSTBOLT_H                       = 20822,

    // yells
    SAY_CYANIGOSA_SPAWN         = -1608055,
    SAY_EREKEM_SPAWN            = -1608015,
    SAY_ICHORON_SPAWN           = -1608023,
    SAY_XEVOZZ_SPAWN            = -1608032,
    SAY_ZURAMAT_SPAWN           = -1608042,
};

/*######
## mob_vh_dragons
## This script is for ALL mobs which are spawned from portals,
## they have to be scripted in SD2 because in EventAI you cant
## check for distance from door seal :/
## (Intro not implented yet)
######*/
struct MANGOS_DLL_DECL mob_vh_dragonsAI : public ScriptedAI
{
    mob_vh_dragonsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegular = pCreature->GetMap()->IsRegularDifficulty();
        WayPointList.clear();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegular;
    uint32 creatureEntry;
    uint32 motherPortalID;
    uint32 WalkTimer;
    int8 portalLoc;
    bool IsWalking;
    bool IsInCombat;
    bool MovementStarted;
    uint64 pDoorSealGUID;

    std::list<WayPoints> WayPointList;
    std::list<WayPoints>::iterator WayPoint;

    //Azure Captain
    uint32 m_uiMortalStrike_Timer;
    uint32 m_uiWhirlwind_Timer;

    //Azure Raider
    uint32 m_uiConcussionBlow_Timer;
    uint32 m_uiMagicReflection_Timer;

    //Azure Sorceror
    uint32 m_uiArcaneStream_Timer;
    uint32 m_uiManaDetonation_Timer;

    //Azure Stalker
    uint32 m_uiBackstab_Timer;
    uint32 m_uiBlink_Timer;

    //Azure Guardian
    uint32 m_uiAgonisingStrike_Timer;
    uint32 m_uiSideSwipe_Timer;

    //Azure Keepers
    uint32 m_uiArcaneMissile_Timer;
    uint32 m_uiFrostboltVolley_Timer;
    uint32 m_uiDeepFreeze_Timer;

    //Azure Binder
    uint32 m_uiArcaneBarrage_Timer;
    uint32 m_uiExplosion_Timer;

    //Azure Invader
    uint32 m_uiCleave_Timer;
    uint32 m_uiImpale_Timer;

    //Azure Mageslayer
    uint32 m_uiArcaneEmpowerment_Timer;

    //Azure Spellbreaker
    uint32 m_uiArcaneblast_Timer;
    uint32 m_uiSlow_Timer;

    //Veteran Mage Hunter
    uint32 m_uiArcaneboltTimer;
    uint32 m_uiFireballTimer;
    uint32 m_uiFrostboltTimer;

    void Reset()
    {
        creatureEntry = m_creature->GetEntry();
        motherPortalID = 0;
        WalkTimer = 200;
        portalLoc = -1;
        IsWalking = false;
        IsInCombat = false;
        MovementStarted = false;
        m_creature->SetRespawnDelay(DAY);

        if(Creature* pDoorSeal = GetClosestCreatureWithEntry(m_creature, NPC_DOOR_SEAL, 150.0f))
            pDoorSealGUID = pDoorSeal->GetGUID();

        //Azure Captain
        m_uiMortalStrike_Timer      = 3000;
        m_uiWhirlwind_Timer         = 5000;

        //Azure Raider
        m_uiConcussionBlow_Timer    = 3000;
        m_uiMagicReflection_Timer   = 10000;

        //Azure Sorceror
        m_uiArcaneStream_Timer      = 5000;
        m_uiManaDetonation_Timer    = 3000;

        //Azure Stalker
        m_uiBackstab_Timer          = 7100;
        m_uiBlink_Timer             = 7000;

        //Azure Guardian
        m_uiAgonisingStrike_Timer   = 10000;
        m_uiSideSwipe_Timer         = 7000;

        //Azure Keepers
        m_uiArcaneMissile_Timer     = 5500;
        m_uiFrostboltVolley_Timer   = 10000;
        m_uiDeepFreeze_Timer        = 7500;

        //Azure Binder
        m_uiArcaneBarrage_Timer     = 5000;
        m_uiExplosion_Timer         = 8000;

        //Azure Invader
        m_uiCleave_Timer            = 6000;
        m_uiImpale_Timer            = 10000;

        //Azure Mageslayer
        m_uiArcaneEmpowerment_Timer = 7000;

        //Azure Spellbreaker
        m_uiArcaneblast_Timer       = 5000;
        m_uiSlow_Timer              = 9000;

        //Veteran Mage Hunter
        m_uiArcaneboltTimer         = urand(5000, 10000);
        m_uiFireballTimer           = urand(5000, 10000);
        m_uiFrostboltTimer          = urand(5000, 10000);
    }

    void StartMovement()
    {
        if(!WayPointList.empty() || MovementStarted)
            return;

        uint8 start = 0;
        uint8 end = 0;

        switch(portalLoc)
        {
        case -1:
            return;
            //center & ichoron & xevozz
        case 3:
        case 6:
        case 8:
            start = 0;
            end = 2;
            break;
            //From lavanthor
        case 7:
            start = 3;
            end = 5;
            break;
            // From Zuramat
        case 5:
            start = 6;
            end = 11;
            break;
            //From Moragg
        case 2:
            start = 13;
            end = 17;
            break;
            //From Erekem
        case 1:
            start = 18;
            end = 22;
            break;
            //From highest platform
        case 0:
            start = 23;
            end = 27;
            break;
            //From bridge
        case 4:
            start = 12;
            end = 17;
            break;
        }

        uint8 wpId = 0;
        for(uint8 i = start; i <= end; ++i)
        {
            debug_log("AddWP: %u", i);
            AddWaypoint(wpId, DragonsWP[i].x, DragonsWP[i].y, DragonsWP[i].z);
            wpId++;
        }

        WayPoint = WayPointList.begin();
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        IsWalking = true;
        MovementStarted = true;
    }

    void AddWaypoint(uint32 id, float x, float y, float z)
    {
        WayPoints DWP(id, x, y, z);
        WayPointList.push_back(DWP);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(WayPoint->id != uiPointId)
            return;

        ++WayPoint;
        WalkTimer = 200;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAIN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if(portalLoc != -1 && creatureEntry != NPC_PORTAL_GUARDIAN && creatureEntry != NPC_PORTAL_KEEPER)
            StartMovement();

        if (IsWalking && WalkTimer)
        {
            if (WalkTimer <= uiDiff)
            {
                if (WayPoint != WayPointList.end())
                {
                    m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y,WayPoint->z);
                    WalkTimer = 0;
                }
            }else WalkTimer -= uiDiff;
        }

        //Corrupt Seal
        if(Creature* pDoorSeal = m_pInstance->instance->GetCreature(pDoorSealGUID))
        {
            if(!IsInCombat)
            {
                if(m_creature->IsWithinDist(pDoorSeal, 27.0f, false))
                {
                    IsWalking = false;
                    WayPointList.clear();
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                    DoCast(pDoorSeal, SPELL_DESTROY_DOOR_SEAL);
                }
            }
        }

        if(!IsWalking && !IsInCombat && creatureEntry != NPC_PORTAL_GUARDIAN && creatureEntry != NPC_PORTAL_KEEPER) 
        {
            if (Unit* m_uEmbraceTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->GetMotionMaster()->MoveChase(m_uEmbraceTarget);
            m_creature->SetInCombatWithZone();
            IsInCombat = true;
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(creatureEntry != NPC_PORTAL_GUARDIAN && creatureEntry != NPC_PORTAL_KEEPER)
                StartMovement();
            return;
        }

        if(m_creature->getVictim())
            if(m_creature->getVictim()->GetEntry() == NPC_DOOR_SEAL)
                return;

        switch(creatureEntry)
        {
        case NPC_AZURE_CAPTAIN:
            AzureCaptain_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_RAIDER:
            AzureRaider_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_SORCEROR:
            AzureSorceror_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_STALKER:
            AzureStalker_UpdateAI(uiDiff);
            break;
        case NPC_PORTAL_GUARDIAN:
            AzureGuardian_UpdateAI(uiDiff);
            break;
        case NPC_PORTAL_KEEPER:
            AzureKeeper_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_BINDER:
            AzureBinder_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_INVADER:
            AzureInvader_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_MAGE_SLAYER:
            AzureMageslayer_UpdateAI(uiDiff);
            break;
        case NPC_AZURE_SPELLBREAKER:
            AzureSpellbreaker_UpdateAI(uiDiff);
            break;
        case NPC_MAGE_HUNTER:
            VeteranMageHunter_UpdateAI(uiDiff);
            break;
        default:
            debug_log("SD2: The Violet Hold: Unhandled dragon entry %u!", m_creature->GetEntry());
            break;
        }

        DoMeleeAttackIfReady();
    }

    //Azure Captain
    void AzureCaptain_UpdateAI(const uint32 uiDiff)
    {
        //Mortal Strike
        if (m_uiMortalStrike_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
            m_uiMortalStrike_Timer = 6000;
        }else m_uiMortalStrike_Timer -= uiDiff;

        //Whirlwind
        if (m_uiWhirlwind_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
            m_uiWhirlwind_Timer = 15000;
        }else m_uiWhirlwind_Timer -= uiDiff;
    }

    //Azure Raider
    void AzureRaider_UpdateAI(const uint32 uiDiff)
    {
        //Concusion Blow
        if (m_uiConcussionBlow_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CONCUSSION_BLOW);
            m_uiMortalStrike_Timer = 7000;
        }else m_uiConcussionBlow_Timer -= uiDiff;

        //Magic reflection
        if (m_uiMagicReflection_Timer <= uiDiff)
        {
            DoCast(m_creature, SPELL_MAGIC_REFLECTION);
            m_uiMagicReflection_Timer = 30000;
        }else m_uiMagicReflection_Timer -= uiDiff;
    }

    //Azure Sorceror
    void AzureSorceror_UpdateAI(const uint32 uiDiff)
    {
        //Arcane Stream
        if (m_uiArcaneStream_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_STREAM : SPELL_ARCANE_STREAM_H);
            m_uiArcaneStream_Timer = 7000;
        }else m_uiArcaneStream_Timer -= uiDiff;

        //Mana Detonation
        if (m_uiManaDetonation_Timer <= uiDiff)
        {
            DoCast(m_creature, m_bIsRegular ? SPELL_MANA_DETONATION : SPELL_MANA_DETONATION_H);
            m_uiManaDetonation_Timer = 18000;
        }else m_uiManaDetonation_Timer -= uiDiff;
    }

    //Azure Stalker
    void AzureStalker_UpdateAI(const uint32 uiDiff)
    {
        //Backstab
        if (m_uiBackstab_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_BACKSTAB);
            m_uiBackstab_Timer = 15100;
        }else m_uiBackstab_Timer -= uiDiff;

        //Tactical blink
        if (m_uiBlink_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_TACTICAL_BLINK);
            m_uiBlink_Timer = 15000;
        }else m_uiBlink_Timer -= uiDiff;
    }

    //Azure Guardian
    void AzureGuardian_UpdateAI(const uint32 uiDiff)
    {
        //Agonising Strike
        if (m_uiAgonisingStrike_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_AGONISING_STRIKE);
            m_uiAgonisingStrike_Timer = 15100;
        }else m_uiAgonisingStrike_Timer -= uiDiff;

        //SideSwipe
        if (m_uiSideSwipe_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SIDE_SWIPE);
            m_uiSideSwipe_Timer = 10000;
        }else m_uiSideSwipe_Timer -= uiDiff;
    }

    //Azure Keeper
    void AzureKeeper_UpdateAI(const uint32 uiDiff)
    {
        //ArcaneMissile
        if (m_uiArcaneMissile_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_MISSILES : SPELL_ARCANE_MISSILES_H);
            m_uiArcaneMissile_Timer = 10000;
        }else m_uiArcaneMissile_Timer -= uiDiff;

        //Frostbolt volley
        if (m_uiFrostboltVolley_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegular ? SPELL_FROSTBOLT_VOLLEY : SPELL_FROSTBOLT_VOLLEY_H);
            m_uiFrostboltVolley_Timer = 16000;
        }else m_uiFrostboltVolley_Timer -= uiDiff;

        //Deep freeze
        if (m_uiDeepFreeze_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_DEEP_FREEZE);
            m_uiDeepFreeze_Timer = 14000;
        }else m_uiDeepFreeze_Timer -= uiDiff;
    }

    //Azure Binder
    void AzureBinder_UpdateAI(const uint32 uiDiff)
    {
        //Arcane barrage
        if (m_uiArcaneBarrage_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_BARRAGE : SPELL_ARCANE_BARRAGE_H);
            m_uiArcaneBarrage_Timer = 8100;
        }else m_uiArcaneBarrage_Timer -= uiDiff;

        //Arcane explosion
        if (m_uiExplosion_Timer <= uiDiff)
        {
            DoCast(m_creature, m_bIsRegular ? SPELL_ARCANE_EXPLOSION : SPELL_ARCANE_EXPLOSION_H);
            m_uiExplosion_Timer = 13000;
        }else m_uiExplosion_Timer -= uiDiff;
    }

    //Azure Invader
    void AzureInvader_UpdateAI(const uint32 uiDiff)
    {
        //Cleave
        if (m_uiCleave_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleave_Timer = 7100;
        }else m_uiCleave_Timer -= uiDiff;

        //Impale
        if (m_uiImpale_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_IMPALE : SPELL_IMPALE_H);
            m_uiImpale_Timer = 13000;
        }else m_uiImpale_Timer -= uiDiff;
    }

    //Azure Mage slayer
    void AzureMageslayer_UpdateAI(const uint32 uiDiff)
    {
        //Arcane empowerment
        if (m_uiArcaneEmpowerment_Timer <= uiDiff)
        {
            DoCast(m_creature, SPELL_ARCANE_EMPOWERMENT);
            m_uiArcaneEmpowerment_Timer = 13100;
        }else m_uiArcaneEmpowerment_Timer -= uiDiff;
    }

    //Azure Spell breaker
    void AzureSpellbreaker_UpdateAI(const uint32 uiDiff)
    {
        //Arcane blast
        if (m_uiArcaneblast_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_BLAST : SPELL_ARCANE_BLAST_H);
            m_uiArcaneblast_Timer = 10100;
        }else m_uiArcaneblast_Timer -= uiDiff;

        //Slow
        if (m_uiSlow_Timer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SLOW);
            m_uiSlow_Timer = 15000;
        }else m_uiSlow_Timer -= uiDiff;
    }

    //Veteran Mage Hunter
    void VeteranMageHunter_UpdateAI(const uint32 uiDiff)
    {
        //Arcanebolt
        if (m_uiArcaneboltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_BOLT : SPELL_ARCANE_BOLT_H);
            m_uiArcaneboltTimer = urand(8000, 13000);
        }else m_uiArcaneboltTimer -= uiDiff;

        //Fireball
        if (m_uiFireballTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireballTimer = urand(8000, 13000);
        }else m_uiFireballTimer -= uiDiff;

        //Frostbolt
        if (m_uiFrostboltTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegular ? SPELL_FROSTBOLT : SPELL_FROSTBOLT_H);
            m_uiFrostboltTimer = urand(8000, 13000);
        }else m_uiFrostboltTimer -= uiDiff;
    }
};


/*######
## go_activation_crystal
######*/

bool GOHello_go_activation_crystal(Player* pPlayer, GameObject* pGo)
{
    if (instance_violet_hold* pInstance = (instance_violet_hold*)pGo->GetInstanceData())
        pInstance->ProcessActivationCrystal(pPlayer);

    return false;
}

/*######
## npc_door_seal
######*/

struct MANGOS_DLL_DECL npc_door_sealAI : public ScriptedAI
{
    npc_door_sealAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();   
        Reset();
    }

    instance_violet_hold* m_pInstance;

    uint32 CheckTimer;
    uint32 SpellCorrupt_Timer;
    uint8 lastPortal;

    uint32 m_uiHealthPoint;

    void Reset()
    {
        CheckTimer = 0;
        SpellCorrupt_Timer = 0;
        lastPortal = 0;
        m_uiHealthPoint = 75;
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (SpellCorrupt_Timer)
            return;

        if (spell->Id == SPELL_DESTROY_DOOR_SEAL)
            SpellCorrupt_Timer = 1000;
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->Respawn();

        if(Creature* pSinclari = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_SINCLARI)))
            pSinclari->DealDamage(pSinclari, pSinclari->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_MAIN, FAIL);
            m_pInstance->SetData(TYPE_SEAL, NOT_STARTED);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (SpellCorrupt_Timer)
        {
            if (SpellCorrupt_Timer <= diff)
            {
                if (m_creature->HasAura(SPELL_DESTROY_DOOR_SEAL,EFFECT_INDEX_0))
                {
                    SpellCorrupt_Timer = 1500;
                    m_pInstance->SetData(TYPE_SEAL, IN_PROGRESS);
                }
                else
                {
                    m_pInstance->SetData(TYPE_SEAL, NOT_STARTED);
                    SpellCorrupt_Timer = 0;
                }

                if(Creature* pSinclari = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_SINCLARI)))
                {
                    if (m_pInstance->GetCurrentSealCount() <= m_uiHealthPoint)
                    {
                        if(m_uiHealthPoint == 75)
                        {
                            DoScriptText(SAY_SEAL_75, pSinclari);
                            m_uiHealthPoint = 50;
                        }
                        else if (m_uiHealthPoint == 50)
                        {
                            DoScriptText(SAY_SEAL_50, pSinclari);
                            m_uiHealthPoint = 5;
                        }
                        else if (m_uiHealthPoint == 5)
                        {
                            DoScriptText(SAY_SEAL_5, pSinclari);
                            m_uiHealthPoint = 0;
                        }
                    }
                }
            }else SpellCorrupt_Timer -= diff;
        }
    }
};

bool EffectDummyCreature_npc_door_seal(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_DESTROY_DOOR_SEAL && uiEffIndex == EFFECT_INDEX_0)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreatureTarget->GetInstanceData())
            pInstance->SetData(TYPE_SEAL, SPECIAL);

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## npc_sinclari
######*/

enum
{
    SAY_BEGIN                   = -1608000,
    SAY_LOCK_DOOR               = -1608001,

	GOSSIP_ITEM_INTRO           = -3608000,
	GOSSIP_ITEM_START           = -3608001,

    GOSSIP_TEXT_ID_INTRO        = 13853,
    GOSSIP_TEXT_ID_START        = 13854,
};

#define GOSSIP_ITEM_TELE_IN     "I need to go in!"

struct MANGOS_DLL_DECL npc_sinclariAI : public npc_escortAI
{
    npc_sinclariAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        Reset();
    }

    instance_violet_hold* m_pInstance;

    void Reset()
    { }

    void WaypointReached(uint32 uiPointId)
    {
        if (!m_pInstance)
            return;

        switch(uiPointId)
        {
        case 0:
            m_pInstance->ProcessActivationCrystal(m_creature, true);
            break;
        case 1:
            DoScriptText(SAY_BEGIN, m_creature);
            m_pInstance->SetIntroPortals(true);
            m_pInstance->CallGuards(false);
            break;
        case 2:
            DoScriptText(SAY_LOCK_DOOR, m_creature);
            m_pInstance->SetData(TYPE_MAIN, IN_PROGRESS);
            if(GameObject* pDoor = GetClosestGameObjectWithEntry(m_creature, GO_PRISON_SEAL_DOOR, 150.0f))
                m_pInstance->DoUseDoorOrButton(pDoor->GetGUID());
            break;
        case 3:
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            SetEscortPaused(true);
            break;
        }
    }

    void JustRespawned()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAIN, NOT_STARTED);
    }
};

CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI(pCreature);
}

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData();

    if(pInstance->GetData(TYPE_CYANIGOSA) == DONE)
    {
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        return true;
    }

    if(pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INTRO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    else if(pInstance->GetData(TYPE_MAIN) == IN_PROGRESS)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_IN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_INTRO, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_sinclari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
            {
                pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXT_ID_START, pCreature->GetGUID());
            }
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreature->GetInstanceData())
        {
            pPlayer->CLOSE_GOSSIP_MENU();

            if (pInstance->GetData(TYPE_MAIN) == NOT_STARTED)
            {
                pInstance->SetData(TYPE_MAIN, SPECIAL);

                if (npc_sinclariAI* pEscortAI = dynamic_cast<npc_sinclariAI*>(pCreature->AI()))
                    pEscortAI->Start();
            }
        }
        else
            pPlayer->CLOSE_GOSSIP_MENU();
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+3)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_INSIDE, false);
    }

    return true;
}

/*######
## npc_teleportation_portal
######*/

struct MANGOS_DLL_DECL npc_teleportation_portalAI : public ScriptedAI
{
    npc_teleportation_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_violet_hold*)pCreature->GetInstanceData();
        m_uiMyPortalNumber = 0;
        Reset();
    }

    instance_violet_hold* m_pInstance;

    std::set<uint64> m_lMobSet;

    bool m_bNeedInvisible;
    bool m_bIntro;
    uint32 m_uiIntroTimer;
    uint32 m_uiMyPortalNumber;
    uint8 m_uiPortalId;

    void Reset()
    {
        m_bNeedInvisible = false;
        m_bIntro = false;
        m_uiIntroTimer = 10000;
        m_creature->SetRespawnDelay(DAY);

        if (m_pInstance)
        {
            m_uiMyPortalNumber = m_pInstance->GetCurrentPortalNumber();
            m_uiPortalId = m_pInstance->GetCurrentPortalId();
        }
    }

    void DoSummon()
    {
        if (m_creature->GetEntry() == NPC_PORTAL_INTRO)
        {
            //not made yet
            return;
        }
        else if (m_creature->GetEntry() == NPC_PORTAL)
        {
            m_creature->SummonCreature(m_pInstance->GetRandomPortalEliteEntry(), 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
            m_creature->CastSpell(m_creature, SPELL_PORTAL_PERIODIC, true);
        }
        else if (m_pInstance->IsCurrentPortalForTrash())
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                uint32 uiSummonId;

                switch(i)
                {
                case 0: uiSummonId = NPC_AZURE_CAPTAIN; break;
                case 1: uiSummonId = NPC_AZURE_RAIDER; break;
                case 2: uiSummonId = NPC_AZURE_SORCEROR; break;
                case 3: uiSummonId = NPC_AZURE_STALKER; break;
                }

                if(Creature* pDragon = m_creature->SummonCreature(uiSummonId, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS))
                    ((mob_vh_dragonsAI*)pDragon->AI())->portalLoc = m_uiPortalId;
            }

            m_bNeedInvisible = true;
        }
        else
        {
            if(m_uiMyPortalNumber == 18)
            {
                if(Creature *Cyanigosa = m_creature->SummonCreature(NPC_CYANIGOSA, PortalLoc[8].x, PortalLoc[8].y, PortalLoc[8].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 180000))
                    DoScriptText(SAY_CYANIGOSA_SPAWN, Cyanigosa);
                m_bNeedInvisible = true;
            }
            else
            {
                m_creature->SummonCreature(NPC_AZURE_SABOTEUR, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS);
                m_bNeedInvisible = true;
            }
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
        case NPC_PORTAL_GUARDIAN:
            DoScriptText(EMOTE_GUARDIAN_PORTAL, pSummoned);
            m_creature->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
            break;
        case NPC_PORTAL_KEEPER:
            DoScriptText(EMOTE_KEEPER_PORTAL, pSummoned);
            m_creature->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
            break;
        case NPC_AZURE_CAPTAIN:
            DoScriptText(EMOTE_DRAGONFLIGHT_PORTAL, pSummoned);
            m_lMobSet.insert(pSummoned->GetGUID());
            break;
        case NPC_AZURE_RAIDER:
        case NPC_AZURE_SORCEROR:
        case NPC_AZURE_STALKER:
            m_lMobSet.insert(pSummoned->GetGUID());
            return;
        default:
            return;
        }

        // set portal locations
        ((mob_vh_dragonsAI*)pSummoned->AI())->portalLoc = m_uiPortalId;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_PORTAL, SPECIAL);
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
        case NPC_PORTAL_GUARDIAN:
        case NPC_PORTAL_KEEPER:
            break;
        case NPC_AZURE_CAPTAIN:
        case NPC_AZURE_RAIDER:
        case NPC_AZURE_SORCEROR:
        case NPC_AZURE_STALKER:
            {
                m_lMobSet.erase(pSummoned->GetGUID());

                if (!m_lMobSet.empty())
                    return;

                break;
            }
        case NPC_AZURE_SABOTEUR:
            break;
        default:
            return;
        }

        if (m_pInstance)
        {
            // no need if a new portal was made while this was in progress
            if (m_uiMyPortalNumber == m_pInstance->GetCurrentPortalNumber())
                m_pInstance->SetData(TYPE_PORTAL, DONE);
        }

        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAIN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_uiIntroTimer)
        {
            if (m_uiIntroTimer <= uiDiff)
            {
                if (!m_pInstance)
                {
                    m_creature->ForcedDespawn();
                    return;
                }

                m_uiIntroTimer = 0;
            }
            else
            {
                m_uiIntroTimer -= uiDiff;
                return;
            }
        }

        if (!m_bIntro)
        {
            DoSummon();
            m_bIntro = true;
        }

        if (m_bNeedInvisible)
        {
            // hack; find a better way
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_bNeedInvisible = false;
        }
    }
};

CreatureAI* GetAI_npc_teleportation_portal(Creature* pCreature)
{
    return new npc_teleportation_portalAI(pCreature);
}

bool EffectDummyCreature_npc_teleportation_portal(Unit* pCaster, uint32 uiSpellId, SpellEffectIndex uiEffIndex, Creature* pCreatureTarget)
{
    //always check spellid and effectindex
    if (uiSpellId == SPELL_PORTAL_PERIODIC && uiEffIndex == EFFECT_INDEX_0)
    {
        if (instance_violet_hold* pInstance = (instance_violet_hold*)pCreatureTarget->GetInstanceData())
        {
            if(Creature* pDragon = pCreatureTarget->SummonCreature(pInstance->GetRandomMobForNormalPortal(), 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600*IN_MILLISECONDS))
                ((mob_vh_dragonsAI*)pDragon->AI())->portalLoc = pInstance->GetCurrentPortalId();
        }

        //always return true when we are handling this spell and effect
        return true;
    }

    return false;
}

/*######
## npc_azure_saboteur
######*/
struct MANGOS_DLL_DECL npc_azure_saboteurAI : public ScriptedAI
{
    npc_azure_saboteurAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((instance_violet_hold*)pCreature->GetInstanceData());
        Reset();
    }
    instance_violet_hold* m_pInstance;

    bool m_bIsActiving;

    uint32 m_uiDisruption_Timer;
    uint32 m_uiDisruptionCounter;
    uint32 m_uiDisruptionsCount;

    uint8 m_uiBossID;
    uint8 m_bIsRegular;
    uint32 m_uiBossType;
    uint64 m_uiBossGUID;
    uint64 m_uiDoorGUID;

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void Reset()
    {
        m_bIsActiving = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_uiDisruptionCounter = 0;
        m_uiDisruptionsCount = 0;
        m_uiDisruption_Timer = 1000;
        m_creature->SetRespawnDelay(DAY);

        if (m_pInstance)
        {
            // check if first boss is done and portals = 6
            if(m_pInstance->GetData(TYPE_FIRST_BOSS) == DONE && m_pInstance->GetCurrentPortalNumber() == 6)
            {
                    m_uiBossID = m_pInstance->GetData(TYPE_FIRST_BOSS_ID);
                    m_uiBossType = GetBossType(m_uiBossID);
            }
            // check if second boss is done and portals = 12
            else if(m_pInstance->GetData(TYPE_SECOND_BOSS) == DONE && m_pInstance->GetCurrentPortalNumber() == 12)
            {
                    m_uiBossID = m_pInstance->GetData(TYPE_SECOND_BOSS_ID);
                    m_uiBossType = GetBossType(m_uiBossID);
            }
            // if boss for current portal no. isn't done yet then choose random
            else
            {
                m_uiBossID = m_pInstance->GetData(TYPE_LASTBOSS);
                m_uiBossType = GetBossType(m_uiBossID);

                if(m_pInstance->GetCurrentPortalNumber() == 6)
                    m_pInstance->SetData(TYPE_FIRST_BOSS_ID, m_uiBossID);
                if(m_pInstance->GetCurrentPortalNumber() == 12)
                    m_pInstance->SetData(TYPE_SECOND_BOSS_ID, m_uiBossID);
            }

            m_pInstance->SetData(TYPE_LASTBOSS_ID, m_uiBossType);

            if (m_uiBossType != 0)  
                m_creature->GetMotionMaster()->MovePoint(0, BossLoc[m_uiBossID].x,  BossLoc[m_uiBossID].y,  BossLoc[m_uiBossID].z);
            else  
                m_creature->GetMotionMaster()->MovePoint(0, 1827.960f, 804.208f, 44.364f);
        }
    }

    uint32 GetBossType(uint32 m_uiID)
    {
        uint32 m_uiType = 0;
        switch (m_uiID)
        {
        case 12: // Lavanthor
            m_uiType = TYPE_LAVANTHOR;
            m_uiBossGUID = m_pInstance->GetData64(NPC_LAVANTHOR);
            break;
        case 13: // Zuramat
            m_uiType = TYPE_ZURAMAT;
            m_uiBossGUID = m_pInstance->GetData64(NPC_ZURAMAT);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_ZURAMAT))))
                DoScriptText(SAY_ZURAMAT_SPAWN, pTemp);
            break;
        case 9: // Moragg
            m_uiType = TYPE_MORAGG;
            m_uiBossGUID = m_pInstance->GetData64(NPC_MORAGG);
            break;
        case 8: // Erekem
            m_uiType = TYPE_EREKEM;
            m_uiBossGUID = m_pInstance->GetData64(NPC_EREKEM);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_EREKEM))))
                DoScriptText(SAY_EREKEM_SPAWN, pTemp);
            break;
        case 10: // Ichoron
            m_uiType = TYPE_ICHORON;
            m_uiBossGUID = m_pInstance->GetData64(NPC_ICHORON);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_ICHORON))))
                DoScriptText(SAY_ICHORON_SPAWN, pTemp);
            break;
        case 11: // Xevozz
            m_uiType = TYPE_XEVOZZ;
            m_uiBossGUID = m_pInstance->GetData64(NPC_XEVOZZ);
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_XEVOZZ))))
                DoScriptText(SAY_XEVOZZ_SPAWN, pTemp);
            break;
        case 0: // No boss
            m_uiType = 0;
            break;
        }
        return m_uiType;
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE)
            return;

        switch(uiPointId)
        {
        case 0:
            m_bIsActiving = true;
            break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_MAIN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_bIsActiving)
        {
            if (m_uiDisruption_Timer < uiDiff)
            {
                if (m_uiDisruptionCounter < 3) 
                {
                    DoCast(m_creature, SPELL_SHIELD_DISRUPTION);
                    ++m_uiDisruptionsCount;
                }
                else if (m_uiDisruptionCounter == 3)
                    m_pInstance->UpdateCellForBoss(m_uiBossID);
                else 
                {
                    m_creature->ForcedDespawn();
                    if (m_pInstance->GetData(TYPE_LASTBOSS_ID) != 0) 
                        m_pInstance->SetData(m_pInstance->GetData(TYPE_LASTBOSS_ID), SPECIAL);
                    m_bIsActiving = false;
                }

                ++m_uiDisruptionCounter;
                m_uiDisruption_Timer = 1000;
            }
            else m_uiDisruption_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_azure_saboteur(Creature* pCreature)
{
    return new npc_azure_saboteurAI (pCreature);
}

CreatureAI* GetAI_mob_vh_dragons(Creature* pCreature)
{
    return new mob_vh_dragonsAI(pCreature);
}

CreatureAI* GetAI_npc_door_seal(Creature* pCreature)
{
    return new npc_door_sealAI(pCreature);
}

void AddSC_violet_hold()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_activation_crystal";
    newscript->pGOHello = &GOHello_go_activation_crystal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_door_seal";
    newscript->GetAI = &GetAI_npc_door_seal;
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_door_seal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_sinclari";
    newscript->GetAI = &GetAI_npc_sinclari;
    newscript->pGossipHello = &GossipHello_npc_sinclari;
    newscript->pGossipSelect = &GossipSelect_npc_sinclari;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_teleportation_portal";
    newscript->GetAI = &GetAI_npc_teleportation_portal;
    newscript->pEffectDummyCreature = &EffectDummyCreature_npc_teleportation_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_azure_saboteur";
    newscript->GetAI = &GetAI_npc_azure_saboteur;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vh_dragons";
    newscript->GetAI = &GetAI_mob_vh_dragons;
    newscript->RegisterSelf();
}
