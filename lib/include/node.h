#pragma once
#include <iostream>

template<typename Key>
class Node {
public:
    Key value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    explicit Node(Key node_value) : value(node_value) {};

    ~Node() = default;

    bool operator() (const Node<Key>& lhs, const Node<Key>& rhs) const {
        return lhs.value < rhs.value;
    }
};

template<typename Key>
class NodeWrapper {
public:
    using value_type = Key;
    using allocator_type = std::allocator<Node<value_type>>;

    NodeWrapper() = default;

    NodeWrapper(Node<value_type>*& node, allocator_type allocator) : value_(node->value), allocator_(allocator) {};

    ~NodeWrapper() = default;

    NodeWrapper& operator=(const NodeWrapper& rhs) {
        value_ = rhs.value_;
        allocator_ = rhs.allocator_;

        return *this;
    }

    [[nodiscard]] bool empty() const {
        return value_ == value_type{};
    }

    explicit operator bool() const {
        return empty();
    }

    allocator_type get_allocator() const {
        return allocator_;
    }

    value_type value() const {
        return value_;
    }

    void swap(NodeWrapper& rhs) {
        std::swap(value_, rhs.value_);
        std::swap(allocator_, rhs.allocator_);
    }

    friend void swap(NodeWrapper& lhs, NodeWrapper& rhs) {
        lhs.swap(rhs);
    }
private:
    value_type value_;
    allocator_type allocator_;
};