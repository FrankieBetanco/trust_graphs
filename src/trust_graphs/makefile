default: graph_gen

LIB = -L../../lib -lgraph_generator -ldisjoint_set
INC = -I../../include

trust_graph.o: trust_graph.cpp test.cpp
	g++ $(INC) -std=c++11 -c trust_graph.cpp

graph_gen: trust_graph.o test.cpp
	g++ $(INC) -std=c++11 -o trust_graph test.cpp trust_graph.o $(LIB)

clean: 
	rm -rf trust_graph *.o
