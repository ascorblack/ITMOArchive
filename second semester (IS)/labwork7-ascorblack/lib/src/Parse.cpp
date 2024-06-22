#include <charconv>

#include "ArgParser.h"

using namespace ArgumentParser;

int kLongArgShift = 2;
int kShortArgShift = 3;


int StringToNumber(std::string_view string, const int shift = 0) {
  if (string.length() < shift)
    throw std::runtime_error("Неверно передан аргумент");

  const char* start_ptr = &string[shift];
  const char* end_ptr = start_ptr + std::strlen(start_ptr);

  int number;
  auto result = std::from_chars(start_ptr, end_ptr, number, 10);

  if (result.ec == std::errc() && result.ptr == end_ptr)
    return number;
  else
    throw std::runtime_error("Не удалось конвертировать строку в число.");

}

bool ArgParser::Parse(int argc, char **argv) {
  std::vector<std::string> args{argv, argv + argc};
  return Parse(args);
}

bool ArgParser::Parse(const std::vector<std::string>& args) {
  FillPositionalStack();

  for (int index = 1; index < args.size(); ++index) {
    std::string_view arg_view{args[index]};
    ArgumentVariant arguments_variant;

    if (CompareWithHelpKeys(args[index])) {
      HelpDescription();
      return true;
    }

    if (arg_view.starts_with("--")) {
      ParseLongArgument(arg_view, arguments_variant, index, args);
    } else if (arg_view.starts_with("-")) {
      ParseShortArgument(arg_view, arguments_variant, index, args);
    } else {
      ParsePositionalArgument(args, index);
    }

  }

  return ArgsIsCorrect();
}

void ArgParser::ParseLongArgument(std::string_view arg_view, ArgumentVariant& arguments_variant, int& index, const std::vector<std::string>& args) {
  int start_value_index = arg_view.find('=');
  std::string_view arg_name = arg_view.substr(kLongArgShift, start_value_index - kLongArgShift);

  GetArgument(arg_name, arguments_variant);

  if (start_value_index != std::string_view::npos) {
    ParseLongArgumentWithValue(arg_view, arguments_variant, start_value_index);
  } else {
    ParseLongArgumentWithoutValue(arguments_variant, index, args);
  }
}

void ArgParser::ParseLongArgumentWithValue(std::string_view arg_view, ArgumentVariant& arguments_variant, int start_value_index) {
  if (arguments_variant.arg_type == ArgType::String) {
    arguments_variant.string_argument->AddValue(std::string{arg_view.substr(start_value_index + 1)});
  } else if (arguments_variant.arg_type == ArgType::Int) {
    arguments_variant.int_argument->AddValue(StringToNumber(arg_view, start_value_index + 1));
  }
}

void ArgParser::ParseLongArgumentWithoutValue(ArgumentVariant& arguments_variant, int& index, const std::vector<std::string>& args) {
  if (arguments_variant.arg_type == ArgType::Flag) {
    arguments_variant.flag_argument->AddValue(true);
  } else {
    ParseMultiValueArgument(arguments_variant, index, args);
  }
}

void ArgParser::ParseShortArgument(std::string_view arg_view, ArgumentVariant& arguments_variant, int& index, const std::vector<std::string>& args) {
  std::string_view arg_name = arg_view.substr(1, 1);

  GetArgument(arg_name, arguments_variant);

  int start_value_index = arg_view.find('=');

  if (start_value_index != std::string_view::npos) {
    ParseShortArgumentWithValue(arg_view, arguments_variant);
  } else {
    ParseShortArgumentWithoutValue(arg_view, arguments_variant, index, args);
  }
}

void ArgParser::ParseShortArgumentWithValue(std::string_view arg_view, ArgumentVariant& arguments_variant) {
  if (arguments_variant.arg_type == ArgType::String) {
    arguments_variant.string_argument->AddValue(std::string{arg_view.substr(kShortArgShift)});
  } else if (arguments_variant.arg_type == ArgType::Int) {
    arguments_variant.int_argument->AddValue(StringToNumber(arg_view, kShortArgShift));
  }
}

void ArgParser::ParseShortArgumentWithoutValue(std::string_view arg_view, ArgumentVariant& arguments_variant, int& index, const std::vector<std::string>& args) {
  if (arguments_variant.arg_type == ArgType::Flag) {
    for (int i = 2; i <= arg_view.size() && arguments_variant.arg_type == ArgType::Flag; ++i) {
      arguments_variant.flag_argument->AddValue(true);
      GetArgument(arg_view.substr(i, 1), arguments_variant);
    }
  } else {
    ParseMultiValueArgument(arguments_variant, index, args);
  }
}

void ArgParser::ParseMultiValueArgument(ArgumentVariant& arguments_variant, int& index, const std::vector<std::string>& args) {
  if (arguments_variant.arg_type == ArgType::String && arguments_variant.string_argument->GetArgumentStoreType() == ArgStorageType::Multi) {
    if (index + 1 >= args.size()) {
      throw std::overflow_error("Not found positional argument values!");
    }

    for (int i = index + 1; i < args.size(); ++i) {
      index = i;
    }
  } else if (arguments_variant.arg_type == ArgType::Int && arguments_variant.int_argument->GetArgumentStoreType() == ArgStorageType::Multi) {
    if (index + 1 >= args.size()) {
      throw std::overflow_error("Not found positional argument values!");
    }

    for (int i = index + 1; i < args.size(); ++i) {
      index = i;
      if (args[i].starts_with('-')) {
        break;
      }
      arguments_variant.int_argument->AddValue(StringToNumber(args[i]));
    }
  }
}

void ArgParser::ParsePositionalArgument(const std::vector<std::string>& args, int& index) {

  if (positional_stack.empty())
    throw std::runtime_error("Unknown argument!");

  ArgumentVariant argument = positional_stack.top();
  positional_stack.pop();

  while (index < args.size()) {
    if (args[index].starts_with('-')) {
      --index;
      break;
    }

    switch (argument.arg_type) {
      case ArgType::String:
        argument.string_argument->AddValue(args[index]);
        break;
      case ArgType::Int:
        argument.int_argument->AddValue(StringToNumber(args[index]));
        break;
    }

    ++index;

  }
}
