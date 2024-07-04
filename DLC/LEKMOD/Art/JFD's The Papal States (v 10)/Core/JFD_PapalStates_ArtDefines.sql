--==========================================================================================================================
-- ARTDEFINES
--==========================================================================================================================	
-- ArtDefine_StrategicView
------------------------------	
INSERT INTO ArtDefine_StrategicView 
			(StrategicViewType, 				TileType,	Asset)
VALUES		('ART_DEF_UNIT_JFD_SWISS_GUARD', 	'Unit', 	'sv_Swissguard.dds'),
			('ART_DEF_UNIT_JFD_DISCIPLE', 		'Unit', 	'sv_Disciple.dds');
------------------------------
-- ArtDefine_UnitInfos
------------------------------			
INSERT INTO ArtDefine_UnitInfos 
			(Type, 								DamageStates,	Formation)
SELECT		('ART_DEF_UNIT_JFD_SWISS_GUARD'), 	DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE (Type = 'ART_DEF_UNIT_PIKEMAN');

INSERT INTO ArtDefine_UnitInfos 
			(Type, 								DamageStates,	Formation)
SELECT		('ART_DEF_UNIT_JFD_DISCIPLE'),		DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE (Type = 'ART_DEF_UNIT_MISSIONARY');
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
			(UnitInfoType,						UnitMemberInfoType,							NumMembers)
SELECT		('ART_DEF_UNIT_JFD_SWISS_GUARD'), 	('ART_DEF_UNIT_MEMBER_JFD_SWISS_GUARD'), 	NumMembers
FROM ArtDefine_UnitInfoMemberInfos WHERE (UnitInfoType = 'ART_DEF_UNIT_PIKEMAN');

INSERT INTO ArtDefine_UnitInfoMemberInfos 	
			(UnitInfoType,						UnitMemberInfoType,						NumMembers)
