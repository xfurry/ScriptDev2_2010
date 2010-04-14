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
SDName: violet_hold
SDAuthor: ckegg, modified by rsa
SD%Complete: 60
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"
enum
{
    SPELL_TELEPORT_INSIDE                 = 62139,
    SPELL_SHIELD_DISRUPTION               = 58291,

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
};

enum
{
    SAY_BEGIN                   = -1608000,
    SAY_LOCK_DOOR               = -1608001,
    SAY_CYANIGOSA_SPAWN         = -1608005,
    SAY_EREKEM_SPAWN            = -1608015,
    SAY_ICHORON_SPAWN           = -1608023,
    SAY_XEVOZZ_SPAWN            = -1608032,
    SAY_ZURAMAT_SPAWN           = -1608042,

    GOSSIP_TEXT_ID_INTRO        = 13853,
    GOSSIP_TEXT_ID_START        = 13854,
};

uint32 m_uiNextPortal_Timer;
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
    Creature* pDoorSeal;

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

    void Reset(){
        creatureEntry = m_creature->GetEntry();
        motherPortalID = 0;
        WalkTimer = 200;
        portalLoc = -1;
        IsWalking = false;
        IsInCombat = false;
        MovementStarted = false;
        pDoorSeal = GetClosestCreatureWithEntry(m_creature, NPC_DOOR_SEAL, 150.0f);
       
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
            //center & ichoron
            case 0:
            case 5:
                start = 0;
                end = 2;
                break;
            //From lavanthor
            case 1:
                start = 3;
                end = 5;
                break;
            // From Zuramat
            case 2:
                start = 6;
                end = 11;
                break;
            //From Moragg
            case 3:
                start = 12;
                end = 16;
                break;
            //From Erekem
            case 4:
                start = 17;
                end = 21;
                break;
            //From highest platform
            case 6:
                start = 22;
                end = 26;
                break;
        }
        uint8 wpId = 0;
        for(uint8 i = start; i <= end; ++i){
            debug_log("AddWP: %u", i);
            AddWaypoint(wpId, DragonsWP[i].x, DragonsWP[i].y, DragonsWP[i].z);
            wpId++;
        }

        WayPoint = WayPointList.begin();
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
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
        if(uiType != POINT_MOTION_TYPE || creatureEntry == NPC_GUARDIAN || creatureEntry == NPC_KEEPER)
            return;

        if(WayPoint->id != uiPointId)
            return;

        ++WayPoint;
        WalkTimer = 200;
    }
    void UpdateAI(const uint32 uiDiff){
        if(portalLoc != -1)
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
        if(pDoorSeal && !IsInCombat){
            if(m_creature->IsWithinDist(pDoorSeal, 27.0f, false))
            {
                IsWalking = false;
                WayPointList.clear();
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                DoCast(pDoorSeal, SPELL_CORRUPT);
                m_pInstance->SetData(TYPE_DOOR,SPECIAL);
            }
        }
        if(!IsWalking && !IsInCombat) {
                if (Unit* m_uEmbraceTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                m_creature->GetMotionMaster()->MoveChase(m_uEmbraceTarget);
                m_creature->SetInCombatWithZone();
                IsInCombat = true;
                }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

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
            case NPC_GUARDIAN:
                AzureGuardian_UpdateAI(uiDiff);
                break;
            case NPC_KEEPER:
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
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
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_BLAST : SPELL_ARCANE_BLAST_H);
            m_uiArcaneblast_Timer = 10100;
        }else m_uiArcaneblast_Timer -= uiDiff;

        //Slow
        if (m_uiSlow_Timer <= uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_SLOW);
            m_uiSlow_Timer = 15000;
        }else m_uiSlow_Timer -= uiDiff;
    }
};

