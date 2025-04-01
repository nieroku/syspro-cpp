#include <iostream>
#include <string>

#include "parser.h"

int main() {
  while (std::cin) {
    std::string expr;
    for (std::string line; std::getline(std::cin, line) && !line.empty();)
      expr += line;
    if (expr.empty())
      continue;
    try {
      std::cout << (std::string)*parse(expr)->eval() << std::endl;
    } catch (const char* exc) {
      std::cout << exc << std::endl;
    }
  }
}
