#include "gabriel.hpp"

#include "mypoint.hpp"
#include "teventqueue.hpp"
#include "utils.hpp"

#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <sstream>
#include <iomanip>

using namespace std;

graph
generate_gabriel_graph(const cli_args &args)
{
  assert(args.nr_nodes >= 2);
  srand (args.seed);
  // Create a graph with the following number of nodes.
  graph g = graph(args.nr_nodes);

  boost::property_map<graph, boost::edge_weight_t>::type
    wm = get(boost::edge_weight_t(), g);

  // The set of lone vertexes.
  std::set<vertex> lonely = get_vertexes<std::set<vertex> >(g);

  unsigned int number = args.nr_nodes;
  // Square kilometers required for the given number of nodes.
  unsigned skm = number * 10000;
  unsigned int w = std::sqrt(skm);
  unsigned int h = std::sqrt(skm);
  list<TNode *> P = generate_Nodes(w, h, number);
  
  list<TTriangle *> triangles;
  delaunayTriangulation(triangles, P);

  convertDelaunay2GabrielGraph(P);

  unsigned int edges = edgeNumber(P);
  
  map<TNode*, vertex> mapNV;
  
  std::set<vertex>::iterator itV = lonely.begin();
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
    mapNV[*it] = *itV;
  
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it, ++itV)
  {
      vertex src = mapNV[*it];
      set<TNode*> myEdges = (*it)->getEdges();
      
      for (set<TNode*>::iterator itE = myEdges.begin(); itE != myEdges.end(); ++itE)
      {
         vertex dst = mapNV[*itE];      
         (*itE)->removeEdge(*it);
         edge e;
         bool status;
         tie(e, status) = add_edge(src, dst, g);
         int dist = (int) (sqrt(dist2((*it)->getPoint(), (*itE)->getPoint())) + 0.5);
         wm[e] = dist;
         assert(status);    
      }
  }

  // <AG> zwolnić pamięć dla P i tr
  for (list<TNode *>::iterator it = P.begin(); it != P.end(); ++it)
    delete *it;

  for (list<TTriangle *>::iterator it = triangles.begin();
       it != triangles.end(); ++it)
    delete *it;

  return g;
}
