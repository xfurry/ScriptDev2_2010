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
SDName: boss_xt002
SD%Complete: 95%
SDComment: need core support for light and gravity bomb. correct number of adds in 25man missing
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //xt yells
    SAY_AGGRO				= -1603038,
    SAY_DEATH				= -1603030,
    SAY_TANCTRUM			= -1603037,
    SAY_SLAY_01				= -1603036,
    SAY_SLAY_02				= -1603035,
    SAY_BERSERK				= -1603031,
    SAY_ADDS				= -1603032,
    SAY_HEART_OPEN			= -1603034,
    SAY_HEART_CLOSE			= -1603033,

    //xt-002
    SPELL_TANCTRUM			= 62776,
    SPELL_LIGHT_BOMB		= 63018,
    SPELL_LIGHT_BOMB_H		= 65121,
    SPELL_GRAVITY_BOMB		= 63024,
    SPELL_GRAVITY_BOMB_H	= 64234,
    SPELL_ENRAGE			= 47008,
    SPELL_STUN				= 3618,

    // hard mode
    SPELL_HEARTBREAK        = 65737,
    SPELL_HEARTBREAK_H      = 64193,
    SPELL_VOIDZONE          = 64203,
    SPELL_VOIDZONE_H        = 64235,
    SPELL_LIFE_SPARK        = 64210,
    SPELL_STATIC_CHARGED    = 64227,

    NPC_VOIDZONE            = 34001,
    NPC_LIFESPARK           = 34004,

    //heart of the deconstructor
    SPELL_EXPOSED_HEART		= 63849,

    //XE-321 Boombot
    SPELL_BOOM				= 38831,			// replacing real spell

    //XM-024 Pummeller
    SPELL_CLEAVE			= 8374,
    SPELL_TRAMPLE			= 5568,
    SPELL_UPPERCUT			= 10966,

    //NPC ids
    //NPC_HEART				= 33329,
    NPC_SCRAPBOT			= 33343,
    NPC_BOOMBOT				= 33346,
    NPC_PUMMELER			= 33344, 


    // Achievs
    ACHIEV_HEARTBREAKER     = 3058,
    ACHIEV_HEARTBREAKER_H   = 3059,
    ACHIEV_DECONSTRUCT_FAST = 2937,
    ACHIEV_DECONSTRUCT_FAST_H = 2938,
};

// void zone
struct MANGOS_DLL_DECL mob_voidzoneAI : public ScriptedAI
{
    mob_voidzoneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);
        Reset();
    }

    uint32 Spell_Timer;
    uint32 DespawnTimer;
    bool m_bIsRegularMode;

    void Reset()
    {
        Spell_Timer = 4000;
        DespawnTimer = 180000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Spell_Timer < diff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                        i->getSource()->DealDamage(i->getSource(), m_bIsRegularMode ? 5000 : 7500, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
                }
            } 
            Spell_Timer = 4000;
        }else Spell_Timer -= diff;  

        if (DespawnTimer < diff)
            m_creature->ForcedDespawn();
        else DespawnTimer -= diff; 

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_voidzone(Creature* pCreature)
{
    return new mob_voidzoneAI(pCreature);
}

// lifespark
struct MANGOS_DLL_DECL mob_lifesparkAI : public ScriptedAI
{
    mob_lifesparkAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Spell_Timer;

    void Reset()
    {  
        Spell_Timer = 4000;
        DoCast(m_creature, SPELL_STATIC_CHARGED, false);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Spell_Timer < diff)
        {
            Map *map = m_creature->GetMap();
            if (map->IsDungeon())
            {
                Map::PlayerList const &PlayerList = map->GetPlayers();

                if (PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 80)
                        i->getSource()->DealDamage(i->getSource(), 500, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NATURE, NULL, false);
                }
            } 
            Spell_Timer = 3000;

        }else Spell_Timer -= diff;  


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lifespark(Creature* pCreature)
{
    return new mob_lifesparkAI(pCreature);
}

// XM-024 Pummeller
struct MANGOS_DLL_DECL mob_pummelerAI : public ScriptedAI
{
    mob_pummelerAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Spell_Timer;

