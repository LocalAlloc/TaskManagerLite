#pragma once


#include "Processes.h"
#include "resource.h"
#include "ListAll.h"

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID InitializeListViewColumns(HWND);
VOID SetListView (HWND, PProcessList);
VOID ProcessTerminator(HANDLE);
BOOL ProcessCreator(TCHAR*, TCHAR*, TCHAR*);
/* Plays audio stored as a resource. */
bool playAudio(HINSTANCE hInstance, WCHAR* lpName, UINT bFlags) {
    HANDLE hResInfo = FindResourceW(hInstance, lpName, L"WAVE");

    if (hResInfo == nullptr)
        return false;

    HANDLE hRes = LoadResource(hInstance, (HRSRC)hResInfo);

    if (hRes == nullptr)
        return false;

    WCHAR* lpRes = (WCHAR*)LockResource(hRes);
    FreeResource(hRes);

    return sndPlaySoundW(lpRes, SND_MEMORY | bFlags);
}
void RunAsSystem() {
	// Specify the command-line arguments for the process you want to run
	wchar_t commandLine[] = L"cmd.exe";

	// Create the startup info and process info structures
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	// Launch the process with elevated privileges and run it as NT AUTHORITY/SYSTEM
	if (CreateProcessWithLogonW(L"SYSTEM", L".", NULL, LOGON_WITH_PROFILE, NULL, commandLine, CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &si, &pi))
	{
		MessageBoxA(NULL, "Process started as NT AUTHORITY/SYSTEM.", NULL, MB_OK | MB_ICONINFORMATION);

		// Wait for the process to exit
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		MessageBoxA(NULL, "Failed to start process as NT AUTHORITY/SYSTEM.", NULL, MB_OK | MB_ICONSTOP);
	}
}

#pragma comment(lib, "winmm.lib")
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CreateDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK EndDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ModulesDlgProc(HWND, UINT, WPARAM, LPARAM);
VOID ShowModules(HWND);
VOID ShowHeaps(HWND);
VOID ShowThreads(HWND);

// Global Variables:
BOOL bShowWarningDlg = FALSE;
BOOL EnableAudio = FALSE;
BOOL bShowErrors = FALSE;
HINSTANCE hInst;								//current instance!
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWnd;										// Main window handler

// Averages sizes of letters in default font
static int charXSize = LOWORD (GetDialogBaseUnits());
static int charYSize = HIWORD (GetDialogBaseUnits());

// Listable things
enum ListThing {
	Modules,
	Heaps,
	Threads
};
#define ID_OPTIONS_ENABLEMUSIC          32795
// referred to the process from which you want to know modules, heaps or threads
static DWORD PID; 
// referring to what you want to know about the process
static ListThing ListWhat;

