#include "bst.h"

int main() {
    BST::BinarySearchTree<std::vector<int>, BST::InOrderTraversal> bst;
    bst.insert({1,2,3});
}