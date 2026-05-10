CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

SRC = translator.cpp parser.cpp code_writer.cpp utils.cpp
TARGET = vm_translator

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
