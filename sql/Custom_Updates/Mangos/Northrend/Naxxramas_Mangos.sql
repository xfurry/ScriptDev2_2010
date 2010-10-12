/* NAXXRAMAS */
UPDATE creature_template SET ScriptName = "npc_stalagg" WHERE entry= 15929;
UPDATE creature_template SET ScriptName = "npc_feugen" WHERE entry= 15930;
UPDATE creature_template SET ScriptName = "boss_thaddius"  WHERE entry= 15928;
UPDATE creature_template SET ScriptName = "npc_tesla_coil" WHERE entry = 16218;
-- spawn two dummies for teslas
UPDATE creature_template SET InhabitType=4 WHERE entry = 16218;
DELETE FROM creature WHERE id = 16218;
INSERT INTO creature VALUES (800020,16218,533,1,1,0,0,3487.17,-2911.54,319.417,5.71991,25,2,0,300,0,0,1);
INSERT INTO creature VALUES (800021,16218,533,1,1,0,0,3527.79,-2952.2,319.422,4.16378,25,2,0,300,0,0,1);

DELETE FROM spell_script_target WHERE entry in (28087, 28096, 28111, 28159);
INSERT INTO spell_script_target VALUES
(28159, 1, 15928),
(28087, 1, 16218),
(28096, 1, 15929),
(28111, 1, 15930);

DELETE FROM scripted_areatrigger WHERE entry = 4113;
INSERT INTO scripted_areatrigger VALUES
(4113, 'at_naxxramas');

DELETE FROM script_texts WHERE entry BETWEEN -1533149 AND -1533147;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1533147,'%s loses its link!',0,3,0,0,'tesla_coil EMOTE_LOSING_LINK'),
(-1533148,'%s overloads!',0,3,0,0,'tesla_coil EMOTE_TESLA_OVERLOAD'),
(-1533149,'The polarity has shifted!',0,3,0,0,'thaddius EMOTE_POLARITY_SHIFT');

UPDATE `gameobject` SET `state` = 1 WHERE `guid` = 73469;
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '1' where `entry` in (194022, 181195, 181228, 181225, 181201, 181200);
UPDATE `gameobject_template` SET `faction` = 114 where `entry` in (181234, 181169, 181168, 181477, 181478);
-- portals
UPDATE `gameobject_template` SET `faction` = 114 where `entry` in (181212, 181211, 181210, 181213);
update `gameobject_template` set `ScriptName` = 'go_naxxTeleporter' where entry in (181575, 181577, 181578, 181576);
-- Zombie crow, Gluth
Update `creature_template` set `ScriptName` = 'mob_zombie_chows' where `entry` = 16360;
Update `creature_template` set `ScriptName` = 'mob_worshippers' where `entry` = 16506;
Update `creature_template` set `ScriptName` = 'mob_unstoppable_abomination' where `entry` = 16428;
-- Grobbulus
Update `creature_template` set `ScriptName` = 'npc_grobbulus_poison_cloud' where `entry` = 16363;
Update `creature_template` set `ScriptName` = 'boss_grobbulus' where `entry` = 15931;
-- Loatheb
Update `creature_template` set `ScriptName` = 'npc_loatheb_spores' where `entry` = 16286;
-- Noth: fix guardians hp
Update `creature_template` set `minhealth` = 104264, `maxhealth` = 104264 where `entry` = 16983;
Update `creature_template` set `minhealth` = 104264, `maxhealth` = 104264 where `entry` = 16984;
-- Razu
Update `creature_template` set npcFlag =1, unit_flags = 68, `ScriptName` = 'npc_obedience_crystal' where `entry` = 29912;
-- enable los for gothik door
Update `gameobject_template` set data10 = 1 where `entry` = 181170;
UPDATE creature_template SET `unk16` = 200, `type_flags` = 1024 WHERE entry in (30641, 16129);
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14 WHERE entry = 16363;
