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
SDName: culling_of_stratholme
SD%Complete: 5%
SDComment: Placeholder
SDCategory: Culling of Stratholme
EndScriptData */

#include "precompiled.h"
#include "culling_of_stratholme.h"
#include "escort_ai.h"
#include "WorldPacket.h"
#include "Weather.h"

/*###
## npc_mike
###*/

enum
{
   SAY_MIKE01                = -1557270,
   SAY_FORRESTER02           = -1557271,
   SAY_JAMES03               = -1557272,
   SAY_SIABI04               = -1557273,
   SAY_MIKE05                = -1557274,
   SAY_CORICKS06             = -1557275,
   SAY_GRIAN07               = -1557276,
   SAY_CORICKS08             = -1557277,
   SAY_JAMES09               = -1557278,
   SAY_FORRESTER10           = -1557279,

   EMOTE_SHOT                = 5,
   EMOTE_TALK                = 1,
   EMOTE_POINT               = 25,
   EMOTE_NO                  = 274,
   EMOTE_LAUGH               = 11
};

struct MANGOS_DLL_DECL npc_mikeAI : public ScriptedAI
{
    npc_mikeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    uint64 m_uiForesterGUID;  
    uint64 m_uiJamesGUID;  
    uint64 m_uiSiabiGUID;
    uint64 m_uiCorricksGUID;
    uint64 m_uiGryanGUID;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void MoveInLineOfSight(Unit* who)
    {
      if (!who)
          return;

      if(!m_pInstance) return;

           if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 15 && who->GetPositionZ() > 99.50f && m_uiPhase == 0)
           {
             m_creature->SetUInt64Value(UNIT_FIELD_TARGET, who->GetGUID());
             m_uiPhase = 1;
           }
    }

    void TavernEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_MIKE01, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 1:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              m_uiStepTimer = 5000;
              break;
           case 2:
              m_uiForesterGUID = m_pInstance->GetData64(NPC_HEARTHSINGER_FORRESTEN);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiForesterGUID))
                 DoScriptText(SAY_FORRESTER02, pTemp);
              m_uiStepTimer = 6000;
              break;
           case 3:
              m_uiJamesGUID = m_pInstance->GetData64(NPC_FOOTMAN_JAMES);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 DoScriptText(SAY_JAMES03, pTemp);
              m_uiStepTimer = 5000;
              break;
           case 4:
              m_uiSiabiGUID = m_pInstance->GetData64(NPC_FRAS_SIABI);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 DoScriptText(SAY_SIABI04, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 pTemp->HandleEmoteCommand(EMOTE_SHOT);
              m_uiStepTimer = 5000;
              break;
           case 6:
              m_creature->GetMotionMaster()->MovePoint(0, 1554.849f, 588.465f, 99.775f);
              m_uiStepTimer = 3000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiSiabiGUID))
                 pTemp->HandleEmoteCommand(EMOTE_LAUGH);
              m_uiStepTimer = 3000;
              break;
           case 8:
              DoScriptText(SAY_MIKE05, m_creature);
              m_uiStepTimer = 2000;
              break;
           case 9:
              m_creature->HandleEmoteCommand(EMOTE_SHOT);
              m_uiStepTimer = 1000;
              break;
           case 10:
              m_uiCorricksGUID = m_pInstance->GetData64(NPC_MAL_CORRICKS);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 DoScriptText(SAY_CORICKS06, pTemp);
              m_uiStepTimer = 4000;
              break;
           case 11:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              m_uiGryanGUID = m_pInstance->GetData64(NPC_GRYAN_STOUTMANTLE);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiGryanGUID))
                 DoScriptText(SAY_GRIAN07, pTemp);
              m_uiStepTimer = 11000;
              break;
           case 12:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiCorricksGUID))
                 DoScriptText(SAY_CORICKS08, pTemp);
              m_creature->GetMotionMaster()->MovePoint(0, 1549.609f, 575.544f, 100.052f);
              m_uiStepTimer = 2000;
              break;
           case 13:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 DoScriptText(SAY_JAMES09, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 14:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiJamesGUID))
                 pTemp->HandleEmoteCommand(EMOTE_TALK);
              m_uiStepTimer = 5000;
              break;
           case 15:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiForesterGUID))
                 DoScriptText(SAY_FORRESTER10, pTemp);
              m_uiPhase = 2;
              break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               TavernEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_roger
###*/

enum
{
  SAY_ROGER01                     = -1557280,
  SAY_ROGER02                     = -1557281,
  SAY_ROGER03                     = -1557282,
  SAY_ROGER04                     = -1557283,
};

struct MANGOS_DLL_DECL npc_rogerAI : public ScriptedAI
{
    npc_rogerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartRoger()
    {
      m_uiPhase = 1;
    }

    void FirstCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 7000;
              break;
           case 1:
              MoveToPoint(m_creature, 1590.055f, 615.727f, 99.795f, 7000);
              m_uiStepTimer = 6900;
              break;
           case 2:
              MoveToPoint(m_creature, 1584.039f, 622.049f, 99.907f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 3:
              MoveToPoint(m_creature, 1578.787f, 623.924f, 99.855f, 2000);
              m_uiStepTimer = 2500;
              break;
           case 4:
              DoScriptText(SAY_ROGER01, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 5:
              MoveToPoint(m_creature, 1579.393f, 624.018f, 99.886f, 900);
              m_uiStepTimer = 2000;
              break;
           case 6:
              DoScriptText(SAY_ROGER02, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 7:
              MoveToPoint(m_creature, 1579.387f, 623.198f, 99.837f, 300);
              m_uiStepTimer = 1000;
              break;
           case 8:
              DoScriptText(SAY_ROGER03, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 9:
              MoveToPoint(m_creature, 1575.576f, 619.935f, 99.422f, 1500);
              m_uiStepTimer = 2000;
              break;
           case 10:
              MoveToPoint(m_creature, 1575.833f, 620.471f, 99.466f, 300);
              m_uiStepTimer = 1000;
              break;
           case 11:
              DoScriptText(SAY_ROGER04, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 12:
              MoveToPoint(m_creature, 1580.215f, 624.368f, 99.924f, 2000);
              m_uiStepTimer = 1900;
              break;
           case 13:
              MoveToPoint(m_creature, 1587.471f, 618.181f, 99.850f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 14:
              MoveToPoint(m_creature, 1592.646f, 590.888f, 99.151f, 11000);
              m_uiStepTimer = 13000;
              break;
           case 15:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMotionMaster()->MovementExpired(false);
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FirstCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_morigan
###*/

enum
{
  SAY_MORIGAN01                     = -1557284,
  SAY_PERELLI02                     = -1557285,
  SAY_MORIGAN03                     = -1557286,
  SAY_PERELLI04                     = -1557287,
  SAY_MORIGAN05                     = -1557288,
  SAY_PERELLI06                     = -1557289,
  SAY_MORIGAN07                     = -1557290,
};

struct MANGOS_DLL_DECL npc_moriganAI : public ScriptedAI
{
    npc_moriganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiPerelliGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartMorigan()
    {
      m_uiPhase = 1;
    }

    void SecondCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_MORIGAN01, m_creature);
              m_uiStepTimer = 6000;
              break;
           case 1:
              m_uiPerelliGUID = m_pInstance->GetData64(NPC_PERELLI);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI02, pTemp);
              m_uiStepTimer = 2000;
              break;
           case 2:
              m_creature->GetMotionMaster()->MovePoint(0, 1564.138f, 668.343f, 102.058f);
              m_uiStepTimer = 2000;
              break;
           case 3:
              m_creature->GetMotionMaster()->MovePoint(0, 1567.956f, 667.776f, 102.094f);
              m_uiStepTimer = 1500;
              break;
           case 4:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1569.615f, 668.859f, 102.180f);
              m_uiStepTimer = 2000;
              break;
           case 5:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 1000;
              break;
           case 6:
              DoScriptText(SAY_MORIGAN03, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 7:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
              {
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
              }
              m_uiStepTimer = 3000;
              break;
           case 8:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI04, pTemp);
              m_uiStepTimer = 3000;
              break;
           case 9:
              DoScriptText(SAY_MORIGAN05, m_creature);
              m_uiStepTimer = 9000;
              break;
           case 10:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 DoScriptText(SAY_PERELLI06, pTemp);
              m_uiStepTimer = 6000;
              break;
           case 11:
              DoScriptText(SAY_MORIGAN07, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 12:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiPerelliGUID))
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1576.119f, 657.675f, 102.09f, 5000);
              m_uiStepTimer = 4900;
              break;
           case 13:
              MoveToPoint(m_creature, 1586.040f, 646.113f, 100.910f, 6000);
              m_uiStepTimer = 5900;
              break;
           case 14:
              MoveToPoint(m_creature, 1609.189f, 697.134f, 106.902f, 23000);
              m_uiStepTimer = 24900;
              break;
           case 15:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               SecondCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};
     
/*###
## npc_jena
###*/

enum
{
  SAY_JENA01           = -1557291,
  SAY_MARTHA02         = -1557292,
  SAY_JENA03           = -1557293,
  SAY_JENA04           = -1557294,
  SAY_MARTHA05         = -1557295,

};

struct MANGOS_DLL_DECL npc_jenaAI : public ScriptedAI
{
    npc_jenaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiMarthaGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartJena()
    {
      m_uiPhase = 1;
    }

    void ThirdCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1605.546f, 744.869f, 114.731f);
              m_uiStepTimer = 1900;
              break;
           case 1:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1614.967f, 743.673f, 114.063f);
              m_uiStepTimer = 4900;
              break;
           case 2:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1623.848f, 729.251f, 112.410f);
              m_uiStepTimer = 6900;
              break;
           case 3:
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1633.460f, 726.261f, 113.518f);
              m_uiStepTimer = 4000;
              break;
           case 4:
              m_uiMarthaGUID = m_pInstance->GetData64(NPC_MARTHA);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_JENA01, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->RemoveAurasDueToSpell(58925);
                 pTemp->GetMotionMaster()->MovePoint(0, 1635.918f, 724.357f, 113.561f);
              }
              m_uiStepTimer = 1000;
              break;
           case 6:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->GetMotionMaster()->MovementExpired(false);
                 pTemp->GetMotionMaster()->MovePoint(0, 1636.721f, 725.88f, 113.561f);
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
              }
              m_uiStepTimer = 1000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 DoScriptText(SAY_MARTHA02, pTemp);
              m_uiStepTimer = 4000;
              break;
           case 8:
              DoScriptText(SAY_JENA03, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 9:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              m_creature->GetMotionMaster()->MovePoint(0, 1629.278f, 727.894f, 112.636f);
              m_uiStepTimer = 1500;
              break;
           case 10:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                 pTemp->GetMap()->CreatureRelocation(pTemp, 1640.089f, 725.766f, 113.561f, 4.77f);
                 pTemp->SendMonsterMove(1640.089f, 725.766f, 113.561f, SPLINETYPE_NORMAL , pTemp->GetSplineFlags(), 1500);
              }
              m_creature->GetMotionMaster()->MovementExpired(false);
              m_creature->GetMotionMaster()->MovePoint(0, 1629.452f, 729.416f, 112.712f);
              m_uiStepTimer = 1000;
              break;
           case 11:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 pTemp->GetMotionMaster()->MovePoint(0, 1640.103f, 725.522f, 113.561f);
              m_uiStepTimer = 500;
              break;
           case 12:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 pTemp->CastSpell(pTemp, 58925, false);
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 1500;
              break;
           case 13:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 1000;
              break;
           case 14:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_JENA04, m_creature);
              m_uiStepTimer = 3000;
              break;
           case 15:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiMarthaGUID))
              {
                 pTemp->RemoveAurasDueToSpell(58925);
                 DoScriptText(SAY_MARTHA05, pTemp);
                 MoveToPoint(pTemp, 1638.196f, 726.171f, 113.561f, 1000);
              }
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1615.590f, 719.509f, 110.311f, 2000);
              m_uiStepTimer = 2000;
              break;
           case 16:
              MoveToPoint(m_creature, 1596.436f, 670.809f, 103.747f, 7000);
              m_uiStepTimer = 6900;
              break;
           case 17:
              MoveToPoint(m_creature, 1571.549f, 609.837f, 99.767f, 9000);
              m_uiStepTimer = 11000;
              break;
           case 18:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }
    
    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               ThirdCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_malcolm
