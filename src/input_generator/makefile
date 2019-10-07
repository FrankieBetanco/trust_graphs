default: input_gen 

input_generator.o: input_generator.cpp
	g++ -std=c++11 -c input_generator.cpp

input_gen: input_generator.o test.cpp
	g++ -o input_gen input_generator.o test.cpp

clean: 
	rm -rf input_gen *.o
