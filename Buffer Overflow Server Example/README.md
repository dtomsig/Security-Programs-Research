                    BUFFER OVERFLOW DEMONSTRATION

This demonstration is compatabile with Kali linux 2.0                    
                    
In order to use this demonstration, make sure to turn off address space 
randomization.

Turn off address space randomization:
    echo 0 > /proc/sys/kernel/randomize_va_space
    
Turn it back on with (after running the demonstration).:
    echo 1 > /proc/sys/kernel/randomize_va_space
    

Steps to run program:

    0. Open two terminal windows.
    1. Type "make" in a terminal window.
    2. Type "./server 500" in one terminal. This represents your server.
    3. .Type "./overFlowExploit" in the other terminal that is not being used
       by the server. This will execute /bin/sh on the server terminal.
