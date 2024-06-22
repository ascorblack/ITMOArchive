#include "ArgParser.h"

using namespace ArgumentParser;


void ArgParser::GetArgument(std::string_view argument_name, ArgumentVariant& argument) const {
  auto search_arg = arguments.find(std::string{argument_name});

  if (search_arg == arguments.end())
    return;

  if (auto stringArgument = std::get_if<StringArgument*>(&search_arg->second)) {
    argument.string_argument = *stringArgument;
    argument.arg_type = ArgType::String;
  }
  else if (auto intArgument = std::get_if<IntArgument*>(&search_arg->second)) {
    argument.int_argument = *intArgument;
    argument.arg_type = ArgType::Int;
  }
  else if (auto flagArgument = std::get_if<FlagArgument*>(&search_arg->second)) {
    argument.flag_argument = *flagArgument;
    argument.arg_type = ArgType::Flag;
  }

}

bool ArgParser::ArgsIsCorrect() const {
  for (auto& arg : arguments) {
    ArgumentVariant arguments_variant;
    GetArgument(arg.first, arguments_variant);

    switch (arguments_variant.arg_type) {
      case ArgType::String:
        if (!arguments_variant.string_argument->DataIsCorrect())
          return false;
        break;
      case ArgType::Int:
        if (!arguments_variant.int_argument->DataIsCorrect())
          return false;
        break;
      case ArgType::Flag:
        if (!arguments_variant.flag_argument->DataIsCorrect())
          return false;
        break;
      case ArgType::Undefined:break;
    }

  }

  return true;
}

void ArgParser::FillPositionalStack() {

  for (auto& arg : arguments) {
    ArgumentVariant arguments_variant;
    GetArgument(arg.first, arguments_variant);

    switch (arguments_variant.arg_type) {
      case ArgType::String:
        if (arguments_variant.string_argument->IsPositional())
          positional_stack.push(arguments_variant);
        break;
      case ArgType::Int:
        if (arguments_variant.int_argument->IsPositional())
          positional_stack.push(arguments_variant);
        break;
      case ArgType::Flag:
        if (arguments_variant.flag_argument->IsPositional())
          positional_stack.push(arguments_variant);
        break;
      case ArgType::Undefined:
        break;
    }
  }
}
