/* Naxxramas */
DELETE FROM script_texts WHERE entry BETWEEN -1533149 AND -1533147;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1533147,'%s loses its link!',0,3,0,0,'tesla_coil EMOTE_LOSING_LINK'),
(-1533148,'%s overloads!',0,3,0,0,'tesla_coil EMOTE_TESLA_OVERLOAD'),
(-1533149,'The polarity has shifted!',0,3,0,0,'thaddius EMOTE_POLARITY_SHIFT');
