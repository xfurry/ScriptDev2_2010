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
SDName: pit_of_saron
SD%Complete: 0
SDComment:
SDCategory: Pit of Saron
EndScriptData */

/* ContentData
EndContentData */

#include "precompiled.h"
#include "pit_of_saron.h"

/******** JAINA & SYLVANAS *******/
//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {445.941f, 199.304f, 528.707f},
    {452.197f, 214.461f, 528.710f},
    {444.204f, 232.011f, 528.710f},
};

static LocationsXY MoveLoc[]=
{
    {489.709f, 195.165f, 528.709f}, 
    {482.365f, 221.354f, 528.709f}, 
    {475.318f, 253.910f, 528.709f}, 
};

enum
{
    SAY_SPEECH_SYLVANAS1    = -1610047,
    SAY_SPEECH_SYLVANAS2    = -1610048,
    SAY_SPEECH_SYLVANAS3    = -1610049,
    SAY_SPEECH_SYLVANAS4    = -1610050,

    SAY_SPEECH_JAINA1       = -1610042,
    SAY_SPEECH_JAINA2       = -1610043,
    SAY_SPEECH_JAINA3       = -1610044,
    SAY_SPEECH_JAINA4       = -1610045,
    SAY_SPEECH_JAINA5       = -1610046,

    SAY_TYRANNUS1           = -1610200,
    SAY_TYRANNUS2           = -1610201,
    SAY_TYRANNUS3           = -1610202,
    SAY_TYRANNUS4           = -1610203,
    SAY_TYRANNUS5           = -1610204, 
    SPELL_NECROTIC_POWER    = 69347,
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_pos_startAI: public ScriptedAI
{
    npc_sylvanas_jaina_pos_startAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;
    uint32 creatureEntry;

    uint64 m_uiTyrannusGuid;

    void Reset()
    {
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;
        m_uiTyrannusGuid    = 0;
        creatureEntry = m_creature->GetEntry();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_INTRO, NOT_STARTED);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->isInAccessablePlaceFor(m_creature) && !m_bIsIntro && pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 50) && m_creature->IsWithinLOSInMap(pWho))
        {
            if(m_pInstance && m_creature->GetPositionZ() > 525.0f)
                if(m_pInstance->GetData(TYPE_INTRO) != DONE)
                {
                    m_bIsIntro = true;
                    m_uiSpeech_Timer = 5000;
                }
        }
    }

    void SummonHordeChampions()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_HORDE, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y - urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 40), MoveLoc[0].y - urand(0, 10), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_HORDE, SummonLoc[1].x + urand(0, 20), SummonLoc[1].y + urand(0, 20), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_HORDE, SummonLoc[2].x + urand(0, 20), SummonLoc[2].y + urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }
    }

    void SummonAlyChampions()
    {
        for (uint8 i = 0; i < 5; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_ALLIANCE, SummonLoc[0].x + urand(0, 20), SummonLoc[0].y - urand(0, 20), SummonLoc[0].z, SummonLoc[0].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + urand(0, 40), MoveLoc[0].y - urand(0, 10), MoveLoc[0].z);
        }

        for (uint8 i = 5; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_ALLIANCE, SummonLoc[1].x + urand(0, 20), SummonLoc[1].y + urand(0, 20), SummonLoc[1].z, SummonLoc[1].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 40), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 15; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_ALLIANCE, SummonLoc[2].x + urand(0, 20), SummonLoc[2].y + urand(0, 20), SummonLoc[2].z, SummonLoc[2].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[2].x + urand(0, 40), MoveLoc[2].y + urand(0, 40), MoveLoc[2].z);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_INTRO, IN_PROGRESS);

                    if(Creature* pTyrannus = m_creature->SummonCreature(NPC_TYRANNUS_INTRO, 526.501f, 237.639f, 543.686f, 3.431f, TEMPSUMMON_TIMED_DESPAWN, 40000))
                    {
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        pTyrannus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                        pTyrannus->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                        pTyrannus->GetMotionMaster()->MoveIdle();
                        pTyrannus->GetMap()->CreatureRelocation(pTyrannus, 526.501f, 237.639f, 543.686f, 3.431f);
                        pTyrannus->SendMonsterMove(526.501f, 237.639f, 543.686f, SPLINETYPE_NORMAL, pTyrannus->GetSplineFlags(), 1);
                        DoScriptText(SAY_TYRANNUS1, pTyrannus);
                        m_uiTyrannusGuid = pTyrannus->GetGUID();
                    }

                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        SummonAlyChampions();
                        break;
                    case NPC_SYLVANAS_PART1:
                        SummonHordeChampions();
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 1:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        DoScriptText(SAY_SPEECH_JAINA1, m_creature);
                        break;
                    case NPC_SYLVANAS_PART1:
                        DoScriptText(SAY_SPEECH_SYLVANAS1, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 2:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS2, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 13000;
                    break;
                case 3:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS3, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 4:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                    {
                        pTyrannus->CastSpell(m_creature, SPELL_NECROTIC_POWER, false);
                        DoScriptText(SAY_TYRANNUS4, pTyrannus);
                    }
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        DoScriptText(SAY_SPEECH_JAINA2, m_creature);
                        break;
                    case NPC_SYLVANAS_PART1:
                        DoScriptText(SAY_SPEECH_SYLVANAS2, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 5:
                    if(Creature* pTyrannus = m_pInstance->instance->GetCreature(m_uiTyrannusGuid))
                        DoScriptText(SAY_TYRANNUS5, pTyrannus);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 6:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        DoScriptText(SAY_SPEECH_JAINA3, m_creature);
                        m_uiSpeech_Timer = 5000;
                        break;
                    case NPC_SYLVANAS_PART1:
                        m_uiSpeech_Timer = 500;
                        break;
                    }
                    ++m_uiIntro_Phase;
                    break;
                case 7:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        DoScriptText(SAY_SPEECH_JAINA4, m_creature);
                        break;
                    case NPC_SYLVANAS_PART1:
                        DoScriptText(SAY_SPEECH_SYLVANAS3, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 8:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_PART1:
                        DoScriptText(SAY_SPEECH_JAINA5, m_creature);
                        break;
                    case NPC_SYLVANAS_PART1:
                        DoScriptText(SAY_SPEECH_SYLVANAS4, m_creature);
                        break;
                    }
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_INTRO,DONE);
                    ++m_uiIntro_Phase;
                    m_bIsIntro = false;
                    m_uiSpeech_Timer = 10000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_sylvanas_jaina_pos_start(Creature* pCreature)
{
    return new npc_sylvanas_jaina_pos_startAI (pCreature);
}

void AddSC_pit_of_saron()
{
    Script *newscript;

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_pos_start;
    newscript->Name = "npc_slyvanas_jaina_pos_start";
    newscript->RegisterSelf();
}
