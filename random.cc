#include "utils.hpp"

#include <cmath>
#include <time.h>   
#include <sstream>
#include <iomanip>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/range.hpp>

using namespace std;

void
move(vertex v, const graph &g, std::set<vertex> &lonely,
     std::set<vertex> &connected, std::set<vertex> &saturated)
{
  lonely.erase(v);

  if (num_vertices(g) >= 3)
    connected.insert(v);
  else
    saturated.insert(v);
}

void
move_if_needed(vertex v, const graph &g, std::set<vertex> &connected,
               std::set<vertex> &saturated)
{
  int n = boost::num_vertices(g);
  int od = boost::out_degree(v, g);

  // A node can have the out degree of at most (n - 1).
  assert(od < n);

  // A node is saturated if its out degree is (n - 1).
  if (od == n - 1)
    {
      connected.erase(v);
      saturated.insert(v);
    }
}
