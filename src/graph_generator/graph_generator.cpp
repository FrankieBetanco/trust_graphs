#include <iostream> 
#include <iomanip>
#include <string> 
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

#include "graph_generator.h"
using namespace std;

// These are used to index into the fields of the user attributes
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
#define PERMISSIONS_ANON 10

/* constructor */
graph::graph(string filename) {
  this->filename = filename; 
  parse_input();
  construct_graph();
}

/* prints out the information that node "from" can read of node "to". 
 * This is done by examining the edges coming from "to" to "from", and 
 * seeing what is declared as private.
 * params: 
 *  from: index of the viewing node in the adjacency list
 *  to: index of the node to bee viewd in the adjacency list
 */
void graph::node_view(int from, int to) {
  if ( from < 0 || from >= adj_privacy.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
    return;
  } else if ( to < 0 || to >= adj_privacy.size() ) {
    cerr << "Error: to (" << to << ") out of range.\n";
    return;
  }
  unsigned short permissions; 
  string attributes; 

  permissions = adj_privacy[to][from];
  for (int i = 0; i < 9; i++) {
    // if the permission bit is set, then the user has declared this 
    // information private, and it should not be printed
    if ( permissions & (1 << i) ) {
      attributes += "X";
    }
    else { 
      attributes += users[to]->attributes[i];
    }
    if ( i + 1 < 9 ) {
      attributes += ", ";
    }
  }
  cout << "User " << from << " view of User " << to << ": " 
    << attributes << '\n';
}

/* print out the node "from"'s view of all other nodes in the system 
 * params: 
 *  from: the index of the viewing node in the adjacency list
 */
void graph::node_view_all(int from) {
  if ( from < 0 || from >= adj_privacy.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
    return;
  }
  for (int to = 0; to < adj_privacy.size(); to++) {
    node_view(from, to);  
  }
}

/* parse the input csv. For each line, create a new user, and save a pointer
 * to the newly created user in the vector of users
 */
void graph::parse_input() {
  string line; 
  string field;
  map <std::string, group *> groups;
  ifstream infile(filename);  
  int start, end;
  user *new_user;

  while ( getline(infile, line) ) {
    // parse fields of the user and store information in new user class
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
    // determine what groups the user is a member of and add them to the 
    // member list for each group
    stringstream ss(new_user->attributes[GROUPS]); 
    string word; 
    while ( ss >> word ) {
      // create a new group if one does not exist
      if ( groups.find(word) == groups.end() ) {
        groups[word] = new group(); 
        groups[word]->group_name = word; 
        this->groups.push_back(groups[word]);
      }
      // add the index of the user to the group
      groups[word]->members.push_back(users.size() - 1);
    }
  }

}

/* after parsing the input, create adjacency matrices for the privacy 
 * and anonymity graphs
 */
void graph::construct_graph() {
  int n_users; 

  // allocate memory adjacency matrices
  n_users = users.size(); 
  adj_privacy.resize(n_users);
  adj_anonymity.resize(n_users);
  for (int i = 0; i < adj_privacy.size(); i++) {
    adj_privacy[i].resize(n_users);
    adj_anonymity[i].resize(n_users);
    for (int j = 0; j < adj_privacy[i].size(); j++) {
      adj_privacy[i][j] = 0;
      adj_anonymity[i][j] = 0;
    }
  }

  // construct the privacy graph and anonymity graph
  for (int i = 0; i < n_users; i++) {
    for (int j = 0; j < n_users; j++) {
      adj_privacy[i][j] = (unsigned short) stoi(users[i]->attributes[PERMISSIONS]);
      adj_anonymity[i][j] = (unsigned short) stoi(users[i]->attributes[PERMISSIONS_ANON]);
    }
    adj_privacy[i][i] = 0; 
    adj_anonymity[i][i] = 0; 
  }
}

/* print the adjacency matrix for attribute_num in adjacency matrix adj. Since
 * each entry in the matrix will be a number, and whether each bit of the
 * number is set determines if there is an edge. E.g. If there were only 
 * 3 attributes: first name, last name, and age, the number 3 = 0b011 means
 * that first name is publc, but last name and age are private 
 * params: 
 *  attribute_num: The numbeer of the attribute for which an adjacency matrix
 *  should be printed
 *  adj: the adjacency matrix to search
 */
