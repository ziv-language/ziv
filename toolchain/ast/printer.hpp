// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_AST_PRINTER_HPP
#define ZIV_TOOLCHAIN_AST_PRINTER_HPP

#include <optional>
#include <vector>

#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "node_kind.hpp"
#include "tree.hpp"

namespace ziv::toolchain::ast {

class Printer {
public:
    struct PrintOptions {
        bool show_indices;
        bool show_token_info;
        bool show_error_state;
        bool compact_mode;
        bool show_metadata;
        unsigned indent_width;

        PrintOptions()
            : show_indices(true),
              show_token_info(true),
              show_error_state(true),
              compact_mode(false),
              show_metadata(true),
              indent_width(2) {}
    };

    explicit Printer(const AST& ast, PrintOptions options = PrintOptions{})
        : ast_(ast), options_(options) {}

    void print(llvm::raw_ostream& os) const {
        os << "AST Structure ";
        if (ast_.empty()) {
            os << "(empty)\n";
            return;
        }
        os << "(" << ast_.size() << " nodes):\n";
        print_node(os, ast_.get_root(), 0, {});
    }

    static const llvm::StringRef get_kind_name(NodeKind kind);

private:
    void print_indentation(llvm::raw_ostream& os,
                           size_t indent,
                           const std::vector<bool>& last_child) const;

    void print_node_header(llvm::raw_ostream& os, AST::Node node) const;

    void print_node_details(llvm::raw_ostream& os,
                            AST::Node node,
                            size_t indent,
                            const std::vector<bool>& last_child) const;

    void print_node(llvm::raw_ostream& os,
                    AST::Node node,
                    size_t indent,
                    std::vector<bool> last_child) const;

    const AST& ast_;
    PrintOptions options_;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST& ast) {
    Printer(ast).print(os);
    return os;
}

}  // namespace ziv::toolchain::ast

#endif  // ZIV_TOOLCHAIN_AST_PRINTER_HPP
