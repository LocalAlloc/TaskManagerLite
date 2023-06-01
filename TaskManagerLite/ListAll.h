#pragma once

#include "stdafx.h"
#include "Processes.h"
#include <unordered_set>
#include <string>
#include <sstream>

using namespace std;

//static set<string> sDeviceDrivers;
//std::set<std::string> sDeviceDrivers; // Update the container type to store strings
//std::set<std::string> sDeviceDrivers;
extern std::set<std::string> sDeviceDrivers; // Update the container type to store strings

// Helper function to convert TCHAR to std::string
std::string ConvertTCHARToString(const TCHAR* tcharString);

#define ReserveSize kb(1)

BOOL GetProcessList(HWND, PProcessList);
VOID FreeProcessList(PProcessList);

BOOL ListDeviceDrivers(HWND);

BOOL ListProcessModules(HWND, DWORD, PModuleList);
VOID FreeListProcessModules(PModuleList);

BOOL ListProcessHeaps(HWND, DWORD, PHeapList);
VOID FreeListProcessHeaps(PHeapList);

BOOL ListProcessThreads(HWND, DWORD, PThreadList);
VOID FreeListProcessThreads(PThreadList);

VOID printError(HWND, TCHAR*);