#include <tchar.h>
#include <iostream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <easyhook.h>
#include <Psapi.h>
#include <TlHelp32.h>
#pragma comment(lib, "EasyHook64.lib")

extern int KillTaskManager();
extern bool ExtractResourceDll(int iId, LPCWSTR pDest);
extern bool ExtractResourceExe(int iId, LPCWSTR pDest);