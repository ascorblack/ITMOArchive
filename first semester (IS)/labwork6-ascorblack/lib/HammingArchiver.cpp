#include "HammingArchiver.h"

ArgParser* HammingArchiver::GetArgParser() {
  ArgParser* arg_parser = new ArgParser("Hamming Archiver");

  arg_parser->AddHelp(kShortFlagHelp, kLongFlagHelp,
                      "Display help");

  arg_parser->AddFlag(kShortFlagCreate, kLongFlagCreate,
                      "Create new archive")
      .Default(false);

  arg_parser->AddStringArgument(kShortFlagFilename, kLongFlagFilename,
                                "Archive name")
      .Default("output.haf");

  arg_parser->AddFlag(kShortFlagList, kLongFlagList,
                      "Display list of files in archive")
      .Default(false);

  arg_parser->AddStringArgument(kShortFlagExtract, kLongFlagExtract,
                                "Extract files from archive")
      .MultiValue().StoreValues(extract_files).Default(".");

  arg_parser->AddStringArgument(kShortFlagAppend, kLongFlagAppend,
                                "Add file(s) to archive")
      .MultiValue().StoreValues(append_files).Default("").Positional();

  arg_parser->AddStringArgument(kShortFlagDelete, kLongFlagDelete,
                                "Delete file(s) from archive")
      .MultiValue().StoreValues(delete_files).Default("");

  arg_parser->AddStringArgument(kShortFlagConcatenate, kLongFlagConcatenate,
                                "Concatenate two archive")
      .MultiValue().StoreValues(concatenate_archives).Default("");

  return arg_parser;
}

HammingArchiver::HammingArchiver(int argc, char** argv) {
  arg_parser_ = GetArgParser();

  if (!arg_parser_->Parse(argc, argv)) {
    std::cout << arg_parser_->HelpDescription();
    delete arg_parser_;
    std::cerr << "Wrong argument\n";
    exit(0);
  }

  if (arg_parser_->FinishWithHelp())
    exit(0);
}

uint64_t HammingArchiver::GetFileSize(const std::filesystem::path& file_path) {
  std::ifstream file(file_path, std::ios::binary | std::ios::ate);
  uint64_t file_size = file.tellg();
  file.close();

  return file_size;
}

void HammingArchiver::WriteFilesToArchive(const std::vector<std::string>& files) {
  for (const std::string& file_path : files)
    WriteFileToArchive(file_path);
}


void HammingArchiver::WriteFileToArchive(const std::string& file_path) {
  uint64_t file_size = GetFileSize(file_path);

  ArchiveFile archive_file(file_path, file_size * kByteSize);
  archive_file.WriteFileHeader(archive_writer_);

  FileBlockReader file_block_reader(file_path, file_size);

  printf("File Processing: %s\n", archive_file.GetFileName());

  int32_t count_block = archive_file.GetFileBlocksCount();
  for (uint64_t block = 0; block < count_block; ++block) {
    file_block_reader.NextBlock();
    std::vector<char> bitset_block = file_block_reader.GetVectorBlock();

    Encoding(bitset_block);

    archive_file.WriteFileBlock(archive_writer_, bitset_block);
    printf("\rProgress: %.2f%%", (static_cast<double>(block + 1) / static_cast<double>(count_block))*static_cast<double>(100));
  }

  printf("\nFinish!\n");
}

void HammingArchiver::MoveFilesBtwArchives(std::ifstream& archive_reader, ArchiveHeader& archive_header) {
  ArchiveFile archive_file(&archive_reader);
  for (int i = 0; i < archive_header.GetFileCount(); ++i) {
    archive_file.ReadFileInfo();

    archive_file.WriteFileHeader(archive_writer_);

    for (int block = 0; block < archive_file.GetFileBlocksCount(); ++block) {
      std::vector<char> bitset_block(kBlockByteSize, '\0');
      archive_file.GetNextBlock(bitset_block);

      archive_writer_->write(bitset_block.data(), bitset_block.size());
    }
  }
  archive_reader.close();
}

void HammingArchiver::OpenWriter() {
  if (archive_writer_ != nullptr) {
    archive_writer_->close();
    delete archive_writer_;
    archive_writer_ = nullptr;
  }

  archive_writer_ = new std::ofstream(archive_path_, std::ios::binary);

  if (!archive_writer_->is_open()) {
    std::cerr << "Error opening file for writing\n";
    exit(0);
  }
}

