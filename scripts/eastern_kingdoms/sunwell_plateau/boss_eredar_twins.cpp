/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_eredar_twins
SD%Complete:
SDComment: 
SDAuthor:
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"

enum
{
    // texts
    // Lady Sacrolash
    SAY_INTRO                               = -1580044,
    SAY_SACROLASH_SHADOW_NOVA               = -1580045, //only if Alythess is not dead
    SAY_SACROLASH_SISTER_DEAD				= -1580046,
    SAY_SACROLASH_KILL_1					= -1580047,
    SAY_SACROLASH_KILL_2					= -1580048,
    SAY_SACROLASH_DEAD                      = -1580049,
    SAY_SACROLASH_ENRAGE                    = -1580055,

    //Grand Warlock Alythess
    SAY_ALYTHESS_CANFLAGRATION              = -1580050, //only if Sacrolash is not dead
    SAY_ALYTHESS_SISTER_DEAD				= -1580051,
    SAY_ALYTHESS_KILL_1						= -1580052,
    SAY_ALYTHESS_KILL_2						= -1580053,
    SAY_ALYTHESS_DEAD						= -1580054,
    SAY_ALYTHESS_BERSERK                    = -1580055,

    //Lady Sacrolash
    SPELL_DARK_TOUCHED      =   45347,
    SPELL_SHADOW_BLADES     =   45248, //10 secs
    SPELL_DARK_STRIKE       =   45271,
    SPELL_SHADOW_NOVA       =   45329, //30-35 secs
    SPELL_CONFOUNDING_BLOW  =   45256, //25 secs

    //Shadow Image spells
	NPC_SHADOW_IMAGE        =   25214,
    SPELL_SHADOWFURY        =   45270,
    SPELL_IMAGE_VISUAL      =   45263,

    //Grand Warlock Alythess spells
    SPELL_PYROGENICS        =   45230, //15secs
    SPELL_FLAME_TOUCHED     =   45348,
    SPELL_CONFLAGRATION     =   45342, //30-35 secs
    SPELL_BLAZE             =   45235, //on main target every 3 secs
    SPELL_FLAME_SEAR        =   46771,

	//Misc spells
    SPELL_TWINS_ENRAGE      =   46587,
    SPELL_EMPOWER           =   45366,
    SPELL_DARK_FLAME        =   45345,
};

/*######
## mob_alythess
######*/
struct MANGOS_DLL_DECL boss_alythessAI : public ScriptedAI
{
    boss_alythessAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

	ScriptedInstance* m_pInstance;  

	uint32 m_uiPyrogenicsTimer;
	uint32 m_uiFlameTouchedTimer;
	uint32 m_uiConflagrationTimer;
	uint32 m_uiBlazeTimer;
	uint32 m_uiFlameSearTimer;
	uint32 m_uiEnrageTimer;

