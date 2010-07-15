/* VAULT OF ARCHAVON */
-- Scriptnames
-- Archavon
update `creature_template` set `ScriptName` = 'boss_archavon' where `entry` = 31125;
update `creature_template` set `ScriptName` = 'npc_archavon_warder' where `entry` = 32353;
-- Emalon
update `creature_template` set `ScriptName` = 'boss_emalon' where `entry` = 33993;
update `creature_template` set `ScriptName` = 'npc_tempest_minion' where `entry` = 33998;
update `creature_template` set `ScriptName` = 'npc_tempest_warder' where `entry` = 34015;
-- Koralon
update `creature_template` set `ScriptName` = 'boss_koralon' where `entry` = 35013;
UPDATE `creature_template` SET `ScriptName` = 'mob_flame_warder' WHERE `entry`=35143;
-- Toravon
UPDATE `creature_template` SET `ScriptName` = 'boss_toravon' WHERE `entry`= 38433;
UPDATE `creature_template` SET `ScriptName` = 'mob_frost_warder' WHERE `entry`= 38482;
UPDATE `creature_template` SET `ScriptName` = 'mob_frozen_orb_stalker' WHERE `entry`= 38461;
UPDATE `creature_template` SET `ScriptName` = 'mob_frozen_orb' WHERE `entry` = 38456;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('72096', '1', '38433');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('72034', '1', '38433');
-- Instance
update `instance_template` set `script` = 'instance_vault_of_archavon' where `map` = 624;