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
SDName: boss_the_lich_king
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    //TEXTS
    SAY_INTRO1 = -1999926,
    SAY_INTRO2 = -1999927,
    SAY_INTRO3 = -1999928,
    SAY_INTRO4 = -1999929,
    SAY_INTRO5 = -1999930,
    SAY_AGGRO = -1999931,
    SAY_CASTING_WINTER = -1999932,
    SAY_SHATTERING = -1999933,
    SAY_SUMMON_V = -1999934,
    SAY_CASTING_HARV = -1999935,
    SAY_ESC_FROSTM = -1999936,
    SAY_DEVOURED_FROSTM = -1999937,
    SAY_SPECIAL1 = -1999938,
    SAY_SPECIAL2 = -1999939,
    SAY_LOW_HEALTH = -1999940,
    SAY_RANDOM1 = -1999941,
    SAY_RANDOM2 = -1999942,
    SAY_ENRAGE = -1999943,
    SAY_BATTLE1 = -1999944,
    SAY_BATTLE2 = -1999944,
    SAY_BATTLE3 = -1999937,
    SAY_BATTLE4 = -1999938,
    SAY_BATTLE5 = -1999939,
    SAY_BATTLE6 = -1999940,
    SAY_BATTLE7 = -1999941,
    SAY_BATTLE8 = -1999942,
    SAY_BATTLE9 = -1999943,
    SAY_BATTLE10 = -1999944,
    SAY_DEATH = -1999945,

    //SPELLS
    SPELL_SUMMON_HORROR = 70372,
    SPELL_SHOCKWAVE = 72149,
    SPELL_HORROR_ENRAGE = 72143,
    SPELL_INFEST = 70541,
    SPELL_INFEST_H = 0,
    SPELL_NECROTIC_PLAGUE = 70337,
    SPELL_PLAGUE_SIPHON = 74074,
    SPELL_SUMMON_GHOULS = 70358,
    SPELL_SUMMON_VALKYR = 69037,
    SPELL_GRAB = 62712,
    SPELL_WINGS = 74352,
    SPELL_SOUL_REAPER = 69409,
    SPELL_DEFILE = 72754,
    SPELL_HARVEST_SOUL = 68980,
    SPELL_VILE_SPIRITS = 70498,
    SPELL_SPIRIT_BURST = 70503,
    SPELL_LIGHTS_FAVOR = 69382,
    SPELL_RESTORE_SOUL = 72595, 
    SPELL_SOUL_RIP = 69397,
    SPELL_DARK_HUNGER = 69383,
    SPELL_HARVESTED_SOUL = 74322,
    SPELL_REMORSELESS_WINTER = 68981,
    SPELL_PAIN_AND_SUFFERING = 72133,
    SPELL_SUMMON_ICE_SPHERE = 69103,
    SPELL_ICE_PULSE = 69099,
    SPELL_ICE_BURST = 69108,
    SPELL_RAGING_SPIRIT = 69200, 
    SPELL_SOUL_SHRIEK = 69242,
    SPELL_QUAKE = 72262,
    SPELL_FURY_FROSTMOURNE = 70063,
    SPELL_RISE_TELEPORT = 70858,

    NPC_SHAMNLING_HORROR = 37698,

    //PHASES
    PHASE_ONE = 1,
    PHASE_TWO = 2,
    PHASE_THREE = 3,
    PHASE_FINAL = 4,
    PHASE_TRANSITION_1 = 5,
    PHASE_TRANSITION_2 = 6
};
/*
***************************************
******** boss lich king **************
****************************************
*/
struct MANGOS_DLL_DECL boss_the_lich_kingAI : public ScriptedAI
{
    boss_the_lich_kingAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool PhaseDone;
    uint32 Summon_Horror_Timer;
    uint32 Infest_Timer;
    uint32 Summon_Ghouls_Timer;
    uint32 Soul_Reaper_Timer;
    uint32 Defile_Timer;
    uint32 Summon_Valkyr_Timer;
    uint32 Harvest_Soul_Timer;
    uint32 Vile_Spirits_Timer;
    uint32 Remorseless_Winter_Timer;
    uint32 Pain_And_Suffering_Timer;
    uint32 Summon_Ice_Sphere_Timer;
    uint32 Raging_Spirit_Timer;
    uint32 Necrotic_Plague_Timer;
    uint32 Phase_Timer;
    uint8 Phase;
    bool IntroText;

    //timers
    void Reset()
    {
        Phase_Timer = 60000;
        Summon_Horror_Timer = 1000;
        Infest_Timer = 2000;
        Summon_Ghouls_Timer = 1000;
        Soul_Reaper_Timer = 1;
        Defile_Timer = 1;
        Summon_Valkyr_Timer = 1;
        Harvest_Soul_Timer = 6000;
        Vile_Spirits_Timer = 500;
        Remorseless_Winter_Timer = 2500;
        Pain_And_Suffering_Timer = 500;
        Summon_Ice_Sphere_Timer = 1;
        Raging_Spirit_Timer = 1;
        Necrotic_Plague_Timer = 1;
        Phase = 1;
        PhaseDone = false;
    }

