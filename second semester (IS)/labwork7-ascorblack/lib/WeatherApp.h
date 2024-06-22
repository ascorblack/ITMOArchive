#pragma once

#include <WeatherDraw.h>
#include <ArgParser.cpp>

#include <iostream>
#include <curses.h>


const int8_t kQButton = 113;
const int8_t kEscButton = 27;
const int8_t kPreviousButton = 112;
const int8_t kNextButton = 110;

const int32_t kMillisecondInSecond = 1000;
const int32_t kRefreshRate = 1;
const int32_t kErrorAlertTime = 3000;


enum AppStatus {
  Running,
  Finish,
  Error,
  Reconnect,
  ReprocessingConfig
};


class WeatherApp {
private:
  Config config;
  WeatherDraw weather_draw;
  ConfigReader* config_reader{nullptr}; // Для чего использовать динамическую память
  ApiOpenMeteo* api_open_meteo{nullptr};

  std::chrono::steady_clock::time_point start_time;
  AppStatus status;


  int city_num{};
  bool changed{};

public:
  explicit WeatherApp(const ArgParser&);

  ~WeatherApp();

  void run();

private:
  void HandleInput();

  void UpdateWeather(unsigned int timestamp);

  void PrintErrorAlert(const std::string& error_description);

  void PreprocessingConfig();
};

