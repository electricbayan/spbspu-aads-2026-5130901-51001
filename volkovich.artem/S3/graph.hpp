#ifndef GRAPH
#define GRAPH
#include "../common/htable.hpp"

namespace volkovich {
  template < class Vertex, class Hash, class Equal >
  class Graph {
    struct Edge {
      Vertex to;
      int weight;
    };
    volkovich::HashTable< Vertex, Edge*, Hash, Equal > graph_;

   public:
    Graph();
    ~Graph();
    Graph(const Graph& other);
    Graph(Graph&& other);
    Graph& operator=(const Graph& other);
    Graph& operator=(Graph&& other);
    void swap(Graph& other);

    bool addVertex(const Vertex& v) {
      return graph_.add(v, {});
    };

    void addEdge(const Vertex& from, const Vertex& to) {
      Edge* edges_from = graph_.find(from);
    };

    void removeEdge() {};

    bool hasVertex() {
      return true;
    };
  };
}
#endif
