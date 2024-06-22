#pragma once

template<typename ResultType>
class FutureResult {
 public:

  explicit FutureResult(TaskWrapper& task) {
    task_wrapper_ = &task;
  }

  void calcResult() {
    result = static_cast<ResultType>(task_wrapper_->getResult().get<ResultType>());
  }

  operator ResultType&&() {
    calcResult();
    return static_cast<ResultType&&>(result);
  }

 private:
  ResultType result{};
  TaskWrapper* task_wrapper_;

};

