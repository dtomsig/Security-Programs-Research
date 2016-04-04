/*******************************************************************************
* FILE: winhelper.hpp                                                          *
*                                                                              *
* DESCRIPTION: winhelper.hpp contains functions using the Windows api to assist*
* with dll injection.                                                          * 
*                                                                              *
* OUTPUT FILE: n/a                                                             *
*******************************************************************************/

#ifndef WINHELPER_HPP
#define WINHELPER_HPP

#include "windows.h"
#include "tlhelp32.h"


void findProcessAddress()
{
    
}



DWORD findProcessID(const char *processName)
{
    DWORD result = 0;
    HANDLE hProcessSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;

    if(INVALID_HANDLE_VALUE == hProcessSnap) 
        return 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          
        return 0;
    }

    do
    {
        if (strcmp(processName, pe32.szExeFile) == 0)
        {
            result = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return result;
}



void injectDLL(std::string fileName)
{
    char *dllName =  new char[fileName.length()];
    DWORD hearthStonePID; 
    HANDLE hearthstone = OpenProcess(PROCESS_ALL_ACCESS, FALSE, hearthStonePID),
           threadID;
    hearthStonePID = findProcessID("Hearthstone.exe");
    int n;
    LPVOID address, arg;
    strncpy(dllName, fileName.c_str(), sizeof(fileName));

    if(hearthstone)
        std::cout << "Hearthstone was succesfully opened." << std::endl;
    else
        std::cout << "Hearthstone was not successfully opened." << std::endl;

    address = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), 
                                            "LoadLibraryA");
    arg = (LPVOID)VirtualAllocEx(hearthstone, NULL, strlen(dllName),
    	          MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    n = WriteProcessMemory(hearthstone, arg, dllName, strlen(dllName),
                               NULL);
    threadID = CreateRemoteThread(hearthstone, NULL, 0, 
                                         (LPTHREAD_START_ROUTINE)address, arg, 
                                         0, NULL);
    CloseHandle(hearthstone);
}

#endif
