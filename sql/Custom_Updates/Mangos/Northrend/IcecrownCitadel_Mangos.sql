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
-- Sindragosa
update creature_template set scriptName = 'miniboss_spinestalker' where entry = 37534;
update creature_template set scriptName = 'miniboss_rimefang' where entry = 37533;
-- Lich King

-- Minibossed: Stinky, Precious, Svalna
UPDATE creature_template SET ScriptName = 'miniboss_stinky' WHERE entry = 37025;
UPDATE creature_template SET ScriptName = 'miniboss_precious' WHERE entry = 37217;

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
UPDATE instance_template SET script='instance_icecrown_citadel' WHERE map = 631;

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