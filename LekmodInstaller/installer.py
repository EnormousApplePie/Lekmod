"""
Lekmod Installer & Updater
Main application with GUI
"""
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext, filedialog
import threading
import json
import os
from pathlib import Path
from updater import UpdateChecker
from ui_manager import UIManager
from google_drive_api import GoogleDriveDownloader
from installer_updater import InstallerUpdater

class LekmodInstaller:
    def __init__(self, root):
        self.root = root
        self.root.title("Lekmod Installer & Updater v1.0")
        self.root.geometry("650x600")
        self.root.resizable(False, False)
        
        # Load config
        config_path = Path(__file__).parent / 'config.json'
        with open(config_path, 'r') as f:
            self.config = json.load(f)
        
        # Initialize managers
        self.updater = UpdateChecker(self.config)
        self.ui_manager = UIManager()
        self.downloader = GoogleDriveDownloader(self.config)
        self.installer_updater = InstallerUpdater(self.config)
        
        # State
        self.use_eui = tk.BooleanVar(value=False)
        
        self.setup_ui()
        self.check_installer_updates()  # Check on startup
        self.detect_installation()
        
    def setup_ui(self):
        # Header
        header = tk.Frame(self.root, bg="#2c3e50", height=80)
        header.pack(fill=tk.X)
        
        title = tk.Label(header, text="LEKMOD", font=("Arial", 24, "bold"),
                        bg="#2c3e50", fg="white")
        title.pack(pady=10)
        
        version_text = f"v{self.config.get('installer_version', '1.0.0')}"
        subtitle = tk.Label(header, text=f"Installer & Updater - {version_text}", 
                           font=("Arial", 12),
                           bg="#2c3e50", fg="#ecf0f1")
        subtitle.pack()
        
        # Main content
        content = tk.Frame(self.root, padx=20, pady=20)
        content.pack(fill=tk.BOTH, expand=True)
        
        # Installation info
        info_frame = tk.LabelFrame(content, text="Civilization V Installation Path", padx=10, pady=10)
        info_frame.pack(fill=tk.X, pady=(0, 10))
        
        # Path entry with label
        path_label_frame = tk.Frame(info_frame)
        path_label_frame.pack(fill=tk.X, pady=(0, 5))
        
        tk.Label(path_label_frame, text="Installation Path:", anchor=tk.W).pack(side=tk.LEFT)
        
        self.status_label = tk.Label(path_label_frame, text="", 
                                     anchor=tk.W, fg="#27ae60", font=("Arial", 9))
        self.status_label.pack(side=tk.LEFT, padx=(10, 0))
        
        # Path entry field
        path_entry_frame = tk.Frame(info_frame)
        path_entry_frame.pack(fill=tk.X, pady=(0, 5))
        
        self.install_path_var = tk.StringVar()
        self.install_path_var.set(r"C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V")
        
        self.install_path_entry = tk.Entry(path_entry_frame, 
                                           textvariable=self.install_path_var,
                                           font=("Arial", 9))
        self.install_path_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        
        tk.Button(path_entry_frame, text="Browse...", 
                 command=self.browse_civ5_path,
                 bg="#95a5a6", fg="white",
                 width=10).pack(side=tk.LEFT, padx=(5, 0))
        
        # Verify button
        tk.Button(path_entry_frame, text="Verify", 
                 command=self.verify_civ5_path,
                 bg="#3498db", fg="white",
                 width=8).pack(side=tk.LEFT, padx=(5, 0))
        
        self.version_label = tk.Label(info_frame, text="Version: Checking...", 
                                      anchor=tk.W)
        self.version_label.pack(fill=tk.X, pady=(5, 0))
        
        # UI Type Selection
        ui_frame = tk.LabelFrame(content, text="User Interface Configuration", 
                                padx=10, pady=10)
        ui_frame.pack(fill=tk.X, pady=(0, 10))
        
        # Simple checkbox for EUI
        eui_cb = tk.Checkbutton(ui_frame, 
                               text="Install for Enhanced UI (EUI)",
                               variable=self.use_eui,
                               font=("Arial", 10))
        eui_cb.pack(anchor=tk.W)
        
        # Info label
        info_label = tk.Label(ui_frame, 
                             text="Check this box if you have EUI (UI_bc1 or UI_bc1_xits) installed",
                             font=("Arial", 8),
                             fg="#7f8c8d",
                             anchor=tk.W)
        info_label.pack(anchor=tk.W, pady=(2, 0))
        
        # Optional: Show if EUI is detected
        self.eui_detected_label = tk.Label(ui_frame, text="", 
                                          anchor=tk.W, fg="#3498db",
                                          font=("Arial", 8, "italic"))
        self.eui_detected_label.pack(anchor=tk.W, pady=(5, 0))
        
        # Version selection
        version_frame = tk.LabelFrame(content, text="Select Version", padx=10, pady=10)
        version_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.version_var = tk.StringVar()
        self.version_combo = ttk.Combobox(version_frame, textvariable=self.version_var,
                                          state="readonly", width=50)
        self.version_combo.pack(fill=tk.X)
        
        # Buttons
        button_frame = tk.Frame(content)
        button_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.install_btn = tk.Button(button_frame, text="Install/Update", 
                                     command=self.install_update,
                                     bg="#27ae60", fg="white", 
                                     font=("Arial", 12, "bold"),
                                     height=2)
        self.install_btn.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        self.refresh_btn = tk.Button(button_frame, text="Refresh Versions",
                                     command=self.check_updates,
                                     bg="#3498db", fg="white",
                                     font=("Arial", 12, "bold"),
                                     height=2)
        self.refresh_btn.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(5, 0))
        
        # Update Installer button (smaller, below main buttons)
        update_installer_frame = tk.Frame(content)
        update_installer_frame.pack(fill=tk.X, pady=(5, 5))
        
        self.update_installer_btn = tk.Button(update_installer_frame, 
                                              text="Update Installer",
                                              command=self.update_installer,
                                              bg="#e67e22", fg="white",
                                              font=("Arial", 10))
        self.update_installer_btn.pack(side=tk.RIGHT)
        
        # Progress
        self.progress = ttk.Progressbar(content, mode='indeterminate')
        self.progress.pack(fill=tk.X, pady=(0, 10))
        
        # Log
        log_frame = tk.LabelFrame(content, text="Installation Log", padx=5, pady=5)
        log_frame.pack(fill=tk.BOTH, expand=True)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=8, 
                                                  state=tk.DISABLED,
                                                  wrap=tk.WORD,
                                                  bg="#f8f9fa")
        self.log_text.pack(fill=tk.BOTH, expand=True)
        
    def check_eui_presence(self):
        """Check if EUI is installed and show info"""
        civ5_path = self.install_path_var.get()
        if civ5_path and os.path.exists(civ5_path):
            has_eui = self.ui_manager.check_eui_installed(civ5_path)
            if has_eui:
                self.eui_detected_label.config(text="ℹ EUI detected in DLC folder")
                # Auto-check the box if EUI is found
                self.use_eui.set(True)
            else:
                self.eui_detected_label.config(text="")
            
    def browse_civ5_path(self):
        """Browse for Civ 5 installation manually"""
        initial_dir = self.install_path_var.get() if os.path.exists(self.install_path_var.get()) else "C:\\"
        path = filedialog.askdirectory(
            title="Select Civilization V Installation Folder",
            initialdir=initial_dir
        )
        if path:
            self.install_path_var.set(path)
            self.verify_civ5_path()
    
    def verify_civ5_path(self):
        """Verify the entered path is valid"""
        path = self.install_path_var.get()
        
        if not path:
            self.status_label.config(text="⚠ Please enter a path", fg="#e74c3c")
            return
        
        if not os.path.exists(path):
            self.status_label.config(text="✗ Path does not exist", fg="#e74c3c")
            self.log(f"⚠ Invalid path: {path}")
            return
        
        # Check if it looks like a Civ 5 installation
        indicators = [
            "CivilizationV.exe",
            "CivilizationV_DX11.exe",
            "Assets"
        ]
        
        is_valid = any(os.path.exists(os.path.join(path, indicator)) for indicator in indicators)
        
        if is_valid:
            self.ui_manager.civ5_path = path
            self.status_label.config(text="✓ Valid installation found", fg="#27ae60")
            self.log(f"✓ Verified Civ 5 installation: {path}")
            self.check_eui_presence()
            self.check_installed_version()
        else:
            self.status_label.config(text="⚠ Not a valid Civ 5 folder", fg="#e67e22")
            self.log(f"⚠ Path exists but doesn't look like Civ 5: {path}")
            
            # Ask if they want to use it anyway
            use_anyway = messagebox.askyesno(
                "Invalid Path?",
                f"The selected folder doesn't appear to be a Civilization V installation.\n\n"
                f"Path: {path}\n\n"
                f"Use it anyway?"
            )
            if use_anyway:
                self.ui_manager.civ5_path = path
                self.status_label.config(text="⚠ Using unverified path", fg="#e67e22")
                self.check_eui_presence()
                self.check_installed_version()
            
    def log(self, message):
        """Add message to log"""
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, f"{message}\n")
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)
        self.root.update_idletasks()
    
    def check_installer_updates(self):
        """Check for installer updates on startup (silent)"""
        threading.Thread(target=self._check_installer_updates_thread, 
                        daemon=True).start()
    
    def _check_installer_updates_thread(self):
        """Check for installer updates in background"""
        try:
            update_info = self.installer_updater.check_for_installer_update()
            if update_info:
                new_version = update_info.get('version')
                # Show notification in log
                self.log(f"")
                self.log(f"🔔 Installer update available: v{new_version}")
                self.log(f"Click 'Update Installer' button to update")
                
                # Make button more prominent
                self.update_installer_btn.config(bg="#e74c3c", 
                                                text=f"Update Installer (v{new_version} available!)")
        except Exception as e:
            # Silently fail - don't bother user with update check failures
            pass
    
    def update_installer(self):
        """Update the installer itself"""
        self.log("Checking for installer updates...")
        self.update_installer_btn.config(state=tk.DISABLED)
        threading.Thread(target=self._update_installer_thread, 
                        daemon=True).start()
    
    def _update_installer_thread(self):
        """Update installer in background"""
        try:
            # Check for updates
            update_info = self.installer_updater.check_for_installer_update()
            
            if not update_info:
                self.log("✓ Installer is up to date!")
                messagebox.showinfo("No Updates", 
                                   "You're already using the latest version of the installer!")
                self.update_installer_btn.config(state=tk.NORMAL)
                return
            
            new_version = update_info.get('version')
            changelog = update_info.get('changelog', 'No changelog available')
            
            # Confirm update
            confirm = messagebox.askyesno(
                "Update Available",
                f"New installer version available: v{new_version}\n\n"
                f"Current version: v{self.config.get('installer_version', '1.0.0')}\n\n"
                f"Changes:\n{changelog}\n\n"
                f"Update now?\n\n"
                f"The installer will restart after updating."
            )
            
            if not confirm:
                self.update_installer_btn.config(state=tk.NORMAL)
                return
            
            # Download and prepare update
            update_script, temp_installer = self.installer_updater.download_and_update(
                update_info, self.log
            )
            
            self.log("✓ Update ready!")
            self.log("Restarting installer...")
            
            # Small delay for user to see message
            import time
            time.sleep(1)
            
            # Apply update (this will exit the app)
            self.installer_updater.apply_update(update_script)
            
        except Exception as e:
            self.log(f"❌ Installer update failed: {e}")
            messagebox.showerror("Update Failed", 
                               f"Failed to update installer:\n\n{str(e)}")
            self.update_installer_btn.config(state=tk.NORMAL)
        
    def detect_installation(self):
        """Detect Civ 5 and Lekmod installation"""
        threading.Thread(target=self._detect_installation_thread, 
                        daemon=True).start()
        
    def _detect_installation_thread(self):
        self.log("Detecting Civilization V installation...")
        
        # Find Civ 5 installation
        civ5_path = self.ui_manager.find_civ5_installation()
        if civ5_path:
            self.install_path_var.set(civ5_path)
            self.status_label.config(text="✓ Auto-detected", fg="#27ae60")
            self.log(f"✓ Found Civilization V at: {civ5_path}")
        else:
            # Default to common Steam path
            default_path = r"C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V"
            self.install_path_var.set(default_path)
            
            # Check if default path exists
            if os.path.exists(default_path):
                self.status_label.config(text="✓ Using default path", fg="#3498db")
                self.log(f"Using default Steam path: {default_path}")
                self.ui_manager.civ5_path = default_path
            else:
                self.status_label.config(text="⚠ Not detected - verify path", fg="#e67e22")
                self.log("⚠ Civilization V not auto-detected.")
                self.log(f"Please verify the installation path or click 'Browse...'")
        
        # Check for EUI
        self.check_eui_presence()
        
        # Check current version
        self.check_installed_version()
            
        # Load available versions
        self.check_updates()
    
    def check_installed_version(self):
        """Check what version of Lekmod is currently installed"""
        civ5_path = self.install_path_var.get()
        current_version = self.ui_manager.get_current_lekmod_version(civ5_path)
        if current_version:
            self.version_label.config(text=f"Installed: Lekmod {current_version}")
            self.log(f"✓ Lekmod {current_version} currently installed")
        else:
            self.version_label.config(text="Lekmod: Not installed")
            self.log("ℹ Lekmod not currently installed")
        
        
    def check_updates(self):
        """Check for available versions from Google Drive"""
        self.log("Checking Google Drive for available versions...")
        self.progress.start()
        self.refresh_btn.config(state=tk.DISABLED)
        threading.Thread(target=self._check_updates_thread, 
                        daemon=True).start()
        
    def _check_updates_thread(self):
        try:
            # Get versions from Google Drive
            versions = self.updater.get_available_versions()
            
            if versions:
                # Format version list with dates and sizes
                version_display = []
                for v in versions:
                    info = versions[v]
                    date = info.get('release_date', 'Unknown date')
                    size = info.get('size', 'Unknown size')
                    version_display.append(f"{v} - {date} ({size})")
                
                self.version_combo['values'] = version_display
                if version_display:
                    self.version_combo.current(0)
                self.log(f"✓ Found {len(versions)} versions on Google Drive")
            else:
                self.log("⚠ No versions found on Google Drive")
                messagebox.showwarning("No Versions", 
                                      "Could not find any versions.\n"
                                      "Please check your internet connection\n"
                                      "and Google Drive configuration.")
        except Exception as e:
            self.log(f"✗ Error checking updates: {e}")
            messagebox.showerror("Update Check Failed", 
                               f"Failed to check for updates:\n{str(e)}\n\n"
                               f"Please check your internet connection.")
        finally:
            self.progress.stop()
            self.refresh_btn.config(state=tk.NORMAL)
            
    def install_update(self):
        """Install or update Lekmod"""
        selected_version_display = self.version_var.get()
        if not selected_version_display:
            messagebox.showwarning("No Version", 
                                  "Please select a version to install")
            return
            
        # Extract version number (format: "v35.2 - 2025-01-15 (150 MB)")
        selected_version = selected_version_display.split(' - ')[0]
        
        # Get UI type
        ui_type = "Enhanced UI (EUI)" if self.use_eui.get() else "Standard UI"
            
        # Confirm installation
        confirm = messagebox.askyesno(
            "Confirm Installation",
            f"Install Lekmod {selected_version}\n"
            f"UI Mode: {ui_type}\n\n"
            f"This will replace any existing installation.\n"
            f"Continue?"
        )
        
        if not confirm:
            return
            
        self.log(f"Starting installation of Lekmod {selected_version}...")
        self.log(f"UI Mode: {ui_type}")
        self.install_btn.config(state=tk.DISABLED)
        self.refresh_btn.config(state=tk.DISABLED)
        self.progress.start()
        
        threading.Thread(target=self._install_thread, 
                        args=(selected_version, ui_type),
                        daemon=True).start()
        
    def _install_thread(self, version, ui_type):
        try:
            # Get and verify path
            civ5_path = self.install_path_var.get()
            if not civ5_path or not os.path.exists(civ5_path):
                raise Exception("Invalid Civilization V path! Please verify the installation path.")
            
            self.ui_manager.civ5_path = civ5_path
            
            # 1. Download from Google Drive
            self.log(f"📥 Downloading Lekmod {version} from Google Drive...")
            download_path = self.downloader.download_version(version, self.log)
            
            # 2. Extract
            self.log("📦 Extracting files...")
            extract_path = self.ui_manager.extract_mod(download_path, self.log)
            
            # 3. Configure UI files
            self.log(f"⚙️ Configuring for {ui_type}...")
            self.ui_manager.configure_ui_files(extract_path, ui_type, self.log)
            
            # 4. Install to Civ 5
            self.log(f"📂 Installing to Civilization V as LEKMOD_{version}...")
            self.ui_manager.install_mod(extract_path, civ5_path, version, self.log)
            
            # 5. Cleanup
            self.log("🧹 Cleaning up temporary files...")
            import shutil
            try:
                os.remove(download_path)
                shutil.rmtree(extract_path)
            except:
                pass
            
            self.log("✅ Installation complete!")
            messagebox.showinfo("Success", 
                               f"Lekmod {version} installed successfully!\n\n"
                               f"UI Mode: {ui_type}\n"
                               f"Location: {civ5_path}/Assets/DLC/LEKMOD_{version}")
            
        except Exception as e:
            self.log(f"❌ Installation failed: {e}")
            messagebox.showerror("Installation Failed", 
                               f"Installation failed:\n\n{str(e)}\n\n"
                               f"Please check the log for details.")
        finally:
            self.progress.stop()
            self.install_btn.config(state=tk.NORMAL)
            self.refresh_btn.config(state=tk.NORMAL)
            self.detect_installation()

def main():
    root = tk.Tk()
    
    # Set icon if available
    icon_path = Path(__file__).parent / 'icon.ico'
    if icon_path.exists():
        root.iconbitmap(icon_path)
    
    app = LekmodInstaller(root)
    root.mainloop()

if __name__ == "__main__":
    main()

