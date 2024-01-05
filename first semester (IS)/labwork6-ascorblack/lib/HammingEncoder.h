#pragma once

#include "FileBlockReader.h"

class HammingEncoder {
 protected:

  void Encoding(std::vector<char>&);

  int32_t GetBitPositionInByte(int32_t index);

};
