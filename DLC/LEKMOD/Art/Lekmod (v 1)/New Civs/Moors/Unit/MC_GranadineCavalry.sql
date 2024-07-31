--==========================================================================================================================
-- ARTDEFINES
--==========================================================================================================================	
-- ArtDefine_UnitInfos
------------------------------
INSERT INTO ArtDefine_UnitInfos(Type, DamageStates, Formation)
VALUES ('ART_DEF_UNIT_MC_GRANADINECAVALRY', 1, 'DefaultCavalry');
------------------------------
-- ArtDefine_UnitInfoMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitInfoMemberInfos(UnitInfoType, UnitMemberInfoType, NumMembers)
VALUES ('ART_DEF_UNIT_MC_GRANADINECAVALRY', 'ART_DEF_UNIT_MEMBER_MC_GRANADINECAVALRY', 5);
------------------------------
-- ArtDefine_UnitMemberCombats
------------------------------
INSERT INTO ArtDefine_UnitMemberCombats(UnitMemberType, EnableActions, ShortMoveRadius, ShortMoveRate, TurnRateMin, TurnRateMax, TurnFacingRateMin, TurnFacingRateMax, TargetHeight, HasShortRangedAttack, HasStationaryMelee, HasRefaceAfterCombat, ReformBeforeCombat, OnlyTurnInMovementActions)
VALUES ('ART_DEF_UNIT_MEMBER_MC_GRANADINECAVALRY', 'Idle Attack RunCharge AttackCity Bombard Death BombardDefend Run Fortify CombatReady Walk', 24.0, 0.349999994039536, 0.5, 0.75, 15.0, 20.0, 12.0, 1, 1, 1, 1, 1);
------------------------------
-- ArtDefine_UnitMemberCombatWeapons
------------------------------
INSERT INTO ArtDefine_UnitMemberCombatWeapons(UnitMemberType, "Index", SubIndex, WeaponTypeTag, WeaponTypeSoundOverrideTag, MissTargetSlopRadius)
VALUES ('ART_DEF_UNIT_MEMBER_MC_GRANADINECAVALRY', 0, 0, 'ARROW', 'ARROW', 10.0);

INSERT INTO ArtDefine_UnitMemberCombatWeapons(UnitMemberType, "Index", SubIndex, VisKillStrengthMin, VisKillStrengthMax, WeaponTypeTag, MissTargetSlopRadius)
VALUES ('ART_DEF_UNIT_MEMBER_MC_GRANADINECAVALRY', 1, 0, 10.0, 20.0, 'FLAMING_ARROW', 10.0);
------------------------------
-- ArtDefine_UnitMemberInfos
------------------------------
INSERT INTO ArtDefine_UnitMemberInfos(Type, Scale, Model, MaterialTypeTag, MaterialTypeSoundOverrideTag)
VALUES ('ART_DEF_UNIT_MEMBER_MC_GRANADINECAVALRY', 0.119999997317791, 'MC_GranadineCavalry.fxsxml', 'CLOTH', 'FLESH');
------------------------------
-- ArtDefine_StrategicView
------------------------------
INSERT INTO ArtDefine_StrategicView(StrategicViewType, TileType, Asset)
VALUES ('ART_DEF_UNIT_MC_GRANADINECAVALRY', 'Unit', 'sv_GranadineCavalry.dds');
--==========================================================================================================================	
--==========================================================================================================================	
