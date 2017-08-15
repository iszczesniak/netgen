#ifndef CLI_ARGS_HPP
#define CLI_ARGS_HPP

#include <string>
#include <boost/optional.hpp>

using namespace std;

// The network type.
enum class nt_t {random_network, gabriel_network};

/**
 * These are the program arguments.  In this single class we store all
 * information passed at the command line.
 */
struct cli_args
{
  /// -----------------------------------------------------------------
  /// The network options
  /// -----------------------------------------------------------------
  
  /// The network type.
  std::string nt;

  /// The number of nodes in the graph.
  int nr_nodes;

  /// The number of edges in the graph.
  boost::optional<int> nr_edges;

  /// The seed.
  int seed;
};

/**
 * This function parses the command-line arguments.
 */
cli_args
process_cli_args(int argc, const char* argv[]);

#endif /* CLI_ARGS_HPP */
