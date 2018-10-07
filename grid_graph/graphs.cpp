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

std::string graphs::A_STAR(Graph graph, EdgeMapInt& edgeWeights, Node start, Node target, EdgeMapInt& capacity){
    std::string path;
    const double MINIMUM_COST = 1;  // minimum cost é o menor valor entre os edges do grafo
    Node current_node;
    /////////////////////CLOSED SET
    std::set<Node> closedSet;  // nodes alredy evaluated
    
    /////////////////////OPEN SETS
    std::priority_queue<std::pair<Node&, int&>, std::vector<std::pair<Node, int>>, Compare> openSet;  // nodes discovered
    Graph::NodeMap<bool> is_visited(graph, false);  // openSetAuxiliar, pra n ter de perocrrer o priority queue
    //(std::map<int, Node> o; // primeiro smepre maior) 

    /////////////////////CAME FROM
    Graph::NodeMap<Node> parents(graph, INVALID);  // para reconstruir o caminho de volta
    
    /////////////////////G_MAP WITH INFINITY
    Graph::NodeMap<int> _GScore(graph, std::numeric_limits<int>::max());  // gScore, valor de custo sem heuristica
    
    /////////////////////F_MAP WITH INFINITY 
    Graph::NodeMap<int> _FScore(graph, std::numeric_limits<int>::max());  // fScore = gscore +heuristica
    
    /////////////////////COST FOR START NODE IS ZERO
    _GScore[start] = 0;

    /////////////////////FSCORE FOR START IS COMPLETELY HEURISTIC
    _FScore[start] = manhattan_heuristic(start, target, graph, MINIMUM_COST);
    
    //!!!!!
    openSet.push(std::make_pair(start, _FScore[start]));  // adiciono start na lista de nodos a analisar
    //!!!!!

    while(!openSet.empty())
    {  // posso colocar um Priority_queue ou um set, sorted deixa O(1), mas permite repeticao. Set deixa sem repeticao mas fica O(n)
      //~~~~~~~~~~~~~~ current = node com o custo mais baixo, o primieiro
      current_node = openSet.top().first;
      //~~~~~~~~~~~~~~
      if (current_node == target) {
        std::cout << "killme" << std::endl;
        break;
      }
      ///////////////REMOVE FROM OPENSETS
      openSet.pop();
      is_visited[current_node] = true;
      ///////////////ADD TO CLOSED SET
      closedSet.insert(current_node);


      for (OutArcIt it(graph, current_node); it != INVALID; ++it)
      {
         if (capacity[it] == 0) // test for capacity
          continue;
        /////////////////for each neighbour or current
        Node oppositeNode(graph.oppositeNode(current_node, it));
        
        if (closedSet.find(oppositeNode) != closedSet.end()) {
          continue;
        }
        /////////////////Tentative G_score
        auto tentative = _GScore[current_node] + edgeWeights[it];
        

        if (is_visited[oppositeNode] == false) {
        is_visited[oppositeNode] = true;
        } else if(tentative >= _GScore[oppositeNode] ) {
            continue;
        }
        _GScore[oppositeNode] = tentative;
        _FScore[oppositeNode] = _GScore[oppositeNode] + manhattan_heuristic(oppositeNode, target, graph, MINIMUM_COST);
        
        openSet.push(std::make_pair(oppositeNode, _FScore[oppositeNode])); //TODO adaptar pra conter o fscore
        parents[oppositeNode] = current_node;
      }
      std::cout << "visited: " << graph.id(current_node) << std::endl;
    }
    std::cout <<"Found target: " << graph.id(current_node) << " traversing to it has a minimum cost of: " << _GScore[current_node] << std::endl;
    while(current_node != start)
      {
        auto arc = graph.findArc(current_node, parents[current_node]);
        capacity[arc] += -1; // <<<< precisa de um edge, mas tenho nodes aos quais esses edges estao conectados
        path += std::to_string(graph.id(current_node)) + "->";
        current_node = parents[current_node];
      }
      path += std::to_string(graph.id(start));
      return path;
} // end of function

double graphs::manhattan_heuristic(Node current, Node target, Graph graph, double minCost) {
    auto dx = (graph.pos(current).x - graph.pos(target).x);
    auto dy = (graph.pos(current).y - graph.pos(target).y);
    return minCost * abs(dx + dy);
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

void graphs::draw_graph(Graph& gr, std::string fileName, EdgeMapInt& weights, EdgeMapInt& capacity ) {
    Graph::NodeMap<bool> is_visited(gr, false);
    std::ofstream file;
    file.open(fileName, std::ios_base::app);
    file << "\n";
    Node current_node = gr.nodeFromId(0);  // primeiro

      for (NodeIt current_node(gr); current_node != INVALID; ++ current_node) 
      {
        for (OutArcIt it(gr, current_node); it != INVALID; ++it) {
          
        Node oppositeNode(gr.oppositeNode(current_node, it));
        if (is_visited[oppositeNode] == false) {
          file << gr.id(current_node) << "->"
          << gr.id(oppositeNode) << 
          " [arrowhead = \"none\" label=\"" << weights[it] << ":" << capacity[it] << "\"];\n";
          std::fflush;
          is_visited[current_node] = true;
          }
        }
      }
    
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
void graphs::drawPath(std::string path, std::string fileName, std::string color) {
  std::ofstream file;
  
  file.open(fileName, std::ios_base::app);
  file << "\n" << path << "[color = \"" << color << "\" weight = 0 penwidth = 2.5]"; // the arcs created are only in the image not in the actual graph itself
  file.flush();
  file.close();
}