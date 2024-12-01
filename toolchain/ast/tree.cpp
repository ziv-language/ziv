// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "toolchain/ast/tree.hpp"

#include <algorithm>
#include <cassert>

namespace ziv::toolchain::ast {

// AST Core Methods
AST::Node AST::get_root() const noexcept {
    return empty() ? Node() : Node(1, this);
}

ziv::toolchain::lex::TokenBuffer::Token AST::get_token(Node node) const noexcept {
    return is_valid_node(node) ? nodes_[node.index_].token
                               : ziv::toolchain::lex::TokenBuffer::Token(
                                   ziv::toolchain::lex::TokenKind::Sof(), llvm::StringRef(), 0, 0);
}

bool AST::is_valid_node(Node node) const noexcept {
    return node.index_ > 0 && node.index_ < nodes_.size()
           && nodes_[node.index_].kind != NodeKind::Invalid();
}

NodeKind AST::get_kind(Node node) const noexcept {
    return is_valid_node(node) ? nodes_[node.index_].kind : NodeKind::Invalid();
}

llvm::StringRef AST::get_spelling(Node node) const noexcept {
    return is_valid_node(node) ? nodes_[node.index_].token.get_spelling() : llvm::StringRef();
}

size_t AST::get_line(Node node) const noexcept {
    return is_valid_node(node) ? nodes_[node.index_].token.get_line() : 0;
}

bool AST::has_error(Node node) const noexcept {
    return is_valid_node(node) && nodes_[node.index_].has_error;
}

// Tree Modification Methods
AST::Node AST::add_node(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token) {
    size_t index = nodes_.size();
    nodes_.emplace_back(NodeData(kind, token));
    return Node(index, this);
}

void AST::add_child(Node parent, Node child) {
    if (!is_valid_node(parent) || !is_valid_node(child)) {
        return;
    }

    // Prevent cycles
    if (is_ancestor(child, parent)) {
        mark_error(parent);
        return;
    }

    // Remove from old parent if exists
    if (nodes_[child.index_].parent != 0) {
        unlink_child(nodes_[child.index_].parent, child.index_);
    }

    // Add to new parent
    nodes_[parent.index_].children.push_back(child.index_);
    nodes_[child.index_].parent = parent.index_;

    // Propagate errors if needed
    if (nodes_[child.index_].has_error) {
        propagate_error(parent.index_);
    }
}

void AST::mark_error(Node node) {
    if (!is_valid_node(node)) {
        return;
    }

    if (!nodes_[node.index_].has_error) {
        propagate_error(node.index_);
    }
}

void AST::clear_error(Node node) noexcept {
    if (is_valid_node(node)) {
        nodes_[node.index_].has_error = false;
    }
}

// Internal Helper Methods
void AST::propagate_error(size_t index) noexcept {
    size_t current = index;
    while (current != 0) {
        auto& node = nodes_[current];
        if (node.has_error)
            break;
        node.has_error = true;
        current = node.parent;
    }
}

void AST::unlink_child(size_t parent_idx, size_t child_idx) noexcept {
    auto& parent_children = nodes_[parent_idx].children;
    parent_children.erase(std::remove(parent_children.begin(), parent_children.end(), child_idx),
                          parent_children.end());
}

bool AST::is_ancestor(Node ancestor, Node descendant) const noexcept {
    if (!is_valid_node(ancestor) || !is_valid_node(descendant)) {
        return false;
    }

    size_t current = descendant.index_;
    while (current != 0) {
        if (current == ancestor.index_) {
            return true;
        }
        current = nodes_[current].parent;
    }
    return false;
}

// Traversal Methods
llvm::iterator_range<AST::TreeIterator> AST::nodes() const noexcept {
    return empty() ? llvm::make_range(TreeIterator(), TreeIterator())
                   : llvm::make_range(TreeIterator(this, get_root()),
                                      TreeIterator(this, Node::create_end_node(this)));
}

llvm::iterator_range<AST::TreeIterator> AST::subtree(Node node) const noexcept {
    return !is_valid_node(node) ? llvm::make_range(TreeIterator(), TreeIterator())
                                : llvm::make_range(TreeIterator(this, node),
                                                   TreeIterator(this, Node::create_end_node(this)));
}

llvm::iterator_range<AST::ChildIterator> AST::children(Node node) const noexcept {
    return !is_valid_node(node)
               ? llvm::make_range(ChildIterator(), ChildIterator())
               : llvm::make_range(ChildIterator(this, node, 0),
                                  ChildIterator(this, node, nodes_[node.index_].children.size()));
}

// Node Implementation
bool AST::Node::is_valid() const noexcept {
    return ast_ && ast_->is_valid_node(*this);
}

bool AST::Node::has_error() const noexcept {
    return ast_ && ast_->has_error(*this);
}

NodeKind AST::Node::get_kind() const noexcept {
    return ast_ ? ast_->get_kind(*this) : NodeKind::Invalid();
}

llvm::StringRef AST::Node::get_spelling() const noexcept {
    return ast_ ? ast_->get_spelling(*this) : llvm::StringRef();
}

size_t AST::Node::get_line() const noexcept {
    return ast_ ? ast_->get_line(*this) : 0;
}

// TreeIterator Implementation
AST::TreeIterator& AST::TreeIterator::operator++() {
    if (!ast_ || !node_.is_valid()) {
        return *this;
    }

    node_ = find_next_postorder(node_);
    return *this;
}

AST::Node AST::TreeIterator::find_leftmost_leaf(Node start) const noexcept {
    if (!start.is_valid())
        return Node();

    Node current = start;
    while (!ast_->nodes_[current.index_].children.empty()) {
        current = Node(ast_->nodes_[current.index_].children[0], ast_);
    }
    return current;
}

//... previous code remains the same until TreeIterator::find_next_postorder ...

AST::Node AST::TreeIterator::find_next_postorder(Node current) const noexcept {
    if (!current.is_valid())
        return Node();

    // If at root, we're done
    if (current.index_ == 1) {
        return Node(ast_->nodes_.size(), ast_);  // End iterator
    }

    const auto& parent = ast_->nodes_[ast_->nodes_[current.index_].parent];
    auto it = std::find(parent.children.begin(), parent.children.end(), current.index_);

    // If we're not the last child, move to next sibling's leftmost descendant
    if (it != parent.children.end() && std::next(it) != parent.children.end()) {
        Node next_sibling(*(std::next(it)), ast_);
        return find_leftmost_leaf(next_sibling);
    }

    // Otherwise, move up to parent
    return Node(ast_->nodes_[current.index_].parent, ast_);
}

// ChildIterator Implementation
AST::Node AST::ChildIterator::operator*() const noexcept {
    if (!ast_ || !node_.is_valid() || index_ >= ast_->nodes_[node_.index_].children.size()) {
        return Node();
    }
    return Node(ast_->nodes_[node_.index_].children[index_], ast_);
}

AST::ChildIterator& AST::ChildIterator::operator++() noexcept {
    if (ast_ && node_.is_valid() && index_ < ast_->nodes_[node_.index_].children.size()) {
        ++index_;
    }
    return *this;
}

}  // namespace ziv::toolchain::ast