/*######
## npc_violet_portal
######*/
struct MANGOS_DLL_DECL npc_violet_portalAI : public ScriptedAI
{
    npc_violet_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_uiGroupSpawned;
    uint8 portalType; // 0 = nothing, 1 = Guard & spawns, 2 = Group of elites
    uint32 portalID; // To identify portal...
    int8 portalLoc;

    uint32 TimeRiftWave_Timer;
    uint32 Check_Timer;

    void Reset()
    {
        m_uiGroupSpawned = false;
        portalType = 0;
        portalID = 0;
        portalLoc = -1;
        
        TimeRiftWave_Timer = 15000;
        Check_Timer = 5000;
    }
    uint32 SelectRandSummon()
    {
        uint32 entry = 0;
        if(portalType == 1)
        {
            switch (urand(0, 3))
            {
                case 0: entry = NPC_AZURE_BINDER; break;
                case 1: entry = NPC_AZURE_INVADER; break;
                case 2: entry = NPC_AZURE_MAGE_SLAYER; break;
                case 3: entry = NPC_AZURE_SPELLBREAKER; break;
            }      
        }else{
            switch (urand(0, 3))
            {
                case 0: entry = NPC_AZURE_CAPTAIN; break;
                case 1: entry = NPC_AZURE_RAIDER; break;
                case 2: entry = NPC_AZURE_SORCEROR; break;
                case 3: entry = NPC_AZURE_STALKER; break;
            }    
        }
        return entry;
    }
    void SpawnGroup()
    {
        if(portalType == 0)
            return;

        uint8 uiSpawnCount = (m_pInstance->GetData(TYPE_RIFT) < 12) ? 3 : 4;
        for(uint8 i = 0; i < uiSpawnCount; i++)
        {
            uint32 uiSpawnEntry = SelectRandSummon();
            if(Creature* pSummoned = m_creature->SummonCreature(uiSpawnEntry, m_creature->GetPositionX()-5+rand()%10, m_creature->GetPositionY()-5+rand()%10, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
            {
                debug_log("Spawn NPC %u, motherPortalID %u, portalLoc %u", uiSpawnEntry, portalID, portalLoc);
                ((mob_vh_dragonsAI*)pSummoned->AI())->motherPortalID = portalID;
                ((mob_vh_dragonsAI*)pSummoned->AI())->portalLoc = portalLoc;
            }
        }
    }
    bool IsThereNearElite(float range)
    {
        //Azure captain
        if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_CAPTAIN, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure raider
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_RAIDER, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure sorceror
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_SORCEROR, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure Stalker
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_STALKER, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        return false;
    }
    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance)
            return;

        if(m_pInstance->GetData(TYPE_EVENT) != IN_PROGRESS)
            return;

        switch(portalType)
        {
            case 0:
                return;
            case 1:
                if (TimeRiftWave_Timer < diff)
                {
                    debug_log("SpawnGroup()");
                    SpawnGroup();
                    TimeRiftWave_Timer = 15000;
                }else TimeRiftWave_Timer -= diff;

                if (!m_creature->IsNonMeleeSpellCasted(false))
                {
                    m_uiNextPortal_Timer = 5000;
                    debug_log("SD2: npc_time_rift: not casting anylonger, i need to die.");
                    m_creature->setDeathState(JUST_DIED);
                }
                break;
            case 2:
                if(!m_uiGroupSpawned)
                {
                    SpawnGroup();
                    m_uiGroupSpawned = true;
                }
                if (Check_Timer < diff)
                {
                    if(!IsThereNearElite(150.0f))
                    {
                        m_uiNextPortal_Timer = 5000;
                        debug_log("SD2: npc_time_rift: No elite, i need to die.");
                        m_creature->setDeathState(JUST_DIED);
                    }
                    Check_Timer = 1000;
                }else Check_Timer -= diff;
                break;
        }
    }
};
/*######
## npc_sinclari
######*/
struct MANGOS_DLL_DECL npc_sinclariAI : public ScriptedAI
{
    npc_sinclariAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->SetStandFlags(UNIT_STAND_STATE_STAND);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint8 m_uiRiftPortalCount;
    uint8 m_bIsRegular;
    uint32 m_uiBossCheck_Timer;
    uint32 m_uiPortalCheck_Timer;

