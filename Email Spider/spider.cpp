/*******************************************************************************
* FILE: spider.cpp                                                             *
*                                                                              *
* DESCRIPTION: spider.cpp is where the main method for emailSpider.exe lives.  *
* It performs  the core operation of spidering webpages for emails.            *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#include <map>
#include "spider.hpp"
#include "test.hpp"
#include <thread>

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

    std::cout << termcolor::yellow << "\n\nCommand:" << std::endl;
    std::cout << termcolor::reset << termcolor::red << termcolor::bold 
              << "  ./emailSpider " << termcolor::reset << termcolor::bold
              << termcolor::blue << "-h" << termcolor::reset 
              << " [Host Name] " << termcolor::bold << termcolor::blue 
              << "-p" << termcolor::reset << " [Port] " << termcolor::bold 
              << termcolor::blue << "-d " << termcolor::reset
              << "[Search Depth] " << termcolor::bold << termcolor::blue
              << termcolor::blue << "-o " << termcolor::reset 
              << "[Output File] " << termcolor::bold << termcolor::blue 
              << "                    -t" << termcolor::reset  
              << " [Threads] " << termcolor::bold << termcolor::blue << "--" 
              << termcolor::reset << " [Options] " << std::endl;

    std::cout << termcolor::yellow << termcolor::bold << "\nParameters:" 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  -h" << termcolor::reset
              << "The host name of the highest level of the web page." 
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
              << std::left << std::setw(15) << "  --help" << termcolor::reset
              << "Display the help screen." 
              << std::endl;
    std::cout << termcolor::reset << termcolor::bold << termcolor::blue
              << std::left << std::setw(15) << "  --test" << termcolor::reset
              << "Display the results of function testing." 
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
    std::regex email_format("[a-zA-Z0-9.]+@[a-zA-Z0-9]+(.com|.com.pa|.co.uk|"  
                            ".de|.it|.gov|.net|.org|.us)");
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
* FUNCTION: find_subdirectories(std::string &get_response)                     *
*                                                                              *
* DESCRIPTION: find_subdirectories receives a raw GET response and adds the    *
*              subdirectories found to the deque of subdirectories to parse.   *
*                                                                              *
* PARAMETERS:   -get_response: The GET response from the HTTP server that will *
*                              be used to obtain email addresses.              *
*                  -host_name: The current hostName that is being spidered.    *
*        -visited_directories: Directories already visited not to be added.    *
*            -subdirectories: The current subdirectories awaiting visiting.    *
*******************************************************************************/

