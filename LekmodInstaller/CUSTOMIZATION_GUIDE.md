# Lekmod Installer Customization Guide

## Visual Assets

The installer supports custom visual assets to match the Civ 5 aesthetic:

### 1. Window Icon (`icon.ico`)
- **Required**: Yes (you already have this)
- **Location**: `LekmodInstaller/icon.ico`
- **Purpose**: Window icon and taskbar icon
- **Recommended size**: 256x256 pixels

### 2. Background Image (`background.png`)
- **Required**: No (optional)
- **Location**: `LekmodInstaller/background.png`
- **Purpose**: Full window background image (auto-resizes to fit)
- **Recommended**: Use a Civ 5 themed background (like the main menu)
- **Notes**: If not provided, a dark background will be used instead

### 3. Banner Logo (`banner.png`)
- **Required**: No (optional)
- **Location**: `LekmodInstaller/banner.png`
- **Purpose**: Replaces the "LEKMOD" text at the top
- **Recommended size**: 500px wide (height auto-scales)
- **Notes**: If not provided, "LEKMOD" text will be shown instead

## Button Styling

Buttons are styled to match Civ 5:
- **Background**: Dark green with ~80% opacity (#2d5016)
- **Border**: Medium thick beige outline (#f5deb3 / #d2b48c)
- **Text**: Beige color (#f5deb3)
- **Corners**: Slightly rounded effect (via frame borders)
- **Hover**: Slightly lighter green (#3d6020)

## Building

1. Place your custom images in the `LekmodInstaller/` folder
2. Run `build.bat`
3. The executable and resources will be in `dist/`

The build script will automatically:
- Install Pillow (PIL) for image support
- Copy `icon.ico`, `background.png`, and `banner.png` to `dist/` (if they exist)
- Bundle everything into the installer

## Tips

- For the background image, use the Civ 5 main menu background for best results
- For the banner, create a logo with transparent background
- All images are optional - the installer will work without them

