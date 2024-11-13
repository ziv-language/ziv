// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_AST_PRINTER_HPP
#define ZIV_TOOLCHAIN_AST_PRINTER_HPP

#include "node_kind.hpp"
#include "tree.hpp"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"

namespace ziv::toolchain::ast {

class Printer {
public:
    Printer(const AST& ast) : ast_(ast) {}

    void print(llvm::raw_ostream& os) const {
        os << "AST Structure:\n";
        print_node(os, ast_.get_root(), 0);
    }

    static const llvm::StringRef get_kind_name(NodeKind kind);

private:
    void print_indentation(llvm::raw_ostream& os, size_t indent) const {
        os.indent(indent * 2);
    }

    void print_node_header(llvm::raw_ostream& os, AST::Node node) const {
        os << get_kind_name(ast_.get_kind(node)) << " ("
           << "idx:" << node.get_index() << ", "
           << "ln:" << ast_.get_line(node) << ", "
           << "col:" << ast_.get_token(node).get_column() << ")";
    }

    void print_node_details(llvm::raw_ostream& os, AST::Node node) const {
        const auto& token = ast_.get_token(node);
        if (!token.get_spelling().empty()) {
            os << "\n";
            print_indentation(os, 1);
            os << "└─ Token: '" << token.get_spelling() << "'";
        }
        if (!token.get_name().empty() && token.get_name() != token.get_spelling()) {
            os << "\n";
            print_indentation(os, 1);
            os << "└─ Value: '" << token.get_name() << "'";
        }
        const auto& spelling = ast_.get_spelling(node);
        if (!spelling.empty() && spelling != token.get_spelling()) {
            os << "\n";
            print_indentation(os, 1);
            os << "└─ Text: '" << spelling << "'";
        }
    }

    void print_node(llvm::raw_ostream& os, AST::Node node, size_t indent) const {
        print_indentation(os, indent);
        
        // Print node header
        os << "└─ ";
        print_node_header(os, node);
        
        // Handle children
        const auto& children = ast_.get_children(node);
        if (!children.empty()) {
            // Print node details before going into children
            print_node_details(os, node);
            os << "\n";
            
            // Print children with increased indentation
            size_t child_count = 0;
            for (const auto& child : children) {
                child_count++;
                // Use different prefix for last child
                if (child_count == std::distance(children.begin(), children.end())) {
                    print_node(os, child, indent + 1); 
                } else {
                    print_indentation(os, indent + 1);
                    os << "├─ ";
                    print_node_header(os, child);
                    os << "\n";
                }
            }
        } else {
            // Print details for leaf nodes
            print_node_details(os, node);
        }
    }

    const AST& ast_;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST& ast) {
    Printer(ast).print(os);
    return os;
}

} // namespace ziv::toolchain::ast

#endif // ZIV_TOOLCHAIN_AST_PRINTER_HPP
