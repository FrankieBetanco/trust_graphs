#include <vector> 
#include <string>

class user { 
public: 
  std::vector <std::string> attributes; 
  int node_num; 
};

class group {
  public:
  std::vector <int> members; 
  std::string group_name; 
  unsigned short permissions; 
};

class graph { 
public: 
  std::vector <user *> users; 
  std::vector <group *> groups; 
  std::vector <std::vector <unsigned short> > adj;
  std::string filename; 

  graph(std::string filename);
  void node_view(int from, int to);
  void node_view_all(int from); 
  void add_privacy_edge(int attribute_num, int from, int to);
  void print_binary_graph(int attribute_num);
  void print_groups(); 
private: 
  void parse_input();
  void construct_graph();
};

