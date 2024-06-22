#pragma once

#include <ConfigReader.h>

#include <iostream>
#include <vector>
#include <string>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"

using namespace ftxui;


class WeatherDraw {
private:
  std::string FormatFloat(float value);

  Element CreateDayTableRow(const HourlyWeather& weather);

public:

  Screen GetScreen(const WeatherInfo& weather_info,
                   const Config& config,
                   const std::string& city_name);

  Element CreateDayTableRow(const CurrentWeather& weather);

  Screen GetErrorAlertScreen(const std::string& error_description);
};


