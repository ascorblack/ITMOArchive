#include <lib/TTaskScheduler.h>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <iostream>


TEST(TaskSchedulerTester, InitialTest) {
  float a = 1;
  float b = -2;
  float c = 0;

  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

  auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

  auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<float>(id1), 0.0);
  ASSERT_EQ(scheduler.getResult<float>(id2), 4.0);
  ASSERT_EQ(scheduler.getResult<float>(id3), 4.0);
  ASSERT_EQ(scheduler.getResult<float>(id4), 0.0);
  ASSERT_EQ(scheduler.getResult<float>(id5), 2.0);
  ASSERT_EQ(scheduler.getResult<float>(id6), 0.0);
}

float randomFloat(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}


TEST(TaskSchedulerTest, EfficiencyTest) {
  TTaskScheduler scheduler;

  const int numTasks = 100; // Количество задач
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < numTasks; ++i) {
    float a = randomFloat(1.0f, 10.0f);
    float b = randomFloat(-10.0f, 10.0f);
    float c = randomFloat(-10.0f, 10.0f);

    auto taskId = scheduler.add([a, b, c](int status){
      std::this_thread::sleep_for(std::chrono::milliseconds(10)); return status;
    }, 1);
  }

  scheduler.executeAll();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Time taken for executing " << numTasks << " tasks: " << duration.count() << " milliseconds\n";

  ASSERT_LE(duration.count(), 1100);
}

TEST(TaskSchedulerTester, SecondTest) {
  float a = 10;
  float b = -200;
  float c = 2;

  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

  auto id2 = scheduler.add([](float a, float k){return a * a + k;}, b, scheduler.getFutureResult<float>(id1));

  auto id3 = scheduler.add([](float a, float c){return a * c / 2;}, scheduler.getFutureResult<float>(id1), scheduler.getFutureResult<float>(id2));

  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<float>(id1), -80.0);
  ASSERT_EQ(scheduler.getResult<float>(id2), 39920.0);
  ASSERT_EQ(scheduler.getResult<float>(id3), -1596800.0);
}


TEST(TaskSchedulerTester, StringTest) {
  const char* a = "aaaa";
  const char* b = "bbbb";
  const char* c = "cccc";

  TTaskScheduler scheduler;

  auto id1 = scheduler.add([](const char* a, const char* b, const char* c){return std::string(a) + b + c;}, a, b, c);


  scheduler.executeAll();

  ASSERT_EQ(scheduler.getResult<std::string>(id1), "aaaabbbbcccc");
}


TEST(TaskSchedulerTester, MismatchTypeTest) {
  TTaskScheduler scheduler;
  auto id1 = scheduler.add([](float a){return a * a * a;}, 1.0);
  scheduler.executeAll();

  ASSERT_THROW(scheduler.getResult<int>(id1), std::runtime_error);
}

