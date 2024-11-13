// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/ast/tree.hpp"

namespace ziv::toolchain::ast {

    AST::Node AST::get_root() const {
        return nodes_.empty() ? Node() : Node(0, this);
    }

    NodeKind AST::get_kind(Node node) const {
        if (node.index_ >= nodes_.size()) {
            return NodeKind::Invalid();
        }
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

    AST::Node AST::add_node(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token, bool has_error) {
        size_t index = nodes_.size();
        nodes_.emplace_back(NodeData(kind, token));
        nodes_.back().has_error = has_error;

        // Initialize the node with proper AST pointer
        return Node(index, this);
    }

    void AST::mark_error(AST::Node node) {
        if (node.index_ >= nodes_.size()) {
            return;
        }
        
        // Mark error and propagate up the tree
        nodes_[node.index_].has_error = true;
        size_t current = node.index_;
        while (current != 0) { // Stop at root
            current = nodes_[current].parent;
            nodes_[current].has_error = true;
        }
    }

    void AST::add_child(Node parent, Node child) {
        if (parent.index_ >= nodes_.size() || child.index_ >= nodes_.size()) {
            return;
        }

        // Check if child already has a parent
        if (nodes_[child.index_].parent != 0) {
            // Remove child from old parent's children list
            auto& old_parent_children = nodes_[nodes_[child.index_].parent].children;
            old_parent_children.erase(
                std::remove(old_parent_children.begin(), 
                          old_parent_children.end(), 
                          child.index_),
                old_parent_children.end()
            );
        }

        // Add child to parent
        nodes_[parent.index_].children.push_back(child.index_);
        nodes_[child.index_].parent = parent.index_;

        // Propagate errors up the tree
        if (nodes_[child.index_].has_error) {
            mark_error(parent);
        }
    }

     AST::TreeIterator& AST::TreeIterator::operator++() {
        if (!ast_ || node_.index_ >= ast_->nodes_.size()) {
            return *this;
        }

        const auto& current = ast_->nodes_[node_.index_];

        // If we have children, visit the first child
        if (!current.children.empty()) {
            node_ = Node(current.children.front(), ast_);
            return *this;
        }

        // Find next sibling or uncle (post-order traversal)
        size_t current_index = node_.index_;
        while (true) {
            // If we're at root, we're done
            if (current_index == 1) { // Root is at index 1
                node_ = Node(ast_->nodes_.size(), ast_);
                break;
            }

            const auto& parent = ast_->nodes_[ast_->nodes_[current_index].parent];
            auto it = std::find(parent.children.begin(),
                              parent.children.end(),
                              current_index);
            
            if (it != parent.children.end() && it + 1 != parent.children.end()) {
                // Move to next sibling
                node_ = Node(*(it + 1), ast_);
                break;
            }

            // Move up to parent
            current_index = ast_->nodes_[current_index].parent;
            node_ = Node(current_index, ast_);
        }

        return *this;
    }

} // namespace ziv::toolchain::ast
