/*******************************************************************************
* FILE: inject.cpp                                                             *
*                                                                              *
* DESCRIPTION: inject.cpp contains methods for interacting with Hearthstone.   *   
*                                                                              *
* OUTPUT FILE: inject.dll                                                      *
*******************************************************************************/

#include <iostream>
#include "inject.hpp"
#include "windows.h"


void open_ranked_game()
{
    std::cout << "OPENING" << std::endl;
    asm
    (
        ".intel_syntax noprefix;\n\t"
        "mov       eax, 1;"
    );
    return;
}



int obtain_server_ip_address()
{
    
    
}



void play_card(int c)
{

}
