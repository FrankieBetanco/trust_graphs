#include <iostream>

#include "trust_graph.h"
using namespace std;

int main(int argc, char **argv) {
  trust_graph g("filename");

  for (int i = 0; i < 10; i++) {
    g.random_anonymity_event();
  }

  return 0;
}
