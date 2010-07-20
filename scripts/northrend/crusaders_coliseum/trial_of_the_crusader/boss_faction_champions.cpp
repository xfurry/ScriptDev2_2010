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
SDName: faction_champions
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    // the slay are said by varian
    SAY_SLAY1_HORDE         = -1605043,
    SAY_SLAY2_HORDE         = -1605044,
    SAY_SLAY3_HORDE         = -1605045, 
    SAY_SLAY4_HORDE         = -1605046,

    // the slay are said by gorrash
    SAY_SLAY1_ALY           = -1605033,
    SAY_SLAY2_ALY           = -1605034,
    SAY_SLAY3_ALY           = -1605035,
    SAY_SLAY4_ALY           = -1605036,

    // common spells
    SPELL_PVP_TRINKET           = 65547,
    SPELL_CHAMPIONS_AEGIS       = 68595,

    // priest discipline + shadow
    SPELL_DISPEL_MAGIC          = 68624,

    // druid balande + resto
    SPELL_BARKSKIN              = 65860,

    // shaman enhancement + resto
    SPELL_EARTH_SHOCK_10        = 65973,
    SPELL_EARTH_SHOCK_10HC      = 68100,
    SPELL_EARTH_SHOCK_25        = 68101,
    SPELL_EARTH_SHOCK_25HC      = 68102,
    SPELL_HEROISM               = 65983,

    // paladin holy + retri
    SPELL_DIVINE_SHIELD         = 66010,  
};

enum AI
{
    AI_MELEE    = 0,
    AI_RANGED   = 1,
    AI_HEALER   = 2,
};

struct MANGOS_DLL_DECL boss_faction_championAI : public ScriptedAI
{
    boss_faction_championAI(Creature *pCreature, uint32 aitype) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        mAIType = aitype;
        Init();
    }
    
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;
    uint32 mAIType;
    
    uint32 ThreatTimer;
    uint32 CCTimer;

    uint32 TeamInInstance;

    void Init()
    {
        CCTimer = rand()%10000;
        ThreatTimer = 15000;
        TeamInInstance = GetFaction();
        m_creature->SetRespawnDelay(DAY);
    }
    
    float CalculateThreat(float distance, float armor, uint32 health)
    {
        float dist_mod = (mAIType == AI_MELEE) ? 15.0f/(15.0f + distance) : 1.0f;
        float armor_mod = (mAIType == AI_MELEE) ? armor / 16635.0f : 0.0f;
        float eh = (health+1) * (1.0f + armor_mod);
        return dist_mod * 30000.0f / eh;
    }

    void KilledUnit(Unit* pVictim)
    {
        if(TeamInInstance == HORDE)
        {
            if(Creature *Varian = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_VARIAN)))
            {
                switch(urand(0, 2))
                {
                case 0: DoScriptText(SAY_SLAY1_HORDE, Varian); break;
                case 1: DoScriptText(SAY_SLAY2_HORDE, Varian); break;
                case 2: DoScriptText(SAY_SLAY3_HORDE, Varian); break;
                }
            }
        }
        else if(TeamInInstance == ALLIANCE)
        {
            if(Creature *Garrosh = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_GARROSH)))
            {
                switch(urand(0, 2))
                {
                case 0: DoScriptText(SAY_SLAY1_ALY, Garrosh); break;
                case 1: DoScriptText(SAY_SLAY2_ALY, Garrosh); break;
                case 2: DoScriptText(SAY_SLAY3_ALY, Garrosh); break;
                }
            }
        }
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

    void UpdatePower()
    {
        if(m_creature->getPowerType() == POWER_MANA)
            m_creature->ModifyPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA) / 3);
        //else if(m_creature->getPowerType() == POWER_ENERGY)
        //    m_creature->ModifyPower(POWER_ENERGY, 100);
    }
    
    void RemoveCC()
    {
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STUN);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_FEAR);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_PACIFY);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_CONFUSE);
        //DoCast(m_creature, SP_PVP_TRINKET);
    }
    
    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_CHAMPIONS_AEGIS, true);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_FACTION_CHAMPIONS, IN_PROGRESS);

        // call for help
        if(TeamInInstance == HORDE)
        {
            if(m_pInstance)
                m_pInstance->SetData(TYPE_STAGE,3);

            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_1)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_2)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_3)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_4)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_5)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_6)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }

            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_7)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_8)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_9)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_10)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
            } 
        }
        else if(TeamInInstance == ALLIANCE)
        {
            if(m_pInstance)
                m_pInstance->SetData(TYPE_STAGE,4);

            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_1)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_2)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_3)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_4)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_5)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }
            if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_6)))
            {
                if(pTemp->isAlive())
                    pTemp->AI()->AttackStart(m_creature->getVictim());
                else
                    pTemp->Respawn();
            }

            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_7)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_8)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_9)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_10)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
                if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                {
                    if(pTemp->isAlive())
                        pTemp->AI()->AttackStart(m_creature->getVictim());
                    else
                        pTemp->Respawn();
                }
            }
        }
    }
    
    void Reset()
    {
        //if(m_pInstance) 
            //m_pInstance->SetData(TYPE_FACTION_CHAMPIONS, NOT_STARTED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {  
            // do this only for the first who reaches home
            if(m_pInstance->GetData(TYPE_FACTION_CHAMPIONS) != NOT_STARTED)
            {
                // rewpawn dead champs
                if(TeamInInstance == HORDE)
                {
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_1)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_2)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_3)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_4)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_5)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_6)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }

                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    {
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_7)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_8)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_9)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_10)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                    } 
                }
                else if(TeamInInstance == ALLIANCE)
                {
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_1)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_2)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_3)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_4)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_5)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }
                    if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_6)))
                    {
                        if(!pTemp->isAlive())
                            pTemp->Respawn();
                        else
                            pTemp->AI()->EnterEvadeMode();
                    }

                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    {
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_7)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_8)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_9)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_10)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                        if(Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                        {
                            if(!pTemp->isAlive())
                                pTemp->Respawn();
                            else
                                pTemp->AI()->EnterEvadeMode();
                        }
                    }
                }

                // alter the counter
                if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                    m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
                }
                m_pInstance->SetData(TYPE_FACTION_CHAMPIONS, NOT_STARTED);
            }
        }  
    }

    Creature* SelectRandomFriendlyMissingBuff(uint32 spell)
    {
        std::list<Creature *> lst = DoFindFriendlyMissingBuff(40.0f, spell);
        std::list<Creature *>::const_iterator itr = lst.begin();
        if(lst.empty()) 
            return NULL;
        advance(itr, rand()%lst.size());
        return (*itr);
    }

    Unit* SelectEnemyCaster(bool casting)
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator iter;
        for(iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            Unit *target;
            if(target = Unit::GetUnit((*m_creature),(*iter)->getUnitGuid()))
                if(target->getPowerType() == POWER_MANA)
                    return target;
        }
        return NULL;
    }

    uint32 EnemiesInRange(float distance)
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator iter;
        uint32 count = 0;
        for(iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            Unit *target;
            if(target = Unit::GetUnit((*m_creature),(*iter)->getUnitGuid()))
                if(m_creature->GetDistance2d(target) < distance)
                    ++count;
        }
        return count;
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
    
    void UpdateAI(const uint32 diff)
    {
        if(ThreatTimer < diff)
        {
            UpdatePower();
            UpdateThreat();
            ThreatTimer = 4000;
        }
        else ThreatTimer -= diff;
        
        if(CCTimer < diff)
        {
			if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
				RemoveCC();
            CCTimer = 8000+rand()%2000;
        }
        else CCTimer -= diff;

        if(mAIType == AI_MELEE) 
            DoMeleeAttackIfReady();
    }
};

