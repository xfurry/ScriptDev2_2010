/* KARAZHAN */
UPDATE `creature_template` SET `ScriptName` = 'boss_nightbane' WHERE `entry` = 17225;
-- Chess event loot chest
Update `gameobject` set `spawntimesecs` = -604800 where `id` = 185119;
update gameobject_template set faction = 0, data15 = 1 where entry = 185119;