void graph::print_binary_graph(int attribute_num, vector<vector <unsigned short > > &adj) {
  if ( attribute_num < 0 || attribute_num > 8 ) {
    cerr << "Error: attribute_num(" << attribute_num << ") out of range.\n"; 
    return;
  }
  cout << "Attribute " << attribute_num << ": ";
  switch (attribute_num) {
    case 0: cout << "First Name\n"; 
            break;
    case 1: cout << "Last Name\n";
            break;
    case 2: cout << "Age\n"; 
            break;
    case 3: cout << "Gender\n"; 
            break;
    case 4: cout << "Primary Language\n"; 
            break;
    case 5: cout << "SNS ID\n"; 
            break;
    case 6: cout << "Email Address\n"; 
            break;
    case 7: cout << "Services\n"; 
            break;
    case 8: cout << "Groups\n"; 
            break;
    default: 
            cout << "Invalid attribute number\n";
            return;
  }
  cout << setw(6) << " ";
  for (int i = 0; i < adj.size(); i++) {
    cout << setw(5) << i; 
  }
  cout << "\n";;
  for (int i = 0; i < adj.size() * 5 + 10; i++) {
    cout << "-"; 
  }
  cout << "\n";
  for (int i = 0; i < adj.size(); i++) {
    cout << "Node #" << setw(4) << left << to_string(i) + ":"; 
    for (int j = 0; j < adj[i].size(); j++) {
      if ( adj[i][j] & (1 << attribute_num) ) {
        cout << setw(5) << "1";
      }
      else {
        cout << setw(5) << "0 ";
      }
    }
    cout << "\n";
  }
}

/* wrapper method for print_binary_graph to print the adjacency matrix
 * for privacy for a given attribute
 * params: 
 *  attribute_num: the number of the attrbute to be printed 
 */
void graph::print_privacy_graph(int attribute_num) {
  cout << "Privacy graph, ";
  print_binary_graph(attribute_num, adj_privacy);
}

/* wrapper method for print_binary_graph to print the adjacency matrix
 * for anonymity for a given attribute 
 * params: 
 *  attribute_num: the number of the attrbute to be printed 
 */
void graph::print_anonymity_graph(int attribute_num) {
  cout << "Anonymity Graph, ";
  print_binary_graph(attribute_num, adj_anonymity);
}

// print out the list of groups and their members; 
void graph::print_groups() {
  for (int i = 0; i < groups.size(); i++) {
    cout << "group name : " << setw(30) << setfill('.') << left 
      << groups[i]->group_name << "Members : ";
    for (int j = 0; j < groups[i]->members.size(); j++) {
      cout << groups[i]->members[j] << " ";
    }
    cout << '\n';
  }
}

/* This will print a matrix showing the for which groups each user is a member. 
 */
void graph::print_group_membership_graph() {
  for (int i = 0; i < groups.size(); i++) {
    cout << groups[i]->group_name << " = " << i << endl;;
  }
  cout << setw(10) << setfill(' ') << " ";
  for (int i = 0; i < groups.size(); i++) {
    cout << setw(4) << i;
  }
  cout << "\n";
  cout << setw(53) << setfill('-') << "-" << '\n';
  cout << setfill(' ');
  for (int i = 0; i < users.size(); i++) {
    cout << setw(10) << "User " + to_string(i) + ": ";
    for (int j = 0; j < groups.size(); j++) {
      if ( find(groups[j]->members.begin(), groups[j]->members.end(), i) 
          != groups[j]->members.end() ) {
        cout << setw(4) << "1";
      } else {
        cout << setw(4) << "0";
      }
    }
    cout << '\n';
  }
}

int graph::node_out_of_range(int from, int to) {
  if ( from < 0 || from > adj_privacy.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
    return 1;
  } else if ( to < 0 || from > adj_privacy.size() ) {
    cerr << "Error: to(" << to << ") out of range.\n";
    return 1;
  }
  return 0; 
}

/* this will create a new privacy edge for an attribute "attribute_num" from 
 * node "from" to node "to"
 * params: 
 *  attribute_num: the attribute for which to add a privacy edge
 *  from: the originating node for the edge
 *  to: the terminating node for the edge
 */
void graph::add_privacy_edge(int attribute_num, int from, int to) {
  if (node_out_of_range(from, to)) {
    return;
  }
  adj_privacy[from][to] |= (1 << attribute_num);
  adj_anonymity[from][to] &= ~(1 << attribute_num);
}

void graph::remove_privacy_edge(int attribute_num, int from, int to) {
  if (node_out_of_range(from, to)) {
    return;
  }
  adj_privacy[from][to] &= ~(1 << attribute_num);
}

void graph::add_anonymity_edge(int attribute_num, int from, int to) {
  if (node_out_of_range(from, to)) {
    return;
  }
  adj_anonymity[from][to] |= (1 << attribute_num);
  adj_privacy[from][to] &= ~(1 << attribute_num);
}

void graph::remove_anonymity_edge(int attribute_num, int from, int to) {
  if (node_out_of_range(from, to)) {
    return;
  }
  adj_anonymity[from][to] &= ~(1 << attribute_num);
}