void HammingArchiver::OpenReader() {
  if (archive_reader_ != nullptr) {
    archive_reader_->close();
    delete archive_reader_;
    archive_reader_ = nullptr;
  }

  archive_reader_ = new std::ifstream(archive_path_, std::ios::binary);

  if (!archive_reader_->is_open()) {
    std::cerr << "Error opening file for reading\n";
    exit(0);
  }
}

void HammingArchiver::Execute() {

  archive_path_ = arg_parser_->GetStringValue(kLongFlagFilename);

  if (arg_parser_->GetFlag(kLongFlagCreate))
    CreateArchive();
  else if (arg_parser_->IsActiveArgument(kLongFlagExtract))
    ExtractArchive();
  else if (arg_parser_->GetFlag(kLongFlagList))
    ListOfArchive();
  else if (arg_parser_->IsActiveArgument(kLongFlagAppend))
    AppendToArchive();
  else if (arg_parser_->IsActiveArgument(kLongFlagConcatenate))
    ConcatenateArchives();
  else if (arg_parser_->IsActiveArgument(kLongFlagDelete))
    DeleteFromArchive();

  CloseWriter();
  CloseReader();
}

void HammingArchiver::CloseWriter() {
  if (archive_writer_ != nullptr)
    archive_writer_->close();
}

void HammingArchiver::CloseReader() {
  if (archive_reader_ != nullptr)
    archive_reader_->close();
}

void HammingArchiver::CreateArchive() {
  OpenWriter();

  ArchiveHeader archive_header;
  archive_header.SetArchiveName(archive_path_);
  archive_header.SetFileCount(append_files.size());

  archive_header.WriteHeader(archive_writer_);

  WriteFilesToArchive(append_files);
}

void HammingArchiver::ConcatenateArchives() {
  if (concatenate_archives.size() != 2) {
    std::cerr << "Incorrect count of concatenate archives!\n";
    exit(0);
  }

  OpenWriter();

  std::ifstream archive_reader_first(concatenate_archives[0], std::ios::binary);
  std::ifstream archive_reader_second(concatenate_archives[1], std::ios::binary);

  ArchiveHeader first_archive_header;
  ArchiveHeader second_archive_header;
  first_archive_header.ReadHeader(&archive_reader_first);
  second_archive_header.ReadHeader(&archive_reader_second);

  ArchiveHeader output_archive_header;
  output_archive_header.SetArchiveName(archive_path_);
  output_archive_header.SetFileCount(first_archive_header.GetFileCount() + second_archive_header.GetFileCount());

  output_archive_header.WriteHeader(archive_writer_);

  MoveFilesBtwArchives(archive_reader_first, first_archive_header);
  MoveFilesBtwArchives(archive_reader_second, second_archive_header);
}

void HammingArchiver::AppendToArchive() {
  OpenReader();
  std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> files_in_archive = GetFilesFromArchive(
      archive_reader_, false
  );
  CloseReader();

  for (std::filesystem::path file : append_files) {
    if (files_in_archive.contains(file.filename())) {
      std::cerr << "File `" + file.filename().string() + "` is already in archive!\n";
      exit(0);
    }
  }

  OpenReader();

  ArchiveHeader source_archive_header;
  source_archive_header.ReadHeader(archive_reader_);

  std::string temp_filepath = archive_path_ + ".temp";
  archive_writer_ = new std::ofstream(temp_filepath, std::ios::binary);

  ArchiveHeader archive_header;
  archive_header.SetArchiveName(temp_filepath);
  archive_header.SetFileCount(append_files.size() + source_archive_header.GetFileCount());

  archive_header.WriteHeader(archive_writer_);

  MoveFilesBtwArchives(*archive_reader_, source_archive_header);

  WriteFilesToArchive(append_files);

  std::rename(temp_filepath.c_str(), archive_path_.c_str());
}

int GetBlockRealSize(std::vector<char>& block) {
  int length = block.size();

  for (int i = length - 1; i >= 0; --i) {
    if (block[i] != '\0')
      break;

    --length;
  }

  return length;
}

