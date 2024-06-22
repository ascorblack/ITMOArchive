#pragma once

#include "forward.h"

template<typename ResultType, typename... Args>
class FunctionWrapper {
 public:
  virtual ~FunctionWrapper() = default;
  virtual ResultType execute(Args&&... args) = 0;
};

template<typename Func, typename ResultType, typename... Args>
class FunctionHolder : public FunctionWrapper<ResultType, Args...> {
 public:
  explicit FunctionHolder(Func func) : func_(func) {};

  ResultType execute(Args&&... args) {
    return ResultType((func_(forward<Args>(args)...)));
  }

 private:
  Func func_;

};

template<typename Empty>
class Function;

template<typename ResultType, typename... Args>
class Function<ResultType(Args...)> {
 public:

  template<typename Func>
  explicit Function(Func func) {
    function_wrapper_ = new FunctionHolder<Func, ResultType, Args...>(func);
  };

  ResultType operator()(Args&... args) {
    return function_wrapper_->execute(forward<Args>(args)...);
  }

 private:
  FunctionWrapper<ResultType, Args...>* function_wrapper_;

};

