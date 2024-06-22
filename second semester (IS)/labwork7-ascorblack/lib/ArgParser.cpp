#include "src/Argument.cpp"
#include "src/ArgsAdders.cpp"
#include "src/Parse.cpp"
#include "src/ArgsGetters.cpp"
#include "src/Help.cpp"
#include "src/SubMethods.cpp"

using namespace ArgumentParser;


// Constructor
ArgParser::ArgParser(const char* name): header(name) {}


// Work with Helper in help_
void ArgParser::AddHelp(char short_key, const char* long_key, const char* description) {
  help_ = new ParserHelp(short_key, long_key, description);
}

bool ArgParser::Help() const {
  return help_ != nullptr;
}

bool ArgParser::CompareWithHelpKeys(const std::string& string) const {
  return help_ != nullptr && (string == *help_->long_key || string == *help_->short_key);
}

const char* ArgParser::HelpDescription() {
  return help_ == nullptr ? "" : help_->HelpDescription(arguments, header);
}
