# NEOTOKYO Mod Manager
A Qt application for automating the installing and uninstalling of neotokyo mods.
Uses Qt 5.15.2

## First time setup
A prebuilt version can be found here: https://github.com/NotHosomi/nt-mod-manager/releases/tag/v1.0
Unzip the downloaded file and move the `/ModManager/` directory into your Neotokyo install alongside hl2.exe
Expected filepath: `<Steamapps>/common/NEOTOKYO/ModManager/NeotokyoModManage.exe`
Run the application, then place your mods in `<Steamapps>/common/NEOTOKYO/ModManager/mods/`.
Press refresh in the app, and a list of the found mods should appear.
Double click items from this list to add them to the install list, then click apply.

## Error Troubleshooting
# Could not find game directory
You put the ModManager directory in the wrong place

# "Mod name": Incompatible file structure
The mod manager expects the mod directory to contain a folder called `/NeotokyoSource/` that contains `/materials`, `/models/`, and/or `/sound/` which contain the actual assets.
Some mods don't follow this structure and are currently not supported by the mod manager.

# "Mod name": Mod directory not found
Bro why did you delete the mod folder then try to install it

# "Mod name": trying to replace a file that doesn't exist
As most NT mods are just file replacements, this error will occur if the mod is trying to add a file to your install that isn't referenced by the game.
This is primarily a safeguard to make sure the modloader doesn't put stuff in weird places by accident, but can also be caused by the mod having an unexpected file structure.