    void Reset()
	{	
		m_uiPyrogenicsTimer		= 15000; 
		m_uiFlameTouchedTimer	= 30000; 
		m_uiConflagrationTimer	= 25000 + rand()%15000;
		m_uiBlazeTimer			= 1000; 
		m_uiFlameSearTimer		= 15000; 
		m_uiEnrageTimer			= 360000; // 6 min
	}

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_EREDAR_TWINS) != NOT_STARTED)
				m_pInstance->SetData(TYPE_EREDAR_TWINS, NOT_STARTED);

			if(Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SACROLASH)))
			{
				if(!pSister->isAlive())
					pSister->Respawn();
			}
		}
	}

	void Aggro(Unit* pWho)
	{
        if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_EREDAR_TWINS) != IN_PROGRESS)
				m_pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
		}

        if(Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SACROLASH)))
			pSister->AI()->AttackStart(pWho);

		DoCast(m_creature, SPELL_PYROGENICS);
		DoScriptText(SAY_INTRO, m_creature);
	}

	void AttackStart(Unit* pWho)
    {
        if(!pWho)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20);
        }
    }
	
	void KilledUnit(Unit* pVictim)
	{
       switch(rand()%2)
       {
        	case 0: DoScriptText(SAY_ALYTHESS_KILL_1, m_creature); break;
        	case 1: DoScriptText(SAY_ALYTHESS_KILL_2, m_creature); break;
        }
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_ALYTHESS_DEAD, m_creature);

        if (m_pInstance)
        {
            if(Creature* pSacrolash = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SACROLASH)))
            {
                if(!pSacrolash->isAlive())
                    m_pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
                else
                {
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    DoScriptText(SAY_SACROLASH_SISTER_DEAD, pSacrolash);
					pSacrolash->InterruptNonMeleeSpells(true);
					pSacrolash->CastSpell(pSacrolash, SPELL_EMPOWER, false);
                }
            }
        }
	}

	void DamageDeal(Unit* pDoneTo, uint32 &uiDamage) 
    {
        if(pDoneTo->HasAura(SPELL_DARK_TOUCHED,EFFECT_INDEX_0))
		{
            pDoneTo->RemoveAurasDueToSpell(SPELL_DARK_TOUCHED,0);
			pDoneTo->CastSpell(pDoneTo, SPELL_DARK_FLAME, false);
		}
    }

	void UpdateAI(const uint32 uiDiff)
	{    
		if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
			return;

		if(m_uiEnrageTimer < uiDiff)
		{
			DoScriptText(SAY_ALYTHESS_BERSERK, m_creature);
			DoCast(m_creature, SPELL_TWINS_ENRAGE);
			m_uiEnrageTimer = 60000;
		}
		else m_uiEnrageTimer -= uiDiff;

		if(m_uiPyrogenicsTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_PYROGENICS);
			m_uiPyrogenicsTimer = 15000;
		}
		else m_uiPyrogenicsTimer -= uiDiff;

		if(m_uiFlameTouchedTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				pTarget->CastSpell(pTarget, SPELL_FLAME_TOUCHED, false);
			m_uiFlameTouchedTimer = urand(10000, 13000);
		}
		else m_uiFlameTouchedTimer -= uiDiff;

		if(m_uiConflagrationTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoScriptText(SAY_ALYTHESS_CANFLAGRATION, m_creature);
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				DoCast(pTarget, SPELL_CONFLAGRATION);
			m_uiConflagrationTimer = 20000 + rand()%5000;
		}
		else m_uiConflagrationTimer -= uiDiff;

		if(m_uiBlazeTimer < uiDiff)
		{
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
				DoCast(pTarget, SPELL_BLAZE);
			m_uiBlazeTimer = urand(3000,4000);
		}
		else m_uiBlazeTimer -= uiDiff;

		if(m_uiFlameSearTimer < uiDiff)
		{
			m_creature->InterruptNonMeleeSpells(true);
			DoCast(m_creature, SPELL_FLAME_SEAR);	
			m_uiFlameSearTimer = 30000;
		}
		else m_uiFlameSearTimer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

/*######
## boss_sacrolash
######*/
struct MANGOS_DLL_DECL boss_sacrolashAI : public ScriptedAI
{
    boss_sacrolashAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
			
	ScriptedInstance* m_pInstance;
	
	uint32 m_uiEnrageTimer;
	uint32 m_uiDarkTouchedTimer;
	uint32 m_uiShadowNovaTimer;
	uint32 m_uiConfoundingBlowTimer;
	uint32 m_uiShadowBladesTimer;
	uint32 m_uiSummonShadowImage;
	uint64 m_uiTargetGUID[3];

	void Reset()
	{	
		m_uiEnrageTimer			= 360000; 
		m_uiDarkTouchedTimer	= 30000;
		m_uiShadowNovaTimer		= 15000;
		m_uiConfoundingBlowTimer = 3000;
		m_uiShadowBladesTimer	= 10000;
		m_uiSummonShadowImage	= 30000;
	}