###*/

enum
{
  SOUND_ID_DOG_GROWL    = 1108,
  SOUND_ID_DOG_HOWL     = 1018,
  EMOTE_DOG_HOWL        = 393,

  SAY_MALCOLM01         = -1557296,
  SAY_MALCOLM02         = -1557297,
  SAY_MALCOLM03         = -1557298,
  SAY_MALCOLM04         = -1557299,
};

struct MANGOS_DLL_DECL npc_malcolmAI : public ScriptedAI
{
    npc_malcolmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiDogGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 2)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void StartMalcolm()
    {
      m_uiPhase = 1;
    }

    void FourCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              MoveToPoint(m_creature, 1614.066f, 796.722f, 121.739f, 5500);
              m_uiDogGUID = m_pInstance->GetData64(NPC_DOG);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1611.459f, 793.274f, 121.928f, 5500);
              m_uiStepTimer = 5400;
              break;
           case 1:
              DoScriptText(SAY_MALCOLM01, m_creature);
              MoveToPoint(m_creature, 1622.820f, 798.816f, 120.570f, 3500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1621.467f, 794.323f, 120.323f, 3500);
              m_uiStepTimer = 3400;
              break;
           case 2:
              MoveToPoint(m_creature, 1626.574f, 806.781f, 120.270f, 3500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.232f, 803.629f, 120.011f, 3500);
              m_uiStepTimer = 3400;
              break;
           case 3:
              MoveToPoint(m_creature, 1622.782f, 808.533f, 121.249f, 1500);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.265f, 805.245f, 120.070f, 300);
              m_uiStepTimer = 300;
              break;
           case 4:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 pTemp->PlayDirectSound(SOUND_ID_DOG_GROWL);
              m_uiStepTimer = 500;
              break;
           case 5:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_MALCOLM02, m_creature);
              m_uiStepTimer = 2000;
              break;
           case 6:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1629.163f, 809.738f, 120.369f, 1500);
              m_uiStepTimer = 2000;
              break;
           case 7:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
              {
                 pTemp->HandleEmoteCommand(EMOTE_DOG_HOWL);
                 pTemp->PlayDirectSound(SOUND_ID_DOG_HOWL);
              }
              m_uiStepTimer = 4000;
              break;
           case 8:
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1629.922f, 807.799f, 120.122f, 3000);
              m_uiStepTimer = 2900;
              break;
           case 9:
              MoveToPoint(m_creature, 1632.169f, 809.851f, 120.047f, 1000);
              m_uiStepTimer = 900;
              break;
           case 10:
              MoveToPoint(m_creature, 1630.651f, 811.149f, 120.307f, 800);
              m_uiStepTimer = 800;
              break;
           case 11:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              DoScriptText(SAY_MALCOLM03, m_creature);
              m_uiStepTimer = 4000;
              break;
           case 12:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTemp->GetGUID());
              DoScriptText(SAY_MALCOLM04, m_creature);
              m_uiStepTimer = 7000;
              break;
           case 13:
              if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                 MoveToPoint(pTemp, 1630.692f, 808.011f, 120.083f, 400);
              m_uiStepTimer = 600;
              break;
           case 14:
             if(Creature* pTemp = m_pInstance->instance->GetCreature(m_uiDogGUID))
                pTemp->SetStandState(UNIT_STAND_STATE_SIT);
              m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
              MoveToPoint(m_creature, 1641.452f, 812.600f, 119.948f, 4000);
              m_uiStepTimer = 3900;
              break;
           case 15:
              MoveToPoint(m_creature, 1657.975f, 857.352f, 119.097f, 18000);
              m_uiStepTimer = 17900;
              break;
           case 16:
              MoveToPoint(m_creature, 1679.852f, 912.245f, 120.533f, 23000);
              m_uiStepTimer = 22900;
              break;
           case 17:
              MoveToPoint(m_creature, 1699.915f, 967.110f, 121.643f, 23000);
              m_uiStepTimer = 22900;
              break;
           case 18:
              MoveToPoint(m_creature, 1678.393f, 1026.890f, 125.431f, 25000);
              m_uiStepTimer = 24900;
              break;
           case 19:
              MoveToPoint(m_creature, 1678.943f, 1093.130f, 126.899f, 26000);
              m_uiStepTimer = 25900;
              break;
           case 20:
              MoveToPoint(m_creature, 1700.042f, 1103.880f, 130.872f, 9000);
              m_uiStepTimer = 10900;
              break;
           case 21:
              m_uiPhase = 2;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }
    
    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FourCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

/*###
## npc_bartleby
###*/

enum
{
  SAY_BARTLEBY01         = -1557300,
  SAY_BARTLEBY02         = -1557301, 
  SAY_BARTLEBY03         = -1557302,
  SAY_BARTLEBY04         = -1557303,
  SAY_BARTLEBY05         = -1557304,
};

struct MANGOS_DLL_DECL npc_bartleby_csAI : public ScriptedAI
{
    npc_bartleby_csAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiPhase;

    void Reset()
    {
       if(m_uiPhase != 4)
       {
          m_uiStep = 0;
          m_uiStepTimer = 100;
          m_uiPhase = 0;
       }
    }

    void MoveInLineOfSight(Unit* who)
    {
      if (!who)
          return;

      if(!m_pInstance) return;

           if (who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 20 && m_uiPhase == 0)
           {
             m_uiPhase = 1;
           }
    }

    void StartBartleby()
    {
      m_uiPhase = 3;
    }

