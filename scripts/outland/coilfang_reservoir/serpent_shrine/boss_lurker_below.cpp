/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Lurker_below
SD%Complete: 90
SDComment: 
SDCategory: Coilfang Resevoir, Serpent Shrine Cavern
EndScriptData */

#include "precompiled.h"
#include "serpent_shrine.h"


#define SPELL_SPOUT			37433	//unused until fixed
#define SPELL_SPOUT_2		42835
#define SPELL_KNOCKBACK		19813
#define SPELL_GEYSER		37478
#define SPELL_WHIRL			37660
#define SPELL_WATERBOLT		37138
#define SPELL_SUBMERGE		37550
#define SPELL_EMERGE		20568	//Ragnaros' Emerge :P as long as it works I dont care xP

#define EMOTE_SPOUT	"takes a deep breath."

#define SpoutDist	30

#define MOB_DUMMY				41003
#define MOBID_COILFANG_GUARDIAN 21873
#define MOBID_COILFANG_AMBUSHER 21865

//Ambusher spells
#define SPELL_MULTISHOT		30990

//Guardian spells
#define SPELL_ARCINGSMASH	38761 // Wrong SpellId. Can't find the right one.
#define SPELL_HAMSTRING		26211

#define PI 3.1415

float AddPos[9][3] = 
{
    {2.855381f, -459.823914f, -19.182686f},	//MOVE_AMBUSHER_1 X, Y, Z
    {12.4f, -466.042267f, -19.182686f},		//MOVE_AMBUSHER_2 X, Y, Z
    {51.366653f, -460.836060f, -19.182686f},	//MOVE_AMBUSHER_3 X, Y, Z
    {62.597980f, -457.433044f, -19.182686f},	//MOVE_AMBUSHER_4 X, Y, Z
    {77.607452f, -384.302765f, -19.182686f},	//MOVE_AMBUSHER_5 X, Y, Z
    {63.897900f, -378.984924f, -19.182686f},	//MOVE_AMBUSHER_6 X, Y, Z
    {34.447250f, -387.333618f, -19.182686f},	//MOVE_GUARDIAN_1 X, Y, Z
    {14.388216f, -423.468018f, -19.625271f},	//MOVE_GUARDIAN_2 X, Y, Z
	{42.471519f, -445.115295f, -19.769423f}	//MOVE_GUARDIAN_3 X, Y, Z
};

struct MANGOS_DLL_DECL boss_the_lurker_belowAI : public ScriptedAI
{
    boss_the_lurker_belowAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->SetSplineFlags(SPLINEFLAG_FLYING);
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 SpoutTimer;
	uint32 SVTimer;
	uint32 PhaseTimer;
	uint32 GeyserTimer;
	uint32 WaterboltTimer;
	uint32 WhirlTimer;
	uint32 OrientationUpdated;
	double SpoutAngle;
	std::map<uint64,bool>guids;
	bool Spawned;
	bool Submerged;
	bool Clockwise;
	Creature* Summoned;
	int SpTimer;

    void Reset()
    {
		//m_creature->RemoveAllAuras();
        //m_creature->DeleteThreatList();
		//m_creature->CombatStop(true);
		//DoGoHome();

        m_creature->SetSplineFlags(SPLINEFLAG_FLYING);
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
		m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);//if wiped when submerged flag wasn't removed
		WhirlTimer = rand()%5000 + 15000;
		PhaseTimer = 120000;
		SpoutTimer = 15000;
		SVTimer = 0;
		OrientationUpdated = -1;
		GeyserTimer = rand()%5000 + 15000;
		WaterboltTimer = 3000;
		Submerged = false;
		Spawned = false;
		Clockwise = true;
		SpoutAngle = 0;
		guids.clear();
		SpTimer = 0;

