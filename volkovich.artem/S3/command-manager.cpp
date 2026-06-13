#include "command-manager.hpp"

namespace volkovich {
  void CommandManager::handleGraphs(std::istream&, std::ostream& output) {
    for (auto it = gr.begin(); it != gr.end(); ++it) {
      output << it->key << '\n';
    }
  };
  void CommandManager::handleVertexes(std::istream& input, std::ostream& output) {
    std::string name;
    if (!(input >> name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto graph = gr.graphs_.find(name);
    if (!graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    for (auto it = graph->graph_.begin(); it != graph->graph_.end(); ++it) {
      output << it->key << '\n';
    }
  };
  void CommandManager::handleOutbound(std::istream& input, std::ostream& output) {
    std::string graph_name, vertex_name;
    if (!(input >> graph_name >> vertex_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto graph = gr.graphs_.find(graph_name);
    if (!graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto vertex = graph->graph_.find(vertex_name);
    if (!vertex) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    for (size_t i = 0; i < vertex->count; i++) {
      output << vertex->edges[i].to << ' ' << vertex->edges[i].weight << '\n';
    }
  };
  void CommandManager::handleInbound(std::istream& input, std::ostream& output) {
    std::string graph_name, vertex_name;
    if (!(input >> graph_name >> vertex_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto graph = gr.graphs_.find(graph_name);
    if (!graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    for (auto it = graph->graph_.begin(); it != graph->graph_.end(); ++it) {
      for (size_t i = 0; i < it->value.count; i++) {
        if (it->value.edges[i].to == vertex_name) {
          output << it->key << ' ' << it->value.edges[i].weight << '\n';
        }
      }
    }
  };
  void CommandManager::handleBind(std::istream& input, std::ostream& output) {
    std::string graph_name, from_name, to_name;
    int weight;
    if (!(input >> graph_name >> from_name >> to_name >> weight)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto graph = gr.graphs_.find(graph_name);
    if (!graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (!graph->hasVertex(from_name)) {
      graph->addVertex(from_name);
    }
    if (!graph->hasVertex(to_name)) {
      graph->addVertex(to_name);
    }
    graph->addEdge(from_name, to_name, weight);
  };
  void CommandManager::handleCut(std::istream& input, std::ostream& output) {
    std::string graph_name, from_name, to_name;
    int weight;
    if (!(input >> graph_name >> from_name >> to_name >> weight)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto graph = gr.graphs_.find(graph_name);
    if (!graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (!graph->hasVertex(from_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (!graph->hasVertex(to_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (!graph->hasEdge(from_name, to_name, weight)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    graph->removeEdge(from_name, to_name);
  };
  void CommandManager::handleCreate(std::istream& input, std::ostream& output) {

  };
  void CommandManager::handleMerge(std::istream& input, std::ostream& output) {};
  void CommandManager::handleExtract(std::istream& input, std::ostream& output) {};

  CommandManager::CommandManager(CommandManager::GraphTable gr) : gr(gr) {
    commands_.add("graphs", &CommandManager::handleGraphs);
    commands_.add("vertexes", &CommandManager::handleVertexes);
    commands_.add("outbound", &CommandManager::handleOutbound);
    commands_.add("inbound", &CommandManager::handleInbound);
    commands_.add("bind", &CommandManager::handleBind);
    commands_.add("cut", &CommandManager::handleCut);
    commands_.add("create", &CommandManager::handleCreate);
    commands_.add("merge", &CommandManager::handleMerge);
    commands_.add("extract", &CommandManager::handleExtract);
  }

  void CommandManager::readCommand(const std::string& command, std::ostream& output) {
    if (command.empty()) {
      return;
    }
    std::stringstream ss(command);
    std::string instruction;
    ss >> instruction;
    Handler* fn = commands_.find(instruction);
    if (!fn) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    (this->**fn)(ss, output);
  };

}
