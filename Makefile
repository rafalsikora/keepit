all: keepit

CXX = g++
WARNING_FLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
override CXXFLAGS += -g $(WARNING_FLAGS) -std=c++17

INC=-Iinclude
SRCS = $(shell find . -type f -name '*.cpp' -print | sed -e 's/ /\\ /g')
HEADERS = $(shell find . -type f -name '*.h' -print)
LIBS += -lstdc++fs

keepit: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -pthread -o "$@" $(LIBS) $(INC)

clean:
	rm -f keepit
