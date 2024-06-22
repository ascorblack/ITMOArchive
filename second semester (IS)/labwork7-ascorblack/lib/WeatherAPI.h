#pragma once

#include <cpr/cpr.h>
#include <src/date.h>
#include <nlohmann/json.hpp>

#include <cstring>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <chrono>

using json = nlohmann::json;


struct DateRange {
  std::string start_date;
  std::string end_date;
  std::vector<std::string> dates;

  ~DateRange() = default;
};

struct Coordinates {
  double latitude{};
  double longitude{};

  ~Coordinates() = default;
};

struct CurrentWeather {
  std::string time;
  std::string weather_code_fullname;
  float temperature;
  float apparent_temperature;
  float precipitation;
  int32_t cloud_cover;
  float pressure;
  float wind_speed;
  int32_t wind_direction;
  float wind_gusts;
  int32_t relative_humidity;

  ~CurrentWeather() = default;
};

struct HourlyWeather {
  std::string time;
  float temperature;
  float apparent_temperature;
  int32_t relative_humidity;
  float precipitation;
  int32_t precipitation_probability;
  std::string weather_code_fullname;
  float visibility;
  float wind_speed;
  int32_t wind_direction;
  float wind_gusts;

  ~HourlyWeather() = default; // zacHEM?
};

struct WeatherInfo {
  CurrentWeather current{};
  std::vector<HourlyWeather> hours{};

  ~WeatherInfo() = default;
};

template<typename T>
struct Response {
  int8_t status{};
  std::string description{};
  T result;

  ~Response() = default;
};

template<typename T>
struct Result {
  int8_t status{};
  std::string description{};
  T data;

  ~Result() = default;
};

class ApiNinja {
private:
  const char* ninja_api_key{};

public:

  explicit ApiNinja(const char* ninja_api_key) {
    this->ninja_api_key = ninja_api_key;
  };

  ~ApiNinja() = default;

  Response<Coordinates> GetCityCoordinates(std::string city_name);

};

class ApiOpenMeteo {
private:
  int32_t _count_days;

  std::string GenerateUrl(const Coordinates& coordinates);

public:

  explicit ApiOpenMeteo(int32_t count_days) : _count_days(count_days) {};

  ~ApiOpenMeteo() = default;

  Response<WeatherInfo> GetWeatherInfo(const Coordinates& coordinates);

};

DateRange GetDateRange(int32_t);
