all: keepit

CXX = g++
override CXXFLAGS += -g -Wall -std=c++17

SRCS = $(shell find . -type f -name '*.cpp' -print | sed -e 's/ /\\ /g')
HEADERS = $(shell find . -type f -name '*.h' -print)

keepit: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o "$@"

clean:
	rm -f keepit