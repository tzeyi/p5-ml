#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(bst_test_empty) {
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.empty() == true);
  ASSERT_TRUE(tree.size() == 0);
  ASSERT_TRUE(tree.height() == 0);
}

TEST(bst_test_insert_empty) {
  BinarySearchTree<int> tree;

  tree.insert(5);
    
  ASSERT_TRUE(tree.empty() == false);     
  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);
}

TEST(bst_test_insert_not_empty) {
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(6);
  tree.insert(7);
    
  ASSERT_TRUE(tree.empty() == false);  
  ASSERT_TRUE(tree.size() == 3);
  ASSERT_TRUE(tree.height() == 3);
}

TEST(bst_test_insert_not_empty2) {
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(6);
  tree.insert(4);
  tree.insert(2);
  tree.insert(0);
    
  ASSERT_TRUE(tree.empty() == false);  
  ASSERT_TRUE(tree.size() == 5);
  ASSERT_TRUE(tree.height() == 4);
}

TEST(bst_test_min_element){
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(6);
  tree.insert(4);
  tree.insert(2);
  tree.insert(0);

  ASSERT_TRUE(*tree.min_element() == 0);
}

TEST(bst_test_max_element){
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(6);
  tree.insert(4);
  tree.insert(2);
  tree.insert(0);
  tree.insert(10);

  ASSERT_TRUE(*tree.max_element() == 10);
}

TEST(bst_test_check_sorting_invariant){
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant2){
  BinarySearchTree<int> tree;

  tree.insert(1);
  tree.insert(2);
  tree.insert(3);
  tree.insert(4);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant3){
  BinarySearchTree<int> tree;

  tree.insert(1);
  tree.insert(0);

  *tree.begin() = 2;

  ASSERT_FALSE(tree.check_sorting_invariant());
}




TEST_MAIN()
