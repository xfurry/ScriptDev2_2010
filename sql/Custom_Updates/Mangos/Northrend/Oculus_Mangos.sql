
-- Boss
UPDATE creature_template SET ScriptName = 'boss_drakos' WHERE entry = 27654;
UPDATE creature_template SET ScriptName = 'npc_unstable_sphere' WHERE entry = 28166;
UPDATE creature_template SET ScriptName = 'boss_varos' WHERE entry = 27447;
UPDATE creature_template SET ScriptName = 'npc_centrifuge_sphere' WHERE entry = 28183;
UPDATE creature_template SET ScriptName = 'boss_urom' WHERE entry = 27655;
UPDATE creature_template SET ScriptName = 'boss_eregos', `type_flags` = 1120 WHERE entry = 27656;
update creature set spawnMask = 0 where id = 28166;
UPDATE `creature_template` SET `modelid_2` = 11686 WHERE `entry` = 28239;
UPDATE `creature_template` SET `type_flags` = 1032 WHERE `entry` in (28276, 27636);

-- Instance
UPDATE `instance_template` SET `scriptName` = 'instance_oculus' WHERE `map` = 578;
UPDATE creature_template SET ScriptName = 'oculus_dragons' WHERE entry in(27659, 27657, 27658);

-- Objects
update gameobject_template set faction = 114 where entry in (189986, 193995);
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (191349, 193603);
update gameobject set spawntimesecs = -604800 where id in (191349, 193603);
update gameobject set spawnMask = 1 where id = 191349;
update gameobject set spawnMask = 2 where id = 193603;
-- Tele
UPDATE gameobject_template SET ScriptName="oculus_teleporter" WHERE entry in (188715, 189985);
REPLACE INTO spell_target_position VALUES
(48760, 571, 3872.43, 6984.34, 108.055, 6.20),
(49305, 578, 988.303, 1056.88, 359.967, 3.42);