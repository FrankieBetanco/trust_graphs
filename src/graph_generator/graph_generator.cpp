#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream>
#include <sstream>
using namespace std;

class user { 
public: 
  string first; 
  string last; 
  string age; 
  string gender; 
  string language; 
  string id; 
  string email;
  string services;
  string groups; 
  string permissions;

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
  }

  void parse_input() {
    string line; 
    string field;
    vector <string> fields; 
    ifstream infile(filename);  
    int start, end;
    user *new_user;

    while ( getline(infile, line) ) {
      start = 0; 
      end = 0;
      while ( line.find(",", end) != string::npos ) {
        end = line.find(",", start);
        field = line.substr(start, end - start);
        fields.push_back(field);
        start = end+1; 
      }
      new_user = new user();
      new_user->first       = fields[0];
      new_user->last        = fields[1];
      new_user->age         = fields[2];
      new_user->gender      = fields[3];
      new_user->language    = fields[4];
      new_user->id          = fields[5];
      new_user->email       = fields[6];
      new_user->services    = fields[7];
      new_user->groups      = fields[8];
      new_user->permissions = fields[9];
      users.push_back(new_user);
      fields.clear();
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
        adj[i][j] = (unsigned short) stoi(users[i]->permissions);
      }
    }
  }
};

int main(int argc, char **argv) {
  string filename; 
  if ( argc < 2 ) {
    cout << "usage: graph_gen [filename]\n";
  }
  filename = argv[1];

  graph g(filename);
  g.parse_input();
  g.construct_graph();
}
