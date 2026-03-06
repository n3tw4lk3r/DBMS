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
    Command parseUse(const std::vector<std::string>& tokens);
    Command parseCreateTable(const std::vector<std::string>& tokens);
};

} // namespace dbms
