REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (30996, 0, 0, 0, 0, 0, 11686, 0, 11686, 0, 'CoT Stratholme - Crates KC Bunny', '', '', 1, 1, 1, 1, 0, 0, 17, 35, 35, 0, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '');
UPDATE `quest_template` SET `ReqCreatureOrGOCount1` = '5' WHERE entry=13149;
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (20562, 0, 0, 0, 0, 0, 11686, 0, 11686, 0, 'Invisible Stalker (Scale x5)', '', '', 1, 1, 42, 42, 0, 0, 17, 35, 35, 0, 5, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554944, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '');

UPDATE `creature` SET `spawntimesecs`='36000' WHERE `id` IN (31127, 31126, 28167, 28169);
replace into `spell_script_target` (`entry`, `type`, `targetEntry`) values('58825','1','27733');

UPDATE `creature_template` SET `faction_A`='35', `faction_H`='35' WHERE `entry` IN (31127, 31126, 28167, 28169);
UPDATE `creature_template` SET `faction_A`='35', `faction_H`='35' WHERE `entry` IN (10002, 10003, 10004, 10005);
UPDATE `creature_template` SET `minhealth`='15000', `maxhealth`='15000' WHERE `entry` IN (27737);
UPDATE `creature_template` SET `minhealth`='25000', `maxhealth`='25000' WHERE `entry` IN (31208);

replace into `spell_area` (`spell`, `area`, `quest_start`, `quest_start_active`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`) values('35481','4100','0','0','0','0','0','1','1');
replace into `spell_area` (`spell`, `area`, `quest_start`, `quest_start_active`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`) values('35480','4100','0','0','0','0','0','0','1');

UPDATE `instance_template` SET `script` = 'instance_culling_of_stratholme' WHERE map=595;
update `creature_template` set `AIName`='', `Scriptname`='npc_mike' where `entry` in (30571);
update `creature_template` set npcflag = 3, `AIName`='', `Scriptname`='npc_chromi_start' where `entry` in (26527);
update `creature_template` set `AIName`='', `Scriptname`='npc_roger' where `entry` in (27903);
update `creature_template` set `AIName`='', `Scriptname`='npc_morigan' where `entry` in (27877);
update `creature_template` set `AIName`='', `Scriptname`='npc_stratholme_crates' where `entry` in (30996);
update `creature_template` set `AIName`='', `Scriptname`='npc_jena' where `entry` in (27885);
update `creature_template` set `AIName`='', `Scriptname`='npc_malcolm' where `entry` in (27891);
update `creature_template` set `AIName`='', `Scriptname`='npc_bartleby_cs' where `entry` in (27907);
update `creature_template` set npcflag = 3, `AIName`='', `Scriptname`='npc_chromi_middle' where `entry` in (27915);
update `creature_template` set `AIName`='', `Scriptname`='npc_uther' where `entry` in (26528);
update `creature_template` set `AIName`='', `Scriptname`='npc_arthas' where `entry` in (26499);
update `creature_template` set `AIName`='', `Scriptname`='' where `entry` in (26533);
update `creature_template` set `AIName`='', `Scriptname`='npc_arthas_priest' where `entry` in (27747);
update `creature_template` set `AIName`='', `Scriptname`='npc_arthas_marine' where `entry` in (27745);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_dark_conversion' WHERE `entry` IN (31127, 31126, 28167, 28169);
update `creature_template` set `AIName`='', `Scriptname`='npc_cs_gnoul' where `entry` in (28249);
update `creature_template` set `AIName`='', `Scriptname`='npc_cs_necromancer' where `entry` in (28200);
update `creature_template` set `AIName`='', `Scriptname`='npc_cs_field' where `entry` in (27734);
update `creature_template` set `AIName`='', `Scriptname`='npc_cs_acolyte' where `entry` in (27731);
update `creature_template` set `AIName`='', `Scriptname`='npc_cs_butcher' where `entry` in (27736);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_meathook' WHERE `entry` IN (26529);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_salramm' WHERE `entry` IN (26530);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_salramm_gnoul' WHERE `entry` IN (27733);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_lord_epoch' WHERE `entry` IN (26532);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_malganis' WHERE `entry` IN (26533);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_time_riftCS' WHERE `entry` IN (28409);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_infinite_corruptor' WHERE `entry` IN (32273);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='mob_risen_zombie' WHERE `entry` IN (27737);
delete from creature_loot_template where item = 43951 and entry = 32273;
insert into creature_loot_template values (32273, 43951, 100, 0, 1, 1, 0, 0, 0);

DELETE FROM `npc_gossip` WHERE (`npc_guid`=115298);

-- loot chests
UPDATE gameobject_template SET faction = 0, data15 = 1, flags = 0 WHERE entry in (190663, 193597);
update gameobject set spawntimesecs = -604800 where id in (190663, 193597);

REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('58825', '1', '27733');

DELETE FROM `creature` WHERE `id` in (27731);
DELETE FROM `creature` WHERE `id` in (27734);
DELETE FROM `creature` WHERE `id` in (28249);
DELETE FROM `creature` WHERE `id` in (27736);

DELETE FROM `creature` WHERE `id` in (27915);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9777887, 27915, 595, 3, 1, 0, 0, 1812.49, 1284.81, 142.248, 4.03364, 300, 0, 0, 17010, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (30571);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9777888, 30571, 595, 3, 1, 27139, 0, 1549.61, 575.545, 100.052, 4.16253, 360, 0, 0, 8982, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (26499);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (138007, 26499, 595, 3, 1, 0, 1613, 1920.87, 1287.12, 142.935, 6.25562, 43200, 0, 0, 44100, 7988, 0, 0);

DELETE FROM `creature` WHERE `id` in (26497);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9337048, 26497, 595, 3, 1, 0, 1221, 1896.39, 1292.91, 143.711, 0.016332, 25, 5, 0, 100800, 88140, 0, 0);

DELETE FROM `creature` WHERE `id` in (26528);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9777889, 26528, 595, 3, 1, 0, 1819, 1761.42, 1285.75, 139.945, 4.93874, 25, 5, 0, 126000, 59910, 0, 0);

DELETE FROM `creature` WHERE `id` in (27891);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9797890, 27891, 595, 3, 1, 0, 0, 1603.38, 805.988, 123.272, 1.90688, 25, 5, 0, 8982, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (27892);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9797891, 27892, 595, 3, 1, 0, 0, 1602.3, 809.385, 123.454, 5.02884, 25, 5, 0, 8982, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (27884);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9777892, 27884, 595, 3, 1, 24901, 0, 1635.81, 723.155, 113.562, 3.21455, 360, 0, 0, 8982, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (32273);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9798216, 32273, 595, 2, 1, 0, 1839, 2330.93, 1275.59, 132.848, 3.60489, 25, 5, 0, 417911, 0, 0, 0);

DELETE FROM `creature` WHERE `id` in (28439);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (9798218, 28439, 595, 2, 1, 0, 0, 2336.56, 1277.9, 132.885, 3.47923, 25, 5, 0, 1, 0, 0, 0);