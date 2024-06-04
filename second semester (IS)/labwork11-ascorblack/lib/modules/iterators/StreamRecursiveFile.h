#pragma once

#include <iostream>
#include <filesystem>
#include <stack>
#include <vector>
#include <utility>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

bool isValidUtf8(const std::vector<unsigned char>& data) {
  int i = 0;
  while (i < data.size()) {
    int bytesToFollow = 0;
    unsigned char lead = data[i];

    if (lead <= 0x7F) {
      i++;
      continue;
    } else if (lead >= 0xC2 && lead <= 0xDF) {
      bytesToFollow = 1;
    } else if (lead >= 0xE0 && lead <= 0xEF) {
      bytesToFollow = 2;
    } else if (lead >= 0xF0 && lead <= 0xF4) {
      bytesToFollow = 3;
    } else {
      return false;
    }

    if (i + bytesToFollow >= data.size()) return false;

    for (int j = 1; j <= bytesToFollow; j++) {
      if (data[i + j] < 0x80 || data[i + j] > 0xBF) {
        return false;
      }
    }
    i += bytesToFollow + 1;
  }
  return true;
}

bool isTextFile(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file) {
    std::cerr << "Cannot open file!" << std::endl;
    return false;
  }

  std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(file)),
                                      std::istreambuf_iterator<char>());

  file.close();

  return isValidUtf8(fileData);
}

namespace fs = std::filesystem;

class FileIterator {
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = fs::path;
  using difference_type = std::ptrdiff_t;
  using pointer = const fs::path*;
  using reference = const fs::path&;

 private:
  fs::recursive_directory_iterator end;
  fs::recursive_directory_iterator it;

 public:
  FileIterator() : it(), end() {}
  explicit FileIterator(const fs::path& path) : it(path), end() {}

  reference operator*() const { return it->path(); }
  pointer operator->() const { return &it->path(); }

  FileIterator& operator++() {
    do {
      ++it;
    } while (it != end && !(fs::is_regular_file(*it) && isTextFile((*it).path().string())));
    return *this;
  }

  FileIterator operator++(int) {
    FileIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const FileIterator& other) const {
    return it == other.it;
  }

  bool operator!=(const FileIterator& other) const {
    return it != other.it;
  }
};

class FileIterable {
 private:
  fs::path root;

 public:
  explicit FileIterable(const fs::path& root) : root(root) {}

  FileIterator begin() {
    return FileIterator(root);
  }

  static FileIterator end() {
    return {};
  }
};
