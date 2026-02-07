-- Author: EnormousApplePie

include("Lekmod_utilities.lua")

local this_civ = GameInfoTypes["CIVILIZATION_VATICAN"]
local is_active = LekmodUtilities:is_civilization_active(this_civ)

------------------------------------------------------------------------------------------------------------------------
-- Vatican UA. Add a courthouse to cities that are occupied or puppeted and have the player's religion
------------------------------------------------------------------------------------------------------------------------
function lekmod_ua_courthouse(player_id, _ , x, y)

	local player = Players[player_id]
   local player_religion = player:GetReligionCreatedByPlayer();
   local city = Map.GetPlot(x, y):GetPlotCity()

	if not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

   if not (city:IsOccupied() or city:IsPuppet()) 
   or city:GetReligiousMajority() ~= player_religion
   or city:GetReligiousMajority() == -1 then return end

   city:SetNumRealBuilding(GameInfoTypes["BUILDING_COURTHOUSE"], 1);

end

function lekmod_ua_courthouse_on_capture(_, _, x, y, new_player_id)

   lekmod_ua_courthouse(new_player_id, _, x, y)

end

------------------------------------------------------------------------------------------------------------------------
-- Vatican Legacy (Medieval 1):
-- “Receive a Free Missionary if you have found your Religion and when Enhancing one.”
------------------------------------------------------------------------------------------------------------------------
local Lek_SaveData   = Modding.OpenSaveData()
local Lek_Properties = {}

local function vatican_get_missionaries_given(player_id)
    local key = "VATICAN_MISSIONARIES_" .. tostring(player_id)
    if Lek_Properties[key] == nil then
        Lek_Properties[key] = Lek_SaveData.GetValue(key)
    end
    return Lek_Properties[key] or 0
end

local function vatican_set_missionaries_given(player_id, value)
    local key = "VATICAN_MISSIONARIES_" .. tostring(player_id)
    if Lek_Properties[key] == value then return end
    Lek_Properties[key] = value
    Lek_SaveData.SetValue(key, value)
end

function lekmod_legacy_vatican_medieval_1(player_id)
    local player = Players[player_id]
    if not player or not player:IsAlive() or player:GetCivilizationType() ~= this_civ then return end

    local legacy_id = GameInfoTypes["LEGACY_VATICAN_MEDIEVAL_1"]
    if not player:HasLegacy(legacy_id) then return end

    local player_religion = player:GetReligionCreatedByPlayer()
    if player_religion == -1 then return end

    local capital = player:GetCapitalCity()
    if not capital then return end

    local unit_missionary = GameInfoTypes["UNIT_MISSIONARY"]
    
    local has_enhancer_belief = false
    local beliefs = Game.GetBeliefsInReligion(player_religion)
    if beliefs then
        for _, beliefID in ipairs(beliefs) do
            local belief = GameInfo.Beliefs[beliefID]
            if belief and (belief.Enhancer == 1 or belief.Enhancer == true) then
                has_enhancer_belief = true
                break
            end
        end
    end

    local expected = 1
    if has_enhancer_belief then
        expected = expected + 1
    end

    local already_given = vatican_get_missionaries_given(player_id)
    local to_give = expected - already_given
    if to_give <= 0 then return end

    for i = 1, to_give do
        player:InitUnit(unit_missionary, capital:GetX(), capital:GetY())
    end

    vatican_set_missionaries_given(player_id, already_given + to_give)
end

-- Triggers
function lekmod_legacy_vatican_on_found(player_id)
    lekmod_legacy_vatican_medieval_1(player_id)
end

function lekmod_legacy_vatican_on_enhance(player_id)
    lekmod_legacy_vatican_medieval_1(player_id)
end
------------------------------------------------------------------------------------------------------------------------
if is_active then
	GameEvents.CityConvertsReligion.Add(lekmod_ua_courthouse)
	GameEvents.CityCaptureComplete.Add(lekmod_ua_courthouse_on_capture)

    -- Legacy
    -- PlayerChooseLegacy is a Lekmod custom event.
    GameEvents.PlayerChooseLegacy.Add(lekmod_legacy_vatican_medieval_1)
    GameEvents.ReligionFounded.Add(lekmod_legacy_vatican_on_found)
    GameEvents.ReligionEnhanced.Add(lekmod_legacy_vatican_on_enhance)
end