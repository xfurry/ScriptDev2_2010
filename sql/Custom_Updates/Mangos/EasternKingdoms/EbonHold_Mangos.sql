-- Into the Realm of Shadows
UPDATE `creature_template` SET `ScriptName`='npc_salanar_the_horseman',ainame="" WHERE `entry`='28653';
UPDATE `creature_template` SET `ScriptName`='mob_dark_rider_of_acherus',ainame="" WHERE `entry`='28768';

-- The Gift That Keeps On Giving
UPDATE `creature_template` SET `ScriptName`='mob_scarlet_miner',ainame="" WHERE `entry`='28822';

-- How To Win Friends And Influence Enemies
UPDATE `creature_template` SET `ScriptName`='npc_crusade_persuaded',ainame="" WHERE `entry` IN (28939,28940,28610);

-- An End To All Things...
UPDATE creature_template SET InhabitType = 3 WHERE entry = 28670;

-- Ambush At The Overlook
DELETE FROM `creature` WHERE `id` = 28957;
INSERT INTO `creature` (`id`, `map`, `phaseMask`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES 
(28957, 609, 4, 1, 0, 0, 1416.466, -6017.807, 116.357, 1, 360, 0, 0, 1, 0, 0, 1);
UPDATE `creature_template` SET `ScriptName`='mob_scarlet_courier_controller',flags_extra=130,ainame="" WHERE `entry`='28957';
UPDATE `creature_template` SET `ScriptName`='mob_scarlet_courier',ainame="" WHERE `entry`='29076';

-- Bloody Breakout
UPDATE `creature_template` SET `ScriptName`='mob_high_inquisitor_valroth',minmana=6489,maxmana=6489,unit_flags=32768,ainame="" WHERE `entry`='29001';
UPDATE `creature_template` SET `AIName`='EventAI',minmana=1020,maxmana=1058,unit_flags=32768 WHERE (`entry`='29007');
DELETE FROM creature_ai_scripts WHERE id BETWEEN 90030 AND 90033;
INSERT INTO `creature_ai_scripts` VALUES ('90030', '29007', '0', '0', '100', '1', '1000', '4000', '1000', '4000', '11', '15498', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Crimson Acolyte - Holy Smite');
INSERT INTO `creature_ai_scripts` VALUES ('90031', '29007', '0', '0', '100', '1', '1000', '10000', '20000', '21000', '11', '34809', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Crimson Acolyte - Holy Fury');
INSERT INTO `creature_ai_scripts` VALUES ('90032', '29007', '0', '0', '100', '1', '1000', '5000', '1000', '5000', '11', '19725', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Crimson Acolyte - Turn Undead');
INSERT INTO `creature_ai_scripts` VALUES ('90033', '29007', '4', '0', '100', '0', '0', '0', '0', '0', '11', '15498', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Crimson Acolyte aggro');

-- The Light of Dawn (to do)
UPDATE `creature_template` SET `ScriptName`='npc_highlord_darion_mograine',ainame="" WHERE `entry`='29173';
UPDATE `creature_template` SET `ScriptName`='npc_the_lich_king_tirion_dawn',ainame="" WHERE `entry` in (29183,29175);
UPDATE creature_template SET AIName='EventAI',unit_flags=0,type_flags=8 WHERE entry IN (29199,29204,29200,29174,29182,29186,29190,29219,29206,29176,29178,29179,29180,29177,29181);
DELETE FROM `spell_script_target` WHERE `entry` in (53658, 53679, 53701, 53705, 53706, 53677, 53685);
-- insert into spell_script_target values (53658, 1, 29173);
insert into spell_script_target values (53679, 1, 29183);
insert into spell_script_target values (53701, 1, 29175);
insert into spell_script_target values (53705, 1, 29183);
insert into spell_script_target values (53706, 1, 29183);
insert into spell_script_target values (53677, 1, 29227);
insert into spell_script_target values (53685, 1, 29175);

DELETE FROM `gameobject` WHERE `id` IN (191330);
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
('191330','609','1','128','2283.896','-5287.914','83.066','1','0.0','0.0','0.0','1.0','180','0','1');

DELETE FROM creature_ai_scripts WHERE id BETWEEN 90051 AND 90084;
INSERT INTO `creature_ai_scripts` VALUES ('90051', '29199', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52374', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Koltira Deathweaver - SPELL_BLOOD_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90052', '29199', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '49576', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Koltira Deathweaver - SPELL_DEATH_GRIP');
INSERT INTO `creature_ai_scripts` VALUES ('90053', '29199', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52372', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Koltira Deathweaver - SPELL_ICY_TOUCH2');
INSERT INTO `creature_ai_scripts` VALUES ('90054', '29199', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '50668', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Koltira Deathweaver - SPELL_PLAGUE_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90055', '29204', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52374', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Orbaz Bloodbane - SPELL_BLOOD_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90056', '29204', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '49576', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Orbaz Bloodbane - SPELL_DEATH_GRIP');
INSERT INTO `creature_ai_scripts` VALUES ('90057', '29204', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52372', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Orbaz Bloodbane - SPELL_ICY_TOUCH2');
INSERT INTO `creature_ai_scripts` VALUES ('90058', '29204', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '50668', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Orbaz Bloodbane - SPELL_PLAGUE_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90059', '29200', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52374', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Thassarian - SPELL_BLOOD_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90060', '29200', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '49576', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Thassarian - SPELL_DEATH_GRIP');
INSERT INTO `creature_ai_scripts` VALUES ('90061', '29200', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '52372', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Thassarian - SPELL_ICY_TOUCH2');
INSERT INTO `creature_ai_scripts` VALUES ('90062', '29200', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '50668', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Thassarian - SPELL_PLAGUE_STRIKE1');
INSERT INTO `creature_ai_scripts` VALUES ('90063', '29174', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '29427', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_LIGHT1');
INSERT INTO `creature_ai_scripts` VALUES ('90064', '29174', '4', '0', '100', '0', '0', '0', '0', '0', '11', '53625', '1', '5', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light aggro');
INSERT INTO `creature_ai_scripts` VALUES ('90065', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53625', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HEROIC_LEAP');
INSERT INTO `creature_ai_scripts` VALUES ('90066', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53643', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_STRIKE');
INSERT INTO `creature_ai_scripts` VALUES ('90067', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53638', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_WRATH');
INSERT INTO `creature_ai_scripts` VALUES ('90068', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53629', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_UPPERCUT');
INSERT INTO `creature_ai_scripts` VALUES ('90069', '29182', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '33642', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rimblat Earthshatter - SPELL_CHAIN_HEAL');
INSERT INTO `creature_ai_scripts` VALUES ('90070', '29182', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53630', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rimblat Earthshatter - SPELL_THUNDER');
INSERT INTO `creature_ai_scripts` VALUES ('90071', '29186', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53633', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rampaging Abomination - SPELL_CLEAVE1');
INSERT INTO `creature_ai_scripts` VALUES ('90072', '29186', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '50335', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rampaging Abomination - SPELL_SCOURGE_HOOK');
INSERT INTO `creature_ai_scripts` VALUES ('90073', '29190', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53634', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_SCOURGE_HOOK');
INSERT INTO `creature_ai_scripts` VALUES ('90074', '29190', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '36706', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_THUNDERCLAP');
INSERT INTO `creature_ai_scripts` VALUES ('90075', '29190', '0', '0', '100', '3', '5000', '10000', '5000', '10000', '11', '53627', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_THUNDERCLAP');
INSERT INTO `creature_ai_scripts` VALUES ('90076', '29219', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53632', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Volatile Ghoul - SPELL_GHOULPLOSION');
INSERT INTO `creature_ai_scripts` VALUES ('90077', '29206', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53631', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Warrior of the Frozen Wastes - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90078', '29176', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53631', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Korfax - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90079', '29176', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53625', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Korfax - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90080', '29177', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '37979', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Commander Eligor Dawnbringer - SPELL_HOLY_LIGHT2');
INSERT INTO `creature_ai_scripts` VALUES ('90081', '29181', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '20664', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_REJUVENATION');
INSERT INTO `creature_ai_scripts` VALUES ('90082', '29181', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '25817', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_TRANQUILITY');
INSERT INTO `creature_ai_scripts` VALUES ('90083', '29181', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '20678', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_STARFALL');
INSERT INTO `creature_ai_scripts` VALUES ('90084', '29181', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '21807', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_WRATH');
