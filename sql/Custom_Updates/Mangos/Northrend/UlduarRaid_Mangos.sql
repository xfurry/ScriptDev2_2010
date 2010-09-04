/* ULDUAR */

-- Flame Leviathan
UPDATE creature_template SET ScriptName = "boss_flame_leviathan" WHERE entry = 33113;
UPDATE creature_template SET ScriptName = "mob_defense_turret" WHERE entry = 33142;

-- Ignis
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_ignis' WHERE entry=33118;
UPDATE creature_template SET ScriptName = "mob_iron_construct" WHERE entry = 33121;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('64474', '1', '33118');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('64475', '1', '33118');
UPDATE creature_template SET scriptname='mob_scorch_target' WHERE entry=33221;

-- Razorscale
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_razorscale' WHERE entry=33186;
-- original x=587.547, y= -174.927, z = 391.517;
update creature set position_x = 590.346741, position_y = -226.947647, position_z = 442.897583 where id = 33186;
UPDATE gameobject_template SET flags= 6553648, ScriptName="go_broken_harpoon" WHERE entry = 194565;
-- only 2 harpoons for 10 man
update gameobject set spawnMask = 2 where guid in (73595, 73592);
-- mole machines
UPDATE creature_template SET ScriptName = "mob_mole_machine" WHERE entry = 33245;
UPDATE creature_template SET ScriptName = "mob_dark_rune_watcher" WHERE entry = 33453;
UPDATE creature_template SET ScriptName = "mob_dark_rune_sentinel" WHERE entry = 33846;
UPDATE creature_template SET ScriptName = "mob_dark_rune_guardian" WHERE entry = 33388;
UPDATE creature_template SET ScriptName = "npc_expedition_commander" WHERE entry = 33210;
UPDATE creature_template SET ScriptName = "mob_devouring_flame_target" WHERE entry in (34189, 34188);

-- XT002
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_xt002' WHERE entry=33293;
UPDATE creature_template SET ScriptName = "mob_pummeler" WHERE entry = 33344;
UPDATE creature_template SET speed_run=0.5, faction_a=1925, faction_h=1925, scriptname='mob_boombot' WHERE entry=33346;
UPDATE creature_template SET speed_run=0.5 WHERE entry=33343;
UPDATE creature_template SET mechanic_immune_mask=652951551, scriptname='mob_xtheart' WHERE entry=33329;
UPDATE creature_template SET ScriptName = "mob_voidzone" WHERE entry = 34001;
UPDATE creature_template SET minhealth = 176400, maxhealth = 176400, minlevel = 80, maxlevel = 80, faction_a = 14, faction_h = 14, ScriptName = "mob_lifespark" WHERE entry = 34004;
Update creature set spawnMask = 0 where id in (34004);

