/* DRAKTHARON KEEP */
update instance_template set scriptName = 'instance_draktharon' where map = 600;
UPDATE creature_template SET ScriptName = "boss_dred" WHERE entry = 27483;
UPDATE creature_template SET ScriptName = "mob_dred_raptor" WHERE entry in (26641, 26628);
UPDATE creature_template SET ScriptName = "mob_novos_minionAI" WHERE entry in (27597, 27598, 27600);
UPDATE creature_template SET ScriptName = "crystal_handler" WHERE entry = 26627;
UPDATE creature_template SET ScriptName = "crystal_channel" WHERE entry = 26712;