    void FifthCrateEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_BARTLEBY03, m_creature);
              MoveToPoint(m_creature, 1672.539f, 872.277f, 120.113f, 1000); 
              m_uiStepTimer = 1000;
              break;
           case 1:
              m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
              m_uiStepTimer = 3000;
              break;
           case 2:
              DoScriptText(SAY_BARTLEBY04, m_creature);
              m_uiStepTimer = 7000; 
              break;
           case 3:
              m_creature->SetStandState(UNIT_STAND_STATE_STAND);
              m_uiStepTimer = 1000;
              break;
           case 4:
              DoScriptText(SAY_BARTLEBY05, m_creature);
              MoveToPoint(m_creature, 1663.054f, 869.959f, 119.734f, 3000);
              m_uiStepTimer = 2900; 
              break;
           case 5:
              MoveToPoint(m_creature, 1640.732f, 812.422f, 119.933f, 24000);
              m_uiStepTimer = 23900; 
              break;
           case 6:
              MoveToPoint(m_creature, 1623.704f, 755.741f, 115.710f, 23000);
              m_uiStepTimer = 22900; 
              break;
           case 7:
              MoveToPoint(m_creature, 1607.108f, 699.637f, 106.971f, 23000);
              m_uiStepTimer = 22900; 
              break;
           case 8:
              MoveToPoint(m_creature, 1587.750f, 646.929f, 100.990f, 21000);
              m_uiStepTimer = 20900; 
              break;
           case 9:
              MoveToPoint(m_creature, 1571.103f, 660.949f, 102.084f, 8000);
              m_uiStepTimer = 10900; 
              break;
           case 10:
              m_uiPhase = 4;
              m_creature->SetVisibility(VISIBILITY_OFF);
              break;
        }
    }
    
    void SpeechEvent()
    {
        switch(m_uiStep)
        {
           case 0:
              DoScriptText(SAY_BARTLEBY01, m_creature);
              m_uiStepTimer = 5000;
              break;
           case 1:
              DoScriptText(SAY_BARTLEBY02, m_creature);
              if(m_uiPhase == 1)
                 m_uiPhase = 2;
              break;
        }
    }

    void MoveToPoint(Creature* unit, float X, float Y, float Z, uint32 Timer)
    {
        unit->GetMap()->CreatureRelocation(unit, X, Y, Z, unit->GetOrientation());
        unit->SendMonsterMove(X, Y, Z, SPLINETYPE_NORMAL , unit->GetSplineFlags(), Timer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(m_uiPhase == 3)
       {
            if(m_uiStepTimer < uiDiff)
            {
               FifthCrateEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       if(m_uiPhase == 1)
       {
            if(m_uiStepTimer < uiDiff)
            {
               SpeechEvent();
               m_uiStep++;
            }
            else m_uiStepTimer -= uiDiff;
       }

       return;
    }
};

CreatureAI* GetAI_npc_mike(Creature* pCreature)
{
    return new npc_mikeAI(pCreature);
}

CreatureAI* GetAI_npc_roger(Creature* pCreature)
{
    return new npc_rogerAI(pCreature);
}

CreatureAI* GetAI_npc_morigan(Creature* pCreature)
{
    return new npc_moriganAI(pCreature);
}

CreatureAI* GetAI_npc_jena(Creature* pCreature)
{
    return new npc_jenaAI(pCreature);
}

CreatureAI* GetAI_npc_malcolm(Creature* pCreature)
{
    return new npc_malcolmAI(pCreature);
}

CreatureAI* GetAI_npc_bartleby_cs(Creature* pCreature)
{
    return new npc_bartleby_csAI(pCreature);
}

/*###
## npc_crates
###*/

enum
{
    SPELL_LIGHT            = 49590
};

struct MANGOS_DLL_DECL npc_stratholme_cratesAI : public ScriptedAI
{
    npc_stratholme_cratesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
		pCreature->SetDisplayId(11686);
        Reset();
    }
    
    instance_culling_of_stratholme* m_pInstance;

    bool Active;

    void Reset()
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
       if(!m_pInstance) return;

       if(m_creature->HasAura(SPELL_LIGHT) && Active != true)
       {    
            if(Creature* pRoger = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ROGER_OWENS)))
            { 
               if(m_creature->GetDistance2d(pRoger->GetPositionX(), pRoger->GetPositionY()) < 50.0f)
               {
                   ((npc_rogerAI*)pRoger->AI())->StartRoger();
               }
            }

            if(Creature* pMorigan = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_SERGEANT_MORIGAN)))
            { 
               if(m_creature->GetDistance2d(pMorigan->GetPositionX(), pMorigan->GetPositionY()) < 50.0f)
               {
                  ((npc_moriganAI*)pMorigan->AI())->StartMorigan();
               }
            }

            if(Creature* pJena = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_JENA_ANDERSON)))
            { 
               if(m_creature->GetDistance2d(pJena->GetPositionX(), pJena->GetPositionY()) < 50.0f)
               {
                   ((npc_jenaAI*)pJena->AI())->StartJena();
               }
            }

            if(Creature* pMalcolm = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALCOM_MOORE)))
            { 
               if(m_creature->GetDistance2d(pMalcolm->GetPositionX(), pMalcolm->GetPositionY()) < 50.0f)
               {
                   ((npc_malcolmAI*)pMalcolm->AI())->StartMalcolm();
               }
            }

            if(Creature* pBartleby = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_BARTLEBY_BATTSON)))
            { 
               if(m_creature->GetDistance2d(pBartleby->GetPositionX(), pBartleby->GetPositionY()) < 50.0f)
               {
                   ((npc_bartleby_csAI*)pBartleby->AI())->StartBartleby();
               }
            }

			if (m_pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
				m_pInstance->SetData(TYPE_GRAIN_EVENT, IN_PROGRESS);
			if(GameObject* pLight = GetClosestGameObjectWithEntry(m_creature, GO_CRATE_LIGHT, 5.0f))
				pLight->SetPhaseMask(0, true);
            m_creature->SetPhaseMask(0, true);
            Active = true;
       }

       return;
    }
};

CreatureAI* GetAI_npc_stratholme_crates(Creature* pCreature)
{
    return new npc_stratholme_cratesAI(pCreature);
}

/* *************
** npc_crates_bunny (spell aura effect dummy)
************* */

enum
{
    SPELL_ARCANE_DISRUPTION = 49590
};

