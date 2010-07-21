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
SDName: boss_deathbringer_saurfang
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    //aly intro
    SAY_ALY_INTRO1      = -1609426,
    SAY_ALY_INTRO2      = -1609427,
    SAY_ALY_INTRO3      = -1609428,
    SAY_ALY_INTRO4      = -1609429,
    SAY_ALY_INTRO5      = -1609430,

    //aly outro
    SAY_ALY_OUTRO1      = -1609440,
    SAY_ALY_OUTRO2      = -1609441,
    SAY_ALY_OUTRO3      = -1609442,
    SAY_ALY_OUTRO4      = -1609443,
    SAY_ALY_OUTRO5      = -1609444,
    SAY_ALY_OUTRO6      = -1609445,
    SAY_ALY_OUTRO7      = -1609446,
    SAY_ALY_OUTRO8      = -1609447,
    SAY_ALY_OUTRO9      = -1609448,
    SAY_ALY_OUTRO10     = -1609449,
    SAY_ALY_OUTRO11     = -1609450,
    SAY_ALY_OUTRO12     = -1609451,
    SAY_ALY_OUTRO13     = -1609452,
    SAY_ALY_OUTRO14     = -1609453,
    SAY_ALY_OUTRO15     = -1609454,
    SAY_ALY_OUTRO16     = -1609455,

    //horde intro
    SAY_HORDE_INTRO1    = -1609431,
    SAY_HORDE_INTRO2    = -1609432,
    SAY_HORDE_INTRO3    = -1609433,
    SAY_HORDE_INTRO4    = -1609434,
    SAY_HORDE_INTRO5    = -1609435,
    SAY_HORDE_INTRO6    = -1609436,
    SAY_HORDE_INTRO7    = -1609437,
    SAY_HORDE_INTRO8    = -1609438,
    SAY_HORDE_INTRO9    = -1609439,

    //horde outro
    SAY_HORDE_OUTRO1    = -1609456,
    SAY_HORDE_OUTRO2    = -1609457,
    SAY_HORDE_OUTRO3    = -1609458,
    SAY_HORDE_OUTRO4    = -1609459,

    //yells
    SAY_AGGRO           = -1609419,
    SAY_FALLENCHAMPION  = -1609420,
    SAY_BLOODBEASTS     = -1609421,
    SAY_KILL1           = -1609422,
    SAY_KILL2           = -1609423,
    SAY_BERSERK         = -1609424,
    SAY_DEATH           = -1609425,

    SPELL_BLOOD_LINK            = 72178,
    SPELL_BLOOD_POWER           = 72371,
    SPELL_RUNE_OF_BLOOD         = 72408,
    SPELL_BLOOD_NOVA            = 72378,
    SPELL_BLOOD_NOVA_10         = 72380,
    SPELL_BLOOD_NOVA_25         = 72438,
    SPELL_BLOOD_NOVA_10HC       = 72439,
    SPELL_BLOOD_NOVA_25HC       = 72440,
    SPELL_BOILING_BLOOD_10      = 72385,
    SPELL_BOILING_BLOOD_25      = 72441,
    SPELL_BOILING_BLOOD_10HC    = 72442,
    SPELL_BOILING_BLOOD_25HC    = 72443,
    SPELL_FRENZY                = 72737,
    SPELL_MARK_FALLEN_CHAMP     = 72293,
    SPELL_RUNE_OF_BLOOD_10      = 72409,
    SPELL_RUNE_OF_BLOOD_25      = 72447,
    SPELL_RUNE_OF_BLOOD_10HC    = 72448,
    SPELL_RUNE_OF_BLOOD_25HC    = 72449,
    SPELL_SCENT_OF_BLOOD        = 72769,
    SPELL_CALL_BLOOD_BEASTS     = 72172,
    SPELL_RESISTANT_SKIN        = 72723,
    SPELL_BERSERK               = 26662,

    NPC_BLOOD_BEAST             = 38508,

    EQUIP_ID                    = 51905,
};

struct MANGOS_DLL_DECL boss_saurfangAI : public ScriptedAI
{
    boss_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->setPowerType(POWER_RAGE); 
        pCreature->SetMaxPower(POWER_RAGE,1000);
        pCreature->SetPower(POWER_RAGE,0); 
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiBloodBeast_Timer;
    uint32 m_uiRuneOfBlood_Timer;
    uint32 m_uiBoilingBlood_Timer;
    uint32 m_uiBloodNova_Timer;
    uint32 m_uiBerserkTimer;

