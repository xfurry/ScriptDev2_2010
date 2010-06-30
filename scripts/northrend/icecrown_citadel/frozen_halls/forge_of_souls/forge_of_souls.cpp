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
SDName: forge_of_souls
SD%Complete: 70%
SDComment:  Implement event (Jaina & Sylvanas)
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum
{
    /**** SPELLS ****/

    //Spiteful Apparition
    SPELL_SPITE                 = 68895,
    SPELL_SPITE_H               = 70212,

    //Spectral Warden
    SPELL_VEIL_OF_SHADOWS       = 69633,
    SPELL_WAIL_OF_SOULS         = 69148,
    SPELL_WAIL_OF_SOULS_H       = 70210,

    //Soulguard Watchman
    SPELL_SHROUD_OF_RUNES       = 69056,
    SPELL_UNHOLY_RAGE           = 69053,

    //Soulguard Reaper
    SPELL_FROST_NOVA            = 69060,
    SPELL_FROST_NOVA_H          = 70209,
    SPELL_SHADOW_LANCE          = 69058,

    //Soulguard Bonecaster
    SPELL_BONE_VOLLEY           = 69080,
    SPELL_BONE_VOLLEY_H         = 70206,
    SPELL_RAISE_DEAD            = 69562,
    SPELL_SHIELD_OF_BONES       = 69069,
    SPELL_SHIELD_OF_BONES_H     = 70207,

    //Soulguard Animator
    // Raise dead 69562
    SPELL_SHADOW_BOLT           = 69068,
    SPELL_SHADOW_BOLT_H         = 70208, 
    SPELL_SOUL_SICKNESS         = 69131,
    SPELL_SOUL_SIPHON           = 69128,

    //Soulguard Adept
    //Raise dead 69562
    //Shadow Bolt 69068/70208
    SPELL_DRAIN_LIFE            = 69066,
    SPELL_DRAIN_LIFE_H          = 70213,
    SPELL_SHADOW_MEND           = 69564,
    SPELL_SHADOW_MEND_H         = 70205,

    //Soul Horror
    SPELL_SOUL_STRIKE           = 69088,
    SPELL_SOUL_STRIKE_H         = 70211,
};

struct MANGOS_DLL_DECL npc_SpitefulApparitionAI: public ScriptedAI
{
    npc_SpitefulApparitionAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpiteTimer;

