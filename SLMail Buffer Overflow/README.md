                            README
                            
These are working python exploit files for SLMail server v 5.5 as shown in the 
OSCP course. "exploit.py" and "exploit2.py" are working attack payloads against
a Windows machine running SLMail. To use them simply "python exploit.py". However,
the shellcode may need to be modified because the LHOST used in the exploit is
192.168.1.60. It will not work if the LAN IP is different.

To gain a reverse shell, use nc -nlvp 443.

643.c and 646.c are modified files from exploit-db.