		//m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
		//m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
		//m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
		//m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPTED, true);
		//m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, NOT_STARTED);
	}
    /* Can get the event buggy
	void MoveInLineOfSight(Unit *who)
    {	
		if (!who) 
		return;

		if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

				if(!m_creature->isInCombat())
					Aggro(who);
			}
		}
    }

	void AttackStart(Unit *who)
    {
		if (!who || m_creature->getVictim()) return;
		
		if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
				m_creature->AddThreat(who,1.0f);
				Aggro(who);
			}else EnterEvadeMode();
		}
    }*/

	void Aggro(Unit *who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, IN_PROGRESS);
    }

    void JustDied(Unit* pVictim)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THELURKER_EVENT, DONE);
    }
	
	void UpdateAI(const uint32 diff)
    {
		//Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
            return;

		if( m_creature->getVictim() && m_creature->isAlive())
        {
			if(!Submerged)
			{
				if(SpoutTimer < diff)
				{
					//Turning while spouting ;) bit fuzzy but working
					if (OrientationUpdated == -1)
					{
						OrientationUpdated = 0;
						Unit* target = NULL;
						target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
						m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
						//DoScriptText(EMOTE_SPOUT, m_creature);
						if(target)
							SpoutAngle = target->GetAngle(m_creature)-PI;
						else
							SpoutAngle = 0;

						SpoutTimer = 3000;

						switch(rand()%2)
						{
						case 0:
							Clockwise = true; 
							break;
						case 1: 
							Clockwise = false; 
							break;
						}
						guids.clear();	//clear targets
						return;
					} else {
						if(Clockwise)
							SpoutAngle += PI/30;
						else
							SpoutAngle -= PI/30;

						if(SpoutAngle > 2*PI)
							SpoutAngle -= 2*PI;
						if(SpoutAngle < 0)
							SpoutAngle += 2*PI;

						//So spin around ya evil fish
						m_creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
						m_creature->SetOrientation(SpoutAngle);
						m_creature->StopMoving();

						//Spout part
						if(SVTimer < diff)
						{
							DoCast(m_creature, SPELL_SPOUT_2);
							SVTimer = 1250;
						}
						Unit* Temp = NULL;
                        /*
						std::list<HostileReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
						for (;i != m_creature->getThreatManager().getThreatList().end(); ++i)
						{
							Temp = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());

							std::map<uint64, bool>::iterator itr = guids.find(Temp->GetGUID());
							if(itr == guids.end())
							{
								if (Temp != SelectUnit(SELECT_TARGET_TOPAGGRO,0))
								{
									if (Temp && m_creature->HasInArc(PI/4,Temp) && !Temp->IsInWater() && m_creature->GetDistance(Temp) < SpoutDist)
									{
										guids[Temp->GetGUID()] = false;
										Creature* dummy0 = m_creature->SummonCreature(MOB_DUMMY,Temp->GetPositionX(),Temp->GetPositionY(),Temp->GetPositionZ(),m_creature->GetOrientation(),TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,5000);
										if (dummy0)
										{
											dummy0->DealDamage(Temp, 2000+rand()%3500, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_FROST, NULL, false);
											dummy0->CastSpell(Temp,SPELL_KNOCKBACK,true);
											dummy0->DealDamage(dummy0, dummy0->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
										}
									}
								}
							}
						}*/

						SVTimer -= diff;
						OrientationUpdated++;
						SpoutTimer = 250;

						if (OrientationUpdated == 60)
						{
							SpoutTimer = 45000;
							OrientationUpdated = -1;
							WhirlTimer = 0;
						}
					}
				}else SpoutTimer -= diff;

				if(PhaseTimer >= diff)
					PhaseTimer -= diff; //need to count the phase timer while spouting too
				if(OrientationUpdated != -1)
					return;

				//Whirl directly after a Spout and at random times
				if(WhirlTimer < diff)
				{
					WhirlTimer = rand()%5000 + 15000;
					DoCast(m_creature,SPELL_WHIRL);
				}else WhirlTimer -= diff;

				if(PhaseTimer < diff)
				{
					DoCast(m_creature,SPELL_SUBMERGE);
					PhaseTimer = 60000;
					Submerged = true;
				}/*else PhaseTimer -= diff;*/

				if(GeyserTimer < diff)
				{
					Unit* target = NULL;
					target = SelectUnit(SELECT_TARGET_RANDOM,0);
					if(target == SelectUnit(SELECT_TARGET_TOPAGGRO,0))
						target = SelectUnit(SELECT_TARGET_TOPAGGRO,1);
					GeyserTimer = rand()%5000 + 15000;
					if(target)
						DoCast(target,SPELL_GEYSER);
					else
						DoCast(m_creature->getVictim(),SPELL_GEYSER);
				}else GeyserTimer -= diff;

				if(WaterboltTimer < diff)
				{
					Unit* target = NULL;
					int i = 0;
					bool meleeTarget = false;
					target = SelectUnit(SELECT_TARGET_TOPAGGRO,0);
					if (!target)
						target = m_creature->getVictim();
					while (target)
					{
						if( m_creature->IsWithinDistInMap(target, 14))
						{
							meleeTarget = true;
							break;
						}
						target = SelectUnit(SELECT_TARGET_TOPAGGRO,i);
						i++;
					}

					if(!meleeTarget)
					{
						target = SelectUnit(SELECT_TARGET_RANDOM,0);
						if(target)
							DoCast(target,SPELL_WATERBOLT);
						else
							DoCast(m_creature->getVictim(),SPELL_WATERBOLT);
						WaterboltTimer = 3000;
					} else DoMeleeAttackIfReady();
					
				}else WaterboltTimer -= diff;

				DoMeleeAttackIfReady();

			} else {
				if(PhaseTimer < diff)
				{
					Submerged = false;
					m_creature->RemoveAllAuras();
					m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					m_creature->RemoveFlag(UNIT_NPC_EMOTESTATE,EMOTE_STATE_SUBMERGED);
					m_creature->RemoveFlag(UNIT_FIELD_BYTES_1,9);
					DoCast(m_creature,SPELL_EMERGE);
					Spawned = false;
					SpoutTimer = 2000;	// directly cast Spout after emerging!
					PhaseTimer = 120000;
					return;
				}
				else
				{
					if(!m_creature->isInCombat())
						m_creature->SetInCombatState(false);
					PhaseTimer -= diff;
				}
				if(!Spawned)
				{
					m_creature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[0][0],AddPos[0][1],AddPos[0][2]);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[1][0],AddPos[1][1],AddPos[1][2]);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[2][0],AddPos[2][1],AddPos[2][2]);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[3][0],AddPos[3][1],AddPos[3][2]);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[4][0],AddPos[4][1],AddPos[4][2]);
					SummonAdd(MOBID_COILFANG_AMBUSHER,AddPos[5][0],AddPos[5][1],AddPos[5][2]);
					SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[6][0],AddPos[6][1],AddPos[6][2]);
					SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[7][0],AddPos[7][1],AddPos[7][2]);
					SummonAdd(MOBID_COILFANG_GUARDIAN,AddPos[8][0],AddPos[8][1],AddPos[8][2]);
					Spawned = true;
				}
			}
		}
	}
	void SummonAdd(uint32 entry, float x, float y, float z)
	{
		Summoned = m_creature->SummonCreature(entry, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
		Unit *target = NULL;
		target = SelectUnit(SELECT_TARGET_RANDOM, 0);
		if(target)
			Summoned->AI()->AttackStart(target);
	}
};

