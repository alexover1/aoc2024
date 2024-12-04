CXX      ?= clang++
CXXFLAGS ?= -std=c++11 -Wno-write-strings

OUTPUTS = Day1 Day2 Day3 Day4

.PHONY: clean

all: $(OUTPUTS)

%: code/%.cpp code/AdventOfCode.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OUTPUTS)
