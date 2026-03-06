#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"
#include "common/value.hpp"

namespace dbms {

class Table {
public:
    Table(const std::string& name,
          const std::vector<ColumnSchema>& schema);
    const std::string getName() const;
    void insertRow(const std::vector<Value>& row);
    const std::vector<std::vector<Value>> getRows() const;

private:
    std::string name;
    std::vector<ColumnSchema> schema;
    std::vector<std::vector<Value>> rows;
};

} // namespace dbms
