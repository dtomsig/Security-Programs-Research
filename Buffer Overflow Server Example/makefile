CXXFLAGS  = -fno-stack-protector -z execstack

all: overflow assembly server example

overflow:
	g++ -c overFlowExploit.cpp -o overFlowExploit.o $(CXXFLAGS)
	g++ overFlowExploit.o -o overFlowExploit $(CXXFLAGS)


assembly:
	g++ openShell.s -o openShell
	
server:
	g++ -pthread -c server.cpp -o server.o $(CXXFLAGS)
	g++ -pthread server.o -o server $(CXXFLAGS)

clean:
	rm -f *.o overFlowExploit openShell server
