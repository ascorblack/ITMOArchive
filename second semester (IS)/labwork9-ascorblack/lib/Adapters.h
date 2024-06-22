#include <iostream>
#include <vector>
#include <algorithm>

#include <Filter.h>
#include <Transform.h>
#include <Take.h>
#include <Drop.h>
#include <Reverse.h>
#include <Keys.h>
#include <Values.h>

struct Filter {};
struct Transform {};
struct Take {};
struct Drop {};
struct Reverse {};
struct Keys {};
struct Values {};

template<typename Predicate, typename OperationType>
struct OperationFactory {
  Predicate predicate_;
};

template<typename Predicate>
OperationFactory<Predicate, Filter> filter(Predicate&& predicate) {
  return OperationFactory<Predicate, Filter>{predicate};
}

template<typename Predicate>
OperationFactory<Predicate, Transform> transform(Predicate&& predicate) {
  return OperationFactory<Predicate, Transform>{predicate};
}

OperationFactory<size_t, Take> take(size_t predicate) {
  return OperationFactory<size_t, Take>{predicate};
}

OperationFactory<size_t, Drop> drop(size_t predicate) {
  return OperationFactory<size_t, Drop>{predicate};
}

OperationFactory<bool, Reverse> reverse() {
  return OperationFactory<bool, Reverse>{true};
}

OperationFactory<bool, Keys> keys() {
  return OperationFactory<bool, Keys>{true};
}

OperationFactory<bool, Values> values() {
  return OperationFactory<bool, Values>{true};
}

template<typename Container, typename Predicate, typename OperationType>
auto operator|(Container&& container, OperationFactory<Predicate, OperationType> operation) {

  if constexpr (std::is_same_v<OperationType, Transform>) {
    return TransformView(container, operation.predicate_);
  }
  else if constexpr (std::is_same_v<OperationType, Filter>) {
    return FilterView(container, operation.predicate_);
  }
  else if constexpr (std::is_same_v<OperationType, Take>) {
    return TakeView(container, operation.predicate_);
  }
  else if constexpr (std::is_same_v<OperationType, Drop>) {
    return DropView(container, operation.predicate_);
  }
  else if constexpr (std::is_same_v<OperationType, Reverse>) {
    return ReverseView(container);
  }
  else if constexpr (std::is_same_v<OperationType, Keys>) {
    return KeysView(container);
  }
  else if constexpr (std::is_same_v<OperationType, Values>) {
    return ValuesView(container);
  }

}



