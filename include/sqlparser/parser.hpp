#pragma once

#include <string>
#include <vector>

#include "sqlparser/command.hpp"

namespace dbms {

class Parser {
public:
    Command parse(const std::string& query);

private:
    std::vector<std::string> tokenize(const std::string& query);
    std::string normalize(const std::string& token);

    Command parseCreate(const std::vector<std::string>& tokens);
    Command parseCreateTable(const std::vector<std::string>& tokens);

    Command parseUse(const std::vector<std::string>& tokens);
    Command parseDrop(const std::vector<std::string>& tokens);

    Command parseInsert(const std::vector<std::string>& tokens);
    Command parseSelect(const std::vector<std::string>& tokens);
    Command parseUpdate(const std::vector<std::string>& tokens);
    Command parseDelete(const std::vector<std::string>& tokens);

    Value parseValue(const std::string& token);
};

} // namespace dbms
