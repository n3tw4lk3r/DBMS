#pragma once

#include <vector>

#include "common/value.hpp"
#include "storage/row_id.hpp"

namespace dbms {

struct Row {
    RowId id = 0;
    std::vector<Value> values;
    bool deleted = false;
};

} // namespace dbms

