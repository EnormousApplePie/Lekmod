------------------------------------------------------------------------------
-- Lekmod version helpers (menu status + lobby version-name checks)
------------------------------------------------------------------------------

LekmodVersion = LekmodVersion or {}

-- Technical version for MP lobby name / update checks (display titles live in TXT_KEY_LEKMOD_*).
LekmodVersion.LOCAL_VERSION = "v35.3000"

-- Plain lobby chat posted by clients that skipped ui_check.bat (Steam invite bypasses the legal screen).
LekmodVersion.UI_CHECK_CHAT = "UI_CHECK NOT LAUNCHED"
-- System / "Game:" chat lines (draft notices, etc.).
LekmodVersion.GAME_CHAT_PREFIX = "#LGAME#"
LekmodVersion.GAME_CHAT_NAME = "Game"
-- Lobby/in-game protocol prefixes that must never appear as player chat.
LekmodVersion.OLD_HANDSHAKE_PREFIX = "#LEKVER#"
LekmodVersion.LOBBY_CHAT_REQ = "#LCHREQ#"
LekmodVersion.LOBBY_CHAT_CLEAR = "#LCHCLEAR#"
LekmodVersion.LOBBY_CHAT_PREFIX = "#LCH#"
LekmodVersion.DRAFT_PREFIX = "#LDRAFT#"
LekmodVersion.VERSIONS_PAGE_URL = "https://github.com/EnormousApplePie/Lekmod/blob/main/LekmodInstaller/github_setup/versions.json"
-- FrontEnd fetches these via undocumented vanilla Network.HttpRequest (Civ5-Patch pattern).
LekmodVersion.VERSIONS_RAW_URLS = {
	"https://raw.githubusercontent.com/EnormousApplePie/Lekmod/main/LekmodInstaller/github_setup/versions.json",
	"https://cdn.jsdelivr.net/gh/EnormousApplePie/Lekmod@main/LekmodInstaller/github_setup/versions.json",
}
LekmodVersion.HTTP_TIMEOUT = 10 -- seconds

function LekmodVersion.Normalize(versionText)
	if versionText == nil then
		return nil
	end
	local version = string.match(tostring(versionText), "[vV]?(%d+%.%d+[%d%.]*)")
	if version == nil then
		return nil
	end
	return "v" .. version
end

function LekmodVersion.GetLocal()
	if LekmodVersion.LOCAL_VERSION ~= nil then
		return LekmodVersion.Normalize(LekmodVersion.LOCAL_VERSION) or LekmodVersion.LOCAL_VERSION;
	end
	local raw = Locale.Lookup("TXT_KEY_LEKMOD_VERSION")
	return LekmodVersion.Normalize(raw) or raw
end

-- Stamp file written by ui_check.bat / the installer (Lua/Utilities, not Lua/UI).
function LekmodVersion.IsUiCheckConfigured()
	local previous = LekmodUiConfigured
	LekmodUiConfigured = nil
	pcall(function()
		include("LekmodUiConfigured")
	end)
	local ok = LekmodUiConfigured == true
	LekmodUiConfigured = previous
	return ok
end

-- Visible as this player's own chat so vanilla hosts still see it.
function LekmodVersion.SendUiCheckNotLaunchedChat()
	if LekmodVersion.IsUiCheckConfigured() then
		return false
	end
	if Network == nil or type(Network.SendChat) ~= "function" then
		return false
	end
	Network.SendChat(LekmodVersion.UI_CHECK_CHAT)
	return true
end

