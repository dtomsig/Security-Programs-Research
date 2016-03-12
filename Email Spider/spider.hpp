/*******************************************************************************
* FILE: spider.hpp                                                             *
*                                                                              *                                                                                     *
* DESCRIPTION: spider.hpp is the header file for spider.cpp. It is mainly used *
* so that spider.cpp's functions can be tested.                                *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#ifndef SPIDER_HPP
#define SPIDER_HPP

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <netdb.h>
#include <pthread.h>
#include <queue> 
#include <regex>
#include <sstream>		
#include <string>
#include <sys/socket.h>
#include <termcolor/termcolor.hpp>

/*
** GLOBAL VARIABLES:
** 
** "url" represents an entire page that will be used for spidering. It contains
** a hostname (i.e. www.google.com) and a subdirectory/file within that larger 
** page.
*/

struct url 
{
    std::string hostName, subDirectory;
    int searchDepth;
    
    url() {}
        
    url(std::string h, std::string s, int d) : hostName(h), subDirectory(s),
        searchDepth(d) {}	  
};

void printOptions();
void findEmails(std::string &getResponse, std::fstream &outputFile);
void findUrls(std::string &getResponse, std::deque<url> &urls);
int connect(std::string hostName);
void disconnect(int socketfd);
std::string obtainGetResponse(std::string url, int socketfd);
void spider(std::string hostName, std::ifstream &outputFile, int searchDepth);

#endif
