OBJECTS = 
CXX = g++
CXXFLAGS = -std=gnu++0x 
PROGS = spider
INCLUDES = -I./include/

all: $(PROGS)

spider: ${OBJECTS} spider.cpp
	${CXX} ${CXXFLAGS} ${OBJECTS} ${INCLUDES} spider.cpp -o emailSpider

clean:

