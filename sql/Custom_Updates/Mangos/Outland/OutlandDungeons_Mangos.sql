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
#update `creature_template` set `ScriptName` = 'boss_vazruden_herald', `inhabitType` = 4 where `entry` = 17536;
#update `creature_template` set `ScriptName` = 'boss_vazruden' where `entry` = 17537;
#update `creature_template` set `lootid` = 18435 where `entry` = 18435;
#Delete from `creature_loot_template` where `entry` = 17307 and `item` = 23901;
#Delete from `creature_loot_template` where `entry` = 18435 and (`item` = 23901 or `item` = 33833);
#INSERT INTO `creature_loot_template` VALUES (17307, 23901, -100, 0, 1, 1, 0, 0, 0), 
#(18435, 23901, -100, 0, 1, 1, 0, 0, 0),
#(18435, 33833, -100, 0, 1, 1, 0, 0, 0);

-- Blood Furnace
#update `creature` set `position_y` = 50.55695 where `id` = 17380;
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (181766, 181811, 181812, 181822, 181819, 181823);
-- cell doors
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (181813, 181814, 181815, 181816, 181817, 181818, 181820, 181821);
update `gameobject_template` set flags = 6553632 where entry = 181982