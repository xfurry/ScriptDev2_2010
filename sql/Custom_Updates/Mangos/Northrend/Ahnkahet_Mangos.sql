/* AHN'KAHET */
UPDATE `gameobject_template` SET `faction` = 114, `data0` = '0' where `entry` in (192236);
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, ScriptName = "mob_twisted_visage" WHERE entry = 30621;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, ScriptName = "mob_ancient_void" WHERE entry = 30622;
UPDATE creature_template SET ScriptName = "npc_twilight_volunteer" WHERE entry = 30385;
UPDATE creature_template SET ScriptName = "mob_flame_orb" WHERE entry = 30702;
UPDATE creature_template SET ScriptName = "boss_amanitar" WHERE entry = 30258;
UPDATE creature_template SET ScriptName = "npc_amanitar_mushroom" WHERE entry in (30391, 30435);
UPDATE creature_template SET ScriptName = "mob_ahnkahet_guardian" where entry = 30176;
