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
                print_node(os, ast_.get_root(), 0);
            }

            // get_kind_name is a static function that returns the name of the node kind
            static const llvm::StringRef get_kind_name(NodeKind kind);

        private:
            void print_node(llvm::raw_ostream& os, AST::Node node, size_t indent) const {
                for (size_t i = 0; i < indent; ++i) {
                    os << "  ";
                }

                os << get_kind_name(ast_.get_kind(node)) << " ("
                    << ast_.get_spelling(node) << ")\n";

                for (const auto& child : ast_.get_children(node)) {
                    print_node(os, child, indent + 1);
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
