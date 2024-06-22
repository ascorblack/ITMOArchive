#include <lib/BinarySearchTreeSTL.cpp>
#include <gtest/gtest.h>
#include <sstream>

#include <vector>

using namespace BinarySearchTreeSTL;

template<typename T, typename OrderType>
std::vector<T> ForwardOrder(BinarySearchTree<T>& binary_tree) {
  auto iter = binary_tree.template begin<OrderType>();
  auto end = binary_tree.template end<OrderType>();
  std::vector<T> result;

  while (iter != end) {
    result.push_back(*iter);
    ++iter;
  }

  return result;
}

template<typename T, typename OrderType>
std::vector<T> BackwardOrder(BinarySearchTree<T>& binary_tree) {
  auto r_iter = binary_tree.template rbegin<OrderType>();
  auto rend = binary_tree.template rend<OrderType>();
  std::vector<T> result;

  while (r_iter != rend) {
    result.push_back(*r_iter);
    ++r_iter;
  }

  return result;
}

TEST(BinarySearchTreeSTLTestSuite, EmptyTest) {
  BinarySearchTree<int> int_binary_tree;
  BinarySearchTree<std::string> string_binary_tree;
  BinarySearchTree<double> double_binary_tree;
  BinarySearchTree<char> char_binary_tree;
}

TEST(BinarySearchTreeSTLTestSuite, InsertSizeTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 0; i < 13; ++i) {
    int_binary_tree.insert(i);
  }

  ASSERT_EQ(int_binary_tree.Size(), 13);

  BinarySearchTree<std::string> string_binary_tree;
  string_binary_tree.insert("test1");
  string_binary_tree.insert("test2");
  string_binary_tree.insert("test3");
  string_binary_tree.insert("test4");
  string_binary_tree.insert("test5");

  ASSERT_EQ(string_binary_tree.Size(), 5);
}

TEST(BinarySearchTreeSTLTestSuite, PopSizeTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 0; i < 13; ++i) {
    int_binary_tree.insert(i);
    if (i % 2 == 0)
      int_binary_tree.erase(i);
  }

  ASSERT_EQ(int_binary_tree.Size(), 6);

  BinarySearchTree<std::string> string_binary_tree;
  string_binary_tree.insert("test1");
  string_binary_tree.insert("test2");
  string_binary_tree.insert("test3");
  string_binary_tree.insert("test4");
  string_binary_tree.insert("test5");

  string_binary_tree.erase("test3");

  ASSERT_EQ(string_binary_tree.Size(), 4);
}


TEST(BinarySearchTreeSTLTestSuite, ClearTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 20; i < 40; ++i) {
    int_binary_tree.insert(i * i / 2);
  }

  ASSERT_EQ(int_binary_tree.empty(), false);

  int_binary_tree.clear();

  ASSERT_EQ(int_binary_tree.empty(), true);
}

TEST(BinarySearchTreeSTLTestSuite, PreOrderTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 20; i < 40; ++i) {
    int_binary_tree.insert(i * i / 2);
  }

  auto forward_order = ForwardOrder<int, OrderType::PreOrder>(int_binary_tree);
  auto backward_order = BackwardOrder<int, OrderType::PreOrder>(int_binary_tree);

  std::reverse(forward_order.begin(), forward_order.end());
  ASSERT_EQ(forward_order, backward_order);
}

TEST(BinarySearchTreeSTLTestSuite, InOrderTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 20; i < 40; ++i) {
    int_binary_tree.insert(i * i / 2);
  }

  auto forward_order = ForwardOrder<int, OrderType::InOrder>(int_binary_tree);
  auto backward_order = BackwardOrder<int, OrderType::InOrder>(int_binary_tree);

  std::reverse(forward_order.begin(), forward_order.end());
  ASSERT_EQ(forward_order, backward_order);
}

TEST(BinarySearchTreeSTLTestSuite, PostOrderTest) {
  BinarySearchTree<int> int_binary_tree;
  for (int i = 20; i < 40; ++i) {
    int_binary_tree.insert(i * i / 2);
  }

  auto forward_order = ForwardOrder<int, OrderType::PostOrder>(int_binary_tree);
  auto backward_order = BackwardOrder<int, OrderType::PostOrder>(int_binary_tree);

  std::reverse(forward_order.begin(), forward_order.end());
  ASSERT_EQ(forward_order, backward_order);
}

TEST(BinarySearchTreeSTLTestSuite, FindTest) {
  // Создание и заполнение дерева
  BinarySearchTree<int> int_binary_tree;
  for (int i = 0; i < 10; ++i) {
    int_binary_tree.insert(i * i);
  }

  // Проверка поиска существующего элемента
  auto found = int_binary_tree.find(25);
  ASSERT_TRUE(found.second); // Проверка наличия элемента
  ASSERT_EQ(*found.first, 25); // Проверка значения найденного элемента

  // Проверка поиска несуществующего элемента
  auto not_found = int_binary_tree.find(7);
  ASSERT_FALSE(not_found.second); // Проверка отсутствия элемента
}

TEST(BinarySearchTreeSTLTestSuite, ContainsTest) {
  // Создание и заполнение дерева
  BinarySearchTree<int> int_binary_tree;
  for (int i = 0; i < 10; ++i) {
    int_binary_tree.insert(i * i);
  }

  // Проверка наличия существующего элемента
  ASSERT_TRUE(int_binary_tree.contains(16));

  // Проверка отсутствия несуществующего элемента
  ASSERT_FALSE(int_binary_tree.contains(21));
}

TEST(BinarySearchTreeSTLTestSuite, UpperLowerBoundTest) {
  // Создание и заполнение дерева
  BinarySearchTree<int> int_binary_tree;
  for (int i = 0; i < 10; ++i) {
    int_binary_tree.insert(i * i);
  }
  int_binary_tree.insert(20);

  // Проверка upper_bound
  auto upper = int_binary_tree.upper_bound(20);
  ASSERT_EQ(*upper, 25); // Ожидается элемент со значением 25

  // Проверка lower_bound
  auto lower = int_binary_tree.lower_bound(20);
  ASSERT_EQ(*lower, 20); // Ожидается элемент со значением 16
}
