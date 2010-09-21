-- Brewfest
UPDATE `creature_template` SET `ScriptName`='boss_coren_direbrew' WHERE `entry` = 23872;
UPDATE `creature_template` SET `ScriptName`='mob_direbrew_maiden' WHERE `entry` in (26764, 26822);
update creature set position_x =  890.93, position_y = -130.455, position_z = -49.745, orientation =  5.187 where id = 23872;

-- Hallows end
Delete from `spell_script_target` where entry in (42405, 42410, 43101);
Insert into `spell_script_target` values
(42405, 1, 23775),
(42410, 1, 23682),
(43101, 1, 23775);