    void Reset()
    {
        m_uiSpiteTimer = 8000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpiteTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SPITE : SPELL_SPITE_H);
            m_uiSpiteTimer = 8000;
        }
        else
            m_uiSpiteTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SpectralWardenAI: public ScriptedAI
{
    npc_SpectralWardenAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiVeilOfShadowsTimer;
    uint32 m_uiWailOfSoulsTimer;

    void Reset()
    {
        m_uiVeilOfShadowsTimer  = 5000;
        m_uiWailOfSoulsTimer    = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVeilOfShadowsTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_VEIL_OF_SHADOWS);
            m_uiVeilOfShadowsTimer = 10000;
        }
        else
            m_uiVeilOfShadowsTimer -= uiDiff;

        if (m_uiWailOfSoulsTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_WAIL_OF_SOULS : SPELL_WAIL_OF_SOULS_H);
            m_uiWailOfSoulsTimer = 5000;
        }
        else
            m_uiWailOfSoulsTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulguardWatchmanAI: public ScriptedAI
{
    npc_SoulguardWatchmanAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShroudOfRunesTimer;
    uint32 m_uiUnholyRageTimer;

    void Reset()
    {
        m_uiShroudOfRunesTimer  = 1000;
        m_uiUnholyRageTimer     = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShroudOfRunesTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_SHROUD_OF_RUNES);
            m_uiShroudOfRunesTimer = 5000;
        }
        else
            m_uiShroudOfRunesTimer -= uiDiff;

        if (m_uiUnholyRageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_UNHOLY_RAGE);
            m_uiUnholyRageTimer = 8000;
        }
        else
            m_uiUnholyRageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulguardReaperAI: public ScriptedAI
{
    npc_SoulguardReaperAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFrostNovaTimer;
    uint32 m_uiShadowLanceTimer;

    void Reset()
    {
        m_uiFrostNovaTimer      = 8000;
        m_uiShadowLanceTimer    = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFrostNovaTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_FROST_NOVA : SPELL_FROST_NOVA_H);
            m_uiFrostNovaTimer = 9600;
        }
        else
            m_uiFrostNovaTimer -= uiDiff;

        if (m_uiShadowLanceTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SHADOW_LANCE);
            m_uiShadowLanceTimer = 8000;
        }
        else
            m_uiShadowLanceTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulguardBonecasterAI: public ScriptedAI
{
    npc_SoulguardBonecasterAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBoneVolleyTimer;
    uint32 m_uiRaiseDeadTimer;
    bool m_bHasShield;

    void Reset()
    {
        m_uiBoneVolleyTimer     = 6000;
        m_uiRaiseDeadTimer      = 25000;
        m_bHasShield            = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBoneVolleyTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_BONE_VOLLEY : SPELL_BONE_VOLLEY_H);
            m_uiBoneVolleyTimer = 7000;
        }
        else
            m_uiBoneVolleyTimer -= uiDiff;

        if (m_uiRaiseDeadTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_RAISE_DEAD);
            m_uiRaiseDeadTimer = 25000;
        }
        else
            m_uiRaiseDeadTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 30.0f && !m_bHasShield)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHIELD_OF_BONES : SPELL_SHIELD_OF_BONES_H);
            m_bHasShield = true;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulguardAnimatorAI: public ScriptedAI
{
    npc_SoulguardAnimatorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRaiseDeadTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiSoulSicknessTimer;
    uint32 m_uiSoulSiphonTimer;

    void Reset()
    {
        m_uiRaiseDeadTimer      = 25000;
        m_uiShadowBoltTimer     = 5000;
        m_uiSoulSicknessTimer   = 8000;
        m_uiSoulSiphonTimer     = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRaiseDeadTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_RAISE_DEAD);
            m_uiRaiseDeadTimer = 25000;
        }
        else
            m_uiRaiseDeadTimer -= uiDiff;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
            m_uiShadowBoltTimer = 5000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        if (m_uiSoulSicknessTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SOUL_SICKNESS);
            m_uiSoulSicknessTimer = 10000;
        }
        else
            m_uiSoulSicknessTimer -= uiDiff;

        if (m_uiSoulSiphonTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SOUL_SIPHON);
            m_uiSoulSiphonTimer = 8000;
        }
        else
            m_uiSoulSiphonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulguardAdeptAI: public ScriptedAI
{
    npc_SoulguardAdeptAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiRaiseDeadTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiDrainLifeTimer;
    uint32 m_uiShadowMendTimer;

    void Reset()
    {
        m_uiRaiseDeadTimer      = 25000;
        m_uiShadowBoltTimer     = 8000;
        m_uiDrainLifeTimer      = 7000;
        m_uiShadowMendTimer     = 35000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRaiseDeadTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_RAISE_DEAD);
            m_uiRaiseDeadTimer = 25000;
        }
        else
            m_uiRaiseDeadTimer -= uiDiff;

        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
            m_uiShadowBoltTimer = 4000;
        }
        else
            m_uiShadowBoltTimer -= uiDiff;

        if (m_uiDrainLifeTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DRAIN_LIFE : SPELL_DRAIN_LIFE_H);
            m_uiDrainLifeTimer = 9000;
        }
        else
            m_uiDrainLifeTimer -= uiDiff;

        if (m_uiShadowMendTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_MEND : SPELL_SHADOW_MEND_H);
            m_uiShadowMendTimer = 20000;
        }
        else
            m_uiShadowMendTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_SoulHorrorAI: public ScriptedAI
{
    npc_SoulHorrorAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSoulStrikeTimer;

    void Reset()
    {
        m_uiSoulStrikeTimer = 6000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSoulStrikeTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SOUL_STRIKE : SPELL_SOUL_STRIKE_H);
            m_uiSoulStrikeTimer = 8000;
        }
        else
            m_uiSoulStrikeTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/******** JAINA & SYLVANAS *******/

#define GOSSIP_ITEM_SYLVANAS "What would you have of me, Banshee Queen?"
#define GOSSIP_ITEM_JAINA "What should we do, Lady Jaina?"

enum
{
    SAY_SPEECH_SYLVANAS1                              = -1610009,
    SAY_SPEECH_SYLVANAS2                              = -1610010,
    SAY_SPEECH_SYLVANAS3                              = -1610011,
    SAY_SPEECH_SYLVANAS4                              = -1610012,
    SAY_SPEECH_SYLVANAS5                              = -1610013,
    SAY_SPEECH_SYLVANAS6                              = -1610014,

    SAY_SPEECH_JAINA1                              = -1610000,
    SAY_SPEECH_JAINA2                              = -1610001,
    SAY_SPEECH_JAINA3                              = -1610002,
    SAY_SPEECH_JAINA4                              = -1610003,
    SAY_SPEECH_JAINA5                              = -1610004,
    SAY_SPEECH_JAINA6                              = -1610005,
    SAY_SPEECH_JAINA7                              = -1610006,
    SAY_SPEECH_JAINA8                              = -1610007,
};

