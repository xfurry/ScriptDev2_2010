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
SDName: Boss_Krikthir
SD%Complete: 20%
SDComment:
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SAY_AGGRO                       = -1601000,
    SAY_KILL_1                      = -1601001,
    SAY_KILL_2                      = -1601002,
    SAY_KILL_3                      = -1601003,
    SAY_SEND_GROUP_1                = -1601004,
    SAY_SEND_GROUP_2                = -1601005,
    SAY_SEND_GROUP_3                = -1601006,
    SAY_PREFIGHT_1                  = -1601007,
    SAY_PREFIGHT_2                  = -1601008,
    SAY_PREFIGHT_3                  = -1601009,
    SAY_SWARM_1                     = -1601010,
    SAY_SWARM_2                     = -1601011,
    SAY_DEATH                       = -1601012,
    EMOTE_BOSS_GENERIC_FRENZY       = -1000005
};

enum Spells
{
    SPELL_MIND_FLAY                        =   52586,
    H_SPELL_MIND_FLAY                      =   59367,
    SPELL_CURSE_OF_FATIGUE                 =   52592,
    H_SPELL_CURSE_OF_FATIGUE               =   59368,
    SPELL_FRENZY                           =   28747, //maybe 53361

    SPELL_SUMMON_SKITTERING_SWARMER        =   52438, //AOE Effect 140, maybe 52439
    SPELL_SUMMON_SKITTERING_SWARMER_1      =   52439, //Summon 3x 28735

    H_SPELL_ACID_SPLASH                    =   59363,
    SPELL_ACID_SPLASH                      =   52446,

    SPELL_CHARGE                           =   16979,//maybe is another spell
    SPELL_BACKSTAB                         =   52540,

    SPELL_SHADOW_BOLT                      =   52534,
    H_SPELL_SHADOW_BOLT                    =   59357,
    SPELL_SHADOW_NOVA                      =   52535,
    H_SPELL_SHADOW_NOVA                    =   59358,

    SPELL_STRIKE                           =   52532,
    SPELL_CLEAVE                           =   49806,

    SPELL_ENRAGE                           =   52470,

    SPELL_INFECTED_BITE                    =   52469,
    H_SPELL_INFECTED_BITE                  =   59364,
    SPELL_WEB_WRAP                         =   52086,//the spell is not working propperly

    SPELL_BLINDING_WEBS                    =   52524,
    H_SPELL_BLINDING_WEBS                  =   59365,

    SPELL_POSION_SPRAY                     =   52493,
    H_SPELL_POSION_SPRAY                   =   59366,
};

enum Mobs
{
    MOB_SKITTERING_SWARMER                 =   28735,
    MOB_SKITTERING_SWARMER_CONTROLLER      =   32593,
    MOB_SKITTERING_INFECTIOR               =   28736,
};

enum Misc
{
    ACHIEVEMENT_WATCH_HIM_DIE              =   1296
};

/*######
## boss_krikthir
######*/

struct MANGOS_DLL_DECL boss_krikthirAI : public ScriptedAI
{
    boss_krikthirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 MindFlayTimer;
    uint32 CurseFatigueTimer;
    uint32 SummonTimer;
    uint32 m_uiSpeakTimer;

    void Reset()
    {
        MindFlayTimer       = 15000;
        CurseFatigueTimer   = 12000;
        m_uiSpeakTimer      = 15000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRIKTHIR, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        Summon();
        SummonTimer = 15000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRIKTHIR, IN_PROGRESS);

