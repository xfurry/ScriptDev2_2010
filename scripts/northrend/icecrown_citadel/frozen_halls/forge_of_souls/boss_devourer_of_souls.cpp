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
SDName: boss_devourer_of_souls
SD%Complete: 60%
SDComment:  Texts, outro, well of souls
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    NPC_HORDE_CHAMPION1         = 37584,
    NPC_HORDE_CHAMPION2         = 37588,
    NPC_HORDE_CHAMPION3         = 37587,
    NPC_HORDE_SLYVANAS          = 38161,
    NPC_HORDE_LORALEN           = 37779,
    NPC_HORDE_KELIRA            = 37583,

    SPELL_PHANTOM_BLAST         = 68982,
    SPELL_PHANTOM_BLAST_H       = 70322,
    SPELL_MIRRORED_SOUL         = 69051,
    SPELL_WELL_OF_SOULS         = 68820,
    SPELL_UNLEASHED_SOULS       = 68939,
    SPELL_WAILING_SOULS         = 70324,
    SPELL_WELL_OF_SOULS_VIS     = 68854,

    SPELL_WELL_OF_SOULS_AURA    = 68854,

    ACHIEVEMENT_THREE_FACED     = 4523,

    /*Others*/
    MODEL_FAT                     = 30149,
    MODEL_WOMAN                   = 30150,

    MOB_WELL_OF_SOULS           = 36536,

    ACHIEV_THREE_FACED          = 4523,
};
//Spells

enum Yells
{
    EMOTE_MIRRORED_SOUL                     = -1610023,
    EMOTE_WAILING_SOULS                     = -1610024,
    EMOTE_UNLEASHED_SOULS                   = -1610025,
    SAY_AGGRO_FEMALE                        = -1610026,
    SAY_WAILING_SOULS_FEMALE                = -1610027,
    SAY_UNLEASH_SOULS_FEMALE                = -1610028,
    SAY_SLAY1_FEMALE                        = -1610029,
    SAY_SLAY2_FEMALE                        = -1610030,
    SAY_DEATH_FEMALE                        = -1610031,
    SAY_AGGRO_MALE                          = -1610032,
    SAY_WAILING_SOULS_MALE                  = -1610033,
    SAY_UNLEASH_SOULS_MALE                  = -1610034,
    SAY_SLAY1_MALE                          = -1610035,
    SAY_SLAY2_MALE                          = -1610036,
    SAY_DEATH_MALE                          = -1610037,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};
static LocationsXY SummonLoc[]=
{
    {5590.47f, 2427.79f, 705.935f, 0.802851f}, //champ1
    {5593.59f, 2428.34f, 705.935f, 0.977384f},
    {5600.81f, 2429.31f, 705.935f, 0.890118f},
    {5600.80f, 2421.12f, 705.935f, 0.890118f},
    {5601.43f, 2426.53f, 705.935f, 0.890118f},
    {5601.55f, 2418.36f, 705.935f, 1.151920f},
    {5598.00f, 2429.14f, 705.935f, 1.047200f},
    {5594.04f, 2424.87f, 705.935f, 1.151920f},
    {5597.89f, 2421.54f, 705.935f, 0.610865f},
    {5598.57f, 2434.62f, 705.935f, 1.134460f},
    {5585.46f, 2417.99f, 705.935f, 1.064650f},// champ2
    {5585.46f, 2417.99f, 705.935f, 1.064650f},
    {5605.81f, 2428.42f, 705.935f, 0.820305f},
    {5591.61f, 2412.66f, 705.935f, 0.925025f},
    {5593.90f, 2410.64f, 705.935f, 0.872665f},
    {5586.76f, 2416.73f, 705.935f, 0.942478f},
    {5592.23f, 2419.14f, 705.935f, 0.855211f},// champ3
    {5594.61f, 2416.87f, 705.935f, 0.907571f},
    {5589.77f, 2421.03f, 705.935f, 0.855211f},
    {5606.12f, 2436.60f, 705.935f, 0.890110f},// sylvanas / jaina
    {5602.58f, 2435.95f, 705.935f, 0.959931f},// lolaren / elandra
    {5606.13f, 2433.16f, 705.935f, 0.785398f},// kalira / koreln
};

static LocationsXY MoveLoc[]=
{
    {5650.92f, 2488.89f, 708.696f}, // jaina / sylvanas
    {5637.01f, 2482.89f, 708.695f}, // champions
};

#define HOME_X                      5661.58f
#define HOME_Y                      2507.2f

/******** JAINA & SYLVANAS *******/

