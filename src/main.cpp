#include <exception>
#include <iostream>
#include <string>

#include "catalog/system.hpp"
#include "execution/executor.hpp"
#include "sqlparser/parser.hpp"
#include "sqlparser/query_buffer.hpp"

int main() {
    using namespace dbms;

    System system;
    Parser parser;
    Executor executor(system);
    QueryBuffer buffer;

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

        auto queries = buffer.append(line);
        for (const auto& query : queries) {
            try {
                auto cmd = parser.parse(query);
                executor.execute(cmd);
            } catch (const std::exception& e) {
                std::cout << "An error occured: " << e.what() << '\n';
            }
        }
    }
}
