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
SDName: Boss_Rajaxx
SD%Complete: 0
SDComment: Place Holder
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "ruins_of_ahnqiraj.h"

#define GOSSIP_START  "Let's find out."
 
enum
{
    SAY_ANDOROV_READY   =   -1509003,
    SAY_ANDOROV_INTRO2  =   -1509029,
    SAY_ANDOROV_ATTACK  =   -1509030,
    SAY_ANDOROV_INTRO   =   -1509004,
    
    SAY_WAVE3           =   -1509005,
    SAY_WAVE4           =   -1509006,
    SAY_WAVE5           =   -1509007,
    SAY_WAVE6           =   -1509008,
    SAY_WAVE7           =   -1509009,
    SAY_WAVE8           =   -1509010,
    SAY_UNK1            =   -1509011,
    SAY_UNK2            =   -1509012,
    SAY_UNK3            =   -1509013,
    SAY_DEATH           =   -1509014,
    SAY_DEAGGRO         =   -1509015,
    SAY_KILLS_ANDOROV   =   -1509016,                       //Yell when realm complete quest 8743 for world event
    SAY_COMPLETE_QUEST  =   -1509017,                       //Yell when realm complete quest 8743 for world event
    EMOTE_FRENZY        =   -1000001,
    //General Rajaxx
    SPELL_DISARM            =   6713,
    SPELL_FRENZY            =   8269,
    SPELL_SUMMON_PLAYER     =   20477,
    SPELL_THUNDERCRASH      =   25599,
    
    //NPC General Andorov

    SPELL_AURA_OF_COMMAND   =   25516,
    SPELL_BASH              =   25515,
    SPELL_STRIKE            =   22591,

    NPC_SWARMGUARD_NEEDLER  =   15344,
    NPC_QIRAJI_WARRIOR      =   15387
};

struct MoveLocations
{
    float x, y, z, an;
};

struct SpawnLocations
{
    uint32 id;
    float x, y, z, an;
};

static SpawnLocations NPCs[]=
{
    {NPC_KALDOREI_ELITE,-8871.37f,1650.34f,21.386f,5.49113f},
    {NPC_KALDOREI_ELITE,-8872.51f,1648.88f,21.386f,5.62072f},   
    {NPC_KALDOREI_ELITE,-8874.36f,1646.08f,21.386f,5.69533f},
    {NPC_KALDOREI_ELITE,-8875.29f,1644.89f,21.386f,5.69533f},
    {NPC_GENERAL_ANDOROV,-8873.42f,1647.67f,21.386f,5.69141f}
};

static MoveLocations Andorov[]=
{
    {-8872,1628,21.386f},
    {-8919.6f,1550,21.38f},
    {-8939.8f,1550.3f,21.58f}
};

static SpawnLocations WaveOne[]=
{
    {15387, -9012.73f, 1559.7f, 21.3877f, 3.07796f},
    {15387, -9013.22f, 1552.06f, 21.3877f, 3.07796f},
    {15387, -9013.77f, 1543.49f, 21.3863f, 3.07796f},
    {15387, -9014.38f, 1533.8f, 21.3863f, 3.07796f},
    {15344, -9007.9f, 1543.82f, 21.3867f, 3.10152f},
    {15344, -9007.6f, 1551.29f, 21.4505f, 3.10152f},
    {15391, -9021.57f, 1550.71f, 21.5396f, 2.97586f}
};

static SpawnLocations WaveTwo[]=
{
    {15387, -9029.46f, 1515.14f, 21.3873f, 2.0648f},
    {15387, -9037.19f, 1510.98f, 21.398f, 2.0648f},
    {15387, -9045.89f, 1506.29f, 21.4754f, 2.0648f},
    {15344, -9041.85f, 1498.79f, 21.9046f, 2.0648f},
    {15344, -9034.38f, 1502.82f, 21.615f, 2.0648f},
    {15344, -9025.83f, 1507.42f, 21.4418f, 2.0648f},
    {15392, -9041.76f, 1517.72f, 21.3877f, 2.0648f}
};

