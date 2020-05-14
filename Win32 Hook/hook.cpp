#include "hook.h"

Hook::Hook() {
	this->srcfunc = 0;
	this->detourfunc = 0;
}

void Hook::Init(void *srcfunc, void *detourfunc) {
	this->srcfunc = srcfunc;   // Store the api func. So that the bytes can be restored after detour function called
	this->detourfunc = detourfunc;
	
	// Hook detour function to the source function location
	Restore();
}

void Hook::Init(LPCWSTR module, LPCSTR funcname, void *detourfunc) {
	this->srcfunc = GetProcAddress(GetModuleHandle(module), funcname);   // Store the api func. So that the bytes can be restored after detour function called
	this->detourfunc = detourfunc;
	
	// Hook detour function to the source function location
	Restore();
}


void Hook::ClearProtection(void) {
	DWORD currprotection;
	VirtualProtect(this->srcfunc, hook_len, PAGE_EXECUTE_READWRITE, &currprotection);  // unlock memory to write in source func region
	this->sourceProtection = currprotection;
}

void Hook::RestoreProtection(void) {
	DWORD temp;
	VirtualProtect(srcfunc, hook_len, this->sourceProtection, &temp);
}


void Hook::Clear(void) {
	ClearProtection();  // Unlock Memory
	memcpy(this->srcfunc, this->sourceBytes, hook_len);
	RestoreProtection();   // Reset original protection
}

void Hook::Restore(void) {
	ClearProtection();   // Get Read Write Access to Source Location

	memcpy(sourceBytes, srcfunc, hook_len);  // store source func instruction safely to restore later
	memset(srcfunc, 0x90, hook_len);	// 0x90 for No Operation

	DWORD relativeaddr = ((DWORD)detourfunc - (DWORD)srcfunc) - 5;  // size of jump instruction if 4 byte
	*(BYTE*)srcfunc = 0xE9;   // jump instruction
	*(DWORD*)((DWORD)srcfunc + 1) = relativeaddr;  // Set Offset to our function

	RestoreProtection();   // Reset original protection
}

