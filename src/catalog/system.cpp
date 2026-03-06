#include "catalog/system.hpp"

namespace dbms {

void System::createDatabase(const std::string& name) {
    databases[name] = std::make_unique<Database>(name);
}

Database* System::getDatabase(const std::string& name) {
    auto it = databases.find(name);

    if (it == databases.end()) {
        return nullptr;
    }

    return it->second.get();
}

void System::useDatabase(const std::string& name) {
    current_database = getDatabase(name);
}

Database* System::getCurrentDatabase() {
    return current_database;
}

} // namespace dbms
