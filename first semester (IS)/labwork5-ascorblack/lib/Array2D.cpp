#include "Array2D.h"


Array2D::Array2D(ArrayData* array_data, uint16_t* begin_16, uint8_t* begin_8) :
    array_data_(array_data), begin_16_(begin_16), begin_8_(begin_8) {}

Array1D Array2D::operator[](int32_t row) {

  if (row < 0 || row >= array_data_->rows)
    throw std::out_of_range("Index out of range!");

  uint64_t left_bits_shift = row * array_data_->cols;
  uint64_t last_bits_shift = (row * array_data_->cols) / kUint8Length;

  return Array1D(array_data_,
                 begin_16_ + left_bits_shift,
                 begin_8_ + last_bits_shift);
}
