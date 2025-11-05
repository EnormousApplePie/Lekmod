include("InstanceManager")

-- ================= Force hidden on init (avoid auto-show) ===================
if not ContextPtr:IsHidden() then
  ContextPtr:SetHide(true)
end
if Controls.ChooseConfirm then
  Controls.ChooseConfirm:SetHide(true)
end
-- ===========================================================================

-- State
local g_PopupInfo    = nil
local g_PlayerID     = nil
local g_SelectedInst = nil   -- selected card's instance table
local g_SelectedID   = nil   -- integer Legacies.ID selected

-- Instance managers for each column
local g_LeftIM  = InstanceManager:new("LegacyCard", "Button", Controls.LeftStack)
local g_RightIM = InstanceManager:new("LegacyCard", "Button", Controls.RightStack)

-- Utilities ------------------------------------------------------------------
local function L(s) return (s and s ~= "" and Locale) and Locale.ConvertTextKey(s) or (s or "") end
local function GetContextPlayerID() return Game.GetActivePlayer() end

local function GetPlayerAndEra(pid)
  local p = Players[pid]
  if not p then return nil, nil, nil end
  local eraID  = p:GetCurrentEra()
  local eraRow = (eraID and GameInfo.Eras[eraID]) and GameInfo.Eras[eraID] or nil
  local civID  = p:GetCivilizationType()
  local civRow = (civID and GameInfo.Civilizations[civID]) and GameInfo.Civilizations[civID] or nil
  return p, eraRow, civRow
end

-- Safe sizing helpers
local function SafeCalc(ctrl)
  if ctrl and ctrl.CalculateSize then ctrl:CalculateSize() end
  if ctrl and ctrl.ReprocessAnchoring then ctrl:ReprocessAnchoring() end
end
local function SafeScrollCalc(panel)
  if panel and panel.CalculateInternalSize then panel:CalculateInternalSize() end
end

-- Data: Legacies by civ+era ---------------------------------------------------
local function TableExists(name)
  for row in DB.Query("SELECT name FROM sqlite_master WHERE type='table' AND name=?;", name) do
    if row and row.name == name then return true end
  end
  return false
end

-- Legacies(ID, Type, Description, Civilopedia, Strategy, Help, CivilizationType, EraOffered)
local function BuildDataListForCivEra(pid)
  local _, eraRow, civRow = GetPlayerAndEra(pid)
  local eraTypeStr = eraRow and eraRow.Type or nil
  local civTypeStr = civRow and civRow.Type or nil

  local items = {}

  if TableExists("Legacies") and GameInfo.Legacies then
    for row in GameInfo.Legacies() do
      local okEra = true
      if row.EraOffered and row.EraOffered ~= "" then
        okEra = (eraTypeStr ~= nil and row.EraOffered == eraTypeStr)
      end
      local okCiv = true
      if row.CivilizationType and row.CivilizationType ~= "" then
        okCiv = (civTypeStr ~= nil and row.CivilizationType == civTypeStr)
      end

      if okEra and okCiv then
        items[#items+1] = {
          ID      = row.ID,
          Type    = row.Type,
          nameKey = row.Description or row.Type,
          helpKey = row.Help or "",
        }
      end
    end
  end

  if #items == 0 then
    items = {
      { ID=1, Type="LEGACY_A", nameKey="Option #1", helpKey="" },
      { ID=2, Type="LEGACY_B", nameKey="Option #2", helpKey="" },
    }
  end

  return items
end

-- Selection + confirm ---------------------------------------------------------
local function ClearGlow()
  if g_SelectedInst and g_SelectedInst.SelectGlow then
    g_SelectedInst.SelectGlow:SetHide(true)
  end
  g_SelectedInst = nil
end

local function ClearSelection()
  ClearGlow()
  g_SelectedID = nil
end

-- Clicked a card → open confirm overlay
function SelectLegacyChoice(choiceID)
  g_SelectedID = choiceID

  local txt = "Confirm?"
  if GameInfo.Legacies and choiceID then
    local row = GameInfo.Legacies[choiceID]
    if row and row.Description then
      local name = L(row.Description)
      if Locale and Locale.HasTextKey and Locale.HasTextKey("TXT_KEY_CHOOSE_LEGACY_CONFIRM_FMT") then
        txt = Locale.ConvertTextKey("TXT_KEY_CHOOSE_LEGACY_CONFIRM_FMT", name)
      else
        txt = "Adopt " .. name .. "?"
      end
    end
  end
  if Controls.ConfirmText   then Controls.ConfirmText:SetText(txt) end
  if Controls.ChooseConfirm then Controls.ChooseConfirm:SetHide(false) end
