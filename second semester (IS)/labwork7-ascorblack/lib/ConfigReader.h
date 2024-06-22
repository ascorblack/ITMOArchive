#pragma once

#include <WeatherAPI.h>

#include <nlohmann/json.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

using json = nlohmann::json;

struct City {
  std::string name;
  Coordinates coordinates;
};

struct Config {
  std::vector<City> cities{};
  int update_frequency{0};
  int forecast_days{0};
};


class ConfigReader {
private:
  std::string config_path{};
  ApiNinja* api_ninja{};

  void _initialization();

public:

  explicit ConfigReader(const std::string& path_to_json, const char* ninja_api_key) { // Почему не в cpp?????
    this->config_path = path_to_json;
    this->api_ninja = new ApiNinja(ninja_api_key);

    this->_initialization();
  }

  explicit ConfigReader(const char* path_to_json, const char* ninja_api_key) {
    this->config_path = std::string{path_to_json};
    this->api_ninja = new ApiNinja(ninja_api_key);

    this->_initialization();
  }

  ~ConfigReader() {
    delete api_ninja;
  }; // кОНСТРУКТОР КОПИРОВАНИЯ ПРИСВАИВАНИЯ?


  Result<Config> Parse();

};

