
-- Boss
update creature_template set scriptName = 'boss_malygos' where entry = 28859;
update creature_template set scriptName = 'mob_power_spark' where entry = 30084;
update creature_template set scriptName = 'mob_scion_of_eternity' where entry = 30249;
UPDATE `creature_template` SET `modelid_2` = 16925, `modelid_4` = 16925 WHERE `entry` = 22517;
#update creature set spawnMask = 3 where id = 28859;

-- Loot
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (194158, 194159, 193905, 193967);
update gameobject set spawntimesecs = -604800 where id in (194158, 194159, 193905, 193967);

-- Instance
#REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('56505', '1', '22517');
#REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('56152', '1', '28859');
UPDATE `instance_template` SET `scriptName` = 'instance_eye_of_eternity' WHERE `map` = 616;