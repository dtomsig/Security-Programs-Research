/*******************************************************************************
* FILE: spider.cpp                                                             *
*                                                                              *
* DESCRIPTION: spider.cpp is where the main method for emailSpider.exe lives.  *
* It performs the core operation of spidering webpages for emails.             *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#include <map>
#include "spider.hpp"
#include "test.hpp"
#include <thread>

/*******************************************************************************
* FUNCTION: connect(std::string hostname)                                      *
*                                                                              *
* DESCRIPTION: connect attempts to extablish a tcp connection with the         *
* top-level hostname. If it is unsuccessful, -1 is returned. Otherwise, the    *
* file descriptor is returned after a connection.                              *
*                                                                              *
* PARAMETERS:      -hostname: The host name that will be connected to.         *
*******************************************************************************/

int connect(std::string hostname)
{
    int socket_fd, status_addr, status_socket_connect;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    /* 
    ** These commands are necessary to fill the address info structures with 
    ** information associated with the particular webpage.
    */
    hostname.erase(0, 4);
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    status_addr = getaddrinfo(hostname.c_str(), "80", &host_info, 
                              &host_info_list);
    if(status_addr != 0)
    {
        std::cout << termcolor::red << "Error: Get addr info error." 
                  << std::endl;
        return -1;
    }
    else
        std::cout << "No getaddrinfo error" << std::endl;
            
    /* 
    ** These commands are necessary in order to create a socket given the 
    ** address information used above. 
    */
    socket_fd = socket((*host_info_list).ai_family, 
                       (*host_info_list).ai_socktype, 
                       (*host_info_list).ai_protocol);	
                         
    if(socket_fd == -1)
    {
        std::cout << "Socket error" << std::endl;
        return -1;
    }
    else
        std::cout << "There is no socket error" << std::endl;
       
    /* These commands connect the socket to the web server. */
    status_socket_connect = connect(socket_fd, (*host_info_list).ai_addr, 
                                    (*host_info_list).ai_addrlen);
                                  
    if(status_socket_connect == -1)  
    {
        std::cout << "Connect error" << std::endl;
        return -1;
    }
    
    return socket_fd;
}



/*******************************************************************************
* FUNCTION: disconnect(int socket_fd)                                          *
*                                                                              *
* DESCRIPTION: disconnect closes the socket specified by socket_fd.            *
*                                                                              *
* PARAMETERS:     -socket_fd: The file descriptor of the socket that will be   *
*                             closed.                                          *
*******************************************************************************/

void disconnect(int socket_fd)
{
    close(socket_fd);
    return;
}



/*******************************************************************************
* FUNCTION: find_emails(std::string &get_response, std::fstream &output_file)  *
*                                                                              *
* DESCRIPTION: find_emails receives a raw GET response and writes the emails   *
* directly to an output file stream.                                           *
*                                                                              *
* PARAMETERS:   -get_response: The GET response from the HTTP server that will *
*                              be used to obtain email addresses.              *
*                -output_file: The ouptut file that will contain spidered email*
*                              addresses.                                      *
*******************************************************************************/

void find_emails(std::string &get_response, std::fstream &output_file)
{
    std::regex email_format("(?<=:[)[a-zA-Z0-9.]+@[a-zA-Z0-9]+(\\.com|"
                            "\\.com.pa|\\.co.uk|\\.de|\\.it|\\.gov|\\.net|"
                            "\\.org|\\.us)(?=\])");
    std::regex_iterator<std::string::iterator> iterator(get_response.begin(), 
                                                        get_response.end(), 
                                                        email_format);
    std::regex_iterator<std::string::iterator> iterator_end;

    while(iterator != iterator_end)
    {
        output_file << (*iterator).str() << "\n";
        iterator++;
    }
    return;
}



/*******************************************************************************
* FUNCTION: find_subdirectories(std::string &get_response,                     *
*                               std::string &hostname,                         *
*                               std::deque<st_subdirectory> &subdirectories,   *
*                               std::map<std::string, int>                     *
*                                   &visited_directories)                      *
*                                                                              *
* DESCRIPTION: find_subdirectories receives a raw GET response and adds the    *
*              subdirectories found to the deque of subdirectories to parse.   *
*                                                                              *
* PARAMETERS:   -get_response: The GET response from the HTTP server that will *
*                              be used to hostname email addresses.            *
*                   -hostname: The current hostname that is being spidered.    *
*             -subdirectories: The current subdirectories awaiting visiting.   *
*        -visited_directories: Directories already visited not to be added.    *
*******************************************************************************/

