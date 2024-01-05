#include "ArrayData.h"


ArrayData::~ArrayData() {
  delete[] left_bits;
  delete[] last_bits;
}

ArrayData::ArrayData(const ArrayData& rvalue) {

  delete[] left_bits;
  delete[] last_bits;

  depth = rvalue.depth;
  rows = rvalue.rows;
  cols = rvalue.cols;
  size = rvalue.size;
  size_last_bits = rvalue.size_last_bits;

  left_bits = new uint16_t[rvalue.size];
  last_bits = new uint8_t[size_last_bits];

  for (int i = 0; i < rvalue.size; ++i)
    left_bits[i] = rvalue.left_bits[i];

  for (int i = 0; i < size_last_bits; ++i)
    last_bits[i] = rvalue.last_bits[i];

  begin_16 = left_bits;
  begin_8 = last_bits;

}

ArrayData& ArrayData::operator=(const ArrayData& rvalue) {

  if (this == &rvalue)
    return *this;

  *this = ArrayData(rvalue);

  return *this;
}

ArrayData::ArrayData(int32_t depth,
                     int32_t rows,
                     int32_t cols,
                     uint64_t size,
                     uint64_t size_last_bits,
                     uint16_t *left_bits,
                     uint8_t *last_bits)
   : depth(depth)
   , rows(rows)
   , cols(cols)
   , size(size)
   , size_last_bits(size_last_bits)
   , left_bits(left_bits)
   , last_bits(last_bits)
{
  begin_16 = left_bits;
  begin_8 = last_bits;
}
