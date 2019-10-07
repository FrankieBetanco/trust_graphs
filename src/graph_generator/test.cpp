#include <iostream>

#include "graph_generator.h"
using namespace std;

int main(int argc, char **argv) {
  string filename; 
  string action;

  if ( argc < 3 ) {
    cout << "usage: graph_gen [filename] [action]\n";
    cout << "actions: v, p\n";
    return -1;
  }
  filename = argv[1];
  action = argv[2];

  graph g(filename);

  if ( action == "v" ) {
    if ( argc == 4 ) {
      g.node_view_all(stoi(argv[3]));
    } else if ( argc >= 5 ) {
      for (int i = 4; i < argc; i++) {
        g.node_view(stoi(argv[3]), stoi(argv[i]));
      }
    } else {
      cout << "Usage: graph_gen [filename] v (node)+\n"; 
      cout << " -if action is v, then specify one or more nodes\n"; 
      cout << " -if a single node is specified, then that nodes view of all\n"; 
      cout << "  other nodes in the graph is shown\n"; 
      cout << " -if more than one node is specified, then that nodes view\n";
      cout << "  of all of the nodes following the first will be shown\n"; 
      cout << "Example Usages:\n"; 
      cout << "graph_gen [filename] v 10\n"; 
      cout << "graph_gen [filename] v 10 15 20 30\n";
    }
  }

  if ( action == "p" ) {
    if ( argc == 4 ) {
      g.print_binary_graph(stoi(argv[3]));
    } else {
      cout << "Usage: graph_gen [filename] p [attribute num]\n";
    }
  }

  return 0;
}
