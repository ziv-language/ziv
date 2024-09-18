#include <symbol_table.hpp>


Symbol::Symbol(const std::string &name, SymbolType type, int scope_level, int memory_address, const std::string &value)
    : name(name), type(type), scope_level(scope_level), memory_address(memory_address), value(value) {}

SymbolTable::SymbolTable() : current_scope_level(0) {}

void SymbolTable::enter_scope() {
    ++current_scope_level;
}

void SymbolTable::exit_scope() {
    for (auto it = symbols.begin(); it != symbols.end(); ) {
        if (it->second.scope_level == current_scope_level) {
            it = symbols.erase(it);
        } else {
            ++it;
        }
    }
    --current_scope_level;
}

bool SymbolTable::insert(const Symbol& symbol) {
    std::string key = symbol.name + "_" + std::to_string(symbol.scope_level);
    auto result = symbols.insert({key, symbol});
    return result.second;
}

bool SymbolTable::lookup(const std::string& name, Symbol& symbol) {
    for (int level = current_scope_level; level >= 0; --level) {
        std::string key = name + "_" + std::to_string(level);
        auto it = symbols.find(key);
        if (it != symbols.end()) {
            symbol = it->second;
            return true;
        }
    }
    return false;
}
