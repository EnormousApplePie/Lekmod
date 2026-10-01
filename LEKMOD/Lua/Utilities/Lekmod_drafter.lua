-- Lekmod lobby drafter: civ tags + deal logic (Hellblazer parity)
LekmodDrafter = LekmodDrafter or {}

LekmodDrafter.DEFAULT_BANS = 2
LekmodDrafter.DEFAULT_PICKS = 3
LekmodDrafter.GUARANTEE_RANDOM = -1

-- key -> { tags={...}, displayName=... }
LekmodDrafter.CivMeta = {
  Akkad = { tags = { "modded" }, displayName = "Akkad" },
  Aksum = { tags = { "modded", "seasonal" }, displayName = "Aksum" },
  America = { tags = { "vanilla" }, displayName = "America" },
  Arabia = { tags = { "vanilla" }, displayName = "Arabia" },
  Argentina = { tags = { "modded" }, displayName = "Argentina" },
  Armenia = { tags = { "modded" }, displayName = "Armenia" },
  Assyria = { tags = { "vanilla", "seasonal" }, displayName = "Assyria" },
  Australia = { tags = { "modded", "coastal", "seasonal" }, displayName = "Australia" },
  Austria = { tags = { "vanilla", "seasonal" }, displayName = "Austria" },
  Ayyubids = { tags = { "modded" }, displayName = "Ayyubids" },
  Aztec = { tags = { "vanilla", "seasonal" }, displayName = "Aztec" },
  Babylon = { tags = { "vanilla" }, displayName = "Babylon" },
  Belgium = { tags = { "modded", "seasonal" }, displayName = "Belgium" },
  Boers = { tags = { "modded", "seasonal" }, displayName = "Boers" },
  Bolivia = { tags = { "modded", "seasonal" }, displayName = "Bolivia" },
  Brazil = { tags = { "vanilla", "seasonal" }, displayName = "Brazil" },
  Brunei = { tags = { "modded", "coastal" }, displayName = "Brunei" },
  Buganda = { tags = { "modded" }, displayName = "Buganda" },
  Bulgaria = { tags = { "modded", "seasonal" }, displayName = "Bulgaria" },
  Burma = { tags = { "modded" }, displayName = "Burma" },
  Byzantium = { tags = { "vanilla", "seasonal" }, displayName = "Byzantium" },
  Canada = { tags = { "modded", "seasonal" }, displayName = "Canada" },
  Carthage = { tags = { "vanilla", "coastal", "seasonal" }, displayName = "Carthage" },
  Celts = { tags = { "vanilla", "seasonal" }, displayName = "Celts" },
  Chile = { tags = { "modded", "coastal" }, displayName = "Chile" },
  China = { tags = { "vanilla" }, displayName = "China" },
  Colombia = { tags = { "modded" }, displayName = "Colombia" },
  Cuba = { tags = { "modded", "seasonal" }, displayName = "Cuba" },
  Czechia = { tags = { "modded", "seasonal" }, displayName = "Czechia" },
  Denmark = { tags = { "vanilla", "coastal", "seasonal" }, displayName = "Denmark" },
  Egypt = { tags = { "vanilla" }, displayName = "Egypt" },
  England = { tags = { "vanilla", "coastal", "seasonal" }, displayName = "England" },
  Ethiopia = { tags = { "vanilla", "seasonal" }, displayName = "Ethiopia" },
  Finland = { tags = { "modded", "seasonal" }, displayName = "Finland" },
  France = { tags = { "vanilla", "seasonal" }, displayName = "France" },
  Franks = { tags = { "modded", "seasonal" }, displayName = "Franks" },
  Gaul = { tags = { "modded", "seasonal" }, displayName = "Gaul" },
  Georgia = { tags = { "modded" }, displayName = "Georgia" },
  Germany = { tags = { "vanilla", "seasonal" }, displayName = "Germany" },
  Golden = { tags = { "modded", "seasonal" }, displayName = "Golden Horde" },
  Goths = { tags = { "modded" }, displayName = "Goths" },
  Greece = { tags = { "vanilla", "seasonal" }, displayName = "Greece" },
  Hittites = { tags = { "modded", "seasonal" }, displayName = "Hittites" },
  Hungary = { tags = { "modded", "seasonal" }, displayName = "Hungary" },
  Huns = { tags = { "vanilla" }, displayName = "Huns" },
  Inca = { tags = { "vanilla" }, displayName = "Inca" },
  India = { tags = { "vanilla", "seasonal" }, displayName = "India" },
  Indonesia = { tags = { "vanilla", "coastal" }, displayName = "Indonesia" },
  Ireland = { tags = { "modded", "seasonal" }, displayName = "Ireland" },
  Iroquois = { tags = { "vanilla" }, displayName = "Iroquois" },
  Israel = { tags = { "modded", "seasonal" }, displayName = "Israel" },
  Italy = { tags = { "modded", "seasonal" }, displayName = "Italy" },
  Japan = { tags = { "vanilla", "coastal" }, displayName = "Japan" },
  Jerusalem = { tags = { "modded", "seasonal" }, displayName = "Jerusalem" },
  Khmer = { tags = { "modded" }, displayName = "Khmer" },
  Kilwa = { tags = { "modded", "coastal" }, displayName = "Kilwa" },
  Kongo = { tags = { "modded", "seasonal" }, displayName = "Kongo" },
  Korea = { tags = { "vanilla", "coastal", "seasonal" }, displayName = "Korea" },
  Lithuania = { tags = { "modded" }, displayName = "Lithuania" },
  Macedonian = { tags = { "modded" }, displayName = "Macedonia" },
  Madagascar = { tags = { "modded" }, displayName = "Madagascar" },
  Mali = { tags = { "modded" }, displayName = "Mali" },
  Manchuria = { tags = { "modded", "seasonal" }, displayName = "Manchuria" },
  Maori = { tags = { "modded" }, displayName = "Maori" },
  Maurya = { tags = { "modded", "seasonal" }, displayName = "Maurya" },
  Maya = { tags = { "vanilla" }, displayName = "Maya" },
  Mexican = { tags = { "modded", "seasonal" }, displayName = "Mexico" },
  Mongolia = { tags = { "vanilla", "seasonal" }, displayName = "Mongolia" },
  Moors = { tags = { "modded", "seasonal" }, displayName = "Moors" },
  Morocco = { tags = { "vanilla", "seasonal" }, displayName = "Morocco" },
  Mughals = { tags = { "modded" }, displayName = "Mughals" },
  Mysore = { tags = { "modded", "seasonal" }, displayName = "Mysore" },
  Nabatea = { tags = { "modded" }, displayName = "Nabatea" },
  Netherlands = { tags = { "vanilla", "coastal" }, displayName = "Netherlands" },
  NewZealand = { tags = { "modded", "coastal" }, displayName = "New Zealand" },
  Normandy = { tags = { "modded", "seasonal" }, displayName = "Normandy" },
  Norway = { tags = { "modded", "coastal", "seasonal" }, displayName = "Norway" },
  Nubia = { tags = { "modded" }, displayName = "Nubia" },
  Oman = { tags = { "modded", "coastal", "seasonal" }, displayName = "Oman" },
  Ottomans = { tags = { "vanilla" }, displayName = "Ottomans" },
  Palmyra = { tags = { "modded", "seasonal" }, displayName = "Palmyra" },
  Persia = { tags = { "vanilla" }, displayName = "Persia" },
  Philippines = { tags = { "modded", "coastal" }, displayName = "Philippines" },
  Phoenician = { tags = { "modded", "coastal" }, displayName = "Phoenician" },
  Poland = { tags = { "vanilla", "seasonal" }, displayName = "Poland" },
  Polynesia = { tags = { "vanilla", "coastal", "seasonal" }, displayName = "Polynesia" },
  Portugal = { tags = { "vanilla", "coastal" }, displayName = "Portugal" },
  Prussian = { tags = { "modded" }, displayName = "Prussia" },
  Romania = { tags = { "modded" }, displayName = "Romania" },
  Rome = { tags = { "vanilla", "seasonal" }, displayName = "Rome" },
  Russia = { tags = { "vanilla" }, displayName = "Russia" },
  Scotland = { tags = { "modded", "seasonal" }, displayName = "Scotland" },
  Shoshone = { tags = { "vanilla" }, displayName = "Shoshone" },
  Siam = { tags = { "vanilla", "seasonal" }, displayName = "Siam" },
  Sioux = { tags = { "modded" }, displayName = "Sioux" },
  Songhai = { tags = { "vanilla" }, displayName = "Songhai" },
  Spain = { tags = { "vanilla", "coastal" }, displayName = "Spain" },
  Sumeria = { tags = { "modded" }, displayName = "Sumeria" },
  Sweden = { tags = { "vanilla", "seasonal" }, displayName = "Sweden" },
  Switzerland = { tags = { "modded", "seasonal" }, displayName = "Switzerland" },
  Tibet = { tags = { "modded" }, displayName = "Tibet" },
  Timurids = { tags = { "modded", "seasonal" }, displayName = "Timurids" },
  Tonga = { tags = { "modded", "coastal" }, displayName = "Tonga" },
  Tunisia = { tags = { "modded", "coastal", "seasonal" }, displayName = "Tunisia" },
  Turkey = { tags = { "modded", "seasonal" }, displayName = "Turkey" },
  UAE = { tags = { "modded", "coastal", "seasonal" }, displayName = "UAE" },
  Ukraine = { tags = { "modded", "seasonal" }, displayName = "Ukraine" },
  Vatican = { tags = { "modded" }, displayName = "Vatican" },
  Venetian = { tags = { "modded", "coastal", "seasonal" }, displayName = "Venice" },
  Vietnam = { tags = { "modded" }, displayName = "Vietnam" },
  Wales = { tags = { "modded" }, displayName = "Wales" },
  Yugoslavia = { tags = { "modded", "seasonal" }, displayName = "Yugoslavia" },
  Zimbabwe = { tags = { "modded" }, displayName = "Zimbabwe" },
  Zulu = { tags = { "vanilla" }, displayName = "Zulu" },
}