	void JustReachedHome()
	{
		if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_EREDAR_TWINS) != NOT_STARTED)
				m_pInstance->SetData(TYPE_EREDAR_TWINS, NOT_STARTED);

			if(Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ALYTHESS)))
			{
				if(!pSister->isAlive())
					pSister->Respawn();
			}
		}
	}

    void DamageDeal(Unit* pDoneTo, uint32 &uiDamage) 
    {
        if(pDoneTo->HasAura(SPELL_FLAME_TOUCHED,EFFECT_INDEX_0))
		{
            pDoneTo->RemoveAurasDueToSpell(SPELL_FLAME_TOUCHED,0);
			pDoneTo->CastSpell(pDoneTo, SPELL_DARK_FLAME, false);
		}
    }

	void Aggro(Unit* pWho)
	{
        if(m_pInstance)
		{
			if(m_pInstance->GetData(TYPE_EREDAR_TWINS) != IN_PROGRESS)
				m_pInstance->SetData(TYPE_EREDAR_TWINS, IN_PROGRESS);
		}

		if(Creature* pSister = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ALYTHESS)))
			pSister->AI()->AttackStart(pWho);
	}

	void KilledUnit(Unit* pVictim)
	{    
	   switch(rand()%2)
       {
        	case 0: DoScriptText(SAY_SACROLASH_KILL_1, m_creature); break;
        	case 1: DoScriptText(SAY_SACROLASH_KILL_2, m_creature); break;
       }
	}

	void JustDied(Unit* pKiller)
	{
		DoScriptText(SAY_SACROLASH_DEAD, m_creature);

        if (m_pInstance)
        {
            if(Creature* pAlythess = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_ALYTHESS)))
            {
                if(!pAlythess->isAlive())
                    m_pInstance->SetData(TYPE_EREDAR_TWINS, DONE);
                else
                {
                    m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    DoScriptText(SAY_ALYTHESS_SISTER_DEAD, pAlythess);
					pAlythess->InterruptNonMeleeSpells(true);
					pAlythess->CastSpell(pAlythess, SPELL_EMPOWER, false);
                }
            }
        }
	}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if(m_uiEnrageTimer < uiDiff)
        {
            DoScriptText(SAY_SACROLASH_ENRAGE, m_creature);
            DoCast(m_creature, SPELL_TWINS_ENRAGE);
			m_uiEnrageTimer = 60000;
        }
		else m_uiEnrageTimer -= uiDiff;
		
        if(m_uiDarkTouchedTimer < uiDiff)
        {
			if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
				pTarget->CastSpell(pTarget, SPELL_DARK_TOUCHED, false);
            m_uiDarkTouchedTimer = urand(10000,13000);
        }
		else m_uiDarkTouchedTimer -= uiDiff;

        if(m_uiShadowBladesTimer < uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_SHADOW_BLADES, true);
            m_uiShadowBladesTimer = urand(10000,13000);
        }
		else m_uiShadowBladesTimer -= uiDiff;
		
        if(m_uiShadowNovaTimer < uiDiff)
        {
			m_creature->InterruptNonMeleeSpells(true);
            DoScriptText(SAY_SACROLASH_SHADOW_NOVA, m_creature);
            DoCast(m_creature, SPELL_SHADOW_NOVA);
            m_uiShadowNovaTimer = urand(30000,35000);
        }
		else m_uiShadowNovaTimer -= uiDiff;
		
        if(m_uiConfoundingBlowTimer < uiDiff)
        {
			DoCast(m_creature->getVictim(), SPELL_CONFOUNDING_BLOW);
            m_uiConfoundingBlowTimer = urand(25000,30000);
        }
		else m_uiConfoundingBlowTimer -= uiDiff;
		
		if (m_uiSummonShadowImage < uiDiff)
        {
            for(uint8 i = 0; i < 3; ++i)
            {
				float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
                float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
			    if(Creature* pImage = m_creature->SummonCreature(NPC_SHADOW_IMAGE, homeX, homeY, m_creature->GetPositionZ(), 0.0, TEMPSUMMON_TIMED_DESPAWN, 10000))
				{
					if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
						pImage->AI()->AttackStart(pTarget);
				}
            }
            m_uiSummonShadowImage = urand(30000,40000);
        } 
		else m_uiSummonShadowImage -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

/*######
## mob_shadow_image
######*/
struct MANGOS_DLL_DECL npc_shadow_imageAI : public ScriptedAI
{
	npc_shadow_imageAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		pCreature->setFaction(14);
		Reset();
	}

	ScriptedInstance* pInstance; 
	
	uint32 m_uiDarkStrikeTimer;
	uint32 m_uiShadowfuryTimer;
	
    void Reset()
	{	
        DoCast(m_creature, SPELL_IMAGE_VISUAL);
		m_uiDarkStrikeTimer = 1000;
		m_uiShadowfuryTimer = 9000;
	}
	
	void UpdateAI(const uint32 diff)
    {       	
	    if(m_uiDarkStrikeTimer < diff)
        {
			DoCast(m_creature->getVictim(), SPELL_DARK_STRIKE);
            m_uiDarkStrikeTimer = 2000;
        }
		else m_uiDarkStrikeTimer -= diff;
		
		if(m_uiShadowfuryTimer < diff)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pTarget, SPELL_SHADOWFURY, false);
			m_uiShadowfuryTimer = 8000;	
        }
		else m_uiShadowfuryTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_shadow_image(Creature* pCreature)
{
    return new npc_shadow_imageAI (pCreature);
}

CreatureAI* GetAI_boss_alythess(Creature* pCreature)
{
    return new boss_alythessAI (pCreature);
}

CreatureAI* GetAI_boss_sacrolash(Creature* pCreature)
{
    return new boss_sacrolashAI (pCreature);
}

void AddSC_boss_eredar_twins()
{
    Script* newscript;
	
	newscript = new Script;
    newscript->Name = "npc_shadow_image";
    newscript->GetAI = &GetAI_npc_shadow_image;
    newscript->RegisterSelf();
	
    newscript = new Script;
    newscript->Name = "boss_alythess";
    newscript->GetAI = &GetAI_boss_alythess;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_sacrolash";
    newscript->GetAI = &GetAI_boss_sacrolash;
    newscript->RegisterSelf();
}
