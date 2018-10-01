#include "graphs.h"
#include <fstream>
class Compare {
    using Node          = lemon::GridGraph::Node; //!!!
      public:
       bool operator() (std::pair<Node, int>& a , std::pair<Node, int>& b) {
          return a.second > b.second;
          //TODO esse cara tem q comparar o fScore e nao g
        }
    };
void graphs::dijkstraGambiarra(Graph& graph, EdgeMapInt& edgeWeights, Node start, Node target){
    Graph::NodeMap<bool> is_visited(graph, false); //openSet at times when necessary
    Graph::NodeMap<Node> parents(graph, INVALID);
    Graph::NodeMap<int> nodeCost(graph, std::numeric_limits<int>::max());
    std::priority_queue<std::pair<Node&, int&>, std::vector<std::pair<Node, int>>, Compare> priority;
    Node bestNode = target; //!problams 
    is_visited[start] = true;
    parents[start] = start;
    nodeCost[start] = 0;
    auto current_node = start;
    const double MINIMUM_COST = 1; 
    
 while(current_node != target)
  {
    for (IncEdgeIt it(graph, current_node); it != INVALID; ++it)
    {
      Node oppositeNode(graph.oppositeNode(current_node, it)); //opposite nodes of current Node
      
      if (is_visited[oppositeNode] == false) {
        
        //auto h = heuristic(oppositeNode, target, graph, MINIMUM_COST);

        if(nodeCost[oppositeNode] > (nodeCost[current_node] + edgeWeights[it] ) )
        {//if the cost of the node is bigger than the current path then overwrite cost
          nodeCost[oppositeNode] = nodeCost[current_node] + edgeWeights[it] ;
          parents[oppositeNode] = current_node;
          priority.push(std::make_pair(oppositeNode, nodeCost[oppositeNode]));
        }
      }
    }

  //percorrer todos os nodes e achar o melhor
  //(menor custo entre eles , inclusive de iterações anteriores) e colocar em current_node ou seja o proximo a ser iterado
  is_visited[current_node] = true; // falta tirar esse opposite node da lista ? mas n precisa porq vou marcar como visitado true?
  
  //std::cout << "next node will be: " << graph.id(priority.top().first) << std::endl;
  current_node = priority.top().first;
  priority.pop();
  //std::cout << "next!" << std::endl;
  }
  std::cout <<"Found target: " << graph.id(current_node) << " traversing to it has a minimum cost of: " << nodeCost[current_node] << std::endl;
    while(current_node != start)
  {
    std::cout << graph.id(current_node) << "<-";
    current_node = parents[current_node];
  }


} // end of function

std::string graphs::A_STAR(Graph graph, EdgeMapInt& edgeWeights, Node start, Node target){
    Graph::NodeMap<bool> is_visited(graph, false);  // openSetAuxiliar, pra n ter de perocrrer o priority queue
    Graph::NodeMap<Node> parents(graph, INVALID);  // para reconstruir o caminho de volta
    Graph::NodeMap<int> nodeCostG(graph, std::numeric_limits<int>::max());  // gScore, valor de custo sem heuristica
    Graph::NodeMap<int> nodeCostF(graph, std::numeric_limits<int>::max());  // fScore = gscore +heuristica
    std::set<Node> closedSet;  // nodes alredy evaluated
    std::priority_queue<std::pair<Node&, int&>, std::vector<std::pair<Node, int>>, Compare> openSet;  // nodes discovered
    std::string path;

    std::map<int, Node> o; // primeiro smepre maior

    nodeCostG[start] = 0;  // custo de start é zero
    const double MINIMUM_COST = .5;  // minimum cost é o menor valor entre os edges do grafo
    // first node cost completely heuristic
    nodeCostF[start] = heuristic(start, target, graph, MINIMUM_COST);
    openSet.push(std::make_pair(start, nodeCostF[start]));  // adiciono start na lista de nodos a analisar
    auto current_node = start;  // primeiro node a analisar é o start
    
    while(current_node != target)
    {  // posso colocar um Priority_queue ou um set, sorted deixa O(1), mas permite repeticao. Set deixa sem repeticao mas fica O(n)
      current_node = openSet.top().first;
      openSet.pop();
      is_visited[current_node] = true;
      closedSet.insert(current_node);

      for (IncEdgeIt it(graph, current_node); it != INVALID; ++it)
      {
        Node oppositeNode(graph.oppositeNode(current_node, it));
        
        if (closedSet.find(oppositeNode) != closedSet.end()) {
          continue;
        }
        auto tentative = nodeCostG[current_node] + edgeWeights[it];

        if (is_visited[oppositeNode] == false) {
        is_visited[oppositeNode] = true;
        nodeCostG[oppositeNode] = tentative;
        nodeCostF[oppositeNode] = nodeCostG[oppositeNode] + heuristic(oppositeNode, target, graph, MINIMUM_COST);
        parents[oppositeNode] = current_node;
        openSet.push(std::make_pair(oppositeNode, nodeCostF[oppositeNode])); //TODO adaptar pra conter o fscore
        
        } else if(tentative >= nodeCostG[oppositeNode] ) {
            continue;
        }
      }
      //std::cout << "visited: " << graph.id(current_node) << std::endl;
    }
    std::cout <<"Found target: " << graph.id(current_node) << " traversing to it has a minimum cost of: " << nodeCostG[current_node] << std::endl;
    while(current_node != start)
      {
        // std::cout << graph.id(current_node) << "<-";
        path += std::to_string(graph.id(current_node)) + "->";
        current_node = parents[current_node];
      }
      path += std::to_string(graph.id(start));
      return path;
} // end of function

