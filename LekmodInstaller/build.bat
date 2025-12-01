@echo off
echo ========================================
echo Lekmod Installer Builder
echo ========================================
echo.

REM Check if PyInstaller is installed
pip show pyinstaller >nul 2>&1
if errorlevel 1 (
    echo PyInstaller not found. Installing...
    pip install pyinstaller
)

echo.
echo Building installer...
echo.

REM Build the executable
pyinstaller --onefile --windowed --name="LekmodInstaller" --icon=icon.ico installer.py

echo.
echo ========================================
echo Build complete!
echo.
echo Executable location:
echo %CD%\dist\LekmodInstaller.exe
echo ========================================
echo.

pause

