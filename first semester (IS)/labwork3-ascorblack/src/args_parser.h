#pragma once

#include <cstdint>

const int kMaxPath = 4096;
const char kNullCharacter = '\0';

struct Options {
  const char* input_filepath{};
  const char* output_dir{};
  int32_t max_iter{};
  int32_t frequency = 0;
  bool print_console = false;
};


class ArgsParser {
 public:
  explicit ArgsParser(int argc, char** argv);

  Options GetUserArgs();
  void Parse();

 private:
  int32_t argc;
  char** argv;
  Options options{};

  static bool StringStartsWith(const char* string, const char* prefix);
  static int StringToNumber(const char* string, int shift = 0);

};
