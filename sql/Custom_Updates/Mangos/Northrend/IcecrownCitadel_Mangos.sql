/* ICECROWN CITADEL */

-- Marrowgar
UPDATE creature_template SET ScriptName = 'boss_marrowgar' WHERE entry = 36612;
UPDATE creature_template SET ScriptName = 'mob_coldflame' WHERE entry = 36672;
UPDATE creature_template SET ScriptName = 'mob_bone_spike' WHERE entry = 38711;

-- Deathwhisper
UPDATE creature_template SET mechanic_immune_mask = 634339327, ScriptName = 'boss_deathwhisper' WHERE entry = 36855;
UPDATE creature_template SET ScriptName = 'mob_vengeful_shade' WHERE entry = 38222;
UPDATE creature_template SET ScriptName = 'mob_cult_adherent' WHERE entry = 37949;
UPDATE creature_template SET ScriptName = 'mob_cult_fanatic' WHERE entry = 37890;

-- Deathbringer Saurfang
UPDATE creature_template SET ScriptName = 'boss_saurfang' WHERE entry = 37813;
UPDATE creature_template SET ScriptName = 'mob_blood_beast' WHERE entry = 38508;

-- Festergut
UPDATE creature_template SET ScriptName = 'boss_festergut' WHERE entry = 36626;

-- Rotface
UPDATE creature_template SET ScriptName = 'boss_rotface' WHERE entry = 36627;
UPDATE creature_template SET ScriptName = 'mob_small_ooze' WHERE entry = 36897;
UPDATE creature_template SET ScriptName = 'mob_big_ooze' WHERE entry = 36899;
UPDATE creature_template SET ScriptName = 'mob_sticky_ooze' WHERE entry = 37006;

-- Putricide
UPDATE creature_template SET ScriptName = 'boss_professor_putricide' WHERE entry = 36678;
UPDATE creature_template SET ScriptName = 'mob_icc_volatile_ooze' WHERE entry = 37697;
UPDATE creature_template SET ScriptName = 'mob_icc_gas_cloud' WHERE entry = 37562;
UPDATE creature_template SET ScriptName = 'mob_icc_gas_bomb' WHERE entry = 38159;
UPDATE creature_template SET ScriptName = 'mob_malleable_ooze' WHERE entry = 38556;
UPDATE creature_template SET ScriptName = 'mob_slime_puddle' WHERE entry = 38234;

-- Blood prince council
UPDATE creature_template SET ScriptName = 'boss_prince_taldaram' WHERE entry = 37973;
UPDATE creature_template SET ScriptName = 'boss_prince_keleseth' WHERE entry = 37972;
UPDATE creature_template SET ScriptName = 'boss_prince_valanar' WHERE entry = 37970;

-- Blood queen Lanathel
UPDATE creature_template SET ScriptName = 'boss_blood_queen_lanathel' WHERE entry = 37955;

-- Valithria Dreamwalker
update creature_template set scriptName = 'mob_risen_archmage' where entry = 37868;
update creature_template set scriptName = 'mob_blazing_skeleton' where entry = 36791;
update creature_template set scriptName = 'mob_suppresser' where entry = 37863;
update creature_template set scriptName = 'mob_blistering_zombie' where entry = 37934;
update creature_template set scriptName = 'mob_gluttonous_abomination' where entry = 37886;
-- Sindragosa
update creature_template set scriptName = 'boss_sindragosa' where entry = 36853;
update creature_template set scriptName = 'miniboss_spinestalker' where entry = 37534;
update creature_template set scriptName = 'miniboss_rimefang' where entry = 37533;

-- Lich King
update creature_template set scriptName = 'boss_the_lich_king' where entry = 36597;
update creature_template set npcflag = 1, scriptName = 'npc_tirion_final' where entry = 38995;
-- LK platform
-- delete from gameobject where id in (202438);
-- insert into gameobject VALUES (115000,202438,631,16,65535,494.463, -2520.409 ,1040.860,3.275,0,0,0,0,604800,0,1);

-- Minibossed: Stinky, Precious, Svalna
UPDATE creature_template SET ScriptName = 'miniboss_stinky' WHERE entry = 37025;
UPDATE creature_template SET ScriptName = 'miniboss_precious' WHERE entry = 37217;

-- Frostwing
update creature_template set scriptName = 'miniboss_sister_svalna' where entry = 37126;
update creature_template set scriptName = 'mob_crok_scourgebane' where entry = 37129;
update creature_template set scriptName = 'mob_captain_arnath' where entry = 37122;
update creature_template set scriptName = 'mob_captain_brandon' where entry = 37123;
update creature_template set scriptName = 'mob_captain_grondel' where entry = 37124;
update creature_template set scriptName = 'mob_captain_rupert' where entry = 37125;
update creature_template set scriptName = 'mob_ymirjar_battlemaiden' where entry = 37132;
update creature_template set scriptName = 'mob_ymirjar_deathbringer' where entry = 38125;
update creature_template set scriptName = 'mob_ymirjar_warlord' where entry = 37133;
update creature_template set scriptName = 'mob_ymirjar_huntress' where entry = 37134;
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

-- Instance teleporters: light's hammer, oratory of dammed, rampart of skulls, deathbringers rise, sindragosa, central spire, upper spire;
UPDATE gameobject_template SET ScriptName="icecrown_citadel_teleporter" WHERE entry in (202242, 202243, 202244, 202245, 202246, 202223, 202235);
-- In oder to proper activate this, delete id 5718 from areatrigger_teleport
#delete from areatrigger_teleport where id = 5718;
UPDATE `gameobject_template` SET `flags` = 32 WHERE `entry` = 202223; -- original flag = 48
update gameobject_template set faction = 114 where entry in (201857, 201563, 201370, 201371, 201372, 201618, 201617, 201614, 201613,
201182, 202181, 202183);
-- deathbringers & dreamwalkers loot
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (202238, 202239, 202240, 202241, 201959, 202338, 202339, 202340);
update gameobject set spawntimesecs = -604800 where id in (202238, 202239, 202240, 202241, 201959, 202338, 202339, 202340);
-- gunship battle loot -> not on the map yet
UPDATE gameobject_template SET faction = 0, flags = 0 WHERE entry in (201872, 201873, 201874, 201875, 202177, 202178, 202179, 202180);
#update gameobject set spawntimesecs = -604800 where id in (201872, 201873, 201874, 201875, 202177, 202178, 202179, 202180);
-- game tele
delete from game_tele where id = 1502;
insert into game_tele values (1502, 5867.100, 2107.9404, 635.998, 3.582, 571, 'IcecrownCitadel');