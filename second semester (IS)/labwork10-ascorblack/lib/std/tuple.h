#pragma once

#include "forward.h"

template<typename... Types>
class tuple;

template<typename Head, typename... Tail>
class tuple<Head, Tail...> {
 public:
  explicit tuple(Head head, Tail... tail) : head_(head), tail_(tail...) {}

  template<size_t Index>
  auto& get() {
    if constexpr (Index == 0) {
      return head_;
    } else {
      return tail_.template get<Index - 1>();
    }
  }

 private:
  Head head_;
  tuple<Tail...> tail_;
};

template<>
class tuple<> {
};

template<typename... Args>
static tuple<Args...> make_tuple(Args&& ... args) {
  return tuple<Args...>(forward<Args>(args)...);
}

template<typename Func, typename Tuple, size_t... Indices>
decltype(auto) apply_impl(Func&& func, Tuple&& tuple, std::index_sequence<Indices...>);

template<typename Func, typename... Args>
decltype(auto) apply(Func&& func, tuple<Args...>&& tup) {
  return apply_impl(forward<Func>(func), tup, std::index_sequence_for<Args...>{});
}

template<typename Func, typename Tuple, size_t... Indices>
decltype(auto) apply_impl(Func&& func, Tuple&& tuple, std::index_sequence<Indices...>) {
  return forward<Func>(func)(forward<Tuple>(tuple).template get<Indices>()...);
}