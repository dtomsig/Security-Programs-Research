/*******************************************************************************
* FILE: spider.cpp                                                             *
*                                                                              *                                                                                     *
* DESCRIPTION: spider.cpp is where the main method for emailSpider.exe lives.  *
* It performs  the core operation of spidering webpages for emails.            *
*                                                                              *
* OUTPUT FILE: emailSpider.exe                                                 *
*******************************************************************************/

#include "spider.hpp"
#include "test.hpp"
#include <thread>

/*******************************************************************************
* FUNCTION: printOptions()                                                     *
*                                                                              *                                                                                     *
* DESCRIPTION: printOptions prints all of the options available to the user    *        
* running ./emailSpider. It is called when --help is used as an option.        *
*                                                                              *
* PARAMETERS: None                                                             *
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
              << "Number of threads to be used in the application." 
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
}



/*******************************************************************************
* FUNCTION: findEmails(std::string getResponse)                                *
*                                                                              *                                                                                 *
* DESCRIPTION: findEmails receives a raw GET response and writes the emails    *
* directly to an output file stream.                                           *
*                                                                              *
* PARAMETERS:   -getResponse: The GET response from the HTTP server that will  *
*                             be used to obtain email addresses.               *
*                -outputFile: The ouptut file that will contain spidered email *
*                             addresses.                                       *                                                                       *
*******************************************************************************/

void findEmails(std::string &getResponse, std::fstream &outputFile)
{
    /* 
    ** "emailFormat" is a regular expression for email addresses.
    ** 
    ** "iterator" is a regex iterator for the incoming getResponse.
    **
    ** "iteratorEnd" is the last location in the incoming getResponse.
    */
    std::regex emailFormat("[a-zA-Z0-9.]+@[a-zA-Z0-9]+(.com|.com.pa|.co.uk|"  
                           ".de|.it|.gov|.net|.org|.us)");
    std::regex_iterator<std::string::iterator> iterator(getResponse.begin(), 
                                                        getResponse.end(), 
                                                        emailFormat);
    std::regex_iterator<std::string::iterator> iteratorEnd;

    /* 
    ** Searches through the getResponse for emails and adds them to the 
    ** email file. 
    */
    while(iterator != iteratorEnd) 
    {
        outputFile << (*iterator).str() << "\n";
        iterator++;
    }
}



/*******************************************************************************
* FUNCTION: findUrls(std::string getResponse)                                  *
*                                                                              *                                                                                     *
* DESCRIPTION: findUrls receives a raw GET response and modifies a deque of    *
* URLS to hold additional URLs for spidering purposes.                         *
*                                                                              *
* PARAMETERS:   -getResponse: The GET response from the HTTP server that will  *
*                             be used to obtain email addresses.               *                                                        *
*                      -urls: The deque of URLS that will be added to.         *               
*******************************************************************************/

void findUrls(std::string &getResponse, std::deque<url> &urls)
{
    /*
    ** "currentDepth" holds the current depth of the struct URL that is passed 
    ** in.
    **
    ** "posBeginningHostName" holds the first location of the hostname of the 
    ** URL in order to calculate hostName.
    **
    ** "posEndHostName" holds the end location of the hostname in order to
    ** calculate hostName as well as subDirectory.
    **
    ** "urlFormat" is a regular expression for email addresses.
    ** 
    ** "iterator" is a regex iterator for the incoming getResponse.
    **
    ** "iteratorEnd" is the last location in the incoming getResponse.
    **
    ** "hostName" holds the hostname of the URL that was spidered.
    **
    ** "subDirectory" holds the subdiretory fo the URL that was spidered.
    */
    int currentDepth, posBeginningHostname, posEndHostname; 
    std::regex urlFormat("htt(p|ps)://[a-zA-Z0-9]+.[a-zA-Z0-9]+.(.com|.com.pa|"
                          ".co.uk|.de|.it|.gov|.net|.org|.us)[^\"']*");
    std::regex_iterator<std::string::iterator> iterator(getResponse.begin(), 
                                                        getResponse.end(), 
                                                        urlFormat);
    std::regex_iterator<std::string::iterator> iteratorEnd;
    std::string hostName, subDirectory = "";
    
    currentDepth = urls.front().searchDepth;
    
    /* Searches through the GET response for URLS and adds them to the deque. */
    while(iterator != iteratorEnd) 
    {
        posBeginningHostname = ((*iterator).str()).find("//") + 2; 
        posEndHostname = std::min(((*iterator).str()).find("/", 
                                    posBeginningHostname),
                                  (*iterator).str().size());
        hostName = (*iterator).str().substr(posBeginningHostname, 
                                            posEndHostname - 
                                            posBeginningHostname);
        subDirectory = (*iterator).str().substr(posEndHostname, 
                                                - posEndHostname);
                                                    
        /* Adjustments for input that doesn't match hostname format. */                                        
        if(subDirectory[subDirectory.size() - 1] == '/')
            subDirectory.pop_back();
        if(subDirectory == "")
            subDirectory = "/";
        if(hostName[0] != 'w' && hostName[1] != 'w' && hostName[2] !='w')
            hostName = "www." + hostName;
        urls.push_back(url(hostName, subDirectory, currentDepth + 1));
        iterator++;
    }    
}