        Creature* pGashra = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_GASHRA)));
        Creature* pSilthik = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_SILTHIK)));
        Creature* pNarjil = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_NARJIL)));;

        if (pGashra && pSilthik && pNarjil)
        {
            if (m_creature->getVictim())
            {
                pGashra->AI()->AttackStart(m_creature->getVictim());
                pSilthik->AI()->AttackStart(m_creature->getVictim());
                pNarjil->AI()->AttackStart(m_creature->getVictim());
            }
        }

        m_creature->CallForHelp(50.0f);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRIKTHIR, DONE);

        Creature* pGashra = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_GASHRA)));
        Creature* pSilthik = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_SILTHIK)));
        Creature* pNarjil = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_WATCHER_NARJIL)));;

        if (pGashra && pSilthik && pNarjil)
            if(pGashra->isAlive() && pSilthik->isAlive() && pNarjil->isAlive() && !m_bIsRegularMode)
            {
                if(m_pInstance)
                    m_pInstance->DoCompleteAchievement(ACHIEVEMENT_WATCH_HIM_DIE);
            }
    }

    void Summon()
    {
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,566.164f,682.087f,769.079f,2.21657f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,566.164f,682.087f,769.079f,2.21657f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,529.042f,706.941f,777.298f,1.0821f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,529.042f,706.941f,777.298f,1.0821f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,489.975f,671.239f,772.131f,0.261799f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,489.975f,671.239f,772.131f,0.261799f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,488.556f,692.95f,771.764f,4.88692f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,488.556f,692.95f,771.764f,4.88692f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,553.34f,640.387f,777.419f,1.20428f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,553.34f,640.387f,777.419f,1.20428f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,517.486f,706.398f,777.335f,5.35816f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,517.486f,706.398f,777.335f,5.35816f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_INFECTIOR,504.01f,637.693f,777.479f,0.506145f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,504.01f,637.693f,777.479f,0.506145f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,552.625f,706.408f,777.177f,3.4383f,TEMPSUMMON_TIMED_DESPAWN,25000);
            m_creature->SummonCreature(MOB_SKITTERING_SWARMER,552.625f,706.408f,777.177f,3.4383f,TEMPSUMMON_TIMED_DESPAWN,25000);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->GetMotionMaster()->MovePoint(m_creature->GetEntry(),m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Sounds Krikthir
        if(!m_creature->getVictim())
        {
            if(m_uiSpeakTimer < uiDiff)
            {
		        switch (rand()%3)
		        {
			        case 0: DoPlaySoundToSet(m_creature, SAY_PREFIGHT_1); break;
			        case 1: DoPlaySoundToSet(m_creature, SAY_PREFIGHT_2); break;
			        case 2: DoPlaySoundToSet(m_creature, SAY_PREFIGHT_3); break;
		        }
                m_uiSpeakTimer = 20000;
            }else m_uiSpeakTimer -= uiDiff;
        }

        if (SummonTimer <= uiDiff)
        {
            switch(urand(0, 1))
            {
                case 0: DoScriptText(SAY_SWARM_1, m_creature); break;
                case 1: DoScriptText(SAY_SWARM_2, m_creature); break;
            }
            Summon();
            SummonTimer = 15000;
        } else SummonTimer -= uiDiff;


        if (MindFlayTimer <= uiDiff)
        {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_MIND_FLAY : H_SPELL_MIND_FLAY) == CAST_OK)
                MindFlayTimer = 20000;
            } else MindFlayTimer -= uiDiff;

        if (CurseFatigueTimer <= uiDiff)
        {
            //WowWiki say "Curse of Fatigue-Kirk'thir will cast Curse of Fatigue on 2-3 targets periodically."
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_CURSE_OF_FATIGUE : H_SPELL_CURSE_OF_FATIGUE) == CAST_OK)
                    CurseFatigueTimer = 10000;
            }

               if (Unit* pTarget_1 = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (DoCastSpellIfCan(pTarget_1, m_bIsRegularMode ? SPELL_CURSE_OF_FATIGUE : H_SPELL_CURSE_OF_FATIGUE) == CAST_OK)
                    CurseFatigueTimer = 10000;
            }

            CurseFatigueTimer = 10000;
        } else CurseFatigueTimer -= uiDiff;

        if (!m_creature->HasAura(SPELL_FRENZY) && m_creature->GetHealth()*100 / m_creature->GetMaxHealth() <= 10)
        {
            DoScriptText(EMOTE_BOSS_GENERIC_FRENZY, m_creature);
            DoCast(m_creature, SPELL_FRENZY, true);
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_watcher_gashraAI : public ScriptedAI
{
    npc_watcher_gashraAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }
    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 WebWrapTimer;
    uint32 InfectedBiteTimer;

    void Reset()
    {
        WebWrapTimer      = 11000;
        InfectedBiteTimer = 4000;
    }

    void EnterCombat(Unit* who)
    {
        DoCast(m_creature, SPELL_ENRAGE, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (WebWrapTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_WEB_WRAP, true);
                WebWrapTimer = 17000;
            }
        } else WebWrapTimer -= diff;

        if (InfectedBiteTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_INFECTED_BITE : H_SPELL_INFECTED_BITE);
            InfectedBiteTimer = 15000;
        } else InfectedBiteTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_watcher_narjilAI : public ScriptedAI
{
    npc_watcher_narjilAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }
    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 WebWrapTimer;
    uint32 InfectedBiteTimer;
    uint32 BlindingWebsTimer;

    void Reset()
    {
        WebWrapTimer      = 11000;
        InfectedBiteTimer = 4000;
        BlindingWebsTimer = 17000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (WebWrapTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_WEB_WRAP, true);
                WebWrapTimer = 15000;
            }
        } else WebWrapTimer -= diff;

        if (InfectedBiteTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_INFECTED_BITE : H_SPELL_INFECTED_BITE);
            InfectedBiteTimer = 11000;
        } else InfectedBiteTimer -= diff;

        if (BlindingWebsTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_BLINDING_WEBS : H_SPELL_BLINDING_WEBS);
            BlindingWebsTimer = 17000;
        } else BlindingWebsTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_watcher_silthikAI : public ScriptedAI
{
    npc_watcher_silthikAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
    }
    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;

    uint32 WebWrapTimer;
    uint32 InfectedBiteTimer;
    uint32 PosionSprayTimer;

    void Reset()
    {
        WebWrapTimer      = 11000;
        InfectedBiteTimer = 4000;
        PosionSprayTimer  = 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (WebWrapTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, SPELL_WEB_WRAP, true);
                WebWrapTimer = 15000;
            }
        } else WebWrapTimer -= diff;

        if (InfectedBiteTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_INFECTED_BITE : H_SPELL_INFECTED_BITE);
                    InfectedBiteTimer = 15000;
        } else InfectedBiteTimer -= diff;

        if (PosionSprayTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_POSION_SPRAY : H_SPELL_POSION_SPRAY);
                    PosionSprayTimer = 17000;
        } else PosionSprayTimer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_krikthir(Creature* pCreature)
{
    return new boss_krikthirAI(pCreature);
}

CreatureAI* GetAI_npc_watcher_gashra (Creature* pCreature)
{
    return new npc_watcher_gashraAI (pCreature);
}

CreatureAI* GetAI_npc_watcher_narjil (Creature* pCreature)
{
    return new npc_watcher_narjilAI (pCreature);
}

CreatureAI* GetAI_npc_watcher_silthik (Creature* pCreature)
{
    return new npc_watcher_silthikAI (pCreature);
}

void AddSC_boss_krikthir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_krikthir";
    newscript->GetAI = &GetAI_boss_krikthir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_gashra";
    newscript->GetAI = &GetAI_npc_watcher_gashra;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_silthik";
    newscript->GetAI = &GetAI_npc_watcher_silthik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_watcher_narjil";
    newscript->GetAI = &GetAI_npc_watcher_narjil;
    newscript->RegisterSelf();
}
