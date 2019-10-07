#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream>
#include <sstream>
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

class user { 
public: 
  vector <string> attributes; 
  int node_num; 
};

class graph { 
private: 
  vector <user *> users; 
  vector <vector <unsigned short> > adj;
  string filename; 
public: 
  graph(string filename) {
    this->filename = filename; 
    parse_input();
    construct_graph();
  }

  void parse_input() {
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

  void construct_graph() {
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

  void print_binary_graph(int attribute_num) {
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

  void add_privacy_edge(int attribute_num, int from, int to) {
        adj[from][to] ^= (1 << attribute_num);
  }

  void edge_view(int from, int to) {
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
};

int main(int argc, char **argv) {
  string filename; 
  if ( argc < 2 ) {
    cout << "usage: graph_gen [filename]\n";
    return -1;
  }
  filename = argv[1];

  graph g(filename);

  string rv;
  for (int i = 0; i < 10; i++) {
    g.edge_view(9, i); 
  }

  return 0;
}
