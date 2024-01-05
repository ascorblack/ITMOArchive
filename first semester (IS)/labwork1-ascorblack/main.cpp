#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

#define String std::string
#define ULL unsigned long long
#define SLL signed long long

const char kNullCharacter = '\0';

struct UserArguments {
  uint64_t count_lines = UINT64_MAX;
  char delimiter = '\n';
  bool tail = false;
  char* file_path{};
};

// Вспомогательные функции
ULL GetStringLength(const char* string) {
  ULL len = 0;

  while (string[len] != kNullCharacter)
    ++len;

  return len;
}

static void ExitWithMessage(const String& message) {
  std::cout << "Упс, что-то пошло не так:\n" << message << "\n";
  exit(-1);
}

static bool IsFile(const char* path) {
  std::ifstream file(path);
  bool file_is_good = file.good();
  file.close();

  return file_is_good;
}

// Функции для обработки аргументов пользователя
ULL StringToNumber(const char* string, const int shift = 0) {

  if (GetStringLength(string) < shift)
    ExitWithMessage(String("Неверно передан аргумент `") + string + String("` в число."));

  char* p_end;
  ULL number = std::strtoull(&string[shift], &p_end, 10);

  if (*p_end == kNullCharacter)
    return number;
  else
    ExitWithMessage(String("Не удалось конвертировать строку `") + &string[shift] + String("` в число."));

}

bool StringStartsWith(const char* string, const char* prefix) {

  for (int index = 0; prefix[index] != kNullCharacter; index++)
    if (string[index] == kNullCharacter || string[index] != prefix[index]) return false;

  return true;
}

char ParseDelimiter(const char* delimiter, const int start_index) {
  ULL delimiter_length = GetStringLength(delimiter);

  if (delimiter_length <= start_index)
    ExitWithMessage(String("Для аргумента `") + delimiter + String("` не найдено значение."));

  if ((delimiter_length - start_index) == 1) {

    return static_cast<char> (delimiter[delimiter_length - 1]);

  } else if (delimiter_length - start_index == 2 && delimiter[delimiter_length - 2] == '\\') {

    switch (delimiter[delimiter_length - 1]) {
      case 'n': return '\n';
      case 't': return '\t';
      case '0': return '\0';
      case 'a': return '\a';
      case 'b': return '\b';
      case 'f': return '\f';
      case 'r': return '\r';
      case 'v': return '\v';
      case '"': return '\"';
      case '\'': return '\'';
      case '\\': return '\"';
      case '\?': return '\?';
      default: break;
    }

  }

  ExitWithMessage(String("Указано невалидное значение для аргумента delimiter `") + delimiter + String("`."));
}

void ParseArgs(UserArguments* user_arguments, int args_count, char** argv) {
  int i = 1;
  ULL argument_length;

  while (i < args_count) {
    argument_length = GetStringLength(argv[i]);

    if (argv[i][0] == '-' && argv[i][1] == '-') {

      if (StringStartsWith(argv[i], "--lines=")) {
        user_arguments->count_lines = StringToNumber(argv[i], 8);
      } else if (argument_length == 6 && StringStartsWith(argv[i], "--tail")) {
        user_arguments->tail = true;
      } else if (StringStartsWith(argv[i], "--delimiter=")) {
        user_arguments->delimiter = ParseDelimiter(argv[i], 12);
      } else
        ExitWithMessage(String("Неизвестный аргумент `") + argv[i] + String("`."));

    } else if (argv[i][0] == '-' && argument_length == 2) {

      if (argv[i][1] == 't') {
        user_arguments->tail = true;
        ++i;
        continue;
      }

      if (i + 1 >= args_count)
        ExitWithMessage(String("Не найдено значение для аргумента `") + argv[i] + String("`."));

      switch (argv[i][1]) {
        case 'l':++i;
          user_arguments->count_lines = StringToNumber(argv[i]);
          break;
        case 't':break;
        case 'd':++i;
          user_arguments->delimiter = ParseDelimiter(argv[i], 0);
          break;
        default:ExitWithMessage(String("Неизвестный аргумент `") + argv[i] + String("`."));
      }

    } else if (IsFile(argv[i])) {
      user_arguments->file_path = argv[i];
    } else {
      ExitWithMessage(String("Файл по пути `") + argv[i] + String("` не найден."));
    }

    ++i;
  }
}

// Функции для вывода файла
void PrintFile(std::ifstream* file, SLL index = 0) {
  char symbol;

  if (index == 0) {

    while (file->get(symbol)) {
      std::cout << symbol;
    }

  } else {

    while (index <= -1) {
      file->seekg(index, std::ifstream::end);
      file->get(symbol);
      std::cout << symbol;
      ++index;
    }

  }
}

void PrintTailFile(UserArguments* user_arguments, SLL index, ULL count_delimiter, char symbol, std::ifstream* file) {
  file->seekg(index, std::ifstream::end);

  while (file->get(symbol)) {

    if (symbol == user_arguments->delimiter) {
      ++count_delimiter;

      if (count_delimiter >= user_arguments->count_lines) break;

    }

    --index;
    file->seekg(index, std::ifstream::end);
  }

  if (count_delimiter < user_arguments->count_lines) {
    file->close();
    file->open(user_arguments->file_path, std::ifstream::binary);
    PrintFile(file, 0);
  } else {
    PrintFile(file, index);
  }

  file->close();
}

void PrintHeadFile(UserArguments* user_arguments, SLL index, ULL count_delimiter, char symbol, std::ifstream* file) {

  while (file->get(symbol)) {
    std::cout << symbol;

    if (symbol == user_arguments->delimiter) {
      ++count_delimiter;

      if (count_delimiter >= user_arguments->count_lines) {
        break;
      }

    }

    ++index;
  }

  file->close();
}

int main(int args_count, char** argv) {
  setlocale(LC_ALL, "Russian");

  UserArguments user_arguments;
  ParseArgs(&user_arguments, args_count, argv);

  std::ifstream input_file(user_arguments.file_path, std::ifstream::binary);
  SLL index = 0;
  ULL count_delimiter = 0;
  char symbol;

  if (user_arguments.tail)
    PrintTailFile(&user_arguments, index - 1, count_delimiter, symbol, &input_file);
  else
    PrintHeadFile(&user_arguments, index, count_delimiter, symbol, &input_file);

}
