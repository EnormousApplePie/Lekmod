"""
UI Manager - Handles UI detection and file configuration
"""
import os
import shutil
import zipfile
from pathlib import Path

try:
    import winreg
except ImportError:
    winreg = None  # Not on Windows

class UIManager:
    def __init__(self):
        self.civ5_path = None
        
    def find_civ5_installation(self):
        """Find Civ 5 installation path"""
        # Check common locations
        common_paths = [
            r"C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V",
            r"C:\Program Files\Steam\steamapps\common\Sid Meier's Civilization V",
            r"D:\Steam\steamapps\common\Sid Meier's Civilization V",
            r"E:\Steam\steamapps\common\Sid Meier's Civilization V",
        ]
        
        for path in common_paths:
            if os.path.exists(path):
                self.civ5_path = path
                return path
        
        # Check Steam registry (Windows only)
        if winreg:
            try:
                key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 
                                    r"SOFTWARE\WOW6432Node\Valve\Steam")
                steam_path, _ = winreg.QueryValueEx(key, "InstallPath")
                winreg.CloseKey(key)
                
                civ5_path = os.path.join(steam_path, 
                                        "steamapps", "common", 
                                        "Sid Meier's Civilization V")
                if os.path.exists(civ5_path):
                    self.civ5_path = civ5_path
                    return civ5_path
            except:
                pass
        
        # Check user documents (for non-Steam versions)
        try:
            docs_path = Path.home() / "Documents" / "My Games" / "Sid Meier's Civilization 5"
            if docs_path.exists():
                # Try to find installation from here
                pass
        except:
            pass
            
        return None
        
    def check_eui_installed(self, civ5_path):
        """Check if EUI is installed in Assets/DLC (for informational purposes)"""
        if not civ5_path:
            return False
            
        # DLC folder is in the same directory as Civ5.exe
        dlc_path = os.path.join(civ5_path, "Assets", "DLC")
        
        if not os.path.exists(dlc_path):
            return False
        
        # Check for EUI folders in Assets/DLC
        eui_folders = ["UI_bc1", "UI_bc1_xits"]
        
        for folder in eui_folders:
            folder_path = os.path.join(dlc_path, folder)
            if os.path.exists(folder_path):
                return True
                
        return False
        
    def get_current_lekmod_version(self, civ5_path):
        """Get currently installed Lekmod version (from Assets/DLC)"""
        if not civ5_path:
            return None
            
        dlc_path = os.path.join(civ5_path, "Assets", "DLC")
        
        if not os.path.exists(dlc_path):
            return None
        
        # Look for LEKMOD_* folders
        lekmod_folders = []
        try:
            for item in os.listdir(dlc_path):
                if item.startswith("LEKMOD_"):
                    lekmod_folders.append(item)
        except:
            return None
        
        # If multiple versions, get the newest
        if not lekmod_folders:
            return None
        
        # Return the version from the folder name (e.g., LEKMOD_v34.10 -> v34.10)
        latest_folder = sorted(lekmod_folders)[-1]  # Simple alphabetical sort
        version_from_folder = latest_folder.replace("LEKMOD_", "")
        
        lekmod_path = os.path.join(dlc_path, latest_folder)
        
        # Check various version indicators
        version_files = [
            os.path.join(lekmod_path, "version.txt"),
            os.path.join(lekmod_path, "VERSION"),
            os.path.join(lekmod_path, "Lekmod (v 1).modinfo"),
            os.path.join(lekmod_path, "LEKMOD.modinfo"),
        ]
        
        for version_file in version_files:
            if os.path.exists(version_file):
                try:
                    with open(version_file, 'r', encoding='utf-8') as f:
                        content = f.read()
                        # Try to extract version number
                        import re
                        match = re.search(r'v?\d+\.\d+', content)
                        if match:
                            return match.group(0)
                except:
                    pass
        
        # If no version file found, return version from folder name
        if version_from_folder:
            return version_from_folder
            
        return None
        
    def extract_mod(self, zip_path, log_callback):
        """Extract downloaded mod"""
        extract_dir = os.path.join(os.path.dirname(zip_path), "lekmod_temp")
        
        if os.path.exists(extract_dir):
            shutil.rmtree(extract_dir)
        os.makedirs(extract_dir)
            
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            # Get total file count for progress
            total_files = len(zip_ref.namelist())
            extracted = 0
            
            for file in zip_ref.namelist():
                zip_ref.extract(file, extract_dir)
                extracted += 1
                if extracted % 100 == 0 or extracted == total_files:
                    log_callback(f"Extracting... {extracted}/{total_files} files")
            
        return extract_dir
        
    def configure_ui_files(self, mod_path, ui_type, log_callback):
        """Configure UI files based on UI type (replicates ui_check.bat logic)"""
        # Find LEKMOD folder (supports LEKMOD or LEKMOD_vX.X naming)
        lekmod_path = None
        for root, dirs, files in os.walk(mod_path):
            for dir_name in dirs:
                if dir_name.startswith('LEKMOD'):
                    lekmod_path = os.path.join(root, dir_name)
                    break
            if lekmod_path:
                break
        
        if not lekmod_path or not os.path.exists(lekmod_path):
            raise Exception(f"LEKMOD folder not found in extracted files")
        
        ui_dest = os.path.join(lekmod_path, "Lua", "UI")
        tmp_ui = os.path.join(lekmod_path, "Lua", "tmp", "ui")
        tmp_eui = os.path.join(lekmod_path, "Lua", "tmp", "eui")
        
        # Clear existing UI files
        if os.path.exists(ui_dest):
            shutil.rmtree(ui_dest)
        os.makedirs(ui_dest)
        
        is_eui = "Enhanced UI" in ui_type
        log_callback(f"✓ Configuring for {'EUI' if is_eui else 'Standard UI'}")
        
        file_count = 0
        
        # Files ALWAYS copied from tmp/ui (regardless of EUI)
        always_ui_files = [
            "AncientRuins/GoodyHutPopup.lua",
            "Core/IconSupport.lua",
            "CultureOverview/CultureOverview.lua",
            "CultureOverview/CultureOverview.xml",
            "FrontEnd/EULA.lua",
            "FrontEnd/FrontEnd.lua",  # Main menu!
            "GPList/GPList.lua",
            "GPList/GPList.xml",
            "LeaderHead/SimpleDiploTrade.lua",
            "LeaderHead/SimpleDiploTrade.xml",
            "Lobby/AdvancedSetup.lua",
            "Lobby/JoiningRoom.lua",
            "Lobby/JoiningRoom.xml",
            "Lobby/LoadMenu.lua",
            "Lobby/Lobby.lua",
            "Lobby/Lobby.xml",
            "Lobby/MPGameOptions.lua",
            "Lobby/MPGameSetupScreen.lua",
            "Lobby/MPGameSetupScreen.xml",
            "Lobby/StagingRoom.lua",
            "Lobby/StagingRoom.xml",
            "PlotMouseoverInclude.lua",
            "MiniMapPanel/MiniMapPanel.lua",
            "MiniMapPanel/MiniMapPanel.xml",
            "MPTurnPanel/MPTurnPanel.lua",
            "MPTurnPanel/MPTurnPanel.xml",
            "Replays/Demographics.lua",
            "Replays/GameMenu.lua",
            "Replays/ReplayViewer.lua",
            "Replays/ReplayViewer.xml",
            "VotingSystem/CCVotePopup.lua",
            "VotingSystem/CCVotePopup.xml",
            "VotingSystem/NetworkKickedPopup.lua",
            "VotingSystem/ProposalChartPopup.lua",
            "VotingSystem/ProposalChartPopup.xml",
            "VotingSystem/VictoryProgress.lua",
            "VotingSystem/VictoryProgress.xml",
            "Bombardment.lua",
            "DiscussionDialog.lua",
            "EndGameMenu.lua",
            "Highlights.xml",
            "InGame.lua",
            "mountain.lua",
            "prophetreplace.lua",
            "UnitList/UnitList.lua",
            "UnitList/UnitList.xml",
            "UnitPanel/EnemyUnitPanel.lua",
        ]
        
        # Copy always-ui files
        log_callback(f"Copying common UI files...")
        for rel_path in always_ui_files:
            source_file = os.path.join(tmp_ui, rel_path + ".ignore")
            if not os.path.exists(source_file):
                source_file = os.path.join(tmp_ui, rel_path + ".IGNORE")
            
            if os.path.exists(source_file):
                dest_file = os.path.join(ui_dest, rel_path)
                os.makedirs(os.path.dirname(dest_file), exist_ok=True)
                shutil.copy2(source_file, dest_file)
                file_count += 1
        
        # Conditional files (depends on EUI)
        conditional_files = {
            # Format: "destination": ("ui_source", "eui_source")
            "CityBannerManager.lua": ("ui/CityBanners/CityBannerManager.lua", "eui/CityBanners/CityBannerManager_2.lua"),
            "CityBannerManager.xml": ("ui/CityBanners/CityBannerManager.xml", "eui/CityBanners/CityBannerManager_2.xml"),
            "CityStateDiploPopup.lua": ("ui/CityStatePopup/CityStateDiploPopup.lua", "eui/CityStatePopup/CityStateDiploPopup.lua"),
            "CityView.lua": ("ui/CityView/CityView.lua", "eui/CityView/CityView.lua"),
            "CityView.xml": ("ui/CityView/CityView.xml", "eui/CityView/CityView.xml"),
            "CityView_small.xml": ("ui/CityView/CityView_small.xml", "eui/CityView/CityView_small.xml"),
            "ProductionPopup.lua": ("ui/CityView/ProductionPopup.lua", None),
            "ProductionPopup.xml": ("ui/CityView/ProductionPopup.xml", None),
            "CityStateStatusHelper.lua": ("ui/Core/CityStateStatusHelper.lua", None),
            "EUI_tooltip_library.lua": (None, "eui/Core/EUI_tooltip_library.lua"),
            "EUI_unit_include.lua": (None, "eui/Core/EUI_unit_include.lua"),
            "EconomicGeneralInfo.lua": ("ui/CultureOverview/EconomicGeneralInfo.lua", "eui/EconomicGeneralInfo.lua"),
            "EconomicGeneralInfo.xml": ("ui/CultureOverview/EconomicGeneralInfo.xml", "eui/EconomicGeneralInfo.xml"),
            "SelectCivilization.lua": ("ui/GameSetup/SelectCivilization.lua", "eui/GameSetup/SelectCivilization.lua"),
            "WorldView.lua": ("ui/Improvements/WorldView.lua", "eui/Improvements/WorldView.lua"),
            "TradeLogic.lua": ("ui/LeaderHead/TradeLogic.lua", "eui/LeaderHead/TradeLogic.lua"),
            "DiploCorner.lua": ("ui/NotificationPanel/DiploCorner.lua", "eui/NotificationPanel/DiploCorner.lua"),
            "DiploCorner.xml": ("ui/NotificationPanel/DiploCorner.xml", "eui/NotificationPanel/DiploCorner.xml"),
            "NotificationPanel.lua": ("ui/NotificationPanel/NotificationPanel.lua", "eui/NotificationPanel/NotificationPanel.lua"),
            "NotificationPanel.xml": ("ui/NotificationPanel/NotificationPanel.xml", "eui/NotificationPanel/NotificationPanel.xml"),
            "OptionsMenu.lua": ("ui/Options/OptionsMenu.lua", "eui/Options/OptionsMenu.lua"),
            "OptionsMenu.xml": ("ui/Options/OptionsMenu.xml", "eui/Options/OptionsMenu.xml"),
            "TechPopup.lua": ("ui/TechTree/TechPopup.lua", "eui/TechTree/TechPopup.lua"),
            "TechPopup.xml": (None, "eui/TechTree/TechPopup.xml"),
            "TechTree.lua": ("ui/TechTree/TechTree.lua", "eui/TechTree/TechTree.lua"),
            "TechButtonInclude.lua": ("ui/ToolTips/TechButtonInclude.lua", "eui/ToolTips/TechButtonInclude.lua"),
            "InfoTooltipInclude.lua": ("ui/ToolTips/InfoTooltipInclude.lua", "eui/ToolTips/InfoTooltipInclude.lua"),
            "UnitFlagManager.lua": ("ui/UnitFlagManager/UnitFlagManager.lua", "eui/UnitFlagManager/UnitFlagManager.lua"),
            "UnitFlagManager.xml": ("ui/UnitFlagManager/UnitFlagManager.xml", "eui/UnitFlagManager/UnitFlagManager.xml"),
            "UnitPanel.lua": ("ui/UnitPanel/UnitPanel.lua", "eui/UnitPanel/UnitPanel.lua"),
            "YieldIconManager.lua": ("ui/YieldIconManager.lua", "eui/Improvements/YieldIconManager.lua"),
            "TopPanel.lua": ("ui/TopPanel.lua", "eui/TopPanel.lua"),
            "TopPanel.xml": (None, "eui/TopPanel.xml"),
        }
        
        log_callback(f"Copying UI-specific files...")
        for dest_name, (ui_source, eui_source) in conditional_files.items():
            source_path = eui_source if is_eui and eui_source else ui_source
            
            if source_path:
                source_file = os.path.join(lekmod_path, "Lua", "tmp", source_path + ".ignore")
                if not os.path.exists(source_file):
                    source_file = os.path.join(lekmod_path, "Lua", "tmp", source_path + ".IGNORE")
                
                if os.path.exists(source_file):
                    dest_file = os.path.join(ui_dest, dest_name)
                    os.makedirs(os.path.dirname(dest_file), exist_ok=True)
                    shutil.copy2(source_file, dest_file)
                    file_count += 1
        
        log_callback(f"✓ Configured {file_count} total UI files")
        
    def find_existing_lekmod_folders(self, civ5_path):
        """Find any existing LEKMOD installations"""
        dlc_path = os.path.join(civ5_path, "Assets", "DLC")
        
        if not os.path.exists(dlc_path):
            return []
        
        existing_folders = []
        try:
            for item in os.listdir(dlc_path):
                item_path = os.path.join(dlc_path, item)
                if os.path.isdir(item_path) and item.startswith("LEKMOD"):
                    existing_folders.append(item)
        except:
            pass
        
        return existing_folders
    
    def remove_lekmod_folders(self, civ5_path, folders, log_callback):
        """Remove specified LEKMOD folders"""
        dlc_path = os.path.join(civ5_path, "Assets", "DLC")
        
        for folder in folders:
            folder_path = os.path.join(dlc_path, folder)
            if os.path.exists(folder_path):
                log_callback(f"Removing {folder}...")
                shutil.rmtree(folder_path)
                log_callback(f"✓ Removed {folder}")
    
    def install_mod(self, mod_source, civ5_path, version, log_callback):
        """Install mod to Civ 5 Assets/DLC (for multiplayer compatibility)"""
        # Install to Assets/DLC instead of MODS
        dlc_path = os.path.join(civ5_path, "Assets", "DLC")
        
        # Ensure DLC folder exists
        if not os.path.exists(dlc_path):
            os.makedirs(dlc_path)
            log_callback(f"✓ Created DLC folder: {dlc_path}")
        
        # Destination is Assets/DLC/LEKMOD_v34.10 (uses version number)
        folder_name = f"LEKMOD_{version}"
        dlc_dest = os.path.join(dlc_path, folder_name)
            
        # Find LEKMOD source folder in the extracted archive
        # Supports both "LEKMOD" and "LEKMOD_v34.10" folder names
        lekmod_source = None
        for root, dirs, files in os.walk(mod_source):
            for dir_name in dirs:
                if dir_name.startswith('LEKMOD'):
                    lekmod_source = os.path.join(root, dir_name)
                    log_callback(f"Found mod folder: {dir_name}")
                    break
            if lekmod_source:
                break
        
        if not lekmod_source:
            # Try direct path as fallback
            test_paths = [
                os.path.join(mod_source, "LEKMOD"),
                os.path.join(mod_source, f"LEKMOD_{version}")
            ]
            for test_path in test_paths:
                if os.path.exists(test_path):
                    lekmod_source = test_path
                    break
            
        if not lekmod_source or not os.path.exists(lekmod_source):
            raise Exception("LEKMOD source folder not found in downloaded archive!")
        
        # Copy to DLC folder
        log_callback(f"Copying files to {dlc_dest}...")
        shutil.copytree(lekmod_source, dlc_dest)
        log_callback(f"✓ Files copied successfully!")
        
        # Delete ui_check.bat as it's only needed for manual installation
        ui_check_path = os.path.join(dlc_dest, "ui_check.bat")
        if os.path.exists(ui_check_path):
            try:
                os.remove(ui_check_path)
                log_callback(f"✓ Removed ui_check.bat (not needed for installer-based installation)")
            except Exception as e:
                log_callback(f"⚠ Could not remove ui_check.bat: {e}")

