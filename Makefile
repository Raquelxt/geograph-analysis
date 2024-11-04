# Simple Makefile example
CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17

all: tp1

tp1: src/main.cpp src/other_file.cpp
    $(CXX) $(CXXFLAGS) -o tp1 src/main.cpp src/other_file.cpp

clean:
    rm -f tp1