#include "command-manager.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace volkovich {
  void CommandManager::handleGraphs(std::istream&, std::ostream& output) {
    std::vector< std::string > names;
    for (auto it = gr.begin(); it != gr.end(); ++it) {
      names.push_back(it->key);
    }
    if (names.empty()) {
      output<<'\n';
      return;
    }
    std::sort(names.begin(), names.end());
    for (const auto& name : names) {
      output << name << '\n';
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
    std::vector< std::string > vertexes;
    for (auto it = graph->graph_.begin(); it != graph->graph_.end(); ++it) {
      vertexes.push_back(it->key);
    }
    if (vertexes.empty()) {
      output<<'\n';
      return;
    }
    std::sort(vertexes.begin(), vertexes.end());
    for (const auto& vertex : vertexes) {
      output << vertex << '\n';
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
    struct OutEdge {
      std::string to;
      std::vector< int > weights;
    };
    std::vector< OutEdge > edges;
    for (size_t i = 0; i < vertex->count; i++) {
      OutEdge edge;
      edge.to = vertex->edges[i].to;
      for (size_t j = 0; j < vertex->edges[i].weight_size; ++j) {
        edge.weights.push_back(vertex->edges[i].weight[j]);
      }
      edges.push_back(std::move(edge));
    }
    std::sort(edges.begin(), edges.end(), [](const OutEdge& lhs, const OutEdge& rhs) {
      return lhs.to < rhs.to;
    });
    if (edges.empty()) {
      output << '\n';
      return;
    }
    for (const auto& edge : edges) {
      output << edge.to;
      for (int weight : edge.weights) {
        output << ' ' << weight;
      }
      output << '\n';
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
    auto vertex = graph->graph_.find(vertex_name);
    if (!vertex) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    struct InEdge {
      std::string from;
      std::vector< int > weights;
    };
    std::vector< InEdge > edges;
    for (auto it = graph->graph_.begin(); it != graph->graph_.end(); ++it) {
      for (size_t i = 0; i < it->value.count; i++) {
        if (it->value.edges[i].to == vertex_name) {
          InEdge edge;
          edge.from = it->key;
          for (size_t j = 0; j < it->value.edges[i].weight_size; ++j) {
            edge.weights.push_back(it->value.edges[i].weight[j]);
          }
          edges.push_back(std::move(edge));
        }
      }
    }
    std::sort(edges.begin(), edges.end(), [](const InEdge& lhs, const InEdge& rhs) {
      return lhs.from < rhs.from;
    });
    if (edges.empty()) {
      output << '\n';
      return;
    }
    for (const auto& edge : edges) {
      output << edge.from;
      for (int weight : edge.weights) {
        output << ' ' << weight;
      }
      output << '\n';
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
    if (!graph->removeEdge(from_name, to_name, weight)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
  };
  void CommandManager::handleCreate(std::istream& input, std::ostream& output) {
    std::string graph_name;
    size_t vertex_count = 0;
    if (!(input >> graph_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    input >> vertex_count;
    auto graph = gr.graphs_.find(graph_name);
    if (graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto& new_graph = gr.addGraph(graph_name);
    for (size_t i = 0; i < vertex_count; i++) {
      std::string vertex_name;
      if (!(input >> vertex_name)) {
        output << "<INVALID COMMAND>\n";
        return;
      }
      new_graph.addVertex(vertex_name);
    }
  };
  void CommandManager::handleMerge(std::istream& input, std::ostream& output) {
    std::string new_graph_name, old_graph_name_first, old_graph_name_second;
    if (!(input >> new_graph_name >> old_graph_name_first >> old_graph_name_second)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto old_graph1 = gr.graphs_.find(old_graph_name_first);
    if (!old_graph1) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto old_graph2 = gr.graphs_.find(old_graph_name_second);
    if (!old_graph2) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (gr.graphs_.find(new_graph_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto& new_graph = gr.addGraph(new_graph_name);
    new_graph.mergeFrom(*old_graph1);
    new_graph.mergeFrom(*old_graph2);
  };
  void CommandManager::handleExtract(std::istream& input, std::ostream& output) {
    std::string new_graph_name, old_graph_name;
    size_t vertex_count = 0;
    if (!(input >> new_graph_name >> old_graph_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (!(input >> vertex_count)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    if (gr.graphs_.find(new_graph_name)) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    auto old_graph = gr.graphs_.find(old_graph_name);
    if (!old_graph) {
      output << "<INVALID COMMAND>\n";
      return;
    }
    std::vector< std::string > selected_vertexes;
    selected_vertexes.reserve(vertex_count);
    for (size_t i = 0; i < vertex_count; i++) {
      std::string vertex_name;
      if (!(input >> vertex_name)) {
        output << "<INVALID COMMAND>\n";
        return;
      }
      if (!old_graph->hasVertex(vertex_name)) {
        output << "<INVALID COMMAND>\n";
        return;
      }
      selected_vertexes.push_back(vertex_name);
    }
    auto& new_graph = gr.addGraph(new_graph_name);
    for (const auto& vertex_name : selected_vertexes) {
      new_graph.addVertex(vertex_name);
    }
    for (auto it = old_graph->graph_.begin(); it != old_graph->graph_.end(); ++it) {
      if (std::find(selected_vertexes.begin(), selected_vertexes.end(), it->key) ==
          selected_vertexes.end()) {
        continue;
      }
      for (size_t i = 0; i < it->value.count; ++i) {
        if (std::find(selected_vertexes.begin(), selected_vertexes.end(), it->value.edges[i].to) ==
            selected_vertexes.end()) {
          continue;
        }
        for (size_t j = 0; j < it->value.edges[i].weight_size; ++j) {
          new_graph.addEdge(it->key, it->value.edges[i].to, it->value.edges[i].weight[j]);
        }
      }
    }
  };

  CommandManager::CommandManager(CommandManager::GraphTable& gr) : gr(gr) {
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
