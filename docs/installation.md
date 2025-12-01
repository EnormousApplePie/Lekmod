# Installation Instructions


# Table of Contents
1. [Requirements](#requirements)
1. [LEKMOD Installation](#lekmod-installation)
    1. [Windows](#windows)
        1. [LEKMOD v33 and higher](#option-a-lekmod-v33-and-higher---with-custom-eui-and-standard-ui-support)
        1. [LEKMOD v32 and lower](#option-b-lekmod-v32-and-lower---no-specific-ui-support)
    1. [Linux](#linux)
        1. [LEKMOD v33 and higher - (Currently Unavailable)](#a-lekmod-v33-and-higher---eui-and-standard-ui-support---currently-unavailable)
        1. [LEKMOD v32 and lower](#b-lekmod-v32-and-lower---no-specific-ui-support)
1. [Lekmap Installation](#lekmap-installation-windowslinux)

# Requirements
1. OS
    1. **Windows 7+** - Full Support
    1. **Linux** - Limited/Untested Support
    1. **MacOS** - **NOT SUPPORTED**
1. Game: Sid Meier's Civilization V - All available DLC's except for map packs.
1. Standard UI or EUI (Enhanced User Interface) - LEKMOD v33 and higher support both UIs, Note EUI v 1.29 or higher is currently not supported.

# LEKMOD Installation 

## Windows
### Option A. LEKMOD v33 and higher *- with Custom EUI and Standard UI support*
1. Download the version you wish to install
1. Extract the contents of the zip file
1. Copy the extracted files into your DLC folder, replacing any existing files
Usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\DLC` <br/> 
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. <br/> The final path should look like this:
`...\Sid Meier's Civilization V\Assets\DLC\LEKMODvXX_X\`
1. Open the folder and run ui_check.bat
This will copy the necessary UI files based on your current UI setup. (UI / EUI up to v1.28 supported)

1. Launch the game. You should see "LEKMOD" in your multiplayer lobby.


### Option B. LEKMOD v32 and lower *- No specific UI support*
1. Download the version you wish to install
1. Extract the contents of the zip file
1. Copy the extracted files into your LEKMOD folder, replacing any existing files
Usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\DLC` <br/> 
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. <br/> The final path should look like this:
`...\Sid Meier's Civilization V\Assets\DLC\LEKMODvXX_X\`
1. Launch the game. You should see "LEKMOD" in your multiplayer lobby.

## Linux
### A. LEKMOD v33 and higher - EUI and Standard UI support - (Currently Unavailable)
1. Ensure you have Proton enabled for Civilization V
For steam, right click Civilization V in your library, select Properties, go to the Compatibility tab, and check "Force the use of a specific Steam Play compatibility tool", then select the latest Proton version from the dropdown.
Suggested Proton version: Proton Experimental or Proton 10.0-3
1. Download the version you wish to install
1. Extract the contents of the zip file
    ```sh
    unzip LEKMODvXX_X.zip -d LEKMODvXX_X
    ```
1. Copy the extracted files into your DLC folder, replacing any existing files
Usually under: `~/.steam/steam/steamapps/common/Sid Meier's Civilization V/Assets/DLC`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. <br/>The final path should look like this: <br/> `.../Sid Meier's Civilization V/Assets/DLC/LEKMODvXX_X/`
1. cd into the folder and run linux_ui_check.sh
This will copy the necessary UI files based on your current UI setup. (UI / EUI up to v1.28 supported)
1. Launch the game. You should see "LEKMOD" in your multiplayer lobby.

### B. LEKMOD v32 and lower - No specific UI support
1. Ensure you have Proton enabled for Civilization V
For steam, right click Civilization V in your library, select Properties, go to the Compatibility tab, and check "Force the use of a specific Steam Play compatibility tool", then select the latest Proton version from the dropdown.
Suggested Proton version: Proton Experimental or Proton 10.0-3
1. Download the version you wish to install
1. Extract the contents of the zip file
    ```sh
    unzip LEKMODvXX_X.zip -d LEKMODvXX_X
    ```
1. Copy the extracted files into your DLC folder, replacing any existing files
Usually under: `~/.steam/steam/steamapps/common/Sid Meier's Civilization V/Assets/DLC`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. <br/>The final path should look like this: <br/> `.../Sid Meier's Civilization V/Assets/DLC/LEKMODvXX_X/`
1. Launch the game. You should see "LEKMOD" in your multiplayer lobby.


# Lekmap Installation (Windows/Linux)
1. Download Lekmap
1. Extract the contents of the zip file
1. Copy the extracted folder into your MAPS folder
    For Windows, usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\Maps` <br/>
    To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files.
    The final path should look like this: <br/>
    `...\Sid Meier's Civilization V\Assets\Maps\Lekmap vX_X\`
1. Launch the game. You should see one ore more Lekmap vX_X options in the additional maps selection options.