#pragma once

#include <iostream>
#include <vector>
#include <algorithm>


template<typename Container, typename Predicate> requires std::invocable<Predicate, typename Container::value_type>
class FilterView {
 public:

  FilterView(Container& container, Predicate predicate) : container_(container), predicate_(predicate) {}

  template<typename Iterator>
  class IteratorView {
   public:
    IteratorView(Iterator current, Iterator end, Predicate predicate)
        : current_(current), end_(end), predicate_(predicate) {
      while (current_ != end_ && !predicate_(*current_)) {
        ++current_;
      }
    }

    IteratorView& operator++() {
      ++current_;
      while (current_ != end_ && !predicate_(*current_)) {
        ++current_;
      }
      return *this;
    }

    auto operator*() const { return *current_; }

    bool operator==(const IteratorView& other) const { return current_ == other.current_; }

    bool operator!=(const IteratorView& other) const { return !(*this == other); }

   private:
    Iterator current_;
    Iterator end_;
    Predicate predicate_;
  };

  auto begin() {
    return IteratorView(container_.begin(), container_.end(), predicate_);
  }

  auto end() {
    return IteratorView(container_.end(), container_.end(), predicate_);
  }

 private:
  Container& container_;
  Predicate predicate_;
};
