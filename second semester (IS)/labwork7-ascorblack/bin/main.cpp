#include <WeatherApp.h>


int main(int argc, char** argv) {
  ArgParser arg_parser = ArgParser("Погода");
  arg_parser.AddStringArgument('c',
                               "config",
                               "Путь к json файлу с конфигурацией")
          .Default("config.json");
  arg_parser.AddHelp('h', "help", "Описание работы утилиты \"Погода\"");
  arg_parser.Parse(argc, argv);

  WeatherApp app(arg_parser);
  app.run();

  return 0;
}
