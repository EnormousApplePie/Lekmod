--==========================================================================================================================
-- ART DEFINES
--==========================================================================================================================	
-- IconTextureAtlases
------------------------------
INSERT INTO IconTextureAtlases 
		(Atlas, 												IconSize, 	Filename, 												IconsPerRow, 	IconsPerColumn)
VALUES	('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					256, 		'JFD_PapalStatesInnocentIIIAtlas_256.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					128, 		'JFD_PapalStatesInnocentIIIAtlas_128.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					80, 		'JFD_PapalStatesInnocentIIIAtlas_80.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					64, 		'JFD_PapalStatesInnocentIIIAtlas_64.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					45, 		'JFD_PapalStatesInnocentIIIAtlas_45.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_ATLAS', 					32, 		'JFD_PapalStatesInnocentIIIAtlas_32.dds',				2, 				2),
		('JFD_PAPAL_STATES_INNOCENT_NOTIFICATION_ATLAS', 		80, 		'JFD_PapalStatesInnocentIIINotificationAtlas_80.dds',	1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				128, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_128.dds',			1,				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				80, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_80.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				64, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_64.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				48, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_48.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				45, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_45.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				32, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_32.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				24, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_24.dds',			1, 				1),
		('JFD_PAPAL_STATES_INNOCENT_ALPHA_ATLAS', 				16, 		'JFD_PapalStatesInnocentIIIAlphaAtlas_16.dds',			1, 				1),
		('JFD_UNIT_FLAG_CARROCCIO_ATLAS', 						32, 		'JFD_UnitFlagCarroccio_32.dds',							1, 				1);
------------------------------
-- Colors
------------------------------		
INSERT INTO Colors 
		(Type, 													Red, 	Green, 	Blue, 	Alpha)
VALUES	('COLOR_PLAYER_JFD_PAPAL_STATES_INNOCENT_ICON', 		0.603, 	0.443, 	0.117,	1),
		('COLOR_PLAYER_JFD_PAPAL_STATES_INNOCENT_BACKGROUND',	0.878,	0.870,	0.760,	1);
------------------------------
-- PlayerColors
------------------------------			
INSERT INTO PlayerColors 
		(Type, 										PrimaryColor, 										SecondaryColor, 										TextColor)
VALUES	('PLAYERCOLOR_JFD_PAPAL_STATES_INNOCENT',	'COLOR_PLAYER_JFD_PAPAL_STATES_INNOCENT_ICON', 		'COLOR_PLAYER_JFD_PAPAL_STATES_INNOCENT_BACKGROUND',	'COLOR_PLAYER_WHITE_TEXT');
--==========================================================================================================================
-- AUDIO
--==========================================================================================================================	
-- Audio_Sounds
------------------------------	
INSERT INTO Audio_Sounds 
		(SoundID, 										Filename, 								LoadType)
VALUES	('SND_BUILDING_JFD_ECCLESIASTIC_COURT', 		'EcclesiasticCourt',					'DynamicResident'),
		('SND_LEADER_MUSIC_JFD_INNOCENT_PEACE', 		'InnocentIII_Peace',					'DynamicResident'),
		('SND_LEADER_MUSIC_JFD_INNOCENT_WAR', 			'InnocentIII_War', 						'DynamicResident');		
------------------------------
-- Audio_2DSounds
------------------------------	
INSERT INTO Audio_2DSounds 
		(ScriptID, 										SoundID, 									SoundType, 		TaperSoundtrackVolume,	MinVolume, 	MaxVolume,  IsMusic, Looping)
VALUES	('AS2D_BUILDING_JFD_ECCLESIASTIC_COURT',		'SND_BUILDING_JFD_ECCLESIASTIC_COURT',		'GAME_SFX', 	-1.0,					100, 		100, 		0, 		 0),
		('AS2D_LEADER_MUSIC_JFD_INNOCENT_PEACE', 		'SND_LEADER_MUSIC_JFD_INNOCENT_PEACE',		'GAME_MUSIC', 	-1.0,					45, 		45, 		1, 		 0),
		('AS2D_LEADER_MUSIC_JFD_INNOCENT_WAR', 			'SND_LEADER_MUSIC_JFD_INNOCENT_WAR',		'GAME_MUSIC', 	-1.0,					45, 		45, 		1,		 0);
--==========================================================================================================================
-- UNIT GRAPHICS
--==========================================================================================================================	
-- ArtDefine_StrategicView
------------------------------
INSERT INTO ArtDefine_StrategicView 
		(StrategicViewType, 					TileType,		Asset)
VALUES	('ART_DEF_UNIT_JFD_CARROCCIO',			'Unit', 		'sv_Carroccio.dds');
------------------------------
-- ArtDefine_UnitInfos
------------------------------	
INSERT INTO ArtDefine_UnitInfos 
		(Type, 									DamageStates,	Formation)
SELECT	'ART_DEF_UNIT_JFD_CARROCCIO',			DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE Type = 'ART_DEF_UNIT_TREBUCHET';
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
		(UnitInfoType,							UnitMemberInfoType,							NumMembers)
SELECT	'ART_DEF_UNIT_JFD_CARROCCIO',			'ART_DEF_UNIT_MEMBER_JFD_CARROCCIO',		1
FROM ArtDefine_UnitInfoMemberInfos WHERE UnitInfoType = 'ART_DEF_UNIT_TREBUCHET';
------------------------------
-- ArtDefine_UnitMemberCombats
------------------------------
INSERT INTO ArtDefine_UnitMemberCombats 
		(UnitMemberType,						EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_CARROCCIO',	EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, 1,					 1,					  HasLeftRightAttack, 1,				  HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_TREBUCHET';
------------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons	
		(UnitMemberType,						"Index", SubIndex, ID, VisKillStrengthMin,		VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_CARROCCIO',	"Index", SubIndex, ID, 25.0,					50.0,				ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, 1,			IsDropped, ('BOULDER<'),  ('BOULDER')
FROM ArtDefine_UnitMemberCombatWeapons WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_TREBUCHET';
------------------------------
-- ArtDefine_UnitMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitMemberInfos 	
		(Type, 									Scale,  ZOffset, Domain, Model, 				MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_CARROCCIO',	Scale,	ZOffset, Domain, 'GreatCross.fxsxml',	'WOOD',			 'WOODMED'
FROM ArtDefine_UnitMemberInfos WHERE Type = 'ART_DEF_UNIT_MEMBER_TREBUCHET';
--==========================================================================================================================	
--==========================================================================================================================	
