all: fcore-tsp

fcore-tsp:
	g++-10.1 -I../../src --std=c++20 -fconcepts -fcoroutines src/mainTSP-fcore.cpp -o ./build/app_TSP-fcore 2> error-gcc.err 

run:
	./build/app_TSP-fcore 