bool EffectAuraDummy_spell_aura_dummy_npc_crates_dummy(const Aura* pAura, bool bApply)
{
    if (pAura->GetId() == SPELL_ARCANE_DISRUPTION && pAura->GetEffIndex() == EFFECT_INDEX_0 && bApply)
    {
        if (Creature* pTarget = (Creature*)pAura->GetTarget())
        {
            std::list<Creature*> lCrateBunnyList;
            if (instance_culling_of_stratholme* pInstance = (instance_culling_of_stratholme*)pTarget->GetInstanceData())
            {
                pInstance->GetCratesBunnyOrderedList(lCrateBunnyList);
                uint8 i = 0;
                for (std::list<Creature*>::const_iterator itr = lCrateBunnyList.begin(); itr != lCrateBunnyList.end(); ++itr)
                {
                    i++;
                    if (*itr == pTarget)
                        break;
                }

				switch (i)
				{
				case 1:
					// Start NPC_ROGER_OWENS Event
					if(Creature* pRoger = pTarget->GetMap()->GetCreature(pInstance->GetData64(NPC_ROGER_OWENS)))
					{ 
						if(pTarget->GetDistance2d(pRoger->GetPositionX(), pRoger->GetPositionY()) < 50.0f)
						{
							((npc_rogerAI*)pRoger->AI())->StartRoger();
						}
					}
					break;
				case 2:
					// Start NPC_SERGEANT_MORIGAN  Event
					if(Creature* pMorigan = pTarget->GetMap()->GetCreature(pInstance->GetData64(NPC_SERGEANT_MORIGAN)))
					{ 
						if(pTarget->GetDistance2d(pMorigan->GetPositionX(), pMorigan->GetPositionY()) < 50.0f)
						{
							((npc_moriganAI*)pMorigan->AI())->StartMorigan();
						}
					}
					break;
				case 3:
					// Start NPC_JENA_ANDERSON Event
					if(Creature* pJena = pTarget->GetMap()->GetCreature(pInstance->GetData64(NPC_JENA_ANDERSON)))
					{ 
						if(pTarget->GetDistance2d(pJena->GetPositionX(), pJena->GetPositionY()) < 50.0f)
						{
							((npc_jenaAI*)pJena->AI())->StartJena();
						}
					}
					break;
				case 4:
					// Start NPC_MALCOM_MOORE Event
					if(Creature* pMalcolm = pTarget->GetMap()->GetCreature(pInstance->GetData64(NPC_MALCOM_MOORE)))
					{ 
						if(pTarget->GetDistance2d(pMalcolm->GetPositionX(), pMalcolm->GetPositionY()) < 50.0f)
						{
							((npc_malcolmAI*)pMalcolm->AI())->StartMalcolm();
						}
					}
					break;
				case 5:
					// Start NPC_BARTLEBY_BATTSON Event
					if(Creature* pBartleby = pTarget->GetMap()->GetCreature(pInstance->GetData64(NPC_BARTLEBY_BATTSON)))
					{ 
						if(pTarget->GetDistance2d(pBartleby->GetPositionX(), pBartleby->GetPositionY()) < 50.0f)
						{
							((npc_bartleby_csAI*)pBartleby->AI())->StartBartleby();
						}
					}
					break;
				}

                if (pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
                    pInstance->SetData(TYPE_GRAIN_EVENT, IN_PROGRESS);
                // pTarget->ForcedDespawn();    // direct despawn has influence on visual effects,
                                                // but despawning makes it impossible to multi-use the spell at the same place
                // perhaps some add. GO-Visual
            }
        }
    }
    return true;
}

/* ******
*** Arthas Event
******* */

/*###
## npc_arthas
###*/

enum
{
    SAY_INTRO01                           = -1594071,    //Arthas
    SAY_INTRO02                           = -1594072,    //Uther
    SAY_INTRO03                           = -1594073,    //Arthas
    SAY_INTRO04                           = -1594074,    //Arthas
    SAY_INTRO05                           = -1594075,    //Uther
    SAY_INTRO06                           = -1594076,    //Arthas
    SAY_INTRO07                           = -1594077,    //Uther
    SAY_INTRO08                           = -1594078,    //Arthas
    SAY_INTRO09                           = -1594079,    //Arthas
    SAY_INTRO10                           = -1594080,    //Uther
    SAY_INTRO11                           = -1594081,    //Arthas
    SAY_INTRO12                           = -1594082,    //Uther
    SAY_INTRO13                           = -1594083,    //Jaina
    SAY_INTRO14                           = -1594084,    //Arthas
    SAY_INTRO15                           = -1594085,    //Uther
    SAY_INTRO16                           = -1594086,    //Arthas
    SAY_INTRO17                           = -1594087,    //Jaina
    SAY_INTRO18                           = -1594088,    //Arthas

    SAY_ENTER01                           = -1594089,    //Arthas
    SAY_ENTER02                           = -1594090,    //Cityman
    SAY_ENTER03                           = -1594091,    //Arthas
    SAY_ENTER04                           = -1594092,    //Crazyman
    SAY_ENTER05                           = -1594093,    //Crazyman2
    SAY_ENTER06                           = -1594094,    //Arthas
    SAY_ENTER07                           = -1594095,    //Malganis
    SAY_ENTER08                           = -1594096,    //Malganis
    SAY_ENTER09                           = -1594097,    //Arthas
    SAY_ENTER10                           = -1594098,    //Arthas

    SAY_SALRAMM_SPAWN                     = -1594129,
    SAY_MEATHOOK_SPAWN                    = -1594110,

    SAY_PHASE501                          = -1594142,    //Arthas
    SAY_PHASE502                          = -1594143,    //Arthas
    SAY_PHASE503                          = -1594144,    //Human
    SAY_PHASE504                          = -1594145,    //Arthas
    SAY_PHASE505                          = -1594146,    //Arthas
    SAY_PHASE506                          = -1594147,    //Human
    SAY_PHASE507                          = -1594148,    //Arthas
    SAY_PHASE508                          = -1594149,    //Arthas
    SAY_PHASE509                          = -1594150,    //Arthas
    SAY_PHASE510                          = -1594151,    //Arthas
    SAY_PHASE511                          = -1594152,    //Arthas
    SAY_PHASE512                          = -1594153,    //Arthas
    SAY_PHASE513                          = -1594154,    //Arthas

    SAY_EPOCH_INTRO                       = -1594155,
    SAY_ARTHAS_INTRO                      = -1594156,
    SAY_EPOCH_AGGRO                       = -1594157,

    SAY_PHASE514                        = -1594158,   //Arthas Shkaf 01
    SAY_PHASE515                        = -1594159,   //Arthas Shkaf 02
    SAY_PHASE601                        = -1594160,  //Arthas Fire
    SAY_PHASE602                        = -1594161,  //Arthas Picnic
    SAY_PHASE603                        = -1594162,  //Arthas Picnic End
    SAY_PHASE605                        = -1594164,  //Arthas mall start
    SAY_PHASE606                        = -1594188,

    SAY_MALGANIS_ESCAPE02    = -1594180, 
    SAY_MALGANIS_ESCAPE01    = -1594187, 
    SAY_MALGANIS_OUTRO       = -1594182,
    SAY_ARTHAS_OUTRO01       = -1594181,
    SAY_ARTHAS_OUTRO02       = -1594183,
    SAY_ARTHAS_OUTRO03       = -1594184,

    /*SPELL*/
    SPELL_EXORCISM_N          = 52445,
    SPELL_EXORCISM_H          = 58822,
    SPELL_HOLY_LIGHT          = 52444,
    SPELL_ARTHAS_AURA         = 52442,

    /*NPC*/
    NPC_CITYMAN                    = 28167,
    NPC_CRAZYMAN                   = 28169,
    NPC_MALGANIS_INTRO             = 26533,

    /*OTHER*/
    POINT_LAST_POINT    = 0xFFFFFF,
    FACTION             = 2076
};

const float SummonScourge[2][4] =
{
    {2340.058f, 1253.570f, 132.733f, 5.09f}, //right wing
    {2272.773f, 1331.824f, 124.171f, 3.12f}, //left wing
};

struct MANGOS_DLL_DECL npc_arthasAI : public npc_escortAI
{
    npc_arthasAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroic;

    uint64 m_uiUtherGUID;
    uint64 m_uiJainaGUID;
    uint64 m_uiPeople01GUID;
    uint64 m_uiPeople02GUID;
    uint64 m_uiMalganisGUID;
    uint64 m_uiMarine01GUID;
    uint64 m_uiMarine02GUID;
    uint64 m_uiMarine03GUID;
    uint64 m_uiMarine04GUID;
    uint64 m_uiPriest01GUID;
    uint64 m_uiPriest02GUID;
    uint64 m_uiHuman01GUID;
    uint64 m_uiHuman02GUID;
    uint64 m_uiHuman03GUID;

    uint32 culling_faction;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    uint32 m_uiMoveTimer;
    uint32 m_uiHealTimer;
    uint32 m_uiExorcismTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiWaveCount;

    Creature* Malganis;
    Creature* pEpoch;
    bool StartEvent;
    bool MoveSoldier;

    float LastX;
    float LastY;
    float LastZ;

    void Reset()
    {
        if(!m_pInstance) return;

        if(m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
        {
            m_creature->setFaction(35);
            RemoveGossip();
        }

        if(m_pInstance->GetData(TYPE_PHASE) == 11)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
        }
    }

    void RemoveGossip()
    {     
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void MoveSoldiers()
    {
        if(Creature* Marine01 = m_creature->GetMap()->GetCreature(m_uiMarine01GUID))
        {
            Marine01->GetMotionMaster()->MovePoint(0, 2083.483f,1282.313f,141.198f);
            Marine01->setFaction(culling_faction);
        }
        if(Creature* Marine02 = m_creature->GetMap()->GetCreature(m_uiMarine02GUID))
        {
            Marine02->GetMotionMaster()->MovePoint(0, 2083.681f,1292.809f,141.141f);
            Marine02->setFaction(culling_faction);
        }
        if(Creature* Marine03 = m_creature->GetMap()->GetCreature(m_uiMarine03GUID))
        {
            Marine03->GetMotionMaster()->MovePoint(0, 2082.158f,1290.406f,141.261f);
            Marine03->setFaction(culling_faction);
        }
        if(Creature* Marine04 = m_creature->GetMap()->GetCreature(m_uiMarine04GUID))
        {
            Marine04->GetMotionMaster()->MovePoint(0, 2081.899f,1285.122f,141.302f);
            Marine04->setFaction(culling_faction);
        }
        if(Creature* Priest01 = m_creature->GetMap()->GetCreature(m_uiPriest01GUID))
        {
            Priest01->GetMotionMaster()->MovePoint(0, 2081.072f,1292.233f,141.329f);
            Priest01->setFaction(culling_faction);
        }
        if(Creature* Priest02 = m_creature->GetMap()->GetCreature(m_uiPriest02GUID))
        {
            Priest02->GetMotionMaster()->MovePoint(0, 2080.632f,1283.004f,141.358f);
            Priest02->setFaction(culling_faction);
        }
    }

    void EnableEscort()
    {
        SetEscortPaused(false);
    }

    void SummonPeople()
    {
        if(Creature* Cityman = m_creature->SummonCreature(NPC_CITYMAN,2091.977f,1275.021f,140.757f,0.558f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
            m_uiPeople01GUID = Cityman->GetGUID();
        if(Creature* Crazyman = m_creature->SummonCreature(NPC_CRAZYMAN,2093.514f,1275.842f,140.408f,3.801f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
            m_uiPeople02GUID =  Crazyman->GetGUID();
    }

    void StartAI()
    {
        SummonPeople();
        m_uiStep = 0;
        m_uiStepTimer = 100;
        StartEvent = true;
    }

    void Aggro(Unit* who)
    {
        DoCast(m_creature, SPELL_ARTHAS_AURA);
    }

    void EnterEvadeMode()
    {
        if(!m_pInstance) return;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreaturesAddon();
        m_uiExorcismTimer = 7400;
        m_uiHealTimer = 100;

        m_creature->SetLootRecipient(NULL);

        if(m_pInstance->GetData(TYPE_PHASE) > 4)
        {
            npc_escortAI::EnterEvadeMode();
        }

        if(m_pInstance->GetData(TYPE_PHASE) > 2 && m_pInstance->GetData(TYPE_PHASE) < 5)
        {
            m_creature->GetMotionMaster()->MovePoint(POINT_LAST_POINT, LastX, LastY, LastZ);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if(!pWho || pWho == m_creature)
            return;

        if(m_pInstance && m_pInstance->GetData(TYPE_PHASE) == 4) return;

        npc_escortAI::AttackStart(pWho);
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->Respawn();

        // restart from intro
        // the event is reseted to the previous waypoint phase after finishing intro
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_PHASE, 0);
            m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVE, 0);
        }

        // reset waves (bosses killed are unlootable)
        // todo: reset event if arthas is killed after the waves are done
        /*if(m_pInstance->GetData(TYPE_ENCOUNTER) == DONE)
        {
            if(Creature *pChromie = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_CHROMI02)))
                pChromie->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }*/
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    AttackStart(pWho);
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho, 0.0f);
                }
            }
        }
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
        case 2:
            DoScriptText(SAY_INTRO18, m_creature);
            SetRun(true);
            break;
        case 8:
            GetSoldier();
            SetEscortPaused(true);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            m_pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, DONE);
            SetRun(false);
            break;
        case 9:
            DoScriptText(SAY_ENTER01, m_creature);
            MoveSoldier = true;
            m_uiMoveTimer = 12000;
            break;
        case 10:
            SetEscortPaused(true);
            m_pInstance->SetData(TYPE_PHASE, 2);
            ResetStep(2000);
            if(Creature* Cityman = m_creature->GetMap()->GetCreature(m_uiPeople01GUID))
            {
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Cityman->GetGUID());
                Cityman->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                Cityman->GetMotionMaster()->MovePoint(0, 2088.625f,1279.191f,140.743f);
            }
            break;
        case 14:
            if(Creature* Human01 = m_creature->SummonCreature(NPC_STRATHOLME_CITIZEN,2397.308f,1207.565f,134.038f,5.593f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                m_uiHuman01GUID = Human01->GetGUID();
            if(Creature* Human02 = m_creature->SummonCreature(NPC_STRATHOLME_CITIZEN,2400.770f,1207.362f,134.038f,3.454f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                m_uiHuman02GUID = Human02->GetGUID();
            if(Creature* Human03 = m_creature->SummonCreature(NPC_STRATHOLME_CITIZEN,2400.547f,1204.892f,134.038f,2.479f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,30000))
                m_uiHuman03GUID = Human03->GetGUID();
            break;
        case 20:
            SetEscortPaused(true);
            m_creature->setFaction(35);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            SetRun(false);
            break;  
        case 21:
            DoScriptText(SAY_PHASE502, m_creature);
            break;
        case 22:    
            SetEscortPaused(true);
            m_pInstance->SetData(TYPE_PHASE, 6);
            ResetStep(1000);
            break;    
        case 25:
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
            m_creature->SummonCreature(NPC_TIME_RIFT,2428.901f, 1192.164f, 148.076f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            DoScriptText(SAY_PHASE508, m_creature);
            break;
        case 26:
            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
            DoScriptText(SAY_PHASE509, m_creature);
            break;
        case 29:
            m_creature->SummonCreature(NPC_TIME_RIFT,2413.773f, 1137.820f, 148.076f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_TIME_RIFT,2404.990f, 1175.121f, 148.076f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            DoScriptText(SAY_PHASE510, m_creature);
            break;
        case 30:
            DoScriptText(SAY_PHASE513, m_creature);
            break;
        case 31:
            ResetStep(1000);
            m_pInstance->SetData(TYPE_PHASE, 7);
            break; 
        case 32:
            SetEscortPaused(true);
            m_pInstance->SetData(TYPE_PHASE, 8);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            SetRun(false);
            break;   
        case 36:
            DoScriptText(SAY_PHASE514, m_creature);
            break;
        case 37:
            if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_DOOR_BOOKCASE)))
                pGate->SetGoState(GO_STATE_ACTIVE); 
            SetRun(true);                  
            DoScriptText(SAY_PHASE515, m_creature);
            break;
        case 45:
            DoScriptText(SAY_PHASE601, m_creature);
            break;
        case 48:
            DoScriptText(SAY_PHASE602, m_creature);
            break;
        case 51:
            SetEscortPaused(true);
            m_pInstance->SetData(TYPE_PHASE, 9);
            DoScriptText(SAY_PHASE606, m_creature);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            break;   
        case 53:
            SetEscortPaused(true);
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->MovementExpired(false);
            m_creature->setFaction(FACTION);
            DoScriptText(SAY_PHASE605, m_creature);
            if(Creature* Malganis = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALGANIS)))
            {
                m_pInstance->SetData(TYPE_MALGANIS_EVENT, IN_PROGRESS);
                Malganis->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->AI()->AttackStart(Malganis);
                Malganis->AI()->AttackStart(m_creature);
            }
            break;
        } 
    }

    void JumpNextStep(uint32 Timer)
    {
        m_uiStepTimer = Timer;
        m_uiStep++;
    }

    void GetSoldier() //huck
    {
        //Marine Close Left
        if(Creature* pEscort01 = GetClosestCreatureWithEntry(m_creature, NPC_MARINE, 50.0f))
        {
            m_uiMarine01GUID = pEscort01->GetGUID();
            pEscort01->UpdateEntry(NPC_CITYMAN);
            if(Creature* pEscort02 = GetClosestCreatureWithEntry(m_creature, NPC_MARINE, 50.0f))
            {
                m_uiMarine02GUID = pEscort02->GetGUID();
                pEscort02->UpdateEntry(NPC_CITYMAN);
                // Right marine 2
                if(Creature* pEscort03 = GetClosestCreatureWithEntry(m_creature, NPC_MARINE, 50.0f))
                {
                    m_uiMarine03GUID = pEscort03->GetGUID();
                    pEscort03->UpdateEntry(NPC_CITYMAN);
                    if(Creature* pEscort04 = GetClosestCreatureWithEntry(m_creature, NPC_MARINE, 50.0f))
                    {
                        m_uiMarine04GUID = pEscort04->GetGUID();
                        pEscort01->UpdateEntry(NPC_MARINE);
                        pEscort02->UpdateEntry(NPC_MARINE);
                        pEscort03->UpdateEntry(NPC_MARINE);
                    }
                }
            }
        }

        if(Creature* pEscort05 = GetClosestCreatureWithEntry(m_creature, NPC_PRIEST, 50.0f))
        {
            m_uiPriest01GUID = pEscort05->GetGUID();
            pEscort05->UpdateEntry(NPC_CITYMAN);
            if(Creature* pEscort06 = GetClosestCreatureWithEntry(m_creature, NPC_PRIEST, 50.0f))
            {
                m_uiPriest02GUID = pEscort06->GetGUID();
                pEscort05->UpdateEntry(NPC_PRIEST);
            }
        }
    }

    void ResetStep(uint32 Timer)
    {
        m_uiStep = 0;
        m_uiStepTimer = Timer;
    }

    void IntroEvent()
    {
        switch(m_uiStep)
        {
        case 0:
            DoScriptText(SAY_INTRO01, m_creature);
            JumpNextStep(2000);
            break;
        case 1:
            m_uiUtherGUID = m_pInstance->GetData64(NPC_UTHER);
            m_uiJainaGUID = m_pInstance->GetData64(NPC_JAINA);
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO02, pUther);
            JumpNextStep(8000);
            break;
        case 2:
            m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
            DoScriptText(SAY_INTRO03, m_creature);
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            m_creature->GetMotionMaster()->MovePoint(0, 1908.334f, 1315.354f, 149.551f);
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                pUther->GetMotionMaster()->MovePoint(0, 1903.600f, 1296.678f, 143.383f);
            JumpNextStep(2000);
            break;
        case 3:
            if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGUID))
                pJaina->GetMotionMaster()->MovePoint(0, 1899.641f, 1298.684f, 143.831f);
            JumpNextStep(7000);
            break;
        case 4:
            m_creature->GetMotionMaster()->MovementExpired(false);
            m_creature->GetMotionMaster()->MovePoint(0, 1911.087f, 1314.263f, 150.026f);
            JumpNextStep(1000);
            break;
        case 5:
            DoScriptText(SAY_INTRO04, m_creature);
            JumpNextStep(10000);
            break;
        case 6:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO05, pUther);
            JumpNextStep(1000);
            break;
        case 7:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pUther->GetGUID());
            DoScriptText(SAY_INTRO06, m_creature);
            JumpNextStep(4000);
            break;
        case 8:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO07, pUther);
            JumpNextStep(6000);
            break;
        case 9:
            DoScriptText(SAY_INTRO08, m_creature);
            JumpNextStep(4000);
            break;
        case 10:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO09, pUther);
            JumpNextStep(8000);
            break;
        case 11:
            DoScriptText(SAY_INTRO10, m_creature);
            JumpNextStep(4000);
            break;
        case 12:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO11, pUther);
            JumpNextStep(4000);
            break;
        case 13:
            DoScriptText(SAY_INTRO12, m_creature);
            JumpNextStep(11000);
            break;
        case 14:
            if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGUID))
                DoScriptText(SAY_INTRO13, pJaina);
            JumpNextStep(3000);
            break;
        case 15:
            DoScriptText(SAY_INTRO14, m_creature);
            JumpNextStep(9000);
            break;
        case 16:
            if(Creature* pUther = m_pInstance->instance->GetCreature(m_uiUtherGUID))
                DoScriptText(SAY_INTRO15, pUther);
            JumpNextStep(5000);
            break;
        case 17:
            if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGUID))
            {
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pJaina->GetGUID());
                pJaina->GetMotionMaster()->MovePoint(0, 1794.357f,1272.183f,140.558f);
            }
            JumpNextStep(1000);
            break;
        case 18:
            DoScriptText(SAY_INTRO16, m_creature);
            JumpNextStep(1000);
            break;
        case 19:
            if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGUID))
                DoScriptText(SAY_INTRO17, pJaina);
            JumpNextStep(3000);
            break;
        case 20:
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            ((npc_arthasAI*)m_creature->AI())->Start(true, false);
            JumpNextStep(3000);
            break;
        }
    }

    void EnterEvent()
    {
        switch(m_uiStep)
        {
        case 0:
            if(Creature* Cityman = m_creature->GetMap()->GetCreature(m_uiPeople01GUID))
                DoScriptText(SAY_ENTER02, Cityman);
            JumpNextStep(4000);
            break;
        case 1:
            m_creature->GetMotionMaster()->MovePoint(0, 2087.689f,1280.344f,140.73f);
            DoScriptText(SAY_ENTER03, m_creature);
            JumpNextStep(3000);
            break;
        case 2:
            if(Creature* Cityman = m_creature->GetMap()->GetCreature(m_uiPeople01GUID))
                DoScriptText(SAY_ENTER04, Cityman);
            m_creature->HandleEmoteCommand(37);
            JumpNextStep(1000);
            break;
        case 3:
            if(Creature* Cityman = m_creature->GetMap()->GetCreature(m_uiPeople01GUID))
                m_creature->DealDamage(Cityman, Cityman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            if(Creature* Crazyman = m_creature->GetMap()->GetCreature(m_uiPeople02GUID))
            {
                DoScriptText(SAY_ENTER05, Crazyman);
                Crazyman->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Crazyman->GetGUID());
                m_creature->GetMotionMaster()->MovePoint(0, 2092.154f,1276.645f,140.52f);
            }
            JumpNextStep(3000);
            break;
        case 4:
            m_creature->HandleEmoteCommand(37);
            JumpNextStep(1000);
            break;
        case 5:
            if(Creature* Crazyman = m_creature->GetMap()->GetCreature(m_uiPeople02GUID))
                Crazyman->DealDamage(Crazyman, Crazyman->GetMaxHealth(),NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            JumpNextStep(1000);
            break;
        case 6:
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            m_creature->GetMotionMaster()->MovePoint(0, 2091.179f,1278.065f,140.476f);
            DoScriptText(SAY_ENTER06, m_creature);
            JumpNextStep(3000);
            break;
        case 7:
            if(Creature* StalkerM = m_creature->SummonCreature(20562,2117.349f,1288.624f,136.271f,1.37f,TEMPSUMMON_TIMED_DESPAWN,60000))
                StalkerM->CastSpell(StalkerM,63793,false);
            JumpNextStep(1000);
            break;
        case 8:
            m_pInstance->SetData(TYPE_ENCOUNTER, IN_PROGRESS);
            if(Creature* TempMalganis = m_creature->SummonCreature(NPC_MALGANIS_INTRO,2117.349f,1288.624f,136.271f,1.37f,TEMPSUMMON_TIMED_DESPAWN,29000))
            {
                m_uiMalganisGUID = TempMalganis->GetGUID();
                DoScriptText(SAY_ENTER07, TempMalganis);
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, TempMalganis->GetGUID());
                TempMalganis->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                TempMalganis->setFaction(35);
            }
            JumpNextStep(11000);
            break;
        case 9:
            if(Creature* TempMalganis = m_creature->GetMap()->GetCreature(m_uiMalganisGUID))
                DoScriptText(SAY_ENTER08, TempMalganis);
            JumpNextStep(17000);
            break;
        case 10:
            DoScriptText(SAY_ENTER09, m_creature);
            JumpNextStep(7000);
            break;
        case 11:
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            DoScriptText(SAY_ENTER10, m_creature);
            JumpNextStep(12000);
            break;
        case 12:
            m_creature->GetMotionMaster()->MovePoint(0, 2084.584f,1278.331f,141.479f);
            JumpNextStep(4000);
            break;
        case 13:
            m_creature->GetMotionMaster()->MovementExpired(false);
            m_creature->GetMotionMaster()->MovePoint(0, 2087.414f,1279.293f,140.933f);
            JumpNextStep(2000);
            break;
        case 14:
            LastX = m_creature->GetPositionX();
            LastY = m_creature->GetPositionY();
            LastZ = m_creature->GetPositionZ();
            if(m_bIsHeroic && m_pInstance->GetData(TYPE_INFINITE_CORRUPTER) == NOT_STARTED)
                m_pInstance->SetData(TYPE_INFINITE_CORRUPTER, IN_PROGRESS);
            m_uiWaveCount = 0;
            SetRun(true);
            m_pInstance->SetData(TYPE_WING, RIGHT);
            m_creature->setFaction(FACTION);
            m_uiSummonTimer = 100;
            m_pInstance->SetData(TYPE_PHASE, 3);
            break;
        }
    }

    void SummonWing()
    {
        m_uiWaveCount++;
        m_pInstance->DoUpdateWorldState(WORLD_STATE_WAVE, m_uiWaveCount);

        switch(m_uiWaveCount)
        {
        case 1:
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 2:
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 3:
            m_pInstance->SetData(TYPE_WING, LEFT);
            m_creature->SummonCreature(NPC_GHOUL,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 4:
            m_pInstance->SetData(TYPE_WING, RIGHT);
            m_creature->SummonCreature(NPC_ACOLYTE,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_ACOLYTE,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 5:
            m_pInstance->SetData(TYPE_PHASE, 4);
            if(Creature* pMeathook = m_creature->SummonCreature(NPC_MEATHOOK, 2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000))
            {
                DoScriptText(SAY_MEATHOOK_SPAWN, pMeathook);
                pMeathook->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pMeathook->GetMotionMaster()->MovePoint(0, 2351.9038f, 1201.3353f, 130.4317f);
            }
            break;
        case 6:
            m_pInstance->SetData(TYPE_WING, LEFT);
            m_creature->SummonCreature(NPC_GHOUL,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 7:
            m_pInstance->SetData(TYPE_WING, RIGHT);
            m_creature->SummonCreature(NPC_CONSTRUCT,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 8:
            m_pInstance->SetData(TYPE_WING, LEFT);
            m_creature->SummonCreature(NPC_CONSTRUCT,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2272.773f, 1331.824f, 124.171f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 9:
            m_pInstance->SetData(TYPE_WING, RIGHT);
            m_creature->SummonCreature(NPC_CONSTRUCT,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_FIEND,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_NECROMANCER,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            m_creature->SummonCreature(NPC_GHOUL,2340.058f, 1253.570f, 132.733f, 5.09f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000);
            break;
        case 10:
            m_pInstance->SetData(TYPE_PHASE, 4);
            if(Creature* pSalramm = m_creature->SummonCreature(NPC_SALRAMM_THE_FLESHCRAFTER,2196.036f, 1328.818f, 129.997f, 3.12f,TEMPSUMMON_CORPSE_TIMED_DESPAWN,29000))
            {
                DoScriptText(SAY_SALRAMM_SPAWN, pSalramm);
                pSalramm->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pSalramm->GetMotionMaster()->MovePoint(0, 2196.036f, 1328.818f, 129.997f);
            }
            break;
        }
    }

    void HouseEvent()
    {
        switch(m_uiStep)
        {
        case 0:
            if(Creature* Human = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
            {
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Human->GetGUID());
                Human->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                DoScriptText(SAY_PHASE503, Human);
            }
            JumpNextStep(4000);
            break;
        case 1:
            DoScriptText(SAY_PHASE504, m_creature);
            m_creature->GetMotionMaster()->MovePoint(0, 2396.035f, 1206.942f, 134.038f);
            JumpNextStep(3000);
            break;
        case 2:
            m_creature->HandleEmoteCommand(37);
            JumpNextStep(2000);
            break;
        case 3:
            DoScriptText(SAY_PHASE505, m_creature);
            JumpNextStep(2000);
            break;
        case 4:
            if(Creature* Human = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
                DoScriptText(SAY_PHASE506, Human);
            JumpNextStep(6000);
            break;
        case 5:
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            if(Creature* Human = m_pInstance->instance->GetCreature(m_uiHuman01GUID))
            {
                Human->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                Human->UpdateEntry(NPC_INFINITE_ADVERSARY);
            }
            if(Creature* Human2 = m_pInstance->instance->GetCreature(m_uiHuman02GUID))
                Human2->UpdateEntry(NPC_INFINITE_HUNTER);
            if(Creature* Human3 = m_pInstance->instance->GetCreature(m_uiHuman03GUID))
                Human3->UpdateEntry(NPC_INFINITE_AGENT);
            JumpNextStep(1000);
            break;
        case 6:
            SetRun(true);
            m_creature->GetMotionMaster()->MovePoint(0, 2384.320f, 1202.779f, 134.040f);
            DoScriptText(SAY_PHASE507, m_creature);
            JumpNextStep(5000);
            break;
        case 7:
            SetEscortPaused(false);
            m_creature->setFaction(FACTION);
            m_pInstance->SetData(TYPE_PHASE, 5);
            JumpNextStep(1000);
            break;
        }
    }

    void EpochEvent()
    {
        switch(m_uiStep)
        {
        case 0:
            m_creature->SummonCreature(NPC_TIME_RIFT_2,2445.629f,1111.500f,148.076f,3.229f,TEMPSUMMON_TIMED_DESPAWN,9000);
            JumpNextStep(2000);
            break;
        case 1:
            pEpoch = m_creature->SummonCreature(NPC_CHRONO_LORD_EPOCH,2445.629f,1111.500f,148.076f,3.229f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000);
            if(pEpoch)
            {
                pEpoch->setFaction(35);
                DoScriptText(SAY_EPOCH_INTRO, pEpoch);
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pEpoch->GetGUID());
            }
            JumpNextStep(20000);
            break;
        case 2:
            DoScriptText(SAY_ARTHAS_INTRO, m_creature);
            JumpNextStep(6000);
            break;
        case 3:
            if(pEpoch)
            {
                DoScriptText(SAY_EPOCH_AGGRO, pEpoch);
                m_creature->AI()->AttackStart(pEpoch);
                pEpoch->AI()->AttackStart(m_creature);
                pEpoch->setFaction(14);
            }
            m_pInstance->SetData(TYPE_PHASE, 5);
            SetRun(false);
            JumpNextStep(6000);
            break;
        }
    }

    void MalganisEvent()
    {
        switch(m_uiStep)
        {
        case 0:
            m_creature->setFaction(35);
            m_creature->GetMotionMaster()->MovePoint(0, 2302.326f, 1491.386f, 128.362f);
            if(Creature* Malganis = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALGANIS)))
            {
                DoScriptText(SAY_MALGANIS_ESCAPE01, Malganis);
                Malganis->InterruptNonMeleeSpells(false);
                Malganis->GetMotionMaster()->MovePoint(0, 2296.665f,1502.362f,128.362f);
                m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Malganis->GetGUID());
                Malganis->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
            }
            JumpNextStep(10000);
            break;
        case 1:
            if(Creature* Malganis = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALGANIS)))
                DoScriptText(SAY_MALGANIS_ESCAPE02, Malganis);
            JumpNextStep(10000);
            break;
        case 2:
            DoScriptText(SAY_ARTHAS_OUTRO01, m_creature);
            JumpNextStep(5000);
            break;
        case 3:
            if(Creature* Malganis = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALGANIS)))
                DoScriptText(SAY_MALGANIS_OUTRO, Malganis);
            JumpNextStep(20000);
            break;
        case 4:
            if(Creature* Malganis = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_MALGANIS)))
            {
                Malganis->SetVisibility(VISIBILITY_OFF);
                m_creature->GetMotionMaster()->MovePoint(0, Malganis->GetPositionX(), Malganis->GetPositionY(), Malganis->GetPositionZ());
            }
            m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            JumpNextStep(3000);
            break;
        case 5:
            DoScriptText(SAY_ARTHAS_OUTRO02, m_creature);
            JumpNextStep(6000);
            break;
        case 6:
            m_creature->GetMotionMaster()->MovePoint(0, 2298.298f,1500.362f,128.362f);
            DoScriptText(SAY_ARTHAS_OUTRO03, m_creature);
            // spawn chest & chronie
            //m_creature->SummonCreature(NPC_CHROMIE_DRAGON, 2296.665f, 1502.362f, 128.362f, 4.961f,TEMPSUMMON_TIMED_DESPAWN,900000);
            if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_EXIT)))
                 pGate->SetGoState(GO_STATE_ACTIVE);
            if(m_pInstance->GetData(TYPE_MALGANIS_EVENT) != DONE)
            {
                //if(GameObject* pChest = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(m_bIsHeroic ? GO_DARK_RUNED_CHEST_H : GO_DARK_RUNED_CHEST)))
                    //m_pInstance->DoRespawnGameObject(pChest->GetGUID(), 30*MINUTE);
                m_pInstance->SetData(TYPE_MALGANIS_EVENT, DONE);
                m_pInstance->DoCompleteAchievement(m_bIsHeroic ? ACHIEV_COS_H : ACHIEV_COS);
            }
            JumpNextStep(11000);
            break;
        case 7:
            m_creature->GetMotionMaster()->MovePoint(0, 2243.311f, 1476.025f, 132.352f);
            JumpNextStep(11000);
            break;
        case 8:
            m_pInstance->SetData(TYPE_PHASE, 12);
            m_creature->SetVisibility(VISIBILITY_OFF);
            break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        npc_escortAI::UpdateAI(uiDiff);

        if(!m_pInstance) return;

        if(StartEvent == true)
        {
            if(m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) != DONE)
            {
                if(m_uiStepTimer < uiDiff)
                {
                    IntroEvent();
                }
                else m_uiStepTimer -= uiDiff;
            }

            if(m_pInstance->GetData(TYPE_PHASE) == 2)
            {
                if(m_uiStepTimer < uiDiff)
                {
                    EnterEvent();
                }
                else m_uiStepTimer -= uiDiff;
            }

            if(MoveSoldier == true)
            {
                if(m_uiMoveTimer < uiDiff)
                {
                    MoveSoldiers();
                    MoveSoldier = false;
                }
                else m_uiMoveTimer -= uiDiff;
            }

            if(m_pInstance->GetData(TYPE_PHASE) == 3)
            {
                if(m_uiSummonTimer < uiDiff)
                {
                    SummonWing();
                    m_uiSummonTimer = 70000;
                }
                else m_uiSummonTimer -= uiDiff;
            }

            if(m_pInstance->GetData(TYPE_PHASE) == 4 && m_pInstance->GetData(TYPE_ENCOUNTER) == DONE)
            {
                m_pInstance->SetData(TYPE_PHASE, 5);
                SetRun(true);
                EnableEscort();
                DoScriptText(SAY_PHASE501, m_creature);
            }

            if(m_pInstance->GetData(TYPE_PHASE) == 6)
            {
                if(m_uiStepTimer < uiDiff)
                {
                    HouseEvent();
                }
                else m_uiStepTimer -= uiDiff;
            }

            if(m_pInstance->GetData(TYPE_PHASE) == 7)
            {
                if(m_uiStepTimer < uiDiff)
                {
                    EpochEvent();
                }
                else m_uiStepTimer -= uiDiff;
            }

        } //close event!

        if(m_pInstance->GetData(TYPE_PHASE) == 10)
        {
            SetEscortPaused(true);
            ResetStep(1000);
            m_creature->AttackStop();
            m_pInstance->SetData(TYPE_PHASE, 11);
        }

        if(m_pInstance->GetData(TYPE_PHASE) == 11)
        {
            if(m_uiStepTimer < uiDiff)
            {
                MalganisEvent();
            }
            else m_uiStepTimer -= uiDiff;
        }

        //} //close event!

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_pInstance->GetData(TYPE_PHASE) > 9) return;

        if (m_uiExorcismTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, m_bIsHeroic ? SPELL_EXORCISM_H : SPELL_EXORCISM_N);

            m_uiExorcismTimer = 7300;
        }else m_uiExorcismTimer -= uiDiff;

        if (m_uiHealTimer < uiDiff)
        {
            if(m_creature->GetHealthPercent() < 40.0f)
            {
                DoCast(m_creature, SPELL_HOLY_LIGHT);
                m_uiHealTimer = 20000;
            }
        } else m_uiHealTimer -= uiDiff;

        return;
    }
};

