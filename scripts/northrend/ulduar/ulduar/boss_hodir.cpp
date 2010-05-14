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
SDName: boss_hodir
SD%Complete: 60%
SDComment:Auras needs core fix, implement flash freeze and icicles, snowdrift
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SPELL_ENRAGE            = 26662,

    SPELL_FROZEN_BLOWS      = 62478,
    SPELL_FROZEN_BLOWS_H    = 63512,
    SPELL_FREEZE            = 62469,
    SPELL_BITTER_COLD_AURA  = 48094,//62039  // or maybe 62038? -> broken needs more core support -> changed to Intense cold from Keristrasza
    SPELL_ICICLE            = 62460,
    SPELL_ICE_SHARDS        = 65370,
    SPELL_SNOWDRIFT         = 62463,
    SPELL_FLASH_FREEZE      = 61968,
    SPELL_ROOT              = 43956,
    SPELL_ROOT_AURA         = 40885,

    // not used
    SPELL_FROZEN_BLOWS_HIT  = 62867,
    SPELL_FROZEN_BLOWS_AURA = 64544,
    SPELL_FROZEN_BLOWS_HIT_H    = 63511,
    SPELL_FROZEN_BLOWS_AURA_H   = 64545,

    NPC_SNOWDRIFT           = 33174,
    GO_SNOWDRIFT            = 194173,
    NPC_FLASH_FREEZE        = 32938,        //32926

    SAY_AGGRO               = -1603085,
    SAY_DEATH               = -1603084,
    SAY_SLAY01              = -1603083,
    SAY_SLAY02              = -1603082,
    SAY_FLASH_FREEZE        = -1603081,
    SAY_FROZEN_BLOWS        = -1603080,
    SAY_BERSERK             = -1603087,
    SOUND_FROZEN_BLOWS      = 15556,

    ACHIEV_RARE_CACHE       = 3182,
    ACHIEV_RARE_CACHE_H     = 3184,
    ACHIEV_COOLEST_FRIEND   = 2963,
    ACHIEV_COOLEST_FRIEND_H = 2965,


    // helper npcs
    // druid
    SPELL_WRATH         = 62793,
    SPELL_STARLIGHT     = 62807, // friendly

    // shaman
    SPELL_LAVA_BURST    = 61924,
    SPELL_STORM_CLOUD   = 65123, // friendly
    SPELL_STORM_CLOUD_H = 65133,
    SPELL_STORM_POWER   = 65134, // friendly

    // mage
    SPELL_FIREBALL      = 61909,
    SPELL_CONJURE_TOASTY_FIRE   = 62823, // friendly
    SPELL_MELT_ICE      = 64528,
    SPELL_SIGNED        = 65280,

    // priest
    SPELL_SMITE         = 61923,
    SPELL_GREAT_HEAL    = 62809,    //friendly
    SPELL_DISPEL_MAGIC  = 63499,    //friendly

};

bool m_bCoolestFriend;

struct MANGOS_DLL_DECL mob_snowdriftAI : public ScriptedAI
{
    mob_snowdriftAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    void SetVictim(Unit* pVictim)
    {
        if (pVictim)
        {
            m_uiVictimGUID = pVictim->GetGUID();
            pVictim->CastSpell(pVictim, SPELL_SNOWDRIFT, true);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_SNOWDRIFT);
            }
        }
    }

    void UpdateAI(const uint32 diff) {}
};

struct MANGOS_DLL_DECL mob_flashFreezeAI : public ScriptedAI
{
    mob_flashFreezeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        SetCombatMovement(false);
        Reset();
    }

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    void SetVictim(Unit* pVictim)
    {
        if (pVictim)
        {
            m_uiVictimGUID = pVictim->GetGUID();
            pVictim->CastSpell(pVictim, SPELL_ROOT_AURA, true);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_ROOT_AURA);
            }
        }
    }

    void UpdateAI(const uint32 diff) {}
};

struct MANGOS_DLL_DECL boss_hodirAI : public ScriptedAI
{
    boss_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    // hard mode timer
    uint64 SpeedKillTimer;