end

-- Yes/No handlers
local function OnConfirmYes()
  if not g_SelectedID then return end
  local pid = g_PlayerID or Game.GetActivePlayer()

  -- hijack ideology path: (ID + 10) * -1
  local offsetID = (g_SelectedID + 10) * -1
  Network.SendIdeologyChoice(pid, offsetID)

  print(string.format("Legacy chosen (raw=%d, encoded=%d) by player %d",
        g_SelectedID, offsetID, pid or -1))

  UIManager:DequeuePopup(ContextPtr)
end

local function OnConfirmNo()
  if Controls.ChooseConfirm then Controls.ChooseConfirm:SetHide(true) end
  ClearSelection()   -- remove highlight + selection when user cancels
end

if Controls.ConfirmYes then Controls.ConfirmYes:RegisterCallback(Mouse.eLClick, OnConfirmYes) end
if Controls.ConfirmNo  then Controls.ConfirmNo:RegisterCallback(Mouse.eLClick, OnConfirmNo)   end

-- Build the 2-column grid -----------------------------------------------------
local function PopulateLegacyCards_ByCivEra(pid)
  g_LeftIM:ResetInstances()
  g_RightIM:ResetInstances()
  ClearSelection()

  local items = BuildDataListForCivEra(pid)

  for i = 1, #items do
    local item = items[i]
    local im   = (i % 2 == 1) and g_LeftIM or g_RightIM
    local inst = im:GetInstance()

    inst.Name:SetText(L(item.nameKey or item.Type))
    inst.Help:SetText(L(item.helpKey or ""))

    local thisID = item.ID or i
    inst.Button:RegisterCallback(Mouse.eLClick, function()
      if g_SelectedInst and g_SelectedInst ~= inst and g_SelectedInst.SelectGlow then
        g_SelectedInst.SelectGlow:SetHide(true)
      end
      inst.SelectGlow:SetHide(false)
      g_SelectedInst = inst
      SelectLegacyChoice(thisID)
    end)
  end

  SafeCalc(Controls.LeftStack)
  SafeCalc(Controls.RightStack)
  SafeCalc(Controls.CardColumns)
  SafeScrollCalc(Controls.LegacyScrollPanel)
end

-- Popup lifecycle -------------------------------------------------------------
local function ClosePopup()
  UIManager:DequeuePopup(ContextPtr)
end

ContextPtr:SetShowHideHandler(function(bIsHide)
  if not bIsHide then
    if g_PopupInfo then
      Events.SerialEventGameMessagePopupShown(g_PopupInfo)
    end
  else
    -- when closing, also clear any highlight
    ClearSelection()
    Events.SerialEventGameMessagePopupProcessed.CallImmediate(ButtonPopupTypes.BUTTONPOPUP_MODDER_1, 0)
  end
end)

ContextPtr:SetInputHandler(function(uiMsg, wParam)
  if uiMsg == KeyEvents.KeyDown and wParam == Keys.VK_ESCAPE then
    if Controls.ChooseConfirm and not Controls.ChooseConfirm:IsHidden() then
      Controls.ChooseConfirm:SetHide(true)
      ClearSelection()
    else
      ClosePopup()
    end
    return true
  end
end)

-- Entry point -----------------------------------------------------------------
local function OnPopupMessage(popupInfo)
  if popupInfo.Type ~= ButtonPopupTypes.BUTTONPOPUP_MODDER_1 then return end

  g_PopupInfo = popupInfo
  g_PlayerID  = GetContextPlayerID()

  if Controls.TitleLabel then
    if Locale and Locale.HasTextKey and Locale.HasTextKey("TXT_KEY_CHOOSE_LEGACY_TITLE") then
      Controls.TitleLabel:SetText(Locale.ConvertTextKey("TXT_KEY_CHOOSE_LEGACY_TITLE"))
    else
      Controls.TitleLabel:SetText("Choose a Legacy")
    end
  end
  if Controls.DescriptionLabel then
    Controls.DescriptionLabel:SetText("")
  end

  PopulateLegacyCards_ByCivEra(g_PlayerID)
  UIManager:QueuePopup(ContextPtr, PopupPriority.SocialPolicy)
end
Events.SerialEventGameMessagePopup.Add(OnPopupMessage)
