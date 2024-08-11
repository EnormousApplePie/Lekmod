--==========================================================================================================================
-- ART DEFINES
--==========================================================================================================================	
-- IconTextureAtlases
------------------------------
INSERT INTO IconTextureAtlases 
		(Atlas, 									IconSize, 	Filename, 								IconsPerRow, 	IconsPerColumn)
VALUES	('JFD_PRUSSIA_ATLAS', 						256, 		'JFD_PrussiaAtlas_256.dds',				2, 				2),
		('JFD_PRUSSIA_ATLAS', 						128, 		'JFD_PrussiaAtlas_128.dds',				2, 				2),
		('JFD_PRUSSIA_ATLAS', 						80, 		'JFD_PrussiaAtlas_80.dds',				2, 				2),
		('JFD_PRUSSIA_ATLAS', 						64, 		'JFD_PrussiaAtlas_64.dds',				2, 				2),
		('JFD_PRUSSIA_ATLAS', 						45, 		'JFD_PrussiaAtlas_45.dds',				2, 				2),
		('JFD_PRUSSIA_ATLAS', 						32, 		'JFD_PrussiaAtlas_32.dds',				2, 				2),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				128, 		'JFD_PrussiaAlphaAtlas_128.dds',		2, 				2),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				80, 		'JFD_PrussiaAlphaAtlas_80.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				64, 		'JFD_PrussiaAlphaAtlas_64.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				48, 		'JFD_PrussiaAlphaAtlas_48.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				45, 		'JFD_PrussiaAlphaAtlas_45.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				32, 		'JFD_PrussiaAlphaAtlas_32.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				24, 		'JFD_PrussiaAlphaAtlas_24.dds',			1, 				1),
		('JFD_PRUSSIA_ALPHA_ATLAS', 				16, 		'JFD_PrussiaAlphaAtlas_16.dds',			1, 				1),
		('JFD_PRUSSIA_PROMOTION_ATLAS', 			256, 	    'Tomatekh_PromotionAtlas_256.dds',		2, 				1),
		('JFD_PRUSSIA_PROMOTION_ATLAS', 			64, 	    'Tomatekh_PromotionAtlas_64.dds',		2, 				1),
		('JFD_PRUSSIA_PROMOTION_ATLAS', 			45, 	    'Tomatekh_PromotionAtlas_45.dds',		2, 				1),
		('JFD_PRUSSIA_PROMOTION_ATLAS', 			32, 	    'Tomatekh_PromotionAtlas_32.dds',		2, 				1),
		('JFD_UNIT_FLAG_LANDWEHR_ATLAS', 			32, 		'Unit_Landwehr_Flag_32.dds',			1, 				1),
		('JFD_UNIT_FLAG_DEATHS_HEAD_ATLAS', 		32, 		'Unit_DeathsHead_Flag_32.dds',			1, 				1);
------------------------------
-- Colors
------------------------------		
INSERT INTO Colors 
		(Type, 										Red, 	Green, 	Blue, 	Alpha)
VALUES	('COLOR_PLAYER_JFD_PRUSSIA_ICON', 			0, 		0, 		0, 		1),
		('COLOR_PLAYER_JFD_PRUSSIA_BACKGROUND', 	0.141,	0.411,	0.615, 	1);
------------------------------
-- PlayerColors
------------------------------			
INSERT INTO PlayerColors 
		(Type, 										PrimaryColor, 						SecondaryColor, 						TextColor)
VALUES	('PLAYERCOLOR_JFD_PRUSSIA', 				'COLOR_PLAYER_JFD_PRUSSIA_ICON', 	'COLOR_PLAYER_JFD_PRUSSIA_BACKGROUND', 	'COLOR_PLAYER_WHITE_TEXT');
--==========================================================================================================================
-- AUDIO
--==========================================================================================================================	
-- Audio_Sounds
------------------------------	
INSERT INTO Audio_Sounds 
		(SoundID, 											Filename, 									LoadType)
VALUES	('SND_LEADER_MUSIC_JFD_FREDERICK_PEACE', 			'Frederick_Peace', 							'DynamicResident'),
		('SND_LEADER_MUSIC_JFD_FREDERICK_WAR', 				'Frederick_War', 							'DynamicResident');		
------------------------------
-- Audio_2DSounds
------------------------------	
INSERT INTO Audio_2DSounds 
		(ScriptID, 											SoundID, 									SoundType, 		MinVolume, 	MaxVolume,	IsMusic, Looping)
