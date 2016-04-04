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
    int search_depth;
    
    st_subdirectory() {}
    
    st_subdirectory(std::string s, int sd) : subdirectory(s), search_depth(sd)
    {}
};

struct st_url 
{
    std::string host_name, subdirectory;
    int search_depth;
    
    st_url() {}
        
    st_url(std::string h, std::string s, int d) : host_name(h), subdirectory(s),
        search_depth(d) {}	 
};

void print_options();
void find_emails(std::string &get_response, std::fstream &output_file);
void find_subdirectories(std::string &get_response, 
                        std::string &host_name,
                        std::map<std::string, int> &visited_directories,
                        std::deque<st_subdirectory> &subdirectories);
void find_urls(std::string &get_response, std::deque<st_url> &urls);
int connect(std::string host_name);
void disconnect(int socket_fd);
std::string obtain_get_repsonse(st_subdirectory &subdirectory, 
                                std::string &host_name, int socket_fd);
void spider(std::string host_name, std::ifstream &output_file, 
            int search_depth);
void thread_get_requests_helper(std::fstream &output_file, 
                               std::map<std::string, int> &visited_directories,
                               std::deque<st_subdirectory> &subdirectories,
                               st_subdirectory &target_directory,
                               std::string &host_name);

#endif
