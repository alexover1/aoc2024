CXX      ?= clang++
CXXFLAGS ?= -std=c++11 -Wno-writable-strings -g

all: Day1 Day2 Day3 Day4

%: %.cpp AdventOfCode.h
	$(CXX) $(CXXFLAGS) -o $@ $<
