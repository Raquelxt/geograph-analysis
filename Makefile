CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

SRCS = src/main.cpp src/Graph.cpp
OBJS = $(SRCS:src/%.cpp=obj/%.o)
TARGET = geograph-analysis

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o $(TARGET)