    //text, when he gets agroo
    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
    }

    //he will go to the chear if raid gets wiped
    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, FAIL);

		if (Creature* pTirion = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_TIRION_FINAL))))
		{
			if(pTirion->isAlive())
				pTirion->SetVisibility(VISIBILITY_ON);
		}
    }

    //boss yells, when player dies
    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_RANDOM1, m_creature); break;
        case 1: DoScriptText(SAY_RANDOM2, m_creature); break;
        }
    }

    //boss yells, when he dies
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, DONE);
    }

    // despawns adds
    void DespawnAdds(std::list<uint64>& List)
    {
        if (!List.empty())
        {
            for (std::list<uint64>::iterator itr = List.begin(); itr != List.end(); ++itr)
            {
                if (Creature* pSummon = (Creature*)Unit::GetUnit((*m_creature), *itr))
                    pSummon->ForcedDespawn();
            }
            List.clear();
        }
    }

	void JustSummoned(Creature* pSummoned)
	{
		pSummoned->SetInCombatWithZone();
	}

    // boss event
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;


        switch (Phase)
        {
        case PHASE_ONE:
            if (!PhaseDone)
            {
                if (m_creature->GetHealthPercent() < 70.0f)
                    Phase = 5;
            } 

            if (Summon_Horror_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_SUMMON_HORROR);
                Summon_Horror_Timer = 60000;
            } else Summon_Horror_Timer -= uiDiff;

            if (Summon_Ghouls_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_SUMMON_GHOULS);
                Summon_Ghouls_Timer = 50000;
            } else Summon_Ghouls_Timer -= uiDiff;

            //this spell effect is bugged, if you want to disable it, put this to /* */
            if (Infest_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_INFEST);
                Infest_Timer = 20000;
            } else Infest_Timer -= uiDiff;

            if (Necrotic_Plague_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_NECROTIC_PLAGUE);
                Necrotic_Plague_Timer = 30000;
            } else Necrotic_Plague_Timer -= uiDiff;
            DoMeleeAttackIfReady();
            break; 

        case PHASE_TWO:
            if (!PhaseDone)
            {
                if (m_creature->GetHealthPercent() < 40.0f)
                    Phase = 6;
            }

            if (Summon_Valkyr_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_SUMMON_VALKYR);
                Summon_Valkyr_Timer = 50000;
            } else Summon_Valkyr_Timer -= uiDiff;

            if (Infest_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_INFEST);
                Infest_Timer = 20000;
            } else Infest_Timer -= uiDiff;

            if (Soul_Reaper_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_SOUL_REAPER);
                Soul_Reaper_Timer = 30000;
            } else Soul_Reaper_Timer -= uiDiff;

            if (Defile_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEFILE);
                Defile_Timer = 35000;
            } else Defile_Timer -= uiDiff;


            DoMeleeAttackIfReady();
            break;


        case PHASE_THREE: //phase is not completed, because phase forstmourne is in inside of frostmourne phase

            if (Defile_Timer < uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_DEFILE);
                Defile_Timer = 35000;
            } else Defile_Timer -= uiDiff;

            if (Soul_Reaper_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_SOUL_REAPER);
                Soul_Reaper_Timer = 30000;
            } else Soul_Reaper_Timer -= uiDiff;

            if (Harvest_Soul_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_HARVEST_SOUL);
                Harvest_Soul_Timer = 60000;
            } else Harvest_Soul_Timer -= uiDiff;

            if (Vile_Spirits_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_VILE_SPIRITS);
                Vile_Spirits_Timer = 30000;
            } else Vile_Spirits_Timer -= uiDiff;

            DoMeleeAttackIfReady();
            break;

        case PHASE_TRANSITION_1:
            if (!PhaseDone)
            {
                if (Phase_Timer <= uiDiff)
                {
                    DoCast(m_creature, SPELL_QUAKE);
                    Phase = 2;
                    Phase_Timer = 60000;
                    break;
                } else Phase_Timer -= uiDiff;
            }

            if (Remorseless_Winter_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_REMORSELESS_WINTER);
                Remorseless_Winter_Timer = 60000;
            } else Remorseless_Winter_Timer -= uiDiff; 

            if (Pain_And_Suffering_Timer < uiDiff) 
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                Pain_And_Suffering_Timer = 3000;
            } else Pain_And_Suffering_Timer -= uiDiff;

            if (Summon_Ice_Sphere_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_SUMMON_ICE_SPHERE);
                Summon_Ice_Sphere_Timer = 5000;
            } else Summon_Ice_Sphere_Timer -= uiDiff;

            if (Raging_Spirit_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_RAGING_SPIRIT);
                Raging_Spirit_Timer = 20000;
            } else Raging_Spirit_Timer -= uiDiff;
            DoMeleeAttackIfReady(); 
            break;

        case PHASE_TRANSITION_2:
            if (!PhaseDone)
            {
                if (Phase_Timer <= uiDiff)
                {
                    DoCast(m_creature, SPELL_QUAKE);
                    Phase = 2;
                    Phase_Timer = 60000;
                    break;
                } else Phase_Timer -= uiDiff;
            }

            if (Remorseless_Winter_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_REMORSELESS_WINTER);
                Remorseless_Winter_Timer = 60000;
            } else Remorseless_Winter_Timer -= uiDiff; 

            if (Pain_And_Suffering_Timer < uiDiff) 
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(m_creature->getVictim(), SPELL_PAIN_AND_SUFFERING);
                Pain_And_Suffering_Timer = 3000;
            } else Pain_And_Suffering_Timer -= uiDiff;

            if (Summon_Ice_Sphere_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_SUMMON_ICE_SPHERE);
                Summon_Ice_Sphere_Timer = 5000;
            } else Summon_Ice_Sphere_Timer -= uiDiff;

            if (Raging_Spirit_Timer < uiDiff) 
            {
                DoCast(m_creature->getVictim(), SPELL_RAGING_SPIRIT);
                Raging_Spirit_Timer = 20000;
            } else Raging_Spirit_Timer -= uiDiff;
            DoMeleeAttackIfReady(); 
            break;
        }
    }
};

