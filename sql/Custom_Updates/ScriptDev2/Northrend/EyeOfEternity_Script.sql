DELETE FROM script_texts WHERE entry BETWEEN -1616036 AND -1615999;
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
-- Intro
('-1616000', 'Lesser beings, intruding here! A shame that your excess courage does not compensate for your stupidity!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14512', '1', '0', '457', NULL),
('-1616001', 'None but the blue dragonflight are welcome here! Perhaps this is the work of Alexstrasza? Well then, she has sent you to your deaths.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14513', '1', '0', '457', NULL),
('-1616002', 'What could you hope to accomplish, to storm brazenly into my domain? To employ MAGIC? Against ME? <Laughs>', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14514', '1', '0', '457', NULL),
('-1616003', 'I am without limits here... the rules of your cherished reality do not apply... In this realm, I am in control...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14515', '1', '0', '457', NULL),
('-1616004', 'I give you one chance. Pledge fealty to me, and perhaps I won''t slaughter you for your insolence!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14516', '1', '0', '457', NULL),
-- Phase 1
('-1616005', 'My patience has reached its limit, I WILL BE RID OF YOU!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14517', '1', '0', '1', 'AGGRO'),
('-1616006', 'Watch helplessly as your hopes are swept away...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14525', '1', '0', '1', 'VORTEX'),
('-1616007', 'I AM UNSTOPPABLE!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14533', '1', '0', '1', 'SPARK_BUFF'),
('-1616035', 'A Power Spark forms from a nearby rift!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '3', '0', '1', 'SPARK'),
('-1616008', 'Your stupidity has finally caught up to you!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14519', '1', '0', '1', 'KILL'),
('-1616009', 'More artifacts to confiscate...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14520', '1', '0', '1', 'KILL'),
('-1616010', '<Laughs> How very... naive...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14521', '1', '0', '1', 'KILL'),
('-1616012', 'I had hoped to end your lives quickly, but you have proven more...resilient then I had anticipated. Nonetheless, your efforts are in vain, it is you reckless, careless mortals who are to blame for this war! I do what I must...And if it means your...extinction...THEN SO BE IT!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14522', '1', '0', '1', 'END_P1'),
-- Phase 2
('-1616013', 'Few have experienced the pain I will now inflict upon you!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14523', '1', '0', '1', 'AGGRO2'),
('-1616014', 'YOU WILL NOT SUCCEED WHILE I DRAW BREATH!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14523', '1', '0', '1', 'BREATH'),
('-1616015', 'Malygos takes a deep breath...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '0', '3', '0', '1', NULL),
('-1616016', 'I will teach you IGNORANT children just how little you know of magic...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14524', '1', '0', '1', 'SHELL'),
('-1616020', 'Your energy will be put to good use!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14526', '1', '0', '1', 'KILL'),
('-1616021', 'I AM THE SPELL-WEAVER! My power is INFINITE!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14527', '1', '0', '1', 'KILL'),
('-1616022', 'Your spirit will linger here forever!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14528', '1', '0', '1', 'KILL'),
('-1616017', 'ENOUGH! If you intend to reclaim Azeroth\'s magic, then you shall have it...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14529', '1', '0', '1', 'PHASE_END'),
-- Phase 3
('-1616018', 'Now your benefactors make their appearance...But they are too late. The powers contained here are sufficient to destroy the world ten times over! What do you think they will do to you?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14530', '1', '0', '1', 'INTRO_P3'),
('-1616019', 'SUBMIT!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14531', '1', '0', '1', 'AGGRO3'),
('-1616023', 'Alexstrasza! Another of your brood falls!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14534', '1', '0', '1', 'KILL'),
('-1616024', 'Little more then gnats!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14535', '1', '0', '1', 'KILL'),
('-1616025', 'Your red allies will share your fate...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14536', '1', '0', '1', 'KILL'),
('-1616026', 'The powers at work here exceed anything you could possibly imagine!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14532', '1', '0', '1', 'SURGE'),
('-1616027', 'Still standing? Not for long...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14537', '1', '0', '1', 'SPELL'),
('-1616028', 'Your cause is lost!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14538', '1', '0', '1', 'SPELL'),
('-1616029', 'Your fragile mind will be shattered!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14539', '1', '0', '1', 'SPELL'),
('-1616030', 'UNTHINKABLE! The mortals will destroy... e-everything... my sister... what have you-', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14540', '1', '0', '1', 'DEATH'),
-- Outro
('-1616031', 'I did what I had to, brother. You gave me no alternative.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14406', '1', '0', '1', NULL),
('-1616032', 'And so ends the Nexus War.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14407', '1', '0', '1', NULL),
('-1616033', 'This resolution pains me deeply, but the destruction, the monumental loss of life had to end. Regardless of Malygos\' recent transgressions, I will mourn his loss. He was once a guardian, a protector. This day, one of the world\'s mightiest has fallen.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14408', '1', '0', '1', NULL),
('-1616034', 'The red dragonflight will take on the burden of mending the devastation wrought on Azeroth. Return home to your people and rest. Tomorrow will bring you new challenges, and you must be ready to face them. Life...goes on.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14409', '1', '0', '1', NULL);
