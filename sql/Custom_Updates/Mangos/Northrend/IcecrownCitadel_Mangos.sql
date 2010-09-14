/* ICECROWN CITADEL */

-- Marrowgar
UPDATE creature_template SET ScriptName = 'boss_marrowgar' WHERE entry = 36612;
UPDATE creature_template SET ScriptName = 'mob_coldflame' WHERE entry = 36672;
UPDATE creature_template SET ScriptName = 'mob_bone_spike' WHERE entry = 38711;
UPDATE `creature_template` SET `flags_extra` = 0 WHERE `entry` = 36672;

-- Deathwhisper
UPDATE creature_template SET mechanic_immune_mask = 634339327, ScriptName = 'boss_deathwhisper' WHERE entry = 36855;
UPDATE creature_template SET ScriptName = 'mob_vengeful_shade' WHERE entry = 38222;
UPDATE creature_template SET ScriptName = 'mob_cult_adherent' WHERE entry = 37949;
UPDATE creature_template SET ScriptName = 'mob_cult_fanatic' WHERE entry = 37890;

-- Gunship Battle
update creature_template set scriptName = 'boss_gunship_commander' where entry in (36948, 36939);
update creature_template set scriptName = 'mob_sergeant' where entry in (36960, 36961);
update creature_template set scriptName = 'mob_rm_soldiers' where entry in (36982, 36978);
update creature_template set scriptName = 'mob_rifleman' where entry in (36968, 36969);
update creature_template set scriptName = 'mob_battle_mage' where entry in (37117, 37116);
update creature_template set scriptName = 'mob_marine' where entry in (36957, 36950);

-- Deathbringer Saurfang
UPDATE creature_template SET `unit_flags` = 256, ScriptName = 'boss_saurfang' WHERE entry = 37813;
UPDATE creature_template SET ScriptName = 'mob_blood_beast' WHERE entry = 38508;
update creature set position_x = -490 where id = 37813;

-- Festergut
UPDATE creature_template SET ScriptName = 'boss_festergut' WHERE entry = 36626;
UPDATE creature_template SET ScriptName = 'mob_vile_gas_stalker' WHERE entry = 38548;

-- Rotface
UPDATE creature_template SET ScriptName = 'boss_rotface' WHERE entry = 36627;
UPDATE creature_template SET ScriptName = 'mob_small_ooze', `flags_extra` = 256 WHERE entry = 36897;
UPDATE creature_template SET ScriptName = 'mob_big_ooze' WHERE entry = 36899;
UPDATE creature_template SET ScriptName = 'mob_sticky_ooze' WHERE entry = 37006;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69783', '1', '37013');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69797', '1', '37013');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69799', '1', '37013');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69802', '1', '37013');
update creature set spawnMask = 0 where id in (37013);

-- Putricide
UPDATE creature_template SET ScriptName = 'boss_professor_putricide' WHERE entry = 36678;
UPDATE creature_template SET ScriptName = 'mob_icc_volatile_ooze' WHERE entry = 37697;
UPDATE creature_template SET ScriptName = 'mob_icc_gas_cloud' WHERE entry = 37562;
UPDATE creature_template SET ScriptName = 'mob_icc_gas_bomb' WHERE entry = 38159;
UPDATE creature_template SET ScriptName = 'mob_slime_puddle' WHERE entry = 37690;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('71617', '1', '36678');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('71704', '1', '36678');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('71621', '1', '36678');

-- Blood prince council
UPDATE creature_template SET ScriptName = 'boss_prince_taldaram' WHERE entry = 37973;
UPDATE creature_template SET ScriptName = 'boss_prince_keleseth' WHERE entry = 37972;
UPDATE creature_template SET ScriptName = 'boss_prince_valanar' WHERE entry = 37970;
UPDATE creature_template SET ScriptName = 'mob_kinetic_bomb' WHERE entry = 38454;
UPDATE creature_template SET ScriptName = 'mob_conjured_flame' WHERE entry in (38332, 38451);
UPDATE creature_template SET ScriptName = 'mob_dark_nucleus' WHERE entry = 38369;
UPDATE creature_template SET ScriptName = 'mob_shock_vortex' WHERE entry = 38422;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70981', '1', '37972');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70952', '1', '37970');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70982', '1', '37973');


-- Blood queen Lanathel
UPDATE creature_template SET ScriptName = 'boss_blood_queen_lanathel' WHERE entry = 37955;
update creature_template set scriptName = 'mob_swarming_shadow' where entry = 38163;

-- Svalna event
update creature_template set scriptName = 'miniboss_sister_svalna' where entry = 37126;
update creature_template set scriptName = 'mob_valkyr_spear' where entry = 38248;
update creature set spawnMask = 0 where id = 38248;
update creature_template set scriptName = 'mob_crok_scourgebane' where entry = 37129;
update creature_template set scriptName = 'mob_captain_arnath' where entry = 37122;
update creature_template set scriptName = 'mob_captain_brandon' where entry = 37123;
update creature_template set scriptName = 'mob_captain_grondel' where entry = 37124;
update creature_template set scriptName = 'mob_captain_rupert' where entry = 37125;

