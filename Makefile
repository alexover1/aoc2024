CXX      ?= clang++
CXXFLAGS ?= -I. -std=c++11 -Wno-write-strings

SOURCES = $(wildcard code/*.cpp)
OUTPUTS = $(patsubst code/%.cpp,%,$(SOURCES))
HEADERS = $(wildcard *.h)

.PHONY: clean

all: $(OUTPUTS)

%: code/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OUTPUTS)
