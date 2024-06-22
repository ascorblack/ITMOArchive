#include "ArgParser.h"

using namespace ArgumentParser;


void GetArgument(std::string_view argument_name, ArgumentVariant& argument, std::unordered_map<std::string, ArgumentVariants> arguments) {
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


ParserHelp::ParserHelp(char short_name, const char* long_name, const char* description): description(description) {
  auto* alloc_short_name = new std::string{"-" + std::string{&short_name, 1}};
  auto* alloc_long_name = new std::string{"--" + std::string{long_name}};

  short_key = alloc_short_name;
  long_key = alloc_long_name;
}


const char* ParserHelp::HelpDescription(const std::unordered_map<std::string, ArgumentVariants>& arguments, const char*& header) {
  std::string helpDescription;

  helpDescription += header + std::string{"\n"};
  helpDescription += this->description + std::string{"\n\n"};

  for (auto& arg : arguments) {
    ArgumentVariant arguments_variant;
    GetArgument(arg.first, arguments_variant, arguments);

    if (arg.first.length() == 1)
      continue;

    switch (arguments_variant.arg_type) {
      case ArgType::String:
        if (arguments_variant.string_argument->names[0] != "\0")
          helpDescription += "-" + arguments_variant.string_argument->names[0] + ",\t";
        else
          helpDescription += "   \t";

        helpDescription += "--" + arguments_variant.string_argument->names[1] + "=<string>,\t";
        helpDescription += arguments_variant.string_argument->description;

        if (arguments_variant.string_argument->GetDefault().has_value())
          helpDescription += " [default = " + arguments_variant.string_argument->GetDefault().value() + "] ";
        if (arguments_variant.string_argument->GetArgumentStoreType() == ArgStorageType::Multi)
          helpDescription += " [repeated, min args = " + std::to_string(arguments_variant.string_argument->GetMinCount().value()) + "] ";

        helpDescription += '\n';

        break;
      case ArgType::Int:
        if (arguments_variant.int_argument->names[0] != "\0")
          helpDescription += "-" + arguments_variant.int_argument->names[0] + ",\t";
        else
          helpDescription += "   \t";

        helpDescription += "--" + arguments_variant.int_argument->names[1] + "=<int>,\t";
        helpDescription += arguments_variant.int_argument->description;

        if (arguments_variant.int_argument->GetDefault().has_value())
          helpDescription += " [default = " + std::to_string(arguments_variant.int_argument->GetDefault().value()) + "] ";
        if (arguments_variant.int_argument->GetArgumentStoreType() == ArgStorageType::Multi)
          helpDescription += " [repeated, min args = " + std::to_string(arguments_variant.int_argument->GetMinCount().value()) + "] ";

        helpDescription += '\n';

        break;
      case ArgType::Flag:
        if (arguments_variant.flag_argument->names[0] != "\0")
          helpDescription += "-" + arguments_variant.flag_argument->names[0] + ",\t";
        else
          helpDescription += "   \t";

        helpDescription += "--" + arguments_variant.flag_argument->names[1] + ",\t";
        helpDescription += arguments_variant.flag_argument->description;

        if (arguments_variant.flag_argument->GetDefault().has_value() && arguments_variant.flag_argument->GetDefault().value())
          helpDescription += " [default = " + std::to_string(arguments_variant.flag_argument->GetDefault().value()) + "] ";

        helpDescription += '\n';

        break;
      case ArgType::Undefined:break;
    }
  }

  helpDescription += '\n' + *this->short_key + ",  " + *this->long_key + " " + this->description + '\n';

  const char* result = helpDescription.c_str();

  return result;
}
