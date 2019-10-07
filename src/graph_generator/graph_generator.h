#include <vector> 

class user { 
public: 
  vector <string> attributes; 
  int node_num; 
};

class graph { 
public: 
  std::vector <user *> users; 
  std::vector <std::vector <unsigned short> > adj;
  string filename; 

  graph(string filename);
  void edge_view(int from, int to);
  void add_privacy_edge(int attribute_num, int from, int to);
private: 
  void parse_input();
  void construct_graph();
  void print_binary_graph(int attribute_num);
};