/*******************************************************************************
* FUNCTION: connect(std::string hostName)                                      *
*                                                                              *                                                                                     *
* DESCRIPTION: connect attempts to extablish a tcp connection with the         *
* top-level hostname. If it is unsuccessful, -1 is returned. Otherwise, the    *
* file descriptor is returned after a connection.                              *
*                                                                              *
* PARAMETERS:      -hostName: The host name that will be connected to.         *
*******************************************************************************/

int connect(std::string hostName)
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
    ** "host_info" refers to the structure that contains address information in
    **  order to create a socket connection to the hostName. 
    **
    ** "host_info_list" is a pointer to an addrinfo structure that is used in 
    **  the getaddrinfo() function. 
    */
    int statusAddr, statusSocketConnect, socketfd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    /* 
    ** These commands are necessary to fill the address info structures with 
    ** information associated with the particular webpage.
    */

    hostName.erase(0,4);
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    statusAddr = getaddrinfo(hostName.c_str(), "80", &host_info, 
                             &host_info_list);
    if(statusAddr != 0)
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
    socketfd = socket((*host_info_list).ai_family, 
                      (*host_info_list).ai_socktype, 
                      (*host_info_list).ai_protocol);	
                         
    if(socketfd == -1)
    {
        std::cout << "Socket error" << std::endl;
        return -1;
    }
    else
        std::cout << "There is no socket error" << std::endl;
       
    /* These commands connect the socket to the web server. */
    statusSocketConnect = connect(socketfd, (*host_info_list).ai_addr, 
                                  (*host_info_list).ai_addrlen);
                                  
    if(statusSocketConnect == -1)  
    {
        std::cout << "Connect error" << std::endl;
        return -1;
    }
    
    return socketfd;
}



/*******************************************************************************
* FUNCTION: disconnect(int socketfd)                                           *
*                                                                              *                                                                                     *
* DESCRIPTION: disconnect closes the socket                                    *
*                                                                              *
* PARAMETERS:      -socketfd: The file descriptor of the socket that will be   *
*                             closed.                                          *
*******************************************************************************/

void disconnect(int socketfd)
{
    close(socketfd);
    return;
}



/*******************************************************************************
* FUNCTION: obtainGetResponse(std::string hostName)                            *
*                                                                              *                                                                                     *
* DESCRIPTION: obtainGetResponse attempts to send a GET request to the server  *
*              and receive a response.                                         *
*                                                                              *
* PARAMETERS:      -url: The particular struct url that will be spidered.      *
*                  -socketfd: The handle to the current socket file descriptor.*
*******************************************************************************/

std::string obtainGetResponse(url &url, int socketfd)
{
    /*
    ** "incoming_data_buffer" is an array that stores the response from the 
    **  webserver.
    **
    ** "len" refers to the length of the GET message to the web server.
    **
    ** "bytes_sent" refers to the number of bytes sent to the server. 
    **
    ** "bytes_received" refers to the number of bytes received from GET request.
    **
    ** "pageSource" refers to the string that contains the page source.
    **
    ** "msg" refers to the GET message sent to the webserver. 
    */		
    char incoming_data_buffer[1448];
    int len;
    ssize_t bytes_sent, bytes_received;
    std::string pageSource, msg = ("GET " + url.subDirectory  
                                    + " HTTP/1.0\r\nHost:" +
                                    url.hostName + "\r\n\r\n");
    std::cout << msg;                
    len = strlen(msg.c_str());
    bytes_sent = send(socketfd, msg.c_str(), len, 0);	
    
    /* 
    ** Receives data from the socket until the socket has no more data. The data
    ** is added to the page source string. 
    */
    while((bytes_received = recv(socketfd, incoming_data_buffer, 1447, 0)))
    {
        incoming_data_buffer[bytes_received] = '\0';
        pageSource += incoming_data_buffer;      
    }
        
    return pageSource;
} 



