#pragma once

#include <string>

#include "common/value.hpp"

namespace dbms {

class ValueComparator {
public:
    static bool compare(
        const Value& left,
        const Value& right,
        const std::string& operator_str
    );

    static bool between(
        const Value& value,
        const Value& left,
        const Value& right
    );
};

} // namespace dbms

