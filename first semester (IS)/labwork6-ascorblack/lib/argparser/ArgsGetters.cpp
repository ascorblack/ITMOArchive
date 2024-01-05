#include "ArgParser.h"

using namespace ArgumentParser;


template <typename LT, typename RT>
RT ArgParser::GetValue(const std::string& string_key, int index) const {
  auto value = arguments.find(string_key);

  if (value == arguments.end())
    throw std::runtime_error("Invalid key!");

  auto argValue = std::get_if<LT>(&value->second);

  if (!argValue)
    throw std::runtime_error("Invalid type!");

  if ((*argValue)->GetArgumentStoreType() == ArgStorageType::Multi) {

    if ((*argValue)->GetValue(index).has_value())
      return (*argValue)->GetValue(index).value();

  } else {

    if (argValue && (*argValue)->GetValue().has_value())
      return (*argValue)->GetValue().value();
    else
      throw std::runtime_error("Value not found!");

  }

}

std::string ArgParser::GetStringValue(const std::string& string_key, int index) const {
  return GetValue<StringArgument*, std::string>(string_key, index);
}

std::string ArgParser::GetStringValue(const std::string& string_key) const {
  return GetValue<StringArgument*, std::string>(string_key, 0);
}

int ArgParser::GetIntValue(const std::string& string_key, int index) const {
  return GetValue<IntArgument*, int>(string_key, index);
}

int ArgParser::GetIntValue(const std::string& string_key) const {
  return GetValue<IntArgument*, int>(string_key, 0);
}

bool ArgParser::GetFlag(const std::string& string_key, int index) const {
  return GetValue<FlagArgument*, bool>(string_key, index);
}

bool ArgParser::GetFlag(const std::string& string_key) const {
  return GetValue<FlagArgument*, bool>(string_key, 0);
}
