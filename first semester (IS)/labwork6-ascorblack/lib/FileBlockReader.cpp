#include "FileBlockReader.h"



FileBlockReader::FileBlockReader(const std::filesystem::path& file_path, uint64_t file_size) : file_size_(file_size) {
  file_stream_.open(file_path, std::ios::binary);

  if (!file_stream_.is_open())
    throw std::runtime_error("An error occurred while opening the file!");

  bitset_block_ = std::vector<char>(kBlockByteSize, '\0');
}

uint64_t FileBlockReader::NextBlock() {
  bitset_block_ = std::vector<char>(kBlockByteSize, '\0');
  uint64_t block_size;

  if (kInfoBitCount / kByteSize > file_size_)
    block_size = file_size_;
  else
    block_size = kInfoByteBlockSize;

  char buffer[block_size];
  file_stream_.read(buffer, sizeof(buffer));

  file_size_ -= file_stream_.gcount();

  uint64_t ignored_index = 0;
  uint64_t index = 0;

  for (std::size_t i = 0; i < block_size; ++i) {
    for (std::size_t j = 0; j < kByteSize; ++j) {
      if (index == (1 << ignored_index) - 1 && ignored_index <= kControlBitCount) {
        --j;
        ++ignored_index;
      } else if (buffer[i] & (1 << GetBitPositionInByte(j)))
        bitset_block_[index / kByteSize] |= 1 << GetBitPositionInByte(index);

      index++;
    }
  }

  if (file_size_ == 0)
    eof = true;

  return block_size;
}

std::vector<char>& FileBlockReader::GetVectorBlock() {
  return bitset_block_;
}

bool FileBlockReader::GetEof() const {
  return eof;
}

int32_t FileBlockReader::GetBitPositionInByte(int32_t index)  {
  return kByteSize - 1 - (index % kByteSize);
}
