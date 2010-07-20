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
    SAY_AGGRO           = -1605052,
    SAY_PACT            = -1605053,
    SAY_DARK            = -1605054,
    SAY_LIGHT           = -1605055,
    SAY_SLAY1           = -1605056,
    SAY_SLAY2           = -1605057,
    SAY_DEATH           = -1605058,
    SAY_BERSERK         = -1605059,
    SAY_OUTRO_ALY       = -1605081,
    SAY_OUTRO_HORDE     = -1605080,
    SAY_OUTRO_TIRION    = -1605026,
    EMOTE_PACT          = -1605133,
    EMOTE_DARK_VORTEX   = -1605134,
    EMOTE_LIGHT_VORTEX  = -1605135,

    // light & dark
    SPELL_DARK_ESSENCE          = 65684,
    SPELL_SURGE_OF_SPEED        = 67243,
    SPELL_LIGHT_ESSENCE         = 65686,

    NPC_CONCENTRATED_LIGHT      = 34630,
    NPC_CONCENTRATED_DARKNESS   = 34628,

    SPELL_POWERING_UP           = 67604,
    SPELL_EMPOWERED_DARKNESS    = 67215,
    SPELL_EMPOWERED_LIGHT       = 67218,
    SPELL_UNLEASHED_DARK_10     = 65808,
    SPELL_UNLEASHED_DARK_25     = 67173,
    SPELL_UNLEASHED_DARK_10HC   = 67172,
    SPELL_UNLEASHED_DARK_25HC   = 67174,
    SPELL_UNLEASHED_LIGHT_10    = 65795,
    SPELL_UNLEASHED_LIGHT_25    = 67239,
    SPELL_UNLEASHED_LIGHT_10HC  = 67238,
    SPELL_UNLEASHED_LIGHT_25HC  = 67240,

    // fjola lightbane
    SPELL_TWIN_SPIKE_LIGHT_10   = 66075,
    SPELL_TWIN_SPIKE_LIGHT_25   = 67313,
    SPELL_TWIN_SPIKE_LIGHT_10HC = 67312,
    SPELL_TWIN_SPIKE_LIGHT_25HC = 67314,
    SPELL_LIGHT_SURGE_10        = 65766,
    SPELL_LIGHT_SURGE_25        = 67271,
    SPELL_LIGHT_SURGE_10HC      = 67270,
    SPELL_LIGHT_SURGE_25HC      = 67272,
    SPELL_SHIELD_OF_LIGHTS_10   = 65858,
    SPELL_SHIELD_OF_LIGHTS_25   = 67260,
    SPELL_SHIELD_OF_LIGHTS_10HC = 67259,
    SPELL_SHIELD_OF_LIGHTS_25HC = 67261, 
    SPELL_TWIN_PACT_LIGHT_10    = 65876,
    SPELL_TWIN_PACT_LIGHT_25    = 67307,
    SPELL_TWIN_PACT_LIGHT_10HC  = 67306,
    SPELL_TWIN_PACT_LIGHT_25HC  = 67308,
    SPELL_LIGHT_VORTEX_10       = 66046,
    SPELL_LIGHT_VORTEX_25       = 67207,
    SPELL_LIGHT_VORTEX_10HC     = 67206,
    SPELL_LIGHT_VORTEX_25HC     = 67208,
    SPELL_TOUCH_OF_LIGHT_10HC   = 67297,
    SPELL_TOUCH_OF_LIGHT_25HC   = 67298,

    SPELL_POWER_TWINS_10        = 65916,
    SPELL_POWER_TWINS_25        = 67249,
    SPELL_POWER_TWINS_10HC      = 67248,
    SPELL_POWER_TWINS_25HC      = 67250,
    SPELL_VALKYR_QUICKNESS      = 65949,

    // eydis darkbane
    SPELL_TWIN_SPIKE_DARK_10        = 66069,
    SPELL_TWIN_SPIKE_DARK_25        = 67310,
    SPELL_TWIN_SPIKE_DARK_10HC      = 67309,
    SPELL_TWIN_SPIKE_DARK_25HC      = 67311,
    SPELL_DARK_SURGE_10             = 65768,
    SPELL_DARK_SURGE_25             = 67263,
    SPELL_DARK_SURGE_10HC           = 67262,
    SPELL_DARK_SURGE_25HC           = 67264,
    SPELL_SHIELD_OF_DARKNESS_10     = 65874,
    SPELL_SHIELD_OF_DARKNESS_25     = 67257,
    SPELL_SHIELD_OF_DARKNESS_10HC   = 67256,
    SPELL_SHIELD_OF_DARKNESS_25HC   = 67258,
    SPELL_TWIN_PACT_DARK_10         = 65875,
    SPELL_TWIN_PACT_DARK_25         = 67304,
    SPELL_TWIN_PACT_DARK_10HC       = 67303,
    SPELL_TWIN_PACT_DARK_25HC       = 67305,
    SPELL_DARK_VORTEX_10            = 66058,
    SPELL_DARK_VORTEX_25            = 67183,
    SPELL_DARK_VORTEX_10HC          = 67182,
    SPELL_DARK_VORTEX_25HC          = 67184,
    SPELL_TOUCH_OF_DARKNESS_10HC    = 67282,
    SPELL_TOUCH_OF_DARKNESS_25HC    = 67283,

    SPELL_BERSERK                   = 26662,

    ACHIEV_SALT_AND_PEPPER          = 3799,
    ACHIEV_SALT_AND_PEPPER_H        = 3815,
};

