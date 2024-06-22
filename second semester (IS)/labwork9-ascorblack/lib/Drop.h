#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Container>
class DropView {
 public:

  DropView(Container& container, size_t n) : container_(container), n_(n) {}

  template<typename Iterator>
  class IteratorView {
   public:
    IteratorView(Iterator current, Iterator end, size_t n)
        : current_(current), end_(end), remaining_(n) {
      while (remaining_ > 0 && current_ != end_) {
        ++current_;
        --remaining_;
      }
    }

    IteratorView& operator++() {
      ++current_;
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
    return IteratorView(container_.begin(), container_.end(), n_);
  }

  auto end() {
    return IteratorView(container_.end(), container_.end(), 0);
  }

 private:
  Container& container_;
  size_t n_;
};
