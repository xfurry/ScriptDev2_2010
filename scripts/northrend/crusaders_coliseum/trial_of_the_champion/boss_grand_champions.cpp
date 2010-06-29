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
SDName: boss_grand_champions
SD%Complete: 92%
SDComment: missing yells. hunter AI sucks. no pvp diminuishing returns
SDCategory: Trial Of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum Spells
{
    //warrior
    SPELL_MORTAL_STRIKE         = 68783,
    SPELL_MORTAL_STRIKE_H       = 68784,
    SPELL_BLADESTORM            = 63784,
    SPELL_INTERCEPT             = 67540,
    SPELL_ROLLING_THROW         = 67546,
    //mage
    SPELL_FIREBALL              = 66042,
    SPELL_FIREBALL_H            = 68310,
    SPELL_BLAST_WAVE            = 66044,
    SPELL_BLAST_WAVE_H          = 68312,
    SPELL_HASTE                 = 66045,
    SPELL_POLYMORPH             = 66043,
    SPELL_POLYMORPH_H           = 68311,
    //shaman
    SPELL_CHAIN_LIGHTNING       = 67529,
    SPELL_CHAIN_LIGHTNING_H     = 68319,
    SPELL_EARTH_SHIELD          = 67530,
    SPELL_HEALING_WAVE          = 67528,
    SPELL_HEALING_WAVE_H        = 68318,
    SPELL_HEX_OF_MENDING        = 67534,
    //hunter
    SPELL_DISENGAGE             = 68340,
    SPELL_LIGHTNING_ARROWS      = 66083,
    SPELL_MULTI_SHOT            = 66081,
    SPELL_SHOOT                 = 66079,
    //rogue
    SPELL_DEADLY_POISON         = 67710,
    SPELL_DEADLY_POISON_H       = 68315,
    SPELL_EVISCERATE            = 67709,
    SPELL_EVISCERATE_H          = 68317,
    SPELL_FAN_OF_KNIVES         = 67706,
    SPELL_POISON_BOTTLE         = 67701
};

enum AI
{
    AI_MELEE    = 0,
    AI_RANGED   = 1,
};

// common parts for all champions
struct MANGOS_DLL_DECL toc5_champion_baseAI: public ScriptedAI
{
    toc5_champion_baseAI(Creature* pCreature, uint32 aitype): ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        mAIType = aitype;
        Init();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 mAIType;
    uint32 m_uiThreatTimer;
    uint32 TeamInInstance;

    void Init()
    {
        m_uiThreatTimer = 15000;
        TeamInInstance = GetFaction();
        m_creature->SetRespawnDelay(DAY);
    }

    uint32 GetFaction()
    {
        uint32 faction = 0;
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    faction = pPlayer->GetTeam();
            }
        }
        return faction;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if(mAIType==AI_MELEE)
                DoStartMovement(pWho);
            else
                DoStartMovement(pWho, 20.0f);
        }
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_GRAND_CHAMPIONS) != IN_PROGRESS) 
            m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, IN_PROGRESS);

        // call for help
        if(TeamInInstance == HORDE)
        {
            m_pInstance->SetData(TYPE_STAGE, 1);

            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JACOB)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_AMBROSE)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_COLOSOS)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JAELYNE)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LANA)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
        }
        else if(TeamInInstance == ALLIANCE)
        {
            m_pInstance->SetData(TYPE_STAGE, 1);

            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_MOKRA)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ERESSEA)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_RUNOK)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ZULTORE)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_VISCERI)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {  
            // do this only for the first who reaches home
            if(m_pInstance->GetData(TYPE_GRAND_CHAMPIONS) != NOT_STARTED)
            {
                // rewpawn dead champs
                if(TeamInInstance == HORDE)
                {
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JACOB)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_AMBROSE)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_COLOSOS)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JAELYNE)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LANA)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                }
                else if(TeamInInstance == ALLIANCE)
                {
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_MOKRA)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ERESSEA)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_RUNOK)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ZULTORE)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_VISCERI)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                }

                m_pInstance->SetData(TYPE_STAGE, 0);
                m_pInstance->SetData(TYPE_GRAND_CHAMPIONS, NOT_STARTED);
            }
        }  
    }

    void UpdatePower()
    {
        if(m_creature->getPowerType() == POWER_MANA)
            m_creature->ModifyPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA) / 3);
    }

    float CalculateThreat(float distance, float armor, uint32 health)
    {
        float dist_mod = (mAIType == AI_MELEE) ? 15.0f/(15.0f + distance) : 1.0f;
        float armor_mod = (mAIType == AI_MELEE) ? armor / 16635.0f : 0.0f;
        float eh = (health+1) * (1.0f + armor_mod);
        return dist_mod * 30000.0f / eh;
    }

    void UpdateThreat()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator itr;
        bool empty = true;
        for(itr = tList.begin(); itr!=tList.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if (pUnit && m_creature->getThreatManager().getThreat(pUnit))
            {
                if(pUnit->GetTypeId()==TYPEID_PLAYER)
                {
                    float threat = CalculateThreat(m_creature->GetDistance2d(pUnit), (float)pUnit->GetArmor(), pUnit->GetHealth());
                    m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
                    m_creature->AddThreat(pUnit, 1000000.0f * threat);
                    empty = false;
                }
            }
        }
        //if(empty) 
            //EnterEvadeMode();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiThreatTimer < uiDiff)
        {
            UpdatePower();
            UpdateThreat();
            m_uiThreatTimer = 4000;
        }
        else m_uiThreatTimer -= uiDiff;
    }
};

