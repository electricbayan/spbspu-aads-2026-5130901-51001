#include "command-manager.hpp"

namespace volkovich {
  class CommandManager {
    void handleGraphs(std::istream&, std::ostream&) {};
    void handleVertexes(std::istream& input, std::ostream& output) {};
    void handleOutbound(std::istream& input, std::ostream& output) {};
    void handleInbound(std::istream& input, std::ostream& output) {};
    void handleBind(std::istream& input, std::ostream& output) {};
    void handleCut(std::istream& input, std::ostream& output) {};
    void handleCreate(std::istream& input, std::ostream& output) {};
    void handleMerge(std::istream& input, std::ostream& output) {};
    void handleExtract(std::istream& input, std::ostream& output) {};

    using Handler = void (CommandManager::*)(std::istream&, std::ostream&);
    HashTable<std::string, Handler, SipHash, std::equal_to<std::string>> commands_;
   public:
    CommandManager() {
      commands_.add("graphs", handleGraphs);
      commands_.add("vertexes", handleVertexes);
      commands_.add("outbound", handleOutbound);
      commands_.add("inbound", handleInbound);
      commands_.add("bind", handleBind);
      commands_.add("cut", handleCut);
      commands_.add("create", handleCreate);
      commands_.add("merge", handleMerge);
      commands_.add("extract", handleExtract);
    }
    void readCommand(const std::string& command, std::istream& input, std::ostream& output) {
      if (command.empty()) {
        return;
      }

    };
  };
}