enum
{
    SAY_SYLVANAS                           = -1610015,
    SAY_JAINA                              = -1610008,
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_fos_endAI: public ScriptedAI
{
    npc_sylvanas_jaina_fos_endAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiOutro_Phase;
    bool m_bIsOutro;
    uint32 creatureEntry;

    void Reset()
    {
        m_uiOutro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsOutro          = true;
        creatureEntry = m_creature->GetEntry();
    }

    void SummonHordeChampions()
    {
        for (uint8 i = 0; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_HORDE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 16; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_HORDE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        for (uint8 i = 16; i < 19; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_HORDE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        Creature *pLoralen = m_creature->SummonCreature(NPC_LORALEN, SummonLoc[20].x, SummonLoc[20].y, SummonLoc[20].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pLoralen)
            pLoralen->GetMotionMaster()->MovePoint(0, MoveLoc[0].x - 5, MoveLoc[0].y + 5, MoveLoc[0].z);

        Creature *pKalira = m_creature->SummonCreature(NPC_KALIRA, SummonLoc[21].x, SummonLoc[21].y, SummonLoc[21].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pKalira)
            pKalira->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + 5, MoveLoc[0].y - 5, MoveLoc[0].z);

    }

    void SummonAlyChampions()
    {
        for (uint8 i = 0; i < 10; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_1_ALLIANCE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        for (uint8 i = 10; i < 16; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_2_ALLIANCE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        for (uint8 i = 16; i < 19; i++)
        {
            Creature *pTemp = m_creature->SummonCreature(NPC_CHAMPION_3_ALLIANCE, SummonLoc[i].x, SummonLoc[i].y, SummonLoc[i].z, SummonLoc[i].o, TEMPSUMMON_DEAD_DESPAWN, 0);
            if (pTemp)
                pTemp->GetMotionMaster()->MovePoint(0, MoveLoc[1].x + urand(0, 40), MoveLoc[1].y + urand(0, 50), MoveLoc[1].z);
        }

        Creature *pElandra = m_creature->SummonCreature(NPC_ELANDRA, SummonLoc[20].x, SummonLoc[20].y, SummonLoc[20].z, SummonLoc[20].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pElandra)
            pElandra->GetMotionMaster()->MovePoint(0, MoveLoc[0].x - 5, MoveLoc[0].y + 5, MoveLoc[0].z);

        Creature *pKoreln = m_creature->SummonCreature(NPC_KORELN, SummonLoc[21].x, SummonLoc[21].y, SummonLoc[21].z, SummonLoc[21].o, TEMPSUMMON_DEAD_DESPAWN, 0);
        if (pKoreln)
            pKoreln->GetMotionMaster()->MovePoint(0, MoveLoc[0].x + 5, MoveLoc[0].y - 5, MoveLoc[0].z);
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
                    case NPC_JAINA_END:
                        SummonAlyChampions();
                        break;
                    case NPC_SYLVANAS_END:
                        SummonHordeChampions();
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 30000;
                    break;
                case 1:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_END:
                        DoScriptText(SAY_JAINA, m_creature);
                        break;
                    case NPC_SYLVANAS_END:
                        DoScriptText(SAY_SYLVANAS, m_creature);
                        break;
                    }
                    ++m_uiOutro_Phase;
                    m_uiSpeech_Timer = 5000;
                    break;

                default:
                    m_uiSpeech_Timer = 100000;
                }
            }else m_uiSpeech_Timer -= uiDiff;
        }
    }
};

struct MANGOS_DLL_DECL boss_Devourer_of_SoulsAI : public ScriptedAI
{
    boss_Devourer_of_SoulsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPhantomBlastTimer;
    uint32 m_uiMirroredSoulTimer;
    uint32 m_uiWellOfSoulsTimer;
    uint32 m_uiUnleashedSoulsTimer;
    uint32 m_uiUnleashedSoulsEnd;
    uint32 m_uiWailingSoulsTimer;

    bool m_bIsThreeFaced;

    uint32 TeamInInstance;