double graphs::heuristic(Node current, Node target, Graph graph, double minCost) {
    auto dx = abs(graph.pos(current).x - graph.pos(target).x);
    auto dy = abs(graph.pos(current).y - graph.pos(target).y);
    return minCost * (dx + dy);
}

void graphs::open_digraph_definition(std::string fileName, std::string digraphName) {
  std::ofstream file;
  file.open(fileName);
  file << "digraph " << digraphName << " {\n";
  file.close();
}

void graphs::close_graph_definition(std::string fileName) {
  std::ofstream file;
  file.open(fileName, std::ios_base::app);
  file << "\n}";
  file.close();
}

void graphs::draw_graph(Graph& gr, std::string fileName, EdgeMapInt& weights, bool with_arrow_head) {
    
    std::ofstream file;
    file.open(fileName, std::ios_base::app);
    Node current_node = gr.nodeFromId(0);  // primeiro
    if (with_arrow_head == false) {

      for (NodeIt current_node(gr); current_node != INVALID; ++ current_node) 
      {
        for (IncEdgeIt it(gr, current_node); it != INVALID; ++it) {
          
        Node oppositeNode(gr.oppositeNode(current_node, it));
        //std::cout << gr.id(current_node) << "->" << gr.id(oppositeNode) << " " << std::flush;
        file << gr.id(current_node) << "->"
        << gr.id(oppositeNode) << 
        " [arrowhead = \"none\" label=\"" << weights[it] << "\"];\n";
        std::fflush;
        }
      }
    } else {
      for (NodeIt current_node(gr); current_node != INVALID; ++ current_node) 
      {
        for (IncEdgeIt it(gr, current_node); it != INVALID; ++it) {
          
        Node oppositeNode(gr.oppositeNode(current_node, it));
        //std::cout << gr.id(current_node) << "->" << gr.id(oppositeNode) << " " << std::flush;
        file << gr.id(current_node) << "->"
        << gr.id(oppositeNode) << 
        " [label=\"" << weights[it] << "\"];\n";
        std::fflush;
        }
      }
    }
    // grid graphs que sejam nxn !!!
    //auto n_squared = gr.maxNodeId() + 1;
    //int n = std::sqrt(n_squared);
    //std::cout << n << std::endl;
    
    auto colunas = gr.height();
    auto linhas = gr.width();

    std::string line[(int)linhas];

    for (NodeIt it(gr); it != INVALID; ++it) {
      int a = ((gr.id(it))%linhas);
      line[a] += std::to_string(gr.id(it)) + " ";
    }
     
    for (int i = 0; i != linhas; i++) {
      file << "{rank = same; " << line[i] + "" << std::flush;
      file << "}\n";
      std::fflush;
    }
    file << "\nsize = \"35,35\"\n";
    std::fflush;
    file.close();
}
void graphs::drawPath(std::string path, std::string fileName) {
  std::ofstream file;
  
  file.open(fileName, std::ios_base::app);
  file << "\n" << path << "[color = \"red\"]";
  file.flush();
  file.close();
}