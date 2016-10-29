ASM = -masm=intel
CXX = wine g++
CXXFLAGS = -std=gnu++0x
DLLS = dll/inject.dll
OBJECTS = game/game.o
VPATH = Z:/root/Desktop/Programming/C++/Hearthstone\ Bot

all: bot

bot: game_file inject client/bot.cpp
	${CXX} ${CXXFLAGS} ${OBJECTS} ${DLLS} client/bot.cpp -o hBot

game_file: game/game.h game/game.cpp
	$(CXX) -c game/game.cpp -o game/game.o

inject: dll/inject.hpp dll/inject.cpp
	${CXX} -DBUILDING_INJECT_DLL dll/inject.cpp -shared -o dll/inject.dll ${ASM}

clean:
	rm -f $(PROGS) *.o *~ *.exe *~ *.dll *~
	rm -f dll/*.o dll/*.dll
	rm -f game/*.o
