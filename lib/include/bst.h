#include "node.h"
#include <iostream>
#include <memory>
#include <numeric>
#include <vector> // FOR TRAVERSAL TESTING

namespace BST {
    struct PreOrderTraversal {};

    struct InOrderTraversal {};

    struct PostOrderTraversal {};

    template<typename Key, typename TraversalTag, typename Comparator = std::less<Key>, typename Allocator = std::allocator<Node<Key>>>
    class BinarySearchTree {
    public:
        template<bool IsConst>
        class Iterator {
        public:
            using key_type = Key;
            using value_type = Node<Key>;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;
            using difference_type = std::ptrdiff_t;
            using size_type = unsigned int;
            using iterator_category = std::bidirectional_iterator_tag;
            using traversal_tag = TraversalTag;

            using conditional_ptr = std::conditional_t<IsConst, const_pointer, pointer>;
            using conditional_key_ref = std::conditional_t<IsConst, const key_type&, const key_type&>; // BST INVARIANT
            using conditional_ref = std::conditional_t<IsConst, const_reference, const_reference>; // BST INVARIANT

            Iterator() = default;

            explicit Iterator(pointer node, traversal_tag tag, pointer begin_ptr, pointer end_ptr) : node_ptr_(node), tag_(tag), begin_ptr_(begin_ptr), end_ptr_(end_ptr) {};

            Iterator(const Iterator& other) : node_ptr_(other.node_ptr_), tag_(other.tag_), begin_ptr_(other.begin_ptr_), end_ptr_(other.end_ptr_) {};

            Iterator(Iterator&& other) noexcept = default;

            ~Iterator() = default;

            bool operator==(const Iterator& rhs_iter) const {
                return node_ptr_ == rhs_iter.node_ptr_;
            }

            bool operator!=(const Iterator& rhs_iter) const {
                return !(operator==(rhs_iter));
            }

            Iterator& operator=(const Iterator& rhs) {
                if (*this != rhs) Iterator(rhs).swap(*this);

                return *this;
            }

            Iterator& operator=(Iterator&& rhs) noexcept {
                swap(rhs);

                return *this;
            }

            Iterator& operator++() {
                if (node_ptr_ == end_ptr_) throw std::out_of_range("Iterator is out of range");

                Increment(tag_);

                return *this;
            }

            Iterator operator++(int) {
                auto temp_iter = *this;
                ++*this;

                return temp_iter;
            }

            Iterator& operator--() {
                if (node_ptr_== begin_ptr_) throw std::out_of_range("Iterator is out of range");

                Decrement(tag_);

                return *this;
            }

            Iterator operator--(int) {
                auto temp_iter = *this;
                --*this;

                return temp_iter;
            }

            conditional_key_ref operator*() const {
                ValidateIterator();

                return node_ptr_->value;
            }

            void swap(Iterator& rhs) {
                std::swap(node_ptr_, rhs.node_ptr_);
                std::swap(begin_ptr_, rhs.begin_ptr_);
                std::swap(end_ptr_, rhs.end_ptr_);
                std::swap(tag_, rhs.tag_);
            }

            friend void swap(Iterator& lhs, Iterator& rhs) {
                lhs.swap(rhs);
            }
        private:
            conditional_ptr node_ptr_ = nullptr;
            conditional_ptr begin_ptr_ = nullptr;
            conditional_ptr end_ptr_ = nullptr;
            traversal_tag tag_;

            void ValidateIterator() const {
                if (node_ptr_ == nullptr) throw std::runtime_error("Invalid iterator");
            }

            void Increment(PreOrderTraversal tag) {
                if (node_ptr_->left != nullptr) {
                    node_ptr_ = node_ptr_->left;
                } else if (node_ptr_->right != nullptr) {
                    node_ptr_ = node_ptr_->right;
                } else {
                    if (node_ptr_->parent == nullptr) {
                        node_ptr_ = node_ptr_->right;
                    } else {
                        conditional_ptr temp_node = node_ptr_->parent;
                        conditional_ptr prev_node = node_ptr_;
                        while (temp_node->right == nullptr || temp_node->right == prev_node) {
                            prev_node = temp_node;
                            temp_node = temp_node->parent;
                        }
                        node_ptr_ = temp_node->right;
                    }
                }
            }

