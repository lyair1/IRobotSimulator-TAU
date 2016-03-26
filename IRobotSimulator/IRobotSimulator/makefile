.DEFAULT_GOAL := simulator

# place here your source files
src = main.cpp AlgorithmNaive.cpp configReader.cpp House.cpp Sensor.cpp Simulation.cpp Simulator.cpp
obj = $(src:.cpp=.o)

CC_FLAGS = -std=c++11 -g -Wall -O2 -pedantic -pthread

# may add boost libraries here
LD_FLAGS = -lm

simulator: $(obj)
g++ -o $@ $^ $(CC_FLAGS)

.PHONY: clean
clean:
rm -f $(obj) simulator