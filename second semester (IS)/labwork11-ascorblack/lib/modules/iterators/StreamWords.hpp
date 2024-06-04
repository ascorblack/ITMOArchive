#pragma once

#include <sstream>
#include <utility>
#include <ranges>
#include <modules/utils/defines.hpp>
#include <algorithm>

class WordIterator : public std::iterator<std::input_iterator_tag, std::string> {
private:
  std::istringstream iss;
  std::string current_word;
  bool is_end = false;

public:
  explicit WordIterator() : is_end(true) {};

  explicit WordIterator(const std::string& text) : iss(text) {
    ++(*this);
  }


  bool operator==(const WordIterator& other) const {
    return is_end == other.is_end;
  }

  bool operator!=(const WordIterator& other) const {
    return !(*this == other);
  }

  WordIterator& operator++() {
    if (!(iss >> current_word))
      is_end = true;

    return *this;
  }

  std::string operator*() const {
    return current_word;
  }
};

class StreamWords {
private:
  std::string text;

public:
  explicit StreamWords(std::string str) : text(std::move(str)) {}

  WordIterator begin() const {
    return WordIterator(text);
  }

  WordIterator end() const {
    return WordIterator();
  }
};
