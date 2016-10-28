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
// ASM.
extern "C" void* BinSearch(void* Search, int SearchLength, unsigned char* Bytes, int BytesLength, int AddMod, int SubMod);
// Work around because my ASM function GetTextSectionData has multiple return value data types.
extern "C" void* GetTextSectionAddr(HMODULE Module, int DataType);
extern "C" int GetTextSectionSize(HMODULE Module, int DataType);

// Return values
// -1 = Process is NOT expected target.
// -2 = Log file creation failed.

extern "C" __declspec(dllexport) int Setup() {

	LPCTSTR ExpectedProcess01 = L"Fallout4.exe";
	// These bytes will land us just beneath where the achivements mods disabler code is at.
	unsigned char BytesToFind01[] = { 0x8B, 0xD7, 0x4C, 0x8B, 0x00, 0x48, 0x8B, 0xC8, 0x41, 0xFF, 0x50, 0x28, 0x48, 0x8B, 0x9C, 0x24, 0x50, 0x04, 0x00, 0x00 };
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch01[] = { 0x90, 0x90, 0x90, 0x90 };

	LPCTSTR ExpectedProcess02 = L"SkyrimSE.exe";
	// These bytes will land us just beneath where the achivements mods disabler code is at.
	unsigned char BytesToFind02[] = { 0xC3, 0x48, 0x89, 0x5C, 0x24, 0x40, 0x48, 0x89, 0x6C, 0x24, 0x48, 0x8B, 0xA9, 0x70, 0x0D, 0x00, 0x00 };
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch02[] = { 0xB0, 0x00, 0xC3 };

	// Pointers.
	unsigned char *BytesToFind = NULL;
	unsigned char *BytesPatch = NULL;
	int BytesToFindSize = NULL;
	int BytesPatchSize = NULL;

	// The address we get from GetTextSectionAddr.
	void* SearchAddress = (void*)NULL;
	// The size too.
	int SearchSize = NULL;
	// The address we get from BinSearch.
	void* PatchAddress = (void*)NULL;
	// We need to go back 9 bytes so we land at the right address.
	int PatchAddressModifier = NULL;
	int PatchAddressModifier01 = 9; // Fallout 4.
	int PatchAddressModifier02 = 0x35; // Skyrim SE.

	// Misc.
	DWORD OldVP = NULL;

	//////// Setup Part 1 - Addresses & Logging ////////

	// Get module of target to writes hooks to.
	// Fallout4.exe
	HMODULE TargetModule = GetModuleHandle(ExpectedProcess01);
	if (TargetModule != NULL) {
		PatchAddressModifier = PatchAddressModifier01;
		BytesToFind = BytesToFind01;
		BytesPatch = BytesPatch01;
		BytesToFindSize = sizeof BytesToFind01;
		BytesPatchSize = sizeof BytesPatch01;
	}
	// SkyrimSE.exe
	else {
		TargetModule = GetModuleHandle(ExpectedProcess02);
		if (TargetModule != NULL) {
			PatchAddressModifier = PatchAddressModifier02;
			BytesToFind = BytesToFind02;
			BytesPatch = BytesPatch02;
			BytesToFindSize = sizeof BytesToFind02;
			BytesPatchSize = sizeof BytesPatch02;
		}
		else if (TargetModule == NULL) {
			return -1;
		}
	}

	// Open up fresh log file.
	std::ofstream LogFile;
	LogFile.open("AchievementsModsEnabler.log");
	// Log file creation failed.
	if (!LogFile) {
		return -2;
	}

	// Get size and address of ExpectedProcess's .text section.
	SearchSize = GetTextSectionSize(TargetModule, 1);
	SearchAddress = GetTextSectionAddr(TargetModule, 2);

	// Get address and patch it.
	PatchAddress = BinSearch(SearchAddress, SearchSize, BytesToFind, BytesToFindSize, NULL, PatchAddressModifier);
	// Bytes not found!
	if (PatchAddress == NULL) {
		// Log message.
		LogFile << "NO" << std::endl;
	}
	// Bytes found!
	else {
		// Patch it! (with NOPS)
		VirtualProtect(PatchAddress, BytesPatchSize, PAGE_EXECUTE_READWRITE, &OldVP);
		memcpy(PatchAddress, BytesPatch, BytesPatchSize);
		VirtualProtect(PatchAddress, BytesPatchSize, OldVP, &OldVP);
		// Log message.
		LogFile << "YES" << std::endl;
	}

	// Cleanup.
	LogFile.close();

	return 0;
}