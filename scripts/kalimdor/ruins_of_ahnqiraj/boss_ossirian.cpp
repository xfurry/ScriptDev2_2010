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
SDName: Boss_Ossirian
SD%Complete: 0
SDComment: Place holder
SDCategory: Ruins of Ahn'Qiraj
EndScriptData */

#include "precompiled.h"
#include "Weather.h"

enum
{
     SAY_SURPREME1               =    -1509018,
     SAY_SURPREME2               =    -1509019,
     SAY_SURPREME3               =    -1509020,

     SAY_RAND_INTRO1             =    -1509021,
     SAY_RAND_INTRO2             =    -1509022,
     SAY_RAND_INTRO3             =    -1509023,
     SAY_RAND_INTRO4             =    -1509024,                           //possibly old?
     SAY_AGGRO                   =    -1509025,

     SAY_SLAY                    =    -1509026,
     SAY_DEATH                   =    -1509027,

     SPELL_CURSE_OF_TONGUES      =  25195,
     SPELL_STRENGTH_OF_OSSIRIAN  =  25176,
     SPELL_SUMMON_PLAYER         =  20477,
     SPELL_WAR_STOMP             =  25188,

     //Crystal Weaknesses
     SPELL_FIRE_WEAKNESS         =   25177,
     SPELL_NATURE_WEAKNESS       =   25180,
     SPELL_FROST_WEAKNESS        =   25178,
     SPELL_ARCANE_WEAKNESS       =   25171,
     SPELL_SHADOW_WEAKNESS       =   25183,

     //Tornado
     NPC_TORNADO                 =  19922,
     SPELL_ENVELOPING_WINDS      =  25189,

     GO_OSSIRIAN_CRYSTAL        =   180619,
     CRYSTAL_TRIGGER            =   15590
};
struct SpawnLocations
{
    float x,y,z;
};

static SpawnLocations TornadoSpawn[]=
{
    {-9444,1857,85.55f},
    {-9352,2012,85.55f}
};

struct MANGOS_DLL_DECL boss_ossirianAI : public ScriptedAI
{
    boss_ossirianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    GameObject *pCrystal;
    Creature *pTornado;

    uint32 m_uiCurseOfTongues_Timer;
    uint32 m_uiStrengthOfOssirian_Timer;
    uint32 m_uiWarStomp_Timer;
    uint32 m_uiSummonTornado_Timer;
    uint32 m_uiCastWeakness_Timer;
    uint32 m_uiCrystalCheck_Timer;
    
    bool m_bIsEnraged;
    bool m_bCastWeakness;
        
    void Reset()
    {
        Unit *pTarget = NULL;
        GameObject *pCrystal = NULL;

        m_uiCurseOfTongues_Timer = 30000;
        m_uiStrengthOfOssirian_Timer = 25000;
        m_uiWarStomp_Timer = 25000;
        m_uiCastWeakness_Timer = 5000;

        m_bCastWeakness = true;
        m_bIsEnraged = true;

        std::list<GameObject*> lCrystals;
        GetGameObjectListWithEntryInGrid(lCrystals,m_creature,GO_OSSIRIAN_CRYSTAL,MAX_VISIBILITY_DISTANCE);
        if (lCrystals.empty())
            debug_log("SD2 ERROR : In AQ 20 no ossirian crystals with the entry %i were found",GO_OSSIRIAN_CRYSTAL);
        else
        {
            for(std::list<GameObject*>::iterator iter = lCrystals.begin(); iter != lCrystals.end(); ++iter)
                if ((*iter)) 
                {
                    (*iter)->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    (*iter)->ResetDoorOrButton();
                }
        }
        SetWeather(WEATHER_STATE_FINE, 0.0f);  
        //Weather *wth 
        //Weather *wth = Weather::SetWeather(WEATHER_TYPE_STORM,1.0f);
        
        //SetWeather(WEATHER_STATE_HEAVY_SANDSTORM, 0.0f);
        /*if (Weather* wth = sWorld.FindWeather(m_creature->GetZoneId()))
            wth->SetWeather(WEATHER_TYPE_STORM, 0);*/
    }
    void SpellHit(Unit* pUnit, const SpellEntry* pSpell)
    {
        if (pUnit != m_creature)
            return;

        if (pSpell->Id == SPELL_FIRE_WEAKNESS || pSpell->Id == SPELL_FROST_WEAKNESS || pSpell->Id == SPELL_SHADOW_WEAKNESS || pSpell->Id == SPELL_NATURE_WEAKNESS || pSpell->Id == SPELL_ARCANE_WEAKNESS)
        {
            if (pCrystal)
                pCrystal->Delete();
            m_creature->RemoveAurasDueToSpell(SPELL_STRENGTH_OF_OSSIRIAN);
            m_uiStrengthOfOssirian_Timer = 45000;
            m_bIsEnraged = false;
            m_bCastWeakness = true;
        }
    }

    void SetWeather(uint32 weather, float grade)
    {
        Map *map = m_creature->GetMap();
        if (!map->IsDungeon()) return;
        WorldPacket data(SMSG_WEATHER, (4+4+4));
        data << uint32(weather) << (float)grade << uint8(0);
        ((InstanceMap*)map)->SendToPlayers(&data);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO,m_creature);
        DoCast(m_creature,SPELL_STRENGTH_OF_OSSIRIAN);
        for (uint8 i = 0; i < 2; ++i)
            m_creature->SummonCreature(NPC_TORNADO,TornadoSpawn[i].x,TornadoSpawn[i].y,TornadoSpawn[i].z,0,TEMPSUMMON_MANUAL_DESPAWN,0);

