## [PC] Achievements Mods Enabler - Fallout 4 & Skyrim SE

**What is this?**

It's a plugin I made that re-enables achievements while using mods by patching Fallout4.exe or SkyrimSE.exe during runtime.
It's designed to be version independent.
It also works on already modded saves!

**How to install**

1. Install my Dll Loader manually -OR- SKSE64.
2. If using the dll loader version: copy the plugin folder to Skyrim SE's Data folder.
3. If using the SKSE64 version: copy the SKSE folder to Skyrim SE's Data folder.
3. Play!

**How to uninstall**

1. Uninstall via NMM or manually:
- If using dll loader version, delete AchievementsModsEnabler.dll, AchievementsModsEnabler.dll_Exports.txt & AchievementsModsEnabler.ini from Data\Plugins\Sumwunn.
- If using SKSE64 version, delete AchievementsModsEnabler.dll & AchievementsModsEnabler.ini from Data\SKSE\Plugins.

Options:

AchievementsModsEnabler.ini located in Data\Plugins\Sumwunn contains these options:

- iEnableLogging=1
- 0 = Disabled.
- 1 = Enabled.
- iIgnoreExpectedProcessName=0
- 0 = Expected process name detection enabled.
- 1 = Ignore Fallout4.exe name detection. Allows mod to work regardless of EXE name.
- 2 = Ignore SkyrimSE.exe name detection. Allows mod to work regardless of EXE name.

**How do I know if it's working?**

You'll get no achievement warnings, and the [M] will won't appear in the saves menu (already modded saves will need to be re-saved for [M] to disappear), alternatively you can check the log just to be sure.

After you've started Skyrim SE (you can alt-tab for this part), go to Data\Plugins\Sumwunn (for SKSE64: Data\SKSE\Plugins), there's a log file called: AchievementsModsEnabler.log

It will either say "YES" or "NO". Indicating whether or not the plugin successfully patched game. So if it says NO, lemme know!

**Will it need updating?**

Probably not! I've designed this to be version independent.
However in the case that it does, I will have it fixed very quickly.
Below is a list of Fallout 4 & Skyrim SE versions that have been tested and working.

**Fallout 4 tested & working versions: w/ 1.0.0.8+**

- 1.10.20.0.0
- 1.9.4.0.1
- 1.8.7.0.0
- 1.7.22.0

**Skyrim AE Tested & working versions w/ 1.0.1.2+:**
- 1.6.318.0

**Skyrim SE Tested & working versions w/ 1.0.0.8 - 1.0.1.1+:**
- 1.5.39.0
- 1.5.3.0
- 1.4.2.0
- 1.3.9.0
- 1.3.5.0
- 1.2.39.0
- 1.2.36.0
- 1.1.51.0
- 1.1.47.0

**Discussion**: 

Fallout 4: http://www.nexusmods.com/fallout4/mods/15639/?

Skyrim SE: http://www.nexusmods.com/skyrimspecialedition/mods/245/?

**Compile Notes**

- UASM must be setup before opening the project.

**NOTES**

- Achievements are baked into the saves. So any achievement that you would of gotten most likely won't be obtainable unless you load an earlier save or start again.
- This is not available for consoles.
- If your save is already modded, you'll have to save again to order for the [M] to disappear.
- This will not get you VAC banned in any manner as Skyrim SE does not use any anti-cheat.
- If the mod is not working, try restarting Steam and/or your computer.
- The dll loader version does not use SKSE. It will also not conflict with it.
- Written in C++/ASM.

**CREDITS**

Bethesda for Fallout 4. (https://store.steampowered.com/app/377160/)

Bethesda for Skyrim SE. (http://store.steampowered.com/app/489830/)

Microsoft for Visual Studio. (https://www.visualstudio.com/)

The UASM devs. (https://github.com/Terraspace/UASM)
