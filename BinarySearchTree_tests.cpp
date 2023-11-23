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
  cout << tree.size() << endl;  
  ASSERT_TRUE(tree.size() == 3);
  ASSERT_TRUE(tree.height() == 2);
}




TEST_MAIN()