CreatureAI* GetAI_boss_the_lich_king(Creature* pCreature)
{
    return new boss_the_lich_kingAI(pCreature);
}

#define GOSSIP_START     "Start event!"

// Tirion 
struct MANGOS_DLL_DECL npc_tirion_finalAI : public ScriptedAI
{
    npc_tirion_finalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool m_bHasPlayerNear;
    bool m_bIsIntro;
    uint64 m_uiPlayerGUID;
    uint32 m_uiSpeech_Timer;
    uint32 m_uiIntro_Phase;

    void Reset()
    {
        m_uiPlayerGUID      = 0;
        m_uiSpeech_Timer    = 3000;
        m_bIsIntro          = false;
        m_uiIntro_Phase     = 0;
    }

    void BeginEvent(Player* pPlayer)
    {
        m_uiPlayerGUID      = pPlayer->GetGUID();
        m_bIsIntro          = true;
        m_uiSpeech_Timer    = 3000;
        m_uiIntro_Phase     = 0;

		// start here
		if (Creature* pLichKing = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_LICH_KING))))
		{
			if(pLichKing->isAlive())
			{
				pLichKing->GetMotionMaster()->MovePoint(0, 497.83f, -2125.89f, 1040.86f);
				pLichKing->SetInCombatWithZone();
				pLichKing->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			}
		}
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_bIsIntro)
        {
            if(m_uiSpeech_Timer < uiDiff)
            {
                switch(m_uiIntro_Phase)
                {
                case 0:
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 1:
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 7000;
                    break;
                case 2:
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;
                case 3:
                    m_bIsIntro = false;
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_tirion_final(Creature* pCreature)
{
    return new npc_tirion_finalAI(pCreature);
}

bool GossipHello_npc_tirion_final(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = (ScriptedInstance *) pCreature->GetInstanceData();

    if(pInstance->GetData(TYPE_LICH_KING) != DONE)
	    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tirion_final(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		pCreature->SetVisibility(VISIBILITY_OFF);
        ((npc_tirion_finalAI*)pCreature->AI())->BeginEvent(pPlayer);
    }

    return true;
}

/* 
************************************************** *******
*************mob summoned honnor*****************
************************************************** *******/

struct MANGOS_DLL_DECL mob_summon_honor_targetAI : public ScriptedAI
{
    mob_summon_honor_targetAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}
    void MoveInLineOfSight(Unit* pWho) {}
    void AttackStart(Unit* pWho) {}

    void JustSummoned(Creature* pSummoned)
    {
        if (!m_pInstance)
            return;

        /*if (Creature* pTheLichKing = ((Creature*)Unit::GetUnit(*m_creature, m_pInstance->GetData64(NPC_SLADRAN))))
        {
            float fPosX, fPosY, fPosZ;
            pSladran->GetPosition(fPosX, fPosY, fPosZ);
            pSummoned->GetMotionMaster()->MovePoint(0, fPosX, fPosY, fPosZ);
        }*/
    }

    void UpdateAI(const uint32 diff) {}
};

CreatureAI* GetAI_mob_summon_honor_target(Creature* pCreature)
{
    return new mob_summon_honor_targetAI(pCreature);
}

void AddSC_boss_the_lich_king()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_the_lich_king";
    NewScript->GetAI = &GetAI_boss_the_lich_king;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "npc_tirion_final";
    NewScript->GetAI = &GetAI_npc_tirion_final;
    NewScript->pGossipHello = &GossipHello_npc_tirion_final;
    NewScript->pGossipSelect = &GossipSelect_npc_tirion_final;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_summon_honor";
    //NewScript->GetAI = &GetAI_mob_summon_honor;
    NewScript->RegisterSelf();
}

