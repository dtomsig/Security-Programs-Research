/*******************************************************************************
* FILE: bot.cpp                                                                *
*                                                                              *
* DESCRIPTION: bot.cpp contains the main bot program.                          *
*                                                                              *
* OUTPUT FILE: hBot.exe                                                        *
*******************************************************************************/

#include <iostream>
#include "compatability_layer.hpp"
#include "winhelper.hpp"

void init()
{
    inject_dll("inject.dll");
    return;
}



void run_bot_loop()
{
    open_ranked_game();
    return;
}



int main()
{
    std::cout << "Welcome to HBOT." << std::endl;
    init();
    run_bot_loop();
    return 0;
}
