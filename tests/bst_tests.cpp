#include "gtest/gtest.h"
#include <bst.h>

TEST(ConstructorsTestSuite, DefaultConstructor_PreOrderTraversal) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst;

    ASSERT_TRUE(bst.empty() && bst.size() == 0 && bst.begin() == bst.end() && bst.cbegin() == bst.cend());
}
TEST(ConstructorsTestSuite, DefaultConstructor_InOrderTraversal) {
    BST::BinarySearchTree<std::string, BST::InOrderTraversal> bst;

    ASSERT_TRUE(bst.empty() && bst.size() == 0 && bst.begin() == bst.end() && bst.cbegin() == bst.cend());
}

TEST(ConstructorsTestSuite, DefaultConstructor_PostOrderTraversal) {
    BST::BinarySearchTree<char, BST::PostOrderTraversal> bst;

    ASSERT_TRUE(bst.empty() && bst.size() == 0 && bst.begin() == bst.end() && bst.cbegin() == bst.cend());
}

TEST(ConstructorsTestSuite, CopyConstructor_PreOrderTraversal) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst;
    bst.insert({10, 2, 1, 3, 11, 5, 100, 99, 101, 4});
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst_copy(bst);

    ASSERT_EQ(bst, bst_copy);
}

TEST(ConstructorsTestSuite, CopyConstructor_InOrderTraversal) {
    BST::BinarySearchTree<char, BST::InOrderTraversal> bst;
    bst.insert({'a', '\n', '1', 'm', '#', '*'});
    BST::BinarySearchTree<char, BST::InOrderTraversal> bst_copy(bst);

    ASSERT_EQ(bst, bst_copy);
}

TEST(ConstructorsTestSuite, CopyConstructor_PostOrderTraversal) {
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst;
    bst.insert({"test", "name", "12345678910", "#", "adasd", "\n"});
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst_copy(bst);

    ASSERT_EQ(bst, bst_copy);
}

TEST(ConstructorsTestSuite, InitializerListConstructor_PreOrderTraversal) {
    BST::BinarySearchTree<std::vector<double>, BST::PreOrderTraversal> bst({{1, 2.33, 3}, {10, -11.0001, 31.88}, {-1}, {23, 31.1}});
    std::vector<std::vector<double>> correct_traversal = {{1, 2.33, 3}, {-1}, {10, -11.0001, 31.88}, {23, 31.1}};

    ASSERT_EQ(bst.TraversalToVector(), correct_traversal);
}

TEST(ConstructorsTestSuite, InitializerListConstructor_InOrderTraversal) {
    BST::BinarySearchTree<long long, BST::InOrderTraversal> bst = {300, 11, 2, -1, -5, 20, 21, 15};
    std::vector<long long> correct_traversal = {-5, -1, 2, 11, 15, 20, 21, 300};

    ASSERT_EQ(bst.TraversalToVector(),  correct_traversal);
}

TEST(ConstructorsTestSuite, InitializerListConstructor_PostOrderTraversal) {
    BST::BinarySearchTree<char, BST::PostOrderTraversal> bst = {'a', 'c', 'b', 'd'};
    std::vector<char> correct_traversal = {'b', 'd', 'c', 'a'};

    ASSERT_EQ(bst.TraversalToVector(), correct_traversal);
}

TEST(ConstructorsTestSuite, IteratorsRangeConstructor_PreOrderTraversal) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {2, 3, 1};
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst_new(bst.begin(), bst.end());

    ASSERT_EQ(bst, bst_new);
}

TEST(ConstructorsTestSuite, IteratorsRangeConstructor_InOrderTraversal) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {2, 3, 1, 4};
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst_new(++bst.begin(), bst.end());

    ASSERT_FALSE(bst == bst_new);
}

TEST(ConstructorsTestSuite, IteratorsRangeConstructor_PostOrderTraversal) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {2, 3, 1, 4};
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst_new(++bst.begin(), --bst.end());

    ASSERT_FALSE(bst == bst_new);
}

TEST(OperatorsTestSuite, EqualOperator_EmptyContainers) {
    BST::BinarySearchTree<char, BST::PreOrderTraversal> bst_1, bst_2;

    ASSERT_EQ(bst_1, bst_2);
}

TEST(OperatorsTestSuite, EqualOperator_True) {
    BST::BinarySearchTree<std::string, BST::InOrderTraversal> bst_1 = {"1", "aboasd", "!"}, bst_2 = {"1", "!", "aboasd"};

    ASSERT_EQ(bst_1, bst_2);
}

TEST(OperatorsTestSuite, EqualOperator_False) {
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst_1 = {"1", "aboasd", "!"}, bst_2 = {"1", "!!", "aboasd"};

    ASSERT_FALSE(bst_1 == bst_2);
}