/********************************************************************
                            HEALERS
********************************************************************/
enum druid_resto
{
    // druid resto
    SPELL_LIFEBLOOM_10          = 66093,
    SPELL_LIFEBLOOM_10HC        = 67957,
    SPELL_LIFEBLOOM_25          = 67958,
    SPELL_LIFEBLOOM_25HC        = 67959,
    SPELL_NATURES_GRASP         = 66071,
    SPELL_NOURISH_10            = 66066,
    SPELL_NOURISH_10HC          = 67965,
    SPELL_NOURISH_25            = 67966,
    SPELL_NOURISH_25HC          = 67967,
    SPELL_REGROWTH_10           = 66067,
    SPELL_REGROWTH_10HC         = 67968,
    SPELL_REGROWTH_25           = 67969,
    SPELL_REGROWTH_25HC         = 67970,
    SPELL_REJUVENATION_10       = 66065,
    SPELL_REJUVENATION_10HC     = 67971,
    SPELL_REJUVENATION_25       = 67972,
    SPELL_REJUVENATION_25HC     = 67973,
    SPELL_THORNS                = 66068,
    SPELL_TRANQUILITY_10        = 66086,
    SPELL_TRANQUILITY_10HC      = 67974,
    SPELL_TRANQUILITY_25        = 67975,
    SPELL_TRANQUILITY_25HC      = 67976,
};
struct MANGOS_DLL_DECL boss_fc_resto_druidAI : public boss_faction_championAI
{
    boss_fc_resto_druidAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_HEALER) {Init();}
    
    uint32 m_uiGCDTimer;
    uint32 m_uiGraspTimer;
    uint32 m_uiTranquilityTimer;
    uint32 m_uiBarkSkinTimer;
    
    void Init()
    {
        m_uiGCDTimer = 2500;
        m_uiBarkSkinTimer = 3000;
        m_uiGraspTimer = 5000 + rand()%40000;
        m_uiTranquilityTimer = 20000 + rand()%20000;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
        
        if(m_uiGraspTimer < diff)
        {
            DoCast(m_creature, SPELL_NATURES_GRASP);
            m_uiGraspTimer = 60000 + rand()%10000;
        }
        else m_uiGraspTimer -= diff;
        
        if(m_uiTranquilityTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature, SPELL_TRANQUILITY_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature, SPELL_TRANQUILITY_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature, SPELL_TRANQUILITY_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature, SPELL_TRANQUILITY_25HC);
            m_uiTranquilityTimer = 600000;
        }
        else m_uiTranquilityTimer -= diff;

        if(m_uiBarkSkinTimer < diff)
		{
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
			    DoCast (m_creature, SPELL_BARKSKIN);
			    m_uiBarkSkinTimer = 60000;
            }
            else
                m_uiBarkSkinTimer = 5000;
		}
		else m_uiBarkSkinTimer -= diff;
        
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,4))
            {
            case 0:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_LIFEBLOOM_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_LIFEBLOOM_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_LIFEBLOOM_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_LIFEBLOOM_25HC);
                }
                break;
            case 1:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_NOURISH_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_NOURISH_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_NOURISH_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_NOURISH_25HC);
                }
                break;
            case 2:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_REGROWTH_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_REGROWTH_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_REGROWTH_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_REGROWTH_25HC);
                }
                break;
            case 3:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_REJUVENATION_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_REJUVENATION_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_REJUVENATION_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_REJUVENATION_25HC);
                }
                break;
            case 4:
                if(Creature *target = SelectRandomFriendlyMissingBuff(SPELL_THORNS))
                    DoCast(target, SPELL_THORNS);
                break;
            }
            m_uiGCDTimer = 2000;
        }
        else m_uiGCDTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};


enum shamy_resto
{
    // shaman restoration
    SPELL_CLEANSE_SPIRIT            = 68627,
    SPELL_EARTH_SHIELD              = 66063,
    SPELL_HEX                       = 66054,
    SPELL_LESSER_HEALING_WAVE_10    = 66055,
    SPELL_LESSER_HEALING_WAVE_10HC  = 68115,
    SPELL_LESSER_HEALING_WAVE_25    = 68116,
    SPELL_LESSER_HEALING_WAVE_25HC  = 68117,
    SPELL_RIPTIDE_10                = 66053,
    SPELL_RIPTIDE_10HC              = 68118,
    SPELL_RIPTIDE_25                = 68119,
    SPELL_RIPTIDE_25HC              = 68120,
};

struct MANGOS_DLL_DECL boss_fc_resto_shammyAI : public boss_faction_championAI
{
    boss_fc_resto_shammyAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_HEALER) {Init();}
    
    uint32 m_uiHeroTimer;
    uint32 m_uiHexTimer;
    uint32 m_uiGCDTimer;
    
    void Init()
    {
        m_uiHeroTimer = 30000 + rand()%30000;
        m_uiHexTimer = 10000 + rand()%30000;
        m_uiGCDTimer = 2500;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
        
        if(m_uiHeroTimer < diff)
        {
            DoCast(m_creature, SPELL_HEROISM);
            m_uiHeroTimer = 600000;
        }
        else m_uiHeroTimer -= diff;
        
        if(m_uiHexTimer < diff)
        {
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(target, SPELL_HEX);
            m_uiHexTimer = 20000 + rand()%20000;
        }
        else m_uiHexTimer -= diff;
        
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,5))
            {
            case 0: case 1:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_LESSER_HEALING_WAVE_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_LESSER_HEALING_WAVE_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_LESSER_HEALING_WAVE_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_LESSER_HEALING_WAVE_25HC);
                }
                break;
            case 2:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_RIPTIDE_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_RIPTIDE_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_RIPTIDE_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_RIPTIDE_25HC);
                }
                break;
            case 3:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_EARTH_SHOCK_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_EARTH_SHOCK_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_EARTH_SHOCK_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_EARTH_SHOCK_25HC);
                }
                break;
            case 4:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                    DoCast(pTarget, SPELL_CLEANSE_SPIRIT);
                break;
            case 5:
                if(Unit *pTarget = SelectRandomFriendlyMissingBuff(SPELL_EARTH_SHIELD))
                    DoCast(pTarget, SPELL_EARTH_SHIELD);
                break;
            }
            m_uiGCDTimer = 2000;
        }
        else m_uiGCDTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};

enum paly_holy
{
    // paladin holy
    SPELL_CLEANSE               = 68623,
    SPELL_FLASH_OF_LIGHT_10     = 66113,
    SPELL_FLASH_OF_LIGHT_10HC   = 68008,
    SPELL_FLASH_OF_LIGHT_25     = 68009,
    SPELL_FLASH_OF_LIGHT_25HC   = 68010,
    SPELL_HAMMER_OF_JUSTICE     = 66613,
    SPELL_HAND_OF_FREEDOM       = 66115,
    SPELL_HAND_OF_PROTECTION    = 66009,
    SPELL_HOLY_LIGHT_10         = 66112,
    SPELL_HOLY_LIGHT_10HC       = 68011,
    SPELL_HOLY_LIGHT_25         = 68012,
    SPELL_HOLY_LIGHT_25HC       = 68013,
    SPELL_HOLY_SHOCK_10         = 66114,
    SPELL_HOLY_SHOCK_10HC       = 68014,
    SPELL_HOLY_SHOCK_25         = 68015,
    SPELL_HOLY_SHOCK_25HC       = 68016,

    EQUIP_ID_PALY_HOLY_HORDE    = 45205,
};

struct MANGOS_DLL_DECL boss_fc_holy_pallyAI : public boss_faction_championAI
{
    boss_fc_holy_pallyAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_HEALER) 
    {
        if(m_uiCreatureEntry == 34445)
            SetEquipmentSlots(false, EQUIP_ID_PALY_HOLY_HORDE, -1, -1);
        Init();
    }
    
    bool m_bBubbleUsed;
    bool m_bProtectionUsed;
    uint32 m_uiFreedomTimer;
    uint32 m_uiGCDTimer;
    uint32 m_uiShockTimer;
    uint32 m_uiHammerOfJusticeTimer;
    uint32 m_uiCreatureEntry;
    
    void Init()
    {
        m_uiGCDTimer = 3000;
        m_uiShockTimer = 6000 + rand()%4000;
        m_bBubbleUsed = false;
        m_bProtectionUsed = false;
        m_uiFreedomTimer = 10000 + rand()%15000;
        m_uiHammerOfJusticeTimer = 15000 + rand()%10000;
        m_uiCreatureEntry = m_creature->GetEntry();
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
       
        //cast bubble at 20% hp
        if(!m_bBubbleUsed && m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
        {
            DoCast(m_creature, SPELL_DIVINE_SHIELD);
            m_bBubbleUsed = true;
        }
        
        if(!m_bProtectionUsed)
        {
            if(Unit *target = DoSelectLowestHpFriendly(40.0f))
            {
                if(target->GetHealth() * 100 / target->GetMaxHealth() < 15)
                {
                    DoCast(target, SPELL_HAND_OF_PROTECTION);
                    m_bProtectionUsed = true;
                }
            }
        }

        if(m_uiShockTimer < diff)
        {
            if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_HOLY_SHOCK_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_HOLY_SHOCK_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_HOLY_SHOCK_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_HOLY_SHOCK_25HC);
                m_uiShockTimer = 6000 + rand()%4000;
            }
            else
                m_uiShockTimer = 1000;
        }
        else m_uiShockTimer -=diff;
        
        if(m_uiFreedomTimer < diff)
        {   
            if(Unit *target = SelectRandomFriendlyMissingBuff(SPELL_HAND_OF_FREEDOM))
                DoCast(target, SPELL_HAND_OF_FREEDOM);
            m_uiFreedomTimer = 25000 + rand()%10000;
        }
        else m_uiFreedomTimer -= diff;

        if(m_uiHammerOfJusticeTimer < diff)
        {
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(m_creature->GetDistance2d(target) < 15)
                {
                    DoCast(target, SPELL_HAMMER_OF_JUSTICE);
                    m_uiHammerOfJusticeTimer=40000+rand()%5000;
                }
                else
                    m_uiHammerOfJusticeTimer = 1000;
            }
        }
        else m_uiHammerOfJusticeTimer -= diff;
        
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,4))
            {
            case 0: case 1:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_FLASH_OF_LIGHT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_FLASH_OF_LIGHT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_FLASH_OF_LIGHT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_FLASH_OF_LIGHT_25HC);
                }
                break;
            case 2: case 3:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_HOLY_LIGHT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_HOLY_LIGHT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_HOLY_LIGHT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_HOLY_LIGHT_25HC);
                }
                break;
            case 4:
                if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                    DoCast(target, SPELL_CLEANSE);
                break;
            }
            m_uiGCDTimer = 2500;
        }
        else m_uiGCDTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};