static SpawnLocations WaveThree[]=
{
    {15389, -9025, 1595.24f, 21.3867f, 4.0393f},
    {15387, -9019.72f, 1601.85f, 21.5376f, 4.0393f},
    {15344, -9012.63f, 1595.78f, 21.3866f, 4.00395f},
    {15344, -9021.2f, 1589.84f, 21.3866f, 4.03223f},
    {15344, -9030.41f, 1589.14f, 21.3866f, 3.94976f},
    {15344, -9030.8f, 1598.04f, 21.3866f, 3.97725f},
    {15344, -9027.72f, 1607.21f, 21.4041f, 4.00867f} 
};

static SpawnLocations WaveFour[]=
{
    {15390, -9084.72f, 1526.44f, 21.3864f, 1.82841f},
    {15344, -9083.12f, 1520.36f, 21.3864f, 1.82841f},
    {15344, -9076.62f, 1522.07f, 21.3864f, 1.82841f},
    {15344, -9090.98f, 1518.29f, 21.3864f, 1.82841f},
    {15387, -9081.5f, 1513.86f, 21.3864f, 1.82841f},
    {15387, -9089.45f, 1511.77f, 21.42f, 1.82841f},
    {15387, -9075.02f, 1515.57f, 21.3876f, 1.82841f}
};

static SpawnLocations WaveFive[]=
{
    {15386, -9077.43f, 1625.86f, 21.3873f, 3.3725f},
    {15387, -9073.02f, 1623.79f, 21.3873f, 3.36464f},
    {15387, -9074.51f, 1630.35f, 21.3873f, 3.36464f},
    {15344, -9067.97f, 1616.7f, 21.397f, 3.36464f},
    {15344, -9069.66f, 1624.16f, 21.387f, 3.36464f},
    {15344, -9071.27f, 1631.26f, 21.389f, 3.36464f},
    {15344, -9072.92f, 1638.55f, 21.3867f, 3.36464f}
};

static SpawnLocations WaveSix[]=
{
    {15388, -9136.52f, 1547.56f, 21.3871f, 1.51111f},
    {15344, -9140.45f, 1542.85f, 21.3871f, 1.53074f},
    {15344, -9134.11f, 1542.6f, 21.3871f, 1.53074f},
    {15387, -9134.36f, 1536.43f, 21.3877f, 1.53074f},
    {15387, -9141.08f, 1536.69f, 21.3877f, 1.53074f},
    {15387, -9148.35f, 1536.99f, 21.3863f, 1.53074f},
    {15387, -9126.9f, 1536.13f, 21.4453f, 1.53074f}
};

static SpawnLocations WaveSeven[]=
{
    {15385, -9109.64f, 1586.35f, 21.387f, 2.5714f},
    {15344, -9098.42f, 1589.87f, 21.3873f, 2.53998f},
    {15344, -9103.71f, 1582.17f, 21.3875f, 2.53998f},
    {15344, -9108.99f, 1574.48f, 21.4036f, 2.53998f},
    {15387, -9102.06f, 1569.72f, 21.393f, 2.53998f},
    {15387, -9096.78f, 1577.42f, 21.3871f, 2.53998f},
    {15387, -9091.7f, 1584.81f, 21.3871f, 2.53998f}
};

