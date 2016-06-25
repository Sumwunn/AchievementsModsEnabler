##[PC] [WIPz] Achievements Mods Enabler - Fallout 4

**What is this?**

It's a plugin I made that allows re-enables achievements while using mods by patching Fallout4.exe during runtime.
It's designed to be version independent.

**What's in the download?**

- Plugin folder which contains the plugin itself along with its dll loader.
- Copy of this thread in a readme & licenses.

**How to install**

1. Copy the Plugin folders contents to Fallout 4's root folder.

2. Play!

**How to uninstall**

1. Delete DllLoader.txt, X3DAudio1_7.dll & AchievementsModsEnabler_FO4.dll from Fallout 4's root folder.

**Will it need updating?**

Probably not! I've designed this to be version independent.
However in the case that it does, I will have it fixed very quickly.
Below is a list of Fallout 4 versions that have been tested and working.

**How do I know if it's working?**

In your Fallout4 folder in My Games. There's a log file called: AchievementsModsEnabler_FO4.log
It will either say "YES" or "NO". Indicating whether or not the plugin successfully found and patched what is needed to be.
So if it says NO, lemme know!

**Discussion**: 

**Compile Notes**

- HJWasm must be setup before opening the project.

**NOTES**

- Written in C++/ASM (Visual Studio 2015 & HJWasm).
- The Dll loader can actually be used to load any dll into Fallout 4 or the Creation Kit. (edit DllLoader.txt for Fallout4.exe & DllLoaderEditor.txt for CreationKit.exe)
- If you so happen to be using my SteelSeries plugin, be sure to combine the contents of DllLoader.txt so both mods load.

**CREDITS**

Bethesda for Fallout 4. (https://store.steampowered.com/app/377160/)

Microsoft for Visual Studio 2015. (https://www.visualstudio.com/)

The HJWasm devs. (https://github.com/Terraspace/HJWasm)