enum priest_discipline
{
    // priest discipline
    SPELL_FLASH_HEAL_10         = 66104,
    SPELL_FLASH_HEAL_10HC       = 68023,
    SPELL_FLASH_HEAL_25         = 68024,
    SPELL_FLASH_HEAL_25HC       = 68025,
    SPELL_MANA_BURN_10          = 66100,
    SPELL_MANA_BURN_10HC        = 68026,
    SPELL_MANA_BURN_25          = 68027,
    SPELL_MANA_BURN_25HC        = 68028,
    SPELL_PENANCE_10            = 66098,
    SPELL_PENANCE_10HC          = 68029,
    SPELL_PENANCE_25            = 68030,
    SPELL_PENANCE_25HC          = 68031,
    SPELL_PSYCHIC_SCREAM        = 65543,
    SPELL_RENEW_10              = 66177,
    SPELL_RENEW_10HC            = 68035,
    SPELL_RENEW_25              = 68036,
    SPELL_RENEW_25HC            = 68037,
    SPELL_POWER_WORD_SHIELD_10  = 66099,
    SPELL_POWER_WORD_SHIELD_25  = 68033,
    SPELL_POWER_WORD_SHIELD_10HC= 68032,
    SPELL_POWER_WORD_SHIELD_25HC= 68034,
};

struct MANGOS_DLL_DECL boss_fc_holy_priestAI : public boss_faction_championAI
{
    boss_fc_holy_priestAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_HEALER) {Init();}
    
    uint32 m_uiGCDTimer;
    uint32 m_uiScreamTimer;
    
    void Init()
    {
        m_uiGCDTimer = 2500;
        m_uiScreamTimer = 5000;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiScreamTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                DoCast(m_creature, SPELL_PSYCHIC_SCREAM);
                m_uiScreamTimer = 30000;
            }
            else
                m_uiScreamTimer = 3000;
        }
        else m_uiScreamTimer -= diff;
        
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,6))
            {
            case 0:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_RENEW_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_RENEW_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_RENEW_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_RENEW_25HC);
                }
                break;
            case 1:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_POWER_WORD_SHIELD_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_POWER_WORD_SHIELD_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_POWER_WORD_SHIELD_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_POWER_WORD_SHIELD_25HC);
                }
                break;
            case 2: case 3:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_FLASH_HEAL_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_FLASH_HEAL_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_FLASH_HEAL_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_FLASH_HEAL_25HC);
                }
                break;
            case 4:
                if(Unit *target = urand(0,1) ? m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0) : DoSelectLowestHpFriendly(40.0f))
                    DoCast(target, SPELL_DISPEL_MAGIC);
                break;
            case 5:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_MANA_BURN_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_MANA_BURN_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_MANA_BURN_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_MANA_BURN_25HC);
                }
                break;
            case 6:
                if(Unit *pTarget = DoSelectLowestHpFriendly(40.0f))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_PENANCE_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_PENANCE_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_PENANCE_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_PENANCE_25HC);
                }
                break;
            }
            m_uiGCDTimer = 2000;
        }
        else m_uiGCDTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};

/********************************************************************
                            RANGED
********************************************************************/
enum priest_shadow
{
        // priest shadow
    SPELL_DISPERSION            = 65544,
    SPELL_MIND_BLAST_10         = 65492,
    SPELL_MIND_BLAST_10HC       = 68038,
    SPELL_MIND_BLAST_25         = 68039,
    SPELL_MIND_BLAST_25HC       = 68040,
    SPELL_MIND_FLAY_10          = 65488,
    SPELL_MIND_FLAY_25          = 68043,
    SPELL_MIND_FLAY_10HC        = 68042,
    SPELL_MIND_FLAY_25HC        = 68044,
    SPELL_PSYHIC_HORROR         = 65545,
    SPELL_SILENCE               = 65542,
    SPELL_SHADOW_WORD_PAIN_10   = 65541,
    SPELL_SHADOW_WORD_PAIN_10HC = 68088,
    SPELL_SHADOW_WORD_PAIN_25   = 68089,
    SPELL_SHADOW_WORD_PAIN_25HC = 68090,
    SPELL_VAMPIRIC_TOUCH_10     = 65490,
    SPELL_VAMPIRIC_TOUCH_10HC   = 68091,
    SPELL_VAMPIRIC_TOUCH_25     = 68092,
    SPELL_VAMPIRIC_TOUCH_25HC   = 68093,
    SPELL_SHADOWFORM            = 16592,
};

struct MANGOS_DLL_DECL boss_fc_shadow_priestAI : public boss_faction_championAI
{
    boss_fc_shadow_priestAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_RANGED) {Init();}
    
    uint32 m_uiSilenceTimer;
    uint32 m_uiPsychicScreamTimer;
    uint32 m_uiMindBlastTimer;
    uint32 m_uiGCDTimer;
    uint32 m_uiDispersionTimer;
    
    void Init()
    {
        m_uiSilenceTimer = 15000;
        m_uiPsychicScreamTimer = 8000;
        m_uiMindBlastTimer = 15000;
        m_uiGCDTimer = 3000;
        m_uiDispersionTimer = 1000;
    }

    void Aggro(Unit *who)
    {
        boss_faction_championAI::Aggro(who);
        DoCast(m_creature, SPELL_SHADOWFORM, true);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
        
        if(m_uiPsychicScreamTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                DoCast(m_creature, SPELL_PSYHIC_HORROR);
                m_uiPsychicScreamTimer = 24000;
            }
            else
                m_uiPsychicScreamTimer = 3000;
        }
        else m_uiPsychicScreamTimer -= diff;

        if(m_uiDispersionTimer < diff)
        {
            if(m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_DISPERSION);
                m_uiDispersionTimer = 180000;
            }
            else m_uiDispersionTimer = 1000;
        }
        else m_uiDispersionTimer -= diff;
        
        if(m_uiSilenceTimer < diff)
        {
            if(Unit *target = SelectEnemyCaster(false))
                DoCast(target, SPELL_SILENCE);
            m_uiSilenceTimer = 45000;
        }
        else m_uiSilenceTimer -= diff;

        if(m_uiMindBlastTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_MIND_BLAST_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_MIND_BLAST_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_MIND_BLAST_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_MIND_BLAST_25HC);
            m_uiMindBlastTimer = 8000;
        }
        else m_uiMindBlastTimer -= diff;
        
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,4))
            {
            case 0: case 1:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY_25HC);
                break;
            case 2:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_VAMPIRIC_TOUCH_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_VAMPIRIC_TOUCH_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_VAMPIRIC_TOUCH_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_VAMPIRIC_TOUCH_25HC);
                }
                break;
            case 3:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_SHADOW_WORD_PAIN_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_SHADOW_WORD_PAIN_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_SHADOW_WORD_PAIN_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_SHADOW_WORD_PAIN_25HC);
                }
                break;
            case 4:
                    if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_DISPEL_MAGIC);
                    break;
            }
            m_uiGCDTimer = 2000;
        }
        else m_uiGCDTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};