-- Iron council
UPDATE creature_template SET mechanic_immune_mask=617299711, scriptname='boss_brundir' WHERE entry=32857;
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_molgeim' WHERE entry=32927;
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_steelbreaker' WHERE entry=32867;
UPDATE creature_template SET ScriptName = "mob_rune_of_power" WHERE entry = 33705;
UPDATE creature_template SET ScriptName = "mob_rune_of_summoning" WHERE entry = 33051;
UPDATE creature_template SET ScriptName = "mob_ulduar_lightning_elemental" WHERE entry = 32958;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` in (32857, 33694);

-- Kologarn
DELETE FROM creature WHERE id IN (32933, 32934);
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) VALUES (32933, 603, 3, 65535, 0, 0, 1799.68, -24.3599, 452.227, 3.14747, 604800, 0, 0, 543855, 0, 0, 0);
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) VALUES (32934, 603, 3, 65535, 0, 0, 1799.68, -24.3599, 452.227, 3.14747, 604800, 0, 0, 543855, 0, 0, 0);
UPDATE creature_model_info SET bounding_radius=15, combat_reach=15 WHERE modelid IN (28638, 28822, 28821);
UPDATE creature_template SET mechanic_immune_mask=617299803, unit_flags = 0, scriptname='boss_kologarn' WHERE entry=32930;
UPDATE creature_template SET unit_flags = 0 WHERE entry in (32930, 33909);
UPDATE creature_template SET mechanic_immune_mask=652951551, scriptname='boss_right_arm' WHERE entry=32934;
UPDATE creature_template SET mechanic_immune_mask=652951551, scriptname='boss_left_arm' WHERE entry=32933;
UPDATE creature_template SET ScriptName = "mob_ulduar_rubble" WHERE entry in (33768, 33809, 33908, 33942);
update `gameobject` set `position_y` = -35.6824, `position_x` = 1837.59 where `id` in (195047);
UPDATE `creature_template` SET `RegenHealth` = 1 WHERE `entry` = 33910;
UPDATE `creature_template` SET `RegenHealth` = 1 WHERE `entry` = 33911;

-- Auriaya
UPDATE creature_template SET mechanic_immune_mask=583745371, equipment_id = 103000, scriptname='boss_auriaya' WHERE entry=33515;
UPDATE creature_template SET mechanic_immune_mask=619395071, scriptname='mob_feral_defender' WHERE entry=34035;
UPDATE creature_template SET minlevel=80, maxlevel=80, faction_h=14, faction_a=14, scriptname='mob_seeping_feral_essence' WHERE entry=34098;
UPDATE creature_template SET ScriptName = "mob_sanctum_sentry" WHERE entry = 34014;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` in (33515, 34175);
delete from creature_equip_template where entry = 103000;
insert into creature_equip_template values (103000, 45315, 0, 0);
-- 2 more defenders for 25 man
delete from creature where guid in (800010, 800011);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(800010, 34014, 603, 2, 65535, 0, 0, 1945.2, 37.2442, 411.356, 3.62107, 7200, 0, 0, 334680, 0, 0, 0),
(800011, 34014, 603, 2, 65535, 0, 0, 1936.11, 49.8233, 411.352, 3.85276, 7200, 0, 0, 334680, 0, 0, 0);
DELETE FROM `creature_movement` WHERE `id`=94378;
INSERT INTO `creature_movement` (`id`,`point`,`position_x`,`position_y`,`position_z`,`waittime`,`textid1`,`textid2`,`textid3`,`textid4`,`textid5`,`emote`,`spell`,`wpguid`,`orientation`,`model1`,`model2`) VALUES

(94378, 1, 1925.012, 30.0067, 411.356, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.69685, 0, 0),	-- before yogg down
(94378, 2, 1957.04, 49.3067, 411.355, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.37071, 0, 0),	-- after yogg down
(94378, 3, 1967.38, 51.4931, 417.561, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.09545, 0, 0),	-- after yogg up
(94378, 4, 2013.07, 44.3788, 417.715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.71365, 0, 0), 	-- before mimiron up
(94378, 5, 2021.35, 37.9771, 411.387, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.25205, 0, 0), 	-- before mimiron down
(94378, 6, 2046.36, 8.56725, 411.524, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.203, 0, 0),		-- after mimiron down 
(94378, 7, 2053.32, -7.1366, 421.78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.85107, 0, 0),		-- before freya up
(94378, 8, 2052.87, -40.8556, 421.706, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.4223, 0, 0), 	-- after freya up
(94378, 9, 2045.00, -56.79369, 411.359, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.20538, 0, 0),	-- before thorim down
(94378, 10, 2022.18, -86.5468, 411.355, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.60096, 0, 0),	-- after thorim down
(94378, 11, 2012.94, -92.7106, 417.717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.28968, 0, 0),	-- after thorim up
(94378, 12, 1968.83, -101.0946, 417.722, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.662873, 0, 0),-- before hodir up
(94378, 13, 1958.08, -96.7855, 411.864, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.763719, 0, 0),	-- before hodir down
(94378, 14, 1924.12, -78.5404, 411.488, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.210024, 0, 0),	-- after hodir down

