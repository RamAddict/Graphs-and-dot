#ifndef graphs_h
#define graphs_h
#include "catch.hpp"
//#include <lemon/list_graph.h>
#include <lemon/grid_graph.h>
#include <queue>
#include <set>
#include <limits>
#include <iostream>
#include <map>
using lemon::INVALID;


class graphs {
public:
using Graph            = lemon::GridGraph;
using Node             = Graph::Node;
using Edge             = Graph::Edge;
using EdgeMapInt       = Graph::EdgeMap<int>;
using NodeIt           = Graph::NodeIt;
using EdgeIt           = Graph::EdgeIt;
using ArcIt            = Graph::ArcIt;
using IncEdgeIt        = Graph::IncEdgeIt;
using Arc              = Graph::Arc;
using NodeMapInt       = Graph::NodeMap<int>;
using NodeMapNode      = Graph::NodeMap<Node>;
using OutArcIt         = Graph::OutArcIt;

    //! A version of dijkstra
    void dijkstraGambiarra(Graph& graph, EdgeMapInt& edgeWeights, Node start, Node target);
    
    //! A version of A_star algorythm, decrements the capacity of the nodes that have been routed. Returns a path to the target in the form (A->B->C)
    std::string A_STAR(Graph graph, EdgeMapInt& edgeWeights, Node start, Node target, EdgeMapInt& capacity);
    
    //! Returns the modulo of the manhattan distance between 2 nodes
    double manhattan_heuristic(Node current, Node target, Graph graph, double minCost);
    
    //! Opens the graph with the given name
    void open_digraph_definition(std::string fileName, std::string digraphName);
    
    //! Prints the graph with edge weights also opens the file in append mode
    void draw_graph(Graph& gr, std::string fileName, EdgeMapInt& weights, EdgeMapInt& capacity);
    
    //! Opens the file at the end and inserts the path received already in the apropiate form (A->B) by parameter 
    void drawPath(std::string path, std::string fileName, std::string color);
    
    //! Appends at the end of the file "\n}"
    void close_graph_definition(std::string fileName);
private:
};

#endif