#include "sqlparser/parser.hpp"

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

std::vector<std::string> Parser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_string = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];

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

        if (ch == '(' || ch == ')' || ch == ',' || ch == ';') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }

            tokens.emplace_back(1, ch);
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
        return parseCreateTable(tokens);
    }

    return cmd;
}

Command Parser::parseCreateTable(const std::vector<std::string>& tokens) {
    Command cmd;

    cmd.type = CommandType::kCreateTable;

    if (tokens.size() < 4) {
        return cmd;
    }

    cmd.table_name = tokens[2];
    size_t pos = 3;

    if (tokens[pos] != "(") {
        return cmd;
    }

    ++pos;

    while (pos < tokens.size()) {
        if (tokens[pos] == ")") {
            break;
        }

        ColumnSchema column;
        column.name = tokens[pos];
        ++pos;
        std::string type = tokens[pos];
        ++pos;

        if (type == "INT") {
            column.type = ColumnType::kInt;
        } else if (type == "STRING") {
            column.type = ColumnType::kString;
        }

        while (pos < tokens.size()) {
            if (tokens[pos] == "," || tokens[pos] == ")") {
                break;
            }

            if (tokens[pos] == "NOT_NULL") {
                column.not_null = true;
            }

            if (tokens[pos] == "INDEXED") {
                column.indexed = true;
                column.not_null = true;
            }

            ++pos;
        }

        cmd.columns.push_back(column);

        if (tokens[pos] == ",") {
            ++pos;
        }
    }

    return cmd;
}

} // namespace dbms
