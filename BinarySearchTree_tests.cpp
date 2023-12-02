#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(bst_test_empty) {
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.empty() == true);
  ASSERT_TRUE(tree.size() == 0);
  ASSERT_TRUE(tree.height() == 0);
  ASSERT_EQUAL(tree.end(), tree.max_element());
  ASSERT_EQUAL(tree.end(), tree.min_element());
  ASSERT_EQUAL(tree.end(), tree.find(5));
  ASSERT_EQUAL(tree.end(), tree.min_greater_than(5));
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

TEST(bst_test_find){
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(10);
  tree.insert(15);

  ASSERT_TRUE(tree.find(5) != tree.end());
  ASSERT_EQUAL(tree.begin(), tree.find(5));

  *tree.begin() = 2;

  ASSERT_EQUAL(tree.find(5), tree.end());
  ASSERT_EQUAL(tree.begin(), tree.find(2));
}

TEST(bst_test_find_empty){
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.find(5) == tree.end());
  ASSERT_EQUAL(tree.begin(), tree.end());
}

TEST(bst_test_find_not_empty){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(9);

  ASSERT_TRUE(tree.find(6) == tree.end());
}


TEST(bst_test_min_element){
  BinarySearchTree<int> tree;

  tree.insert(6);
  tree.insert(5);
  tree.insert(4);
  tree.insert(1);
  tree.insert(0);

  ASSERT_TRUE(*tree.min_element() == 0);
}

TEST(bst_test_min_element_nullptr){
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.min_element() == tree.end());
}

TEST(bst_test_max_element){
  BinarySearchTree<int> tree;

  tree.insert(10);
  tree.insert(9);
  tree.insert(6);
  tree.insert(5);
  tree.insert(2);
  tree.insert(0);

  ASSERT_TRUE(*tree.max_element() == 10);
}

TEST(bst_test_minmax_element_negativevalues){
  BinarySearchTree<int> tree;

  tree.insert(-5);
  tree.insert(-4);
  tree.insert(-3);
  tree.insert(-2);
  tree.insert(-1);
  tree.insert(0);

  ASSERT_TRUE(*tree.max_element() == 0);
  ASSERT_TRUE(*tree.min_element() == -5);
}

TEST(bst_test_max_element_nullptr){
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.max_element() == tree.end());
}

TEST(bst_test_check_sorting_invariant){
  BinarySearchTree<int> tree;

  tree.insert(3);
  tree.insert(5);
  tree.insert(7);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant_right_heavy){
  BinarySearchTree<int> tree;

  tree.insert(1);
  tree.insert(2);
  tree.insert(3);
  tree.insert(4);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant_left_heavy){
  BinarySearchTree<int> tree;

  tree.insert(4);
  tree.insert(3);
  tree.insert(2);
  tree.insert(1);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant_edge){
  BinarySearchTree<int> tree;

  tree.insert(1);
  tree.insert(0);

  *tree.begin() = 2;

  ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant_edge2){
  BinarySearchTree<int> tree;

  tree.insert(1);
  tree.insert(0);
  tree.insert(5);
  tree.insert(6);

  *tree.begin() = 2;

  ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(bst_test_check_sorting_invariant_negative){
  BinarySearchTree<int> tree;
  tree.insert(50);
  tree.insert(25);
  tree.insert(0);
  tree.insert(-25);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_test_traverse_inorder){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(bst_test_traverse_inorder_empty){
  BinarySearchTree<int> tree;

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  ASSERT_TRUE(oss_inorder.str() == "");
}

TEST(bst_test_traverse_preorder){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");
}

TEST(bst_test_traverse_preorder_empty){
  BinarySearchTree<int> tree;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  ASSERT_TRUE(oss_preorder.str() == "");
}

TEST(bst_test_min_greater_than){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(*tree.min_greater_than(5) == 7);
}

TEST(bst_test_min_greater_than2){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);
  tree.insert(-1);
  tree.insert(0);
  tree.insert(-2);
  tree.insert(-3);

  ASSERT_TRUE(*tree.min_greater_than(-2) == -1);
}

TEST(bst_test_min_greater_than_edge_no_greater){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.min_greater_than(10) == tree.end());
}

TEST(bst_test_min_greater_than_edge_nullptr){
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.min_greater_than(5) == tree.end());
}

TEST(bst_test_copy_constructor){
  BinarySearchTree<int> tree;
  tree.insert(5);
  tree.insert(7);
  tree.insert(3);

  BinarySearchTree<int> new_tree(tree);
  cout << "cout << tree.to_string()" << endl;
  cout << new_tree.to_string() << endl << endl;  

  ASSERT_EQUAL(new_tree.size(), tree.size());
  ASSERT_EQUAL(tree.height(),new_tree.height());

  auto x = tree.begin();
  auto y = new_tree.begin();
  while(x != tree.end() && y != new_tree.end()) {
        ASSERT_EQUAL(*x, *y);
        ASSERT_NOT_EQUAL(x, y);
        ++x, ++y;
  }

}

TEST(bst_test_copy_empty){
  BinarySearchTree<int> tree;
  BinarySearchTree<int> new_tree(tree);
  
  ASSERT_TRUE(new_tree.size()==0);
  ASSERT_TRUE(new_tree.height()==0);
  ASSERT_TRUE(new_tree.empty() == true);

}

TEST(bst_test_breakinginvariant) {
    BinarySearchTree<int> tree;
    tree.insert(50);
    tree.insert(75);
    tree.insert(25);
    ASSERT_TRUE(tree.check_sorting_invariant());
    
}



TEST_MAIN()
