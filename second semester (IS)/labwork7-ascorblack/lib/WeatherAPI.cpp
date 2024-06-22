#include "WeatherAPI.h"


const char* kNinjaApiUrl = "https://api.api-ninjas.com/v1/city?name=";
const char* kOpenMeteoUrl = "https://api.open-meteo.com/v1/forecast?current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,rain,showers,snowfall,weather_code,cloud_cover,pressure_msl,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m&hourly=temperature_2m,relative_humidity_2m,dew_point_2m,apparent_temperature,precipitation_probability,precipitation,rain,showers,snowfall,snow_depth,weather_code,visibility,wind_speed_10m,wind_direction_10m,wind_gusts_10m,temperature_80m&timezone=Europe%2FMoscow";
const std::unordered_map<int32_t, std::string> kWeatherCodeFullNames = {
        {0,  "Ясное небо"},
        {1,  "В основном ясно"},
        {2,  "Частично облачно"},
        {3,  "Пасмурно"},
        {45, "Туман"},
        {48, "Морозный туман"},
        {51, "Мелкий дождь"},
        {53, "Умеренный дождь"},
        {55, "Интенсивный дождь"},
        {56, "Легкий ледяной дождь"},
        {57, "Сильный ледяной дождь"},
        {61, "Легкий дождь"},
        {63, "Умеренный дождь"},
        {65, "Сильный дождь"},
        {66, "Легкий ледяной дождь"},
        {67, "Сильный ледяной дождь"},
        {71, "Слабый снег"},
        {73, "Умеренный снег"},
        {75, "Сильный снег"},
        {77, "Снежные зерна"},
        {80, "Легкий дождь с прояснениями"},
        {81, "Умеренный дождь с прояснениями"},
        {82, "Сильный дождь с прояснениями"},
        {85, "Легкий снег с прояснениями"},
        {86, "Сильный снег с прояснениями"},
        {95, "Слабая гроза"},
        {96, "Гроза с небольшим градом"},
        {99, "Гроза с сильным градом"}
};


void replace_all(
        std::string& s,
        std::string const& toReplace, // чЕ ПО НЭЙМИГУ
        std::string const& replaceWith
) {
  std::string buf;
  std::size_t pos = 0;
  std::size_t prevPos;

  buf.reserve(s.size());

  while (true) {
    prevPos = pos;
    pos = s.find(toReplace, pos);
    if (pos == std::string::npos)
      break;
    buf.append(s, prevPos, pos - prevPos);
    buf += replaceWith;
    pos += toReplace.size();
  }

  buf.append(s, prevPos, s.size() - prevPos);
  s.swap(buf);
}

DateRange GetDateRange(int32_t count_days) {
  DateRange date_range;

  for (int32_t day = 0; day < count_days; ++day) {
    auto timestamp = date::year_month_day{floor < date::days > (std::chrono::system_clock::now()) + date::days(day)};
    std::stringstream date;
    date << timestamp.year() << '-' << std::setw(2) << std::setfill('0')
         << static_cast<unsigned>(timestamp.month()) << '-' << timestamp.day();
    date_range.dates.emplace_back(date.str());
  }

  date_range.start_date = date_range.dates.at(0);
  date_range.end_date = date_range.dates.at(date_range.dates.size() - 1);

  return date_range;
}


Response<Coordinates> ApiNinja::GetCityCoordinates(std::string city_name) {
  replace_all(city_name, " ", "%20");
  cpr::Response response = cpr::Get(cpr::Url{kNinjaApiUrl + city_name},
                                    cpr::Header{{"X-Api-Key", ninja_api_key}});

  if (response.status_code != 200)
    return Response<Coordinates>{2, "Error while get city coordinates. Maybe no access to internet", {}};

  json data = json::parse(response.text);
  Coordinates coordinates;

  try {
    coordinates.latitude = data[0].at("latitude").get<double>();
    coordinates.longitude = data[0].at("longitude").get<double>();
  } catch (json::exception& e) {
    return Response<Coordinates>{1, "Error while parse response from Ninja Api.", {}};
  }

  return Response<Coordinates>{0, "", coordinates};
}

Response<WeatherInfo> ApiOpenMeteo::GetWeatherInfo(const Coordinates& coordinates) {
  cpr::Response response = cpr::Get(cpr::Url{this->GenerateUrl(coordinates)});

  if (response.status_code != 200)
    return Response<WeatherInfo>{2, "Error while get weather info. Maybe no access to internet.", {}};

  json data = json::parse(response.text);
  WeatherInfo weather_info;

  try {
    weather_info.current.temperature = data["current"].at("temperature_2m").get<float>();
    weather_info.current.time = data["current"].at("time").get<std::string>();
    weather_info.current.relative_humidity = data["current"].at("relative_humidity_2m").get<int32_t>();
    weather_info.current.apparent_temperature = data["current"].at("apparent_temperature").get<float>();
    weather_info.current.precipitation = data["current"].at("precipitation").get<float>();
    weather_info.current.weather_code_fullname = kWeatherCodeFullNames.at(
            data["current"].at("weather_code").get<int32_t>());
    weather_info.current.pressure = data["current"].at("pressure_msl").get<float>();
    weather_info.current.wind_speed = data["current"].at("wind_speed_10m").get<float>();
    weather_info.current.wind_direction = data["current"].at("wind_direction_10m").get<int32_t>();
    weather_info.current.wind_gusts = data["current"].at("wind_gusts_10m").get<float>();

    uint32_t count_results = data["hourly"]["time"].get<std::vector<std::string>>().size();

    for (int i = 0; i < count_results; i += 6) {
      weather_info.hours.emplace_back(
              data["hourly"]["time"][i].get<std::string>(),
              data["hourly"]["temperature_2m"][i].get<float>(),
              data["hourly"]["apparent_temperature"][i].get<float>(),
              data["hourly"]["relative_humidity_2m"][i].get<int32_t>(),
              data["hourly"]["precipitation"][i].get<float>(),
              data["hourly"]["precipitation_probability"][i].get<int32_t>(),
              kWeatherCodeFullNames.at(data["hourly"]["weather_code"][i].get<int32_t>()),
              data["hourly"]["visibility"][i].get<float>(),
              data["hourly"]["wind_speed_10m"][i].get<float>(),
              data["hourly"]["wind_direction_10m"][i].get<int32_t>(),
              data["hourly"]["wind_gusts_10m"][i].get<float>()
      );
    }

  } catch (json::exception& e) {
    return Response<WeatherInfo>{1, "Error while parse response from OpenMeteo.", {}};
  }

  return Response<WeatherInfo>{0, "", weather_info};
}

std::string ApiOpenMeteo::GenerateUrl(const Coordinates& coordinates) {
  std::ostringstream params;
  params << "&latitude=" << coordinates.latitude << "&longitude=" << coordinates.longitude;
  DateRange date_range = GetDateRange(_count_days);
  params << "&start_date=" << date_range.start_date << "&end_date=" << date_range.end_date;

  std::string test = params.str();
  replace_all(test, ",", ".");

  return std::string{kOpenMeteoUrl + test};
}
