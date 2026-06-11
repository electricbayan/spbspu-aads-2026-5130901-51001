#ifndef COMMAND_MANAGER
#define COMMAND_MANAGER
#include <iostream>
#include "graph.hpp"

namespace volkovich {
  class CommandManager {
    void handleGraphs(std::istream&, std::ostream&);
    void handleVertexes(std::istream&, std::ostream&);
    void handleOutbound(std::istream&, std::ostream&);
    void handleInbound(std::istream&, std::ostream&);
    void handleBind(std::istream&, std::ostream&);
    void handleCut(std::istream&, std::ostream&);
    void handleCreate(std::istream&, std::ostream&);
    void handleMerge(std::istream&, std::ostream&);
    void handleExtract(std::istream&, std::ostream&);

    public:
      CommandManager();
      void readCommand(const std::string&, std::istream&, std::ostream&);
  };
}
#endif
