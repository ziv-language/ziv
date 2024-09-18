#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

enum class SymbolType
{
    VARIABLE,
    FUNCTION,
    OTHER
};

struct Symbol
{
    std::string name;
    SymbolType type;
    int scope_level;
    int memory_address;
    std::string value;

    Symbol(const std::string &name, SymbolType type, int scope_level, int memory_address, const std::string &value);
};

class SymbolTable {
private:
    std::unordered_map<std::string, Symbol> symbols;
    int current_scope_level;

public:
    SymbolTable();

    void enter_scope();
    void exit_scope();
    bool insert(const Symbol& symbol);
    bool lookup(const std::string& name, Symbol& symbol);
};

#endif 
