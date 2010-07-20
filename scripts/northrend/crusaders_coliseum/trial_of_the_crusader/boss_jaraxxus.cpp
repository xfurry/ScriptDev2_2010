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
SDName: trial_of_the_crusader
SD%Complete: 0
SDComment:
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    // yells
    SAY_AGGRO       = -1605014,
    SAY_INCINERATE  = -1605015,
    SAY_SISTER      = -1605016,
    SAY_INFERNO     = -1605017,
    SAY_SLAY1       = -1605018,
    SAY_SLAY2       = -1605019,
    SAY_DEATH       = -1605020,
    SAY_BERSERK     = -1605025,
    EMOTE_PORTAL    = -1605138,
    EMOTE_VOLCANO   = -1605140,

    SAY_TIRION_JARU_OUTRO1      = -1605021,
    SAY_GARROSH_JARU_OUTRO2     = -1605022,
    SAY_VARIAN_JARU_OUTRO3      = -1605023,
    SAY_TIRION_JARU_OUTRO4      = -1605024,

    SPELL_FEL_FIREBALL_10       = 66532,
    SPELL_FEL_FIREBALL_10HC     = 66963,
    SPELL_FEL_FIREBALL_25       = 66964,
    SPELL_FEL_FIREBALL_25HC     = 66965,
    SPELL_FEL_LIGHTNING_10      = 66528,
    SPELL_FEL_LIGHTNING_25      = 67030,
    SPELL_FEL_LIGHTNING_10HC    = 67029,
    SPELL_FEL_LIGHTNING_25HC    = 67031,
    SPELL_INCINERATE_FLESH_10   = 66237,
    SPELL_INCINERATE_FLESH_10HC = 67049,
    SPELL_INCINERATE_FLESH_25   = 67050,
    SPELL_INCINERATE_FLESH_25HC = 67051,
    SPELL_BURNING_INFERNO_10    = 66242,
    SPELL_BURNING_INFERNO_25    = 67060,  
    SPELL_LEGION_FLAME          = 68124,
    SPELL_LEGION_FLAME_JAR_10   = 66199,
    SPELL_LEGION_FLAME_JAR_25   = 68127,
    SPELL_LEGION_FLAME_JAR_10HC = 68126,
    SPELL_LEGION_FLAME_JAR_25HC = 68128,
    SPELL_LEGION_FLAME_SUMMON   = 66200,
    NPC_LEGIONS_FLAME           = 34784,
    SPELL_LEGION_FLAME_TRIG     = 66201,

    SPELL_LEGION_FLAME_10       = 66877,
    SPELL_LEGION_FLAME_25       = 67071,
    SPELL_LEGION_FLAME_10HC     = 67070,
    SPELL_LEGION_FLAME_25HC     = 67072,

    SPELL_INFERNAL_ERUPTION     = 66258,
    NPC_INFERNAL_VOLCANO        = 34813,
    NPC_FELFLAME_INFERNAL       = 34815,
    SPELL_INFERNAL_ERUPTION_VOL = 66255,
    SPELL_FEL_INFERNO           = 67047,
    SPELL_FEL_STEAK_10          = 66519,
    SPELL_FEL_STEAK_25          = 67043,
    SPELL_FEL_STEAK_10HC        = 67042,
    SPELL_FEL_STEAK_25HC        = 67044,

    SPELL_NETHER_PORTAL_TRIG    = 66263,  // cast by nether portal
    NPC_NETHER_PORTAL           = 34825,
    NPC_MISTRESS_OF_PAIN        = 34826,
    SPELL_SHIVAN_SLASH_10       = 66378,
    SPELL_SHIVAN_SLASH_25       = 67097,
    SPELL_MISTRESS_KISS         = 67078,    // 67077; 66425
    SPELL_SPINNING_PAIN_SPIKE   = 66283,

    SPELL_ERUPTION              = 66252,    // 3 infernals
    SPELL_ERUPTION_HC           = 67069,    // unlimited infernals

    SPELL_NETHER_POWER          = 67108,    // 5 10man, 10 25man

    SPELL_BERSERK               = 26662,

    ACHIEV_THREE_SIXTY_PAIN_SPIKE   = 3996,
    ACHIEV_THREE_SIXTY_PAIN_SPIKE_H = 3997,
};

