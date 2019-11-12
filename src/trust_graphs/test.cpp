#include <iostream>
#include <iomanip>
#include <cstdio>

#include "trust_graph.h"
using namespace std;

void cout_table_header() {
  cout << setw(15) << left << "attribute";
  cout << setw(15) << left << "privacy";
  cout << setw(15) << left << "anonymity";
  cout << setfill('-') << setw(46) << '\n' << setfill(' ');
  cout << '\n';
}

void cout_row(int col1, int col2, int col3) {
  cout << setw(15) << left << col1
       << setw(15) << left << col2
       << setw(15) << left << col3 << '\n';
}

int main(int argc, char **argv) {
  if (argc < 4) {
    cerr << "Usage: trust_graph [input_file] [n_events] [action]\n";
    return 1;
  }
  string file = argv[1];
  int n_events = stoi(argv[2]);
  string action = argv[3];

  trust_graph g(file);

  if (n_events > 0) {
    cout << "Doing " << n_events << " random events\n";
    for (int i = 1; i <= n_events; i++) {
      cout << '\r' << "Progress: " << i << "/" << n_events;
      cout << flush;
      g.random_event();
    }
    cout << '\n';
  }

  if (action == "edge") {
    if (argc < 6) {
      cerr << "Usage: trust_graph [input_file] [n_events] edge [node] [indegree/outdegree]\n";
      return 1;
    }
    int node = stoi(argv[4]);
    string arg = argv[5];
    if (node > g.users.size()) {
      cerr << "Node #" << node << " is out of range\n";
      return 1;
    }
    if (arg == "indegree") {
      cout << "Computing indegree for node " << node << "\n";
      cout_table_header();
      for (int i = 0; i < 9; i++) {
        pair <int, int> indegree = g.compute_indegree(node, i);
        cout_row(i, indegree.first, indegree.second);
      }
    } else if (arg == "outdegree") {
      cout << "Computing outdegree for node " << node << "\n";
      cout_table_header();
      for (int i = 0; i < 9; i++) {
        pair <int, int> outdegree = g.compute_outdegree(node, i);
        cout_row(i, outdegree.first, outdegree.second);
      }
    }
  }
  return 0;
}
