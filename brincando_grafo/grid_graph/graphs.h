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

    void dijkstraGambiarra(Graph& graph, EdgeMapInt& edgeWeights, Node start, Node target);
    std::string A_STAR(Graph graph, EdgeMapInt& edgeWeights, Node start, Node target, EdgeMapInt& capacity);
    double heuristic(Node current, Node target, Graph graph, double minCost);
    void open_digraph_definition(std::string fileName, std::string digraphName);
    void draw_graph(Graph& gr, std::string fileName, EdgeMapInt& weights, EdgeMapInt& capacity);
    void drawPath(std::string path, std::string fileName);
    void close_graph_definition(std::string fileName);
private:
};

#endif