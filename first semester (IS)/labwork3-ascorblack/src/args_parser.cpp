#include <cstring>
#include <iostream>
#include <charconv>

#include "args_parser.h"

#define String std::string

const int32_t kInputArgLen = 8;
const int32_t kOutputArgLen = 9;


static void ExitWithMessage(const String& message) {
  std::cout << "Упс, что-то пошло не так:\n" << message << "\n";
  exit(-1);
}

bool ArgsParser::StringStartsWith(const char* string, const char* prefix) {
  return std::strncmp(string, prefix, std::strlen(prefix)) == 0;
}

int ArgsParser::StringToNumber(const char* string, const int shift) {
  if (std::strlen(string) < shift)
    ExitWithMessage(String("Неверно передан аргумент `") + string + String("` в число."));

  const char* startPtr = &string[shift];
  const char* endPtr = startPtr + std::strlen(startPtr);

  int number;
  auto result = std::from_chars(startPtr, endPtr, number, 10);

  if (result.ec == std::errc() && result.ptr == endPtr)
    return number;
  else
    ExitWithMessage(String("Не удалось конвертировать строку `") + startPtr + String("` в число."));
}

Options ArgsParser::GetUserArgs() {
  return options;
}

void ArgsParser::Parse() {
  int8_t i = 1;
  std::size_t argument_length;

  while (i < argc) {
    argument_length = std::strlen(argv[i]);

    if (argv[i][0] == '-' && argv[i][1] == '-') {

      if (StringStartsWith(argv[i], "--max-iter=")) {
        options.max_iter = StringToNumber(argv[i], 11);
      } else if (StringStartsWith(argv[i], "--freq=")) {
        options.frequency = StringToNumber(argv[i], 7);
      } else if (StringStartsWith(argv[i], "--input=")) {
        options.input_filepath = argv[i] + kInputArgLen;
      } else if (StringStartsWith(argv[i], "--output=")) {
        options.output_dir = argv[i] + kOutputArgLen;
      } else if (StringStartsWith(argv[i], "--print")) {
        options.print_console = true;
      } else
        ExitWithMessage(String("Неизвестный аргумент `") + argv[i] + String("`."));

    } else if (argv[i][0] == '-' && argument_length == 2) {

      if (i + 1 >= argc)
        ExitWithMessage(String("Не найдено значение для аргумента `") + argv[i] + String("`."));

      switch (argv[i][1]) {
        case 'm':
          ++i;
          options.max_iter = StringToNumber(argv[i]);
          break;
        case 'f':
          ++i;
          options.frequency = StringToNumber(argv[i]);
          break;
        case 'i':
          ++i;
          options.input_filepath = argv[i];
          break;
        case 'o':
          ++i;
          options.output_dir = argv[i];
          break;
        case 'p':
          options.print_console = true;
          break;
        default:
          ExitWithMessage(String("Неизвестный аргумент `") + argv[i] + String("`."));
      }

    } else {
      ExitWithMessage(String("Файл по пути `") + argv[i] + String("` не найден."));
    }

    ++i;
  }
}

ArgsParser::ArgsParser(int32_t argc, char** argv): argc(argc), argv(argv) {}
