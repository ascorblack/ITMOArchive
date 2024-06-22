#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Container>
class KeysView {
 public:

  explicit KeysView(Container& container) : container_(container) {}

  template<typename Iterator>
  class IteratorView {
   public:
    explicit IteratorView(Iterator current) : current_(current) {}

    IteratorView& operator++() {
      ++current_;
      return *this;
    }

    auto operator*() const { return current_->first; }

    bool operator==(const IteratorView& other) const { return current_ == other.current_; }

    bool operator!=(const IteratorView& other) const { return !(*this == other); }

   private:
    Iterator current_;
  };

  auto begin() {
    return IteratorView(container_.begin());
  }

  auto end() {
    return IteratorView(container_.end());
  }

 private:
  Container& container_;
};