            void Decrement(PreOrderTraversal tag) {
                if (node_ptr_->parent->left == node_ptr_) {
                    node_ptr_ = node_ptr_->parent;
                } else {
                    if (node_ptr_->parent->left == nullptr) {
                        node_ptr_ = node_ptr_->parent;
                    } else {
                        conditional_ptr temp_node = node_ptr_->parent->left;
                        while (temp_node->left != nullptr || temp_node->right != nullptr) {
                            if (temp_node->right != nullptr) {
                                temp_node = temp_node->right;
                            } else {
                                temp_node = temp_node->left;
                            }
                        }
                        node_ptr_ = temp_node;
                    }
                }
            }

            void Increment(InOrderTraversal tag) {
                if (node_ptr_->right != nullptr) {
                    conditional_ptr temp_node = node_ptr_->right;
                    while (temp_node->left != nullptr) {
                        temp_node = temp_node->left;
                    }
                    node_ptr_ = temp_node;
                } else {
                    if (node_ptr_->parent->left == node_ptr_) {
                        node_ptr_ = node_ptr_->parent;
                    } else {
                        conditional_ptr temp_node = node_ptr_->parent;
                        conditional_ptr prev_node = node_ptr_;
                        while (temp_node->right == nullptr || temp_node->right == prev_node) {
                            prev_node = temp_node;
                            temp_node = temp_node->parent;
                        }
                        node_ptr_ = temp_node;
                    }
                }
            }

            void Decrement(InOrderTraversal tag) {
                if (node_ptr_->left == nullptr) {
                    conditional_ptr temp_node = node_ptr_;
                    while (temp_node->parent->right != temp_node) {
                        temp_node = temp_node->parent;
                    }
                    node_ptr_ = temp_node->parent;
                } else {
                    conditional_ptr temp_node = node_ptr_->left;
                    while (temp_node->right != nullptr) {
                        temp_node = temp_node->right;
                    }
                    node_ptr_ = temp_node;
                }
            }

            void Increment(PostOrderTraversal tag) {
                if (node_ptr_->parent->right == node_ptr_) {
                    node_ptr_ = node_ptr_->parent;
                } else {
                    conditional_ptr temp_node = node_ptr_->parent;
                    if (temp_node->right == nullptr) {
                        node_ptr_ = temp_node;
                    } else {
                        temp_node = temp_node->right;
                        while (temp_node->left != nullptr || temp_node->right != nullptr) {
                            if (temp_node->left != nullptr) {
                                temp_node = temp_node->left;
                            } else {
                                temp_node = temp_node->right;
                            }
                        }
                        node_ptr_ = temp_node;
                    }
                }
            }

            void Decrement(PostOrderTraversal tag) {
                if (node_ptr_->right != nullptr) {
                    node_ptr_ = node_ptr_->right;
                } else if (node_ptr_->left != nullptr) {
                    node_ptr_ = node_ptr_->left;
                } else {
                    conditional_ptr temp_node = node_ptr_->parent;
                    conditional_ptr prev_node = node_ptr_;
                    while (temp_node->left == nullptr || temp_node->left == prev_node) {
                        prev_node = temp_node;
                        temp_node = temp_node->parent;
                    }
                    node_ptr_ = temp_node->left;
                }
            }
        };

        // AllocatorAwareContainer
        using allocator_type = Allocator;
        using allocator_traits = std::allocator_traits<allocator_type>;

        // Container
        using value_type = Node<Key>;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;
        using difference_type = allocator_traits::difference_type;
        using size_type = allocator_traits::size_type;
        using traversal_tag = TraversalTag;

        // AssociativeContainer
        using key_type = Key;
        using key_compare = Comparator;
        using value_compare = key_compare;
        using node_type = NodeWrapper<key_type>;

        // ReversibleContainer
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        BinarySearchTree() {
            DefaultConstructor();
        }

        BinarySearchTree(const BinarySearchTree& other) : comparator_(other.comparator_), tag_(other.tag_), end_ptr_(other.end_ptr_) {
            if (other.empty()) DefaultConstructor();

            head_root_ = CopyTree(other.head_root_);
            UpdateBeginAndEnd(tag_);
        }

