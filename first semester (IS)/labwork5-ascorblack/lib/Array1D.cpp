#include "Array1D.h"


Array1D::Array1D(ArrayData* array_data, uint16_t* begin_16, uint8_t *begin_8)
  : array_data_(array_data)
  , begin_16_(begin_16)
  , begin_8_(begin_8)
{}

uint17_t Array1D::operator[](int32_t index) {

  if (index < 0 || index >= array_data_->cols)
    throw std::out_of_range("Index out of range!");

  return uint17_t{(begin_16_ + index),
                  (begin_8_ + (index / kUint8Length)),
                  static_cast<uint64_t>(index)};
}
