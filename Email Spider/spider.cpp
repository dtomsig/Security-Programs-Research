/*******************************************************************************
* FILE: spider.cpp                                                             *         *                                                                              *
* DESCRIPTION: spider.cpp is where the main method for emailSpider.exe lives.  *
* It defines functions that are displayed when the process is run. It also     *
* performs the core operation of spidering webpages for emails.                *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sys/socket.h>
#include <termcolor/termcolor.hpp>


void printOptions()
{
    std::cout << "\n\n\n" << std::endl;
    std::cout << termcolor::green << termcolor::bold
              << std::setw(65) 
              << "                   _ _   ______       _     _             " 
              << std::endl;
    std::cout << std::setw(65)
              << "                  (_) | / _____)     (_)   | |            "
              << std::endl;
    std::cout << std::setw(65)
              << " _____ ____  _____ _| |( (____  ____  _  __| |_____  ____ "
              << std::endl;
    std::cout << std::setw(65)
              << "| ___ |    \\(____ | | | \\____ \\|  _ \\| |/ _  | ___ |/ ___)"
              << std::endl;
    std::cout << std::setw(65)
              << "| ____| | | / ___ | | | _____) ) |_| | ( (_| | ____| |    "
              << std::endl;
    std::cout << std::setw(65)
              << "|_____)_|_|_\\_____|_|\\_|______/|  __/|_|\\____|_____)_|    "
              << std::endl;
    std::cout << std::setw(65)
              << "                               |_|                        "
              << std::endl;

    std::cout << termcolor::yellow << "\n\nCommand:" << std::endl;
    std::cout << termcolor::reset << termcolor::red << termcolor::bold 
              << termcolor::dark
              << "  ./emailSpider " << termcolor::bold << termcolor::magenta
              << "-h" << termcolor::reset << " [Host Name] " << termcolor::bold
              << termcolor::magenta << "-p" << termcolor::reset << " [Port]" 
              /* -o [Output File] 
              << "--[Options]" */<< std::endl;

    std::cout << termcolor::yellow << termcolor::bold << "\nParameters:" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::magenta
              << std::left << std::setw(15) << "  -h" << termcolor::reset
              << "The host name of the highest level of the web page." 
              << std::endl;
}


int main(int argc, char **argv)
{
    printOptions();


    int socket_desc;
    struct sockaddr_in server;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc == -1)
    {
        std::cout << "Could not create socket " << std::endl;
    }
    else
        std::cout << "Succesfully created" << std::endl;
    
    server.sin_addr.s_addr = inet_addr("173.194.121.9");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server) < 0))
        std::cout << "Error connecting to server"; 
    else
        std::cout << "Succesfully connected to server";
}
