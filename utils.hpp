#ifndef UTILS_NETGEN_HPP
#define UTILS_NETGEN_HPP

#include "graph.hpp"
#include "mypoint.hpp"
#include "cli_args.hpp"
#include "utils.hpp"
#include "teventqueue.hpp"

#include <iostream>
#include <iterator>
#include <set>
#include <vector>
#include <utility>

#include <boost/graph/random.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/iterator/counting_iterator.hpp>

using namespace std;

// The network type.
enum class nt_t {random_network, gabriel_network};

nt_t
nt_interpret (const string &nt);

/**
 * Names the vertices.
 */
void
name_vertices(graph &g);

/**
 * Return a container with vertexes of a graph.
 */
template<typename C>
C
get_vertexes(const graph &g)
{
  C c;

  // Copy the vertex descriptors.
  std::copy(vertices(g).first, vertices(g).second,
            inserter(c, c.begin()));

  return c;
}

/**
 * Generate a random number from min to max, including both min and
 * max.
 */
template<typename E>
int
get_random_int(int min, int max, E &eng)
{
  std::uniform_int_distribution<> d(min, max);
  return d(eng);
}

/**
 * Get a random element from a container.
 */
template <typename C, typename E>
typename C::value_type
get_random_element(const C &c, E &eng)
{
  assert(!c.empty());

  typename C::const_iterator i = c.begin();
  std::advance(i, get_random_int(0, c.size() - 1, eng));

  return *i;
}

/**
 * Interpret a string, and return the matched enum.
 */
template <typename T>
T
interpret(const std::string &name, const std::string &text,
          const std::map <std::string, T> &m)
{
  auto i = m.find (text);

  if (i == m.end ())
    {
      std::cerr << "Wrong value of " << name << ".  Choose one of:";
      for (auto &p: m)
        std::cerr << " " << p.first;
      std::cerr << std::endl;
      exit (1);
    }

  return i->second;
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
      BGL_FORALL_OUTEDGES_T(src, e, g, graph)
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

  return g;
}

/**
 * Generate the Gabriel graph.  The graph has one connected component.
 * We don't allow for loop edges (i.e. that start and end at the same
 * node), and we don't allow for parallel edges.
 *
 * @return the graph
 */

graph
generate_gabriel_graph(const cli_args &args);

template<typename T>
graph
generate_graph(const cli_args &args, T &eng)
{
  graph g;

  nt_t nt = nt_interpret(args.nt);
  switch (nt)
    {
    case nt_t::random_network:
      g = generate_random_graph(args, eng);
      break;
      
    case nt_t::gabriel_network:
      g = generate_gabriel_graph(args);
      break;

    default:
      abort();
    }
  
  // Name the vertexes.
  name_vertices(g);

  return g;
}

#endif /* UTILS_NETGEN_HPP */
