#include <lib/Adapters.cpp>

#include <iostream>
#include <vector>
#include <map>

int main() {
  std::vector<int> array = {1, 2, 3, 4, 5, 6};
  auto filter_test = array | filter([](int i) { return i % 2; });
  auto transform_test = array | transform([](int i) { return i * i; });
  auto take_test = array | take(3);
  auto drop_test = array | drop(5);
  auto reverse_test = array | reverse();

  std::map<int, long long> associative_container;
  associative_container[432] = 3253246;
  associative_container[6453] = 89654;
  associative_container[845] = 12347675;

  auto keys_test = associative_container | keys();
  auto values_test = associative_container | values();

  for (auto i : keys_test | transform([](int i) {return i * i; })) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::vector<int> v = {1,2,3,4,5,6};

  for(int i : v | filter([](int i){return i % 2;}) | transform([](int i){return i * i;}))
    std::cout << i << " ";

  std::cout << std::endl;

  return 0;
}
