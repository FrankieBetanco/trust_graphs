#include "graph_generator.h" 
#include <random>
class trust_graph : public graph {
  private: 
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

  public: 
    trust_graph(std::string filename);
    double rand();
    void random_anonymity_event();
    void random_privacy_event();
    void random_event();
    int compute_indegree(int node); 
    int compute_outdegree(int node);
    double compute_trust(int node); 
    double node_trust(int from, int to); 
    double friendship_trust(int from, int to);
};