enum Equipment
{
    EQUIP_MAIN_1         = 49206, //49303,
    EQUIP_OFFHAND_1      = 47146,
    EQUIP_RANGED_1       = 47267,
    EQUIP_MAIN_2         = 42161, //45990,
    EQUIP_OFFHAND_2      = 47470,
    EQUIP_RANGED_2       = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

struct LocationsXY
{
    float x, y;
    uint32 id;
};
static LocationsXY OrbLoc[]=
{
    {563.687622f, 89.948326f},      // West
    {597.263062f, 106.157730f},     // North-West
    {612.368408f, 139.41394f},      // North
    {594.863525f, 176.002731f},     // North-east
    {563.728516f, 181.137650f},     // East
    {532.748169f, 175.445145f},     // South-east
    {515.690369f, 139.53311f},      // South
    {530.050049f, 106.749100f},     // South-west
};

#define LOC_Z           394.33f
#define ROOM_RADIUS     50.0f

/*######
## boss_fjola
######*/

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        SetEquipmentSlots(false, EQUIP_MAIN_1, -1, -1);
        doorClosed = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiTwinSpikeTimer;
    uint32 m_uiSpecialSpellTimer;
    uint32 m_uiTwinPactTimer;
	uint32 m_uiTouchTimer;
    uint32 m_uiCastTimeOut;
    bool m_bHasVortexCasted;

    uint32 m_uiDoorTimer;
    bool doorClosed;

    uint32 m_uiBerserkTimer;
    uint32 TeamInInstance;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        m_uiTwinSpikeTimer      = 7000;
        m_uiSpecialSpellTimer   = urand(40000, 45000); 
        m_uiTwinPactTimer       = 900000;
        m_uiCastTimeOut         = 900000;
		m_uiTouchTimer			= 10000;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;
        m_uiDoorTimer           = 5000;

        TeamInInstance = GetFaction();

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			m_uiBerserkTimer        = 480000;  // 8 min
		else
			m_uiBerserkTimer		= 360000;	// 6 min

