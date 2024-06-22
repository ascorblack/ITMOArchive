#include "ConfigReader.h"


void ConfigReader::_initialization() {
  std::fstream file_reader(this->config_path);

  if (!file_reader.is_open()) {
    std::ofstream file_writer(this->config_path);

    file_writer << std::setw(4) << json{
            {"cities",           {"Saint Petersburg", "Moscow"}},
            {"update_frequency", 1},
            {"forecast_days",    3}
    } << std::endl;

    file_writer.close();
  }

  file_reader.close();
}

Result<Config> ConfigReader::Parse() {

  std::ifstream file(this->config_path);

  if (!file.is_open())
    return Result<Config>{1, "Failed while opening config json file.", {{}, 1, 1}};
      // кАЖЕТСЯ EXCEPTION БЫЛО БЫ КИДАТЬ ЛУЧШИМ ВЫБОРОМ ЧЕМ СОЗДАВАТЬ ОТЕДЛЬНУЮ СТРУКТУРУ ДЛЯ ЭТОГО И УСЛОЖНЯТЬ КОД
  json data = json::parse(file);

  Config config;

  try {
    std::vector<std::string> cities = data.at("cities").get<std::vector<std::string>>(); // МОЖЕТ СКАСТИТЬСЯ САМО
    for (std::string& city_name: cities) {
      Response<Coordinates> coordinates_response = api_ninja->GetCityCoordinates(city_name);

      if (coordinates_response.status != 0)
        return Result<Config>{coordinates_response.status, coordinates_response.description, {{}, 1, 1}};

      City city = City{city_name, coordinates_response.result};
      config.cities.emplace_back(city);
    }

    config.update_frequency = data.at("update_frequency").get<int>();
    config.forecast_days = data.at("forecast_days").get<int>();

    if (config.forecast_days > 3) config.forecast_days = 3;
  } catch (const json::exception& e) {
    return Result<Config>{1, "Incorrect config format!", {{}, 1, 1}};
    // Лучше Exception
  }

  file.close();

  return Result<Config>{0, "", config};
}
