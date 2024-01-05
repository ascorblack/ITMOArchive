#pragma once

#include "HammingDecoder.h"
#include "HammingEncoder.h"
#include "ArchiveFile.h"
#include "ArchiveHeader.h"
#include "ArgParser.cpp"

const char* kLongFlagHelp = "help";
const char* kLongFlagCreate = "create";
const char* kLongFlagExtract = "extract";
const char* kLongFlagList = "list";
const char* kLongFlagFilename = "file";
const char* kLongFlagDelete = "delete";
const char* kLongFlagAppend = "append";
const char* kLongFlagConcatenate = "concatenate";
const char kShortFlagHelp = 'h';
const char kShortFlagCreate = 'c';
const char kShortFlagExtract = 'x';
const char kShortFlagList = 'l';
const char kShortFlagDelete = 'd';
const char kShortFlagAppend = 'a';
const char kShortFlagConcatenate = 'A';
const char kShortFlagFilename = 'f';

class HammingArchiver : public HammingEncoder, public HammingDecoder {
 private:
  std::ofstream* archive_writer_{};
  std::ifstream* archive_reader_{};
  std::string archive_path_{};
  ArgParser* arg_parser_{nullptr};

  std::vector<std::string> append_files;
  std::vector<std::string> delete_files;
  std::vector<std::string> extract_files;
  std::vector<std::string> concatenate_archives;

  static uint64_t GetFileSize(const std::filesystem::path&);

  void OpenWriter();
  void OpenReader();

  void CloseWriter();
  void CloseReader();

  void CreateArchive();
  void ExtractArchive();
  void ListOfArchive();

  ArgParser* GetArgParser();
  void WriteFileToArchive(const std::string&);

  void AppendToArchive();
  void ConcatenateArchives();
  std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> GetFilesFromArchive(std::ifstream* archive_reader,
                                                                                     bool);
  void DeleteFromArchive();
  void MoveFilesBtwArchives(std::ifstream& archive_reader, ArchiveHeader& archive_header);

 public:

  explicit HammingArchiver(int argc, char** argv);
  ~HammingArchiver();

  void Execute();

  void WriteFilesToArchive(const std::vector<std::string>& files);
};
