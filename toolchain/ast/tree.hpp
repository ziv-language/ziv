// Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
// See /LICENSE for license details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef ZIV_TOOLCHAIN_AST_TREE_HPP
#define ZIV_TOOLCHAIN_AST_TREE_HPP

#include "node_kind.hpp"
#include "toolchain/lex/token_buffer.hpp"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/iterator.h"
#include "llvm/ADT/iterator_range.h"
#include <stack>
#include <memory>

namespace ziv::toolchain::ast {

class AST {
public:
    class Node;
    class TreeIterator;
    class ChildIterator;

    struct NodeData {
        NodeKind kind;
        ziv::toolchain::lex::TokenBuffer::Token token;
        llvm::SmallVector<size_t, 4> children;
        size_t parent;
        bool has_error : 1;
        bool visited : 1;

        NodeData() : kind(NodeKind::Invalid()),
                    token(ziv::toolchain::lex::TokenKind::Sof(), llvm::StringRef(), 0, 0),
                    parent(0), has_error(false), visited(false) {}

        NodeData(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token)
            : kind(kind), token(token), parent(0), has_error(false), visited(false) {}
    };

    AST() {
        // Reserve index 0 for sentinel
        nodes_.emplace_back(NodeData(NodeKind::Invalid(),
            ziv::toolchain::lex::TokenBuffer::Token(
                ziv::toolchain::lex::TokenKind::Sof(),
                llvm::StringRef(),
                0,
                0
            )));
    }

    // Core tree operations
    [[nodiscard]] size_t size() const noexcept { return nodes_.size(); }
    [[nodiscard]] Node get_root() const noexcept;
    [[nodiscard]] bool empty() const noexcept { return nodes_.size() <= 1; }

    // Node property accessors
    [[nodiscard]] NodeKind get_kind(Node node) const noexcept;
    [[nodiscard]] ziv::toolchain::lex::TokenBuffer::Token get_token(Node node) const noexcept;
    [[nodiscard]] llvm::StringRef get_spelling(Node node) const noexcept;
    [[nodiscard]] size_t get_line(Node node) const noexcept;
    [[nodiscard]] bool has_error(Node node) const noexcept;

    // Tree modification operations
    Node add_node(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token);
    void add_child(Node parent, Node child);
    void mark_error(Node node);
    void clear_error(Node node) noexcept;

    // Traversal interfaces
    [[nodiscard]] llvm::iterator_range<TreeIterator> nodes() const noexcept;
    [[nodiscard]] llvm::iterator_range<TreeIterator> subtree(Node node) const noexcept;
    [[nodiscard]] llvm::iterator_range<ChildIterator> children(Node node) const noexcept;

    // Validation helpers
    [[nodiscard]] bool is_valid_node(Node node) const noexcept;
    [[nodiscard]] bool is_ancestor(Node ancestor, Node descendant) const noexcept;

private:
    llvm::SmallVector<NodeData, 32> nodes_; // Pre-allocated for typical AST size

    friend class Node;
    friend class TreeIterator;
    friend class ChildIterator;

    // Internal helper methods
    void propagate_error(size_t index) noexcept;
    void unlink_child(size_t parent_idx, size_t child_idx) noexcept;
};

class AST::Node {
public:
    Node() noexcept : index_(0), ast_(nullptr) {}

    // Core node operations
    [[nodiscard]] bool is_valid() const noexcept;
    [[nodiscard]] bool has_error() const noexcept;
    [[nodiscard]] NodeKind get_kind() const noexcept;
    [[nodiscard]] llvm::StringRef get_spelling() const noexcept;
    [[nodiscard]] size_t get_line() const noexcept;

    // Comparison operators
    bool operator==(const Node& rhs) const noexcept { return index_ == rhs.index_; }
    bool operator!=(const Node& rhs) const noexcept { return !(*this == rhs); }
    bool operator<(const Node& rhs) const noexcept { return index_ < rhs.index_; }

    static Node create_end_node(const AST* ast) {
            return Node(ast->nodes_.size(), ast);
    }

    size_t get_index() const noexcept { return index_; }

protected:
    Node(size_t index, const AST* ast) noexcept : index_(index), ast_(ast) {}

    size_t index_;
    const AST* ast_;

    friend class AST;
    friend class TreeIterator;
    friend class ChildIterator;
};

// Post-order tree iterator
class AST::TreeIterator
    : public llvm::iterator_facade_base<TreeIterator,
                                      std::forward_iterator_tag,
                                      Node,
                                      ptrdiff_t,
                                      const Node*,
                                      const Node> {
public:
    TreeIterator() noexcept = default;
    TreeIterator(const AST* ast, Node node) noexcept : ast_(ast), node_(node) {}

    bool operator==(const TreeIterator& rhs) const noexcept {
        return node_ == rhs.node_ && ast_ == rhs.ast_;
    }

    Node operator*() const noexcept { return node_; }
    TreeIterator& operator++();

private:
    const AST* ast_ {nullptr};
    Node node_;

    // Helper methods for traversal
    [[nodiscard]] Node find_leftmost_leaf(Node start) const noexcept;
    [[nodiscard]] Node find_next_postorder(Node current) const noexcept;
};

// Efficient child iterator
class AST::ChildIterator
    : public llvm::iterator_facade_base<ChildIterator,
                                      std::forward_iterator_tag,
                                      Node,
                                      ptrdiff_t,
                                      const Node*,
                                      const Node> {
public:
    ChildIterator() noexcept = default;

    bool operator==(const ChildIterator& rhs) const noexcept {
        return index_ == rhs.index_ && ast_ == rhs.ast_ && node_ == rhs.node_;
    }

    Node operator*() const noexcept;
    ChildIterator& operator++() noexcept;

private:
    friend class AST;

    ChildIterator(const AST* ast, Node node, size_t index) noexcept
        : ast_(ast), node_(node), index_(index) {}

    const AST* ast_ {nullptr};
    Node node_;
    size_t index_ {0};
};

} // namespace ziv::toolchain::ast

#endif // ZIV_TOOLCHAIN_AST_TREE_HPP
