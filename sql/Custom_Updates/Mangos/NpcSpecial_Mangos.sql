-- Snakes trap
UPDATE `creature_template` SET `ScriptName`='npc_snake_trap_serpents' WHERE `entry` IN (19921, 19833);
-- Mirror Immage
UPDATE `creature_template` SET `modelid_A` = 11686, `modelid_H` = 11686, `ScriptName`='npc_mirror_image' WHERE `entry` = 31216;
-- Rune blade
UPDATE `creature_template` SET `modelid_A` = 11686, `modelid_H` = 11686, `ScriptName`='npc_runeblade' WHERE `entry` = 27893;
-- Gargoyle
UPDATE `creature_template` SET `ScriptName` = 'npc_death_knight_gargoyle' WHERE `entry` = 27829;
-- XP eliminator
UPDATE `creature_template` SET `ScriptName` = 'npc_experience_eliminator' WHERE `entry` in (35364, 35365);
-- Training dummy
UPDATE `creature_template` SET `ScriptName` = 'npc_training_dummy' WHERE `entry` in (31144, 32666, 32546, 32542, 30527, 31146);