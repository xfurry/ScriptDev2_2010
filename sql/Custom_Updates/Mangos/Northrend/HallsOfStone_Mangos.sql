/* HALLS OF STONE */
UPDATE `instance_template` SET `scriptName`='instance_halls_of_stone' WHERE `map`=599;
UPDATE `creature_template` SET `ScriptName`='boss_krystallus' WHERE `entry`='27977';
UPDATE `creature_template` SET `ScriptName`='mob_iron_sludge' WHERE `entry`=28165;
UPDATE `creature_template` SET `ScriptName`='mob_tribuna_controller', `faction_A`=16, `faction_H`=16 WHERE `entry`='28234';
UPDATE `creature_template` SET `flags_extra` = '2' WHERE `entry` IN (30897, 30898, 30899, 28237, 28265);
DELETE FROM `creature` WHERE map = 599 AND `id`='28234';
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES 
(28234, 599, 3, 1, 11686, 0, 941.279, 377.052, 207.421, 2.28638, 3600, 2, 0, 7212, 0, 0, 1); -- mob_tribuna_controller
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=190586;
UPDATE `gameobject` SET `state`='1' WHERE `map`=599 AND `id` IN (191292,191295,191296);
-- Fix invisible stalker!
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35 where `entry` = 28130;
-- Tribunal objects
UPDATE `gameobject_template` SET `faction` = 114 where `entry` in (193907, 191670, 191669, 191671, 193906); 
-- Tribunal chest
UPDATE `gameobject_template` SET `faction` = 0, `data3` = 1, `data12` = 1, `data13` = 1, data15 = 1 where `entry` in (190586, 193996); 
Update `gameobject` set `spawntimesecs` = -604800 where `id` in (193996, 190586);
-- Doors
UPDATE `gameobject_template` SET `faction` = 114 where `entry` in (191292, 191295, 191296, 191293, 191294); 
-- Spawn tribunal on heroic
Update `creature` set `spawnMask` = 3 where `id` in (30898, 30899, 30897);
