#include <vector> 
#include <string>
#include <utility>
#include <limits>

class user { 
public: 
  std::vector <std::string> attributes; 
  int predecessor;
  double distance;
  int discovery_time; 
  int finishing_time;
  int discovered;
  int node_num; 
  int printed; 
  user() { 
    predecessor = -1; 
    distance = std::numeric_limits <double>::max(); 
    discovery_time = -1;
    finishing_time = -1;
    discovered = 0;
    printed = 0;
  }
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
  std::vector <std::vector <unsigned short> > adj_privacy;
  std::vector <std::vector <unsigned short> > adj_anonymity;
  std::string filename; 

  graph(std::string filename);
  void node_view(int from, int to);
  void node_view_all(int from); 
  void add_privacy_edge(int attribute_num, int from, int to);
  void remove_privacy_edge(int attribute_num, int from, int to);
  void add_anonymity_edge(int attribute_num, int from, int to);
  int node_out_of_range(int from, int to);
  void remove_anonymity_edge(int attribute_num, int from, int to);
  void print_binary_graph(int attribute_num, std::vector<std::vector <unsigned short > > &adj);
  void print_privacy_graph(int attribute_num);
  void print_anonymity_graph(int attribute_num);
  void print_groups(); 
  void print_group_membership_graph();
private: 
  void parse_input();
  void construct_graph();
};