enum warlock
{
    // warlock affliction + zhaagrym
    SPELL_CORRUPTION_10             = 65810,
    SPELL_CORRUPTION_10HC           = 68133,
    SPELL_CORRUPTION_25             = 68134,
    SPELL_CORRUPTION_25HC           = 68135,
    SPELL_CURSE_OF_AGONY_10         = 65814,
    SPELL_CURSE_OF_AGONY_10HC       = 68136,
    SPELL_CURSE_OF_AGONY_25         = 68137,
    SPELL_CURSE_OF_AGONY_25HC       = 68138,
    SPELL_CURSE_OF_EXHAUSTION       = 65815,
    SPELL_DEATH_COIL                = 65820,
    SPELL_DEATH_COIL_HC             = 68139,
    SPELL_FEAR                      = 65809,
    SPELL_HELLFIRE_10               = 65816,
    SPELL_HELLFIRE_25               = 68146,
    SPELL_HELLFIRE_10HC             = 68145,
    SPELL_HELLFIRE_25HC             = 68147,
    SPELL_SEARING_PAIN_10           = 65819,
    SPELL_SEARING_PAIN_10HC         = 68148,
    SPELL_SEARING_PAIN_25           = 68149,
    SPELL_SEARING_PAIN_25HC         = 68150,
    SPELL_SHADOW_BOLT_10            = 65821,
    SPELL_SHADOW_BOLT_10HC          = 68151,
    SPELL_SHADOW_BOLT_25            = 68152,
    SPELL_SHADOW_BOLT_25HC          = 68153,
    SPELL_SUMMON_FELHUNTER          = 67514,
    SPELL_UNSTABLE_AFFLICTION_10    = 65812,
    SPELL_UNSTABLE_AFFLICTION_10HC  = 68154,
    SPELL_UNSTABLE_AFFLICTION_25    = 68155,
    SPELL_UNSTABLE_AFFLICTION_25HC  = 68156,
};

struct MANGOS_DLL_DECL boss_fc_warlockAI : public boss_faction_championAI
{
    boss_fc_warlockAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_RANGED) {Init();}
    
	uint32 m_uiFearTimer;
	uint32 m_uiUnstableAfflictionTimer;
    uint32 m_uiHellfireTimer;
	uint32 m_uiGCDTimer;

    void Init()
    {
        m_uiFearTimer = 4000+rand()%1000;
	    m_uiUnstableAfflictionTimer = 2000+rand()%1000;
        m_uiHellfireTimer = 15000;
	    m_uiGCDTimer = 2500;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiFearTimer < diff)
		{
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_FEAR);
            m_uiFearTimer = 8000;
        }
		else m_uiFearTimer -= diff;

        if(m_uiHellfireTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature, SPELL_HELLFIRE_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature, SPELL_HELLFIRE_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature, SPELL_HELLFIRE_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature, SPELL_HELLFIRE_25HC);
                m_uiHellfireTimer = 40000;
            }
            else
                m_uiHellfireTimer = 5000;
        }
        else m_uiHellfireTimer -= diff;

        if(m_uiUnstableAfflictionTimer  < diff)
        {
            if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_UNSTABLE_AFFLICTION_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_UNSTABLE_AFFLICTION_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_UNSTABLE_AFFLICTION_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_UNSTABLE_AFFLICTION_25HC);
            }
            m_uiUnstableAfflictionTimer = 15000;
        }
        else m_uiUnstableAfflictionTimer  -= diff;
		
		if(m_uiGCDTimer < diff)
        {
            switch(urand(0,5))
            {
            case 0:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SEARING_PAIN_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SEARING_PAIN_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SEARING_PAIN_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SEARING_PAIN_25HC);
                break;
            case 1: case 2:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SHADOW_BOLT_25HC);
                break;
            case 3:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_CORRUPTION_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_CORRUPTION_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_CORRUPTION_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_CORRUPTION_25HC);
                }
                break;
            case 4:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_CURSE_OF_AGONY_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_CURSE_OF_AGONY_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_CURSE_OF_AGONY_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_CURSE_OF_AGONY_25HC);
                }
                break;
            case 5:
                if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_CURSE_OF_EXHAUSTION);
                break;
			 }
			 m_uiGCDTimer = 2500;
		}
		else m_uiGCDTimer -= diff;

		boss_faction_championAI::UpdateAI(diff);
    }
};

enum mage
{
    // mage arcane
    SPELL_ARCANE_BARRAGE_10     = 65799,
    SPELL_ARCANE_BARRAGE_10HC   = 67994,
    SPELL_ARCANE_BARRAGE_25     = 67995,
    SPELL_ARCANE_BARRAGE_25HC   = 67996,
    SPELL_ARCANE_BLAST_10       = 65791,
    SPELL_ARCANE_BLAST_10HC     = 67997,
    SPELL_ARCANE_BLAST_25       = 67998,
    SPELL_ARCANE_BLAST_25HC     = 67999,
    SPELL_ARCANE_EXPLOSION      = 65800,
    SPELL_BLINK                 = 65793,
    SPELL_COUNTERSPELL          = 65790,
    SPELL_FROST_NOVA            = 65792,
    SPELL_FROSTBOLT_10          = 65807,
    SPELL_FROSTBOLT_10HC        = 68003,
    SPELL_FROSTBOLT_25          = 68004,
    SPELL_FROSTBOLT_25HC        = 68005,
    SPELL_ICEBLOCK              = 65802,
    SPELL_POLYMORPH             = 65801,

    EQUIP_ID_MAGE_ALY           = 45129,
    EQUIP_ID_MAGE_HORDE         = 45208,
};

struct MANGOS_DLL_DECL boss_fc_mageAI : public boss_faction_championAI
{
    boss_fc_mageAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_RANGED) 
    {
        if(m_uiCreatureEntry == 34468)
            SetEquipmentSlots(false, EQUIP_ID_MAGE_ALY, -1, -1);
        if(m_uiCreatureEntry == 34449)
            SetEquipmentSlots(false, EQUIP_ID_MAGE_HORDE, -1, -1);
        Init();
    }
    
	uint32 m_uiArcaneBarrageTimer;
    uint32 m_uiBlinkTimer;
	uint32 m_uiCounterspellTimer;
	uint32 m_uiFrostNovaTimer;
	bool m_bIceBlockUsed;
	uint32 m_uiPolymorphTimer;
	uint32 m_uiGCDTimer;
    uint32 m_uiCreatureEntry;

    void Init()
    {
        m_uiArcaneBarrageTimer = 2000+rand()%1000;
        m_uiBlinkTimer =  8000+rand()%1000;
        m_uiCounterspellTimer = 5000+rand()%1000;
        m_uiFrostNovaTimer =  7000+rand()%2000;
        m_bIceBlockUsed = false;
        m_uiPolymorphTimer = 3000+rand()%3000;
        m_uiGCDTimer = 2000;
        m_uiCreatureEntry = m_creature->GetEntry();
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

		if(m_uiCounterspellTimer < diff)
		{
            if(Unit *target = SelectEnemyCaster(false))
			    DoCast (target, SPELL_COUNTERSPELL);
			m_uiCounterspellTimer = 24000;
		}
		else m_uiCounterspellTimer -= diff;

		if(m_uiBlinkTimer < diff) // escape
		{
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth() && EnemiesInRange(10.0f)>3)
            {
				DoCast(m_creature, SPELL_FROST_NOVA, true);
			    DoCast (m_creature, SPELL_BLINK);
                m_uiBlinkTimer = 25000;
            }
            else
                m_uiBlinkTimer = 5000;
		}
		else m_uiBlinkTimer -= diff;

		if(!m_bIceBlockUsed && m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
		{
			DoCast(m_creature, SPELL_ICEBLOCK);
			m_bIceBlockUsed = true;
		}

        if(m_uiPolymorphTimer < diff)
        {
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                DoCast(target, SPELL_POLYMORPH);
            m_uiPolymorphTimer = 15000;
        }
        else m_uiPolymorphTimer -= diff;
    
        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,2))
            {
            case 0:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BARRAGE_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BARRAGE_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BARRAGE_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BARRAGE_25HC);
                break;
            case 1:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BLAST_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BLAST_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BLAST_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_ARCANE_BLAST_25HC);
                break;
            case 2:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_FROSTBOLT_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_FROSTBOLT_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_FROSTBOLT_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_FROSTBOLT_25HC);
                }
                break;
            }
            m_uiGCDTimer = 2500;
        }
        else m_uiGCDTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum hunter
{
    // hunter survival + cat
    SPELL_AIMED_SHOT_10         = 65883,
    SPELL_AIMED_SHOT_25         = 67978,
    SPELL_AIMED_SHOT_10HC       = 67977,
    SPELL_AIMED_SHOT_25HC       = 67979,
    SPELL_CALL_PET              = 67777,
    SPELL_DETERRENCE            = 65871,
    SPELL_DISENGAGE             = 65869,
    SPELL_FROST_TRAP            = 65880,
    SPELL_SHOOT_10              = 65868,
    SPELL_SHOOT_25              = 67989,
    SPELL_SHOOT_10HC            = 67988,
    SPELL_SHOOT_25HC            = 67990,
    SPELL_EXPLOSIVE_SHOOT_10    = 65866,
    SPELL_EXPLOSIVE_SHOOT_10HC  = 67983,
    SPELL_EXPLOSIVE_SHOOT_25    = 67984,
    SPELL_EXPLOSIVE_SHOOT_25HC  = 67985,
    SPELL_WING_CLIP             = 66207,
    SPELL_STEADY_SHOT           = 65867,
    SPELL_WYVERN_STING          = 65877,

    EQUIP_ID_HUNTER_ALY         = 45128,
};

