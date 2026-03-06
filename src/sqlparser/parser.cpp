#include "sqlparser/parser.hpp"

#include <sstream>

namespace dbms {

std::string Parser::normalize(const std::string& token) {
    std::string result;

    for (char ch : token) {
        if (ch == '(' || ch == ')' || ch == ',' || ch == ';') {
            continue;
        }

        result += ch;
    }

    return result;
}

std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_string = false;

    for (char ch : input) {
        if (ch == '"') {
            if (in_string) {
                tokens.push_back(current);
                current.clear();
                in_string = false;
            } else {
                in_string = true;
            }
            continue;
        }

        if (in_string) {
            current += ch;
            continue;
        }

        if (std::isspace(ch)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }

        if (ch == ';') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }

            tokens.push_back(";");
            continue;
        }

        current += ch;
    }

    if (!current.empty()) {
        tokens.push_back(current);
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
