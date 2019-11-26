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
  : dis(0,1), graph(filename), gen(25555), time(0)
{
  adj_trust.resize(users.size());
  for(int i = 0; i < adj_trust.size(); i++) {
    adj_trust[i].resize(users.size());
  }

  adj_interaction.resize(users.size());
  for (int i = 0; i < users.size(); i++) {
      adj_interaction[i].resize(users.size());
      for(int j = 0; j < adj_interaction[i].size(); j++) {
        adj_interaction[i][j] = 0;
      }
  }

  dfs_tree.resize(users.size()); 
  for (int i = 0; i < dfs_tree.size(); i++) {
    dfs_tree[i].resize(users.size());
    for (int j = 0; j < dfs_tree.size(); j++) {
      dfs_tree[i][j] = 0;
    }
  }
  compute_trust_graph();
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
    user_interaction(from, to, 0);
    add_privacy_edge(attribute_num, from, to);
  } else if (privacy && !add) {
    remove_privacy_edge(attribute_num, from, to);
    user_interaction(from, to, 1);
  } else if (!privacy && add) {
    add_anonymity_edge(attribute_num, from, to);
    user_interaction(from, to, 0);
  } else if (!privacy && !add) {
    remove_anonymity_edge(attribute_num, from, to);
    user_interaction(from, to, 1);
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
    trustedness -= (((adj_privacy[from][to] >> i) & 1) * TRUST_PERCENTAGES[i]);
    trustedness -= (((adj_anonymity[from][to] >> i) & 1) * TRUST_PERCENTAGES[i]);
  }

  // make trust strictly between 0 and 1
  trustedness += adj_interaction[from][to];
  if (trustedness > 1) trustedness = 1;
  if (trustedness < 0) trustedness = 0;
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

/* compute the shortest trust path from source to destination
 * params: 
 *  source: the starting node of the path
 *  destination: the terminating node of the path
 */
void trust_graph::min_path(int source, int destination) {
  priority_queue <pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;

  users[source]->distance = 0;

  pq.push(make_pair(users[source]->distance, source));

  while ( !pq.empty() ) {
    pair <double, int> u = pq.top();
    pq.pop();

    for (int i = 0; i < adj_trust[u.second].size(); i++) {
      if ( i != u.second && adj_trust[u.second][i] != 0 ) {
        double test_distance = u.first + adj_trust[u.second][i];
        if (test_distance < users[i]->distance) {
          users[i]->distance = test_distance;
          users[i]->predecessor = u.second;
          pq.push(make_pair(users[i]->distance, i));
        }
      }
    }
  }

  // print out the path that was found
  cout << "Path from destination to source\n";
  int tmp = destination;
  double length = users[tmp]->distance;
  while ( tmp != -1 ) {
    tmp = users[tmp]->predecessor;
    if ( tmp != -1 ) {
      cout << tmp << "<-";
    } else {
      cout << destination << '\n';
    }
  }
  cout << "Path length = " << length << '\n';
}

double EDGE_THRESHOLD = 0.5;

/* run dfs starting at the given source node. This will construct a 
 * dfs tree, dfs_tree that is a field of the trust_graph class, 
 * and store the discovery and finishing times in the user node
 * params: 
 *  source: the starting node of the dfs
 */
void trust_graph::dfs(int source) {
  users[source]->discovered = 1;
  users[source]->discovery_time = time++;
  for (int i = 0; i < adj_trust[source].size(); i++) {
    if ( adj_trust[source][i] > EDGE_THRESHOLD && !users[i]->discovered ) {
      dfs_tree[source][i] = 1;
      dfs(i);
    }
  }
  users[source]->finishing_time = time++;
}

/* this will print the dfs tree creaated afer dfs is called */
void trust_graph::print_dfs_tree() {
  for (int i = 0; i < dfs_tree.size(); i++) {
    for (int j = 0; j < dfs_tree[i].size(); j++) {
      if ( i != j ) {
        cout << setw(8) << dfs_tree[i][j];
      } else {
        string times = "(" + to_string(users[i]->discovery_time) + "/" + to_string(users[i]->finishing_time) + ")";
        cout << setw(8) << times;
      }
    }
    cout << '\n';
  }
}

/* this will find the strongly trusted components of the trust graph */
void trust_graph::strongly_trusted_components() {
  priority_queue <pair<int, int> > finishing_times;

  // run the initial dfs to find finishing times
  for (int i = 0; i < users.size(); i++) {
    if (!users[i]->discovered) {
      dfs(i);
    }
  }

  for (int i = 0; i < users.size(); i++) {
    finishing_times.push(make_pair(users[i]->finishing_time, i)); 
  }

  // reset users discovered to 0
  for (int i = 0; i < users.size(); i++) {
    users[i]->discovered = 0;
    users[i]->discovery_time = -1;
    users[i]->finishing_time = -1;
  }

  // reset the dfs tree
  for (int i = 0; i < dfs_tree.size(); i++) {
    for (int j = 0; j < dfs_tree[i].size(); j++) {
      dfs_tree[i][j] = 0;
    }
  }

  // transpose the graph
  for (int i = 0; i < adj_trust.size(); i++) {
    for (int j = 0; j < adj_trust.size(); j++) {
      double tmp = adj_trust[i][j]; 
      adj_trust[i][j] = adj_trust[j][i];
      adj_trust[j][i] = adj_trust[i][j];
    }
  }

  // run dfs again on componenets in order of decreasing finishing time
  int n = 0;
  while (!finishing_times.empty()) {
    if ( !users[finishing_times.top().second]->discovered ) {
      cout << "component " << n << " consists of: ";
      n++;
      dfs(finishing_times.top().second);
      for (int j = 0; j < users.size(); j++) {
        if (users[j]->finishing_time > 0 && !users[j]->printed) {
          cout << j << " ";
          users[j]->printed = 1;
        }
      }
      cout << '\n';
    }
    finishing_times.pop();
  }
}

/* modify interaction number. this is used when calculating node to node trust
 * params: 
 *  n1: the node that is doing the interaction
 *  n2: the node that is receiving the interaction
 *  pn: whether the interaction is positive (n1 increases trust of n2) or 
 *      negative (n1 increases distrust of n2)
 */
void trust_graph::user_interaction(int n1, int n2, int pn) {
  if (pn) {
    adj_interaction[n1][n2] += .01;
    if (adj_interaction[n1][n2] > 1) adj_interaction[n1][n2] = 1;
  } else {
    adj_interaction[n1][n2] -= .01;
    if (adj_interaction[n1][n2] < -1) adj_interaction[n1][n2] = -1;
  }
}

void trust_graph::group_cross_section() {
  // create a new graph of the cross sectional graph
  vector<vector<unsigned short> > user_group_graph(groups.size(), vector<unsigned short>(users.size()));

  for (int i = 0; i < groups.size(); i++) {
    for (int j = 0; j < groups[i]->members.size(); j++) {
      if (i != j) {
        int num = random() % 512;
        user_group_graph[i][j] = num;
        user_group_graph[j][i] = num;
      }
    }
  }

  for (int i = 0; i < user_group_graph.size(); i++) {
    for (int j = 0; j < user_group_graph[i].size(); j++) {
      cout << setw(5) << user_group_graph[i][j];
    }
    cout << '\n';
  }
}