VALUES	('AS2D_AMBIENCE_LEADER_JFD_FREDERICK_AMBIENCE', 	'SND_AMBIENCE_NAPOLEON_AMBIENCE', 			'GAME_SFX', 	50, 		50, 		0, 		1),
		('AS2D_LEADER_MUSIC_JFD_FREDERICK_PEACE', 			'SND_LEADER_MUSIC_JFD_FREDERICK_PEACE', 	'GAME_MUSIC', 	45, 		45, 		1, 		0),
		('AS2D_LEADER_MUSIC_JFD_FREDERICK_WAR', 			'SND_LEADER_MUSIC_JFD_FREDERICK_WAR', 		'GAME_MUSIC', 	45, 		45, 		1,		0);
--==========================================================================================================================
-- UNIT GRAPHICS
--==========================================================================================================================	
-- ArtDefine_StrategicView
------------------------------
INSERT INTO ArtDefine_StrategicView 
		(StrategicViewType, 					TileType,		Asset)
VALUES	('ART_DEF_UNIT_JFD_LANDWEHR', 			'Unit', 		'sv_Landwehr.dds'),
		('ART_DEF_UNIT_JFD_DEATHS_HEAD', 		'Unit', 		'sv_DeathsHead.dds');
------------------------------
-- ArtDefine_UnitInfos
------------------------------		
INSERT INTO ArtDefine_UnitInfos 
		(Type, 									DamageStates,	Formation)
SELECT	'ART_DEF_UNIT_JFD_DEATHS_HEAD', 		DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE Type = 'ART_DEF_UNIT_CAVALRY'; 

INSERT INTO ArtDefine_UnitInfos 
		(Type, 									DamageStates,	Formation)
SELECT	'ART_DEF_UNIT_JFD_LANDWEHR', 			DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE	Type = 'ART_DEF_UNIT_RIFLEMAN';
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
		(UnitInfoType,							UnitMemberInfoType,						NumMembers)
SELECT	'ART_DEF_UNIT_JFD_DEATHS_HEAD', 		'ART_DEF_UNIT_MEMBER_JFD_DEATHS_HEAD', 	NumMembers
FROM ArtDefine_UnitInfoMemberInfos WHERE UnitInfoType = 'ART_DEF_UNIT_CAVALRY';
	
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
		(UnitInfoType,							UnitMemberInfoType,						NumMembers)
SELECT	'ART_DEF_UNIT_JFD_LANDWEHR', 			'ART_DEF_UNIT_MEMBER_JFD_LANDWEHR', 	NumMembers
FROM ArtDefine_UnitInfoMemberInfos WHERE UnitInfoType = 'ART_DEF_UNIT_RIFLEMAN';
------------------------------
-- ArtDefine_UnitMemberCombats
------------------------------
INSERT INTO ArtDefine_UnitMemberCombats 
		(UnitMemberType,						EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_DEATHS_HEAD',	EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_CAVALRY';
	
INSERT INTO ArtDefine_UnitMemberCombats 
		(UnitMemberType,						EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_LANDWEHR',		EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_RIFLEMAN';
------------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons	
		(UnitMemberType,						"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_DEATHS_HEAD',	"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag
FROM ArtDefine_UnitMemberCombatWeapons WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_CAVALRY';

INSERT INTO ArtDefine_UnitMemberCombatWeapons	
		(UnitMemberType,						"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_LANDWEHR',		"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag
FROM ArtDefine_UnitMemberCombatWeapons WHERE UnitMemberType = 'ART_DEF_UNIT_MEMBER_RIFLEMAN';
------------------------------
-- ArtDefine_UnitMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitMemberInfos 	
		(Type, 									Scale, ZOffset, Domain, Model, 					MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_DEATHS_HEAD',	Scale, ZOffset, Domain, 'gerca1.fxsxml',		MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE Type = 'ART_DEF_UNIT_MEMBER_CAVALRY';

INSERT INTO ArtDefine_UnitMemberInfos 	
		(Type, 									Scale, ZOffset, Domain, Model, 					MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT	'ART_DEF_UNIT_MEMBER_JFD_LANDWEHR',		Scale, ZOffset, Domain, 'civ5_gerrm.fxsxml',	MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE Type = 'ART_DEF_UNIT_MEMBER_RIFLEMAN';
--==========================================================================================================================	
--==========================================================================================================================	
