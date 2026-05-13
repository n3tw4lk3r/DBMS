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

RowId Table::insertRow(const std::vector<Value>& values) {
    Row row;

    row.id = next_row_id;
    ++next_row_id;
    row.values = values;

    rows.push_back(row);

    return row.id;
}

const std::vector<Row>& Table::getRows() const {
    return rows;
}

std::vector<Row>& Table::getRowsMutable() {
    return rows;
}

const std::vector<ColumnSchema>& Table::getSchema() const {
    return schema;
}

} // namespace dbms

