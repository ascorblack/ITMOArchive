#include "Argument.cpp"
#include "ArgsAdders.cpp"
#include "Parse.cpp"
#include "ArgsGetters.cpp"
#include "Help.cpp"
#include "SubMethods.cpp"

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

std::string ArgParser::HelpDescription() {
  return help_ == nullptr ? "" : help_->HelpDescription(arguments, header);
}