(94378, 15, 1958.08, -96.7855, 411.864, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.763719, 0, 0),	-- before hodir down
(94378, 16, 1968.83, -101.0946, 417.722, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.662873, 0, 0),-- before hodir up
(94378, 17, 2012.94, -92.7106, 417.717, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.28968, 0, 0),	-- after thorim up
(94378, 18, 2022.18, -86.5468, 411.355, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.60096, 0, 0),	-- after thorim down
(94378, 19, 2045.00, -56.79369, 411.359, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.20538, 0, 0),	-- before thorim down
(94378, 20, 2052.87, -40.8556, 421.706, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.4223, 0, 0), 	-- after freya up
(94378, 21, 2053.32, -7.1366, 421.78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4.85107, 0, 0),	-- before freya up
(94378, 22, 2046.36, 8.56725, 411.524, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.203, 0, 0),		-- after mimiron down
(94378, 23, 2021.35, 37.9771, 411.387, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.25205, 0, 0), 	-- before mimiron down
(94378, 24, 2013.07, 44.3788, 417.715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5.71365, 0, 0), 	-- before mimiron up
(94378, 25, 1967.38, 51.4931, 417.561, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.09545, 0, 0),	-- after yogg up
(94378, 26, 1957.04, 49.3067, 411.355, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.37071, 0, 0);	-- after yogg down

-- Freya
UPDATE creature_template SET ScriptName = "boss_freya" WHERE entry = 32906;
UPDATE creature_template SET ScriptName = "boss_elder_brightleaf" WHERE entry = 32915;
UPDATE creature_template SET ScriptName = "boss_elder_ironbranch" WHERE entry = 32913;
UPDATE creature_template SET ScriptName = "boss_elder_stonebark" WHERE entry = 32914;
UPDATE creature_template SET ScriptName = "mob_iron_roots" WHERE entry in (33088, 33168);
UPDATE creature_template SET ScriptName = "mob_freya_ground" WHERE entry in (33215, 33228, 33170, 33050, 34129);
UPDATE creature_template SET ScriptName = "mob_freya_spawned" WHERE entry in (32916, 32919, 33202, 33203, 32918);
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62525', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62524', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62521', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62385', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62387', '1', '32906');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62386', '1', '32906');

-- Hodir
UPDATE creature_template SET ScriptName = "boss_hodir" WHERE entry = 32845;
UPDATE creature_template SET ScriptName = "mob_toasty_fire" WHERE entry = 33342;
UPDATE creature_template SET ScriptName = "mob_flashFreeze" WHERE entry in (32926);
UPDATE `creature_template` SET `modelid_1` = 15880 WHERE `entry` = 33174;
UPDATE `creature_template` SET `modelid_3` = 28470, ScriptName = "mob_icicle" WHERE `entry` = 33169;
-- flash freeze that will lock the npcs in iceblock
UPDATE creature_template SET `modelid_1` = 25865, ScriptName = "mob_npc_flashFreeze" WHERE entry in (32938, 33353);
Update creature set spawnMask = 0 where id in (32938); -- will be summoned from script
update creature set spawnMask = 2 where id in (32901, 32900, 32950, 32946,33333, 33330, 33326);
UPDATE creature_template SET ScriptName = "npc_hodir_priest" WHERE entry in (32897, 33326, 32948, 33330);
UPDATE creature_template SET ScriptName = "npc_hodir_druid" WHERE entry in (33325, 32901, 32941, 33333);
UPDATE creature_template SET ScriptName = "npc_hodir_shaman" WHERE entry in (33328, 32900, 33332, 32950);
UPDATE creature_template SET ScriptName = "npc_hodir_mage" WHERE entry in (32893, 33327, 33331, 32946);
-- fix some npcs
delete from creature where guid in (800005, 800008);
insert into creature VALUES
(800005, 32897, 603, 3, 128,0,0, 2009.06, -244.058, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0),		-- aly priest
(800008, 33327, 603, 2, 128,0,0, 1978.49, -241.476, 432.687, 1.68485, 7200,0,0, 5647, 0, 0, 0); 	-- aly mage

