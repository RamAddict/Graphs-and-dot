#define CATCH_CONFIG_MAIN
#include "graphs.h"
#include <lemon/grid_graph.h>
#include <queue>
#include <limits>
#include <iostream>
#include <fstream>
#include <map>
#include <set>

using lemon::INVALID;
using Graph         = lemon::GridGraph;
using Node          = Graph::Node;
using Edge          = Graph::Edge;
using EdgeMapInt    = Graph::EdgeMap<int>;
using NodeIt        = Graph::NodeIt;
using EdgeIt        = Graph::EdgeIt;
using ArcIt         = Graph::ArcIt;
using IncEdgeIt     = Graph::IncEdgeIt;
using Arc           = Graph::Arc;
using NodeMapInt    = Graph::NodeMap<int>;
struct Node_compare {
    using Node          = lemon::GridGraph::Node; //!!!
       bool operator() (std::pair<Node, int> a , std::pair<Node, int> b) {
          return a.second < b.second;
        }
          //TODO esse cara tem q comparar o fScore e nao g
    };
TEST_CASE("gridGraph") {
    SECTION("creating a grid graph") {
        Graph grid(20, 20);
        EdgeMapInt grid_weight(grid);
        for (EdgeIt it(grid); it != INVALID; ++it) {
            grid_weight[it] =1;
            //std::cout << "edge: " << grid.id(it) << " with weight: " << grid_weight[it] << std::endl;
        }
        // std::set<std::pair<Node, int>, Node_compare> as;

        // as.insert(std::make_pair(grid.nodeFromId(0), 923));
        // for (auto it = as.begin(); it != as.end(); ++it) {
        //      std::cout << it->second << "->" << grid.id(it->first) << std::endl;
        // }  // menor fica no primeiro, tis settled


        auto gr = new  graphs{};
        //gr.dijkstraGambiarra(grid, grid_weight, grid.nodeFromId(0), grid.nodeFromId(grid.maxNodeId()));
        auto fileName = "graph_viz.gv";
        auto digraphName = "G";
        
        gr->open_digraph_definition(fileName, digraphName);
        gr->draw_graph(grid, fileName, grid_weight, false);
        std::string path = gr->A_STAR(grid, grid_weight, grid(0,0), grid.nodeFromId(grid.maxNodeId()));
        gr->drawPath(path, fileName);
        gr->close_graph_definition(fileName);
        free(gr);
    }
}