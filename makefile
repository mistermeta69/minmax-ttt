# Makes the tic-tac-toe program
# creates the executable "ttt"
# Uses board.cpp : the board object
# the main program logic and user interface is playttt.C
#

CXX = g++
CXXFLAGS = -Wall -g -std=c++11

ttt: build/playttt.o build/board.o 
	$(CXX) $(CXXFLAGS) build/playttt.o build/board.o -o build/ttt

playttt.o: src/playttt.cpp src/board.h
	$(CXX) $(CXXFLAGS) -c src/playttt.cpp

board.o: src/board.cpp src/board.h
	$(CXX) $(CXXFLAGS) -c src/board.cpp

clean:
	rm -f build/*.o build/ttt