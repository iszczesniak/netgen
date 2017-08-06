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

// The set of slices.  It's used to describe available
// slices, which are all avaiable along a single path.
typedef std::set<int> SSC;

// The set of SSC.
typedef std::set<SSC> SSSC;

namespace boost {
  // Describes the set of available slices on an edge.
  enum edge_ssc_t {edge_ssc};

  BOOST_INSTALL_PROPERTY(edge, ssc);

  // Describes the number of slices on an edge.
  enum edge_nosc_t {edge_nosc};

  BOOST_INSTALL_PROPERTY(edge, nosc);
}

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

// The label is a tuple (c, S, e, v) of cost c, SSC S, edge descriptor
// e and vertex descriptor v.  The label stores cost c and SSC S of
// reaching along the edge e the target vertex v.  Why do we store
// both an edge descriptor and a vertex descriptor?  Read on.
//
// In the tuple we chose to store an edge descriptor, not a vertex
// descriptor, so that we can allow for multigraphs (i.e. graphs with
// parallel edges).  However, we cannot store only the edge descriptor
// and figure out the target vertex descriptor from that edge
// descriptor for two reasons:
//
// 1) There is one special case of an edge descriptor for which we
// cannot figure out the target vertex descriptor: the null edge,
// which will not give us the source vertex descriptor, while we would
// need the source vertex descriptor.
//
// 2) For the BGL's undirected graph, using only an edge descriptor in
// a tuple would be wrong, because an edge descriptor does not
// uniquely identify an edge.  To uniquely identify an edge in the
// BGL's undirected graph, we need both an edge descriptor and a
// vertex descriptor.
//
// A note on the edge descriptors in an undirected graph in BGL.  Let
// e be an edge descriptor of a given edge, and r be the edge
// descriptor of the reverse edge of the given edge.  The can consider
// the descriptors equal, because e == r.  However, the descriptors
// still remember the direction of the edge, because target(e, g) !=
// target(r, g), more specifically, target(e) == source(r), and
// source(e) == target(r).  If we inserted two labels into a set which
// differed only with the edge descriptor (one descriptor is e, the
// other r), then one label would be overwritten by the other label.
typedef std::tuple<COST, SSC, edge> label;

// The set of labels we just call "labels".
typedef std::set<label> labels;

// A map of vertexes to labels.  It's used to store the labels for a
// given vertex.
typedef std::map<vertex, labels> V2L;

// The path.
typedef std::list<edge> path;

// The list of paths.
typedef std::list<path> plist;

// The SSC path.
typedef std::pair<path, SSC> sscpath;

// Node pair.
typedef std::pair<vertex, vertex> npair;

// The demand type: npair and the number of slices (nsc).
typedef std::pair<npair, int> demand;

#endif /* GRAPH_HPP */
