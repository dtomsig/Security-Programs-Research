OBJECTS = 
CXX = g++
CXXFLAGS = -std=gnu++0x -lpthread
PROGS = spider
INCLUDES = -I./include/	
LIBRARIES = -L./include/curl/lib -lcurl

all: $(PROGS)

spider: ${OBJECTS} spider.cpp
	${CXX} ${CXXFLAGS} ${OBJECTS} ${INCLUDES} ${LIBRARIES} spider.cpp -o emailSpider 

clean:
	rm -f $(PROGS) *.o *~ *.exe *.txt emailSpider	
