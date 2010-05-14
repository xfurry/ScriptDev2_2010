/* OBSIDIAN SANCTUM */
#UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'mob_twilight_fissure' WHERE `entry` = 30641;
UPDATE `creature_template` SET `ScriptName` = 'mob_acolyte_of_shadron' WHERE `entry` = 30688;
UPDATE `creature_template` SET `ScriptName` = 'mob_acolyte_of_vesperon' WHERE `entry` = 30858;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'mob_shadow_fissure' WHERE `entry` = 30641;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `ScriptName` = 'mob_tsunami' WHERE `entry` = 30616;
UPDATE `creature_template` SET `ScriptName` = 'mob_safe_area' WHERE `entry` = 30494;
UPDATE `creature_template` SET `ScriptName` = 'mob_lava_blaze' WHERE `entry` = 30643;
UPDATE `creature_template` SET `ScriptName` = 'mob_onyx_flight_captain' WHERE `entry` = 30682;
UPDATE `creature_template` SET `scriptName` = 'mob_onyx_sanctum_guardian' WHERE `entry` = 30453;
#UPDATE `creature_template` SET `ScriptName` = 'npc_flame_wave' WHERE `entry` = 30616;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `ScriptName` = 'npc_flame_tornado' WHERE `entry` = 30648;
DELETE FROM `gameobject` WHERE `guid` in (800065, 800032, 800170);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800065, 193988, 615, 3, 1, 3137.26, 501.08, 87.9118, 0.846795, 0, 0, 0.41086, 0.911698, -30, 0, 1),
(800032, 193988, 615, 3, 1, 3362.01, 553.726, 95.7068, 4.56818, 0, 0, 0.756211, -0.654328, -30, 0, 1),
(800170, 193988, 615, 1, 1, 3219.67, 656.795, 87.2898, 5.92596, 0, 0, 0.177664, -0.984091, 25, 0, 1);