TEST(OperatorsTestSuite, AssignmentOperator_EmptyContainers) {
    BST::BinarySearchTree<char, BST::PreOrderTraversal> bst_1, bst_2;
    bst_1 = bst_2;

    ASSERT_EQ(bst_1, bst_2);
}

TEST(OperatorsTestSuite, AssignmentOperator_True) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst_1 = {2, 23, 1, -4, 10}, bst_2;
    bst_2 = bst_1;
    bst_1.erase(23);
    std::vector<int> correct_traversal = {-4, 1, 2, 10, 23};

    ASSERT_TRUE(bst_2.TraversalToVector() == correct_traversal && bst_1 != bst_2);
}

TEST(OperatorsTestSuite, AssignmentOperator_False) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst_1 = {2, 23, 1, -4, 10}, bst_2;
    bst_1 = bst_2;
    std::vector<int> correct_traversal = {-4, 1, 10, 23, 2};

    ASSERT_FALSE(bst_1.TraversalToVector() == correct_traversal);
}

TEST(IteratorsTestSuite, NonConstIterators) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {3, 1, 5, 4, 100, -9};
    std::vector<int> traversal = std::vector<int>(bst.begin(), bst.end());
    std::vector<int> reverse_traversal = std::vector<int>(bst.rbegin(), bst.rend());
    std::reverse(traversal.begin(), traversal.end());

    ASSERT_EQ(traversal, reverse_traversal);
}

TEST(IteratorsTestSuite, ConstIterators) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {3, 1, 5, 4, 100, -9};
    std::vector<int> traversal = std::vector<int>(bst.cbegin(), bst.cend());
    std::vector<int> reverse_traversal = std::vector<int>(bst.crbegin(), bst.crend());
    std::reverse(traversal.begin(), traversal.end());

    ASSERT_EQ(traversal, reverse_traversal);
}

TEST(IteratorsTestSuite, OutOfRangeBegin) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {1, 2, 3};

    ASSERT_THROW(--bst.begin(), std::out_of_range);
}

TEST(IteratorsTestSuite, OutOfRangeEnd) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {1, 2, 3, -1, 6, 5};

    ASSERT_THROW(++bst.end(), std::out_of_range);
}

TEST(MethodsTestSuite, EmplaceNonExistentElement) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {1, 2};
    auto result = bst.emplace(3);

    ASSERT_TRUE(result.second && bst.find(3) == result.first);
}

TEST(MethodsTestSuite, EmplaceExistingElement) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {1, 2};
    auto result = bst.emplace(2);

    ASSERT_FALSE(result.second);
}

TEST(MethodsTestSuite, InsertByKey) {
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst = {"true", "test"};
    auto result = bst.insert("new_string");

    ASSERT_TRUE(result.second && bst.find("new_string") == result.first);
}

TEST(MethodsTestSuite, DoubleInsert) {
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst = {"true", "test"};
    bst.insert("new_string");

    ASSERT_EQ(bst.count("new_string"), 1);
}

TEST(MethodsTestSuite, InsertByIterators) {
    BST::BinarySearchTree<char, BST::InOrderTraversal> bst_1 = {'d', 'c', 'b', 'a'}, bst_2;
    bst_2.insert(++bst_1.begin(), bst_1.end());
    std::vector<char> correct_traversal = {'b', 'c', 'd'};

    ASSERT_EQ(bst_2.TraversalToVector(), correct_traversal);
}

TEST(MethodsTestSuite, InsertByInitializerList) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst;
    bst.insert({2, 3, 1});
    std::vector<int> correct_traversal = {2, 1, 3};

    ASSERT_EQ(bst.TraversalToVector(), correct_traversal);
}

TEST(MethodsTestSuite, ExtractByKeyExistingElement) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {3, 1, 2, 6, 4};
    auto element = bst.extract(2);

    ASSERT_TRUE(element.value() == 2 && bst.find(2) == bst.end());
}

TEST(MethodsTestSuite, ExtractByKeyNonExistentElement) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {3, 1, 2, 6, 4};
    auto element = bst.extract(5);

    ASSERT_TRUE(element.empty());
}

TEST(MethodsTestSuite, ExtractByIteratorValid) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {3, 1, 2, 6, 4};
    auto element = bst.extract(++bst.cbegin());

    ASSERT_TRUE(element.value() == 1 && bst.find(1) == bst.end());
}

TEST(MethodsTestSuite, ExtractByIteratorInvalid) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {3, 1, 2, 6, 4};

    ASSERT_THROW(bst.extract(bst.cend()), std::runtime_error);
}

TEST(MethodsTestSuite, EraseByKeyExistingElement) {
    BST::BinarySearchTree<char, BST::InOrderTraversal> bst = {'\0', '@', 'a'};

    ASSERT_EQ(bst.erase('a'), 1);
}

TEST(MethodsTestSuite, EraseByKeyNonExistentElement) {
    BST::BinarySearchTree<char, BST::PreOrderTraversal> bst = {'\0', '@', 'a'};

    ASSERT_EQ(bst.erase('b'), 0);
}