    uint64 EnrageTimer;
    uint64 FlashFreezeTimer;
    uint64 FrozenBlowsTimer;
    uint32 FlashFreezeVisualTimer;
    uint64 FreezeTimer;
    uint32 IcicleTimer;
    uint8 IcicleCount;
    uint32 AuraCheckTimer;

    bool isOutro;
    uint32 OutroTimer;
    uint32 Step;
    std::vector<Unit*> frozenTargets;
    std::vector<Unit*> snowdriftTargets;
    std::vector<Unit*> flashFreezeTargets;

    std::list<Creature*> lFriends;

    void Reset()
    {
        SpeedKillTimer = 0;
        m_bCoolestFriend = true;

        EnrageTimer = 480000;
        FlashFreezeTimer = 70000;
        FrozenBlowsTimer = 60000;
        FlashFreezeVisualTimer = 60000;
        FreezeTimer = urand(15000, 20000);
        IcicleTimer = 10000;
        IcicleCount = 0;
        frozenTargets.clear();
        snowdriftTargets.clear();
        flashFreezeTargets.clear();

        AuraCheckTimer = 10000;
        OutroTimer = 10000;
        Step = 1;

        isOutro = false;

        if(m_pInstance) 
            m_pInstance->SetData(TYPE_HODIR, NOT_STARTED);

        // respawn friendly npcs
        // druids
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33325, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32901, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32941, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33333, DEFAULT_VISIBILITY_INSTANCE);
        // shamys
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33328, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32900, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33332, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32950, DEFAULT_VISIBILITY_INSTANCE);
        // mages
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32893, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33327, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33331, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32946, DEFAULT_VISIBILITY_INSTANCE);
        // priests
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32897, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33326, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 32948, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lFriends, m_creature, 33330, DEFAULT_VISIBILITY_INSTANCE);
        if (!lFriends.empty())
        {
            for(std::list<Creature*>::iterator iter = lFriends.begin(); iter != lFriends.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->Respawn();
            }
        }
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_HODIR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        m_creature->CastSpell(who, SPELL_BITTER_COLD_AURA, true);
    }

    void DoOutro()
    {
        if(m_pInstance) 
        {
            if(SpeedKillTimer < 180000)
            {
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
                m_pInstance->SetData(TYPE_HODIR, DONE);
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_RARE_CACHE : ACHIEV_RARE_CACHE_H);
            }
            else
                m_pInstance->SetData(TYPE_HODIR, DONE);

            if (m_bCoolestFriend)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_COOLEST_FRIEND : ACHIEV_COOLEST_FRIEND_H);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
       if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_HODIR, DONE);
            if(SpeedKillTimer > 0)
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            isOutro = true;
        }
    }

    void KilledUnit(Unit *who)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY01, m_creature);
        else
            DoScriptText(SAY_SLAY02, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!isOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // hard mode check
            SpeedKillTimer += uiDiff;

            // Flash freeze visual
            if(FlashFreezeVisualTimer < uiDiff)
            {
                DoScriptText(SAY_FLASH_FREEZE, m_creature);
                DoCast(m_creature->getVictim(), SPELL_FLASH_FREEZE);
                FlashFreezeVisualTimer = 40000;
                FlashFreezeTimer = 5000;
            }
            else FlashFreezeVisualTimer -= uiDiff;

            // flash freeze -> workaround
            if(FlashFreezeTimer < uiDiff)
            {    
                /*
                // kill targets which are frozen
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                {
                    Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                    {
                        if (pUnit->HasAura(SPELL_FREEZE))
                        {
                            for(std::vector<Unit*>::iterator itr = frozenTargets.begin(); itr!= frozenTargets.end(); ++itr)
                            {
                                if (*itr)
                                {
                                    if (pUnit->GetDistance2d(*itr) <= 7 && (*itr)->HasAura(SPELL_FREEZE))
                                        m_creature->DealDamage(pUnit, pUnit->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FROST, NULL, false);
                                }
                            }
                        }
                    }
                }

                // apply imune to snowdrifted targets
                for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                {
                    Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                    {
                        if (pUnit->HasAura(SPELL_SNOWDRIFT))
                        {
                            for(std::vector<Unit*>::iterator itr = frozenTargets.begin(); itr!= frozenTargets.end(); ++itr)
                            {
                                if (*itr)
                                {
                                    if (pUnit->GetDistance2d(*itr) <= 7 && (*itr)->HasAura(SPELL_SNOWDRIFT))
                                        pUnit->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                }
                            }
                        }
                    }
                }

                // cast flash freeze
                for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                {
                    Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                    {
                        if (!pUnit->HasAura(SPELL_SNOWDRIFT) && !pUnit->HasAura(SPELL_FREEZE))
                        {
                            for(std::vector<Unit*>::iterator itr = flashFreezeTargets.begin(); itr!= flashFreezeTargets.end(); ++itr)
                            {
                                if (*itr)
                                {
                                    float fPosX, fPosY, fPosZ;
                                    pUnit->GetPosition(fPosX, fPosY, fPosZ);

                                    if (Creature* pFlashFreeze = m_creature->SummonCreature(NPC_FLASH_FREEZE, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
                                    {
                                        pFlashFreeze->AddThreat(pUnit);
                                    }
                                }
                            }
                        }
                    }
                }

                FlashFreezeTimer = 40000;
                frozenTargets.clear();
                snowdriftTargets.clear();
                flashFreezeTargets.clear();
                */
            }
            else FlashFreezeTimer -= uiDiff;

            // icicles
            if(IcicleTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoCast(pTarget, SPELL_ICICLE);
                    DoCast(pTarget, SPELL_ICE_SHARDS); // not working
                    //DoCast(pTarget, SPELL_SNOWDRIFT);   //not working -> workaround
                    m_creature->SummonCreature(NPC_SNOWDRIFT, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 15000);

                    // two icicles at 5 secs
                    if(IcicleCount < 1)
                    {
                        IcicleTimer = 3000;
                        IcicleCount ++;
                    }
                    else
                    {
                        IcicleTimer = 10000;
                        IcicleCount = 0;
                    }
                }
            }
            else IcicleTimer -= uiDiff;

            // frozen blows
            if(FrozenBlowsTimer < uiDiff)
            {
                DoScriptText(SAY_FROZEN_BLOWS, m_creature);
                DoCast(m_creature, m_bIsRegularMode ? SPELL_FROZEN_BLOWS : SPELL_FROZEN_BLOWS_H);
                FrozenBlowsTimer = 60000;
            }
            else FrozenBlowsTimer -= uiDiff;

            // freeze
            if(FreezeTimer < uiDiff)
            {
                Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if(target) 
                {
                    DoCast(target, SPELL_FREEZE);
                    frozenTargets.push_back(target);
                }
                FreezeTimer = urand(5000, 10000);
            }
            else FreezeTimer -= uiDiff;

            // enrage
            if(EnrageTimer < uiDiff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_ENRAGE);
                EnrageTimer = 30000;
            }
            else EnrageTimer -= uiDiff;

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
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovePoint(0, 1984.64f, -206.37f, 432.68f);
                ++Step;
                OutroTimer = 10000;
                break;
            case 3:
                m_creature->SetOrientation(4.99f);
                DoScriptText(SAY_DEATH, m_creature);
                ++Step;
                OutroTimer = 5000;
                break;
            case 5:
                DoOutro();
                ++Step;
                OutroTimer = 10000;
                break;
            }
        }
        else return;

        if (OutroTimer <= uiDiff)
        {
            ++Step;
            OutroTimer = 330000;
        } OutroTimer -= uiDiff;


    }
};