-- Extra Type aliases when key does not match CIVILIZATION_<KEY>
LekmodDrafter.TypeAliases = {
  Golden = "CIVILIZATION_GOLDEN_HORDE",
  Mexican = "CIVILIZATION_MEXICO",
  Macedonian = "CIVILIZATION_MACEDON",
  Venetian = "CIVILIZATION_VENICE",
  Prussian = "CIVILIZATION_PRUSSIA",
  NewZealand = "CIVILIZATION_NEW_ZEALAND",
  Phoenician = "CIVILIZATION_PHOENICIA",
  UAE = "CIVILIZATION_UAE",
  Celts = "CIVILIZATION_CELTS",
  Huns = "CIVILIZATION_HUNS",
  Inca = "CIVILIZATION_INCA",
  Maya = "CIVILIZATION_MAYA",
  Aztec = "CIVILIZATION_AZTEC",
}

-- NQ Tournament 11 allow-list. Keys match CivMeta (Prussia/Venice/New Zealand use internal keys).
LekmodDrafter.Tournament11Keys = {
	"Akkad", "Aksum", "Arabia", "Argentina", "Assyria", "Aztec", "Belgium", "Brazil",
	"Buganda", "Burma", "Byzantium", "Carthage", "Chile", "China", "Colombia", "Czechia",
	"France", "Georgia", "Inca", "Japan", "Kilwa", "Kongo", "Lithuania", "Mali",
	"Manchuria", "Moors", "Mughals", "NewZealand", "Normandy", "Nubia", "Oman", "Persia",
	"Philippines", "Poland", "Polynesia", "Portugal", "Prussian", "Romania", "Rome", "Russia",
	"Songhai", "Spain", "Sumeria", "Switzerland", "Tibet", "Tunisia", "Turkey", "Vatican",
	"Venetian", "Wales",
}

