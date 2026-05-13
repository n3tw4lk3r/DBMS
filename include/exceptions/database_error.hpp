#pragma once

#include <stdexcept>
#include <string>

namespace dbms {

class DatabaseError : public std::runtime_error {
public:
    explicit DatabaseError(const std::string& message) :
        std::runtime_error(message)
    {}
};

} // namespace dbms

