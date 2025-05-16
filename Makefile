CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -ggdb -Ilib

PROJECT := gsc

APP := app/main.cpp
APP_OBJ := $(APP:.cpp=.o)

SRCS := $(wildcard src/*.cpp) $(wildcard src/dbg/*.cpp)
OBJS := $(SRCS:.cpp=.o)

TEST_SRCS := $(wildcard test/*.cpp) lib/catch2/catch_amalgamated.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

.PHONY: all build build-test run test clean partial_clean bear

all: build build-test

build: $(APP_OBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(PROJECT)
	@echo "[+] Build complete."
	@echo

build-test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(PROJECT)Test
	@echo "[+] Test build complete."
	@echo

run: build
	./$(PROJECT)

test: build-test 
	./$(PROJECT)Test 

partial_clean:
	rm -f $(OBJS) $(TEST_OBJS)

clean: partial_clean
	rm -f $(PROJECT) $(PROJECT)Test

bear:
	bear -- make -j4
	@echo "[+] Bear file generated."
	@echo
	make clean