CreatureAI* GetAI_boss_the_lurker_below(Creature *_Creature)
{
    return new boss_the_lurker_belowAI (_Creature);
}


void AddSC_boss_the_lurker_below()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_the_lurker_below";
    newscript->GetAI = GetAI_boss_the_lurker_below;
    newscript->RegisterSelf();
}
/* ?!?
CreatureAI* GetAI_mob_coilfang_guardian(Creature *_Creature)
{
    SimpleAI* ai = new SimpleAI (_Creature);

    ai->Spell[0].Enabled = true;
    ai->Spell[0].Spell_Id = SPELL_ARCINGSMASH;
    ai->Spell[0].Cooldown = 15000;
    ai->Spell[0].First_Cast = 5000;
    ai->Spell[0].Cast_Target_Type = CAST_HOSTILE_TARGET;

    ai->Spell[1].Enabled = true;
    ai->Spell[1].Spell_Id = SPELL_HAMSTRING;
    ai->Spell[1].Cooldown = 10000;
    ai->Spell[1].First_Cast = 2000;
    ai->Spell[1].Cast_Target_Type = CAST_HOSTILE_TARGET;

	return ai;
}

void AddSC_mob_coilfang_guardian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_coilfang_guardian";
    newscript->GetAI = GetAI_mob_coilfang_guardian;
    newscript->RegisterSelf();
}*/

struct MANGOS_DLL_DECL mob_coilfang_ambusherAI : public ScriptedAI
{
    mob_coilfang_ambusherAI(Creature *c) : ScriptedAI(c) {Reset();}

	uint32 MultiShotTimer;
	uint32 ShootBowTimer;
	uint32 GlobalCooldown;
    
    void Reset()
    {
		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

		MultiShotTimer = 10000;
		ShootBowTimer = 4000;
		GlobalCooldown = 1500;
        //DoGoHome();
	}

	void AttackStart(Unit *who)
    {
		if (!who || m_creature->getVictim()) return;

		if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
			Aggro(who);
        }
    }

    void MoveInLineOfSight(Unit *who)
	{
		if (!who || m_creature->getVictim()) return;

		if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
			Aggro(who);
        }
	}

	void Aggro(Unit *who)
    {
    }
	
	void UpdateAI(const uint32 diff)
    {
		if(MultiShotTimer < diff && GlobalCooldown < diff)
		{
			Unit* target = NULL;
			target = m_creature->getVictim();
			if(target)
				DoCast(target,SPELL_MULTISHOT);
			MultiShotTimer = 10000;
			GlobalCooldown = 1500;
		}else MultiShotTimer -= diff;

		if(ShootBowTimer < diff && GlobalCooldown < diff)
		{
			Unit* target = NULL;
			target = m_creature->getVictim();
			if(target)
				DoCast(target,30493);
			ShootBowTimer = 5000;
			GlobalCooldown = 1500;
		}else ShootBowTimer -= diff;
		GlobalCooldown -= diff;
	}
};

CreatureAI* GetAI_mob_coilfang_ambusher(Creature *_Creature)
{
    return new mob_coilfang_ambusherAI (_Creature);
}

void AddSC_mob_coilfang_ambusher()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="mob_coilfang_ambusher";
    newscript->GetAI = GetAI_mob_coilfang_ambusher;
    newscript->RegisterSelf();
}
