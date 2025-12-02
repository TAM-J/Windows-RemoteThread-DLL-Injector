#include <windows.h>

// Define DLL_EXPORT before including the header to ensure __declspec(dllexport) is used
#define DLL_EXPORT
#include "DllHeader.h"

// The main entry point for the DLL
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.
        // Disable thread library calls to prevent race conditions during injection
        DisableThreadLibraryCalls((HMODULE)hModule);
        
		// Execute payload logic (e.g., show a message box for proof-of-concept)
		MessageBoxA(NULL, (LPCSTR)"i was here\n", (LPCSTR)"DLL Injection Example", MB_DEFBUTTON2);
		break;
	case DLL_THREAD_ATTACH:
		// A thread is being created in the host process.
		break;
	case DLL_THREAD_DETACH:
		// A thread is exiting.
		break;
	case DLL_PROCESS_DETACH:
		// A process is unloading the DLL (e.g., process exiting or FreeLibrary is called).
		break;
	}
	return TRUE;
}

// Implementation of the exported function (Currently unused, but good practice)
DECLDIR void Share()
{
    // Add logic here if you want to export functions for the host process to call
}