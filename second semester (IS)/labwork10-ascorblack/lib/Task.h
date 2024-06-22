#pragma once

#include "std/any.h"
#include "std/Function.h"
#include "std/tuple.h"

class TaskWrapper {
 public:
  virtual ~TaskWrapper() = default;
  virtual void calcResult() = 0;
  virtual Any getResult() = 0;
};

template<typename Func, typename... Args>
class Task : public TaskWrapper {
 public:

  explicit Task(Func func, Args... args) : function_(func), args(make_tuple(forward<Args>(args)...)) {}

  void calcResult() override {
    result = apply(function_, forward(args));
  }

  Any getResult() override {
    if (!result)
      calcResult();

    return result;
  }

 private:
  tuple<Args...> args;
  Function<Any(Args...)> function_;
  Any result{};

};

