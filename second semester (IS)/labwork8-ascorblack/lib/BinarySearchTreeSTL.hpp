#pragma once

#include <type_traits>
#include <iostream>
#include <algorithm>

#define BST_METHOD_TEMPLATE template<typename T, typename Compare, typename Alloc>
#define BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::node_pointer
#define BST_NODE_TYPE BinarySearchTree<T, Compare, Alloc>::node_type

#define BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::iterator
#define ITER_NODE_POINTER BST_ITERATOR::node_pointer

namespace OrderType {

  struct Order {
  };

  struct PreOrder : Order {
  };

  struct InOrder : Order {
  };

  struct PostOrder : Order {
  };

}

namespace BinarySearchTreeSTL {

  struct input_iterator_tag {
  };

  struct output_iterator_tag {
  };

  struct forward_iterator_tag : public input_iterator_tag {
  };

  struct bidirectional_iterator_tag : public forward_iterator_tag, public std::input_iterator_tag {
  };

  struct random_access_iterator_tag : public bidirectional_iterator_tag {
  };

  enum STLEnd {
    EndIter,
    Iter
  };

  template<typename T>
  struct TreeNode {
    T value{};
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
    TreeNode* parent{nullptr};

    explicit TreeNode(T value_) : value(value_) {}

    explicit TreeNode() = default;

    ~TreeNode() {
      delete left;
      delete right;
    }

    bool operator==(const TreeNode<T>& rvalue) {
      return this->value == rvalue.value;
    }

  };

  template<
          typename T,
          typename Compare = std::less<T>,
          typename Alloc = std::allocator<TreeNode<T>>
  >
  class BinarySearchTree {
  private:
    typedef Alloc allocator_type;
    typedef T value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    typedef TreeNode<T> node_type;
    typedef node_type* node_pointer;
    typedef node_type& node_reference;
    typedef BinarySearchTree<T, Compare, Alloc> self;

    node_pointer head_{nullptr};

    Compare compare_;
    allocator_type allocator_;

    enum IterOrderType {
      PreOrder,
      InOrder,
      PostOrder
    };

  public:
    const node_pointer& top();

    size_t Size();

    static node_pointer Minimum(node_pointer);

    static node_pointer Maximum(node_pointer);

    static node_pointer RMostMin(node_pointer);

    static node_pointer RMostMax(node_pointer);

    class Iterator : public std::iterator<std::bidirectional_iterator_tag, value_type> {
    public:
      typedef T value_type;
      typedef value_type& reference;
      typedef value_type* pointer;

      typedef Iterator iterator;
      typedef std::ptrdiff_t difference_type;
      typedef bidirectional_iterator_tag iterator_category;

    private:
      typedef TreeNode<value_type> node_type;
      typedef node_type& node_reference;
      typedef node_type* node_pointer;

      node_pointer current_node{nullptr};
      STLEnd iter_type_{STLEnd::Iter};

      node_pointer InOrderNext();

      node_pointer InOrderPrev();

      node_pointer PreOrderNext();

      node_pointer PreOrderPrev();

      node_pointer PostOrderNext();

      node_pointer PostOrderPrev();

      IterOrderType order_type_;

      friend BinarySearchTree;

    public:

      explicit Iterator(node_pointer, node_pointer, IterOrderType = InOrder, STLEnd = STLEnd::Iter);

      iterator operator++();

      iterator operator--();

      bool operator==(const iterator& rvalue);

      bool operator!=(const iterator& rvalue);

      reference operator*() const;

      node_pointer operator->() const;

    };

    typedef Iterator iterator;
    typedef Iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<iterator> const_reverse_iterator;

    template<class Order>
    iterator begin();

    iterator begin();

    template<class Order>
    reverse_iterator rbegin();

    reverse_iterator rbegin();

    template<class Order>
    iterator end();

    iterator end();

    template<class Order>
    reverse_iterator rend();

    reverse_iterator rend();

    std::pair<iterator, bool> find(value_type value);

    bool contains(value_type value);

    bool empty();

    void clear();

    explicit BinarySearchTree() = default;

    BinarySearchTree(BinarySearchTree<T, Compare, Alloc>& rvalue);

    BinarySearchTree(iterator& begin_, iterator& end_, Compare user_compare = std::less<T>());

    BinarySearchTree& operator=(self& rvalue);

    std::pair<iterator, bool> insert(value_type value);

    std::pair<iterator, bool> insert(node_type node);

    void insert(iterator& iter, iterator& end_p);

    std::pair<iterator, bool> erase(value_type value);

    iterator erase(iterator& iter);

    iterator erase(iterator& iter, iterator& end_);

