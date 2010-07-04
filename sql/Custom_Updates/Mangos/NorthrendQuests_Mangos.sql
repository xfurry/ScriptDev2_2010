#Quest Name:Frankly, It Makes No Sense...
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 21462 WHERE `entry` = 10672;
#Quest Name:Let Them Eat Crow
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 23943 WHERE `entry` = 11227;
#Quest Name:The Spider God
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 7272 WHERE `entry` = 2936;
#Quest Name:Enraged Spirits of Fire and Earth
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 21050, `ReqCreatureOrGOId2` = 21060 WHERE `entry` = 10458;
#Quest Name:Enraged Spirits of Water
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 21059 WHERE `entry` = 10480;
#Quest Name:Enraged Spirits of Air
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 21060 WHERE `entry` = 10481;
#Quest Name:The Armor's Secrets
UPDATE `creature_template` SET `npcflag` = 1 WHERE `entry` = 30190;
UPDATE `creature_template` SET `gossip_menu_id` = 1 WHERE `entry` = 30190;
#Quest Name:The Heart of the Storm
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = -192181 WHERE `entry` = 12998;
#Quest Name:The Reckoning
UPDATE `quest_template` SET `SpecialFlags` = 0, `ObjectiveText1` = 'Meet Thorim on the Bridge', `ReqSourceId2` = 1 WHERE `entry` = 13047;
#Quest Name:The Black Knight's Orders
UPDATE `creature_template` SET `gossip_menu_id` = 1, `npcflag` = 1 WHERE `entry` = 33513;
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 33513, `ObjectiveText1` = 'Black Knight''s Gryphon taken' WHERE `entry` = 13663;
#Quest Name:You've Really Done It This Time, Kul
UPDATE `quest_template` SET `ObjectiveText1` = 'Kul the Reckless rescued', `ObjectiveText2` = 'Captive Aspirant rescued', `ReqCreatureOrGOId1` = 34956, `ReqSpellCast1` = 0 WHERE `entry` = 14096;
UPDATE `creature_template` SET `gossip_menu_id` = 1 WHERE `entry` = 34956;
#Quest Name:The Art of Being a Water Terror
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = 30243 WHERE `entry` = 13091;
#Quest Name:Killing Two Scourge With One Skeleton
UPDATE `quest_template` SET `ObjectiveText1` = 'Chained Abominations burned', `ReqCreatureOrGOId1` = 30689 WHERE `entry` = 13144;
#Quest Name:The Hunter and the Prince
UPDATE `quest_template` SET `ObjectiveText1` = 'The Prince''s Destiny', `ReqCreatureOrGOId1` = 32588 WHERE `entry` = 13400;
#Quest Name:  The Darkness Beneath
UPDATE `quest_template` SET `ReqSpellCast1` = 48218, `ReqSpellCast2` = 48218, `ReqSpellCast3` = 48218 WHERE `entry` = 12213;
#Quest Name:  ...Or Maybe We Don't
UPDATE `quest_template` SET `ObjectiveText1` = 'Charge Level.', `ReqCreatureOrGOId1` = 26407 WHERE `entry` = 12198;
#Quest Name:  See You On The Other Side
UPDATE `creature` SET `position_x` = 4759.08, `position_y` = -4861.95, `position_z` = 26.0434, `orientation` = 2.80601 WHERE `guid` = 120463;
#Quest Name:  Gambling Debt
DELETE FROM `npc_vendor` WHERE (`entry`=24539) AND (`item`=34115);
INSERT INTO `npc_vendor` (`entry`, `item`, `maxcount`, `incrtime`, `ExtendedCost`) VALUES (24539, 34115, 0, 0, 0);
#Quest Name:  A Carver and a Croaker
UPDATE `quest_template` SET `ReqItemId1` = 0, `ReqItemCount1` = 0, `ReqCreatureOrGOId1` = 26503, `ReqCreatureOrGOCount1` = 1 WHERE `entry` = 11476;
#Quest Name:  Reconnaissance Flight
UPDATE `creature_template` SET `gossip_menu_id` = 1, `npcflag` = 1 WHERE `entry` = 24653;
UPDATE `quest_template` SET `ObjectiveText1` = 'Take the plane', `ReqCreatureOrGOId1` = 24653, `ReqCreatureOrGOCount1` = 1, `SpecialFlags` = 0 WHERE `entry` = 12671;
#Quest Name: The Demon Seed
UPDATE `gameobject_template` SET `data0` = 0, `flags` = 0 WHERE `entry` = 3525;
UPDATE `quest_template` SET `SrcItemId` = 4986 WHERE `entry` = 924;
#Quest Name:Borrowed Technology
UPDATE `quest_template` SET `ReqItemId1` = 43616, `ReqItemId2` = 43610, `ReqItemId3` = 43609, `ReqItemId4` = 44048, `ReqItemCount1` = 3, `ReqItemCount2` = 3, `ReqItemCount3` = 3, `ReqItemCount4` = 1, `ReqCreatureOrGOId1` = 0, `ReqCreatureOrGOCount1` = 0 WHERE `entry` = 13291;
#Quest Name: An Improper Burial
UPDATE `quest_template` SET `ReqSpellCast1` = 39189, `ReqSpellCast2` = 39189 WHERE `entry` = 10913;
#Quest Name: The Big Bone Worm
DELETE FROM `creature` WHERE `id`=22038;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x` ,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(200054, 22038, 530, 1, 1, 0, 0, -3751.23, 4601.95, -17.7454, 1.94797, 25, 5, 0, 35484, 0, 0, 0);