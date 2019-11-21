/* disjoint_set.h
  Interface and subclass specification for disjoint sets.
  James S. Plank
  Tue Sep 25 15:48:18 EDT 2018
*/

#pragma once
#include <vector>
using namespace std;

/* The Disjoint Set API is implemented as a c++ interface, 
 *    because I am implementing it three ways.  Each subclass
 *       implementation is in its own cpp file. */

class DisjointSet {
  public:
    DisjointSet(int nelements);
    int Union(int s1, int s2);
    int Find(int element);
    void Print();
  protected:
    vector <int> links;
    vector <int> ranks;
};
