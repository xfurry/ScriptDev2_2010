/* ZUL'AMAN */
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (186728, 186304, 186303, 186305, 186858, 186859);
-- original flags = 2
update gameobject_template set faction = 0, flags = 0, data15 = 1 where entry in (186648, 187021, 186667, 186672);
Update `gameobject` set `spawntimesecs` = -604800 where `id` in (186648, 187021, 186667, 186672);
update gameobject set position_y = 1411.92 where id = 187021;
update gameobject set position_x = -78.0 where id = 186667;
update gameobject set position_x = 377.89 where id = 186672;
update gameobject set position_x = -142.91, position_y = 1332.07, position_z = 48.17 where id = 186648;
UPDATE `creature_template` SET `ScriptName` = 'mob_zuljin_vortex' WHERE `entry` = 24136;