void find_subdirectories(std::string &get_response, std::string &hostname,
                         std::deque<st_subdirectory> &subdirectories,
                         std::map<std::string, int> &visited_directories)
{
    int current_depth;
    std::regex subdirectory_format("href=\"(\\S(?!;|.css|.gif|.img|.png))"
                                   "*[/]*\"");
    std::regex_iterator<std::string::iterator> iterator(get_response.begin() , 
                                                        get_response.end()   , 
                                                        subdirectory_format);
    std::regex_iterator<std::string::iterator> iterator_end;
    std::string subdirectory;
    
    current_depth = subdirectories.front().search_depth;

    /* 
    ** Searches through the get response for subdirectories and adds them to the 
    ** subdirectory queue. 
    */
    while(iterator != iterator_end) 
    {
        subdirectory = (*iterator).str().substr(
                           (*iterator).str().find("\"") + 1, 
                           (*iterator).str().size() - 2 - 
                           (*iterator).str().find("\""));
        if(subdirectory.substr(0,4) == "http")
        {
            if(subdirectory.find(hostname) == std::string::npos)
            {
                iterator++;
                continue;
            }
            subdirectory = subdirectory.substr
                              (std::min(subdirectory.find("/", 10), 
                              subdirectory.size()), 
                              std::min(subdirectory.size() - 
                                  subdirectory.find("/", 10), 
                              subdirectory.size()));
        }
        if(subdirectory[0] != '/')
            subdirectory = '/' + subdirectory;
        if(!visited_directories.count(subdirectory))
        {
            std::cout << "added: " << subdirectory << std::endl;
            subdirectories.push_back(st_subdirectory(subdirectory, 
                                        current_depth + 1));
            visited_directories.insert(std::pair<std::string, int>(subdirectory
                                                                   , 1));
                                                 
        }
        iterator++;
    }
    return;
}



/*******************************************************************************
* FUNCTION: find_urls(std::string &get_response, std::deque<st_url> &urls)     *
*                                                                              *
* DESCRIPTION: find_urls receives a raw GET response and modifies a deque of   *
* URLS to hold additional URLs for spidering purposes.                         *
*                                                                              *
* PARAMETERS:  -get_response: The GET response from the HTTP server that will  *
*                             be used to obtain email addresses.               *  
*                      -urls: The deque of URLS that will be added to.         *
*******************************************************************************/

void find_urls(std::string &get_response, std::deque<st_url> &urls)
{
    int current_depth, pos_beginning_hostname, pos_end_hostname; 
    std::regex url_format("htt(p|ps)://[a-zA-Z0-9]+.[a-zA-Z0-9]+.(.com|.com.pa|"
                          ".co.uk|.de|.it|.gov|.net|.org|.us)[^\"']*");
    std::regex_iterator<std::string::iterator> iterator(get_response.begin(), 
                                                        get_response.end(), 
                                                        url_format);
    std::regex_iterator<std::string::iterator> iterator_end;
    std::string hostname, subdirectory = "";
    
    current_depth = urls.front().search_depth;
    
    /* Searches through the GET response for URLS and adds them to the deque. */
    while(iterator != iterator_end) 
    {
        pos_beginning_hostname = ((*iterator).str()).find("//") + 2; 
        pos_end_hostname = std::min(((*iterator).str()).find("/", 
                                         pos_beginning_hostname),
                                    (*iterator).str().size());
        hostname = (*iterator).str().substr(pos_beginning_hostname, 
                                            pos_end_hostname - 
                                            pos_beginning_hostname);
        subdirectory = (*iterator).str().substr(pos_end_hostname, 
                                                - pos_end_hostname);
                                                    
        /* Adjustments for input that doesn't match hostname format. */                                        
        if(subdirectory[subdirectory.size() - 1] == '/')
            subdirectory.pop_back();
        if(subdirectory == "")
            subdirectory = "/";
        if(hostname[0] != 'w' && hostname[1] != 'w' && hostname[2] !='w')
            hostname = "www." + hostname;
        urls.push_back(st_url(hostname, subdirectory, current_depth + 1));
        iterator++;
    }
    return;    
}



