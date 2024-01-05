#pragma once

#include <cstdint>
#include <cmath>


const uint8_t kUint8Length = 8;

struct ArrayData {
  int32_t depth;
  int32_t rows;
  int32_t cols;
  uint64_t size;
  uint64_t size_last_bits;

  uint16_t* left_bits;
  uint8_t* last_bits;

  uint16_t* begin_16;
  uint8_t* begin_8;

  ArrayData(const ArrayData& rvalue);
  ArrayData(int32_t depth,
            int32_t rows,
            int32_t cols,
            uint64_t size,
            uint64_t size_last_bits,
            uint16_t* left_bits,
            uint8_t* last_bits);

  ArrayData& operator=(const ArrayData& rvalue);

  ~ArrayData();
};
