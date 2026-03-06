#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"
#include "common/value.hpp"

namespace dbms {

enum class CommandType {
    kCreateDatabase,
    kDropDatabase,
    kUseDatabase,
    kCreateTable,
    kDropTable,
    kInsert,
    kSelect,
    kUnknown
};

struct Command {
    CommandType type = CommandType::kUnknown;

    std::string database_name;
    std::string table_name;

    std::vector<ColumnSchema> columns;

    std::vector<std::string> column_names;
    std::vector<Value> values;
};

} // namespace dbms