/*******************************************************************************
* FUNCTION: obtain_get_response(st_subdirectory &subdirectory,                 *
*                               std::string &hostname, int socket_fd)          *
*                                                                              *
* DESCRIPTION: obtain_get_response attempts to send a GET request to the server*
*              and receive a response.                                         *
*                                                                              *
* PARAMETERS:                                                                  *
*                  -hostname: The hostname that is currently connected.        *
*                 -socket_fd: The handle to the current socket file descriptor.*
*             -sub_directory: The struct subdirectory that will be spidered.   *
*******************************************************************************/

std::string obtain_get_response(std::string &hostname, int socket_fd, 
                                st_subdirectory &subdirectory)
{
    char incoming_data_buffer[1448];
    int len, content_length;
    ssize_t bytes_sent, bytes_received;
    std::string page_source, msg = ("GET " + subdirectory.subdirectory  
                                    + " HTTP/1.0\r\nHost: " +  
                                    hostname + "\r\nConnection: Keep-Alive" + 
                                    "\r\n\r\n");
                                    
    len = strlen(msg.c_str());
    bytes_sent = send(socket_fd, msg.c_str(), len, 0);	
    
    /* 
    ** Receives data from the socket until the socket has no more data. The data
    ** is added to the page source string. 
    */
    while((bytes_received = recv(socket_fd, incoming_data_buffer, 1447, 0)))
    {
        std::cout << "got some data" << std::endl;
        incoming_data_buffer[bytes_received] = '\0';
        page_source += incoming_data_buffer;      
    }
    std::cout << "\n\n" + page_source + "\n\n";
    return page_source;
} 



/*******************************************************************************
* FUNCTION: obtain_get_response_curl(st_subdirectory &subdirectory,            *
*                                    std::string &hostname, int socket_fd)     *
*                                                                              *
* DESCRIPTION: obtain_get_response attempts to send a GET request to the server*
*              and receive a response.                                         *
*                                                                              *
* PARAMETERS:                                                                  *
*                 -hostname: The hostname that is currently connected.         *
*                 -protocol: The protocol that is used to acquire the resource *
*                            of the subdirectory.                              *
*            -sub_directory: The struct subdirectory that will be spidered.    *
*******************************************************************************/

std::string obtain_get_response_curl(std::string &hostname, 
                                     std::string &protocol,
                                     st_subdirectory &subdirectory)
{
    CURL *curl;
    CURLcode res;
    std::string get_response;
    curl = curl_easy_init();
    
    std::cout << "SPIDERING " << protocol + hostname + 
                 subdirectory.subdirectory + "\n";
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, (protocol + hostname +
                         subdirectory.subdirectory).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &get_response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return get_response;
}



/*******************************************************************************
* FUNCTION: print_options()                                                    *
*                                                                              *
* DESCRIPTION: print_options prints all of the options available to the user   *
* running ./emailSpider. It is called when --help is used as an option.        *
*                                                                              *
* PARAMETERS: None                                                             *
*******************************************************************************/

