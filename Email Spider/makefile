OBJECTS = 
CXX = g++
CXXFLAGS = -std=gnu++0x
PROGS = spider

all: $(PROGS)

spider: ${OBJECTS} spider.cpp
	${CXX} ${CXXFLAGS} ${OBJECTS} spider.cpp -o emailSpider

