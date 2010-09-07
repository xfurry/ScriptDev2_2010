
-- Boss
update creature_template set scriptName = 'boss_malygos' where entry = 28859;
update creature_template set scriptName = 'mob_power_spark' where entry = 30084;
update creature_template set scriptName = 'mob_scion_of_eternity' where entry = 30249;
update creature_template set scriptName = 'mob_nexus_lord' where entry = 30245;
update creature_template set scriptName = 'npc_alexstrasza_eoe' where entry = 32295;
UPDATE `creature_template` SET `modelid_2` = 16925, `modelid_4` = 16925 WHERE `entry` = 22517;
UPDATE `creature_template` SET `unit_flags` = 33554436 WHERE `entry` in (30334, 30592, 30282);
-- original data2 = 20711
Update `gameobject_template` set `data2` = 0, `ScriptName` = 'go_focusing_iris' where `entry` in (193958, 193960);

-- Loot
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (194158, 194159, 193905, 193967);
update gameobject set spawntimesecs = -604800 where id in (194158, 194159, 193905, 193967);

-- Instance
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('56505', '1', '22517');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('56152', '1', '28859');
UPDATE `instance_template` SET `scriptName` = 'instance_eye_of_eternity' WHERE `map` = 616;