    TreeNode<T> extract(value_type value);

    node_type extract(iterator& iter);

    void merge(self& rvalue);

    iterator upper_bound(value_type value);

    iterator lower_bound(value_type value);

    ~BinarySearchTree() {
      clear();
    };

    bool operator==(self& rvalue);

    bool operator!=(self& rvalue);

    void swap(self& rvalue);

  private:

    node_pointer insert_(node_pointer node, value_type value);

    node_pointer pop_(node_pointer node, value_type value);

    node_pointer find_(BST_NODE_POINTER node, value_type value);

    node_pointer findMinValue(node_pointer node);
  };

}

using namespace BinarySearchTreeSTL;

BST_METHOD_TEMPLATE
BinarySearchTree<T, Compare, Alloc>::BinarySearchTree(BinarySearchTree::iterator& begin_,
                                                      BinarySearchTree::iterator& end_,
                                                      Compare user_compare_) {
  compare_ = user_compare_;

  while (begin_ != end_) {
    this->insert(*begin_);
    ++begin_;
  }
}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::lower_bound(value_type value) {
  auto node_iter = std::upper_bound(begin<OrderType::InOrder>(), end<OrderType::InOrder>(), value);

  --node_iter;
  if (node_iter != end() && *node_iter == value)
    return node_iter;
  else
    ++node_iter;

  return node_iter;
}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::upper_bound(value_type value) {
  return std::upper_bound(begin(), end(), value);
}


BST_METHOD_TEMPLATE
BinarySearchTree<T, Compare, Alloc>::BinarySearchTree(BinarySearchTree<T, Compare, Alloc>& rvalue) {
  Iterator iterator = rvalue.begin<OrderType::PreOrder>();
  Iterator p_end = rvalue.end<OrderType::PreOrder>();

  while (iterator != p_end) {
    this->insert(*iterator);
    ++iterator;
  }

}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::erase(BST_ITERATOR& iter) {
  std::pair<iterator, bool> next_node = find(iter);
  iterator return_node = next_node.second ? ++next_node.first : end();

  pop_(iter);
  return return_node;
}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::erase(BST_ITERATOR& iter, BST_ITERATOR& end_) {
  std::pair<iterator, bool> next_node = find(end_);
  iterator return_node = next_node.second ? ++next_node.first : end();

  while (iter != end_) {
    this->pop_(*iter);
    ++iter;
  }

  return return_node;
}


BST_METHOD_TEMPLATE
std::pair<typename BST_ITERATOR, bool> BinarySearchTree<T, Compare, Alloc>::erase(T value) {
  if (find(value).first == end())
    return {end(), false};

  node_pointer temp_node = pop_(head_, value);

  head_ = temp_node;

  return {iterator(head_, temp_node), true};
}


BST_METHOD_TEMPLATE
BST_NODE_TYPE BinarySearchTree<T, Compare, Alloc>::extract(BST_ITERATOR& iter) {
  return extract(*iter);
}

BST_METHOD_TEMPLATE
TreeNode<T> BinarySearchTree<T, Compare, Alloc>::extract(BinarySearchTree<T, Compare, Alloc>::value_type value) {
  std::pair<iterator, bool> pop_node = erase(value);
  TreeNode<T> return_node = pop_node.second ? *pop_node.first.current_node : TreeNode<T>();

  return return_node;
}


BST_METHOD_TEMPLATE
void BinarySearchTree<T, Compare, Alloc>::merge(BinarySearchTree<T, Compare, Alloc>::self& rvalue) {
  iterator iter = rvalue.template begin<OrderType::PostOrder>();
  iterator end_p = rvalue.template end<OrderType::PostOrder>();

  while (iter != end_p) {
    if (!this->contains(*iter)) {
      auto temp_node = rvalue.extract(iter);
      this->insert(temp_node);
    }

    ++iter;
  }

}


BST_METHOD_TEMPLATE
const BST_NODE_POINTER& BinarySearchTree<T, Compare, Alloc>::top() {
  return head_;
}

BST_METHOD_TEMPLATE
size_t BinarySearchTree<T, Compare, Alloc>::Size() {
  if (empty())
    return static_cast<size_t>(0);

  size_t size = 0;

  auto iter = begin();
  auto end_p = end();

  while (iter != end_p) {
    size++;
    ++iter;
  }

  return size;
}

