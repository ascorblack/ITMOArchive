#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "Task.h"
#include "FutureResult.h"

class TTaskScheduler {
 public:

  template<typename Func, typename... Args>
  Task<Func, Args...>& add(Func func, Args... args) {
    auto* new_task = new Task<Func, Args...>(func, args...);
    tasks.emplace_back(dynamic_cast<TaskWrapper*>(new_task));

    return *new_task;
  }

  template<typename Type>
  Type getResult(TaskWrapper& task_wrapper) {
    task_wrapper.calcResult();
    return static_cast<Type>(task_wrapper.getResult().get<Type>());
  }

  template<typename Type>
  FutureResult<Type> getFutureResult(TaskWrapper& task_wrapper) {
    return FutureResult<Type>(task_wrapper);
  }

  void executeAll() {
    for (auto task : tasks) {
      task->calcResult();
    }
  }

  ~TTaskScheduler() {
    for (auto task : tasks) {
      delete task;
    }
  }

 private:
  std::vector<TaskWrapper*> tasks;
};


