#pragma once

#include "FileBlockReader.h"

struct ArchiveFileHeader {
  const char* filename_{};
  uint64_t bit_source_file_size{};
  uint64_t bit_haf_file_size{};

  ~ArchiveFileHeader();
};

class ArchiveFile {
 private:
  ArchiveFileHeader* header_{};
  std::ifstream* archive_reader_{nullptr};

  void ReadFileName_();
  void ReadBitFileSizes_();

 public:
  explicit ArchiveFile(const std::filesystem::path&, uint64_t);
  explicit ArchiveFile(std::ifstream* archive_reader) :
      archive_reader_(archive_reader) {}
  ~ArchiveFile();

  void WriteFileHeader(std::ofstream*);
  void WriteFileBlock(std::ofstream*, std::vector<char>&);

  void ReadFileInfo();

  void GetNextBlock(std::vector<char>&);

  ArchiveFileHeader GetHeader();
  uint64_t GetSourceFileByteSize() const;
  const char* GetFileName() const;
  uint64_t GetFileBlocksCount() const;
  uint64_t GetHafFileByteSize() const;
  void SkipFile();
};
