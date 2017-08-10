#include "graph.hpp"
#include "cli_args.hpp"
#include "utils.hpp"

#include <boost/graph/graphviz.hpp>

#include <iostream>
#include <random>

using namespace std;

int
main(int argc, const char* argv[])
{
  cli_args args = process_cli_args(argc, argv);
  default_random_engine eng;
  eng.seed(args.seed);
  graph g = generate_graph(args, eng);
  write_graphviz(cout, g);
}
