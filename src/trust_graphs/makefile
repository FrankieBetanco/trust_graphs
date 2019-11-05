default: graph_gen

LIB = -L../../lib -lgraph_generator
INC = -I../../include

trust_graph.o: trust_graph.cpp test.cpp
	g++ $(INC) $(LIB) -std=c++11 -c trust_graph.cpp

graph_gen: trust_graph.o test.cpp
	g++ -o trust_graph trust_graph.o test.cpp

clean: 
	rm -rf trust_graph *.o
