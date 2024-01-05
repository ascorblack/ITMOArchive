#include "HammingDecoder.h"

std::vector<char> HammingDecoder::Decode(std::vector<char>& bitset_block, int32_t& error_count) {

  int32_t error_index = CheckControlBits(bitset_block, error_count);

  if (error_index != -1) {
    if (bitset_block[error_index / kByteSize] & (1 << (error_index % kByteSize)))
      bitset_block[error_index / kByteSize] ^= (1 << (error_index % kByteSize));
    else
      bitset_block[error_index / kByteSize] |= (1 << (error_index % kByteSize));
  }

  std::vector<char> result_block(kInfoByteBlockSize, '\0');

  uint64_t ignored_index = 0;
  uint64_t index = 0;
  uint64_t total_pointer = 0;

  for (std::size_t i = 0; i < kBlockByteSize; ++i) {
    for (std::size_t j = 0; j < kByteSize; ++j) {
      if (total_pointer == (1 << ignored_index) - 1 && ignored_index <= kControlBitCount) {
        --index;
        ++ignored_index;
      } else if (bitset_block[i] & (1 << GetBitPositionInByte(j)))
        result_block[index / kByteSize] |= 1 << GetBitPositionInByte(index);

      index++;
      total_pointer++;
    }
  }

  return result_block;
}

int32_t HammingDecoder::CheckControlBits(std::vector<char>& bitset_block, int32_t& error_count) {
  int32_t error_index = -1;

  for (int32_t i = 0; i <= kControlBitCount; ++i) {
    int32_t control_bit_position = 1 << i;

    int32_t sum = 0;
    for (int32_t j = control_bit_position - 1; j < kBlockBitSize; j += 2 * control_bit_position) {
      for (int32_t k = 0; k < control_bit_position && j + k < kBlockBitSize; ++k) {
        if (j + k != control_bit_position - 1)
          sum += bitset_block[(j + k) / kByteSize] & (1 << GetBitPositionInByte(j + k)) ? 1 : 0;
      }
    }

    if (sum % 2 != static_cast<bool>(bitset_block[(control_bit_position - 1) / kByteSize]
        & (1 << GetBitPositionInByte(control_bit_position - 1)))) {
      if (error_index == -1)
        error_index = control_bit_position - 1;
      else
        error_index += control_bit_position - 1;

      error_count++;
    }
  }

  return error_index;
}

int32_t HammingDecoder::GetBitPositionInByte(int32_t index) {
  return kByteSize - 1 - (index % kByteSize);
}