        BinarySearchTree(BinarySearchTree&& other) noexcept = default;

        BinarySearchTree(const std::initializer_list<key_type>& values_list) {
            DefaultConstructor();

            insert(values_list);
        }

        BinarySearchTree(iterator it1, iterator it2)  {
            DefaultConstructor();

            insert(it1, it2);
        }

        ~BinarySearchTree() {
            Clear(head_root_);
            DestroyNode(end_ptr_);
        }

        [[nodiscard]] std::vector<key_type> TraversalToVector() const {
            return std::vector<key_type>(begin(), end());
        }

        bool operator==(const BinarySearchTree& rhs) const {
            return std::equal(begin(), end(), rhs.begin(), rhs.end());
        }

        bool operator!=(const BinarySearchTree& rhs) const {
            return !(operator==(rhs));
        }

        BinarySearchTree& operator=(const BinarySearchTree& rhs) {
            if (*this != rhs) BinarySearchTree(rhs).swap(*this);

            return *this;
        }

        BinarySearchTree& operator=(BinarySearchTree&& rhs) noexcept {
            swap(*this);

            return *this;
        }

        iterator begin() const {
            return iterator(begin_ptr_, tag_, begin_ptr_, end_ptr_);
        }

        iterator end() const {
            return iterator(end_ptr_, tag_, begin_ptr_, end_ptr_);
        }

        const_iterator cbegin() const {
            return const_iterator(begin_ptr_, tag_, begin_ptr_, end_ptr_);
        }

        const_iterator cend() const {
            return const_iterator(end_ptr_, tag_, begin_ptr_, end_ptr_);
        }

        reverse_iterator rbegin() const {
            return reverse_iterator(end());
        }

        reverse_iterator rend() const {
            return reverse_iterator(begin());
        }

        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(cend());
        }

        const_reverse_iterator crend() const {
            return const_reverse_iterator(cbegin());
        }

        [[nodiscard]] bool empty() const {
            return head_root_ == nullptr;
        }

        [[nodiscard]] size_type size() const {
            return tree_size_;
        }

        [[nodiscard]] size_type max_size() const {
            return std::numeric_limits<size_type>::max();
        }

        [[nodiscard]] key_compare key_comp() const {
            return comparator_;
        }

        [[nodiscard]] value_compare value_comp() const {
            return key_comp();
        }

        [[nodiscard]] allocator_type get_allocator() const {
            return allocator_;
        }

        template <typename... Args>
        std::pair<iterator, bool> emplace(Args&&... args) {
            key_type key_value = Key(std::forward<Args>(args)...);

            return insert(key_value);
        }

        std::pair<iterator, bool> insert(key_type key_value) {
            return Insert(key_value);
        }

        void insert(iterator it1, iterator it2) {
            for (auto it = it1; it != it2; ++it) {
                insert(*it);
            }
        }

        void insert(const std::initializer_list<key_type>& values_list) {
            for (auto value : values_list) {
                insert(value);
            }
        }

        node_type extract(key_type key_value) {
            return Delete(key_value).second;
        }

        node_type extract(const_iterator node_iter) {
            if (node_iter == cend()) throw std::runtime_error("Attempt to extract end of container");

            return extract(*node_iter);
        }

        size_type erase(key_type key_value) {
            return Delete(key_value).first != end();
        }

        iterator erase(const_iterator node_iter) {
            if (node_iter == cend()) throw std::runtime_error("Attempt to extract end of container");

            return Delete(*node_iter).first;
        }

        iterator erase(iterator node_iter) {
            if (node_iter == end()) throw std::runtime_error("Attempt to extract end of container");

            return Delete(*node_iter).first;
        }

        iterator erase(const_iterator it1, const_iterator it2) {
            key_type it2_value = *it2;
            size_type array_length = std::distance(it1, it2);
            key_type values_to_delete[array_length];

            for (auto it = it1; it != it2; ++it) {
                values_to_delete[std::distance(it1, it)] = *it;
            }

            for (int i = 0; i < array_length; ++i) {
                Delete(values_to_delete[i]);
            }

            return iterator(Search(it2_value), tag_, begin_ptr_, end_ptr_);
        }