SELECT		('ART_DEF_UNIT_JFD_DISCIPLE'),		('ART_DEF_UNIT_MEMBER_JFD_DISCIPLE'), 	1
FROM ArtDefine_UnitInfoMemberInfos WHERE (UnitInfoType = 'ART_DEF_UNIT_MISSIONARY');
------------------------------
-- ArtDefine_UnitMemberCombats
------------------------------
INSERT INTO ArtDefine_UnitMemberCombats 
			(UnitMemberType,							EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT		('ART_DEF_UNIT_MEMBER_JFD_SWISS_GUARD'),	EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE (UnitMemberType = 'ART_DEF_UNIT_MEMBER_PIKEMAN');

INSERT INTO ArtDefine_UnitMemberCombats 
			(UnitMemberType,							EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT		('ART_DEF_UNIT_MEMBER_JFD_DISCIPLE'),		EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE (UnitMemberType = 'ART_DEF_UNIT_MEMBER_MISSIONARY');
------------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons	
			(UnitMemberType,							"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT		('ART_DEF_UNIT_MEMBER_JFD_SWISS_GUARD'),	"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag
FROM ArtDefine_UnitMemberCombatWeapons WHERE (UnitMemberType = 'ART_DEF_UNIT_MEMBER_PIKEMAN');
------------------------------
-- ArtDefine_UnitMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitMemberInfos 	
			(Type, 										Scale,  ZOffset, Domain, Model, 				MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT		('ART_DEF_UNIT_MEMBER_JFD_SWISS_GUARD'),	Scale,	ZOffset, Domain, ('reislaufer.fxsxml'),	MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE (Type = 'ART_DEF_UNIT_MEMBER_PIKEMAN');

INSERT INTO ArtDefine_UnitMemberInfos 	
			(Type, 										Scale,  ZOffset, Domain, Model, 					MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT		('ART_DEF_UNIT_MEMBER_JFD_DISCIPLE'),		Scale,	ZOffset, Domain, ('missionary_01.fxsxml'),	MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE (Type = 'ART_DEF_UNIT_MEMBER_MISSIONARY');
------------------------------
-- IconTextureAtlases
------------------------------
INSERT INTO IconTextureAtlases 
			(Atlas, 								IconSize, 	Filename, 								IconsPerRow, 	IconsPerColumn)
VALUES		('JFD_PAPAL_STATES_ATLAS', 				256, 		'JFD_PapalStatesAtlas_256.dds',			3, 				3),
			('JFD_PAPAL_STATES_ATLAS', 				128, 		'JFD_PapalStatesAtlas_128.dds',			3, 				3),
			('JFD_PAPAL_STATES_ATLAS', 				80, 		'JFD_PapalStatesAtlas_80.dds',			3, 				3),
			('JFD_PAPAL_STATES_ATLAS', 				64, 		'JFD_PapalStatesAtlas_64.dds',			3, 				3),
			('JFD_PAPAL_STATES_ATLAS', 				45, 		'JFD_PapalStatesAtlas_45.dds',			3, 				3),
			('JFD_PAPAL_STATES_ATLAS', 				32, 		'JFD_PapalStatesAtlas_32.dds',			3, 				3),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		128, 		'JFD_PapalStatesAlphaAtlas_128.dds',	1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		80, 		'JFD_PapalStatesAlphaAtlas_80.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		64, 		'JFD_PapalStatesAlphaAtlas_64.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		48, 		'JFD_PapalStatesAlphaAtlas_48.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		45, 		'JFD_PapalStatesAlphaAtlas_45.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		32, 		'JFD_PapalStatesAlphaAtlas_32.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		24, 		'JFD_PapalStatesAlphaAtlas_24.dds',		1, 				1),
			('JFD_PAPAL_STATES_ALPHA_ATLAS', 		16, 		'JFD_PapalStatesAlphaAtlas_16.dds',		1, 				1),
			('JFD_UNIT_FLAG_DISCIPLE_ATLAS', 		32, 		'Unit_Disciple_Flag_32.dds',			1, 				1),
			('JFD_UNIT_FLAG_SWISS_GUARD_ATLAS', 	32, 		'Unit_SwissGuard_Flag_32.dds',			1, 				1);
------------------------------	
-- Colors
------------------------------		
INSERT INTO Colors 
			(Type, 											Red, 	Green, 	Blue, 	Alpha)
VALUES		('COLOR_PLAYER_JFD_PAPAL_STATES_ICON', 			0.992,	0.8,	0.364,	1),
			('COLOR_PLAYER_JFD_PAPAL_STATES_BACKGROUND', 	0.572,	0.137,	0.141,	1);
------------------------------	
-- PlayerColors
------------------------------				
INSERT INTO PlayerColors 
			(Type, 								PrimaryColor, 							SecondaryColor, 							TextColor)
VALUES		('PLAYERCOLOR_JFD_PAPAL_STATES',	'COLOR_PLAYER_JFD_PAPAL_STATES_ICON', 	'COLOR_PLAYER_JFD_PAPAL_STATES_BACKGROUND', 'COLOR_PLAYER_WHITE_TEXT');
--==========================================================================================================================
-- AUDIO
--==========================================================================================================================	
-- Audio_Sounds
------------------------------	
INSERT INTO Audio_Sounds 
			(SoundID, 							Filename, 						LoadType)
VALUES		('SND_LEADER_MUSIC_JFD_PIUS_PEACE', 'Pius_Peace',					'Streamed'),
			('SND_LEADER_MUSIC_JFD_PIUS_WAR', 	'Pius_War', 					'Streamed');
------------------------------	
-- Audio_2DSounds
------------------------------		
INSERT INTO Audio_2DSounds 
			(ScriptID, 								SoundID, 							SoundType, 		MinVolume, 	MaxVolume,  IsMusic, Looping)
VALUES		('AS2D_LEADER_MUSIC_JFD_PIUS_PEACE', 	'SND_LEADER_MUSIC_JFD_PIUS_PEACE', 	'GAME_MUSIC', 	60, 		60, 		1, 		 0),
			('AS2D_LEADER_MUSIC_JFD_PIUS_WAR', 		'SND_LEADER_MUSIC_JFD_PIUS_WAR', 	'GAME_MUSIC', 	60, 		60, 		1,		 0);
--==========================================================================================================================	
--==========================================================================================================================	
