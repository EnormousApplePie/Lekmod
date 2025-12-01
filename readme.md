# Lekmod

Lekmod is a comprehensive community-driven balance/content mod for Sid Meier's Civilization V. It introduces new civilizations, balance changes, bugfixes, QoL changes, and enhanced gameplay features for both singleplayer and multiplayer. The project is primarely focused for multiplayer, some content may be missing in singleplayer games.

The project includes a custom mapscript, custom .dll source files, and the main mod files.

# Features
- New and reworked civilizations
- Extensive balance changes for units, buildings, technologies, policies and more
- Quality of Life changes and various bugfixes
- Support for both Standard UI and Enhanced User Interface (EUI) up to v1.28
- Custom mapscript (Lekmap), providing a more balanced map generation and customization and support for new resources
- Regular updates and active community support

<br/>

# Table of Contents
1. [Main Page](#main-page)
1. [Discord](#discord)
1. [Requirements](#requirements)
1. [LEKMOD Installation](#lekmod-installation)
    1. [Windows](#windows)
        1. [Option A. LEKMOD v33 and higher](#option-a-lekmod-v33-and-higher---with-custom-eui-and-standard-ui-support)
        1. [Option B. LEKMOD v32 and lower](#option-b-lekmod-v32-and-lower---no-specific-ui-support)
    1. [Linux](#linux)
1. [Lekmap Installation - universal](#lekmap-installation)
1. [Troubleshooting](#troubleshooting)
1. [Contributing](#contributing)

<br/>

# Main Page
You can find all the information about LEKMOD/Lekmap, including changelogs, features, and download links in the [LEKMOD main page](https://docs.google.com/document/d/18tsjg2C1wKA7I41GktDRr6R83eUrhn4FHi9EUEtpKvI/edit?tab=t.0#heading=h.sgkkio8aa458).

# Discord
You can find more resources and help in LEKMOD Discord
https://discord.gg/VQBNPmc

<br/>

# Requirements
1. OS: Windows 7+ - Full Support or Linux (Proton) - Limited/Untested Support, (MAC - No Support)
1. Game: Sid Meier's Civilization V - All available DLC's except for map packs.
1. Standard UI or EUI (Enhanced User Interface) - LEKMOD v33 and higher support both UIs, Note EUI v 1.29 or higher is currently not supported.

<br/>

# LEKMOD Installation 

## Windows
### Option A. LEKMOD v33 and higher *- with Custom EUI and Standard UI support*
#### 1. Download the version you wish to install
#### 2. Extract the contents of the zip file
#### 3. Copy the extracted files into your DLC folder, replacing any existing files
Usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\DLC`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. 

The final path should look like this:
`...\Sid Meier's Civilization V\Assets\DLC\LEKMODvXX_X\`
#### 4. Open the folder and run ui_check.bat
This will copy the necessary UI files based on your current UI setup. (UI / EUI up to v1.28 supported)

#### 5. Launch the game. You should see "LEKMOD" in your multiplayer lobby.

<div style="height: 1px;" alt="Separator"></div>

### Option B. LEKMOD v32 and lower *- No specific UI support*
#### 1. Download the version you wish to install
#### 2. Extract the contents of the zip file
#### 3. Copy the extracted files into your LEKMOD folder, replacing any existing files
Usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\DLC`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files. 

The final path should look like this:
`...\Sid Meier's Civilization V\Assets\DLC\LEKMODvXX_X\`
#### 4. Launch the game. You should see "LEKMOD" in your multiplayer lobby.

## Linux (Currently Unavailable)
### A. LEKMOD v33 and higher - EUI and Standard UI support
#### 0. Ensure you have Proton enabled for Civilization V
For steam, right click Civilization V in your library, select Properties, go to the Compatibility tab, and check "Force the use of a specific Steam Play compatibility tool", then select the latest Proton version from the dropdown.
Suggested Proton version: Proton Experimental or Proton 10.0-3
#### 1. Download the version you wish to install
#### 2. Extract the contents of the zip file
```sh
unzip LEKMODvXX_X.zip -d LEKMODvXX_X
```
#### 3. Copy the extracted files into your DLC folder, replacing any existing files
Usually under: `~/.steam/steam/steamapps/common/Sid Meier's Civilization V/Assets/DLC`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files.
The final path should look like this:
`.../Sid Meier's Civilization V/Assets/DLC/LEKMODvXX_X/`
#### 4. cd into the folder and run linux_ui_check.sh
This will copy the necessary UI files based on your current UI setup. (UI / EUI up to v1.28 supported)
#### 5. Launch the game. You should see "LEKMOD" in your multiplayer lobby.

<br/>

# Lekmap Installation (Windows/Linux)
### 1. Download Lekmap
### 2. Extract the contents of the zip file
### 3. Copy the extracted folder into your MAPS folder
For Windows, usually under: `C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\Assets\Maps`
To find your game folder, right click Civilization V in your Steam library, select Properties, go to the Local Files tab, and click Browse Local Files.
The final path should look like this:
`...\Sid Meier's Civilization V\Assets\Maps\Lekmap vX_X\`
### 4. Launch the game. You should see one ore more Lekmap vX_X options in the additional maps selection options.

<br/>

# Troubleshooting
## Common Issues
- **LEKMOD/Lekmap not showing in multiplayer lobby**: Ensure that you have copied the files to the correct DLC folder. The final path should be as shown in the installation instructions. eg.: `...\Sid Meier's Civilization V\Assets\DLC\LEKMODvXX_X\` / `...\Sid Meier's Civilization V\Assets\Maps\Lekmap vX_X\`
- **UI issues**: If you are using EUI, ensure you are using a supported version (v1.29 and higher is NOT SUPPORTED). Run the ui_check.bat (Windows) or linux_ui_check.sh (Linux) to ensure the correct UI files are in place.


If you encounter any issues during installation or gameplay, you can refer to the LEKMOD Discord server for assistance. Check the install-troubleshooting channel.

<br/>

# Contributing
We welcome contributions to LEKMOD! If you have improvements, bugfixes, or new features to suggest, please submit a pull request. Make sure your changes are well-documented and tested. All contributions should follow the project's quality standards. Incomplete or low-quality contributions may be declined.

<br/>

# Known Issues & Limitations
- **Linux Support**: While LEKMOD can be run on Linux using Proton, some features may not work as intended and some linux distributions may have compatibility issues.
- **Mac Support**: LEKMOD does not support macOS. Users attempting to run the mod on Mac may encounter significant issues or may not be able to run it at all.
- **EUI/UI Support**: LEKMOD v33 and up requires the ui_check.bat file to be ran. Users with EUI v1.29 or higher may experience compatibility issues.

