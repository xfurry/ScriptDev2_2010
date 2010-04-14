/* CULLING OF STRATHOLME */
update `creature_template` set `minhealth`='8600', `maxhealth`='8600' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `minhealth`='8600', `maxhealth`='8600' where `entry` in (10002, 10003, 10004, 10005);
update `creature_template` set `faction_A`='35', `faction_H`='35' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `faction_A`='35', `faction_H`='35' where `entry` in (10002, 10003, 10004, 10005);
update `creature_template` set `minhealth`='15000', `maxhealth`='15000' where `entry` in (27737);
update `creature_template` set `minhealth`='25000', `maxhealth`='25000' where `entry` in (31208);

DELETE FROM `creature` WHERE (`id`=27744);
DELETE FROM `creature` WHERE (`id`=26530);
DELETE FROM `creature` WHERE (`id`=26529);
DELETE FROM `creature` WHERE (`id`=26532);
DELETE FROM `creature` WHERE (`id`=26533);
DELETE FROM `creature` WHERE (`id`=27744);
DELETE FROM `creature` WHERE (`id`=26530);
DELETE FROM `creature` WHERE (`id`=26529);
DELETE FROM `creature` WHERE (`id`=26532);
DELETE FROM `creature` WHERE (`id`=26533);
DELETE FROM `creature` WHERE (`id`=26499);
DELETE FROM `creature` WHERE `id` in (26497, 28878, 28199, 27732, 27731, 28201, 27734, 28200, 27736, 28249);
update `creature_template` set `Scriptname`='generic_creature' where `entry` in (26497, 28878, 28199, 27732, 27731, 28201, 27734, 28200, 27736, 28249);

update `creature_template` set `AIName`='', `Scriptname`='npc_arthas' where `entry` in (26499);
update `creature_template` set `AIName`='', `Scriptname`='dark_conversion' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `AIName`='', `Scriptname`='npc_patricia' where `entry` in (31028);
update `creature_template` set `AIName`='', `Scriptname`='npc_time_riftCS' where `entry` in (28409);
update `creature_template` set `AIName`='', `Scriptname`='boss_salramm' where `entry` in (26530);
update `creature_template` set `AIName`='', `Scriptname`='boss_meathook' where `entry` in (26529);
update `creature_template` set `AIName`='', `Scriptname`='boss_lord_epoch' where `entry` in (26532);
update `creature_template` set `AIName`='', `Scriptname`='boss_malganis' where `entry` in (26533);
update `creature_template` set `AIName`='', `Scriptname`='boss_infinite_corruptor' where `entry` in (32273);
update `instance_template` set `script` = 'instance_culling_of_stratholme' WHERE map=595;
-- Spawning Salramm in the instance --
INSERT INTO creature VALUES (4458724,26530,595,3,1,0,0,2174.32,1307.32,131.866,4.32264,604800,0,0,337025,62535,0,0);
-- Spawning Meathook in the instance --
INSERT INTO creature VALUES (4458738,26529,595,3,1,0,0,2351.43,1218.03,130.078,4.64621,604800,0,0,337025,0,0,0);
-- Spawning Lord Epoch in the instance --
INSERT INTO creature VALUES (4458740,26532,595,3,1,0,1820,2446.25,1111.97,148.077,3.37779,604800,0,0,337025,41690,0,0);
-- Spawning Malganis in the instance
INSERT INTO creature VALUES (4459981,26533,595,3,1,0,0,2298.33,1501.03,128.362,5.11213,604800,0,0,404430,41690,0,0);
update `creature` set `spawntimesecs`='3600' where `id` in (31127, 31126, 28167, 28169);
update `creature` set `spawntimesecs`='604800' where `id` in (26533, 26532, 26529, 26530);

DELETE FROM `creature` WHERE (`id`=26499);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (4456649, 26499, 595, 3, 0, 0, 1920.87, 1287.12, 142.935, 6.25562, 25, 0, 0, 44100, 7988, 0, 0);