    void Reset()
    {
        m_uiRiftPortalCount = 0;
        m_uiNextPortal_Timer = 0;
        m_uiBossCheck_Timer = 0;
        m_uiPortalCheck_Timer = 1000;
        m_bIsRegular = m_creature->GetMap()->IsRegularDifficulty();
    }

    void SetEvent()
    {
        //m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        m_uiNextPortal_Timer = 5000;
        m_creature->GetMotionMaster()->MovePoint(0, 1815.571f, 800.112f, 44.364f);
        if (m_pInstance){
            m_pInstance->SetData(TYPE_EVENT, IN_PROGRESS);
            m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(DATA_SEAL_DOOR));
        }
    }

    void DoSpawnPortal()
    {
        int tmp = urand(1, 6);
        if (Creature* pTemp = m_creature->SummonCreature(NPC_PORTAL, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            //set portal type
            uint8 portalType = rand()%2+1;
            uint32 portalID = rand()%50000;
            ((npc_violet_portalAI*)pTemp->AI())->portalType = portalType; 
            ((npc_violet_portalAI*)pTemp->AI())->portalID = portalID;
            ((npc_violet_portalAI*)pTemp->AI())->portalLoc = tmp;
            
            if(portalType == 1)
            {
                uint32 entry = urand(0, 1) ? NPC_GUARDIAN : NPC_KEEPER;
                if (Creature* pSummoned = pTemp->SummonCreature(entry, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, pTemp->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000))
                {
                    pSummoned->AddThreat(pTemp);
                    pTemp->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL,false);
                    ((mob_vh_dragonsAI*)pSummoned->AI())->motherPortalID = portalID;
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_EVENT) != IN_PROGRESS)
            return;

        if (m_uiNextPortal_Timer && m_pInstance->GetData(TYPE_RIFT) != DONE)
        {
            if (m_uiNextPortal_Timer <= uiDiff)
            {
                ++m_uiRiftPortalCount;
                if (m_pInstance && m_uiRiftPortalCount <= 18)
                {
                    m_pInstance->DoUpdateWorldState(WORLD_STATE_VH_PORTALS, m_uiRiftPortalCount);
                    m_pInstance->SetData(TYPE_RIFT, SPECIAL);
                }
                /* this is all bosses version -> switched to blizzlike version
                if ( m_uiRiftPortalCount != 3
                    && m_uiRiftPortalCount != 6
                    && m_uiRiftPortalCount != 9 
                    && m_uiRiftPortalCount != 12 
                    && m_uiRiftPortalCount != 15 
                    && m_uiRiftPortalCount != 18
                    && m_uiRiftPortalCount < 19 )
                    */
                if (m_uiRiftPortalCount != 6 && m_uiRiftPortalCount != 12 && m_uiRiftPortalCount != 18 && m_uiRiftPortalCount < 19)
                {
                    DoSpawnPortal();
                    if (m_uiRiftPortalCount < 12)
                        m_uiNextPortal_Timer = 120000;
                    else
                        m_uiNextPortal_Timer = 90000;
                }
                /*
                else if ( m_uiRiftPortalCount == 3 
                         || m_uiRiftPortalCount == 6
                         || m_uiRiftPortalCount == 9
                         || m_uiRiftPortalCount == 12
                         || m_uiRiftPortalCount == 15
                         || m_uiRiftPortalCount == 18 )
                         */
                else if (m_uiRiftPortalCount == 6 || m_uiRiftPortalCount == 12)
                {
                    if (Creature* pTemp = m_creature->SummonCreature(NPC_PORTAL, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_TIMED_DESPAWN, 1500))
                    {
                        Creature* pSummoned = m_creature->SummonCreature(NPC_AZURE_SABOTEUR, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000);

                        pSummoned->AddThreat(pTemp);
                        pTemp->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
                    }
                    m_pInstance->SetData(TYPE_RIFT, IN_PROGRESS);
                    m_uiBossCheck_Timer = 1000;
                    m_uiNextPortal_Timer =  m_bIsRegular ? 180000 : 120000;
                }
                else if (m_uiRiftPortalCount == 18)
                {
                    if(Creature *Cyanigosa = GetClosestCreatureWithEntry(m_creature, NPC_CYANIGOSA, 100.0f))
                    //if(Creature *Cyanigosa = m_creature->SummonCreature(NPC_CYANIGOSA, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 180000))
                    {
                        DoScriptText(SAY_CYANIGOSA_SPAWN, Cyanigosa);
                        Cyanigosa->setFaction(14);
                        Cyanigosa->SetVisibility(VISIBILITY_ON);
                    }
                    if (m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_RIFT, DONE);
                        m_pInstance->SetData(TYPE_DISRUPTIONS, 20);
                        m_creature->ForcedDespawn();
                    }
                    m_uiNextPortal_Timer = 0;
                }
            }
            else
                m_uiNextPortal_Timer -= uiDiff;

            return;
        }

        if (m_uiBossCheck_Timer)
        {
            if (m_uiBossCheck_Timer <= uiDiff)
            {
                if (!m_pInstance->GetData(DATA_BOSSTIME))
                    m_uiNextPortal_Timer = 10000;
                m_uiBossCheck_Timer = 1000;
            }
            else
                m_uiBossCheck_Timer -= uiDiff;

            return;
        }
    }
};

