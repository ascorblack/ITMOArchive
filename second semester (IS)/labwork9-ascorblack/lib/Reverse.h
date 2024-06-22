#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Container>
class ReverseView {
 public:

  explicit ReverseView(Container& container) : container_(container) {}

  template<typename Iterator>
  class IteratorView {
   public:
    IteratorView(Iterator current, Iterator end)
        : current_(current), end_(end) {}

    IteratorView& operator++() {
      --current_;
      return *this;
    }

    auto operator*() const { return *current_; }

    bool operator==(const IteratorView& other) const { return current_ == other.current_; }

    bool operator!=(const IteratorView& other) const { return !(*this == other); }

   private:
    Iterator current_;
    Iterator end_;
  };

  auto begin() {
    return IteratorView(container_.end() - 1, container_.begin() - 1);
  }

  auto end() {
    return IteratorView(container_.begin() - 1, container_.begin() - 1);
  }

 private:
  Container& container_;
};