struct MANGOS_DLL_DECL npc_hodir_druidAI : public ScriptedAI
{
    npc_hodir_druidAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void Aggro(Unit* pWho)
    {
        DoStartMovement(pWho, 20.0f);
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return NULL;
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
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
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 50.0f))
                    DoCast(pHodir, SPELL_WRATH);
                break;
            case 1:
                Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, SPELL_STARLIGHT);
                break;
            }
            spellTimer = 5000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_hodir_druid(Creature* pCreature)
{
    return new npc_hodir_druidAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_shamanAI : public ScriptedAI
{
    npc_hodir_shamanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void Aggro(Unit* pWho)
    {
        DoStartMovement(pWho, 20.0f);
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return NULL;
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
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
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 50.0f))
                    DoCast(pHodir, SPELL_LAVA_BURST);
                break;
            case 1:
                Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, m_bIsRegularMode ? SPELL_STORM_CLOUD : SPELL_STORM_CLOUD_H);
                break;
            }
            spellTimer = 5000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_hodir_shaman(Creature* pCreature)
{
    return new npc_hodir_shamanAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_mageAI : public ScriptedAI
{
    npc_hodir_mageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void Aggro(Unit* pWho)
    {
        DoStartMovement(pWho, 20.0f);
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return NULL;
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 50.0f))
                    DoCast(pHodir, SPELL_FIREBALL);
                break;
            case 1:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 50.0f))
                    DoCast(pHodir, SPELL_SIGNED);
                break;
            case 2:
                if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_FLASH_FREEZE, 50.0f))
                    DoCast(pTemp, SPELL_MELT_ICE);
                break;
            case 3:
                Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer()));
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, SPELL_CONJURE_TOASTY_FIRE);
                break;
            }
            spellTimer = 5000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_hodir_mage(Creature* pCreature)
{
    return new npc_hodir_mageAI(pCreature);
}