/*###
## npc_uther
###*/

struct MANGOS_DLL_DECL npc_utherAI : public npc_escortAI
{
    npc_utherAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 m_uiArthasGUID;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;
    bool StartEvent;

    uint64 m_uiKnightGUID01;
    uint64 m_uiKnightGUID02;
    uint64 m_uiKnightGUID03;

    void Reset()
    {
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_uiStep = 0;
        m_uiStepTimer = 100;
    }

    void StartAI()
    {
        SetWeather(WEATHER_STATE_MEDIUM_RAIN, 0.9999f);
        StartEvent = true;
        m_creature->SetVisibility(VISIBILITY_ON);
		m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        ((npc_utherAI*)m_creature->AI())->Start(false, true);

        if(Creature* Knight01 = m_creature->SummonCreature(NPC_KNIGHT,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,110000))
        {
            m_uiKnightGUID01 = Knight01->GetGUID();
            Knight01->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            Knight01->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,M_PI_F/2);
        }

        if(Creature* Knight02 = m_creature->SummonCreature(NPC_KNIGHT,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,110000))
        {
            m_uiKnightGUID02 = Knight02->GetGUID();
            Knight02->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            Knight02->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,M_PI_F/4);
        }

        if(Creature* Knight03 = m_creature->SummonCreature(NPC_KNIGHT,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN,110000))
        {
            m_uiKnightGUID03 = Knight03->GetGUID();
            Knight03->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            Knight03->GetMotionMaster()->MoveFollow(m_creature,PET_FOLLOW_DIST,M_PI_F/3);
        }
    }

    void SetWeather(uint32 weather, float grade)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) return;
        WorldPacket data(SMSG_WEATHER, (4+4+4));
        data << uint32(weather) << (float)grade << uint8(0);
        ((InstanceMap*)map)->SendToPlayers(&data);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
        case 3:
            m_uiArthasGUID = m_pInstance->GetData64(NPC_ARTHAS);
            if(Creature* pArthas = m_pInstance->instance->GetCreature(m_uiArthasGUID))
            {
                pArthas->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pArthas->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                pArthas->GetMotionMaster()->MovePoint(0, 1902.974f, 1291.635f, 143.337f);
            }
            break;
        case 4:
            SetRun(false);
            if(Creature *pArthas = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ARTHAS)))
                ((npc_arthasAI*)pArthas->AI())->StartAI();
            break;
        case 6:
            m_creature->SetVisibility(VISIBILITY_OFF);
            uint64 m_uiJainaGUID = m_pInstance->GetData64(NPC_JAINA);
            if(Creature* pJaina = m_pInstance->instance->GetCreature(m_uiJainaGUID))
                pJaina->SetVisibility(VISIBILITY_OFF);
            break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        npc_escortAI::UpdateAI(uiDiff);

        if(!m_pInstance) return;

        return;
    }
};

