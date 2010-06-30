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
SDName: boss_forgemaster_gafrost
SD%Complete: 0%
SDComment:
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_AGGRO           = -1610051,
    SAY_SLAY1           = -1610052,
    SAY_SLAY2           = -1610054,
    SAY_PHASE2          = -1610055,
    SAY_PHASE3          = -1610056,
    EMOTE_SARONITE      = -1610053,
    SAY_DEATH           = -1610057,

    SPELL_PERMAFROST                 = 70326,
    SPELL_PERMAFROST_AURA            = 68786,
    SPELL_THROW_SARONITE             = 68788,
    SPELL_THUNDERING_STOMP           = 68771,
    SPELL_CHILLING_WAVE              = 68778,
    SPELL_CHILLING_WAVE_H            = 70333, 
    SPELL_DEEP_FREEZE                = 70381,
    SPELL_DEEP_FREEZE_H              = 72930,
    SPELL_FORGE_MACE                 = 68785,
    SPELL_FORGE_MACE_H               = 70335,
    SPELL_FORGE_BLADE                = 68774,
    SPELL_FORGE_BLADE_H              = 70334,

    EQUIP_ID_SWORD  = 49345,
    EQUIP_ID_MACE   = 49344,

    SAY_FREE_SLAVE                  = -1610059,
    SAY_TYRANNUS_OUTRO              = -1610058,

    ACHIEV_DOESNT_GO_TO_ELEVEN      = 4524,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {719.812f, -167.183f, 526.721f,},
    {698.703f, -165.497f, 527.464f,},
    {671.455f, -167.968f, 526.741f,},
};

static LocationsXY MoveLoc[]=
{
    {677.445f, -186.521f, 526.702f},
    {708.190f, -194.619f, 526.805f},
    {687.257f, -193.644f, 526.717f}, 
};

#define HOME_X                      712.927f
#define HOME_Y                      -231.605f

struct MANGOS_DLL_DECL npc_martin_gorkunAI: public ScriptedAI
{
    npc_martin_gorkunAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiOutro_Phase;
    bool m_bIsOutro;
    uint32 creatureEntry;

    uint64 m_uiTyrannusGuid;

    void Reset()
    {
        m_uiOutro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsOutro          = true;
        m_uiTyrannusGuid    = 0;
        creatureEntry = m_creature->GetEntry();
    }

