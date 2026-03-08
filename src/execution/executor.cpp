#include <algorithm>
#include <iostream>
#include <regex>

#include "execution/executor.hpp"

namespace dbms {

Executor::Executor(System& system) :
    system(system)
{}

void Executor::execute(const Command& cmd) {
    switch (cmd.type) {

    case CommandType::kCreateDatabase:
        executeCreateDatabase(cmd);
        break;

    case CommandType::kUseDatabase:
        executeUseDatabase(cmd);
        break;

    case CommandType::kCreateTable:
        executeCreateTable(cmd);
        break;

    case CommandType::kDropDatabase:
        executeDropDatabase(cmd);
        break;

    case CommandType::kDropTable:
        executeDropTable(cmd);
        break;

    case CommandType::kInsert:
        executeInsert(cmd);
        break;

    case CommandType::kSelect:
        executeSelect(cmd);
        break;

    case CommandType::kUpdate:
        executeUpdate(cmd);
        break;

    case CommandType::kDelete:
        executeDelete(cmd);
        break;

    default:
        std::cout << "Unknown command\n";
        break;
    }
}

void Executor::executeCreateDatabase(const Command& cmd) {
    system.createDatabase(cmd.database_name);
    std::cout << "Database " << cmd.database_name << " created\n";
}

void Executor::executeUseDatabase(const Command& cmd) {
    system.useDatabase(cmd.database_name);
    std::cout << "Using database " << cmd.database_name << "\n";
}

void Executor::executeDropDatabase(const Command& cmd) {
    // nothing interesting here so far
    std::cout << "Database dropped: " << cmd.database_name << "\n";
}

void Executor::executeDropTable(const Command& cmd) {
    // also nothing interesting here yet
    auto db = system.getCurrentDatabase();

    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    std::cout << "Table dropped: " << cmd.table_name << "\n";
}

void Executor::executeCreateTable(const Command& cmd) {
    auto db = system.getCurrentDatabase();

    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    db->createTable(cmd.table_name, cmd.columns);
    std::cout << "Table " << cmd.table_name << " created\n";
}

void Executor::executeInsert(const Command& cmd) {
    auto db = system.getCurrentDatabase();

    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    auto table = db->getTable(cmd.table_name);
    if (!table) {
        std::cout << "Table not found\n";
        return;
    }

    for (const auto& row : cmd.values) {
        table->insertRow(row);
    }
    std::cout << cmd.values.size() << " rows inserted\n";
}

void Executor::executeSelect(const Command& cmd) {
    auto db = system.getCurrentDatabase();

    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    auto table = db->getTable(cmd.table_name);

    if (!table) {
        std::cout << "Table not found\n";
        return;
    }

    const auto& rows = table->getRows();
    const auto& schema = table->getSchema();

    if (rows.empty()) {
        std::cout << "(empty)\n";
        return;
    }

    bool select_all = cmd.select_columns.size() == 1 &&
                      cmd.select_columns[0].name == "*";
    for (const auto& row : rows) {
        if (!matchConditions(cmd.conditions, row, schema)) {
            continue;
        }

        if (select_all) {
            for (size_t i = 0; i < row.size(); ++i) {
                const auto& v = row[i];
                if (v.getType() == Value::Type::kInt) {
                    std::cout << v.asInt();
                } else if (v.getType() == Value::Type::kString) {
                    std::cout << v.asString();
                } else {
                    std::cout << "NULL";
                }

                if (i + 1 != row.size()) {
                    std::cout << ", ";
                }
            }

        } else {
            for (size_t c = 0; c < cmd.select_columns.size(); ++c) {
                const auto& col = cmd.select_columns[c];
                int idx = -1;
                for (size_t i = 0; i < schema.size(); ++i) {
                    if (schema[i].name == col.name) {
                        idx = i;
                        break;
                    }
                }

                if (idx == -1) {
                    std::cout << "Unknown column\n";
                    return;
                }

                const auto& v = row[idx];
                if (v.getType() == Value::Type::kInt) {
                    std::cout << v.asInt();
                } else if (v.getType() == Value::Type::kString) {
                    std::cout << v.asString();
                } else {
                    std::cout << "NULL";
                }

                if (c + 1 != cmd.select_columns.size()) {
                    std::cout << ", ";
                }
            }
        }

        std::cout << "\n";
    }
}

void Executor::executeUpdate(const Command& cmd) {
    auto db = system.getCurrentDatabase();
    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    auto table = db->getTable(cmd.table_name);
    if (!table) {
        std::cout << "Table not found\n";
        return;
    }

    auto& rows = table->getRowsMutable();
    const auto& schema = table->getSchema();
    int updated = 0;
    for (auto& row : rows) {
        if (!matchConditions(cmd.conditions, row, schema)) {
            continue;
        }

        for (const auto& assignment : cmd.assignments) {
            for (size_t i = 0; i < schema.size(); ++i) {
                if (schema[i].name == assignment.column) {
                    row[i] = assignment.value;
                }
            }
        }

        ++updated;
    }

    std::cout << updated << " rows updated\n";
}

void Executor::executeDelete(const Command& cmd) {
    auto db = system.getCurrentDatabase();
    if (!db) {
        std::cout << "No database selected\n";
        return;
    }

    auto table = db->getTable(cmd.table_name);
    if (!table) {
        std::cout << "Table not found\n";
        return;
    }

    auto& rows = table->getRowsMutable();
    const auto& schema = table->getSchema();
    size_t before = rows.size();

    rows.erase(
        std::remove_if(
            rows.begin(),
            rows.end(),
            [&](const auto& row) {
                return matchConditions(cmd.conditions, row, schema);
            }),
        rows.end());

    std::cout << before - rows.size() << " rows deleted\n";
}

bool Executor::matchConditions(const std::vector<Condition>& conditions,
                               const std::vector<Value>& row,
                               const std::vector<ColumnSchema>& schema) {
    if (conditions.empty()) {
        return true;
    }

    for (const auto& condition : conditions) {
        Value left = resolveOperand(condition.lhs, row, schema);
        Value right = resolveOperand(condition.rhs, row, schema);

        if (condition.operator_type == "BETWEEN") {
            Value second = resolveOperand(condition.range_end, row, schema);
            if (!betweenValues(left, right, second)) {
                return false;
            }
            continue;
        }

        if (condition.operator_type == "LIKE") {
            if (!likeValues(left, right)) {
                return false;
            }
            continue;
        }

        if (!compareValues(left, right, condition.operator_type)) {
            return false;
        }
    }

    return true;
}


int Executor::findColumnIndex(const std::vector<ColumnSchema>& schema, const std::string& name) {
    for (size_t i = 0; i < schema.size(); ++i) {
            if (schema[i].name == name) {
                return static_cast<int>(i);
            }
        }
    return -1;
}

Value Executor::resolveOperand(const Operand& operand, const std::vector<Value>& row,
                               const std::vector<ColumnSchema> schema) {
    if (operand.is_column) {
        int idx = findColumnIndex(schema, operand.column);
        if (idx < 0 || idx >= static_cast<int>(row.size())) {
            return Value();
        }
        return row[idx];
    }
    return operand.value;
}

bool Executor::compareValues(const Value& a, const Value& b, const std::string& operator_str) {
    if (a.getType() != b.getType()) {
        return false;
    }

    if (a.getType() == Value::Type::kInt) {
        int x = a.asInt();
        int y = b.asInt();

        if (operator_str == "==") {
            return x == y;
        }
        if (operator_str == "!=") {
            return x != y;
        }
        if (operator_str == "<") {
            return x < y;
        }
        if (operator_str == ">") {
            return x > y;
        }
        if (operator_str == "<=") {
            return x <= y;
        }
        if (operator_str == ">=") {
            return x >= y;
        }
    } else if (a.getType() == Value::Type::kString) {
        const auto& x = a.asString();
        const auto& y = b.asString();

        if (operator_str == "==") {
            return x == y;
        }
        if (operator_str == "!=") {
            return x != y;
        }
        if (operator_str == "<") {
            return x < y;
        }
        if (operator_str == ">") {
            return x > y;
        }
        if (operator_str == "<=") {
            return x <= y;
        }
        if (operator_str == ">=") {
            return x >= y;
        }
    }

    return false;
}

bool Executor::betweenValues(const Value& v, const Value& l, const Value& r) {
    if (v.getType() != l.getType() || v.getType() != r.getType()) {
        return false;
    }

    if (v.getType() == Value::Type::kInt) {
        int x = v.asInt();
        return l.asInt() <= x && x < r.asInt();

    } else if (v.getType() == Value::Type::kString) {
        const auto& x = v.asString();
        return l.asString() <= x && x < r.asString();
    }

    return false;
}

bool Executor::likeValues(const Value& v, const Value& pattern) {
    if (v.getType() != Value::Type::kString ||
        pattern.getType() != Value::Type::kString) {
        return false;
    }

    try {
        std::regex re(pattern.asString());
        return std::regex_match(v.asString(), re);
    } catch (...) {
        return false;
    }
}

} // namespace dbms
