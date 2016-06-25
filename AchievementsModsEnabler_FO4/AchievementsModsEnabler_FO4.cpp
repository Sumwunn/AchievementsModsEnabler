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
#include <ShlObj.h>
#include <stdio.h>
#include <tchar.h>

// Defined functions.
// ASM.
extern "C" void* BinSearch(void* Search, int SearchLength, unsigned char* Bytes, int BytesLength, int AddMod, int SubMod);
// Work around because my ASM function GetTextSectionData has multiple return value data types.
extern "C" void* GetTextSectionAddr(HMODULE Module, int DataType);
extern "C" int GetTextSectionSize(HMODULE Module, int DataType);

int Setup() {

	// These bytes will land us just beneath where the achivements mods disabler code is at.
	unsigned char BytesToFind[20] = { 0x8B, 0xD7, 0x4C, 0x8B, 0x00, 0x48, 0x8B, 0xC8, 0x41, 0xFF, 0x50, 0x28, 0x48, 0x8B, 0x9C, 0x24, 0x50, 0x04, 0x00, 0x00 };
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch[4] = { 0x90, 0x90, 0x90, 0x90 };
	// The address we get from GetTextSectionAddr.
	void* SearchAddress = (void*)NULL;
	// The size too.
	int SearchSize = NULL;
	// The address we get from BinSearch.
	void* PatchAddress = (void*)NULL;
	// We need to go back 9 bytes so we land at the right address.
	int PatchAddressModifier = 9;
	// Misc.
	DWORD OldVP = NULL;

	//////// Setup Part 1 - Addresses ////////

	// Get module of target to writes hooks to.
	HMODULE TargetModule = GetModuleHandle(L"Fallout4.exe");
	if (TargetModule == NULL) {
		// Incorrect process.
		return -1;
	}

	// Setup Log.
	PWSTR LogPathPTR = NULL;
	TCHAR LogPath[MAX_PATH];
	FILE *pLogFile = NULL;

	// Get log path.
	SHGetKnownFolderPath(FOLDERID_Documents, NULL, NULL, &LogPathPTR);
	_tcscpy_s(LogPath, MAX_PATH, LogPathPTR);
	_tcscat_s(LogPath, MAX_PATH, L"\\My Games\\Fallout4\\AchievementsModsEnabler_FO4.log");
	// Open up fresh log file.
	_tfopen_s(&pLogFile, LogPath, L"w+");

	// Get size and address of Fallout4.exe's .text section.
	SearchSize = GetTextSectionSize(TargetModule, 1);
	SearchAddress = GetTextSectionAddr(TargetModule, 2);

	// Get address and patch it.
	PatchAddress = BinSearch(SearchAddress, SearchSize, BytesToFind, sizeof BytesToFind, NULL, PatchAddressModifier);
	// Bytes not found!
	if (PatchAddress == NULL) {
		// Log message.
		_fputts(L"NO", pLogFile);
	}
	// Bytes found!
	else {
		// Patch it! (with NOPS)
		VirtualProtect(PatchAddress, sizeof BytesPatch, PAGE_EXECUTE_READWRITE, &OldVP);
		memcpy(PatchAddress, BytesPatch, sizeof BytesPatch);
		VirtualProtect(PatchAddress, sizeof BytesPatch, OldVP, &OldVP);
		// Log message.
		_fputts(L"YES", pLogFile);
	}
	// Cleanup.
	fclose(pLogFile);

	return 0;
}