#define GOSSIP_ITEM_INTRO   "Activate the crystals when we get in trouble, right."
#define GOSSIP_ITEM_START   "Get your people to safety, we'll keep the Blue Dragonflight's forces at bay."
#define GOSSIP_ITEM_TELE_IN "I need to go in!"

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu( pCreature->GetGUID() );

    if(m_pInstance->GetData(TYPE_EVENT) == NOT_STARTED){
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }else{
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_IN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_sinclari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            ((npc_sinclariAI*)pCreature->AI())->SetEvent();
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_INSIDE, false);
    }
    return true;
}
/*######
## npc_door_seal_vh
######*/
struct MANGOS_DLL_DECL npc_door_sealAI : public ScriptedAI
{
    npc_door_sealAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();   
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 CheckTimer;
    uint32 SpellCorrupt_Timer;
    uint8 lastPortal;

    void Reset()
    {
        CheckTimer = 0;
        SpellCorrupt_Timer = 0;
        lastPortal = 0;
}
    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (SpellCorrupt_Timer)
            return;

        if (spell->Id == SPELL_CORRUPT) {
            SpellCorrupt_Timer = 1000;
            }
    }
    void JustDied(Unit* pKiller)
    {
        m_creature->Respawn();
    }

    void UpdateAI(const uint32 diff){
        if (SpellCorrupt_Timer)
        {
            if (SpellCorrupt_Timer <= diff)
            {
                if (m_creature->HasAura(SPELL_CORRUPT,EFFECT_INDEX_0))
                    SpellCorrupt_Timer = 1500;
                else
                    SpellCorrupt_Timer = 0;
            }else SpellCorrupt_Timer -= diff;
        }
    }
};

/*######
## npc_azure_saboteur
######*/
struct MANGOS_DLL_DECL npc_azure_saboteurAI : public ScriptedAI
{
    npc_azure_saboteurAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

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
//        m_bIsRegular = m_creature->GetMap()->IsRegularDifficulty();

