-- mountain
-- Author: lek10
-- DateCreated: 4/23/2018 2:48:15 AM
--------------------------------------------------------------
Events.SequenceGameInitComplete.Add(function()
	for i = 0, Map:GetNumPlots() - 1, 1 do
		local fPlot = Map.GetPlotByIndex( i );
		if fPlot:IsMountain() then
			if fPlot:GetFeatureType() == -1  then
				fPlot:SetFeatureType(GameInfoTypes.FEATURE_ARARAT_MOUNTAIN,-1);
			end
		end
	end
end)