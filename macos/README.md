# macOS (experimental)

Requires Python 3.9+, Xcode Command Line Tools, Steam Civilization V (Aspyr build
180925) with all non-map DLC, and Rosetta on Apple silicon. Standard UI only.

## Install

First, close Civilization V.
In Finder, open `macos/` and double-click **Install Lekmod.command**. Follow the instructions.

To run the GUI via bash:
```sh
python3 macos/install.py --gui
```

Steam libraries, including external drives, are detected automatically.
Use **Browse…** to select the folder containing `Civilization V.app`.

Installs this checkout's native library and assets inside the game app: Lekmod
under `Contents/Assets/Assets/DLC/LEKMOD`, Lekmap under
`Contents/Assets/Assets/Maps/Lekmap`. Launch normally through Steam.
Rerun after updating the checkout. Windows release downloads are not used.

The previous app is retained in `.lekmod-backups` beside the game; its exact
path is printed after installation. To restore, close the game, move the current
app aside, and move the backup app into its place. Steam updates/Verify Files
may undo installation.

## Experimental Windows cross-play

Cross-play is opt-in and requires the same Lekmod release and required DLC on
both machines. For the supported Aspyr 180925 executable, the native library
adapts the multiplayer registration string to Windows build 403694 at a verified
call site. The game retains its original global build metadata and version
checks, and the installer preserves the setting across Lekmod updates.

After installing, close the game and enable cross-play:

```sh
python3 macos/crossplay.py --enable
```

Launch through Steam, host a multiplayer game on Windows, and invite the Mac
Steam account. To disable cross-play, close the game and run:

```sh
python3 macos/crossplay.py --disable
```
