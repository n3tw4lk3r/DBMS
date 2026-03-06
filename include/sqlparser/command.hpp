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
    kUpdate,
    kDelete,
    kUnknown
};

struct Assignment {
    std::string column;
    Value value;
};

struct Condition {
    std::string left;
    std::string op;
    Value right;
};

struct SelectColumn {
    std::string name;
    std::string alias;
};

struct Command {
    CommandType type = CommandType::kUnknown;

    std::string database_name;
    std::string table_name;

    std::vector<ColumnSchema> columns;
    std::vector<std::string> column_names;

    std::vector<std::vector<Value>> values;

    std::vector<Assignment> assignments;
    std::vector<SelectColumn> select_columns;
    std::vector<Condition> conditions;
};

} // namespace dbms