LekmodDrafter.Tournament11 = {}
for _, key in ipairs(LekmodDrafter.Tournament11Keys) do
	LekmodDrafter.Tournament11[key] = true
end

function LekmodDrafter.IsTournamentCiv(meta)
	return meta ~= nil and meta.key ~= nil and LekmodDrafter.Tournament11[meta.key] == true
end

-- Civ IDs still legal under the active draft rules.
-- nil means no extra filter (full playable list).
function LekmodDrafter.RulesAllowSet(rules)
	rules = rules or LekmodDrafter.DefaultRules()
	if not rules.vanillaOnly and not rules.seasonalBans then
		return nil
	end
	local allow = {}
	for civID, meta in pairs(LekmodDrafter.BuildIDIndex()) do
		local ok = true
		if rules.vanillaOnly and not LekmodDrafter.HasTag(meta, "vanilla") then
			ok = false
		end
		if rules.seasonalBans and not LekmodDrafter.IsTournamentCiv(meta) then
			ok = false
		end
		if ok then
			allow[civID] = true
		end
	end
	return allow
end

-- Wrapped civ names for the Draft Rules checkbox (one source of truth with the allow-list).
function LekmodDrafter.TournamentListText()
	local names = {}
	for _, key in ipairs(LekmodDrafter.Tournament11Keys) do
		local meta = LekmodDrafter.CivMeta[key]
		table.insert(names, (meta and meta.displayName) or key)
	end
	table.sort(names)
	local lines = {}
	local row = {}
	for i, name in ipairs(names) do
		table.insert(row, name)
		if #row == 8 or i == #names then
			table.insert(lines, table.concat(row, ", "))
			row = {}
		end
	end
	return "Tournament 11 allowed civs:[NEWLINE]" .. table.concat(lines, "[NEWLINE]")
