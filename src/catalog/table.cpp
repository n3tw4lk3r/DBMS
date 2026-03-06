#include "catalog/table.hpp"

namespace dbms {

Table::Table(const std::string& name,
             const std::vector<ColumnSchema>& schema) :
    name(name),
    schema(schema)
{}

const std::string Table::getName() const {
    return name;
}

void Table::insertRow(const std::vector<Value>& row) {
    rows.push_back(row);
}

const std::vector<std::vector<Value>>& Table::getRows() const {
    return rows;
}

std::vector<std::vector<Value>>& Table::getRowsMutable() {
    return rows;
}

const std::vector<ColumnSchema>& Table::getSchema() const {
    return schema;
}

} // namespace dbms
