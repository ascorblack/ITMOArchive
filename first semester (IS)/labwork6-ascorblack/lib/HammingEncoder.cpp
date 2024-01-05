#include "HammingEncoder.h"


void HammingEncoder::Encoding(std::vector<char>& bitset_block) {
  for (int i = 0; i <= kControlBitCount; ++i) {
    int control_bit_position = 1 << i;

    int sum = 0;
    for (int j = control_bit_position - 1; j < kBlockBitSize; j += 2 * control_bit_position) {
      for (int k = 0; k < control_bit_position && j + k < kBlockBitSize; ++k) {
        sum += bitset_block[(j + k) / kByteSize] & (1 << GetBitPositionInByte(j + k)) ? 1 : 0;
      }
    }

    if (sum % 2)
      bitset_block[(control_bit_position - 1) / kByteSize] |= (1 << GetBitPositionInByte(control_bit_position - 1));

  }
}

int32_t HammingEncoder::GetBitPositionInByte(int32_t index) {
  return kByteSize - 1 - (index % kByteSize);
}
