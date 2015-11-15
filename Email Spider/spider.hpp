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
#include <queue> 
#include <regex>
#include <sstream>		
#include <string>
#include <sys/socket.h>
#include <termcolor/termcolor.hpp>

struct url;
void printOptions();
void findEmails(std::string &getResponse, std::fstream &outputFile);
void findUrls(std::string &getResponse, std::queue<url> &urls, 
              int currentDepth);
int connect(std::string hostName);
void disconnect(int socketfd);
std::string obtainGetResponse(std::string url, int socketfd);
void spider(std::string hostName, std::ifstream &outputFile, int searchDepth);

#endif
