/*******************************************************************************
* FILE: spider.cpp                                                             *
*                                                                              *                                                                                     *
* DESCRIPTION: spider.cpp is where the main method for emailSpider.exe lives.  *
* It defines functions that are displayed when the process is run. It also     *	
* performs the core operation of spidering webpages for emails.                *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <getopt.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <termcolor/termcolor.hpp>


/*******************************************************************************
* FUNCTION: printOptions()                                                     *
*                                                                              *                                                                                     *
* DESCRIPTION: printOptions() prints all of the options available to the user  *
* running ./emailSpider. It is called when --help is used as an option or      *
* when there is no input into the emailSpider program.                         *
*                                                                              *
* PARAMETERS: (none)                                                           *
*******************************************************************************/
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
              << "| ___ |    \\(____ | | | \\____"
              " \\|  _ \\| |/ _  | ___ |/ ___)" 
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
              << "  ./emailSpider " << termcolor::reset << termcolor::bold
              << termcolor::magenta << "-h" << termcolor::reset 
              << " [Host Name] " << termcolor::bold << termcolor::magenta 
              << "-p" << termcolor::reset << " [Port] " << termcolor::bold 
              << termcolor::magenta << "-o " << termcolor::reset 
              << "[Output File] " << termcolor::bold << termcolor::magenta 
              << "--" << termcolor::reset << " [Options]" << std::endl;

    std::cout << termcolor::yellow << termcolor::bold << "\nParameters:" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::magenta
              << std::left << std::setw(15) << "  -h" << termcolor::reset
              << "The host name of the highest level of the web page." 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::magenta
              << std::left << std::setw(15) << "  -p" << termcolor::reset
              << "The port used either 443 or 80." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::magenta
              << std::left << std::setw(15) << "  -o" << termcolor::reset
              << "The name of the output file for spidered email addresses." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::magenta
              << std::left << std::setw(15) << "  --help" << termcolor::reset
              << "Display the help screen." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::yellow << termcolor::bold 
              << "\nExample:" << std::endl;
    std::cout << termcolor::reset << std::endl;
}

void connect()
{
    int status;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;

    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("www.google.com", "80", &host_info, &host_info_list);
 
    if(status)
        std::cout << "The status is connected" << std::endl;
    else
        std::cout << "The status is not connected" << std::endl;

    /* Creates a socket. */
    int socketfd;
    socketfd = socket((*host_info_list).ai_family, 
                      (*host_info_list).ai_socktype, 
                      (*host_info_list).ai_protocol);

    if(socketfd == -1)
        std::cout << "Socket error" << std::endl;
    else
        std::cout << "There is no socket error" << std::endl;
}

int main(int argc, char **argv)
{
	/*
	** "c" is an argument that is used in getopt_long() for parsing arguments.
	** "displayHelpFlag" is a flag for displaying the help menu based on what
	**  the user enters as arguments.
	** "searchDepth" represents the 
	**
	*/ 
	
    int c, displayHelpFlag, searchDepth;
    std::string hostName, outputFileName;
    
    /* 
    ** Default options are set here. These can change based upon what the user
    ** enters in as arguments. 
    */                                                                       
    hostName = "127.0.0.1";
    displayHelpFlag = 1; 
    outputFileName = "default.txt";
    searchDepth = 3;

    /* Structure used to define what the arguments are. */
    static option longopts[] = 	
    { 
        {       "help",              no_argument,   &displayHelpFlag,     1},
        {            0,        required_argument,                  0,   'h'},
        {            0,        optional_argument,                  0,   'o'},         
        {            0,                        0,                  0,     0}
    };

    /* 
    ** Cycles through argv to obtain what the arguments are that were passed 
    ** in. 
    */
    while ((c = getopt_long(argc, argv, "h:o:", longopts, 0)) != -1) 
    {
        switch (c) 
        {
            case('h'):
                std::cout << "GWAS";
                hostName = optarg;
                displayHelpFlag = 0;
                break;
            case('o'):
                outputFileName = optarg;
                displayHelpFlag = 0;
                break;
            case('?'):
                displayHelpFlag = 0;
        }
    };
    std::cout << "Hostname is " << hostName << "and outputfile name is "
              << outputFileName;

    /* 
     * Determines what operation to perform based upon what was entered into
     * the program. 
     */

    if(displayHelpFlag)
        printOptions();
    else
    {
	    
    }


}
