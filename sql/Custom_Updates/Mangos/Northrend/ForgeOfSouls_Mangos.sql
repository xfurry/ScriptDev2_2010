/* FORGE OF SOULS */
-- Trash
UPDATE `creature_template` SET `ScriptName`='npc_SpitefulApparition' WHERE `entry`=36551;
UPDATE `creature_template` SET `ScriptName`='npc_SpectralWarden' WHERE `entry`=36666;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardWatchman' WHERE `entry`=36478;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardReaper' WHERE `entry`=36499;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardBonecaster' WHERE `entry`=36564;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardAnimator' WHERE `entry`=36516;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardAdept' WHERE `entry`=36620;
UPDATE `creature_template` SET `ScriptName`='npc_SoulHorror' WHERE `entry`=36522;
-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_Bronjahn' WHERE `entry`=36497;
UPDATE `creature_template` SET `ScriptName`='mob_corrupted_soul_fragment' WHERE `entry`=36535;
UPDATE `creature_template` SET `ScriptName`='boss_Devourer_of_Souls' WHERE `entry`=36502;
UPDATE `creature_template` SET `ScriptName`='mob_well_of_souls' WHERE `entry`=36536;
-- Npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_fos_start' WHERE `entry` in (37596, 37597);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_fos_end' WHERE `entry` in (38160, 38161);
update creature set spawnMask = 0 where id in (37584, 37497, 37496, 37588, 38161, 38160);