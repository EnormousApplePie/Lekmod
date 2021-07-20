--==========================================================================================================================
-- AUDIO
--==========================================================================================================================	
-- Audio_Sounds
------------------------------	
INSERT INTO Audio_Sounds 
		(SoundID, 									Filename, 				LoadType)
VALUES	('SND_LEADER_MUSIC_US_OWAIN_PEACE', 		'Wales_Peace',			'DynamicResident'),
		('SND_LEADER_MUSIC_US_OWAIN_WAR',			'Wales_War', 			'DynamicResident');		
------------------------------
-- Audio_2DSounds
------------------------------	
INSERT INTO Audio_2DSounds 
		(ScriptID, 										SoundID, 									SoundType, 		TaperSoundtrackVolume,	MinVolume, 	MaxVolume,  IsMusic, Looping)
VALUES	('AS2D_LEADER_MUSIC_US_OWAIN_PEACE', 			'SND_LEADER_MUSIC_US_OWAIN_PEACE',			'GAME_MUSIC', 	-1.0,					60, 		60, 		1, 		 0),
		('AS2D_LEADER_MUSIC_US_OWAIN_WAR', 				'SND_LEADER_MUSIC_US_OWAIN_WAR', 			'GAME_MUSIC', 	-1.0,					60, 		60, 		1,		 0);
--==========================================================================================================================
--==========================================================================================================================	