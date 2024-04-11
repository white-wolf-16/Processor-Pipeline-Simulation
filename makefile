# Compiler
CXX := g++

# Flags
CXXFLAGS := -std=c++11 -Wall

# Source files
SRCS := main.cpp Instruction.cpp Pipeline.cpp
OBJS := $(SRCS:.cpp=.o)

# Include directories
INC := -I.

# Targets
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

.PHONY: clean
clean:
	rm -f main $(OBJS)
