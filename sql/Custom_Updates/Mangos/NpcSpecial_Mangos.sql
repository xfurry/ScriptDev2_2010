-- Snakes trap
UPDATE `creature_template` SET `ScriptName`='npc_snake_trap_serpents' WHERE `entry` IN (19921, 19833);
-- Mirror Immage
UPDATE `creature_template` SET `modelid_1` = 11686, `modelid_3` = 11686, `ScriptName`='npc_mirror_image' WHERE `entry` = 31216;
-- Rune blade
UPDATE `creature_template` SET `modelid_1` = 11686, `modelid_3` = 11686, `ScriptName`='npc_runeblade' WHERE `entry` = 27893;
-- Gargoyle
UPDATE `creature_template` SET `ScriptName` = 'npc_death_knight_gargoyle' WHERE `entry` = 27829;
-- XP eliminator
UPDATE `creature_template` SET `ScriptName` = 'npc_experience_eliminator' WHERE `entry` in (35364, 35365);
-- Training dummy
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_training_dummy' WHERE `entry` IN (31146, 31144, 30527, 32541, 32542, 32543, 32545, 32546, 32666, 32667);