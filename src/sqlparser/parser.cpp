#include "sqlparser/parser.hpp"

#include <sstream>

namespace dbms {

std::vector<std::string> Parser::tokenize(const std::string& query) {
    std::vector<std::string> tokens;

    std::stringstream ss(query);
    std::string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

Command Parser::parse(const std::string& query) {
    auto tokens = tokenize(query);

    if (tokens.empty()) {
        return {};
    }

    if (tokens[0] == "CREATE") {
        return parseCreate(tokens);
    }

    if (tokens[0] == "USE") {
        return parseUse(tokens);
    }

    return {};
}

Command Parser::parseUse(const std::vector<std::string>& tokens) {
    Command cmd;

    cmd.type = CommandType::kUseDatabase;

    if (tokens.size() >= 2) {
        cmd.database_name = tokens[1];
    }

    return cmd;
}

Command Parser::parseCreate(const std::vector<std::string>& tokens) {
    Command cmd;

    if (tokens.size() < 3) {
        return cmd;
    }

    if (tokens[1] == "DATABASE") {
        cmd.type = CommandType::kCreateDatabase;
        cmd.database_name = tokens[2];
        return cmd;
    }

    if (tokens[1] == "TABLE") {
        cmd.type = CommandType::kCreateTable;
        cmd.table_name = tokens[2];
        return cmd;
    }

    return cmd;
}

} // namespace dbms
