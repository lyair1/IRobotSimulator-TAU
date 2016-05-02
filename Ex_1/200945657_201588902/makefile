.DEFAULT_GOAL := simulator

# place here your source files
src = main.cpp AlgorithmNaive.cpp configReader.cpp House.cpp Sensor.cpp Simulation.cpp Simulator.cpp
obj = $(src:.cpp=.o)

CC_FLAGS = -c -O2 -Wall -pthread -std=c++11 -pedantic -g
CC = g++

# may add boost libraries here
LD_FLAGS = -lm

all: simulator

simulator: $(obj)
	$(CC) -o $@ $^ $(LD_FLAGS)

$(obj): %.o: %.cpp
	$(CC) $(CC_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(obj) simulator