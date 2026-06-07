#include <boost/hash2/hash_append.hpp>
#include <fstream>

#include "command-manager.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Wrong arg number";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Wrong file";
    return 1;
  }

  volkovich::CommandManager cm;
  std::string command;
  while (std::cin >> command) {
    try {
      // cm.readCommand(command);
    } catch (std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  return 0;
}
