#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "cli_args.hpp"
#include "gabriel.hpp"
#include "graph.hpp"
#include "random.hpp"
#include "utils.hpp"

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

#endif // GENERATE_HPP
