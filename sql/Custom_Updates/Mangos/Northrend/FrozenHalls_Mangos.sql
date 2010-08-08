/* FORGE OF SOULS */
-- Trash
UPDATE `creature_template` SET `ScriptName`='npc_SpitefulApparition' WHERE `entry`=36551;
UPDATE `creature_template` SET `ScriptName`='npc_SpectralWarden' WHERE `entry`=36666;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardWatchman' WHERE `entry`=36478;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardReaper' WHERE `entry`=36499;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardBonecaster' WHERE `entry`=36564;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardAnimator' WHERE `entry`=36516;
UPDATE `creature_template` SET `ScriptName`='npc_SoulguardAdept' WHERE `entry`=36620;
UPDATE `creature_template` SET `ScriptName`='npc_SoulHorror' WHERE `entry`=36522;
-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_Bronjahn' WHERE `entry`=36497;
UPDATE `creature_template` SET `ScriptName`='mob_corrupted_soul_fragment' WHERE `entry`=36535;
UPDATE `creature_template` SET mechanic_immune_mask = 764362751, `ScriptName`='boss_Devourer_of_Souls' WHERE `entry`=36502;
UPDATE `creature_template` SET `ScriptName`='mob_well_of_souls' WHERE `entry`=36536;
-- Npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_fos_start' WHERE `entry` in (37596, 37597);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_fos_end' WHERE `entry` in (38160, 38161);
update creature set spawnMask = 0 where id in (37584, 37497, 37496, 37588, 38161, 38160);

/* PIT OF SARON */
-- Trash
UPDATE `creature_template` SET `ScriptName`='npc_YmirjarWrathbringer' WHERE `entry`=36840;
UPDATE `creature_template` SET `ScriptName`='npc_YmirjarSkycaller' WHERE `entry`=31260;
UPDATE `creature_template` SET `ScriptName`='npc_YmirjarFlamebearer' WHERE `entry`=36893;
UPDATE `creature_template` SET `ScriptName`='npc_YmirjarDeathbringer' WHERE `entry`=36892;
UPDATE `creature_template` SET `ScriptName`='npc_WrathboneLaborer' WHERE `entry`=36830;
UPDATE `creature_template` SET `ScriptName`='npc_WrathboneColdwraith' WHERE `entry`=36842;
UPDATE `creature_template` SET `ScriptName`='npc_StonespineGargoyle' WHERE `entry`=36896;
UPDATE `creature_template` SET `ScriptName`='npc_PlaguebornHorror' WHERE `entry`=36879;
UPDATE `creature_template` SET `ScriptName`='npc_IcebornProtoDrake' WHERE `entry`=36891;
UPDATE `creature_template` SET `ScriptName`='npc_HungeringGhoul' WHERE `entry`=37711;
UPDATE `creature_template` SET `ScriptName`='npc_FallenWarrior' WHERE `entry`=36841;
UPDATE `creature_template` SET `ScriptName`='npc_DeathwhisperTorturer' WHERE `entry`=37713;
UPDATE `creature_template` SET `ScriptName`='npc_DeathwhisperShadowcaster' WHERE `entry`=37712;
UPDATE `creature_template` SET `ScriptName`='npc_DeathwhisperNecrolyte' WHERE `entry`=36788;
UPDATE `creature_template` SET `ScriptName`='npc_WrathboneSorcerer' WHERE `entry`=37728;
UPDATE `creature_template` SET `ScriptName`='npc_GeistAmbusher' WHERE `entry`=36886;
UPDATE `creature_template` SET `ScriptName`='npc_disturbedGlacialRevenant' WHERE `entry`=36874;
-- Areatrigger: maybe wrong id :(
delete from areatrigger_scripts where entry = 5578;
insert into areatrigger_scripts values (5578, 'at_tyrannus');
-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_Garfrost' WHERE `entry`=36494;
UPDATE `creature_template` SET `ScriptName`='boss_Ick' WHERE `entry`=36476;
UPDATE `creature_template` SET `ScriptName`='boss_Krick' WHERE `entry`=36477;
UPDATE `creature_template` SET `ScriptName`='mob_explosive_orb' WHERE `entry`=36610;
UPDATE `creature_template` SET `ScriptName`='boss_Tyrannus' WHERE `entry`=36658;
UPDATE `creature_template` SET `ScriptName`='boss_Rimefang' WHERE `entry`=36661;
UPDATE `creature_template` SET `ScriptName`='mob_icy_blast' WHERE `entry`=36731;
-- npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_start' WHERE `entry` in (36993, 36990);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_end' WHERE `entry` in (38188, 38189);
UPDATE `creature_template` SET `ScriptName`='npc_martin_gorkun' WHERE `entry` in (37591, 37592);
update creature set spawnMask = 0 where id in (37591, 37592, 38188, 38189);
update creature set spawnMask = 0 where guid in (123745, 123584, 123548, 123585);
UPDATE gameobject_template SET faction = 114, data10 = 1 WHERE entry = 196485;
-- spells
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69347', '1', '36794');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70521', '1', '37581');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70521', '1', '37580');

/* HALLS OF REFLECTION */
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('69708', '1', '37226');
REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('70194', '1', '37226');
-- trash
UPDATE `creature_template` SET `ScriptName`='mob_frostsworn_general' WHERE `entry`=36723;
UPDATE `creature_template` SET `ScriptName`='mob_lichKing_minion' WHERE `entry` in (36940, 36941, 37069);
UPDATE `creature_template` SET `ScriptName`='mob_hallsOfReflectionSoul' WHERE `entry` in (38176, 38175, 38172, 38567, 38177, 38173);
-- boss
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
UPDATE `creature_template` SET `ScriptName`='boss_falric' WHERE `entry`=38112;
-- npc
update creature set position_x = 5550.833984, position_y = 2260.596191, position_z = 733.011414, orientation = 3.934108 where id = 37226;
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_start' WHERE `entry` in (37221, 37223);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_hor_end' WHERE `entry` in (36955, 37554);
UPDATE `creature_template` SET `ScriptName`='npc_lich_king_hor_start' WHERE `entry`=36954;
UPDATE `creature_template` SET `ScriptName`='boss_lich_king_hor_end' WHERE `entry`=37226;
update creature set spawnMask = 0 where guid in (135387, 135386);
update creature set spawnMask = 0 where id in (36940, 36941, 37069);
update creature set spawnMask = 0 where id in (38176, 38175, 38172, 38567, 38177, 38173); -- Temporary until script fulll implemented -> spawnMask should be 3 and the npc invisible until the event starts

update instance_template set scriptName = 'instance_halls_of_reflection' where map = 668;
update instance_template set scriptName = 'instance_pit_of_saron' where map = 658;
update instance_template set scriptName = 'instance_forge_of_souls' where map = 632;

update gameobject set spawntimesecs = -604800 where id in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 0 WHERE entry in (202212, 201710, 202337, 202336);
UPDATE gameobject_template SET faction = 114 WHERE entry in (201848, 201385, 197342, 197343, 201976, 197341, 202236, 202302, 201709);

#UPDATE `creature_template` SET `ScriptName`='generic_creature' WHERE `entry` in ();
delete from game_tele where id = 1500;
insert into game_tele values (1500, 5634.508, 2042.124, 798.270, 4.71, 571, 'FrozenHalls');
delete from game_tele where id = 1501;
insert into game_tele values (1501, 8516.319, 634.558, 547.393, 1.62, 571, 'CrusadersColliseum');