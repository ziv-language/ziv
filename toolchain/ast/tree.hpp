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

namespace ziv::toolchain::ast {

    class AST {
        public:
            class Node;
            class TreeIterator;
            class ChildIterator;

            AST() = default;

            bool has_errors() const  {
                for (const auto& node: nodes_) {
                    if (node.has_error) {
                        return true;
                    }
                }
                return false;
            }

            size_t size() const { return nodes_.size(); }

            Node get_root() const;

            llvm::iterator_range<TreeIterator> get_nodes() const;

            llvm::iterator_range<TreeIterator> get_subtree(Node node) const;

            llvm::iterator_range<ChildIterator> get_children(Node node) const;

            NodeKind get_kind(Node node) const;
            ziv::toolchain::lex::TokenBuffer::Token get_token(Node node) const;
            llvm::StringRef get_spelling(Node node) const;
            size_t get_line(Node node) const;

            Node add_node(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token, bool has_error = false);

            void mark_error(Node node);

            void add_child(Node parent, Node child);

        private:
            struct NodeData {
                NodeKind kind;
                ziv::toolchain::lex::TokenBuffer::Token token;
                llvm::SmallVector<size_t, 4> children; // quad tree
                size_t parent;
                bool has_error {false};

                NodeData(NodeKind kind, ziv::toolchain::lex::TokenBuffer::Token token):
                    kind(kind), token(token), parent(0) {}

            };

            llvm::SmallVector<NodeData, 0> nodes_;
            bool has_errors_ = false;
    };

    class AST::Node {
        public:
            Node(): index_(0), ast_(nullptr) {}

            friend bool operator==(const Node& lhs, const Node& rhs) {
                return lhs.index_ == rhs.index_;
            }

            friend bool operator!=(const Node& lhs, const Node& rhs) {
                return lhs.index_ != rhs.index_;
            }

            friend bool operator<(const Node& lhs, const Node& rhs) {
                return lhs.index_ < rhs.index_;
            }

            friend bool operator>(const Node& lhs, const Node& rhs) {
                return lhs.index_ > rhs.index_;
            }

            friend bool operator<=(const Node& lhs, const Node& rhs) {
                return lhs.index_ <= rhs.index_;
            }

            friend bool operator>=(const Node& lhs, const Node& rhs) {
                return lhs.index_ >= rhs.index_;
            }

            llvm::StringRef get_spelling() const {
                return ast_->get_spelling(*this);
            }

            size_t get_index() const {
                return index_;
            }

            bool is_valid() const {
                // Check if the index is within the valid ranges
                if (index_ == 0) {
                    return false;
                }

                const NodeData& data = ast_->nodes_[index_];
                if (data.parent >= ast_->nodes_.size()) {
                    return false; // points to an invalid parent
                }

                if (data.kind == NodeKind::Invalid()) {
                    return false;
                }

                return true;
            }

        private:
            // Constructor a node into the post-order traversal
            // Node() : index_(0), ast_(nullptr) {}

            explicit Node(size_t index) : index_(index), ast_(nullptr) {}

            Node(size_t index, const AST* ast) : index_(index), ast_(ast) {}
            size_t index_;

            const AST* ast_ {nullptr};

            friend class AST;
    };

    class AST::TreeIterator
        : public llvm::iterator_facade_base<
            TreeIterator,// This is the type we're defining
            std::forward_iterator_tag, // This iterator is a forward iterator
            AST::Node, // The type of the elements
            ptrdiff_t, // The type of the difference between two iterators
            const AST::Node*,
            const AST::Node> {
        public:
            TreeIterator() = default;

            bool operator==(const TreeIterator& rhs) const {
                return node_ == rhs.node_ && ast_ == rhs.ast_;
            }

            bool operator!=(const TreeIterator& other) const {
                return node_ != other.node_;
            }

            TreeIterator& operator++();

        private:
            friend class AST;

            TreeIterator(const AST* ast, AST::Node node):
                ast_(ast), node_(node) {}

            const AST* ast_ {nullptr};
            AST::Node node_;
    };

    class AST::ChildIterator
        : public llvm::iterator_facade_base<
            ChildIterator,// This is the type we're defining
            std::forward_iterator_tag, // This iterator is a forward iterator
            AST::Node, // The type of the elements
            ptrdiff_t, // The type of the difference between two iterators
            const AST::Node*,
            const AST::Node> {
        public:
            ChildIterator() = default;

            bool operator==(const ChildIterator& rhs) const {
                return index_ == rhs.index_ && ast_ == rhs.ast_ && node_ == rhs.node_;
            }

            AST::Node operator*() const {
                return AST::Node(ast_->nodes_[node_.index_].children[index_]);
            }

            ChildIterator& operator++() {
                ++index_;
                return *this;
            }

        private:
            friend class AST;

            ChildIterator(const AST* ast, AST::Node node, size_t index):
                ast_(ast), node_(node), index_(index) {}

            const AST* ast_ {nullptr};
            AST::Node node_;
            size_t index_ {0};
    };

        inline llvm::iterator_range<AST::TreeIterator> AST::get_nodes() const {
        if (nodes_.empty()) {
            return llvm::make_range(
                TreeIterator(this, Node()),
                TreeIterator(this, Node())
            );
        }
        return llvm::make_range(
            TreeIterator(this, get_root()),
            TreeIterator(this, Node(nodes_.size()))
        );
    }

    inline llvm::iterator_range<AST::TreeIterator> AST::get_subtree(Node node) const {
        if (node.index_ >= nodes_.size()) {
            return llvm::make_range(
                TreeIterator(this, Node()),
                TreeIterator(this, Node())
            );
        }
        return llvm::make_range(
            TreeIterator(this, node),
            TreeIterator(this, Node(nodes_.size()))
        );
    }

    inline llvm::iterator_range<AST::ChildIterator> AST::get_children(Node node) const {
        if (node.index_ >= nodes_.size()) {
            return llvm::make_range(
                ChildIterator(),
                ChildIterator()
            );
        }
        return llvm::make_range(
            ChildIterator(this, node, 0),
            ChildIterator(this, node, nodes_[node.index_].children.size())
        );
    }

} // namespace ziv::toolchain::ast

#endif // ZIV_TOOLCHAIN_AST_TREE_HPP