void print_options()
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
    std::cout << termcolor::yellow << "\n\nGeneral Information:\n" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::green 
              << "     emailSpider has two modes: \n\n(1)." << termcolor::reset 
              << termcolor::reset << " Mode #1 (" << termcolor::reset 
              << termcolor::blue <<  "single_resource_search" 
              << termcolor::reset << termcolor::reset << ") searches a single "
              << "website. Use the " << termcolor::bold << termcolor:: blue 
              << "-u " << termcolor::reset << "\n     flag to specify the " 
              << "top-level domain of the resource. Do not use the " 
              << termcolor::bold << termcolor::blue << "\n     -k " 
              << termcolor::reset << "flag in a single_resource_search. " 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::green 
              << "\n\n(2)." << termcolor::reset 
              << termcolor::reset << " Mode #2 (" << termcolor::reset 
              << termcolor::blue <<  "google_keyword_search" 
              << termcolor::reset << termcolor::reset << ") searches google "
              << "for a list of urls \n     based on an inputted keyword. Use "
              << "the " << termcolor::bold << termcolor:: blue 
              << "-k " << termcolor::reset << " flag to specify the keyword " 
              << "\n     used to find urls in google. Do not use the " 
              << termcolor::bold << termcolor::blue << "-u " << termcolor::reset 
              << "flag in a \n     google_keyword_search." << std::endl; 
    std::cout << termcolor::yellow << termcolor::bold << "\n\nCommand:" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::red << termcolor::bold 
              << "  ./emailSpider " << termcolor::reset << termcolor::bold 
              << termcolor::blue 
              << "-p" << termcolor::reset << " [Port] " << termcolor::bold 
              << termcolor::blue << "-d " << termcolor::reset
              << "[Search Depth] " << termcolor::bold << termcolor::blue
              << termcolor::blue << "-k " << termcolor::reset 
              << "[Keyword] " << termcolor::bold << termcolor::blue
              << termcolor::blue << "\n                -o " << termcolor::reset 
              << "[Output File] " << termcolor::bold << termcolor::blue 
              << "-t" << termcolor::reset  
              << " [Number Threads] " << termcolor::bold << termcolor::blue 
              << termcolor::bold << termcolor::blue << "-u" << termcolor::reset 
              << " [Fully Qualified URL] " << "\n                --" 
              << termcolor::reset << " [Options] " << std::endl;
    std::cout << termcolor::yellow << termcolor::bold 
              << "\nParameters:" << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -k" << termcolor::reset
              << "The keyword used for a google keyword search." 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -o" << termcolor::reset
              << "The name of the output file for spidered email addresses." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue 
              << std::left << std::setw(15) << "  -p" << termcolor::reset
              << "The port used either 443 or 80." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue 
              << std::left << std::setw(15) << "  -t" << termcolor::reset
              << "Number of threads to be used in the program." 
              << std::endl; 
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -u" << termcolor::reset
              << "The fully qualified URL of the highest level of the web \n " 
              << "              resource you want to spider in a single " 
              << "web resource search." << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  --help" << termcolor::reset
              << "Display the help screen." 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  --test" << termcolor::reset
              << "Display the results of function testing." 
              << std::endl;
    std::cout << termcolor::reset;
    std::cout << termcolor::reset << termcolor::yellow << termcolor::bold 
              << "\nExamples:" << std::endl;
    std::cout << termcolor::reset;
    std::cout << termcolor::red << termcolor::bold << "  ./emailSpider" 
              << termcolor::blue << " -u " << termcolor::reset
              << "https://www.google.com" << termcolor::blue << termcolor::bold 
              << " -p " << termcolor::reset <<  "80" << termcolor::blue 
              << termcolor::bold << " -o " << termcolor::reset << "emails.txt"
              << termcolor::blue << termcolor::bold << " -d " 
              << termcolor::reset << "3" << std::endl;
    std::cout << termcolor::red << termcolor::bold << "  ./emailSpider" 
              << termcolor::blue << " -k " << termcolor::reset
              << "bengals" << termcolor::blue << termcolor::bold 
              << " -p " << termcolor::reset <<  "80" << termcolor::blue 
              << termcolor::bold << " -o " << termcolor::reset << "emails.txt"
              << termcolor::blue << termcolor::bold << " -d " 
              << termcolor::reset << "3" << std::endl;
    std::cout << "\n" << std::endl;
    return;
}



void spider_google_keyword(std::string keyword, std::fstream &output_file)
{
    std::cout << "YO" << std::endl;     
}



/*******************************************************************************
* FUNCTION: spider_single_resource(st_url &initial_url,                        *
*                                  std::fstream &output_file,                  *
*                                  int max_search_depth, int num_threads)      *
*                                                                              *
* DESCRIPTION: This function controls the logical level of spidering.          *
*                                                                              *
* PARAMETERS:   -initial_url: The initial url that will be connected to.       *
*          -max_search_depth: The highest search depth that will be spidered.  *
*               -num_threads: The number of threads the program will use.      *
*               -output_file: The output file stream that will contain the     *
*                             the spidered  email addresses.                   *
*******************************************************************************/

