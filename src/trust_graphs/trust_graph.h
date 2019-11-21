#include "graph_generator.h" 
#include <random>
#include <utility>

class trust_graph : public graph {
  private: 
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

  public: 
    std::vector <std::vector<double> > adj_trust;
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
};