struct MANGOS_DLL_DECL npc_sylvanas_jaina_fos_startAI: public ScriptedAI
{
    npc_sylvanas_jaina_fos_startAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiSpeech_Timer;
    uint8 m_uiIntro_Phase;
    bool m_bIsIntro;
    uint32 creatureEntry;

    void Reset()
    {
        m_uiIntro_Phase     = 0;
        m_uiSpeech_Timer    = 1000;
        m_bIsIntro          = false;
        creatureEntry = m_creature->GetEntry();
    }

    void StartIntro()
    {
        m_bIsIntro = true;
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
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA1, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS1, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 12000;
                    break;
                case 1:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA2, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS2, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 2:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA3, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS3, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 3:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA4, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS4, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 4:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA5, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS5, m_creature);
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 10000;
                    break;
                case 5:
                    switch (creatureEntry)
                    {
                    case NPC_JAINA_START:
                        DoScriptText(SAY_SPEECH_JAINA6, m_creature);
                        break;
                    case NPC_SYLVANAS_START:
                        DoScriptText(SAY_SPEECH_SYLVANAS6, m_creature);
                        m_bIsIntro = false;
                        break;
                    }
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 12000;
                    break;
                case 6:
                    DoScriptText(SAY_SPEECH_JAINA7, m_creature);
                    ++m_uiIntro_Phase;
                    m_uiSpeech_Timer = 6000;
                    break;
                case 8:
                    DoScriptText(SAY_SPEECH_JAINA8, m_creature);
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

bool GossipHello_npc_slyvanas_jaina_fos(Player* pPlayer, Creature* pCreature)
{
    uint32 creatureEntry;
    creatureEntry = pCreature->GetEntry();

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    switch (creatureEntry)
    {
    case NPC_JAINA_START:
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_JAINA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        break;
    case NPC_SYLVANAS_START:
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SYLVANAS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        break;
    }
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_slyvanas_jaina_fos(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
    case GOSSIP_ACTION_INFO_DEF+1:
        ((npc_sylvanas_jaina_fos_startAI*)pCreature->AI())->StartIntro();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pPlayer->CLOSE_GOSSIP_MENU();
    }

    return true;
}

CreatureAI* GetAI_npc_SpitefulApparitionAI(Creature* pCreature)
{
    return new npc_SpitefulApparitionAI (pCreature);
}

CreatureAI* GetAI_npc_SpectralWardenAI(Creature* pCreature)
{
    return new npc_SpectralWardenAI (pCreature);
}

CreatureAI* GetAI_npc_SoulguardWatchmanAI(Creature* pCreature)
{
    return new npc_SoulguardWatchmanAI (pCreature);
}

CreatureAI* GetAI_npc_SoulguardReaperAI(Creature* pCreature)
{
    return new npc_SoulguardReaperAI (pCreature);
}

CreatureAI* GetAI_npc_SoulguardBonecasterAI(Creature* pCreature)
{
    return new npc_SoulguardBonecasterAI (pCreature);
}

CreatureAI* GetAI_npc_SoulguardAnimatorAI(Creature* pCreature)
{
    return new npc_SoulguardAnimatorAI (pCreature);
}

CreatureAI* GetAI_npc_SoulguardAdeptAI(Creature* pCreature)
{
    return new npc_SoulguardAdeptAI (pCreature);
}

CreatureAI* GetAI_npc_SoulHorrorAI(Creature* pCreature)
{
    return new npc_SoulHorrorAI (pCreature);
}

CreatureAI* GetAI_npc_sylvanas_jaina_fos_start(Creature* pCreature)
{
    return new npc_sylvanas_jaina_fos_startAI (pCreature);
}

void AddSC_Forge_Of_Souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_SpitefulApparition";
    newscript->GetAI = &GetAI_npc_SpitefulApparitionAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SpectralWarden";
    newscript->GetAI = &GetAI_npc_SpectralWardenAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulguardWatchman";
    newscript->GetAI = &GetAI_npc_SoulguardWatchmanAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulguardReaper";
    newscript->GetAI = &GetAI_npc_SoulguardReaperAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulguardBonecaster";
    newscript->GetAI = &GetAI_npc_SoulguardBonecasterAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulguardAnimator";
    newscript->GetAI = &GetAI_npc_SoulguardAnimatorAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulguardAdept";
    newscript->GetAI = &GetAI_npc_SoulguardAdeptAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_SoulHorror";
    newscript->GetAI = &GetAI_npc_SoulHorrorAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->GetAI = &GetAI_npc_sylvanas_jaina_fos_start;
    newscript->Name = "npc_slyvanas_jaina_fos_start";
    newscript->pGossipHello = &GossipHello_npc_slyvanas_jaina_fos;
    newscript->pGossipSelect = &GossipSelect_npc_slyvanas_jaina_fos;
    newscript->RegisterSelf();
}