        void merge(const BinarySearchTree& other) {
            if (get_allocator() != other.get_allocator()) throw std::runtime_error("Different allocators for merged allocators");

            insert(other.begin(), other.end());
        }

        iterator find(key_type key_value) const {
            return iterator(Search(key_value), tag_, begin_ptr_, end_ptr_);
        }

        size_type count(key_type key_value) const {
            return find(key_value) != end();
        }

        bool contains(key_type key_value) const {
            return count(key_value);
        }

        iterator lower_bound(key_type key_value) const {
            return iterator(LowerBound(key_value), tag_, begin_ptr_, end_ptr_);
        }

        iterator upper_bound(key_type key_value) const {
            return iterator(UpperBound(key_value), tag_, begin_ptr_, end_ptr_);
        }

        void clear() {
            erase(cbegin(), cend());
        }

        void swap(BinarySearchTree& rhs) {
            std::swap(head_root_, rhs.head_root_);
            std::swap(allocator_, rhs.allocator_);
            std::swap(comparator_, rhs.comparator_);
            std::swap(tree_size_, rhs.tree_size_);
            std::swap(tag_, rhs.tag_);
            std::swap(begin_ptr_, rhs.begin_ptr_);
            std::swap(end_ptr_, rhs.end_ptr_);
        }

        friend void swap(BinarySearchTree& lhs, BinarySearchTree& rhs) {
            lhs.swap(rhs);
        }
    private:
        pointer head_root_ = nullptr;
        allocator_type allocator_;
        key_compare comparator_;
        size_type tree_size_ = 0;
        traversal_tag tag_;

        pointer begin_ptr_ = nullptr;
        pointer end_ptr_ = nullptr;

        void DefaultConstructor() {
            end_ptr_ = ConstructNewNode(key_type{});
            begin_ptr_ = end_ptr_;
            tree_size_ = 0;
        }

        void UpdateBeginAndEnd(PreOrderTraversal tag) {
            if (head_root_ == nullptr) {
                end_ptr_->parent = nullptr;
            } else {
                pointer temp_node = head_root_;
                while (temp_node->right != nullptr && temp_node->right != end_ptr_) {
                    temp_node = temp_node->right;
                }
                temp_node->right = end_ptr_;
                end_ptr_->parent = temp_node;
            }

            if (head_root_ == nullptr) {
                begin_ptr_ = end_ptr_;
            } else {
                begin_ptr_ = head_root_;
            }
        }

        void UpdateBeginAndEnd(InOrderTraversal tag) {
            if (head_root_ == nullptr) {
                end_ptr_->parent = nullptr;
            } else {
                pointer temp_node = head_root_;
                while (temp_node->right != nullptr && temp_node->right != end_ptr_) {
                    temp_node = temp_node->right;
                }
                temp_node->right = end_ptr_;
                end_ptr_->parent = temp_node;
            }

            if (head_root_ == nullptr) {
                begin_ptr_ = end_ptr_;
            } else {
                pointer temp_node = head_root_;
                while (temp_node->left != nullptr) {
                    temp_node = temp_node->left;
                }
                begin_ptr_ = temp_node;
            }
        }

        void UpdateBeginAndEnd(PostOrderTraversal) {
            end_ptr_->parent = nullptr;
            if (head_root_ == nullptr) {
                end_ptr_->left = nullptr;
                end_ptr_->right = nullptr;
            } else {
                end_ptr_->left = head_root_;
                end_ptr_->right = head_root_;
                head_root_->parent = end_ptr_;
            }

            if (head_root_ == nullptr) {
                begin_ptr_ = end_ptr_;
            } else {
                pointer temp_node = head_root_;
                while (temp_node->left != nullptr || temp_node->right != nullptr) {
                    if (temp_node->left != nullptr) {
                        temp_node = temp_node->left;
                    } else {
                        temp_node = temp_node->right;
                    }
                }
                begin_ptr_ = temp_node;
            }
        }

