#pragma once

#include <iostream>
#include <filesystem>
#include <stack>
#include <vector>
#include <utility>
#include <regex>
#include <fstream>

#include <modules/utils/defines.hpp>

namespace fs = std::filesystem;



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


std::vector<fs::path> get_files_recursively(const fs::path& root) {
  std::vector<fs::path> files;

  try {
    if (fs::exists(root) && fs::is_directory(root))
      for (const auto& entry : fs::recursive_directory_iterator(root))
        if (fs::is_regular_file(entry) && isTextFile(entry.path()))
          files.push_back(entry.path());

  } catch (const fs::filesystem_error& e) {
    std::cerr << "Filesystem error: " << e.what() << '\n';
  }

  return files;
}

