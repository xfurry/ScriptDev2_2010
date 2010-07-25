/* RUINS OF AHN'QIRAJ */
-- Creature template
UPDATE `creature_template` SET `ScriptName` = 'mob_flesh_hunter' WHERE `entry` = 15335;
UPDATE `creature_template` SET `ScriptName` = 'mob_anubisath_guardian' WHERE `entry` = 15355;
UPDATE `creature_template` SET `ScriptName` = 'mob_buru_egg' WHERE `entry` = 15514;
UPDATE `creature_template` SET `ScriptName` = 'boss_buru' WHERE `entry` = 15370;
UPDATE `creature_template` SET `ScriptName` = 'boss_moam' WHERE `entry` = 15340;
UPDATE `creature_template` SET `ScriptName` = 'boss_ossirian' WHERE `entry` = 15339;
UPDATE `creature_template` SET `ScriptName` = 'boss_kurinnaxx' WHERE `entry` = 15348;
UPDATE `creature_template` SET `ScriptName` = 'boss_rajaxx' WHERE `entry` = 15341;
UPDATE `creature_template` SET `ScriptName` = 'boss_ayamiss' WHERE `entry` = 15369;
UPDATE `creature_template` SET `ScriptName` = 'npc_andorov' WHERE `entry` = 15471;
-- Fixes
Delete from `creature_ai_texts` where `entry` = -2110;
Delete from `npc_text` where `id` = 14442;
INSERT INTO `creature_ai_texts` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
('-2110', 'Kill $N', '', '', '', '', '', '', '', '', 0, '1', '0', '0', 'Captain Tuubid - Say Kill');
insert into `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) values
('14442','<Andorov looks at you grimly.>$B$BIf it bleeds, we can kill it.','','0','1','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0','','','0','0','0','0','0','0','0','0');
update `creature` set `id` = 15339 where `guid` = 51942;
update `creature` set `id` = 15355 where `guid` = 51869;