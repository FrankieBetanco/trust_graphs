default: input_gen 

main: main.cpp
	gcc -o main main.cpp

input_gen: input_generator.cpp
	g++ -std=c++11 -o input_gen input_generator.cpp
clean: 
	rm -rf main input_gen 
