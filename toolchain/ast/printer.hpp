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
            Printer(const AST& ast): ast_(ast) {}

            void print(llvm::raw_ostream& os) const {
                os << "[\n";
                print_node(os, ast_.get_root(), 0);
                os << "]\n";
            }
            static const llvm::StringRef get_kind_name(NodeKind kind);

        private:
            void print_node(llvm::raw_ostream& os, AST::Node node, size_t indent) const {
                const auto& children = ast_.get_children(node);
                bool has_children = !children.empty();

                // Print indentation
                for (size_t i = 0; i < indent; ++i) {
                    os << "  ";
                }

                // Start node
                os << "{node_index: " << node.get_index()
                   << ", kind: '" << get_kind_name(ast_.get_kind(node))
                   << "', line: " << ast_.get_line(node)
                   << ", column: " << ast_.get_token(node).get_column()
                   << ", token: '" << ast_.get_token(node).get_spelling()
                   << "', value: '" << ast_.get_token(node).get_name()
                   << "', text: '" << ast_.get_spelling(node) << "'";

                // Handle children if any
                if (has_children) {
                    os << ", children: [\n";

                    bool first = true;
                    for (const auto& child : children) {
                        if (!first) {
                            os << ",\n";
                        }
                        print_node(os, child, indent + 1);
                        first = false;
                    }

                    os << "\n";
                    for (size_t i = 0; i < indent; ++i) {
                        os << "  ";
                    }
                    os << "]}";
                } else {
                    os << "}";
                }

                if (indent == 0) {
                    os << ",\n";
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