struct MANGOS_DLL_DECL boss_fc_hunterAI : public boss_faction_championAI
{
    boss_fc_hunterAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_RANGED) 
    {
        if(m_uiCreatureEntry == 34467)
            SetEquipmentSlots(false, EQUIP_ID_HUNTER_ALY, -1, -1);
        Init();
    }
    
	uint32 m_uiAimedShotTimer;
    uint32 m_uiCallPetTimer;
	uint32 m_uiDeterrenceTimer;
	uint32 m_uiDisengageTimer;
	uint32 m_uiExplosiveShotTimer;
	uint32 m_uiFrostTrapTimer;
	uint32 m_uiAutoShootTimer;
	uint32 m_uiSteadyShotTimer;
	uint32 m_uiWingClipTimer;
	uint32 m_uiWyvernStingTimer;
    uint32 m_uiGCDTimer;
    uint32 m_uiCreatureEntry;

    void Init()
    {
		m_uiAimedShotTimer = 1000+rand()%2000;
		m_uiDeterrenceTimer = 20000;
		m_uiDisengageTimer = 10000+rand()%3000;
		m_uiExplosiveShotTimer = 2000+rand()%3000;
		m_uiFrostTrapTimer = 11000+rand()%5000;
		m_uiSteadyShotTimer = 3200+rand()%2000;
		m_uiWingClipTimer =  6000+rand()%2000;
		m_uiWyvernStingTimer = 7000+rand()%3000;
		m_uiGCDTimer = 1000;
        m_uiCreatureEntry = m_creature->GetEntry();
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

		if(m_uiDisengageTimer < diff)
		{
            if(EnemiesInRange(10.0f) > 3)
            {
			    DoCast(m_creature , SPELL_DISENGAGE);
			    m_uiDisengageTimer = 30000;
            }
            else
                m_uiDisengageTimer = 3000;
		}
		else m_uiDisengageTimer -= diff;

		if(m_uiDeterrenceTimer < diff)
		{
            if(m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
            {
			    DoCast(m_creature, SPELL_DETERRENCE);
			    m_uiDeterrenceTimer = 90000;
            }
            else m_uiDeterrenceTimer = 3000;
		}
		else m_uiDeterrenceTimer -= diff;

        if(m_uiWyvernStingTimer < diff)
		{
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			    DoCast(target, SPELL_WYVERN_STING);
            m_uiWyvernStingTimer = 60000;
		}
		else m_uiWyvernStingTimer -= diff;

        if(m_uiFrostTrapTimer < diff)
		{
			DoCast(m_creature, SPELL_FROST_TRAP);
            m_uiFrostTrapTimer = 30000 + rand()%1000;
		}
		else m_uiFrostTrapTimer -= diff;

        if(m_uiWingClipTimer < diff)
        {
            if(m_creature->GetDistance2d(m_creature->getVictim()) < 5.0f)
                DoCast(m_creature->getVictim(), SPELL_WING_CLIP);
            m_uiWingClipTimer = 5000;
        }
        else m_uiWingClipTimer-= diff;

        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,3))
            {
            case 0: case 1:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SHOOT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_SHOOT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SHOOT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_SHOOT_25HC);
                break;
            case 2:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_EXPLOSIVE_SHOOT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_EXPLOSIVE_SHOOT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_EXPLOSIVE_SHOOT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_EXPLOSIVE_SHOOT_25HC);
                break;
            case 3:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_AIMED_SHOT_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_AIMED_SHOT_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_AIMED_SHOT_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_AIMED_SHOT_25HC);
                break;
            }
		    m_uiGCDTimer = 2500;
		}
		else m_uiGCDTimer -= diff; 

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum druid_balance
{
    // druid balance
    SPELL_CYCLONE               = 65859,
    SPELL_ENTANGLING_ROOTS      = 65857,
    SPELL_FAERIE_FIRE           = 65863,
    SPELL_FORCE_OF_NATURE       = 65861,
    NPC_TREANT                  = 36070,
    SPELL_INSECT_SWARM_10       = 65855,
    SPELL_INSECT_SWARM_10HC     = 67941,
    SPELL_INSECT_SWARM_25       = 67942,
    SPELL_INSECT_SWARM_25HC     = 67943,
    SPELL_MOONFIRE_10           = 65856,
    SPELL_MOONFIRE_10HC         = 67944,
    SPELL_MOONFIRE_25           = 67945,
    SPELL_MOONFIRE_25HC         = 67946,
    SPELL_STARFIRE_10           = 65854,
    SPELL_STARFIRE_10HC         = 67947,
    SPELL_STARFIRE_25           = 67948,
    SPELL_STARFIRE_25HC         = 67949,
    SPELL_WRATH_10              = 65862,
    SPELL_WRATH_10HC            = 67951,
    SPELL_WRATH_25              = 67952,
    SPELL_WRATH_25HC            = 67953,

    EQUIP_ID_DRUID_HORDE        = 45212,
    EQUIP_ID_DRUID_ALY          = 45128,
};

struct MANGOS_DLL_DECL boss_fc_boomkinAI : public boss_faction_championAI
{
    boss_fc_boomkinAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_RANGED) 
    {
        if(m_uiCreatureEntry == 34451)
            SetEquipmentSlots(false, EQUIP_ID_DRUID_HORDE, -1, -1);
        if(m_uiCreatureEntry == 34460)
            SetEquipmentSlots(false, EQUIP_ID_DRUID_ALY, -1, -1);
        Init();
    }
    
	uint32 m_uiBarkskinTimer;
	uint32 m_uiCycloneTimer;
	uint32 m_uiEntanglingRootsTimer;
	uint32 m_uiMoonfireTimer;
    uint32 m_uiFaerieFireTimer;
    uint32 m_uiForceOfNatureTimer;
	uint32 m_uiGCDTimer;
    uint32 m_uiCreatureEntry;
    
	void Init()
    {
		m_uiBarkskinTimer = 6000;
		m_uiCycloneTimer = 5000;
		m_uiEntanglingRootsTimer = 2000+rand()%1000;
		m_uiMoonfireTimer = 1000+rand()%1000;
        m_uiFaerieFireTimer = 10000;
        m_uiForceOfNatureTimer = 12000+rand()%2000;
		m_uiGCDTimer = 1500;
        m_uiCreatureEntry = m_creature->GetEntry();
    }

    void Aggro(Unit *who)
    {
        boss_faction_championAI::Aggro(who);
        DoCast(m_creature, SPELL_FORCE_OF_NATURE, true);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiBarkskinTimer < diff)
		{
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
			    DoCast (m_creature, SPELL_BARKSKIN);
			    m_uiBarkskinTimer = 60000;
            }
            else
                m_uiBarkskinTimer = 3000;
		}
		else m_uiBarkskinTimer -= diff;

        if(m_uiForceOfNatureTimer < diff)
        {
            DoCast(m_creature, SPELL_FORCE_OF_NATURE, true);
            m_uiForceOfNatureTimer = 12000+rand()%3000;
        }
        else m_uiForceOfNatureTimer -= diff;

        if(m_uiCycloneTimer < diff)
		{
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			    DoCast (target, SPELL_CYCLONE);
			m_uiCycloneTimer = 6000+rand()%2000;
		}
		else m_uiCycloneTimer -= diff;

        if(m_uiEntanglingRootsTimer < diff)
		{
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
			    DoCast(target, SPELL_ENTANGLING_ROOTS);
			m_uiEntanglingRootsTimer = 10000 + rand()%2000;
		}
		else m_uiEntanglingRootsTimer -= diff;

        if(m_uiFaerieFireTimer < diff)
        {
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_FAERIE_FIRE);
            m_uiFaerieFireTimer = 10000 + rand()%8000;
        }
        else m_uiFaerieFireTimer -= diff;

        if(m_uiGCDTimer < diff)
        {
            switch(urand(0,6))
            {
            case 0: case 1:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_MOONFIRE_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_MOONFIRE_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_MOONFIRE_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_MOONFIRE_25HC);
                break;
            case 2:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_INSECT_SWARM_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_INSECT_SWARM_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_INSECT_SWARM_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_INSECT_SWARM_25HC);
                }
                break;
            case 3:
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_STARFIRE_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_STARFIRE_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_STARFIRE_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_STARFIRE_25HC);
                }
                break;
            case 4: case 5: case 6:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_WRATH_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_WRATH_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_WRATH_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_WRATH_25HC);
                break;
            }
            m_uiGCDTimer = 2000;
		}
		else m_uiGCDTimer -= diff; 
          
        boss_faction_championAI::UpdateAI(diff);
    }
};

