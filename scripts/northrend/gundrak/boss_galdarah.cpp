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
SDName: Boss_Galdarah
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    SAY_AGGRO                       = -1604019,
    SAY_TRANSFORM_1                 = -1604020,
    SAY_TRANSFORM_2                 = -1604021,
    SAY_SUMMON_1                    = -1604022,
    SAY_SUMMON_2                    = -1604023,
    SAY_SUMMON_3                    = -1604024,
    SAY_SLAY_1                      = -1604025,
    SAY_SLAY_2                      = -1604026,
    SAY_SLAY_3                      = -1604027,
    SAY_DEATH                       = -1604028
};

enum Spells
{
    SPELL_ENRAGE                                  = 55285,
    H_SPELL_ENRAGE                                = 59828,
    SPELL_IMPALING_CHARGE                         = 54956,
    H_SPELL_IMPALING_CHARGE                       = 59827,
    SPELL_STOMP                                   = 55292,
    H_SPELL_STOMP                                 = 59826,
    SPELL_PUNCTURE                                = 55276,
    H_SPELL_PUNCTURE                              = 59826,
    SPELL_STAMPEDE                                = 55218,
    SPELL_WHIRLING_SLASH                          = 55250,
    H_SPELL_WHIRLING_SLASH                        = 59824,
    SPELL_ECK_RESIDUE                             = 55817
};

enum Achievements
{
    ACHIEVEMENT_WHAT_THE_ECK      = 1864,
    ACHIEVEMENT_SHARE_THE_LOVE    = 2152
};

enum Displays
{
    DISPLAY_RHINO                                 = 26265,
    DISPLAY_TROLL                                 = 27061
};

enum CombatPhase
{
    TROLL       = 1,
    RHINO       = 2,
};

/*######
## boss_galdarah
######*/

struct MANGOS_DLL_DECL boss_galdarahAI : public ScriptedAI
{
    boss_galdarahAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 uiStampedeTimer;
    uint32 uiWhirlingSlashTimer;
    uint32 uiPunctureTimer;
    uint32 uiEnrageTimer;
    uint32 uiImpalingChargeTimer;
    uint32 uiStompTimer;

    uint8 m_uiCombatPhase;

    bool bStartOfTransformation;
    uint32 uiTransformationTimer;

    uint8 uiPhaseCounter;
    std::set<uint64> lImpaledPlayers;

    void Reset()
    {
        uiStampedeTimer = 10000;
        uiWhirlingSlashTimer = 20000;
        uiPunctureTimer = 10000;
        uiEnrageTimer = 15000;
        uiImpalingChargeTimer = 20000;
        uiStompTimer = 25000;

        m_uiCombatPhase = TROLL;
        bStartOfTransformation = true;
        uiTransformationTimer = 2000;
        uiPhaseCounter = 0;

        m_creature->SetDisplayId(DISPLAY_TROLL);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, NOT_STARTED);

        lImpaledPlayers.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, DONE);

        if (!m_bIsRegularMode)
        {
            AchievementEntry const *achievWhatTheEck = GetAchievementStore()->LookupEntry(ACHIEVEMENT_WHAT_THE_ECK);
            if (achievWhatTheEck)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (itr->getSource()->HasAura(SPELL_ECK_RESIDUE))
                            itr->getSource()->CompletedAchievement(achievWhatTheEck);
                }
            }

            if(lImpaledPlayers.size() == 5)
            {
                if(m_pInstance)
                    m_pInstance->DoCompleteAchievement(ACHIEVEMENT_SHARE_THE_LOVE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiCombatPhase)
        {
            case TROLL:
                if (uiPhaseCounter == 2)
                {
                    if (uiTransformationTimer <= uiDiff)
                    {
                        m_creature->SetDisplayId(DISPLAY_RHINO);
                        m_uiCombatPhase = RHINO;
                        uiPhaseCounter = 0;
                        DoScriptText(SAY_TRANSFORM_1,m_creature);
                        uiTransformationTimer = 2000;
                        bStartOfTransformation = true;
                    }
                    else
                    {
                        uiTransformationTimer -= uiDiff;

                        if (bStartOfTransformation)
                            bStartOfTransformation = false;
                    }
                }
                else
                {
                    if (uiStampedeTimer <= uiDiff)
                    {
                        DoCast(m_creature, SPELL_STAMPEDE);
                        switch(urand(0, 2))
                        {
                            case 0: DoScriptText(SAY_SUMMON_1, m_creature); break;
                            case 1: DoScriptText(SAY_SUMMON_2, m_creature); break;
                            case 2: DoScriptText(SAY_SUMMON_3, m_creature); break;
                        }
                        uiStampedeTimer = 15000;
                    } else uiStampedeTimer -= uiDiff;

                    if (uiWhirlingSlashTimer <= uiDiff)
                    {
                        DoCast(m_creature, m_bIsRegularMode ? SPELL_WHIRLING_SLASH : H_SPELL_WHIRLING_SLASH);
                        ++uiPhaseCounter;
                        uiWhirlingSlashTimer = 20000;
                    } else uiWhirlingSlashTimer -= uiDiff;
                }
            break;
            case RHINO:
                if (uiPhaseCounter == 2)
                {
                    if (uiTransformationTimer <= uiDiff)
                    {
                        m_creature->SetDisplayId(DISPLAY_TROLL);
                        m_uiCombatPhase = TROLL;
                        uiPhaseCounter = 0;
                        DoScriptText(SAY_TRANSFORM_2,m_creature);
                        uiTransformationTimer = 2000;
                        bStartOfTransformation = true;
                    }
                    else
                    {
                        uiTransformationTimer -= uiDiff;

                        if (bStartOfTransformation)
                            bStartOfTransformation = false;
                    }
                }
                else
                {
                    if (uiPunctureTimer <= uiDiff)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            DoCast(pTarget, m_bIsRegularMode ? SPELL_PUNCTURE : H_SPELL_PUNCTURE);
                        uiPunctureTimer = 8000;
                    } else uiPunctureTimer -= uiDiff;

                    if (uiEnrageTimer <= uiDiff)
                    {
                        DoCast(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : H_SPELL_ENRAGE);
                        uiEnrageTimer = 20000;
                    } else uiEnrageTimer -= uiDiff;

                    if (uiStompTimer <= uiDiff)
                    {
                        DoCast(m_creature, m_bIsRegularMode ? SPELL_STOMP : H_SPELL_STOMP);
                        uiStompTimer = 20000;
                    } else uiStompTimer -= uiDiff;

                    if (uiImpalingChargeTimer <= uiDiff)
                    {
                        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            DoCast(pTarget, m_bIsRegularMode ? SPELL_IMPALING_CHARGE : H_SPELL_IMPALING_CHARGE);
                            lImpaledPlayers.insert(pTarget->GetGUID());
                            ++uiPhaseCounter;
                            uiImpalingChargeTimer = 30000;
                        }                        
                    } else uiImpalingChargeTimer -= uiDiff;
                }
            break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_galdarah(Creature* pCreature)
{
    return new boss_galdarahAI(pCreature);
}

void AddSC_boss_galdarah()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_galdarah";
    newscript->GetAI = &GetAI_boss_galdarah;
    newscript->RegisterSelf();
}