/*******************************************************************************
* FUNCTION: threadGetRequestsHelper(int socketfd)                              *
*                                                                              *                                                                                     *
* DESCRIPTION: threadGetRequestsHelper is a helper function for the spidering  *
*              function.                                                       *
*                                                                              *
* PARAMETERS:      -hostName: The hostName that the thread will connect to.    *
*                -outPutFile: The output file stream that will contain the     *
*                             the spidered  email addresses.                   *
*              -specificUrl:  The particular url to bespidered.                *
*                       urls: The url deque to add spidered email addresses.   *
*******************************************************************************/

void threadGetRequestsHelper(std::string hostName, std::fstream &outputFile,
                             url &specificUrl, std::deque<url> &urls)
{
    /* 
    ** "currentSocketfd" represents the current socket file descriptor that the
    ** program is connected to.
    **
    ** "getResponse" stores the get response from the target server.
    */     
    int currentSocketfd;
    std::string getResponse;
    currentSocketfd = connect(hostName);
    if(currentSocketfd == -1)
        return;
    getResponse = obtainGetResponse(specificUrl, currentSocketfd);
    std::thread callFindEmails (findEmails, std::ref(getResponse), 
                                std::ref(outputFile));
    std::thread callFindUrls (findUrls, std::ref(getResponse), std::ref(urls));
    
    callFindEmails.join();
    callFindUrls.join();        
    disconnect(currentSocketfd);
    return;
}



/*******************************************************************************
* FUNCTION: spider(std::string hostName, std::fstream &outputFile )            *
*                                                                              *                                                                                     *
* DESCRIPTION: This function controls the logical level of spidering.          *
*                                                                              *
* PARAMETERS:     initialUrl: The initail url that will be connected to.       *
*                -outPutFile: The output file stream that will contain the     *
*                             the spidered  email addresses.                   *
*            -maxSearchDepth: The highest search depth that will be spidered.  *
*                -numThreads: The number of threads the program will use.      *                                        
*******************************************************************************/

void spider(url &initialUrl, std::fstream &outputFile, int maxSearchDepth,
            int numThreads)
{   
    /* 
    ** "currentSocketfd" represents the current socket file descriptor that the
    ** program is connected to. 
    ** 
    ** "threadCounter" counts the number of threads that are currently active.
    **
    ** "urls" represents all of the urls that remain to be spidered by the 
    ** program. It is a deque of the url structure that was declared before.
    **
    ** "currentHost" stores the current host that is connected to. 
    **
    ** "getResponse" represents the GET response to be parsed.
    ** 
    ** "getRequestThreads" represents the threads for get requests according
    ** to the number that the user reqeusted. 
    */
    int currentSocketfd, threadCounter;
    std::deque<url> urls;
    std::string currentHost, getResponse;
    std::vector<std::thread> getRequestThreads;
    getRequestThreads.reserve(numThreads);
        
	/* Attempts initial connection to determine if hostname is valid. */
    if((currentSocketfd = connect(initialUrl.hostName)) && 
                          currentSocketfd == -1)
    {
	    std::cout << termcolor::reset << termcolor::red 
	              << "\nError: Unable to connect to specified host: " 
	                 + initialUrl.hostName << std::endl;	
        disconnect(currentSocketfd);
	}
	else
	{
        urls.push_back(initialUrl);
        currentHost = urls.front().hostName;
                
        /* Main spidering logic controller is here. */
        while(!urls.empty())
        {   
            /* Spawns threads that won't exceed the number of max threads. */ 
            for(threadCounter = 0; threadCounter < numThreads && threadCounter <
                urls.size(); threadCounter++)
            {    
                while(urls[threadCounter].searchDepth == maxSearchDepth)
                {
                    std::cout << "Max depth reached!!!" << std::endl;
                    urls.erase(urls.begin() + threadCounter);
                }
                std::cout << "size is " << urls.size() << std::endl;
                getRequestThreads.emplace_back(std::thread(
                                            threadGetRequestsHelper, 
                                            urls[threadCounter].hostName,
                                            std::ref(outputFile),
                                            std::ref(urls[threadCounter]), 
                                            std::ref(urls)));
                std::cout << "Added " << urls[threadCounter].hostName << std::endl;
            }
            
            /* Rejoins the threads and frees memory used for threads. */
            for(auto &t: getRequestThreads)
            {
                t.join();
                urls.pop_front();
            }
            
            /* Erases the threads from the vector at the end of execution. */
            getRequestThreads.erase(getRequestThreads.begin(), 
                                        getRequestThreads.begin() 
                                        + threadCounter);
        }
    }
    return;
}



