#include "common/value.hpp"

namespace dbms {

Value::Value() :
    type(Type::kNull),
    int_value(0)
{}

Value::Value(int value) :
    type(Type::kInt),
    int_value(value)
{}

Value::Value(const std::string& value) :
    type(Type::kString),
    int_value(0),
    string_value(value)
{}

Value::Type Value::getType() const {
    return type;
}

int Value::asInt() const {
    return int_value;
}

const std::string Value::asString() const {
    return string_value;
}

bool Value::isNull() const {
    return type == Type::kNull;
}

} // namespace dbms