-- Mimiron
UPDATE creature_template SET ScriptName = "boss_mimiron" WHERE entry = 33350;
UPDATE creature_template SET `RegenHealth`= 0, ScriptName = "boss_leviathan_mk" WHERE entry = 33432;
UPDATE creature_template SET ScriptName = "leviathan_turret" WHERE entry = 34071;
UPDATE creature_template SET ScriptName = "mob_mimiron_flames" WHERE entry in (34363, 34121);
UPDATE creature_template SET `RegenHealth`= 0, ScriptName = "boss_vx001" WHERE entry = 33651;
UPDATE creature_template SET `RegenHealth`= 0, ScriptName = "boss_aerial_command_unit" WHERE entry = 33670;
update creature set position_x = 2784.35, position_y = 2578.03, orientation = 3.2 where id = 33350;
update creature set position_x = 2794.86, position_y = 2597.83, orientation = 3.57, spawnMask = 3 where id = 33432;
UPDATE gameobject_template SET flags = 6553632, data2 = 2000, ScriptName="go_red_button" WHERE entry = 194739;
UPDATE creature_template SET ScriptName = "mob_proximity_mine" WHERE entry = 34362;
UPDATE creature_template SET ScriptName = "mob_bomb_bot" WHERE entry in (33836, 34192);
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, `minlevel` = 80, `maxlevel` = 80, ScriptName = "mob_emergency_bot" WHERE entry = 34147;
UPDATE creature_template SET ScriptName = "mob_frost_bomb" WHERE entry = 34149;
UPDATE creature_template SET ScriptName = "mob_mimiron_inferno" WHERE entry = 33370;
UPDATE creature_template SET ScriptName = "mob_assault_bot" WHERE entry = 34057;
UPDATE creature_template SET ScriptName = "mob_magnetic_core" WHERE entry = 34068;
update `gameobject` set `position_x` = 2734.73 where `id` in (194789, 194956);
-- spells
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('64444', '1', '33670');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('63414', '1', '33651');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('65101', '1', '33350');
-- REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62909', '1', '33350');
-- mimiron loot fix:
-- insert two new boxes
DELETE FROM `gameobject` WHERE `id` in (194957, 194958);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(110004, 194957, 603, 1, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(110005, 194958, 603, 2, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1);
-- 10 man:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=194789);
INSERT INTO `gameobject_loot_template` VALUES 
(194789, 45089, 5, 0, -45089, 1, 0, 0, 0),
(194789, 45095, 2.0408, 0, 1, 1, 0, 0, 0),
(194789, 45649, 100, 0, -45649, 1, 0, 0, 0),
(194789, 45663, 0.4028, 0, 1, 1, 0, 0, 0),
(194789, 47241, 100, 0, 1, 1, 0, 0, 0),
(194789, 45972, 0, 1, 1, 1, 0, 0, 0),
(194789, 45973, 0, 1, 1, 1, 0, 0, 0),
(194789, 45974, 0, 1, 1, 1, 0, 0, 0),
(194789, 45975, 0, 1, 1, 1, 0, 0, 0),
(194789, 45976, 0, 1, 1, 1, 0, 0, 0);
-- 25 man:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=194956);
INSERT INTO `gameobject_loot_template` VALUES 
(194956, 45038, 10, 0, 1, 1, 0, 0, 0),
(194956, 45087, 33, 0, 1, 1, 0, 0, 0),
(194956, 45089, 5, 0, -45089, 1, 0, 0, 0),
(194956, 45643, 100, 0, -45643, 1, 0, 0, 0),
(194956, 47241, 100, 0, 1, 1, 0, 0, 0),
(194956, 45489, 0, 1, 1, 1, 0, 0, 0),
(194956, 45490, 0, 1, 1, 1, 0, 0, 0),
(194956, 45491, 0, 1, 1, 1, 0, 0, 0),
(194956, 45492, 0, 1, 1, 1, 0, 0, 0),
(194956, 45493, 0, 1, 1, 1, 0, 0, 0);
-- 10 man hard:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=194957);
INSERT INTO `gameobject_loot_template` VALUES 
(194957, 45089, 5, 0, -45089, 1, 0, 0, 0),
(194957, 45095, 2.0408, 0, 1, 1, 0, 0, 0),
(194957, 45649, 100, 0, -45649, 1, 0, 0, 0),
(194957, 45663, 0.4028, 0, 1, 1, 0, 0, 0),
(194957, 45787, -100, 0, 1, 1, 0, 0, 0),
(194957, 47241, 100, 0, 1, 1, 0, 0, 0),
(194957, 45972, 0, 1, 1, 1, 0, 0, 0),
(194957, 45973, 0, 1, 1, 1, 0, 0, 0),
(194957, 45974, 0, 1, 1, 1, 0, 0, 0),
(194957, 45975, 0, 1, 1, 1, 0, 0, 0),
(194957, 45976, 0, 1, 1, 1, 0, 0, 0),
(194957, 45982, 0, 2, 1, 1, 0, 0, 0),
(194957, 45988, 0, 2, 1, 1, 0, 0, 0),
(194957, 45989, 0, 2, 1, 1, 0, 0, 0),
(194957, 45990, 0, 2, 1, 1, 0, 0, 0),
(194957, 45993, 0, 2, 1, 1, 0, 0, 0);
-- 25 man hard:
DELETE FROM `gameobject_loot_template` WHERE (`entry`=194958);
INSERT INTO `gameobject_loot_template` VALUES 
(194958, 45038, 10, 0, 1, 1, 0, 0, 0),
(194958, 45087, 33, 0, 1, 1, 0, 0, 0),
(194958, 45089, 5, 0, -45089, 1, 0, 0, 0),
(194958, 45643, 100, 0, -45643, 1, 0, 0, 0),
(194958, 45816, -100, 0, 1, 1, 0, 0, 0),
(194958, 47241, 100, 0, 1, 1, 0, 0, 0),
(194958, 45489, 0, 1, 1, 1, 0, 0, 0),
(194958, 45490, 0, 1, 1, 1, 0, 0, 0),
(194958, 45491, 0, 1, 1, 1, 0, 0, 0),
(194958, 45492, 0, 1, 1, 1, 0, 0, 0),
(194958, 45493, 0, 1, 1, 1, 0, 0, 0),
(194958, 45494, 0, 2, 1, 1, 0, 0, 0),
(194958, 45495, 0, 2, 1, 1, 0, 0, 0),
(194958, 45496, 0, 2, 1, 1, 0, 0, 0),
(194958, 45497, 0, 2, 1, 1, 0, 0, 0),
(194958, 45663, 0, 2, 1, 1, 0, 0, 0);

