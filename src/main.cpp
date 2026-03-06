#include <iostream>

#include "catalog/system.hpp"
#include "sqlparser/parser.hpp"
#include "sqlparser/query_buffer.hpp"

using namespace dbms;

static void execute(System& system, const Command& cmd) {
    switch (cmd.type) {

    case CommandType::kCreateDatabase:
        system.createDatabase(cmd.database_name);
        std::cout << "Database " << cmd.database_name << " created\n";
        break;

    case CommandType::kUseDatabase:
        system.useDatabase(cmd.database_name);
        std::cout << "Using database " << cmd.database_name << '\n';
        break;

    default:
        std::cout << "Unknown command\n";
        break;
    }
}

int main() {
    System system;
    Parser parser;
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
            auto cmd = parser.parse(query);
            execute(system, cmd);
        }
    }
}