-- Valithria Dreamwalker
update creature_template set scriptName = 'boss_valithria' where entry = 36789;
update creature_template set scriptName = 'mob_risen_archmage' where entry = 37868;
update creature_template set scriptName = 'mob_blazing_skeleton' where entry = 36791;
update creature_template set scriptName = 'mob_suppresser' where entry = 37863;
update creature_template set scriptName = 'mob_blistering_zombie' where entry = 37934;
update creature_template set scriptName = 'mob_gluttonous_abomination' where entry = 37886;
update creature_template set scriptName = 'mob_nightmare_cloud' where entry = 38421;
update creature_template set scriptName = 'mob_nightmare_preportal' where entry = 38429;
update creature_template set scriptName = 'mob_nightmare_portal' where entry = 38430;
update creature_template set scriptName = 'mob_column_of_frost' where entry = 37918;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70602', '1', '36789');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70588', '1', '36789');

-- Sindragosa
update creature_template set scriptName = 'boss_sindragosa' where entry = 36853;
update creature_template set scriptName = 'miniboss_spinestalker' where entry = 37534;
update creature_template set scriptName = 'miniboss_rimefang' where entry = 37533;
update creature_template set scriptName = 'mob_icc_ice_tomb' where entry = 36980;
update creature_template set scriptName = 'mob_icc_frost_bomb' where entry = 37186;

-- Lich King
update creature_template set `unit_flags` = 0, `type_flags` = 104, scriptName = 'boss_the_lich_king' where entry = 36597;
-- original: 268435564
update creature_template set npcflag = 1, scriptName = 'npc_tirion_final' where entry = 38995;
-- adds
update creature_template set scriptName = 'mob_shambling_horror' where entry = 37698;
update creature_template set scriptName = 'mob_valkyr_shadowguard' where entry = 36609;
update creature_template set scriptName = 'mob_icc_vile_spirit' where entry = 37799;
update creature_template set scriptName = 'mob_icc_raging_spirit' where entry = 36701;
update creature_template set scriptName = 'mob_icc_ice_sphere' where entry = 36633;
update creature_template set scriptName = 'npc_terenas_menethil' where entry = 36823;
update creature_template set scriptName = 'mob_spirit_warden' where entry = 36824;
update creature_template set scriptName = 'mob_defile_target' where entry = 38757;
UPDATE creature_template set scriptName = 'mob_frostmourneDummy' WHERE `entry` = 38584;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('71614', '1', '38995');

-- Dummy frostmourne platform until the real on is found
DELETE FROM `gameobject_template` WHERE (`entry`=250001);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `mingold`, `maxgold`, `ScriptName`) VALUES (250001, 33, 8387, 'Frostmourne Platform', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 0, '');
-- On map
DELETE FROM `gameobject` WHERE `id`=250001;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800201, 250001, 631, 15, 65535, 487.033, -2520.054, 1036.61, 0, 0, 0, 0, 0, 300, 0, 1);

