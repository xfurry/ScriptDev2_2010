/* SUNWELL PLATEAU */

-- Madrigosa & Kalec should not be spawned
update `creature` set `spawnMask` = 0 where `id` in (24895, 25319, 24848);

-- Kalecgos
replace into `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) values 
(102510, 24891, 580, 1, 1, 23350, 0, 1704.316, 928.544, -74.558, 4.65823, 360, 0, 0, 828555, 169350, 0, 0),
(100698, 24892, 580, 1, 1, 0, 0, 1681.422, 909.523, -74.558, 0.67234, 604800, 0, 0, 2018275, 1693500, 0, 0);
UPDATE `creature_template` SET `modelid_1` = 20577 WHERE `entry` in (25796, 25795);

-- Felmyst
UPDATE creature_template SET `ScriptName` = 'boss_felmyst', `modelid_2` = 22838 WHERE `entry` = 25038; 
UPDATE creature_template SET `ScriptName` = 'mob_felmyst_vapor' WHERE `entry` = 25265;  
UPDATE creature_template SET `ScriptName` = 'mob_fog_of_corruption' WHERE `entry` = 25703; 
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('44883', '1', '24882');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('45063', '1', '24882');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('44885', '1', '25038');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('46350', '1', '25038');

-- Eredar Twins
UPDATE `creature_template` SET `ScriptName` = 'boss_sacrolash' WHERE `entry` = 25165; 
UPDATE `creature_template` SET `ScriptName` = 'boss_alythess' WHERE `entry` = 25166; 
UPDATE `creature_template` SET `ScriptName` = 'npc_shadow_image' WHERE `entry` = 25214; 

-- Muru and Entropius
UPDATE `creature_template` SET `ScriptName` = 'boss_muru' WHERE `entry` = 25741;
UPDATE `creature_template` SET `ScriptName` = 'boss_entropius', `modelid_2` = 23428, `modelid_4` = 23428 WHERE `entry` = 25840;
UPDATE `creature_template` SET `ScriptName` = 'mob_dark_fiend' WHERE `entry` = 25744; 
UPDATE `creature_template` SET `ScriptName` = 'mob_voidsentinel' WHERE `entry` = 25772; 
UPDATE creature_template SET  `ScriptName` = 'mob_singularity' WHERE `entry` = 25855; 
UPDATE creature_template SET  `ScriptName` = 'mob_darkness' WHERE `entry` = 25879; 
UPDATE `creature_template` SET `flags_extra` = 0 WHERE `entry` = 25770;

-- Kiljaeden 
UPDATE `creature_template` SET `ScriptName` = 'boss_kiljaeden', `unit_flags` = 4 WHERE `entry` = 25315; 
UPDATE `creature_template` SET `ScriptName` = 'mob_deceiver' WHERE `entry` = 25588; 
UPDATE `creature_template` SET `ScriptName` = 'mob_shield_orb' WHERE `entry` = 25502; 
UPDATE `creature_template` SET `ScriptName` = 'mob_felfire_portal' WHERE `entry` = 25603; 
UPDATE `creature_template` SET `ScriptName` = 'mob_armagedon_target' WHERE `entry` = 25735; 
UPDATE `creature_template` SET `ScriptName` = 'mob_felfire_fiend' WHERE `entry` = 25598;
UPDATE `creature_template` SET `ScriptName` = 'mob_kiljaeden_controller' WHERE `entry` = 25608; 
UPDATE `creature_template` SET `unit_flags` = 33554438 WHERE `entry` = 25603; 
update creature set position_z = 60.0 where id = 26046;
UPDATE `gameobject_template` SET `type` = 10 WHERE `entry` = 188415;
Update `gameobject_template` set `flags` = 6553648, `ScriptName` = 'go_orb_of_the_blue_flight' where `entry` = 188415;
-- Controller
delete from creature where id = 25608;
insert into creature values (800110,25608,580,1,1,0,0,1698.61,628.414,27.5395,3.99799,604800,0,0,9347800,1693500,0,0);

-- Missing Doors
-- Will be added later
#delete from `gameobject` where `id` in (187990, 187764, 187765, 187766, 188118);
#insert into `gameobject` values (160001, 187990, 580, 1, 1, 1849.66, 597.861, 86.105, 2.42601, 0, 0, 0.936672, 0.350207, 180, 100, 0),
#(160002, 187764, 580, 1, 1, 1876.03, 616.974, 44.6421, 1.46608, 0, 0, 0.669131, 0.743144, 180, 100, 0),
#(160003, 187765, 580, 1, 1, 1700.56, 711.332, 85.444, -0.191984, 0, 0, -0.095845, 0.995396, 180, 100, 0),
#(160004, 187766, 580, 1, 1, 1709.94, 530.091, 93.4119, 1.11701, 0, 0, 0.529919, 0.848048, 180, 100, 0),
#(160005, 188118, 580, 1, 1, 1775.49, 673.15, 91.9614, 1.55334, 0, 0, 0.700909, 0.71325, 180, 100, 1);
UPDATE `gameobject_template` SET `faction` = 114 WHERE `entry` in (187990, 187764, 187765, 187766, 188118);

