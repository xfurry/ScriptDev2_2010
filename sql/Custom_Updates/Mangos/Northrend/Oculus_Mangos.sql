
-- Boss
UPDATE creature_template SET ScriptName = 'boss_drakos' WHERE entry = 27654;
UPDATE creature_template SET ScriptName = 'npc_unstable_sphere' WHERE entry = 28166;

-- Instance
UPDATE `instance_template` SET `scriptName` = 'instance_oculus' WHERE `map` = 578;

-- Objects
update gameobject_template set faction = 114 where entry in (189986, 193995);
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (191349, 193603);
update gameobject set spawntimesecs = -604800 where id in (191349, 193603);
update gameobject set spawnMask = 1 where id = 191349;
update gameobject set spawnMask = 2 where id = 193603;