void spider_single_resource(st_url &initial_url, int max_search_depth, 
                            int num_threads, std::fstream &output_file)
{   
    int current_socket_fd, thread_counter;
    std::deque<st_subdirectory> subdirectories; 
    std::map<std::string, int> visited_directories; 
    std::string current_host, get_response;
    std::vector<std::thread> get_request_threads;
    
    /* Initializiation commands as the spidering process begins. */
    current_host = initial_url.hostname;
    get_request_threads.reserve(num_threads);    
    subdirectories.push_back(st_subdirectory(initial_url.subdirectory, 
                             initial_url.search_depth));
                             
	/* Attempts initial connection to determine if hostname is valid. */
    if((current_socket_fd = connect(initial_url.hostname)) && 
                            current_socket_fd == -1)
    {
	    std::cout << termcolor::reset << termcolor::red 
	              << "\nError: Unable to connect to specified host: " 
	                 + initial_url.hostname << ".\n" << std::endl;	
        disconnect(current_socket_fd);
	}
	else
	{                       
        /* Main spidering logic controller is here. */
        while(!subdirectories.empty())
        {              
            /* Spawns threads that won't exceed the number of max threads. */ 
            for(thread_counter = 0; thread_counter < num_threads 
                && thread_counter < subdirectories.size(); thread_counter++)
            {    
                visited_directories.insert(
                    std::pair<std::string, int>(subdirectories[thread_counter]
                        .subdirectory, 1));
                get_request_threads.emplace_back(
                    std::thread(thread_get_requests_helper,
                    std::ref(initial_url.hostname),
                    std::ref(output_file),
                    std::ref(initial_url.protocol),
                    std::ref(subdirectories),
                    std::ref(subdirectories[thread_counter]),
                    std::ref(visited_directories)));
            }
            
            /* Rejoins the threads and frees memory used for threads. */
            for(auto &t: get_request_threads)
            {
                t.join();
                subdirectories.pop_front();
            }

            /* Erases the threads from the vector at the end of execution. */
            get_request_threads.erase(get_request_threads.begin(), 
                                      get_request_threads.begin() + 
                                        thread_counter);
        }
    }
    return;
}



/*******************************************************************************
* FUNCTION: thread_get_requests_helper(std::string &hostname,                  *
*                                       std::fstream &output_file,             *
*                                       std::string &protocol,                 *
*                                       std::deque<st_subdirectory>            *
*                                            &subdirectories,                  *
*                                       st_subdirectory &target_directory,     *
*                                       std::map<std::string, int>             *
*                                            &visited_directories)             *
*                                                                              *
* DESCRIPTION: thread_get_requests_helper is a helper function for the         *
*              spidering function.                                             *
*                                                                              *
* PARAMETERS-      -hostname: The hostname associated with the socket file     *
*                             descriptor.                                      *
*               -output_file: The output file stream that will contain the     *
*                             the spidered email addresses.                    *
*                  -protocol: The protocol that is used to acquire the resource*
*                             of the subdirectory.                             *
*            -subdirectories: The st_subdirectory deque to add spidered        *
*                             email addresses.                                 *
*          -target_directory: The target directory that is going to be spidered*    
*       -visited_directories: The list of visited directories.                 *                                                                              *
*******************************************************************************/

void thread_get_requests_helper(std::string &hostname,
                                std::fstream &output_file,
                                std::string &protocol, 
                                std::deque<st_subdirectory> &subdirectories,
                                st_subdirectory &target_directory,
                                std::map<std::string, int> &visited_directories)
{
    std::string get_response;
    get_response = obtain_get_response_curl(hostname, protocol, 
                                            target_directory);
    std::thread call_find_emails(find_emails, std::ref(get_response), 
                                 std::ref(output_file));
    std::thread call_find_subdirectories(find_subdirectories,
                                         std::ref(get_response),
                                         std::ref(hostname),
                                         std::ref(subdirectories),
                                         std::ref(visited_directories));
    call_find_emails.join();
    call_find_subdirectories.join();
    return;
}



