#include <iostream> 
#include <string> 
#include <fstream>
#include <sstream>

#include "graph_generator.h"
using namespace std;

#define FIRST 0
#define LAST 1
#define AGE 2
#define GENDER 3
#define LANGUAGE 4
#define ID 5
#define EMAIL 6
#define SERVICES 7
#define GROUPS 8
#define PERMISSIONS 9

graph::graph(string filename) {
  this->filename = filename; 
  parse_input();
  construct_graph();
}

void graph::node_view(int from, int to) {
  unsigned short permissions; 
  string attributes; 

  permissions = adj[to][from];
  for (int i = 0; i < 9; i++) {
    if ( permissions & (1 << i) ) {
      attributes += users[to]->attributes[i];
    }
    else { 
      attributes += "X";
    }
    if ( i + 1 < 9 ) {
      attributes += ", ";
    }
  }
  cout << "User " << from << " view of User " << to << ": " 
    << attributes << '\n';
}

void graph::node_view_all(int from) {
  for (int to = 0; to < adj.size(); to++) {
    node_view(from, to);  
  }
}

void graph::parse_input() {
  string line; 
  string field;
  ifstream infile(filename);  
  int start, end;
  user *new_user;

  while ( getline(infile, line) ) {
    new_user = new user();
    users.push_back(new_user);
    start = 0; 
    end = 0;
    while ( line.find(",", end) != string::npos ) {
      end = line.find(",", start);
      field = line.substr(start, end - start);
      new_user->attributes.push_back(field);
      start = end+1; 
    }
  }
}

void graph::construct_graph() {
  int n_users; 

  n_users = users.size(); 
  adj.resize(n_users);
  for (int i = 0; i < adj.size(); i++) {
    adj[i].resize(n_users);
  }

  for (int i = 0; i < n_users; i++) {
    for (int j = 0; j < n_users; j++) {
      adj[i][j] = (unsigned short) stoi(users[i]->attributes[PERMISSIONS]);
    }
    adj[i][i] = 511; 
  }
}

void graph::print_binary_graph(int attribute_num) {
  cout << "Attribute " << attribute_num << '\n';
  for (int i = 0; i < adj.size(); i++) {
    for (int j = 0; j < adj[i].size(); j++) {
      if ( adj[i][j] & (1 << attribute_num) ) {
        cout << "1 ";
      }
      else {
        cout << "0 ";
      }
    }
    cout << "\n";
  }
}

void graph::add_privacy_edge(int attribute_num, int from, int to) {
      adj[from][to] |= (1 << attribute_num);
}