enum Equipment
{
    EQUIP_MAIN           = 47266,
    EQUIP_OFFHAND        = 46996,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

// npc_jaina -> needed for outro
struct MANGOS_DLL_DECL npc_jainaAI : public ScriptedAI
{
    npc_jainaAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 OutroTimer;
    uint32 OutroStep;

    bool isJaruOutro;

    void Reset()
    {
        OutroTimer          = 1000;
        OutroStep           = 0;
        isJaruOutro         = false;
    }

    void SetJaruOutro()
    {
        isJaruOutro = true;
        OutroTimer = 5000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(isJaruOutro)
        {
            switch(OutroStep)
            {
            case 1:
                if(Creature* Garrosh = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_GARROSH)))
                    DoScriptText(SAY_GARROSH_JARU_OUTRO2, Garrosh);
                ++OutroStep;
                OutroTimer = 10000;
                break;
            case 3:
                if(Creature* Varian = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_VARIAN)))
                    DoScriptText(SAY_VARIAN_JARU_OUTRO3, Varian);
                ++OutroStep;
                OutroTimer = 8000;
                break;
            case 5:
                if(Creature* Tirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
                    DoScriptText(SAY_TIRION_JARU_OUTRO4, Tirion);
                ++OutroStep;
                OutroTimer = 15000;
                break;
            case 7:
                if(Creature* Tirion = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_TIRION)))
                    DoScriptText(SAY_TIRION_JARU_OUTRO1, Tirion);
                ++OutroStep;
                OutroTimer = 15000;
                break;
            case 9:
                if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
                {
                    pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    pBarret->SetVisibility(VISIBILITY_ON);
                }
                isJaruOutro = false;
                ++OutroStep;
                OutroTimer = 10000;
                break;
            }

            if (OutroTimer <= uiDiff)
            {
                ++OutroStep;
                OutroTimer = 330000;
            } OutroTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_jaina(Creature* pCreature)
{
    return new npc_jainaAI (pCreature);
}

/*######
## boss_jaraxxus
######*/

struct MANGOS_DLL_DECL boss_jaraxxusAI : public ScriptedAI
{
    boss_jaraxxusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    uint32 m_uiFelFireballTimer;
    uint32 m_uiFelLightningTimer;
    uint32 m_uiIncinerateFleshTimer;
    uint32 m_uiBurningInfernoTimer;
    uint32 m_uiLegionFlameTimer;
    uint32 m_uiSummonTimer;
    bool m_bVolcanoSummon;
    uint8 m_uiMaxNetherPower;
    uint32 m_uiNetherPowerTimer;

    uint32 m_uiBerserkTimer;
    uint32 m_uiWipeCheckTimer;

    float VolcanoX[3];
    float VolcanoY[3];

    std::list<Creature*> lFlames;
    std::list<Creature*> lMistres;

    uint8 m_uiAchievCounter;
	bool m_bHasAura;

