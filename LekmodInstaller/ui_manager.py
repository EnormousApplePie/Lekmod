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
        version = latest_folder.replace("LEKMOD_", "")
        
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
        
        # If folder exists but no version found
        if lekmod_path:
            return "Unknown Version"
            
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
        """Configure UI files based on detected UI"""
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
        
        # Clear existing UI files
        if os.path.exists(ui_dest):
            shutil.rmtree(ui_dest)
        os.makedirs(ui_dest)
        
        # Determine source folder based on UI type
        if "Enhanced UI" in ui_type:
            source_folder = "eui"
            log_callback(f"✓ Configuring for {ui_type}")
        else:
            source_folder = "ui"
            log_callback(f"✓ Configuring for Standard UI")
            
        # Copy files from tmp folder
        tmp_path = os.path.join(lekmod_path, "Lua", "tmp", source_folder)
        
        if not os.path.exists(tmp_path):
            raise Exception(f"UI source folder not found: {tmp_path}")
            
        # Walk through all UI files and copy them
        file_count = 0
        for root, dirs, files in os.walk(tmp_path):
            for file in files:
                if file.endswith('.ignore') or file.endswith('.IGNORE'):
                    source_file = os.path.join(root, file)
                    
                    # Calculate relative path
                    rel_path = os.path.relpath(source_file, tmp_path)
                    dest_file = os.path.join(ui_dest, rel_path)
                    
                    # Remove .ignore extension
                    dest_file = dest_file.replace('.ignore', '').replace('.IGNORE', '')
                    
                    # Create destination directory
                    os.makedirs(os.path.dirname(dest_file), exist_ok=True)
                    
                    # Copy file
                    shutil.copy2(source_file, dest_file)
                    file_count += 1
                    
                    if file_count % 10 == 0:
                        log_callback(f"Configured {file_count} UI files...")
                    
        log_callback(f"✓ Configured {file_count} UI files for {source_folder}")
        
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
        
        # Backup existing installation
        if os.path.exists(dlc_dest):
            backup_path = dlc_dest + ".backup"
            log_callback(f"Creating backup of existing installation...")
            
            if os.path.exists(backup_path):
                shutil.rmtree(backup_path)
            shutil.move(dlc_dest, backup_path)
            log_callback(f"✓ Backup created at: {backup_path}")
            
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

