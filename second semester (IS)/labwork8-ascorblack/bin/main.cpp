#include <BinarySearchTreeSTL.cpp>

#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace BinarySearchTreeSTL;


template<typename OrderType>
void BidirectOrderTest(BinarySearchTree<int>& binary_tree) {
  auto begin = binary_tree.begin<OrderType>();
  auto rend = binary_tree.rend<OrderType>();
  auto end = binary_tree.end<OrderType>();

  while (begin != end) {
    printf("%d ", *begin);
    ++begin;
  }

  std::cout << '\n';

  auto rbegin = binary_tree.rbegin<OrderType>();

  while (rend != rbegin) {
    printf("%d ", *rbegin);
    ++rbegin;
  }

  printf("\n\n");
}


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



int main(int argc, char** argv) {
  BinarySearchTree<int> binary_tree;
  binary_tree.insert(23);
  binary_tree.insert(-24);
  binary_tree.insert(0);
  binary_tree.insert(1);
  binary_tree.insert(-10);
  binary_tree.insert(-2);
  auto temp = binary_tree.erase(-10);
  binary_tree.insert(21);
  binary_tree.insert(-12);
  binary_tree.insert(-23);
  binary_tree.insert(10);
  binary_tree.insert(40);
  binary_tree.insert(30);
  binary_tree.insert(29);

//  auto end = binary_tree.rend();
//  auto begin = binary_tree.begin();
//
//  std::cout << *begin << '\n';
//  std::cout << *end << '\n';
//
//  --begin;
//  --begin;
//
//  std::set<int> test;


  BinarySearchTree<int> new_tree(binary_tree);

  BidirectOrderTest<OrderType::PreOrder>(new_tree);
  BidirectOrderTest<OrderType::InOrder>(new_tree);
  BidirectOrderTest<OrderType::PostOrder>(new_tree);

  std::cout << (binary_tree == new_tree) << '\n';

  binary_tree.insert(65436);
  binary_tree.insert(24);
  binary_tree.insert(-3245345);
  binary_tree.insert(-234);

  std::cout << (binary_tree == new_tree) << '\n';

  new_tree = binary_tree;

  std::cout << (binary_tree == new_tree) << '\n';

  binary_tree.insert(7546);
  binary_tree.insert(23412);
  binary_tree.insert(-8234);
  binary_tree.insert(-98723);

  std::cout << (binary_tree == new_tree) << "\n\n";

  binary_tree.swap(new_tree);

  BidirectOrderTest<OrderType::PreOrder>(new_tree);
  BidirectOrderTest<OrderType::InOrder>(new_tree);
  BidirectOrderTest<OrderType::PostOrder>(new_tree);

  binary_tree.swap(new_tree);

  BidirectOrderTest<OrderType::PreOrder>(new_tree);
  BidirectOrderTest<OrderType::InOrder>(new_tree);
  BidirectOrderTest<OrderType::PostOrder>(new_tree);

  BinarySearchTree<int> int_binary_tree;
  for (int i = 20; i < 40; ++i) {
    int_binary_tree.insert(i * i / 2);
  }

  BidirectOrderTest<OrderType::PostOrder>(int_binary_tree);

  auto forward_order = ForwardOrder<int, OrderType::PostOrder>(int_binary_tree);
  auto backward_order = BackwardOrder<int, OrderType::PostOrder>(int_binary_tree);

  std::reverse(forward_order.begin(), forward_order.end());

  std::cout << (forward_order == backward_order) << '\n';

  auto _begin = int_binary_tree.begin();
  auto _end = int_binary_tree.end();

  BinarySearchTree<int> bst_from_iters(_begin, _end);

  BidirectOrderTest<OrderType::PostOrder>(bst_from_iters);

  return 0;
}
