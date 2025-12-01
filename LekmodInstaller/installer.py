"""
Lekmod Installer & Updater
Main application with GUI
"""
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext, filedialog
import threading
import json
import os
import sys
from pathlib import Path
from updater import UpdateChecker
from ui_manager import UIManager
from google_drive_api import GoogleDriveDownloader
from installer_updater import InstallerUpdater

try:
    from PIL import Image, ImageTk
    HAS_PIL = True
except ImportError:
    HAS_PIL = False
    print("Warning: PIL not installed. Background image will not be shown.")

class LekmodInstaller:
    def __init__(self, root):
        self.root = root
        self.root.title("Lekmod Installer & Updater v1.0")
        self.root.geometry("600x600")
        self.root.resizable(True, True)
        self.root.minsize(560, 560)
        
        # Set window icon
        try:
            if getattr(sys, 'frozen', False):
                # Running as exe - icon is in same folder as exe
                icon_path = Path(sys.executable).parent / 'icon.ico'
            else:
                # Running as script - icon is in same folder as installer.py
                icon_path = Path(__file__).parent / 'icon.ico'
            
            if icon_path.exists():
                self.root.iconbitmap(str(icon_path))
        except Exception as e:
            print(f"Could not load icon: {e}")
        
        # Load config (handle both dev and PyInstaller bundled)
        if getattr(sys, 'frozen', False):
            # Running as compiled exe
            config_path = Path(sys._MEIPASS) / 'config.json'
        else:
            # Running as script
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
        self.versions_data = {}  # Store full version data
        
        self.setup_ui()
        self.check_installer_updates()  # Check on startup
        self.detect_installation()
        
    def setup_ui(self):
        # Create main canvas for background image
        self.canvas_bg = "#2c3e2e"
        self.canvas = tk.Canvas(self.root, highlightthickness=0, bg=self.canvas_bg)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        # Try to load background image
        self.bg_image = None
        self.bg_photo = None
        self.resize_timer = None
        if HAS_PIL:
            try:
                if getattr(sys, 'frozen', False):
                    bg_path = Path(sys.executable).parent / 'background.png'
                else:
                    bg_path = Path(__file__).parent / 'background.png'
                
                if bg_path.exists():
                    self.bg_image = Image.open(bg_path)
                    # Do initial background setup
                    self.root.after(100, self.update_background)
            except Exception as e:
                print(f"Could not load background: {e}")
        
        # Create main frame - match canvas background color to blend seamlessly
        self.main_frame = tk.Frame(self.canvas, bg=self.canvas_bg)
        
        # Header with banner logo
        header = tk.Frame(self.main_frame, bg=self.canvas_bg)
        header.pack(fill=tk.X, pady=(0, 2))
        
        # Try to load banner logo
        if HAS_PIL:
            try:
                if getattr(sys, 'frozen', False):
                    banner_path = Path(sys.executable).parent / 'banner.png'
                else:
                    banner_path = Path(__file__).parent / 'banner.png'
                
                if banner_path.exists():
                    banner_img = Image.open(banner_path)
                    # Use native size - no resizing to avoid blurriness
                    self.banner_photo = ImageTk.PhotoImage(banner_img)
                    
                    banner_label = tk.Label(header, image=self.banner_photo, bg=self.canvas_bg)
                    banner_label.pack(pady=2)
                else:
                    # Fallback to text
                    self._create_text_header(header)
            except Exception as e:
                print(f"Could not load banner: {e}")
                self._create_text_header(header)
        else:
            self._create_text_header(header)
        
        # Main content - scales with window
        content = tk.Frame(self.main_frame, padx=6, pady=2, bg=self.canvas_bg)
        content.pack(fill=tk.BOTH, expand=True)
        
        # Create canvas window - center it
        self.canvas_window_id = self.canvas.create_window(
            0, 0, window=self.main_frame, anchor='center', tags="frame_window"
        )
        
        # Bind canvas resize with debouncing for performance
        def on_canvas_resize(event):
            # Cancel previous timer if exists
            if self.resize_timer:
                self.root.after_cancel(self.resize_timer)
            
            # Debounce resize - only update after user stops resizing (100ms delay)
            self.resize_timer = self.root.after(100, lambda: self._do_resize(event.width, event.height))
        
        self.canvas.bind('<Configure>', on_canvas_resize)
        
        # Initial center
        self.root.after(100, lambda: self._center_content())
        
        # Combined settings frame (Installation Path + UI Config + Version)
        settings_outer, settings_frame = self.create_civ5_frame(content, padx=8, pady=5)
        settings_outer.pack(fill=tk.X, pady=(0, 2))
        
        # Title for settings section
        tk.Label(settings_frame, text="Installation Settings", 
                font=("Arial", 11, "bold"), bg='#1a3d0f', fg='#f5deb3').pack(anchor=tk.W, pady=(0, 2))
        
        # Installation Path
        tk.Label(settings_frame, text="Installation Path:", 
                font=("Arial", 10), bg='#1a3d0f', fg='#f5deb3', anchor=tk.W).pack(fill=tk.X)
        
        path_entry_frame = tk.Frame(settings_frame, bg='#1a3d0f')
        path_entry_frame.pack(fill=tk.X, pady=(2, 2))
        
        self.install_path_var = tk.StringVar()
        self.install_path_var.set(r"C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V")
        
        self.install_path_entry = tk.Entry(path_entry_frame, 
                                           textvariable=self.install_path_var,
                                           font=("Arial", 9),
                                           bg='white', fg='#000000',
                                           insertbackground='#000000')
        self.install_path_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        
        browse_frame, self.browse_btn = self.create_civ5_button(
            path_entry_frame, "Browse...", self.browse_civ5_path,
            width=9, height=1, font_size=9, font_weight='normal'
        )
        browse_frame.pack(side=tk.LEFT, padx=(3, 0))
        
        verify_frame, self.verify_btn = self.create_civ5_button(
            path_entry_frame, "Verify", self.verify_civ5_path,
            width=7, height=1, font_size=9, font_weight='normal'
        )
        verify_frame.pack(side=tk.LEFT, padx=(3, 0))
        
        # Status label
        self.status_label = tk.Label(settings_frame, text="", 
                                     anchor=tk.W, fg="#90ee90", font=("Arial", 9),
                                     bg='#1a3d0f')
        self.status_label.pack(fill=tk.X, pady=(2, 0))
        
        self.version_label = tk.Label(settings_frame, text="Version: Checking...", 
                                      anchor=tk.W, bg='#1a3d0f', fg='#f5deb3',
                                      font=("Arial", 9))
        self.version_label.pack(fill=tk.X, pady=(2, 3))
        
        # Separator line
        tk.Frame(settings_frame, bg='#d2b48c', height=1).pack(fill=tk.X, pady=(0, 3))
        
        # UI Type Selection
        tk.Label(settings_frame, text="User Interface Mode:", 
                font=("Arial", 10), bg='#1a3d0f', fg='#f5deb3', anchor=tk.W).pack(fill=tk.X)
        
        eui_cb = tk.Checkbutton(settings_frame, 
                               text="Install for Enhanced UI (EUI)",
                               variable=self.use_eui,
                               font=("Arial", 10),
                               bg='#1a3d0f', fg='#f5deb3',
                               selectcolor='#0d2607',
                               activebackground='#1a3d0f',
                               activeforeground='#f5deb3')
        eui_cb.pack(anchor=tk.W, pady=(2, 1))
        
        info_label = tk.Label(settings_frame, 
                             text="Check if you have EUI (UI_bc1 or UI_bc1_xits) installed",
                             font=("Arial", 8),
                             fg="#a0a0a0",
                             bg='#1a3d0f',
                             anchor=tk.W)
        info_label.pack(anchor=tk.W, pady=(0, 1))
        
        self.eui_detected_label = tk.Label(settings_frame, text="", 
                                          anchor=tk.W, fg="#90ee90",
                                          bg='#1a3d0f',
                                          font=("Arial", 9, "italic"))
        self.eui_detected_label.pack(anchor=tk.W, pady=(0, 3))
        
        # Separator line
        tk.Frame(settings_frame, bg='#d2b48c', height=1).pack(fill=tk.X, pady=(0, 3))
        
        # Version selection
        tk.Label(settings_frame, text="Select Version:", 
                font=("Arial", 10), bg='#1a3d0f', fg='#f5deb3', anchor=tk.W).pack(fill=tk.X)
        
        self.version_var = tk.StringVar()
        self.version_combo = ttk.Combobox(settings_frame, textvariable=self.version_var,
                                          state="readonly", width=45, font=("Arial", 9))
        self.version_combo.pack(fill=tk.X, pady=(2, 3))
        
        # Separator line
        tk.Frame(settings_frame, bg='#d2b48c', height=1).pack(fill=tk.X, pady=(0, 3))
        
        # Action buttons - now inside settings frame
        button_container = tk.Frame(settings_frame, bg='#1a3d0f')
        button_container.pack(fill=tk.X, pady=(0, 2))
        
        # Install/Update button (same size as Refresh)
        install_frame, self.install_btn = self.create_civ5_button(
            button_container, "Install/Update", self.install_update,
            width=18, height=1, font_size=10, font_weight='bold'
        )
        install_frame.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 3))
        
        # Refresh Versions button
        refresh_frame, self.refresh_btn = self.create_civ5_button(
            button_container, "Refresh Versions", self.check_updates,
            width=18, height=1, font_size=10, font_weight='normal'
        )
        refresh_frame.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(3, 0))
        
        # Update installer button (below main buttons, full width)
        update_installer_frame, self.update_installer_btn = self.create_civ5_button(
            settings_frame, "Update Installer", self.update_installer,
            width=40, height=1, font_size=10, font_weight='normal'
        )
        update_installer_frame.pack(pady=(3, 0))
        
        # Log frame - now gets more space
        log_outer, log_frame = self.create_civ5_frame(content, padx=6, pady=4)
        log_outer.pack(fill=tk.BOTH, expand=True, pady=(2, 0))
        
        # Progress bars inside log frame header (shown during operations)
        progress_frame = tk.Frame(log_frame, bg='#1a3d0f')
        
        # Indeterminate progress (for general operations)
        self.progress_indeterminate = ttk.Progressbar(progress_frame, mode='indeterminate')
        
        # Determinate progress (for downloads with known size)
        self.progress_determinate = ttk.Progressbar(progress_frame, mode='determinate', maximum=100)
        
        # Progress label (shows what's happening)
        self.progress_label = tk.Label(progress_frame, text="", anchor=tk.W, fg="#90ee90", bg='#1a3d0f', font=("Arial", 9))
        
        # Hide both initially
        self.current_progress_mode = None
        
        # Log header
        log_header = tk.Frame(log_frame, bg='#1a3d0f')
        log_header.pack(fill=tk.X, pady=(0, 1))
        
        tk.Label(log_header, text="Installation Log", 
                font=("Arial", 10, "bold"), bg='#1a3d0f', fg='#f5deb3').pack(side=tk.LEFT)
        
        # Log buttons on the right
        log_button_container = tk.Frame(log_header, bg='#1a3d0f')
        log_button_container.pack(side=tk.RIGHT)
        
        # Progress bars below log header (initially hidden)
        progress_frame.pack(fill=tk.X, pady=(0, 1))
        
        expand_log_frame, expand_log_btn = self.create_civ5_button(
            log_button_container, "📋 Expand", self.expand_log,
            width=9, height=1, font_size=9, font_weight='normal'
        )
        expand_log_frame.pack(side=tk.LEFT, padx=(0, 3))
        
        clear_log_frame, clear_log_btn = self.create_civ5_button(
            log_button_container, "🗑️ Clear", self.clear_log,
            width=7, height=1, font_size=9, font_weight='normal'
        )
        clear_log_frame.pack(side=tk.LEFT)
        
        # Log text area - expands to fill available space
        self.log_text = scrolledtext.ScrolledText(log_frame, height=8, 
                                                  state=tk.NORMAL,
                                                  wrap=tk.WORD,
                                                  bg="#2c2416",
                                                  fg="#d2b48c",
                                                  font=("Consolas", 9),
                                                  insertbackground='#d2b48c')
        self.log_text.pack(fill=tk.BOTH, expand=True)
        
        # Add initial welcome message
        self.log("Lekmod Installer v1.0.0")
        self.log("Ready to install!")
        self.log("")
        
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
        """Add message to log (thread-safe)"""
        def _update_log():
            # If message starts with "Downloading..." and previous line also does, replace it
            try:
                current_text = self.log_text.get("end-2l", "end-1l").strip()
                if message.startswith("Downloading...") and current_text.startswith("Downloading..."):
                    # Delete the previous downloading line and replace it
                    self.log_text.delete("end-2l", "end-1l")
            except:
                pass
                
            self.log_text.insert(tk.END, f"{message}\n")
            self.log_text.see(tk.END)
            self.root.update()
        
        # Ensure UI update happens in main thread
        if threading.current_thread() != threading.main_thread():
            self.root.after(0, _update_log)
        else:
            _update_log()
    
    def set_progress(self, value, message=""):
        """Set progress bar value (0-100) with optional message"""
        def _update():
            if self.current_progress_mode != 'determinate':
                # Hide indeterminate, show determinate
                if self.current_progress_mode == 'indeterminate':
                    self.progress_indeterminate.pack_forget()
                    self.progress_indeterminate.stop()
                
                self.progress_label.pack(fill=tk.X, pady=(0, 2))
                self.progress_determinate.pack(fill=tk.X)
                self.current_progress_mode = 'determinate'
            
            self.progress_determinate['value'] = value
            if message:
                self.progress_label.config(text=message)
            self.root.update()
        
        self.root.after(0, _update)
    
    def show_indeterminate_progress(self):
        """Show bouncing progress bar"""
        def _update():
            if self.current_progress_mode != 'indeterminate':
                if self.current_progress_mode == 'determinate':
                    self.progress_determinate.pack_forget()
                    self.progress_label.pack_forget()
                
                self.progress_indeterminate.pack(fill=tk.X)
                self.progress_indeterminate.start()
                self.current_progress_mode = 'indeterminate'
        
        self.root.after(0, _update)
    
    def hide_progress(self):
        """Hide all progress bars"""
        def _update():
            if self.current_progress_mode == 'indeterminate':
                self.progress_indeterminate.stop()
                self.progress_indeterminate.pack_forget()
            elif self.current_progress_mode == 'determinate':
                self.progress_determinate.pack_forget()
                self.progress_label.pack_forget()
            
            self.current_progress_mode = None
        
        self.root.after(0, _update)
    
    def expand_log(self):
        """Open log in a larger window"""
        log_window = tk.Toplevel(self.root)
        log_window.title("Installation Log - Full View")
        log_window.geometry("900x600")
        
        # Copy log content
        log_content = self.log_text.get("1.0", tk.END)
        
        # Create larger text widget
        text_widget = scrolledtext.ScrolledText(log_window, 
                                                wrap=tk.WORD,
                                                bg="#f8f9fa",
                                                fg="#000000",
                                                font=("Consolas", 10))
        text_widget.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        text_widget.insert("1.0", log_content)
        text_widget.see(tk.END)
        
        # Close button
        tk.Button(log_window, text="Close", command=log_window.destroy,
                 bg="#3498db", fg="white", font=("Arial", 10)).pack(pady=10)
    
    def clear_log(self):
        """Clear the log"""
        self.log_text.delete("1.0", tk.END)
        self.log("Log cleared")
        self.log("")
    
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
        self.show_indeterminate_progress()
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
            self.hide_progress()
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
        self.show_indeterminate_progress()
        self.refresh_btn.config(state=tk.DISABLED)
        threading.Thread(target=self._check_updates_thread, 
                        daemon=True).start()
        
    def _check_updates_thread(self):
        try:
            self.log("Fetching versions from GitHub...")
            
            # Get versions from GitHub
            versions = self.updater.get_available_versions()
            
            if not versions:
                self.log("⚠ No versions found")
                self.root.after(0, lambda: messagebox.showwarning(
                    "No Versions", 
                    "Could not find any versions.\n"
                    "Please check your internet connection\n"
                    "and GitHub/Google Drive configuration."
                ))
                return
            
            # Store versions data for later use
            self.versions_data = versions
            
            # Format version list with dates and sizes
            version_display = []
            for v in versions:
                info = versions[v]
                date = info.get('release_date', 'Unknown date')
                size = info.get('size', 'Unknown size')
                version_display.append(f"{v} - {date} ({size})")
            
            # Update combo box in main thread
            def update_combo():
                self.version_combo['values'] = version_display
                if version_display:
                    self.version_combo.current(0)
            
            self.root.after(0, update_combo)
            
            self.log(f"✓ Found {len(versions)} version(s): {', '.join(versions.keys())}")
            
        except Exception as e:
            self.log(f"✗ Error checking updates: {e}")
            self.root.after(0, lambda: messagebox.showerror(
                "Update Check Failed", 
                f"Failed to check for updates:\n{str(e)}\n\n"
                f"Please check your internet connection."
            ))
        finally:
            self.hide_progress()
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
        self.log("=" * 50)
        self.install_btn.config(state=tk.DISABLED)
        self.refresh_btn.config(state=tk.DISABLED)
        
        # Show indeterminate progress for now
        self.show_indeterminate_progress()
        
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
            
            # Check for existing LEKMOD installations
            existing_folders = self.ui_manager.find_existing_lekmod_folders(civ5_path)
            
            if existing_folders:
                # Ask user what to do
                folder_list = "\n  - ".join(existing_folders)
                
                response = messagebox.askyesnocancel(
                    "Existing LEKMOD Installation Found",
                    f"Found existing LEKMOD installation(s):\n\n  - {folder_list}\n\n"
                    f"These must be removed before installing {version}.\n\n"
                    f"Remove and continue?\n\n"
                    f"(Click 'No' to cancel installation)",
                    icon='warning'
                )
                
                if response is None or response is False:
                    # User clicked Cancel or No
                    self.log("Installation cancelled by user")
                    return
                
                # User clicked Yes - remove existing installations
                self.log(f"Found {len(existing_folders)} existing LEKMOD folder(s)")
                self.ui_manager.remove_lekmod_folders(civ5_path, existing_folders, self.log)
                self.log("✓ Existing installations removed")
            
            # 1. Download from Google Drive
            self.log(f"📥 Downloading Lekmod {version} from Google Drive...")
            
            # Get version info from GitHub (not bundled config)
            all_versions = self.updater.get_available_versions()
            if version not in all_versions:
                raise Exception(f"Version {version} not found in available versions!")
            
            version_info = all_versions[version]
            download_path = self.downloader.download_version_with_info(
                version, version_info, self.log, self.set_progress
            )
            
            # 2. Extract
            self.show_indeterminate_progress()
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
            
            self.log("=" * 50)
            self.log("✅ Installation complete!")
            self.log("=" * 50)
            messagebox.showinfo("Success", 
                               f"Lekmod {version} installed successfully!\n\n"
                               f"UI Mode: {ui_type}\n"
                               f"Location: {civ5_path}/Assets/DLC/LEKMOD_{version}")
            
        except Exception as e:
            self.log("=" * 50)
            self.log(f"❌ Installation failed: {e}")
            self.log("=" * 50)
            messagebox.showerror("Installation Failed", 
                               f"Installation failed:\n\n{str(e)}\n\n"
                               f"Please check the log for details.")
        finally:
            self.hide_progress()
            self.install_btn.config(state=tk.NORMAL)
            self.refresh_btn.config(state=tk.NORMAL)
            self.check_installed_version()

    def _create_text_header(self, header):
        """Fallback text header if banner image not found"""
        title = tk.Label(header, text="LEKMOD", font=("Arial", 20, "bold"),
                        bg=self.canvas_bg, fg="#f5deb3")  # Beige color
        title.pack(pady=2)
        
        version_text = f"v{self.config.get('installer_version', '1.0.0')}"
        subtitle = tk.Label(header, text=f"Installer & Updater - {version_text}", 
                           font=("Arial", 10),
                           bg=self.canvas_bg, fg="#d2b48c")
        subtitle.pack()
    
    def _do_resize(self, width, height):
        """Perform the actual resize operation"""
        self.update_background()
        self._center_content()
    
    def _center_content(self):
        """Center the main content on the canvas"""
        try:
            canvas_width = self.canvas.winfo_width()
            canvas_height = self.canvas.winfo_height()
            
            if canvas_width > 1 and canvas_height > 1:
                # Center the window
                x = canvas_width // 2
                y = canvas_height // 2
                self.canvas.coords(self.canvas_window_id, x, y)
        except Exception as e:
            print(f"Error centering content: {e}")
    
    def update_background(self):
        """Update background image to fit current window size while maintaining aspect ratio"""
        if not self.bg_image or not HAS_PIL:
            return
        
        try:
            # Get current canvas size
            canvas_width = self.canvas.winfo_width()
            canvas_height = self.canvas.winfo_height()
            
            if canvas_width <= 1 or canvas_height <= 1:
                return
            
            # Get original image dimensions
            img_width, img_height = self.bg_image.size
            
            # Calculate scaling to cover the canvas while maintaining aspect ratio
            width_ratio = canvas_width / img_width
            height_ratio = canvas_height / img_height
            
            # Use the larger ratio to ensure the image covers the entire canvas
            scale_ratio = max(width_ratio, height_ratio)
            
            # Calculate new dimensions
            new_width = int(img_width * scale_ratio)
            new_height = int(img_height * scale_ratio)
            
            # Resize background image maintaining aspect ratio
            resized = self.bg_image.resize((new_width, new_height), Image.Resampling.LANCZOS)
            self.bg_photo = ImageTk.PhotoImage(resized)
            
            # Update canvas - center the background
            self.canvas.delete("bg")
            self.canvas.create_image(canvas_width//2, canvas_height//2, 
                                    image=self.bg_photo, anchor='center', tags="bg")
            self.canvas.tag_lower("bg")
        except Exception as e:
            print(f"Error updating background: {e}")
    
    def create_civ5_frame(self, parent, **kwargs):
        """Create a Civ 5 styled frame with beige border and dark green background"""
        padx = kwargs.get('padx', 12)
        pady = kwargs.get('pady', 10)
        
        # Outer frame for beige border
        outer_frame = tk.Frame(parent, bg='#d2b48c', bd=0)
        
        # Inner frame with dark green background
        inner_frame = tk.Frame(outer_frame, bg='#1a3d0f', padx=padx, pady=pady)
        inner_frame.pack(padx=2, pady=2, fill=tk.BOTH, expand=True)
        
        return outer_frame, inner_frame
    
    def create_civ5_button(self, parent, text, command, **kwargs):
        """Create a Civ 5 styled button with beige background and darker border"""
        # Extract standard button options
        width = kwargs.get('width', 15)
        height = kwargs.get('height', 1)
        font_size = kwargs.get('font_size', 10)
        font_weight = kwargs.get('font_weight', 'bold')
        border_width = kwargs.get('border_width', 2)
        
        # Create frame for border effect
        frame = tk.Frame(parent, bg='#8B7355', bd=0)
        
        # Create button with beige background and black text
        btn = tk.Button(frame, text=text, command=command,
                       bg='#e8dcc4', fg='#000000',  # Light beige bg, black text
                       activebackground='#d4c5a9',  # Darker beige when pressed
                       activeforeground='#000000',
                       font=("Arial", font_size, font_weight),
                       bd=0, relief=tk.FLAT,
                       width=width, height=height)
        # Pack with exact same padding on all sides for even border
        btn.pack(padx=border_width, pady=border_width)
        
        return frame, btn

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

