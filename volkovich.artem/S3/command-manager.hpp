#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER
#include <iostream>
#include <sstream>

#include "graph.hpp"

namespace volkovich {
  class CommandManager {
    using Handler = void (CommandManager::*)(std::istream&, std::ostream&);
    HashTable< std::string, Handler, SipHash, std::equal_to< std::string > > commands_;

    void handleGraphs(std::istream&, std::ostream&);
    void handleVertexes(std::istream&, std::ostream&);
    void handleOutbound(std::istream&, std::ostream&);
    void handleInbound(std::istream&, std::ostream&);
    void handleBind(std::istream&, std::ostream&);
    void handleCut(std::istream&, std::ostream&);
    void handleCreate(std::istream&, std::ostream&);
    void handleMerge(std::istream&, std::ostream&);
    void handleExtract(std::istream&, std::ostream&);
    using GraphTable =
        volkovich::Graphs< std::string, volkovich::SipHash, std::equal_to< std::string > >;
    GraphTable gr;

   public:
    CommandManager(GraphTable gr);
    void readCommand(const std::string& command, std::ostream& output);
  };
}
#endif