DELETE FROM `creature` WHERE (`id`=26497);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (4456653, 26497, 595, 3, 0, 0, 1895.48, 1292.66, 143.706, 0.023475, 25, 0, 0, 100800, 88140, 0, 0);

DELETE FROM `gameobject_template` WHERE (`entry`=188686);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (188686, 0, 7831, 'Doodad_LD_hidden_door_room01', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

DELETE FROM `gameobject_template` WHERE (`entry`=187711);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (187711, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

DELETE FROM `gameobject_template` WHERE (`entry`=187723);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (187723, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

update `gameobject` set `state` = '1' where `id` in (187711);
update `gameobject` set `spawntimesecs` = '-604800', `state` = '1'  where `id` in (190663, 193597);
update `gameobject_template` set `faction` = 0 where `entry` in (190663, 193597);

UPDATE `creature` set `curhealth` = '8600' where `id`=28167;
UPDATE `creature` set `curhealth` = '8600' where `id`=28169;
UPDATE `creature` set `curhealth` = '15000' where `id`=27737;

-- skeletal minion 
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(256928, 28878, 595, 3, 1, 0, 0, 2495.24, 1341.05, 133.626, 0.740786, 36000, 5, 0, 5040, 0, 0, 0),
(256918, 28878, 595, 3, 1, 0, 0, 2579.84, 1142.57, 126.193, 3.02551, 36000, 5, 0, 5040, 0, 0, 0),
(256920, 28878, 595, 3, 1, 0, 0, 2580.27, 1146.28, 126.191, 3.02551, 36000, 5, 0, 5040, 0, 0, 0),
(256922, 28878, 595, 3, 1, 0, 0, 2576.2, 1144.36, 126.422, 3.20615, 36000, 5, 0, 5040, 0, 0, 0),
(256924, 28878, 595, 3, 1, 0, 0, 2557.82, 1194.96, 126.807, 5.04084, 36000, 5, 0, 5040, 0, 0, 0),
(256926, 28878, 595, 3, 1, 0, 0, 2568.24, 1193.43, 125.702, 4.56646, 36000, 5, 0, 5040, 0, 0, 0),
(256930, 28878, 595, 3, 1, 0, 0, 2490.16, 1343.15, 133.473, 0.740786, 36000, 5, 0, 5040, 0, 0, 0),
(256932, 28878, 595, 3, 1, 0, 0, 2504.42, 1369.21, 132.898, 4.06302, 36000, 5, 0, 5040, 0, 0, 0),
(256934, 28878, 595, 3, 1, 0, 0, 2485.6, 1402.34, 131.162, 4.70155, 36000, 5, 0, 5040, 0, 0, 0),
(256936, 28878, 595, 3, 1, 0, 0, 2458.27, 1436.52, 131.62, 3.85646, 36000, 5, 0, 5040, 0, 0, 0),
(256938, 28878, 595, 3, 1, 0, 0, 2462.98, 1431.78, 131.231, 3.62712, 36000, 5, 0, 5040, 0, 0, 0),
(256940, 28878, 595, 3, 1, 0, 0, 2410.73, 1412.61, 130.436, 0.252265, 36000, 5, 0, 5040, 0, 0, 0),
(256942, 28878, 595, 3, 1, 0, 0, 2407.78, 1424.02, 131.534, 0.252265, 36000, 5, 0, 5040, 0, 0, 0);
-- tomb stalker 
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(256900, 28199, 595, 3, 1, 0, 0, 2421.24, 1428.25, 131.267, 5.53716, 36000, 5, 0, 50400, 0, 0, 0),
(256902, 28199, 595, 3, 1, 0, 0, 2407.5, 1417.59, 130.572, 0.00952625, 36000, 0, 0, 50400, 0, 0, 0),
(256904, 28199, 595, 3, 1, 0, 0, 2465.33, 1437.3, 132.117, 3.50769, 36000, 5, 0, 50400, 0, 0, 0),
(256912, 28199, 595, 3, 1, 0, 0, 2576.95, 1133.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0);
-- master necromancer
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(256944, 27732, 595, 3, 1, 0, 484, 2491.12, 1365.86, 130.722, 4.92696, 36000, 0, 0, 40320, 44070, 0, 0),
(256954, 27732, 595, 3, 1, 0, 484, 2518.89, 1129.91, 132.127, 4.83114, 36000, 2, 0, 40320, 44070, 0, 0);
-- fanatic 
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(256815, 27731, 595, 3, 1, 0, 0, 2423.24, 1428.25, 131.267, 5.53716, 36000, 5, 0, 50400, 0, 0, 0),
(256816, 27731, 595, 3, 1, 0, 0, 2410.5, 1417.59, 130.572, 0.00952625, 36000, 0, 0, 50400, 0, 0, 0),
(256817, 27731, 595, 3, 1, 0, 0, 2468.33, 1437.3, 132.117, 3.50769, 36000, 5, 0, 50400, 0, 0, 0),
(256818, 27731, 595, 3, 1, 0, 0, 2495.21, 1345.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256819, 27731, 595, 3, 1, 0, 0, 2593.59, 1166.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256820, 27731, 595, 3, 1, 0, 0, 2579.95, 1133.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256821, 27731, 595, 3, 1, 0, 0, 2567.08, 1205.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
-- bile golem
(256822, 28201, 595, 3, 1, 0, 0, 2492.21, 1340.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256823, 28201, 595, 3, 1, 0, 0, 2586.59, 1170.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256824, 28201, 595, 3, 1, 0, 0, 2573.95, 1139.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256825, 28201, 595, 3, 1, 0, 0, 2555.08, 1209.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
-- crypt fiend
(256826, 27734, 595, 3, 1, 0, 0, 2492.21, 1348.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256827, 27734, 595, 3, 1, 0, 0, 2589.59, 1163.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256829, 27734, 595, 3, 1, 0, 0, 2562.08, 1210.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
-- dark necro
(256830, 28200, 595, 3, 1, 0, 0, 2574.95, 1135.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256831, 28200, 595, 3, 1, 0, 0, 2566.08, 1207.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
-- patchwerk construct
(256833, 27736, 595, 3, 1, 0, 0, 2584.59, 1163.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256835, 27736, 595, 3, 1, 0, 0, 2556.08, 1213.35, 126.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
-- devouring ghoul
(256836, 28249, 595, 3, 1, 0, 0, 2420.24, 1423.25, 131.267, 5.53716, 36000, 5, 0, 50400, 0, 0, 0),
(256828, 28249, 595, 3, 1, 0, 0, 2576.95, 1129.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256906, 28249, 595, 3, 1, 0, 0, 2492.21, 1345.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256910, 28249, 595, 3, 1, 0, 0, 2589.59, 1166.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256914, 28249, 595, 3, 1, 0, 0, 2562.08, 1205.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0),
(256832, 28249, 595, 3, 1, 0, 0, 2498.21, 1340.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256834, 28249, 595, 3, 1, 0, 0, 2573.95, 1135.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256837, 28249, 595, 3, 1, 0, 0, 2404.5, 1415.59, 130.572, 0.00952625, 36000, 0, 0, 50400, 0, 0, 0),
(256838, 28249, 595, 3, 1, 0, 0, 2467.33, 1438.3, 132.117, 3.50769, 36000, 5, 0, 50400, 0, 0, 0),
(256839, 28249, 595, 3, 1, 0, 0, 2493.21, 1349.35, 133.309, 0.110111, 36000, 7, 0, 48700, 0, 0, 0),
(256840, 28249, 595, 3, 1, 0, 0, 2590.59, 1167.42, 126.153, 2.85587, 36000, 5, 0, 48700, 0, 0, 0),
(256841, 28249, 595, 3, 1, 0, 0, 2578.95, 1135.71, 127.075, 2.19299, 36000, 5, 0, 50400, 0, 0, 0),
(256842, 28249, 595, 3, 1, 0, 0, 2565.08, 1207.35, 125.664, 4.89947, 36000, 5, 0, 48700, 0, 0, 0);