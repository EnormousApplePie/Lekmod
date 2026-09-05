------------------------------------------------------------------------------
-- Staging Room civ draft / ban UI + #LDRAFT# sync
-- Included by StagingRoom.lua (shares Controls / Matchmaking / PreGame).
------------------------------------------------------------------------------
include("Lekmod_drafter.lua");

local DRAFT_PREFIX = "#LDRAFT#";
local SLOT_ROW_HEIGHT = 122; -- ~14% taller than vanilla 107 for draft-row spacing

-- Wide lobby = player + ban + summary (1275). Compact = player + ban only (~960),
-- used when the screen cannot fit the wide layout (e.g. 1280x720). No proportional
-- scaling of columns — that caused clipping; we just hide the summary strip.
local LOBBY_WIDE_W = 1275;
local LOBBY_COMPACT_W = 960;
local LOBBY_COMPACT_MAX_SCREEN = 1320; -- below this, use compact
g_LobbyCompact = g_LobbyCompact or false;
g_CompactSecondary = g_CompactSecondary or "draft"; -- shared top tab: "draft" | "options"
g_CompactRightPane = g_CompactRightPane or "bans"; -- right column: "bans" | "summary"

function Draft_IsLobbyCompact()
	return g_LobbyCompact == true;
end

function Draft_GetBanColumnWidth()
	return 310;
end

function Draft_GetPlayerColumnWidth()
	return 575;
end

function Draft_GetHostColumnWidth()
	return 610;
end

-- Page mode: "players" | "draft" | "options"
g_DraftPageMode = g_DraftPageMode or "players";

g_DraftRules = g_DraftRules or LekmodDrafter.DefaultRules();
g_DraftBans = g_DraftBans or {};          -- [playerID] = { civID or -1, ... }
g_DraftBanReady = g_DraftBanReady or {};  -- [playerID] = true/false
-- True only after we locally check ban-ready, until PreGame confirms our R_* bit.
-- Must NOT keep ready forever — that blocked host clears on rules change.
g_DraftBanReadyPendingLocal = g_DraftBanReadyPendingLocal or false;
g_DraftPools = g_DraftPools or {};        -- [playerID] = { civID, ... }
g_DraftLocked = g_DraftLocked or false;
g_DraftSwapDesire = g_DraftSwapDesire or {}; -- [playerID] = otherPlayerID they want to swap drafts with
g_DraftSwapFirst = nil; -- legacy unused
g_DraftParticipantKey = g_DraftParticipantKey or nil; -- legacy; prefer g_DraftParticipantCount
g_DraftParticipantCount = g_DraftParticipantCount or nil; -- draft deal size at create; clear only if count grows
g_PreviousDraftSnapshot = g_PreviousDraftSnapshot or nil; -- last successful Create Draft (for Restore)

g_DraftBanHostControl = g_DraftBanHostControl or {}; -- [playerID] = true if they let the host edit their bans
g_BanSlotInstances = g_BanSlotInstances or {}; -- [selectionIndex] = BanSlot instance (1..MAX)
g_BanIconIMs = g_BanIconIMs or {};             -- [stackControl] = InstanceManager
g_BanIconControls = g_BanIconControls or {};   -- [stackControl] = { BanIconInstance, ... }
g_DraftIconIMs = g_DraftIconIMs or {};         -- [stackControl] = InstanceManager
g_PendingBan = g_PendingBan or nil;            -- { playerID, slotIndex }
g_SkipStagingFullRefresh = g_SkipStagingFullRefresh or false; -- set when opening Civilopedia
-- Once true, Draft_Init must not re-load PreGame over live chat-synced ban-ready.
g_DraftLiveSession = g_DraftLiveSession or false;

local function IsHumanSlot(playerID)
	if playerID == nil or playerID < 0 then
		return false;
	end
	local status = PreGame.GetSlotStatus(playerID);
	return status == SlotStatus.SS_TAKEN;
end

local function IsAISlot(playerID)
	if playerID == nil or playerID < 0 then
		return false;
	end
	return PreGame.GetSlotStatus(playerID) == SlotStatus.SS_COMPUTER;
end

-- Empty "Human Required" slots: show ban UI as if occupied (solo-test friendly).
local function IsHumanRequiredSlot(playerID)
	if playerID == nil or playerID < 0 then
		return false;
	end
	return PreGame.GetSlotStatus(playerID) == SlotStatus.SS_OPEN
		and PreGame.GetSlotClaim(playerID) == SlotClaim.SLOTCLAIM_RESERVED;
end

-- Draft participants: humans, human-required stand-ins, and AI (host-controlled).
local function IsBanParticipantSlot(playerID)
	return IsHumanSlot(playerID) or IsHumanRequiredSlot(playerID) or IsAISlot(playerID);
end

local function IsGameReady(playerID)
	if playerID == nil or playerID < 0 or PreGame == nil or PreGame.IsReady == nil then
		return false;
	end
	return PreGame.IsReady(playerID) == true;
end

-- True only when every taken human is ready-to-start (countdown about to run / running).
-- A single player greening up must NOT freeze draft edits for everyone else.
local function IsDraftLockedByGameReady()
	if PreGame == nil or PreGame.IsReady == nil then
		return false;
	end
	local foundHuman = false;
	local max = GameDefines.MAX_MAJOR_CIVS;
	for i = 0, max - 1 do
		if PreGame.GetSlotStatus(i) == SlotStatus.SS_TAKEN then
			foundHuman = true;
			if not PreGame.IsReady(i) then
				return false;
			end
		end
	end
	return foundHuman;
end

-- Host may edit AI bans the same way they pick AI civs — until ban-ready or game-ready.
-- Humans may temporarily cede ban control to the host (g_DraftBanHostControl).
local function CanEditBansForPlayer(playerID)
	if playerID == nil or g_DraftLocked then
		return false;
	end
	if Draft_IsHistoryOnly ~= nil and Draft_IsHistoryOnly() then
		return false;
	end
	-- All humans ready-to-start (countdown) freezes ban edits (own + AI).
	if IsDraftLockedByGameReady() then
		return false;
	end
	local localID = Matchmaking.GetLocalID();
	-- Ban-ready locks your own bans and (for host) AI / ceded bans until you unready.
	if g_DraftBanReady[localID] == true then
		return false;
	end
	if playerID == localID then
		-- You gave control to the host — wait until you reclaim it.
		return g_DraftBanHostControl[localID] ~= true;
	end
	if not Matchmaking.IsHost() then
		return false;
	end
	-- Even if ceded, a human who readied their bans is locked.
	if g_DraftBanReady[playerID] == true and not IsAISlot(playerID) then
		return false;
	end
	return IsAISlot(playerID) or g_DraftBanHostControl[playerID] == true;
end

----------------------------------------------------------------
-- Persist draft into PreGame game-options (survives lobby config / game saves)
----------------------------------------------------------------
local DRAFT_OPT_VER = "GAMEOPTION_LEKMOD_DRAFT_V";
local DRAFT_OPT_RULES = "GAMEOPTION_LEKMOD_DRAFT_RULES";
local DRAFT_OPT_READY = "GAMEOPTION_LEKMOD_DRAFT_READY"; -- host aggregate mask (save / fallback)
local DRAFT_OPT_HOSTCTRL = "GAMEOPTION_LEKMOD_DRAFT_HOSTCTRL";
local DRAFT_OPT_LAUNCHED = "GAMEOPTION_LEKMOD_DRAFT_LAUNCHED";
local DRAFT_SAVE_VERSION = 1;
local DRAFT_MAX_BANS_STORE = 5;
local DRAFT_MAX_PICKS_STORE = 10;

-- Per-player ban-ready bit (mirrors PreGame.SetReady / IsReady).
-- Each client only writes their own slot so they cannot clobber others' ready state.
local function Draft_ReadyOptName(pid)
	return string.format("GAMEOPTION_LEKMOD_DRAFT_R_%d", pid);
end

local function Draft_OptGet(name)
	local v = PreGame.GetGameOption(name);
	if v == nil then
		return 0;
	end
	return tonumber(v) or 0;
end

local function Draft_OptSet(name, value)
	PreGame.SetGameOption(name, math.floor(tonumber(value) or 0));
end

-- Ongoing / continue-game lobby: show draft history (no create/edit/swap).
-- Pre-start lobby config saves stay editable. Turn 0 is a valid started turn, so we
-- cannot use TurnNumber / GetLoadFileName; we persist LAUNCHED when the game starts.
function Draft_IsHistoryOnly()
	if PreGame.GameStarted() == true then
		return true;
	end
	return Draft_OptGet(DRAFT_OPT_LAUNCHED) == 1;
end

-- Stamp PreGame so later loads of this save are draft history-only (incl. turn 0).
function Draft_MarkGameLaunched()
	if PreGame == nil or PreGame.SetGameOption == nil then
		return;
	end
	Draft_OptSet(DRAFT_OPT_LAUNCHED, 1);
end

local function Draft_Pow2(bit)
	local v = 1;
	for _ = 1, bit do
		v = v * 2;
	end
	return v;
end

local function Draft_PackRules()
	local r = g_DraftRules or LekmodDrafter.DefaultRules();
	local bans = math.max(0, math.min(15, tonumber(r.bansPerPlayer) or 0));
	local picks = math.max(0, math.min(15, tonumber(r.picksPerPlayer) or 0));
	local coast = tonumber(r.guaranteedCoastals);
	if coast == nil then coast = -1; end
	local inland = tonumber(r.guaranteedInlands);
	if inland == nil then inland = -1; end
	local coastN = math.max(0, math.min(15, coast + 1));
	local inlandN = math.max(0, math.min(15, inland + 1));
	local flags = 0;
	if r.vanillaOnly then flags = flags + 1; end
	if r.seasonalBans then flags = flags + 2; end
	if g_DraftLocked then flags = flags + 4; end
	return bans + picks * 16 + coastN * 256 + inlandN * 4096 + flags * 65536;
end

local function Draft_UnpackRules(packed)
	packed = tonumber(packed) or 0;
	local bans = packed % 16;
	local picks = math.floor(packed / 16) % 16;
	local coastN = math.floor(packed / 256) % 16;
	local inlandN = math.floor(packed / 4096) % 16;
	local flags = math.floor(packed / 65536) % 16;
	g_DraftRules = {
		bansPerPlayer = bans,
		picksPerPlayer = math.max(1, picks),
		guaranteedCoastals = coastN - 1,
		guaranteedInlands = inlandN - 1,
		vanillaOnly = (flags % 2) == 1,
		seasonalBans = (math.floor(flags / 2) % 2) == 1,
	};
	g_DraftLocked = (math.floor(flags / 4) % 2) == 1;
end

function Draft_ClearPreGamePersist()
	if PreGame == nil or PreGame.SetGameOption == nil then
		return;
	end
	Draft_OptSet(DRAFT_OPT_VER, 0);
	Draft_OptSet(DRAFT_OPT_RULES, 0);
	Draft_OptSet(DRAFT_OPT_READY, 0);
	Draft_OptSet(DRAFT_OPT_HOSTCTRL, 0);
	Draft_OptSet(DRAFT_OPT_LAUNCHED, 0);
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, maxP - 1 do
		Draft_OptSet(Draft_ReadyOptName(pid), 0);
		Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_PN_%d", pid), 0);
		for i = 1, DRAFT_MAX_BANS_STORE do
			Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_B_%d_%d", pid, i), 0);
		end
		for i = 1, DRAFT_MAX_PICKS_STORE do
			Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_P_%d_%d", pid, i), 0);
		end
	end
end

