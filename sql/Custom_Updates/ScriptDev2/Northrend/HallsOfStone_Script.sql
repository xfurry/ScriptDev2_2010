/* HALLS OF STONE */
delete from `script_texts` where `entry` in (-1599065,-1599066,-1599067,-1599068);
INSERT INTO `script_texts` (`entry`,`content_default`,`sound`,`type`,`language`,`comment`) VALUES
-- Krystallus
   (-1599065, 'Crush....', 14176, 1,0,'krystallus SAY_AGGRO'),
   (-1599066, 'Ha...ha...ha...ha...', 14177,1,0, 'krystallus SAY_KILL'),
   (-1599067, 'Uuuuhhhhhhhhhh......', 14179,1,0, 'krystallus SAY_DEATH'),
   (-1599068, 'Break.... you....', 14178,1,0, 'krystallus SAY_SHATTER');