/* *************
** npc_chromie (gossip, quest-accept)
************* */

enum
{
    QUEST_DISPELLING_ILLUSIONS = 13149,
    QUEST_A_ROYAL_ESCORT       = 13151,

    ITEM_ARCANE_DISRUPTOR      = 37888,

    GOSSIP_MENU_INN_1          = 12939,
    GOSSIP_MENU_INN_2          = 12949,
    GOSSIP_MENU_INN_3          = 12950,
    GOSSIP_MENU_INN_4          = 12952,

    GOSSIP_MENU_ENTRANCE_1     = 12992,
    GOSSIP_MENU_ENTRANCE_2     = 12993,
    GOSSIP_MENU_ENTRANCE_3     = 12994,
    GOSSIP_MENU_ENTRANCE_4     = 12995,
};

const std::string GOSSIP_ITEM_INN_1      = "Why have I been sent back to this particular place and time?";
const std::string GOSSIP_ITEM_INN_2      = "What was this decision?";
const std::string GOSSIP_ITEM_INN_3      = "So how does the Infinite Dragonflight plan to interfere?";

const std::string GOSSIP_ITEM_ENTRANCE_1 = "What do you think they're up to?";
const std::string GOSSIP_ITEM_ENTRANCE_2 = "You want me to do what?";
const std::string GOSSIP_ITEM_ENTRANCE_3 = "Very well, Chromie";

