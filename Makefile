all: Day1 Day2

Day1: Day1.cpp AdventOfCode.h
	clang++ -std=c++11 -o Day1 Day1.cpp

Day2: Day2.cpp AdventOfCode.h
	clang++ -std=c++11 -g -o Day2 Day2.cpp