-- Frostwing
update creature_template set scriptName = 'mob_ymirjar_battlemaiden', `unit_flags` = 0 where entry = 37132;
update creature_template set scriptName = 'mob_ymirjar_deathbringer' where entry = 38125;
update creature_template set scriptName = 'mob_ymirjar_warlord', `unit_flags` = 0 where entry = 37133;
update creature_template set scriptName = 'mob_ymirjar_huntress', `unit_flags` = 0 where entry = 37134;
update creature_template set scriptName = 'mob_ymirjar_frostbinder' where entry = 37127;
update creature_template set scriptName = 'mob_frostwarden_handler' where entry = 37531;
update creature_template set scriptName = 'mob_frostwarden_whelp' where entry = 37532;
-- Crimson halls trash:
update creature_template set scriptName = 'mob_valkyr_herald' where entry = 37098;
update creature_template set scriptName = 'mob_darkfallen_archmage' where entry = 37664;
update creature_template set scriptName = 'mob_darkfallen_blood_knight' where entry = 37595;
update creature_template set scriptName = 'mob_darkfallen_noble' where entry = 37663;
update creature_template set scriptName = 'mob_darkfallen_advisor' where entry = 37571;
update creature_template set scriptName = 'mob_darkfallen_lieutenant' where entry = 37665;
update creature_template set scriptName = 'mob_darkfallen_tactician' where entry = 37666;
update creature_template set scriptName = 'mob_darkfallen_commander' where entry = 37662;
-- Plagueworks trash:
update creature_template set scriptName = 'mob_plague_scientist' where entry = 37023;
update creature_template set scriptName = 'mob_blighted_abomination' where entry = 37022;
update creature_template set scriptName = 'mob_vengefull_fleshreaper' where entry = 37038;
update creature_template set scriptName = 'mob_decaying_colossus' where entry = 36880;
UPDATE creature_template SET ScriptName = 'miniboss_stinky' WHERE entry = 37025;
UPDATE creature_template SET ScriptName = 'miniboss_precious' WHERE entry = 37217;
-- Aly & Horde trash:
update creature_template set scriptName = 'mob_gunship_luminary' where entry in (37016, 37031);
update creature_template set scriptName = 'mob_gunship_primalist' where entry in (37027, 37030);
update creature_template set scriptName = 'mob_gunship_sorcerer' where entry in (37026, 37033);
update creature_template set scriptName = 'mob_gunship_assasin' where entry in (37017, 37028);
update creature_template set scriptName = 'mob_gunship_protector' where entry in (36998, 37032);
update creature_template set scriptName = 'mob_gunship_marksman' where entry in (37144, 37146);
update creature_template set scriptName = 'mob_gunship_dreadblade' where entry in (37004, 37029);
update creature_template set scriptName = 'mob_gunship_vindicator' where entry in (37003, 37035);
update creature_template set scriptName = 'mob_gunship_templar' where entry in (37021, 37034);
update creature_template set scriptName = 'mob_gunship_necrolyte' where entry in (37148, 37149);
-- Rampart trash
UPDATE creature_template SET ScriptName = 'mob_spire_frostwyrm' WHERE entry = 37230;
UPDATE creature_template SET ScriptName = 'mob_rotting_frost_giant' WHERE entry in (38494, 38490);
update creature_template set scriptName = 'mob_spire_minion' where entry = 37545;
update creature_template set scriptName = 'mob_spire_gargoyle' where entry = 37544;
update creature_template set scriptName = 'mob_frenzied_abomination' where entry = 37546;
-- Oratory trash
update creature_template set scriptName = 'mob_deathspeaker_zealot' where entry = 36808;
update creature_template set scriptName = 'mob_deathspeaker_servant' where entry = 36805;
update creature_template set scriptName = 'mob_deathspeaker_attendant' where entry = 36811;
update creature_template set scriptName = 'mob_deathspeaker_high_priest' where entry = 36829;
update creature_template set scriptName = 'mob_deathspeaker_disciple' where entry = 36807;
-- Entrance trash
update creature_template set scriptName = 'mob_deathbound_ward' where entry = 37007;
update creature_template set scriptName = 'mob_servant_of_the_throne' where entry = 36724;
update creature_template set scriptName = 'mob_nerubar_broodkeeper' where entry = 36725;
update creature_template set scriptName = 'mob_ancient_skeletal_soldier' where entry = 37012;
update creature_template set scriptName = 'mob_the_damned' where entry = 37011;

-- Instance
UPDATE instance_template SET scriptName ='instance_icecrown_citadel' WHERE map = 631;

-- Trash respawn:
update creature set spawntimesecs = 7200 where id in (36725, 36808, 36805, 36811, 36829, 36807, 37016, 37031, 37027, 37030, 37026, 37033, 37017, 37028, 36998, 37032,
37144, 37146, 37004, 37029, 37003, 37035, 37021, 37034, 37148, 37149, 37098, 37664, 37595, 37663, 37571, 37665, 37666, 37662, 37132, 38125, 37133, 37134, 37127);

-- Instance teleporters: light's hammer, oratory of dammed, rampart of skulls, deathbringers rise, sindragosa, central spire, upper spire;
UPDATE gameobject_template SET ScriptName="icecrown_citadel_teleporter", `flags` = 32 WHERE entry in (202242, 202243, 202244, 202245, 202246, 202223, 202235);

-- In oder to proper activate this, delete id 5718 from areatrigger_teleport
delete from areatrigger_teleport where id = 5718;
UPDATE `gameobject_template` SET `flags` = 32 WHERE `entry` = 202223; -- original flag = 48

-- Delete the buff from DB: will be added later
delete from spell_area where spell in (73822, 73828);
#insert into spell_area values
#(73822,4812,0,0,0,0,690,2,1), 	-- horde
#(73828,4812,0,0,0,0,1101,2,1);	-- aly
-- buff info:
-- aly - horde - percent
-- 73762 - 73816 - 5%
-- 73824 - 73818 - 10%
-- 73825 - 73819 - 15%
-- 73826 - 73820 - 20%
-- 73827 - 73821 - 25%
-- 73828 - 73822 - 30%

-- doors
update gameobject_template set faction = 114 where entry in (201857, 201563, 201370, 201371, 201372, 201618, 201617, 201614, 201613,
201182, 202181, 202183, 201377, 201378);
update gameobject_template set faction = 0 where entry in (201920, 201919);

-- loot
-- deathbringers & dreamwalkers loot
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (202238, 202239, 202240, 202241, 201959, 202338, 202339, 202340);
update gameobject set spawntimesecs = -604800 where id in (202238, 202239, 202240, 202241, 201959, 202338, 202339, 202340);
-- gunship battle loot -> not on the map yet
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (201872, 201873, 201874, 201875, 202177, 202178, 202179, 202180);
#update gameobject set spawntimesecs = -604800 where id in (201872, 201873, 201874, 201875, 202177, 202178, 202179, 202180);

-- game tele
delete from game_tele where id = 1502;
insert into game_tele values (1502, 5867.100, 2107.9404, 635.998, 3.582, 571, 'IcecrownCitadel');