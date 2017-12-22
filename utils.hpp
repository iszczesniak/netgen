#ifndef UTILS_HPP
#define UTILS_HPP

#include "cli_args.hpp"
#include "graph.hpp"
#include "utils.hpp"

#include <boost/graph/connected_components.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <iostream>

using namespace std;

// The accumulator with double values.                                                                      
typedef ba::accumulator_set<double, ba::stats<ba::tag::min,
                                              ba::tag::mean,
                                              ba::tag::max,
                                              ba::tag::variance> > dbl_acc;

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

#endif /* UTILS_HPP */
