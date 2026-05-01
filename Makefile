CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

TARGET = myProg
SRCS   = main.cpp Parser.cpp Batch.cpp Menu.cpp GraphColoring.cpp
OBJS   = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)