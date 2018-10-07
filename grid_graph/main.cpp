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
          return a.second > b.second;
        }
          //o top do queue vai ser o menor quando a.second > b.second
          //i am convinced now
    };
TEST_CASE("gridGraph") {
    SECTION("creating a grid graph") {
        Graph grid(10, 8);
        EdgeMapInt grid_weight(grid);
        EdgeMapInt capacity(grid);
        int i = 1;
        //std::priority_queue<std::pair<Node&, int&>, std::vector<std::pair<Node, int>>, Node_compare> openSet;
        for (EdgeIt it(grid); it != INVALID; ++it) {
            grid_weight[it] = 1;
            capacity[it] = 1;
            std::cout << "edge: " << grid.id(it) << " with weight: " << grid_weight[it] << std::endl;
            i++;
        }
        // openSet.push(std::make_pair(grid.nodeFromId(0), 20));
        // std::cout << openSet.top().second << std::endl;
        // openSet.push(std::make_pair(grid.nodeFromId(2),10));
        // std::cout << openSet.top().second << std::endl;
        // openSet.push(std::make_pair(grid.nodeFromId(2),324));
        // std::cout << openSet.top().second << std::endl;
        // grid_weight[grid.edgeFromId(27)] = 200;
            // capacity[grid.edgeFromId(3)] = 0;
            // capacity[grid.edgeFromId(8)] = 0;
            // capacity[grid.edgeFromId(4)] = 0;
            // capacity[grid.edgeFromId(8)] = 0;
            // capacity[grid.edgeFromId(9)] = 0;
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
        std::string path = gr->A_STAR(grid, grid_weight, grid(0,0), grid.nodeFromId(grid.maxNodeId()), capacity);
        gr->drawPath(path, fileName, "red");
        path = gr->A_STAR(grid, grid_weight, grid(0,0), grid(4,2), capacity);
        gr->drawPath(path, fileName, "green");
        path = gr->A_STAR(grid, grid_weight, grid(0,1), grid(4,2), capacity);
        gr->drawPath(path, fileName, "blue");
        path = gr->A_STAR(grid, grid_weight, grid(2,0), grid.nodeFromId(grid.maxNodeId()), capacity);
        gr->drawPath(path, fileName, "yellow");
        // draw graph must be invoked before all the drawPaths
        gr->draw_graph(grid, fileName, grid_weight, capacity);
        gr->close_graph_definition(fileName);
        free(gr);
    }
}