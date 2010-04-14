/* TK: THE EYE */
-- Al'ar
update `creature_template` set `ScriptName`='boss_alar' where `entry`='19514';
update `creature_template` set `ScriptName`='mob_ember_of_alar' where `entry`='19551';
update `creature_template` set `ScriptName`='mob_flame_patch_alar' where `entry`='20602';
-- Al'ar movement taken from UDB
delete from `creature_movement` where `id` = 84905;
insert into `creature_movement` (`id`, `point`, `position_x`, `position_y`, `position_z`) values
(84905, 1, 366.74, -32.3, 30), 
(84905, 2, 334.92, -45.48, 30),   
(84905, 3, 303.1, -32.3, 30), 
(84905, 4, 289.92, -0.48, 30),
(84905, 5, 303.1, 31.34, 30),
(84905, 6, 334.92, 44.52, 30),
(84905, 7, 366.74, 31.34, 30),
(84905, 8, 379.92, -0.48, 30);
update `creature` set `position_x` = 379.92, `position_y` = -0.48, `position_z` = 30, `orientation` = 3.19012, `MovementType` = 2 where id = 19514;
-- Doors
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '0', `flags` = 34  where `entry` in (184324, 184325, 184326, 184327, 184328, 184329);