-- Thorim
UPDATE creature_template SET ScriptName = "boss_thorim" WHERE entry = 32865;
UPDATE creature_template SET ScriptName = "boss_runic_colossus" WHERE entry = 32872;
UPDATE creature_template SET ScriptName = "boss_ancient_rune_giant" WHERE entry = 32873;
UPDATE creature_template SET ScriptName = "npc_lightning_orb" WHERE entry = 33138;
UPDATE creature_template SET ScriptName = "mob_thorim_trap_bunny" WHERE entry in (33725, 33054);
UPDATE creature_template SET ScriptName = "mob_thorim_preadds" WHERE entry in (32885, 32883, 32907, 32908, 32882);
update creature set spawnMask = 3 where id = 32873;
UPDATE creature_template SET ScriptName = "npc_sif" WHERE entry = 33196;
update `gameobject` set `position_y` = -286.67, `position_z` = 419.50 where `id` in (194312, 194313, 194314, 194315);
UPDATE gameobject_template SET flags = 6553632, ScriptName="go_thorim_lever" WHERE entry = 194264;
-- adds
UPDATE creature_template SET ScriptName = "mob_dark_rune_acolyte" WHERE entry = 33110;
UPDATE creature_template SET ScriptName = "mob_dark_rune_champion" WHERE entry = 32876;
UPDATE creature_template SET ScriptName = "mob_dark_rune_commoner" WHERE entry = 32904;
UPDATE creature_template SET ScriptName = "mob_dark_rune_warbringer" WHERE entry = 32877;
UPDATE creature_template SET ScriptName = "mob_dark_rune_ring_guard" WHERE entry = 32874;
UPDATE creature_template SET ScriptName = "mob_dark_rune_honor_guard" WHERE entry = 33125;
update `creature` set `phaseMask` = 128 where `id` in (32907, 32883); 	-- horde soldiers: phase 128 for aly: 65535
update `creature` set `phaseMask` = 64 where `id` in (32885, 32908);	-- alliance soldiers: phase 64 for horde
-- reset pos to some creatures 
update creature set spawnMask = 0 where guid in (129413, 129412, 129856, 129857);
update `creature` set `position_y` = -437.73 where `guid` = 129860;
update `creature` set `position_y` = -437.73 where `guid` = 129861;
update `creature` set `position_y` = -434.64 where `guid` = 129862;
update `creature` set `position_y` = -434.64 where `guid` = 129863;
update `creature` set `position_y` = -434.64 where `guid` = 129391;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62565', '1', '32865');

