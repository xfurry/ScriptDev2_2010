/* FORGE OF SOULS */
UPDATE creature_template SET ScriptName='boss_bronjahm' WHERE entry=36497;
UPDATE creature_template SET ScriptName='npc_corrupted_soul_fragment' WHERE entry=36535;
UPDATE creature_template SET ScriptName='boss_devourer_of_souls' WHERE entry=36502;
UPDATE creature_template SET ScriptName='npc_silvana_and_jaina_fos_intro' WHERE entry in (37596, 37597);
UPDATE instance_template SET ScriptName='instance_forge_of_souls' WHERE map=632;

/* PIT OF SARON */
-- Areatrigger:
delete from scripted_areatrigger where entry = 5578;
insert into scripted_areatrigger values (5578, 'at_tyrannus');
-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_Garfrost' WHERE `entry`=36494;
UPDATE `creature_template` SET `ScriptName`='boss_Ick' WHERE `entry`=36476;
UPDATE `creature_template` SET `ScriptName`='boss_Krick' WHERE `entry`=36477;
UPDATE `creature_template` SET `ScriptName`='mob_explosive_orb' WHERE `entry`=36610;
UPDATE `creature_template` SET `ScriptName`='boss_Tyrannus' WHERE `entry`=36658;
UPDATE `creature_template` SET `ScriptName`='boss_Rimefang' WHERE `entry`=36661;
UPDATE `creature_template` SET `ScriptName`='npc_colapsing_icicle' WHERE `entry`=36847;
-- npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_start' WHERE `entry` in (36993, 36990);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_end' WHERE `entry` in (38188, 38189);
UPDATE `creature_template` SET `ScriptName`='npc_martin_gorkun' WHERE `entry` in (37591, 37592);
update creature set spawnMask = 0 where id in (37591, 37592, 38188, 38189, 36731);
update creature set spawnMask = 0 where guid in (123745, 123584, 123548, 123585);
UPDATE gameobject_template SET faction = 114, data10 = 1 WHERE entry = 196485;
DELETE FROM `creature` WHERE `id`=36658;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(93924, 36658, 658, 3, 1, 0, 0, 1018.38, 167.25, 651.871, 5.2709, 14400, 0, 0, 653828, 0, 0, 0);
-- spells
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69347', '1', '36794');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70521', '1', '37581');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70521', '1', '37580');

/* HALLS OF REFLECTION */
-- start npcs
-- jaina
DELETE FROM `creature` WHERE `id`=37221;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(117106, 37221, 668, 3, 128, 0, 0, 5266.78, 1953.42, 707.697, 0.740877, 7200, 0, 0, 5040000, 881400, 0, 0);
-- sylvanas
DELETE FROM `creature` WHERE `id`=37223;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(117460, 37223, 668, 3, 64, 0, 0, 5266.78, 1953.42, 707.697, 0.740877, 7200, 0, 0, 6972500, 85160, 0, 0);
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69708', '1', '37226');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70194', '1', '37226');
-- trash
UPDATE `creature_template` SET `ScriptName`='mob_frostsworn_general' WHERE `entry`=36723;
UPDATE `creature_template` SET `ScriptName`='mob_lichKing_minion' WHERE `entry` in (36940, 36941, 37069);
UPDATE `creature_template` SET `ScriptName`='mob_hallsOfReflectionSoul' WHERE `entry` in (38176, 38175, 38172, 38567, 38177, 38173);
-- boss
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
UPDATE `creature_template` SET `ScriptName`='boss_falric' WHERE `entry`=38112;
-- npc
update creature set position_x = 5550.833984, position_y = 2260.596191, position_z = 733.011414, orientation = 3.934108 where id = 37226;
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_start' WHERE `entry` in (37221, 37223);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_end' WHERE `entry` in (36955, 37554);
UPDATE `creature_template` SET `ScriptName`='npc_lich_king_hor_start' WHERE `entry`=36954;
UPDATE `creature_template` SET `ScriptName`='boss_lich_king_hor_end' WHERE `entry`=37226;
update creature set spawnMask = 0 where guid in (135387, 135386);
update creature set spawnMask = 0 where id in (36940, 36941, 37069);
update creature set spawnMask = 0 where id in (38176, 38175, 38172, 38567, 38177, 38173); -- Temporary until script fulll implemented -> spawnMask should be 3 and the npc invisible until the event starts

update instance_template set scriptName = 'instance_halls_of_reflection' where map = 668;
update instance_template set scriptName = 'instance_pit_of_saron' where map = 658;
update instance_template set scriptName = 'instance_forge_of_souls' where map = 632;

update gameobject set spawntimesecs = -604800 where id in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 0 WHERE entry in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 114 WHERE entry in (201848, 201385, 197342, 197343, 201976, 197341, 202236, 202302, 201709);

#UPDATE `creature_template` SET `ScriptName`='generic_creature' WHERE `entry` in ();
delete from game_tele where id = 1500;
insert into game_tele values (1500, 5634.508, 2042.124, 798.270, 4.71, 571, 'FrozenHalls');
delete from game_tele where id = 1501;
insert into game_tele values (1501, 8516.319, 634.558, 547.393, 1.62, 571, 'CrusadersColliseum');
