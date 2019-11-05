default: input_gen 

INC = ../../include
LIB = ../../lib

input_generator.o: input_generator.cpp
	g++ -I$(INC) -std=c++11 -c input_generator.cpp
	ar rcs $(LIB)/libinput_generator.a input_generator.o

input_gen: input_generator.o test.cpp
	g++ -I$(INC) -o input_gen input_generator.o test.cpp

clean: 
	rm -rf input_gen *.o
