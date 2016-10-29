#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

void readData(void *socket_desc)
{
    char dataBuffer[512];
    std::cout << "\n\n\n\n\nAddress of RBP: " << static_cast<void*>(&dataBuffer[512]) 
              << std::endl;
    std::cout << "Address of First Element of Buffer: " 
              << static_cast<void*>(&dataBuffer[0]) << std::endl;
    recv(*(static_cast<int*>(socket_desc)), dataBuffer, 700, 0);
    std::cout << dataBuffer << std::endl;
}
    
void *connectionHandlerer(void *socket_desc)
{
    readData(socket_desc);
}

void createServer(int port)
{
    int c, socket_desc, client_sock, read_size;
    struct sockaddr_in server, client;
    
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port  = htons(port);
    
    bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
    listen(socket_desc, 3);
    
    c = sizeof(struct sockaddr_in);
    
    while(client_sock = accept(socket_desc, (struct sockaddr *)&client, 
                              (socklen_t*)&c))
    {
        pthread_t newThread;
        pthread_create(&newThread, NULL, &connectionHandlerer, &client_sock);
    }
    return;
}


int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "The server encountered an error while starting. "  
                  << "Enter only one port." 
                  << std::endl;
        return 0;              
    }
    
    std::istringstream ss(argv[1]);
    int port;
    
    if(!(ss >> port) || !(ss.eof()))
    {
        std::cout << "Invalid port entered while starting the server. " 
                  << std::endl;
        return 0;
    }
    
    createServer(port);
}
