#include <iostream>
#include <fstream>
#include <string>
#include "calc.hpp"

int main(int argc, char* argv[]) {
  if (argc > 2)  {
    std::cerr << "Wrong args number";
    return 2;
  }
  std::istream* input = &std::cin;
  if (argc == 2) {
    std::ifstream file;
    file.open(argv[1]);
    if (!file) { std::cerr<<"Cannot open file"; return 2;}
    input = &file;
  }

  std::string eval;
  while (std::getline(*input, eval)) {
    if (!eval.empty()){
      long long res = volkovich::calculate(eval);
    }
  }
}