end

function LekmodDrafter.DefaultRules()
	return {
		bansPerPlayer = LekmodDrafter.DEFAULT_BANS,
		picksPerPlayer = LekmodDrafter.DEFAULT_PICKS,
		guaranteedCoastals = LekmodDrafter.GUARANTEE_RANDOM,
		guaranteedInlands = LekmodDrafter.GUARANTEE_RANDOM,
		vanillaOnly = false,
		seasonalBans = false,
	}
end

function LekmodDrafter.HasTag(meta, tag)
	if meta == nil or meta.tags == nil then
		return false
	end
	for _, t in ipairs(meta.tags) do
		if t == tag then
			return true
		end
	end
	return false
end

function LekmodDrafter.IsCoastal(meta)
	return LekmodDrafter.HasTag(meta, "coastal")
end

function LekmodDrafter.IsPlayableCiv(civ)
	if civ == nil then
		return false
	end
	local p = civ.Playable
	return p == 1 or p == true or p == "true" or p == "1"
end

-- Match a DB civ row to a CivMeta key (best effort).
function LekmodDrafter.FindMetaKeyForCiv(civ)
	if civ == nil then
		return nil
	end
	local typeName = tostring(civ.Type or "")
	-- Exact alias reverse lookup
	for key, alias in pairs(LekmodDrafter.TypeAliases) do
		if alias == typeName then
			return key
		end
	end
	-- CIVILIZATION_FOO / CIVILIZATION_MC_FOO → Foo / FOO key variants
	local suffix = string.match(typeName, "^CIVILIZATION_(?:MC_|LEKMOD_|D3_)?(.+)$")
	if suffix ~= nil then
		-- Try exact CivMeta key casings
		if LekmodDrafter.CivMeta[suffix] ~= nil then
			return suffix
		end
		-- Title-case / common forms
		local lower = string.lower(suffix)
		for key, _ in pairs(LekmodDrafter.CivMeta) do
			if string.lower(key) == lower then
				return key
			end
			-- underscores removed
			if string.lower(string.gsub(key, "_", "")) == string.lower(string.gsub(suffix, "_", "")) then
				return key
			end
		end
	end
	-- Display / short description match
	local shortDesc = Locale.Lookup(civ.ShortDescription or "")
	if shortDesc ~= nil and shortDesc ~= "" then
		local want = string.lower(shortDesc)
		for key, meta in pairs(LekmodDrafter.CivMeta) do
			if string.lower(meta.displayName) == want then
				return key
			end
		end
	end
	return nil
