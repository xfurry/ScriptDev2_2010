-- instance
UPDATE instance_template SET scriptName = 'instance_trial_of_the_champion' WHERE map = 650;

-- announcers
UPDATE creature_template SET npcflag = 1, ScriptName = 'npc_toc5_announcer' WHERE entry IN (35004, 35005);

-- grand champions
UPDATE creature_template SET ScriptName = 'mob_toc5_warrior' WHERE entry IN (34705, 35572);
UPDATE creature_template SET ScriptName = 'mob_toc5_mage' WHERE entry IN (34702, 35569);
UPDATE creature_template SET ScriptName = 'mob_toc5_shaman' WHERE entry IN (34701, 35571);
UPDATE creature_template SET ScriptName = 'mob_toc5_hunter' WHERE entry IN (34657, 35570);
UPDATE creature_template SET ScriptName = 'mob_toc5_rogue' WHERE entry IN (34703, 35617);

-- argent challenge
UPDATE creature_template SET ScriptName = 'boss_eadric' WHERE entry = 35119;
UPDATE creature_template SET ScriptName = 'boss_paletress' WHERE entry = 34928;
UPDATE creature_template SET ScriptName = 'mob_toc_memory' WHERE entry IN
(35052, 35041, 35033, 35046, 35043, 35047, 35044, 35039, 35034, 35049, 35030, 34942, 35050,
 35042, 35045, 35037, 35031, 35038, 35029, 35048, 35032, 35028, 35040, 35036, 35051);
 UPDATE creature_template SET ScriptName = 'mob_toc_argent_trash' WHERE entry in (35309, 35305, 35307);

-- black knight
UPDATE creature_template SET ScriptName = 'mob_toc_risen_ghoul' WHERE entry IN (35545, 35564, 35590);
UPDATE creature_template SET ScriptName = 'boss_black_knight' WHERE entry = 35451;
UPDATE creature_template SET ScriptName = 'mob_toc_desecration_bunny' WHERE entry = 35614;
