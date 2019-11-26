#include "graph_generator.h" 
#include <random>
#include <utility>

class trust_graph : public graph {
  private: 
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
    int time;

  public: 
    std::vector <std::vector<double> > adj_trust;
    std::vector <std::vector<double> > dfs_tree;
    trust_graph(std::string filename);
    double rand();
    void random_event();
    std::pair <int, int> compute_indegree(int node, int attribute); 
    std::pair <int, int> compute_outdegree(int node, int attribute);
    double compute_trustworthiness(int node); 
    double node_trust(int from, int to); 
    double friendship_trust(int from, int to);
    void compute_trust_graph();
    void print_trust_graph();
    void min_spanning_tree();
    void min_path(int source, int destination);
    void dfs(int source);
    void print_dfs_tree();
    void strongly_trusted_components();
};
