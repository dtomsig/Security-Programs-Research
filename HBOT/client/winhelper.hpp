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


void find_process_address()
{
    
}



DWORD find_process_id(const char *processName)
{
    DWORD result = 0;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
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



void inject_dll(std::string file_name)
{
    char *dll_name =  new char[file_name.length()];
    DWORD hearthstone_pid; 
    HANDLE hearthstone = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 
                                     hearthstone_pid),
           thread_id;
    hearthstone_pid = find_process_id("Hearthstone.exe");
    int n;
    LPVOID address, arg;
    
    strncpy(dll_name, file_name.c_str(), sizeof(file_name));

    if(hearthstone)
        std::cout << "Hearthstone was succesfully opened." << std::endl;
    else
        std::cout << "Hearthstone was not successfully opened." << std::endl;

    address = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"),
                                     "LoadLibraryA");
    arg = (LPVOID)VirtualAllocEx(hearthstone, NULL, strlen(dll_name),
                                 MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    n = WriteProcessMemory(hearthstone, arg, dll_name, strlen(dll_name), NULL);
    thread_id = CreateRemoteThread(hearthstone, NULL, 0, 
                                   (LPTHREAD_START_ROUTINE)address, arg, 
                                   0, NULL);
    CloseHandle(hearthstone);
    return;
}

#endif