-- Write current draft Lua state into PreGame (included in lobby/game saves).
function Draft_PersistToPreGame()
	if PreGame == nil or PreGame.SetGameOption == nil then
		return;
	end
	Draft_OptSet(DRAFT_OPT_VER, DRAFT_SAVE_VERSION);
	-- Rules are host-authoritative (same as MPGameOptions). Clients must not
	-- overwrite GAMEOPTION_LEKMOD_DRAFT_RULES with a stale local pack.
	if Matchmaking.IsHost() then
		Draft_OptSet(DRAFT_OPT_RULES, Draft_PackRules());
	end

	local maxP = GameDefines.MAX_MAJOR_CIVS;
	-- Each player writes only their own ban-ready slot (same idea as PreGame.SetReady).
	-- Never let the host mass-overwrite others' R_* bits here — that caused ready desyncs.
	local localID = Matchmaking.GetLocalID();
	if localID ~= nil and localID >= 0 then
		Draft_OptSet(Draft_ReadyOptName(localID), (g_DraftBanReady[localID] == true) and 1 or 0);
	end
	-- Host aggregates the legacy mask + host-ctrl for lobby saves / older clients.
	if Matchmaking.IsHost() then
		local readyMask = 0;
		for pid = 0, maxP - 1 do
			if g_DraftBanReady[pid] == true then
				readyMask = readyMask + Draft_Pow2(pid);
			end
		end
		Draft_OptSet(DRAFT_OPT_READY, readyMask);

		local hostCtrlMask = 0;
		for pid = 0, maxP - 1 do
			if g_DraftBanHostControl[pid] == true then
				hostCtrlMask = hostCtrlMask + Draft_Pow2(pid);
			end
		end
		Draft_OptSet(DRAFT_OPT_HOSTCTRL, hostCtrlMask);
	end

	for pid = 0, maxP - 1 do
		local bans = g_DraftBans[pid] or {};
		for i = 1, DRAFT_MAX_BANS_STORE do
			local civ = tonumber(bans[i]);
			local store = 0;
			if civ ~= nil and civ >= 0 then
				store = civ + 1;
			end
			Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_B_%d_%d", pid, i), store);
		end
		local pool = g_DraftPools[pid] or {};
		local poolLen = math.min(DRAFT_MAX_PICKS_STORE, #pool);
		Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_PN_%d", pid), poolLen);
		for i = 1, DRAFT_MAX_PICKS_STORE do
			local civ = tonumber(pool[i]);
			local store = 0;
			if civ ~= nil and civ >= 0 then
				store = civ + 1;
			end
			Draft_OptSet(string.format("GAMEOPTION_LEKMOD_DRAFT_P_%d_%d", pid, i), store);
		end
	end
end

-- Load draft state from PreGame if a prior save wrote it. Returns true if restored.
function Draft_RestoreFromPreGame()
	if PreGame == nil or PreGame.GetGameOption == nil then
		return false;
	end
	local ver = Draft_OptGet(DRAFT_OPT_VER);
	if ver < 1 then
		return false;
	end

	Draft_UnpackRules(Draft_OptGet(DRAFT_OPT_RULES));
	g_DraftBans = {};
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	g_DraftBanHostControl = {};
	g_DraftPools = {};
	g_DraftSwapDesire = {};
	g_DraftSwapFirst = nil;

	local maxP = GameDefines.MAX_MAJOR_CIVS;
	local readyMask = Draft_OptGet(DRAFT_OPT_READY);
	local hostCtrlMask = Draft_OptGet(DRAFT_OPT_HOSTCTRL);
	local banSlots = math.max(0, math.min(DRAFT_MAX_BANS_STORE, tonumber(g_DraftRules.bansPerPlayer) or 0));
	local anyPerPlayerReady = false;
	for pid = 0, maxP - 1 do
		if Draft_OptGet(Draft_ReadyOptName(pid)) == 1 then
			anyPerPlayerReady = true;
			break;
		end
	end
	for pid = 0, maxP - 1 do
		if anyPerPlayerReady then
			if Draft_OptGet(Draft_ReadyOptName(pid)) == 1 then
				g_DraftBanReady[pid] = true;
			end
		elseif math.floor(readyMask / Draft_Pow2(pid)) % 2 == 1 then
			g_DraftBanReady[pid] = true;
		end
		if math.floor(hostCtrlMask / Draft_Pow2(pid)) % 2 == 1 then
			g_DraftBanHostControl[pid] = true;
		end
		local bans = {};
		for i = 1, banSlots do
			local store = Draft_OptGet(string.format("GAMEOPTION_LEKMOD_DRAFT_B_%d_%d", pid, i));
			if store > 0 then
				table.insert(bans, store - 1);
			else
				table.insert(bans, -1);
			end
		end
		g_DraftBans[pid] = bans;

		local poolLen = Draft_OptGet(string.format("GAMEOPTION_LEKMOD_DRAFT_PN_%d", pid));
		poolLen = math.max(0, math.min(DRAFT_MAX_PICKS_STORE, poolLen));
		if poolLen > 0 then
			local pool = {};
			for i = 1, poolLen do
				local store = Draft_OptGet(string.format("GAMEOPTION_LEKMOD_DRAFT_P_%d_%d", pid, i));
				if store > 0 then
					table.insert(pool, store - 1);
				end
			end
			g_DraftPools[pid] = pool;
		end
	end

	g_DraftParticipantKey = nil; -- set by Draft_Init after locals are available
	return true;
end

-- Real humans only (must ban-ready before Create Draft). AI are auto-ready.
local function GetHumanPlayerOrder()
	local order = {};
	local max = GameDefines.MAX_MAJOR_CIVS;
	for i = 0, max - 1 do
		if IsHumanSlot(i) then
			table.insert(order, i);
		end
	end
	return order;
end

-- Draft deal order: humans + human-required + AI.
local function GetDraftPlayerOrder()
	local order = {};
	local max = GameDefines.MAX_MAJOR_CIVS;
	for i = 0, max - 1 do
		if IsBanParticipantSlot(i) then
			table.insert(order, i);
		end
	end
	return order;
end

local function GetDraftParticipantKey()
	local parts = {};
	for _, pid in ipairs(GetDraftPlayerOrder()) do
		table.insert(parts, tostring(pid));
	end
	return table.concat(parts, ",");
end

local function GetDraftParticipantCount()
	return #GetDraftPlayerOrder();
end

local function CopyDraftPools(pools)
	local out = {};
	if pools == nil then
		return out;
	end
	for pid, pool in pairs(pools) do
		local copy = {};
		if pool ~= nil then
			for i, civID in ipairs(pool) do
				copy[i] = civID;
			end
		end
		out[pid] = copy;
	end
	return out;
end

local function CopyBanReady(ready)
	local out = {};
	if ready == nil then
		return out;
	end
	for pid, v in pairs(ready) do
		if v == true then
			out[pid] = true;
		end
	end
	return out;
end

local function Draft_SavePreviousSnapshot()
	g_PreviousDraftSnapshot = {
		pools = CopyDraftPools(g_DraftPools),
		banReady = CopyBanReady(g_DraftBanReady),
		participantCount = GetDraftParticipantCount(),
	};
end

local function CanSelectDraftCivForPlayer(playerID)
	if playerID == nil or not g_DraftLocked then
		return false;
	end
	if Draft_IsHistoryOnly ~= nil and Draft_IsHistoryOnly() then
		return false;
	end
	if PreGame.GetLoadFileName ~= nil and PreGame.GetLoadFileName() ~= "" and PreGame.GameStarted() then
		return false;
	end
	local localID = Matchmaking.GetLocalID();
	if IsHumanSlot(playerID) then
		if PreGame.IsReady(playerID) then
			return false;
		end
		return playerID == localID;
	end
	-- Host may pick for AI / Human Required the same way as the civ pulldown.
	if not Matchmaking.IsHost() then
		return false;
	end
	if PreGame.IsReady(localID) then
		return false;
	end
	return IsAISlot(playerID) or IsHumanRequiredSlot(playerID);
end

local function ResetAllIconInstanceManagers()
	for _, im in pairs(g_BanIconIMs) do
		pcall(function() im:ResetInstances(); end);
	end
	for _, im in pairs(g_DraftIconIMs) do
		pcall(function() im:ResetInstances(); end);
	end
	g_BanIconIMs = {};
	g_BanIconControls = {};
	g_DraftIconIMs = {};
end

local function EnsureBanArray(playerID)
	g_DraftBans[playerID] = g_DraftBans[playerID] or {};
	local n = g_DraftRules.bansPerPlayer or LekmodDrafter.DEFAULT_BANS;
	while #g_DraftBans[playerID] < n do
		table.insert(g_DraftBans[playerID], -1);
	end
	while #g_DraftBans[playerID] > n do
		table.remove(g_DraftBans[playerID]);
	end
	return g_DraftBans[playerID];
end

local function SendDraftChat(body)
	if PreGame.IsHotSeatGame() then
		return;
	end
	Network.SendChat(DRAFT_PREFIX .. body);
end

local function Draft_PackReadyMask()
	local readyMask = 0;
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, maxP - 1 do
		if g_DraftBanReady[pid] == true then
			readyMask = readyMask + Draft_Pow2(pid);
		end
	end
	return readyMask;
end

local function Draft_ApplyReadyMask(mask, preserveLocalOptimistic)
	mask = tonumber(mask) or 0;
	local localID = Matchmaking.GetLocalID();
	local localBitSet = localID ~= nil and (math.floor(mask / Draft_Pow2(localID)) % 2 == 1);
	-- Only keep an unconfirmed local ready click — never ignore a host clear after rules reset.
	local keepLocal = preserveLocalOptimistic
		and g_DraftBanReadyPendingLocal
		and not Matchmaking.IsHost()
		and localID ~= nil
		and g_DraftBanReady[localID] == true
		and not localBitSet;
	g_DraftBanReady = {};
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, maxP - 1 do
		if math.floor(mask / Draft_Pow2(pid)) % 2 == 1 then
			g_DraftBanReady[pid] = true;
		end
	end
	if keepLocal then
		g_DraftBanReady[localID] = true;
	else
		g_DraftBanReadyPendingLocal = false;
	end
end

-- Host echo so every client converges on the same green-up / Create Draft gate.
-- Event-driven only (toggle / kick / restore) — do not spam chat on a timer.
function Draft_BroadcastReadyMask()
	if not Matchmaking.IsHost() or PreGame.IsHotSeatGame() or Draft_IsHistoryOnly() then
		return;
	end
	SendDraftChat("READYMASK|" .. tostring(Draft_PackReadyMask()));
end

-- Pull ban-ready like PreGame.IsReady(): each player's own GameOption bit.
-- Returns true if g_DraftBanReady changed (caller may refresh UI).
-- Host reads per-player R_* (players own their bits). Clients prefer the host
-- aggregate DRAFT_OPT_READY mask so a rules-reset (mask 0) is not undone by
-- stale remote R_* bits the host cannot reliably clear.
function Draft_PullBanReadyFromPreGame()
	if PreGame == nil or PreGame.GetGameOption == nil or PreGame.IsHotSeatGame() then
		return false;
	end
	local localID = Matchmaking.GetLocalID();
	local changed = false;
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	local mask = Draft_OptGet(DRAFT_OPT_READY);
	local isHost = Matchmaking.IsHost();

	for pid = 0, maxP - 1 do
		local ready;
		if isHost then
			ready = Draft_OptGet(Draft_ReadyOptName(pid)) == 1;
		else
			ready = math.floor(mask / Draft_Pow2(pid)) % 2 == 1;
		end
		-- Keep only a pending local ready click until host mask / our bit lands.
		if pid == localID and g_DraftBanReadyPendingLocal and g_DraftBanReady[localID] == true and not ready then
			ready = true;
		elseif pid == localID and ready then
			g_DraftBanReadyPendingLocal = false;
		end
		if (g_DraftBanReady[pid] == true) ~= ready then
			changed = true;
		end
		if ready then
			g_DraftBanReady[pid] = true;
		else
			g_DraftBanReady[pid] = nil;
		end
	end
	return changed;
end

local function Draft_WriteLocalBanReadyOption(bChecked)
	local localID = Matchmaking.GetLocalID();
	if localID == nil or localID < 0 or PreGame == nil or PreGame.SetGameOption == nil then
		return;
	end
	Draft_OptSet(Draft_ReadyOptName(localID), bChecked and 1 or 0);
end

local function AnnounceGame(msg)
	if LekmodVersion ~= nil and LekmodVersion.EncodeGameChat ~= nil then
		Network.SendChat(LekmodVersion.EncodeGameChat(msg));
	else
		Network.SendChat(msg);
	end
end

function Draft_IsProtocol(text)
	return text ~= nil and string.sub(text, 1, #DRAFT_PREFIX) == DRAFT_PREFIX;
end

function Draft_IsDraftLocked()
	return g_DraftLocked == true;
end

-- Wipe all draft lobby state. Only call when the host voids the lobby
-- (backs out) or the local client leaves their session / launches.
-- clearPersist: also wipe PreGame-saved draft (leave-lobby). Do NOT clear on
-- game launch � mid-game / hotjoin lobbies still need the saved draft history.
function Draft_ResetState(clearPersist)
	g_DraftPageMode = "players";
	g_DraftRules = LekmodDrafter.DefaultRules();
	g_DraftBans = {};
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	g_DraftBanHostControl = {};
	g_DraftPools = {};
	g_DraftLocked = false;
	g_DraftSwapFirst = nil;
	g_DraftSwapDesire = {};
	g_DraftParticipantKey = nil;
	g_DraftParticipantCount = nil;
	g_PreviousDraftSnapshot = nil;
	g_PendingBan = nil;
	g_HScrollOffset = {};
	g_DraftLiveSession = false;
	ResetAllIconInstanceManagers();
	Draft_HideBanPicker();
	if clearPersist then
		Draft_ClearPreGamePersist();
	end
	-- Restore Name@Team / hide draft rows (g_DraftLocked already false).
	Draft_RefreshDraftIconsAll();
end

-- Host-only: clear a kicked player's bans/ready/pool. Normal disconnects and
-- slot replacements must NOT call this (bans stay with the slot).
function Draft_OnPlayerKicked(playerID)
	if playerID == nil or playerID < 0 then
		return;
	end
	if not Matchmaking.IsHost() then
		return;
	end
	g_DraftBans[playerID] = {};
	EnsureBanArray(playerID);
	g_DraftBanReady[playerID] = false;
	g_DraftBanHostControl[playerID] = nil;
	g_DraftPools[playerID] = nil;
	g_DraftSwapDesire[playerID] = nil;
	if g_DraftSwapFirst == playerID then
		g_DraftSwapFirst = nil;
	end
	Draft_OptSet(Draft_ReadyOptName(playerID), 0);
	Draft_BroadcastBans(playerID);
	SendDraftChat("BANREADY|" .. tostring(playerID) .. "|0");
	SendDraftChat("BANCTRL|" .. tostring(playerID) .. "|0");
	if g_DraftLocked then
		SendDraftChat("DRAFT|" .. tostring(playerID) .. "|");
	end
	Draft_BroadcastReadyMask();
	Draft_RefreshBanUI();
	Draft_RefreshDraftIconsAll();
	Draft_UpdateActionButtons();
end

function Draft_GetPoolForPlayer(playerID)
	return g_DraftPools[playerID];
end

function Draft_CivAllowedForPlayer(playerID, civID)
	if not g_DraftLocked then
		return true;
	end
	local pool = g_DraftPools[playerID];
	if pool == nil or #pool == 0 then
		return true;
	end
	if civID == nil or civID < 0 then
		return false; -- no Random while draft-locked
	end
	for _, id in ipairs(pool) do
		if id == civID then
			return true;
		end
	end
	return false;
end

----------------------------------------------------------------
-- Ban icon rendering + temporary picker under the clicked icon
----------------------------------------------------------------
local g_BanPickerIM = nil;
g_BanPickerScrollPos = g_BanPickerScrollPos or 0; -- remember vertical picker scroll across opens
g_BanHostIconCount = g_BanHostIconCount or nil;   -- reset H-scroll only when ban-slot count changes
g_BanSlotIconCount = g_BanSlotIconCount or {};    -- [selectionIndex] = last ban count

local g_HScrollOffset = g_HScrollOffset or {}; -- [scrollKey] = pixel offset from left

local function HScrollKey(scrollControl)
	return tostring(scrollControl);
end

-- Ban icon strip is a Container (nested ScrollPanel broke vertical list clipping),
-- so it does not clip horizontally. Hide visuals that sit outside the view width.
local BAN_ICON_W = 64;
local BAN_ICON_PAD = 6;
local function ClipBanIconsToView(scrollControl, stackControl)
	local ctrls = stackControl ~= nil and g_BanIconControls[stackControl] or nil;
	if ctrls == nil or scrollControl == nil then
		return;
	end
	local viewW = scrollControl:GetSizeX() or 210;
	if viewW < 1 then viewW = 210; end
	local stackX = 0;
	pcall(function() stackX = stackControl:GetOffsetX() or 0; end);
	local x = stackX;
	for _, inst in ipairs(ctrls) do
		local left = x;
		local right = x + BAN_ICON_W;
		-- Fully inside only — partial icons would paint past the ban box edges.
		local visible = (left >= -0.5) and (right <= viewW + 0.5);
		-- Keep Root sized in the Stack; hide drawables only.
		if inst.BanFrame ~= nil then inst.BanFrame:SetHide(not visible); end
		if inst.BanLeaderIcon ~= nil then inst.BanLeaderIcon:SetHide(not visible); end
		if inst.BanCivFrame ~= nil then inst.BanCivFrame:SetHide(not visible); end
		if inst.BanCivIcon ~= nil then inst.BanCivIcon:SetHide(not visible); end
		if inst.BanButton ~= nil then inst.BanButton:SetHide(not visible); end
		x = x + BAN_ICON_W + BAN_ICON_PAD;
	end
end

-- Pixel-based horizontal scroll (Civ5 horizontal ScrollPanel SetScrollValue is unreliable).
-- alignLeft: draft icons under names stay left-aligned; ban boxes stay centered when they fit.
local function FinalizeHorizontalIconScroll(scrollControl, stackControl, leftBtn, rightBtn, alignLeft)
	if scrollControl == nil or stackControl == nil then
		return;
	end
	pcall(function()
		-- Always measure from offset 0. Leftover SetOffsetX after ban-count
		-- changes makes contentW/maxScroll wrong and sticks the left arrow on.
		stackControl:SetOffsetX(0);
		stackControl:CalculateSize();
		stackControl:ReprocessAnchoring();
		-- Ban icon rows use a Container (not ScrollPanel) so parent list clipping works.
		pcall(function() scrollControl:CalculateInternalSize(); end);

		local contentW = stackControl:GetSizeX() or 0;
		local viewW = scrollControl:GetSizeX() or 0;
		if viewW < 1 then
			viewW = 210;
		end
		local maxScroll = contentW - viewW;
		local key = HScrollKey(scrollControl);

		if maxScroll <= 2 then
			g_HScrollOffset[key] = 0;
			if alignLeft then
				stackControl:SetOffsetX(0);
			else
				local pad = math.floor((viewW - contentW) / 2);
				if pad < 0 then pad = 0; end
				stackControl:SetOffsetX(pad);
			end
			if leftBtn ~= nil then leftBtn:SetHide(true); end
			if rightBtn ~= nil then rightBtn:SetHide(true); end
			ClipBanIconsToView(scrollControl, stackControl);
			return;
		end

		local off = g_HScrollOffset[key] or 0;
		if off < 0 then off = 0; end
		if off > maxScroll then off = maxScroll; end
		g_HScrollOffset[key] = off;
		stackControl:SetOffsetX(-off);

		-- Only show an arrow if that direction still has content to reveal.
		if leftBtn ~= nil then leftBtn:SetHide(off <= 0); end
		if rightBtn ~= nil then rightBtn:SetHide(off >= maxScroll - 1); end
		ClipBanIconsToView(scrollControl, stackControl);
	end);
end

-- Wipe stored offsets + visible arrow state (used when bans-per-player changes).
local function ResetHorizontalBanScrolls()
	g_HScrollOffset = {};
	local function clearRow(scroll, stack, leftBtn, rightBtn)
		if stack ~= nil then
			pcall(function() stack:SetOffsetX(0); end);
		end
		if leftBtn ~= nil then leftBtn:SetHide(true); end
		if rightBtn ~= nil then rightBtn:SetHide(true); end
		if scroll ~= nil and stack ~= nil then
			FinalizeHorizontalIconScroll(scroll, stack, leftBtn, rightBtn, false);
		end
	end
	clearRow(Controls.BanHostIconScroll, Controls.BanHostIconStack, Controls.BanHostScrollLeft, Controls.BanHostScrollRight);
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		local banInst = g_BanSlotInstances[i];
		if banInst ~= nil then
			clearRow(banInst.BanIconScroll, banInst.BanIconStack, banInst.BanScrollLeft, banInst.BanScrollRight);
		end
	end
end

local function NudgeHorizontalIconScroll(scrollControl, stackControl, leftBtn, rightBtn, deltaPx, alignLeft)
	if scrollControl == nil or stackControl == nil then
		return;
	end
	local key = HScrollKey(scrollControl);
	local off = g_HScrollOffset[key] or 0;
	g_HScrollOffset[key] = off + (deltaPx or 0);
	FinalizeHorizontalIconScroll(scrollControl, stackControl, leftBtn, rightBtn, alignLeft);
end

local function WireHScrollButtons(leftBtn, rightBtn, scrollControl, stackControl, stepPx, alignLeft)
	stepPx = stepPx or 70;
	if leftBtn ~= nil then
		leftBtn:RegisterCallback(Mouse.eLClick, function()
			NudgeHorizontalIconScroll(scrollControl, stackControl, leftBtn, rightBtn, -stepPx, alignLeft);
		end);
	end
	if rightBtn ~= nil then
		rightBtn:RegisterCallback(Mouse.eLClick, function()
			NudgeHorizontalIconScroll(scrollControl, stackControl, leftBtn, rightBtn, stepPx, alignLeft);
		end);
	end
end

local function SetUnknownCivIcon(imageControl, size)
	size = size or 64;
	SimpleCivIconHookup(-1, size, imageControl);
end

local function SetCivIconByID(imageControl, civID, size)
	size = size or 64;
	if civID == nil or civID < 0 then
		SetUnknownCivIcon(imageControl, size);
		return;
	end
	local civ = GameInfo.Civilizations[civID];
	if civ == nil then
		SetUnknownCivIcon(imageControl, size);
		return;
	end
	IconHookup(civ.PortraitIndex, size, civ.IconAtlas, imageControl);
end

local function GetLeaderForCivID(civID)
	if civID == nil or civID < 0 then
		return nil, nil;
	end
	local civ = GameInfo.Civilizations[civID];
	if civ == nil then
		return nil, nil;
	end
	local link = GameInfo.Civilization_Leaders("CivilizationType = '" .. civ.Type .. "'")();
	if link == nil then
		return civ, nil;
	end
	return civ, GameInfo.Leaders[link.LeaderheadType];
end

-- UA + UU/UB/UI tooltips (trait-style: colored name + full Help/Description text).
function Draft_GetCivBonusTooltip(civID)
	local civ, leader = GetLeaderForCivID(civID);
	if civ == nil then
		return "";
	end

	local parts = {};

	if leader ~= nil then
		local ok, traitRow = pcall(function()
			return GameInfo.Leader_Traits("LeaderType ='" .. leader.Type .. "'")();
		end);
		if ok and traitRow ~= nil then
			local trait = GameInfo.Traits[traitRow.TraitType];
			if trait ~= nil then
				local traitShort = Locale.ConvertTextKey(trait.ShortDescription);
				local traitDesc = Locale.ConvertTextKey(trait.Description);
				table.insert(parts, string.format("[COLOR_POSITIVE_TEXT]%s:[ENDCOLOR] %s", traitShort, traitDesc));
			end
		end
	end

	local function addUniqueLine(nameKey, helpKey)
		if nameKey == nil then
			return;
		end
		local name = Locale.Lookup(nameKey);
		local help = nil;
		if helpKey ~= nil then
			help = Locale.ConvertTextKey(helpKey);
			-- ConvertTextKey returns the key itself when missing — treat as empty.
			if help == nil or help == "" or help == helpKey then
				help = Locale.Lookup(helpKey);
				if help == helpKey then
					help = nil;
				end
			end
		end
		if help ~= nil and help ~= "" then
			table.insert(parts, string.format("[COLOR_POSITIVE_TEXT]%s:[ENDCOLOR] %s", name, help));
		else
			table.insert(parts, string.format("[COLOR_POSITIVE_TEXT]%s[ENDCOLOR]", name));
		end
	end

	local function addFromRows(query, args)
		for row in DB.Query(query, args) do
			addUniqueLine(row.Description, row.Help);
		end
	end

	local gotUniques = false;
	local okQuery = pcall(function()
		local before = #parts;
		addFromRows([[SELECT Description, Help FROM Units INNER JOIN
			Civilization_UnitClassOverrides ON Units.Type = Civilization_UnitClassOverrides.UnitType
			WHERE Civilization_UnitClassOverrides.CivilizationType = ? AND
			Civilization_UnitClassOverrides.UnitType IS NOT NULL]], civ.Type);
		addFromRows([[SELECT Description, Help FROM Buildings INNER JOIN
			Civilization_BuildingClassOverrides ON Buildings.Type = Civilization_BuildingClassOverrides.BuildingType
			WHERE Civilization_BuildingClassOverrides.CivilizationType = ? AND
			Civilization_BuildingClassOverrides.BuildingType IS NOT NULL]], civ.Type);
		addFromRows([[SELECT Description, Help FROM Improvements WHERE CivilizationType = ?]], civ.Type);
		gotUniques = (#parts > before);
	end);

	if not okQuery or not gotUniques then
		for row in GameInfo.Civilization_UnitClassOverrides() do
			if row.CivilizationType == civ.Type and row.UnitType ~= nil then
				local unit = GameInfo.Units[row.UnitType];
				if unit ~= nil then addUniqueLine(unit.Description, unit.Help); end
			end
		end
		for row in GameInfo.Civilization_BuildingClassOverrides() do
			if row.CivilizationType == civ.Type and row.BuildingType ~= nil then
				local building = GameInfo.Buildings[row.BuildingType];
				if building ~= nil then addUniqueLine(building.Description, building.Help); end
			end
		end
		for row in GameInfo.Improvements() do
			if row.CivilizationType == civ.Type then
				addUniqueLine(row.Description, row.Help);
			end
		end
	end

	if #parts > 0 then
		return table.concat(parts, "[NEWLINE][NEWLINE]");
	end
	if leader ~= nil then
		return Locale.Lookup(leader.Description) .. " (" .. Locale.Lookup(civ.ShortDescription) .. ")";
	end
	return Locale.Lookup(civ.Description);
end

function Draft_OpenCivilopedia(searchString)
	-- FrontEnd Other/Civilopedia is often never loaded in MP lobby, so Events alone
	-- does nothing. Prefer the StagingRoom-embedded instance; fall back to FrontEnd.
	local pedia = Controls.Civilopedia;
	if pedia == nil then
		pcall(function()
			pedia = LookUpControl("/FrontEnd/MainMenu/Other/Civilopedia");
		end);
	end
	if pedia == nil then
		return;
	end

	-- Tell StagingRoom ShowHideHandler to skip Draft_Init / ActivateDLC on return.
	g_SkipStagingFullRefresh = true;

	searchString = searchString or "";
	UIManager:QueuePopup(pedia, PopupPriority.eUtmost);

	if searchString ~= "" then
		-- Navigate to article (each loaded CivilopediaScreen listens).
		Events.SearchForPediaEntry(searchString);
		-- If FrontEnd also opened, close it so we don't stack two pedias.
		pcall(function()
			local fe = LookUpControl("/FrontEnd/MainMenu/Other/Civilopedia");
			if fe ~= nil and Controls.Civilopedia ~= nil and fe ~= Controls.Civilopedia and not fe:IsHidden() then
				UIManager:DequeuePopup(fe);
			end
		end);
	end
end

function Draft_CloseCivilopedia()
	local closed = false;
	local function tryClose(ctrl)
		if ctrl ~= nil and not ctrl:IsHidden() then
			UIManager:DequeuePopup(ctrl);
			return true;
		end
		return false;
	end
	-- Prefer embedded lobby instance first.
	closed = tryClose(Controls.Civilopedia);
	if not closed then
		pcall(function()
			closed = tryClose(LookUpControl("/FrontEnd/MainMenu/Other/Civilopedia"));
		end);
	end
	return closed;
end

function Draft_IsCivilopediaOpen()
	if Controls.Civilopedia ~= nil and not Controls.Civilopedia:IsHidden() then
		return true;
	end
	local open = false;
	pcall(function()
		local pedia = LookUpControl("/FrontEnd/MainMenu/Other/Civilopedia");
		if pedia ~= nil and not pedia:IsHidden() then
			open = true;
		end
	end);
	return open;
end

function Draft_OpenCivPediaByCivID(civID)
	local civ, leader = GetLeaderForCivID(civID);
	if leader ~= nil then
		Draft_OpenCivilopedia(Locale.Lookup(leader.Description));
	elseif civ ~= nil then
		Draft_OpenCivilopedia(Locale.Lookup(civ.ShortDescription));
	else
		Draft_OpenCivilopedia("");
	end
end

function Draft_ApplyLeaderPortraitHelp(portraitButton, civID, hoverAnim)
	if portraitButton == nil then
		return;
	end
	if civID ~= nil and civID >= 0 then
		local tip = Draft_GetCivBonusTooltip(civID);
		if tip ~= "" then
			tip = tip .. "[NEWLINE][NEWLINE]Right-click: Civilopedia";
		else
			tip = "Right-click: Civilopedia";
		end
		portraitButton:SetToolTipString(tip);
		portraitButton:SetVoid1(civID);
		portraitButton:SetDisabled(false);
		portraitButton:SetHide(false);
		-- SetDisabled does not suppress ShowOnMouseOver; hide the glow anim instead.
		if hoverAnim ~= nil then hoverAnim:SetHide(false); end
	else
		-- Random / no civ: no hover, tooltip, or pedia.
		portraitButton:SetToolTipString("");
		portraitButton:SetVoid1(-1);
		portraitButton:SetDisabled(true);
		if hoverAnim ~= nil then hoverAnim:SetHide(true); end
	end
end

function Draft_WireLeaderPortraitButton(portraitButton)
	if portraitButton == nil then
		return;
	end
	portraitButton:RegisterCallback(Mouse.eRClick, function()
		local civID = portraitButton:GetVoid1();
		if civID ~= nil and civID >= 0 then
			Draft_OpenCivPediaByCivID(civID);
		end
	end);
end

-- Leader portrait + corner civ symbol (empty slot = random / question).
local function SetBanLeaderCivIcons(inst, civID)
	if inst == nil then
		return;
	end
	local civ, leader = GetLeaderForCivID(civID);
	if leader ~= nil then
		IconHookup(leader.PortraitIndex, 64, leader.IconAtlas, inst.BanLeaderIcon);
	else
		IconHookup(22, 64, "LEADER_ATLAS", inst.BanLeaderIcon);
	end
	if civ ~= nil then
		IconHookup(civ.PortraitIndex, 32, civ.IconAtlas, inst.BanCivIcon);
		if inst.BanCivFrame ~= nil then
			inst.BanCivFrame:SetHide(false);
		end
	else
		SimpleCivIconHookup(-1, 32, inst.BanCivIcon);
		if inst.BanCivFrame ~= nil then
			inst.BanCivFrame:SetHide(false);
		end
	end
end

function Draft_HideBanPicker()
	if Controls.BanPickerScroll ~= nil then
		pcall(function()
			g_BanPickerScrollPos = Controls.BanPickerScroll:GetScrollValue() or 0;
		end);
	end
	if Controls.BanPickerDismiss ~= nil then
		Controls.BanPickerDismiss:SetHide(true);
	end
	if Controls.BanPickerPanel ~= nil then
		Controls.BanPickerPanel:SetHide(true);
		pcall(function()
			Controls.BanPickerPanel:ChangeParent(Controls.MainGrid);
			Controls.BanPickerPanel:SetOffsetVal(640, 200);
		end);
	end
	-- Re-enable ban ready when picker closes (unless draft/game-ready locked).
	if Controls.BanHostReadyCheck ~= nil
		and not g_DraftLocked
		and not IsDraftLockedByGameReady()
		and not Draft_IsHistoryOnly() then
		Controls.BanHostReadyCheck:SetDisabled(false);
	end
end

-- ESC / click-elsewhere dismiss (wired from StagingRoom InputHandler + BanPickerDismiss).
function Draft_HandleInput(uiMsg, wParam, lParam)
	if uiMsg == KeyEvents.KeyDown and wParam == Keys.VK_ESCAPE then
		if Draft_CloseCivilopedia ~= nil and Draft_CloseCivilopedia() then
			return true;
		end
		if Controls.BanPickerPanel ~= nil and not Controls.BanPickerPanel:IsHidden() then
			g_PendingBan = nil;
			Draft_HideBanPicker();
			Draft_RefreshBanUI();
			return true;
		end
		return false;
	end
	return false;
end

function Draft_OnBanPickerDismiss()
	g_PendingBan = nil;
	Draft_HideBanPicker();
	Draft_RefreshBanUI();
end

local function Draft_PlaceBanPicker(playerID, banRoot)
	if Controls.BanPickerPanel == nil then
		return;
	end
	-- Dismiss catcher behind the picker (same click-elsewhere behavior as civ PullDown).
	if Controls.BanPickerDismiss ~= nil then
		Controls.BanPickerDismiss:ChangeParent(Controls.MainGrid);
		Controls.BanPickerDismiss:SetHide(false);
	end
	-- Always parent to MainGrid so the picker draws above slots/scroll clipping.
	Controls.BanPickerPanel:ChangeParent(Controls.MainGrid);

	local x = 635;
	local y = 150;
	local localID = Matchmaking.GetLocalID();
	if playerID == localID then
		y = 150; -- just below BanHost
	elseif m_SlotInstances ~= nil then
		local row = 0;
		local found = false;
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local slot = m_SlotInstances[i];
			if slot ~= nil and slot.Root ~= nil and not slot.Root:IsHidden() then
				if slot.playerID == localID then
					-- local sits in Host row, not in listing
				elseif slot.playerID == playerID then
					found = true;
					break;
				else
					row = row + 1;
				end
			end
		end
		if found then
			local scrollPx = 0;
			pcall(function()
				-- Ban column mirrors the player list scroll (no ban ScrollPanel).
				local sv = Controls.ListingScrollPanel:GetScrollValue() or 0;
				local contentH = Controls.SlotStack:GetSizeY() or 0;
				local viewH = Controls.ListingScrollPanel:GetSizeY() or 0;
				scrollPx = sv * math.max(0, contentH - viewH);
			end);
			y = 201 + (row * SLOT_ROW_HEIGHT) - scrollPx + 70;
		end
	end

	local _, screenY = UIManager:GetScreenSizeVal();
	if y + 280 > screenY - 60 then
		y = math.max(60, screenY - 340);
	end
	if y < 60 then
		y = 60;
	end

	Controls.BanPickerPanel:SetOffsetVal(x, y);
	Controls.BanPickerPanel:SetHide(false);
end

function Draft_ApplyBanSelection(civID)
	if g_PendingBan == nil then
		return;
	end
	local playerID = g_PendingBan.playerID;
	local slotIndex = g_PendingBan.slotIndex;
	g_PendingBan = nil;
	Draft_HideBanPicker();
	if g_DraftLocked or not CanEditBansForPlayer(playerID) then
		return;
	end
	EnsureBanArray(playerID);
	civID = civID or -1;

	-- Reject civs already banned by anyone (including your other slots).
	if civID >= 0 then
		local taken = Draft_GetTakenBans(nil); -- all players
		-- Allow keeping the same civ if THIS slot already has it
		local current = g_DraftBans[playerID][slotIndex];
		if taken[civID] ~= nil and current ~= civID then
			-- Conflict while picker was open: reset this slot
			g_DraftBans[playerID][slotIndex] = -1;
			Draft_BroadcastBans(playerID);
			Draft_RefreshBanUI();
			Draft_PersistToPreGame();
			return;
		end
	end

	g_DraftBans[playerID][slotIndex] = civID;
	Draft_BroadcastBans(playerID);
	Draft_RefreshBanUI();
	Draft_PersistToPreGame();
end

-- Returns map civID -> playerID for bans already taken.
-- excludePlayerID: ignore that player's bans (nil = include everyone).
-- excludeSlot: optional {playerID, slotIndex} to ignore one of your own slots.
function Draft_GetTakenBans(excludePlayerID, excludeSlot)
	local taken = {};
	for pid, list in pairs(g_DraftBans) do
		if excludePlayerID == nil or pid ~= excludePlayerID then
			for idx, civID in ipairs(list) do
				if civID ~= nil and civID >= 0 then
					if excludeSlot == nil
						or excludeSlot.playerID ~= pid
						or excludeSlot.slotIndex ~= idx then
						taken[civID] = pid;
					end
				end
			end
		end
	end
	return taken;
end

function Draft_PopulateBanPicker()
	if Controls.BanPickerStack == nil then
		return;
	end
	if g_BanPickerIM == nil then
		g_BanPickerIM = InstanceManager:new("BanPickerEntry", "Button", Controls.BanPickerStack);
	else
		g_BanPickerIM:ResetInstances();
	end

	local excludeSlot = g_PendingBan;
	-- Exclude bans from everyone except the slot we're currently editing
	local taken = {};
	for pid, list in pairs(g_DraftBans) do
		for idx, civID in ipairs(list) do
			if civID ~= nil and civID >= 0 then
				local isCurrentSlot = excludeSlot ~= nil
					and excludeSlot.playerID == pid
					and excludeSlot.slotIndex == idx;
				if not isCurrentSlot then
					taken[civID] = pid;
				end
			end
		end
	end

	local entries = {};
	for row in DB.Query([[SELECT Civilizations.ID as CivID,
		Civilizations.ShortDescription as CivShortDescription,
		Civilizations.Description as CivDescription,
		Leaders.Description as LeaderDescription
		FROM Civilizations, Leaders, Civilization_Leaders
		WHERE Civilizations.Playable = 1
		AND Civilizations.Type = Civilization_Leaders.CivilizationType
		AND Leaders.Type = Civilization_Leaders.LeaderheadType]]) do
		if taken[row.CivID] == nil then
			local title = Locale.Lookup("TXT_KEY_RANDOM_LEADER_CIV", Locale.Lookup(row.LeaderDescription), Locale.Lookup(row.CivShortDescription));
			table.insert(entries, { ID = row.CivID, Title = title, Description = Locale.Lookup(row.CivDescription) });
		end
	end
	table.sort(entries, function(a, b) return a.Title < b.Title; end);

	for _, v in ipairs(entries) do
		local inst = g_BanPickerIM:GetInstance();
		SetCivIconByID(inst.Icon, v.ID, 32);
		inst.Label:SetText(v.Title);
		inst.Button:SetToolTipString(v.Description);
		local civID = v.ID;
		inst.Button:RegisterCallback(Mouse.eLClick, function()
			Draft_ApplyBanSelection(civID);
		end);
	end
	Controls.BanPickerStack:CalculateSize();
	Controls.BanPickerStack:ReprocessAnchoring();
	if Controls.BanPickerScroll ~= nil then
		Controls.BanPickerScroll:CalculateInternalSize();
		-- Keep prior scroll position (do not jump back to the top after each pick).
		local pos = g_BanPickerScrollPos or 0;
		if pos < 0 then pos = 0; end
		if pos > 1 then pos = 1; end
		pcall(function() Controls.BanPickerScroll:SetScrollValue(pos); end);
	end
end

local function RefreshBanIconsForPlayer(playerID, stackControl)
	if stackControl == nil then
		return;
	end
	EnsureBanArray(playerID);
	-- Key IMs by stack control (not playerID) so BanHost / BanSlot never collide and
	-- lobby recreate + ban-count changes don't leave orphan icons.
	local im = g_BanIconIMs[stackControl];
	if im == nil then
		im = InstanceManager:new("BanIconInstance", "Root", stackControl);
		g_BanIconIMs[stackControl] = im;
	else
		im:ResetInstances();
	end
	g_BanIconControls[stackControl] = {};
	local bans = g_DraftBans[playerID];
	local canEdit = CanEditBansForPlayer(playerID);
	for i = 1, #bans do
		local inst = im:GetInstance();
		local civID = bans[i];
		SetBanLeaderCivIcons(inst, civID);
		local tip = "";
		if civID ~= nil and civID >= 0 then
			tip = Draft_GetCivBonusTooltip(civID);
			if tip ~= "" then
				tip = tip .. "[NEWLINE][NEWLINE]Right-click: Civilopedia";
			else
				tip = "Banned civilization[NEWLINE]Right-click: Civilopedia";
			end
			if canEdit then
				tip = tip .. "[NEWLINE]Left-click: change ban";
			end
		else
			tip = canEdit and "Click to ban a civilization" or "No ban selected";
		end
		inst.BanButton:SetToolTipString(tip);
		-- Glow is ShowOnMouseOver (SetDisabled does not suppress it). Only editable bans glow.
		local allowHover = canEdit;
		inst.BanButton:SetDisabled(not allowHover);
		if inst.BanHoverAnim ~= nil then
			inst.BanHoverAnim:SetHide(not allowHover);
		end
		local slotIndex = i;
		local banRoot = inst.Root;
		local bannedCiv = civID;
		inst.BanButton:RegisterCallback(Mouse.eLClick, function()
			if CanEditBansForPlayer(playerID) then
				Draft_OnBanIconClicked(playerID, slotIndex, banRoot);
			end
		end);
		inst.BanButton:RegisterCallback(Mouse.eRClick, function()
			if bannedCiv ~= nil and bannedCiv >= 0 then
				Draft_OpenCivPediaByCivID(bannedCiv);
			end
		end);
		table.insert(g_BanIconControls[stackControl], inst);
	end
	stackControl:CalculateSize();
	stackControl:ReprocessAnchoring();
end

function Draft_OnBanIconClicked(playerID, slotIndex, banRoot)
	if not CanEditBansForPlayer(playerID) then
		return;
	end
	-- Toggle closed if clicking the same slot again
	if g_PendingBan ~= nil
		and g_PendingBan.playerID == playerID
		and g_PendingBan.slotIndex == slotIndex
		and Controls.BanPickerPanel ~= nil
		and not Controls.BanPickerPanel:IsHidden() then
		g_PendingBan = nil;
		Draft_HideBanPicker();
		Draft_RefreshBanUI();
		return;
	end
	g_PendingBan = { playerID = playerID, slotIndex = slotIndex };
	Draft_PopulateBanPicker();
	Draft_PlaceBanPicker(playerID, banRoot);
	if Controls.BanHostReadyCheck ~= nil and playerID == Matchmaking.GetLocalID() then
		Controls.BanHostReadyCheck:SetDisabled(true);
	end
end

function Draft_OnBanCivSelected(civID, _)
	Draft_ApplyBanSelection(civID);
end

----------------------------------------------------------------
-- Draft icons under player names
----------------------------------------------------------------
function Draft_OnDraftCivIconClicked(playerID, civID)
	if not CanSelectDraftCivForPlayer(playerID) then
		return;
	end
	if civID == nil or civID < 0 then
		return;
	end
	if not Draft_CivAllowedForPlayer(playerID, civID) then
		return;
	end
	if PreGame.GetCivilization(playerID) == civID then
		return;
	end
	PreGame.SetCivilization(playerID, civID);
	pcall(function()
		PreGame.SetLeaderName(playerID, "");
		PreGame.SetCivilizationDescription(playerID, "");
		PreGame.SetCivilizationShortDescription(playerID, "");
		PreGame.SetCivilizationAdjective(playerID, "");
	end);
	Network.BroadcastPlayerInfo();
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	else
		Draft_RefreshDraftIconsAll();
	end
end

local function RefreshDraftIcons(playerID, stackControl)
	if stackControl == nil then
		return;
	end
	local im = g_DraftIconIMs[stackControl];
	if im == nil then
		im = InstanceManager:new("DraftCivIcon", "Root", stackControl);
		g_DraftIconIMs[stackControl] = im;
	else
		im:ResetInstances();
	end
	local pool = playerID ~= nil and g_DraftPools[playerID] or nil;
	if not g_DraftLocked or pool == nil or #pool == 0 then
		stackControl:CalculateSize();
		return;
	end
	local canPick = CanSelectDraftCivForPlayer(playerID);
	for _, civID in ipairs(pool) do
		local inst = im:GetInstance();
		SetCivIconByID(inst.Icon, civID, 32);
		local tip = "";
		local civ = GameInfo.Civilizations[civID];
		if civ ~= nil then
			tip = Locale.Lookup(civ.Description);
		end
		if canPick then
			if tip ~= "" then
				tip = tip .. "[NEWLINE]Click to play as this civilization";
			else
				tip = "Click to play as this civilization";
			end
		end
		local btn = inst.Button or inst.Root;
		if btn ~= nil then
			btn:SetToolTipString(tip);
			btn:SetDisabled(not canPick);
			local pid, cid = playerID, civID;
			btn:RegisterCallback(Mouse.eLClick, function()
				Draft_OnDraftCivIconClicked(pid, cid);
			end);
		elseif inst.Root ~= nil then
			inst.Root:SetToolTipString(tip);
		end
	end
	stackControl:CalculateSize();
	stackControl:ReprocessAnchoring();
end

----------------------------------------------------------------
-- Ban column slot instances
----------------------------------------------------------------
function Draft_CreateBanSlots()
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		if g_BanSlotInstances[i] == nil and Controls.BanSlotStack ~= nil then
			local instance = {};
			ContextPtr:BuildInstanceForControl("BanSlot", instance, Controls.BanSlotStack);
			instance.Root:SetHide(true);
			if instance.DraftSwapButton ~= nil then
				instance.DraftSwapButton:RegisterCallback(Mouse.eLClick, function()
					local pid = instance.DraftSwapButton:GetVoid1();
					Draft_OnBanSwapClick(pid);
				end);
			end
			WireHScrollButtons(instance.BanScrollLeft, instance.BanScrollRight, instance.BanIconScroll, instance.BanIconStack, 70, false);
			g_BanSlotInstances[i] = instance;
		end
	end
	Draft_HideBanPicker();
end

function Draft_RefreshBanUI()
	-- Never resurface ban UI while on Draft Rules / Game Options.
	if g_DraftPageMode ~= nil and g_DraftPageMode ~= "players" then
		if Controls.BanHost ~= nil then Controls.BanHost:SetHide(true); end
		Draft_HideBanPicker();
		Draft_UpdateActionButtons();
		return;
	end

	local localID = Matchmaking.GetLocalID();
	local isHost = Matchmaking.IsHost();
	local showDraftSwap = g_DraftLocked and not PreGame.IsHotSeatGame() and not Draft_IsHistoryOnly();
	local swapTip = "Click to request a draft-pool swap with this player (both must click)";
	local gameReadyLock = IsDraftLockedByGameReady();
	local pickerOpen = g_PendingBan ~= nil
		and Controls.BanPickerPanel ~= nil
		and not Controls.BanPickerPanel:IsHidden();
	-- Close ban picker when anyone is ready-to-start (bans are frozen).
	if gameReadyLock and pickerOpen then
		g_PendingBan = nil;
		Draft_HideBanPicker();
		pickerOpen = false;
	end

	-- Local row (fixed at top, mirrors Host player row)
	if Controls.BanHost ~= nil then
		Controls.BanHost:SetHide(false);
		if Controls.BanHostPlayerLabel ~= nil then
			Controls.BanHostPlayerLabel:SetText("Your Bans");
		end
		EnsureBanArray(localID);
		RefreshBanIconsForPlayer(localID, Controls.BanHostIconStack);
		-- Only rewind horizontal ban scroll when the number of ban slots changes.
		local banCount = #(g_DraftBans[localID] or {});
		local hostKey = HScrollKey(Controls.BanHostIconScroll);
		if g_BanHostIconCount ~= banCount then
			g_HScrollOffset[hostKey] = 0;
			g_BanHostIconCount = banCount;
		end
		FinalizeHorizontalIconScroll(Controls.BanHostIconScroll, Controls.BanHostIconStack, Controls.BanHostScrollLeft, Controls.BanHostScrollRight, false);
		if Controls.BanHostReadyCheck ~= nil then
			Controls.BanHostReadyCheck:SetCheck(g_DraftBanReady[localID] == true);
			Controls.BanHostReadyCheck:SetDisabled(g_DraftLocked or pickerOpen or Draft_IsHistoryOnly() or gameReadyLock);
			Controls.BanHostReadyCheck:SetHide(Draft_IsHistoryOnly());
		end
		if Controls.BanHostReadyHighlight ~= nil then
			Controls.BanHostReadyHighlight:SetHide(g_DraftBanReady[localID] ~= true);
		end
		if Controls.BanHostStatusLabel ~= nil then
			if g_DraftLocked then
				Controls.BanHostStatusLabel:SetText("Draft locked");
			elseif g_DraftBanHostControl[localID] then
				Controls.BanHostStatusLabel:SetText("Host controls your bans");
			elseif g_DraftBanReady[localID] then
				Controls.BanHostStatusLabel:SetText("Bans ready");
			else
				Controls.BanHostStatusLabel:SetText("Select bans, then ready");
			end
		end
		-- Pre-draft: non-host can cede ban control to host (same corner as swap).
		-- Post-draft: swap button (own row stays display-only / disabled).
		local showDelegate = (not isHost) and (not g_DraftLocked) and (not Draft_IsHistoryOnly())
			and (not gameReadyLock) and (not PreGame.IsHotSeatGame());
		if Controls.BanHostDelegateButton ~= nil then
			Controls.BanHostDelegateButton:SetHide(not showDelegate);
			Controls.BanHostDelegateButton:SetDisabled(not showDelegate);
			local ceded = g_DraftBanHostControl[localID] == true;
			Controls.BanHostDelegateButton:SetToolTipString(
				ceded and "Reclaim ban control from the host"
				or "Let the host edit your bans (toggle)");
			if Controls.BanHostDelegateButtonHighAlpha ~= nil then
				Controls.BanHostDelegateButtonHighAlpha:SetHide(not (showDelegate and ceded));
			end
		end
		if Controls.BanHostDraftSwapButton ~= nil then
			local someoneWantsUs = false;
			for pid, target in pairs(g_DraftSwapDesire) do
				if target == localID then
					someoneWantsUs = true;
					break;
				end
			end
			Controls.BanHostDraftSwapButton:SetHide(not showDraftSwap);
			Controls.BanHostDraftSwapButton:SetDisabled(true);
			Controls.BanHostDraftSwapButton:SetToolTipString(
				someoneWantsUs and "Another player requested a draft swap with you — click their swap button to accept"
				or "Use another player's swap button to request a draft-pool swap");
			if Controls.BanHostDraftSwapButtonHighAlpha ~= nil then
				Controls.BanHostDraftSwapButtonHighAlpha:SetHide(not (showDraftSwap and someoneWantsUs));
			end
		end
	end

	-- Collapse every ban row first. Hidden rows must not keep 107px height or they
	-- leave a blank gap under BanHost while SlotStack collapses the local slot.
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		local banInst = g_BanSlotInstances[i];
		if banInst ~= nil and banInst.Root ~= nil then
			banInst.Root:SetHide(true);
			banInst.Root:SetSizeVal(0, 0);
			-- Clear icons on collapsed rows so lobby recreate can't leave orphans.
			if banInst.BanIconStack ~= nil and g_BanIconIMs[banInst.BanIconStack] ~= nil then
				pcall(function() g_BanIconIMs[banInst.BanIconStack]:ResetInstances(); end);
			end
		end
	end

	-- Align ban rows with visible SlotStack rows (same indices as RefreshPlayerList).
	-- Local player sits in BanHost; every other visible slot gets a ban box.
	if m_SlotInstances ~= nil then
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local banInst = g_BanSlotInstances[i];
			local slotInst = m_SlotInstances[i];
			local slotVisible = slotInst ~= nil and slotInst.Root ~= nil and not slotInst.Root:IsHidden();
			local playerID = slotInst and slotInst.playerID or nil;

			if banInst ~= nil and slotVisible and playerID ~= nil and playerID ~= localID then
				banInst.Root:SetHide(false);
				banInst.Root:SetSizeVal(310, SLOT_ROW_HEIGHT);

				local name = (m_PlayerNames and m_PlayerNames[playerID])
					or ("Player " .. tostring(playerID));
				if not IsHumanSlot(playerID) then
					local slotTypeStr = GetSlotTypeString and GetSlotTypeString(playerID) or nil;
					if slotTypeStr ~= nil then
						name = Locale.ConvertTextKey(slotTypeStr);
					end
				end
				banInst.BanPlayerLabel:SetText(name);
				-- Other players never show a ready checkbox (display-only for humans;
				-- host edits AI bans directly via icons).
				banInst.BanReadyCheck:SetCheck(false);
				banInst.BanReadyCheck:SetHide(true);
				banInst.BanReadyCheck:SetDisabled(true);

				EnsureBanArray(playerID);
				RefreshBanIconsForPlayer(playerID, banInst.BanIconStack);
				FinalizeHorizontalIconScroll(banInst.BanIconScroll, banInst.BanIconStack, banInst.BanScrollLeft, banInst.BanScrollRight, false);

				local participant = IsBanParticipantSlot(playerID);
				local hostID = Matchmaking.GetHostID();
				local showReadyHL = (g_DraftBanReady[playerID] == true)
					or (IsAISlot(playerID) and hostID ~= nil and g_DraftBanReady[hostID] == true);
				if banInst.BanReadyHighlight ~= nil then
					banInst.BanReadyHighlight:SetHide(not showReadyHL);
				end

				if participant then
					if g_DraftLocked then
						banInst.BanStatusLabel:SetText("Draft locked");
					elseif IsAISlot(playerID) then
						if showReadyHL then
							banInst.BanStatusLabel:SetText("AI bans ready");
						else
							banInst.BanStatusLabel:SetText(isHost and "AI bans (host)" or "AI bans");
						end
					elseif IsHumanRequiredSlot(playerID) then
						banInst.BanStatusLabel:SetText("Human required");
					elseif g_DraftBanHostControl[playerID] then
						banInst.BanStatusLabel:SetText(isHost and "You may edit their bans" or "Host controls bans");
					elseif g_DraftBanReady[playerID] then
						banInst.BanStatusLabel:SetText("Bans ready");
					else
						banInst.BanStatusLabel:SetText("Selecting bans...");
					end
					if banInst.DraftSwapButton ~= nil then
						-- Visible after draft; disabled when game-ready / history (same idea as own row).
						local showSwap = showDraftSwap and IsBanParticipantSlot(playerID);
						local canClickSwap = showSwap and not gameReadyLock;
						banInst.DraftSwapButton:SetHide(not showSwap);
						banInst.DraftSwapButton:SetDisabled(not canClickSwap);
						local tip = swapTip;
						if not canClickSwap and showSwap then
							tip = "Draft swap locked while a player is ready to start";
						elseif IsHumanRequiredSlot(playerID) then
							tip = "Swap drafted civ pools with this Human Required slot (instant, for testing)";
						elseif IsAISlot(playerID) then
							tip = "Swap drafted civ pools with this AI (instant)";
						end
						banInst.DraftSwapButton:SetToolTipString(tip);
						banInst.DraftSwapButton:SetVoid1(playerID);
						local highlightSwap = canClickSwap and not IsAISlot(playerID) and not IsHumanRequiredSlot(playerID) and (
							(g_DraftSwapDesire[localID] == playerID)
							or (g_DraftSwapDesire[playerID] == localID)
						);
						if banInst.DraftSwapButtonHighAlpha ~= nil then
							banInst.DraftSwapButtonHighAlpha:SetHide(not highlightSwap);
						end
					end
				else
					banInst.BanStatusLabel:SetText("-");
					if banInst.DraftSwapButton ~= nil then
						banInst.DraftSwapButton:SetHide(true);
						banInst.DraftSwapButton:SetDisabled(true);
						if banInst.DraftSwapButtonHighAlpha ~= nil then
							banInst.DraftSwapButtonHighAlpha:SetHide(true);
						end
					end
				end
			end
		end
	end

	if Controls.BanSlotStack ~= nil then
		Controls.BanSlotStack:CalculateSize();
		Controls.BanSlotStack:ReprocessAnchoring();
	end
	Draft_SyncBanScroll();
	Draft_UpdateActionButtons();
end

function Draft_SyncBanScroll()
	-- BanSlotStack is inside ListingScrollPanel (engine scrolls both columns).
	-- Keep Y at 0; X aligns with BanHost.
	if Controls.BanSlotStack ~= nil then
		pcall(function()
			Controls.BanSlotStack:SetOffsetY(0);
			Controls.BanSlotStack:SetOffsetX(618);
		end);
	end
end

function Draft_RefreshDraftIconsAll()
	local show = g_DraftLocked == true;

	-- Host and other slots: Name → Draft icons → Civ/leader in one stack.
	-- DraftIconRow is height 0 when hidden, so civ/leader sits on the SlotType row
	-- until a draft is showing (then it drops to the Handicap/difficulty row).
	local function ApplyDraftRow(row, scroll, stack, leftBtn, rightBtn, playerID, leftStack)
		if stack ~= nil and playerID ~= nil then
			RefreshDraftIcons(playerID, stack);
		end
		if show then
			if row ~= nil then
				row:SetHide(false);
				row:SetSizeVal(250, 27);
			end
			FinalizeHorizontalIconScroll(scroll, stack, leftBtn, rightBtn, true);
		else
			if row ~= nil then
				row:SetHide(true);
				row:SetSizeVal(0, 0);
			end
			if leftBtn ~= nil then leftBtn:SetHide(true); end
			if rightBtn ~= nil then rightBtn:SetHide(true); end
		end
		if leftStack ~= nil then
			pcall(function()
				leftStack:CalculateSize();
				leftStack:ReprocessAnchoring();
			end);
		end
	end

	ApplyDraftRow(Controls.DraftIconRow, Controls.DraftIconScroll, Controls.DraftIconStack,
		Controls.DraftScrollLeft, Controls.DraftScrollRight, Matchmaking.GetLocalID(), Controls.LeftInfoStack);

	if m_SlotInstances ~= nil then
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local slot = m_SlotInstances[i];
			if slot ~= nil then
				-- Humans show Team + SlotType + Handicap (3 rows). Start higher so handicap
				-- fits in the box. AI (no handicap) keep Team@36 / SlotType@67.
				-- Left stack stays at topY so Name is always the top row.
				local humanLayout = slot.playerID ~= nil and IsHumanSlot(slot.playerID);
				local topY = humanLayout and 20 or 36;
				if slot.RightInfoStack ~= nil then
					slot.RightInfoStack:SetOffsetVal(400, topY);
				end
				if slot.LeftInfoStack ~= nil then
					slot.LeftInfoStack:SetOffsetVal(128, topY);
				end
				ApplyDraftRow(slot.DraftIconRow, slot.DraftIconScroll, slot.DraftIconStack,
					slot.DraftScrollLeft, slot.DraftScrollRight, slot.playerID, slot.LeftInfoStack);
			end
		end
	end
end

----------------------------------------------------------------
-- Ready / create / swap / reset
----------------------------------------------------------------
function Draft_AllHumansBanReady()
	local order = GetHumanPlayerOrder();
	if #order == 0 then
		return false;
	end
	for _, pid in ipairs(order) do
		if g_DraftBanReady[pid] ~= true then
			return false;
		end
	end
	return true;
end

function Draft_UpdateActionButtons()
	local isHost = Matchmaking.IsHost();
	local hotseat = PreGame.IsHotSeatGame();
	local showPlayers = (g_DraftPageMode == nil or g_DraftPageMode == "players");
	local history = Draft_IsHistoryOnly();
	local gameReadyLock = IsDraftLockedByGameReady();
	if Controls.CreateDraftButton ~= nil then
		local canCreate = isHost and not g_DraftLocked and not history and not gameReadyLock and Draft_AllHumansBanReady();
		Controls.CreateDraftButton:SetDisabled(not canCreate);
		Controls.CreateDraftButton:SetHide(hotseat or not showPlayers or history);
	end
	if Controls.ClearDraftButton ~= nil then
		Controls.ClearDraftButton:SetDisabled(not isHost or history or gameReadyLock);
		Controls.ClearDraftButton:SetHide(hotseat or not showPlayers or history);
	end
	if Controls.RestoreDraftButton ~= nil then
		local snap = g_PreviousDraftSnapshot;
		local hasSnap = snap ~= nil and snap.pools ~= nil;
		local countOk = hasSnap and GetDraftParticipantCount() <= (tonumber(snap.participantCount) or 0);
		-- Useful after Clear / accidental wipe; host-only. Hide until a draft has been created once.
		local showRestore = isHost and hasSnap and not hotseat and showPlayers and not history;
		local canRestore = showRestore and countOk and not gameReadyLock;
		Controls.RestoreDraftButton:SetHide(not showRestore);
		Controls.RestoreDraftButton:SetDisabled(not canRestore);
		if showRestore and not countOk then
			Controls.RestoreDraftButton:SetToolTipString(
				"Cannot restore: more draft players than when that draft was made");
		else
			Controls.RestoreDraftButton:SetToolTipString(
				"Restore the last created draft (if it was cleared accidentally)");
		end
	end
	if Controls.DraftResetButton ~= nil then
		Controls.DraftResetButton:SetDisabled(not isHost or history or gameReadyLock);
		Controls.DraftResetButton:SetHide(history);
	end
	Draft_RefreshBottomButtonBar();
end

-- Re-measure Create Draft / Save / etc. strip after show/hide or resize.
function Draft_RefreshBottomButtonScroll()
	FinalizeHorizontalIconScroll(
		Controls.BottomButtonScroll,
		Controls.BottomButtonStack,
		Controls.BottomButtonScrollLeft,
		Controls.BottomButtonScrollRight,
		true
	);
end

-- Back stays left. When Launch / Game Starting is on the right, shrink the
-- remaining buttons into a horizontal scroll so they are not drawn under it.
function Draft_RefreshBottomButtonBar()
	if Controls.BottomButtonBar == nil then
		return;
	end

	local function setSize(ctrl, w, h)
		if ctrl == nil then return; end
		if w ~= nil and h ~= nil then
			ctrl:SetSizeVal(w, h);
		elseif w ~= nil then
			ctrl:SetSizeX(w);
		elseif h ~= nil then
			ctrl:SetSizeY(h);
		end
	end
	local function setOffset(ctrl, x, y)
		if ctrl == nil then return; end
		pcall(function() ctrl:SetOffsetVal(x, y); end);
	end

	local screenX = UIManager:GetScreenSizeVal();
	local compact = (screenX < LOBBY_COMPACT_MAX_SCREEN);
	local mainW = compact and LOBBY_COMPACT_W or LOBBY_WIDE_W;
	local barLeft = 20;
	local barRightInset = 20;
	local startBtnW = 260;
	local startBtnRight = 38;
	local startBtnGap = 8;
	local backW = 150;
	local backGap = 8;
	local arrowSlot = 36;

	local startVisible = false;
	if Controls.CountdownButton ~= nil and not Controls.CountdownButton:IsHidden() then
		startVisible = true;
	end
	if Controls.LaunchButton ~= nil and not Controls.LaunchButton:IsHidden() then
		startVisible = true;
	end

	local bottomBarW = math.max(360, mainW - barLeft - barRightInset);
	if startVisible then
		bottomBarW = math.max(200, mainW - barLeft - startBtnRight - startBtnW - startBtnGap);
	end
	setSize(Controls.BottomButtonBar, bottomBarW, 70);
	setOffset(Controls.BottomButtonBar, barLeft, 54);

	-- Keep Launch / countdown in the same bottom-right slot (not in the chat).
	setOffset(Controls.LaunchButton, startBtnRight, 54);
	setOffset(Controls.CountdownButton, startBtnRight, 54);

	local backVisible = Controls.BackButton ~= nil and not Controls.BackButton:IsHidden();
	local scrollOffsetX = 0;
	if backVisible then
		scrollOffsetX = backW + backGap;
	end
	local scrollRowW = math.max(120, bottomBarW - scrollOffsetX);
	if Controls.BottomButtonScrollRow ~= nil then
		setSize(Controls.BottomButtonScrollRow, scrollRowW, 45);
		setOffset(Controls.BottomButtonScrollRow, scrollOffsetX, 0);
	end
	if Controls.BottomButtonScroll ~= nil then
		setSize(Controls.BottomButtonScroll, math.max(80, scrollRowW - arrowSlot * 2), 40);
		setOffset(Controls.BottomButtonScroll, arrowSlot, 0);
	end
	pcall(function() Controls.BottomButtonBar:ReprocessAnchoring(); end);
	Draft_RefreshBottomButtonScroll();
end

function Draft_OnBanSwapClick(playerID)
	if Draft_IsHistoryOnly() or IsDraftLockedByGameReady() then
		return;
	end
	if not g_DraftLocked or PreGame.IsHotSeatGame() then
		return;
	end
	if not IsBanParticipantSlot(playerID) then
		return;
	end
	Draft_TrySelectSwapPlayer(playerID);
	Draft_RefreshBanUI();
end

-- Non-host only: toggle letting the host edit your bans (pre-draft).
function Draft_OnDelegateBanControl()
	if Matchmaking.IsHost() or PreGame.IsHotSeatGame() then
		return;
	end
	if g_DraftLocked or Draft_IsHistoryOnly() or IsDraftLockedByGameReady() then
		return;
	end
	local localID = Matchmaking.GetLocalID();
	local ceded = g_DraftBanHostControl[localID] ~= true;
	g_DraftBanHostControl[localID] = ceded;
	SendDraftChat("BANCTRL|" .. tostring(localID) .. "|" .. (ceded and "1" or "0"));
	Draft_PersistToPreGame();
	Draft_RefreshBanUI();
end

-- After a pool swap the previous civ pick may no longer be in-pool.
local function Draft_ClearSelectedCiv(playerID)
	if playerID == nil or playerID < 0 then
		return;
	end
	-- In MP only the local human (or host for AI) can actually change that slot's civ.
	local localID = Matchmaking.GetLocalID();
	local isAI = IsAISlot(playerID);
	if playerID ~= localID and not (Matchmaking.IsHost() and isAI) then
		return;
	end

	PreGame.SetCivilization(playerID, -1);
	pcall(function()
		PreGame.SetLeaderName(playerID, "");
		PreGame.SetCivilizationDescription(playerID, "");
		PreGame.SetCivilizationShortDescription(playerID, "");
		PreGame.SetCivilizationAdjective(playerID, "");
	end);
	-- Ready locks civ changes on some paths — drop ready so Random sticks.
	if playerID == localID and PreGame.IsReady(localID) then
		PreGame.SetReady(localID, false);
	end
	Network.BroadcastPlayerInfo();
end

-- Clear a player's civ pick unless it is still in their draft pool.
local function Draft_ValidateCivAgainstPool(playerID)
	if playerID == nil or playerID < 0 then
		return;
	end
	local civ = PreGame.GetCivilization(playerID);
	if civ == nil or civ < 0 then
		return;
	end
	local pool = g_DraftPools[playerID];
	if pool ~= nil then
		for _, id in ipairs(pool) do
			if id == civ then
				return; -- still valid
			end
		end
	end
	Draft_ClearSelectedCiv(playerID);
end

local function Draft_ValidateAllCivsAgainstPools()
	local max = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, max - 1 do
		if IsBanParticipantSlot(pid) then
			Draft_ValidateCivAgainstPool(pid);
		end
	end
end

local function Draft_ApplyPoolSwap(a, b, broadcast)
	if a == nil or b == nil or a == b then
		return;
	end
	local poolA = g_DraftPools[a] or {};
	local poolB = g_DraftPools[b] or {};
	g_DraftPools[a] = poolB;
	g_DraftPools[b] = poolA;
	g_DraftSwapDesire[a] = nil;
	g_DraftSwapDesire[b] = nil;
	g_DraftSwapFirst = nil;
	-- Keep pick only if it still exists in the pool you just received.
	Draft_ValidateCivAgainstPool(a);
	Draft_ValidateCivAgainstPool(b);
	Draft_RefreshDraftIconsAll();
	Draft_RefreshBanUI();
	Draft_UpdateActionButtons();
	PopulateCivPulldown(Controls.CivPulldown, 0);
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		local inst = m_SlotInstances[i];
		if inst ~= nil then
			PopulateCivPulldown(inst.CivPulldown, i);
		end
	end
	if broadcast then
		SendDraftChat(
			"SWAP|" .. tostring(a) .. "|" .. tostring(b)
			.. "|" .. LekmodDrafter.EncodeCivList(g_DraftPools[a])
			.. "|" .. LekmodDrafter.EncodeCivList(g_DraftPools[b])
		);
	end
	Draft_PersistToPreGame();
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	end
	-- Re-check after UI/network refresh in case a bounce restored an out-of-pool civ.
	Draft_ValidateCivAgainstPool(a);
	Draft_ValidateCivAgainstPool(b);
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	end
end

local function Draft_TryCompleteMutualSwap(a, b)
	if a == nil or b == nil then
		return false;
	end
	if g_DraftSwapDesire[a] == b and g_DraftSwapDesire[b] == a then
		Draft_ApplyPoolSwap(a, b, true);
		return true;
	end
	return false;
end

function Draft_OnLocalBanReady(bChecked)
	if Draft_IsHistoryOnly() then
		if Controls.BanHostReadyCheck ~= nil then
			Controls.BanHostReadyCheck:SetCheck(g_DraftBanReady[Matchmaking.GetLocalID()] == true);
		end
		return;
	end
	local localID = Matchmaking.GetLocalID();
	if g_DraftLocked or IsDraftLockedByGameReady() then
		if Controls.BanHostReadyCheck ~= nil then
			Controls.BanHostReadyCheck:SetCheck(g_DraftBanReady[localID] == true);
		end
		return;
	end
	-- Don't allow ban-ready while the ban picker is open.
	if g_PendingBan ~= nil
		and Controls.BanPickerPanel ~= nil
		and not Controls.BanPickerPanel:IsHidden() then
		if Controls.BanHostReadyCheck ~= nil then
			Controls.BanHostReadyCheck:SetCheck(g_DraftBanReady[localID] == true);
		end
		return;
	end
	g_DraftBanReady[localID] = bChecked and true or false;
	g_DraftBanReadyPendingLocal = bChecked and true or false;
	if bChecked then
		-- Close any open picker so ready state can't be bypassed mid-edit.
		g_PendingBan = nil;
		Draft_HideBanPicker();
	end
	-- Engine-synced per-player bit (same role as PreGame.SetReady for the player box).
	Draft_WriteLocalBanReadyOption(bChecked);
	-- One chat notify for immediate UI; no periodic reannounce (that caused lobby lag).
	SendDraftChat("BANREADY|" .. tostring(localID) .. "|" .. (bChecked and "1" or "0"));
	if Matchmaking.IsHost() then
		Draft_BroadcastReadyMask();
	end
	Draft_RefreshBanUI();
	Draft_PersistToPreGame();
end

function Draft_BroadcastBans(playerID)
	EnsureBanArray(playerID);
	local encoded = LekmodDrafter.EncodeCivList(g_DraftBans[playerID]);
	SendDraftChat("BAN|" .. tostring(playerID) .. "|" .. encoded);
end

-- Same path as MPGameOptions.SendGameOptionChanged: write PreGame then broadcast.
-- Chat RULES is kept as a thin backup for clients that miss a settings packet.
function Draft_BroadcastRules()
	if not Matchmaking.IsHost() then
		return;
	end
	if PreGame ~= nil and PreGame.SetGameOption ~= nil then
		Draft_OptSet(DRAFT_OPT_VER, DRAFT_SAVE_VERSION);
		Draft_OptSet(DRAFT_OPT_RULES, Draft_PackRules());
	end
	if Network ~= nil and Network.BroadcastGameSettings ~= nil then
		Network.BroadcastGameSettings();
	end
	local r = g_DraftRules;
	local body = string.format(
		"RULES|%d|%d|%d|%d|%d|%d",
		tonumber(r.bansPerPlayer) or 2,
		tonumber(r.picksPerPlayer) or 3,
		tonumber(r.guaranteedCoastals) or -1,
		tonumber(r.guaranteedInlands) or -1,
		r.vanillaOnly and 1 or 0,
		r.seasonalBans and 1 or 0
	);
	SendDraftChat(body);
end

-- Clients apply host draft rules from PreGame (like reading other game options).
-- Returns true if local rules changed.
function Draft_PullRulesFromPreGame()
	if PreGame == nil or PreGame.GetGameOption == nil then
		return false;
	end
	-- Host is the writer; never pull over in-progress local edits.
	if Matchmaking.IsHost() then
		return false;
	end
	if Draft_OptGet(DRAFT_OPT_VER) < 1 then
		return false;
	end
	local prevPacked = Draft_PackRules();
	local packed = Draft_OptGet(DRAFT_OPT_RULES);
	if packed == prevPacked then
		return false;
	end

	Draft_UnpackRules(packed);
	-- Host cleared ban-ready with the rules write; drop local ready UI too.
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	Draft_WriteLocalBanReadyOption(false);
	for _, pid in ipairs(GetDraftPlayerOrder()) do
		EnsureBanArray(pid);
	end
	EnsureBanArray(Matchmaking.GetLocalID());
	Draft_PopulateRulesUI();
	ResetHorizontalBanScrolls();
	Draft_RefreshBanUI();
	Draft_UpdateActionButtons();
	return true;
end

function Draft_BroadcastPools()
	if not Matchmaking.IsHost() then
		return;
	end
	-- DRAFT|pid|id,id,id  (one message per player to stay under 255)
	for pid, pool in pairs(g_DraftPools) do
		SendDraftChat("DRAFT|" .. tostring(pid) .. "|" .. LekmodDrafter.EncodeCivList(pool));
	end
	SendDraftChat("LOCK|1");
end

function Draft_OnCreateDraft()
	if Draft_IsHistoryOnly() or IsDraftLockedByGameReady() then
		return;
	end
	if not Matchmaking.IsHost() or g_DraftLocked then
		return;
	end
	if not Draft_AllHumansBanReady() then
		AnnounceGame("Cannot create draft: not all players have readied bans.");
		return;
	end
	local order = GetDraftPlayerOrder();
	-- Ensure empty ban arrays exist for human-required test slots.
	for _, pid in ipairs(order) do
		EnsureBanArray(pid);
	end
	local result = LekmodDrafter.CreateDraft(g_DraftRules, order, g_DraftBans);
	if not result.ok then
		AnnounceGame("Draft failed: " .. tostring(result.error));
		return;
	end
	g_DraftPools = result.drafts or {};
	g_DraftLocked = true;
	g_DraftSwapFirst = nil;
	g_DraftBanHostControl = {}; -- delegate button gone; bans are locked anyway
	g_DraftParticipantKey = GetDraftParticipantKey();
	g_DraftParticipantCount = GetDraftParticipantCount();
	Draft_SavePreviousSnapshot();
	Draft_ValidateAllCivsAgainstPools();
	Draft_BroadcastPools();
	AnnounceGame("Draft created. Select your civilization from your drafted pool.");
	if result.warning ~= nil then
		AnnounceGame(result.warning);
	end
	Draft_RefreshBanUI();
	Draft_RefreshDraftIconsAll();
	Draft_UpdateActionButtons();
	-- Repopulate civ pulldowns with filtered lists
	if BuildSlots ~= nil then
		-- Only refresh civ pulldowns
		PopulateCivPulldown(Controls.CivPulldown, 0);
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local inst = m_SlotInstances[i];
			if inst ~= nil then
				PopulateCivPulldown(inst.CivPulldown, i);
			end
		end
	end
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	end
	Draft_PersistToPreGame();
end

function Draft_OnRestorePreviousDraft()
	if Draft_IsHistoryOnly() or IsDraftLockedByGameReady() then
		return;
	end
	if not Matchmaking.IsHost() or PreGame.IsHotSeatGame() then
		return;
	end
	local snap = g_PreviousDraftSnapshot;
	if snap == nil or snap.pools == nil then
		AnnounceGame("No previous draft to restore.");
		return;
	end
	local curCount = GetDraftParticipantCount();
	local snapCount = tonumber(snap.participantCount) or 0;
	if curCount > snapCount then
		AnnounceGame("Cannot restore draft: more players than when it was made.");
		return;
	end
	g_DraftPools = CopyDraftPools(snap.pools);
	g_DraftLocked = true;
	g_DraftSwapFirst = nil;
	g_DraftSwapDesire = {};
	g_DraftBanHostControl = {};
	g_DraftParticipantKey = GetDraftParticipantKey();
	g_DraftParticipantCount = snapCount;
	-- Restore ban-ready (green highlight) for slots that still exist in the deal.
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	local snapReady = snap.banReady or {};
	for _, pid in ipairs(GetDraftPlayerOrder()) do
		if snapReady[pid] == true then
			g_DraftBanReady[pid] = true;
		end
	end
	Draft_ValidateAllCivsAgainstPools();
	Draft_BroadcastPools();
	for _, pid in ipairs(GetDraftPlayerOrder()) do
		if g_DraftBanReady[pid] == true then
			SendDraftChat("BANREADY|" .. tostring(pid) .. "|1");
		end
	end
	Draft_BroadcastReadyMask();
	AnnounceGame("Previous draft restored.");
	Draft_RefreshBanUI();
	Draft_RefreshDraftIconsAll();
	Draft_UpdateActionButtons();
	PopulateCivPulldown(Controls.CivPulldown, 0);
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		local inst = m_SlotInstances[i];
		if inst ~= nil then
			PopulateCivPulldown(inst.CivPulldown, i);
		end
	end
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	end
	Draft_PersistToPreGame();
end

function Draft_OnResetDraft()
	if Draft_IsHistoryOnly() or IsDraftLockedByGameReady() then
		return;
	end
	if not Matchmaking.IsHost() then
		return;
	end
	g_DraftLocked = false;
	g_DraftPools = {};
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	g_DraftBanHostControl = {};
	g_DraftSwapFirst = nil;
	g_DraftSwapDesire = {};
	g_DraftParticipantKey = nil;
	g_DraftParticipantCount = nil;
	-- Clear per-player ready bits so Create Draft doesn't see stale PreGame ready.
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, maxP - 1 do
		Draft_OptSet(Draft_ReadyOptName(pid), 0);
	end
	-- Keep bans + g_PreviousDraftSnapshot so Restore Draft can undo this.
	SendDraftChat("RESET|1");
	Draft_BroadcastReadyMask();
	AnnounceGame("Draft reset. Players may change bans and ready again.");
	Draft_BroadcastRules();
	Draft_RefreshBanUI();
	Draft_RefreshDraftIconsAll();
	Draft_UpdateActionButtons();
	PopulateCivPulldown(Controls.CivPulldown, 0);
	for i = 1, GameDefines.MAX_MAJOR_CIVS do
		local inst = m_SlotInstances[i];
		if inst ~= nil then
			PopulateCivPulldown(inst.CivPulldown, i);
		end
	end
	if UpdateDisplay ~= nil then
		UpdateDisplay();
	end
	Draft_PersistToPreGame();
end

function Draft_OnSwapDraftsButton()
	-- Legacy no-op; swap is mutual via DraftSwapButton on each ban box.
	local localID = Matchmaking.GetLocalID();
	if g_DraftSwapDesire[localID] ~= nil then
		g_DraftSwapDesire[localID] = nil;
		SendDraftChat("SWAPREQ|" .. tostring(localID) .. "|-1");
		Draft_RefreshBanUI();
	end
end

-- Mutual draft-pool swap (same idea as seat SwapButton / DesiredSlot).
-- Click another player's ban-box swap to request; they click yours to accept.
function Draft_TrySelectSwapPlayer(playerID)
	if Draft_IsHistoryOnly() or not g_DraftLocked then
		return false;
	end
	local localID = Matchmaking.GetLocalID();
	if playerID == nil or playerID == localID then
		return false;
	end
	if not IsBanParticipantSlot(playerID) then
		return true;
	end

	-- Human Required has nobody to click back � instant swap for solo testing.
	-- Human Required / AI have nobody to click back � instant swap.
	if IsHumanRequiredSlot(playerID) or IsAISlot(playerID) then
		Draft_ApplyPoolSwap(localID, playerID, true);
		return true;
	end

	-- Toggle off if clicking the same target again.
	if g_DraftSwapDesire[localID] == playerID then
		g_DraftSwapDesire[localID] = nil;
		SendDraftChat("SWAPREQ|" .. tostring(localID) .. "|-1");
		return true;
	end

	g_DraftSwapDesire[localID] = playerID;
	SendDraftChat("SWAPREQ|" .. tostring(localID) .. "|" .. tostring(playerID));
	Draft_TryCompleteMutualSwap(localID, playerID);
	return true;
end

----------------------------------------------------------------
-- Incoming protocol
----------------------------------------------------------------
function Draft_HandleProtocol(fromPlayer, text)
	local body = string.sub(text, #DRAFT_PREFIX + 1);
	local op, rest = string.match(body, "^([^|]+)|(.*)$");
	if op == nil then
		op = body;
		rest = "";
	end

	if op == "RULES" then
		local bans, picks, coast, inland, vanilla, seasonal = string.match(rest, "^(%-?%d+)|(%-?%d+)|(%-?%d+)|(%-?%d+)|(%d+)|(%d+)$");
		if bans ~= nil then
			g_DraftRules.bansPerPlayer = tonumber(bans);
			g_DraftRules.picksPerPlayer = tonumber(picks);
			g_DraftRules.guaranteedCoastals = tonumber(coast);
			g_DraftRules.guaranteedInlands = tonumber(inland);
			g_DraftRules.vanillaOnly = tonumber(vanilla) == 1;
			g_DraftRules.seasonalBans = tonumber(seasonal) == 1;
			-- Host rules change clears everyone's ban-ready (see OnRulesChanged).
			g_DraftBanReady = {};
			g_DraftBanReadyPendingLocal = false;
			Draft_WriteLocalBanReadyOption(false);
			-- Resize ban arrays (all draft participants, including AI).
			for _, pid in ipairs(GetDraftPlayerOrder()) do
				EnsureBanArray(pid);
			end
			EnsureBanArray(Matchmaking.GetLocalID());
			Draft_PopulateRulesUI();
			Draft_RefreshBanUI();
			ResetHorizontalBanScrolls();
			Draft_UpdateActionButtons();
			Draft_PersistToPreGame();
		end
		return true;
	elseif op == "BAN" then
		local pid, list = string.match(rest, "^(%d+)|(.*)$");
		pid = tonumber(pid);
		if pid ~= nil then
			g_DraftBans[pid] = LekmodDrafter.DecodeCivList(list);
			EnsureBanArray(pid);
			Draft_RefreshBanUI();
			Draft_PersistToPreGame();
		end
		return true;
	elseif op == "BANREADY" then
		local pid, flag = string.match(rest, "^(%d+)|(%d+)$");
		pid = tonumber(pid);
		if pid ~= nil then
			local hostID = Matchmaking.GetHostID();
			-- Own ready announce, or host correcting (kick / restore / echo).
			if fromPlayer == pid or fromPlayer == hostID then
				local ready = (tonumber(flag) == 1);
				g_DraftBanReady[pid] = ready and true or nil;
				-- Host mirrors into that player's PreGame ready slot so others can
				-- poll it like PreGame.IsReady (without chat reannounce spam).
				if Matchmaking.IsHost() then
					Draft_OptSet(Draft_ReadyOptName(pid), ready and 1 or 0);
					Draft_BroadcastReadyMask();
				end
				Draft_RefreshBanUI();
				Draft_PersistToPreGame();
			end
		end
		return true;
	elseif op == "READYMASK" then
		-- Host-authoritative ready bits (green-up + Create Draft gate).
		if fromPlayer == Matchmaking.GetHostID() then
			local mask = tonumber(rest);
			if mask ~= nil then
				Draft_ApplyReadyMask(mask, true);
				Draft_RefreshBanUI();
				Draft_UpdateActionButtons();
				Draft_PersistToPreGame();
			end
		end
		return true;
	elseif op == "BANCTRL" then
		-- BANCTRL|pid|0/1 — player ceded (or reclaimed) ban edit rights to the host.
		local pid, flag = string.match(rest, "^(%d+)|(%d+)$");
		pid = tonumber(pid);
		if pid ~= nil then
			-- Owning player toggles; host may clear (kick/reset).
			if fromPlayer == pid or fromPlayer == Matchmaking.GetHostID() then
				g_DraftBanHostControl[pid] = (tonumber(flag) == 1);
				Draft_RefreshBanUI();
				Draft_PersistToPreGame();
			end
		end
		return true;
	elseif op == "DRAFT" then
		local pid, list = string.match(rest, "^(%d+)|(.*)$");
		pid = tonumber(pid);
		if pid ~= nil then
			g_DraftPools[pid] = LekmodDrafter.DecodeCivList(list);
			Draft_RefreshDraftIconsAll();
			Draft_PersistToPreGame();
		end
		return true;
	elseif op == "LOCK" then
		g_DraftLocked = (string.sub(rest, 1, 1) == "1");
		if g_DraftLocked then
			g_DraftParticipantKey = GetDraftParticipantKey();
			g_DraftParticipantCount = GetDraftParticipantCount();
			g_DraftBanHostControl = {};
			Draft_ValidateAllCivsAgainstPools();
		else
			g_DraftParticipantKey = nil;
			g_DraftParticipantCount = nil;
		end
		Draft_RefreshBanUI();
		Draft_RefreshDraftIconsAll();
		PopulateCivPulldown(Controls.CivPulldown, 0);
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local inst = m_SlotInstances[i];
			if inst ~= nil then
				PopulateCivPulldown(inst.CivPulldown, i);
			end
		end
		Draft_PersistToPreGame();
		if UpdateDisplay ~= nil then
			UpdateDisplay();
		end
		return true;
	elseif op == "SWAPREQ" then
		if Draft_IsHistoryOnly() then
			return true;
		end
		local fromID, toID = string.match(rest, "^(%-?%d+)|(%-?%d+)$");
		fromID, toID = tonumber(fromID), tonumber(toID);
		if fromID ~= nil then
			if toID == nil or toID < 0 then
				g_DraftSwapDesire[fromID] = nil;
			else
				g_DraftSwapDesire[fromID] = toID;
			end
			-- Do not complete here - only the accepting click completes (avoids double-swap).
			Draft_RefreshBanUI();
		end
		return true;
	elseif op == "SWAP" then
		if Draft_IsHistoryOnly() then
			return true;
		end
		-- SWAP|a|b|poolA|poolB  (absolute post-swap pools)
		local a, b, listA, listB = string.match(rest, "^(%d+)|(%d+)|([^|]*)|(.*)$");
		a, b = tonumber(a), tonumber(b);
		if a ~= nil and b ~= nil then
			if listA ~= nil then
				g_DraftPools[a] = LekmodDrafter.DecodeCivList(listA);
			end
			if listB ~= nil then
				g_DraftPools[b] = LekmodDrafter.DecodeCivList(listB);
			end
			g_DraftSwapDesire[a] = nil;
			g_DraftSwapDesire[b] = nil;
			Draft_ValidateCivAgainstPool(a);
			Draft_ValidateCivAgainstPool(b);
			Draft_RefreshDraftIconsAll();
			Draft_RefreshBanUI();
			PopulateCivPulldown(Controls.CivPulldown, 0);
			for i = 1, GameDefines.MAX_MAJOR_CIVS do
				local inst = m_SlotInstances[i];
				if inst ~= nil then
					PopulateCivPulldown(inst.CivPulldown, i);
				end
			end
			Draft_PersistToPreGame();
			if UpdateDisplay ~= nil then
				UpdateDisplay();
			end
			Draft_ValidateCivAgainstPool(a);
			Draft_ValidateCivAgainstPool(b);
			if UpdateDisplay ~= nil then
				UpdateDisplay();
			end
		end
		return true;
	elseif op == "RESET" then
		g_DraftLocked = false;
		g_DraftPools = {};
		g_DraftBanReady = {};
		g_DraftBanReadyPendingLocal = false;
		g_DraftBanHostControl = {};
		g_DraftSwapFirst = nil;
		g_DraftSwapDesire = {};
		g_DraftParticipantKey = nil;
		g_DraftParticipantCount = nil;
		Draft_WriteLocalBanReadyOption(false);
		Draft_RefreshBanUI();
		Draft_RefreshDraftIconsAll();
		PopulateCivPulldown(Controls.CivPulldown, 0);
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local inst = m_SlotInstances[i];
			if inst ~= nil then
				PopulateCivPulldown(inst.CivPulldown, i);
			end
		end
		Draft_PersistToPreGame();
		return true;
	end
	return true;
end

----------------------------------------------------------------
-- Draft Rules UI
----------------------------------------------------------------
local function FillNumberPull(pull, minV, maxV, includeRandom, selected)
	if pull == nil then
		return;
	end
	pull:ClearEntries();
	if includeRandom then
		local ct = {};
		pull:BuildEntry("InstanceOne", ct);
		ct.Button:SetText("Random");
		ct.Button:SetVoid1(-1);
	end
	for v = minV, maxV do
		local ct = {};
		pull:BuildEntry("InstanceOne", ct);
		ct.Button:SetText(tostring(v));
		ct.Button:SetVoid1(v);
	end
	pull:CalculateInternals();
	-- Set label roughly
	local label = "Random";
	if selected ~= nil and selected >= 0 then
		label = tostring(selected);
	elseif selected == nil then
		label = tostring(minV);
	end
	if selected ~= nil and selected < 0 then
		label = "Random";
	end
	pull:GetButton():SetText(label);
end

function Draft_PopulateRulesUI()
	local r = g_DraftRules;
	FillNumberPull(Controls.DraftBansPull, 0, 5, false, r.bansPerPlayer);
	FillNumberPull(Controls.DraftPicksPull, 1, 10, false, r.picksPerPlayer);
	FillNumberPull(Controls.DraftCoastalsPull, 0, math.max(1, r.picksPerPlayer or 3), true, r.guaranteedCoastals);
	FillNumberPull(Controls.DraftInlandsPull, 0, math.max(1, r.picksPerPlayer or 3), true, r.guaranteedInlands);
	if Controls.DraftVanillaCheck ~= nil then
		Controls.DraftVanillaCheck:SetCheck(r.vanillaOnly == true);
	end
	if Controls.DraftSeasonalCheck ~= nil then
		Controls.DraftSeasonalCheck:SetCheck(r.seasonalBans == true);
	end
	local canEdit = Matchmaking.IsHost() and not g_DraftLocked and not Draft_IsHistoryOnly() and not IsDraftLockedByGameReady();
	if Controls.DraftBansPull ~= nil then Controls.DraftBansPull:SetDisabled(not canEdit); end
	if Controls.DraftPicksPull ~= nil then Controls.DraftPicksPull:SetDisabled(not canEdit); end
	if Controls.DraftCoastalsPull ~= nil then Controls.DraftCoastalsPull:SetDisabled(not canEdit); end
	if Controls.DraftInlandsPull ~= nil then Controls.DraftInlandsPull:SetDisabled(not canEdit); end
	if Controls.DraftVanillaCheck ~= nil then Controls.DraftVanillaCheck:SetDisabled(not canEdit); end
	if Controls.DraftSeasonalCheck ~= nil then Controls.DraftSeasonalCheck:SetDisabled(not canEdit); end
	if Controls.DraftRulesHostOnlyLabel ~= nil then
		if Draft_IsHistoryOnly() then
			Controls.DraftRulesHostOnlyLabel:SetText("Ongoing game lobby - draft rules are view-only.");
		elseif IsDraftLockedByGameReady() then
			Controls.DraftRulesHostOnlyLabel:SetText("A player is ready to start - draft rules are locked.");
		elseif Matchmaking.IsHost() then
			Controls.DraftRulesHostOnlyLabel:SetText(g_DraftLocked and "Draft is locked. Reset Draft to edit rules." or "You are the host - changes sync to all players.");
		else
			Controls.DraftRulesHostOnlyLabel:SetText("Only the host can edit these settings.");
		end
	end
end

local function OnRulesChanged()
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then
		Draft_PopulateRulesUI();
		return;
	end
	-- Clear ban-ready when rules change (host clears PreGame ready bits too).
	g_DraftBanReady = {};
	g_DraftBanReadyPendingLocal = false;
	local maxP = GameDefines.MAX_MAJOR_CIVS;
	for pid = 0, maxP - 1 do
		Draft_OptSet(Draft_ReadyOptName(pid), 0);
	end
	-- Resize ban arrays in place. Do NOT destroy InstanceManagers here — abandoning
	-- them and creating new ones on the same stacks leaves orphan icons / bad widths
	-- on BanHost horizontal scroll after bans-per-player changes.
	for _, pid in ipairs(GetDraftPlayerOrder()) do
		EnsureBanArray(pid);
	end
	EnsureBanArray(Matchmaking.GetLocalID());
	-- Persist full draft state, then broadcast like other lobby game options.
	Draft_PersistToPreGame();
	Draft_BroadcastRules();
	Draft_BroadcastReadyMask();
	-- May no-op while on Draft Rules tab; Players tab refresh rebuilds icons.
	Draft_RefreshBanUI();
	-- After rebuild (or immediately if still on Draft Rules), reset horizontal scroll.
	ResetHorizontalBanScrolls();
	Draft_PopulateRulesUI();
end

function Draft_OnBansPull(value)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then return; end
	g_DraftRules.bansPerPlayer = value;
	OnRulesChanged();
end
function Draft_OnPicksPull(value)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then return; end
	g_DraftRules.picksPerPlayer = value;
	OnRulesChanged();
end
function Draft_OnCoastalsPull(value)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then return; end
	g_DraftRules.guaranteedCoastals = value;
	OnRulesChanged();
end
function Draft_OnInlandsPull(value)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then return; end
	g_DraftRules.guaranteedInlands = value;
	OnRulesChanged();
end
function Draft_OnVanillaCheck(bChecked)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then
		if Controls.DraftVanillaCheck ~= nil then
			Controls.DraftVanillaCheck:SetCheck(g_DraftRules.vanillaOnly == true);
		end
		return;
	end
	g_DraftRules.vanillaOnly = bChecked;
	OnRulesChanged();
end
function Draft_OnSeasonalCheck(bChecked)
	if not Matchmaking.IsHost() or g_DraftLocked or IsDraftLockedByGameReady() then
		if Controls.DraftSeasonalCheck ~= nil then
			Controls.DraftSeasonalCheck:SetCheck(g_DraftRules.seasonalBans == true);
		end
		return;
	end
	g_DraftRules.seasonalBans = bChecked;
	OnRulesChanged();
end

----------------------------------------------------------------
-- Page tab integration
----------------------------------------------------------------
local function Draft_ApplyCompactRightPane()
	local compact = Draft_IsLobbyCompact();
	local showPlayers = (g_DraftPageMode == nil or g_DraftPageMode == "players");
	local showBans = false;
	local showSummary = false;
	if showPlayers then
		if compact then
			showBans = (g_CompactRightPane == "bans");
			showSummary = (g_CompactRightPane == "summary");
		else
			showBans = true;
			showSummary = true;
		end
	end

	if Controls.BanHost ~= nil then Controls.BanHost:SetHide(not showBans); end
	if Controls.BanSlotStack ~= nil then Controls.BanSlotStack:SetHide(not showBans); end
	if Controls.BanPickerPanel ~= nil and not showBans then Draft_HideBanPicker(); end

	if Controls.CompactRightToggle ~= nil then
		Controls.CompactRightToggle:SetHide(not (compact and showPlayers));
		if Controls.CompactRightToggleLabel ~= nil then
			Controls.CompactRightToggleLabel:SetText(g_CompactRightPane == "bans" and "Settings" or "Bans");
		end
		Controls.CompactRightToggle:SetToolTipString(
			g_CompactRightPane == "bans"
				and "Show game settings summary in the right column"
				or "Show ban list in the right column"
		);
	end

	if Controls.VerticalTrim2 ~= nil then
		-- Only between ban + summary in wide layout.
		Controls.VerticalTrim2:SetHide(compact or not showSummary);
	end
	if Controls.GameOptionsSummary ~= nil then
		Controls.GameOptionsSummary:SetHide(not showSummary);
	end
	if Controls.GameOptionsSummaryTitle ~= nil then
		Controls.GameOptionsSummaryTitle:SetHide(true);
	end
end

local function Draft_ApplyCompactSecondaryTabs()
	-- Always show Draft Rules + Game Options tabs (no compact shared-slot / arrow cycle).
	if Controls.CompactSecondaryPrev ~= nil then Controls.CompactSecondaryPrev:SetHide(true); end
	if Controls.CompactSecondaryNext ~= nil then Controls.CompactSecondaryNext:SetHide(true); end
	if Controls.DraftRulesPageTab ~= nil then Controls.DraftRulesPageTab:SetHide(false); end
	if Controls.OptionsPageTab ~= nil then Controls.OptionsPageTab:SetHide(false); end
end

function Draft_UpdatePageTabView()
	local mode = g_DraftPageMode or "players";
	local showPlayers = (mode == "players");
	local showDraft = (mode == "draft");
	local showOptions = (mode == "options");

	-- Reuse m_bEditOptions for options compatibility with existing code paths
	m_bEditOptions = showOptions;

	if Controls.Host ~= nil then Controls.Host:SetHide(not showPlayers); end
	if Controls.ListingScrollPanel ~= nil then Controls.ListingScrollPanel:SetHide(not showPlayers); end
	if Controls.VerticalTrim ~= nil then Controls.VerticalTrim:SetHide(not showPlayers); end
	if Controls.BanPickerPanel ~= nil and not showPlayers then Draft_HideBanPicker(); end

	if Controls.OptionsScrollPanel ~= nil then Controls.OptionsScrollPanel:SetHide(not showOptions); end
	if Controls.DraftRulesScrollPanel ~= nil then Controls.DraftRulesScrollPanel:SetHide(not showDraft); end

	if Controls.PlayersPageTabHighlight ~= nil then Controls.PlayersPageTabHighlight:SetHide(not showPlayers); end
	if Controls.DraftRulesPageTabHighlight ~= nil then Controls.DraftRulesPageTabHighlight:SetHide(not showDraft); end
	if Controls.OptionsPageTabHighlight ~= nil then Controls.OptionsPageTabHighlight:SetHide(not showOptions); end

	if showDraft then
		g_CompactSecondary = "draft";
		Draft_PopulateRulesUI();
		if Controls.DraftRulesScrollPanel ~= nil then
			Controls.DraftRulesScrollPanel:CalculateInternalSize();
		end
	elseif showOptions then
		g_CompactSecondary = "options";
	end

	Draft_ApplyCompactSecondaryTabs();
	Draft_ApplyCompactRightPane();

	if showPlayers then
		-- Rebuild ban/draft icons after rules changes done on other tabs
		-- (OnRulesChanged clears IMs while Ban UI refresh is skipped off-page).
		if (not Draft_IsLobbyCompact()) or g_CompactRightPane == "bans" then
			Draft_RefreshBanUI();
		end
		Draft_RefreshDraftIconsAll();
	end
	Draft_UpdateActionButtons();
end

function Draft_OnPlayersTab()
	g_DraftPageMode = "players";
	Draft_UpdatePageTabView();
	if UpdatePageTitle ~= nil then UpdatePageTitle(); end
end
function Draft_OnDraftRulesTab()
	g_CompactSecondary = "draft";
	g_DraftPageMode = "draft";
	Draft_UpdatePageTabView();
	if Controls.TitleLabel ~= nil then
		Controls.TitleLabel:SetText("DRAFT RULES");
	end
end
function Draft_OnOptionsTab()
	g_CompactSecondary = "options";
	g_DraftPageMode = "options";
	Draft_UpdatePageTabView();
	if UpdatePageTabView ~= nil then
		-- Call original options populate path
		m_bEditOptions = true;
		if Controls.OptionsScrollPanel ~= nil then
			Controls.OptionsScrollPanel:SetSizeY(Controls.VerticalTrim:GetSizeY() - 2);
			Controls.OptionsScrollPanel:CalculateInternalSize();
		end
		if PopulateMapSizePulldown ~= nil then PopulateMapSizePulldown(); end
		if RefreshMapScripts ~= nil then RefreshMapScripts(); end
		PreGame.SetRandomMapScript(false);
		if UpdateGameOptionsDisplay ~= nil then UpdateGameOptionsDisplay(true); end
	end
	if UpdatePageTitle ~= nil then UpdatePageTitle(); end
end

function Draft_OnCompactRightToggle()
	if not Draft_IsLobbyCompact() then
		return;
	end
	-- Only meaningful on the players page (bans / summary column).
	if g_DraftPageMode ~= nil and g_DraftPageMode ~= "players" then
		return;
	end
	if g_CompactRightPane == "bans" then
		g_CompactRightPane = "summary";
	else
		g_CompactRightPane = "bans";
	end
	Draft_ApplyCompactRightPane();
	if g_CompactRightPane == "bans" then
		Draft_RefreshBanUI();
	elseif UpdateGameOptionsDisplay ~= nil then
		pcall(function() UpdateGameOptionsDisplay(true); end);
	end
end

----------------------------------------------------------------
-- Init / hooks
----------------------------------------------------------------
function Draft_AdjustScreenSize()
	local screenX, screenY = UIManager:GetScreenSizeVal();
	-- Compact = hide right-hand summary so player+ban fit like vanilla 960.
	-- Wide (1366+) keeps the full 1275 layout. No column scaling.
	g_LobbyCompact = (screenX < LOBBY_COMPACT_MAX_SCREEN);
	local compact = g_LobbyCompact;
	local mainW = compact and LOBBY_COMPACT_W or LOBBY_WIDE_W;

	local TOP_COMPENSATION = 52 + ((screenY - 768) * 0.3);
	local CHAT_TOP_FROM_BOTTOM = 282;
	if Controls.ChatBox ~= nil and Controls.ChatBox:IsHidden() then
		CHAT_TOP_FROM_BOTTOM = 80;
	end
	local mainH = screenY - TOP_COMPENSATION;
	local trimH = mainH - 75 - CHAT_TOP_FROM_BOTTOM;
	local listH = mainH - 201 - CHAT_TOP_FROM_BOTTOM;

	local function setSize(ctrl, w, h)
		if ctrl == nil then return; end
		if w ~= nil and h ~= nil then
			ctrl:SetSizeVal(w, h);
		elseif w ~= nil then
			ctrl:SetSizeX(w);
		elseif h ~= nil then
			ctrl:SetSizeY(h);
		end
	end
	local function setOffset(ctrl, x, y)
		if ctrl == nil then return; end
		pcall(function() ctrl:SetOffsetVal(x, y); end);
	end

	if Controls.MainGrid ~= nil then
		Controls.MainGrid:SetSizeX(mainW);
		Controls.MainGrid:SetSizeY(mainH);
	end

	-- Fixed player + ban geometry (same in both modes).
	if Controls.ListingScrollPanel ~= nil then
		setSize(Controls.ListingScrollPanel, 926, listH);
		setOffset(Controls.ListingScrollPanel, 18, 201);
		Controls.ListingScrollPanel:CalculateInternalSize();
	end
	if Controls.ListingScrollBar ~= nil then
		setOffset(Controls.ListingScrollBar, 590, 18);
		Controls.ListingScrollBar:SetSizeY(math.max(40, listH - 36));
	end
	if Controls.ListingScrollUp ~= nil then setOffset(Controls.ListingScrollUp, 590, 0); end
	if Controls.ListingScrollDown ~= nil then setOffset(Controls.ListingScrollDown, 590, 0); end

	if Controls.Host ~= nil then
		setSize(Controls.Host, 610, SLOT_ROW_HEIGHT);
		setOffset(Controls.Host, 17, 76);
	end
	if Controls.HostFrame ~= nil then setSize(Controls.HostFrame, 610, SLOT_ROW_HEIGHT); end
	if Controls.BanHost ~= nil then
		setSize(Controls.BanHost, 310, SLOT_ROW_HEIGHT);
		setOffset(Controls.BanHost, 635, 76);
	end
	if Controls.BanHostFrame ~= nil then setSize(Controls.BanHostFrame, 310, SLOT_ROW_HEIGHT); end
	if Controls.VerticalTrim ~= nil then
		setOffset(Controls.VerticalTrim, 625, 75);
		Controls.VerticalTrim:SetSizeY(trimH);
	end

	-- Summary column geometry (visibility handled by Draft_ApplyCompactRightPane).
	if Controls.VerticalTrim2 ~= nil then
		setOffset(Controls.VerticalTrim2, 948, 75);
		Controls.VerticalTrim2:SetSizeY(trimH);
	end
	if Controls.GameOptionsSummary ~= nil then
		if compact then
			-- Sit in the ban column slot when toggled to Settings; leave a strip above chat for the toggle.
			setSize(Controls.GameOptionsSummary, 310, math.max(80, trimH - 48));
			setOffset(Controls.GameOptionsSummary, 635, 80);
		else
			setSize(Controls.GameOptionsSummary, 315, trimH - 8);
			setOffset(Controls.GameOptionsSummary, 958, 80);
		end
		Controls.GameOptionsSummary:CalculateInternalSize();
	end
	if Controls.GameOptionsSummaryTitle ~= nil then
		Controls.GameOptionsSummaryTitle:SetHide(true);
	end
	if Controls.CompactRightToggle ~= nil then
		-- Just above chat in the right column (Anchor R,B). Not over Draft Rules tabs.
		setOffset(Controls.CompactRightToggle, 25, CHAT_TOP_FROM_BOTTOM + 2);
		-- Visibility is owned by Draft_ApplyCompactRightPane (players page only).
	end

	-- Chrome that must match MainGrid width.
	local barW = compact and 920 or 1240;
	local fullInner = compact and 920 or 1220;
	local chatW = compact and (mainW - 40) or 1240;
	if Controls.PageTabBar ~= nil then
		setSize(Controls.PageTabBar, barW, 32);
		if compact then
			-- Three tabs across the compact bar (no shared-slot arrows).
			local tabW = math.floor((barW - 40) / 3);
			setOffset(Controls.PlayersPageTab, 10, 2);
			setSize(Controls.PlayersPageTab, tabW, 24);
			setOffset(Controls.DraftRulesPageTab, 20 + tabW, 2);
			setSize(Controls.DraftRulesPageTab, tabW, 24);
			setOffset(Controls.OptionsPageTab, 30 + tabW * 2, 2);
			setSize(Controls.OptionsPageTab, tabW, 24);
		else
			setOffset(Controls.PlayersPageTab, 155, 2);
			setSize(Controls.PlayersPageTab, 300, 24);
			setOffset(Controls.DraftRulesPageTab, 620, 2);
			setSize(Controls.DraftRulesPageTab, 310, 24);
			setOffset(Controls.OptionsPageTab, 940, 2);
			setSize(Controls.OptionsPageTab, 300, 24);
		end
	end
	-- Horizontal dividers: SetSizeVal (SetSizeX alone fails on "W.H" Image sizes).
	local function setTrimW(ctrl, w)
		if ctrl == nil then return; end
		pcall(function()
			local _, h = ctrl:GetSizeVal();
			if h == nil or h < 1 then h = 5; end
			ctrl:SetSizeVal(w, h);
		end);
	end
	setTrimW(Controls.PageTabBarTrim, barW);
	setTrimW(Controls.HostBottomTrim, 610);
	setTrimW(Controls.BanHostBottomTrim, 310);
	setTrimW(Controls.ChatPanelTopTrim, chatW);
	setTrimW(Controls.ChatEntryTrim, chatW);
	setTrimW(Controls.MainBottomTrim, math.min(chatW, mainW - 20));

	if Controls.DraftRulesScrollPanel ~= nil then
		setSize(Controls.DraftRulesScrollPanel, fullInner, mainH - 76 - CHAT_TOP_FROM_BOTTOM);
		Controls.DraftRulesScrollPanel:CalculateInternalSize();
	end
	if Controls.OptionsScrollPanel ~= nil then
		setSize(Controls.OptionsScrollPanel, fullInner, nil);
		pcall(function()
			Controls.OptionsScrollPanel:SetSizeY(trimH - 2);
		end);
		Controls.OptionsScrollPanel:CalculateInternalSize();
	end
	if Controls.BottomButtonBar ~= nil then
		Draft_RefreshBottomButtonBar();
	end
	if Controls.ChatBox ~= nil then
		setSize(Controls.ChatBox, chatW, 164);
		-- Keep chat inside MainGrid on compact (was clipping the bottom gold line).
		if compact then
			setOffset(Controls.ChatBox, 20, 110);
		else
			setOffset(Controls.ChatBox, 17, 110);
		end
		if Controls.ChatPanel ~= nil then setSize(Controls.ChatPanel, chatW, 160); end
		if Controls.ChatScroll ~= nil then
			setSize(Controls.ChatScroll, math.max(200, chatW - 30), 123);
			Controls.ChatScroll:CalculateInternalSize();
		end
		-- Chat entry row / field must shrink with chatW or the gold line sticks out.
		local entryFieldW = math.max(120, chatW - 105);
		if Controls.ChatEntryRow ~= nil then setSize(Controls.ChatEntryRow, math.max(200, chatW - 2), 36); end
		if Controls.ChatEntryFieldBox ~= nil then setSize(Controls.ChatEntryFieldBox, entryFieldW, 27); end
		if Controls.ChatEntry ~= nil then setSize(Controls.ChatEntry, entryFieldW, 23); end
	end

	-- Restore fixed slot widths.
	if m_SlotInstances ~= nil then
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local slot = m_SlotInstances[i];
			if slot ~= nil and slot.Root ~= nil then
				local _, h = slot.Root:GetSizeVal();
				if h == nil or h < 1 then h = SLOT_ROW_HEIGHT; end
				slot.Root:SetSizeVal(575, h);
			end
		end
	end
	if g_BanSlotInstances ~= nil then
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local ban = g_BanSlotInstances[i];
			if ban ~= nil and ban.Root ~= nil and not ban.Root:IsHidden() then
				ban.Root:SetSizeVal(310, SLOT_ROW_HEIGHT);
			end
		end
	end

	Draft_SyncBanScroll();
	Draft_ApplyCompactSecondaryTabs();
	Draft_ApplyCompactRightPane();
end

function Draft_OnUpdateDisplay()
	-- Only invalidate when the draft deal grows (new human/AI/human-required slot).
	-- Slot type swaps that keep the same count (e.g. Human <-> AI) leave the draft alone.
	if g_DraftLocked and Matchmaking.IsHost() and not Draft_IsHistoryOnly() then
		local count = GetDraftParticipantCount();
		if g_DraftParticipantCount ~= nil and count > g_DraftParticipantCount then
			AnnounceGame("Draft cleared: additional players joined the draft.");
			Draft_OnResetDraft();
			return;
		end
	end
	if g_DraftLocked and not Draft_IsHistoryOnly() then
		Draft_ValidateAllCivsAgainstPools();
	end
	-- Rules first: a host rules reset clears ready before we re-pull bits/mask.
	Draft_PullRulesFromPreGame();
	Draft_PullBanReadyFromPreGame();
	Draft_RefreshBanUI();
	Draft_RefreshDraftIconsAll();
	Draft_UpdateActionButtons();
end

function Draft_Init()
	math.randomseed(os.time());
	g_DraftRules = g_DraftRules or LekmodDrafter.DefaultRules();
	-- Mid-game / hotjoin lobby: ensure launched flag is set for this save going forward.
	if PreGame.GameStarted() == true then
		Draft_MarkGameLaunched();
	end
	-- Restore bans/rules/pools from a loaded lobby/game save (PreGame options).
	-- Skip on repeat Draft_Init in the same lobby visit — that wipe was erasing
	-- chat-synced ban-ready and blocking Create Draft on the host.
	if not g_DraftLiveSession then
		if Draft_RestoreFromPreGame() then
			if g_DraftLocked then
				g_DraftParticipantKey = GetDraftParticipantKey();
				g_DraftParticipantCount = GetDraftParticipantCount();
				-- Treat restored draft as restorable if host clears it later.
				if g_PreviousDraftSnapshot == nil then
					Draft_SavePreviousSnapshot();
				end
			end
		end
		g_DraftLiveSession = true;
	end
	Draft_CreateBanSlots();
	Draft_PopulateRulesUI();

	WireHScrollButtons(Controls.BanHostScrollLeft, Controls.BanHostScrollRight, Controls.BanHostIconScroll, Controls.BanHostIconStack, 70, false);
	WireHScrollButtons(Controls.DraftScrollLeft, Controls.DraftScrollRight, Controls.DraftIconScroll, Controls.DraftIconStack, 34, true);
	WireHScrollButtons(Controls.BottomButtonScrollLeft, Controls.BottomButtonScrollRight, Controls.BottomButtonScroll, Controls.BottomButtonStack, 160, true);
	if m_SlotInstances ~= nil then
		for i = 1, GameDefines.MAX_MAJOR_CIVS do
			local slot = m_SlotInstances[i];
			if slot ~= nil then
				WireHScrollButtons(slot.DraftScrollLeft, slot.DraftScrollRight, slot.DraftIconScroll, slot.DraftIconStack, 34, true);
			end
		end
	end

	if Controls.BanHostReadyCheck ~= nil then
		Controls.BanHostReadyCheck:RegisterCheckHandler(Draft_OnLocalBanReady);
	end
	if Controls.BanPickerClearButton ~= nil then
		Controls.BanPickerClearButton:RegisterCallback(Mouse.eLClick, function()
			Draft_ApplyBanSelection(-1);
		end);
	end
	if Controls.BanPickerDismiss ~= nil then
		Controls.BanPickerDismiss:RegisterCallback(Mouse.eLClick, Draft_OnBanPickerDismiss);
	end
	if Controls.CreateDraftButton ~= nil then
		Controls.CreateDraftButton:RegisterCallback(Mouse.eLClick, Draft_OnCreateDraft);
	end
	if Controls.ClearDraftButton ~= nil then
		Controls.ClearDraftButton:RegisterCallback(Mouse.eLClick, Draft_OnResetDraft);
	end
	if Controls.RestoreDraftButton ~= nil then
		Controls.RestoreDraftButton:RegisterCallback(Mouse.eLClick, Draft_OnRestorePreviousDraft);
	end
	if Controls.BanHostDraftSwapButton ~= nil then
		Controls.BanHostDraftSwapButton:RegisterCallback(Mouse.eLClick, function()
			Draft_OnBanSwapClick(Matchmaking.GetLocalID());
		end);
	end
	if Controls.BanHostDelegateButton ~= nil then
		Controls.BanHostDelegateButton:RegisterCallback(Mouse.eLClick, Draft_OnDelegateBanControl);
	end
	if Controls.DraftResetButton ~= nil then
		Controls.DraftResetButton:RegisterCallback(Mouse.eLClick, Draft_OnResetDraft);
	end
	if Controls.DraftRulesPageTab ~= nil then
		Controls.DraftRulesPageTab:RegisterCallback(Mouse.eLClick, Draft_OnDraftRulesTab);
	end
	-- Override players/options tabs to use three-mode page
	if Controls.PlayersPageTab ~= nil then
		Controls.PlayersPageTab:RegisterCallback(Mouse.eLClick, Draft_OnPlayersTab);
	end
	if Controls.OptionsPageTab ~= nil then
		Controls.OptionsPageTab:RegisterCallback(Mouse.eLClick, Draft_OnOptionsTab);
	end
	if Controls.CompactRightToggle ~= nil then
		Controls.CompactRightToggle:RegisterCallback(Mouse.eLClick, Draft_OnCompactRightToggle);
	end

	if Controls.DraftBansPull ~= nil then
		Controls.DraftBansPull:RegisterSelectionCallback(function(v) Draft_OnBansPull(v); end);
	end
	if Controls.DraftPicksPull ~= nil then
		Controls.DraftPicksPull:RegisterSelectionCallback(function(v) Draft_OnPicksPull(v); end);
	end
	if Controls.DraftCoastalsPull ~= nil then
		Controls.DraftCoastalsPull:RegisterSelectionCallback(function(v) Draft_OnCoastalsPull(v); end);
	end
	if Controls.DraftInlandsPull ~= nil then
		Controls.DraftInlandsPull:RegisterSelectionCallback(function(v) Draft_OnInlandsPull(v); end);
	end
	if Controls.DraftVanillaCheck ~= nil then
		Controls.DraftVanillaCheck:RegisterCheckHandler(Draft_OnVanillaCheck);
	end
	if Controls.DraftSeasonalCheck ~= nil then
		Controls.DraftSeasonalCheck:RegisterCheckHandler(Draft_OnSeasonalCheck);
	end

	if Matchmaking.IsHost() then
		Draft_BroadcastRules();
		-- Re-sync restored draft to clients joining a loaded lobby config.
		for _, pid in ipairs(GetDraftPlayerOrder()) do
			if g_DraftBans[pid] ~= nil then
				Draft_BroadcastBans(pid);
			end
			if g_DraftBanReady[pid] == true then
				SendDraftChat("BANREADY|" .. tostring(pid) .. "|1");
			end
			if g_DraftBanHostControl[pid] == true then
				SendDraftChat("BANCTRL|" .. tostring(pid) .. "|1");
			end
		end
		if g_DraftLocked then
			Draft_BroadcastPools();
		end
		Draft_BroadcastReadyMask();
	else
		-- Publish our ban-ready bit into PreGame (like SetReady) after lobby UI rebuild.
		local localID = Matchmaking.GetLocalID();
		if localID ~= nil and g_DraftBanReady[localID] == true then
			Draft_WriteLocalBanReadyOption(true);
			SendDraftChat("BANREADY|" .. tostring(localID) .. "|1");
		end
	end
	Draft_PersistToPreGame();
	Draft_RefreshBanUI();
	g_DraftPageMode = "players";
	Draft_UpdatePageTabView();
	-- Keep OnStagingUpdate running so ban column scroll stays synced with players.
	if EnsureStagingUpdate ~= nil then
		EnsureStagingUpdate();
	end
end