BST_METHOD_TEMPLATE
template<class Order>
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::end() {

  if (!head_)
    return Iterator(head_, head_);

  if constexpr (std::is_same_v<Order, OrderType::PreOrder>)
    return Iterator(head_, RMostMax(head_), PreOrder, STLEnd::EndIter);

  if constexpr (std::is_same_v<Order, OrderType::InOrder>)
    return Iterator(head_, Maximum(head_), InOrder, STLEnd::EndIter);

  if constexpr (std::is_same_v<Order, OrderType::PostOrder>)
    return Iterator(head_, head_, PostOrder, STLEnd::EndIter);

}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::end() {
  return end < OrderType::InOrder > ();
}

BST_METHOD_TEMPLATE
template<class Order>
BinarySearchTree<T, Compare, Alloc>::reverse_iterator BinarySearchTree<T, Compare, Alloc>::rend() {

  if (!head_) {
    auto iter = reverse_iterator(Iterator(head_, head_));
    return iter;
  } else {
    auto iter = reverse_iterator(begin<Order>());
    ++iter;
    return iter;
  }
}

BST_METHOD_TEMPLATE
BinarySearchTree<T, Compare, Alloc>::reverse_iterator BinarySearchTree<T, Compare, Alloc>::rend() {
  return rend < OrderType::InOrder > ();
}

BST_METHOD_TEMPLATE
template<class Order>
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::begin() {

  if (!head_)
    return Iterator(head_, head_);

  if constexpr (std::is_same_v<Order, OrderType::PreOrder>)
    return Iterator(head_, head_, PreOrder);

  if constexpr (std::is_same_v<Order, OrderType::InOrder>)
    return Iterator(head_, Minimum(head_), InOrder);

  if constexpr (std::is_same_v<Order, OrderType::PostOrder>)
    return Iterator(head_, RMostMin(head_), PostOrder);

}

BST_METHOD_TEMPLATE
BST_ITERATOR BinarySearchTree<T, Compare, Alloc>::begin() {
  return begin < OrderType::InOrder > ();
}

BST_METHOD_TEMPLATE
template<class Order>
BinarySearchTree<T, Compare, Alloc>::reverse_iterator BinarySearchTree<T, Compare, Alloc>::rbegin() {

  if (!head_) {
    auto iter = reverse_iterator(end<Order>());
    return iter;
  } else {
    auto iter = reverse_iterator(end<Order>());
    ++iter;
    return iter;
  }

}

BST_METHOD_TEMPLATE
BinarySearchTree<T, Compare, Alloc>::reverse_iterator BinarySearchTree<T, Compare, Alloc>::rbegin() {
  return rbegin < OrderType::InOrder > ();
}

BST_METHOD_TEMPLATE
std::pair<typename BST_ITERATOR, bool> BinarySearchTree<T, Compare, Alloc>::insert(T value) {
  if (find(value).first != end())
    return {end(), false};

  node_pointer result = insert_(head_, value);

  if (head_ == nullptr)
    head_ = result;

  return {iterator(head_, result), true};
}

BST_METHOD_TEMPLATE
std::pair<typename BST_ITERATOR, bool> BinarySearchTree<T, Compare, Alloc>::insert(node_type node) {
  value_type node_value = node.value;
  allocator_.deallocate(&node, 1);
  return insert(node_value);
}

BST_METHOD_TEMPLATE
void BinarySearchTree<T, Compare, Alloc>::insert(BinarySearchTree::iterator& iter,
                                                 BinarySearchTree::iterator& end_p) {

  while (iter != end_p) {
    this->insert(*iter);
    ++iter;
  }

}

BST_METHOD_TEMPLATE
std::pair<typename BST_ITERATOR, bool> BinarySearchTree<T, Compare, Alloc>::find(T value) {
  auto temp_node = find_(head_, value);
  if (!temp_node)
    return {end(), false};

  return {iterator(head_, temp_node), true};
}

