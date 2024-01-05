#pragma once

#include <filesystem>
#include <fstream>

struct Header {
  uint16_t file_count{0};
};

class ArchiveHeader {
 private:
  std::string archive_name_;

  Header header_ = Header{};

  void Read16Bit(std::ifstream* archive, char* array);

 public:

  void ReadHeader(std::ifstream* archive);

  void WriteHeader(std::ofstream* archive);

  Header GetHeader() const;
  uint16_t GetFileCount() const;

  void SetArchiveName(std::string& archive_name);

  void SetFileCount(uint16_t file_count);

};
