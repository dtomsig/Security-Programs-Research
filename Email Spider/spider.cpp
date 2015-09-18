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
#include <fcntl.h>
#include <fstream>
#include <getopt.h>
#include <netdb.h>
#include <queue> 		
#include <string>
#include <sys/socket.h>
#include <termcolor/termcolor.hpp>



/*******************************************************************************
* FUNCTION: printOptions()                                                     *
*                                                                              *                                                                                     *
* DESCRIPTION: printOptions prints all of the options available to the user    *
* running ./emailSpider. It is called when --help is used as an option or      *
* when there is no input into the emailSpider program.                         *
*                                                                              *
* PARAMETERS: (None)                                                           *
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
              << termcolor::blue << "-h" << termcolor::reset 
              << " [Host Name] " << termcolor::bold << termcolor::blue 
              << "-p" << termcolor::reset << " [Port] " << termcolor::bold 
              << termcolor::blue << "-o " << termcolor::reset 
              << "[Output File] " << termcolor::bold << termcolor::blue 
              << "--" << termcolor::reset << " [Options]" << std::endl;

    std::cout << termcolor::yellow << termcolor::bold << "\nParameters:" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -h" << termcolor::reset
              << "The host name of the highest level of the web page." 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue 
              << std::left << std::setw(15) << "  -p" << termcolor::reset
              << "The port used either 443 or 80." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -o" << termcolor::reset
              << "The name of the output file for spidered email addresses." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  --help" << termcolor::reset
              << "Display the help screen." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::yellow << termcolor::bold 
              << "\nExample:" << std::endl;
    std::cout << termcolor::reset;
    std::cout << termcolor::red << termcolor::bold << "  ./emailSpider" 
              << termcolor::blue << " -h " << termcolor::reset
              << "www.google.com" << termcolor::blue << termcolor::bold 
              << " -p " << termcolor::reset <<  "80" << termcolor::blue 
              << termcolor::bold << " -o " << termcolor::reset << "emails.txt"
              << termcolor::blue << termcolor::bold << " -d " 
              << termcolor::reset << "3" << std::endl;
    std::cout << "\n" << std::endl;
}



/*******************************************************************************
* FUNCTION: parse(std::string hostName)                                        *
*                                                                              *                                                                                     *
* DESCRIPTION: parse receives .                                                *
*                                                                              *
* PARAMETERS: (-hostName: The host name that will be connected to.)            *
*******************************************************************************/

std::string* parse(std::string hostName)
{
    /*
    ** "statusAddr" refers to the status after getaddrinfo() has been used 
    **  to fill the necessary structures for connecting to the host.
    **
    ** "statusSocketConnect" refers to the status after a socket connection has
    **  been attempted
    **
    ** "socketfd" refers to the handle associated with creating a socket.
    **
    ** "len" refers to the length of the GET message to the web server.
    **
    ** "bytes_sent" refers to the number of bytes sent to the server. 
    **
    ** "bytes_received" refers to the number of bytes received from GET request.
    **
    ** "incoming_data_buffer" is an array that stores the response from the 
    **  webserver.
    **
    ** "pageSource" refers to an entire webpage that is going to be spidered.
    **
    ** "msg" refers to the GET message sent to the webserver.
    **
    ** "addrinfo" refers to the structure that contains address information in
    **  order to create a socket connection. 
    **
    ** "host_info_list" is a pointer to an addrinfo structure that is used in 
    **  the getaddrinfo() function. 
    */	
    int statusAddr, statusSocketConnect, socketfd, len;
    ssize_t bytes_sent, bytes_received;
    char incoming_data_buffer[1448];
    std::string pageSource,
                msg = ("GET / HTTP/1.1\r\nHost: " + hostName + "\r\n\r\n");
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    /* 
    ** These commands are necessary to fill the address info structures with 
    ** information associated with the particular webpage.
    */
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = PF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    statusAddr = getaddrinfo(hostName.c_str(), "80", &host_info, 
                             &host_info_list);
                             
    if(statusAddr != 0)
    {
        std::cout << "Get addr info error" << std::endl;
        return NULL;
    }
    else
        std::cout << "No getaddrinfo error" << std::endl;

    /* 
    ** These commands are necessary in order to create a socket given the 
    ** address information used above. 
    */
    socketfd = socket((*host_info_list).ai_family, 
                      (*host_info_list).ai_socktype, 
                      (*host_info_list).ai_protocol);	
    
   
    if(socketfd == -1)
    {
        std::cout << "Socket error" << std::endl;
        return NULL;
    }
    else
        std::cout << "There is no socket error" << std::endl;
       
     
        
    /* 
    ** These commands connect the socket to the web server and sends the GET 
    ** request (stored in "msg") to the webserver. It also receives the message
    ** and stores it in "incoming_data_buffer". 
    */
    statusSocketConnect = connect(socketfd, host_info_list->ai_addr, 
                                  host_info_list->ai_addrlen);
    //fcntl(socketfd, F_SETFL, O_NONBLOCK);
                                  
    if(statusSocketConnect == -1)  
    {
        std::cout << "Connect error" << std::endl;
        return NULL;
    }
    else
    {
        len = strlen(msg.c_str());
        bytes_sent = send(socketfd, msg.c_str(), len, 0);	
        bytes_received = recv(socketfd, incoming_data_buffer, 1447, 0);
        incoming_data_buffer[bytes_received] = '\0';
        pageSource += incoming_data_buffer;
    }
    std::cout << pageSource;
    close(socketfd);
} 



