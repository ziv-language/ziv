// compiler/include/ast/expression_ast.hpp

#ifndef EXPRESSION_AST_HPP
#define EXPRESSION_AST_HPP

#include "node_ast.hpp"

namespace ziv {

    using NodeASTList = std::vector<std::unique_ptr<NodeAST>>;

    // Expression class for numeric literals like '1.0'
    class LiteralAST : public NodeAST {
        std::vector<std::unique_ptr<NodeAST>> values;
        std::vector<int64_t> dims;
        public:
            LiteralAST(Location location, std::vector<std::unique_ptr<NodeAST>> values, std::vector<int64_t> dims)
            : NodeAST(AST_Literal, location), values(std::move(values)), dims(std::move(dims)) {}

            llvm::ArrayRef<std::unique_ptr<NodeAST>> get_values();
            llvm::ArrayRef<int64_t> get_dims();

            static bool classof(const NodeAST *node);
    };

    // Expression class for variable reference like 'a' 
    class VariableAST : public NodeAST {
        std::string name;
        public:
            VariableAST(Location location, std::string name)
            : NodeAST(AST_VarDecl, location), name(std::move(name)) {}

            llvm::StringRef get_name();

            static bool classof(const NodeAST *node);
    };

    // Expression class for variable declarations like 'let x = 1'
    class VarDeclAST : public NodeAST {
        std::string name;
        VarType type;
        std::unique_ptr<NodeAST> value;

        public:
            VarDeclAST(Location location, std::string name, VarType type, std::unique_ptr<NodeAST> value)
            : NodeAST(AST_VarDecl, location), name(std::move(name)), type(type), value(std::move(value)) {}

            llvm::StringRef get_name();
            const VarType& get_type() const;
            NodeAST *get_value();

            static bool classof(const NodeAST *node);
    };

    // Expression class for return statements like 'return 1'
    class ReturnAST : public NodeAST {
        std::optional<std::unique_ptr<NodeAST>> value;

        public:
            ReturnAST(Location location, std::optional<std::unique_ptr<NodeAST>> value)
            : NodeAST(AST_Return, location), value(std::move(value)) {}

            std::optional<NodeAST*> get_value();

            static bool classof(const NodeAST *node);
    };

    // Expression class for binary operations like '+', '-', '*', '/'
    class BinaryAST : public NodeAST {
        char op;
        std::unique_ptr<NodeAST> lhs, rhs;

        public:
            BinaryAST(Location location, char op, std::unique_ptr<NodeAST> lhs, std::unique_ptr<NodeAST> rhs)
            : NodeAST(AST_Binary, location), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

            char get_op();
            NodeAST *get_lhs();
            NodeAST *get_rhs();

            static bool classof(const NodeAST *node);
    };

    // Expression class for function calls like 'foo(1, 2)'
    class FnCallAST : public NodeAST {
        std::string callee;
        NodeASTList args;

        public:
            FnCallAST(Location location, std::string callee, NodeASTList args)
            : NodeAST(AST_FnCall, location), callee(std::move(callee)), args(std::move(args)) {}

            llvm::StringRef get_callee();
            llvm::ArrayRef<std::unique_ptr<NodeAST>> get_args();

            static bool classof(const NodeAST *node);
    };

    // Expression class for function prototypes like 'fn foo(a: int, b: int) -> int'
    class FnDeclAST {
        Location location;
        std::string name;
        std::vector<std::unique_ptr<VariableAST>> args;

        public:
            FnDeclAST(Location location, const std::string &name, std::vector<std::unique_ptr<VariableAST>> args)
            : location(std::move(location)), name(name), args(std::move(args)) {}

            const Location &get_location();
            llvm::StringRef get_name() const;
            llvm::ArrayRef<std::unique_ptr<VariableAST>> get_args();
    };

    // Expression class for function definitions like 'fn foo(a: int, b: int) -> int { return a + b }'
    class FnDefAST {
        std::unique_ptr<FnDeclAST> prototype;
        std::unique_ptr<NodeASTList> body;

        public:
            FnDefAST(std::unique_ptr<FnDeclAST> prototype, std::unique_ptr<NodeASTList> body)
            : prototype(std::move(prototype)), body(std::move(body)) {}

            FnDeclAST *get_prototype();
            NodeASTList *get_body();
    };

    // Expression class for modules
    class ModuleAST {
        std::vector<FnDefAST> functions;

        public:
            ModuleAST(std::vector<FnDefAST> functions)
            : functions(std::move(functions)) {}

            auto begin() { return functions.begin(); }
            auto end() { return functions.end(); }
    };

    void dump_ast(ModuleAST &);

} // namespace ziv

#endif // EXPRESSION_AST_HPP