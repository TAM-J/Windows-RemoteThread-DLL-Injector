# Windows-RemoteThread-DLL-Injector

A simple proof-of-concept for **DLL Injection** on Windows using the `CreateRemoteThread` method with `LoadLibraryA`.

## ⚠️ Disclaimer

**This project is for educational and ethical research purposes only.** DLL injection can be used for both legitimate purposes (e.g., debugging, security tools) and malicious purposes. Misuse of this code is strictly the user's responsibility.

## ⚙️ How It Works

The injection process is handled by the `Source1.cpp` executable:

1.  **Finds** the address of the Windows API function `LoadLibraryA`.
2.  **Opens** the target process specified by its Process ID (PID).
3.  **Allocates** memory within the target process's address space.
4.  **Writes** the full path of the malicious/utility DLL (`myDLL.dll`) into the allocated memory.
5.  **Executes** `LoadLibraryA` in the context of the target process using `CreateRemoteThread`, passing the memory address of the DLL path as the argument. This forces the target process to load the DLL.

The injected DLL (`DllEntry.cpp` / `myDLL.dll`) simply shows a **MessageBox** when it attaches to the process (`DLL_PROCESS_ATTACH`), confirming the injection was successful.

## 🛠️ Requirements

* A Windows environment.
* A C++ compiler (e.g., Visual Studio).

## 🚀 Usage

1.  **Compile the DLL:** Compile `DllEntry.cpp` and `DllHeader.h` as a **Dynamic Link Library (DLL)** named `myDLL.dll` and place it in the path defined in `injector.cpp` (currently `C:\\Users\\Amdocs\\source\\repos\\myDLL\\x64\\Debug\\myDLL.dll`).
2.  **Compile the Injector:** Compile `Injector.cpp` as an executable (e.g., `injector.exe`).
3.  **Identify Target:** Find the Process ID (PID) of a running process (e.g., a simple notepad.exe or calc.exe).
4.  **Update PID:** Change the hardcoded PID in `Injector.cpp` to your target's PID.
    ```c
    int process = 10072; // <-- Change this PID
    ```
5.  **Run:** Execute `injector.exe`.

If successful, the target process will display a message box with the text "i was here".
