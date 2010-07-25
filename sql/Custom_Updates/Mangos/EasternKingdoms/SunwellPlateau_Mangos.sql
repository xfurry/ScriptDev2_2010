/* SUNWELL PLATEAU */
-- SWP KALECGNOS
-- Madrigosa should not be spawned
update `creature` set `spawnMask` = 0 where `id` in (24895, 25319);
-- position
replace into `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) values (102510, 24891, 580, 1, 1, 23350, 0, 1704.316, 928.544, -74.558, 4.65823, 360, 0, 0, 828555, 169350, 0, 0);
replace into `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) values (100698, 24892, 580, 1, 1, 0, 0, 1681.422, 909.523, -74.558, 0.67234, 604800, 0, 0, 2018275, 1693500, 0, 0);
-- SWP FELMYST
-- Felmyst
UPDATE creature_template SET `ScriptName` = 'boss_felmyst' WHERE `entry` IN (25038); 
UPDATE creature_template SET `ScriptName` = 'mob_felmyst_vapor' WHERE `entry` IN (25265); 
UPDATE creature_template SET `ScriptName` = 'mob_felmyst_trail' WHERE `entry` IN (25267); 
UPDATE creature_template SET `ScriptName` = 'mob_deathcloud' WHERE `entry` IN (25703); 

-- SWP MURU
-- Muru and Entropius
UPDATE `creature_template` SET `ScriptName` = 'boss_muru' WHERE `entry` IN (25741);
-- Dark Fiend
UPDATE `creature_template` SET `ScriptName` = 'dark_fiend' WHERE `entry` IN (25744); 
-- Void Sentinel
UPDATE `creature_template` SET `ScriptName` = 'mob_voidsentinel' WHERE `entry` IN (25772); 
-- Singularity
UPDATE creature_template SET `faction_A` = '14', `faction_H` = '14', `ScriptName` = 'mob_singularity' WHERE `entry` IN (25855); 

-- SWP KILJAEDEN
-- Kiljaeden 
UPDATE `creature_template` SET `ScriptName` = 'boss_kiljaeden' WHERE `entry` IN (25315); 
UPDATE `creature_template` SET `ScriptName` = 'mob_deceiver' WHERE `entry` IN (25588); 
UPDATE `creature_template` SET `ScriptName` = 'mob_orb' WHERE `entry` IN (25502); 
UPDATE `creature_template` SET `ScriptName` = 'mob_armagedon' WHERE `entry` IN (25735); 
UPDATE `creature_template` SET `ScriptName` = 'mob_killimp' WHERE `entry` IN (25598); 
-- Blue Dragon
UPDATE `creature_template` SET `unit_flags` = '6',`npcflag` = '1', `ScriptName` = 'blue_dragon', `RegenHealth` = '0' WHERE `entry` IN (25653); 
-- Shadow Spike
UPDATE `creature_template` SET `faction_A` = '14', `faction_H` = '14', `ScriptName` = 'mob_shadowspike' WHERE `entry` IN (30598); 
-- Controller
delete from creature where id = 25608;
insert into creature values (800110,25608,580,1,1,0,0,1698.61,628.414,27.5395,3.99799,604800,0,0,9347800,1693500,0,0);

-- SWP EREDAR TWINS
-- Sacrolash
UPDATE `creature_template` SET `ScriptName` = 'boss_sacrolash' WHERE `entry` IN (25165); 
-- Alysthes
UPDATE `creature_template` SET `ScriptName` = 'boss_alythess' WHERE `entry` IN (25166); 
-- ShadowImage
UPDATE `creature_template` SET `ScriptName` = 'npc_shadow_image' WHERE `entry` IN (25214); 

-- Missing or missmatched Doors, from UDB
delete from `gameobject` where `id` in (187990, 187764, 187765, 187766, 188118);
delete from `gameobject_template` where `entry` in (187990, 187764, 187765, 187766, 188118);
insert into `gameobject` values (160001, 187990, 580, 1, 1, 1849.66, 597.861, 86.105, 2.42601, 0, 0, 0.936672, 0.350207, 180, 100, 0),
(160002, 187764, 580, 1, 1, 1876.03, 616.974, 44.6421, 1.46608, 0, 0, 0.669131, 0.743144, 180, 100, 0),
(160003, 187765, 580, 1, 1, 1700.56, 711.332, 85.444, -0.191984, 0, 0, -0.095845, 0.995396, 180, 100, 0),
(160004, 187766, 580, 1, 1, 1709.94, 530.091, 93.4119, 1.11701, 0, 0, 0.529919, 0.848048, 180, 100, 0),
(160005, 188118, 580, 1, 1, 1775.49, 673.15, 91.9614, 1.55334, 0, 0, 0.700909, 0.71325, 180, 100, 1);
insert into `gameobject_template` values (187990, 0, 7652, 'Doodad_SunwellRaid_Gate_07', '', '', '', 1375, 32, 3.90739, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,''),
(187764, 0, 7651, 'Rohendor, The Second Gate', '', '', '', 0, 32, 4.45398, 0, 0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0, ''),
(187765, 0, 7651, 'Archonisus, The Third Gate', '', '', '', 0, 32, 4.7524, 0, 0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0, ''),
(187766, 0, 7652, 'Agamath, The First Gate', '', '', '', 0, 32, 2.1379, 0, 0, 0, 0, 0, 0, 0, 86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0, ''),
(188118, 0, 7703, 'Doodad_SunwellRaid_Gate_08', '', '', '', 1375, 32, 8.68847, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0, '');
-- untargetable
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (187990, 187764, 187765, 187766, 188118);