/*******************************************************************************
* FUNCTION: spider(std::string hostName, std::ofstream &outputFile )           *
*                                                                              *                                                                                     *
* DESCRIPTION: This function controls the logical level of spidering.          *
*                                                                              *
* PARAMETERS: (-hostName: The hostname that will be connected to.,             *
* -outPutFile: The output file stream that will contain the store the spidered *
* email addresses.)                                                            *
*******************************************************************************/

void spider(std::string hostName, std::ofstream &outputFile)
{   
    
    std::queue <std::string> urls;
    
    /* Sets the default values for 
	
	/* Attempts initial connection to determine if hostname is valid. */
    if(parse(hostName) == NULL)
    {
	    std::cout << termcolor::reset << termcolor::red 
	              << "\nError: Unable to connect to specified host: \"" 
	                 + hostName + "\"." << std::endl;	
	}
	else
	{
        urls.push(hostName);
        std::cout << "\nURL left to spider is " << urls.empty() << std::endl;
        
        /* Main spidering loop lies here. */
        while(urls.empty() != true)
        {
        
	    }
    }
    
}

int main(int argc, char **argv)
{
    /*
    ** "c" is an argument that is used in getopt_long() for parsing arguments.
    ** 
    ** "displayHelpFlag" is a flag for displaying the help menu based on what
    **  the user enters as arguments.
    **
    ** "searchDepth" represents the depth that the spider will go into when
    **  searching for emails.
    **
    ** "hostName" represents the highest level that will be spidered by the 
    **  program.
    ** 
    ** "outputFileName" represents the output file that will store all of the 
    **  spidered email addresses. 
    **
    ** "outputFile" refers to the output filestream associated with 
    **  outputFileName
    */ 
    int c, displayHelpFlag, searchDepth;
    std::string hostName, outputFileName;
    std::ofstream outputFile;

    /* 
    ** Default options are set here. These can change based upon what the user
    ** enters in as arguments. 
    */                                                                       
    hostName = "127.0.0.1";
    displayHelpFlag = 1; 
    outputFileName = "emails.txt";
    searchDepth = 3;

    /*
    ** Structure used to define what the arguments are that can be passed into
    ** the program. This is a standard structure in getopt.h
    */
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
    }
    
    /* 
    ** Determines what operation to perform based upon what was entered into
    ** the program. 
    */

    if(displayHelpFlag)
        printOptions();
    else
	{
		outputFile.open(outputFileName);
		spider(hostName, outputFile);
	}
	
	outputFile.close();

}
