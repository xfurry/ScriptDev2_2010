/* GRUUL'S LAIR */
-- Gruuls Lair doors
UPDATE `gameobject_template` SET `faction` = 114 where `entry` in (184662, 184468);

/* WATER NPCS */
Update `creature_template` set `InhabitType` = 2, `unit_flags` = 32768 where `entry` in (35061, 35324, 35071, 35060, 34925, 29392, 25203);

/* HELLFIRE CITADEL */
-- Hellfire Rampants
UPDATE `gameobject_template` SET `faction` = 0 WHERE `entry` in (185168, 185169);
Update `gameobject` set `spawntimesecs` = -604800 where `id` in (185168, 185169);
DELETE FROM `creature` WHERE `id`=17537;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(9799519, 17537, 543, 2, 1, 0, 997, -1427.91, 1766.39, 81.8241, 5.49795, 25, 5, 0, 19000, 0, 0, 1);

-- Blood Furnace
UPDATE `creature_template` SET `unit_flags` = 514 WHERE `entry` = 17398;
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (181766, 181811, 181812, 181822, 181819, 181823, 182539, 182540);
-- cell doors
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (181813, 181814, 181815, 181816, 181817, 181818, 181820, 181821);
update `gameobject_template` set flags = 6553632, scriptName = 'go_prison_cell_lever' where entry = 181982;

DELETE FROM `gameobject` WHERE `id`=182540;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800567, 182540, 540, 2, 1, 215.772, 266.396, -13.223, 3.18488, 0, 0, 0.999766, -0.0216416, 25, 0, 1);

update gameobject set position_z = -11.223 where id in (182539, 182540);