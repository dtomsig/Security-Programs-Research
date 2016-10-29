/*******************************************************************************
* FILE: bot.hpp                                                                *
*                                                                              *
* DESCRIPTION: compatbility_layer.hpp interacts with the injected dll and      *
* calls its functions. It acts as an api and alllows for calling functions     *
* within the injected dll.                                                     *
*                                                                              *
* OUTPUT FILE: N/a                                                             *
*******************************************************************************/

#ifndef COMPATABILITY_LAYER_HPP
#define COMPATABILITY_LAYER_HPP

#include "windows.h"
#include "winhelper.hpp"


DWORD hearthstone_pid = find_process_id("Hearthstone.exe");


int obtain_server_ip_address()
{
    HANDLE hearthstone = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 
                                     hearthstone_pid);
    CloseHandle(hearthstone);
    return 0;     
}



void open_ranked_game()
{
    HANDLE hearthstone = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 
                                     hearthstone_pid);
    CloseHandle(hearthstone);                                     
    return;
}



void play_card(int c)
{
    HANDLE hearthstone = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 
                                     hearthstone_pid);
    CloseHandle(hearthstone);                                     
    return;
}

#endif
