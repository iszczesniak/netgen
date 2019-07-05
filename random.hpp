#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "graph.hpp"
#include "cli_args.hpp"
#include "utils.hpp"

#include <boost/range.hpp>

#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <vector>
#include <utility>

using namespace std;

template<typename T>
void
set_distances(graph &g, int min, int max, T &eng)
{
  std::uniform_int_distribution<> rng(min, max);
  for (const auto &e: boost::make_iterator_range(boost::edges(g)))
    boost::get(boost::edge_weight, g, e) = rng(eng);
}

/**
 * Move the vertex from the lonely group to either the set of
 * connected or saturated vertexes.
 */
void
move(vertex v, const graph &g, std::set<vertex> &lonely,
     std::set<vertex> &connected, std::set<vertex> &saturated);

/**
 * Check whether to move the vertex from the set of connected vertexes
 * to the set of saturated vertexes.
 */
void
move_if_needed(vertex v, const graph &g, std::set<vertex> &connected,
               std::set<vertex> &saturated);

/**
 * Add a random edge.
 * @return true on success, false otherwise.
 */
template<typename T>
bool
add_random_edge(graph &g, std::set<vertex> &lonely,
                std::set<vertex> &connected,
                std::set<vertex> &saturated,
                T &eng)
{
  // The condition for the first edge ever created in the graph.
  if (lonely.size() >= 2 && connected.empty() && saturated.empty())
    {
      // Select two lone vertexes, which will be the end nodes of the
      // first edge created.
      vertex src = get_random_element(lonely, eng);
      // Move the src node from lonely before we pick the dst node.
      move(src, g, lonely, connected, saturated);
      vertex dst = get_random_element(lonely, eng);
      move(dst, g, lonely, connected, saturated);
      bool status = add_edge(src, dst, g).second;
      assert(status);
      return status;
    }
  // The condition for lonely vertexes and a connected component.
  else if (!lonely.empty() && !connected.empty())
    {
      // Add a new edge where one vertex belongs to the connected
      // component, while the other is a lone one.
      vertex src = get_random_element(lonely, eng);
      vertex dst = get_random_element(connected, eng);
      bool status = add_edge(src, dst, g).second;
      assert(status);
      move(src, g, lonely, connected, saturated);
      move_if_needed(dst, g, connected, saturated);
      return status;
    }
  // The condition for a connected component only.
  else if (lonely.empty() && connected.size() >= 2)
    {
      // Now we have to create an edge where both vertexes of the edge
      // belong to the connected component.  We have to be carefull
      // not to create a parallel edge.
      vertex src = get_random_element(connected, eng);
      // These are the vertexes that can be destination nodes.
      std::set<vertex> sifted = connected;
      sifted.erase(src);
      for(edge e: make_iterator_range(out_edges(src, g)))
        sifted.erase(target(e, g));
      // Now pick from the sifted set.
      vertex dst = get_random_element(sifted, eng);
      bool status = boost::add_edge(src, dst, g).second;
      assert(status);
      move_if_needed(src, g, connected, saturated);
      move_if_needed(dst, g, connected, saturated);
      return status;
    }

  return false;
}

/**
 * Generate the graph.  The graph has one connected component, but
 * there can be some lone vertexes.  We don't allow loop edges
 * (i.e. that start and end at some node), and we don't allow parallel
 * edges.
 *
 * @return the number of edges actually created.
 */
template<typename T>
graph
generate_random_graph(const cli_args &args, T &eng)
{
  assert(args.nr_nodes >= 2);
  assert(args.nr_edges.get() >= 0);

  // Create a graph with the following number of nodes.
  graph g = graph(args.nr_nodes);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);
  // The set of vertexes in the connected component that have not been
  // saturated yet.
  std::set<vertex> connected;
  // The set of saturated vertexes.  A saturated node is connected to
  // every other node with a single edge.
  std::set<vertex> saturated;

  // In every iteration we add a new random edge.
  for (int created = 0; created < args.nr_edges.get(); ++created)
    if (!add_random_edge(g, lonely, connected, saturated, eng))
      {
        assert(lonely.empty());
        assert(connected.size() <= 1);
        assert(saturated.size() >= args.nr_nodes - 1);
        // Fail, because we can't create the requested number of edges.
        abort();
      }

  // Make sure we created the requested number of edges.
  assert(num_edges (g) == args.nr_edges.get());

  set_distances(g, 1, 50, eng);

  return g;
}

#endif /* RANDOM_HPP */
