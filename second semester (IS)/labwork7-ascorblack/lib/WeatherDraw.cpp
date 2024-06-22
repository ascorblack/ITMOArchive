#include <WeatherDraw.h>


std::string WeatherDraw::FormatFloat(float value) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(1) << value;
  return ss.str();
}

ftxui::Decorator GetTemperatureColor(float temperature) {
  if (temperature < 0.0) return color(Color::Cyan);
  else if (temperature > 25.0) return color(Color::Red);
  else if (temperature > 15.0) return color(Color::Orange1);
  else if (temperature < 10.0) return color(Color::LightSkyBlue1);
  else return color(Color::Yellow);
}

std::string GetWindDirection(int32_t wind_direction_deg) {
  std::map<int, std::string> direction_map = {
          {0, "↓"},
          {1, "⬋"},
          {2, "←"},
          {3, "⬉"},
          {4, "↑"},
          {5, "⬈"},
          {6, "→"},
          {7, "⬊"}
  };

  int index = (wind_direction_deg + 22) / 45 % 8;
  return direction_map[index];
}

Element WeatherDraw::CreateDayTableRow(const CurrentWeather& weather) {
  std::string temperature = FormatFloat(weather.temperature);
  std::string apparent_temperature = FormatFloat(weather.apparent_temperature);
  std::string wind_speed = FormatFloat(weather.wind_speed);
  std::string precipitation = FormatFloat(weather.precipitation);


  if (temperature.at(0) != '-') temperature = "+" + temperature;

  return vbox(
          {
                  text(weather.weather_code_fullname) | flex,
                  hbox({
                               text(temperature) | GetTemperatureColor(weather.temperature),
                               text(" ("),
                               text(apparent_temperature) | GetTemperatureColor(weather.apparent_temperature),
                               text(") °C")
                       }) | flex,
                  hbox({
                               text(GetWindDirection(weather.wind_direction) + " "),
                               text(wind_speed + " м/с"),
                       }) | flex,
                  text(precipitation + " мм") | flex
          }
  );
}


Element WeatherDraw::CreateDayTableRow(const HourlyWeather& weather) {
  std::string temperature = FormatFloat(weather.temperature);
  std::string apparent_temperature = FormatFloat(weather.apparent_temperature);
  std::string wind_speed = FormatFloat(weather.wind_speed);
  std::string precipitation = FormatFloat(weather.precipitation);
  std::string precipitation_probability = FormatFloat(weather.precipitation_probability);


  if (temperature.at(0) != '-') temperature = "+" + temperature;

  return vbox(
          {
                  text(weather.weather_code_fullname) | flex,
                  hbox({
                               text(temperature) | GetTemperatureColor(weather.temperature),
                               text(" ("),
                               text(apparent_temperature) | GetTemperatureColor(weather.apparent_temperature),
                               text(") °C")
                       }) | flex,
                  hbox({
                               text(GetWindDirection(weather.wind_direction) + " "),
                               text(wind_speed + " м/с"),
                       }) | flex,
                  text(precipitation + " мм | " + precipitation_probability + "%") | flex
          }
  );
}

Screen WeatherDraw::GetScreen(const WeatherInfo& weather_info, const Config& config, const std::string& city_name) {
  std::vector<ftxui::Element> table_data = {};

  table_data.emplace_back(center(bold(text(city_name) | color(Color::Cyan))));

  table_data.emplace_back(separator());
  table_data.emplace_back(text("Текущая погода:") | bold);
  table_data.emplace_back(CreateDayTableRow(weather_info.current) | flex);
  table_data.emplace_back(separator());

  uint32_t weather_index = 0;
  for (int day_num = 0; day_num < config.forecast_days; day_num++) {
//    std::vector<Element> day_weather{};
//
//    for (int i = 0; i < 4; ++i) {
//      HourlyWeather weather = weather_info.hours.at(weather_index);
//      day_weather.emplace_back(CreateDayTableRow(weather));
//      weather_index++;
//    }

    table_data.emplace_back(
            vbox({
                         center(
                                 text(GetDateRange(config.forecast_days).dates[day_num]) | bold | color(Color::Green)),
                         hbox({
                                      vbox({
                                                   center(text("Утро") | flex),
                                                   separator(),
                                                   CreateDayTableRow(weather_info.hours.at(weather_index++)) | flex |
                                                   border
                                           }) | flex,
                                      vbox({
                                                   center(text("День") | flex),
                                                   separator(),
                                                   CreateDayTableRow(weather_info.hours.at(weather_index++)) | flex |
                                                   border
                                           }) | flex,
                                      vbox({
                                                   center(text("Вечер") | flex),
                                                   separator(),
                                                   CreateDayTableRow(weather_info.hours.at(weather_index++)) | flex |
                                                   border
                                           }) | flex,
                                      vbox({
                                                   center(text("Ночь") | flex),
                                                   separator(),
                                                   CreateDayTableRow(weather_info.hours.at(weather_index++)) | flex |
                                                   border
                                           }) | flex
                              }) | border
                 }) | border | flex
    );

  }

  auto table = vbox(table_data);

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(table));
  Render(screen, table);

  return screen;
}

Screen WeatherDraw::GetErrorAlertScreen(const std::string& error_description) {
  auto alert = vbox(text(error_description) | bold | center) | flex | border | color(Color::Red);
  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(alert));
  Render(screen, alert);

  return screen;
}