void HammingArchiver::ExtractArchive() {
  OpenReader();

  ArchiveHeader archive_header;
  archive_header.ReadHeader(archive_reader_);

  ArchiveFile archive_file(archive_reader_);
  for (int file_num = 0; file_num < archive_header.GetFileCount(); ++file_num) {
    archive_file.ReadFileInfo();

    bool flag = false;
    if (!extract_files.empty()) {
      for (std::string& file_name : extract_files) {
        if (archive_file.GetFileName() == file_name) {
          flag = true;
          break;
        }
      }
      if (!flag) {
        archive_file.SkipFile();
        continue;
      }
    }

    printf("File Processing: %s\n", archive_file.GetFileName());

    std::ofstream output(archive_file.GetFileName(), std::ios::binary);
    int32_t error_count = 0;

    uint32_t count_block = archive_file.GetFileBlocksCount() - 1;
    for (uint32_t block = 0; block <= count_block; ++block) {
      std::vector<char> bitset_block(kBlockByteSize, '\0');
      archive_file.GetNextBlock(bitset_block);

      std::vector<char> decoded_block = Decode(bitset_block, error_count);

      if (block == count_block)
        output.write(decoded_block.data(), GetBlockRealSize(decoded_block));
      else
        output.write(decoded_block.data(), decoded_block.size());

      printf("\rProgress: %.2f%%", (static_cast<double>(block + 1) / static_cast<double>(count_block))*static_cast<double>(100));
    }

    printf("\nFinish! (Error count: %d)\n", error_count);

    output.close();
  }
}

std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> HammingArchiver::GetFilesFromArchive(
    std::ifstream* archive_reader,
    bool print_head
    )
{
  ArchiveHeader archive_header;
  archive_header.ReadHeader(archive_reader);

  if (print_head)
    printf("Archive name: %s\nCount files in archive: %d\n\n"
           "", archive_path_.c_str(), archive_header.GetFileCount());

  std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> files_map;

  ArchiveFile archive_file(archive_reader);
  for (int file_num = 0; file_num < archive_header.GetFileCount(); ++file_num) {
    archive_file.ReadFileInfo();

    files_map[archive_file.GetFileName()] = std::pair<uint64_t, uint64_t>(archive_file.GetSourceFileByteSize(),
                                                                          archive_file.GetHafFileByteSize());
    archive_file.SkipFile();
  }

  return files_map;
}

void HammingArchiver::ListOfArchive() {
  OpenReader();

  std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> files = GetFilesFromArchive(archive_reader_, true);

  for (auto const& file : files)
    printf("Filename: %s\nSource file size: %lu byte\nHaf file size: %lu byte\n\n",
           file.first.c_str(), file.second.first, file.second.second);

}

void HammingArchiver::DeleteFromArchive() {
  OpenReader();
  std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> files_in_archive = GetFilesFromArchive(
      archive_reader_, false
  );
  CloseReader();

  bool file_exists = false;
  for (std::filesystem::path file : delete_files) {
    if (files_in_archive.contains(file.filename())) {
      file_exists = true;
      break;
    }
  }
  if (!file_exists) {
    std::cerr << "One of the files is not in the archive!\n";
    exit(0);
  }

  OpenReader();

  ArchiveHeader source_archive_header;
  source_archive_header.ReadHeader(archive_reader_);

  std::string temp_filepath = archive_path_ + ".temp";
  archive_writer_ = new std::ofstream(temp_filepath, std::ios::binary);

  ArchiveHeader archive_header;
  archive_header.SetArchiveName(temp_filepath);
  archive_header.SetFileCount(source_archive_header.GetFileCount() - delete_files.size());

  archive_header.WriteHeader(archive_writer_);

  ArchiveFile archive_file(archive_reader_);
  for (int i = 0; i < source_archive_header.GetFileCount(); ++i) {
    archive_file.ReadFileInfo();

    bool flag = true;
    for (std::string& file_name : delete_files) {
      if (archive_file.GetFileName() == file_name) {
        flag = false;
        break;
      }
    }
    if (!flag) {
      archive_file.SkipFile();
      continue;
    }

    archive_file.WriteFileHeader(archive_writer_);

    for (int block = 0; block < archive_file.GetFileBlocksCount(); ++block) {
      std::vector<char> bitset_block(kBlockByteSize, '\0');
      archive_file.GetNextBlock(bitset_block);

      archive_writer_->write(bitset_block.data(), bitset_block.size());
    }
  }

  std::rename(temp_filepath.c_str(), archive_path_.c_str());

  printf("Successfully deleted %zu files from the archive.\nIt now contains %i files.\n",
         delete_files.size(), archive_header.GetFileCount());
}

HammingArchiver::~HammingArchiver() {
  CloseReader();
  CloseWriter();

  delete arg_parser_;
}
