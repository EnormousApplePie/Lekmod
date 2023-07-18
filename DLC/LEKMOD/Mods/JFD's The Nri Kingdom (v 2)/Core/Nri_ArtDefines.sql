--==========================================================================================================================
-- ART DEFINES
--==========================================================================================================================	
-- IconTextureAtlases
------------------------------
INSERT INTO IconTextureAtlases 
		(Atlas, 								IconSize, 	Filename, 							IconsPerRow, 	IconsPerColumn)
VALUES	('JFD_NRI_ATLAS', 						256, 		'JFD_NriAtlas_256.dds',				2, 				2),
		('JFD_NRI_ATLAS', 						128, 		'JFD_NriAtlas_128.dds',				2, 				2),
		('JFD_NRI_ATLAS', 						80, 		'JFD_NriAtlas_80.dds',				2, 				2),
		('JFD_NRI_ATLAS', 						64, 		'JFD_NriAtlas_64.dds',				2, 				2),
		('JFD_NRI_ATLAS', 						45, 		'JFD_NriAtlas_45.dds',				2, 				2),
		('JFD_NRI_ATLAS', 						32, 		'JFD_NriAtlas_32.dds',				2, 				2),
		('JFD_NRI_RELIGION_ATLAS', 				256, 		'JFD_NriReligionAtlas_256.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				214, 		'JFD_NriReligionAtlas_214.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				128, 		'JFD_NriReligionAtlas_128.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				80, 		'JFD_NriReligionAtlas_80.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				64, 		'JFD_NriReligionAtlas_64.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				48, 		'JFD_NriReligionAtlas_48.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				32, 		'JFD_NriReligionAtlas_32.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				24, 		'JFD_NriReligionAtlas_24.dds',		1, 				1),
		('JFD_NRI_RELIGION_ATLAS', 				16, 		'JFD_NriReligionAtlas_16.dds',		1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				128, 		'JFD_NriAlphaAtlas_128.dds',		1,				1),
		('JFD_NRI_ALPHA_ATLAS', 				80, 		'JFD_NriAlphaAtlas_80.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				64, 		'JFD_NriAlphaAtlas_64.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				48, 		'JFD_NriAlphaAtlas_48.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				45, 		'JFD_NriAlphaAtlas_45.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				32, 		'JFD_NriAlphaAtlas_32.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				24, 		'JFD_NriAlphaAtlas_24.dds',			1, 				1),
		('JFD_NRI_ALPHA_ATLAS', 				16, 		'JFD_NriAlphaAtlas_16.dds',			1, 				1),
		('JFD_UNIT_FLAG_MBURICHI_ATLAS',		32, 		'JFD_MburichiUnitFlag_32.dds',		1, 				1);
------------------------------
-- Colors
------------------------------		
INSERT INTO Colors 
		(Type, 									Red, 	Green, 	Blue, 	Alpha)
VALUES	('COLOR_PLAYER_JFD_NRI_ICON', 			0.572, 	0.584,	0.168, 	1),
		('COLOR_PLAYER_JFD_NRI_BACKGROUND',		0.839,	0.905,	0.682, 	1);
------------------------------	
-- PlayerColors
------------------------------				
INSERT INTO PlayerColors 
		(Type, 									PrimaryColor, 					SecondaryColor, 					TextColor)
VALUES	('PLAYERCOLOR_JFD_NRI', 				'COLOR_PLAYER_JFD_NRI_ICON', 	'COLOR_PLAYER_JFD_NRI_BACKGROUND', 	'COLOR_PLAYER_WHITE_TEXT');
------------------------------
-- IconFontTextures
------------------------------
INSERT INTO IconFontTextures 
		(IconFontTexture, 						IconFontTextureFile)
VALUES	('ICON_FONT_TEXTURE_JFD_ODINANI',		'JFD_ReligionOdinaniFontIcons_22');
------------------------------
-- IconFontMapping
------------------------------
INSERT INTO IconFontMapping 
		(IconName, 								IconFontTexture,						IconMapping)
VALUES	('ICON_RELIGION_JFD_ODINANI', 			'ICON_FONT_TEXTURE_JFD_ODINANI',		1);
--==========================================================================================================================
-- AUDIO
--==========================================================================================================================	
-- Audio_Sounds
------------------------------	
INSERT INTO Audio_Sounds 
		(SoundID, 							Filename, 		LoadType)
VALUES	('SND_LEADER_MUSIC_JFD_ERI_PEACE', 	'Eri_Peace', 	'DynamicResident'),
		('SND_LEADER_MUSIC_JFD_ERI_WAR', 	'Eri_War', 		'DynamicResident');				
------------------------------	
-- Audio_2DSounds
------------------------------		
INSERT INTO Audio_2DSounds 
		(ScriptID, 							SoundID, 							SoundType, 		MinVolume, 	MaxVolume, IsMusic, Looping)
VALUES	('AS2D_LEADER_MUSIC_JFD_ERI_PEACE',	'SND_LEADER_MUSIC_JFD_ERI_PEACE',	'GAME_MUSIC',	120, 		120, 		1, 		0),
		('AS2D_LEADER_MUSIC_JFD_ERI_WAR', 	'SND_LEADER_MUSIC_JFD_ERI_WAR', 	'GAME_MUSIC',	120, 		120, 		1,		0);
--==========================================================================================================================
-- UNIT GRAPHICS
--==========================================================================================================================	
-- ArtDefine_StrategicView
------------------------------
INSERT INTO ArtDefine_StrategicView 
		(StrategicViewType, 				TileType,		Asset)
VALUES	('ART_DEF_UNIT_JFD_MBURICHI', 		'Unit', 		'sv_Mburichi.dds');
------------------------------
-- ArtDefine_UnitInfos
------------------------------			
INSERT INTO ArtDefine_UnitInfos 
		(Type, 								DamageStates,	Formation)
SELECT	'ART_DEF_UNIT_JFD_MBURICHI',		DamageStates, 	'UnFormed'
FROM ArtDefine_UnitInfos WHERE Type = 'ART_DEF_UNIT_MERCHANT';
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
		(UnitInfoType,						UnitMemberInfoType,					NumMembers)
SELECT	'ART_DEF_UNIT_JFD_MBURICHI', 		'ART_DEF_UNIT_MEMBER_JFD_MBURICHI',	1
FROM ArtDefine_UnitInfoMemberInfos WHERE UnitInfoType = 'ART_DEF_UNIT_MERCHANT';
------------------------
-- ArtDefine_UnitMemberCombats
------------------------
INSERT INTO ArtDefine_UnitMemberCombats 
		(UnitMemberType,					EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_MBURICHI',	EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_GREATMERCHANT_EARLY_LEADER';
------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons	
		(UnitMemberType,					"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_MBURICHI',	"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag
FROM ArtDefine_UnitMemberCombatWeapons WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_GREATMERCHANT_EARLY_LEADER';
------------------------
-- ArtDefine_UnitMemberInfos
------------------------
INSERT INTO ArtDefine_UnitMemberInfos 	
		(Type, 								Scale,	ZOffset, Domain, Model, 			MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_MBURICHI',	Scale,	ZOffset, Domain, 'Mburichi.fxsxml',	MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE Type = 'ART_DEF_UNIT_MEMBER_GREATMERCHANT_EARLY_LEADER';
--==========================================================================================================================	
--==========================================================================================================================	