// Warrior
struct MANGOS_DLL_DECL mob_toc5_warriorAI: public toc5_champion_baseAI
{
    mob_toc5_warriorAI(Creature* pCreature): toc5_champion_baseAI(pCreature, AI_MELEE)
    {
        Reset();
    }

    uint32 m_uiMortal_Strike_Timer;
    uint32 m_uiBladestorm_Timer;
    uint32 m_uiRolling_Throw_Timer;
    uint32 m_uiIntercept_Cooldown;

    void Reset()
    {
        m_uiMortal_Strike_Timer = 6000;
        m_uiBladestorm_Timer    = 20000;
        m_uiRolling_Throw_Timer = 30000;
        m_uiIntercept_Cooldown  = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortal_Strike_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MORTAL_STRIKE : SPELL_MORTAL_STRIKE_H);
            m_uiMortal_Strike_Timer = 6000;
        }
        else
            m_uiMortal_Strike_Timer -= uiDiff;  

        if (m_uiRolling_Throw_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_ROLLING_THROW);
            m_uiRolling_Throw_Timer = 30000;
        }
        else
            m_uiRolling_Throw_Timer -= uiDiff;

        if (m_uiBladestorm_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BLADESTORM);
            m_uiBladestorm_Timer = 90000;
        }
        else
            m_uiBladestorm_Timer -= uiDiff;

        if (m_uiIntercept_Cooldown < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(m_creature->GetDistance2d(pTarget) > 10.0f)
                {
                    DoCast(pTarget, SPELL_INTERCEPT);
                    m_uiIntercept_Cooldown = 15000;
                }
                else
                    m_uiIntercept_Cooldown = 1000;
            }
        }
        else 
            m_uiIntercept_Cooldown -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Mage
struct MANGOS_DLL_DECL mob_toc5_mageAI: public toc5_champion_baseAI
{
    mob_toc5_mageAI(Creature* pCreature): toc5_champion_baseAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiFireball_Timer;
    uint32 m_uiBlast_Wave_Timer;
    uint32 m_uiHaste_Timer;
    uint32 m_uiPolymorph_Timer;

    void Reset()
    {
        m_uiFireball_Timer      = 1000;
        m_uiBlast_Wave_Timer    = 20000;
        m_uiHaste_Timer         = 9000;
        m_uiPolymorph_Timer     = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFireball_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
            m_uiFireball_Timer = 3000;
        }
        else
            m_uiFireball_Timer -= uiDiff;  

        if (m_uiBlast_Wave_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_BLAST_WAVE : SPELL_BLAST_WAVE_H);
            m_uiBlast_Wave_Timer = 20000;
        }
        else
            m_uiBlast_Wave_Timer -= uiDiff;

        if (m_uiHaste_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_HASTE);
            m_uiHaste_Timer = 10000;
        }
        else
            m_uiHaste_Timer -= uiDiff;

        if (m_uiPolymorph_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_POLYMORPH : SPELL_POLYMORPH_H);
            m_uiPolymorph_Timer = 15000;
        }
        else
            m_uiPolymorph_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Shaman
struct MANGOS_DLL_DECL mob_toc5_shamanAI: public toc5_champion_baseAI
{
    mob_toc5_shamanAI(Creature* pCreature): toc5_champion_baseAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiChain_Lightning_Timer;
    uint32 m_uiEarth_Shield_Timer;
    uint32 m_uiHealing_Wave_Timer;
    uint32 m_uiHex_Timer;

    void Reset()
    {
        m_uiChain_Lightning_Timer   = 1000;
        m_uiEarth_Shield_Timer      = 5000;
        m_uiHealing_Wave_Timer      = 13000;
        m_uiHex_Timer               = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiChain_Lightning_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
            m_uiChain_Lightning_Timer = urand(5000, 10000);
        }
        else
            m_uiChain_Lightning_Timer -= uiDiff;  

        if (m_uiHex_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_HEX_OF_MENDING);
            m_uiHex_Timer = 20000;
        }
        else
            m_uiHex_Timer -= uiDiff;

        if (m_uiHealing_Wave_Timer < uiDiff)
        {
            switch(urand(0, 2))
            {
            case 0:
            case 1:
                if(Unit* pTarget = DoSelectLowestHpFriendly(80.0f))
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_HEALING_WAVE : SPELL_HEALING_WAVE_H);
                break;
            case 2:
                DoCast(m_creature, m_bIsRegularMode ? SPELL_HEALING_WAVE : SPELL_HEALING_WAVE_H);
                break;
            }
            m_uiHealing_Wave_Timer = 8000;
        }
        else
            m_uiHealing_Wave_Timer -= uiDiff;

        if (m_uiEarth_Shield_Timer < uiDiff)
        {
            switch(urand(0, 2))
            {
            case 0:
            case 1:
                if(Unit* pTarget = DoSelectLowestHpFriendly(80.0f))
                    DoCast(pTarget, SPELL_EARTH_SHIELD);
            case 2:
                DoCast(m_creature, SPELL_EARTH_SHIELD);
                break;
            }
            m_uiEarth_Shield_Timer = 25000;
        }
        else
            m_uiEarth_Shield_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Hunter
