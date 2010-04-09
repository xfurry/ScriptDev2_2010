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
-- Boss
UPDATE `creature_template` SET `ScriptName`='boss_Garfrost' WHERE `entry`=36494;
UPDATE `creature_template` SET `ScriptName`='boss_Ick' WHERE `entry`=36476;
UPDATE `creature_template` SET `ScriptName`='boss_Krick' WHERE `entry`=36477;
UPDATE `creature_template` SET `ScriptName`='mob_explosive_orb' WHERE `entry`=36610;
UPDATE `creature_template` SET `ScriptName`='boss_Tyrannus' WHERE `entry`=36658;
UPDATE `creature_template` SET `ScriptName`='boss_Rimefang' WHERE `entry`=36661;
-- npc
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_start' WHERE `entry` in (36993, 36990);
UPDATE `creature_template` SET `ScriptName`='npc_slyvanas_jaina_pos_end' WHERE `entry` in (38188, 38189);
UPDATE `creature_template` SET `ScriptName`='npc_martin_gorkun' WHERE `entry` in (37591, 37592);
update creature set spawnMask = 0 where id in (37591, 37592, 38188, 38189);
update creature set spawnMask = 0 where guid in (123745, 123584, 123548, 123585);