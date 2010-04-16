/* Copyright  2006,2007 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
 
/* ScriptData
SDName: boss_muru_entropius
SD%Complete: 99
SDComment:
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

#include "precompiled.h"
#include "sunwell_plateau.h"
 
enum spells // Boss spells
{
    ENRAGE                      = 26662,
    SPELL_NEGATIVE              = 46285,
    SPELL_NEGATIVEENERGY        = 46008,
    SPELL_NEGATIVEENERGY_CHAIN  = 46289,

    AURA_SINGULARITY            = 46228,
    AURA_SUNWELL_RADIANCE       = 45769,

    DARKNESS                    = 45996, // dokladnie sprawdzic
    ENTROPIUS_EFFECT            = 46223, // lipny ale zawsze cos

    //Dark Fiend
    DARK_FIEND_AURA             = 45934, // latajaca czaszka :D
    DARK_FIEND_DEBUFF           = 45944,
    SPELL_DISPELL               = 32375,
    SPELL_PURGE                 = 8012,

    SPELL_ARCANEFORM            = 48019, // Chaotic Rift visual

    VOID_AURA                   = 46087,
    VOID_BLAST                  = 46161,
};

enum summons
{
    ID_SWB                      = 25799, // ok
    ID_SWM                      = 25798, // ok niema bufa
    ID_DARK_FIEND               = 25744, // ok
    ID_VOID_SENTINEL            = 25772, // ok
    ID_SINGULARITY              = 25855, // visual effect i nie bic mele zmeiniac target?
    ID_SPAWN                    = 25824, // ok
};

//Boss sounds
#define SOUND_CHANGE_PHASE 12560
 
// Sumoned trash coordinates
float Trash[6][2] =
{
        {1853.300f,588.653f},
        {1781.502f,659.254f},
        {1853.300f,588.653f},
        {1853.300f,588.653f},     
        {1781.502f,659.254f},
        {1781.502f,659.254f},
};

float ShadowPortalSpawn[5][3] =
{
        {1797.644f,613.744f,0.56290f},
        {1822.956f,646.437f,4.40262f},
        {1794.275f,626.539f,6.23522f},
        {1809.726f,646.494f,5.01348f},     
        {1798.854f,638.952f,5.62435f}
};
 
/*######
## boll_muru
######*/
struct MANGOS_DLL_DECL boss_muruAI : public ScriptedAI
{
    boss_muruAI(Creature *c) : ScriptedAI(c) 
    {
	    m_pInstance = ((ScriptedInstance*)c->GetInstanceData());
	    Reset(); 
    }

    ScriptedInstance* m_pInstance; 
    bool Phase1;

    float m_fDarkPosX;
    float m_fDarkPosY;

    uint32 TargetsCount;
    uint32 TargetsCountTimer;
    uint32 SingularityTimer;
    uint32 NegativeEnergyTimer;
    uint32 SummonTrashTimer;
    uint32 SummonVoidTimer;
    uint32 DarknessTimer;
    uint32 EnrageTimer;
    uint32 DarkFiendTimer;

    bool Darkness;
    
