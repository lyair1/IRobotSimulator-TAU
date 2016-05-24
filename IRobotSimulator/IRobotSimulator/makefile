# source files and object files
src = main.cpp configReader.cpp House.cpp Sensor.cpp Simulation.cpp Simulator.cpp AlgorithmRegistration.cpp AlgorithmLoader.cpp AlgorithmBase.cpp MakeUnique.cpp score_formula.cpp 
obj = $(src:.cpp=.o)

# shared object source files and object files
so_src = 200945657_A_.cpp 200945657_C_.cpp 200945657_B_.cpp
so_dep = AlgorithmBase.cpp score_formula.cpp
s_obj = $(so_src:.cpp=.so)
so_dep_obj = $(so_dep:.cpp=.o)

target = simulator

# flags
CC_FLAGS = -c -O2 -Wall -pthread -std=c++11 -pedantic -g 
SO_FLAGS = -Wall -pthread -std=c++11 -pedantic -g  -shared -fPIC
LD_FLAGS = -lm -lboost_system -lboost_filesystem -ldl
CC = g++

.DEFAULT_GOAL := all
.PHONY: all clean


all: clean $(target)

$(target): $(obj) $(s_obj)
	$(CC) -rdynamic -o $@ $(obj) $(LD_FLAGS)

$(s_obj): %.so: %.cpp $(so_dep_obj)
	$(CC) $(SO_FLAGS) $^ -o $@

$(obj): %.o: %.cpp
	$(CC) $(CC_FLAGS) $< -o $@
	
$(so_dep_obj): %.o: %.cpp
	$(CC) $(CC_FLAGS) -fPIC $< -o $@

clean:
	rm -f $(obj) $(so_obj) $(s_obj) $(target)