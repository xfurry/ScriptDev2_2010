/* OBSIDIAN SANCTUM */
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 30688;
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 30858;
UPDATE `creature_template` SET `ScriptName` = 'mob_twilight_fissure' WHERE `entry` = 30641;
UPDATE `creature_template` SET  modelid_A = 11686, `ScriptName` = 'mob_flame_tsunami' WHERE `entry` = 30616;
#UPDATE `creature_template` SET `ScriptName` = 'mob_safe_area' WHERE `entry` = 30494; -- unknown
#UPDATE `creature_template` SET `ScriptName` = 'mob_lava_blaze' WHERE `entry` = 30643; -- not used
UPDATE `creature_template` SET `ScriptName` = 'mob_onyx_flight_captain' WHERE `entry` = 30682;
UPDATE `creature_template` SET `scriptName` = 'mob_onyx_sanctum_guardian' WHERE `entry` = 30453;
UPDATE `creature_template` SET `ScriptName` = 'npc_flame_tornado' WHERE `entry` = 30648;
UPDATE `creature_template` SET `ScriptName` = 'mob_twilight_eggs' WHERE `entry` in (31204);
-- fix loot:
-- 10 man:
DELETE FROM `creature_loot_template` WHERE (`entry`=28860);
INSERT INTO `creature_loot_template` VALUES 
(28860, 43345, 100, 0, 1, 1, 0, 0, 0),
(28860, 43347, 100, 0, 1, 1, 0, 0, 0),
(28860, 43986, 5, 0, 1, 1, 0, 0, 0),
(28860, 47241, 100, 0, 5, 5, 0, 0, 0),
(28860, 40426, 0, 1, 1, 1, 0, 0, 0),
(28860, 40427, 0, 1, 1, 1, 0, 0, 0),
(28860, 40428, 0, 1, 1, 1, 0, 0, 0),
(28860, 40429, 0, 1, 1, 1, 0, 0, 0),
(28860, 40430, 0, 1, 1, 1, 0, 0, 0),
(28860, 40613, 0, 3, 1, 1, 0, 0, 0),
(28860, 40614, 0, 3, 1, 1, 0, 0, 0),
(28860, 40615, 0, 3, 1, 1, 0, 0, 0);
-- 25 man:
DELETE FROM `creature_loot_template` WHERE (`entry`=31311);
INSERT INTO `creature_loot_template` VALUES 
(31311, 43345, 100, 0, 1, 1, 0, 0, 0),
(31311, 43346, 100, 0, 1, 1, 0, 0, 0),
(31311, 47241, 100, 0, 5, 5, 0, 0, 0),
(31311, 40432, 0, 2, 1, 1, 0, 0, 0),
(31311, 40438, 0, 2, 1, 1, 0, 0, 0),
(31311, 40439, 0, 2, 1, 1, 0, 0, 0),
(31311, 40455, 0, 2, 1, 1, 0, 0, 0),
(31311, 40431, 0, 2, 1, 1, 0, 0, 0),
(31311, 40437, 0, 2, 1, 1, 0, 0, 0),
(31311, 40446, 0, 2, 1, 1, 0, 0, 0),
(31311, 40451, 0, 2, 1, 1, 0, 0, 0),
(31311, 40453, 0, 2, 1, 1, 0, 0, 0),
(31311, 40433, 0, 2, 1, 1, 0, 0, 0),
(31311, 40628, 0, 3, 1, 1, 0, 0, 0),
(31311, 40629, 0, 3, 1, 1, 0, 0, 0),
(31311, 40630, 0, 3, 1, 1, 0, 0, 0);
-- loot chests:
DELETE FROM `gameobject_template` WHERE `entry` in (500101, 500102, 500103, 500104, 500105, 500106);
INSERT INTO `gameobject_template` VALUES (500101, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500101, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `gameobject_template` VALUES (500102, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500102, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `gameobject_template` VALUES (500103, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500103, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `gameobject_template` VALUES (500104, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500104, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `gameobject_template` VALUES (500105, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500105, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
INSERT INTO `gameobject_template` VALUES (500106, 3, 1387, 'Sartharion Bonus Chest', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 57, 500106, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');
-- on map:
DELETE FROM `gameobject` WHERE `guid` in (800301, 800302, 800303, 800304, 800305, 800306);
INSERT INTO `gameobject` values
(800301, 500101, 615, 1, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1),
(800302, 500102, 615, 1, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1),
(800303, 500103, 615, 1, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1),
(800304, 500104, 615, 2, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1),
(800305, 500105, 615, 2, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1),
(800306, 500106, 615, 2, 1, 3242.42, 476.855,  57.426, 1.66, 0, 0, 0.366481, -0.930425, -604800, 100, 1);
-- chests loot:
-- 10 man:
-- 1 drake:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500101);
INSERT INTO `gameobject_loot_template` VALUES 
(500101, 43988, 0, 2, 1, 1, 0, 0, 0),
(500101, 43989, 0, 2, 1, 1, 0, 0, 0),
(500101, 43990, 0, 2, 1, 1, 0, 0, 0),
(500101, 43991, 0, 2, 1, 1, 0, 0, 0),
(500101, 43992, 0, 2, 1, 1, 0, 0, 0);
-- 2 drakes:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500102);
INSERT INTO `gameobject_loot_template` VALUES 
(500102, 43988, 0, 2, 1, 1, 0, 0, 0),
(500102, 43989, 0, 2, 1, 1, 0, 0, 0),
(500102, 43990, 0, 2, 1, 1, 0, 0, 0),
(500102, 43991, 0, 2, 1, 1, 0, 0, 0),
(500102, 43992, 0, 2, 1, 1, 0, 0, 0),
(500102, 43993, 0, 3, 1, 1, 0, 0, 0),
(500102, 43994, 0, 3, 1, 1, 0, 0, 0),
(500102, 43995, 0, 3, 1, 1, 0, 0, 0),
(500102, 43996, 0, 3, 1, 1, 0, 0, 0),
(500102, 43998, 0, 3, 1, 1, 0, 0, 0);
-- 3 drakes:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500103);
INSERT INTO `gameobject_loot_template` VALUES 
(500103, 43988, 0, 2, 1, 1, 0, 0, 0),
(500103, 43989, 0, 2, 1, 1, 0, 0, 0),
(500103, 43990, 0, 2, 1, 1, 0, 0, 0),
(500103, 43991, 0, 2, 1, 1, 0, 0, 0),
(500103, 43992, 0, 2, 1, 1, 0, 0, 0),
(500103, 43993, 0, 3, 1, 1, 0, 0, 0),
(500103, 43994, 0, 3, 1, 1, 0, 0, 0),
(500103, 43995, 0, 3, 1, 1, 0, 0, 0),
(500103, 43996, 0, 3, 1, 1, 0, 0, 0),
(500103, 43998, 0, 3, 1, 1, 0, 0, 0),
(500103, 43986, 100, 0, 1, 1, 0, 0, 0);
-- 25 man:
-- 1 drake:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500104);
INSERT INTO `gameobject_loot_template` VALUES 
(500104, 44000, 0, 4, 1, 1, 0, 0, 0),
(500104, 44002, 0, 4, 1, 1, 0, 0, 0),
(500104, 44003, 0, 4, 1, 1, 0, 0, 0),
(500104, 44004, 0, 4, 1, 1, 0, 0, 0);
-- 2 drakes:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500105);
INSERT INTO `gameobject_loot_template` VALUES 
(500105, 44000, 0, 4, 1, 1, 0, 0, 0),
(500105, 44002, 0, 4, 1, 1, 0, 0, 0),
(500105, 44003, 0, 4, 1, 1, 0, 0, 0),
(500105, 44004, 0, 4, 1, 1, 0, 0, 0),
(500105, 44005, 0, 1, 1, 1, 0, 0, 0),
(500105, 44006, 0, 1, 1, 1, 0, 0, 0),
(500105, 44007, 0, 1, 1, 1, 0, 0, 0),
(500105, 44008, 0, 1, 1, 1, 0, 0, 0),
(500105, 44011, 0, 1, 1, 1, 0, 0, 0);
-- 3 drakes:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=500106);
INSERT INTO `gameobject_loot_template` VALUES 
(500106, 44000, 0, 4, 1, 1, 0, 0, 0),
(500106, 44002, 0, 4, 1, 1, 0, 0, 0),
(500106, 44003, 0, 4, 1, 1, 0, 0, 0),
(500106, 44004, 0, 4, 1, 1, 0, 0, 0),
(500106, 44005, 0, 1, 1, 1, 0, 0, 0),
(500106, 44006, 0, 1, 1, 1, 0, 0, 0),
(500106, 44007, 0, 1, 1, 1, 0, 0, 0),
(500106, 44008, 0, 1, 1, 1, 0, 0, 0),
(500106, 44011, 0, 1, 1, 1, 0, 0, 0),
(500106, 43954, 100, 0, 1, 1, 0, 0, 0);
-- portals
DELETE FROM `gameobject` WHERE `guid` in (800065, 800032, 800170);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800065, 193988, 615, 3, 1, 3137.26, 501.08, 87.9118, 0.846795, 0, 0, 0.41086, 0.911698, -30, 0, 1),
(800032, 193988, 615, 3, 1, 3362.01, 553.726, 95.7068, 4.56818, 0, 0, 0.756211, -0.654328, -30, 0, 1),
(800170, 193988, 615, 3, 1, 3219.67, 656.795, 87.2898, 5.92596, 0, 0, 0.177664, -0.984091, -30, 0, 1);