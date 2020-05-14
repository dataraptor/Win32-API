#ifndef __WIN32HOOK
#define __WIN32HOOK

#include "windows.h"

#define hook_len 6      // Jump Instruction is 5 byte(1 byte opcode + 4 byte data)

class Hook {
	void *srcfunc;
	void *detourfunc;
	BYTE sourceBytes[hook_len];
	DWORD sourceProtection;

	void ClearProtection(void);
	void RestoreProtection(void);

public:
	Hook();
	void Init(void *srcfunc, void *detourfunc);
	void Init(LPCWSTR module, LPCSTR funcname, void *detourfunc);
	void Clear(void);
	void Restore(void);
};


#endif
