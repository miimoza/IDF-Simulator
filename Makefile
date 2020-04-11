CC=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17 -Iinclude -g
SRC=src/main.cc src/graph.cc src/dijkstra.cc src/create_graph.cc src/log.cc \
 	src/dot.cc
BIN=idf-simulator

all:
	$(CC) $(CXXFLAGS) $(SRC) -o $(BIN)

clean:
	$(RM) $(BIN) *.dot *.png
