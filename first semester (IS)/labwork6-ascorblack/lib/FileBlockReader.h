#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <filesystem>
#include <unordered_map>

const uint16_t kFileNameSize = 256;
const std::size_t kInfoBitCount = 1032;
const std::size_t kControlBitCount = 10;
const std::size_t kByteSize = 8;
const std::size_t
    kInfoByteBlockSize = kInfoBitCount % kByteSize == 0 ? kInfoBitCount / kByteSize : kInfoBitCount / kByteSize + 1;
const std::size_t kBlockByteSize =
    (kInfoBitCount + kControlBitCount) % kByteSize == 0 ? (kInfoBitCount + kControlBitCount) / kByteSize :
    (kInfoBitCount + kControlBitCount) / kByteSize + 1;
const std::size_t kBlockBitSize = kInfoBitCount + kControlBitCount;

class FileBlockReader {
 private:
  std::ifstream file_stream_;
  uint64_t file_size_;
  std::vector<char> bitset_block_;
  bool eof = false;

 public:

  explicit FileBlockReader(const std::filesystem::path&, uint64_t);

  bool GetEof() const;

  uint64_t NextBlock();

  std::vector<char>& GetVectorBlock();

  int32_t GetBitPositionInByte(int32_t index);

};

