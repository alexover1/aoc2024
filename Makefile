CXX      ?= clang++
CXXFLAGS ?= -std=c++11 -Wno-write-strings

SOURCES = $(wildcard code/*.cpp)
OUTPUTS = $(patsubst code/%.cpp,%,$(SOURCES))

.PHONY: clean

all: $(OUTPUTS)

%: code/%.cpp code/AdventOfCode.h
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OUTPUTS)
