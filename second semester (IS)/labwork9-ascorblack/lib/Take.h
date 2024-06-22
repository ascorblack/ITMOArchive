#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Container>
class TakeView {
 public:

  TakeView(Container& container, size_t n) : container_(container), n_(n) {}

  template<typename Iterator>
  class IteratorView {
   public:
    IteratorView(Iterator current, Iterator end, size_t n)
        : current_(current), end_(end), remaining_(n) {}

    IteratorView& operator++() {
      ++current_;
      --remaining_;
      return *this;
    }

    auto operator*() const { return *current_; }

    bool operator==(const IteratorView& other) const { return current_ == other.current_; }

    bool operator!=(const IteratorView& other) const { return !(*this == other); }

   private:
    Iterator current_;
    Iterator end_;
    size_t remaining_;
  };

  auto begin() {
    return IteratorView(container_.begin(), container_.begin() + std::min(n_, container_.size()), n_);
  }

  auto end() {
    return IteratorView(container_.begin() + std::min(n_, container_.size()), container_.begin() + container_.size(), 0);
  }

 private:
  Container& container_;
  size_t n_;
};
