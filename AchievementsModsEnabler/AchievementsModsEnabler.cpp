/*                             The MIT License (MIT)

Copyright (c) 2016 Sumwunn @ github.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>

// Defined functions.
// ASM
extern "C" void* BinSearch(void* Search, int SearchLength, unsigned char* Bytes, int BytesLength, int AddMod, int SubMod);
// Work around because my ASM function GetTextSectionData has multiple return value data types.
extern "C" void* GetTextSectionAddr(HMODULE Module, int DataType);
extern "C" int GetTextSectionSize(HMODULE Module, int DataType);
// C++
int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub);

// Return values
// 0 = Patching failed, bytes not found.
// 1 = Patching successful, bytes found.
// -1 = Process is NOT expected target.
// -2 = Log file creation failed.

extern "C" __declspec(dllexport) int Setup() {

	LPCTSTR ExpectedProcess01 = L"Fallout4.exe";
	// These bytes will land us just beneath where the achivements mods disabler code is at.
	unsigned char BytesToFind01[] = { 0xC3, 0x40, 0x32, 0xFF, 0x48, 0x89, 0x5C, 0x24, 0x40, 0x48, 0x89, 0x6C, 0x24, 0x48 };
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch01[] = { 0xB0, 0x00, 0xC3 };

	LPCTSTR ExpectedProcess02 = L"SkyrimSE.exe";
	// These bytes will land us just beneath where the achivements mods disabler code is at.
	unsigned char BytesToFind02_01[] = { 0xC3, 0x48, 0x89, 0x5C, 0x24, 0x40, 0x48, 0x89, 0x6C, 0x24, 0x48, 0x8B, 0xA9, 0x70, 0x0D, 0x00, 0x00 }; // Skyrim SE v1.1.
	unsigned char BytesToFind02_02[] = { 0xC3, 0x40, 0x32, 0xFF, 0x48, 0x89, 0x5C, 0x24, 0x40, 0x48, 0x89, 0x6C, 0x24, 0x48 }; // Skyrim SE v1.2.
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch02[] = { 0xB0, 0x00, 0xC3 };

	// We need to go back X bytes so we land at the right address.
	int AddressModifierSub01 = 0x29; // Fallout 4.
	int AddressModifierSub02_01 = 0x35; // Skyrim SE v1.1.
	int AddressModifierSub02_02 = 0x30; // Skyrim SE v1.2.

	// Misc.
	HMODULE hModule = NULL;
	std::ofstream LogFileHandle;

	//////// Setup Part 1 - Addresses & Logging ////////

	// Fallout 4.
	hModule = GetModuleHandle(ExpectedProcess01);
	if (hModule != NULL) {
		// Open up fresh log file.
		LogFileHandle.open(L"AchievementsModsEnabler.log");
		// Log file creation failed.
		if (!LogFileHandle) {
			return -2;
		}
		// Find bytes and patch them.
		if (BinPatch(hModule, BytesToFind01, sizeof BytesToFind01, BytesPatch01, sizeof BytesPatch01, NULL, AddressModifierSub01) == 0) {
			// Bytes not found!
			// Log message.
			LogFileHandle << "NO" << std::endl;
			// Cleanup.
			LogFileHandle.close();
			return 0;
		}
		else {
			// Bytes found!
			// Log message.
			LogFileHandle << "YES" << std::endl;
			// Cleanup.
			LogFileHandle.close();
			return 1;
		}
	}

	// Skyrim SE.
	hModule = GetModuleHandle(ExpectedProcess02);
	if (hModule != NULL) {
		// Open up fresh log file.
		LogFileHandle.open(L"Data\\Plugins\\Sumwunn\\AchievementsModsEnabler.log");
		// Log file creation failed.
		if (!LogFileHandle) {
			return -2;
		}
		// Find bytes and patch them.
		if (BinPatch(hModule, BytesToFind02_01, sizeof BytesToFind02_01, BytesPatch02, sizeof BytesPatch02, NULL, AddressModifierSub02_01) == 0 &&
			BinPatch(hModule, BytesToFind02_02, sizeof BytesToFind02_02, BytesPatch02, sizeof BytesPatch02, NULL, AddressModifierSub02_02) == 0) {
			// Bytes not found!
			// Log message.
			LogFileHandle << "NO" << std::endl;
			// Cleanup.
			LogFileHandle.close();
			return 0;
		}
		else {
			// Bytes found!
			// Log message.
			LogFileHandle << "YES" << std::endl;
			// Cleanup.
			LogFileHandle.close();
			return 1;
		}
	}

	// Process not found.
	return -1;
}

int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub) { // BinSearch + MEMCPY patching.

	// The address we get from GetTextSectionAddr.
	void* SearchAddress = (void*)NULL;
	// The size too.
	int SearchSize = NULL;
	// The address we get from BinSearch.
	void* PatchAddress = (void*)NULL;
	// Misc.
	DWORD lpflOldProtect = NULL;

	// Get size and address of ExpectedProcess's .text section.
	SearchSize = GetTextSectionSize(hModule, 1);
	SearchAddress = GetTextSectionAddr(hModule, 2);
	// Get address and patch it.
	PatchAddress = BinSearch(SearchAddress, SearchSize, BytesToFind, BytesToFindSize, AddressModifierAdd, AddressModifierSub);
	if (PatchAddress == NULL) {
		// Bytes not found.
		return 0;
	}
	// Bytes found!
	else {
		// Patch it! (with NOPS)
		VirtualProtect(PatchAddress, BytesPatchSize, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		memcpy(PatchAddress, BytesPatch, BytesPatchSize);
		VirtualProtect(PatchAddress, BytesPatchSize, lpflOldProtect, &lpflOldProtect);
		return 1;
	}

	return 0;
}