/********************************************************************
                            MELEE
********************************************************************/
enum warrior
{
    // warrior arms
    SPELL_BLADESTORM            = 65947,
    SPELL_BLADESTORM_TRIG       = 65946,
    SPELL_CHARGE                = 68764,
    SPELL_DISARM                = 65935,
    SPELL_INTIMIDATING_SHOUT    = 65930,
    SPELL_OVERPOWER             = 65924,
    SPELL_MORTAL_STRIKE_10      = 65926,
    SPELL_MORTAL_STRIKE_10HC    = 68782,
    SPELL_MORTAL_STRIKE_25      = 68783,
    SPELL_MORTAL_STRIKE_25HC    = 68784,
    SPELL_RETALIATION           = 65932,
    SPELL_SHATTERING_THROW      = 65940,
    SPELL_SUNDER_ARMOR          = 65936,
};

struct MANGOS_DLL_DECL boss_fc_warriorAI : public boss_faction_championAI
{
    boss_fc_warriorAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
    
    uint32 m_uiBladestormTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiMortalStrikeTimer;
    uint32 m_uiChargeTimer;
    uint32 m_uiSunderArmorTimer;
    uint32 m_uiRetaliationTimer;
    uint32 m_uiShatteringThrowTimer;
    uint32 m_uiDisarmTimer;
    uint32 m_uiOverpowerTimer;
    
    void Init()
    {
        m_uiBladestormTimer = 20000 + rand()%10000;
        m_uiFearTimer = 10000 + rand()%10000;
        m_uiMortalStrikeTimer = 6000 + rand()%4000;
        m_uiChargeTimer = 1000;
        m_uiSunderArmorTimer = 5000;
        m_uiRetaliationTimer = 30000 + rand()%8000;
        m_uiShatteringThrowTimer =  10000 + rand()%30000;
        m_uiDisarmTimer = 5000 + rand()%20000;
        m_uiOverpowerTimer = 5000;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
        
        if(m_uiBladestormTimer < diff)
        {
            DoCast(m_creature, SPELL_BLADESTORM);
            m_uiBladestormTimer = 90000 + rand()%5000;
        }
        else m_uiBladestormTimer -= diff;

        if(m_uiOverpowerTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_OVERPOWER);
            m_uiOverpowerTimer = 5000;
        }
        else m_uiOverpowerTimer -= diff;
        
        if(m_uiFearTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_INTIMIDATING_SHOUT);
            m_uiFearTimer = 40000 + rand()%40000;
        }
        else m_uiFearTimer -= diff;
        
        if(m_uiMortalStrikeTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE_25HC);
            m_uiMortalStrikeTimer = 6000 + rand()%2000;
        }
        else m_uiMortalStrikeTimer -= diff;

        if(m_uiSunderArmorTimer < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_SUNDER_ARMOR);
			m_uiSunderArmorTimer = 2000+rand()%5000;
		}
		else m_uiSunderArmorTimer -= diff;

        if(m_uiChargeTimer < diff)
        {
            if(m_creature->IsInRange(m_creature->getVictim(), 8.0f, 25.0f, false))
            {
                DoCast(m_creature->getVictim(), SPELL_CHARGE);
                m_uiChargeTimer = 12000;
            }
            else
                m_uiChargeTimer = 2000;
        }
        else m_uiChargeTimer -= diff;

        if(m_uiRetaliationTimer < diff)
		{
			DoCast(m_creature, SPELL_RETALIATION);
			m_uiRetaliationTimer = 300000 + rand()%10000;
		}
		else m_uiRetaliationTimer -= diff;

		if(m_uiShatteringThrowTimer < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHATTERING_THROW);
			m_uiShatteringThrowTimer = 300000 + rand()%30000;
		}
		else m_uiShatteringThrowTimer -= diff;
        
        if(m_uiDisarmTimer < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_DISARM);
            m_uiDisarmTimer = 60000 + rand()%6000;
		}
		else m_uiDisarmTimer -= diff;
        
        boss_faction_championAI::UpdateAI(diff);
    }
};

enum deathknight
{
        // dk frost dps
    SPELL_CHAINS_OF_ICE         = 66020,
    SPELL_DEATH_GRIP            = 68755,
    SPELL_DEATH_COIL_10         = 66019,
    SPELL_DEATH_COIL_10HC       = 67929,
    SPELL_DEATH_COIL_25         = 67930,
    SPELL_DEATH_COIL_25HC       = 67931,
    SPELL_FROST_STRIKE_10       = 66047,
    SPELL_FROST_STRIKE_10HC     = 67935,
    SPELL_FROST_STRIKE_25       = 67936,
    SPELL_FROST_STRIKE_25HC     = 67937,
    SPELL_ICEBOUND_FORTITUDE    = 66023,
    SPELL_ICY_TOUCH_10          = 66021,
    SPELL_ICY_TOUCH_10HC        = 67938,
    SPELL_ICY_TOUCH_25          = 67939,
    SPELL_ICY_TOUCH_25HC        = 67940,
    SPELL_STRANGULATE           = 66018,
};             

struct MANGOS_DLL_DECL boss_fc_deathknightAI : public boss_faction_championAI
{
    boss_fc_deathknightAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
	
	uint32 m_uiChainsOfIceTimer;
	uint32 m_uiDeathCoilTimer;
	uint32 m_uiDeathGripTimer;
	uint32 m_uiFrostStrikeTimer;
	uint32 m_uiIceboundFortitudeTimer;
	uint32 m_uiIcyTouchTimer;
	uint32 m_uiStrangulateTimer;

    void Init()
    {
        m_uiChainsOfIceTimer = 2000+rand()%3000;
	    m_uiDeathCoilTimer = 3000+rand()%4000;
	    m_uiDeathGripTimer = 1000+rand()%2000;
	    m_uiFrostStrikeTimer = 4000+rand()%2000;
	    m_uiIceboundFortitudeTimer = 20000;
	    m_uiIcyTouchTimer = 6000+rand()%2000;
	    m_uiStrangulateTimer = 6000+rand()%10000;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;
        
        if(m_uiIceboundFortitudeTimer < diff)
        {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_ICEBOUND_FORTITUDE);
                m_uiIceboundFortitudeTimer = 60000 + rand()%10000;
            }
            else
                m_uiIceboundFortitudeTimer = 5000;
        }
        else m_uiIceboundFortitudeTimer -= diff;

		if(m_uiChainsOfIceTimer < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_CHAINS_OF_ICE);
            m_uiChainsOfIceTimer = 8000 + rand()%6000;
		}
		else m_uiChainsOfIceTimer -= diff;

		if(m_uiDeathCoilTimer < diff)
		{
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_DEATH_COIL_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_DEATH_COIL_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_DEATH_COIL_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_DEATH_COIL_25HC);
            m_uiDeathCoilTimer = 6000 + rand()%4000;
		}
		else m_uiDeathCoilTimer -= diff;

		if(m_uiStrangulateTimer < diff)
		{
			if(Unit *target = SelectEnemyCaster(false))
				DoCast(target , SPELL_STRANGULATE);
			m_uiStrangulateTimer = 60000 + rand()%6000;
		}
		else m_uiStrangulateTimer -= diff;

		if(m_uiFrostStrikeTimer < diff)
		{
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_FROST_STRIKE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_FROST_STRIKE_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_FROST_STRIKE_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_FROST_STRIKE_25HC);
            m_uiFrostStrikeTimer = 5000 + rand()%5000;
		}
		else m_uiFrostStrikeTimer -= diff;

		if(m_uiIcyTouchTimer < diff)
		{
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_ICY_TOUCH_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_ICY_TOUCH_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_ICY_TOUCH_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_ICY_TOUCH_25HC);
            m_uiIcyTouchTimer = 8000 + rand()%4000;
		}
		else m_uiIcyTouchTimer -= diff;

		if(m_uiDeathGripTimer < diff)
		{
            if(m_creature->IsInRange(m_creature->getVictim(), 10.0f, 30.0f, false))
			{
			    DoCast(m_creature->getVictim(), SPELL_DEATH_GRIP); 
			    m_uiDeathGripTimer = 35000 + rand()%1000;
			}
			else m_uiDeathGripTimer = 3000;
		}
		else m_uiDeathGripTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum rogue
{
    // rogue sublety
    SPELL_BLADE_FLURRY          = 65956,
    SPELL_BLIND                 = 65960,
    SPELL_CLOAK_OF_SHADOWS      = 65961,
    SPELL_EVISCERATE_10         = 65957,
    SPELL_EVISCERATE_10HC       = 68094,
    SPELL_EVISCERATE_25         = 68095,
    SPELL_EVISCERATE_25HC       = 68096,
    SPELL_HEMORRHAGE            = 65954,
    SPELL_SHADOWSTEP            = 66178,
    SPELL_FAN_OF_KNIVES_10      = 65955,
    SPELL_FAN_OF_KNIVES_25      = 68098,
    SPELL_FAN_OF_KNIVES_10HC    = 68097,
    SPELL_FAN_0F_KNIVES_25HC    = 68099,
};              