end

-- Resolve drafter key -> civilization ID (or nil if not in DB / not playable).
function LekmodDrafter.ResolveCivID(key)
	if key == nil then
		return nil
	end
	local alias = LekmodDrafter.TypeAliases[key]
	local candidates = {}
	if alias ~= nil then
		table.insert(candidates, alias)
	end
	table.insert(candidates, "CIVILIZATION_" .. string.upper(key))
	table.insert(candidates, "CIVILIZATION_MC_" .. string.upper(key))
	table.insert(candidates, "CIVILIZATION_LEKMOD_" .. string.upper(key))

	for _, typeName in ipairs(candidates) do
		local civ = GameInfo.Civilizations[typeName]
		if LekmodDrafter.IsPlayableCiv(civ) then
			return civ.ID
		end
	end

	local meta = LekmodDrafter.CivMeta[key]
	local want = meta and string.lower(meta.displayName) or string.lower(key)
	for civ in GameInfo.Civilizations() do
		if LekmodDrafter.IsPlayableCiv(civ) then
			local shortDesc = Locale.Lookup(civ.ShortDescription or "")
			if shortDesc ~= nil and string.lower(shortDesc) == want then
				return civ.ID
			end
		end
	end
	return nil
end

-- Build map civID -> meta for all playable civs (DB-first so pool is never empty).
function LekmodDrafter.BuildIDIndex()
	local byID = {}
	-- Prefer the same query StagingRoom uses for civ pulldowns (reliable in lobby).
	local ok, err = pcall(function()
		for row in DB.Query([[SELECT ID, Type, ShortDescription, Description, Playable
			FROM Civilizations WHERE Playable = 1]]) do
			local civ = {
				ID = row.ID,
				Type = row.Type,
				ShortDescription = row.ShortDescription,
				Description = row.Description,
				Playable = row.Playable,
			}
			local key = LekmodDrafter.FindMetaKeyForCiv(civ)
			local meta = key and LekmodDrafter.CivMeta[key] or nil
			byID[civ.ID] = {
				key = key or civ.Type,
				tags = (meta and meta.tags) or { "modded" },
				displayName = (meta and meta.displayName) or Locale.Lookup(civ.ShortDescription or civ.Description or civ.Type),
			}
		end
	end)
	if not ok or next(byID) == nil then
		-- Fallback: GameInfo iterator
		for civ in GameInfo.Civilizations() do
			if LekmodDrafter.IsPlayableCiv(civ) then
				local key = LekmodDrafter.FindMetaKeyForCiv(civ)
				local meta = key and LekmodDrafter.CivMeta[key] or nil
				byID[civ.ID] = {
					key = key or civ.Type,
					tags = (meta and meta.tags) or { "modded" },
					displayName = (meta and meta.displayName) or Locale.Lookup(civ.ShortDescription or civ.Description or civ.Type),
				}
			end
		end
	end
	return byID
end

function LekmodDrafter.NormalizeGuarantee(value)
	local n = tonumber(value)
	if n == nil or n < 0 then
		return 0
	end
	return n
end