bool GossipHello_npc_chromie(Player *pPlayer, Creature *pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
    {
        switch (pCreature->GetEntry())
        {
            case NPC_CHROMIE_INN:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) != DONE)
                {
                    if (pPlayer->GetQuestRewardStatus(QUEST_DISPELLING_ILLUSIONS) && !pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                }
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_1, pCreature->GetGUID());
                break;
            case NPC_CHROMIE_ENTRANCE:
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == DONE && m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED && pPlayer->GetQuestRewardStatus(QUEST_A_ROYAL_ESCORT))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_1, pCreature->GetGUID());
                break;
        }
    }
       return true;
}

bool GossipSelect_npc_chromie(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 uiAction)
{
    switch (pCreature->GetEntry())
    {
        case NPC_CHROMIE_INN:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_2, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_INN_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_3, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_INN_4, pCreature->GetGUID());
                    if (!pPlayer->HasItemCount(ITEM_ARCANE_DISRUPTOR, 1))
                    {
                        if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_ARCANE_DISRUPTOR, 1))
                        {
                            pPlayer->SendNewItem(pItem, 1, true, false);
                            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                                    m_pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
                        }
                    }
                    break;
            }
            break;
        case NPC_CHROMIE_ENTRANCE:
            switch (uiAction)
            {
                case GOSSIP_ACTION_INFO_DEF+1:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_2, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+2:
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ENTRANCE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_3, pCreature->GetGUID());
                    break;
                case GOSSIP_ACTION_INFO_DEF+3:
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ENTRANCE_4, pCreature->GetGUID());
                    if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
						if (m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
						{
							//m_pInstance->DoSpawnArthasIfNeeded();
							m_pInstance->DoUpdateWorldState(WORLD_STATE_CRATES, 0);

							if(Creature *pUther = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_UTHER)))
								((npc_utherAI*)pUther->AI())->StartAI();

							m_pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, IN_PROGRESS);
						}
						break;
            }
            break;
    }
       return true;
}

bool QuestAccept_npc_chromie(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    switch (pQuest->GetQuestId())
    {
        case QUEST_DISPELLING_ILLUSIONS:
            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
                if (m_pInstance->GetData(TYPE_GRAIN_EVENT) == NOT_STARTED)
                    m_pInstance->SetData(TYPE_GRAIN_EVENT, SPECIAL);
            break;
        case QUEST_A_ROYAL_ESCORT:
            if (instance_culling_of_stratholme* m_pInstance = (instance_culling_of_stratholme*)pCreature->GetInstanceData())
				if (m_pInstance->GetData(TYPE_ARTHAS_INTRO_EVENT) == NOT_STARTED)
				{
					//m_pInstance->DoSpawnArthasIfNeeded();
					m_pInstance->DoUpdateWorldState(WORLD_STATE_CRATES, 0);

					if(Creature *pUther = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_UTHER)))
						((npc_utherAI*)pUther->AI())->StartAI(); 

					m_pInstance->SetData(TYPE_ARTHAS_INTRO_EVENT, IN_PROGRESS);
				}
            break;
    }
    return true;
}

/*###
## npc_arthas_dialog
###*/

enum
{
    GOSSIP_MENU_ARTHAS_1                        = 100001,
    GOSSIP_MENU_ARTHAS_2                        = 100002,
    GOSSIP_MENU_ARTHAS_3                        = 100003,
    GOSSIP_MENU_ARTHAS_4                        = 100004,
    GOSSIP_MENU_ARTHAS_5                        = 100005
};

#define GOSSIP_ITEM_ARTHAS_0 "I'm ready to start Culling of Stratholme."
#define GOSSIP_ITEM_ARTHAS_1 "Yes, my Prince. We're ready."
#define GOSSIP_ITEM_ARTHAS_2 "We're only doing what is best for Loarderon your Highness."
#define GOSSIP_ITEM_ARTHAS_3 "I'm ready."
#define GOSSIP_ITEM_ARTHAS_4 "For Lordaeron!"
#define GOSSIP_ITEM_ARTHAS_5 "I'm ready to battle the dreadlord, sire."

bool GossipHello_npc_arthas(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = ((ScriptedInstance*)pCreature->GetInstanceData()); 

    if(pInstance && pInstance->GetData(TYPE_PHASE) == 0) 
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_1, pCreature->GetGUID()); 
    }

    if(pInstance && pInstance->GetData(TYPE_PHASE) == 5)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_2, pCreature->GetGUID()); 
    }

    if(pInstance && pInstance->GetData(TYPE_PHASE) == 8)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_3, pCreature->GetGUID()); 
    }

    if(pInstance && pInstance->GetData(TYPE_PHASE) == 9)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ARTHAS_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_MENU_ARTHAS_4, pCreature->GetGUID()); 
    }

    return true; 
}

