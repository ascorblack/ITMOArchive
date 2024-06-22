#include <WeatherApp.h>


WeatherApp::WeatherApp(const ArgParser& arg_parser) : status(AppStatus::Running), city_num(0), changed(false) {
  setlocale(LC_ALL, "Russian");
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);

  const char* ninja_api_key = getenv("NINJA_API_KEY");

  if (ninja_api_key == nullptr) {
    status = AppStatus::Error;
    PrintErrorAlert("Ninja api key not found in environment.");
    return;
  }
  config_reader = new ConfigReader(arg_parser.GetStringValue("config"), ninja_api_key);

  PreprocessingConfig();

  start_time = std::chrono::steady_clock::now();
}

WeatherApp::~WeatherApp() { // зАЧЕМ динамическая память
  if (api_open_meteo != nullptr)
    delete api_open_meteo;
  if (config_reader != nullptr)
    delete config_reader;

  endwin();
}

void WeatherApp::PreprocessingConfig() {
  Result<Config> config_parse_result = config_reader->Parse();
  if (config_parse_result.status != 0) {
    status = AppStatus::ReprocessingConfig;
    PrintErrorAlert(config_parse_result.description);
    return;
  }
  status = AppStatus::Running;
  config = config_parse_result.data;
  api_open_meteo = new ApiOpenMeteo(config.forecast_days);
}

void WeatherApp::run() {
  if (status == AppStatus::Error) return;

  UpdateWeather(0);

  while (status == AppStatus::Running || status == AppStatus::ReprocessingConfig || status == AppStatus::Reconnect) {
    unsigned int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start_time).count();

    HandleInput();
    UpdateWeather(timestamp);

    std::this_thread::sleep_for(std::chrono::milliseconds(kRefreshRate));
  }
}

void WeatherApp::HandleInput() {
  int32_t ch = getch();

  switch (ch) {
    case kQButton:
      status = AppStatus::Finish;
      break;
    case kEscButton:
      status = AppStatus::Finish;
      break;
    case kPreviousButton:
      if (city_num > 0) {
        city_num -= 1;
        changed = true;
      }
      break;
    case kNextButton:
      if (!config.cities.empty() && city_num + 1 < config.cities.size()) {
        city_num += 1;
        changed = true;
      }
      break;
  }
}

void WeatherApp::PrintErrorAlert(const std::string& error_description) {
  clear();
  refresh();

  Screen alert = weather_draw.GetErrorAlertScreen(error_description);
  alert.Print();

  std::this_thread::sleep_for(std::chrono::milliseconds(kErrorAlertTime));
}

void WeatherApp::UpdateWeather(unsigned int timestamp) {
  if (status == AppStatus::ReprocessingConfig || config.cities.empty()) {
    PreprocessingConfig();
    return;
  }

  if (timestamp % (config.update_frequency * kMillisecondInSecond) == 0 || changed) {
    clear();

    Response<WeatherInfo> weather_info_response = api_open_meteo->GetWeatherInfo(config.cities[city_num].coordinates);

    switch (weather_info_response.status) {
      case 1:
        status = AppStatus::Error;
        PrintErrorAlert(weather_info_response.description);
        return;
      case 2:
        status = AppStatus::Reconnect;
        PrintErrorAlert(weather_info_response.description);
        return;
      default:
        status = AppStatus::Running;
    }


    Screen screen = weather_draw.GetScreen(weather_info_response.result, config, config.cities[city_num].name);
    refresh();
    screen.Print();

    changed = false;
  }
}
