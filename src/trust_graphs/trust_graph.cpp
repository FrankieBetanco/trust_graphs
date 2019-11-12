#include <iostream>
#include <iomanip>
#include <vector>
#include <queue> 
#include <algorithm> 
#include <random>

#include "trust_graph.h"
using namespace std;

trust_graph::trust_graph(string filename) : graph(filename), gen(rd()), 
  dis(0,1) { }

double trust_graph::rand() {
  return dis(gen);
}

/* either adds or removes a privacy or anonymity edge between two random 
 * users. Equal probability of adding edge, equal probability of doing 
 * a privacy or anonymity edge. 
 * TODO: give ability to adjust probabilities
 */
void trust_graph::random_event() {
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

  int n_nodes = users.size();
  int from = n_nodes * rand();
  int to = n_nodes * rand();
  while ( from == to ) {
    to = n_nodes * rand();
  }

  int n_attributes = 9;
  int attribute_num = 10 * rand();
  if (privacy && add) {
    add_privacy_edge(attribute_num, from, to);
  } else if (privacy && !add) {
    remove_privacy_edge(attribute_num, from, to);
  } else if (!privacy && add) {
    add_anonymity_edge(attribute_num, from, to);
  } else if (!privacy && !add) {
    remove_anonymity_edge(attribute_num, from, to);
  }
}

// computes the indegree of node for attribute.
pair <int, int> trust_graph::compute_indegree(int node, int attribute) {
  int anonymity_indegree = 0;
  int privacy_indegree = 0;

  for (int i = 0; i < adj_anonymity.size(); i++) {
    if ((adj_anonymity[i][node] >> attribute) & 1) {
      anonymity_indegree++;
    } else if ((adj_privacy[i][node] >> attribute) & 1) {
      privacy_indegree++;
    }
  }
  return make_pair(anonymity_indegree, privacy_indegree);
}

// compute outdegree of a node for an attribute
pair<int, int> trust_graph::compute_outdegree(int node, int attribute) {
  int anonymity_outdegree = 0; 
  int privacy_outdegree = 0;

  for (int i = 0; i < adj_anonymity.size(); i++) {
    if ((adj_anonymity[node][i] >> attribute) & 1) {
      anonymity_outdegree++;
    } else if ((adj_privacy[node][i] >> attribute) & 1) {
      privacy_outdegree++;
    }
  }
  return make_pair(anonymity_outdegree, privacy_outdegree);
}

double trust_graph::compute_trustworthiness(int node) {

  return 0.0;
}

// compute how much node from trusts node to
double trust_graph::node_trust(int from, int to) {
  return 0.0;
}

double trust_graph::friendship_trust(int from, int to) {
  return 0.0;
}
