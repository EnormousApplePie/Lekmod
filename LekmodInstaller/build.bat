@echo off
echo ========================================
echo Lekmod Installer Builder
echo ========================================
echo.

REM Check if Python is available
python --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Python not found!
    echo Please install Python and make sure it's in your PATH.
    echo.
    pause
    exit /b 1
)

echo Python found: 
python --version
echo.

REM Check if dependencies are installed
echo Checking dependencies...
echo.

echo Checking for PyInstaller...
python -m pip show pyinstaller >nul 2>&1
if %errorlevel% neq 0 (
    echo PyInstaller not found. Installing...
    python -m pip install pyinstaller
    REM Check if PyInstaller is now installed (ignore pip exit code)
    python -m pip show pyinstaller >nul 2>&1
    if %errorlevel% neq 0 (
        echo ERROR: Failed to install PyInstaller!
        echo.
        pause
        exit /b 1
    )
    echo PyInstaller installed successfully!
) else (
    echo PyInstaller found!
)

echo.
echo Checking for Pillow...
python -m pip show Pillow >nul 2>&1
if %errorlevel% neq 0 (
    echo Pillow not found. Installing...
    python -m pip install Pillow
    REM Check if Pillow is now installed (ignore pip exit code)
    python -m pip show Pillow >nul 2>&1
    if %errorlevel% neq 0 (
        echo ERROR: Failed to install Pillow!
        echo.
        pause
        exit /b 1
    )
    echo Pillow installed successfully!
) else (
    echo Pillow found!
)

echo.
echo All dependencies OK!
echo.
echo Building installer...
echo This may take a minute...
echo.

REM Build the executable with all resources
REM Note: Images (background.png, banner.png, icon.ico) will be bundled if they exist
python -m PyInstaller --onefile --windowed --name="LekmodInstaller" --icon=icon.ico --add-data "config.json;." --hidden-import=tkinter --hidden-import=tkinter.ttk --hidden-import=tkinter.messagebox --hidden-import=tkinter.filedialog --hidden-import=tkinter.scrolledtext installer.py

if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    echo Check the output above for errors.
    echo.
    pause
    exit /b 1
)

REM Copy image files to dist folder (if they exist)
echo.
echo Copying resources to dist folder...
if exist icon.ico (
    copy icon.ico dist\icon.ico >nul 2>&1
    echo - Copied icon.ico
)
if exist background.png (
    copy background.png dist\background.png >nul 2>&1
    echo - Copied background.png
)
if exist banner.png (
    copy banner.png dist\banner.png >nul 2>&1
    echo - Copied banner.png
)

echo.
echo ========================================
echo Build complete!
echo.
echo Executable location:
echo %CD%\dist\LekmodInstaller.exe
echo ========================================
echo.

pause

