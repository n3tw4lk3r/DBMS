#include <iostream>
#include <algorithm>

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
    for (const auto& row : rows) {
        if (!matchConditions(cmd.conditions, row, schema)) {
            continue;
        }

        for (size_t i = 0; i < row.size(); ++i) {
            const auto& v = row[i];

            if (v.getType() == Value::Type::kInt) {
                std::cout << v.asInt();
            } else {
                std::cout << v.asString();
            }

            if (i + 1 != row.size()) {
                std::cout << ", ";
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

        for (const auto& a : cmd.assignments) {
            for (size_t i = 0; i < schema.size(); ++i) {
                if (schema[i].name == a.column) {
                    row[i] = a.value;
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

bool Executor::matchConditions(const std::vector<Condition>& conds,
                               const std::vector<Value>& row,
                               const std::vector<ColumnSchema>& schema) {
    if (conds.empty()) {
        return true;
    }

    for (const auto& cond : conds) {
        int idx = -1;

        for (size_t i = 0; i < schema.size(); ++i) {
            if (schema[i].name == cond.left) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            return false;
        }

        const auto& v = row[idx];
        const auto& r = cond.right;

        if (v.getType() != r.getType()) {
            return false;
        }

        if (v.getType() == Value::Type::kInt) {
            int a = v.asInt();
            int b = r.asInt();

            if (cond.op == "==" && !(a == b)) {
                return false;
            }

            if (cond.op == "!=" && !(a != b)) {
                return false;
            }

            if (cond.op == "<"  && !(a < b)) {
                return false;
            }

            if (cond.op == ">"  && !(a > b)) {
                return false;
            }

            if (cond.op == "<=" && !(a <= b)) {
                return false;
            }

            if (cond.op == ">=" && !(a >= b)) {
                return false;
            }

        } else if (v.getType() == Value::Type::kString) {
            const auto& a = v.asString();
            const auto& b = r.asString();

            if (cond.op == "==" && !(a == b)) {
                return false;
            }

            if (cond.op == "!=" && !(a != b)) {
                return false;
            }

            if (cond.op == "<"  && !(a < b)) {
                return false;
            }

            if (cond.op == ">"  && !(a > b)) {
                return false;
            }

            if (cond.op == "<=" && !(a <= b)) {
                return false;
            }

            if (cond.op == ">=" && !(a >= b)) {
                return false;
            }
        }
    }

    return true;
}

} // namespace dbms