static size_t write_call_back(void *contents, size_t size, size_t nmemb, 
                              void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



int main(int argc, char **argv)
{
    int c, display_help_flag, google_keyword_search_flag, num_threads, 
        hostname_beginning_position, search_depth, single_resource_search_flag,
        testing_mode_flag;
    std::fstream output_file;
    std::string hostname, keyword, mode, output_file_name, protocol, 
                subdirectory;
    st_url initial_url;

    
    /* 
    ** Default options are set here. These can change based upon what the user
    ** enters in as arguments. 
    */                                                                       
    display_help_flag = 0;
    google_keyword_search_flag = 0;
    hostname = "www.castroco.com";
    num_threads = 5;
    output_file_name = "emails.txt";
    protocol = "http";
    search_depth = 3;
    single_resource_search_flag = 0;
    subdirectory = "/";
    testing_mode_flag = 0;
    
    /* Initializes curl for use in the program. */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /*
    ** Structure used to define what the arguments are that can be passed into
    ** the program. This is a standard structure in getopt.h
    */
    int option_index = 4;
    static option longopts[] = 	
    { 
        {       "help",              no_argument, &display_help_flag,     1},
        {       "test",              no_argument, &testing_mode_flag,     1},
        {            0,        required_argument,                  0,   'h'},
        {            0,        optional_argument,                  0,   'o'}, 
        {            0,        optional_argument,                  0,   'd'},
        {            0,        optional_argument,                  0,   't'},
        {            0,                        0,                  0,     0}
    };

    /* 
    ** Cycles through argv to obtain what the arguments are that were passed 
    ** in using the GNU getopt library. 
    */
    while((c = getopt_long(argc, argv, "u:o:d:k:t:", longopts, &option_index)) 
            != -1) 
    {
        switch (c) 
        {
            case('u'):
                if((static_cast<std::string>(optarg).find("https://")
                        == std::string::npos 
                            &&
                    static_cast<std::string>(optarg).find("http://") 
                        != std::string::npos))
                {
                    hostname_beginning_position = 7;
                    protocol = "http://";
                }
                else if(static_cast<std::string>(optarg).find("https://")
                            != std::string::npos 
                                &&
                        static_cast<std::string>(optarg).find("http://") 
                             == std::string::npos)
                {
                    hostname_beginning_position = 8;
                    protocol = "https://";
                }
                else
                {
                    std::cout << termcolor::red 
                              << "\nError: URL must contain either the https://" 
                              << "or http:// protocol.\n" << std::endl;
                    return 0;
                }
                hostname = static_cast<std::string>(optarg).substr
                               (hostname_beginning_position, 
                                static_cast<std::string>(optarg).find("/", 
                                    hostname_beginning_position)
                                - hostname_beginning_position);
                
                subdirectory = static_cast<std::string>(optarg).substr(
                                   std::min(
                                       static_cast<std::string>(optarg).find(
                                           "/", hostname_beginning_position),
                                       static_cast<std::string> (optarg).
                                       size()), 
                                   static_cast<std::string>(optarg).size());
                                   
                std::cout << hostname << "  " << subdirectory << std::endl;
                if(subdirectory.size() == 0)
                    subdirectory = "/";
                display_help_flag = 0;
                single_resource_search_flag = 1;
                break;
                
            case('d'):
                search_depth = std::stoi(optarg);
                display_help_flag = 0;
                break;  
                
            case('k'):
                google_keyword_search_flag = 1;             
                keyword = optarg;
                break;
                
            case('o'):
                output_file_name = optarg;
                display_help_flag = 0;
                break;
            
            case('t'):
                num_threads = std::stoi(optarg);
                display_help_flag = 0;
                
            case('?'):
                display_help_flag = 0;
                break;
        }
    }
    
    initial_url.hostname = hostname;
    initial_url.protocol = protocol;
    initial_url.subdirectory = subdirectory;
    initial_url.search_depth = 0;

    /* 
    ** Determines what operation to perform based upon what was entered into
    ** the program. 
    */
    if(display_help_flag)
        print_options();
    else if(testing_mode_flag)
        run_tests();
    else if(single_resource_search_flag && google_keyword_search_flag)
        std::cout << termcolor::red << "\nError: Cannot use "
                  << "both -k and -h flags simultaneously.\n" << std::endl;
    else if(single_resource_search_flag)
    {
        output_file.open(output_file_name, std::ios::out | std::ofstream::app);
        spider_single_resource(initial_url, search_depth, num_threads, 
                               output_file);    
        output_file.close();
        system(("sort -u " + output_file_name + " -o " + output_file_name)
               .c_str());
    }
    else if(google_keyword_search_flag)
    {
        output_file.open(output_file_name, std::ios::out | std::ofstream::app);
        spider_google_keyword(keyword, output_file);
        output_file.close();
        system(("sort -u " + output_file_name + " -o " + output_file_name)
               .c_str());
    }
    else
    {
        output_file.open(output_file_name, std::ios::out | std::ofstream::app);
        spider_single_resource(initial_url, search_depth, num_threads, 
                               output_file);
        output_file.close();
        system(("sort -u " + output_file_name + " -o " + output_file_name)
               .c_str());
    }
    return 0;
}