        std::pair<iterator, bool> Insert(key_type key_value) {
            pointer inserted_node = Search(key_value);

            if (inserted_node != end_ptr_) return std::make_pair(iterator(inserted_node, tag_, begin_ptr_, end_ptr_), false);

            if (head_root_ == nullptr) {
                head_root_ = ConstructNewNode(key_value);
                inserted_node = head_root_;
            } else {
                pointer temp_root = head_root_;
                while (temp_root != nullptr && temp_root != end_ptr_) {
                    if (comparator_(temp_root->value, key_value)) {
                        if (temp_root->right != nullptr && temp_root->right != end_ptr_) {
                            temp_root = temp_root->right;
                        } else {
                            pointer new_node = ConstructNewNode(key_value);
                            new_node->parent = temp_root;
                            temp_root->right = new_node;
                            inserted_node = new_node;
                            break;
                        }
                    } else {
                        if (temp_root->left != nullptr) {
                            temp_root = temp_root->left;
                        } else {
                            pointer new_node = ConstructNewNode(key_value);
                            new_node->parent = temp_root;
                            temp_root->left = new_node;
                            inserted_node = new_node;
                            break;
                        }
                    }
                }
            }

            UpdateBeginAndEnd(tag_);

            return std::make_pair(iterator(inserted_node, tag_, begin_ptr_, end_ptr_), true);
        }

        std::pair<iterator, node_type> Delete(key_type key_value) {
            pointer delete_node = Search(key_value);

            if (delete_node == end_ptr_) return std::make_pair(end(), node_type{});

            key_type next_traversal_node_value = *(++iterator(delete_node, tag_, begin_ptr_, end_ptr_));

            pointer node_to_destroy = nullptr;

            if (delete_node == head_root_) {
                if (head_root_->right == nullptr || head_root_->right == end_ptr_) {
                    pointer temp_root = head_root_->left;
                    node_to_destroy = head_root_;
                    head_root_ = temp_root;
                    if (head_root_ != nullptr) {
                        head_root_->parent = nullptr;
                    }
                } else {
                    pointer temp_root = Next(head_root_);
                    node_to_destroy = temp_root;
                    std::swap(temp_root->value, head_root_->value);

                    if (temp_root->right == nullptr || temp_root->right == end_ptr_) {
                        if (temp_root->parent->right == temp_root) {
                            temp_root->parent->right = nullptr;
                        } else {
                            temp_root->parent->left = nullptr;
                        }
                    } else {
                        if (temp_root->parent->left == temp_root) {
                            temp_root->parent->left = temp_root->right;
                        } else {
                            temp_root->parent->right = temp_root->right;
                        }
                        temp_root->right->parent = temp_root->parent;
                    }
                }
            } else {
                node_to_destroy = delete_node;
                if (delete_node->left == nullptr && (delete_node->right == nullptr || delete_node->right == end_ptr_)) {
                    if (delete_node->parent->right == delete_node) {
                        delete_node->parent->right = nullptr;
                    } else {
                        delete_node->parent->left = nullptr;
                    }
                } else if (delete_node->left == nullptr || delete_node->right == nullptr || delete_node->right == end_ptr_) {
                    if (delete_node->left == nullptr) {
                        if (delete_node->parent->left == delete_node) {
                            delete_node->parent->left = delete_node->right;
                        } else {
                            delete_node->parent->right = delete_node->right;
                        }
                        delete_node->right->parent = delete_node->parent;
                    } else {
                        if (delete_node->parent->left == delete_node) {
                            delete_node->parent->left = delete_node->left;
                        } else {
                            delete_node->parent->right = delete_node->left;
                        }
                        delete_node->left->parent = delete_node->parent;
                    }
                } else {
                    pointer temp_root = Next(delete_node);
                    node_to_destroy = temp_root;
                    std::swap(temp_root->value, delete_node->value);

                    if (temp_root->parent->left == temp_root) {
                        temp_root->parent->left = temp_root->right;
                        if (temp_root->right != nullptr && temp_root->right != end_ptr_) {
                            temp_root->right->parent = temp_root->parent;
                        }
                    } else {
                        temp_root->parent->right = temp_root->right;
                        if (temp_root->right != nullptr && temp_root->right != end_ptr_) {
                            temp_root->right->parent = temp_root->parent;
                        }
                    }
                }
            }

            node_type node_to_return = node_type(node_to_destroy, allocator_);

            DestroyNode(node_to_destroy);
            UpdateBeginAndEnd(tag_);

            iterator next_to_delete_node_iter = iterator(Search(next_traversal_node_value), tag_, begin_ptr_, end_ptr_);

            return std::make_pair(next_to_delete_node_iter, node_to_return);
        }

