#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Container, typename Transformer>
class TransformView {
 public:

  TransformView(Container& container, Transformer transformer) : container_(container), transformer_(transformer) {}

  template<typename Iterator>
  class IteratorView {
   public:
    IteratorView(Iterator current, Iterator end, Transformer transformer)
        : current_(current), end_(end), transformer_(transformer) {}

    IteratorView& operator++() {
      ++current_;
      return *this;
    }

    auto operator*() const { return transformer_(*current_); }

    bool operator==(const IteratorView& other) const { return current_ == other.current_; }

    bool operator!=(const IteratorView& other) const { return !(*this == other); }

   private:
    Iterator current_;
    Iterator end_;
    Transformer transformer_;
  };

  auto begin() {
    return IteratorView(container_.begin(), container_.end(), transformer_);
  }

  auto end() {
    return IteratorView(container_.end(), container_.end(), transformer_);
  }

 private:
  Container& container_;
  Transformer transformer_;
};
