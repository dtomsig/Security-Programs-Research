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
** "st_subDirectory" represents an entire subdirectory that will be used for
** spidering. No hostname is associated with the subdirectory structure.
** 
** "st_url" represents an entire page that will be used for spidering. It 
** contains a hostname (i.e. www.google.com) and a subdirectory/file within that 
** larger page.
*/

struct st_subdirectory
{
    std::string subdirectory;
    int searchDepth;
    
    st_subdirectory() {}
    
    st_subdirectory(std::string s, int sd) : subdirectory(s), searchDepth(sd) {}
};

struct st_url 
{
    std::string hostName, subdirectory;
    int searchDepth;
    
    st_url() {}
        
    st_url(std::string h, std::string s, int d) : hostName(h), subdirectory(s),
        searchDepth(d) {}	 
};

void printOptions();
void findEmails(std::string &getResponse, std::fstream &outputFile);
void findSubdirectories(std::string &getResponse, 
                        std::string &hostName,
                        std::map<std::string, int> &visitedDirectories,
                        std::deque<st_subdirectory> &subdirectories);
void findUrls(std::string &getResponse, std::deque<st_url> &urls);
int connect(std::string hostName);
void disconnect(int socketfd);
std::string obtainGetResponse(st_subdirectory &subdirectory, 
                              std::string &hostName, int socketfd);
void spider(std::string hostName, std::ifstream &outputFile, int searchDepth);
void threadGetRequestsHelper(std::fstream &outputFile, 
                             std::map<std::string, int> &visitedDirectories,
                             std::deque<st_subdirectory> &subdirectories,
                             st_subdirectory &targetDirectory,
                             std::string &hostName);

#endif
