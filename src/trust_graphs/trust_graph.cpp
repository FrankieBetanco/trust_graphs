#include <iostream>
#include <iomanip>
#include <vector>
#include <queue> 
#include <algorithm> 
#include <random>
#include <map>
#include <set>

#include "trust_graph.h"
#include "disjoint_set.h"
using namespace std;

const double FIRST_NAME = 0.25;
const double LAST_NAME = 0.25;
const double AGE = 0.25;
const double GENDER = 0.05;
const double ID = 0.05;
const double EMAIL = 0.05;
const double SERVICES = 0.05;
const double GROUPS = 0.05;
const double TRUST_PERCENTAGES[] = {0.25, 0.25, 0.25, 0.05, 0.05, 0.05, 0.05, 
                                    0.05, 0.05};

trust_graph::trust_graph(string filename) 
  : dis(0,1), graph(filename), gen(rd()) {
  adj_trust.resize(users.size());
  for(int i = 0; i < adj_trust.size(); i++) {
    adj_trust[i].resize(users.size());
  }
}

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

// compute the overall trustworthiness of a user by summing up the 
// amount all other users trust "node" and dividing by the number of 
// users - 1 since you don't want to consider how much a node trusts 
// itself in the calculation of overall trust
double trust_graph::compute_trustworthiness(int node) {
  double trustworthiness = 0.0;
  for (int i = 0; i < adj_privacy.size(); i++) {
    if ( i != node) trustworthiness += node_trust(i, node);   
  }
  return trustworthiness / (users.size() - 1);
}

// compute how much node "from" trusts node "to"
double trust_graph::node_trust(int from, int to) {
  double trustedness = 1.0; 
  for (int i = 0; i < 8; i++) {
    trustedness -= ((adj_privacy[from][to] >> i) & 1) * TRUST_PERCENTAGES[i];
    trustedness -= ((adj_anonymity[from][to] >> i) & 1) * TRUST_PERCENTAGES[i];
  }
  // round off to 0 when values are very small
  if (trustedness < 1e-10) trustedness = 0.0;
  return trustedness;
}

// computes the sum of the trust from node "from" to node "to", and
// vice versa, then sums up, and divides by two. Intuitively, this is the
// "strength" of the friendship between two nodes
double trust_graph::friendship_trust(int from, int to) {
  return (node_trust(from, to) + node_trust(to, from)) /2;
}

void trust_graph::trust_graph::compute_trust_graph() {
  for (int i = 0; i < adj_trust.size(); i++) {
    for (int j = 0; j < adj_trust[i].size(); j++) {
      if (i != j) {
        adj_trust[i][j] = node_trust(i,j);
      }
    }
  }
}

// prints the weighted graph of the trust between users
void trust_graph::print_trust_graph() {
  // print table header
  cout << setw(6) << " ";
  for(int i = 0; i < adj_trust.size(); i++) {
    cout << setw(6) << left << i << " "; 
  }
  cout << '\n';
  for (int i = 0; i < adj_trust.size() + 1; i++) {
    cout << setw(7) << setfill('-') << "";
  }
  cout << setfill(' ') << '\n';

  for (int i = 0; i < adj_trust.size(); i++) {
    cout << setw(4) << left << i << "| ";
    for (int j = 0; j < adj_trust[i].size(); j++) {
      cout << setw(6) << setprecision(2) << left << adj_trust[i][j] << " "; 
    }
    cout << '\n';
  }
}

// compute minimum spanning trust tree using kruskals algorithm
void trust_graph::min_spanning_tree() {
  multimap <double, pair<int, int>, greater<double> > edges; 
  set <pair <int, int> > mst_edges;
  DisjointSet *d = new DisjointSet(adj_trust.size());

  // insert all edges into multimap
  for (int i = 0; i < adj_trust.size(); i++) {
    for (int j = 0; j < adj_trust[i].size(); j++) {
      if (i != j) {
        edges.insert(make_pair(adj_trust[i][j], make_pair(i,j)));
      }
    }
  }

  // start contructing the mst
  for (auto it = edges.begin(); it != edges.end(); it++) {
    int from = it->second.first; 
    int to = it->second.second;
    if ( d->Find(from) != d->Find(to) ) {
      d->Union(d->Find(from), d->Find(to));
      mst_edges.insert(make_pair(from, to));
    }
  }

  // print mst
  cout << "MST contains: \n";
  for (auto it = mst_edges.begin(); it != mst_edges.end(); it++) {
    int from = it->first;
    int to = it->second;
    cout << from << "->" << to << ": " << adj_trust[from][to] << '\n';
  }
}
