#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <argp.h>
#include <sys/socket.h>

/* The program name that will be displayed to the user. */
const char *argp_program_version = "emailSpider v 1.0.0";


/* The program documentation that will be displayed to the user. */
char doc[] = "emailSpider - a program to harvest emails from a domain";

/* The description for the arguments that will be passed into the program */
//char args_doc[] = "ARG1 ARG2";

/* List of arguments that the program will take. */
struct arguments
{
    std::string hostName;
};

/* List of arguments displayed in option form. */
static struct argp_option options[] = 
{
    {"Host Name",      'h', 0, 0, ""}

};


/* Function that sets the parsing options. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    arguments *arguments = static_cast<*arguments>((*state).input);

    switch(key)
    {
        case('h'):
            (*arguments).hostName = "gwas";
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}


/* The argument parser with no option for argument documentation. */
static struct argp argp = {options, parse_opt, 0, doc};


int main(int argc, char **argv)
{

    arguments arguments;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);


    int socket_desc;
    struct sockaddr_in server;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc == -1)
    {
        std::cout << "Could not create socket " << std::endl;
    }
    else
        std::cout << "Succesfully created" << std::endl;
    
    server.sin_addr.s_addr = inet_addr("173.194.121.9");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server) < 0))
        std::cout << "Error connecting to server"; 
    else
        std::cout << "Succesfully connected to server";
}