int main(int argc, char **argv)
{
    /*
    ** "c" is an argument that is used in getopt_long() for parsing arguments.
    ** 
    ** "displayHelpFlag" is a flag for displaying the help menu based on what
    **  the user enters as arguments.
    **
    ** "numThreads" represents the number of threads the program will use.
    **
    ** "searchDepth" represents the depth that the spider will go into when
    **  searching for emails.
    **
    ** "testingModeFlag" represents whether or not the tests will be run in the 
    **  program.
    **
    ** "initialUrl" represents the initial url that will be spidered.
    **
    ** "outputFile" refers to the output filestream associated with 
    **  outputFileName
    **
    ** "hostName" represents the highest level that will be spidered by the 
    **  program.
    ** 
    ** "outputFileName" represents the output file that will store all of the 
    **  spidered email addresses. 
    **
    ** "subDirectory" represents the subdomain that will be spidered.
    */ 
    int c, displayHelpFlag, numThreads, searchDepth, testingModeFlag;
    std::fstream outputFile;
    std::string hostName, outputFileName, subDirectory;
    url initialUrl;

    /* 
    ** Default options are set here. These can change based upon what the user
    ** enters in as arguments. 
    */                                                                       
    hostName = "www.zybez.net";
    displayHelpFlag = 0; 
    outputFileName = "emails.txt";
    searchDepth = 3;
    subDirectory = "/";
    testingModeFlag = 0;
    numThreads = 5;

    /*
    ** Structure used to define what the arguments are that can be passed into
    ** the program. This is a standard structure in getopt.h
    */
    int option_index = 4;
    static option longopts[] = 	
    { 
        {       "help",              no_argument,   &displayHelpFlag,     1},
        {       "test",              no_argument,   &testingModeFlag,     1},
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
                hostName = static_cast<std::string>(optarg).substr(0, 
                                                   static_cast<std::string>
                                                   (optarg).find("/"));
                subDirectory = static_cast<std::string>(optarg).
                                                       substr(std::min(static_cast
                                                       <std::string>
                                                       (optarg).find("/"), 
                                       static_cast<std::string>(optarg).size()), 
                                       static_cast<std::string>(optarg).size());
                if(subDirectory.size() == 0)
                    subDirectory = "/";
                std::cout << "Hostname is" << hostName;
                displayHelpFlag = 0;
                break;
                
            case('d'):
                searchDepth = std::stoi(optarg);
                displayHelpFlag = 0;
                break;  
                
            case('o'):
                outputFileName = optarg;
                displayHelpFlag = 0;
                break;
            
            case('t'):
                numThreads = std::stoi(optarg);
                displayHelpFlag = 0;
                
            case('?'):
                displayHelpFlag = 0;
                break;
        }
    }
    
    initialUrl.hostName = hostName;
    initialUrl.subDirectory = subDirectory;
    initialUrl.searchDepth = 0;
    
    /* 
    ** Determines what operation to perform based upon what was entered into
    ** the program. 
    */
    if(displayHelpFlag)
        printOptions();
    else if(testingModeFlag)
        runTests();
    else
    {
        outputFile.open(outputFileName, std::ios::out | std::ofstream::app);
        spider(initialUrl, outputFile, searchDepth, numThreads);
    }
    
	outputFile.close();
    return 0;
}
