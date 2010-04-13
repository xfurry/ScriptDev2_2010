/* UTGARDE KEEP */
-- Utgarde Doors
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '0' where `entry` in (186756, 186694, 186608, 186612);
UPDATE creature_template SET ScriptName = "mob_frost_tomb" where entry = 23965;
