project(WeatherForecast)

add_library(ArgParser ArgParser.cpp)

set(SOURCES WeatherApp.cpp
        WeatherDraw.cpp
        WeatherAPI.cpp
        ConfigReader.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC cpr::cpr)
target_link_libraries(${PROJECT_NAME} PUBLIC curses)
target_link_libraries(${PROJECT_NAME}
        PUBLIC ftxui::screen
        PUBLIC ftxui::dom
        PUBLIC ftxui::component
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
