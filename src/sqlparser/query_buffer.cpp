#include "sqlparser/query_buffer.hpp"

namespace dbms {

std::vector<std::string> QueryBuffer::append(const std::string& line) {
    buffer += line;
    buffer += '\n';

    std::vector<std::string> queries;
    size_t pos = 0;

    bool should_exit = false;
    while (!should_exit) {
        size_t semicolon = buffer.find(';', pos);

        if (semicolon == std::string::npos) {
            should_exit = true;
            break;
        }

        queries.push_back(buffer.substr(pos, semicolon - pos));
        pos = semicolon + 1;
    }

    buffer = buffer.substr(pos);
    return queries;
}

} // namespace dbms
