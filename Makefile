CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -ggdb -Iinclude 
CXXFLAGSTEST := $(CXXFLAGS) -Iexternal/catch2

PROJECT := verboseLIS

APP := app/main.cpp
APP_OBJ := $(APP:.cpp=.o)

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

app/%.o: app/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

TEST_SRCS := $(wildcard test/*.cpp) external/catch2/catch_amalgamated.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

test/%.o: test/%.cpp
	$(CXX) $(CXXFLAGSTEST) -c $< -o $@

external/catch2/%.o: external/catch2/%.cpp
	$(CXX) $(CXXFLAGSTEST) -c $< -o $@

.PHONY: all build build-test run test clean partial_clean bear

all: build build-test

build: $(APP_OBJ) $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(PROJECT)
	@echo "[+] Build complete."
	@echo

build-test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGSTEST) $^ -o test$(PROJECT)
	@echo "[+] Test build complete."
	@echo

run: build
	./$(PROJECT)

test: build-test 
	./test$(PROJECT)

partial_clean:
	rm -f $(OBJS) $(TEST_OBJS)

clean: partial_clean
	rm -f $(PROJECT) test$(PROJECT)

bear:
	bear -- make -j4
	@echo "[+] Bear file generated."
	@echo
	make clean
