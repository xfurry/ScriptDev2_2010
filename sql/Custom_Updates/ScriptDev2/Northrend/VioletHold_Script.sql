 /* VIOLET HOLD */
delete from script_texts where entry between -1608044 and -1608010;
INSERT IGNORE INTO `script_texts` (`entry`,`content_default`,`sound`,`type`,`language`,`comment`) VALUES
-- Cyanigosa
   (-1608050, 'We finish this now, champions of Kirin Tor!', 13947, 1, 0, 'cyanigosa SAY_AGGRO'),
   (-1608051, 'I will end the Kirin Tor!', 13952, 1, 0, 'cyanigosa SAY_SLAY_1'),
   (-1608052, 'Dalaran will fall!', 13953, 1, 0, 'cyanigosa SAY_SLAY_2'),
   (-1608053, 'So ends your defiance of the Spell-Weaver!', 13954, 1, 0, 'cyanigosa SAY_SLAY_3'),
   (-1608054, 'Perhaps... we have... underestimated... you.', 13955, 1, 0, 'cyanigosa SAY_DEATH'),
   (-1608055, 'A valiant defense, but this city must be razed. I will fulfill Malygos\'s wishes myself!', 13946, 1, 0, 'cyanigosa SAY_SPAWN'),
   (-1608056, 'Am I interrupting?', 13951, 1, 0, 'cyanigosa SAY_DISRUPTION'),
   (-1608057, 'Shiver and die!', 13948, 1, 0, 'cyanigosa SAY_BREATH_ATTACK'),
   (-1608058, 'The world has forgotten what true magic is! Let this be a reminder!', 13949, 1, 0, 'cyanigosa SAY_SPECIAL_ATTACK_1'),
   (-1608059, 'Who among you can withstand my power?', 13950, 1, 0, 'cyanigosa SAY_SPECIAL_ATTACK_2'),
-- Erekem
   (-1608010, 'Notcawwget in way ofrrak-rrakflee!', 14219, 1, 0, 'erekem SAY_AGGRO'),
   (-1608011, '...', 14222, 1, 0, 'erekem SAY_SLAY_1'),
   (-1608012, 'Precious life ... wasted.', 14223, 1, 0, 'erekem SAY_SLAY_2'),
   (-1608013, 'Only strong ... survive.', 14224, 1, 0, 'erekem SAY_SLAY_3'),
   (-1608014, 'Nokaw, kawflee...', 14225, 1, 0, 'erekem SAY_DEATH'),
   (-1608015, 'Free tommfly onw. Ra-aak... Not find usekh-ekh! Escape!', 14218, 1, 0, 'erekem SAY_SPAWN'),
   (-1608016, 'My-raaakfavorite! Awk awk awk! Raa-kaa!', 14220, 1, 0, 'erekem SAY_ADD_KILLED'),
   (-1608017, 'Nasty little...A-ak, kaw! Kill! Yes, kill you!', 14221, 1, 0, 'erekem SAY_BOTH_ADDS_KILLED'),
-- Ichoron
   (-1608018, 'Stand aside, mortals!', 14230, 1, 0, 'ichoron SAY_AGGRO'),
   (-1608019, 'I am a force of nature!', 14234, 1, 0, 'ichoron SAY_SLAY_1'),
   (-1608020, 'I shall pass!', 14235, 1, 0, 'ichoron SAY_SLAY_2'),
   (-1608021, 'You can not stop the tide!', 14236, 1, 0, 'ichoron SAY_SLAY_3'),
   (-1608022, 'I... recede.', 14237, 1, 0, 'ichoron SAY_DEATH'),
   (-1608023, 'I... am fury... unrestrained!', 14239, 1, 0, 'ichoron SAY_SPAWN'),
   (-1608024, 'I shall consume, decimate, devastate, and destroy! Yield now to the wrath of the pounding sea!', 14231, 1, 0, 'ichoron SAY_ENRAGE'),
   (-1608025, 'I will not be contained! Ngyah!!', 14233, 1, 0, 'ichoron SAY_SHATTER'),
   (-1608026, 'Water can hold any form, take any shape... overcome any obstacle.', 14232, 1, 0, 'ichoron SAY_BUBBLE'),
-- Xevozz
   (-1608027, 'It seems my freedom must be bought with blood...', 14499, 1, 0, 'xevozz SAY_AGGRO'),
   (-1608028, 'Nothing personal.', 14504, 1, 0, 'xevozz SAY_SLAY_1'),
   (-1608029, 'Business concluded.', 14505, 1, 0, 'xevozz SAY_SLAY_2'),
   (-1608030, 'Profit!', 14506, 1, 0, 'xevozz SAY_SLAY_3'),
   (-1608031, 'This is an... unrecoverable... loss.', 14507, 1, 0, 'xevozz SAY_DEATH'),
   (-1608032, 'Back in business! Now to execute an exit strategy.', 14498, 1, 0, 'xevozz SAY_SPAWNED'),
   (-1608033, 'It would seem that a renegotiation is in order.', 14503, 1, 0, 'xevozz SAY_CHARGED'),
   (-1608034, 'The air teems with latent energy... quite the harvest!', 14501, 1, 0, 'xevozz SAY_REPEAT_SUMMON_1'),
   (-1608035, 'Plentiful, exploitable resources... primed for acquisition!', 14502, 1, 0, 'xevozz SAY_REPEAT_SUMMON_2'),
   (-1608036, 'Intriguing... a high quantity of arcane energy is near. Time for some prospecting...', 14500, 1, 0, 'xevozz SAY_SUMMON_ENERGY'),
-- Zuramat
   (-1608037, 'Eradicate.', 13996, 1, 0, 'zuramat SAY_AGGRO'),
   (-1608038, 'More... energy.', 13999, 1, 0, 'zuramat SAY_SLAY_1'),
   (-1608039, 'Relinquish.', 14000, 1, 0, 'zuramat SAY_SLAY_2'),
   (-1608040, 'Fall... to shadow.', 14001, 1, 0, 'zuramat SAY_SLAY_3'),
   (-1608041, 'Disperse.', 14002, 1, 0, 'zuramat SAY_DEATH'),
   (-1608042, 'I am... renewed.', 13995, 1, 0, 'zuramat SAY_SPAWN'),
   (-1608043, 'Know... my... pain.', 13997, 1, 0, 'zuramat SAY_SHIELD'),
   (-1608044, 'Gaze... into the void.', 13998, 1, 0, 'zuramat SAY_WHISPER');
   
-- sinclari waypoints: rewrite
DELETE FROM script_waypoint WHERE entry=30658;
INSERT INTO script_waypoint VALUES
(30658, 0, 1830.504517, 799.356506, 44.341801, 5000, 'use activation'),
(30658, 1, 1832.461792, 800.431396, 44.311745, 10000, 'SAY_BEGIN call back guards'),
(30658, 2, 1824.786987, 803.828369, 44.363434, 0, 'SAY_LOCK_DOOR close door'),
(30658, 3, 1807.245483, 803.904114, 44.363434, 0, ''),
(30658, 4, 1785.160400, 803.856873, 44.364830, 30000, '');