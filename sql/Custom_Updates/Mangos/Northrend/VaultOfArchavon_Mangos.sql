/* VAULT OF ARCHAVON */
-- Scriptnames
update `creature_template` set `ScriptName` = 'boss_archavon' where `entry` = 31125;
update `creature_template` set `ScriptName` = 'boss_koralon' where `entry` = 35013;
update `creature_template` set `ScriptName` = 'boss_emalon' where `entry` = 33993;
update `creature_template` set `ScriptName` = 'npc_tempest_minion' where `entry` = 33998;
update `creature_template` set `ScriptName` = 'npc_tempest_warder' where `entry` in (33998, 34049);
UPDATE `creature_template` SET `ScriptName` = 'mob_flame_warder' WHERE `entry`=35143;
UPDATE `creature_template` SET `ScriptName` = 'boss_toravon' WHERE `entry`= 38433;
UPDATE `creature_template` SET `ScriptName` = 'mob_frost_warder' WHERE `entry`= 38482;
UPDATE `creature_template` SET `ScriptName` = 'mob_frozen_orb_stalker' WHERE `entry`= 38461;
UPDATE `creature_template` SET `ScriptName` = 'mob_frozen_orb' WHERE `entry` = 38456;
update `instance_template` set `script` = 'instance_vault_of_archavon' where `map` = 624;