--[[
  rules: DefaultRules()-shaped table
  playerOrder: array of playerIDs (humans) in deal order
  bansByPlayer: [playerID] = { civID, civID, ... } (may contain nils/duplicates)
  returns: { ok=bool, error=string|nil, drafts={[playerID]={civID,...}}, warning=string|nil }
]]
function LekmodDrafter.CreateDraft(rules, playerOrder, bansByPlayer)
	rules = rules or LekmodDrafter.DefaultRules()
	playerOrder = playerOrder or {}
	bansByPlayer = bansByPlayer or {}

	local picks = math.max(1, tonumber(rules.picksPerPlayer) or LekmodDrafter.DEFAULT_PICKS)
	local players = #playerOrder
	local needed = players * picks
	local coastNeed = LekmodDrafter.NormalizeGuarantee(rules.guaranteedCoastals)
	local inlandNeed = LekmodDrafter.NormalizeGuarantee(rules.guaranteedInlands)

	if coastNeed + inlandNeed > picks then
		return {
			ok = false,
			error = "Guaranteed coastals + inlands exceed picks per player.",
		}
	end

	local byID = LekmodDrafter.BuildIDIndex()
	local banned = {}
	for _, pid in ipairs(playerOrder) do
		local list = bansByPlayer[pid] or {}
		for _, civID in ipairs(list) do
			if civID ~= nil and civID >= 0 then
				banned[civID] = true
			end
		end
	end

	local allowed = {}
	local coastal = {}
	local inland = {}
	for civID, meta in pairs(byID) do
		if banned[civID] then
			-- skip
		elseif rules.vanillaOnly and not LekmodDrafter.HasTag(meta, "vanilla") then
			-- skip
		elseif rules.seasonalBans and not LekmodDrafter.IsTournamentCiv(meta) then
			-- NQ Tournament 11: every other playable civ is banned
		else
			table.insert(allowed, civID)
			if LekmodDrafter.IsCoastal(meta) then
				table.insert(coastal, civID)
			else
				table.insert(inland, civID)
			end
		end
	end

	if needed > #allowed then
		return {
			ok = false,
			error = string.format("Not enough civilizations for draft (%d needed, %d available).", needed, #allowed),
		}
	end
	if coastNeed > 0 and #coastal < coastNeed * players then
		return {
			ok = false,
			error = string.format("Not enough coastal civs (%d needed, %d available).", coastNeed * players, #coastal),
		}
	end
	if inlandNeed > 0 and #inland < inlandNeed * players then
		return {
			ok = false,
			error = string.format("Not enough inland civs (%d needed, %d available).", inlandNeed * players, #inland),
		}
	end

	local function removeFrom(list, value)
		for i = #list, 1, -1 do
			if list[i] == value then
				table.remove(list, i)
				return true
			end
		end
		return false
	end

	local function takeRandom(list)
		if #list == 0 then
			return nil
		end
		local idx = math.random(1, #list)
		local value = list[idx]
		table.remove(list, idx)
		return value
	end

	local workingAllowed = {}
	local workingCoastal = {}
	local workingInland = {}
	for _, v in ipairs(allowed) do
		table.insert(workingAllowed, v)
	end
	for _, v in ipairs(coastal) do
		table.insert(workingCoastal, v)
	end
	for _, v in ipairs(inland) do
		table.insert(workingInland, v)
	end

	local drafts = {}
	local warning = nil
	for _, pid in ipairs(playerOrder) do
		local picksForPlayer = {}
		local cLeft = coastNeed
		local iLeft = inlandNeed
		for _ = 1, picks do
			local selected = nil
			if cLeft > 0 and #workingCoastal > 0 then
				selected = takeRandom(workingCoastal)
				cLeft = cLeft - 1
			elseif iLeft > 0 and #workingInland > 0 then
				selected = takeRandom(workingInland)
				iLeft = iLeft - 1
			else
				selected = takeRandom(workingAllowed)
			end
			if selected ~= nil then
				removeFrom(workingAllowed, selected)
				removeFrom(workingCoastal, selected)
				removeFrom(workingInland, selected)
				table.insert(picksForPlayer, selected)
			else
				warning = "Draft filled incompletely for some players."
			end
		end
		drafts[pid] = picksForPlayer
	end

	return { ok = true, error = nil, drafts = drafts, warning = warning }
end

-- Encode helpers for #LDRAFT# packets (compact). Preserves empty slots as -1.
function LekmodDrafter.EncodeCivList(list)
	local parts = {}
	for _, id in ipairs(list or {}) do
		local n = tonumber(id)
		if n == nil then
			n = -1
		end
		table.insert(parts, tostring(n))
	end
	return table.concat(parts, ",")
end

function LekmodDrafter.DecodeCivList(text)
	local out = {}
	if text == nil or text == "" then
		return out
	end
	for token in string.gmatch(text, "[^,]+") do
		local n = tonumber(token)
		if n ~= nil then
			table.insert(out, n)
		end
	end
	return out
end

