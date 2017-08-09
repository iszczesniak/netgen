#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

// The type of cost of reaching a vertex.
typedef double COST;

/**
 * The type of the graph we use.  The edge_ssc_t property describes
 * the slices available, i.e. not already taken.
 */
typedef
boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		      boost::property<boost::vertex_name_t, std::string>,
                      boost::property<boost::edge_weight_t, COST> >
graph;

typedef graph::edge_descriptor edge;
typedef graph::vertex_descriptor vertex;

#endif /* GRAPH_HPP */
