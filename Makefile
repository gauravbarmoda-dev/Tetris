CXX = g++
CXXFLAGS = -O3 -Wall -flto
LDFLAGS = -lraylib -flto

SRC = game.cpp
OUT = tetris

all: build

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)
