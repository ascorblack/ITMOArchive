#pragma once


#include <sstream>
#include <utility>
#include <ranges>
#include <modules/utils/defines.hpp>


class LetterIterator : public std::iterator<std::input_iterator_tag, std::string> {
 private:
  const std::string current_word;
  uint16_t current_pos{0};
  std::string current_letter;
  bool is_end = false;

 public:
  explicit LetterIterator() : is_end(true) {};

  explicit LetterIterator(std::string word) : current_word(std::move(word)) {
    ++(*this);
  }


  bool operator==(const LetterIterator& other) const {
    return is_end == other.is_end;
  }

  bool operator!=(const LetterIterator& other) const {
    return !(*this == other);
  }

  static int32_t getLetterLength(const unsigned char& letter) {
    if ((letter & 0x80) == 0) return 1;
    else if ((letter & 0xE0) == 0xC0) return 2;
    else if ((letter & 0xF0) == 0xE0) return 3;
    else if ((letter & 0xF8) == 0xF0) return 4;
    else throw std::runtime_error("Некорректный символ!");
  }

  LetterIterator& operator++() {
    if (current_pos == current_word.length()) {
      is_end = true;
      return *this;
    }

    auto first_byte = static_cast<unsigned char>(current_word[current_pos++]);
    int32_t letter_length = getLetterLength(first_byte);
    current_letter = static_cast<char>(first_byte);

    while (--letter_length > 0)
      current_letter.push_back(current_word[current_pos++]);

    return *this;
  }

  const std::string& operator*() const {
    return current_letter;
  }
};

class StreamLetter {
 private:
  std::string word;

 public:
  explicit StreamLetter(std::string str) : word(str) {}

  LetterIterator begin() const {
    return LetterIterator(word);
  }

  LetterIterator end() const {
    return LetterIterator();
  }
};
