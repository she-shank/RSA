CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = rsa_test

# All source files
SOURCES = bigint.cpp modular.cpp prime.cpp rsa.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile each .cpp file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean

