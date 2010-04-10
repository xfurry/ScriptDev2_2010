/* HALLS OF REFLECTION */
-- trash
UPDATE `creature_template` SET `ScriptName`='mob_frostsworn_general' WHERE `entry`=36723;
UPDATE `creature_template` SET `ScriptName`='mob_lichKing_minion' WHERE `entry` in (36940, 36941, 37069);
UPDATE `creature_template` SET `ScriptName`='mob_hallsOfReflectionSoul' WHERE `entry` in (38176, 38175, 38172, 38567, 38177, 38173);
-- boss
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
UPDATE `creature_template` SET `ScriptName`='boss_falric' WHERE `entry`=38112;
-- npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_start' WHERE `entry` in (37221, 37223);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_end' WHERE `entry` in (36955, 37554);
UPDATE `creature_template` SET `ScriptName`='npc_lich_king_hor_start' WHERE `entry`=36954;
UPDATE `creature_template` SET `ScriptName`='boss_lich_king_hor_end' WHERE `entry`=37226;
update creature set spawnMask = 0 where guid in (135387, 135386);
update creature set spawnMask = 0 where id in (36940, 36941, 37069);
update creature set spawnMask = 0 where id in (38176, 38175, 38172, 38567, 38177, 38173); -- Temporary until script fulll implemented -> spawnMask should be 3 and the npc invisible until the event starts

update instance_template set script = 'instance_halls_of_reflection' where map = 668;
update instance_template set script = 'instance_pit_of_saron' where map = 658;
update instance_template set script = 'instance_forge_of_souls' where map = 632;

update gameobject set spawntimesecs = -604800 where id in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 0 WHERE entry in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 114 WHERE entry in (201385, 197342, 197343, 201976, 197341, 202236, 202302, 201709);