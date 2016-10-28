##[PC] Achievements Mods Enabler - Fallout 4 & Skyrim SE

**What is this?**

It's a plugin I made that re-enables achievements while using mods by patching Fallout4.exe or SkyrimSE.exe during runtime.
It's designed to be version independent.
It also works on already modded saves!

**What's in the download?**

- Plugin folder which contains the plugin itself along with its dll loader.
- Copy of this thread in a readme & licenses.

**How to install**

1. Copy the Plugin folder's contents to Fallout 4/SkyrimSE's root folder.

2. Play!

**How to uninstall**

1. Delete DllLoader.txt, X3DAudio1_7.dll & AchievementsModsEnabler.dll from Fallout 4/SkyrimSE's root folder.

**Will it need updating?**

Probably not! I've designed this to be version independent.
However in the case that it does, I will have it fixed very quickly.
Below is a list of Fallout 4 versions that have been tested and working.

**How do I know if it's working?**

In your Fallout4/SkyrimSE folder in My Games. There's a log file called: AchievementsModsEnabler.log
It will either say "YES" or "NO". Indicating whether or not the plugin successfully found and patched what is needed to be.
So if it says NO, lemme know!

**Fallout 4 tested & working versions:**

- 1.5.416.0
- 1.6.3.0
- 1.6.9.0
- 1.7.7.0
- 1.7.9.0
- 1.7.10.0
- 1.7.12.0
- 1.7.15.0

**Skyrim SE tested & working versions:**

- 1.1.47.0.8

**Discussion**: 

Fallout 4: http://www.nexusmods.com/fallout4/mods/15639/?

Skyrim SE: http://www.nexusmods.com/skyrimspecialedition/mods/245/?

**Compile Notes**

- HJWasm must be setup before opening the project.

**NOTES**

- Written in C++/ASM (Visual Studio 2015 & HJWasm).
- The Dll loader can actually be used to load any dll into Fallout 4 or the Creation Kit. (edit DllLoader.txt for Fallout4.exe and ???.exe)
- If you so happen to be using my SteelSeries plugin, be sure to combine the contents of DllLoader.txt so both mods load.
- If loading up a pre-modded save, the game will still warn you about achievements being disabled, you can ignore this, achievements will still work.

**CREDITS**

Bethesda for Fallout 4. (https://store.steampowered.com/app/377160/)

Bethesda for Skyrim SE. (http://store.steampowered.com/app/489830/)

Microsoft for Visual Studio 2015. (https://www.visualstudio.com/)

The HJWasm devs. (https://github.com/Terraspace/HJWasm)
