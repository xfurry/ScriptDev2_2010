/* HALLS OF LIGHTNING */
-- Fix invisible stalker!
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35 where `entry` = 28947;
/* HALLS OF LIGHTNING */
-- EventAI doesn't work properly, set npcs to generic creature
-- Todo:
#update `creature_template` set `AIName` = '', `ScriptName` = 'generic_creature' where `entry` in (28961, 28965);
