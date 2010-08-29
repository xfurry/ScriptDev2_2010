
-- Boss
UPDATE creature_template SET ScriptName = 'boss_drakos' WHERE entry = 27654;
UPDATE creature_template SET ScriptName = 'npc_unstable_sphere' WHERE entry = 28166;
UPDATE creature_template SET ScriptName = 'boss_varos' WHERE entry = 27447;
UPDATE creature_template SET ScriptName = 'npc_centrifuge_sphere' WHERE entry = 28183;
UPDATE creature_template SET ScriptName = 'boss_eregos' WHERE entry = 27656;
update creature set spawnMask = 0 where id = 28166;
UPDATE `creature_template` SET `modelid_2` = 11686 WHERE `entry` = 28239;

-- Instance
UPDATE `instance_template` SET `scriptName` = 'instance_oculus' WHERE `map` = 578;

-- Objects
update gameobject_template set faction = 114 where entry in (189986, 193995);
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (191349, 193603);
update gameobject set spawntimesecs = -604800 where id in (191349, 193603);
update gameobject set spawnMask = 1 where id = 191349;
update gameobject set spawnMask = 2 where id = 193603;
-- Tele
REPLACE INTO spell_target_position VALUES
(48760, 571, 3878.91, 6984.5, 106.321, 3.15),
(49305, 578, 988.303, 1056.88, 359.967, 3.42);