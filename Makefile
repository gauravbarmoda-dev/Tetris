CXX = g++
CXXFLAGS = -O3 -Wall -flto -Iinclude
LDFLAGS = -lraylib -flto

SRC = src/game.cpp
OUT = bin/tetris

all: build

build:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -rf bin