struct MANGOS_DLL_DECL npc_hodir_priestAI : public ScriptedAI
{
    npc_hodir_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 spellTimer;
    std::list<uint64> FriendlyList;

    void Reset()
    {
        spellTimer = 5000;
        FriendlyList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        m_bCoolestFriend = false;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // friendly list
        if (!m_creature->IsHostileTo(pWho) && !ListContains(FriendlyList, pWho->GetGUID()) && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 100, true))
            FriendlyList.push_back(pWho->GetGUID());
    }

    void Aggro(Unit* pWho)
    {
        DoStartMovement(pWho, 20.0f);
    }

    uint64 SelectRandomPlayer()
    {
        //This should not appear!
        if (FriendlyList.empty()){
            spellTimer = 5000;
            return NULL;
        }

        std::list<uint64>::iterator iter = FriendlyList.begin();
        advance(iter, urand(0, FriendlyList.size()-1));

        return *iter;
    }

    bool ListContains(std::list<uint64> &plist, uint64 element)
    {
        if (plist.empty())
            return false;

        std::list<uint64>::iterator i;
        for (i = plist.begin(); i!=plist.end(); ++i)
        {
            if ((*i) == element)
                return true;
        }
        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            Unit *pTemp = Unit::GetUnit((*m_creature),(SelectRandomPlayer()));

            switch(urand(0, 2))
            {
            case 0:
                if(Creature *pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 50.0f))
                    DoCast(pHodir, SPELL_SMITE);
                break;
            case 1:
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40)
                    DoCast(pTemp, SPELL_GREAT_HEAL);
                break;
            case 2:
                if (pTemp && pTemp->isAlive() && m_creature->GetDistance(pTemp) < 40 && pTemp->HasAura(SPELL_FREEZE, EFFECT_INDEX_0))
                    DoCast(pTemp, SPELL_DISPEL_MAGIC);
                break;
            }
            spellTimer = 5000;
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_hodir_priest(Creature* pCreature)
{
    return new npc_hodir_priestAI(pCreature);
}

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

CreatureAI* GetAI_mob_snowdrift(Creature* pCreature)
{
    return new mob_snowdriftAI(pCreature);
}

CreatureAI* GetAI_mob_flashFreeze(Creature* pCreature)
{
    return new mob_flashFreezeAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_snowdrift";
    newscript->GetAI = &GetAI_mob_snowdrift;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flashFreeze";
    newscript->GetAI = &GetAI_mob_flashFreeze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_priest";
    newscript->GetAI = &GetAI_npc_hodir_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_mage";
    newscript->GetAI = &GetAI_npc_hodir_mage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_druid";
    newscript->GetAI = &GetAI_npc_hodir_druid;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_shaman";
    newscript->GetAI = &GetAI_npc_hodir_shaman;
    newscript->RegisterSelf();
}