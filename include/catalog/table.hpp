#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"
#include "storage/row.hpp"

namespace dbms {

class Table {
public:
    Table(
        const std::string& name,
        const std::vector<ColumnSchema>& schema
    );

    const std::string getName() const;

    RowId insertRow(const std::vector<Value>& values);

    const std::vector<Row>& getRows() const;
    std::vector<Row>& getRowsMutable();

    const std::vector<ColumnSchema>& getSchema() const;

private:
    RowId next_row_id = 1;

    std::string name;

    std::vector<ColumnSchema> schema;
    std::vector<Row> rows;
};

} // namespace dbms
