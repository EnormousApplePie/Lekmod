@echo off
echo ========================================
echo Lekmod Installer Builder
echo ========================================
echo.

REM Check if PyInstaller is installed
python -m pip show pyinstaller >nul 2>&1
if errorlevel 1 (
    echo PyInstaller not found. Installing...
    python -m pip install pyinstaller
)

echo.
echo Building installer...
echo.

REM Build the executable
python -m PyInstaller --onefile --windowed --name="LekmodInstaller" --icon=icon.ico --add-data "config.json;." --hidden-import=tkinter --hidden-import=tkinter.ttk --hidden-import=tkinter.messagebox --hidden-import=tkinter.filedialog --hidden-import=tkinter.scrolledtext installer.py

echo.
echo ========================================
echo Build complete!
echo.
echo Executable location:
echo %CD%\dist\LekmodInstaller.exe
echo ========================================
echo.

pause

