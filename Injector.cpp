#include "windows.h"
#include <stdio.h>

// IMPORTANT: Update this path to the location where you build your DLL!
// Use a literal string to ensure it includes the null terminator '\0'
#define DLL_PATH "C:\\Users\\Amdocs\\source\\repos\\myDLL\\x64\\Debug\\myDLL.dll"

int main()
{
	// Calculate the string length and add 1 for the null terminator
	INT PATH_SIZE = strlen(DLL_PATH) + 1;
	int err = 0;

	// IMPORTANT: Update this to the PID of the target process you want to inject into
	int target_pid = 10072;

	// 1. Get the address of LoadLibraryA in kernel32.dll
	LPVOID addrLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (NULL == addrLoadLibrary) {
		printf("Error: Could not find LoadLibraryA address.\n");
		return 1;
	}

	// 2. Open the target process
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, target_pid);
	if (NULL == proc) {
		printf("Error: Could not open process with PID %d.\n", target_pid);
		err = GetLastError();
		return 1;
	}

	// 3. Allocate memory inside the target process for the DLL path string
	PVOID memAddr = (PVOID)VirtualAllocEx(proc, NULL, PATH_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (NULL == memAddr) {
		printf("Error: Could not allocate remote memory.\n");
		err = GetLastError();
		CloseHandle(proc);
		return 1;
	}

	// 4. Write the DLL path string into the allocated remote memory
	DWORD bytesWritten = 0;
	if (!WriteProcessMemory(proc, memAddr, DLL_PATH, PATH_SIZE, &bytesWritten) || bytesWritten != PATH_SIZE) {
		printf("Error: Could not write to remote memory. Bytes written: %lu\n", bytesWritten);
		err = GetLastError();
		VirtualFreeEx(proc, memAddr, 0, MEM_RELEASE);
		CloseHandle(proc);
		return 1;
	}

	// 5. Create a remote thread that executes LoadLibraryA(DLL_PATH)
	HANDLE hRemote = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, memAddr, 0, NULL);
	if (NULL == hRemote) {
		printf("Error: Could not create remote thread.\n");
		err = GetLastError();
		VirtualFreeEx(proc, memAddr, 0, MEM_RELEASE);
		CloseHandle(proc);
		return 1;
	}

	// Wait for the thread to finish loading the DLL
	WaitForSingleObject(hRemote, INFINITE);

	// Clean up handles and remote memory
	CloseHandle(hRemote);
	VirtualFreeEx(proc, memAddr, 0, MEM_RELEASE);
	CloseHandle(proc);

	printf("DLL Injection successful!\n");
	return 0;
}