#pragma once

#include "FileBlockReader.h"

class HammingDecoder {
 protected:

  std::vector<char> Decode(std::vector<char>&, int32_t&);

  int32_t GetBitPositionInByte(int32_t index);

  int32_t CheckControlBits(std::vector<char>&, int32_t&);
};