        if(m_pInstance)
            m_pInstance->SetData(TYPE_TWINS_CASTING, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        switch(urand(0, 1))
        {
        case 0: m_bHasVortexCasted = true;  break;
        case 1: m_bHasVortexCasted = false; break;
        }

        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(TeamInInstance == ALLIANCE)
        {
            if(Creature *Varian = GetClosestCreatureWithEntry(m_creature, NPC_VARIAN, 150.0f))
                DoScriptText(SAY_OUTRO_ALY, Varian);
        }
        else if(TeamInInstance == HORDE)
        {
            if(Creature *Garrosh = GetClosestCreatureWithEntry(m_creature, NPC_GARROSH, 150.0f))
                DoScriptText(SAY_OUTRO_HORDE, Garrosh);
        }

        if(m_pInstance) 
        {
            if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
                if(pEydis->isAlive())
                    pEydis->DealDamage(pEydis, pEydis->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            m_pInstance->SetData(TYPE_TWIN_VALKYR, DONE);
            RemoveAllAuras();
        }
        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }
        // despawn all adds
        std::list<Creature*> lAdds;
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_LIGHT_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_DARK_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_CONCENTRATED_DARKNESS, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_CONCENTRATED_LIGHT, DEFAULT_VISIBILITY_INSTANCE);
        if (!lAdds.empty())
        {
            for(std::list<Creature*>::iterator iter = lAdds.begin(); iter != lAdds.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoScriptText(SAY_AGGRO, m_creature);  

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_TWIN_VALKYR, IN_PROGRESS);
            if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
            {
                if(pEydis->isAlive())
                    pEydis->AI()->AttackStart(m_creature->getVictim());
            }
        }

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature, SPELL_LIGHT_SURGE_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature, SPELL_LIGHT_SURGE_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature, SPELL_LIGHT_SURGE_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature, SPELL_LIGHT_SURGE_25HC);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_TWIN_VALKYR) != NOT_STARTED)
            {
                if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                    m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
                }
                m_pInstance->SetData(TYPE_TWIN_VALKYR, NOT_STARTED);
            }
        }

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        // despawn all adds
        std::list<Creature*> lAdds;
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_LIGHT_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_DARK_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_CONCENTRATED_DARKNESS, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lAdds, m_creature, NPC_CONCENTRATED_LIGHT, DEFAULT_VISIBILITY_INSTANCE);
        if (!lAdds.empty())
        {
            for(std::list<Creature*>::iterator iter = lAdds.begin(); iter != lAdds.end(); ++iter)
            {
                if ((*iter) && !(*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }

        m_creature->ForcedDespawn();
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

    void DamageTaken(Unit *pDoneBy, uint32 &uiDamage)
    {
        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
            return;
 
        if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if(pDoneBy->HasAura(SPELL_LIGHT_ESSENCE))
                uiDamage /= 2;
            else if(pDoneBy->HasAura(SPELL_DARK_ESSENCE))
                uiDamage += uiDamage/2;
        }
        else
            return;

        if(uiDamage > 0)
        {
            if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
                pEydis->DealDamage(pEydis, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateHealth()
    {
        if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
            m_creature->SetHealth(pEydis->GetHealth());
    }

    void RemoveAllAuras()
    {
        if(m_pInstance)
        {
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EMPOWERED_DARKNESS);
            m_pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EMPOWERED_LIGHT);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
            m_uiDoorTimer = 100;
        }
        else m_uiAttackStartTimer -= uiDiff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
        {
            if(m_creature->getVictim()->GetGUID() == pEydis->GetGUID()) 
                EnterEvadeMode();
        }

        if (m_uiDoorTimer < uiDiff && !doorClosed)
        {
            if(GameObject* pMainGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_MAIN_GATE)))
                m_pInstance->DoUseDoorOrButton(pMainGate->GetGUID());
            doorClosed = true;
            m_uiDoorTimer = 30000;
        }
        else
            m_uiDoorTimer -= uiDiff;

        if (m_uiCastTimeOut < uiDiff)
        {
            m_pInstance->SetData(TYPE_TWINS_CASTING, NOT_STARTED);
            m_uiCastTimeOut = 600000;
        }
        else
            m_uiCastTimeOut -= uiDiff;

        // spells
        if (m_uiTwinSpikeTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_LIGHT_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_LIGHT_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_LIGHT_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_LIGHT_25HC);
            m_uiTwinSpikeTimer = 5000 + urand(1000, 5000);
        }
        else
            m_uiTwinSpikeTimer -= uiDiff;

		// on heroic only
		if (m_uiTouchTimer < uiDiff)
		{
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_TOUCH_OF_LIGHT_10HC);
			}
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_TOUCH_OF_LIGHT_25HC);
			}
			m_uiTouchTimer = 20000;
		}
		else m_uiTouchTimer -= uiDiff;

        if (m_uiSpecialSpellTimer < uiDiff)
        {
            if(m_pInstance->GetData(TYPE_TWINS_CASTING) != IN_PROGRESS)
            {
                if(!m_bHasVortexCasted)
                {
                    DoScriptText(SAY_LIGHT, m_creature);
                    m_pInstance->SetData(TYPE_TWINS_CASTING, IN_PROGRESS);
                    DoScriptText(EMOTE_LIGHT_VORTEX, m_creature);

                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(m_creature, SPELL_LIGHT_VORTEX_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(m_creature, SPELL_LIGHT_VORTEX_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(m_creature, SPELL_LIGHT_VORTEX_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(m_creature, SPELL_LIGHT_VORTEX_25HC);

                    m_bHasVortexCasted = true;
                    m_uiCastTimeOut = 9000;
                }
                else
                {
                    m_pInstance->SetData(TYPE_TWINS_CASTING, IN_PROGRESS);

                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(m_creature, SPELL_SHIELD_OF_LIGHTS_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(m_creature, SPELL_SHIELD_OF_LIGHTS_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(m_creature, SPELL_SHIELD_OF_LIGHTS_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(m_creature, SPELL_SHIELD_OF_LIGHTS_25HC);

                    m_bHasVortexCasted = false;
                    m_uiTwinPactTimer = 500;
                    m_uiCastTimeOut = 16000;
                }
                m_uiSpecialSpellTimer = urand(45000, 50000);
            }
            else
                m_uiSpecialSpellTimer = urand(40000, 45000);
        }
        else
            m_uiSpecialSpellTimer -= uiDiff;

        if (m_uiTwinPactTimer < uiDiff)
        {
            if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 150.0f))
            {
                DoScriptText(SAY_PACT, m_creature);
                DoScriptText(EMOTE_PACT, m_creature);
                m_creature->CastStop();
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    if(DoCastSpellIfCan(pEydis, SPELL_TWIN_PACT_LIGHT_10) == CAST_OK)
                        UpdateHealth();
                    pEydis->CastSpell(pEydis, SPELL_POWER_TWINS_10,false);
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                {
                    if(DoCastSpellIfCan(pEydis, SPELL_TWIN_PACT_LIGHT_25) == CAST_OK)
                        UpdateHealth();
                    pEydis->CastSpell(pEydis, SPELL_POWER_TWINS_25,false);
                }
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    if(DoCastSpellIfCan(pEydis, SPELL_TWIN_PACT_LIGHT_10HC) == CAST_OK)
                        UpdateHealth();
                    pEydis->CastSpell(pEydis, SPELL_POWER_TWINS_10HC,false);
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    if(DoCastSpellIfCan(pEydis, SPELL_TWIN_PACT_LIGHT_25HC) == CAST_OK)
                        UpdateHealth();
                    pEydis->CastSpell(pEydis, SPELL_POWER_TWINS_25HC,false);
                }
            }
            m_uiTwinPactTimer = 600000;
        }
        else
            m_uiTwinPactTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fjola(Creature* pCreature)
{
    return new boss_fjolaAI(pCreature);
}

/*######
## boss_eydis
######*/
struct MANGOS_DLL_DECL boss_eydisAI : public ScriptedAI
{
    boss_eydisAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetEquipmentSlots(false, EQUIP_MAIN_2, -1, -1);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;
    bool m_bIsRegularMode;

    uint32 m_uiTwinSpikeTimer;
    uint32 m_uiSpecialSpellTimer;
    uint32 m_uiTwinPactTimer;
	uint32 m_uiTouchTimer;
    uint32 m_uiOrbSummonTimer;
    uint32 m_uiCastTimeOut;

    uint32 m_uiEssenceBuffCheckTimer;
    std::list<Creature*> lEssences;
    std::list<Creature*> lConcentrates;

    uint8 m_uiMaxOrbs;
    bool m_bHasVortexCasted;

    float angle;
    float homeX, homeY;

    uint32 m_uiBerserkTimer;
    uint32 m_uiEncounterTimer;

    bool m_bStartAttack;
    uint32 m_uiAttackStartTimer;

    void Reset() 
    {
        m_uiTwinSpikeTimer     = 7000;
        m_uiSpecialSpellTimer  = urand(40000, 45000); 
        m_uiTwinPactTimer      = 900000;
        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            m_uiOrbSummonTimer  = urand(30000, 35000);
        else
            m_uiOrbSummonTimer  = urand(10000, 15000);

		m_uiTouchTimer			= 20000;
        m_uiCastTimeOut         = 900000;

		if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
			m_uiBerserkTimer        = 480000;  // 8 min
		else
			m_uiBerserkTimer		= 360000;	// 6 min

        m_uiEncounterTimer      = 0;

        m_bStartAttack          = false;
        m_uiAttackStartTimer    = 10000;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_TWINS_CASTING, NOT_STARTED);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        switch(urand(0, 1))
        {
        case 0: m_bHasVortexCasted = true;  break;
        case 1: m_bHasVortexCasted = false; break;
        }

        m_uiEssenceBuffCheckTimer = 500;
        lEssences.clear();
        lConcentrates.clear();

        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        if(!m_bStartAttack)
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
        {
            if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f))
            {
                if(pFjola->isAlive())
                    pFjola->DealDamage(pFjola, pFjola->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            m_pInstance->SetData(TYPE_TWIN_VALKYR, DONE);
            DespawnAdds();
        }

        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        if(m_uiEncounterTimer < 180000)
        {
            if(m_pInstance)
            {
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_SALT_AND_PEPPER);
                else if (Difficulty == RAID_DIFFICULTY_25MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                    m_pInstance->DoCompleteAchievement(ACHIEV_SALT_AND_PEPPER_H);
            }
        }
    }

    void DespawnAdds()
    {
        GetCreatureListWithEntryInGrid(lEssences, m_creature, NPC_LIGHT_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lEssences, m_creature, NPC_DARK_ESSENCE, DEFAULT_VISIBILITY_INSTANCE);
        if (!lEssences.empty())
        {
            for(std::list<Creature*>::iterator iter = lEssences.begin(); iter != lEssences.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
        GetCreatureListWithEntryInGrid(lConcentrates, m_creature, NPC_CONCENTRATED_DARKNESS, DEFAULT_VISIBILITY_INSTANCE);
        GetCreatureListWithEntryInGrid(lConcentrates, m_creature, NPC_CONCENTRATED_LIGHT, DEFAULT_VISIBILITY_INSTANCE);
        if (!lConcentrates.empty())
        {
            for(std::list<Creature*>::iterator iter = lConcentrates.begin(); iter != lConcentrates.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoScriptText(SAY_AGGRO, m_creature);  

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_TWIN_VALKYR, IN_PROGRESS);
            if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f))
            {
                if(pFjola->isAlive())
                    pFjola->AI()->AttackStart(m_creature->getVictim());
            }
        }

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature, SPELL_DARK_SURGE_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature, SPELL_DARK_SURGE_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature, SPELL_DARK_SURGE_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature, SPELL_DARK_SURGE_25HC);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if(m_pInstance->GetData(TYPE_TWIN_VALKYR) != NOT_STARTED)
            {
                if (Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    m_pInstance->SetData(TYPE_COUNTER, m_pInstance->GetData(TYPE_COUNTER) - 1);
                    m_pInstance->DoUpdateWorldState(UPDATE_STATE_UI_COUNT, m_pInstance->GetData(TYPE_COUNTER));
                }
                m_pInstance->SetData(TYPE_TWIN_VALKYR, NOT_STARTED);
            }
        }
        if(Creature *pBarret = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(DATA_BARRET)))
        {
            pBarret->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            pBarret->SetVisibility(VISIBILITY_ON);
        }

        m_creature->ForcedDespawn();
    }

    void DamageTaken(Unit *pDoneBy, uint32 &uiDamage)
    {
        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
            return;
 
        if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if(pDoneBy->HasAura(SPELL_DARK_ESSENCE))
                uiDamage /= 2;
            else if(pDoneBy->HasAura(SPELL_LIGHT_ESSENCE))
                uiDamage += uiDamage/2;
        }
        else
            return;

        if(uiDamage > 0)
        {
            if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f))
                pFjola->DealDamage(pFjola, uiDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void UpdateHealth()
    {
        if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f))
            m_creature->SetHealth(pFjola->GetHealth());
    }

    void DoCheckEssenceBuff()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(i->getSource()->HasAura(SPELL_LIGHT_ESSENCE, EFFECT_INDEX_0))
                {
                    if(i->getSource()->HasAura(SPELL_POWERING_UP, EFFECT_INDEX_0))
                    {
                        uint8 m_uiAuraStack = i->getSource()->GetAura(SPELL_POWERING_UP, EFFECT_INDEX_0)->GetStackAmount();
                        if(m_uiAuraStack >= 100)
                        {
                            i->getSource()->RemoveAurasDueToSpell(SPELL_POWERING_UP);
                            i->getSource()->CastSpell(i->getSource(), SPELL_EMPOWERED_LIGHT, false);
                        }
                    }
                }
                else if(i->getSource()->HasAura(SPELL_DARK_ESSENCE, EFFECT_INDEX_0))
                {
                    if(i->getSource()->HasAura(SPELL_POWERING_UP, EFFECT_INDEX_0))
                    {
                        uint8 m_uiAuraStack = i->getSource()->GetAura(SPELL_POWERING_UP, EFFECT_INDEX_0)->GetStackAmount();
                        if(m_uiAuraStack >= 100)
                        {
                            i->getSource()->RemoveAurasDueToSpell(SPELL_POWERING_UP);
                            i->getSource()->CastSpell(i->getSource(), SPELL_EMPOWERED_DARKNESS, false);
                        }
                    }
                }
            }
        } 
    }

    void SummonOrbs()
    {
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC || Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            m_uiMaxOrbs = urand(15, 20);
        else
            m_uiMaxOrbs = urand(40, 50);

        for(uint8 i = 0; i < m_uiMaxOrbs; i++)
        {
            // init random orb
            uint32 m_uiCreatureEntry;
            switch (urand(0, 1))
            {
            case 0:
                m_uiCreatureEntry = NPC_CONCENTRATED_DARKNESS;
                break;
            case 1:
                m_uiCreatureEntry = NPC_CONCENTRATED_LIGHT;
                break;
            }
            // init random location
            angle = (float) rand()*360/RAND_MAX + 1;
            homeX = SpawnLoc[1].x + ROOM_RADIUS*cos(angle*(M_PI/180));
            homeY = SpawnLoc[1].y + ROOM_RADIUS*sin(angle*(M_PI/180));
            // summon orbs
            if(Creature* pOrb = m_creature->SummonCreature(m_uiCreatureEntry, homeX, homeY, LOC_Z, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                pOrb->setFaction(14);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiAttackStartTimer < uiDiff && !m_bStartAttack)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetInCombatWithZone();
            m_bStartAttack = true;
        }
        else m_uiAttackStartTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
            return;

        // achiev timer
        m_uiEncounterTimer += uiDiff;

        if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f))
        {
            if(m_creature->getVictim()->GetGUID() == pFjola->GetGUID()) 
                EnterEvadeMode();
        }

        if(m_uiEssenceBuffCheckTimer < uiDiff)
        {
            DoCheckEssenceBuff();
            m_uiEssenceBuffCheckTimer = 500;
        }
        else m_uiEssenceBuffCheckTimer -= uiDiff;
        
        if(m_uiOrbSummonTimer < uiDiff)
        {
            SummonOrbs();
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || RAID_DIFFICULTY_25MAN_NORMAL)
                m_uiOrbSummonTimer = urand(30000, 40000);
            else
                m_uiOrbSummonTimer = urand(5000, 10000);
        }
        else m_uiOrbSummonTimer -= uiDiff;

        if (m_uiCastTimeOut < uiDiff)
        {
            m_pInstance->SetData(TYPE_TWINS_CASTING, NOT_STARTED);
            m_uiCastTimeOut = 600000;
        }
        else
            m_uiCastTimeOut -= uiDiff;

        // spells
        if (m_uiTwinSpikeTimer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_DARK_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_DARK_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_DARK_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_TWIN_SPIKE_DARK_25HC);
            m_uiTwinSpikeTimer = 5000 + urand(1000, 5000);
        }
        else
            m_uiTwinSpikeTimer -= uiDiff;

		// on heroic only
		if (m_uiTouchTimer < uiDiff)
		{
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_TOUCH_OF_DARKNESS_10HC);
			}
			if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
			{
				if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					DoCast(pTarget, SPELL_TOUCH_OF_DARKNESS_25HC);
			}
			m_uiTouchTimer = 20000;
		}
		else m_uiTouchTimer -= uiDiff;

        if (m_uiSpecialSpellTimer < uiDiff)
        {
            if(m_pInstance->GetData(TYPE_TWINS_CASTING) != IN_PROGRESS)
            {
                if(!m_bHasVortexCasted)
                {
                    DoScriptText(SAY_DARK, m_creature);
                    m_pInstance->SetData(TYPE_TWINS_CASTING, IN_PROGRESS);
                    DoScriptText(EMOTE_DARK_VORTEX, m_creature);

                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(m_creature, SPELL_DARK_VORTEX_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(m_creature, SPELL_DARK_VORTEX_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(m_creature, SPELL_DARK_VORTEX_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(m_creature, SPELL_DARK_VORTEX_25HC);

                    m_bHasVortexCasted = true;
                    m_uiCastTimeOut = 9000;
                }
                else
                {
                    m_pInstance->SetData(TYPE_TWINS_CASTING, IN_PROGRESS);

                    if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        DoCast(m_creature, SPELL_SHIELD_OF_DARKNESS_10);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        DoCast(m_creature, SPELL_SHIELD_OF_DARKNESS_25);
                    if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        DoCast(m_creature, SPELL_SHIELD_OF_DARKNESS_10HC);
                    if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        DoCast(m_creature, SPELL_SHIELD_OF_DARKNESS_25HC);

                    m_bHasVortexCasted = false;
                    m_uiTwinPactTimer = 500;
                    m_uiCastTimeOut = 16000;
                }
                m_uiSpecialSpellTimer = urand(45000, 50000);
            }
            else   
                m_uiSpecialSpellTimer = urand(40000, 45000);
        }
        else
            m_uiSpecialSpellTimer -= uiDiff;

        if (m_uiTwinPactTimer < uiDiff)
        {
            DoScriptText(SAY_PACT, m_creature);
            DoScriptText(EMOTE_PACT, m_creature);
            if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 150.0f)) 
            {
                m_creature->CastStop();
                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                {
                    if(DoCastSpellIfCan(pFjola, SPELL_TWIN_PACT_DARK_10) == CAST_OK)
                        UpdateHealth();
                    pFjola->CastSpell(pFjola, SPELL_POWER_TWINS_10,false);
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                {
                    if(DoCastSpellIfCan(pFjola, SPELL_TWIN_PACT_DARK_25) == CAST_OK)
                        UpdateHealth();
                    pFjola->CastSpell(pFjola, SPELL_POWER_TWINS_25,false);
                }
                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                {
                    if(DoCastSpellIfCan(pFjola, SPELL_TWIN_PACT_DARK_10HC) == CAST_OK)
                        UpdateHealth();
                    pFjola->CastSpell(pFjola, SPELL_POWER_TWINS_10HC,false);
                }
                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                {
                    if(DoCastSpellIfCan(pFjola, SPELL_TWIN_PACT_DARK_25HC) == CAST_OK)
                        UpdateHealth();
                    pFjola->CastSpell(pFjola, SPELL_POWER_TWINS_25HC,false);
                }
            }
            m_uiTwinPactTimer = 600000;
        }
        else
            m_uiTwinPactTimer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff && !m_creature->HasAura(SPELL_BERSERK, EFFECT_INDEX_0))
        {
            m_creature->CastStop();
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            //m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

//#####
//### Concentrated Light/Darkness
//#####
struct MANGOS_DLL_DECL mob_valkyr_orbAI : public ScriptedAI
{
    mob_valkyr_orbAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);  
        m_uiCreatureEntry = pCreature->GetEntry();
        ChooseDirection();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiCreatureEntry;
    uint32 m_uiDieTimer;
    uint32 m_uiCheckTimer;
    bool m_bMustDie;

    double angle;
    double destX,destY;

    void Reset() 
    {
        m_uiDieTimer    = 2000;
        m_bMustDie      = false;
        m_uiCheckTimer  = 500;
    }

    void CheckDistance()
    {
        // check for lightbane
        if(Creature* pFjola = GetClosestCreatureWithEntry(m_creature, NPC_FJOLA, 2.0f))
        {
			if(m_uiCreatureEntry == NPC_CONCENTRATED_LIGHT)
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(m_creature, SPELL_UNLEASHED_LIGHT_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(m_creature, SPELL_UNLEASHED_LIGHT_25);
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					DoCast(m_creature, SPELL_UNLEASHED_LIGHT_10HC);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					DoCast(m_creature, SPELL_UNLEASHED_LIGHT_25HC);

				m_uiDieTimer = 1000;
				m_bMustDie = true;
				return;
			}
        }
        // check for darkbane
        if(Creature* pEydis = GetClosestCreatureWithEntry(m_creature, NPC_EYDIS, 2.0f))
        {
			if(m_uiCreatureEntry == NPC_CONCENTRATED_DARKNESS)
			{
				if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
					DoCast(m_creature, SPELL_UNLEASHED_DARK_10);
				if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
					DoCast(m_creature, SPELL_UNLEASHED_DARK_25);
				if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
					DoCast(m_creature, SPELL_UNLEASHED_DARK_10HC);
				if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
					DoCast(m_creature, SPELL_UNLEASHED_DARK_25HC);

				m_uiDieTimer = 1000;
				m_bMustDie = true;
				return;
			}
        }

        // check for players
        Map* pMap = m_creature->GetMap();
        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();
            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if(m_creature->GetDistance2d(i->getSource()) <= 3.0f && !m_bMustDie)
                    {
                        switch (m_uiCreatureEntry)
                        {
                        case NPC_CONCENTRATED_DARKNESS:
                            if(i->getSource()->HasAura(SPELL_DARK_ESSENCE))
                                i->getSource()->CastSpell(i->getSource(), SPELL_POWERING_UP, false);
                            else if(i->getSource()->HasAura(SPELL_LIGHT_ESSENCE))
                            {
                                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                    DoCast(m_creature, SPELL_UNLEASHED_DARK_10);
                                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                    DoCast(m_creature, SPELL_UNLEASHED_DARK_25);
                                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                    DoCast(m_creature, SPELL_UNLEASHED_DARK_10HC);
                                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                    DoCast(m_creature, SPELL_UNLEASHED_DARK_25HC);
                            }
                            break;
                        case NPC_CONCENTRATED_LIGHT:
                            if(i->getSource()->HasAura(SPELL_LIGHT_ESSENCE))
                                i->getSource()->CastSpell(i->getSource(), SPELL_POWERING_UP, false);
                            else if(i->getSource()->HasAura(SPELL_DARK_ESSENCE))
                            {
                                if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                    DoCast(m_creature, SPELL_UNLEASHED_LIGHT_10);
                                if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                    DoCast(m_creature, SPELL_UNLEASHED_LIGHT_25);
                                if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                    DoCast(m_creature, SPELL_UNLEASHED_LIGHT_10HC);
                                if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                    DoCast(m_creature, SPELL_UNLEASHED_LIGHT_25HC);
                            }
                            break;
                        }
                        m_uiDieTimer = 1000;
                        m_bMustDie = true;
                        return;
                    }
                }
            }
        }
    }

    void ChooseDirection()
    {       
        angle=(double) rand()*360/RAND_MAX+1;
        destX=SpawnLoc[1].x+ROOM_RADIUS*cos(angle*(M_PI/180));
        destY=SpawnLoc[1].y+ROOM_RADIUS*sin(angle*(M_PI/180));

        //m_creature->SetSpeedRate(MOVE_RUN, 2.0f);
        //m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, destX, destY, LOC_Z);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(abs(m_creature->GetPositionX() - destX) < 2.0f && abs(m_creature->GetPositionY() - destY) < 2.0f)
        {
            m_creature->GetMotionMaster()->MovementExpired();
            ChooseDirection();
        }

        if (m_uiCheckTimer < uiDiff)
        {
            CheckDistance();
            m_uiCheckTimer = 300;
        }
        else
            m_uiCheckTimer -= uiDiff;

        if (m_uiDieTimer < uiDiff && m_bMustDie)
        {
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            m_uiDieTimer = 60000;
        }
        else
            m_uiDieTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_valkyr_orb(Creature *pCreature) 
{
    return new mob_valkyr_orbAI(pCreature);
}
//#####
//### Essence Light/Darkness
//#####
struct MANGOS_DLL_DECL mob_valkyr_essenceAI : public ScriptedAI
{
    mob_valkyr_essenceAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;

    uint32 m_uiPlayerCheckTimer;
    uint32 m_uiCreatureEntry;

    void Reset() 
    {
        m_uiPlayerCheckTimer = 500;
        m_uiCreatureEntry = m_creature->GetEntry();
        m_creature->SetRespawnDelay(DAY);
    }

    void DoCastEssenceBuff()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(m_uiCreatureEntry == NPC_LIGHT_ESSENCE)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) < 2)
                    {
                        if(i->getSource()->HasAura(SPELL_DARK_ESSENCE))
                            i->getSource()->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);

                        i->getSource()->CastSpell(i->getSource(), SPELL_LIGHT_ESSENCE, false);

                        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_VORTEX_10, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_SURGE_10, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_VORTEX_25, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_SURGE_25, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_VORTEX_10HC, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_SURGE_10HC, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_VORTEX_25HC, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_LIGHT_SURGE_25HC, 0, 0, true);
                        }
                    }
                }
                else if(m_uiCreatureEntry == NPC_DARK_ESSENCE)
                {
                    if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(),i->getSource()->GetPositionY()) < 2)
                    {
                        if(i->getSource()->HasAura(SPELL_LIGHT_ESSENCE))
                            i->getSource()->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);

                        i->getSource()->CastSpell(i->getSource(), SPELL_DARK_ESSENCE, false);

                        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_DARK_VORTEX_10, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_DARK_SURGE_10, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_DARK_VORTEX_25, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_DARK_SURGE_25, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_DARK_VORTEX_10HC, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_DARK_SURGE_10HC, 0, 0, true);
                        }
                        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                        {
                            i->getSource()->ApplySpellImmune(SPELL_DARK_VORTEX_25HC, 0, 0, true);
                            i->getSource()->ApplySpellImmune(SPELL_DARK_SURGE_25HC, 0, 0, true);
                        }
                    }
                }
            }
        } 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPlayerCheckTimer < uiDiff)
        {
            DoCastEssenceBuff();
            m_uiPlayerCheckTimer = 500;
        }
        else
            m_uiPlayerCheckTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_valkyr_essence(Creature *pCreature) 
{
    return new mob_valkyr_essenceAI(pCreature);
}

void AddSC_twin_valkyr()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_fjola";
    newscript->GetAI = &GetAI_boss_fjola;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_eydis";
    newscript->GetAI = &GetAI_boss_eydis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_valkyr_orb";
    newscript->GetAI = &GetAI_mob_valkyr_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_valkyr_essence";
    newscript->GetAI = &GetAI_mob_valkyr_essence;
    newscript->RegisterSelf();
}