    void Reset()
    {
        m_fDarkPosX = 0;
        m_fDarkPosY = 0;

        m_creature->SetDisplayId(23404);

        NegativeEnergyTimer = 1000;
        SummonTrashTimer = 10000;
        SummonVoidTimer = 30000;
        DarknessTimer = 45000;
        EnrageTimer = 600000;
        Phase1 = true;
        Darkness = false;

        m_creature->SetVisibility(VISIBILITY_ON);
   
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(!m_creature->HasAura(AURA_SUNWELL_RADIANCE, EFFECT_INDEX_0))
            m_creature->CastSpell(m_creature, AURA_SUNWELL_RADIANCE, true);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, NOT_STARTED);
    }
    
    void Aggro(Unit *who) 
    {
        m_creature->StopMoving();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, IN_PROGRESS);
    }  

    void KilledUnit(Unit *Victim) {}

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MURU, DONE);
    }
    
    void UpdateAI(const uint32 diff)
    {       
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
        if(EnrageTimer < diff)
        {
            m_creature->CastSpell(m_creature, ENRAGE, true);
            EnrageTimer = 60000;
        }else EnrageTimer -= diff;
        
        if(Phase1)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();

            if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 42)
            {
                m_creature->SetDisplayId(23428);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                DoPlaySoundToSet(m_creature, SOUND_CHANGE_PHASE);
                DoCast(m_creature, ENTROPIUS_EFFECT, true);

                TargetsCount = 1;
                TargetsCountTimer = 10000;
                SingularityTimer = 50000;
                Phase1 = false;
            }
            if(NegativeEnergyTimer < diff)
            {
                for(uint8 i=0; i<2; ++i)
                {
                    if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        m_creature->CastSpell(target, SPELL_NEGATIVE, false);
                }
                NegativeEnergyTimer = 1000;
            }else NegativeEnergyTimer -= diff;

            //Spawns Shadow portals and then one Void Sentinel
            if(SummonVoidTimer < diff)
            {
                //Shadow Portals not implemented yet :X
                uint8 x = rand()%5;
                Creature* VoidSentinel = m_creature->SummonCreature(ID_VOID_SENTINEL, ShadowPortalSpawn[x][0], ShadowPortalSpawn[x][1], m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                if(VoidSentinel) 
                    VoidSentinel->AI()->AttackStart(m_creature->getVictim());
                
                SummonVoidTimer = 30000;
            }else SummonVoidTimer -= diff;
            
            //Summon 6 humanoids every 1min (1mage & 2berserkers)
            if(SummonTrashTimer < diff)
            {
                    for(uint8 i = 0; i < 6; i++)
                    {
                        uint32 ID;

                        if((i == 1) | (i == 2))
                            ID = ID_SWB;
                        else
                            ID = ID_SWM;

                        Creature* sTrash = m_creature->SummonCreature(ID, Trash[i][0], Trash[i][1], m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                        
                        if(Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                            if(sTrash)
                                sTrash->AI()->AttackStart(sTarget);
                    }
                    SummonTrashTimer = 60000;
            }else SummonTrashTimer -= diff;
        }
        else // else Entropius phase
        {
            // +1 target every 10 seconds
            if(TargetsCountTimer < diff)
            {
                ++TargetsCount;
                TargetsCountTimer = 10000;
            }else TargetsCountTimer -= diff;

            //Utrudnienie -> chain negative energy nie dziala wiec sa 3 beamy full dmg
            if(NegativeEnergyTimer < diff)
            {
                for(uint8 i=0; i<TargetsCount; ++i)
                {
                    if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        m_creature->CastSpell(target, SPELL_NEGATIVE, false);
                }

                NegativeEnergyTimer = 1000;
            }else NegativeEnergyTimer -= diff;

            //Summon Singularity
            if(SingularityTimer < diff)
            {
                if(Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    Creature* Singularity = m_creature->SummonCreature(ID_SINGULARITY, sTarget->GetPositionX(), sTarget->GetPositionY(), sTarget->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                    if(Singularity)
                        Singularity->AI()->AttackStart(sTarget);

                }
                SingularityTimer = 50000;
            }else SingularityTimer -= diff; 

            DoMeleeAttackIfReady();
        }

        //Cast Darkness
        if(DarknessTimer < diff)
        {
            m_fDarkPosX = m_creature->GetPositionX();
            m_fDarkPosY = m_creature->GetPositionY();
            // Creature* Darkness = m_creature->SummonCreature(25879, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
            m_creature->CastSpell(m_creature, DARKNESS, true);

            Darkness = true;
            DarkFiendTimer = 3000;
            DarknessTimer = 45000;
        }else DarknessTimer -= diff;

        if(DarkFiendTimer < diff && Darkness)
        {
            // Phase2 1 dark fiend : Phase1 8 dark fiend
            uint8 i=1;
            if(Phase1)
                i=8;
            for(uint8 j=0; j<i; ++j)
            {
                //Using Instance Data to stop exploding after first explode
                if(m_pInstance)
                    m_pInstance->SetData(TYPE_MURU, NOT_STARTED);

                Creature* sTrash = m_creature->SummonCreature(ID_DARK_FIEND, m_fDarkPosX, m_fDarkPosY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                if(Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    if(sTrash)
                        sTrash->AI()->AttackStart(sTarget);
            }
            Darkness = false;
            DarkFiendTimer = 45000;
        }else DarkFiendTimer -= diff;
    }
};
 
/*######
## mob_dark_fiend
######*/
struct MANGOS_DLL_DECL dark_fiendAI : public ScriptedAI
{
    dark_fiendAI(Creature *c) : ScriptedAI(c) 
    {
	    pInstance = ((ScriptedInstance*)c->GetInstanceData());
	    Reset(); 
    }

    ScriptedInstance* pInstance; 

    bool Reached;

    void Reset() 
    {
        m_creature->SetDisplayId(25206);
        if(!m_creature->HasAura(DARK_FIEND_AURA,EFFECT_INDEX_0))
            m_creature->CastSpell(m_creature, DARK_FIEND_AURA, true);
        Reached = false;
    }
    
    void Aggro(Unit *who) {}
    void JustDied(Unit* Killer) {}
    void KilledUnit(Unit *Victim) {}
        
    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (Reached == false)
        {
            if (spell->Id == SPELL_DISPELL)
                Reached  = true;
            if (spell->Id == SPELL_PURGE)
                Reached  = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
        if( m_creature->isAttackReady() && !m_creature->IsNonMeleeSpellCasted(false))
        {
            //If we are within range melee the target
            /*
            if (!get data Explosion == Done)
            */
            if(!Reached)
                if(m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                {
                    m_creature->SetVisibility(VISIBILITY_OFF);
                    if(pInstance && pInstance->GetData(TYPE_MURU) == NOT_STARTED)
                    {
                        m_creature->CastSpell(m_creature->getVictim(), DARK_FIEND_DEBUFF, true);
                        m_creature->AttackerStateUpdate(m_creature->getVictim());
                        pInstance->SetData(TYPE_MURU, DONE);
                        Reached = true;
                    }
                    // set data explosion done
                }
        }
    }
};

/*######
## mob_voidsentinel
######*/
struct MANGOS_DLL_DECL mob_voidsentinelAI : public ScriptedAI
{
    mob_voidsentinelAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 AuraTimer;
    uint32 BlastTimer;

    void Reset() 
    {   
        AuraTimer   = 3000;
        BlastTimer  = 15000;
    }
    
    void Aggro(Unit *who){}
    
    void JustDied(Unit* Killer) 
    {
        for(uint8 i=0; i<8; ++i)
        {
            Creature* sTrash = m_creature->SummonCreature(ID_SPAWN, m_creature->GetPositionX()+rand()%2, m_creature->GetPositionY()+rand()%2, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
            if(Unit* sTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                if(sTrash)
                    sTrash->AI()->AttackStart(sTarget);
        }
    }
    void KilledUnit(Unit *Victim) {}
      
    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
        
        if(AuraTimer < diff)
        {
            DoCast(m_creature, VOID_AURA);
            AuraTimer = 3000;
        }else AuraTimer -=diff;

        if(BlastTimer < diff)
        {
            DoCast(m_creature->getVictim(), VOID_BLAST, true);
            BlastTimer = 20000;
        }else BlastTimer -= diff;

        DoMeleeAttackIfReady();

    }
};

/*######
## mob_singularity
######*/
struct MANGOS_DLL_DECL mob_singularityAI : public ScriptedAI
{
    mob_singularityAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    uint32 ChangeTargetTimer;

    void Reset()                    
    {
        m_creature->SetDisplayId(25206);
        m_creature->CastSpell(m_creature, AURA_SINGULARITY, true);  //Sigularity aura
        m_creature->CastSpell(m_creature, SPELL_ARCANEFORM, true);
        ChangeTargetTimer = 5000;
    }
    void Aggro(Unit *who)           {} 
    void JustDied(Unit* Killer)     {}
    void KilledUnit(Unit *Victim)   {}
      
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

        if(ChangeTargetTimer < diff)
        {
            if(Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                m_creature->Attack(target, true);
            ChangeTargetTimer = 5000;
        }else ChangeTargetTimer -= diff;
    }
};

CreatureAI* GetAI_boss_muru(Creature *_Creature)
{
    return new boss_muruAI(_Creature);
}
 
CreatureAI* GetAI_dark_fiend(Creature *_Creature)
{
    return new dark_fiendAI(_Creature);
}
 
CreatureAI* GetAI_mob_voidsentinel(Creature *_Creature)
{
    return new mob_voidsentinelAI(_Creature);
}

CreatureAI* GetAI_mob_singularity(Creature *_Creature)
{
    return new mob_singularityAI(_Creature);
}

void AddSC_boss_muru_entropius()
{
    Script *newscript;
        
    newscript = new Script;
    newscript->Name="boss_muru";
    newscript->GetAI = &GetAI_boss_muru;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="dark_fiend";
    newscript->GetAI = &GetAI_dark_fiend;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_voidsentinel";
    newscript->GetAI = &GetAI_mob_voidsentinel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="mob_singularity";
    newscript->GetAI = &GetAI_mob_singularity;
    newscript->RegisterSelf();
}

