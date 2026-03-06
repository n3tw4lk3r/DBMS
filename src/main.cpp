#include <iostream>

#include "catalog/system.hpp"

int main() {
    using namespace dbms;

    System system;

    std::string line;
    bool should_exit = false;

    std::cout << "Enter 'exit' to exit." << std::endl;

    while (!should_exit) {

        if (!std::getline(std::cin, line)) {
            should_exit = true;
            break;
        }

        if (line == "exit") {
            should_exit = true;
            break;
        }

        std::cout << "Query: " << line << std::endl;
    }

}
