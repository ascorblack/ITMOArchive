#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include <cstring>
#include <stack>

namespace ArgumentParser {


enum class ArgStorageType {
  Single,
  Multi
};

enum class ArgType {
  String,
  Int,
  Flag,
  Undefined
};

template<typename T>
struct MultiDataType {
  std::vector<T>* value{nullptr};
  std::optional<size_t> min_count{};
};

template<typename T>
struct SingleDataType {
  T* value{nullptr};
};

template<typename T>
class Argument {
 public:
  std::string names[2]{"\0", "\0"};
  const char* description{};

  virtual Argument& Default(const T&);
  virtual Argument& MultiValue(const size_t& min_count = 0);
  virtual Argument& Positional();

  virtual Argument& StoreValue(T&);
  virtual Argument& StoreValues(std::vector<T>&);

  virtual std::optional<T> GetValue() const;
  virtual std::optional<T> GetValue(int) const;
  virtual std::optional<T> GetDefault() const;
  [[nodiscard]] virtual std::optional<size_t> GetMinCount() const;
  virtual void AddValue(const T&);

  virtual ArgStorageType GetArgumentStoreType() const;
  virtual bool DataIsCorrect() const;
  virtual bool IsPositional() const;

  virtual void SetDescription(const char*);

  virtual ~Argument() = default;

 protected:
  std::variant<SingleDataType<T>, MultiDataType<T> > value_ = SingleDataType<T>{};
  std::optional<T> default_{};
  bool is_positional{false};

};

using IntArgument = Argument<int>;
using StringArgument = Argument<std::string>;
using FlagArgument = Argument<bool>;

using ArgumentVariants = std::variant<FlagArgument*, IntArgument*, StringArgument*>;

struct ArgumentVariant {
  ArgType arg_type{ArgType::Undefined};
  StringArgument* string_argument{nullptr};
  IntArgument* int_argument{nullptr};
  FlagArgument* flag_argument{nullptr};
};

struct ParserHelp {
  std::string* short_key{nullptr};
  std::string* long_key{nullptr};
  const char* description{nullptr};

  explicit ParserHelp() = default;
  explicit ParserHelp(char short_name, const char* long_name, const char* description);

  const char* HelpDescription(const std::unordered_map<std::string, ArgumentVariants>&, const char*&);

};

class ArgParser {
 public:
  explicit ArgParser(const char* header);

  bool Parse(const std::vector<std::string>&);
  bool Parse(int argc, char** argv);

  StringArgument& AddStringArgument(const char*);
  StringArgument& AddStringArgument(const char*, const char*);
  StringArgument& AddStringArgument(char, const char*);
  StringArgument& AddStringArgument(char, const char*, const char*);
  std::string GetStringValue(const std::string&) const;
  std::string GetStringValue(const std::string&, int) const;

  IntArgument& AddIntArgument(const char*);
  IntArgument& AddIntArgument(const char*, const char*);
  IntArgument& AddIntArgument(char, const char*);
  IntArgument& AddIntArgument(char, const char*, const char*);
  int GetIntValue(const std::string&) const;
  int GetIntValue(const std::string&, int) const;

  FlagArgument& AddFlag(const char*);
  FlagArgument& AddFlag(const char*, const char*);
  FlagArgument& AddFlag(char, const char*);
  FlagArgument& AddFlag(char, const char*, const char*);
  bool GetFlag(const std::string&) const;
  bool GetFlag(const std::string&, int) const;

  void AddHelp(char, const char*, const char*);
  bool Help() const;
  const char* HelpDescription();

 private:
  std::unordered_map<std::string, ArgumentVariants> arguments{};
  std::stack<ArgumentVariant> positional_stack;
  ParserHelp* help_{nullptr};
  const char* header{};

  bool CompareWithHelpKeys(const std::string&) const;

  template<typename T>
  T &AddArgument(char short_name, const char *long_name, const char *description);
  template<typename T>
  T &AddArgument(const char *long_name, const char *description);

  template <typename LT, typename RT>
  RT GetValue(const std::string &string_key, int index) const;

  void GetArgument(std::string_view, ArgumentVariant&) const;
  bool ArgsIsCorrect() const;

  void FillPositionalStack();

  void ParseLongArgument(std::string_view, ArgumentVariant&, int&, const std::vector<std::string>&);
  void ParseLongArgumentWithValue(std::string_view, ArgumentVariant&, int);
  void ParseLongArgumentWithoutValue(ArgumentVariant&, int&, const std::vector<std::string>&);

  void ParseShortArgument(std::string_view, ArgumentVariant&, int&, const std::vector<std::string>&);
  void ParseShortArgumentWithValue(std::string_view, ArgumentVariant&);
  void ParseShortArgumentWithoutValue(std::string_view, ArgumentVariant&, int&, const std::vector<std::string>&);

  void ParseMultiValueArgument(ArgumentVariant&, int&, const std::vector<std::string>&);

  void ParsePositionalArgument(const std::vector<std::string>&, int&);

};


}
