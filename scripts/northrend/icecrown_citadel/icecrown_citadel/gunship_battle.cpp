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
SDName: gunship_battle
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
	// saurfang & bronzebeard
	SPELL_BATTLE_FURY		= 69638,
	SPELL_CLEAVE			= 15284,
	SPELL_RENDING_THROW		= 70309,
	SPELL_TASTE_BLOOD		= 69634,
	NPC_BRONZEBEARD			= 36948,
	NPC_SAURFANG			= 36939,

	// all troops
	SPELL_EXPERIENCED		= 71188,
	SPELL_VETERAN			= 71193,
	SPELL_ELITE				= 71195,
	SPELL_DESPERATE_RESOLVE	= 72536,

	// sergeant
	SPELL_BLADESTORM		= 69652,
	SPELL_WOUNDING_STRIKE	= 69651,
	NPC_KORKRON_SERGEANT	= 36960,
	NPC_SKYBREAKER_SERGEANT	= 36961,

	// soldiers
	SPELL_BURNING_PITCH		= 71335,
	NPC_KORKRON_REAVER		= 36957,
	NPC_SKYBREKER_MARINE	= 36950,

	// axethrower / rifleman
	SPELL_HURL_AXE			= 72539,
	NPC_KORKRON_AXETHROWER	= 36968,
	SPELL_SHOOT				= 70162,
	NPC_SKYBREAKER_RIFLEMAN	= 36969,

	// rocketeer / mortar soldier
	SPELL_ROCKET_ARTILLERY	= 69679,
	NPC_KORKRON_ROCKETEER	= 36982,
	NPC_SKYBREAKER_MORTAR	= 36978,

	SPELL_BELOW_ZERO		= 69705,
	NPC_KORKRON_BATTLEMAGE	= 37117,
	NPC_SKYBREAKER_SORCERER	= 37116,
};