BST_METHOD_TEMPLATE
bool BinarySearchTree<T, Compare, Alloc>::contains(value_type value) {
  return find(value).second;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::insert_(node_pointer node, T value) {
  if (node == nullptr) {
    node_pointer new_node = allocator_.allocate(1);
    std::allocator_traits<allocator_type>::construct(allocator_, new_node, value);
    return new_node;
  }

  if (compare_(value, node->value)) {
    node->left = insert_(node->left, value);
    node->left->parent = node;
  } else {
    node->right = insert_(node->right, value);
    node->right->parent = node;
  }

  return node;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::pop_(node_pointer node, T value) {
  if (node == nullptr)
    return node;

  if (compare_(value, node->value)) {
    node->left = pop_(node->left, value);
    if (node->left)
      node->left->parent = node;
  } else if (!compare_(value, node->value) && value != node->value) {
    node->right = pop_(node->right, value);
    if (node->right)
      node->right->parent = node;
  } else if (node->left != nullptr && node->right != nullptr) {
    node->value = findMinValue(node->right)->value;
    node->right = pop_(node->right, node->value);
    if (node->right)
      node->right->parent = node;
  } else {
    node_pointer temp_node = node;

    if (node->left != nullptr)
      node = node->left;
    else if (node->right != nullptr)
      node = node->right;
    else
      node = nullptr;

    allocator_.deallocate(temp_node, 1);
  }

  return node;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::findMinValue(node_pointer node) {
  BST_NODE_POINTER current = node;

  while (current->left != nullptr)
    current = current->left;

  return current;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::find_(BST_NODE_POINTER node, T value) {
  if (node == nullptr || node->value == value)
    return node;

  if (compare_(value, node->value))
    return find_(node->left, value);

  return find_(node->right, value);
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::Minimum(BST_NODE_POINTER root) {
  if (!root)
    return nullptr;

  BST_NODE_POINTER min_node = root;

  while (min_node->left != nullptr)
    if (min_node->left != nullptr)
      min_node = min_node->left;

  return min_node;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::Maximum(BST_NODE_POINTER root) {
  if (!root)
    return nullptr;

  BST_NODE_POINTER max_node = root;

  while (max_node->right)
    if (max_node->right)
      max_node = max_node->right;

  return max_node;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::RMostMin(BinarySearchTree::node_pointer node) {
  if (!node)
    return nullptr;

  node_pointer temp_node = node;

  while (temp_node->left || temp_node->right) {
    if (temp_node->left)
      temp_node = temp_node->left;
    else if (temp_node->right)
      temp_node = temp_node->right;
  }

  return temp_node;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BinarySearchTree<T, Compare, Alloc>::RMostMax(BinarySearchTree::node_pointer node) {
  if (!node)
    return nullptr;

  node_pointer temp_node = node;

  while (temp_node->left || temp_node->right) {
    if (temp_node->right)
      temp_node = temp_node->right;
    else if (temp_node->left)
      temp_node = temp_node->left;
  }

  return temp_node;
}

BST_METHOD_TEMPLATE
bool BinarySearchTree<T, Compare, Alloc>::operator==(BinarySearchTree::self& rvalue) {
  if (this->Size() != rvalue.Size())
    return false;

  auto l_iter = begin<OrderType::InOrder>();
  auto r_iter = rvalue.template begin<OrderType::InOrder>();

  auto l_end = end<OrderType::InOrder>();

  if (l_iter.current_node->value != r_iter.current_node->value)
    return false;

  while (l_iter != l_end) {
    if (l_iter.current_node->value != r_iter.current_node->value)
      return false;

    ++l_iter;
    ++r_iter;
  }

  return true;
}

BST_METHOD_TEMPLATE
bool BinarySearchTree<T, Compare, Alloc>::operator!=(BinarySearchTree::self& rvalue) {
  return !(this == rvalue);
}

BST_METHOD_TEMPLATE
BinarySearchTree<T, Compare, Alloc>& BinarySearchTree<T, Compare, Alloc>::operator=(BinarySearchTree::self& rvalue) {
  clear();

  Iterator iterator = rvalue.template begin<OrderType::PreOrder>();
  Iterator end = rvalue.template end<OrderType::PreOrder>();

  while (iterator != end) {
    this->insert(static_cast<T>(*iterator));
    ++iterator;
  }

  return *this;
}

BST_METHOD_TEMPLATE
void BinarySearchTree<T, Compare, Alloc>::swap(BinarySearchTree::self& rvalue) {
  self temp_node(*this);
  *this = rvalue;
  rvalue = temp_node;
}

BST_METHOD_TEMPLATE
void BinarySearchTree<T, Compare, Alloc>::clear() {
  if (empty())
    return;

  auto iterator = begin<OrderType::InOrder>();
  auto p_end = end<OrderType::InOrder>();

  T temp_value = *iterator;
  while (iterator != p_end) {
    temp_value = *iterator;
    ++iterator;
    this->erase(temp_value);
  }

  if (!empty())
    throw std::runtime_error("An error occurred while deleting items.");
}

BST_METHOD_TEMPLATE
bool BinarySearchTree<T, Compare, Alloc>::empty() {
  auto iterator = begin<OrderType::InOrder>();
  auto p_end = end<OrderType::InOrder>();

  return iterator == p_end;
}

BST_METHOD_TEMPLATE
BST_ITERATOR::Iterator(ITER_NODE_POINTER head,
                       ITER_NODE_POINTER node,
                       IterOrderType order_type,
                       STLEnd stl_end) : iter_type_ (stl_end), order_type_(order_type) {
  this->current_node = (stl_end == STLEnd::Iter) ? node : node + 1;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::InOrderPrev() {
  TreeNode<T>* predecessor = nullptr;

  if (current_node->left) {
    predecessor = current_node->left;
    while (predecessor->right)
      predecessor = predecessor->right;
  } else {
    TreeNode<T>* parent = current_node->parent;
    while (parent && current_node == parent->left) {
      current_node = parent;
      parent = parent->parent;
    }
    predecessor = parent;
  }

  return predecessor;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::InOrderNext() {
  TreeNode<T>* successor = nullptr;

  if (current_node->right) {
    successor = current_node->right;
    while (successor->left)
      successor = successor->left;
  } else {
    TreeNode<T>* parent = current_node->parent;
    while (parent && current_node == parent->right) {
      current_node = parent;
      parent = parent->parent;
    }
    successor = parent;
  }

  return successor;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::PreOrderNext() {
  if (current_node->left)
    return current_node->left;

  if (current_node->right)
    return current_node->right;

  node_pointer temp_node = current_node;
  node_pointer parent = temp_node->parent;
  while (parent && (parent->right == temp_node || !parent->right)) {
    temp_node = parent;
    parent = parent->parent;
  }

  return parent ? parent->right : nullptr;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::PostOrderNext() {
  node_pointer successor = nullptr;

  if (current_node->parent) {
    if (current_node == current_node->parent->left) {
      if (current_node->parent->right) {
        successor = current_node->parent->right;
        successor = RMostMin(successor);
      } else {
        successor = current_node->parent;
      }
    } else {
      successor = current_node->parent;
    }
  }

  return successor;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::PostOrderPrev() {
  if (current_node->right)
    return current_node->right;

  if (current_node->left)
    return current_node->left;

  node_pointer pred = current_node->parent;
  while (pred && (current_node == pred->left || !pred->left)) {
    current_node = pred;
    pred = pred->parent;
  }

  return pred ? pred->left : nullptr;
}

BST_METHOD_TEMPLATE
ITER_NODE_POINTER BST_ITERATOR::PreOrderPrev() {
  if (!current_node->parent)
    return nullptr;

  if (current_node->parent->right) {
    if (current_node->parent->right == current_node && current_node->parent->left)
      return RMostMax(current_node->parent->left);

    if (current_node->parent->right == current_node && !current_node->parent->left)
      return current_node->parent;
  }

  if (current_node->parent->left && current_node->parent->left == current_node)
    return current_node->parent;

  return nullptr;
}

BST_METHOD_TEMPLATE
BST_ITERATOR::iterator BST_ITERATOR::operator++() {
  iterator tmp_ = *this;
  node_pointer temp_node = current_node;

  if (iter_type_ == STLEnd::EndIter) {
    current_node = current_node - 1;
    iter_type_ = STLEnd::Iter;
    return tmp_;
  }

  if (order_type_ == PreOrder)
    current_node = PreOrderNext();

  if (order_type_ == InOrder)
    current_node = InOrderNext();

  if (order_type_ == PostOrder)
    current_node = PostOrderNext();

  if (!current_node) {
    current_node = temp_node + 1;
    iter_type_ = STLEnd::EndIter;
  }

  return tmp_;
}

BST_METHOD_TEMPLATE
BST_ITERATOR::iterator BST_ITERATOR::operator--() {
  iterator temp_self = *this;
  node_pointer temp_node = current_node;

  if (iter_type_ == STLEnd::EndIter) {
    current_node = current_node - 1;
    iter_type_ = STLEnd::Iter;
    return temp_self;
  }

  if (order_type_ == PreOrder)
    current_node = PreOrderPrev();

  if (order_type_ == InOrder)
    current_node = InOrderPrev();

  if (order_type_ == PostOrder)
    current_node = PostOrderPrev();

  if (!current_node) {
    current_node = temp_node + 1;
    iter_type_ = STLEnd::EndIter;
  }

  return temp_self;
}

BST_METHOD_TEMPLATE
BST_NODE_POINTER BST_ITERATOR::operator->() const {
  return &current_node->value;
}

BST_METHOD_TEMPLATE
bool BST_ITERATOR::operator==(const iterator& rvalue) {
  return this->current_node == rvalue.current_node;
}

BST_METHOD_TEMPLATE
bool BST_ITERATOR::operator!=(const iterator& rvalue) {
  return this->current_node != rvalue.current_node;
}

BST_METHOD_TEMPLATE
BST_ITERATOR::reference BST_ITERATOR::operator*() const {
  return current_node->value;
}
