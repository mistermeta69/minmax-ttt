# Makes the tic-tac-toe program
# creates the executable "ttt"
# Uses ttt.C : the board object
# the main program logic and user interface is playttt.C
# this is a c++ program, so use capital C file extensions
# T. Barake  03/1995   v.0.0
#

CXX = g++
CXXFLAGS = -Wall -g -std=c++17

ttt: playttt.o board.o 
	$(CXX) $(CXXFLAGS) playttt.o board.o -o ttt

playttt.o: playttt.cpp board.h
	$(CXX) $(CXXFLAGS) -c playttt.cpp

board.o: board.cpp board.h
	$(CXX) $(CXXFLAGS) -c board.cpp

clean:
	rm -f *.o ttt