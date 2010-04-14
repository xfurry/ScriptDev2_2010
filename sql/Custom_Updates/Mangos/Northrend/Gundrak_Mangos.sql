/* GUNDRAK */
-- Temporary change the Drakkari colossus with Drakkari Elemental: original id = 29307
update `creature` set `id` = 29307 where `guid` = 85546;
-- Script name 
update `creature_template` set `scriptName` = 'boss_colossus' where `entry` = 29307;
update `creature_template` set `scriptName` = 'boss_drakkari_elemental' where `entry` = 29573;
update `creature_template` set `scriptName` = 'npc_living_mojo' where `entry` = 29830;
update `creature_template` set `scriptName` = 'boss_eck' where `entry` = 29932;
-- Fish fixing
Update `creature_template` set `InhabitType` = 2 where `entry` in (29834, 30928);
-- Objects
Update `gameobject` set `spawnMask` = 3 where `id` in (192632, 192569, 192568, 193208, 193209, 192564, 192567, 192565, 192566);
update `gameobject_template` set `faction` = 114 where `entry` in (192564, 192567, 192565, 192566);
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '0' where `entry` in (192632, 192569, 192568, 193208, 193209);
Update `creature` set `spawnMask` = 0 where `id` = 29748;
-- Galdarah rhinos: temporary -> original faction = 35; min & max hp = 1, min & max dmg = 1
#Update `creature_template` set `faction_A` = 14, `faction_H` = 14, `minhealth` = 10000, `maxhealth` = 20000, `mindmg` = 388, `maxdmg` = 582, `dmg_multiplier` = 10, `minrangedmg` = 388, `maxrangedmg` = 582  where `entry` = 29791;