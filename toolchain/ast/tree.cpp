// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/ast/tree.hpp"

namespace ziv::toolchain::ast {

    AST::Node AST::get_root() const {
        return nodes_.empty() ? Node() : Node(0);
    }

    NodeKind AST::get_kind(Node node) const {
        return nodes_[node.index_].kind;
    }

    ziv::toolchain::lex::TokenBuffer::Token AST::get_token(Node node) const {
        return nodes_[node.index_].token;
    }

    llvm::StringRef AST::get_spelling(Node node) const {
        return get_token(node).get_spelling();
    }

    size_t AST::get_line(Node node) const {
        return get_token(node).get_line();
    }

    AST::Node AST::add_node(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token) {
        size_t index = nodes_.size();
        nodes_.emplace_back(NodeData(kind, token));
        return Node(index, this);
    }

    void AST::add_child(Node parent, Node child) {
        nodes_[parent.index_].children.push_back(child.index_);
        nodes_[child.index_].parent = parent.index_;
    }

    AST::TreeIterator& AST::TreeIterator::operator++() {
        if (!ast_) return *this;

        // If we have children, visit the first child
        const auto& current_impl = ast_->nodes_[node_.index_];
        if (!current_impl.children.empty()) {
            node_ = Node(current_impl.children.front());
            return *this;
        }

        // Otherwise, find the next sibling or uncle
        while (true) {
            const auto& node_impl = ast_->nodes_[node_.index_];
            if (node_impl.parent == node_.index_) {
                // We've reached the root, end iteration
                node_ = Node(ast_->size());
                break;
            }

            const auto& parent_impl = ast_->nodes_[node_impl.parent];
            auto sibling_it = std::find(parent_impl.children.begin(),
                                      parent_impl.children.end(),
                                      node_.index_);

            if (sibling_it != parent_impl.children.end() - 1) {
                // Move to next sibling
                node_ = Node(*(sibling_it + 1));
                break;
            }

            // Move up to parent and continue search
            node_ = Node(node_impl.parent);
        }

        return *this;
    }

} // namespace ziv::toolchain::ast
