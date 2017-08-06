#include "graph.hpp"
#include "cli_args.hpp"
#include "sim.hpp"
#include "stats.hpp"
#include "utils.hpp"

using namespace std;

int
main(int argc, const char* argv[])
{
  cli_args args = process_cli_args(argc, argv);
  graph g = generate_graph(args);
}
