# source files and object files
src = main.cpp configReader.cpp House.cpp Sensor.cpp Simulation.cpp Simulator.cpp AlgorithmLoader.cpp AlgorithmBase.cpp Path.cpp Point.cpp MakeUnique.cpp
obj = $(src:.cpp=.o)

# shared object source files and object files
so_src = 200945657_A_.cpp 200945657_C_.cpp 200945657_B_.cpp score_formula.cpp
so_dep = AlgorithmBase.cpp
s_obj = $(so_src:.cpp=.so)

target = simulator

# flags
CC_FLAGS = -c -O2 -Wall -pthread -std=c++11 -pedantic -g
SO_FLAGS = -Wall -pthread -std=c++11 -pedantic -g -shared -fPIC
LD_FLAGS = -lm -lboost_system -lboost_filesystem -ldl
CC = g++

.DEFAULT_GOAL := all
.PHONY: all clean


all: clean $(target)

$(target): $(obj) $(s_obj)
	$(CC) -o $@ $(obj) $(LD_FLAGS)

$(s_obj): %.so: %.cpp $(so_dep)
	$(CC) $(SO_FLAGS) $^ -o $@

$(obj): %.o: %.cpp
	$(CC) $(CC_FLAGS) $< -o $@
	
clean:
	rm -f $(obj) $(s_obj) $(target)