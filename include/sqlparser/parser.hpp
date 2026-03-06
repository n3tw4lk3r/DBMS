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

    Command parseCreate(const std::vector<std::string>& tokens);
    Command parseUse(const std::vector<std::string>& tokens);
};

} // namespace dbms
