/* UTGARDE PINNACLE */
-- Utgarde Doors
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '0' where `entry` in (192173, 192174);
-- Svala
update `creature` set `spawnMask` = 0 where `id` = 26668; -- Undead Svala should not be spawned
update `creature` set `spawnMask` = 3 where `id` = 29281; -- Only human Svala should be spawned
update `creature_template` set `ScriptName` = 'boss_svala' where `entry` = 29281;
update `creature_template` set `ScriptName` = 'mob_ritual_channeler' where `entry` = 27281;
update `creature_template` set `ScriptName` = 'boss_skadi' where `entry` = 26693;
update `creature_template` set `ScriptName` = 'mob_massive_jormungar' where `entry` = 26685;
update `creature_template` set `ScriptName` = 'mob_ferocious_rhino' where `entry` = 26686;
update `creature_template` set `ScriptName` = 'mob_ravenous_furbolg' where `entry` = 26684;
update `creature_template` set `ScriptName` = 'mob_frenzied_worgen' where `entry` = 26683;
UPDATE gameobject_template SET ScriptName = "go_statis_generator" WHERE entry = 188593;
UPDATE gameobject_template SET ScriptName = "go_skaldi_harpoonluncher" WHERE entry in (192175, 192176, 192177);
UPDATE `creature_template` SET `flags_extra` = 1 WHERE `entry` = 30809;
Update `creature` set `spawnTimeSecs` = 43200 where `id` = 29281;
