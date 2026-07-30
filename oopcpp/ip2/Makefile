# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

# Target Names
LIB_NAME = trie
DEMO_EXEC = demo
TEST_EXEC = test
LOG_FILE = log.txt

# Source Files
LIB_SRC = $(LIB_NAME).cpp
LIB_OBJ = $(LIB_NAME).o
DEMO_SRC = demo.cpp
TEST_SRC = test.cpp

# Default: Build everything
all: $(DEMO_EXEC) $(TEST_EXEC)

# a) Build the module .o from .h and .cpp
$(LIB_OBJ): $(LIB_SRC) $(LIB_NAME).h
	$(CXX) $(CXXFLAGS) -c $(LIB_SRC) -o $(LIB_OBJ)

# b) Build demo
$(DEMO_EXEC): $(DEMO_SRC) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) $(DEMO_SRC) $(LIB_OBJ) -o $(DEMO_EXEC)

# d) Build test
$(TEST_EXEC): $(TEST_SRC) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(LIB_OBJ) -o $(TEST_EXEC)

# c) Run demo
run_demo: $(DEMO_EXEC)
	./$(DEMO_EXEC)

# e) Run test
run_test: $(TEST_EXEC)
	./$(TEST_EXEC)

# f) Clean everything
clean:
	rm -f $(LIB_OBJ) $(DEMO_EXEC) $(TEST_EXEC) $(LOG_FILE)

# g) Rebuild from scratch
rebuild: clean all

# Phony targets to prevent conflicts with files named 'clean', etc.
.PHONY: all clean run_demo run_test rebuild