-- Vezax
UPDATE creature_template SET unit_flags = 0, ScriptName = "boss_vezax" WHERE entry = 33271;
UPDATE creature_template SET MinHealth = 23009250, MaxHealth = 23009250, ScriptName = "boss_vezax" WHERE entry = 33449;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` in (33271, 33449);
UPDATE creature_template SET ScriptName = "mob_saronite_animus" WHERE entry = 33524;
UPDATE creature_template SET ScriptName = "mob_saronite_vapor", movementType = 1  WHERE entry = 33488;

-- Yogg
UPDATE creature_template SET ScriptName = "boss_yogg_saron" WHERE entry = 33288;
update creature_template set `RegenHealth`= 0, `flags_extra` = 1,`type_flags` = 108, ScriptName = "boss_sara" where entry = 33134;
update creature set spawnMask = 3, MovementType = 0 where id = 33134;
update creature_template set `RegenHealth`= 0, ScriptName = "boss_brain_of_yogg_saron" where entry = 33890;
update creature set `spawntimesecs` = 604800 where `id` = 33134;
UPDATE creature_template SET ScriptName = "mob_corruptor_tentacle" WHERE entry = 33985;
UPDATE creature_template SET ScriptName = "mob_constrictor_tentacle" WHERE entry = 33983;
UPDATE creature_template SET MinHealth = 40000, MaxHealth = 40000, minLevel = 80, maxLevel = 80, ScriptName = "mob_vision_tentacle" WHERE entry = 33943;
UPDATE creature_template SET MinHealth = 400000, MaxHealth = 400000, ScriptName = "mob_crusher_tentacle" WHERE entry = 33966;
UPDATE creature_template SET MinHealth = 220000, MaxHealth = 220000, ScriptName = "mob_guardian_of_yogg_saron" WHERE entry = 33136;
UPDATE creature_template SET ScriptName = "mob_immortal_guardian" WHERE entry = 33988;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, ScriptName = "mob_death_orb" WHERE entry = 33882;
UPDATE creature_template SET ScriptName = "mob_sanity_well" WHERE entry = 33991;
UPDATE creature_template SET scriptname='mob_madness_portal' WHERE `entry`=34072;
UPDATE creature_template SET scriptname='mob_laughing_skull' WHERE `entry`=33990;
UPDATE creature_template SET scriptname='mob_ominous_cloud' WHERE `entry`=33292;
update creature set spawnMask = 3 where id = 33292;
-- spells
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('63886', '1', '33882');
-- Keepers
UPDATE creature_template SET ScriptName = "keeper_hodir" WHERE entry = 33213;
UPDATE creature_template SET ScriptName = "keeper_freya" WHERE entry = 33241;
UPDATE creature_template SET ScriptName = "keeper_thorim" WHERE entry = 33242;
UPDATE creature_template SET ScriptName = "keeper_mimiron" WHERE entry = 33244;
-- insert doors & portals into the brain room
delete from gameobject where id in (194635);
insert into gameobject VALUES (110000,194635,603,3,65535,2022.490,-25.389,261.961,0,0,0,0,0,604800,0,1);
delete from gameobject where guid = 110001;
insert into gameobject VALUES (110001,194462,603,3,65535,2104.555, -25.635,242.646,0,0,0,0,0,604800,100,1);


-- Algalon
UPDATE creature_template SET ScriptName = "boss_algalon" WHERE entry = 32871;
UPDATE creature_template SET ScriptName = "mob_collapsing_star" WHERE entry = 32955;
UPDATE creature_template SET ScriptName = "mob_living_constellation" WHERE entry = 33052;
UPDATE creature_template SET ScriptName = "mob_black_hole" WHERE entry = 32953;
UPDATE creature_template SET ScriptName = "mob_cosmic_smash_target" WHERE entry in (33105, 33104);
update creature_template set minhealth = 39099, maxhealth = 39099 where entry = 33089;
UPDATE gameobject_template SET flags= 6553632, ScriptName="go_celestial_acces" WHERE entry in (194628, 194752);

-- Teleporter
UPDATE gameobject_template SET ScriptName="ulduar_teleporter" WHERE entry = 194569;
-- Destination Locations for Ulduar Teleporter spells
REPLACE INTO spell_target_position VALUES
(64014, 603, -706.122, -92.6024, 429.876, 0),      /* Expedition Base Camp */
(64032, 603, 131.248, -35.3802, 409.804, 0),       /* Formation Grounds */
(64028, 603, 553.233, -12.3247, 409.679, 0),       /* Colossal Forge */
(64031, 603, 926.292, -11.4635, 418.595, 3.14159), /* Scrapyard */
(64030, 603, 1498.09, -24.246, 420.967, 0),        /* Antechamber of Ulduar */
(64029, 603, 1859.45, -24.1, 448.9, 0),            /* Shattered Walkway */
(64024, 603, 2086.27, -24.3134, 421.239, 0),       /* Conservatory of Life */
(65061, 603, 2517.31, 2568.87, 412.299, 0),        /* Spark of Imagination */
(65042, 603, 1854.73, -11.637, 334.575, 0);        /* Prison of Yogg-Saron */

-- Keepers
-- Keepers images
UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = "hodir_image" WHERE entry = 33411;
UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = "freya_image" WHERE entry = 33410;
UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = "thorim_image" WHERE entry = 33413;
UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = "mimiron_image" WHERE entry = 33412;

-- Doors 
UPDATE gameobject_template SET faction = 114 WHERE entry in (194553, 194554, 194556, 194148, 194634, 194635, 194905, 194441,
194442, 194416, 194774, 194775, 194776, 194560, 194557, 194558, 194750, 194910, 194559, 194635, 194636, 194637, 194631, 194255, 194630, 194767);
UPDATE gameobject_template SET faction = 114, `flags` = 4 WHERE entry in (192075, 194173); -- snowdrifts
-- consoles
UPDATE gameobject_template SET faction = 0 WHERE entry in (194555, 194628);

-- loot chests
UPDATE gameobject_template SET faction = 0, data15 = 1 WHERE entry in (195046, 195047, 194307, 194308, 194200, 194201, 194312, 194313, 194314, 194315, 194821,
194822, 194823, 194324, 194325, 194326, 194327, 194328, 194329, 194330, 194331, 194789, 194956, 194957, 194958);
update gameobject set spawntimesecs = -604800 where id in (195046, 195047, 194307, 194308, 194200, 194201, 194312, 194313, 194314, 194315, 194821,
194822, 194823, 194324, 194325, 194326, 194327, 194328, 194329, 194330, 194331, 194789, 194956);
