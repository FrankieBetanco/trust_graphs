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

graph::graph(string filename) {
  this->filename = filename; 
  parse_input();
  construct_graph();
}

void graph::node_view(int from, int to) {
  if ( from < 0 || from >= adj.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
    return;
  } else if ( to < 0 || to >= adj.size() ) {
    cerr << "Error: to (" << to << ") out of range.\n";
    return;
  }
  unsigned short permissions; 
  string attributes; 

  permissions = adj[to][from];
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

void graph::node_view_all(int from) {
  if ( from < 0 || from >= adj.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
    return;
  }
  for (int to = 0; to < adj.size(); to++) {
    node_view(from, to);  
  }
}

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
    adj[i][i] = 0; 
  }
}

void graph::print_binary_graph(int attribute_num) {
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

void graph::add_privacy_edge(int attribute_num, int from, int to) {
  if ( from < 0 || from > adj.size() ) {
    cerr << "Error: from(" << from << ") out of range.\n";
  } else if ( to < 0 || from > adj.size() ) {
    cerr << "Error: to(" << to << ") out of range.\n";
  }
      adj[from][to] |= (1 << attribute_num);
}
