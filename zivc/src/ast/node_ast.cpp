// compiler/src/ast/ast_node.cpp

#include "ast/node_ast.hpp"

namespace ziv {
    NodeAST::NodeKindAST NodeAST::get_kind() const {
        return kind;
    }
    const Location& NodeAST::get_location() const {
        return location;
    }
}