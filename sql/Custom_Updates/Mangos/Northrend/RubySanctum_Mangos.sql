-- Ruby sanctum
UPDATE `instance_template` SET `scriptname`='instance_ruby_sanctum' WHERE `map`= 724;
-- Halion
UPDATE `creature_template` SET `ScriptName`='boss_halion_real', unit_flags = 0 WHERE `entry`= 39863;
UPDATE `creature_template` SET `ScriptName`='boss_halion_twilight', unit_flags = 0 WHERE `entry`= 40142;
UPDATE `creature_template` SET `ScriptName`='mob_halion_control' WHERE `entry` = 40146;
UPDATE `creature_template` SET `ScriptName`='mob_halion_meteor' WHERE `entry` = 40029;
UPDATE gameobject_template SET ScriptName="go_halion_portal" WHERE entry in (202794, 202795, 202796);
-- insert 3 portals
DELETE FROM `gameobject` WHERE `id` in (202794, 202795, 202796);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800500, 202794, 724, 15, 1, 3188.023, 538.893, 72.930, 3.108, 0, 0, 0, 0, -604800, 100, 1);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800501, 202795, 724, 15, 32, 3134.239, 505.739, 72.871, 0.819, 0, 0, 0, 0, -604800, 100, 1);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800502, 202796, 724, 15, 32, 3131.327, 568.850, 72.871, 5.296, 0, 0, 0, 0, -604800, 100, 1);
-- UPDATE `creature_template` SET `ScriptName`='mob_halion_flame' WHERE `entry` IN (36673,36674);
UPDATE `creature_template` SET `ScriptName`='mob_halion_orb', `AIName` ='' WHERE `entry` IN (40144, 40145);

-- Baltharus
UPDATE `creature_template` SET `ScriptName`='boss_baltharus' WHERE `entry` = 39751;
UPDATE `creature_template` SET `ScriptName`='mob_baltharus_clone' WHERE `entry` = 39899;
-- UPDATE `creature` SET `position_x` = '3152.415771',`position_y` = '396.458405',`position_z` = '85.755531',`orientation` = '4.415168' WHERE `id` = 39751;
DELETE FROM `creature_movement` WHERE `id`=93593;
-- INSERT INTO `creature_movement` (`id`,`point`,`position_x`,`position_y`,`position_z`,`waittime`,`script_id`,`textid1`,`textid2`,`textid3`,`textid4`,`textid5`,`emote`,`spell`,`wpguid`,`orientation`,`model1`,`model2`) VALUES
-- (93593, 1, 3153.06, 389.486, 86.2596, 30000, 0, 0, 0, 0, 0, 0, 0, 76221, 0, 0, 0, 0);
-- INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
-- (39900, 0, 0, 0, 0, 0, 11686, 0, 11686, 0, 'Baltharus Target Dummy', '', NULL, 0, 1, 1, 25, 27, 0, 0, 17, 35, 35, 0, 1, 1.14286, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 258, '');
-- UPDATE `creature_template` SET `ScriptName`='', `AIName` ='' WHERE `entry` = 39900;

-- Zarithrian
UPDATE `creature_template` SET `ScriptName`='boss_zarithrian' WHERE `entry` = 39746;
UPDATE `creature` SET `position_x` = '3008.552734',`position_y` = '530.471680',`position_z` = '89.195290',`orientation` = '6.16' WHERE `id` = 39746;
UPDATE `creature_template` SET `ScriptName`='mob_flamecaller_ruby' WHERE `entry` = 39814;

-- Saviana Ragefire
UPDATE `creature_template` SET `ScriptName`='boss_saviana' WHERE `entry`=39747;
-- DELETE FROM `spell_script_target` WHERE `entry` IN (74455);
-- INSERT INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES 
-- ('74455', '1', '39747');

-- Xerestrasza
UPDATE `creature_template` SET `ScriptName`='mob_xerestrasza' WHERE `entry`=40429;

-- Trash mobs
UPDATE `creature_template` SET `ScriptName`='mob_charscale_elite' WHERE `entry` = 40421;
UPDATE `creature_template` SET `ScriptName`='mob_charscale_commander' WHERE `entry` = 40423;
UPDATE `creature_template` SET `ScriptName`='mob_charscale_assaulter' WHERE `entry` = 40419;
UPDATE `creature_template` SET `ScriptName`='mob_charscale_invoker' WHERE `entry` = 40417;
