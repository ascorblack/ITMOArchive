#include "ArchiveFile.h"


ArchiveFile::ArchiveFile(const std::filesystem::path& filepath, uint64_t bit_file_size) {
  std::string filename = filepath.filename().string();
  if (filename.length() >= kFileNameSize)
    throw std::runtime_error("The file name is too long!");

  uint64_t control_bits;
  if (bit_file_size % kInfoBitCount == 0)
    control_bits = (bit_file_size / kInfoBitCount) * kControlBitCount;
  else
    control_bits = (bit_file_size / kInfoBitCount + 1) * kControlBitCount;

  header_ = new ArchiveFileHeader{};
  
  header_->bit_haf_file_size = control_bits + bit_file_size;
  header_->bit_source_file_size = bit_file_size;

  char* c_filename = new char[kFileNameSize];
  for (int i = 0; i < kFileNameSize; ++i) {
    if (i < filename.length())
      c_filename[i] = filename[i];
    else
      c_filename[i] = '\0';
  }
  header_->filename_ = c_filename;
}

void ArchiveFile::ReadFileName_() {
  char* filename = new char[kFileNameSize];

  archive_reader_->read(filename, kFileNameSize);

  header_->filename_ = filename;
}

void ArchiveFile::ReadBitFileSizes_() {
  char array[sizeof(header_->bit_haf_file_size) + sizeof(header_->bit_source_file_size)];

  archive_reader_->read(array, sizeof(array));

  if (archive_reader_->fail()) {
    archive_reader_->close();
    throw std::runtime_error("ArchiveFile read error.");
  }

  for (int i = 0; i < sizeof(header_->bit_source_file_size); ++i)
    header_->bit_source_file_size |= static_cast<uint64_t>((unsigned char) array[i]) << (8 * i);

  for (int i = sizeof(header_->bit_source_file_size); i < sizeof(header_->bit_haf_file_size) + sizeof(header_->bit_source_file_size); ++i)
    header_->bit_haf_file_size |= static_cast<uint64_t>((unsigned char) array[i]) << (8 * i);

}

void ArchiveFile::ReadFileInfo() {
  if (archive_reader_ == nullptr)
    throw std::runtime_error("The file reader hasn't been set!");

  delete header_;
  header_ = new ArchiveFileHeader{};

  ReadFileName_();
  ReadBitFileSizes_();
}

void ArchiveFile::GetNextBlock(std::vector<char>& bitset_block) {
  char block[kBlockByteSize];

  archive_reader_->read(block, sizeof(block));
  for (int i = 0; i < kBlockByteSize; ++i)
    bitset_block[i] = block[i];

}

void ArchiveFile::WriteFileHeader(std::ofstream* archive_stream) {
  archive_stream->write(header_->filename_, kFileNameSize);
  archive_stream->write(reinterpret_cast<const char*>(&header_->bit_source_file_size), sizeof(header_->bit_source_file_size));
  archive_stream->write(reinterpret_cast<const char*>(&header_->bit_haf_file_size), sizeof(header_->bit_haf_file_size));
}

void ArchiveFile::WriteFileBlock(std::ofstream* archive_stream, std::vector<char>& bitset_block) {
  archive_stream->write(reinterpret_cast<const char*>(bitset_block.data()), kBlockByteSize);
}

ArchiveFileHeader ArchiveFile::GetHeader() {
  return *header_;
}

uint64_t ArchiveFile::GetSourceFileByteSize() const {
  return header_->bit_source_file_size / kByteSize + (header_->bit_source_file_size % kByteSize == 0 ? 0 : 1);
}

uint64_t ArchiveFile::GetHafFileByteSize() const {
  return header_->bit_haf_file_size / kByteSize + (header_->bit_haf_file_size % kByteSize == 0 ? 0 : 1);
}

uint64_t ArchiveFile::GetFileBlocksCount() const {
  if ((header_->bit_source_file_size / kByteSize) % kInfoByteBlockSize == 0)
    return header_->bit_source_file_size / kByteSize / kInfoByteBlockSize;
  else
    return header_->bit_source_file_size / kByteSize / kInfoByteBlockSize + 1;
}

const char* ArchiveFile::GetFileName() const {
  return header_->filename_;
}

void ArchiveFile::SkipFile() {
  uint64_t count_block = GetFileBlocksCount();
  for (uint64_t block = 0; block < count_block; ++block)
    archive_reader_->seekg(archive_reader_->tellg() + static_cast<std::streampos>(kBlockByteSize));
}

ArchiveFile::~ArchiveFile() {
  delete header_;
}

ArchiveFileHeader::~ArchiveFileHeader() {
  delete[] filename_;
}
