#include "generate.hpp"
#include "graph.hpp"
#include "cli_args.hpp"
#include "utils.hpp"

#include <boost/graph/graphviz.hpp>

#include <iostream>
#include <random>

int
main(int argc, const char* argv[])
{
  cli_args args = process_cli_args(argc, argv);
  default_random_engine eng;
  eng.seed(args.seed);
  graph g = generate_graph(args, eng);
  
  boost::dynamic_properties dp;
  dp.property("node_id", get(boost::vertex_name, g));
  dp.property("weight", get(boost::edge_weight, g));
  write_graphviz_dp(cout, g, dp);
}
