#include "ArgParser.h"

namespace ArgumentParser {

template <typename T>
T& ArgParser::AddArgument(char short_name, const char* long_name, const char* description) {
  T* new_value = new T();

  if (description != nullptr) {
    new_value->SetDescription(description);
  }

  std::string long_key = std::string{long_name};
  std::string short_key = std::string{&short_name, 1};

  new_value->names[0] = short_key;
  new_value->names[1] = long_key;

  arguments[long_key] = new_value;
  arguments[short_key] = new_value;

  return *new_value;
}

template <typename T>
T& ArgParser::AddArgument(const char* long_name, const char* description) {
  T* new_value = new T();

  if (description != nullptr)
    new_value->SetDescription(description);

  std::string long_key = std::string{long_name};
  new_value->names[1] = long_key;
  arguments[long_key] = new_value;

  return *new_value;
}


// Strings
StringArgument& ArgParser::AddStringArgument(const char* long_name) {
  return AddArgument<StringArgument>(long_name, nullptr);
}

StringArgument& ArgParser::AddStringArgument(char short_name, const char* long_name) {
  return AddArgument<StringArgument>(short_name, long_name, nullptr);
}

StringArgument& ArgParser::AddStringArgument(char short_name, const char* long_name, const char* description) {
  return AddArgument<StringArgument>(short_name, long_name, description);
}

StringArgument& ArgParser::AddStringArgument(const char* long_name, const char* description) {
  return AddArgument<StringArgument>(long_name, description);
}


// Integers
IntArgument& ArgParser::AddIntArgument(const char* long_name) {
  return AddArgument<IntArgument>(long_name, nullptr);
}

IntArgument& ArgParser::AddIntArgument(char short_name, const char* long_name) {
  return AddArgument<IntArgument>(short_name, long_name, nullptr);
}

IntArgument& ArgParser::AddIntArgument(char short_name, const char* long_name, const char* description) {
  return AddArgument<IntArgument>(short_name, long_name, description);
}

IntArgument& ArgParser::AddIntArgument(const char* long_name, const char* description) {
  return AddArgument<IntArgument>(long_name, description);
}


// Flags
FlagArgument& ArgParser::AddFlag(const char* long_name) {
  return AddArgument<FlagArgument>(long_name, nullptr);
}

FlagArgument& ArgParser::AddFlag(char short_name, const char* long_name) {
  return AddArgument<FlagArgument>(short_name, long_name, nullptr);
}

FlagArgument& ArgParser::AddFlag(char short_name, const char* long_name, const char* description) {
  return AddArgument<FlagArgument>(short_name, long_name, description);
}

FlagArgument& ArgParser::AddFlag(const char* long_name, const char* description) {
  return AddArgument<FlagArgument>(long_name, description);
}


}