    void Reset()
    {
        Spell_Timer = urand(15000, 25000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Spell_Timer < diff)
        {
            switch(urand(0, 2))
            {
            case 0:
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                break;
            case 1:
                DoCast(m_creature->getVictim(), SPELL_TRAMPLE);
                break;
            case 2:
                DoCast(m_creature->getVictim(), SPELL_UPPERCUT);
                break;
            }
            Spell_Timer = urand(15000, 25000);
        }else Spell_Timer -= diff;        

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_pummeler(Creature* pCreature)
{
    return new mob_pummelerAI(pCreature);
}

// XE-321 Boombot
struct MANGOS_DLL_DECL mob_boombotAI : public ScriptedAI
{
    mob_boombotAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth()){
            uiDamage = 0;
            DoCast(m_creature, SPELL_BOOM);
        }
    }

    void DoMeleeAttackIfReady()
    {
        //If we are within range melee the target
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
        {
            DoCast(m_creature, SPELL_BOOM);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_boombot(Creature* pCreature)
{
    return new mob_boombotAI(pCreature);
}

// Heart of the Deconstructor
struct MANGOS_DLL_DECL mob_xtheartAI : public ScriptedAI
{
    mob_xtheartAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        SetCombatMovement(false);
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 Exposed_Timer;
    uint32 heartdamage;

    void Reset()
    {
        Exposed_Timer = 30000;
        DoCast(m_creature, SPELL_EXPOSED_HEART);
        heartdamage = 0;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        heartdamage += uiDamage;
    }

    void JustDied(Unit* pKiller)
    {
        if (heartdamage != 0)
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_XT002))))
                if (pTemp->isAlive())
                {
                    pTemp->DealDamage(pTemp, heartdamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    heartdamage = 0;
                }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Exposed_Timer < diff)
        {
            if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_XT002))))
                if (pTemp->isAlive())
                    pTemp->DealDamage(pTemp, heartdamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            heartdamage = 0;
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }else Exposed_Timer -= diff;

    }

};

CreatureAI* GetAI_mob_xtheart(Creature* pCreature)
{
    return new mob_xtheartAI(pCreature);
}

float XtAddX[4];
float XtAddY[4];
float XtAddZ[4];

//XT-002 Deconstructor
struct MANGOS_DLL_DECL boss_xt002AI : public ScriptedAI
{
    boss_xt002AI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        XtAddX[0] = 792.706f; XtAddY[0] = 64.033f; XtAddZ[0] = 413.632f;
        XtAddX[1] = 879.750f; XtAddY[1] = 64.815f; XtAddZ[1] = 409.804f;
        XtAddX[2] = 896.488f; XtAddY[2] = -93.018f; XtAddZ[2] = 411.731f;
        XtAddX[3] = 791.016f; XtAddY[3] = -83.516f; XtAddZ[3] = 409.804f;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lScrapbotsGUIDList;
    std::list<uint64> m_lBoombotsGUIDList;
    std::list<uint64> m_lPummelerGUIDList;

    uint32 Heart_Timer;
    uint32 Light_Bomb_Timer;
    uint32 Gravity_Bomb_Timer;
    uint32 Tanctrum_Timer;
    uint32 add_summon_delay;
    uint32 Enrage_Timer;
    uint32 Range_Check_Timer;
    uint32 Addcount;
    uint32 VoidZoneTimer;
    uint32 LifeSparkTimer;
    uint8 hp;
    uint32 hpDelayTimer;
    uint64 m_uiHeartGUID;
    bool heart1;
    bool heart2;
    bool heart3;
    bool enrage;
    bool phase2;
    bool add1;
    bool add2;
    bool add3;
    bool add4;

    uint64 pLightBombTarGUID;
    uint64 pGravityBombTarGUID;

    bool isHardMode;
    bool hasMoreHealth;

    uint32 uiEncounterTimer;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        Light_Bomb_Timer = 5000; //7 seconds the first 14 secs all after(7 secs in 25man)
        Gravity_Bomb_Timer = 30000; //11 seconds first 18 secs all after(11 secs in 25man)
        Tanctrum_Timer = 38000; // 38 seconds first 40 secs all after
        add_summon_delay = 5000;
        Enrage_Timer = 600000;
        Range_Check_Timer = 1000;
        VoidZoneTimer = 60000;
        LifeSparkTimer = urand (5000, 10000);
        hp = m_bIsRegularMode? 0.5 : 0.6;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Addcount = 0;
        m_uiHeartGUID   = 0;
        heart1 = false;
        heart2 = false;
        heart3 = false;
        enrage = false;
        phase2 = false;
        add1 = false;
        add2 = false;
        add3 = false;
        add4 = false;
        isHardMode = false;
        hasMoreHealth = false;
        m_lScrapbotsGUIDList.clear();
        m_lBoombotsGUIDList.clear();
        m_lPummelerGUIDList.clear();