        if (m_pInstance)
        {
            m_uiBossID = m_pInstance->GetData(TYPE_LASTBOSS);
            m_uiDisruptionsCount = m_pInstance->GetData(TYPE_DISRUPTIONS);
            switch (m_uiBossID)
            {
                case 6: // Lavanthor
                    m_uiBossType = TYPE_LAVANTHOR;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_LAVANTHOR);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_LAVANTHOR_DOOR);
                    break;
                case 7: // Zuramat
                    m_uiBossType = TYPE_ZURAMAT;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_ZURAMAT);
                    if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ZURAMAT))))
                        DoScriptText(SAY_ZURAMAT_SPAWN, pTemp);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_ZURAMAT_DOOR);
                    break;
                case 3: // Moragg
                    m_uiBossType = TYPE_MORAGG;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_MORAGG);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_MORAGG_DOOR);
                    break;
                case 2: // Erekem
                    m_uiBossType = TYPE_EREKEM;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_EREKEM);
                    if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_EREKEM))))
                        DoScriptText(SAY_EREKEM_SPAWN, pTemp);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_EREKEM_DOOR);
                    break;
                case 4: // Ichoron
                    m_uiBossType = TYPE_ICHORON;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_ICHORON);
                    if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ICHORON))))
                        DoScriptText(SAY_ICHORON_SPAWN, pTemp);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_ICHORON_DOOR);
                    break;
                case 5: // Xevozz
                    m_uiBossType = TYPE_XEVOZZ;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_XEVOZZ);
                    if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_XEVOZZ))))
                        DoScriptText(SAY_XEVOZZ_SPAWN, pTemp);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_XEVOZZ_DOOR);
                    break;
                case 0: // No boss
                    m_uiBossType = 0;
                    break;
            }
            m_pInstance->SetData(TYPE_LASTBOSS_ID, m_uiBossType);
            if (m_uiBossType != 0)  m_creature->GetMotionMaster()->MovePoint(0, BossLoc[m_uiBossID].x,  BossLoc[m_uiBossID].y,  BossLoc[m_uiBossID].z);
            else  m_creature->GetMotionMaster()->MovePoint(0, 1827.960f, 804.208f, 44.364f);
        }
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
        if (m_bIsActiving)
            if (m_uiDisruption_Timer < uiDiff)
            {
                if (m_uiDisruptionCounter < 3) {
                    DoCast(m_creature, SPELL_SHIELD_DISRUPTION);
                ++m_uiDisruptionsCount;
                m_pInstance->SetData(TYPE_DISRUPTIONS, m_uiDisruptionsCount);}
                else if (m_uiDisruptionCounter == 3)
                {
                    m_pInstance->DoUseDoorOrButton(m_uiDoorGUID);
                    if (m_uiBossType == TYPE_EREKEM) {
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(DATA_EREKEM_DOOR_L));
                        m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(DATA_EREKEM_DOOR_R));
                        }
                }
                else {
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    if (m_pInstance->GetData(TYPE_LASTBOSS_ID) != 0) m_pInstance->SetData(m_pInstance->GetData(TYPE_LASTBOSS_ID), SPECIAL);
                    m_bIsActiving = false;
                    }

                ++m_uiDisruptionCounter;
                m_uiDisruption_Timer = 1000;
            }
            else m_uiDisruption_Timer -= uiDiff;
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
CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI (pCreature);
}
CreatureAI* GetAI_npc_violet_portal(Creature* pCreature)
{
    return new npc_violet_portalAI (pCreature);
}
CreatureAI* GetAI_npc_door_seal(Creature* pCreature)
{
    return new npc_door_sealAI(pCreature);
}

void AddSC_violet_hold()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_sinclari";
    newscript->GetAI = &GetAI_npc_sinclari;
    newscript->pGossipHello =  &GossipHello_npc_sinclari;
    newscript->pGossipSelect = &GossipSelect_npc_sinclari;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_violet_portal";
    newscript->GetAI = &GetAI_npc_violet_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_door_seal_vh";
    newscript->GetAI = &GetAI_npc_door_seal;
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