struct MANGOS_DLL_DECL boss_fc_rogueAI : public boss_faction_championAI
{
    boss_fc_rogueAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
	
	uint32 m_uiFanOfKnivesTimer;
	uint32 m_uiBlindTimer;
	uint32 m_uiCloakTimer;
	uint32 m_uiBladeFlurryTimer;
	uint32 m_uiShadowstepTimer;
	uint32 m_uiHemorrhageTimer;
	uint32 m_uiEviscerateTimer;

    void Init()
    {
        m_uiFanOfKnivesTimer = 7000 + rand()%1000;
	    m_uiBlindTimer = 6000 + rand()%3000;
	    m_uiCloakTimer = 20000 + rand()%20000;
	    m_uiBladeFlurryTimer = 10000 + rand()%1000;
	    m_uiShadowstepTimer = 9000 + rand()%2000;
	    m_uiHemorrhageTimer = 5000 + rand()%1000;
	    m_uiEviscerateTimer = 14000 + rand()%2000;
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiFanOfKnivesTimer < diff)
        {
            if(EnemiesInRange(15.0f) > 2)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_FAN_OF_KNIVES_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(m_creature->getVictim(), SPELL_FAN_OF_KNIVES_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_FAN_OF_KNIVES_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature->getVictim(), SPELL_FAN_0F_KNIVES_25HC);
            }
            m_uiFanOfKnivesTimer = 3000;
        }
        else m_uiFanOfKnivesTimer -= diff;

		if(m_uiHemorrhageTimer < diff)
		{
            DoCast(m_creature->getVictim(), SPELL_HEMORRHAGE);
            m_uiHemorrhageTimer = 5000 + rand()%2000;
		}
		else m_uiHemorrhageTimer -= diff;

		if(m_uiEviscerateTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_EVISCERATE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_EVISCERATE_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_EVISCERATE_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_EVISCERATE_25HC);
            m_uiEviscerateTimer = 7000 + rand()%3000;
        }
        else m_uiEviscerateTimer -= diff;

        if(m_uiShadowstepTimer < diff)
		{
			if(m_creature->IsInRange(m_creature->getVictim(), 10.0f, 40.0f))
			{
		 	    DoCast(m_creature->getVictim(), SPELL_SHADOWSTEP);
                m_uiShadowstepTimer = 30000 + rand()%6000;
			}
			else m_uiShadowstepTimer = 2000;
		}
		else m_uiShadowstepTimer -= diff;

		if(m_uiBlindTimer < diff)
		{
			if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			{
                if(m_creature->IsInRange(target, 0.0f, 15.0f, false))
                {
				    DoCast(target, SPELL_BLIND);
				    m_uiBlindTimer = 120000 + rand()%6000;
                }
			    else m_uiBlindTimer = 2000;
            }
            else m_uiBlindTimer = 2000;
		}
		else m_uiBlindTimer -= diff;

		if(m_uiCloakTimer < diff) //If rogue really damaged...
        {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_CLOAK_OF_SHADOWS);  //...She will cloak.
                m_uiCloakTimer = 90000;
            }
            else m_uiCloakTimer = 5000;
        }
        else m_uiCloakTimer -= diff;

		if(m_uiBladeFlurryTimer < diff)
        {
            DoCast(m_creature, SPELL_BLADE_FLURRY);
            m_uiBladeFlurryTimer = 120000 + rand()%5000;
        }
        else m_uiBladeFlurryTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum shamy_enhancement
{
    // shaman enhancement
    SPELL_LAVA_LASH             = 65974,
    SPELL_STORMSTRIKE           = 65970,
};

struct MANGOS_DLL_DECL boss_fc_enh_shammyAI : public boss_faction_championAI
{
    boss_fc_enh_shammyAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
    
	uint32 m_uiHeroismTimer;
	uint32 m_uiShockTimer;
	uint32 m_uiLavaLashTimer;
	uint32 m_uiStormstrikeTimer;

    void Init()
    {
        m_uiShockTimer=3000+rand()%2000;       
	    m_uiLavaLashTimer=4000+rand()%2000;
	    m_uiStormstrikeTimer=5000+rand()%2000;
	    m_uiHeroismTimer=5000+rand()%20000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiHeroismTimer < diff)
        {
            DoCast(m_creature, SPELL_HEROISM);
            m_uiHeroismTimer = 600000;
        }
        else m_uiHeroismTimer -= diff;

		if(m_uiShockTimer < diff)
		{
            if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_EARTH_SHOCK_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_EARTH_SHOCK_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_EARTH_SHOCK_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_EARTH_SHOCK_25HC);
            }
		    m_uiShockTimer = 6000 + rand()%1000;
		}
		else m_uiShockTimer -= diff;

		if(m_uiStormstrikeTimer < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_STORMSTRIKE);
            m_uiStormstrikeTimer = 8000 + rand()%4000;
		}
		else m_uiStormstrikeTimer -= diff;

		if(m_uiLavaLashTimer < diff)
		{
			DoCast (m_creature->getVictim(), SPELL_LAVA_LASH);
            m_uiLavaLashTimer = 8000 + rand()%5000;
		}
		else m_uiLavaLashTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum paladin_retribution
{
    // paladin retribution
    SPELL_AVENGING_WRATH            = 66011,
    SPELL_CRUSADER_STRIKE           = 66003,
    SPELL_DIVINE_STORM              = 66006,
    SPELL_REPENTANCE                = 66008,
    SPELL_JUDGEMENT_OF_COMMAND_10   = 66005,
    SPELL_JUDGEMENT_OF_COMMAND_10HC = 68017,
    SPELL_JUDGEMENT_OF_COMMAND_25   = 68018,
    SPELL_JUDGEMENT_OF_COMMAND_25HC = 68019,
    SPELL_SEAL_OF_COMMAND_10        = 66004,
    SPELL_SEAL_OF_COMMAND_25        = 68021,
    SPELL_SEAL_OF_COMMAND_10HC      = 68020,
    SPELL_SEAL_OF_COMMAND_25HC      = 68022,

    EQUIP_ID_PALY_RETRI_HORDE       = 35015,
};

