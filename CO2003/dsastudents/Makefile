CXX=g++
CXXFLAGS=-Iinclude -Isrc -std=c++17 -g

# Target to build the executable
build: main.o
	$(CXX) $(CXXFLAGS) main.o -o main

# Target for object file
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Clean rule to remove generated files
.PHONY: clean
clean:
	rm -f *.o main