void find_subdirectories(std::string &get_response,
                         std::string &host_name,
                         std::map<std::string, int> &visited_directories,
                         std::deque<st_subdirectory> &subdirectories)
{
    
    int current_depth;
    std::regex subdirectory_format("href=\"(\\S(?!.css|.gif|.img|.png))"
                                   "*[/]*\"");
    std::regex_iterator<std::string::iterator> iterator(get_response.begin(), 
                                                        get_response.end(), 
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
        std::cout << "SIZE: " << subdirectories.size() << std::endl;
        subdirectory = (*iterator).str().substr((*iterator).str().find("\"") 
                                                 + 1, 
                                                (*iterator).str().size() - 2 - 
                                                (*iterator).str().find("\""));
        if(subdirectory.substr(0,4) == "http")
        {
            if(subdirectory.find(host_name) == std::string::npos)
            {
                iterator++;
                continue;
            }
            subdirectory = subdirectory.substr(std::min(subdirectory.find("/", 
                                                                          10), 
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
    std::string host_name, subdirectory = "";
    
    current_depth = urls.front().search_depth;
    
    /* Searches through the GET response for URLS and adds them to the deque. */
    while(iterator != iterator_end) 
    {
        pos_beginning_hostname = ((*iterator).str()).find("//") + 2; 
        pos_end_hostname = std::min(((*iterator).str()).find("/", 
                                    pos_beginning_hostname),
                                  (*iterator).str().size());
        host_name = (*iterator).str().substr(pos_beginning_hostname, 
                                             pos_end_hostname - 
                                             pos_beginning_hostname);
        subdirectory = (*iterator).str().substr(pos_end_hostname, 
                                                - pos_end_hostname);
                                                    
        /* Adjustments for input that doesn't match hostname format. */                                        
        if(subdirectory[subdirectory.size() - 1] == '/')
            subdirectory.pop_back();
        if(subdirectory == "")
            subdirectory = "/";
        if(host_name[0] != 'w' && host_name[1] != 'w' && host_name[2] !='w')
            host_name = "www." + host_name;
        urls.push_back(st_url(host_name, subdirectory, current_depth + 1));
        iterator++;
    }
    return;    
}



/*******************************************************************************
* FUNCTION: connect(std::string host_name)                                     *
*                                                                              *
* DESCRIPTION: connect attempts to extablish a tcp connection with the         *
* top-level hostname. If it is unsuccessful, -1 is returned. Otherwise, the    *
* file descriptor is returned after a connection.                              *
*                                                                              *
* PARAMETERS:      -host_name: The host name that will be connected to.        *
*******************************************************************************/

int connect(std::string host_name)
{
    int status_addr, status_socket_connect, socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    /* 
    ** These commands are necessary to fill the address info structures with 
    ** information associated with the particular webpage.
    */
    host_name.erase(0,4);
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    status_addr = getaddrinfo(host_name.c_str(), "80", &host_info, 
                             &host_info_list);
    if(status_addr != 0)
    {
        std::cout << "Get addr info error" << std::endl;
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
* DESCRIPTION: disconnect closes the socket.                                   *
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
* FUNCTION: obtain_get_response(st_subdirectory &subdirectory,                 *
*                             std::string &host_name, int socket_fd)           *
*                                                                              *
* DESCRIPTION: obtain_get_response attempts to send a GET request to the server*
*              and receive a response.                                         *
*                                                                              *
* PARAMETERS: -sub_directory: The struct subdirectory that will be spidered.   *
*                 -host_name: The hostname that is currently connected.        *
*                 -socket_fd: The handle to the current socket file descriptor.*
*******************************************************************************/

std::string obtain_get_response(st_subdirectory &subdirectory, 
                                std::string &host_name, int socket_fd)
{
    char incoming_data_buffer[1448];
    int len;
    ssize_t bytes_sent, bytes_received;
    std::string pageSource, msg = ("GET " + subdirectory.subdirectory  
                                    + " HTTP/1.0\r\nHost: " +
                                    host_name + "\r\n\r\n");
                                    
    std::cout << msg;                
    len = strlen(msg.c_str());
    bytes_sent = send(socket_fd, msg.c_str(), len, 0);	
    
    /* 
    ** Receives data from the socket until the socket has no more data. The data
    ** is added to the page source string. 
    */
    while((bytes_received = recv(socket_fd, incoming_data_buffer, 1447, 0)))
    {
        incoming_data_buffer[bytes_received] = '\0';
        pageSource += incoming_data_buffer;      
    }
    return pageSource;
} 



/*******************************************************************************
* FUNCTION: thread_get_requests_helper(int socket_fd)                          *
*                                                                              *
* DESCRIPTION: thread_get_requests_helper is a helper function for the         *
*              spidering function.                                             *
*                                                                              *
* PARAMETERS-                                                                  *
*               -output_file: The output file stream that will contain the     *
*                             the spidered  email addresses.                   *
*       -visited_directories: The list of visited directories.                 *
*            -subdirectories: The st_subDirectory deque to add spidered        *
*                             email addresses.                                 *
*          -target_directory: The target directory that is going to be spidered*                            
*                 -host_name: The hostName associated with the socket file     *
*                             descriptor.                                      *
*******************************************************************************/

void thread_get_requests_helper(std::fstream &output_file, 
                                std::map<std::string, int> &visited_directories,
                                std::deque<st_subdirectory> &subdirectories,
                                st_subdirectory &target_directory,
                                std::string &host_name)
{
    
    int current_socket_fd = connect(host_name);
    std::string get_response;
    get_response = obtain_get_response(target_directory, host_name, 
                                       current_socket_fd);
    std::thread call_find_emails(find_emails, std::ref(get_response), 
                                 std::ref(output_file));
    std::thread call_find_subdirectories(find_subdirectories,
                                         std::ref(get_response),
                                         std::ref(host_name),
                                         std::ref(visited_directories),
                                         std::ref(subdirectories));
    call_find_emails.join();
    call_find_subdirectories.join();
    disconnect(current_socket_fd);
    return;
}



/*******************************************************************************
* FUNCTION: spider(st_url &initial_url, std::fstream &output_file,             *
*                  int max_search_depth, int num_threads)                      *
*                                                                              *
* DESCRIPTION: This function controls the logical level of spidering.          *
*                                                                              *
* PARAMETERS:   -initial_url: The initial url that will be connected to.       *
*               -output_file: The output file stream that will contain the     *
*                             the spidered  email addresses.                   *
*          -max_search_depth: The highest search depth that will be spidered.  *
*               -num_threads: The number of threads the program will use.      *
*******************************************************************************/

void spider(st_url &initial_url, std::fstream &output_file, 
            int max_search_depth, int num_threads)
{   

    int current_socket_fd, thread_counter;
    std::deque<st_subdirectory> subdirectories;
    std::map<std::string, int> visited_directories;
    std::string current_host, get_response;
    std::vector<st_url> urls;
    std::vector<std::thread> get_request_threads;
    
    /* Initializiation commands as the spidering process begins. */
    current_host = initial_url.host_name;
    get_request_threads.reserve(num_threads);    
    subdirectories.push_back(st_subdirectory(initial_url.subdirectory, 
                             initial_url.search_depth));

	/* Attempts initial connection to determine if hostname is valid. */
    if((current_socket_fd = connect(current_host)) && current_socket_fd == -1)
    {
	    std::cout << termcolor::reset << termcolor::red 
	              << "\nError: Unable to connect to specified host: " 
	                 + initial_url.host_name << std::endl;	
        disconnect(current_socket_fd);
	}
	else
	{    
        disconnect(current_socket_fd);            
        /* Main spidering logic controller is here. */
        
        while(!subdirectories.empty())
        {   
            std::cout << "size: " << subdirectories.size() << std::endl;
            
            /* Spawns threads that won't exceed the number of max threads. */ 
            for(thread_counter = 0; thread_counter < num_threads 
                && thread_counter < subdirectories.size(); thread_counter++)
            {    
                visited_directories.insert(std::pair<std::string, int>
                                         (subdirectories[thread_counter]
                                         .subdirectory, 1));
                get_request_threads.emplace_back(std::thread(
                                               thread_get_requests_helper,
                                               std::ref(output_file),
                                               std::ref(visited_directories),
                                               std::ref(subdirectories),
                                               std::ref(subdirectories
                                                        [thread_counter]),
                                               std::ref(current_host)));
            }
            
            /* Rejoins the threads and frees memory used for threads. */
            for(auto &t: get_request_threads)
            {
                t.join();
                subdirectories.pop_front();
            }

            /* Erases the threads from the vector at the end of execution. */
            get_request_threads.erase(get_request_threads.begin(), 
                                      get_request_threads.begin() 
                                      + thread_counter);
        }
    }
    return;
}



int main(int argc, char **argv)
{
    int c, display_help_flag, num_threads, search_depth, testing_mode_flag;
    std::fstream output_file;
    std::string host_name, output_file_name, subdirectory;
    st_url initial_url;

    
    /* 
    ** Default options are set here. These can change based upon what the user
    ** enters in as arguments. 
    */                                                                       
    host_name = "www.zybez.net";
    display_help_flag = 0; 
    output_file_name = "emails.txt";
    search_depth = 3;
    subdirectory = "/";
    testing_mode_flag = 0;
    num_threads = 5;

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
    while ((c = getopt_long(argc, argv, "h:o:d:t:", longopts, &option_index)) 
            != -1) 
    {
        switch (c) 
        {
            case('h'):
                host_name = static_cast<std::string>(optarg).substr(0, 
                                                     static_cast<std::string>
                                                   (optarg).find("/"));
                subdirectory = static_cast<std::string>(optarg).
                                                       substr(std::min(
                                                       static_cast<std::string>
                                                       (optarg).find("/"), 
                                       static_cast<std::string>(optarg).size()), 
                                       static_cast<std::string>(optarg).size());
                if(subdirectory.size() == 0)
                    subdirectory = "/";
                display_help_flag = 0;
                break;
                
            case('d'):
                search_depth = std::stoi(optarg);
                display_help_flag = 0;
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
    
    initial_url.host_name = host_name;
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
    else
    {
        output_file.open(output_file_name, std::ios::out | std::ofstream::app);
        spider(initial_url, output_file, search_depth, num_threads);
    }
    
    output_file.close();
    return 0;
}