    void Reset() 
    {
        m_uiFelFireballTimer        = urand(20000, 25000);
        m_uiFelLightningTimer       = urand(5000, 8000);
        m_uiIncinerateFleshTimer    = 15000;
        m_uiLegionFlameTimer        = 20000;
        m_uiSummonTimer             = 20000;
        m_bVolcanoSummon            = true;
        m_uiNetherPowerTimer        = 40000;
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            m_uiMaxNetherPower      = 10;
        else if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiMaxNetherPower      = 5;

        m_uiBerserkTimer    = 600000;  // 10 min
        m_uiWipeCheckTimer  = 30000;
        lFlames.clear();
        lMistres.clear();
        m_uiAchievCounter   = 0;
		m_bHasAura			= false;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_JARAXXUS, NOT_STARTED);
            if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(!GetClosestCreatureWithEntry(m_creature, NPC_WILFRED, 150.0f))
        {
            switch(urand(0, 1))
            {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
            }
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
			DoCast(m_creature, SPELL_NETHER_POWER);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_STAGE,0);
            m_pInstance->SetData(TYPE_JARAXXUS, DONE);
        }

        if(Creature *pJaina = GetClosestCreatureWithEntry(m_creature, NPC_JAINA, 180.0f))
            ((npc_jainaAI*)pJaina->AI())->SetJaruOutro();

        GetCreatureListWithEntryInGrid(lFlames, m_creature, NPC_LEGIONS_FLAME, DEFAULT_VISIBILITY_INSTANCE);

        if (!lFlames.empty())
        {
            for(std::list<Creature*>::iterator iter = lFlames.begin(); iter != lFlames.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }

        GetCreatureListWithEntryInGrid(lMistres, m_creature, NPC_MISTRESS_OF_PAIN, DEFAULT_VISIBILITY_INSTANCE);
        if (!lMistres.empty() && lMistres.size() >= 2)
        {
            for(std::list<Creature*>::iterator iter = lMistres.begin(); iter != lMistres.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    m_uiAchievCounter += 1;
            }
        }

        if(m_uiAchievCounter >= 2)
        {
            if(m_pInstance)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_THREE_SIXTY_PAIN_SPIKE);
                else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_THREE_SIXTY_PAIN_SPIKE_H);
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();

        if(pWho != GetClosestCreatureWithEntry(m_creature, NPC_WILFRED, 150.0f))
            DoScriptText(SAY_AGGRO, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget, 100.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(SpellAuraHolder* netherAura = m_creature->GetSpellAuraHolder(SPELL_NETHER_POWER))
		{
			if(netherAura->GetStackAmount() < m_uiMaxNetherPower && !m_bHasAura)
			{
				m_bHasAura = true;
				netherAura->SetStackAmount(m_uiMaxNetherPower);
			}
		}

        // spells
        if (m_uiIncinerateFleshTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_INCINERATE_FLESH_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_INCINERATE_FLESH_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_INCINERATE_FLESH_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_INCINERATE_FLESH_25HC);
            }
            m_uiIncinerateFleshTimer = 25000;
        }
        else
            m_uiIncinerateFleshTimer -= uiDiff;

        if (m_uiFelFireballTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEL_FIREBALL_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEL_FIREBALL_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEL_FIREBALL_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEL_FIREBALL_25HC);
            }
            m_uiFelFireballTimer = urand(20000, 30000);
        }
        else
            m_uiFelFireballTimer -= uiDiff;

        if (m_uiFelLightningTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEL_LIGHTNING_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(pTarget, SPELL_FEL_LIGHTNING_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEL_LIGHTNING_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(pTarget, SPELL_FEL_LIGHTNING_25HC);
            }
            m_uiFelLightningTimer = urand(6000, 12000);
        }
        else
            m_uiFelLightningTimer -= uiDiff;

        if (m_uiSummonTimer < uiDiff)
        {
            // choose pos
            float angle = (float) rand()*360/RAND_MAX + 1;
            float posX = SpawnLoc[1].x + urand(10, 30)*cos(angle*(M_PI/180));
            float posY = SpawnLoc[1].y + urand(10, 30)*sin(angle*(M_PI/180));
            // summon portal
            if(m_bVolcanoSummon)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_creature->SummonCreature(NPC_NETHER_PORTAL, posX, posY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 9000);
                else
                    m_creature->SummonCreature(NPC_NETHER_PORTAL, posX, posY, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                DoScriptText(EMOTE_PORTAL, m_creature);
                m_bVolcanoSummon = false;
            }
            // summon volcano
            else
            {
                DoScriptText(EMOTE_VOLCANO, m_creature);
                //DoCast(m_creature, SPELL_INFERNAL_ERUPTION);
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_creature->SummonCreature(NPC_INFERNAL_VOLCANO, posX, posY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 20000);
                else
                    m_creature->SummonCreature(NPC_INFERNAL_VOLCANO, posX, posY, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                m_bVolcanoSummon = true;
            }
            m_uiSummonTimer = 60000;
        }
        else
            m_uiSummonTimer -= uiDiff;

        if (m_uiLegionFlameTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_LEGION_FLAME);
            m_uiLegionFlameTimer = 30000;
        }
        else
            m_uiLegionFlameTimer -= uiDiff;

		if (m_uiNetherPowerTimer < uiDiff)
		{
			DoCast(m_creature, SPELL_NETHER_POWER);
			m_bHasAura = true;
			m_uiNetherPowerTimer = 40000;
		}
        else
            m_uiNetherPowerTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jaraxxus(Creature* pCreature)
{
    return new boss_jaraxxusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_legion_flameAI : public ScriptedAI
{
    mob_legion_flameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetDisplayId(11686);     // make invisible
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiSpellTimer;
    uint32 m_uiDieTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_LEGION_FLAME_TRIG);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_legion_flame(Creature* pCreature)
{
    return new mob_legion_flameAI (pCreature);
}

struct MANGOS_DLL_DECL mob_infernal_volcanoAI : public ScriptedAI
{
    mob_infernal_volcanoAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = (uint8)pCreature->GetMap()->GetDifficulty();
        if (Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            pCreature->SetMaxHealth(189000);
        SetCombatMovement(false);
        pCreature->setFaction(14);
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint8 Difficulty;
    uint32 dieTimer;
    bool m_bDespawn;

    void Reset()
    {
        dieTimer = urand(16000, 22000);
        // don't despawn on heroic
        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
        {
            DoCast(m_creature, SPELL_ERUPTION);
            m_bDespawn = true;
        }
        else
        {
            DoCast(m_creature, SPELL_ERUPTION_HC);
            m_bDespawn = false;
        }
        m_creature->SetRespawnDelay(DAY);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget, 100.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (dieTimer < uiDiff && m_bDespawn)
        {
            m_creature->ForcedDespawn();
            dieTimer = 5000;
        }else dieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_infernal_volcano(Creature* pCreature)
{
    return new mob_infernal_volcanoAI (pCreature);
}

struct MANGOS_DLL_DECL mob_felflame_infernalAI : public ScriptedAI
{
    mob_felflame_infernalAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            pCreature->ApplySpellImmune(0, IMMUNITY_MECHANIC, IMMUNE_TO_SILENCE_AND_STUN_AND_FEAR_MASK, true);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 spellTimer;
    uint32 Difficulty;

    void Reset()
    {
        spellTimer = 10000;
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(pTarget, SPELL_FEL_STEAK_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(pTarget, SPELL_FEL_STEAK_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(pTarget, SPELL_FEL_STEAK_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(pTarget, SPELL_FEL_STEAK_25HC);
                }
                break;
            case 1:
                DoCast(m_creature, SPELL_FEL_INFERNO);
                break;
            }
            spellTimer = urand(7000, 10000);
        }else spellTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_felflame_infernal(Creature* pCreature)
{
    return new mob_felflame_infernalAI (pCreature);
}

struct MANGOS_DLL_DECL mob_nether_portalAI : public ScriptedAI
{
    mob_nether_portalAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        if (Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            pCreature->SetMaxHealth(189000);
        pCreature->SetDisplayId(11686);     // make invisible
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.0f);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint32 Difficulty;
    uint32 spellTimer;

    void Reset()
    {
        spellTimer = 100;
        m_creature->SetRespawnDelay(DAY);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget, 100.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (spellTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_NETHER_PORTAL_TRIG);
            spellTimer = 8000;
        }else spellTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_nether_portal(Creature* pCreature)
{
    return new mob_nether_portalAI (pCreature);
}

struct MANGOS_DLL_DECL mob_mistress_of_painAI : public ScriptedAI
{
    mob_mistress_of_painAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 spellTimer;
    uint32 m_uiMistressKissTimer;

    void Reset()
    {
        spellTimer = urand(5000, 10000);
        m_uiMistressKissTimer = urand(7000, 14000);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_JARAXXUS) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (spellTimer < uiDiff)
        {
            switch(urand(0, 1))
            {
            case 0:
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(m_creature, SPELL_SHIVAN_SLASH_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(m_creature, SPELL_SHIVAN_SLASH_25);
                break;
            case 1:
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_SPINNING_PAIN_SPIKE);
                break;
            }
            spellTimer = urand(5000, 8000);
        }else spellTimer -= uiDiff;

        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
        {
            if(m_uiMistressKissTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_MISTRESS_KISS);
                m_uiMistressKissTimer = urand(13000, 19000);
            }
            else m_uiMistressKissTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mistress_of_pain(Creature* pCreature)
{
    return new mob_mistress_of_painAI (pCreature);
}

void AddSC_boss_jaraxxus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_jaraxxus";
    newscript->GetAI = &GetAI_boss_jaraxxus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_legion_flame";
    newscript->GetAI = &GetAI_mob_legion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_infernal_volcano";
    newscript->GetAI = &GetAI_mob_infernal_volcano;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_felflame_infernal";
    newscript->GetAI = &GetAI_mob_felflame_infernal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nether_portal";
    newscript->GetAI = &GetAI_mob_nether_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mistress_of_pain";
    newscript->GetAI = &GetAI_mob_mistress_of_pain;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_jaina";
    newscript->GetAI = &GetAI_npc_jaina;
    newscript->RegisterSelf();
}
