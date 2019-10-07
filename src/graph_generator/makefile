default: graph_gen

graph_generator.o: graph_generator.cpp test.cpp
	g++ -std=c++11 -c graph_generator.cpp

graph_gen: graph_generator.o test.cpp
	g++ -o graph_gen graph_generator.o test.cpp

clean: 
	rm -rf graph_gen *.o
