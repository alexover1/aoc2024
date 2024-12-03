all: Day1 Day2 Day3

Day1: Day1.cpp AdventOfCode.h
	clang++ -std=c++11 -g -o Day1 Day1.cpp

Day2: Day2.cpp AdventOfCode.h
	clang++ -std=c++11 -g -o Day2 Day2.cpp

Day3: Day3.cpp AdventOfCode.h
	clang++ -std=c++11 -g -o Day3 Day3.cpp
