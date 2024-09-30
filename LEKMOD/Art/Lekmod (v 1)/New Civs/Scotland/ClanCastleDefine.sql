CREATE TRIGGER ArtImprovementCaer
AFTER INSERT ON ArtDefine_LandmarkTypes
BEGIN
INSERT INTO ArtDefine_LandmarkTypes(Type, LandmarkType, FriendlyName)
SELECT 'ART_DEF_IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE', 'Improvement', 'MC_SCOTTISH_CLAN_CASTLE';

INSERT INTO ArtDefine_Landmarks(Era, State, Scale, ImprovementType, LayoutHandler, ResourceType, Model, TerrainContour)
SELECT 'Any', 'UnderConstruction', 0.08,  'ART_DEF_IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE', 'SNAPSHOT', 'ART_DEF_RESOURCE_ALL', 'clancastle_hb.fxsxml', 1 UNION ALL
SELECT 'Any', 'Constructed', 0.08,  'ART_DEF_IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE', 'SNAPSHOT', 'ART_DEF_RESOURCE_ALL', 'clancastle.fxsxml', 1 UNION ALL
SELECT 'Any', 'Pillaged', 0.08,  'ART_DEF_IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE', 'SNAPSHOT', 'ART_DEF_RESOURCE_ALL', 'clancastle_pl.fxsxml', 1;


INSERT INTO ArtDefine_StrategicView(StrategicViewType, TileType, Asset)
SELECT 'ART_DEF_IMPROVEMENT_MC_SCOTTISH_CLAN_CASTLE', 'Improvement', 'sv_clancastle.dds';
END;