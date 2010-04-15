/* ZULGURUB */
-- Scriptnames
update `creature_template` set ScriptName = "mob_brain_wash_totem" where entry = 15112;
UPDATE creature_template SET ScriptName = 'mob_sonofhakkar' WHERE entry = 11357;
UPDATE creature_template SET ScriptName = 'mob_bloodseeker_bat' WHERE entry = 14965;
UPDATE script_texts SET type = 6 WHERE entry = -1309022;
delete from areatrigger_scripts where entry = 3930;
INSERT INTO areatrigger_scripts VALUES (3930,'at_zulgurub_entrance');