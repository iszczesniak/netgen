#include "utils.hpp"

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/range.hpp>

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

  for(vertex v: boost::make_iterator_range(vertices(g)))
    {
      ostringstream out;
      out << "v" << setw(width) << setfill('0') << count++;
      boost::get(boost::vertex_name, g, v) = out.str();
    }
}

void
calc_sp_stats(const graph &g, dbl_acc &hop_acc, dbl_acc &len_acc)
{
  // Calculate stats for shortest paths.
  for (vertex src: boost::make_iterator_range(boost::vertices(g)))
    {
      vector<int> dist(num_vertices(g));
      vector<vertex> pred(num_vertices(g));

      boost::dijkstra_shortest_paths
        (g, src,
         boost::predecessor_map(&pred[0]).distance_map(&dist[0]));

      for (vertex dst: boost::make_iterator_range(boost::vertices(g)))
        if (src != dst)
          {
            // Make sure the path was found.
            assert(pred[dst] != dst);

            // Record the number of hops.
            int hops = 0;
            vertex c = dst;
            while(c != src)
              {
                c = pred[c];
                ++hops;
              }
            hop_acc(hops);

            // Record the path length.
            len_acc(dist[dst]);
          }
    }
}
