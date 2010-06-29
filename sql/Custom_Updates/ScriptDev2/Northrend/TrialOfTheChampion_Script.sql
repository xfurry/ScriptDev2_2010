/* TRIAL OF THE CRUSADER */
delete from `script_texts` where `entry` between -1614050 and -1614000;
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES 
-- Intro:
-- Encounter 2 intro:
(-1614000,"Well fought! Your next challenge comes from the Crusade's own ranks. You will be tested against their considerable prowess.","","","","","","","","",0,1,0,0,"tirion intro"),
(-1614001,"The next combatant is second to none in her passion for upholding the Light. I give you Argent Confessor Paletress!","","","","","","","","",0,1,0,0,"announcer paletress intro"),
(-1614002,"The Alliance spectators cheer for Argent Confessor Paletress.","","","","","","","","",0,2,0,0,"aly intro"),
(-1614003,"The Horde spectators cheer for Argent Confessor Paletress.","","","","","","","","",0,2,0,0,"horde intro"),
(-1614005,"You may begin!","","","","","","","","",0,1,0,0,"tirion begin"),
-- Eadric:
(-1614010,"Are you up to the challenge? I will not hold back!","","","","","","","","",16134,0,0,0,"eadric intro"),
(-1614011,"Prepare yourselves!","","","","","","","","",16135,1,0,0,"eadric aggro"),
(-1614012,"Hammer of the Righteous!","","","","","","","","",16136,1,0,0,"eadric hammer"),
(-1614013,"You... You need more practice.","","","","","","","","",16137,1,0,0,"eadric kill1"),
(-1614014,"Neigh! Neigh! And I say yet again NEIGH! Not good enough!","","","","","","","","",16138,1,0,0,"eadric kill2"),
(-1614015,"I yield! I submit. Excellent work. May I run away now?","","","","","","","","",16139,1,0,0,"eadric defeat"),
-- Paletress:
(-1614020,"Thank you, good herald. Your words are too kind.","","","","","","","","",16245,0,0,0,"paletress intro1"),
(-1614021,"May the Light give me strength to provide a worthy challenge.","","","","","","","","",16246,0,0,0,"paletress intro2"),
(-1614022,"Well then, let us begin.","","","","","","","","",16247,1,0,0,"paletress aggro"),
(-1614023,"Take this time to consider your past deeds.","","","","","","","","",16248,1,0,0,"summon memory"),
(-1614024,"Even the darkest memory fades when confronted.","","","","","","","","",16249,1,0,0,"paletress memory dies"),
(-1614025,"Take your rest.","","","","","","","","",16250,1,0,0,"paletress kill1"),
(-1614026,"Be at ease.","","","","","","","","",16251,1,0,0,"paletress kill2"),
(-1614027,"Excellent work!","","","","","","","","",16252,1,0,0,"paletress defeat"),
-- Black Knight:
(-1614030,"Well done. You have proven yourself today-","","","","","","","","",0,1,0,0,"tirion intro1"),
(-1614031,"What's that, up near the rafters?","","","","","","","","",0,0,0,0,"announcer intro"),
(-1614032,"You spoiled my grand entrance, rat.","","","","","","","","",16256,0,0,0,"dk intro"),
(-1614033,"The Black Knight casts Death's Respite on the herald, killing him.","","","","","","","","",0,2,0,0,"dk intro2"),
(-1614034,"What is the meaning of this?","","","","","","","","",0,1,0,0,"tirion intro2"),
(-1614035,"Did you honestly think that an agent of the Lich King would be bested on the field of your pathetic little tournament?","","","","","","","","",16257,0,0,0,"dk intro2"),
(-1614036,"I've come to finish my task.","","","","","","","","",16258,0,0,0,"dk intro 3"),
(-1614037,"This farce ends here!","","","","","","","","",16259,1,0,0,"dk aggro"),
(-1614038,"Don't just stand there; kill him!","","","","","","","","",0,1,0,0,"king intro"),
(-1614039,"My rotting flesh was just getting in the way!","","","","","","","","",16262,1,0,0,"dk phase2"),
(-1614040,"I have no need for bones to best you!","","","","","","","","",16263,1,0,0,"dk phase 3"),
(-1614041,"A waste of flesh.","","","","","","","","",16260,1,0,0,"dk kill1"),
(-1614042,"Pathetic.","","","","","","","","",16261,1,0,0,"dk kill2"),
(-1614043,"No! I must not fail... again...","","","","","","","","",16264,1,0,0,"dk dies"),
(-1614044,"My congratulations, champions. Through trials both planned and unexpected, you have triumphed.","","","","","","","","",0,1,0,0,"tirion outro1"),
(-1614045,"Go now and rest; you've earned it.!","","","","","","","","",0,1,0,0,"tirion outro2"),
(-1614046,"You fought well.","","","","","","","","",0,1,0,0,"varian outro"),
(-1614047,"Well done Horde!","","","","","","","","",0,1,0,0,"gorash outro");