    void SummonHordeSlaves()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_HORDE_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
        }
    }

    void SummonAlySlaves()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_1, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y + urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 20), MoveLoc[0].y + urand(0, 20), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_2, SummonLoc[1].x + urand(0, 10), SummonLoc[1].y - urand(0, 10), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 20), MoveLoc[2].y - urand(0, 20), MoveLoc[2].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_SLAVE_ALY_3, SummonLoc[2].x - urand(0, 20), SummonLoc[2].y - urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x - urand(0, 20), MoveLoc[1].y - urand(0, 20), MoveLoc[1].z);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsOutro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiOutro_Phase)
                {
                case 0:
                    switch (creatureEntry)
                    {
                    case NPC_MARTIN_VICTUS_SLAVE:
                        SummonAlySlaves();
                        break;
                    case NPC_GORKUN_IRONSKULL_SLAVE:
                        SummonHordeSlaves();
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 2000;
                    break;
                case 1:
                    switch (creatureEntry)
                    {
                    case NPC_MARTIN_VICTUS_SLAVE:
                        DoScriptText(SAY_FREE_SLAVE,m_creature);
                        break;
                    case NPC_GORKUN_IRONSKULL_SLAVE:
                        DoScriptText(SAY_FREE_SLAVE,m_creature);
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 2:
                    if(Creature* pTyrannus = m_creature->SummonCreature(NPC_TYRANNUS_INTRO, 695.46f, -156.31f, 546.061f, 4.77f, TEMPSUMMON_TIMED_DESPAWN, 10000))
                    {
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->GetMap()->CreatureRelocation(pTyrannus, 695.46f, -156.31f, 546.061f, 4.77f);
                        pTyrannus->SendMonsterMove(695.46f, -156.31f, 546.061f, SPLINETYPE_NORMAL, pTyrannus->GetSplineFlags(), 1);
                        m_uiTyrannusGuid = pTyrannus->GetGUID();
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 3000;
                    break;
                case 3:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS_OUTRO, pTyrannus);
                    m_bIsOutro = false;
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL boss_GarfrostAI : public ScriptedAI
{
    boss_GarfrostAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool phase2;
    bool phase3;
    uint32 m_uiThrowSaroniteTimer;
    uint32 m_uiThunderingStompTimer;
    uint32 m_uiDeepFreezeTimer;
    uint32 m_uiChillingWaveTimer;

    uint32 TeamInInstance;

    uint32 CheckAura;
    bool hasMoreThanTen;

    void Reset()
    {
        m_uiThrowSaroniteTimer      = 45000;
        m_uiThunderingStompTimer    = 40000;
        m_uiDeepFreezeTimer         = 30000;
        m_uiChillingWaveTimer       = 25000;

        phase2 = false;
        phase3 = false;

        TeamInInstance = GetFaction();

        CheckAura           = 1000;
        hasMoreThanTen      = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GARFROST, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GARFROST, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        DoCast(m_creature, SPELL_PERMAFROST);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_creature->GetHealthPercent() < 66.0f && !phase2)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FORGE_BLADE : SPELL_FORGE_BLADE_H);
            DoScriptText(SAY_PHASE2, m_creature);
            SetEquipmentSlots(false, EQUIP_ID_SWORD, -1, -1);
            m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
            m_uiChillingWaveTimer = 10000;
            phase2 = true;
        }

        if (m_creature->GetHealthPercent() < 33.0f && !phase3)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FORGE_MACE : SPELL_FORGE_MACE_H);
            DoScriptText(SAY_PHASE3, m_creature);
            SetEquipmentSlots(false, EQUIP_ID_MACE, -1, -1);
            m_creature->SetByteValue(UNIT_FIELD_BYTES_2, 0, SHEATH_STATE_MELEE);
            m_uiDeepFreezeTimer = 10000;
            phase3 = true;
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

    void KilledUnit(Unit *victim)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }
    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (CheckAura < uiDiff && !hasMoreThanTen)
        {
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
            {
                Unit *pTarget = Unit::GetUnit(*m_creature, (*iter)->getUnitGuid());
                if (!pTarget || pTarget->GetTypeId() != TYPEID_PLAYER)
                    continue;

                Aura *AuraFrost = pTarget->GetAura(SPELL_PERMAFROST_AURA, EFFECT_INDEX_0);
                if (AuraFrost && AuraFrost->GetStackAmount() > 10)
                {
                    hasMoreThanTen = true;
                    break;
                }
            }
            CheckAura = 1000;
        } else CheckAura -= uiDiff;

        if (m_uiThrowSaroniteTimer < uiDiff)
        {
            DoScriptText(EMOTE_SARONITE, m_creature);
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_THROW_SARONITE);
            m_uiThrowSaroniteTimer = 35000;
        }
        else
            m_uiThrowSaroniteTimer -= uiDiff;

        if (m_uiThunderingStompTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_THUNDERING_STOMP);
            m_uiThunderingStompTimer = 20000;
        }
        else
            m_uiThunderingStompTimer -= uiDiff;

        if (m_uiDeepFreezeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DEEP_FREEZE : SPELL_DEEP_FREEZE_H);
            m_uiDeepFreezeTimer = 35000;
        }
        else
            m_uiDeepFreezeTimer -= uiDiff;

        if (m_uiChillingWaveTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CHILLING_WAVE : SPELL_CHILLING_WAVE_H);
            m_uiChillingWaveTimer = 40000;
        }
        else
            m_uiChillingWaveTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 80)
            EnterEvadeMode();
    }
    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_GARFROST, DONE);

            /* fix this!!!
            if(!m_bIsRegularMode && !hasMoreThanTen)
                m_pInstance->DoCompleteAchievement(ACHIEV_DOESNT_GO_TO_ELEVEN);*/
        }

        if(TeamInInstance == ALLIANCE)
        {
            Creature *pMartin = m_creature->SummonCreature(NPC_MARTIN_VICTUS_SLAVE, 695.46f, -156.31f, 528.061f, 4.77f, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pMartin)
                pMartin->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX() + 15, m_creature->GetPositionY() - 5, m_creature->GetPositionZ());
        }

        if(TeamInInstance == HORDE)
        {
            Creature *pGorkun = m_creature->SummonCreature(NPC_GORKUN_IRONSKULL_SLAVE, 695.46f, -156.31f, 528.061f, 4.77f, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pGorkun)
                pGorkun->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX() + 15, m_creature->GetPositionY() - 5, m_creature->GetPositionZ());
        }
    }
};

CreatureAI* GetAI_boss_Garfrost(Creature* pCreature)
{
    return new boss_GarfrostAI (pCreature);
}

CreatureAI* GetAI_npc_martin_gorkun(Creature* pCreature)
{
    return new npc_martin_gorkunAI (pCreature);
}

void AddSC_boss_Garfrost()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_Garfrost";
    newscript->GetAI = &GetAI_boss_Garfrost;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_martin_gorkun";
    newscript->GetAI = &GetAI_npc_martin_gorkun;
    newscript->RegisterSelf();
}