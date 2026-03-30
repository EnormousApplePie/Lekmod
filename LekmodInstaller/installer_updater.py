"""
Installer Self-Updater - Allows the installer to update itself
"""
import requests
import os
import sys
import subprocess
import shutil
from pathlib import Path
import tempfile

class InstallerUpdater:
    def __init__(self, config):
        self.config = config
        self.current_version = config.get('installer_version', '1.0.0')
        self.update_url = config.get('installer_update_url')
        
    def check_for_installer_update(self):
        """Check if there's a newer version of the installer available"""
        if not self.update_url:
            return None
            
        try:
            response = requests.get(self.update_url, timeout=10)
            if response.status_code == 200:
                update_info = response.json()
                latest_version = update_info.get('version')
                
                if latest_version and self._is_newer_version(latest_version, self.current_version):
                    return update_info
                    
        except Exception as e:
            print(f"Failed to check for installer updates: {e}")
            
        return None
    
    def _is_newer_version(self, new_version, current_version):
        """Compare version strings (e.g., 1.2.0 vs 1.1.0)"""
        def parse_version(v):
            return tuple(int(x) for x in v.split('.') if x.isdigit())
        
        try:
            new_parts = parse_version(new_version)
            current_parts = parse_version(current_version)
            return new_parts > current_parts
        except:
            return False
    
    def download_and_update(self, update_info, log_callback):
        """Download new installer and replace current one"""
        download_url = update_info.get('download_url')
        if not download_url:
            raise Exception("No download URL in update info")
        
        log_callback("Downloading new installer version...")
        
        # Download to temp file
        temp_dir = tempfile.gettempdir()
        temp_installer = os.path.join(temp_dir, "LekmodInstaller_new.exe")
        
        response = requests.get(download_url, stream=True)
        total_size = int(response.headers.get('content-length', 0))
        downloaded = 0
        
        with open(temp_installer, 'wb') as f:
            for chunk in response.iter_content(chunk_size=8192):
                if chunk:
                    f.write(chunk)
                    downloaded += len(chunk)
                    if total_size > 0:
                        progress = (downloaded / total_size) * 100
                        log_callback(f"Downloading... {progress:.1f}%")
        
        log_callback("✓ Download complete!")
        
        # Create update script
        current_exe = sys.executable if getattr(sys, 'frozen', False) else __file__
        update_script = self._create_update_script(current_exe, temp_installer)
        
        log_callback("Preparing to update installer...")
        log_callback("The installer will restart after update.")
        
        return update_script, temp_installer
    
    def _create_update_script(self, current_exe, new_exe):
        """Create a batch script to replace the installer and restart it"""
        script_path = os.path.join(tempfile.gettempdir(), "update_installer.bat")
        
        script_content = f"""@echo off
echo Updating Lekmod Installer...
timeout /t 2 /nobreak >nul

:retry
del /f /q "{current_exe}"
if exist "{current_exe}" (
    timeout /t 1 /nobreak >nul
    goto retry
)

move /y "{new_exe}" "{current_exe}"

echo Update complete! Restarting installer...
timeout /t 1 /nobreak >nul

start "" "{current_exe}"
del "%~f0"
"""
        
        with open(script_path, 'w') as f:
            f.write(script_content)
        
        return script_path
    
    def apply_update(self, update_script):
        """Execute update script and exit current installer"""
        subprocess.Popen([update_script], shell=True)
        sys.exit(0)