struct MANGOS_DLL_DECL mob_toc5_hunterAI: public toc5_champion_baseAI
{
    mob_toc5_hunterAI(Creature* pCreature): toc5_champion_baseAI(pCreature, AI_RANGED)
    {
        Reset();
    }

    uint32 m_uiShoot_Timer;
    uint32 m_uiLightning_Arrows_Timer;
    uint32 m_uiMulti_Shot_Timer;
    uint32 m_uiDisengage_Timer;

    void Reset()
    {
        m_uiShoot_Timer             = 1000;
        m_uiLightning_Arrows_Timer  = 13000;
        m_uiMulti_Shot_Timer        = 10000;
        m_uiDisengage_Timer         = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDisengage_Timer < uiDiff)
        {
            if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 5))
            {
                DoCast(m_creature, SPELL_DISENGAGE);
                m_uiDisengage_Timer = 15000;
            }
            else
                m_uiDisengage_Timer = 5000;
        }
        else
            m_uiDisengage_Timer -= uiDiff;

        if (m_uiShoot_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHOOT);
            m_uiShoot_Timer = 1500;
        }
        else
            m_uiShoot_Timer -= uiDiff;  

        if (m_uiMulti_Shot_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature->getVictim(), SPELL_MULTI_SHOT);
            m_uiMulti_Shot_Timer = urand(7000, 10000);
        }
        else
            m_uiMulti_Shot_Timer -= uiDiff;

        if (m_uiLightning_Arrows_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_LIGHTNING_ARROWS);
            m_uiLightning_Arrows_Timer = 25000;
        }
        else
            m_uiLightning_Arrows_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Rogue
struct MANGOS_DLL_DECL mob_toc5_rogueAI: public toc5_champion_baseAI
{
    mob_toc5_rogueAI(Creature* pCreature): toc5_champion_baseAI(pCreature, AI_MELEE)
    {
        Reset();
    }

    uint32 m_uiDeadlyPoisonTimer;
    uint32 m_uiEviscerate_Timer;
    uint32 m_uiFoK_Timer;
    uint32 m_uiPoisonBottle_Timer;

    void Reset()
    {
        m_uiDeadlyPoisonTimer   = 12000;
        m_uiEviscerate_Timer    = 15000;
        m_uiFoK_Timer           = 10000;
        m_uiPoisonBottle_Timer  = 7000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiDeadlyPoisonTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DEADLY_POISON : SPELL_DEADLY_POISON_H);
            m_uiDeadlyPoisonTimer = urand(8000, 11000);
        } 
        else
            m_uiDeadlyPoisonTimer -= uiDiff;

        if (m_uiEviscerate_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_EVISCERATE : SPELL_EVISCERATE_H);
            m_uiEviscerate_Timer = 10000;
        }
        else
            m_uiEviscerate_Timer -= uiDiff;  

        if (m_uiFoK_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_FAN_OF_KNIVES);
            m_uiFoK_Timer = 7000;
        }
        else
            m_uiFoK_Timer -= uiDiff;

        if (m_uiPoisonBottle_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(m_creature, SPELL_POISON_BOTTLE);
            m_uiPoisonBottle_Timer = 6000;
        }
        else
            m_uiPoisonBottle_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_toc5_warrior(Creature* pCreature)
{
    return new mob_toc5_warriorAI(pCreature);
}

CreatureAI* GetAI_mob_toc5_mage(Creature* pCreature)
{
    return new mob_toc5_mageAI(pCreature);
}

CreatureAI* GetAI_mob_toc5_shaman(Creature* pCreature)
{
    return new mob_toc5_shamanAI(pCreature);
}

CreatureAI* GetAI_mob_toc5_hunter(Creature* pCreature)
{
    return new mob_toc5_hunterAI(pCreature);
}

CreatureAI* GetAI_mob_toc5_rogue(Creature* pCreature)
{
    return new mob_toc5_rogueAI(pCreature);
}

void AddSC_boss_grand_champions()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "mob_toc5_warrior";
    NewScript->GetAI = &GetAI_mob_toc5_warrior;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc5_mage";
    NewScript->GetAI = &GetAI_mob_toc5_mage;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc5_shaman";
    NewScript->GetAI = &GetAI_mob_toc5_shaman;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc5_hunter";
    NewScript->GetAI = &GetAI_mob_toc5_hunter;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_toc5_rogue";
    NewScript->GetAI = &GetAI_mob_toc5_rogue;
    NewScript->RegisterSelf();
}
