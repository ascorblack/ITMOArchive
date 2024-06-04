#pragma once

#include <fstream>

class LineIterator : public std::iterator<std::input_iterator_tag, std::string> {
 private:
  std::ifstream* file;
  std::string current_line;
  bool is_end;

 public:
  explicit LineIterator() : file(nullptr), is_end(true) {}

  explicit LineIterator(std::ifstream& fileStream) : file(&fileStream),
                                                     is_end(!std::getline(fileStream, current_line)) {}

  bool operator==(const LineIterator& other) const {
    return is_end == other.is_end;
  }

  bool operator!=(const LineIterator& other) const {
    return !(*this == other);
  }

  LineIterator& operator++() {
    if (file && !std::getline(*file, current_line))
      is_end = true;

    return *this;
  }

  const std::string& operator*() const {
    return current_line;
  }
};

class StreamFileLines {
 private:
  std::ifstream file;

 public:
  explicit StreamFileLines(const std::string& filename) : file(filename) {}

  LineIterator begin() {
    return LineIterator(file);
  }

  LineIterator end() {
    return LineIterator();
  }
};