/* SERPENT SHRINE CAVERN */
-- Bridge
Update `gameobject_template` set flags = 6553632, type = 1, `ScriptName` = 'go_main_bridge_console' where `entry` in (185118, 185117, 185116, 185115, 185114);
Update `gameobject_template` set `faction` = 114 where `entry` in (184568, 184203, 184204, 184205);
-- Lurker
Update `creature_template` set `ScriptName` = 'boss_the_lurker_below', `InhabitType` = 3, `unit_flags` = 32768 where `entry` = 21217;
UPDATE gameobject_template SET ScriptName = "go_strange_pool" WHERE entry = 184956;
-- Corect the Lurker's loot
Update `creature_loot_template` set `item` = 29434 where `entry` = 21217  and `item` = 33054;
-- Coilfang frenzy
Update `creature_template` set `InhabitType` = 2, `unit_flags` = 32768  where `entry` = 21508;
