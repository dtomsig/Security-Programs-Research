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
#include <curl/curl.h>
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
    std::string hostname, protocol, subdirectory;
    int search_depth;
    
    st_url() {}
        
    st_url(std::string h, std::string s, int d) : hostname(h), subdirectory(s),
        search_depth(d) {}	 
};

void print_options();
void find_emails(std::string &get_response, std::fstream &output_file);
void find_subdirectories(std::string &get_response, 
                         std::string &hostname,
                         std::deque<st_subdirectory> &subdirectories,
                         std::map<std::string, int> &visited_directories);
void find_urls(std::string &get_response, std::deque<st_url> &urls);
int connect(std::string hostname);
void disconnect(int socket_fd);
std::string obtain_get_repsonse(std::string &hostname, int socket_fd, 
                                st_subdirectory &subdirectories);
std::string obtain_get_response_curl(std::string &hostname, 
                                     std::string &protocol,
                                     st_subdirectory &subdirectory);
void spider_google_keyword(std::string keyword, std::fstream &output_file);
void spider_single_resource(st_url &initial_url, int max_search_depth, 
                            int num_threads, std::fstream &output_file);
void thread_get_requests_helper(std::string &hostname,
                                std::fstream &output_file,
                                std::string &protocol, 
                                std::deque<st_subdirectory> &subdirectories,
                                st_subdirectory &target_directory,
                                std::map<std::string, int> &visited_directories)
                                ;
static size_t write_call_back(void *contents, size_t size, size_t nmemb, 
                              void *userp);

#endif
