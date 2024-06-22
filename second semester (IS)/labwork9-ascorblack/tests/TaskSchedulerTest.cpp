#include <lib/TTaskScheduler.h>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <iostream>


TEST(AdapterTester, FilterTest) {
  std::vector<int> array = {1, 2, 3, 4, 5, 6};
  auto filter_test = array | filter([](int i) { return i % 2; });


}