function LekmodVersion.StartsWith(text, prefix)
	if text == nil or prefix == nil then
		return false
	end
	return string.sub(tostring(text), 1, #prefix) == prefix
end

-- True for handshake / lobby-history / draft tokens. Hide these in in-game chat.
function LekmodVersion.IsHiddenChatProtocol(text)
	if text == nil or text == "" then
		return false
	end
	text = tostring(text)
	if text == LekmodVersion.LOBBY_CHAT_REQ or text == LekmodVersion.LOBBY_CHAT_CLEAR then
		return true
	end
	if LekmodVersion.StartsWith(text, LekmodVersion.OLD_HANDSHAKE_PREFIX) then
		return true
	end
	if LekmodVersion.StartsWith(text, LekmodVersion.LOBBY_CHAT_PREFIX) then
		return true
	end
	if LekmodVersion.StartsWith(text, LekmodVersion.DRAFT_PREFIX) then
		return true
	end
	return false
end

function LekmodVersion.IsGameChat(text)
	if text == nil then
		return false
	end
	local prefix = LekmodVersion.GAME_CHAT_PREFIX
	return string.sub(text, 1, #prefix) == prefix
end

function LekmodVersion.GetGameChatBody(text)
	if not LekmodVersion.IsGameChat(text) then
		return text
	end
	return string.sub(text, #LekmodVersion.GAME_CHAT_PREFIX + 1)
end

function LekmodVersion.EncodeGameChat(body)
	return LekmodVersion.GAME_CHAT_PREFIX .. tostring(body or "")
end

function LekmodVersion.FormatGameChatDisplay(body)
	return "[COLOR_YELLOW]" .. LekmodVersion.GAME_CHAT_NAME .. ": " .. tostring(body or "") .. "[ENDCOLOR]"
end

function LekmodVersion.Compare(a, b)
	local function parts(v)
		local out = {}
		v = LekmodVersion.Normalize(v) or ""
		for n in string.gmatch(v, "%d+") do
			table.insert(out, tonumber(n) or 0)
		end
		return out
	end
	local pa, pb = parts(a), parts(b)
	local n = math.max(#pa, #pb)
	for i = 1, n do
		local va = pa[i] or 0
		local vb = pb[i] or 0
		if va > vb then return 1 end
		if va < vb then return -1 end
	end
	return 0
end

local function ResultFromLatest(localVersion, latest, fileId, source)
	local result = {
		Local = localVersion,
		Success = true,
		IsNewer = LekmodVersion.Compare(latest, localVersion) > 0,
		Latest = latest,
		FileId = fileId,
		DownloadUrl = nil,
		Error = nil,
		Source = source,
	}
	if fileId ~= nil and fileId ~= "" then
		result.DownloadUrl = "https://drive.google.com/uc?export=download&id=" .. tostring(fileId)
	end
	return result
end

function LekmodVersion.UnreachableResult(errorCode)
	return {
		Local = LekmodVersion.GetLocal(),
		Success = false,
		IsNewer = false,
		Latest = nil,
		DownloadUrl = nil,
		Error = errorCode or "unreachable",
		Source = "http",
	}
end

-- Parse installer versions.json body; returns latest version + file_id.
function LekmodVersion.ParseVersionsJson(body)
	if body == nil or body == "" then
		return nil, nil
	end
	local best, bestFileId = nil, nil
	for ver, block in string.gmatch(tostring(body), '"([vV]?%d+%.%d+[%d%.]*)"%s*:%s*(%b{})') do
		local norm = LekmodVersion.Normalize(ver)
		if norm ~= nil then
			local fileId = string.match(block, '"file_id"%s*:%s*"([^"]+)"')
			if best == nil or LekmodVersion.Compare(norm, best) > 0 then
				best = norm
				bestFileId = fileId
			end
		end
	end
	return best, bestFileId
end

function LekmodVersion.ResultFromVersionsBody(body, source)
	local localVersion = LekmodVersion.GetLocal()
	local latest, fileId = LekmodVersion.ParseVersionsJson(body)
	if latest == nil then
		return LekmodVersion.UnreachableResult("parse-failed")
	end
	return ResultFromLatest(localVersion, latest, fileId, source or "http")
end

-- Undocumented vanilla API (FireWire FHttpRequest in CivilizationV*.exe).
function LekmodVersion.HasHttpRequest()
	return Network ~= nil and type(Network.HttpRequest) == "function"
end

function LekmodVersion.StartHttpVersionsRequest(urlIndex)
	if not LekmodVersion.HasHttpRequest() then
		return nil, nil
	end
	local urls = LekmodVersion.VERSIONS_RAW_URLS
	local index = urlIndex or 1
	if index < 1 or index > #urls then
		return nil, nil
	end
	local url = urls[index]
	local ok, req = pcall(function()
		return Network.HttpRequest(url)
	end)
	if ok and req ~= nil then
		return req, index
	end
	return nil, nil
end

function LekmodVersion.OpenDownload(url)
	local target = url
	if target == nil or target == "" then
		target = LekmodVersion.VERSIONS_PAGE_URL
	end
	-- Steam overlay is flaky with https://; always prefer http:// for ActivateGameOverlayToWebPage.
	if string.sub(target, 1, 8) == "https://" then
		target = "http://" .. string.sub(target, 9)
	elseif string.sub(target, 1, 7) ~= "http://" then
		target = "http://" .. target
	end
	pcall(function()
		Steam.ActivateGameOverlayToWebPage(target)
	end)
end
