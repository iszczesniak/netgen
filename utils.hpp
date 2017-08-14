#ifndef UTILS_HPP
#define UTILS_HPP

#include "cli_args.hpp"
#include "gabriel.hpp"
#include "graph.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <iostream>
#include <iterator>
#include <set>
#include <vector>
#include <utility>

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

#endif /* UTILS_HPP */
