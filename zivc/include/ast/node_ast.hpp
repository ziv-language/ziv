// compiler/include/ast/node_ast.hpp

#ifndef NODE_AST_HPP
#define NODE_AST_HPP

#include "lexer/lexer.hpp"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Casting.h"

#include <vector>
#include <utility>
#include <optional>


namespace ziv {
    struct VarType {
        std::vector<int64_t> shape;
    };

    class NodeAST {
        public:
            enum NodeKindAST {
                AST_Binary,
                AST_FnDecl,
                AST_FnDef,
                AST_FnCall,
                AST_For,
                AST_If,
                AST_Literal,
                AST_Match,
                AST_Return,
                AST_Var,
                AST_VarDecl,
                AST_While,
            };

            NodeAST(NodeKindAST kind, Location location)
            : kind(kind), location(location) {}
            virtual ~NodeAST() = default;

            NodeKindAST get_kind() const;
            const Location& get_location() const;
        
        private:
            NodeKindAST kind;
            Location location;

    };
}

#endif // NODE_AST_HPP