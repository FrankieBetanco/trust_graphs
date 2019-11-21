#include <iostream>
#include <iomanip>
#include <cstdio>
#include <map>

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
    g.compute_trust_graph();
  }

  if (action == "node") {
    if (argc < 6) {
      cerr << "Usage: trust_graph [input_file] [n_events] node [node] [indegree/outdegree]\n";
      return 1;
    }
    int node = stoi(argv[4]);
    string arg = argv[5];
    if (node > g.users.size()) {
      cerr << "Node #" << node << " is out of range\n";
      return 1;
    }

    if (arg == "indegree") { // computations on an individual node
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
    } else if (arg == "trustworthiness") {
        cout << "Node " << node << "'s overall trustworthiness is " 
             << g.compute_trustworthiness(node) << '\n';
    }
  } else if (action == "pair") { //computations on a pair of nodes
    if (argc < 7) {
      cerr << "Usage: trust_graph [input_file] [n_events] pair [node1] [node2] [trustedness]\n";
      return 1;
    }
    int node1 = stoi(argv[4]);
    int node2 = stoi(argv[5]);
    string arg = argv[6];
    if (arg == "trustedness") {
      cout << "Node1 trusts Node2: " <<  g.node_trust(node1, node2) << "/1.0\n";
      cout << "Node2 trusts Node1: " <<  g.node_trust(node2, node1) << "/1.0\n";
      cout << "Friendship Strength: " << g.friendship_trust(node1, node2) << '\n';
    }
  } else if (action == "all") { //computations on all of the nodes
    if (argc < 5) {
      cerr << "Usage: trust_graph [input_file] [n_events] all [trustworthiness]\n";
      return 1;
    }
    string arg = argv[4]; 
    if (arg == "trustworthiness") {
      multimap <double, int, greater<double> > users;
      for (int node = 0; node < g.users.size(); node++) {
        users.insert(make_pair(g.compute_trustworthiness(node), node));
      }
      for (auto i = users.begin(); i != users.end(); i++) {
        cout << "Node " << i->second << " overall trustworthiness: " 
             << i->first << '\n';
      }
    } else if (arg == "friendship") {
      multimap <double, string, greater<double> > users;
      for (int i = 0; i < g.users.size(); i++) {
        for (int j = 0; j < i; j++) {
          users.insert(make_pair(g.friendship_trust(i, j), to_string(i) + " to " + to_string(j)));
        }
      }
      for (auto i = users.begin(); i != users.end(); i++) {
        cout << i->second << " friendship strength: " << i->first << '\n';
      }
    }
  } else if (action == "print") {
    //g.print_trust_graph();
    g.min_spanning_tree();
  }

  return 0;
}