    void Reset()
    {
        m_uiBloodBeast_Timer    = 40000;
        m_uiRuneOfBlood_Timer   = 30000;
        m_uiBoilingBlood_Timer  = 18000;
        m_uiBloodNova_Timer     = 20000;
        m_uiBerserkTimer        = 480000;  // 8 min

        m_creature->SetPower(POWER_RAGE,0); 
        
        if(m_creature->HasAura(SPELL_BLOOD_LINK))
            m_creature->RemoveAurasDueToSpell(SPELL_BLOOD_LINK);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        if (!m_creature->HasAura(SPELL_BLOOD_LINK))
            DoCast(m_creature, SPELL_BLOOD_LINK);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SAURFANG, DONE);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_FALLEN_CHAMP);
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SAURFANG, FAIL);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MARK_FALLEN_CHAMP);
        }
    }

    void DamageDeal(Unit * pDoneTo, uint32 &uiDamage)
    {
        int temp1 = m_creature->GetPower(POWER_RAGE);
        int temp2 = 0.001 * uiDamage;
        temp1 = temp1 + temp2;

        if(temp1 > 1000)
            m_creature->SetPower(POWER_RAGE,1000);   
        else
            m_creature->SetPower(POWER_RAGE,temp1); 

        DoCast(m_creature, SPELL_BLOOD_POWER);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < 30.0f && !m_creature->HasAura(SPELL_FRENZY))
            DoCast(m_creature, SPELL_FRENZY);

        if (m_creature->GetPower(m_creature->getPowerType()) == m_creature->GetMaxPower(m_creature->getPowerType()))
        {
            DoScriptText(SAY_FALLENCHAMPION, m_creature);

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(target, SPELL_MARK_FALLEN_CHAMP);
            m_creature->SetPower(m_creature->getPowerType(),0);
            m_creature->RemoveAurasDueToSpell(SPELL_BLOOD_POWER);
        }

        if (m_uiBloodBeast_Timer < uiDiff)
        {
            DoScriptText(SAY_BLOODBEASTS, m_creature);

            uint8 m_uiMaxBloodbeasts = 0;
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                m_uiMaxBloodbeasts = 2;
            else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                m_uiMaxBloodbeasts = 5;

			DoCast(m_creature, SPELL_CALL_BLOOD_BEASTS);
            for(uint8 i = 0; i < m_uiMaxBloodbeasts - 1; i++)
                m_creature->SummonCreature(NPC_BLOOD_BEAST, m_creature->GetPositionX() + urand(0, 10), m_creature->GetPositionY() + urand(0, 3), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

            m_uiBloodBeast_Timer = 40000;
        }
        else m_uiBloodBeast_Timer -= uiDiff;

        if (m_uiRuneOfBlood_Timer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD_25HC);
            m_uiRuneOfBlood_Timer = 30000;
        }
        else m_uiRuneOfBlood_Timer -= uiDiff;

        if (m_uiBoilingBlood_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(target, SPELL_BOILING_BLOOD_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(target, SPELL_BOILING_BLOOD_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(target, SPELL_BOILING_BLOOD_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(target, SPELL_BOILING_BLOOD_25HC);
            }
            m_uiBoilingBlood_Timer = 15000;
        }
        else m_uiBoilingBlood_Timer -= uiDiff;

        if (m_uiBloodNova_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    DoCast(target, SPELL_BLOOD_NOVA_10);
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    DoCast(target, SPELL_BLOOD_NOVA_25);
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                    DoCast(target, SPELL_BLOOD_NOVA_10HC);
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                    DoCast(target, SPELL_BLOOD_NOVA_25HC);
            }
            m_uiBloodNova_Timer = 20000;
        }
        else m_uiBloodNova_Timer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_saurfang(Creature* pCreature)
{
    return new boss_saurfangAI(pCreature);
}

struct MANGOS_DLL_DECL mob_blood_beastAI : public ScriptedAI
{
    mob_blood_beastAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiScentOfBloodTimer;

    void Reset()
    {
        m_uiScentOfBloodTimer = 30000;
        DoCast(m_creature, SPELL_RESISTANT_SKIN);
    }

    void DamageDeal(Unit * pDoneTo, uint32 &uiDamage)
    {
        int temp1;
        if (Creature* pSaurfang = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_SAURFANG))))
            temp1 = pSaurfang->GetPower(POWER_RAGE);

        int temp2 = 0.001 * uiDamage;
        temp1 = temp1 + temp2;

        if (Creature* pSaurfang = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_SAURFANG))))
        {
            if(temp1 > 1000)
                pSaurfang->SetPower(POWER_RAGE,1000);   
            else
                pSaurfang->SetPower(POWER_RAGE,temp1); 

            pSaurfang->CastSpell(pSaurfang, SPELL_BLOOD_POWER, true);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_SAURFANG) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
        {
            if (m_uiScentOfBloodTimer < uiDiff)
            {
                DoCast(m_creature, SPELL_SCENT_OF_BLOOD);
                m_uiScentOfBloodTimer = 30000;
            }else m_uiScentOfBloodTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_blood_beast(Creature* pCreature)
{
    return new mob_blood_beastAI (pCreature);
}

void AddSC_boss_saurfang()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_saurfang";
    NewScript->GetAI = &GetAI_boss_saurfang;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_blood_beast";
    NewScript->GetAI = &GetAI_mob_blood_beast;
    NewScript->RegisterSelf();
}