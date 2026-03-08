#pragma once

#include "catalog/system.hpp"
#include "common/types.hpp"
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

    bool matchConditions(const std::vector<Condition>& conditions,
                         const std::vector<Value>& row,
                         const std::vector<ColumnSchema>& schema);
    int findColumnIndex(const std::vector<ColumnSchema>& schema, const std::string& name);
    Value resolveOperand(const Operand& operand, const std::vector<Value>& row,
                         const std::vector<ColumnSchema> schema);
    bool compareValues(const Value& a, const Value& b, const std::string& operator_str);
    bool betweenValues(const Value& v, const Value& l, const Value& r);
    bool likeValues(const Value& v, const Value& pattern);

private:
    System& system;
};

}
