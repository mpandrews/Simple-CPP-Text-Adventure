CXX = g++
CXXFLAGS = -Wall -std=c++11
HEADERS = Room.hpp Player.hpp Object.hpp Holodeck.hpp Corridor.hpp Item.hpp

default: game

game: main.o Room.o Holodeck.o Corridor.o Object.o Player.o Item.o Console.o \
	Deactivated.o LastRoom.o
	${CXX} ${CXXFLAGS} main.o Room.o Holodeck.o Corridor.o Object.o \
	Player.o Item.o Console.o Deactivated.o LastRoom.o -o game

main.o: main.cpp
	${CXX} ${CXXFLAGS} -c main.cpp

Room.o: Room.cpp Room.hpp Object.hpp Console.hpp
	${CXX} ${CXXFLAGS} -c Room.cpp

Holodeck.o: Holodeck.cpp Holodeck.hpp Room.hpp Object.hpp
	${CXX} ${CXXFLAGS} -c Holodeck.cpp

Corridor.o: Corridor.cpp Corridor.hpp Room.hpp Player.hpp Object.hpp
	${CXX} ${CXXFLAGS} -c Corridor.cpp

Object.o: Object.cpp Object.hpp Holodeck.hpp Room.hpp Item.hpp Player.hpp
	${CXX} ${CXXFLAGS} -c Object.cpp

Player.o: Player.cpp Player.hpp
	${CXX} ${CXXFLAGS} -c Player.cpp

Item.o: Item.cpp Item.hpp
	${CXX} ${CXXFLAGS} -c Item.cpp

Console.o: Console.cpp Console.hpp Player.hpp
	${CXX} ${CXXFLAGS} -c Console.cpp

Deactivated.o: Deactivated.cpp Deactivated.hpp Room.hpp
	${CXX} ${CXXFLAGS} -c Deactivated.cpp

LastRoom.o: LastRoom.cpp LastRoom.hpp Room.hpp
	${CXX} ${CXXFLAGS} -c LastRoom.cpp

clean:
	rm ./*.o ./game
