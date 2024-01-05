#include <iostream>
#include "ArchiveHeader.h"

void ArchiveHeader::Read16Bit(std::ifstream* archive, char* array) {
  archive->read(array, 2);

  if (archive->fail()) {
    archive->close();
    throw std::runtime_error("ArchiveFile read error.");
  }
}

void ArchiveHeader::ReadHeader(std::ifstream* archive) {
  if (!archive->is_open())
    throw std::runtime_error("Failed to open file.");

  Header header;
  char number[2];

  Read16Bit(archive, number);

  header.file_count = static_cast<uint16_t>((unsigned char) number[0]);
  header.file_count |= static_cast<uint16_t>((unsigned char) number[1]) << 8;

  header_ = header;
}

void ArchiveHeader::WriteHeader(std::ofstream* archive) {
  if (!archive->is_open())
    throw std::runtime_error("Failed to open file.");

  archive->write(reinterpret_cast<const char*>(&header_.file_count), sizeof(uint16_t));
}

Header ArchiveHeader::GetHeader() const {
  return header_;
}

void ArchiveHeader::SetArchiveName(std::string& archive_name) {
  archive_name_ = archive_name;
}

void ArchiveHeader::SetFileCount(uint16_t file_count) {
  header_.file_count = file_count;
}

uint16_t ArchiveHeader::GetFileCount() const {
  return header_.file_count;
}
