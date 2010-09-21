-- Brewfest
UPDATE `creature_template` SET `ScriptName`='boss_coren_direbrew' WHERE `entry` = 23872;
UPDATE `creature_template` SET `ScriptName`='mob_direbrew_maiden' WHERE `entry` in (26764, 26822);
update creature set position_x =  890.93, position_y = -130.455, position_z = -49.745, orientation =  5.187 where id = 23872;