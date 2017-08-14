#ifndef GABRIEL_HPP
#define GABRIEL_HPP

#include "graph.hpp"
#include "cli_args.hpp"

using namespace std;

/**
 * Generate the Gabriel graph.  The graph has one connected component.
 * We don't allow for loop edges (i.e. that start and end at the same
 * node), and we don't allow for parallel edges.
 *
 * @return the graph
 */

graph
generate_gabriel_graph(const cli_args &args);

#endif /* GABRIEL_HPP */