TEST(MethodsTestSuite, EraseByIterator) {
    BST::BinarySearchTree<char, BST::PostOrderTraversal> bst = {'\0', '@', 'a', 'c'};
    auto it_1 = bst.erase(bst.begin());
    auto it_2 = bst.erase(--bst.cend());

    ASSERT_TRUE(it_1 != bst.end() && it_2 == bst.end() && bst.size() == 2);
}

TEST(MethodsTestSuite, EraseByIteratorsRange) {
    BST::BinarySearchTree<char, BST::InOrderTraversal> bst = {'\0', '@', 'a', 'c', 'w'};
    auto it = bst.erase(bst.cbegin(), bst.cend());

    ASSERT_TRUE(it == bst.end() && bst.empty());
}

TEST(MethodsTestSuite, MergeTest) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst_1 = {1, 3, 5, 7, 9}, bst_2 = {2, 4, 6, 8, 10};
    bst_1.merge(bst_2);
    std::vector<int> correct_traversal = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    ASSERT_EQ(bst_1.TraversalToVector(), correct_traversal);
}

TEST(MethodsTestSuite, FindTestExistingElement) {
    BST::BinarySearchTree<std::string, BST::PreOrderTraversal> bst = {"here", "yes"};

    ASSERT_EQ(*bst.find("here"), "here");
}

TEST(MethodsTestSuite, FindTestNonExistentElement) {
    BST::BinarySearchTree<std::string, BST::PostOrderTraversal> bst = {"here", "yes"};

    ASSERT_TRUE(bst.find("no") == bst.end());
}

TEST(MethodsTestSuite, CountTestExistingElement) {
    BST::BinarySearchTree<double, BST::InOrderTraversal> bst = {2.3, -1.1, 100};

    ASSERT_EQ(bst.count(-1.1), 1);
}

TEST(MethodsTestSuite, CountTestNonExistentElement) {
    BST::BinarySearchTree<double, BST::PreOrderTraversal> bst = {2.3, -1.1, 100};

    ASSERT_EQ(bst.count(10231203), 0);
}

TEST(MethodsTestSuite, ContainsTestExistingElement) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {1, 2, 3};

    ASSERT_TRUE(bst.contains(2));
}

TEST(MethodsTestSuite, ContainsTestNonExistentElement) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {1, 2, 3};

    ASSERT_FALSE(bst.contains(6));
}

TEST(MethodsTestSuite, LowerBoundFoundEqual) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {1, 2, 3, 4, 5};

    ASSERT_EQ(*bst.lower_bound(3), 3);
}

TEST(MethodsTestSuite, LowerBoundFoundNotEqual) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {1, 2, 4, 5};

    ASSERT_EQ(*bst.lower_bound(3), 4);
}

TEST(MethodsTestSuite, LowerBoundNotFound) {
    BST::BinarySearchTree<int, BST::InOrderTraversal> bst = {1, 2, 3, 4, 5};

    ASSERT_TRUE(bst.lower_bound(6) == bst.end());
}

TEST(MethodsTestSuite, UpperBoundFound) {
    BST::BinarySearchTree<int, BST::PreOrderTraversal> bst = {1, 2, 4, 5};

    ASSERT_EQ(*bst.upper_bound(3), 4);
}

TEST(MethodsTestSuite, UpperBoundNotFoundWithEqual) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst = {1, 2, 3, 4, 5};

    ASSERT_TRUE(bst.upper_bound(5) == bst.end());
}

TEST(MethodsTestSuite, ClearTestWithEmptyContainer) {
    BST::BinarySearchTree<std::set<char>, BST::InOrderTraversal> bst;

    ASSERT_NO_THROW(bst.clear());
}

TEST(MethodsTestSuite, ClearTestDefault) {
    BST::BinarySearchTree<char, BST::PreOrderTraversal> bst = {'a', 'b', 'c'};
    bst.clear();

    ASSERT_TRUE(bst.empty() && bst.size() == 0 && bst.begin() == bst.end());
}

TEST(MethodsTestSuite, SwapTestWithEmptyContainers) {
    BST::BinarySearchTree<int, BST::PostOrderTraversal> bst_1, bst_2;

    ASSERT_NO_THROW(bst_1.swap(bst_2));
}

TEST(MethodsTestSuite, SwapTest) {
    BST::BinarySearchTree<std::string, BST::InOrderTraversal> bst_1 = {"first", "second"}, bst_2 = {"third"};
    swap(bst_1, bst_2);
    std::vector<std::string> correct_traversal_1 = {"first", "second"}, correct_traversal_2 = {"third"};

    ASSERT_TRUE(bst_1.TraversalToVector() == correct_traversal_2 && bst_2.TraversalToVector() == correct_traversal_1);
}