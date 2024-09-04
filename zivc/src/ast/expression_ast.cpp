// compiler/src/ast/expression_ast.cpp

#include "ast/expression_ast.hpp"

namespace ziv {
    llvm::ArrayRef<std::unique_ptr<NodeAST>> LiteralAST::get_values() {
        return values;
    }

    llvm::ArrayRef<int64_t> LiteralAST::get_dims() {
        return dims;
    }

    bool LiteralAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_Literal;
    }

    llvm::StringRef VariableAST::get_name() {
        return name;
    }

    bool VariableAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_Var;
    }

    llvm::StringRef VarDeclAST::get_name() {
        return name;
    }

    const VarType& VarDeclAST::get_type() const {
        return type;
    }

    NodeAST *VarDeclAST::get_value() {
        return value.get();
    }

    bool VarDeclAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_VarDecl;
    }

    std::optional<NodeAST*> ReturnAST::get_value() {
        if (value.has_value()){
            return value->get();
        }
        return std::nullopt;
    }

    bool ReturnAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_Return;
    }

    char BinaryAST::get_op() {
        return op;
    }

    NodeAST *BinaryAST::get_lhs() {
        return lhs.get();
    }

    NodeAST *BinaryAST::get_rhs() {
        return rhs.get();
    }

    bool BinaryAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_Binary;
    }

    llvm::StringRef FnCallAST::get_callee() {
        return callee;
    }

    llvm::ArrayRef<std::unique_ptr<NodeAST>> FnCallAST::get_args() {
        return args;
    }

    bool FnCallAST::classof(const NodeAST *node) {
        return node->get_kind() == AST_FnCall;
    }

    const Location& FnDeclAST::get_location() {
        return location;
    }

    llvm::StringRef FnDeclAST::get_name() const {
        return name;
    }

    llvm::ArrayRef<std::unique_ptr<VariableAST>> FnDeclAST::get_args(){
        return args;
    }

    FnDeclAST* FnDefAST::get_prototype() {
        return prototype.get();
    }

    NodeASTList* FnDefAST::get_body() {
        return body.get();
    }


} // namespace ziv