        /*uint32 zoneid = m_creature->GetZoneId();
        Weather* wth = sWorld.FindWeather(zoneid);
        if (wth)
            wth->SetWeather(WeatherType(3), 2);*/

        SetWeather(WEATHER_STATE_HEAVY_SANDSTORM, 0.0f);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH,m_creature);
        std::list<Creature*> m_lTornados;
        SetWeather(WEATHER_STATE_FINE, 0.0f);  
        GetCreatureListWithEntryInGrid(m_lTornados,m_creature,NPC_TORNADO,MAX_VISIBILITY_DISTANCE);
            if (!m_lTornados.empty())
                for(std::list<Creature*>::iterator iter = m_lTornados.begin(); iter != m_lTornados.end(); ++iter)
                    if ((*iter) && (*iter)->isAlive())
                        (*iter)->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
        DoScriptText(SAY_SLAY,m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsEnraged)
        {
            if (GameObject *pCrystal = GetClosestGameObjectWithEntry(m_creature,GO_OSSIRIAN_CRYSTAL,ATTACK_DISTANCE))
            {
                if (pCrystal->HasFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND))
                    pCrystal->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);      //make it usable
                if (pCrystal->getLootState() == GO_ACTIVATED)
                {   
                    if (m_bCastWeakness)
                    {
                        Creature *pTrigger = m_creature->SummonCreature(CRYSTAL_TRIGGER,pCrystal->GetPositionX(),pCrystal->GetPositionY(),pCrystal->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,6000);
                        if (pTrigger)
                        {
                            pTrigger->Attack(m_creature,false);    //if removed, spell won't hit ossirian, but still not 100%
                            switch (rand()%5)
                            {
                            case 0:
                                pTrigger->CastSpell(m_creature,SPELL_FIRE_WEAKNESS,false);
                                break; 
                            case 1:
                                pTrigger->CastSpell(m_creature,SPELL_ARCANE_WEAKNESS,false);
                                break; 
                            case 2:
                                pTrigger->CastSpell(m_creature,SPELL_NATURE_WEAKNESS,false);
                                break; 
                            case 3:
                                pTrigger->CastSpell(m_creature,SPELL_FROST_WEAKNESS,false);
                                break; 
                            case 4:
                                pTrigger->CastSpell(m_creature,SPELL_SHADOW_WEAKNESS,false);
                                break;
                            default:
                                pTrigger->CastSpell(m_creature,SPELL_SHADOW_WEAKNESS,false);
                                break;
                            }
                            m_bCastWeakness = false;
                        }
                    }

                    if (m_uiCastWeakness_Timer < uiDiff)
                    {
                        m_creature->SendObjectDeSpawnAnim(pCrystal->GetGUID());
                        pCrystal->SetRespawnTime(300);
                        m_creature->RemoveAurasDueToSpell(SPELL_STRENGTH_OF_OSSIRIAN);
                        m_uiStrengthOfOssirian_Timer = 45000;
                        m_bIsEnraged = false;
                        m_bCastWeakness = true;
                        m_uiCastWeakness_Timer = 5000;
                    }
                    else
                        m_uiCastWeakness_Timer -= uiDiff;
                }
            }

            if (m_uiCrystalCheck_Timer < uiDiff)
            {
                std::list<GameObject*> lCrystals2;
                GetGameObjectListWithEntryInGrid(lCrystals2,m_creature,GO_OSSIRIAN_CRYSTAL,MAX_VISIBILITY_DISTANCE);
                if (!lCrystals2.empty())
                     for(std::list<GameObject*>::iterator iter = lCrystals2.begin(); iter != lCrystals2.end(); ++iter)
                         if ((*iter))
                             (*iter)->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                m_uiCrystalCheck_Timer = 5000;
            }
            else
                m_uiCrystalCheck_Timer -= uiDiff;
        }

        if (m_uiCurseOfTongues_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_CURSE_OF_TONGUES);
            m_uiCurseOfTongues_Timer = 10000 + rand()%10000;
        }
        else
            m_uiCurseOfTongues_Timer -= uiDiff;

        if (!m_bIsEnraged && m_uiStrengthOfOssirian_Timer < uiDiff)
        {
            DoCast(m_creature,SPELL_STRENGTH_OF_OSSIRIAN);
            m_bIsEnraged = true;
        }
        else
            m_uiStrengthOfOssirian_Timer -= uiDiff;

        if (Player* pPlayer = GetPlayerAtMinimumRange(DEFAULT_VISIBILITY_DISTANCE))           //port player back if he gets too far
            DoCast(pPlayer, SPELL_SUMMON_PLAYER);

        if (m_uiWarStomp_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_WAR_STOMP);
            m_uiWarStomp_Timer = 25000 + rand()%10000;
        }
        else
            m_uiWarStomp_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ossirian(Creature* pCreature)
{
    return new boss_ossirianAI(pCreature);
}

void AddSC_boss_ossirian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ossirian";
    newscript->GetAI = &GetAI_boss_ossirian;
    newscript->RegisterSelf();
}