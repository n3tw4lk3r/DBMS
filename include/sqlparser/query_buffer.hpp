#pragma once

#include <string>
#include <vector>

namespace dbms {

class QueryBuffer {
public:
    std::vector<std::string> append(const std::string& line);

private:
    std::string buffer;
};

} // namespace dbms