        pointer ConstructNewNode(key_type key_value) {
            ++tree_size_;
            pointer new_node = allocator_traits::allocate(allocator_, 1);
            allocator_traits::construct(allocator_, new_node, key_value);

            return new_node;
        }

        void DestroyNode(pointer& current_node) {
            --tree_size_;
            allocator_traits::destroy(allocator_, current_node);
            allocator_traits::deallocate(allocator_, current_node, 1);
            current_node = nullptr;
        }

        pointer CopyTree(pointer root) {
            if (root == nullptr) return nullptr;

            pointer new_node = ConstructNewNode(root->value);
            if (root == end_ptr_) {
                end_ptr_ = new_node;
                --tree_size_;
            }

            if (root->parent == end_ptr_) {
                end_ptr_ = ConstructNewNode(key_type{});
                --tree_size_;
                new_node->parent = end_ptr_;
            }

            new_node->left = CopyTree(root->left);
            new_node->right = CopyTree(root->right);
            if (new_node->left != nullptr) {
                new_node->left->parent = new_node;
            }
            if (new_node->right != nullptr) {
                new_node->right->parent = new_node;
            }

            return new_node;
        }

        void Clear(pointer root) {
            if (root != nullptr && root != end_ptr_) {
                Clear(root->left);
                Clear(root->right);
                DestroyNode(root);
            }
        }

        pointer Search(key_type key_value) const {
            pointer temp_root = head_root_;
            while (temp_root != nullptr && temp_root != end_ptr_) {
                if (temp_root->value == key_value) {
                    break;
                }
                if (comparator_(key_value, temp_root->value)) {
                    temp_root = temp_root->left;
                } else {
                    temp_root = temp_root->right;
                }
            }

            return (temp_root == nullptr) ? end_ptr_ : temp_root;
        }

        pointer Minimum(pointer root) {
            if (root->left == nullptr) return root;

            return Minimum(root->left);
        }

        pointer Maximum(pointer root) {
            if (root->right == nullptr || root->right == end_ptr_) return root;

            return Maximum(root->right);
        }

        pointer Next(pointer root) {
            if (root == head_root_ && (root->right == nullptr || root->right == end_ptr_)) return nullptr;

            if (root->right != nullptr && root->right != end_ptr_) return Minimum(root->right);

            pointer node_parent = root->parent;
            while ((node_parent != nullptr && node_parent != end_ptr_) && root == node_parent->right) {
                root = node_parent;
                node_parent = node_parent->parent;
            }

            return node_parent;
        }

        pointer LowerBound(key_type key_value) const {
            pointer temp_root = head_root_;
            pointer successor = end_ptr_;
            while (temp_root != nullptr && temp_root != end_ptr_) {
                if (!comparator_(temp_root->value, key_value)) {
                    successor = temp_root;
                    temp_root = temp_root->left;
                } else {
                    temp_root = temp_root->right;
                }
            }

            return successor;
        }

        pointer UpperBound(key_type key_value) const {
            pointer temp_root = head_root_;
            pointer successor = end_ptr_;
            while (temp_root != nullptr && temp_root != end_ptr_) {
                if (comparator_(key_value, temp_root->value)) {
                    successor = temp_root;
                    temp_root = temp_root->left;
                } else {
                    temp_root = temp_root->right;
                }
            }

            return successor;
        }

        pointer Previous(pointer root) {
            if (root == head_root_ && root->left == nullptr) return nullptr;

            if (root->left != nullptr) return Maximum(root->left);

            pointer node_parent = root->parent;
            while ((node_parent != nullptr && node_parent != end_ptr_) && root == node_parent->left) {
                root = node_parent;
                node_parent = node_parent->parent;
            }

            return node_parent;
        }
    };
}