#include <iostream>

#include "graph_generator.h"
using namespace std;

int main(int argc, char **argv) {
  string filename; 
  if ( argc < 2 ) {
    cout << "usage: graph_gen [filename]\n";
    return -1;
  }
  filename = argv[1];

  graph g(filename);

  g.edge_view_all(1); 

  return 0;
}