bool GossipSelect_npc_arthas(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());

    // intro
    if(m_pInstance && m_pInstance->GetData(TYPE_PHASE) == 0)
    {
        m_pInstance->SetData(TYPE_PHASE, 1);
        ((npc_arthasAI*)pCreature->AI())->EnableEscort();
        ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
        ((npc_arthasAI*)pCreature->AI())->culling_faction = pPlayer->getFaction();
    }

    // enter townhall
    if(m_pInstance && m_pInstance->GetData(TYPE_PHASE) == 5)
    {
        ((npc_arthasAI*)pCreature->AI())->EnableEscort();
        ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
    }

    // after epoch
    if(m_pInstance && m_pInstance->GetData(TYPE_PHASE) == 8)
    {
        ((npc_arthasAI*)pCreature->AI())->EnableEscort();
        ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
    }

    // malganis
    if(m_pInstance && m_pInstance->GetData(TYPE_PHASE) == 9)
    {
        ((npc_arthasAI*)pCreature->AI())->EnableEscort();
        ((npc_arthasAI*)pCreature->AI())->RemoveGossip();
        if(Creature* pMalganis = pCreature->SummonCreature(NPC_MALGANIS,2296.665f,1502.362f,128.362f,4.961f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,900000))
        {
            m_pInstance->SetData64(NPC_MALGANIS, pMalganis->GetGUID());
            pMalganis->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MALGANIS_GATE1)))
                 pGate->SetGoState(GO_STATE_ACTIVE); 
        }
    }

    return true; 
}

/*###
## npc_arthas_priest
###*/

enum
{
    SPELL_SMITE         = 61923,
    SPELL_HEAL          = 62442
};

struct MANGOS_DLL_DECL npc_arthas_priestAI : public ScriptedAI
{
    npc_arthas_priestAI(Creature *c) : ScriptedAI(c)
    {
        SetCombatMovement(false);
        Reset();
    }

    uint32 m_uiSmiteTimer;
    uint32 m_uiHealTimer;

    void Reset() 
    {
        m_uiSmiteTimer = 100;
        m_uiHealTimer = 1000;
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
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreaturesAddon();

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    AttackStart(pWho);
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho, 0.0f);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiSmiteTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SMITE);
            m_uiSmiteTimer = 3000;
        }
        else m_uiSmiteTimer -= uiDiff;

        if(m_uiHealTimer < uiDiff)
        {
            if(m_creature->GetHealthPercent() <= 40.0f)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCast(m_creature, SPELL_HEAL);
                m_uiHealTimer = 3000;
            }
        }
        else m_uiHealTimer -= uiDiff;

        DoMeleeAttackIfReady();

        return;
    }
};

/*###
## npc_arthas_marine
###*/

struct MANGOS_DLL_DECL npc_arthas_marineAI : public ScriptedAI
{
    npc_arthas_marineAI(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }

    float LastX;
    float LastY;
    float LastZ;

    uint32 m_uiHealTimer;

    void Reset() 
    {
        m_uiHealTimer = 3000;
    }

    void Aggro(Unit* who)
    {
        LastX = m_creature->GetPositionX();
        LastY = m_creature->GetPositionY();
        LastZ = m_creature->GetPositionZ();
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

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreaturesAddon();

        m_creature->SetLootRecipient(NULL);
        m_creature->GetMotionMaster()->MovePoint(POINT_LAST_POINT, LastX, LastY, LastZ);

        Reset();
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        if (!m_creature->hasUnitState(UNIT_STAT_STUNNED) && pWho->isTargetableForAttack() &&
            m_creature->IsHostileTo(pWho) && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (!m_creature->canFly() && m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if (m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho))
            {
                if (!m_creature->getVictim())
                {
                    AttackStart(pWho);
                    pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                }
                else if (m_creature->GetMap()->IsDungeon())
                {
                    pWho->SetInCombatWith(m_creature);
                    m_creature->AddThreat(pWho, 0.0f);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiHealTimer < uiDiff)
        {
            if(m_creature->GetHealthPercent() <= 40.0f)
            {
                if(Creature* pHeal = GetClosestCreatureWithEntry(m_creature, NPC_PRIEST, 50.0f))
                {
                    if(pHeal->GetHealthPercent() > 40.0f)
                    {
                        pHeal->InterruptNonMeleeSpells(false);
                        pHeal->CastSpell(m_creature, SPELL_HEAL, false);
                        m_uiHealTimer = 3000;
                    }
                }
            }
        }
        else m_uiHealTimer -= uiDiff;

        DoMeleeAttackIfReady();

        return;
    }
};

/*###
## npc_dark_conversion
###*/

/*enum
{
SAY_PEOPLE01         = -1594099,
SAY_PEOPLE02         = -1594100,
SAY_PEOPLE03         = -1594101,
SAY_PEOPLE04         = -1594102,
SAY_PEOPLE05         = -1594103,
};*/

struct MANGOS_DLL_DECL npc_dark_conversionAI : public ScriptedAI
{
    npc_dark_conversionAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    bool Special;
    bool Conversion;
    uint32 m_uiStep;
    uint32 m_uiStepTimer;

    void Reset() 
    {
        m_creature->setFaction(35);
        Conversion = false;
        Special = false;
        m_uiStep = 1;
        m_uiStepTimer = 5000;
    }

    void MalganisScared(Creature* target, float horizontalSpeed, float verticalSpeed)
    {
        float angle = target->GetAngle(m_creature);
        float vsin = sin(angle);
        float vcos = cos(angle);

        float ox, oy, oz;
        m_creature->GetPosition(ox, oy, oz);

        float g = 19.23f;// seems that physic constant g(earth's gravity) in world of warcraft is about 2 times larger than real
        float dh = verticalSpeed*verticalSpeed / (2*g); // maximum parabola height
        float time = sqrtf(dh/(0.124976 * verticalSpeed));  //full move time in seconds     // should be  time = 2*Vert_speed/g, but..

        float dis = time * horizontalSpeed;
        float fx = ox + dis * vcos;
        float fy = oy + dis * vsin;
        float fz = oz;

        m_creature->UpdateGroundPositionZ(fx, fy, fz);

        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, fx, fy, fz);
    }

    void DarkConversion(bool Move)
    {
        m_creature->UpdateEntry(NPC_ZOMBIE);
        if(Move == true)
        {
            uint64 m_uiArthasGUID = m_pInstance->GetData64(NPC_ARTHAS);
            if(Creature* pArthas = m_pInstance->instance->GetCreature(m_uiArthasGUID))
                m_creature->GetMotionMaster()->MovePoint(0, pArthas->GetPositionX(), pArthas->GetPositionY(), pArthas->GetPositionZ());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_pInstance) return;

        if(m_pInstance->GetData(TYPE_ENCOUNTER) == IN_PROGRESS)
        {
            if(Creature* pMalganis = GetClosestCreatureWithEntry(m_creature, NPC_MALGANIS_INTRO, 20.0f))
            {
                if(Special == false)
                {
                    float Dist = m_creature->GetDistance2d(pMalganis->GetPositionX(), pMalganis->GetPositionY());
                    Dist = Dist + 2.0f;
                    MalganisScared(pMalganis, Dist, 1.0f);
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                    m_uiStepTimer = 5000;
                    Special = true;
                }
            }

            if(m_uiStepTimer < uiDiff && Conversion != true)
            {
                Conversion = true;
                if(Special != false)
                    DarkConversion(true);
                else
                    DarkConversion(false);
            }
            else m_uiStepTimer -= uiDiff;

        }

        DoMeleeAttackIfReady();

        return;
    }
};

struct MANGOS_DLL_DECL mob_risen_zombieAI : public ScriptedAI
{
    mob_risen_zombieAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroic = pCreature->GetMap()->IsRaidOrHeroicDungeon();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroic;

    void Reset() 
    {
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance && m_bIsHeroic)
        {
            if(m_pInstance->GetData(TYPE_ZOMBIEFEST) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_ZOMBIEFEST, IN_PROGRESS);
            m_pInstance->SetData(TYPE_ZOMBIE_COUNT, 1);
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_uther(Creature* pCreature)
{
    return new npc_utherAI(pCreature);
}

CreatureAI* GetAI_npc_arthas(Creature* pCreature)
{
    return new npc_arthasAI(pCreature);
}

CreatureAI* GetAI_npc_arthas_priest(Creature* pCreature)
{
    return new npc_arthas_priestAI(pCreature);
}

CreatureAI* GetAI_npc_arthas_marine(Creature* pCreature)
{
    return new npc_arthas_marineAI(pCreature);
}

CreatureAI* GetAI_npc_dark_conversion(Creature* pCreature)
{
    return new npc_dark_conversionAI(pCreature);
}

CreatureAI* GetAI_mob_risen_zombie(Creature* pCreature)
{
    return new mob_risen_zombieAI(pCreature);
}

void AddSC_culling_of_stratholme()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "npc_chromie";
    newscript->pGossipHello = &GossipHello_npc_chromie;
    newscript->pGossipSelect = &GossipSelect_npc_chromie;
    newscript->pQuestAccept = &QuestAccept_npc_chromie;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_mike";
    newscript->GetAI = &GetAI_npc_mike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_roger";
    newscript->GetAI = &GetAI_npc_roger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_morigan";
    newscript->GetAI = &GetAI_npc_morigan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_jena";
    newscript->GetAI = &GetAI_npc_jena;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_malcolm";
    newscript->GetAI = &GetAI_npc_malcolm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_bartleby_cs";
    newscript->GetAI = &GetAI_npc_bartleby_cs;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_stratholme_crates";
    newscript->GetAI = &GetAI_npc_stratholme_crates;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "spell_dummy_npc_crates_bunny";
    newscript->pEffectAuraDummy = &EffectAuraDummy_spell_aura_dummy_npc_crates_dummy;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_uther";
    newscript->GetAI = &GetAI_npc_uther;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_arthas";
    newscript->GetAI = &GetAI_npc_arthas;
    newscript->pGossipHello =  &GossipHello_npc_arthas;
    newscript->pGossipSelect = &GossipSelect_npc_arthas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_arthas_priest";
    newscript->GetAI = &GetAI_npc_arthas_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_arthas_marine";
    newscript->GetAI = &GetAI_npc_arthas_marine;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_dark_conversion";
    newscript->GetAI = &GetAI_npc_dark_conversion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_risen_zombie";
    newscript->GetAI = &GetAI_mob_risen_zombie;
    newscript->RegisterSelf();
}