        pLightBombTarGUID = 0;
        pGravityBombTarGUID = 0;

        uiEncounterTimer = 0;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, DONE);
            if(isHardMode)
                m_pInstance->SetData(TYPE_XT002_HARD, DONE);
        }

        DoScriptText(SAY_DEATH, m_creature);
        if (!m_lScrapbotsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lScrapbotsGUIDList.begin(); itr != m_lScrapbotsGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                    pTemp->ForcedDespawn();
        }
        if (!m_lBoombotsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lBoombotsGUIDList.begin(); itr != m_lBoombotsGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                    pTemp->ForcedDespawn();
        }
        if (!m_lPummelerGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lPummelerGUIDList.begin(); itr != m_lPummelerGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                    pTemp->ForcedDespawn();
        }

        // Deconstruct Fast
        if (uiEncounterTimer < 205000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_DECONSTRUCT_FAST : ACHIEV_DECONSTRUCT_FAST_H);
        }

        // Heartbreaker
        if (isHardMode)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_HEARTBREAKER : ACHIEV_HEARTBREAKER_H);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002, IN_PROGRESS);
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY_01, m_creature); break;
        case 1: DoScriptText(SAY_SLAY_02, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Achiev timer
        uiEncounterTimer += diff;

        if (Light_Bomb_Timer < diff && !phase2)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                // fix spell range
                //DoCast(pTarget, m_bIsRegularMode ? SPELL_LIGHT_BOMB : SPELL_LIGHT_BOMB_H);
                pLightBombTarGUID = pTarget->GetGUID();
            }

            if(isHardMode)
                LifeSparkTimer = 9000;

            Light_Bomb_Timer = urand(10000, 14000);//m_bIsRegularMode ? 14000 :7000;
        }else Light_Bomb_Timer -= diff;   

        if (Gravity_Bomb_Timer < diff && !phase2)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                // fix spell range
                //DoCast(pTarget, m_bIsRegularMode ? SPELL_GRAVITY_BOMB : SPELL_GRAVITY_BOMB_H);
                pGravityBombTarGUID = pTarget->GetGUID();
            }

            if(isHardMode)
                VoidZoneTimer = 9000;

            Gravity_Bomb_Timer = urand(25000, 30000);//18000 :11000; 
        }else Gravity_Bomb_Timer -= diff;  

        if (Tanctrum_Timer < diff && !phase2)
        {
            DoCast(m_creature, SPELL_TANCTRUM);
            DoScriptText(SAY_TANCTRUM, m_creature);
            Tanctrum_Timer = 40000;
        }else Tanctrum_Timer -= diff;

        if (Enrage_Timer < diff && !enrage && !phase2)
        {
            DoCast(m_creature, SPELL_ENRAGE);
            if (m_creature->HasAura(SPELL_ENRAGE))
            {
                enrage = true;
                DoScriptText(SAY_BERSERK, m_creature);
            }
            else
                Enrage_Timer = 5000;
        }else Enrage_Timer -= diff;

        if (Range_Check_Timer < diff)
        {
            if (!m_lScrapbotsGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lScrapbotsGUIDList.begin(); itr != m_lScrapbotsGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.01);
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
            }
            if (!m_lBoombotsGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lBoombotsGUIDList.begin(); itr != m_lBoombotsGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
            }
            Range_Check_Timer = 1000;
        }else Range_Check_Timer -= diff;

        // Hard mode
        
        if (m_pInstance->GetData(TYPE_XT002) == SPECIAL && !isHardMode)
        {
            if (Creature* pTemp = m_pInstance->instance->GetCreature(m_uiHeartGUID))
            {
                if (!pTemp->isAlive())
                {
                    DoScriptText(SAY_HEART_CLOSE, m_creature);
                    m_creature->RemoveAurasDueToSpell(SPELL_STUN);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->AI()->AttackStart(m_creature->getVictim());

                    DoCast(m_creature, m_bIsRegularMode ? SPELL_HEARTBREAK : SPELL_HEARTBREAK_H);
                    hpDelayTimer = 1000;
                    hasMoreHealth = true;
                    isHardMode = true;
                }
            }
        }
        
        if(isHardMode)
        {
            phase2 = false;

            if(hpDelayTimer < diff && hasMoreHealth)
            {
                m_creature->SetHealth(m_creature->GetMaxHealth()+ (m_creature->GetMaxHealth() * hp));
                hasMoreHealth = false;
            }else hpDelayTimer -= diff;

            if (LifeSparkTimer < diff)
            {
                //TODO: fix the target -> should be the same with the light bomb
                //if (Unit* pTarget = Unit::GetUnit(*m_creature, pLightBombTarGUID))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    Creature * LifeSpark = m_creature->SummonCreature(NPC_LIFESPARK, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                    if(m_bIsRegularMode)
                        LifeSpark->SetHealth(50400);
                }
                LifeSparkTimer = 60000;
            }else LifeSparkTimer -= diff;

            if (VoidZoneTimer < diff)
            {
                //TODO: fix the target -> should be the same with the gravity bomb
                //if (Unit* pTarget = Unit::GetUnit(*m_creature, pGravityBombTarGUID))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(NPC_VOIDZONE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000);         
                VoidZoneTimer = 60000;
            }else VoidZoneTimer -= diff;
        }

        if (!phase2 && !heart1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 75)
        {
            Heart_Timer = 30000;
            heart1 = true;
            phase2 = true;
            add1 = false;
            add2 = false;
            add3 = false;
            add4 = false;
            add_summon_delay = 5000;
            DoScriptText(SAY_HEART_OPEN, m_creature);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(m_creature, SPELL_STUN);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if(Creature *Heart = m_creature->SummonCreature(NPC_HEART, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
            {
                m_uiHeartGUID = Heart->GetGUID();
                if(!m_bIsRegularMode)
                    Heart->SetMaxHealth(7199999);
            }

            if (m_pInstance)
                m_pInstance->SetData(TYPE_XT002, SPECIAL);
        }

        if (phase2 && Heart_Timer < diff)
        {
            DoScriptText(SAY_HEART_CLOSE, m_creature);
            m_creature->RemoveAurasDueToSpell(SPELL_STUN);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->AI()->AttackStart(m_creature->getVictim());
            phase2 = false;
            Light_Bomb_Timer = 7000;
            Gravity_Bomb_Timer = 11000;
            Tanctrum_Timer = 38000;

            if (m_pInstance)
                m_pInstance->SetData(TYPE_XT002, IN_PROGRESS);

        }else Heart_Timer -= diff;

        if(!isHardMode)
        {
            if (!phase2 && heart1 && !heart2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
            {
                Heart_Timer = 30000;
                heart2 = true;
                phase2 = true;
                add1 = false;
                add2 = false;
                add3 = false;
                add4 = false;
                add_summon_delay = 5000;
                DoScriptText(SAY_HEART_OPEN, m_creature);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(m_creature, SPELL_STUN);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if(Creature *Heart = m_creature->SummonCreature(NPC_HEART, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                {
                    m_uiHeartGUID = Heart->GetGUID();
                    if(!m_bIsRegularMode)
                        Heart->SetMaxHealth(7199999);
                }

                //if (m_pInstance)
                    //m_pInstance->SetData(TYPE_XT002, SPECIAL);
            }

            if (!phase2 && heart1 && heart2 && !heart3 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25)
            {
                Heart_Timer = 30000;
                heart3 = true;
                phase2 = true;
                add1 = false;
                add2 = false;
                add3 = false;
                add4 = false;
                add_summon_delay = 5000;
                DoScriptText(SAY_HEART_OPEN, m_creature);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(m_creature, SPELL_STUN);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if(Creature *Heart = m_creature->SummonCreature(NPC_HEART, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                {
                    m_uiHeartGUID = Heart->GetGUID();
                    if(!m_bIsRegularMode)
                        Heart->SetMaxHealth(7199999);
                }

                //if (m_pInstance)
                    //m_pInstance->SetData(TYPE_XT002, SPECIAL);
            }

            if (phase2 && add_summon_delay < diff)
            {
                if (!add1)
                {
                    Addcount = 0;
                    do{
                        if (Creature* pTemp = m_creature->SummonCreature(NPC_SCRAPBOT, XtAddX[0], XtAddY[0], XtAddZ[0], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                            pTemp->AI()->AttackStart(m_creature->getVictim());
                            m_lScrapbotsGUIDList.push_back(pTemp->GetGUID());
                            Addcount++;
                        }} while(Addcount<3);
                        Addcount = 0;
                        do{
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_BOOMBOT, XtAddX[0], XtAddY[0], XtAddZ[0], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                m_lBoombotsGUIDList.push_back(pTemp->GetGUID());
                                Addcount++;
                            }} while(Addcount<1);
                            add1 = true;
                            add_summon_delay = 4000;
                }
                if (!add2 && add1)
                {
                    Addcount = 0;
                    do{
                        if (Creature* pTemp = m_creature->SummonCreature(NPC_SCRAPBOT, XtAddX[1], XtAddY[1], XtAddZ[1], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                            pTemp->AI()->AttackStart(m_creature->getVictim());
                            m_lScrapbotsGUIDList.push_back(pTemp->GetGUID());
                            Addcount++;
                        }} while(Addcount<3);
                        Addcount = 0;
                        do{
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_BOOMBOT, XtAddX[1], XtAddY[1], XtAddZ[1], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                m_lBoombotsGUIDList.push_back(pTemp->GetGUID());
                                Addcount++;
                            }} while(Addcount<1);
                            add2 = true;
                            DoScriptText(SAY_ADDS, m_creature);
                            add_summon_delay = 1000;
                }
                if (!add3 && add2 && add1)
                {
                    Addcount = 0;
                    do{
                        if (Creature* pTemp = m_creature->SummonCreature(NPC_SCRAPBOT, XtAddX[2], XtAddY[2], XtAddZ[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                            pTemp->AI()->AttackStart(m_creature->getVictim());
                            m_lScrapbotsGUIDList.push_back(pTemp->GetGUID());
                            Addcount++;
                        }} while(Addcount<3);
                        Addcount = 0;
                        do{
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_BOOMBOT, XtAddX[2], XtAddY[2], XtAddZ[2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                m_lBoombotsGUIDList.push_back(pTemp->GetGUID());
                                Addcount++;
                            }} while(Addcount<1);
                            add3 = true;
                            add_summon_delay = 4000;
                }
                if (!add4 && add3 && add2 && add1)
                {
                    if (Creature* pTemp = m_creature->SummonCreature(NPC_PUMMELER, XtAddX[3], XtAddY[3], XtAddZ[3], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            pTemp->AddThreat(pTarget,0.0f);
                            pTemp->AI()->AttackStart(pTarget);
                            m_lPummelerGUIDList.push_back(pTemp->GetGUID());
                        }
                        Addcount = 0;
                        do{
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_SCRAPBOT, XtAddX[3], XtAddY[3], XtAddZ[3], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                            {
                                pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                                pTemp->AI()->AttackStart(m_creature->getVictim());
                                m_lScrapbotsGUIDList.push_back(pTemp->GetGUID());
                                Addcount++;
                            }} while(Addcount<3);
                            Addcount = 0;
                            do{
                                if (Creature* pTemp = m_creature->SummonCreature(NPC_BOOMBOT, XtAddX[3], XtAddY[3], XtAddZ[3], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                                {
                                    pTemp->AddThreat(m_creature->getVictim(),1000.0f);
                                    pTemp->AI()->AttackStart(m_creature->getVictim());
                                    m_lBoombotsGUIDList.push_back(pTemp->GetGUID());
                                    Addcount++;
                                }} while(Addcount<1);
                                add4 = true;
                                add_summon_delay = 30000;
                }
            }else add_summon_delay -= diff;
        }

        if (!phase2)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_xt002(Creature* pCreature)
{
    return new boss_xt002AI(pCreature);
}

void AddSC_boss_xt002()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_xt002";
    NewScript->GetAI = GetAI_boss_xt002;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_pummeler";
    NewScript->GetAI = &GetAI_mob_pummeler;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_boombot";
    NewScript->GetAI = &GetAI_mob_boombot;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_xtheart";
    NewScript->GetAI = &GetAI_mob_xtheart;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_voidzone";
    NewScript->GetAI = &GetAI_mob_voidzone;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_lifespark";
    NewScript->GetAI = &GetAI_mob_lifespark;
    NewScript->RegisterSelf();
}