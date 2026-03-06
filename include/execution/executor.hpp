#pragma once

#include "catalog/system.hpp"
#include "sqlparser/command.hpp"

namespace dbms {

class Executor {
public:
    explicit Executor(System& system);

    void execute(const Command& cmd);

private:
    void executeCreateDatabase(const Command& cmd);
    void executeDropDatabase(const Command& cmd);
    void executeUseDatabase(const Command& cmd);

    void executeCreateTable(const Command& cmd);
    void executeDropTable(const Command& cmd);

    void executeInsert(const Command& cmd);
    void executeSelect(const Command& cmd);
    void executeUpdate(const Command& cmd);
    void executeDelete(const Command& cmd);

    bool matchConditions(const std::vector<Condition>& conds,
                         const std::vector<Value>& row,
                         const std::vector<ColumnSchema>& schema);

private:
    System& system;
};

}
