CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -ggdb -Ilib/gsc
CXXFLAGSTEST := $(CXXFLAGS) -Ilib/catch2

PROJECT := gsc

APP := app/main.cpp
APP_OBJ := $(APP:.cpp=.o)

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

app/%.o: app/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

TEST_SRCS := $(wildcard test/*.cpp)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

test/%.o: test/%.cpp
	$(CXX) $(CXXFLAGSTEST) -c $< -o $@

.PHONY: all build build-test run test clean partial_clean bear

all: build build-test

build: $(APP_OBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(PROJECT)
	@echo "[+] Build complete."
	@echo

build-test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGSTEST) $^ -o $(PROJECT)Test
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
