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
SDName: Zul'Gurub
SD%Complete: 100
SDComment: The Areatrigger for the speech
SDCategory: Zul'Gurub
EndScriptData */

/* ContentData
at_zulgurub_entrance
EndContentData */

#include "precompiled.h"
#include "zulgurub.h"

/*######
## at_zulgurub_entrance
######*/

enum
{
    ZONE_YELL_HAKKAR_MINIONS    =   1309022
};
bool bHasSay = false;
bool AreaTrigger_at_zulgurub_entrance(Player* pPlayer, AreaTriggerEntry* pAt)
{
   if (!bHasSay)
   {
       ScriptedInstance* m_pInstance = (ScriptedInstance*)pPlayer->GetInstanceData();
       if (m_pInstance && m_pInstance->GetData(DATA_SAY_ENTRANCE) != DONE)
       {
           Unit* pHakkar = Unit::GetUnit((*pPlayer),m_pInstance->GetData64(DATA_HAKKAR));
           if (pHakkar)
            {
                DoScriptText(ZONE_YELL_HAKKAR_MINIONS,pHakkar);
                m_pInstance->SetData(DATA_SAY_ENTRANCE,DONE);
                bHasSay = true;
                return true;
            }
        }
    }
    return false;
}

void AddSC_zulgurub()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "at_zulgurub_entrance";
    newscript->pAreaTrigger = &AreaTrigger_at_zulgurub_entrance;
    newscript->RegisterSelf();
}