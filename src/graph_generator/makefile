default: graph_gen

INC = ../../include
LIB = ../../lib

graph_generator.o: graph_generator.cpp test.cpp
	g++ -I$(INC) -std=c++11 -c graph_generator.cpp
	mkdir -p $(LIB)
	ar rcs $(LIB)/libgraph_generator.a graph_generator.o

graph_gen: graph_generator.o test.cpp
	g++ -I$(INC) -o graph_gen graph_generator.o test.cpp

clean: 
	rm -rf graph_gen *.o
