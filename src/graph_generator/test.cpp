#include <iostream>

#include "graph_generator.h"
using namespace std;

int main(int argc, char **argv) {
  string filename; 
  if ( argc < 3 ) {
    cout << "usage: graph_gen [filename] [action]\n";
    cout << "actions: v\n";
    return -1;
  }
  filename = argv[1];

  graph g(filename);

  if ( strcmp(argv[2], "v") == 0 ) {
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
      cout << " -if more than one node is specified, then that nodes view of\n";
      cout << "  all of the nodes following the first will be shown\n"; 
      cout << "Example Usages:\n"; 
      cout << "graph_gen [filename] v 10\n"; 
      cout << "graph_gen [filename] v 10 15 20 30\n";
    }
  }

  return 0;
}
