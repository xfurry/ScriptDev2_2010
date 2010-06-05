/* VIOLET HOLD */
UPDATE `creature_template` SET `ScriptName`='npc_azure_saboteur' WHERE `entry`='31079';
UPDATE `creature_template` SET `ScriptName`='boss_cyanigosa' WHERE `entry`='31134';
UPDATE `creature_template` SET `ScriptName`='boss_erekem' WHERE `entry`='29315';
UPDATE `creature_template` SET `ScriptName`='mob_erekem_guard' WHERE `entry`='29395';
UPDATE `creature_template` SET `ScriptName`='boss_ichoron' WHERE `entry`='29313';
UPDATE `creature_template` SET `ScriptName`='mob_ichor_globule',`modelid_A`=5492, `modelid_H`=5492 WHERE `entry`='29321';
UPDATE `creature_template` SET `modelid_A`=5492, `modelid_H`=5492 WHERE `entry`='31515'; -- heroic
UPDATE `creature_template` SET `ScriptName`='boss_lavanthor' WHERE `entry`='29312';
UPDATE `creature_template` SET `ScriptName`='boss_moragg' WHERE `entry`='29316';
UPDATE `creature_template` SET `ScriptName`='boss_xevozz' WHERE `entry`='29266';
UPDATE `creature_template` SET `ScriptName`='mob_ethereal_sphere' WHERE `entry`='29271';
UPDATE `creature_template` SET `ScriptName`='boss_zuramat' WHERE `entry`='29314';
UPDATE `creature_template` SET `ScriptName`='mob_zuramat_sentry' WHERE `entry`='29364';

UPDATE `gameobject_template` SET ScriptName = 'go_activation_crystal' where entry = 193611;
UPDATE `gameobject_template` SET `flags` = 36 WHERE `entry` = 191723;	-- door seal
UPDATE `gameobject_template` SET faction = 114 where entry in (191556, 191557, 191558, 191559, 191560,191562, 191563, 191564, 191565, 191566, 191606, 191722);

DELETE FROM `creature` WHERE map = 608 AND `id`='31134';
insert into creature VALUES ('7392', '31134', '608', '0', '1', '0', '0', '1888.271', '810.781', '38.441', '3.11684', '43200', '0', '0', '284475', '37250', '0', '0');
UPDATE `gameobject_template` SET `flags`=`flags`|4 WHERE `entry` IN (191723,191564,191563,191562,191606,191722,191556,191566,191565); -- door untargetable

UPDATE `creature_template` SET AIName='EventAI',`ScriptName`='' WHERE `entry` IN ('30660','30695','30666','30668','30667','32191');
DELETE FROM creature_ai_scripts WHERE creature_id IN ('30660','30695','30666','30668','30667','32191');
INSERT INTO `creature_ai_scripts` VALUES
('3066001', '30660', '0', '0', '100', '1', '5000', '10000', '30000', '32000', '11', '58504', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Agonizing Strike'),
('3066002', '30660', '0', '0', '100', '1', '12000', '15000', '24000', '30000', '11', '58508', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Side Swipe'),
('3069501', '30695', '0', '0', '100', '3', '5000', '10000', '30000', '32000', '11', '58531', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Arcane Missiles'),
('3069502', '30695', '0', '0', '100', '1', '12000', '15000', '24000', '30000', '11', '58534', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Deep Freeze'),
('3069503', '30695', '0', '0', '100', '3', '12000', '15000', '24000', '30000', '11', '58532', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Frostbolt Volley'),
('3069504', '30695', '0', '0', '100', '5', '5000', '10000', '30000', '32000', '11', '61593', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Arcane Missiles'),
('3069505', '30695', '0', '0', '100', '5', '12000', '15000', '24000', '30000', '11', '61594', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Frostbolt Volley'),
('3066601', '30666', '0', '0', '100', '1', '12000', '15000', '24000', '30000', '11', '32736', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Mortal Strike'),
('3066602', '30666', '0', '0', '100', '3', '12000', '15000', '24000', '30000', '11', '41057', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Whirlwind'),
('3066603', '30666', '0', '0', '100', '5', '5000', '10000', '30000', '32000', '11', '41056', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Whirlwind'),
('3066801', '30668', '0', '0', '100', '1', '12000', '15000', '24000', '30000', '11', '60158', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Magic Reflection'),
('3066802', '30668', '0', '0', '100', '1', '12000', '15000', '24000', '30000', '11', '52719', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Concussion Blow'),
('3066701', '30667', '0', '0', '100', '3', '5000', '10000', '30000', '32000', '11', '60181', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Arcane Stream'),
('3066702', '30667', '0', '0', '100', '3', '12000', '15000', '24000', '30000', '11', '60182', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Mana Detonation'),
('3066703', '30667', '0', '0', '100', '5', '5000', '10000', '30000', '32000', '11', '60204', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Arcane Stream'),
('3066704', '30667', '0', '0', '100', '5', '12000', '15000', '24000', '30000', '11', '60205', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Mana Detonation'),
('3219101', '32191', '0', '0', '100', '1', '11000', '11000', '15000', '15000', '11', '58471', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Backstab'),
('3219102', '32191', '0', '0', '100', '1', '10000', '10000', '15000', '15000', '11', '58470', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Tactical Blink');
DELETE FROM `spell_script_target` WHERE `entry` IN (54160,59474);
INSERT INTO spell_script_target VALUES (54160, 1, 29266);
INSERT INTO spell_script_target VALUES (59474, 1, 29266);

UPDATE `creature_template` SET ScriptName='mob_vh_dragons' WHERE entry IN (30661, 30662, 30663, 30664, 30665, 30666, 30667, 30668, 32191, 30660, 30695);
delete from spell_target_position where id = 62139;
insert into spell_target_position VALUES (62139, 608, 1831.821, 804.025, 44.321, 6.217);