struct MANGOS_DLL_DECL boss_rajaxxAI : public ScriptedAI
{
    boss_rajaxxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }    

    ScriptedInstance* m_pInstance;

    uint32 m_uiThunderCrash_Timer;
    uint32 m_uiDisarm_Timer;
    uint32 m_uiWave_Timer;

    uint8 m_uiWaveCount;

    bool m_bHasEnraged;

    void Reset()
    {
        //Rajaxx's spells
        m_uiThunderCrash_Timer = 25000;
        m_uiDisarm_Timer = 5000;

        m_bHasEnraged = false;

        //his army
        m_uiWaveCount = 0;
        m_uiWave_Timer = 1000;
    }
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH,m_creature);
    }

    void KilledUnit(Unit *pKilled)
    {
        if (!m_creature->isInCombat())
            DoScriptText(SAY_DEAGGRO,m_creature,pKilled);
    }

    void StartEvent()
    {
        if (!m_pInstance)
            return;
        if (Unit* pAndorov = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_ANDOROV)))
        {
            pAndorov->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            DoScriptText(SAY_ANDOROV_INTRO,pAndorov);
            DoScriptText(SAY_ANDOROV_INTRO2,pAndorov);
            pAndorov->GetMotionMaster()->MovePoint(0,Andorov[0].x, Andorov[0].y,Andorov[0].z);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_bHasEnraged && (m_creature->GetHealth()+uiDamage)*100 / m_creature->GetMaxHealth() < 30)
        {
            DoCast(m_creature,SPELL_FRENZY);
            DoScriptText(EMOTE_FRENZY,m_creature);
            m_bHasEnraged = true;
        }
    }

    void AttackWave(uint32 entry)
    {
        if (!m_pInstance)
           return;

        if (!entry)
            return;

        if (Creature *pCommander = (Creature*)Unit::GetUnit(*m_creature,m_pInstance->GetData64(entry)))
        {
            pCommander->CallForHelp(20.0f);
            pCommander->SetInCombatWithZone();
            if (Creature* pAndorov = (Creature*)Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_ANDOROV)))
            {
                if (pAndorov->isInCombat())
                    pAndorov->CombatStop(true);
                pAndorov->AI()->AttackStart(pCommander);
                pAndorov->CallForHelp(5.0f);
            }

            //kaldoer attack
            std::list<Creature*> m_lKaldorei;
            GetCreatureListWithEntryInGrid(m_lKaldorei,pCommander,NPC_KALDOREI_ELITE,DEFAULT_VISIBILITY_DISTANCE);
            if (!m_lKaldorei.empty())
                for(std::list<Creature*>::iterator iter = m_lKaldorei.begin(); iter != m_lKaldorei.end(); ++iter)
                    if ((*iter))
                        (*iter)->AI()->AttackStart(pCommander);
        }
    }
    
    void ResetEvent()
    {
        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_RAJAXX,NOT_STARTED);
        m_uiWaveCount = 0;
        m_uiWave_Timer = 1000;

        //for despawning current army
        uint32 CreatureDespawnEntries[]=
        {{NPC_SWARMGUARD_NEEDLER},{NPC_QIRAJI_WARRIOR},{NPC_CAPTAIN_QEEZ},{NPC_CAPTAIN_TUUBID},{NPC_CAPTAIN_DRENN},{NPC_CAPTAIN_XURREM},{NPC_MAJOR_YEGGETH},{NPC_MAJOR_PAKKON},{NPC_COLONEL_ZERRAN},{NPC_KALDOREI_ELITE},{NPC_GENERAL_ANDOROV}};

        for (uint8 i=0; i < 11; ++i)
        {
            std::list<Creature*> m_lCreatures;
            GetCreatureListWithEntryInGrid(m_lCreatures,m_creature,CreatureDespawnEntries[i],MAX_VISIBILITY_DISTANCE);
            if (!m_lCreatures.empty())
                for(std::list<Creature*>::iterator iter = m_lCreatures.begin(); iter != m_lCreatures.end(); ++iter)
                    if ((*iter) && (*iter)->isAlive())
                        (*iter)->setDeathState(JUST_DIED);
        }

        // Summon them again
        Creature* pAndorov = m_creature->SummonCreature(NPC_GENERAL_ANDOROV,NPCs[4].x,NPCs[4].y,NPCs[4].z,NPCs[4].an,TEMPSUMMON_CORPSE_DESPAWN,0);
        if (pAndorov)
        {
            pAndorov->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pAndorov->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
        }
    }
    
    bool IsWaveDead(uint32 entry)
    {
        if (!entry)
            return true;

        //simplify this!
        if (Unit* pCommander = Unit::GetUnit(*m_creature,m_pInstance->GetData64(entry)))
            if (pCommander->isAlive())
                if (!pCommander->isInCombat())
                {
                    ResetEvent();
                    return false;
                }
                else
                    return false;
            else
                return true;
        else
            return false;
    }

    uint32 WaveConversion(uint8 Count)
   {
        switch(Count)
        {
        case 1: return DATA_QEEZ; break;
        case 2: return DATA_TUUBID; break;
        case 3: return DATA_DRENN; break;
        case 4: return DATA_XURREM; break;
        case 5: return DATA_YEGGETH; break;
        case 6: return DATA_PAKKON; break;
        case 7: return DATA_ZERRAN; break;
        case 8: return DATA_RAJAXX; break;
        default: return NULL; break;
        }
    }

    void DoSummonArmy()
    {
        //Wave 1
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveOne)); ++i)
            m_creature->SummonCreature(WaveOne[i].id,WaveOne[i].x,WaveOne[i].y,WaveOne[i].z,WaveOne[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);
        
        //Wave 2
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveTwo)); ++i)
            m_creature->SummonCreature(WaveTwo[i].id,WaveTwo[i].x,WaveTwo[i].y,WaveTwo[i].z,WaveTwo[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Wave 3
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveThree)); ++i)
            m_creature->SummonCreature(WaveThree[i].id,WaveThree[i].x,WaveThree[i].y,WaveThree[i].z,WaveThree[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Wave 4
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveFour)); ++i)
            m_creature->SummonCreature(WaveFour[i].id,WaveFour[i].x,WaveFour[i].y,WaveFour[i].z,WaveFour[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Wave 5
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveFive)); ++i)
            m_creature->SummonCreature(WaveFive[i].id,WaveFive[i].x,WaveFive[i].y,WaveFive[i].z,WaveFive[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Wave 6
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveSix)); ++i)
            m_creature->SummonCreature(WaveSix[i].id,WaveSix[i].x,WaveSix[i].y,WaveSix[i].z,WaveSix[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Wave 7
        for (uint8 i = 0; i < 7; ++i)  // for (uint8 i = 0; i < (sizeof(WaveSeven)); ++i)
            m_creature->SummonCreature(WaveSeven[i].id,WaveSeven[i].x,WaveSeven[i].y,WaveSeven[i].z,WaveSeven[i].an,TEMPSUMMON_CORPSE_TIMED_DESPAWN,1000);

        //Kaldorei Elite's
        for(uint8 i = 0; i < 4 ; ++i)
        {
            Unit *pKaldorei = m_creature->SummonCreature(NPC_KALDOREI_ELITE,NPCs[i].x,NPCs[i].y,NPCs[i].z,NPCs[i].an,TEMPSUMMON_CORPSE_DESPAWN,0);
                if (pKaldorei)
                if (Unit *pAndorov = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_ANDOROV)))
                    pKaldorei->GetMotionMaster()->MoveFollow(pAndorov,2,i+1.5);
        }
    }  

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_pInstance->GetData(TYPE_RAJAXX) == IN_PROGRESS)
        {
            if (m_uiWave_Timer < uiDiff || IsWaveDead(WaveConversion(m_uiWaveCount)))
            {
                ++m_uiWaveCount;
                AttackWave(WaveConversion(m_uiWaveCount));
                if (m_uiWaveCount > 2)
                    DoScriptText(SAY_WAVE3 - m_uiWaveCount + 3,m_creature);
                m_uiWave_Timer = 180000;
            }
            else
                m_uiWave_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_uiThunderCrash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_THUNDERCRASH);
            m_uiThunderCrash_Timer = 30000;
        }
        else
            m_uiThunderCrash_Timer -= uiDiff;

        if (m_uiDisarm_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_DISARM);
            m_uiDisarm_Timer = 15000;
        }
        else
            m_uiDisarm_Timer -= uiDiff;

        if (Player* pPlayer = GetPlayerAtMinimumRange(DEFAULT_VISIBILITY_DISTANCE))           //port player back if he gets too far
            DoCast(pPlayer, SPELL_SUMMON_PLAYER);

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_andorovAI : public ScriptedAI
{
    npc_andorovAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }
    ScriptedInstance* m_pInstance;

    uint32 m_uiWaypoint;
    uint32 m_uiWaitForOthers_Timer;
    uint32 m_uiCommandAura_Timer;
    uint32 m_uiBash_Timer;
    uint32 m_uiStrike_Timer;

    bool m_bCanMoveNext;
    bool m_bWaypointEnd;
    bool m_bSummonOnCreate;

    void Reset() 
    {
        m_uiWaypoint = 0; 

        m_bCanMoveNext = false;
        m_bWaypointEnd = false;

        m_uiWaitForOthers_Timer = 500;

        m_uiCommandAura_Timer = 10000;
        m_uiBash_Timer = 5000;
        m_uiStrike_Timer = 15000;
        m_bSummonOnCreate = false;
        //m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void Aggro(Unit* pWho)
    {
        std::list<Creature*> m_lKaldorei;
        GetCreatureListWithEntryInGrid(m_lKaldorei,m_creature,NPC_KALDOREI_ELITE,DEFAULT_VISIBILITY_DISTANCE);
        if (!m_lKaldorei.empty())
            for(std::list<Creature*>::iterator iter = m_lKaldorei.begin(); iter != m_lKaldorei.end(); ++iter)
                if ((*iter))
                    (*iter)->AI()->AttackStart(pWho);
    }

    void JustDied(Unit *pKiller)
    {
        if (Unit *pRajaxx = Unit::GetUnit(*m_creature,m_pInstance->GetData64(DATA_RAJAXX)))
            if (pRajaxx->isAlive())
                DoScriptText(SAY_KILLS_ANDOROV,pRajaxx);
    }

   void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE || m_bWaypointEnd)
            return;

        uint32 uiCount = (sizeof(Andorov)/sizeof(MoveLocations));

        m_uiWaypoint = uiPointId+1;

        if (uiCount == m_uiWaypoint)
        {
            DoScriptText(SAY_ANDOROV_READY,m_creature);
            DoScriptText(SAY_ANDOROV_ATTACK,m_creature);
            m_creature->HandleEmoteCommand(EMOTE_STATE_READY1H);
            std::list<Creature*> m_lKaldorei;
            GetCreatureListWithEntryInGrid(m_lKaldorei,m_creature,NPC_KALDOREI_ELITE,10.0f);
            if (!m_lKaldorei.empty())
                for(std::list<Creature*>::iterator iter = m_lKaldorei.begin(); iter != m_lKaldorei.end(); ++iter)
                    if ((*iter))
                        (*iter)->HandleEmoteCommand(EMOTE_STATE_READY1H);
            m_bWaypointEnd = true;
        }

        m_bCanMoveNext = true;
        m_uiWaitForOthers_Timer = 500;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (!m_bSummonOnCreate)
        {
            ((boss_rajaxxAI*)m_creature->AI())->DoSummonArmy();
           m_bSummonOnCreate = true;
        }

        if (m_bWaypointEnd)
            m_pInstance->SetData(TYPE_RAJAXX, IN_PROGRESS);
        else
        {
            if (m_bCanMoveNext && m_uiWaitForOthers_Timer < uiDiff)
            {
                m_bCanMoveNext = false;
                if (m_bWaypointEnd)
                    m_creature->GetMotionMaster()->Clear();
                else
                    m_creature->GetMotionMaster()->MovePoint(m_uiWaypoint, Andorov[m_uiWaypoint].x, Andorov[m_uiWaypoint].y, Andorov[m_uiWaypoint].z); 
            }
            else
                m_uiWaitForOthers_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBash_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_BASH);
            m_uiBash_Timer = 30000;
        }
        else
            m_uiBash_Timer -= uiDiff;

        if (m_uiStrike_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_STRIKE);
            m_uiStrike_Timer = 15000;
        }
        else
            m_uiStrike_Timer -= uiDiff;

        if (m_uiCommandAura_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_AURA_OF_COMMAND);
            m_uiCommandAura_Timer = 15000;
        }
        else
            m_uiCommandAura_Timer -= uiDiff; 

        DoMeleeAttackIfReady();
    }
};

bool GossipHello_npc_andorov(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START , GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->SEND_GOSSIP_MENU(14442, pCreature->GetGUID());       //insert into `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) values('14442','<Andorov looks at you grimly.>$B$BIf it bleeds, we can kill it.','','0','1','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0');

    return true;
}

bool GossipSelect_npc_andorov(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        pPlayer->CLOSE_GOSSIP_MENU(); 
        pCreature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        ((boss_rajaxxAI*)pCreature->AI())->StartEvent();
        break;
    }
    return true;
}

CreatureAI* GetAI_boss_rajaxx(Creature* pCreature)
{
    return new boss_rajaxxAI(pCreature);
}

CreatureAI* GetAI_npc_andorov(Creature* pCreature)
{
    return new npc_andorovAI(pCreature);
}

void AddSC_boss_rajaxx()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_andorov";
    newscript->GetAI = &GetAI_npc_andorov;
    newscript->pGossipHello = &GossipHello_npc_andorov;
    newscript->pGossipSelect = &GossipSelect_npc_andorov;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_rajaxx";
    newscript->GetAI = &GetAI_boss_rajaxx;
    newscript->RegisterSelf();
}