struct MANGOS_DLL_DECL boss_fc_ret_pallyAI : public boss_faction_championAI
{
    boss_fc_ret_pallyAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) 
    {
        if(m_uiCreatureEntry == 34456)
            SetEquipmentSlots(false, EQUIP_ID_PALY_RETRI_HORDE, -1, -1);
        Init();
    }
    
	bool m_bShieldUsed;
    uint32 m_uiAvengingWrathTimer;
	uint32 m_uiCrusaderStrikeTimer;
	uint32 m_uiDivineShieldTimer;
	uint32 m_uiDivineStormTimer;
	uint32 m_uiHammerOfJusticeTimer;
	uint32 m_uiHandOfProtectionTimer;
	uint32 m_uiJudgementOfCommandTimer;
	uint32 m_uiRepentanceTimer;
    uint32 m_uiCreatureEntry;

    void Init()
    {
        m_uiAvengingWrathTimer = 7000 + rand()%3000;
        m_uiCrusaderStrikeTimer = 4000 + rand()%1000;
	    m_uiDivineShieldTimer = 30000 + rand()%1000;
        m_uiDivineStormTimer = 5000 + rand()%2000;
        m_uiHammerOfJusticeTimer = 1000 + rand()%5000;
        m_uiHandOfProtectionTimer = 40000 + rand()%2000;
        m_uiJudgementOfCommandTimer = 6000;
	    m_uiRepentanceTimer = 2000 + rand()%5000;
        m_bShieldUsed = false;
        m_uiCreatureEntry = m_creature->GetEntry();
    }

    void Aggro(Unit *who)
    {
        boss_faction_championAI::Aggro(who);

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature, SPELL_SEAL_OF_COMMAND_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature, SPELL_SEAL_OF_COMMAND_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature, SPELL_SEAL_OF_COMMAND_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature, SPELL_SEAL_OF_COMMAND_25HC);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

		if(m_uiRepentanceTimer < diff)
        {
            if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
			    DoCast(target, SPELL_REPENTANCE);
            m_uiRepentanceTimer = 60000 + rand()%4000;
        }
        else m_uiRepentanceTimer -= diff;

		if(m_uiCrusaderStrikeTimer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_CRUSADER_STRIKE);
            m_uiCrusaderStrikeTimer = 6000 + rand()%1000;
        }
        else m_uiCrusaderStrikeTimer -= diff;

		if(m_uiAvengingWrathTimer < diff)  //Wrath+shield problem.
        {
            DoCast(m_creature, SPELL_AVENGING_WRATH);
            m_uiAvengingWrathTimer = 300000 + rand()%5000;
        }
        else m_uiAvengingWrathTimer -= diff;

		if(!m_bShieldUsed && m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
        {
            DoCast(m_creature, SPELL_DIVINE_SHIELD);
            m_bShieldUsed = true;
        }
		 
		if(m_uiDivineStormTimer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_DIVINE_STORM);
            m_uiDivineStormTimer = 10000 + rand()%1000;
        }
        else m_uiDivineStormTimer -= diff;
		
        if(m_uiJudgementOfCommandTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_JUDGEMENT_OF_COMMAND_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_JUDGEMENT_OF_COMMAND_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_JUDGEMENT_OF_COMMAND_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_JUDGEMENT_OF_COMMAND_25HC);
            m_uiJudgementOfCommandTimer = 8000 + rand()%1000;
        }
        else m_uiJudgementOfCommandTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};
/**********************************************************
                            PETS
**********************************************************/
enum cat
{
     // cat
    SPELL_CLAW_10               = 67793,
    SPELL_CLAW_10HC             = 67980,
    SPELL_CLAW_25               = 67981,
    SPELL_CLAW_25HC             = 67982,
};

struct MANGOS_DLL_DECL boss_fc_pet_catAI : public boss_faction_championAI
{
    boss_fc_pet_catAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
    
	uint32 m_uiClawTimer;

    void Init()
    {
        m_uiClawTimer=3000;    
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiClawTimer < diff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_CLAW_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_CLAW_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_CLAW_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_CLAW_25HC);
            m_uiClawTimer = 3000 + rand()%2000;
        }
        else m_uiClawTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};

enum felhunter
{
    // zhaagrym
    SPELL_DEVOUR_MAGIC          = 67518,
    SPELL_LOCK                  = 67519, 
};

struct MANGOS_DLL_DECL boss_fc_pet_zhaagrymAI : public boss_faction_championAI
{
    boss_fc_pet_zhaagrymAI(Creature *pCreature) : boss_faction_championAI(pCreature, AI_MELEE) {Init();}
    
	uint32 m_uiDevourMagicTimer;
    uint32 m_uiSpellLockTimer;

    void Init()
    {
        m_uiDevourMagicTimer = 5000;
        m_uiSpellLockTimer = 6000 + rand()%4000; 
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        if(m_uiSpellLockTimer < diff)
		{
			if(Unit *target = SelectEnemyCaster(false))
				DoCast(target , SPELL_LOCK);
			m_uiSpellLockTimer = 15000 + rand()%6000;
		}
		else m_uiSpellLockTimer -= diff;

        if(m_uiDevourMagicTimer < diff)
        {
            switch (urand(0,1))
            {
            case 0:
                if(Unit *target = SelectEnemyCaster(false))
                    DoCast(target , SPELL_DEVOUR_MAGIC);
                break;
            case 1:
                if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                    DoCast(target , SPELL_DEVOUR_MAGIC);
                break;
            }

            m_uiDevourMagicTimer = 3000 + rand()%3000;
        }
		else m_uiDevourMagicTimer -= diff;

        boss_faction_championAI::UpdateAI(diff);
    }
};


/*========================================================*/
CreatureAI* GetAI_boss_fc_resto_druid(Creature *pCreature) {
    return new boss_fc_resto_druidAI (pCreature);
}
CreatureAI* GetAI_boss_fc_resto_shammy(Creature *pCreature) {
    return new boss_fc_resto_shammyAI (pCreature);
}
CreatureAI* GetAI_boss_fc_holy_pally(Creature *pCreature) {
    return new boss_fc_holy_pallyAI (pCreature);
}
CreatureAI* GetAI_boss_fc_holy_priest(Creature *pCreature) {
    return new boss_fc_holy_priestAI (pCreature);
}
CreatureAI* GetAI_boss_fc_shadow_priest(Creature *pCreature) {
    return new boss_fc_shadow_priestAI (pCreature);
}
CreatureAI* GetAI_boss_fc_warlock(Creature *pCreature) {
    return new boss_fc_warlockAI (pCreature);
}
CreatureAI* GetAI_boss_fc_mage(Creature *pCreature) {
    return new boss_fc_mageAI (pCreature);
}
CreatureAI* GetAI_boss_fc_hunter(Creature *pCreature) {
    return new boss_fc_hunterAI (pCreature);
}
CreatureAI* GetAI_boss_fc_boomkin(Creature *pCreature) {
    return new boss_fc_boomkinAI (pCreature);
}
CreatureAI* GetAI_boss_fc_warrior(Creature *pCreature) {
    return new boss_fc_warriorAI (pCreature);
}
CreatureAI* GetAI_boss_fc_deathknight(Creature *pCreature) {
    return new boss_fc_deathknightAI (pCreature);
}
CreatureAI* GetAI_boss_fc_rogue(Creature *pCreature) {
    return new boss_fc_rogueAI (pCreature);
}
CreatureAI* GetAI_boss_fc_enh_shammy(Creature *pCreature) {
    return new boss_fc_enh_shammyAI (pCreature);
}
CreatureAI* GetAI_boss_fc_ret_pally(Creature *pCreature) {
    return new boss_fc_ret_pallyAI (pCreature);
}
CreatureAI* GetAI_boss_fc_pet_cat(Creature *pCreature) {
    return new boss_fc_pet_catAI (pCreature);
}
CreatureAI* GetAI_boss_fc_pet_zhaagrym(Creature *pCreature) {
    return new boss_fc_pet_zhaagrymAI (pCreature);
}

void AddSC_faction_champions()
{
    Script *newscript;
    
    newscript = new Script;
    newscript->Name = "boss_fc_resto_druid";
    newscript->GetAI = &GetAI_boss_fc_resto_druid;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_resto_shammy";
    newscript->GetAI = &GetAI_boss_fc_resto_shammy;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_holy_pally";
    newscript->GetAI = &GetAI_boss_fc_holy_pally;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_holy_priest";
    newscript->GetAI = &GetAI_boss_fc_holy_priest;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_shadow_priest";
    newscript->GetAI = &GetAI_boss_fc_shadow_priest;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_mage";
    newscript->GetAI = &GetAI_boss_fc_mage;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_warlock";
    newscript->GetAI = &GetAI_boss_fc_warlock;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_hunter";
    newscript->GetAI = &GetAI_boss_fc_hunter;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_boomkin";
    newscript->GetAI = &GetAI_boss_fc_boomkin;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_warrior";
    newscript->GetAI = &GetAI_boss_fc_warrior;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_deathknight";
    newscript->GetAI = &GetAI_boss_fc_deathknight;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_rogue";
    newscript->GetAI = &GetAI_boss_fc_rogue;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_enh_shammy";
    newscript->GetAI = &GetAI_boss_fc_enh_shammy;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_fc_ret_pally";
    newscript->GetAI = &GetAI_boss_fc_ret_pally;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fc_pet_cat";
    newscript->GetAI = &GetAI_boss_fc_pet_cat;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fc_pet_zhaagrym";
    newscript->GetAI = &GetAI_boss_fc_pet_zhaagrym;
    newscript->RegisterSelf();
}
