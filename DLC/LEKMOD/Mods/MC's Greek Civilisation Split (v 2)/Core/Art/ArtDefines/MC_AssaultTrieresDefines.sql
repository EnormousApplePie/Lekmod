--==========================================================================================================================
-- ARTDEFINES
--==========================================================================================================================	
-- Units
------------------------------
UPDATE Units 
SET UnitArtInfo = 'ART_DEF_UNIT_MC_ATHENIAN_ASSAULT_TRIERES'
WHERE Type = 'UNIT_MC_ATHENIAN_ASSAULT_TRIERES';
------------------------------
-- ArtDefine_UnitInfos
------------------------------		
INSERT INTO ArtDefine_UnitInfos 
			(Type, 								DamageStates,	Formation)
SELECT		('ART_DEF_UNIT_MC_ATHENIAN_ASSAULT_TRIERES'), 		DamageStates, 	Formation
FROM ArtDefine_UnitInfos WHERE (Type = 'ART_DEF_UNIT_U_CARTHAGE_QUINQUEREME');
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos 	
			(UnitInfoType,						UnitMemberInfoType,						NumMembers)
SELECT		('ART_DEF_UNIT_MC_ATHENIAN_ASSAULT_TRIERES'), 		('ART_DEF_UNIT_MEMBER_MC_ATHENIAN_ASSAULT_TRIERES'), 	NumMembers
FROM ArtDefine_UnitInfoMemberInfos WHERE (UnitInfoType = 'ART_DEF_UNIT_U_CARTHAGE_QUINQUEREME');
------------------------------
-- ArtDefine_UnitMemberCombats
------------------------------
INSERT INTO ArtDefine_UnitMemberCombats 
			(UnitMemberType,						EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation)
SELECT		('ART_DEF_UNIT_MEMBER_MC_ATHENIAN_ASSAULT_TRIERES'),	EnableActions, DisableActions, MoveRadius, ShortMoveRadius, ChargeRadius, AttackRadius, RangedAttackRadius, MoveRate, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, RollRateMin, RollRateMax, PitchRateMin, PitchRateMax, LOSRadiusScale, TargetRadius, TargetHeight, HasShortRangedAttack, HasLongRangedAttack, HasLeftRightAttack, HasStationaryMelee, HasStationaryRangedAttack, HasRefaceAfterCombat, ReformBeforeCombat, HasIndependentWeaponFacing, HasOpponentTracking, HasCollisionAttack, AttackAltitude, AltitudeDecelerationDistance, OnlyTurnInMovementActions, RushAttackFormation
FROM ArtDefine_UnitMemberCombats WHERE (UnitMemberType = 'ART_DEF_UNIT_MEMBER_U_CARTHAGE_QUINQUEREME');
------------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons	
			(UnitMemberType,						"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag)
SELECT		('ART_DEF_UNIT_MEMBER_MC_ATHENIAN_ASSAULT_TRIERES'),	"Index", SubIndex, ID, VisKillStrengthMin, VisKillStrengthMax, ProjectileSpeed, ProjectileTurnRateMin, ProjectileTurnRateMax, HitEffect, HitEffectScale, HitRadius, ProjectileChildEffectScale, AreaDamageDelay, ContinuousFire, WaitForEffectCompletion, TargetGround, IsDropped, WeaponTypeTag, WeaponTypeSoundOverrideTag
FROM ArtDefine_UnitMemberCombatWeapons WHERE (UnitMemberType = 'ART_DEF_UNIT_MEMBER_U_CARTHAGE_QUINQUEREME');
------------------------------
-- ArtDefine_UnitMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitMemberInfos 	
			(Type, 									Scale, ZOffset, Domain, Model, 							MaterialTypeTag, MaterialTypeSoundOverrideTag)
SELECT		('ART_DEF_UNIT_MEMBER_MC_ATHENIAN_ASSAULT_TRIERES'),	Scale, ZOffset, Domain, ('Epibatas_Naval.fxsxml'),	MaterialTypeTag, MaterialTypeSoundOverrideTag
FROM ArtDefine_UnitMemberInfos WHERE (Type = 'ART_DEF_UNIT_MEMBER_U_CARTHAGE_QUINQUEREME');
------------------------------
-- ArtDefine_StrategicView
------------------------------
INSERT OR REPLACE INTO ArtDefine_StrategicView 
			(StrategicViewType, 					TileType,	Asset)
SELECT		('ART_DEF_UNIT_MC_ATHENIAN_ASSAULT_TRIERES'), 	TileType, 	('sv_AthensAssaultTrieres.dds')
FROM ArtDefine_StrategicView WHERE (StrategicViewType = 'ART_DEF_UNIT_U_CARTHAGE_QUINQUEREME');
--==========================================================================================================================	
--==========================================================================================================================	
