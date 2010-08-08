/* TRIAL OF THE CRUSADER */
-- event
UPDATE instance_template SET scriptName='instance_trial_of_the_crusader' WHERE map=649;
UPDATE creature_template SET npcflag=1, flags_extra = 1, scriptname='npc_crusader_anouncer' WHERE `entry`=34816;
update creature set spawntimesecs = 604800 where id = 34816;
UPDATE creature_template SET flags_extra = 1, scriptname='npc_lich_king' WHERE `entry`=35877;
UPDATE creature_template SET scriptname='npc_tirion' WHERE `entry`=34996;
delete from gameobject where id in (195485);
insert into gameobject VALUES (110011,195485,649,15,65535,665.490,136.864,133.594,6.27,0,0,0,0,604800,0,1);
update gameobject_template set faction = 114 where entry = 195485;
update creature_template set flags_extra = 258, unit_flags = 2 where entry in (34994, 34992, 34816, 34996, 34995, 34990);
-- beasts
UPDATE creature_template SET scriptname='boss_gormok' WHERE `entry`=34796;
UPDATE creature_template SET scriptname='mob_snobold_vassal', flags_extra = 256 WHERE `entry`=34800;
UPDATE creature_template SET scriptname='boss_icehowl' WHERE `entry`=34797;
UPDATE creature_template SET scriptname='boss_acidmaw' WHERE `entry`=35144;
UPDATE creature_template SET scriptname='boss_dreadscale' WHERE `entry`=34799;
UPDATE creature_template SET scriptname='mob_slime_pool' WHERE `entry`=35176;
UPDATE creature_template SET scriptname='mob_toc_fire_bomb' WHERE `entry`=34854;
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('66636', '1', '34796');
-- Jaru
UPDATE creature_template SET mechanic_immune_mask = 752566015, equipment_id = 104000, scriptname='boss_jaraxxus' WHERE `entry` in (34780, 12075, 12077, 12078);
delete from creature_equip_template where entry = 104000;
insert into creature_equip_template values (104000, 42139, 0, 0);
UPDATE creature_template SET `faction_A` = 35, `faction_H` = 35, scriptname='mob_legion_flame' WHERE `entry`=34784;
UPDATE creature_template SET scriptname='mob_infernal_volcano' WHERE `entry`=34813;
UPDATE creature_template SET scriptname='mob_felflame_infernal' WHERE `entry`=34815;
UPDATE creature_template SET scriptname='mob_nether_portal' WHERE `entry`=34825;
UPDATE creature_template SET scriptname='mob_mistress_of_pain' WHERE `entry`=34826;
UPDATE creature_template SET scriptname='npc_jaina' WHERE `entry`=34992;
-- Champions
UPDATE creature_template SET scriptname='boss_fc_resto_druid' WHERE `entry` in (34469, 34459);
UPDATE creature_template SET scriptname='boss_fc_resto_shammy' WHERE `entry` in (34470, 34444);
UPDATE creature_template SET scriptname='boss_fc_holy_pally' WHERE `entry` in (34465, 34445);
UPDATE creature_template SET scriptname='boss_fc_holy_priest' WHERE `entry` in (34466, 34447);
UPDATE creature_template SET scriptname='boss_fc_shadow_priest' WHERE `entry` in (34473, 34441);
UPDATE creature_template SET scriptname='boss_fc_mage' WHERE `entry` in (34468, 34449);
UPDATE creature_template SET scriptname='boss_fc_warlock' WHERE `entry` in (34474, 34450);
UPDATE creature_template SET scriptname='boss_fc_hunter' WHERE `entry` in (34467, 34448);
UPDATE creature_template SET scriptname='boss_fc_boomkin' WHERE `entry` in (34460, 34451);
UPDATE creature_template SET scriptname='boss_fc_warrior' WHERE `entry` in (34475, 34453);
UPDATE creature_template SET scriptname='boss_fc_rogue' WHERE `entry` in (34472, 34454);
UPDATE creature_template SET scriptname='boss_fc_enh_shammy' WHERE `entry` in (34463, 34455);
UPDATE creature_template SET scriptname='boss_fc_ret_pally' WHERE `entry` in (34471, 34456);
UPDATE creature_template SET flags_extra = 1, scriptname='boss_fc_pet_zhaagrym' WHERE `entry` = 35465;
UPDATE creature_template SET flags_extra = 1, scriptname='boss_fc_pet_cat' WHERE `entry` = 35610;
-- Valkyr twins
UPDATE creature_template SET mechanic_immune_mask = 635387903, scriptname='boss_fjola' WHERE `entry`=34497;
UPDATE creature_template SET scriptname='boss_eydis' WHERE `entry`=34496;
UPDATE creature_template SET scriptname='mob_valkyr_orb' WHERE `entry` in (34630, 34628);
UPDATE creature_template SET scriptname='mob_valkyr_essence' WHERE `entry` in (34567, 34568);
-- spells
-- tar eydis
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('65876', '1', '34496');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67307', '1', '34496');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67306', '1', '34496');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67308', '1', '34496');
-- tar fjola
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('65875', '1', '34497');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67304', '1', '34497');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67303', '1', '34497');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('67305', '1', '34497');
-- Anub
UPDATE creature_template SET scriptname='boss_anubarak_trial' WHERE `entry`=34564;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, scriptname='mob_frost_sphere' WHERE `entry`=34606;
UPDATE creature_template SET scriptname='mob_nerubian_burrower' WHERE `entry`=34607;
UPDATE creature_template SET scriptname='mob_swarm_scarab' WHERE `entry`=34605;
UPDATE creature_template SET scriptname='mob_anub_spike' WHERE `entry`=34660;
UPDATE creature_template SET npcFlag = 1, scriptname='npc_tirion_end' WHERE `entry`=36095;
-- loot chests
update `gameobject` set `position_x` = 679.531, `position_y` = 137.507, `position_z` = 142.129, `orientation` = 6.215 where `id` in (195665, 195666, 195667, 195668, 195669, 195670, 195671, 195672);
UPDATE gameobject_template SET faction = 0, data15 = 1 WHERE entry in (195665, 195666, 195667, 195668, 195669, 195670, 195671, 195672, 195631, 195632, 195633, 195635);