    void Reset()
    {        
        m_bIsThreeFaced         = true;

        TeamInInstance = GetFaction();

        m_uiPhantomBlastTimer   = 10000;
        m_uiMirroredSoulTimer   = 20000;
        m_uiWellOfSoulsTimer    = 30000;
        m_uiUnleashedSoulsTimer = 25000;
        m_uiUnleashedSoulsEnd   = 40000;
        m_uiWailingSoulsTimer   = 28000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEVOURER_OF_SOULS, NOT_STARTED);
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

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEVOURER_OF_SOULS, IN_PROGRESS);

        if(irand(0,1))
            DoScriptText(SAY_AGGRO_FEMALE, m_creature);
        else
            DoScriptText(SAY_AGGRO_MALE, m_creature);
    }

    void KilledUnit(Unit *pVictim)
    {
        switch (urand(0, 3))
        {
        case 0:
            DoScriptText(SAY_SLAY1_FEMALE, m_creature);
            break;
        case 1:
            DoScriptText(SAY_SLAY2_FEMALE, m_creature);
            break;
        case 2:
            DoScriptText(SAY_SLAY1_MALE, m_creature);
            break;
        case 3:
            DoScriptText(SAY_SLAY2_MALE, m_creature);
            break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPhantomBlastTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_PHANTOM_BLAST : SPELL_PHANTOM_BLAST_H) == CAST_OK)
                    m_bIsThreeFaced = false;
            }
            m_uiPhantomBlastTimer = urand(6000, 8000);
        }
        else
            m_uiPhantomBlastTimer -= uiDiff;

        if (m_uiMirroredSoulTimer < uiDiff)
        {
            DoScriptText(EMOTE_MIRRORED_SOUL, m_creature);
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_MIRRORED_SOUL);
            m_uiMirroredSoulTimer = 20000;
        }
        else
            m_uiMirroredSoulTimer -= uiDiff;

        if (m_uiWellOfSoulsTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WELL_OF_SOULS);
            m_uiWellOfSoulsTimer = 20000;
        }
        else
            m_uiWellOfSoulsTimer -= uiDiff;

        if (m_uiUnleashedSoulsTimer < uiDiff)
        {
            switch (urand(0, 1))
            {
            case 0:
                DoScriptText(SAY_UNLEASH_SOULS_FEMALE, m_creature);
                break;
            case 1:
                DoScriptText(SAY_UNLEASH_SOULS_MALE, m_creature);
                break;
            }

            DoScriptText(EMOTE_UNLEASHED_SOULS, m_creature);
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_UNLEASHED_SOULS);

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetDisplayId(MODEL_FAT);
            m_creature->RemoveAllAuras();
            m_uiUnleashedSoulsEnd = 5000;
            m_uiUnleashedSoulsTimer = 30000;
        }
        else
            m_uiUnleashedSoulsTimer -= uiDiff;

        if(m_uiUnleashedSoulsEnd < uiDiff)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetDisplayId(MODEL_WOMAN);
            m_uiUnleashedSoulsEnd = 60000;
        }
        else m_uiUnleashedSoulsEnd -= uiDiff;

        if (m_uiWailingSoulsTimer < uiDiff)
        {
            if(irand(0,1))
                DoScriptText(SAY_WAILING_SOULS_FEMALE, m_creature);
            else
                DoScriptText(SAY_WAILING_SOULS_MALE, m_creature);
            DoScriptText(EMOTE_WAILING_SOULS, m_creature);

            DoCast(m_creature->getVictim(), SPELL_WAILING_SOULS);

            m_uiWailingSoulsTimer = 15000;
        }
        else
            m_uiWailingSoulsTimer -= uiDiff;

        DoMeleeAttackIfReady();

        if (m_creature->GetDistance2d(HOME_X, HOME_Y) > 50)
            EnterEvadeMode();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_DEVOURER_OF_SOULS, DONE);

            if(!m_bIsRegularMode && m_bIsThreeFaced)
                m_pInstance->DoCompleteAchievement(ACHIEV_THREE_FACED);
        }

        switch (urand(0, 1))
        {
        case 0:
            DoScriptText(SAY_DEATH_FEMALE, m_creature);
            break;
        case 1:
            DoScriptText(SAY_DEATH_MALE, m_creature);
            break;
        }

        if(TeamInInstance == ALLIANCE)
        {
            if(Creature* pJaina = m_creature->SummonCreature(NPC_JAINA_END, SummonLoc[19].x, SummonLoc[19].y, SummonLoc[19].z, SummonLoc[19].o, TEMPSUMMON_DEAD_DESPAWN, 0))
                pJaina->GetMotionMaster()->MovePoint(0, MoveLoc[0].x, MoveLoc[0].y, MoveLoc[0].z);
        }

        if(TeamInInstance == HORDE)
        {
            if(Creature* pSylvanas = m_creature->SummonCreature(NPC_SYLVANAS_END, SummonLoc[19].x, SummonLoc[19].y, SummonLoc[19].z, SummonLoc[19].o, TEMPSUMMON_DEAD_DESPAWN, 0))
                pSylvanas->GetMotionMaster()->MovePoint(0, MoveLoc[0].x, MoveLoc[0].y, MoveLoc[0].z);
        }
    }
};

CreatureAI* GetAI_boss_Devourer_of_Souls(Creature* pCreature)
{
    return new boss_Devourer_of_SoulsAI (pCreature);
}

struct MANGOS_DLL_DECL mob_well_of_soulsAI : public ScriptedAI
{
    mob_well_of_soulsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetDisplayId(11686);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    { 
        DoCast(m_creature, SPELL_WELL_OF_SOULS_AURA);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_DEVOURER_OF_SOULS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_well_of_souls(Creature* pCreature)
{
    return new mob_well_of_soulsAI (pCreature);
}

CreatureAI* GetAI_npc_sylvanas_jaina_fos_end(Creature* pCreature)
{
    return new npc_sylvanas_jaina_fos_endAI (pCreature);
}

void AddSC_boss_Devourer_Of_Souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_Devourer_of_Souls";
    newscript->GetAI = &GetAI_boss_Devourer_of_Souls;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_well_of_souls";
    newscript->GetAI = &GetAI_mob_well_of_souls;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_fos_end;
    newscript->Name = "npc_slyvanas_jaina_fos_end";
    newscript->RegisterSelf();
}
