#include "utils.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std;

// Handles the network parameter.
nt_t
nt_interpret (const string &nt)
{
  map <string, nt_t> nt_map;
  nt_map["random"] = nt_t::random_network;
  nt_map["gabriel"] = nt_t::gabriel_network;
  return interpret ("network type", nt, nt_map);
}

void
name_vertices(graph &g)
{
  int count = 1;

  int number = num_vertices(g);
  int width = int(log10(number)) + 1;

  for(vertex v: make_iterator_range(vertices(g)))
    {
      ostringstream out;
      out << "v" << setw(width) << setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}
