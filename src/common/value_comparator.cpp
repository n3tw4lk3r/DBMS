#include "common/value_comparator.hpp"

namespace dbms {

bool ValueComparator::compare(
    const Value& a,
    const Value& b,
    const std::string& operator_str
) {
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
    }

    if (a.getType() == Value::Type::kString) {
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

bool ValueComparator::between(
    const Value& value,
    const Value& left,
    const Value& right
) {
    if (value.getType() != left.getType() ||
        value.getType() != right.getType()) {
        return false;
    }

    if (value.getType() == Value::Type::kInt) {
        int x = value.asInt();

        return left.asInt() <= x &&
               x < right.asInt();
    }

    if (value.getType() == Value::Type::kString) {
        const auto& x = value.asString();

        return left.asString() <= x &&
               x < right.asString();
    }

    return false;
}

} // namespace dbms

