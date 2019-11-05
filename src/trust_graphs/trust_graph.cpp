#include <iostream>
#include <iomanip>
#include <vector>
#include <queue> 
#include <algorithm> 
#include <random>

#include "trust_graph.h"
using namespace std;

trust_graph::trust_graph(string filename) : graph(filename), gen(rd()), dis(0,1) {
  //gen = new mt19937(rd());
}

double trust_graph::rand() {
  return dis(gen);
}

/* either adds or removes a privacy or anonymity edge between two random 
 * users. Equal probability of adding edge, equal probability of doing 
 * a privacy or anonymity edge. 
 * TODO: give ability to adjust probabilities
 */
void trust_graph::random_anonymity_event() {
  double privacy_edge_probability   = 0.5;
  double anonymity_edge_probability = 1 - anonymity_edge_probability;
  double add_edge_probability       = 0.5; 
  double remove_edge_probability    = 1 - add_edge_probability;
  int privacy, add;


  if (rand() <= privacy_edge_probability) {
    privacy = 1;
  } else {
    privacy = 0;
  }

  if (rand() <= add_edge_probability) {
    add = 1;
  } else {  
    add = 0;
  }

  if (add) {
    cout << "adding ";
  } else {
    cout << "removing ";
  }

  if (privacy) {
    cout << "privacy edge ";
  } else {
    cout << "anonymity edge ";
  }

  int n_nodes = 10;
  int from = n_nodes * rand();
  int to = n_nodes * rand();
  while ( from == to ) {
    to = n_nodes * rand();
  }
  cout << "from " << from << " to " << to;

  cout << endl;
}

void trust_graph::random_privacy_event() {

}

void trust_graph::random_event() {

}

int trust_graph::compute_indegree(int node) {

  return 0;
}

int trust_graph::compute_outdegree(int node) {

  return 0;
}

double trust_graph::compute_trust(int node) {

  return 0.0;
}

double trust_graph::node_trust(int from, int to) {

  return 0.0;
}

double trust_graph::friendship_trust(int from, int to) {

  return 0.0;
}
