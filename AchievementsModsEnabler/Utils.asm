;                              The MIT License (MIT)
; 
; Copyright (C) 2024 Sumwunn @ GitHub.com
; 
; Permission is hereby granted, free of charge, to any person obtaining a copy of
; this software and associated documentation files (the "Software"), to deal in
; the Software without restriction, including without limitation the rights to
; use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
; the Software, and to permit persons to whom the Software is furnished to do so,
; subject to the following conditions:
; 
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
; 
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
; FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
; COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
; IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
; CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

IFDEF _WIN32

.486
.MODEL FLAT, C
OPTION CASEMAP:NONE

.code

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

ENDIF

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

IFDEF _WIN64

.x64
OPTION CASEMAP:NONE
OPTION FRAME:AUTO
OPTION STACKBASE:RSP
OPTION WIN64:11

VirtualProtect PROTO :QWORD, :DWORD, :DWORD, :QWORD

.data?

OldVP DWORD ?

.code

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

apiHook_Older proc frame

mov al, 0

ret

apiHook_Older endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

apiHook_Newer proc frame

mov byte ptr [RSP+0x10],0
movzx eax, byte ptr [RSP+0x10]

ret

apiHook_Newer endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

HookWrite proc frame HookAddress:QWORD, HookDestination:QWORD

invoke VirtualProtect, HookAddress, 20, 40h, addr OldVP

; Construct Hook
mov rax, [HookAddress]
mov word ptr [rax], 0B848h
mov rdx, [HookDestination]
mov qword ptr [rax+2], rdx
mov word ptr [rax+0Ah], 0E0FFh

invoke VirtualProtect, HookAddress, 20, [OldVP], addr OldVP

ret

HookWrite endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetTEXTSectionAddr proc frame

jmp GetTEXTSectionData

GetTEXTSectionAddr endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetTEXTSectionSize proc frame

jmp GetTEXTSectionData

GetTEXTSectionSize endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetTEXTSectionData proc frame Module:QWORD, DataType:DWORD

; 1 = Section's VirtualSize
; 2 = Section's VirtualAddress

; Get imagebase.
mov rcx, [Module]
xor rax, rax
; Get PE header.
mov eax, [rcx+3Ch]
add rcx, rax

.if [DataType] == 1
; Get .text section VirtualSize.
mov eax, [rcx+110h]
ret
.elseif [DataType] == 2
; Get .text section VirtualAddress.
mov eax, [rcx+114h]
; Make it an actual VirtualAddress.
add rax, [Module]
ret
.endif

xor rax, rax
ret

GetTEXTSectionData endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetRDATASectionAddr proc frame

jmp GetRDATASectionData

GetRDATASectionAddr endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetRDATASectionSize proc frame

jmp GetRDATASectionData

GetRDATASectionSize endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

GetRDATASectionData proc frame Module:QWORD, DataType:DWORD

; 1 = Section VirtualSize
; 2 = Section VirtualAddress

; Get imagebase.
mov rcx, [Module]
xor rax, rax
; Get PE header.
mov eax, [rcx+3Ch]
add rcx, rax

.if [DataType] == 1
; Get .text section VirtualSize.
mov eax, [rcx+138h]
ret
.elseif [DataType] == 2
; Get .text section VirtualAddress.
mov eax, [rcx+13Ch]
; Make it an actual VirtualAddress.
add rax, [Module]
ret
.endif

xor rax, rax
ret

GetRDATASectionData endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

BinSearch proc frame uses rsi rdi SearchAddress:QWORD, SearchLength:DWORD, BytesAddress:QWORD, BytesLength:DWORD, AddMod:DWORD, SubMod:DWORD

; Setup Search.
mov rcx, [SearchAddress]
xor rdx, rdx
; Get end of Search.
mov edx, [SearchLength]
add rdx, rcx
; Setup Bytes.
mov r8, [BytesAddress]
xor r9, r9
; Get the end of Bytes.
mov r9d, [BytesLength]
add r9, r8
; Setup the first bytes.
mov sil, byte ptr [rcx]
mov dil, byte ptr [r8]
; Reset bytes found counter.
xor eax, eax

Begin:
; Find first byte.
.while sil != dil
; Prevent overrread of Search & Bytes.
cmp rcx, rdx
jnle NothingFound
cmp r8, r9
jnle NothingFound
; Next byte.
inc rcx
; Load next byte.
mov sil, byte ptr [rcx]
.endw
; Increment bytes found counter.
inc eax
; Next bytes.
inc rcx
inc r8
; Load next bytes.
mov sil, byte ptr [rcx]
mov dil, byte ptr [r8]

; First byte found, find the rest of the bytes.
.while sil == dil
; Prevent overrread of Search & Bytes.
cmp rcx, rdx
jnle NothingFound
cmp r8, r9
jnle NothingFound
; Next bytes.
inc eax
inc rcx
inc r8
; Load next bytes.
mov sil, byte ptr [rcx]
mov dil, byte ptr [r8]
; If all bytes found, return address of which the first byte was found.
.if eax == [BytesLength]
; Rewind address.
sub rcx, rax
; Apply modifiers.
mov eax, [AddMod]
add rcx, rax
mov eax, [SubMod]
sub rcx, rax
; Return address.
mov rax, rcx
ret
.endif
.endw

; Bytes not found, reset Bytes & jump back to StepOne.
sub r8, rax ; Rewind Bytes.
xor eax, eax
inc rcx
; Load next bytes.
mov sil, byte ptr [rcx]
mov dil, byte ptr [r8]
jmp Begin

NothingFound:
xor rax, rax
ret

BinSearch endp

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

ENDIF

END